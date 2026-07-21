/**
 * @file    app_ui.c
 * @brief   B21 单键菜单 UI，使用 1-bit 缓冲和 ST7789 局部刷新。
 *
 * 修改入口:
 *   1. 页面和选项: 搜索“UI 页面配置区”
 *   2. 布局尺寸: UI_ROW_Y0 / UI_ROW_H / UI_ROW_BOX_H / UI_TEXT_PAD_X
 *   3. 颜色: UI_COLOR_xxx
 *   4. 按键动作: ui_do_action()
 */

#include "app_ui.h"
#include "astra_ui_draw_driver.h"
#include "st7789_font.h"
#include <stdio.h>
#include <string.h>

/*===========================================================================
 * 类型定义
 *=========================================================================*/

/**
 * @brief UI 可识别的按键动作类型，由手势识别状态机产生。
 */
typedef enum
{
    UI_ACTION_SINGLE,   /**< 单击 */
    UI_ACTION_DOUBLE,   /**< 双击 */
    UI_ACTION_LONG,     /**< 长按 */
    UI_ACTION_COMBO,    /**< 组合：单击后立即长按（click+hold） */
} ui_action_enum;

typedef enum
{
    UI_PAGE_MAIN = 0,
    UI_PAGE_OPTION_1,
    UI_PAGE_OPTION_2,
    UI_PAGE_OPTION_3,
    UI_PAGE_OPTION_4,
    UI_PAGE_COUNT,
    UI_PAGE_NONE = 0xFF,
} ui_page_id_enum;

typedef struct
{
    const char *text;           /* 屏幕上显示的选项文字 */
    uint8 next_page;            /* 双击后跳转到哪个页面；UI_PAGE_NONE 表示不跳转 */
    const char *status;         /* 双击/进入后显示在底部状态栏的文字 */
} ui_menu_item_t;

typedef struct
{
    const char *title;              /* 顶部标题 */
    const ui_menu_item_t *items;    /* 本页选项数组 */
    uint8 item_count;               /* 本页选项数量 */
    uint8 back_page;                /* 长按返回到哪个页面 */
} ui_page_t;

/*===========================================================================
 * 布局及颜色常量
 *=========================================================================*/

enum
{
    /**
     * 1-bit 帧缓冲布局：
     * - 每个像素占用 1 bit，0=背景色，1=前景色。
     * - 每行按 8 像素对齐，便于快速位操作和 DMA 传输。
     */
    UI_FB_STRIDE = (OLED_WIDTH + 7) / 8,   /**< 每行字节数 */
    UI_FB_SIZE = UI_FB_STRIDE * OLED_HEIGHT, /**< 总字节数 */

    /* 顶部标题栏和底部状态栏高度（8x16 字体，留 2px 间距） */
    UI_HEADER_H = 18,
    UI_FOOTER_H = 18,
    UI_FOOTER_Y = OLED_HEIGHT - UI_FOOTER_H,

    /* 菜单区域纵向范围（标题栏底部到状态栏顶部） */
    UI_MENU_Y = UI_HEADER_H + 1,
    UI_MENU_H = UI_FOOTER_Y - UI_MENU_Y,

    /* 菜单项通用布局参数 */
    UI_ROW_X = 4,          /**< 菜单项方框左边距 */
    UI_ROW_W = OLED_WIDTH - 20, /**< 菜单项方框最大宽度，右侧预留滑条空间 */
    UI_TEXT_PAD_X = 10,    /**< 文字与方框左右内边距 */
    UI_RADIUS = 3,
    UI_SCROLL_W = 5,
    UI_SCROLL_X = OLED_WIDTH - UI_SCROLL_W - 4,
    UI_SCROLL_Y = UI_MENU_Y + 4,
    UI_SCROLL_H = UI_FOOTER_Y - UI_SCROLL_Y - 4,
    UI_SCROLL_THUMB_H = 18,

#if OLED_WIDTH >= 300
    /* 1.47 寸 320x172 屏幕，行距较宽 */
    UI_ROW_Y0 = 26,
    UI_ROW_H = 24,
    UI_ROW_BOX_H = 20,
#else
    /* 1.14 寸 240x135 屏幕，行距压缩以确保四项菜单完整显示 */
    UI_ROW_Y0 = 22,
    UI_ROW_H = 22,
    UI_ROW_BOX_H = 18,
#endif
};

