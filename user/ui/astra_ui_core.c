/**
 * @file    astra_ui_core.c
 * @brief   Astra UI 核心逻辑 — 动画、刷新、主循环
 * @note    适配 ST7789 320×172 横屏, 16px 字体
 *
 * 移植时需修改:
 *   - ad_astra() 中的按键检测逻辑
 *   - astra_refresh_selector_position() 中的 selector 尺寸常量
 */

#include "astra_ui_core.h"
#include <stdio.h>
#include <math.h>
#include "astra_ui_drawer.h"

bool in_astra = false;

/**
 * @brief 进入 Astra UI 的触发检测
 *
 * @note 需要在主循环中反复调用
 * @note 通过按键等传感器触发, 当 in_astra 为 true 时进入 UI
 * @note 用户需替换注释区域为实际的按键检测代码
 */
void ad_astra()
{
  /** ======== 用户自行修改: 按键检测逻辑 ======== **/
  // if (in_astra) return;
  // static int64_t _key_press_span = 0;
  // static uint32_t _key_start_time = 0;
  // static bool _key_clicked = false;
  // static char _msg[100] = {};
  //
  // // 检测按键 (MSPM0: DL_GPIO_readPins)
  // if (DL_GPIO_readPins(KEY_PORT, KEY_PIN) == 0)
  // {
  //   if (!_key_clicked)
  //   {
  //     _key_clicked = true;
  //     _key_start_time = get_ticks();
  //   }
  //   if (get_ticks() - _key_start_time > 1000 && _key_clicked)
  //   {
  //     _key_press_span = get_ticks() - _key_start_time;
  //     if (_key_press_span <= 2500)
  //     {
  //       sprintf(_msg, "继续长按%.1f秒进入.", (2500 - _key_press_span) / 1000.0f);
  //       astra_push_info_bar(_msg, 2000);
  //     }
  //     else if (_key_press_span > 2500)
  //     {
  //       astra_push_info_bar("玩得开心! :p", 2000);
  //       in_astra = true;
  //       astra_init_list();
  //       _key_clicked = false;
  //       _key_start_time = 0;
  //       _key_press_span = 0;
  //     }
  //   }
  // }
  // else
  // {
  //   _key_clicked = false;
  //   if (_key_press_span != 0)
  //   {
  //     astra_push_info_bar("bye!", 2000);
  //     _key_press_span = 0;
  //   }
  // }
  /** ======== 用户自行修改结束 ======== **/
}

bool astra_is_in_user_item()
{
  if (astra_selector.selected_item == NULL) return false;
  return (astra_selector.selected_item->type == user_item
          && astra_to_user_item(astra_selector.selected_item)->in_user_item)
         ? true : false;
}

/*===========================================================================
 * 动画系统 — 缓动动画
 *===========================================================================*/

/**
 * @brief 通用缓动动画 (指数衰减逼近)
 * @param _pos    当前位置 (会被修改)
 * @param _posTrg 目标位置
 * @param _speed  速度参数 (0-99, 越高越快)
 */
void astra_animation(float *_pos, float _posTrg, float _speed)
{
  if (*_pos != _posTrg)
  {
    if (fabs(*_pos - _posTrg) <= 1.0f) *_pos = _posTrg;
    else *_pos += (_posTrg - *_pos) / (100.0f - _speed) / 1.0f;
  }
}

/*===========================================================================
 * Widget 刷新 — 信息栏 & 弹窗
 *===========================================================================*/

void astra_refresh_info_bar()
{
  astra_animation(&astra_info_bar.y_info_bar, astra_info_bar.y_info_bar_trg, 94);
  astra_animation(&astra_info_bar.w_info_bar, astra_info_bar.w_info_bar_trg, 95);
}

void astra_refresh_pop_up()
{
  astra_animation(&astra_pop_up.y_pop_up, astra_pop_up.y_pop_up_trg, 94);
  astra_animation(&astra_pop_up.w_pop_up, astra_pop_up.w_pop_up_trg, 96);
}

/*===========================================================================
 * 相机刷新 — 视口跟随选择器
 *===========================================================================*/

void astra_refresh_camera_position()
{
  if (astra_camera.selector == NULL) return;

  /* 向下超出屏幕: 相机上移。
   * 使用选择器动态高度 + 2px 下边距，避免滑块（28px）底部被裁剪。 */
  float _sel_bottom_margin = astra_selector.h_selector_trg + 2;
  if (astra_camera.selector->y_selector_trg + _sel_bottom_margin + astra_camera.y_camera_trg > SCREEN_HEIGHT)
    astra_camera.y_camera_trg = SCREEN_HEIGHT - astra_camera.selector->y_selector_trg - _sel_bottom_margin;

  /* 向上进入标题栏: 相机下移 */
  if (astra_camera.selector->y_selector_trg + astra_camera.y_camera_trg < LIST_INFO_BAR_HEIGHT)
    astra_camera.y_camera_trg = LIST_INFO_BAR_HEIGHT - astra_camera.selector->y_selector_trg;

  astra_animation(&astra_camera.x_camera, astra_camera.x_camera_trg, 96);
  astra_animation(&astra_camera.y_camera, astra_camera.y_camera_trg, 96);
}

/*===========================================================================
 * Widget 核心 — 汇总所有 Widget 刷新
 *===========================================================================*/

void astra_refresh_widget_core_position()
{
  astra_refresh_info_bar();
  astra_refresh_pop_up();
}

/*===========================================================================
 * 列表/选择器初始化
 *===========================================================================*/

