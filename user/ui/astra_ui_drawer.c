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
#include "app_ui_style_config.h"

extern const st7789_font_t font_8x16;  /* ASCII 8x16 字体, draw_str 需要切字体 */

/* 彩色点缀使用 ST7789 直写，主体 UI 仍走 1-bit 帧缓冲。 */
static void astra_draw_overlay_rframe(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color)
{
  oled_set_draw_color(color);
  oled_draw_R_frame(x, y, w, h, r);
}

static const char *astra_current_title(void)
{
  if (astra_selector.selected_item == NULL) return "ASTRA UI";

  astra_list_item_t *parent = astra_selector.selected_item->parent;
  if (parent == NULL || parent->layer == 0) return "ASTRA UI";
  return parent->content;
}

static int16_t astra_selector_effective_radius(int16_t w, int16_t h)
{
  int16_t r = UI_SELECTOR_RADIUS;

  if (r > h / 2) r = h / 2;
  if (r > w / 2) r = w / 2;
  return r;
}

static bool astra_list_row_visible(int16_t baseline)
{
  int16_t font_h = oled_get_str_height();
  int16_t row_top = baseline - font_h + 1;

  return row_top > LIST_INFO_BAR_HEIGHT && baseline < SCREEN_HEIGHT;
}

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

  astra_exit_anim(&_temp_h, _temp_h_trg, 98);  /* 高速沙漏动画 */

  /* 状态机推进 */
  if (astra_exit_animation_status == 0 && fabs(_temp_h - _temp_h_trg) <= 1.0f && fabs(_temp_h - (OLED_HEIGHT + 8)) <= 1.0f)
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

  if (astra_exit_animation_status == 2 && fabs(_temp_h - _temp_h_trg) <= 1.0f && fabs(_temp_h - (-8)) <= 1.0f)
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
  oled_set_draw_color(UI_INFO_SHADOW_COLOR);
  oled_draw_R_box(_x_bar + 3, _y1 + 3,
                  (int16_t)astra_info_bar.w_info_bar, INFO_BAR_HEIGHT + 4, 4);

  /* 黑底 */
  oled_set_draw_color(UI_INFO_BG_COLOR);
  oled_draw_R_box((int16_t)(OLED_WIDTH / 2 - (astra_info_bar.w_info_bar + 4) / 2), _y1,
                  (int16_t)(astra_info_bar.w_info_bar + 4), INFO_BAR_HEIGHT + 6, 4);

  /* 前景 */
  oled_set_draw_color(UI_INFO_BOX_COLOR);
  oled_draw_R_box(_x_bar, _y1,
                  (int16_t)astra_info_bar.w_info_bar, INFO_BAR_HEIGHT + 4, 3);

  /* 裁切下半圆角 */
  oled_set_draw_color(UI_INFO_BG_COLOR);
  oled_draw_H_line(_x_bar + 2, _y2 - 2, (int16_t)(astra_info_bar.w_info_bar - 4));
  oled_draw_pixel(_x_bar + 1, _y2 - 3);
  oled_draw_pixel(_x_bar - 2, _y2 - 3);

  /* 文字 */
  int16_t _text_w = oled_get_UTF8_width(astra_info_bar.content);
  int16_t _text_h = oled_get_str_height();
  int16_t _text_x = _x_bar + ((int16_t)astra_info_bar.w_info_bar - _text_w) / 2;
  int16_t _text_y = _y1 + (INFO_BAR_HEIGHT + 4 - _text_h) / 2 + _text_h - 1;
  oled_set_draw_color(UI_INFO_TEXT_COLOR);
  oled_draw_UTF8(_text_x, _text_y, astra_info_bar.content);
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
  oled_set_draw_color(UI_POPUP_SHADOW_COLOR);
  oled_draw_R_box(_x_pop + 1, (int16_t)astra_pop_up.y_pop_up + 3,
                  (int16_t)(astra_pop_up.w_pop_up + 4), POP_UP_HEIGHT, 4);

  /* 黑底 */
  oled_set_draw_color(UI_POPUP_BG_COLOR);
  oled_draw_R_box((int16_t)(OLED_WIDTH / 2 - (astra_pop_up.w_pop_up + 4) / 2 - 2),
                  (int16_t)(astra_pop_up.y_pop_up - 2),
                  (int16_t)(astra_pop_up.w_pop_up + 8), POP_UP_HEIGHT + 4, 5);

  /* 前景 */
  oled_set_draw_color(UI_POPUP_BOX_COLOR);
  oled_draw_R_box(_x_pop - 2, (int16_t)astra_pop_up.y_pop_up,
                  (int16_t)(astra_pop_up.w_pop_up + 4), POP_UP_HEIGHT, 3);

  /* 裁切下半圆角 */
  oled_set_draw_color(UI_POPUP_BG_COLOR);
  oled_draw_H_line(_x_pop, _y_pop - 2, (int16_t)astra_pop_up.w_pop_up);
  oled_draw_pixel(_x_pop - 1, _y_pop - 3);
  oled_draw_pixel((int16_t)(OLED_WIDTH / 2 + astra_pop_up.w_pop_up / 2), _y_pop - 3);

  /* 文字 */
  oled_set_draw_color(UI_POPUP_TEXT_COLOR);
  oled_draw_UTF8(_x_pop + 3,
                 (int16_t)(astra_pop_up.y_pop_up + oled_get_str_height() + 1),
                 astra_pop_up.content);
}

