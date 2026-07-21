/**
 * @file    astra_ui_drawer.c
 * @brief   Astra UI 渲染层 — 列表/选择器/控件绘制实现
 * @note    适配 ST7789 320×172 横屏
 *
 * 移植改动:
 *   - astra_draw_list_appearance(): 右侧滚动条, 适配 320 宽
 *   - 所有 u8g2_font_my_chinese → astra_default_font
 *   - 所有布局常量使用 OLED_WIDTH/SCREEN_HEIGHT/LIST_ITEM_* 宏
 */

#include "astra_ui_drawer.h"
#include <math.h>
#include <stdio.h>
#include "astra_ui_core.h"

/*===========================================================================
 * 退场动画 — 沙漏 + 遮罩，适配任意分辨率
 *===========================================================================*/

/* 退场动画缓动 (与 core 中 astra_animation 逻辑一致) */
static void astra_exit_anim(float *_pos, float _posTrg, float _speed)
{
  if (*_pos != _posTrg)
  {
    if (fabs(*_pos - _posTrg) <= 1.0f)
        *_pos = _posTrg;
    else
        *_pos += (_posTrg - *_pos) / (100.0f - _speed) / 1.0f;
  }
}

uint8_t astra_exit_animation_status = 0;

void astra_draw_exit_animation()
{
  /* 退场动画状态机:
   *   0 → 遮罩下落
   *   1 → 遮罩完全覆盖屏幕, 等待背景切换
   *   2 → 遮罩抬升, 露出新背景
   *   0 → 退场完成
   */
  static float _temp_h = -8;
  static float _temp_h_trg = OLED_HEIGHT + 8;

  oled_set_draw_color(0);
  oled_draw_box(0, 0, OLED_WIDTH, _temp_h);  /* 黑遮罩 */
  oled_set_draw_color(1);

  /* --- 沙漏图标 (居中) --- */
  uint8_t _x_hg = OLED_WIDTH / 2 - 8;
  int8_t  _y_hg = _temp_h - OLED_HEIGHT / 2 - 18;

  if (_y_hg + 20 >= 0)
  {
    /* 顶底矩形 */
    oled_draw_box(_x_hg, _y_hg + 2, 13, 3);
    oled_set_draw_color(0);
    oled_draw_H_line(_x_hg + 2, _y_hg + 3, 9);
    oled_set_draw_color(1);

    /* 主体竖线 */
    oled_draw_V_line(_x_hg + 1, _y_hg + 4, 5);
    oled_draw_V_line(_x_hg + 11, _y_hg + 4, 5);

    /* 斜线 */
    for (uint8_t i = 0; i < 5; ++i)
    {
      int8_t cy = _y_hg + 8 + i;
      int8_t lx = (i < 3) ? (_x_hg + 1 + i) : (_x_hg + 4);
      int8_t rx = (i < 3) ? (_x_hg + 10 - i) : (_x_hg + 7);
      oled_draw_H_line(lx, cy, 2);
      oled_draw_H_line(rx, cy, 2);
    }

    /* 收口 */
    for (uint8_t i = 0; i < 3; ++i)
    {
      int8_t cy = _y_hg + 13 + i;
      oled_draw_H_line(_x_hg + 3 - i, cy, 2);
      oled_draw_H_line(_x_hg + 8 + i, cy, 2);
    }

    /* 底部竖线 */
    oled_draw_V_line(_x_hg + 1, _y_hg + 16, 3);
    oled_draw_V_line(_x_hg + 11, _y_hg + 16, 3);

    /* 底部矩形 */
    oled_draw_box(_x_hg, _y_hg + 19, 13, 3);
    oled_set_draw_color(0);
    oled_draw_H_line(_x_hg + 2, _y_hg + 20, 9);
    oled_set_draw_color(1);

    /* 散点 */
    const uint8_t _pts[][2] = {
      {5,7},{7,7},{6,8},{6,10},{6,14},{6,16},{5,17},{7,17},{4,18},{6,18},{8,18}
    };
    for (uint8_t i = 0; i < sizeof(_pts) / sizeof(_pts[0]); ++i)
      oled_draw_pixel(_x_hg + _pts[i][0], _y_hg + _pts[i][1]);
  }

  /* 遮罩下方强化横线 */
  if (_temp_h + 3 >= 0)
    for (uint8_t i = 0; i <= 3; ++i)
      oled_draw_H_line(0, _temp_h + i, OLED_WIDTH);

  /* 棋盘格过渡边缘 */
  for (int16_t i = 0; i <= OLED_WIDTH; i += 2)
    for (int16_t j = _temp_h - 5; j <= _temp_h - 1; j++)
    {
      if (j % 2 == 0) oled_draw_pixel(i + 1, j);
      if (j % 2 == 1) oled_draw_pixel(i, j);
    }

  astra_exit_anim(&_temp_h, _temp_h_trg, 94);

  /* 状态机推进 */
  if (astra_exit_animation_status == 0 && _temp_h == _temp_h_trg && _temp_h == OLED_HEIGHT + 8)
  {
    astra_exit_animation_status = 1;
    return;
  }

  if (astra_exit_animation_status == 1)
  {
    _temp_h_trg = -8;
    astra_exit_animation_status = 2;
    return;
  }

  if (astra_exit_animation_status == 2 && _temp_h == _temp_h_trg && _temp_h == -8)
  {
    astra_exit_animation_finished = true;
    astra_exit_animation_status = 0;
    _temp_h = -8;
    _temp_h_trg = OLED_HEIGHT + 8;
    return;
  }
}