/**
 * @brief RGB565 颜色值，用于最终刷新时的前景色映射。
 *        注意：1-bit 缓冲只存储 mask，实际颜色在 blit 时指定。
 */
enum
{
    UI_COLOR_BG     = 0x0000, /**< 背景黑色 */
    UI_COLOR_TITLE  = 0x07FF, /**< 标题栏青色 */
    UI_COLOR_TEXT   = 0xC618, /**< 普通菜单文字灰色 */
    UI_COLOR_SELECT = 0xfd96, /**< 选中框粉色 */
    UI_COLOR_STATUS = 0x07E0, /**< 状态栏绿色 */
};

/*===========================================================================
 * 静态数据
 *=========================================================================*/

/*===========================================================================
 * UI 页面配置区
 *
 * 修改方法:
 *   标题：改 s_pages[] 里的 title
 *   选项：改 s_xxx_items[] 里的 text
 *   跳转：改 next_page，例如 UI_PAGE_OPTION_1
 *   返回：改 s_pages[] 里的 back_page
 *
 * 操作约定:
 *   单击：光标下移
 *   双击：进入当前选项 next_page
 *   长按：返回 back_page
 *=========================================================================*/

static const ui_menu_item_t s_main_items[] = {
    {"Option 1", UI_PAGE_OPTION_1, "OPEN OPTION 1"},
    {"Option 2", UI_PAGE_OPTION_2, "OPEN OPTION 2"},
    {"Option 3", UI_PAGE_OPTION_3, "OPEN OPTION 3"},
    {"Option 4", UI_PAGE_OPTION_4, "OPEN OPTION 4"},
};

static const ui_menu_item_t s_option1_items[] = {
    {"Option 1-A", UI_PAGE_NONE, "OPTION 1-A"},
    {"Option 1-B", UI_PAGE_NONE, "OPTION 1-B"},
    {"Back", UI_PAGE_MAIN, "BACK MAIN"},
};

static const ui_menu_item_t s_option2_items[] = {
    {"Option 2-A", UI_PAGE_NONE, "OPTION 2-A"},
    {"Option 2-B", UI_PAGE_NONE, "OPTION 2-B"},
    {"Back", UI_PAGE_MAIN, "BACK MAIN"},
};

static const ui_menu_item_t s_option3_items[] = {
    {"Option 3-A", UI_PAGE_NONE, "OPTION 3-A"},
    {"Option 3-B", UI_PAGE_NONE, "OPTION 3-B"},
    {"Back", UI_PAGE_MAIN, "BACK MAIN"},
};

static const ui_menu_item_t s_option4_items[] = {
    {"Option 4-A", UI_PAGE_NONE, "OPTION 4-A"},
    {"Option 4-B", UI_PAGE_NONE, "OPTION 4-B"},
    {"Back", UI_PAGE_MAIN, "BACK MAIN"},
};

#define UI_ITEM_COUNT(items) ((uint8)(sizeof(items) / sizeof((items)[0])))

