/**
 * @file    astra_ui_item.c
 * @brief   Astra UI 数据模型实现 — 列表项/选择器/相机管理
 * @note    适配 ST7789 320×172 横屏
 */

#include "astra_ui_item.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "astra_ui_core.h"

void astra_set_font(const void *_font)
{
  st7789_set_font(_font);
}

/* --- 信息栏 --- */
astra_info_bar_t astra_info_bar = {0, 1, 0 - 2 * INFO_BAR_HEIGHT, 0 - 2 * INFO_BAR_HEIGHT, 80, 80, false, 0, 1};

/* 信息栏实际可见区域高度 = INFO_BAR_HEIGHT + 4 (含上下边线/阴影)。
 * 这里把整块提示框放到屏幕中线。 */
#define INFO_BAR_TARGET_Y  ((OLED_HEIGHT - (INFO_BAR_HEIGHT + 4)) / 2 + 4)

void astra_push_info_bar(char *_content, const uint16_t _span)
{
  /* 同一次 get_ticks() 调用保证 time>=time_start，避免无符号减法下溢导致立即超时 */
  uint32_t _now = get_ticks();
  astra_info_bar.time = _now;
  astra_info_bar.content = _content;
  astra_info_bar.span = _span;
  /* 每次调用都重新开始动画，直接覆盖旧状态 */
  astra_info_bar.time_start = _now;
  astra_info_bar.y_info_bar_trg = INFO_BAR_TARGET_Y;
  astra_info_bar.is_running = true;

  st7789_set_font(astra_default_font);
  astra_info_bar.w_info_bar_trg = oled_get_UTF8_width(astra_info_bar.content) + INFO_BAR_OFFSET;
}

/* --- 弹窗 --- */
astra_pop_up_t astra_pop_up = {0, 1, POP_UP_INIT_Y, POP_UP_INIT_Y, 80, 80, false, 0, 1};

void astra_push_pop_up(char *_content, const uint16_t _span)
{
  /* 同一次 get_ticks() 调用保证 time>=time_start，避免无符号减法下溢导致立即超时 */
  uint32_t _now = get_ticks();
  astra_pop_up.time = _now;
  astra_pop_up.content = _content;
  astra_pop_up.span = _span;
  /* 每次调用都重新开始动画，直接覆盖旧状态 */
  astra_pop_up.time_start = _now;
  astra_pop_up.y_pop_up_trg = POP_UP_TARGET_Y;
  astra_pop_up.is_running = true;

  st7789_set_font(astra_default_font);
  astra_pop_up.w_pop_up_trg = oled_get_UTF8_width(astra_pop_up.content) + POP_UP_OFFSET;
}

/* --- 类型转换辅助 --- */
astra_switch_item_t *astra_to_switch_item(astra_list_item_t *_astra_list_item)
{
  if (_astra_list_item != NULL && _astra_list_item->type == switch_item)
    return (astra_switch_item_t*)_astra_list_item;
  return NULL;
}

astra_button_item_t *astra_to_button_item(astra_list_item_t *_astra_list_item)
{
  if (_astra_list_item != NULL && _astra_list_item->type == button_item)
    return (astra_button_item_t*)_astra_list_item;
  return NULL;
}

astra_slider_item_t *astra_to_slider_item(astra_list_item_t *_astra_list_item)
{
  if (_astra_list_item != NULL && _astra_list_item->type == slider_item)
    return (astra_slider_item_t*)_astra_list_item;
  return NULL;
}

astra_user_item_t *astra_to_user_item(astra_list_item_t *_astra_list_item)
{
  if (_astra_list_item != NULL && _astra_list_item->type == user_item)
    return (astra_user_item_t*)_astra_list_item;
  return NULL;
}

/* --- 根节点 --  */
astra_list_item_t *astra_get_root_list()
{
  static astra_list_item_t* _astra_list_root_item = NULL;
  if (_astra_list_root_item == NULL)
  {
    _astra_list_root_item = malloc(sizeof(astra_list_item_t));
    memset(_astra_list_root_item, 0, sizeof(astra_list_item_t));
    _astra_list_root_item->type = list_item;
    _astra_list_root_item->content = "root";
  }
  return _astra_list_root_item;
}

/* --- 列表项构造函数 --- */
astra_list_item_t *astra_new_list_item(char *_content, astra_list_item_icon_t icon)
{
  astra_list_item_t *_astra_list_item = malloc(sizeof(astra_list_item_t));
  memset(_astra_list_item, 0, sizeof(astra_list_item_t));
  _astra_list_item->type = list_item;
  _astra_list_item->content = _content;
  _astra_list_item->icon = (icon == default_icon) ? list_icon : icon;
  return _astra_list_item;
}

