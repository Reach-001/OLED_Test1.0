/**
 * @file    app_ui.c
 * @brief   B21 按键到 Astra UI 的适配层。
 * @details 本文件负责：
 *            1. 构建实际控制、PID 调参和控件演示页面
 *            2. 将单键物理按键事件映射为 UI 操作
 *            3. 驱动 UI 的刷新循环
 *
 * @par 操作约定：
 *   - 单击：     切换下一个选项
 *   - 双击：     进入/确认当前选项
 *   - 长按：     返回上一级
 *   - 单击+长按： 返回根菜单
 */

#include "app_ui.h"
#include "astra_ui_core.h"
#include "astra_ui_item.h"
#include "astra_ui_drawer.h"
#include "astra_ui_draw_driver.h"
#include "app_control.h"
#include "app_task.h"
#include <stdbool.h>

/*===========================================================================
 * 控件状态变量（演示用）
 *=========================================================================*/

/** 开关1状态 */
static bool s_demo_switch = false;

/** 开关2状态 */
static bool s_demo_switch2 = true;

/** 滑块1数值（范围 0~100，步长5） */
static int16_t s_demo_value = 50;

/** 滑块2数值（范围 -50~50，步长1） */
static int16_t s_demo_value2 = 30;

/** 滑块3数值（范围 0~255，步长10） */
static int16_t s_demo_value3 = 80;

/** 运行开关显示值，进入页面时从控制模块同步 */
static bool s_ui_run_enable = false;

/** 目标速度调节值 */
static int16_t s_ui_target_speed = TARGET_SPEED_DEFAULT;

/** PID 调参值，使用整数显示，和当前控制模块参数保持一致 */
static int16_t s_ui_steer_kp = (int16_t)PID_STEER_KP;
static int16_t s_ui_steer_kd = (int16_t)PID_STEER_KD;
static int16_t s_ui_speed_kp = (int16_t)PID_SPEED_KP;
static int16_t s_ui_speed_ki = (int16_t)PID_SPEED_KI;

/*===========================================================================
 * 按钮回调函数
 *=========================================================================*/

static void ui_sync_control_values(void)
{
    control_param_t *param = control_get_param();

    s_ui_run_enable = (control_is_enabled() != 0);
    s_ui_target_speed = control_get_target_speed();
    s_ui_steer_kp = (int16_t)param->steer_pid.kp;
    s_ui_steer_kd = (int16_t)param->steer_pid.kd;
    s_ui_speed_kp = (int16_t)param->speed_pid.kp;
    s_ui_speed_ki = (int16_t)param->speed_pid.ki;
}

static void ui_apply_run_enable(void)
{
    if (s_ui_run_enable)
    {
        task_start();
        astra_push_info_bar("RUN", 800);
    }
    else
    {
        task_stop();
        astra_push_info_bar("STOP", 800);
    }

    ui_sync_control_values();
}

static void ui_apply_target_speed(void)
{
    control_set_target_speed(s_ui_target_speed);
    s_ui_target_speed = control_get_target_speed();
    astra_push_info_bar("SPEED OK", 800);
}

static void ui_apply_pid_values(void)
{
    control_set_steer_pid((float)s_ui_steer_kp, PID_STEER_KI, (float)s_ui_steer_kd);
    control_set_speed_pid((float)s_ui_speed_kp, (float)s_ui_speed_ki, PID_SPEED_KD);
    astra_push_info_bar("PID OK", 800);
}

static void ui_reset_pid_action(void)
{
    control_set_steer_pid(PID_STEER_KP, PID_STEER_KI, PID_STEER_KD);
    control_set_speed_pid(PID_SPEED_KP, PID_SPEED_KI, PID_SPEED_KD);
    control_reset();
    ui_sync_control_values();
    astra_push_info_bar("PID RESET", 800);
}