void astra_init_list()
{
  if (astra_get_root_list()->child_num == 0) return;

  /* 根菜单初始化直接对齐目标位置，避免上电首帧从屏幕外跳入。 */
  for (uint8_t i = 0; i < astra_get_root_list()->child_num; i++)
    astra_get_root_list()->child_list_item[i]->y_list_item = astra_get_root_list()->child_list_item[i]->y_list_item_trg;
  astra_selector.selected_index = 0;
  astra_selector.selected_item = astra_get_root_list()->child_list_item[0];
  astra_set_font(astra_default_font);
  /* 选择器顶边 = 基线 - 字体高度，使 16px 文字在 18px 选择器中上下各留 1px 居中 */
  astra_selector.y_selector = astra_selector.selected_item->y_list_item_trg - oled_get_str_height();
  astra_selector.y_selector_trg = astra_selector.y_selector;
  astra_selector.w_selector = oled_get_UTF8_width(astra_selector.selected_item->content) + UI_SELECTOR_TEXT_PADDING;
  astra_selector.w_selector_trg = astra_selector.w_selector;
  astra_selector.h_selector = UI_SELECTOR_HEIGHT;
  astra_selector.h_selector_trg = astra_selector.h_selector;
  astra_camera.x_camera = 0;
  astra_camera.x_camera_trg = 0;
  astra_camera.y_camera = 0;
  astra_camera.y_camera_trg = 0;
}

void astra_init_core()
{
  astra_init_list();
  astra_bind_selector_to_camera(astra_get_selector());
}

/*===========================================================================
 * 列表项位置刷新
 *===========================================================================*/

void astra_refresh_list_item_position()
{
  for (uint8_t i = 0; i < astra_selector.selected_item->parent->child_num; i++)
    astra_animation(&astra_selector.selected_item->parent->child_list_item[i]->y_list_item,
                    astra_selector.selected_item->parent->child_list_item[i]->y_list_item_trg, 84);
}

/*===========================================================================
 * 选择器位置刷新 — 核心布局计算
 *===========================================================================*/

void astra_refresh_selector_position()
{
  astra_set_font(astra_default_font);

  /* 选择器 Y: 文字 — 16px 文字在 18px 选择器中上下各 1px 居中 */
  astra_selector.y_selector_trg = astra_selector.selected_item->y_list_item_trg
                                  - oled_get_str_height();

  /* 宽度: 开关满宽(右侧指示器), 滑块阔宽覆盖刻度线+数值, 普通项紧跟文字 */
  astra_selector.w_selector_trg = (astra_selector.selected_item->type == switch_item)
    ? OLED_WIDTH - UI_SELECTOR_FULL_MARGIN
    : (astra_selector.selected_item->type == slider_item)
      ? OLED_WIDTH - 16
      : oled_get_UTF8_width(astra_selector.selected_item->content) + UI_SELECTOR_TEXT_PADDING;

  /* 高度: 普通18, 滑块暂时不用扩展 */
  astra_selector.h_selector_trg = (astra_selector.selected_item->type == slider_item)
    ? UI_SELECTOR_HEIGHT + 0 : UI_SELECTOR_HEIGHT;

  astra_animation(&astra_selector.y_selector, astra_selector.y_selector_trg, 92);
  astra_animation(&astra_selector.w_selector, astra_selector.w_selector_trg, 92);
  astra_animation(&astra_selector.h_selector, astra_selector.h_selector_trg, 93);
}

/*===========================================================================
 * 主视口刷新
 *===========================================================================*/

void astra_refresh_main_core_position()
{
  astra_refresh_list_item_position();
}

/*===========================================================================
 * Widget 渲染入口
 *===========================================================================*/

void astra_ui_widget_core()
{
  astra_refresh_widget_core_position();
  astra_draw_widget();
}

/*===========================================================================
 * 主循环 — 每帧调用此函数
 *===========================================================================*/

void astra_ui_main_core()
{
  if (!in_astra) return;
  if (astra_selector.selected_item == NULL) return;

  /* 1. User Item 切换逻辑 */
  if (astra_selector.selected_item->type == user_item
      && !astra_to_user_item(astra_selector.selected_item)->in_user_item)
  {
    astra_user_item_t *_selected_user_item = astra_to_user_item(astra_selector.selected_item);

    if (_selected_user_item->entering_user_item && astra_exit_animation_status == 1)
    {
      if (_selected_user_item->init_function != NULL)
        _selected_user_item->init_function();
      _selected_user_item->in_user_item = 1;
    }
  }

  /* 2. 渲染逻辑 */
  if (astra_selector.selected_item->type == user_item
      && astra_to_user_item(astra_selector.selected_item)->in_user_item)
  {
    /* User Item 模式: 运行用户自定义 loop 函数 */
    astra_user_item_t* _selected_user_item = astra_to_user_item(astra_selector.selected_item);

    if (_selected_user_item->loop_function != NULL)
        _selected_user_item->loop_function();

    if (_selected_user_item->exiting_user_item && astra_exit_animation_status == 1)
    {
        if (_selected_user_item->exit_function != NULL)
            _selected_user_item->exit_function();
        _selected_user_item->in_user_item = 0;

        /* 父列表只有一个子项时，退场动画结束直接跳到祖父（跳过中间空壳层） */
        if (astra_selector.selected_item->parent != NULL
            && astra_selector.selected_item->parent->child_num == 1
            && astra_selector.selected_item->parent->parent != NULL)
        {
            astra_selector_exit_current_item();
        }
    }
  }
  else
  {
    /* 正常列表模式: 先更新选择器目标，再用当前目标约束相机。 */
    astra_refresh_selector_position();
    astra_refresh_camera_position();
    astra_refresh_main_core_position();
    astra_draw_list();
  }

  /* 3. 退场动画覆盖绘制 */
  if (!astra_exit_animation_finished)
    astra_draw_exit_animation();
}
