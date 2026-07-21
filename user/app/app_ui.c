/**
 * @file    app_ui.c
 * @brief   B21 按键到 Astra UI 的适配层。
 *
 * 修改入口:
 *   1. 菜单结构: 搜索“UI 页面配置区”
 *   2. 操作映射: app_ui_handle_key()
 *
 * 操作约定:
 *   单击: 下一个选项
 *   双击: 进入当前选项/确认
 *   长按: 返回上一级
 *   单击+长按: 重新回到根菜单
 */

#include "app_ui.h"
#include "astra_ui_core.h"
#include "astra_ui_item.h"
#include "astra_ui_draw_driver.h"
#include <stdbool.h>

/*===========================================================================
 * UI 页面配置区
 *
 * 这里是最常改的地方:
 *   - astra_new_list_item("标题", 图标) 创建页面或选项
 *   - astra_push_item_to_list(父页面, 子页面/子选项) 建立跳转关系
 *   - 有子项的 list_item 双击后进入下一页
 *=========================================================================*/

static bool s_demo_switch = false;
static int16_t s_demo_value = 50;

static void ui_button_action(void)
{
    astra_push_info_bar("BUTTON OK", 800);
}

static void ui_build_astra_tree(void)
{
    static uint8_t built = 0;
    if (built) return;
    built = 1;

    astra_list_item_t *root = astra_get_root_list();

    astra_list_item_t *page_1 = astra_new_list_item("Option 1", list_icon);
    astra_list_item_t *page_2 = astra_new_list_item("Option 2", list_icon);
    astra_list_item_t *page_3 = astra_new_list_item("Option 3", list_icon);
    astra_list_item_t *page_4 = astra_new_list_item("Option 4", list_icon);

    astra_push_item_to_list(root, page_1);
    astra_push_item_to_list(root, page_2);
    astra_push_item_to_list(root, page_3);
    astra_push_item_to_list(root, page_4);

    astra_push_item_to_list(page_1, astra_new_button_item("Option 1-A", ui_button_action, plus_icon));
    astra_push_item_to_list(page_1, astra_new_switch_item("Option 1-B", &s_demo_switch, NULL, NULL, switch_icon));
    astra_push_item_to_list(page_1, astra_new_slider_item("Value 1", &s_demo_value, 5, 0, 100, NULL, NULL, slider_icon));

    astra_push_item_to_list(page_2, astra_new_button_item("Option 2-A", ui_button_action, plus_icon));
    astra_push_item_to_list(page_2, astra_new_button_item("Option 2-B", ui_button_action, plus_icon));

    astra_push_item_to_list(page_3, astra_new_list_item("Option 3-A", list_icon));
    astra_push_item_to_list(page_3, astra_new_button_item("Option 3-B", ui_button_action, plus_icon));

    astra_push_item_to_list(page_4, astra_new_button_item("Option 4-A", ui_button_action, plus_icon));
    astra_push_item_to_list(page_4, astra_new_button_item("Option 4-B", ui_button_action, plus_icon));
}

/*===========================================================================
 * 单键手势识别状态机
 *=========================================================================*/

static uint8 s_key_prev_pressed = 0;
static uint8 s_wait_single = 0;
static uint8 s_second_press = 0;
static uint32 s_first_release_ms = 0;
static uint32 s_second_press_ms = 0;

static void ui_reset_to_root(void)
{
    astra_init_list();
    astra_push_info_bar("MAIN", 600);
}

void app_ui_init(void)
{
    astra_ui_driver_init();
    ui_build_astra_tree();

    in_astra = true;
    astra_init_core();
    astra_push_info_bar("ASTRA UI", 800);
}

void app_ui_handle_key(uint8 pressed, bsp_key_event_enum event, uint32 now_ms)
{
    if (pressed && !s_key_prev_pressed)
    {
        if (s_wait_single)
        {
            s_second_press = 1;
            s_second_press_ms = now_ms;
        }
    }

    if (event == KEY_EVENT_PRESS)
    {
        if (s_wait_single && (now_ms - s_first_release_ms <= 350))
        {
            astra_selector_jump_to_selected_item();
            s_wait_single = 0;
            s_second_press = 0;
        }
        else
        {
            s_wait_single = 1;
            s_second_press = 0;
            s_first_release_ms = now_ms;
        }
    }
    else if (event == KEY_EVENT_LONG_PRESS)
    {
        if (s_wait_single && s_second_press && (s_second_press_ms - s_first_release_ms <= 400))
        {
            ui_reset_to_root();
        }
        else
        {
            astra_selector_exit_current_item();
        }
        s_wait_single = 0;
        s_second_press = 0;
    }

    if (s_wait_single && !pressed && (now_ms - s_first_release_ms > 350))
    {
        astra_selector_go_next_item();
        s_wait_single = 0;
        s_second_press = 0;
    }

    s_key_prev_pressed = pressed;
}

void app_ui_task(void)
{
    if (!in_astra) in_astra = true;

    /* Astra 原始绘制模型没有 ST7789 局部脏区，这里先全屏清除验证框架可用性。 */
    oled_clear_buffer();
    astra_ui_main_core();
    astra_ui_widget_core();
}