/*===========================================================================
 * 列表外观 — 右侧滚动条
 *===========================================================================*/

void astra_draw_list_appearance()
{
#if UI_TITLE_ENABLE
  int16_t title_w = oled_get_UTF8_width(astra_current_title());
  int16_t title_x = (OLED_WIDTH - title_w) / 2;
  if (title_x < 2) title_x = 2;

  oled_set_draw_color(UI_TITLE_TEXT_COLOR);
  oled_draw_UTF8(title_x, UI_TITLE_BASELINE_Y, astra_current_title());
  oled_set_draw_color(UI_TITLE_LINE_COLOR);
  oled_draw_H_line(0, LIST_INFO_BAR_HEIGHT - 1, OLED_WIDTH);
#endif
}

/*===========================================================================
 * 列表项绘制 — 遍历父节点的所有子项
 *===========================================================================*/

void astra_draw_list_item()
{
  for (unsigned char i = 0; i < astra_selector.selected_item->parent->child_num; i++)
  {
    int16_t _x_item = astra_camera.x_camera + LIST_ITEM_LEFT_MARGIN;
    int16_t _baseline = astra_selector.selected_item->parent->child_list_item[i]->y_list_item
                      + astra_camera.y_camera;
    int16_t _y_item = _baseline - oled_get_str_height() / 2;
    bool _row_visible = astra_list_row_visible(_baseline);
    bool _is_sel = (astra_selector.selected_item->parent->child_list_item[i]
                    == astra_selector.selected_item);

    /* 选中行: selector 已画白底, 文字/控件用黑色清出字形; 非选中行: 白色文字 */
    uint8 _c = (_is_sel && UI_SELECTOR_FILL_ENABLE) ? UI_COLOR_BLACK : UI_LIST_TEXT_COLOR;
    oled_set_draw_color(_c);

    /* ---- 列表项 (带箭头) ---- */
    if (astra_selector.selected_item->parent->child_list_item[i]->type == list_item)
    {
      if (_row_visible)
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);
    }
    /* ---- 开关项 ---- */
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == switch_item)
    {
      astra_switch_item_t *_sw = astra_to_switch_item(astra_selector.selected_item->parent->child_list_item[i]);
      if (_sw->init_function && astra_refresh_list_value)
        _sw->init_function();

      if (_row_visible)
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
      if (_row_visible)
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);
    }
    /* ---- 滑块项 ---- */
    else if (astra_selector.selected_item->parent->child_list_item[i]->type == slider_item)
    {
      astra_slider_item_t *_sl = astra_to_slider_item(astra_selector.selected_item->parent->child_list_item[i]);
      if (_sl->init_function && astra_refresh_list_value && !_sl->is_confirmed)
        _sl->init_function();

      if (_row_visible)
      {
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);

        /* 图标 + 文字 + 刻度线 + 数值, 共一行。
         * 文字宽 → 刻度线自动收缩, 长文字条短, 短文字条长。 */
        char _val_str[8] = {};
        sprintf(_val_str, "%d", *_sl->value);
        int16_t _vw     = (int16_t)(strlen(_val_str) * 8);
        int16_t _txt_w  = oled_get_UTF8_width(astra_selector.selected_item->parent->child_list_item[i]->content);
        int16_t bar_gap = 6;                                          /* 文字与刻度线间隙 */
        int16_t bar_x   = 10 + _x_item + _txt_w + bar_gap;           /* 文字末尾 + 间隙 */
        int16_t bar_w   = OLED_WIDTH - bar_x - _vw - 22;             /* 右侧留数值空间 */

        /* 刻度线太窄则不画 (文字太长撑满了) */
        if (bar_w >= 20)
        {
          int16_t line_y = _y_item;  /* 与图标和文字垂直中心对齐 */
          int16_t dot;
          if (_sl->value_max > _sl->value_min)
            dot = bar_x + (int16_t)((int32_t)(*_sl->value - _sl->value_min) * (bar_w - 4)
                                  / (_sl->value_max - _sl->value_min));
          else dot = bar_x + bar_w / 2;
          if (dot < bar_x + 2) dot = bar_x + 2;
          if (dot > bar_x + bar_w - 2) dot = bar_x + bar_w - 2;

          /* 刻度线 + 指示器 — 选择框已扩宽覆盖，用 _c 反色即黑底白线/白底黑线 */
          oled_set_draw_color(_c);
          oled_draw_H_line(bar_x, line_y, bar_w);
          oled_draw_V_line(dot, line_y - 4, 9);

          /* 数值 — 与内容文字同基线对齐，选择框覆盖范围内可正常反色 */
          int16_t _vx = bar_x + bar_w + 6;
          st7789_set_font((const void*)&font_8x16);
          oled_set_draw_color(_c);
          if (!(_sl->is_confirmed && ((get_ticks() / 500) & 1)))
            oled_draw_str(_vx + 2, _baseline, _val_str);
          st7789_set_font(astra_default_font);
        }
      }
    }
    /* ---- 用户自定义项 / 未知类型 ---- */
    else
    {
      if (_row_visible)
        astra_draw_list_icon(astra_selector.selected_item->parent->child_list_item[i]->icon, _x_item, _y_item);
    }

    /* 绘制文字内容 (所有类型共用) — 全部白色, 选择器图案自然透出 */
    astra_set_font(astra_default_font);
    if (_row_visible)
      oled_draw_UTF8(10 + _x_item, _baseline,
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
      {
        uint8 _prev = oled_get_draw_color();
        oled_draw_circle(4 + x, y + 1, 3);
        oled_draw_V_line(4 + x, y - 2, 3);
        oled_set_draw_color(UI_COLOR_BLACK);
        oled_draw_pixel(x + 3, y - 2);
        oled_draw_pixel(x + 5, y - 2);
        oled_set_draw_color(_prev);
      }
      break;

    default:
      break;
  }
}