astra_list_item_t *astra_new_switch_item(char *_content, bool *_value, void (*_init_function)(), void (*_exit_function)(), astra_list_item_icon_t icon)
{
  astra_switch_item_t *_astra_switch_item = malloc(sizeof(astra_switch_item_t));
  memset(_astra_switch_item, 0, sizeof(astra_switch_item_t));
  _astra_switch_item->base_item.type = switch_item;
  _astra_switch_item->base_item.content = _content;
  _astra_switch_item->value = _value;
  _astra_switch_item->init_function = _init_function;
  _astra_switch_item->exit_function = _exit_function;
  _astra_switch_item->base_item.icon = (icon == default_icon) ? switch_icon : icon;
  return (astra_list_item_t*)_astra_switch_item;
}

astra_list_item_t *astra_new_button_item(char *_content, void (*_exit_function)(), astra_list_item_icon_t icon)
{
  astra_button_item_t *_astra_button_item = malloc(sizeof(astra_button_item_t));
  memset(_astra_button_item, 0, sizeof(astra_button_item_t));
  _astra_button_item->base_item.type = button_item;
  _astra_button_item->base_item.content = _content;
  _astra_button_item->exit_function = _exit_function;
  _astra_button_item->base_item.icon = (icon == default_icon) ? plus_icon : icon;
  return (astra_list_item_t*)_astra_button_item;
}

astra_list_item_t *astra_new_slider_item(char *_content, int16_t *_value, uint8_t _step, int16_t _min, int16_t _max, void (*_init_function)(), void (*_exit_function)(), astra_list_item_icon_t icon)
{
  astra_slider_item_t *_astra_slider_item = malloc(sizeof(astra_slider_item_t));
  memset(_astra_slider_item, 0, sizeof(astra_slider_item_t));
  _astra_slider_item->base_item.type = slider_item;
  _astra_slider_item->base_item.content = _content;
  _astra_slider_item->value = _value;
  _astra_slider_item->value_step = _step;
  _astra_slider_item->value_min = _min;
  _astra_slider_item->value_max = _max;
  _astra_slider_item->init_function = _init_function;
  _astra_slider_item->exit_function = _exit_function;
  _astra_slider_item->base_item.icon = (icon == default_icon) ? slider_icon : icon;
  return (astra_list_item_t*)_astra_slider_item;
}

astra_list_item_t *astra_new_user_item(char *_content, void (*_init_function)(), void (*_loop_function)(), void (*_exit_function)(), astra_list_item_icon_t icon)
{
  astra_user_item_t *_astra_user_item = malloc(sizeof(astra_user_item_t));
  memset(_astra_user_item, 0, sizeof(astra_user_item_t));
  _astra_user_item->base_item.type = user_item;
  _astra_user_item->base_item.content = _content;
  _astra_user_item->init_function = _init_function;
  _astra_user_item->loop_function = _loop_function;
  _astra_user_item->exit_function = _exit_function;
  _astra_user_item->base_item.icon = (icon == default_icon) ? user_icon : icon;
  return (astra_list_item_t*)_astra_user_item;
}

/* --- 选择器 --- */
astra_selector_t astra_selector = {};

astra_selector_t *astra_get_selector()
{
  return &astra_selector;
}

bool astra_bind_item_to_selector(astra_list_item_t *_item)
{
  if (_item == NULL) return false;
  if (_item->parent == NULL) return false;

  uint8_t _temp_index = 0;
  for (uint8_t i = 0; i < _item->parent->child_num; i++)
  {
    if (_item->parent->child_list_item[i] == _item)
    {
      _temp_index = i;
      break;
    }
  }

  if (astra_selector.selected_item == NULL)
  {
    astra_selector.y_selector = 2 * SCREEN_HEIGHT;
    astra_selector.h_selector = 160;
  }
  astra_selector.selected_index = _temp_index;
  astra_selector.selected_item = _item;
  return true;
}

bool astra_refresh_list_value = true;

void astra_selector_go_next_item()
{
  if (astra_selector.selected_item == NULL) return;
  if (astra_selector.selected_item->type == slider_item && astra_to_slider_item(astra_selector.selected_item)->is_confirmed)
  {
    astra_slider_item_t* _selected_slider_item = astra_to_slider_item(astra_selector.selected_item);
    *_selected_slider_item->value += _selected_slider_item->value_step;
    if (*_selected_slider_item->value >= _selected_slider_item->value_max) *_selected_slider_item->value = _selected_slider_item->value_max;
    return;
  }
  if (astra_selector.selected_item->type == user_item && astra_to_user_item(astra_selector.selected_item)->in_user_item) return;

  astra_refresh_list_value = true;

  if (astra_selector.selected_index == astra_selector.selected_item->parent->child_num - 1)
  {
    astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[0];
    astra_selector.selected_index = 0;
    /* 回绕到列表首项时重置相机，防止旧偏移把顶部条目推出屏幕 */
    astra_camera.y_camera = 0;
    astra_camera.y_camera_trg = 0;
    return;
  }
  astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[++astra_selector.selected_index];
}