static void ui_show_state_action(void)
{
    switch (task_get_state())
    {
        case SYS_STATE_IDLE:    astra_push_pop_up("STATE IDLE", 1000); break;
        case SYS_STATE_READY:   astra_push_pop_up("STATE READY", 1000); break;
        case SYS_STATE_RUNNING: astra_push_pop_up("STATE RUN", 1000); break;
        case SYS_STATE_STOP:    astra_push_pop_up("STATE STOP", 1000); break;
        case SYS_STATE_ERROR:   astra_push_pop_up("STATE ERROR", 1000); break;
        default:                astra_push_pop_up("STATE ?", 1000); break;
    }
}

/**
 * @brief 按钮控件的通用回调函数（演示用）
 * @note 双击按钮时触发，显示信息提示条
 */
static void ui_button_action(void)
{
    astra_push_info_bar("BUTTON OK", 800);
}

static void ui_push_item(astra_list_item_t *parent, astra_list_item_t *child)
{
    if (!astra_push_item_to_list(parent, child))
        astra_push_info_bar("MENU FULL", 1000);
}

/*===========================================================================
 * UI 页面树构建（核心配置区）
 *=========================================================================*/

/**
 * @brief 构建 Astra UI 的页面树
 * @note 根菜单优先放实际调车入口，控件展示页保留为 UI Demo。
 */
static void ui_build_astra_tree(void)
{
    static uint8_t built = 0;
    if (built) return;
    built = 1;

    ui_sync_control_values();

    astra_list_item_t *root = astra_get_root_list();

    astra_list_item_t *run_page = astra_new_list_item("Run Control", power_icon);
    astra_list_item_t *pid_page = astra_new_list_item("PID Tune", slider_icon);
    astra_list_item_t *demo_page = astra_new_list_item("UI Demo", list_icon);
    astra_list_item_t *system_page = astra_new_list_item("System", flag_icon);

    ui_push_item(root, run_page);
    ui_push_item(root, pid_page);
    ui_push_item(root, demo_page);
    ui_push_item(root, system_page);

    ui_push_item(run_page,
        astra_new_switch_item("Run Enable", &s_ui_run_enable,
                              ui_sync_control_values, ui_apply_run_enable, power_icon));
    ui_push_item(run_page,
        astra_new_slider_item("Target Speed", &s_ui_target_speed,
                              TARGET_SPEED_STEP, 0, TARGET_SPEED_MAX,
                              ui_sync_control_values, ui_apply_target_speed, slider_icon));
    ui_push_item(run_page,
        astra_new_button_item("Show State", ui_show_state_action, flag_icon));
    ui_push_item(run_page,
        astra_new_button_item("Reset PID", ui_reset_pid_action, plus_icon));

    ui_push_item(pid_page,
        astra_new_slider_item("Steer KP", &s_ui_steer_kp,
                              1, 0, 120, ui_sync_control_values, ui_apply_pid_values, slider_icon));
    ui_push_item(pid_page,
        astra_new_slider_item("Steer KD", &s_ui_steer_kd,
                              1, 0, 80, ui_sync_control_values, ui_apply_pid_values, slider_icon));
    ui_push_item(pid_page,
        astra_new_slider_item("Speed KP", &s_ui_speed_kp,
                              1, 0, 100, ui_sync_control_values, ui_apply_pid_values, slider_icon));
    ui_push_item(pid_page,
        astra_new_slider_item("Speed KI", &s_ui_speed_ki,
                              1, 0, 60, ui_sync_control_values, ui_apply_pid_values, slider_icon));

    ui_push_item(demo_page,
        astra_new_button_item("Button A", ui_button_action, plus_icon));
    ui_push_item(demo_page,
        astra_new_button_item("Button B", ui_button_action, list_icon));
    ui_push_item(demo_page,
        astra_new_switch_item("Switch 1", &s_demo_switch, NULL, NULL, switch_icon));
    ui_push_item(demo_page,
        astra_new_switch_item("Switch 2", &s_demo_switch2, NULL, NULL, switch_icon));
    ui_push_item(demo_page,
        astra_new_slider_item("Slider 1", &s_demo_value, 5, 0, 100, NULL, NULL, slider_icon));
    ui_push_item(demo_page,
        astra_new_slider_item("Slider 2", &s_demo_value2, 1, -50, 50, NULL, NULL, slider_icon));
    ui_push_item(demo_page,
        astra_new_slider_item("Slider 3", &s_demo_value3, 10, 0, 255, NULL, NULL, slider_icon));

    astra_list_item_t *sub_list = astra_new_list_item("Sub List", list_icon);
    ui_push_item(sub_list, astra_new_button_item("Sub A", ui_button_action, plus_icon));
    ui_push_item(sub_list, astra_new_button_item("Sub B", ui_button_action, plus_icon));
    ui_push_item(demo_page, sub_list);

    ui_push_item(system_page,
        astra_new_button_item("Dian Sai 2026", ui_button_action, flag_icon));
    ui_push_item(system_page,
        astra_new_button_item("OLED 240x135", ui_button_action, list_icon));
}

