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
#include <stdio.h>

/*===========================================================================
 * 控件状态变量
 *=========================================================================*/

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
 * UART 页面 — 串口参数与开关控制
 *=========================================================================*/

/** UART 使能开关 */
static bool s_uart_enable = false;

/** 无线串口实时发送字节计数 */
static uint32 s_uart_tx_bytes = 0;
/** 无线串口实时接收字节计数 */
static uint32 s_uart_rx_bytes = 0;

static void ui_uart_init(void)
{
    s_uart_enable = false;
}

static void ui_uart_loop(void)
{
    st7789_set_font(astra_default_font);

    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(8,  20, "UART Status");

    /* UART 参数行 */
    oled_set_draw_color(UI_COLOR_GRAY);
    char _buf[32] = {};
    snprintf(_buf, sizeof(_buf), "BAUD: %d", WIRELESS_UART_BAUD);
    oled_draw_UTF8(8, 40, _buf);

    snprintf(_buf, sizeof(_buf), "TX: B%d  RX: B%d", 5, 6);
    oled_draw_UTF8(8, 58, _buf);

    /* 使能状态指示 */
    oled_set_draw_color(s_uart_enable ? UI_COLOR_MINT : UI_COLOR_GRAY);
    oled_draw_frame(8, 72, 48, 14);
    oled_draw_str(12, 82, s_uart_enable ? "ON" : "OFF");

    /* 统计信息 */
    oled_set_draw_color(UI_COLOR_WHITE);
    snprintf(_buf, sizeof(_buf), "TX: %lu", s_uart_tx_bytes);
    oled_draw_UTF8(8, 96, _buf);
    snprintf(_buf, sizeof(_buf), "RX: %lu", s_uart_rx_bytes);
    oled_draw_UTF8(8, 114, _buf);
}

static void ui_uart_exit(void)
{
    /* 关闭 UART 输出 */
    s_uart_enable = false;
}

static void ui_uart_toggle_enable(void)
{
    s_uart_enable = !s_uart_enable;
    astra_push_info_bar(s_uart_enable ? "UART ON" : "UART OFF", 600);
}

/*===========================================================================
 * 陀螺仪页面 — IMU660RA 数据显示
 *=========================================================================*/

/* 引用逐飞 IMU660RA 数据变量（extern，由设备驱动模块提供） */
extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;
extern int16 imu660ra_acc_x,  imu660ra_acc_y,  imu660ra_acc_z;

static char s_imu_gyro_str[48] = {};
static char s_imu_acc_str[48]  = {};
static char s_imu_info_str[32] = {};
static bool s_imu_ok = false;

static void ui_imu_init(void)
{
    s_imu_ok = (imu660ra_gyro_x != 0 || imu660ra_gyro_y != 0
             || imu660ra_gyro_z != 0 || imu660ra_acc_x != 0);
}

static void ui_imu_loop(void)
{
    st7789_set_font(astra_default_font);

    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(8, 20, "IMU660RA");

    /* 陀螺仪数据（°/s） */
    snprintf(s_imu_gyro_str, sizeof(s_imu_gyro_str),
             "G: %6d %6d %6d",
             imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z);
    oled_draw_UTF8(8, 42, s_imu_gyro_str);

    /* 加速度计数据 */
    snprintf(s_imu_acc_str, sizeof(s_imu_acc_str),
             "A: %6d %6d %6d",
             imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z);
    oled_draw_UTF8(8, 60, s_imu_acc_str);

    /* 标签 */
    oled_set_draw_color(UI_COLOR_GRAY);
    oled_draw_UTF8(8, 80, "X(dps)  Y(dps)  Z(dps)");
    oled_draw_UTF8(8, 96, "X(g)    Y(g)    Z(g)");

    /* 状态指示 */
    oled_set_draw_color(s_imu_ok ? UI_COLOR_MINT : UI_COLOR_AMBER);
    snprintf(s_imu_info_str, sizeof(s_imu_info_str),
             "%s", s_imu_ok ? "DATA OK" : "NO DATA");
    oled_draw_UTF8(8, 118, s_imu_info_str);
}

static void ui_imu_exit(void)
{
    /* 无特殊清理 */
}