void astra_selector_go_prev_item()
{
  if (astra_selector.selected_item == NULL) return;
  if (astra_selector.selected_item->type == slider_item && astra_to_slider_item(astra_selector.selected_item)->is_confirmed)
  {
    astra_slider_item_t* _selected_slider_item = astra_to_slider_item(astra_selector.selected_item);
    *_selected_slider_item->value -= _selected_slider_item->value_step;
    if (*_selected_slider_item->value <= _selected_slider_item->value_min) *_selected_slider_item->value = _selected_slider_item->value_min;
    return;
  }
  if (astra_selector.selected_item->type == user_item && astra_to_user_item(astra_selector.selected_item)->in_user_item) return;

  astra_refresh_list_value = true;

  if (astra_selector.selected_index == 0)
  {
    astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[astra_selector.selected_item->parent->child_num - 1];
    astra_selector.selected_index = astra_selector.selected_item->parent->child_num - 1;
    /* 回绕到列表末项时重置相机，防止旧偏移把顶部条目推出屏幕 */
    astra_camera.y_camera = 0;
    astra_camera.y_camera_trg = 0;
    return;
  }
  astra_selector.selected_item = astra_selector.selected_item->parent->child_list_item[--astra_selector.selected_index];
}

bool astra_exit_animation_finished = true;

void astra_enter_child_item(astra_list_item_t *item)
{
  if (item == NULL || item->child_num == 0) return;

  astra_refresh_list_value = true;

  for (uint8_t i = 0; i < item->child_num; i++)
    item->child_list_item[i]->y_list_item = item->child_list_item[i]->y_list_item_trg;

  astra_selector.selected_index = 0;
  astra_selector.selected_item = item->child_list_item[0];
  /* 切换列表时重置相机偏移，防止旧偏移把顶部条目推到屏幕外 */
  astra_camera.y_camera = 0;
  astra_camera.y_camera_trg = 0;

  if (astra_selector.selected_item->type == user_item)
  {
    astra_exit_animation_finished = false;
    astra_user_item_t* _selected_user_item = astra_to_user_item(astra_selector.selected_item);
    _selected_user_item->entering_user_item = true;
    _selected_user_item->exiting_user_item = false;
    _selected_user_item->user_item_inited = false;
    _selected_user_item->user_item_looping = false;
  }
}

void astra_selector_jump_to_selected_item()
{
  if (!in_astra) return;
  if (astra_selector.selected_item == NULL) return;

  if (astra_selector.selected_item->type == user_item)
  {
    astra_exit_animation_finished = false;
    astra_user_item_t* _selected_user_item = astra_to_user_item(astra_selector.selected_item);
    _selected_user_item->entering_user_item = true;
    _selected_user_item->exiting_user_item = false;
    _selected_user_item->user_item_inited = false;
    _selected_user_item->user_item_looping = false;
    return;
  }

  if (astra_selector.selected_item->type == switch_item)
  {
    astra_switch_item_t* _selected_switch_item = astra_to_switch_item(astra_selector.selected_item);
    *_selected_switch_item->value = !*_selected_switch_item->value;
    /* exit_function 先执行（应用新值到硬件），
     * init_function 后执行（从硬件同步回 UI 显示）。 */
    if (_selected_switch_item->exit_function)
      _selected_switch_item->exit_function();
    if (_selected_switch_item->init_function)
      _selected_switch_item->init_function();
    return;
  }

  if (astra_selector.selected_item->type == button_item)
  {
    astra_button_item_t* _selected_switch_item = astra_to_button_item(astra_selector.selected_item);
    if (_selected_switch_item->exit_function)
      _selected_switch_item->exit_function();
    return;
  }

  if (astra_selector.selected_item->type == slider_item)
  {
    astra_slider_item_t* _s = astra_to_slider_item(astra_selector.selected_item);
    if (!_s->is_confirmed)
    {
      /* 进入调值模式: 先从硬件同步当前值, 再备份为取消恢复点 */
      if (_s->init_function) _s->init_function();
      _s->is_confirmed = true;
      _s->value_backup = *_s->value;
      return;
    }
    else
    {
      /* 确认新值: 应用 → 同步回显示 → 退出调值模式 */
      if (_s->exit_function) _s->exit_function();
      _s->is_confirmed = false;
      if (_s->init_function) _s->init_function();
      return;
    }
  }

  if (astra_selector.selected_item->child_num == 0) return;

  astra_enter_child_item(astra_selector.selected_item);
}

