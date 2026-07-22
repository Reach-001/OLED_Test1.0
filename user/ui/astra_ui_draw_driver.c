/**
 * @file    astra_ui_draw_driver.c
 * @brief   ST7789 裸驱动 + 字体渲染 — 支持 1.14 寸 240×135 / 1.47 寸 320×172 横屏
 * @note    基于厂家 ST7789 示例驱动改写
 * @note    使用 MSPM0 硬件 SPI1, SCK=PB9, MOSI=PB8
 * @note    默认使用 1-bit 帧缓冲，oled_send_buffer() 时转换为 RGB565 写屏
 *
 * @par 移植说明:
 *   1. 修改引脚宏定义 (SCL/MOSI/RES/DC/CS/BLK)
 *   2. 修改 GPIO 操作宏 (适配 MSPM0 的 DL_GPIO_* 或 STM32 的 GPIO_*)
 *   3. 实现 delay_ms / get_ticks (基于 SysTick 或定时器)
 *   4. 确认 MADCTL 与偏移值匹配当前屏幕方向
 */

#include "board_config.h"
#include "astra_ui_draw_driver.h"
#include "app_ui_style_config.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*===========================================================================
 * GPIO 操作宏（需根据实际硬件平台修改）
 *=========================================================================*/

#ifndef LCD_RES_Clr
#define LCD_RES_Clr()   gpio_low(LCD_RES_PIN)   /**< 复位引脚拉低 */
#endif
#ifndef LCD_RES_Set
#define LCD_RES_Set()   gpio_high(LCD_RES_PIN)  /**< 复位引脚拉高 */
#endif
#ifndef LCD_DC_Clr
#define LCD_DC_Clr()    gpio_low(LCD_DC_PIN)    /**< 数据/命令选择引脚拉低（命令） */
#endif
#ifndef LCD_DC_Set
#define LCD_DC_Set()    gpio_high(LCD_DC_PIN)   /**< 数据/命令选择引脚拉高（数据） */
#endif
#ifndef LCD_CS_Clr
#define LCD_CS_Clr()    gpio_low(LCD_CS_PIN)    /**< 片选引脚拉低（选中） */
#endif
#ifndef LCD_CS_Set
#define LCD_CS_Set()    gpio_high(LCD_CS_PIN)   /**< 片选引脚拉高（释放） */
#endif
#ifndef LCD_BLK_Clr
#define LCD_BLK_Clr()   gpio_low(LCD_BLK_PIN)   /**< 背光引脚拉低（关背光） */
#endif
#ifndef LCD_BLK_Set
#define LCD_BLK_Set()   gpio_high(LCD_BLK_PIN)  /**< 背光引脚拉高（开背光） */
#endif

/*===========================================================================
 * 屏幕尺寸与偏移 (与厂家驱动严格一致)
 *===========================================================================*/

/** @brief 两块屏都使用厂家横屏模式2（MADCTL=0x70） */
#define USE_HORIZONTAL  2

#if LCD_PANEL_TYPE == LCD_PANEL_147_ST7789
/** @brief 1.47 寸 ST7789 横屏模式2偏移（厂家提供） */
#define TFT_X_OFFSET 0
#define TFT_Y_OFFSET 34
#elif LCD_PANEL_TYPE == LCD_PANEL_114_ST7789
/** @brief 1.14 寸 ST7789 横屏模式2偏移（厂家提供） */
#define TFT_X_OFFSET 40
#define TFT_Y_OFFSET 53
#else
#error "Unsupported LCD_PANEL_TYPE"
#endif

/*===========================================================================
 * 颜色表 — 由 UI_RGB565_xxx 宏组成，加颜色时在此数组末尾补一行
 *===========================================================================*/

static const uint16_t g_rgb565_table[] = {
    [UI_COLOR_BLACK] = UI_RGB565_BLACK,
    [UI_COLOR_WHITE] = UI_RGB565_WHITE,
    [UI_COLOR_GRAY]  = UI_RGB565_GRAY,
    [UI_COLOR_SKY]   = UI_RGB565_SKY,
    [UI_COLOR_MINT]  = UI_RGB565_MINT,
    [UI_COLOR_AMBER] = UI_RGB565_AMBER,
    [UI_COLOR_ROSE]  = UI_RGB565_ROSE,
    [UI_COLOR_PINK]  = UI_RGB565_PINK,
};

/** @brief 根据颜色编号获取 RGB565 色值 */
static inline uint16_t color_rgb(uint8_t idx)
{
    if (idx < UI_COLOR_COUNT) return g_rgb565_table[idx];
    return UI_RGB565_WHITE;
}

/*===========================================================================
 * MADCTL 值 — 厂家驱动已验证
 *
 * 原厂驱动:
 *   USE_HORIZONTAL=0  → 0x00  (竖屏)
 *   USE_HORIZONTAL=1  → 0xC0
 *   USE_HORIZONTAL=2  → 0x70  (横屏, 当前配置)
 *   USE_HORIZONTAL=3  → 0xA0
 *===========================================================================*/
#define ST7789_MADCTL_VALUE  0x70   /**< 横屏模式控制字 */

/*===========================================================================
 * ST7789 命令码
 *===========================================================================*/