/*===========================================================================
 * 单键手势识别状态机
 *=========================================================================*/

/** 上一次按键电平状态 */
static uint8 s_key_prev_pressed = 0;

/** 等待单击确认标志（第一次释放后 350ms 窗口内） */
static uint8 s_wait_single = 0;

/** 第二次按下已发生标志 */
static uint8 s_second_press = 0;

/** 第一次释放时间戳（ms） */
static uint32 s_first_release_ms = 0;

/** 第二次按下时间戳（ms） */
static uint32 s_second_press_ms = 0;

/**
 * @brief 重置到根菜单（组合键触发）
 */
static void ui_reset_to_root(void)
{
    astra_init_list();
    astra_push_info_bar("MAIN", 600);
}

/*===========================================================================
 * 公共 API
 *=========================================================================*/

/**
 * @brief UI 模块初始化
 */
void app_ui_init(void)
{
    astra_ui_driver_init();
    ui_build_astra_tree();

    in_astra = true;
    astra_init_core();
    astra_push_info_bar("2026 Dian_Sai_Demo", 800);
}

/**
 * @brief 按键事件处理
 */
void app_ui_handle_key(uint8 pressed, bsp_key_event_enum event, uint32 now_ms)
{
    /* 检测新的按下（边沿上升） */
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
        /* 按键释放事件 */
        if (s_wait_single && (now_ms - s_first_release_ms <= 350))
        {
            /* 双击 → 进入当前项 */
            astra_selector_jump_to_selected_item();
            s_wait_single = 0;
            s_second_press = 0;
        }
        else
        {
            /* 首次释放，进入等待单击状态 */
            s_wait_single = 1;
            s_second_press = 0;
            s_first_release_ms = now_ms;
        }
    }
    else if (event == KEY_EVENT_LONG_PRESS)
    {
        /* 长按事件 */
        if (s_wait_single && s_second_press && (s_second_press_ms - s_first_release_ms <= 400))
        {
            /* 组合键 → 返回根菜单 */
            ui_reset_to_root();
        }
        else
        {
            /* 普通长按 → 返回上一级 */
            astra_selector_exit_current_item();
        }
        s_wait_single = 0;
        s_second_press = 0;
    }

    /* 超时检测：单击 */
    if (s_wait_single && !pressed && (now_ms - s_first_release_ms > 350))
    {
        astra_selector_go_next_item();
        s_wait_single = 0;
        s_second_press = 0;
    }

    s_key_prev_pressed = pressed;
}

/**
 * @brief UI 任务主循环（约 10ms 调用一次）
 */
void app_ui_task(void)
{
    /* 仅在当前处于 Astra UI 模式时才刷新屏幕。
     * 用户可通过 ALLOW_EXIT_ASTRA_UI_BY_USER 退出 UI，
     * 退出后不再清屏和绘制，恢复外部显示内容。 */
    if (!in_astra) return;

    oled_clear_buffer();
    astra_ui_main_core();
    astra_ui_widget_core();
    oled_send_buffer();
    astra_draw_color_overlay();
}