static const ui_page_t s_pages[UI_PAGE_COUNT] = {
    [UI_PAGE_MAIN]     = {"2026 Dian_Sai_UI_demo", s_main_items,    UI_ITEM_COUNT(s_main_items),    UI_PAGE_MAIN},
    [UI_PAGE_OPTION_1] = {"Option 1",  s_option1_items, UI_ITEM_COUNT(s_option1_items), UI_PAGE_MAIN},
    [UI_PAGE_OPTION_2] = {"Option 2",  s_option2_items, UI_ITEM_COUNT(s_option2_items), UI_PAGE_MAIN},
    [UI_PAGE_OPTION_3] = {"Option 3",  s_option3_items, UI_ITEM_COUNT(s_option3_items), UI_PAGE_MAIN},
    [UI_PAGE_OPTION_4] = {"Option 4",  s_option4_items, UI_ITEM_COUNT(s_option4_items), UI_PAGE_MAIN},
};

/** 1-bit 帧缓冲，所有绘图操作先写入此缓冲，再刷到屏幕。 */
static uint8_t s_fb[UI_FB_SIZE];

/** 当前选中的菜单索引（0 ~ 菜单项数-1） */
static uint8 s_page_index = UI_PAGE_MAIN;
static uint8 s_menu_index = 0;

/** 底部状态栏文本 */
static char s_status[32] = "READY";

/**
 * 选择框动画状态
 * - s_selector_y：当前选择框的垂直位置（实时变化）
 * - s_selector_target_y：目标位置（对应选中菜单项）
 * - s_selector_prev_y：上一次帧的位置，用于计算刷新包围盒
 */
static int16 s_selector_y = UI_ROW_Y0;
static int16 s_selector_target_y = UI_ROW_Y0;
static int16 s_selector_prev_y = UI_ROW_Y0;

/** 动画是否进行中（选择框尚未到达目标位置） */
static uint8 s_anim_active = 0;

/** 全局脏标志：需要重新渲染 1-bit 缓冲 */
static uint8 s_dirty = 1;

/** 全屏刷新标志：首次或强制重绘时置位，刷新所有分区 */
static uint8 s_full_flush = 1;

/** 状态栏脏标志：仅刷新底部状态栏区域 */
static uint8 s_footer_dirty = 1;

/*--------------------- 单键手势识别状态机 ---------------------*/
/**
 * 状态机原理：
 * - 第一次释放后，启动 350ms 超时等待；若超时无新动作，触发单击。
 * - 若在 350ms 内再次按下，标记第二次按下，并等待释放或长按。
 * - 若第二次释放发生在第一次释放后 350ms 内，触发双击。
 * - 若第二次按下后持续按住超过长按阈值，触发组合键（click+hold）。
 */
static uint8 s_key_prev_pressed = 0;   /**< 上一次按键电平状态 */
static uint8 s_wait_single = 0;        /**< 处于单击等待窗口（等待第二次按下或超时） */
static uint8 s_second_press = 0;       /**< 第二次按下已发生标志 */
static uint32 s_first_release_ms = 0;  /**< 第一次释放时刻（系统 ms） */
static uint32 s_second_press_ms = 0;   /**< 第二次按下时刻（系统 ms） */

/*===========================================================================
 * 辅助函数（坐标计算、1-bit 绘图原语）
 *=========================================================================*/

/**
 * @brief 计算第 index 个菜单项方框的顶部 Y 坐标
 * @param index 菜单项索引
 * @return Y 坐标（像素）
 */
static int16 ui_row_y(uint8 index)
{
    return (int16)(UI_ROW_Y0 + (int16)index * UI_ROW_H);
}

static const ui_page_t *ui_current_page(void)
{
    return &s_pages[s_page_index];
}

static const ui_menu_item_t *ui_current_item(uint8 index)
{
    const ui_page_t *page = ui_current_page();

    if (index >= page->item_count) index = 0;
    return &page->items[index];
}

/**
 * @brief 计算字符串在 8x16 字体下的像素宽度
 * @param text 字符串
 * @return 宽度（像素）
 */
static int16 ui_text_w(const char *text)
{
    return (int16)strlen(text) * 8;
}

/**
 * @brief 计算第 index 个菜单项方框的自适应宽度
 * @param index 菜单项索引
 * @return 宽度（像素），会约束在 [112, UI_ROW_W] 之间
 */