#define ST7789_SLPOUT   0x11  /**< 退出睡眠 */
#define ST7789_INVON    0x21  /**< 反转显示开启（某些模块需要） */
#define ST7789_DISPON   0x29  /**< 开启显示 */
#define ST7789_CASET    0x2A  /**< 列地址设置 */
#define ST7789_RASET    0x2B  /**< 行地址设置 */
#define ST7789_RAMWR    0x2C  /**< 写显存 */
#define ST7789_MADCTL   0x36  /**< 内存访问控制 */
#define ST7789_COLMOD   0x3A  /**< 色彩模式 */

/*===========================================================================
 * 内部状态变量
 *===========================================================================*/

static uint16_t g_fg_color   = 0xFFFF;   /**< 当前前景色 (RGB565 白) — 直写模式使用 */
static uint16_t g_bg_color   = 0x0000;   /**< 当前背景色 (RGB565 黑) */
static uint8_t  g_draw_color = 1;             /**< 绘制颜色编号（0~6，对应预定义颜色） */
static uint8_t  g_font_mode  = 1;             /**< 字体模式：0=不透明背景，1=透明背景 */
static uint8_t  g_font_dir   = 0;             /**< 字体方向（保留） */
static const void *g_current_font = NULL;     /**< 当前使用的字体指针（ASCII或中文） */
static bool     g_is_cn_font  = false;        /**< 当前是否为中文字体 */

enum
{
    ST7789_FB_STRIDE = (OLED_WIDTH + 7) / 8,   /**< 1-bit帧缓冲每行字节数 */
    ST7789_FB_SIZE = ST7789_FB_STRIDE * OLED_HEIGHT, /**< 帧缓冲总大小 */
};

static uint8_t g_framebuffer[ST7789_FB_SIZE];      /**< 当前帧缓冲（1-bit） */
static uint8_t g_last_framebuffer[ST7789_FB_SIZE]; /**< 上一帧缓冲，用于差分刷新 */
static uint8_t g_buffer_mode = 1;                  /**< 缓冲模式：1=使用1-bit缓冲，0=直写GRAM */
static bool    g_last_framebuffer_valid = false;   /**< 上一帧是否有效 */

/*===========================================================================
 * 硬件 SPI 底层函数
 *===========================================================================*/

/**
 * @brief 硬件 SPI 发送一个字节 (MSB 优先, 模式0)
 * @param dat 要发送的字节
 */
static void lcd_write_byte(uint8_t dat)
{
    spi_write_8bit(LCD_SPI_INDEX, dat);
}

/**
 * @brief 硬件 SPI 发送多个字节
 * @param data 数据缓冲区
 * @param len  字节数
 */
static void lcd_write_bytes(const uint8_t *data, uint32_t len)
{
    spi_write_8bit_array(LCD_SPI_INDEX, data, len);
}

/*===========================================================================
 * ST7789 命令/数据写入 — 接口与原厂 LCD_WR_REG/LCD_WR_DATA 一致
 *===========================================================================*/

/**
 * @brief 写入命令字节
 * @param cmd 命令码
 */
void st7789_write_cmd(uint8_t cmd)
{
    LCD_DC_Clr();                       /* DC=0: 命令模式 */
    LCD_CS_Clr();
    lcd_write_byte(cmd);
    LCD_CS_Set();
    LCD_DC_Set();                       /* 恢复 DC=1 (数据模式) */
}

/**
 * @brief 写入8位数据
 * @param data 数据字节
 */
void st7789_write_data8(uint8_t data)
{
    LCD_DC_Set();                       /* DC=1: 数据模式 */
    LCD_CS_Clr();
    lcd_write_byte(data);
    LCD_CS_Set();
}

/**
 * @brief 写入16位数据（高字节在前）
 * @param data 16位数据（RGB565颜色值）
 */
void st7789_write_data16(uint16_t data)
{
    LCD_DC_Set();
    LCD_CS_Clr();
    lcd_write_byte(data >> 8);          /* 高字节在前 */
    lcd_write_byte(data & 0xFF);        /* 低字节在后 */
    LCD_CS_Set();
}

/*===========================================================================
 * 时间函数（需适配平台）
 *===========================================================================*/

/**
 * @brief 获取系统运行毫秒数
 * @return 当前毫秒值
 */
uint32_t st7789_get_ticks(void)
{
    extern uint32 task_get_ms(void);    /* 外部提供的系统时间函数 */
    return task_get_ms();
}

/**
 * @brief 毫秒级延时
 * @param ms 延时毫秒数
 */
void st7789_delay_ms(uint32_t ms)
{
    system_delay_ms(ms);                /* 外部提供的延时函数 */
}

/*===========================================================================
 * 设置绘制窗口 — 与原厂 LCD_Address_Set 完全一致
 *===========================================================================*/

/**
 * @brief 设置 ST7789 绘制窗口 (带列/行偏移校正)
 * @param xs 起始列（0~OLED_WIDTH-1）
 * @param ys 起始行（0~OLED_HEIGHT-1）
 * @param xe 结束列（包含）
 * @param ye 结束行（包含）
 * @note  偏移由 LCD_PANEL_TYPE 选择，避免换屏时只改分辨率导致错位。
 */
