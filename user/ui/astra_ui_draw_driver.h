/**
 * @file    astra_ui_draw_driver.h
 * @brief   Astra UI 硬件抽象层 — ST7789 1.14 寸 240×135 横屏版
 * @note    基于厂家 (TFT-147-HSD-ST7789-4WSPI-STM32) 驱动验证
 * @note    MSPM0 硬件 SPI1: SCK=PB9, MOSI=PB8
 * @note    MADCTL=0x70 (横屏模式2), X_OFFSET=40, Y_OFFSET=53
 *
 * 移植说明：
 *   本文件将原 U8g2 的宏定义替换为 ST7789 裸驱动函数调用。
 *   astra_ui_draw_driver.c 中使用硬件 SPI, CS/DC/RES/BLK 使用普通 GPIO。
 *   字体系统: 自建 8×16 ASCII + 16×16 中文字库 (st7789_font.h)
 */

#ifndef ASTRA_UI_DRAW_DRIVER_H
#define ASTRA_UI_DRAW_DRIVER_H

#include "board_config.h"
#include <stdint.h>
#include <stdbool.h>

/*===========================================================================
 * 屏幕参数 — 1.14 寸 ST7789 横屏使用 (240×135)
 *===========================================================================*/

#if LCD_PANEL_TYPE == LCD_PANEL_147_ST7789
/** @brief OLED 逻辑宽度 (1.47 寸横屏: 320 像素) */
#define OLED_WIDTH  320
/** @brief OLED 逻辑高度 (1.47 寸横屏: 172 像素) */
#define OLED_HEIGHT 172
#elif LCD_PANEL_TYPE == LCD_PANEL_114_ST7789
/** @brief OLED 逻辑宽度 (1.14 寸横屏: 240 像素) */
#define OLED_WIDTH  240
/** @brief OLED 逻辑高度 (1.14 寸横屏: 135 像素) */
#define OLED_HEIGHT 135
#else
#error "Unsupported LCD_PANEL_TYPE"
#endif

/*===========================================================================
 * 绘制颜色定义 (RGB565)
 *===========================================================================*/

#define COLOR_BLACK   0x0000  /**< 黑色 */
#define COLOR_WHITE   0xFFFF  /**< 白色 */
#define COLOR_GRAY    0x8410  /**< 灰色 (用于 XOR 效果) */

/*===========================================================================
 * 字体引用 — 指向 st7789_font.h 定义的字体结构
 *===========================================================================*/

#include "st7789_font.h"

/** @brief Astra UI 默认字体 (中文 16×16) */
#define astra_default_font  (&cn_font_16)

/*===========================================================================
 * 系统时间宏 — 适配 MSPM0
 *===========================================================================*/

/** @brief 获取系统毫秒级 tick
 *  @note   MSPM0: 使用 SysTick 或 Timer 实现
 *          示例: #define get_ticks()  g_sys_tick_ms
 */
#define get_ticks()     st7789_get_ticks()

/** @brief 毫秒延时
 *  @note   MSPM0: 使用 DL_DelayMS() 或 SysTick 实现
 */
#define delay(ms)       st7789_delay_ms(ms)

/*===========================================================================
 * OLED 绘制函数宏 — 映射到 ST7789 驱动函数
 *===========================================================================*/

/** @brief 设置当前字体 */
#define oled_set_font(font)             st7789_set_font((void*)(font))

/** @brief 绘制 ASCII 字符串 (x,y) 左上角坐标 */
#define oled_draw_str(x, y, str)        st7789_draw_str((int16_t)(x), (int16_t)(y), (const char*)(str))

/** @brief 绘制 UTF-8 字符串 (支持中英文混排) */
#define oled_draw_UTF8(x, y, str)       st7789_draw_utf8((int16_t)(x), (int16_t)(y), (const char*)(str))

/** @brief 获取 ASCII 字符串像素宽度 */
#define oled_get_str_width(str)         st7789_get_str_width((const char*)(str))

/** @brief 获取 UTF-8 字符串像素宽度 */
#define oled_get_UTF8_width(str)        st7789_get_utf8_width((const char*)(str))

/** @brief 获取当前字体字符高度 (像素) */
#define oled_get_str_height()           st7789_get_font_height()

/** @brief 绘制单个像素 */
#define oled_draw_pixel(x, y)           st7789_draw_pixel((int16_t)(x), (int16_t)(y))

/** @brief 绘制实心圆 */
#define oled_draw_circle(x, y, r)       st7789_draw_circle((int16_t)(x), (int16_t)(y), (int16_t)(r))

/** @brief 绘制圆角矩形 (实心) — 坐标偏移修正
 *  @note  U8g2 的 RBox 以左上角为原点，ST7789 版本保持一致
 */
#define oled_draw_R_box(x, y, w, h, r)  st7789_draw_rbox((int16_t)(x), (int16_t)(y), (int16_t)(w), (int16_t)(h), (int16_t)(r))

/** @brief 绘制矩形 (实心) */
#define oled_draw_box(x, y, w, h)       st7789_draw_box((int16_t)(x), (int16_t)(y), (int16_t)(w), (int16_t)(h))