static int16 ui_row_box_w(uint8 index)
{
    int16 w = (int16)(ui_text_w(ui_current_item(index)->text) + UI_TEXT_PAD_X * 2);
    if (w < 112) w = 112;            /* 最小宽度，避免文字拥挤 */
    if (w > UI_ROW_W) w = UI_ROW_W;  /* 最大宽度，不超出屏幕 */
    return w;
}

/*------------------------- 1-bit 绘图函数 -------------------------*/
/**
 * @brief 清空整个帧缓冲
 * @param color 0=清为全0（背景），1=清为全1（前景）
 */
static void fb_clear(uint8 color)
{
    memset(s_fb, color ? 0xFF : 0x00, sizeof(s_fb));
}

/**
 * @brief 在帧缓冲中绘制一个像素
 * @param x, y 坐标（支持越界，内部裁剪）
 * @param color 0/1
 */
static void fb_pixel(int16 x, int16 y, uint8 color)
{
    uint32 index;
    uint8 mask;

    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
    index = (uint32)y * UI_FB_STRIDE + ((uint16)x >> 3);
    mask = (uint8)(0x80 >> ((uint16)x & 7));
    if (color) s_fb[index] |= mask;
    else s_fb[index] &= (uint8)~mask;
}

/**
 * @brief 绘制填充矩形
 * @param x, y 左上角坐标
 * @param w, h 宽度、高度
 * @param color 0/1
 */
static void fb_box(int16 x, int16 y, int16 w, int16 h, uint8 color)
{
    for (int16 yy = 0; yy < h; yy++)
        for (int16 xx = 0; xx < w; xx++)
            fb_pixel(x + xx, y + yy, color);
}

/**
 * @brief 绘制水平线
 */
static void fb_hline(int16 x, int16 y, int16 w, uint8 color)
{
    fb_box(x, y, w, 1, color);
}

/**
 * @brief 绘制垂直线
 */
static void fb_vline(int16 x, int16 y, int16 h, uint8 color)
{
    fb_box(x, y, 1, h, color);
}

/**
 * @brief 绘制矩形边框（不填充）
 */
static void fb_frame(int16 x, int16 y, int16 w, int16 h, uint8 color)
{
    fb_hline(x, y, w, color);
    fb_hline(x, y + h - 1, w, color);
    fb_vline(x, y, h, color);
    fb_vline(x + w - 1, y, h, color);
}

/**
 * @brief 绘制圆角矩形边框（不填充）
 * @note 半径较小时会自动退化为普通矩形，便于小屏幕压缩布局。
 */
static void fb_rframe(int16 x, int16 y, int16 w, int16 h, int16 r, uint8 color)
{
    int16 r_outer;
    int16 r_inner;

    if (w <= 0 || h <= 0) return;
    if (r <= 1 || w < 2 * r + 2 || h < 2 * r + 2)
    {
        fb_frame(x, y, w, h, color);
        return;
    }

    fb_hline(x + r, y, w - 2 * r, color);
    fb_hline(x + r, y + h - 1, w - 2 * r, color);
    fb_vline(x, y + r, h - 2 * r, color);
    fb_vline(x + w - 1, y + r, h - 2 * r, color);

    r_outer = r * r;
    r_inner = (r - 1) * (r - 1);
    for (int16 py = 0; py <= r; py++)
    {
        for (int16 px = 0; px <= r; px++)
        {
            int16 dx = px - r;
            int16 dy = py - r;
            int16 d = dx * dx + dy * dy;

            if (d >= r_inner && d <= r_outer)
            {
                fb_pixel(x + px, y + py, color);
                fb_pixel(x + w - 1 - px, y + py, color);
                fb_pixel(x + px, y + h - 1 - py, color);
                fb_pixel(x + w - 1 - px, y + h - 1 - py, color);
            }
        }
    }
}