void st7789_set_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    st7789_write_cmd(ST7789_CASET);
    st7789_write_data16(xs + TFT_X_OFFSET);
    st7789_write_data16(xe + TFT_X_OFFSET);

    st7789_write_cmd(ST7789_RASET);
    st7789_write_data16(ys + TFT_Y_OFFSET);
    st7789_write_data16(ye + TFT_Y_OFFSET);

    st7789_write_cmd(ST7789_RAMWR);     /* 准备写入像素数据 */
}

/**
 * @brief 连续填充 N 个 RGB565 像素 (快速批量写入)
 * @param color RGB565颜色值
 * @param count 像素数量
 */
void st7789_fill_pixels(uint16_t color, uint32_t count)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    uint8_t buffer[128];                /* 本地缓冲，减少 SPI 发送次数 */

    LCD_DC_Set();
    LCD_CS_Clr();

    /* 预填缓冲为颜色值（高字节+低字节交替） */
    for (uint32_t i = 0; i < sizeof(buffer); i += 2)
    {
        buffer[i] = hi;
        buffer[i + 1] = lo;
    }

    while (count > 0)
    {
        uint32_t pixels = (count > (sizeof(buffer) / 2)) ? (sizeof(buffer) / 2) : count;
        lcd_write_bytes(buffer, pixels * 2);
        count -= pixels;
    }

    LCD_CS_Set();
}

/**
 * @brief 将单色位图（1-bit）转换为 RGB565 并刷到屏幕指定区域
 * @param x      起始列
 * @param y      起始行
 * @param w      宽度
 * @param h      高度
 * @param bits   单色位图数据（每像素1bit，每行 stride 字节）
 * @param stride 每行字节数（通常 ≥ w/8）
 * @param fg     前景色（mask=1 的颜色）
 * @param bg     背景色（mask=0 的颜色）
 */
void st7789_blit_mono(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                      const uint8_t *bits, uint16_t stride,
                      uint16_t fg, uint16_t bg)
{
    static uint8_t buffer[512];         /* 行缓冲，减少 SPI 调用 */
    uint32_t out = 0;

    if (bits == NULL || w == 0 || h == 0) return;
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return;
    if (x + w > OLED_WIDTH) w = OLED_WIDTH - x;
    if (y + h > OLED_HEIGHT) h = OLED_HEIGHT - y;

    st7789_set_window(x, y, x + w - 1, y + h - 1);
    LCD_DC_Set();
    LCD_CS_Clr();

    for (uint16_t row = 0; row < h; row++)
    {
        const uint8_t *src = bits + (uint32_t)(y + row) * stride;
        for (uint16_t col = 0; col < w; col++)
        {
            uint16_t sx = x + col;
            uint16_t color = (src[sx >> 3] & (uint8_t)(0x80 >> (sx & 7))) ? fg : bg;

            buffer[out++] = (uint8_t)(color >> 8);
            buffer[out++] = (uint8_t)(color & 0xFF);
            if (out >= sizeof(buffer))
            {
                lcd_write_bytes(buffer, out);
                out = 0;
            }
        }
    }

    if (out > 0)
    {
        lcd_write_bytes(buffer, out);
    }

    LCD_CS_Set();
}

/*===========================================================================
 * 显示初始化 — 与原厂 LCD_Init 完全一致
 *===========================================================================*/

/**
 * @brief SPI 与控制 GPIO 初始化（适配平台）
 */
