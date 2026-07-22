/**
 * @file    app_ui_style_config.h
 * @brief   Astra UI 外观参数配置文件
 * 
 * 本文件定义了 Astra UI 界面中所有可调整的外观参数，包括颜色、尺寸、启用开关等。
 * 开发者可通过修改这些宏来定制 UI 风格，而无需改动核心绘制代码。
 * 
 * 主界面仍使用 1-bit 帧缓冲，非黑色编号在缓冲层会作为前景色处理。
 * 标题线、滚动条强调色、弹窗强调框等少量装饰在发送缓冲后直写 RGB565。
 */

#ifndef _APP_UI_STYLE_CONFIG_H_
#define _APP_UI_STYLE_CONFIG_H_

/*===========================================================================
 * 颜色系统 — 加新颜色：先加 UI_COLOR_xxx 编号，再加 UI_RGB565_xxx 色值
 *   编号和色值必须一一对应，末尾补一行后 UI_COLOR_COUNT +1
 *=========================================================================*/

/* 颜色编号（驱动层用编号查 RGB565 表） */
#define UI_COLOR_BLACK  0   /**< 黑色 — 背景、暗色 */
#define UI_COLOR_WHITE  1   /**< 白色 — 前景、高亮 */
#define UI_COLOR_GRAY   2   /**< 灰色 — 中性、次级文字 */
#define UI_COLOR_SKY    3   /**< 天蓝色 — 醒目、信息 */
#define UI_COLOR_MINT   4   /**< 薄荷绿 — 清新、确认 */
#define UI_COLOR_AMBER  5   /**< 琥珀色 — 警告、滑块 */
#define UI_COLOR_ROSE   6   /**< 玫瑰红 — 强调、按钮 */
#define UI_COLOR_PINK   7   /**< 粉色 — 棋盘格装饰 */
#define UI_COLOR_COUNT  8   /**< 颜色总数，加颜色后 +1 */

/* RGB565 色值（加颜色后在此补一行，并在 draw_driver.c 色表中末尾追加） */
#define UI_RGB565_BLACK  0x0000  /**< 纯黑 */
#define UI_RGB565_WHITE  0xFFFF  /**< 纯白 */
#define UI_RGB565_GRAY   0x8410  /**< 中性灰（R4 G8 B4） */
#define UI_RGB565_SKY    0x06FF  /**< 高饱和蓝青 */
#define UI_RGB565_MINT   0x07F0  /**< 高饱和薄荷绿 */
#define UI_RGB565_AMBER  0xFDE0  /**< 明亮琥珀黄 */
#define UI_RGB565_ROSE   0xF81F  /**< 高饱和玫瑰红 */
#define UI_RGB565_PINK   0xED14  /**< 柔和粉色 */

/*===========================================================================
 * 标题栏配置
 *=========================================================================*/

#define UI_TITLE_ENABLE            1   /**< 标题栏启用标志（1=启用，0=禁用） */
#define UI_TITLE_TEXT_COLOR        3  /**< 标题文字颜色 */
#define UI_TITLE_LINE_COLOR        4   /**< 标题栏底部横线颜色 */
#define UI_TITLE_AREA_HEIGHT       22               /**< 标题栏占用高度，文字16px+线1px+上下各1px间距=22 */
#define UI_TITLE_BASELINE_Y        16               /**< 标题文字基线纵坐标（相对于屏幕顶部） */

/*===========================================================================
 * 列表布局、文字与滚动条配置
 *=========================================================================*/

#define UI_LIST_TEXT_COLOR         UI_COLOR_WHITE   /**< 列表项文字颜色 */
#define UI_LIST_TOP_MARGIN         26               /**< 首个列表项基线前的顶部边距 (留出选择框空间) */
#define UI_LIST_ITEM_SPACING_147   24               /**< 320x172 屏列表项间距 */
#define UI_LIST_ITEM_SPACING_114   18               /**< 240x135 屏列表项间距 */
#define UI_LIST_ITEM_LEFT_MARGIN   8                /**< 列表项左边距 */
#define UI_LIST_ITEM_RIGHT_MARGIN  42               /**< 列表项右侧控件预留宽度 */
#define UI_LIST_MAX_CHILD_NUM      16               /**< 单个列表最多子项数 */
#define UI_SCROLLBAR_COLOR         UI_COLOR_WHITE   /**< 滚动条基本颜色（轨道） */
#define UI_SCROLLBAR_ACCENT_ENABLE 1                /**< 滚动条强调色启用标志（1=启用） */
#define UI_SCROLLBAR_ACCENT_COLOR  3    /**< 滚动条滑块/强调色 */

/*===========================================================================
 * 选择框（高亮指示器）配置
 *=========================================================================*/

#define UI_SELECTOR_RADIUS         4                /**< 选择框圆角半径（像素） */
#define UI_SELECTOR_HEIGHT         18               /**< 选择框高度 */
#define UI_SELECTOR_TEXT_PADDING   20               /**< 普通选项选择框水平内边距 */
#define UI_SELECTOR_FULL_MARGIN    32               /**< 开关/滑块选择框左右总预留 */
#define UI_SELECTOR_FRAME_COLOR    UI_COLOR_MINT    /**< 选择框边框颜色 (RGB565直写, 薄荷绿醒目) */
#define UI_SELECTOR_FILL_ENABLE    1                /**< 选择框棋盘格填充 (1=图案灰底, 0=仅线框) */

//选中框右侧棋盘设置
#define UI_SELECTOR_CHESS_COLOR     7               /**< 棋盘格颜色 */
#define UI_SELECTOR_CHESS_WIDTH     5               /**< 棋盘格宽度 (px) */
#define UI_SELECTOR_CHESS_STEP      1                /**< 棋盘格列步进 (1=密, 2=默认, 3=疏) */

/*===========================================================================
 * 弹出提示框（信息提示 / 确认框）配置
 *=========================================================================*/

#define UI_INFO_SHADOW_COLOR      UI_COLOR_WHITE   /**< 信息提示阴影颜色（通常为半透白） */
#define UI_INFO_BG_COLOR          UI_COLOR_BLACK   /**< 信息提示背景色 */
#define UI_INFO_BOX_COLOR         UI_COLOR_WHITE   /**< 信息提示边框颜色 */
#define UI_INFO_TEXT_COLOR        UI_COLOR_BLACK   /**< 信息提示文字颜色（白底框上用黑字） */
#define UI_INFO_ACCENT_COLOR       UI_COLOR_AMBER  /**< 信息提示强调色（如标题线或按钮） */

#define UI_POPUP_SHADOW_COLOR     UI_COLOR_WHITE   /**< 弹出框阴影颜色 */
#define UI_POPUP_BG_COLOR         UI_COLOR_BLACK   /**< 弹出框背景色 */
#define UI_POPUP_BOX_COLOR        UI_COLOR_WHITE   /**< 弹出框边框颜色 */
#define UI_POPUP_TEXT_COLOR       UI_COLOR_BLACK   /**< 弹出框文字颜色（白底框上用黑字） */
#define UI_POPUP_ACCENT_COLOR      UI_COLOR_SKY    /**< 弹出框强调色 */

/*===========================================================================
 * 滑块确认态数值标签（拖动滑块时显示的数值）配置
 *=========================================================================*/

#define UI_SLIDER_VALUE_BOX_COLOR   UI_COLOR_WHITE /**< 数值标签背景/边框色 */
#define UI_SLIDER_VALUE_TEXT_COLOR  UI_COLOR_BLACK /**< 数值标签文字颜色（通常与背景对比明显） */

#endif /* _APP_UI_STYLE_CONFIG_H_ */
