/**
 * @file    astra_ui_item.h
 * @brief   Astra UI 数据模型层 — 列表项/选择器/相机
 * @note    适配 ST7789 320×172 横屏, 16×16 字体
 */

#ifndef ASTRA_UI_ITEM_H
#define ASTRA_UI_ITEM_H

#include "astra_ui_draw_driver.h"
#include "app_ui_style_config.h"
#include <stdint.h>
#include <stdbool.h>

extern void astra_set_font(const void* _font);

extern bool astra_exit_animation_finished;
extern bool astra_refresh_list_value;

/*** 信息栏 ***/
#define INFO_BAR_HEIGHT 20       /**< 信息栏高度 (原 15)               */
#define INFO_BAR_OFFSET 20       /**< 信息栏水平内边距 (原 10)        */

typedef struct astra_info_bar_t
{
  char *content;
  uint16_t span;
  float y_info_bar, y_info_bar_trg, w_info_bar, w_info_bar_trg;
  bool is_running;
  uint32_t time_start;
  uint32_t time;
} astra_info_bar_t;

extern astra_info_bar_t astra_info_bar;
extern void astra_push_info_bar(char *_content, const uint16_t _span);

/*** 弹窗 ***/
#define POP_UP_HEIGHT 28         /**< 弹窗高度 (原 20)                 */
#define POP_UP_OFFSET 16         /**< 弹窗水平内边距 (原 8)           */

typedef struct astra_pop_up_t
{
  char *content;
  uint16_t span;
  float y_pop_up, y_pop_up_trg, w_pop_up, w_pop_up_trg;
  bool is_running;
  uint32_t time_start;
  uint32_t time;
} astra_pop_up_t;

extern astra_pop_up_t astra_pop_up;
extern void astra_push_pop_up(char *_content, const uint16_t _span);

/*** 列表项 — 跟随当前 ST7789 逻辑分辨率 ***/
#define MAX_LIST_CHILD_NUM UI_LIST_MAX_CHILD_NUM
#define MAX_LIST_LAYER 10
#define SCREEN_HEIGHT OLED_HEIGHT
#define SCREEN_WIDTH  OLED_WIDTH
#if OLED_WIDTH >= 300
#define LIST_ITEM_SPACING UI_LIST_ITEM_SPACING_147
#else
#define LIST_ITEM_SPACING UI_LIST_ITEM_SPACING_114
#endif
#define LIST_ITEM_OFFSET 12      /**< 列表项偏移 (原 8)                */
#define LIST_ITEM_LEFT_MARGIN UI_LIST_ITEM_LEFT_MARGIN
#define LIST_ITEM_RIGHT_MARGIN UI_LIST_ITEM_RIGHT_MARGIN
#if UI_TITLE_ENABLE
#define LIST_INFO_BAR_HEIGHT UI_TITLE_AREA_HEIGHT
#else
#define LIST_INFO_BAR_HEIGHT 0
#endif
#define LIST_FONT_TOP_MARGIN UI_LIST_TOP_MARGIN

typedef enum
{
  list_item,
  switch_item,
  slider_item,
  user_item,
  button_item,
} astra_list_item_type_t;

typedef enum {
    default_icon,
    list_icon,
    switch_icon,
    plus_icon,
    user_icon,
    slider_icon,
    flag_icon,
    power_icon,
} astra_list_item_icon_t;

typedef struct astra_list_item_t
{
  astra_list_item_type_t type;
  astra_list_item_icon_t icon;
  char *content;

  uint8_t layer;
  float y_list_item, y_list_item_trg;
  uint8_t child_num;
  struct astra_list_item_t *child_list_item[MAX_LIST_CHILD_NUM];
  struct astra_list_item_t *parent;
} astra_list_item_t;

typedef struct astra_switch_item_t
{
  astra_list_item_t base_item;
  bool *value;
  void (*init_function)();
  void (*exit_function)();
} astra_switch_item_t;

typedef struct astra_button_item_t
{
  astra_list_item_t base_item;
  void (*exit_function)();
} astra_button_item_t;

typedef struct astra_slider_item_t
{
  astra_list_item_t base_item;
  int16_t *value;
  int16_t value_backup;
  bool is_confirmed;
  uint8_t value_step;
  int16_t value_max;
  int16_t value_min;
  void (*init_function)();
  void (*exit_function)();
} astra_slider_item_t;

typedef struct astra_user_item_t
{
  astra_list_item_t base_item;
  bool in_user_item;
  bool entering_user_item;
  bool exiting_user_item;
  void (*init_function)();
  void (*loop_function)();
  void (*exit_function)();
  bool user_item_inited;
  bool user_item_looping;
} astra_user_item_t;

/* --- 列表项 API --- */
extern astra_list_item_t *astra_get_root_list();

extern astra_switch_item_t *astra_to_switch_item(astra_list_item_t *_astra_list_item);
extern astra_button_item_t *astra_to_button_item(astra_list_item_t *_astra_list_item);
extern astra_slider_item_t *astra_to_slider_item(astra_list_item_t *_astra_list_item);
extern astra_user_item_t *astra_to_user_item(astra_list_item_t *_astra_list_item);
extern astra_list_item_t *astra_new_list_item(char *_content, astra_list_item_icon_t icon);
extern astra_list_item_t *astra_new_switch_item(char *_content, bool *_value, void (*_init_function)(), void (*_exit_function)(), astra_list_item_icon_t icon);
extern astra_list_item_t *astra_new_button_item(char *_content, void (*_exit_function)(), astra_list_item_icon_t icon);
extern astra_list_item_t *astra_new_slider_item(char *_content, int16_t *_value, uint8_t _step, int16_t _min, int16_t _max, void (*_init_function)(), void (*_exit_function)(), astra_list_item_icon_t icon);
extern astra_list_item_t *astra_new_user_item(char *_content, void (*_init_function)(), void (*_loop_function)(), void (*_exit_function)(), astra_list_item_icon_t icon);
extern bool astra_push_item_to_list(astra_list_item_t *_parent, astra_list_item_t *_child);

/*** 选择器 ***/
typedef struct astra_selector_t
{
  float y_selector, y_selector_trg, w_selector, w_selector_trg, h_selector, h_selector_trg;
  uint8_t selected_index;
  astra_list_item_t *selected_item;
} astra_selector_t;

extern astra_selector_t astra_selector;
extern astra_selector_t* astra_get_selector();
extern bool astra_bind_item_to_selector(astra_list_item_t *_item);
extern void astra_selector_go_next_item();
extern void astra_selector_go_prev_item();
extern void astra_selector_jump_to_selected_item();
extern void astra_selector_exit_current_item();

/*** 相机 ***/
typedef struct astra_camera_t
{
  float x_camera, x_camera_trg, y_camera, y_camera_trg;
  astra_selector_t *selector;
} astra_camera_t;

extern astra_camera_t astra_camera;
extern astra_camera_t* astra_get_camera();
extern void astra_bind_selector_to_camera(astra_selector_t *_selector);

#endif /* ASTRA_UI_ITEM_H */