/**
 * @brief 在指定位置绘制一个字符（8x16 字体）
 * @param x, y 左上角坐标
 * @param ch 字符
 * @param color 0/1
 */
static void fb_char(int16 x, int16 y, char ch, uint8 color)
{
    const st7789_font_t *font = &font_8x16;
    const uint8_t *glyph;

    if ((uint8_t)ch < font->first || (uint8_t)ch > font->last) ch = '?';
    glyph = font->bitmap + ((uint8_t)ch - font->first) * font->char_h;

    for (uint8_t row = 0; row < font->char_h; row++)
    {
        uint8_t line = glyph[row];
        for (uint8_t col = 0; col < font->char_w; col++)
        {
            if (line & (uint8_t)(0x80 >> col))
                fb_pixel(x + col, y + row, color);
        }
    }
}

/**
 * @brief 绘制字符串
 */
static void fb_str(int16 x, int16 y, const char *str, uint8 color)
{
    while (*str)
    {
        fb_char(x, y, *str++, color);
        x += 8;
    }
}

/*===========================================================================
 * UI 状态管理及动作处理
 *=========================================================================*/

/**
 * @brief 更新状态栏文本并标记脏
 * @param text 新文本
 */
static void ui_set_status(const char *text)
{
    snprintf(s_status, sizeof(s_status), "%s", text);
    s_footer_dirty = 1;
    s_dirty = 1;
}

/**
 * @brief 启动选择框滑动动画
 * @param old_index 旧菜单索引（动画起点）
 * @note 动画从旧行位置滑到当前选中行位置，由 ui_step_animation() 逐帧驱动。
 */
static void ui_start_selector_anim(uint8 old_index)
{
    s_selector_prev_y = s_selector_y;
    if (old_index == s_menu_index)
    {
        /* 目标与当前相同，无需动画 */
        s_selector_y = ui_row_y(s_menu_index);
        s_selector_target_y = s_selector_y;
        s_anim_active = 0;
    }
    else
    {
        s_selector_y = ui_row_y(old_index);
        s_selector_target_y = ui_row_y(s_menu_index);
        s_anim_active = 1;
    }
    s_dirty = 1;
}

static void ui_enter_page(uint8 page_index, const char *status)
{
    if (page_index >= UI_PAGE_COUNT) return;

    s_page_index = page_index;
    s_menu_index = 0;
    s_selector_y = ui_row_y(0);
    s_selector_target_y = s_selector_y;
    s_selector_prev_y = s_selector_y;
    s_anim_active = 0;
    s_full_flush = 1;
    ui_set_status(status);
}

/**
 * @brief 执行按键动作对应的 UI 操作
 * @param action 识别出的动作类型
 */
static void ui_do_action(ui_action_enum action)
{
    char buf[32];
    uint8 old = s_menu_index;
    const ui_page_t *page = ui_current_page();
    const ui_menu_item_t *item = ui_current_item(s_menu_index);

    switch (action)
    {
        case UI_ACTION_SINGLE:
            /* 单击：循环移动到下一项 */
            s_menu_index = (uint8)((s_menu_index + 1) % page->item_count);
            ui_start_selector_anim(old);
            snprintf(buf, sizeof(buf), "SELECT %s", ui_current_item(s_menu_index)->text);
            ui_set_status(buf);
            break;

        case UI_ACTION_DOUBLE:
            /* 双击：进入选项配置的下一页；若无跳转页，则只更新状态栏。 */
            if (item->next_page != UI_PAGE_NONE)
            {
                ui_enter_page(item->next_page, item->status);
            }
            else
            {
                ui_set_status(item->status);
            }
            break;

        case UI_ACTION_LONG:
            /* 长按：返回当前页面配置的 back_page。 */
            ui_enter_page(page->back_page, "BACK");
            break;

        case UI_ACTION_COMBO:
            /* 组合键：快速回主菜单。 */
            ui_enter_page(UI_PAGE_MAIN, "MAIN");
            break;
    }
}