/** @brief 绘制矩形边框 (空心) */
#define oled_draw_frame(x, y, w, h)     st7789_draw_frame((int16_t)(x), (int16_t)(y), (int16_t)(w), (int16_t)(h))

/** @brief 绘制圆角矩形边框 (空心) */
#define oled_draw_R_frame(x, y, w, h, r) st7789_draw_rframe((int16_t)(x), (int16_t)(y), (int16_t)(w), (int16_t)(h), (int16_t)(r))

/** @brief 绘制水平线 */
#define oled_draw_H_line(x, y, l)       st7789_draw_hline((int16_t)(x), (int16_t)(y), (int16_t)(l))

/** @brief 绘制竖直线 */
#define oled_draw_V_line(x, y, h)       st7789_draw_vline((int16_t)(x), (int16_t)(y), (int16_t)(h))

/** @brief 绘制直线 (Bresenham) */
#define oled_draw_line(x1, y1, x2, y2)  st7789_draw_line((int16_t)(x1), (int16_t)(y1), (int16_t)(x2), (int16_t)(y2))

/** @brief 绘制水平虚线 */
#define oled_draw_H_dotted_line(x, y, l) st7789_draw_hline_dotted((int16_t)(x), (int16_t)(y), (int16_t)(l))

/** @brief 绘制竖直虚线 */
#define oled_draw_V_dotted_line(x, y, h) st7789_draw_vline_dotted((int16_t)(x), (int16_t)(y), (int16_t)(h))

/** @brief 绘制位图 (单色，1 bit/像素) */
#define oled_draw_bMP(x, y, w, h, bmp)  st7789_draw_bitmap((int16_t)(x), (int16_t)(y), (int16_t)(w), (int16_t)(h), (const uint8_t*)(bmp))

/** @brief 设置绘制颜色
 *  @param color  0 = 黑色, 1 = 白色, 2 = XOR/灰色
 */
#define oled_set_draw_color(color)      st7789_set_draw_color((uint8_t)(color))

/** @brief 设置字体绘制模式 (1=透明背景, 0=实色背景) */
#define oled_set_font_mode(mode)        st7789_set_font_mode((uint8_t)(mode))

/** @brief 设置字体绘制方向 */
#define oled_set_font_direction(dir)    st7789_set_font_dir((uint8_t)(dir))

/** @brief 清空显示缓冲区 (直接写屏模式: 全屏填充黑色) */
#define oled_clear_buffer()             st7789_clear_screen()

/** @brief 发送缓冲区到屏幕 (直接写屏模式: 空操作) */
#define oled_send_buffer()              ((void)0)

/** @brief 发送局部缓冲区到屏幕 */
#define oled_send_area_buffer(x, y, w, h) ((void)0)

/*===========================================================================
 * 驱动函数声明
 *===========================================================================*/

/** @brief ST7789 初始化 — SPI、GPIO、屏幕配置、背光 */
void astra_ui_driver_init(void);

/* --- 时间函数 (用户需实现或修改) --- */
uint32_t st7789_get_ticks(void);
void     st7789_delay_ms(uint32_t ms);

/* --- 绘制核心 --- */
void st7789_set_draw_color(uint8_t color);
void st7789_set_font_mode(uint8_t mode);
void st7789_set_font_dir(uint8_t dir);
void st7789_clear_screen(void);

/* --- 像素/几何绘制 --- */
void st7789_draw_pixel(int16_t x, int16_t y);
void st7789_draw_hline(int16_t x, int16_t y, int16_t len);
void st7789_draw_vline(int16_t x, int16_t y, int16_t len);
void st7789_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void st7789_draw_box(int16_t x, int16_t y, int16_t w, int16_t h);
void st7789_draw_frame(int16_t x, int16_t y, int16_t w, int16_t h);
void st7789_draw_rbox(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
void st7789_draw_rframe(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r);
void st7789_draw_circle(int16_t cx, int16_t cy, int16_t r);
void st7789_draw_hline_dotted(int16_t x, int16_t y, int16_t len);
void st7789_draw_vline_dotted(int16_t x, int16_t y, int16_t len);

/* --- 位图绘制 --- */
void st7789_draw_bitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *bitmap);

/* --- 字体与文本 --- */
void     st7789_set_font(const void *font);
int16_t  st7789_get_font_height(void);
int16_t  st7789_get_str_width(const char *str);
int16_t  st7789_get_utf8_width(const char *str);
void     st7789_draw_str(int16_t x, int16_t y, const char *str);
void     st7789_draw_utf8(int16_t x, int16_t y, const char *str);

/* --- SPI 底层 (用户需根据 MSPM0 实现) --- */
void st7789_write_cmd(uint8_t cmd);
void st7789_write_data8(uint8_t data);
void st7789_write_data16(uint16_t data);
void st7789_set_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);
void st7789_fill_pixels(uint16_t color, uint32_t count);
void st7789_blit_mono(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                      const uint8_t *bits, uint16_t stride,
                      uint16_t fg, uint16_t bg);

#endif /* ASTRA_UI_DRAW_DRIVER_H */
