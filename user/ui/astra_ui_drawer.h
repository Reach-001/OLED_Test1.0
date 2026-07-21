/**
 * @file    astra_ui_drawer.h
 * @brief   Astra UI 渲染层 — 列表/选择器/控件绘制声明
 */

#ifndef ASTRA_UI_DRAWER_H
#define ASTRA_UI_DRAWER_H

#include "astra_ui_item.h"

/** @brief 退场动画状态: 0=空闲, 1=遮罩落下完成, 2=遮罩抬升中 */
extern uint8_t astra_exit_animation_status;

/** @brief 退场动画 (沙漏图标 + 遮罩) */
extern void astra_draw_exit_animation();

/** @brief 信息栏 — 前景层 */
extern void astra_draw_info_bar();

/** @brief 弹窗 — 前景层 */
extern void astra_draw_pop_up();

/** @brief 列表外观 (状态栏/进度条) — 背景层 */
extern void astra_draw_list_appearance();

/** @brief 列表项 — 背景层 */
extern void astra_draw_list_item();

/** @brief 列表项图标 */
extern void astra_draw_list_icon(astra_list_item_icon_t icon, uint16_t x, uint16_t y);

/** @brief 选择器高亮 — 背景层 */
extern void astra_draw_selector();

/** @brief Widget 总控 — 前景层 */
extern void astra_draw_widget();

/** @brief 列表总控 — 背景层 */
extern void astra_draw_list();

#endif /* ASTRA_UI_DRAWER_H */