/*===========================================================================
 * 信息栏 — 底部弹出条
 *===========================================================================*/

void astra_draw_info_bar()
{
  if (!astra_info_bar.is_running) return;

  /* 弹窗到位后开始计时 */
  if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg)
    astra_info_bar.time = get_ticks();

  /* 超时收回 */
  if (astra_info_bar.time - astra_info_bar.time_start >= astra_info_bar.span)
  {
    astra_info_bar.y_info_bar_trg = 0 - 2 * INFO_BAR_HEIGHT;
    if (astra_info_bar.y_info_bar == astra_info_bar.y_info_bar_trg)
      astra_info_bar.is_running = false;
  }

  int16_t _x_bar = OLED_WIDTH / 2 - astra_info_bar.w_info_bar / 2;
  int16_t _y1    = astra_info_bar.y_info_bar - 4;
  int16_t _y2    = astra_info_bar.y_info_bar + INFO_BAR_HEIGHT;

  astra_set_font(astra_default_font);

  /* 阴影 */
  oled_set_draw_color(1);
  oled_draw_R_box(_x_bar + 3, _y1 + 3,
                  (int16_t)astra_info_bar.w_info_bar, INFO_BAR_HEIGHT + 4, 4);

  /* 黑底 */
  oled_set_draw_color(0);
  oled_draw_R_box((int16_t)(OLED_WIDTH / 2 - (astra_info_bar.w_info_bar + 4) / 2), _y1,
                  (int16_t)(astra_info_bar.w_info_bar + 4), INFO_BAR_HEIGHT + 6, 4);

  /* 前景 */
  oled_set_draw_color(1);
  oled_draw_R_box(_x_bar, _y1,
                  (int16_t)astra_info_bar.w_info_bar, INFO_BAR_HEIGHT + 4, 3);

  /* 裁切下半圆角 */
  oled_set_draw_color(0);
  oled_draw_H_line(_x_bar + 2, _y2 - 2, (int16_t)(astra_info_bar.w_info_bar - 4));
  oled_draw_pixel(_x_bar + 1, _y2 - 3);
  oled_draw_pixel(_x_bar - 2, _y2 - 3);

  /* 文字 */
  oled_draw_UTF8(_x_bar + 6,
                 (int16_t)(astra_info_bar.y_info_bar + oled_get_str_height() - 2),
                 astra_info_bar.content);
}

/*===========================================================================
 * 弹窗 — 屏幕中部弹出
 *===========================================================================*/

