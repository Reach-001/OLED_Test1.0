/**
 * @file    astra_ui_core.h
 * @brief   Astra UI 核心逻辑 — 动画系统、状态刷新、主循环
 * @note    适配 ST7789 320×172 横屏
 */

#ifndef ASTRA_UI_CORE_H
#define ASTRA_UI_CORE_H
#include <stdbool.h>

/** @brief 允许用户在最浅层级手动退出 Astra UI */
#define ALLOW_EXIT_ASTRA_UI_BY_USER 1

/** @brief 全局标志: 是否处于 Astra UI 菜单中 */
extern bool in_astra;

/** @brief 进入 Astra UI 的触发检测 (需在循环中调, 适配硬件按键) */
extern void ad_astra();

extern bool astra_is_in_user_item();
extern void astra_refresh_info_bar();
extern void astra_refresh_pop_up();
extern void astra_refresh_camera_position();
extern void astra_refresh_widget_core_position();
extern void astra_init_list();
extern void astra_init_core();
extern void astra_refresh_list_item_position();
extern void astra_refresh_selector_position();
extern void astra_refresh_main_core_position();
extern void astra_ui_widget_core();
extern void astra_ui_main_core();

#endif /* ASTRA_UI_CORE_H */