static void lcd_gpio_init(void)
{
    spi_init(LCD_SPI_INDEX, SPI_MODE0, LCD_SPI_SPEED, LCD_SCL_SPI_PIN, LCD_SDA_SPI_PIN, SPI_MISO_NULL, SPI_CS_NULL);
    gpio_init(LCD_RES_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(LCD_DC_PIN,  GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(LCD_CS_PIN,  GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(LCD_BLK_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
}

/**
 * @brief ST7789 初始化序列 — 适配 1.14 寸 240×135 模块
 * @note  根据屏幕类型编译不同的 Gamma 和电压参数
 */
static void st7789_hardware_init(void)
{
    lcd_gpio_init();

    /* 硬件复位 */
    LCD_RES_Clr();
    st7789_delay_ms(100);
    LCD_RES_Set();
    st7789_delay_ms(100);

    /* 开启背光 */
    LCD_BLK_Set();
    st7789_delay_ms(100);

    /* ---- ST7789 初始化序列 (厂商提供) ---- */

    st7789_write_cmd(0x11);             /* 退出睡眠 */
    st7789_delay_ms(120);

    st7789_write_cmd(0x36);             /* MADCTL: 内存访问控制 */
    st7789_write_data8(ST7789_MADCTL_VALUE);  /* 横屏模式 */

    st7789_write_cmd(0x3A);             /* COLMOD: 像素格式 */
    st7789_write_data8(0x05);           /* 0x05 = 16-bit/pixel (RGB565) */

    st7789_write_cmd(0xB2);             /* PORCTRL: Porch 控制 */
    st7789_write_data8(0x0C);
    st7789_write_data8(0x0C);
    st7789_write_data8(0x00);
    st7789_write_data8(0x33);
    st7789_write_data8(0x33);

    st7789_write_cmd(0xB7);             /* GCTRL: Gate 控制 */
    st7789_write_data8(0x35);

#if LCD_PANEL_TYPE == LCD_PANEL_147_ST7789
    /* 1.47 寸模块专用参数 */
    st7789_write_cmd(0xBB);             /* VCOMS */
    st7789_write_data8(0x35);

    st7789_write_cmd(0xC0);             /* LCMCTRL */
    st7789_write_data8(0x2C);

    st7789_write_cmd(0xC2);             /* VDVVRHEN */
    st7789_write_data8(0x01);

    st7789_write_cmd(0xC3);             /* VRHS */
    st7789_write_data8(0x13);

    st7789_write_cmd(0xC4);             /* VDVSET */
    st7789_write_data8(0x20);

    st7789_write_cmd(0xC6);             /* FRCTR2 */
    st7789_write_data8(0x0F);

    st7789_write_cmd(0xD0);             /* PWCTRL1 */
    st7789_write_data8(0xA4);
    st7789_write_data8(0xA1);

    st7789_write_cmd(0xD6);
    st7789_write_data8(0xA1);

    st7789_write_cmd(0xE0);             /* 正极性 Gamma: 1.47 寸模块参数 */
    st7789_write_data8(0xF0);
    st7789_write_data8(0x00);
    st7789_write_data8(0x04);
    st7789_write_data8(0x04);
    st7789_write_data8(0x04);
    st7789_write_data8(0x05);
    st7789_write_data8(0x29);
    st7789_write_data8(0x33);
    st7789_write_data8(0x3E);
    st7789_write_data8(0x38);
    st7789_write_data8(0x12);
    st7789_write_data8(0x12);
    st7789_write_data8(0x28);
    st7789_write_data8(0x30);

    st7789_write_cmd(0xE1);             /* 负极性 Gamma: 1.47 寸模块参数 */
    st7789_write_data8(0xF0);
    st7789_write_data8(0x07);
    st7789_write_data8(0x0A);
    st7789_write_data8(0x0D);
    st7789_write_data8(0x0B);
    st7789_write_data8(0x07);
    st7789_write_data8(0x28);
    st7789_write_data8(0x33);
    st7789_write_data8(0x3E);
    st7789_write_data8(0x36);
    st7789_write_data8(0x14);
    st7789_write_data8(0x14);
    st7789_write_data8(0x29);
    st7789_write_data8(0x32);
#else
    /* 1.14 寸模块专用参数 */
    st7789_write_cmd(0xBB);             /* VCOMS */
    st7789_write_data8(0x32);

    st7789_write_cmd(0xC2);             /* VDVVRHEN */
    st7789_write_data8(0x01);

    st7789_write_cmd(0xC3);             /* VRHS */
    st7789_write_data8(0x15);

    st7789_write_cmd(0xC4);             /* VDVSET */
    st7789_write_data8(0x20);

    st7789_write_cmd(0xC6);             /* FRCTR2 */
    st7789_write_data8(0x0F);

    st7789_write_cmd(0xD0);             /* PWCTRL1 */
    st7789_write_data8(0xA4);
    st7789_write_data8(0xA1);

    st7789_write_cmd(0xE0);             /* 正极性 Gamma: 1.14 寸模块参数 */
    st7789_write_data8(0xD0);
    st7789_write_data8(0x08);
    st7789_write_data8(0x0E);
    st7789_write_data8(0x09);
    st7789_write_data8(0x09);
    st7789_write_data8(0x05);
    st7789_write_data8(0x31);
    st7789_write_data8(0x33);
    st7789_write_data8(0x48);
    st7789_write_data8(0x17);
    st7789_write_data8(0x14);
    st7789_write_data8(0x15);
    st7789_write_data8(0x31);
    st7789_write_data8(0x34);

    st7789_write_cmd(0xE1);             /* 负极性 Gamma: 1.14 寸模块参数 */
    st7789_write_data8(0xD0);
    st7789_write_data8(0x08);
    st7789_write_data8(0x0E);
    st7789_write_data8(0x09);
    st7789_write_data8(0x09);
    st7789_write_data8(0x15);
    st7789_write_data8(0x31);
    st7789_write_data8(0x33);
    st7789_write_data8(0x48);
    st7789_write_data8(0x17);
    st7789_write_data8(0x14);
    st7789_write_data8(0x15);
    st7789_write_data8(0x31);
    st7789_write_data8(0x34);
#endif

    /* 反转显示 (此屏模块需要反转才能正常显示颜色) */
    st7789_write_cmd(0x21);             /* INVON */

    /* 开启显示 */
    st7789_write_cmd(0x29);             /* DISPON */
}

/*===========================================================================
 * Astra UI 驱动初始化入口
 *===========================================================================*/

/**
 * @brief 初始化显示驱动并清屏
 * @note 此函数在系统启动时调用一次
 */
void astra_ui_driver_init(void)
{
    st7789_hardware_init();

    /* 清屏为黑色 */
    st7789_set_window(0, 0, OLED_WIDTH - 1, OLED_HEIGHT - 1);
    st7789_fill_pixels(color_rgb(UI_COLOR_BLACK), (uint32_t)OLED_WIDTH * OLED_HEIGHT);

    /* 设置默认字体和模式 */
    st7789_set_font(astra_default_font);
    st7789_set_font_mode(1);            /* 透明背景 */
}

/*===========================================================================
 * 颜色管理
 *===========================================================================*/

/**
 * @brief 设置当前绘图颜色（通过预定义编号）
 * @param color 颜色编号（0~UI_COLOR_COUNT-1），对应 UI_COLOR_xxx 枚举
 */
uint8_t st7789_get_draw_color(void)
{
    return g_draw_color;
}

void st7789_set_draw_color(uint8_t color)
{
    g_draw_color = color;
    g_fg_color = color_rgb(color);
    g_bg_color = (color == UI_COLOR_BLACK) ? color_rgb(UI_COLOR_WHITE) : color_rgb(UI_COLOR_BLACK);
}

void st7789_set_font_mode(uint8_t mode)  { g_font_mode = mode; }   /**< 设置字体模式（0=不透明，1=透明） */
void st7789_set_font_dir(uint8_t dir)    { g_font_dir = dir; }     /**< 设置字体方向（保留） */
void st7789_set_buffer_mode(uint8_t enable) { g_buffer_mode = enable ? 1 : 0; } /**< 启用/禁用缓冲模式 */

/**
 * @brief 在1-bit帧缓冲中画一个像素（内部函数）
 * @param x,y   坐标
 * @param color 缓冲模式下 0=清除，非 0=设置。RGB565 颜色只在直写模式生效。
 */
static void st7789_fb_pixel(int16_t x, int16_t y, uint8_t color)
{
    uint32_t index;
    uint8_t mask;

    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
    index = (uint32_t)y * ST7789_FB_STRIDE + ((uint16_t)x >> 3);
    mask = (uint8_t)(0x80 >> ((uint16_t)x & 7));
    if (color) g_framebuffer[index] |= mask;
    else g_framebuffer[index] &= (uint8_t)~mask;
}

/**
 * @brief 清屏（黑色）
 * @note 缓冲模式下清除1-bit缓冲，直写模式下直接填充黑色到GRAM
 */
void st7789_clear_screen(void)
{
    if (g_buffer_mode)
    {
        memset(g_framebuffer, 0, sizeof(g_framebuffer));
        return;
    }

    st7789_set_window(0, 0, OLED_WIDTH - 1, OLED_HEIGHT - 1);
    st7789_fill_pixels(color_rgb(UI_COLOR_BLACK), (uint32_t)OLED_WIDTH * OLED_HEIGHT);
}

/**
 * @brief 将1-bit帧缓冲刷到屏幕（差分刷新，只更新变化区域）
 * @note 如果上一帧无效则全屏刷新，否则计算差异区域并局部刷新
 */
void st7789_send_buffer(void)
{
    if (!g_last_framebuffer_valid)
    {
        /* 首次刷新：全屏发送 */
        st7789_blit_mono(0, 0, OLED_WIDTH, OLED_HEIGHT,
                         g_framebuffer, ST7789_FB_STRIDE,
                         color_rgb(UI_COLOR_WHITE), color_rgb(UI_COLOR_BLACK));
        memcpy(g_last_framebuffer, g_framebuffer, sizeof(g_framebuffer));
        g_last_framebuffer_valid = true;
        return;
    }

    /* 计算变化区域边界（按字节比较，提高效率） */
    uint16_t min_x_byte = ST7789_FB_STRIDE;
    uint16_t max_x_byte = 0;
    uint16_t min_y = OLED_HEIGHT;
    uint16_t max_y = 0;

    for (uint16_t y = 0; y < OLED_HEIGHT; y++)
    {
        uint32_t row_offset = (uint32_t)y * ST7789_FB_STRIDE;

        for (uint16_t x_byte = 0; x_byte < ST7789_FB_STRIDE; x_byte++)
        {
            uint32_t index = row_offset + x_byte;
            if (g_framebuffer[index] != g_last_framebuffer[index])
            {
                if (x_byte < min_x_byte) min_x_byte = x_byte;
                if (x_byte > max_x_byte) max_x_byte = x_byte;
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;
            }
        }
    }

    if (min_y >= OLED_HEIGHT)
    {
        return;   /* 无变化 */
    }

    uint16_t x = min_x_byte * 8;
    uint16_t w = (max_x_byte - min_x_byte + 1) * 8;
    uint16_t h = max_y - min_y + 1;

    if (x + w > OLED_WIDTH)
    {
        w = OLED_WIDTH - x;
    }

    st7789_blit_mono(x, min_y, w, h,
                     g_framebuffer, ST7789_FB_STRIDE, color_rgb(UI_COLOR_WHITE), color_rgb(UI_COLOR_BLACK));
    memcpy(g_last_framebuffer, g_framebuffer, sizeof(g_framebuffer));
}

void st7789_send_area_buffer(int16_t x, int16_t y, int16_t w, int16_t h)
{
    int16_t x1 = x + w - 1;
    int16_t y1 = y + h - 1;

    if (w <= 0 || h <= 0) return;
    if (x1 < 0 || y1 < 0 || x >= OLED_WIDTH || y >= OLED_HEIGHT) return;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x1 >= OLED_WIDTH) x1 = OLED_WIDTH - 1;
    if (y1 >= OLED_HEIGHT) y1 = OLED_HEIGHT - 1;

    st7789_blit_mono((uint16_t)x, (uint16_t)y,
                     (uint16_t)(x1 - x + 1), (uint16_t)(y1 - y + 1),
                     g_framebuffer, ST7789_FB_STRIDE, color_rgb(UI_COLOR_WHITE), color_rgb(UI_COLOR_BLACK));
}

/*===========================================================================
 * 几何绘制 — 缓冲模式下写入 1-bit framebuffer，直写模式下写入 GRAM
 *===========================================================================*/

/**
 * @brief 绘制一个像素
 */
void st7789_draw_pixel(int16_t x, int16_t y)
{
    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
    if (g_buffer_mode)
    {
        st7789_fb_pixel(x, y, g_draw_color);
        return;
    }

    st7789_set_window(x, y, x, y);
    st7789_write_data16(g_fg_color);
}

/**
 * @brief 绘制水平线
 */
void st7789_draw_hline(int16_t x, int16_t y, int16_t len)
{
    if (y < 0 || y >= OLED_HEIGHT) return;
    if (x < 0) { len += x; x = 0; }
    if (x + len > OLED_WIDTH) len = OLED_WIDTH - x;
    if (len <= 0) return;

    if (g_buffer_mode)
    {
        for (int16_t i = 0; i < len; i++)
            st7789_fb_pixel(x + i, y, g_draw_color);
        return;
    }

    st7789_set_window(x, y, x + len - 1, y);
    st7789_fill_pixels(g_fg_color, len);
}

/**
 * @brief 绘制垂直线
 */
void st7789_draw_vline(int16_t x, int16_t y, int16_t h)
{
    if (x < 0 || x >= OLED_WIDTH) return;
    if (y < 0) { h += y; y = 0; }
    if (y + h > OLED_HEIGHT) h = OLED_HEIGHT - y;
    if (h <= 0) return;

    if (g_buffer_mode)
    {
        for (int16_t i = 0; i < h; i++)
            st7789_fb_pixel(x, y + i, g_draw_color);
        return;
    }

    st7789_set_window(x, y, x, y + h - 1);
    st7789_fill_pixels(g_fg_color, h);
}

/**
 * @brief 使用 Bresenham 算法绘制任意直线
 */
void st7789_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    int16_t dx = abs(x2 - x1), dy = abs(y2 - y1);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;

    while (1)
    {
        st7789_draw_pixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int16_t e2 = err * 2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

/**
 * @brief 绘制填充矩形（实心）
 */
void st7789_draw_box(int16_t x, int16_t y, int16_t w, int16_t h)
{
    if (w <= 0 || h <= 0) return;
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > OLED_WIDTH)  w = OLED_WIDTH  - x;
    if (y + h > OLED_HEIGHT) h = OLED_HEIGHT - y;
    if (w <= 0 || h <= 0) return;

    if (g_buffer_mode)
    {
        for (int16_t yy = 0; yy < h; yy++)
            st7789_draw_hline(x, y + yy, w);
        return;
    }

    st7789_set_window(x, y, x + w - 1, y + h - 1);
    st7789_fill_pixels(g_fg_color, (uint32_t)w * h);
}

/**
 * @brief 绘制矩形边框（空心）
 */
void st7789_draw_frame(int16_t x, int16_t y, int16_t w, int16_t h)
{
    st7789_draw_hline(x,         y,         w);
    st7789_draw_hline(x,         y + h - 1, w);
    st7789_draw_vline(x,         y + 1,     h - 2);
    st7789_draw_vline(x + w - 1, y + 1,     h - 2);
}

/**
 * @brief 绘制水平虚线（每3像素中前2像素实心，后1像素空白）
 */
void st7789_draw_hline_dotted(int16_t x, int16_t y, int16_t len)
{
    for (int16_t i = 0; i < len; i += 3)
    {
        int16_t seg = (i + 2 <= len) ? 2 : len - i;
        st7789_draw_hline(x + i, y, seg);
    }
}

/**
 * @brief 绘制垂直虚线
 */
void st7789_draw_vline_dotted(int16_t x, int16_t y, int16_t len)
{
    for (int16_t i = 0; i < len; i += 3)
    {
        int16_t seg = (i + 2 <= len) ? 2 : len - i;
        st7789_draw_vline(x, y + i, seg);
    }
}

/*===========================================================================
 * 圆角矩形
 *===========================================================================*/

/**
 * @brief 绘制填充圆角矩形（使用预计算平方根）
 */
void st7789_draw_rbox(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r)
{
    if (w <= 0 || h <= 0) return;
    if (r <= 0) { st7789_draw_box(x, y, w, h); return; }
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;

    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > OLED_WIDTH)  w = OLED_WIDTH  - x;
    if (y + h > OLED_HEIGHT) h = OLED_HEIGHT - y;
    if (w <= 0 || h <= 0) return;

    int16_t r2 = r * r;
    /* 绘制中间矩形部分（四个圆角之间的区域） */
    st7789_draw_box(x + r, y,     w - 2 * r, h);
    st7789_draw_box(x,     y + r, w,         h - 2 * r);

    /* 绘制四个圆角（逐行扫描，使用圆方程） */
    for (int16_t dy = 0; dy < r; dy++)
    {
        int16_t cy = r - 1 - dy;
        int16_t dx = (int16_t)(sqrtf((float)(r2 - cy * cy)) + 0.5f);

        st7789_draw_hline(x + r - dx, y + dy,          w - 2 * r + 2 * dx);
        st7789_draw_hline(x + r - dx, y + h - 1 - dy,  w - 2 * r + 2 * dx);
    }
}

/**
 * @brief 绘制圆角矩形边框（空心）
 */
void st7789_draw_rframe(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r)
{
    if (w <= 0 || h <= 0) return;
    if (r <= 0) { st7789_draw_frame(x, y, w, h); return; }
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;

    int16_t cx_l = x + r;
    int16_t cx_r = x + w - r - 1;
    int16_t cy_t = y + r;
    int16_t cy_b = y + h - r - 1;

    /* 四条直边 */
    st7789_draw_hline(x + r, y,         w - 2 * r);
    st7789_draw_hline(x + r, y + h - 1, w - 2 * r);
    st7789_draw_vline(x,     y + r,     h - 2 * r);
    st7789_draw_vline(x + w - 1, y + r, h - 2 * r);

    /* 四个圆角（使用近似画圆方法，通过半径容差绘制像素） */
    int16_t r2 = r * r;
    int16_t band = r + 1;

    for (int16_t py = 0; py <= r; py++)
    {
        for (int16_t px = 0; px <= r; px++)
        {
            int16_t dx = px - r;
            int16_t dy = py - r;
            int16_t delta = dx * dx + dy * dy - r2;
            if (delta < 0) delta = -delta;
            if (delta > band) continue;

            st7789_draw_pixel(cx_l + dx, cy_t + dy);
            st7789_draw_pixel(cx_r - dx, cy_t + dy);
            st7789_draw_pixel(cx_l + dx, cy_b - dy);
            st7789_draw_pixel(cx_r - dx, cy_b - dy);
        }
    }
}

/*===========================================================================
 * 圆形 — Bresenham 中点画圆
 *===========================================================================*/

/**
 * @brief 绘制圆形边框（Bresenham 算法）
 */
void st7789_draw_circle(int16_t cx, int16_t cy, int16_t r)
{
    if (r <= 0) return;

    int16_t x = 0, y = r, d = 1 - r;
    while (x <= y)
    {
        /* 对称绘制8个点 */
        st7789_draw_pixel(cx + x, cy + y);
        st7789_draw_pixel(cx + y, cy + x);
        st7789_draw_pixel(cx - x, cy + y);
        st7789_draw_pixel(cx - y, cy + x);
        st7789_draw_pixel(cx + x, cy - y);
        st7789_draw_pixel(cx + y, cy - x);
        st7789_draw_pixel(cx - x, cy - y);
        st7789_draw_pixel(cx - y, cy - x);

        if (d < 0) { d += 2 * x + 3; }
        else       { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

/*===========================================================================
 * 位图绘制 (单色位图 → RGB565 像素)
 *===========================================================================*/

/**
 * @brief 绘制单色位图（1-bit，0=透明/背景，1=前景色）
 * @param x,y    左上角坐标
 * @param w,h    位图尺寸
 * @param bitmap 位图数据，每行按字节对齐（高位在前）
 */
void st7789_draw_bitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *bitmap)
{
    if (bitmap == NULL) return;

    int16_t bpr = (w + 7) / 8;
    for (int16_t row = 0; row < h; row++)
    {
        for (int16_t col = 0; col < w; col++)
        {
            uint8_t b = bitmap[row * bpr + col / 8];
            if (b & (0x80 >> (col & 0x07)))
                st7789_draw_pixel(x + col, y + row);
        }
    }
}

/*===========================================================================
 * 字体系统 — 字符渲染引擎
 *===========================================================================*/

/**
 * @brief 设置当前字体（ASCII或中文）
 * @param font 字体结构体指针
 */
void st7789_set_font(const void *font)
{
    g_current_font = font;
    extern const st7789_cn_font_t cn_font_16;
    g_is_cn_font = (font == (const void*)&cn_font_16);
}

/**
 * @brief 获取当前字体高度（像素）
 * @return 字体高度
 */
int16_t st7789_get_font_height(void)
{
    if (g_current_font == NULL) return 16;
    if (g_is_cn_font) return 16;

    const st7789_font_t *f = (const st7789_font_t *)g_current_font;
    return f->char_h;
}

/**
 * @brief 绘制单个 ASCII 字符 (8×16 字库)
 * @param x,y 左上角坐标
 * @param ch  字符（ASCII）
 * @return 字符宽度（像素）
 */
static int16_t draw_ascii_char(int16_t x, int16_t y, char ch)
{
    if (g_current_font == NULL || g_is_cn_font) return 0;

    const st7789_font_t *font = (const st7789_font_t *)g_current_font;
    if (ch < font->first || ch > font->last) return 0;

    uint16_t idx        = (uint16_t)(ch - font->first);
    int16_t  cw         = font->char_w;
    int16_t  ch_h       = font->char_h;
    int16_t  bpr        = (cw + 7) / 8;
    uint16_t glyph_off  = (uint16_t)idx * ch_h * bpr;
    const uint8_t *glyph = &font->bitmap[glyph_off];

    for (int16_t row = 0; row < ch_h; row++)
        for (int16_t col = 0; col < cw; col++)
            if (glyph[row * bpr + col / 8] & (0x80 >> (col & 0x07)))
                st7789_draw_pixel(x + col, y + row);

    return cw;
}

/**
 * @brief 绘制单个中文字符 (16×16 字库)
 * @param x,y     左上角坐标
 * @param unicode Unicode 编码
 * @return 宽度（固定16像素）
 */
static int16_t draw_cn_char(int16_t x, int16_t y, uint16_t unicode)
{
    extern const uint8_t *cn_font_lookup(uint16_t unicode);
    const uint8_t *glyph = cn_font_lookup(unicode);

    if (glyph == NULL)
    {
        /* 缺字占位: 方框 + 叉 */
        st7789_draw_frame(x, y, 16, 16);
        st7789_draw_line(x + 4, y + 4, x + 11, y + 11);
        st7789_draw_line(x + 11, y + 4, x + 4, y + 11);
        return 16;
    }

    for (int16_t row = 0; row < 16; row++)
        for (int16_t col = 0; col < 16; col++)
            if (glyph[row * 2 + col / 8] & (0x80 >> (col & 0x07)))
                st7789_draw_pixel(x + col, y + row);

    return 16;
}

/*===========================================================================
 * 字符串绘制
 *===========================================================================*/

/**
 * @brief 计算 ASCII 字符串宽度（像素）
 */
int16_t st7789_get_str_width(const char *str)
{
    if (g_current_font == NULL || str == NULL) return 0;
    if (g_is_cn_font) return (int16_t)(strlen(str) * 16);

    const st7789_font_t *f = (const st7789_font_t *)g_current_font;
    return (int16_t)(strlen(str) * f->char_w);
}

/**
 * @brief 计算 UTF-8 字符串宽度（中英文混合）
 */
int16_t st7789_get_utf8_width(const char *str)
{
    if (str == NULL) return 0;
    int16_t total = 0;
    const char *p = str;
    while (*p)
    {
        uint8_t c = (uint8_t)*p;
        if (c < 0x80)      { total += 8;  p++;      }   /* ASCII 字符 8px */
        else if ((c & 0xE0) == 0xC0) { total += 16; p += 2; } /* 2字节UTF-8，按中文宽 */
        else if ((c & 0xF0) == 0xE0) { total += 16; p += 3; } /* 3字节UTF-8（中文） */
        else                         { total += 16; p++;     } /* 无效字节，占16px */
    }
    return total;
}

/**
 * @brief 绘制 ASCII 字符串（仅支持 ASCII，不处理中文）
 */
void st7789_draw_str(int16_t x, int16_t y, const char *str)
{
    if (g_current_font == NULL || str == NULL) return;
    y -= st7789_get_font_height() - 1;   /* 调整y为基线位置 */
    while (*str) { x += draw_ascii_char(x, y, *str); str++; }
}

/**
 * @brief 绘制 UTF-8 字符串 (中英文混排, 自动识别)
 * @note 中文需要加载中文字库（cn_font_16），ASCII 使用 8x16 字体
 */
void st7789_draw_utf8(int16_t x, int16_t y, const char *str)
{
    if (str == NULL) return;
    extern const st7789_font_t font_8x16;

    y -= st7789_get_font_height() - 1;

    const char *p = str;
    while (*p)
    {
        uint8_t c = (uint8_t)*p;

        if (c < 0x80)
        {
            /* ASCII — 切换到 ASCII 字体绘制 */
            const void *saved = g_current_font;
            bool  scn   = g_is_cn_font;
            g_current_font = (const void*)&font_8x16;
            g_is_cn_font   = false;
            x += draw_ascii_char(x, y, *p);
            g_current_font = saved;
            g_is_cn_font   = scn;
            p++;
        }
        else if ((c & 0xE0) == 0xE0 && p[1] && p[2])
        {
            /* UTF-8 3字节 → Unicode (中文) */
            uint16_t unicode = ((uint16_t)(c & 0x0F) << 12)
                             | ((uint16_t)(p[1] & 0x3F) << 6)
                             |  (uint16_t)(p[2] & 0x3F);
            x += draw_cn_char(x, y, unicode);
            p += 3;
        }
        else if ((c & 0xE0) == 0xC0) { p += 2; }  /* Latin-1 扩展, 跳过 */
        else                         { p++;     }  /* 无效字节, 跳过   */
    }
}

/*===========================================================================
 * 引入字库数据（外部链接）
 *===========================================================================*/

extern const st7789_font_t font_8x16;
#include "font_8x16.h"
#include "font_cn_16.h"

/* ASCII 字库结构体定义（数据在 font_8x16.h 中） */
extern const uint16_t font_8x16_offset[];

const st7789_font_t font_8x16 = {
    .char_w = 8,
    .char_h = 16,
    .first  = 0x20,
    .last   = 0x7E,
    .bitmap = font_8x16_data,
};

/*===========================================================================
 * 文件结束
 *===========================================================================*/