void astra_draw_pop_up()
{
  if (!astra_pop_up.is_running) return;

  if (astra_pop_up.y_pop_up == astra_pop_up.y_pop_up_trg)
    astra_pop_up.time = get_ticks();

  if (astra_pop_up.time - astra_pop_up.time_start >= astra_pop_up.span)
  {
    astra_pop_up.y_pop_up_trg = 0 - 2 * POP_UP_HEIGHT;
    if (astra_pop_up.y_pop_up == astra_pop_up.y_pop_up_trg)
      astra_pop_up.is_running = false;
  }

  int16_t _x_pop = OLED_WIDTH / 2 - astra_pop_up.w_pop_up / 2;
  int16_t _y_pop = astra_pop_up.y_pop_up + POP_UP_HEIGHT;

  astra_set_font(astra_default_font);

  /* 阴影 */
  oled_set_draw_color(1);
  oled_draw_R_box(_x_pop + 1, (int16_t)astra_pop_up.y_pop_up + 3,
                  (int16_t)(astra_pop_up.w_pop_up + 4), POP_UP_HEIGHT, 4);

  /* 黑底 */
  oled_set_draw_color(0);
  oled_draw_R_box((int16_t)(OLED_WIDTH / 2 - (astra_pop_up.w_pop_up + 4) / 2 - 2),
                  (int16_t)(astra_pop_up.y_pop_up - 2),
                  (int16_t)(astra_pop_up.w_pop_up + 8), POP_UP_HEIGHT + 4, 5);

  /* 前景 */
  oled_set_draw_color(1);
  oled_draw_R_box(_x_pop - 2, (int16_t)astra_pop_up.y_pop_up,
                  (int16_t)(astra_pop_up.w_pop_up + 4), POP_UP_HEIGHT, 3);

  /* 裁切下半圆角 */
  oled_set_draw_color(0);
  oled_draw_H_line(_x_pop, _y_pop - 2, (int16_t)astra_pop_up.w_pop_up);
  oled_draw_pixel(_x_pop - 1, _y_pop - 3);
  oled_draw_pixel((int16_t)(OLED_WIDTH / 2 + astra_pop_up.w_pop_up / 2), _y_pop - 3);

  /* 文字 */
  oled_draw_UTF8(_x_pop + 3,
                 (int16_t)(astra_pop_up.y_pop_up + oled_get_str_height() + 1),
                 astra_pop_up.content);
}

/*===========================================================================
 * 列表外观 — 右侧滚动条
 *===========================================================================*/

void astra_draw_list_appearance()
{
  oled_set_draw_color(1);

  /* ---- 右侧滚动条 ---- */
  oled_draw_V_line(OLED_WIDTH - 5, 0, OLED_HEIGHT);
  oled_draw_V_line(OLED_WIDTH - 1, 0, OLED_HEIGHT);

  /* 滑块位置与大小 */
  static float _part_len = 0;
  _part_len = ceilf((SCREEN_HEIGHT - 10.0f) / (float)astra_selector.selected_item->parent->child_num);
  oled_draw_box(OLED_WIDTH - 4,
                5 + astra_selector.selected_index * _part_len,
                3, _part_len);

  /* 滑块内横线 */
  oled_set_draw_color(0);
  oled_draw_H_line(OLED_WIDTH - 4,
                   _part_len + (float)astra_selector.selected_index * _part_len, 3);

  if (_part_len >= 9)
  {
    oled_draw_H_line(OLED_WIDTH - 4,
                     floorf(_part_len - 2.0f + (float)astra_selector.selected_index * _part_len), 3);
    oled_draw_H_line(OLED_WIDTH - 4,
                     floorf(_part_len + 2.0f + (float)astra_selector.selected_index * _part_len), 3);
  }

  /* 滚动条首尾帽子 */
  oled_set_draw_color(1);
  oled_draw_box(OLED_WIDTH - 4, 0, 3, 4);
  oled_draw_box(OLED_WIDTH - 4, OLED_HEIGHT - 4, 3, 4);
  oled_set_draw_color(0);
  oled_draw_H_line(OLED_WIDTH - 4, 2, 3);
  oled_draw_pixel(OLED_WIDTH - 3, 1);
  oled_draw_H_line(OLED_WIDTH - 4, OLED_HEIGHT - 3, 3);
  oled_draw_pixel(OLED_WIDTH - 3, OLED_HEIGHT - 2);
}