/*===========================================================================
 * 循迹传感器页面 — 5路传感器 0/1 状态统计图
 *=========================================================================*/

/* 循迹数据通过 task_get_track_data() 获取，无需 extern */

static void ui_track_init(void)
{
    /* 进入页面时无需特殊操作 */
}

static void ui_track_loop(void)
{
    const track_data_t *td = task_get_track_data();

    st7789_set_font(astra_default_font);

    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(8, 20, "Track Sensors");

    /* ---- 绘制 5 路传感器柱状图 ---- */
    uint16_t bar_area_w = OLED_WIDTH - 16;
    uint16_t bar_w      = bar_area_w / TRACK_SENSOR_NUM - 4;
    uint16_t bar_max_h  = 50;
    uint16_t bar_top_y  = 30;
    uint16_t label_y    = bar_top_y + bar_max_h + 8;

    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        uint16_t bx = 8 + i * (bar_w + 4);

        if (td->digital[i])
        {
            oled_set_draw_color(UI_COLOR_WHITE);
            oled_draw_box(bx, bar_top_y, bar_w, bar_max_h);
        }
        else
        {
            uint16_t raw_h = (uint16_t)((uint32_t)td->raw[i] * bar_max_h / 4095);
            if (raw_h > bar_max_h) raw_h = bar_max_h;
            if (raw_h > 0)
            {
                oled_set_draw_color(UI_COLOR_GRAY);
                oled_draw_box(bx, bar_top_y + bar_max_h - raw_h, bar_w, raw_h);
            }
            oled_set_draw_color(UI_COLOR_WHITE);
            oled_draw_frame(bx, bar_top_y, bar_w, bar_max_h);
        }

        oled_set_draw_color(UI_COLOR_GRAY);
        char _lbl[4] = {};
        snprintf(_lbl, sizeof(_lbl), "S%d", i + 1);
        int16_t lw = oled_get_str_width(_lbl);
        oled_draw_str(bx + (bar_w - lw) / 2, label_y + 13, _lbl);

        oled_set_draw_color(td->digital[i] ? UI_COLOR_MINT : UI_COLOR_GRAY);
        oled_draw_str(bx + (bar_w - 8) / 2, label_y + 29,
                       td->digital[i] ? "1" : "0");
    }

    /* ---- 底部状态 ---- */
    oled_set_draw_color(UI_COLOR_WHITE);
    char _st[40] = {};
    snprintf(_st, sizeof(_st), "LOST:%d X:%d TH:%d",
             track_is_lost(td),
             track_is_cross(td),
             track_get_threshold());
    oled_draw_UTF8(8, OLED_HEIGHT - 6, _st);
}

static void ui_track_exit(void)
{
    /* 无特殊清理 */
}

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

    astra_list_item_t *run_page   = astra_new_list_item("Run Control", power_icon);
    astra_list_item_t *pid_page   = astra_new_list_item("PID Tune", slider_icon);
    astra_list_item_t *uart_page  = astra_new_list_item("UART", switch_icon);
    astra_list_item_t *imu_page   = astra_new_list_item("IMU Gyro", slider_icon);
    astra_list_item_t *track_page = astra_new_list_item("Track Sensor", flag_icon);

    ui_push_item(root, run_page);
    ui_push_item(root, pid_page);
    ui_push_item(root, uart_page);
    ui_push_item(root, imu_page);
    ui_push_item(root, track_page);

    /* ===== Run Control ===== */
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

    /* ===== PID Tune ===== */
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

    /* ===== UART ===== */
    ui_push_item(uart_page,
        astra_new_switch_item("UART Enable", &s_uart_enable,
                              NULL, ui_uart_toggle_enable, switch_icon));
    ui_push_item(uart_page,
        astra_new_user_item("UART Info", ui_uart_init, ui_uart_loop, ui_uart_exit, list_icon));

    /* ===== IMU Gyro ===== */
    ui_push_item(imu_page,
        astra_new_user_item("IMU Data", ui_imu_init, ui_imu_loop, ui_imu_exit, slider_icon));

    /* ===== Track Sensor ===== */
    ui_push_item(track_page,
        astra_new_user_item("Sensor Bar", ui_track_init, ui_track_loop, ui_track_exit, flag_icon));
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