/*===========================================================================
 * 渲染与刷新
 *=========================================================================*/

/**
 * @brief 动画步进函数，计算选择框下一帧位置
 * @note 采用指数逼近算法：距离越远步进越大，接近目标后自然减速，实现平滑效果。
 */
static void ui_step_animation(void)
{
    int16 diff;

    if (!s_anim_active) return;

    s_selector_prev_y = s_selector_y;
    diff = s_selector_target_y - s_selector_y;

    if (diff >= -1 && diff <= 1)
    {
        /* 到达目标，停止动画 */
        s_selector_y = s_selector_target_y;
        s_anim_active = 0;
    }
    else if (diff > 0)
    {
        /* 向下移动，步长为 diff/3 + 1，保证至少移动 1 像素 */
        s_selector_y += (diff / 3) + 1;
    }
    else
    {
        /* 向上移动 */
        s_selector_y -= ((-diff) / 3) + 1;
    }
    s_dirty = 1;
}

/**
 * @brief 绘制一个普通菜单项（未选中状态）
 * @param index 菜单项索引
 */
static void ui_draw_row(uint8 index)
{
    int16 y = ui_row_y(index);
    int16 w = ui_row_box_w(index);

    fb_rframe(UI_ROW_X, y, w, UI_ROW_BOX_H, UI_RADIUS, 1);
    fb_str(UI_ROW_X + UI_TEXT_PAD_X, y + 2, ui_current_item(index)->text, 1);
}

/**
 * @brief 绘制右侧页面滑条
 * @note 当前菜单项越靠后，滑块越靠下；选项少于 2 个时滑块固定在顶部。
 */
static void ui_draw_scrollbar(void)
{
    const ui_page_t *page = ui_current_page();
    int16 thumb_y = UI_SCROLL_Y;
    int16 travel = UI_SCROLL_H - UI_SCROLL_THUMB_H;

    if (travel < 0) travel = 0;
    if (page->item_count > 1)
    {
        thumb_y = (int16)(UI_SCROLL_Y + ((int32)travel * s_menu_index) / (page->item_count - 1));
    }

    fb_rframe(UI_SCROLL_X, UI_SCROLL_Y, UI_SCROLL_W, UI_SCROLL_H, 2, 1);
    fb_rframe(UI_SCROLL_X - 1, thumb_y, UI_SCROLL_W + 2, UI_SCROLL_THUMB_H, 2, 1);
}

/**
 * @brief 重建整个 1-bit 帧缓冲（包含标题、状态栏、所有菜单项和选择框）
 * @note 此函数总是全量绘制，但后续可通过 ui_flush() 仅刷新变化区域。
 */
static void ui_render(void)
{
    int16 sy = s_selector_y;
    const ui_page_t *page = ui_current_page();

    /* 清空缓冲 */
    fb_clear(0);

    /* 绘制外边框 */
    fb_frame(0, 0, OLED_WIDTH, OLED_HEIGHT, 1);

    /* 标题栏 */
    fb_str(8, 1, page->title, 1);
    fb_hline(0, UI_HEADER_H - 1, OLED_WIDTH, 1);

    /* 状态栏分隔线 */
    fb_hline(0, UI_FOOTER_Y, OLED_WIDTH, 1);
    fb_str(8, UI_FOOTER_Y + 2, s_status, 1);

    /* 绘制所有菜单项（普通样式） */
    for (uint8 i = 0; i < page->item_count; i++)
        ui_draw_row(i);
    ui_draw_scrollbar();

    /* 绘制选中框（双层圆角方框）位于当前动画位置 sy */
    if (sy < UI_MENU_Y) sy = UI_MENU_Y;
    if (sy + UI_ROW_BOX_H > UI_FOOTER_Y) sy = UI_FOOTER_Y - UI_ROW_BOX_H;
    fb_rframe(UI_ROW_X - 2, sy - 2, (int16)(ui_row_box_w(s_menu_index) + 4), (int16)(UI_ROW_BOX_H + 4), UI_RADIUS, 1);
    fb_rframe(UI_ROW_X, sy, ui_row_box_w(s_menu_index), UI_ROW_BOX_H, UI_RADIUS, 1);
}