/*===========================================================================
 * 列表项绘制 — 遍历父节点的所有子项
 *===========================================================================*/

void astra_draw_list_item()
{
  for (unsigned char i = 0; i < astra_selector.selected_item->parent->child_num; i++)
  {
    int16_t _x_item = astra_camera.x_camera + LIST_ITEM_LEFT_MARGIN;
    int16_t _y_item = astra_selector.selected_item->parent->child_list_item[i]->y_list_item
                    + astra_camera.y_camera - oled_get_str_height() / 2;

    oled_set_draw_color(1);

    /* ---- 列表项 (带箭头) ---- */
    if (astra_selector.selected_item->parent->child_list_item[i]->type == list_item)
    {
      if (_y_item + 2 > LIST_INFO_BAR_HEIGHT && _y_item - 2 < SCREEN_HEIGHT)
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);
    }
    /* ---- 开关项 ---- */
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == switch_item)
    {
      astra_switch_item_t *_sw = astra_to_switch_item(astra_selector.selected_item->parent->child_list_item[i]);
      if (_sw->init_function && astra_refresh_list_value)
        _sw->init_function();

      if (_y_item + 7 > LIST_INFO_BAR_HEIGHT && _y_item + 1 < SCREEN_HEIGHT)
      {
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);

        /* 开关指示器: 右侧小框 + 填充表示 ON/OFF */
        int16_t _sx = OLED_WIDTH - LIST_ITEM_RIGHT_MARGIN - 7;
        oled_draw_frame(_sx, _y_item - 2, 11, 7);
        if (*_sw->value == true)
        {
          oled_draw_box(_sx + 4, _y_item, 3, 3);
          oled_draw_pixel(_sx + 1, _y_item + 1);
        }
        else
        {
          oled_draw_box(_sx - 4, _y_item, 3, 3);
          oled_draw_pixel(_sx + 5, _y_item + 1);
        }
      }
    }
    /* ---- 按钮项 ---- */
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == button_item)
    {
      if (_y_item + 7 > LIST_INFO_BAR_HEIGHT && _y_item + 1 < SCREEN_HEIGHT)
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);
    }
    /* ---- 滑块项 ---- */
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == slider_item)
    {
      astra_slider_item_t *_sl = astra_to_slider_item(astra_selector.selected_item->parent->child_list_item[i]);
      if (_sl->init_function && astra_refresh_list_value)
        _sl->init_function();

      if (_y_item + 5 > LIST_INFO_BAR_HEIGHT && _y_item - 2 < SCREEN_HEIGHT)
      {
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);

        char _val_str[10] = {};
        sprintf(_val_str, "%d", *_sl->value);

        int16_t _vx = OLED_WIDTH - LIST_ITEM_RIGHT_MARGIN - oled_get_str_width(_val_str) + 2;

        if (_sl->is_confirmed)
        {
          /* 确认状态下闪烁显示 */
          static uint32_t _last_tick = 0;
          static bool _visible = false;
          uint32_t _tick = get_ticks();

          if (_visible)
          {
            oled_set_draw_color(1);
            oled_draw_R_box(_vx, _y_item - 4, oled_get_UTF8_width(_val_str) + 4, oled_get_str_height() - 2, 1);
          }

          oled_set_draw_color(0);
          oled_draw_str(_vx + 2, _y_item + oled_get_str_height() / 2, _val_str);

          if (_tick - _last_tick >= 1000)
          {
            _visible = !_visible;
            _last_tick = _tick;
          }
        }
        else
        {
          oled_draw_str(_vx + 2, _y_item + oled_get_str_height() / 2, _val_str);
        }
      }
    }
    /* ---- 用户自定义项 / 未知类型 ---- */
    else
    {
      if (_y_item + oled_get_str_height() / 2 > LIST_INFO_BAR_HEIGHT
          && _y_item + oled_get_str_height() / 2 < SCREEN_HEIGHT)
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);
    }

    /* 绘制文字内容 (所有类型共用) */
    astra_set_font(astra_default_font);
    if (_y_item + oled_get_str_height() / 2 > LIST_INFO_BAR_HEIGHT
        && _y_item + oled_get_str_height() / 2 < SCREEN_HEIGHT)
      oled_draw_UTF8(10 + _x_item, _y_item + oled_get_str_height() / 2,
                     astra_selector.selected_item->parent->child_list_item[i]->content);
  }

  astra_refresh_list_value = false;
}