void astra_selector_exit_current_item()
{
  if (astra_selector.selected_item == NULL) return;

  /* 滑块确认态：取消 */
  if (astra_selector.selected_item->type == slider_item && astra_to_slider_item(astra_selector.selected_item)->is_confirmed)
  {
    astra_slider_item_t* _selected_slider_item = astra_to_slider_item(astra_selector.selected_item);
    _selected_slider_item->is_confirmed = false;
    *_selected_slider_item->value = _selected_slider_item->value_backup;
    return;
  }

  /* 用户自定义页面：触发退场动画 */
  if (astra_selector.selected_item->type == user_item && astra_to_user_item(astra_selector.selected_item)->in_user_item)
  {
    astra_exit_animation_finished = false;
    astra_user_item_t* _selected_user_item = astra_to_user_item(astra_selector.selected_item);
    _selected_user_item->entering_user_item = false;
    _selected_user_item->exiting_user_item = true;
    _selected_user_item->user_item_inited = false;
    _selected_user_item->user_item_looping = false;
    return;
  }

  astra_list_item_t *parent = astra_selector.selected_item->parent;
  if (parent == NULL) return;

  /* 父节点只有一个子项，跳过中间层直接返回祖父 */
  if (parent->child_num == 1 && parent->parent != NULL)
  {
    astra_list_item_t *_grandparent = parent->parent;
    astra_refresh_list_value = true;

    for (uint8_t i = 0; i < _grandparent->child_num; i++)
        _grandparent->child_list_item[i]->y_list_item =
          _grandparent->child_list_item[i]->y_list_item_trg;

    uint8_t _temp_index = 0;
    for (uint8_t i = 0; i < _grandparent->child_num; i++)
    {
      if (_grandparent->child_list_item[i] == parent)
      { _temp_index = i; break; }
    }
    astra_selector.selected_index = _temp_index;
    astra_selector.selected_item = parent;
    /* 重置选择器位置避免跨屏幕滑行动画 */
    astra_set_font(astra_default_font);
    astra_selector.y_selector_trg = parent->y_list_item_trg - oled_get_str_height();
    astra_selector.y_selector = astra_selector.y_selector_trg;
    astra_camera.y_camera = 0;
    astra_camera.y_camera_trg = 0;
    return;
  }

  /* 根层级或 parent 无效：忽略返回操作 */
  if (parent->parent == NULL) return;

  astra_refresh_list_value = true;

  /* 恢复父级列表项位置 */
  astra_list_item_t *_grandparent = parent->parent;
  for (uint8_t i = 0; i < _grandparent->child_num; i++)
      _grandparent->child_list_item[i]->y_list_item =
        _grandparent->child_list_item[i]->y_list_item_trg;

  /* 在祖父列表中找到父节点索引 */
  uint8_t _temp_index = 0;
  for (uint8_t i = 0; i < _grandparent->child_num; i++)
  {
    if (_grandparent->child_list_item[i] == astra_selector.selected_item->parent)
    {
      _temp_index = i;
      break;
    }
  }
  astra_selector.selected_index = _temp_index;
  astra_selector.selected_item = astra_selector.selected_item->parent;
  /* 重置选择器位置避免跨屏幕滑行动画 */
  astra_set_font(astra_default_font);
  astra_selector.y_selector_trg = astra_selector.selected_item->y_list_item_trg - oled_get_str_height();
  astra_selector.y_selector = astra_selector.y_selector_trg;
  /* 返回父列表时重置相机偏移，防止子菜单留存的偏移把父菜单推出屏幕 */
  astra_camera.y_camera = 0;
  astra_camera.y_camera_trg = 0;
}

/* --- 列表管理 --- */
bool astra_push_item_to_list(astra_list_item_t *_parent, astra_list_item_t *_child)
{
  if (_parent == NULL) return false;
  if (_child == NULL) return false;
  if (_parent->child_num >= MAX_LIST_CHILD_NUM) return false;
  if (_parent->layer >= MAX_LIST_LAYER) return false;

  _child->layer = _parent->layer + 1;
  _child->child_num = 0;

  st7789_set_font(astra_default_font);
  /* 第一个子项: y = 字体高度 + 顶部边距 - 1 */
  if (_parent->child_num == 0)
    _child->y_list_item_trg = oled_get_str_height() + LIST_FONT_TOP_MARGIN - 1;
  else
    _child->y_list_item_trg = _parent->child_list_item[_parent->child_num - 1]->y_list_item_trg + LIST_ITEM_SPACING;

  if (_parent->layer == 0 && _parent->child_num == 0)
  {
    astra_bind_item_to_selector(_child);
    astra_bind_selector_to_camera(&astra_selector);
  }

  _parent->child_list_item[_parent->child_num++] = _child;
  _child->parent = _parent;
  return true;
}

/* --- 相机 --- */
astra_camera_t astra_camera = {0, 0, 0, 0};

void astra_bind_selector_to_camera(astra_selector_t *_selector)
{
  if (_selector == NULL) return;
  astra_camera.selector = _selector;
}