/*------------------------- 屏幕刷新（blit）-------------------------*/

/**
 * @brief 将帧缓冲的矩形区域刷到物理屏幕，使用指定的前景色和背景色
 * @param y     起始行（纵向）
 * @param h     高度
 * @param fg    前景色（RGB565），对应于 mask 中为 1 的像素
 * @note 背景色固定为 UI_COLOR_BG (黑色)，mask 为 0 的像素显示背景色。
 */
static void ui_blit(uint16 y, uint16 h, uint16 fg)
{
    st7789_blit_mono(0, y, OLED_WIDTH, h, s_fb, UI_FB_STRIDE, fg, UI_COLOR_BG);
}

/**
 * @brief 专门刷新选中框所在的纵向区域（使用黄色高亮）
 * @note 自动裁剪到菜单区域边界，避免覆盖标题/状态栏。
 */
static void ui_blit_selector(void)
{
    int16 y = s_selector_y - 2;
    int16 h = UI_ROW_BOX_H + 4;

    if (y < UI_MENU_Y)
    {
        h -= (UI_MENU_Y - y);
        y = UI_MENU_Y;
    }
    if (y + h > UI_FOOTER_Y) h = UI_FOOTER_Y - y;
    if (h > 0)
    {
        st7789_blit_mono(0, (uint16)y, OLED_WIDTH, (uint16)h,
                         s_fb, UI_FB_STRIDE, UI_COLOR_SELECT, UI_COLOR_BG);
    }
}

/**
 * @brief 将 s_fb 中的内容按脏区刷新到屏幕
 * @note 支持全屏刷新和局部（动画包围盒）刷新，以及状态栏单独刷新。
 */
static void ui_flush(void)
{
    int16 top;
    int16 bottom;

    if (s_full_flush)
    {
        /* 首次刷新：分区使用不同前景色 */
        ui_blit(0, UI_HEADER_H, UI_COLOR_TITLE);                          /* 标题栏 */
        ui_blit(UI_HEADER_H, (uint16)(UI_FOOTER_Y - UI_HEADER_H), UI_COLOR_TEXT); /* 菜单区 */
        ui_blit_selector();                                               /* 选中框 */
        ui_blit(UI_FOOTER_Y, UI_FOOTER_H, UI_COLOR_STATUS);               /* 状态栏 */
        s_full_flush = 0;
        s_footer_dirty = 0;
        s_selector_prev_y = s_selector_y;
        return;
    }

    /* 局部刷新：计算旧选择框与新选择框的包围盒（并集） */
    top = s_selector_prev_y;
    bottom = s_selector_y;
    if (bottom < top)
    {
        int16 temp = top;
        top = bottom;
        bottom = temp;
    }
    top -= 2;
    bottom += UI_ROW_BOX_H + 2;
    if (top < UI_MENU_Y) top = UI_MENU_Y;
    if (bottom > UI_FOOTER_Y) bottom = UI_FOOTER_Y;

    if (bottom > top)
    {
        /* 刷新菜单区背景（普通文字颜色） */
        ui_blit((uint16)top, (uint16)(bottom - top), UI_COLOR_TEXT);
        /* 刷新选中框（高亮覆盖） */
        ui_blit_selector();
    }

    /* 若状态栏脏，单独刷新 */
    if (s_footer_dirty)
    {
        ui_blit(UI_FOOTER_Y, UI_FOOTER_H, UI_COLOR_STATUS);
        s_footer_dirty = 0;
    }

    s_selector_prev_y = s_selector_y;
}