/*===========================================================================
 * 列表项图标 — 8 种预设像素图标
 *===========================================================================*/

void astra_draw_list_icon(astra_list_item_icon_t icon, uint16_t x, uint16_t y)
{
  switch (icon)
  {
    case list_icon:
      oled_draw_H_line(2 + x, y - 2, 4);
      oled_draw_H_line(2 + x, y,     5);
      oled_draw_H_line(2 + x, y + 2, 3);
      break;

    case switch_icon:
      oled_draw_circle(4 + x, y + 1, 3);
      oled_draw_V_line(4 + x, y, 3);
      break;

    case plus_icon:
      oled_draw_circle(4 + x, y + 1, 3);
      oled_draw_V_line(4 + x, y, 3);
      oled_draw_H_line(3 + x, y + 1, 3);
      break;

    case slider_icon:
      oled_draw_V_line(3 + x, y - 1, 5);
      oled_draw_V_line(6 + x, y - 1, 5);
      oled_draw_box(2 + x, y - 2, 3, 3);
      oled_draw_box(5 + x, y + 2, 3, 3);
      break;

    case user_icon:
      oled_draw_str(2 + x, y + oled_get_str_height() / 2, "-");
      break;

    case flag_icon:
      oled_draw_V_line(6 + x, y - 1, 5);
      oled_draw_box(3 + x, y - 2, 4, 3);
      break;

    case power_icon:
      oled_draw_circle(4 + x, y + 1, 3);
      oled_draw_V_line(4 + x, y - 2, 3);
      oled_set_draw_color(0);
      oled_draw_pixel(x + 3, y - 2);
      oled_draw_pixel(x + 5, y - 2);
      oled_set_draw_color(1);
      break;

    default:
      break;
  }
}

/*===========================================================================
 * 选择器绘制 — 灰色高亮框 + 棋盘格过渡
 *===========================================================================*/

void astra_draw_selector()
{
  int16_t _xs = astra_camera.x_camera + LIST_ITEM_LEFT_MARGIN;
  int16_t _ys = astra_selector.y_selector + astra_camera.y_camera;

  /* 灰色半透明高亮盒 (color=2 → COLOR_GRAY) */
  oled_set_draw_color(2);
  oled_draw_box(_xs, _ys,
                astra_selector.w_selector, astra_selector.h_selector);

  /* 右侧棋盘格过渡边缘 */
  oled_set_draw_color(1);
  for (int16_t i = astra_selector.w_selector + _xs;
       i <= astra_selector.w_selector + _xs + 7; i += 2)
  {
    for (int16_t j = _ys; j <= _ys + astra_selector.h_selector - 1; j++)
    {
      if (j % 2 == 0) oled_draw_pixel(i + 1, j);
      if (j % 2 == 1) oled_draw_pixel(i, j);
    }
  }
}

/*===========================================================================
 * 组合绘制入口
 *===========================================================================*/

void astra_draw_widget()
{
  astra_draw_info_bar();
  astra_draw_pop_up();
}

void astra_draw_list()
{
  astra_draw_list_appearance();
  astra_draw_list_item();
  astra_draw_selector();
}