/*===========================================================================
 * 选择器绘制 — RGB565 纯色圆角填充
 *
 * 主体内容在1-bit帧缓冲画好发送后, 选择器在直写层用真灰色
 * 圆角填充覆在最上面。白字在灰底上自然可读, 不需要图案技巧。
 *===========================================================================*/

void astra_draw_selector()
{
  int16_t _xs = astra_camera.x_camera + LIST_ITEM_LEFT_MARGIN;
  int16_t _ys = astra_selector.y_selector + astra_camera.y_camera;
  int16_t _w  = (int16_t)astra_selector.w_selector;
  int16_t _h  = (int16_t)astra_selector.h_selector;
  int16_t _r  = astra_selector_effective_radius(_w, _h);

  if (_ys <= LIST_INFO_BAR_HEIGHT) return;

#if UI_SELECTOR_FILL_ENABLE
  /* 帧缓冲内白底填充: 选择区域全写1(白色), 之后画黑字清除位。
   * 帧缓冲和内容一次性发送到屏幕, 不存在直写层覆盖问题。 */
  oled_set_draw_color(UI_LIST_TEXT_COLOR);
  oled_draw_R_box(_xs, _ys, _w, _h, _r);
#endif
}

void astra_draw_color_overlay()
{
  if (!in_astra || astra_selector.selected_item == NULL || astra_is_in_user_item())
    return;

  st7789_set_buffer_mode(0);

  /* 沙漏动画期间: 擦除标题线上次直写的彩色像素 */
  if (!astra_exit_animation_finished)
  {
#if UI_TITLE_ENABLE
    oled_set_draw_color(UI_COLOR_BLACK);
    oled_draw_H_line(0, LIST_INFO_BAR_HEIGHT - 1, OLED_WIDTH);
#endif
    st7789_set_buffer_mode(1);
    return;
  }

#if UI_TITLE_ENABLE
  oled_set_draw_color(UI_TITLE_LINE_COLOR);
  oled_draw_H_line(0, LIST_INFO_BAR_HEIGHT - 1, OLED_WIDTH);
#endif

  /* ---- 右侧滚动条 — 直写真彩色（帧缓冲1-bit只能显示黑白） ---- */
  {
    uint8_t child_num = astra_selector.selected_item->parent->child_num;

    if (child_num > 1)
    {
      static float _thumb_y     = LIST_INFO_BAR_HEIGHT + 4;
      static float _thumb_y_trg = LIST_INFO_BAR_HEIGHT + 4;
      float part_len = ceilf((SCREEN_HEIGHT - LIST_INFO_BAR_HEIGHT - 8.0f) / (float)child_num);
      int16_t bar_top = LIST_INFO_BAR_HEIGHT + 4;

      _thumb_y_trg = LIST_INFO_BAR_HEIGHT + 4 + astra_selector.selected_index * part_len;
      extern void astra_animation(float *_pos, float _posTrg, float _speed);
      astra_animation(&_thumb_y, _thumb_y_trg, 92);

      /* 轨道线 */
      oled_set_draw_color(UI_SCROLLBAR_COLOR);
      oled_draw_V_line(OLED_WIDTH - 5, bar_top, OLED_HEIGHT - bar_top);
      oled_draw_V_line(OLED_WIDTH - 1, bar_top, OLED_HEIGHT - bar_top);

      /* 滑块 */
      oled_draw_box(OLED_WIDTH - 4, (int16_t)_thumb_y, 3, (int16_t)part_len);

      /* 首尾帽 */
      oled_draw_box(OLED_WIDTH - 4, bar_top, 3, 4);
      oled_draw_box(OLED_WIDTH - 4, OLED_HEIGHT - 4, 3, 4);
    }
  }

  /* 信息栏/弹窗的彩色强调边框 */
  if (astra_info_bar.is_running)
  {
    int16_t _x_bar = OLED_WIDTH / 2 - astra_info_bar.w_info_bar / 2;
    int16_t _y_bar = astra_info_bar.y_info_bar - 4;
    astra_draw_overlay_rframe(_x_bar, _y_bar,
                              (int16_t)astra_info_bar.w_info_bar, INFO_BAR_HEIGHT + 4, 3,
                              UI_INFO_ACCENT_COLOR);
  }

  if (astra_pop_up.is_running)
  {
    int16_t _x_pop = OLED_WIDTH / 2 - astra_pop_up.w_pop_up / 2;
    astra_draw_overlay_rframe(_x_pop - 2, (int16_t)astra_pop_up.y_pop_up,
                              (int16_t)(astra_pop_up.w_pop_up + 4), POP_UP_HEIGHT, 3,
                              UI_POPUP_ACCENT_COLOR);
  }

  /* 选择框棋盘格 — 直写真彩色（帧缓冲 1-bit 不支持彩色编号） */
#if UI_SELECTOR_FILL_ENABLE
  {
    int16_t _xs = astra_camera.x_camera + LIST_ITEM_LEFT_MARGIN;
    int16_t _ys = astra_selector.y_selector + astra_camera.y_camera;
    int16_t _w  = (int16_t)astra_selector.w_selector;
    int16_t _h  = (int16_t)astra_selector.h_selector;

    if (_ys > LIST_INFO_BAR_HEIGHT)
    {
      oled_set_draw_color(UI_SELECTOR_CHESS_COLOR);
      for (int16_t px = _w; px < _w + UI_SELECTOR_CHESS_WIDTH; px += UI_SELECTOR_CHESS_STEP)
        for (int16_t py = 0; py < _h; py++)
          if ((px + py) % 2 == 0)
            oled_draw_pixel(_xs + px, _ys + py);
    }
  }
#endif

  st7789_set_buffer_mode(1);
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
  astra_draw_selector();        /* 先画白底填充 */
  astra_draw_list_item();       /* 再画文字/控件: 选中行用黑色清位 */
}