/*===========================================================================
 * 公共 API（初始化、按键处理、任务循环）
 *=========================================================================*/

/**
 * @brief UI 模块初始化
 * @note 初始化显示驱动、重置所有状态、执行首次全屏渲染。
 */
void app_ui_init(void)
{
    astra_ui_driver_init();
    s_page_index = UI_PAGE_MAIN;
    s_menu_index = 0;
    snprintf(s_status, sizeof(s_status), "READY");
    s_selector_y = ui_row_y(s_menu_index);
    s_selector_target_y = s_selector_y;
    s_selector_prev_y = s_selector_y;
    s_anim_active = 0;
    s_full_flush = 1;
    s_footer_dirty = 1;
    s_dirty = 1;
    ui_render();
    ui_flush();
    s_dirty = 0;
}

/**
 * @brief 按键事件处理函数，实现单击/双击/长按/组合键识别
 * @param pressed 当前物理按键电平（1=按下，0=释放）
 * @param event   底层驱动提供的事件类型（KEY_EVENT_PRESS 或 KEY_EVENT_LONG_PRESS）
 * @param now_ms  当前系统时间戳（毫秒）
 * @note 状态机时序：
 *       - 第一次释放后，启动 350ms 超时等待。
 *       - 若在此期间再次按下，记录第二次按下时刻，并等待释放或长按。
 *       - 若第二次释放发生在 350ms 内，触发双击。
 *       - 若第二次按下后持续按住超过长按阈值，触发组合键。
 *       - 若超时无操作，触发单击。
 */
void app_ui_handle_key(uint8 pressed, bsp_key_event_enum event, uint32 now_ms)
{
    /* 检测到新的按下（边沿上升） */
    if (pressed && !s_key_prev_pressed)
    {
        if (s_wait_single)
        {
            /* 在等待窗口内再次按下，标记第二次按下 */
            s_second_press = 1;
            s_second_press_ms = now_ms;
        }
    }

    /* 处理按键事件 */
    if (event == KEY_EVENT_PRESS)
    {
        /* 按键释放事件（对应第一次释放） */
        if (s_wait_single && (now_ms - s_first_release_ms <= 350))
        {
            /* 在 350ms 内再次释放 → 双击 */
            ui_do_action(UI_ACTION_DOUBLE);
            s_wait_single = 0;
            s_second_press = 0;
        }
        else
        {
            /* 第一次释放，进入等待状态 */
            s_wait_single = 1;
            s_second_press = 0;
            s_first_release_ms = now_ms;
        }
    }
    else if (event == KEY_EVENT_LONG_PRESS)
    {
        /* 长按事件触发 */
        if (s_wait_single && s_second_press && (s_second_press_ms - s_first_release_ms <= 400))
        {
            /* 第二次按下后长按 → 组合键 */
            ui_do_action(UI_ACTION_COMBO);
        }
        else
        {
            /* 普通长按 */
            ui_do_action(UI_ACTION_LONG);
        }
        s_wait_single = 0;
        s_second_press = 0;
    }

    /* 若处于等待状态且按键已释放，检测超时（>350ms）触发单击 */
    if (s_wait_single && !pressed && (now_ms - s_first_release_ms > 350))
    {
        ui_do_action(UI_ACTION_SINGLE);
        s_wait_single = 0;
        s_second_press = 0;
    }

    s_key_prev_pressed = pressed;
}

/**
 * @brief UI 任务主循环，由外部定时器（约 10ms）周期性调用
 * @note 执行动画步进，若有脏区则重新渲染并刷新屏幕。
 *       无脏区时立即返回，以节省 CPU 和 SPI 带宽。
 */
void app_ui_task(void)
{
    ui_step_animation();
    if (!s_dirty) return;

    ui_render();
    ui_flush();
    s_dirty = 0;
}
