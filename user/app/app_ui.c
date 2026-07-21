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
#include "app_uart.h"
#include "boot_logo_bitmap.h"
#include <stdbool.h>
#include <stdio.h>

/*===========================================================================
 * 开机 Logo 动画
 *===========================================================================*/

/**
 * @brief 绘制竞赛 Logo 彩图
 * @note  开机彩图直接写入 ST7789 GRAM，避免受 1-bit UI 帧缓冲限制。
 */
static void ui_draw_boot_logo_image(int16_t x, int16_t y)
{
    const uint16_t width = BOOT_LOGO_BITMAP_WIDTH;
    const uint16_t height = BOOT_LOGO_BITMAP_HEIGHT;

    st7789_set_window((uint16_t)x, (uint16_t)y,
                      (uint16_t)(x + (int16_t)width - 1),
                      (uint16_t)(y + (int16_t)height - 1));

    uint32_t target_pixels = (uint32_t)width * height;
    uint32_t written_pixels = 0;
    uint32_t rle_offset = 0;

    while (written_pixels < target_pixels && rle_offset < BOOT_LOGO_RLE4_SIZE)
    {
        uint8_t code = g_boot_logo_rle4[rle_offset++];
        uint8_t color_index;
        uint8_t count;

        if (code & 0xF0U)
        {
            count = code >> 4;
            color_index = code & 0x0FU;
        }
        else
        {
            color_index = code & 0x0FU;
            count = g_boot_logo_rle4[rle_offset++];
        }

        uint32_t remain = target_pixels - written_pixels;
        uint32_t draw_count = (count > remain) ? remain : count;

        for (uint32_t i = 0; i < draw_count; i++)
        {
            st7789_write_data16(g_boot_logo_palette_rgb565[color_index]);
        }

        written_pixels += draw_count;
    }
}

/**
 * @brief 绘制开机 Logo — 电子设计竞赛黑底彩图
 * @note 调用后阻塞约 1.3 秒，展示后自动进入主菜单。
 */
static void ui_draw_boot_logo(void)
{
    int16_t logo_x = (OLED_WIDTH - BOOT_LOGO_BITMAP_WIDTH) / 2;
    int16_t logo_y = (OLED_HEIGHT - BOOT_LOGO_BITMAP_HEIGHT) / 2;

    st7789_set_buffer_mode(0);
    oled_clear_buffer();
    ui_draw_boot_logo_image(logo_x, logo_y);
    delay(1200);

    oled_clear_buffer();
    st7789_set_buffer_mode(1);
    oled_clear_buffer();
    delay(100);
}

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
 * UART 四通道 — 开关 + 长按进详情
 *===========================================================================*/

static bool  s_uart_en[4] = { false, false, false, false };  /* 启动时从 app_uart_get_state 同步 */
static uint8 s_uart_no    = 0;

/* ---- 选中开关时设置通道号 (init_function) ---- */
static void ui_u0_in(void) { s_uart_no = 0; }
static void ui_u1_in(void) { s_uart_no = 1; }
static void ui_u2_in(void) { s_uart_no = 2; }
static void ui_u3_in(void) { s_uart_no = 3; }

/* ---- KEY2 单击开关 → 翻转值 + 硬件初始化 ---- */
static void ui_u0_sw(void) { app_uart_set_enable(0, s_uart_en[0]); }
static void ui_u1_sw(void) { app_uart_set_enable(1, s_uart_en[1]); }
static void ui_u2_sw(void) { app_uart_set_enable(2, s_uart_en[2]); }
static void ui_u3_sw(void) { app_uart_set_enable(3, s_uart_en[3]); }

/* ---- 详情页 (KEY2 长按进入) ---- */
static void ui_uart_exit(void) {}
static void ui_uart_info_loop(void)
{
    uint8 c = s_uart_no;
    const app_uart_state_t *st = app_uart_get_state(c);
    if (st == NULL) return;

    st7789_set_font(astra_default_font);
    char b[32] = {};

    /* 标题: UARTx + 开关状态 */
    oled_set_draw_color(UI_COLOR_WHITE);
    snprintf(b, sizeof(b), "UART%d  %s", c, st->enabled ? "ON" : "OFF");
    oled_draw_UTF8(8, 22, b);

    /* 波特率 + 引脚 */
    oled_set_draw_color(UI_COLOR_GRAY);
    snprintf(b, sizeof(b), "BAUD: %u", st->baud);
    oled_draw_UTF8(8, 42, b);
    snprintf(b, sizeof(b), "TX:%s RX:%s", app_uart_pin_name(st->tx_pin), app_uart_pin_name(st->rx_pin));
    oled_draw_UTF8(8, 58, b);

    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_H_line(8, 72, OLED_WIDTH - 16);

    /* TX 发送字节数 */
    snprintf(b, sizeof(b), "TX: %u", st->tx_count);
    oled_draw_UTF8(8, 88, b);
    /* RX 接收: 字节数 + 最后收到的字节 */
    snprintf(b, sizeof(b), "RX: %u  LAST:0x%02X", st->rx_count, st->last_rx);
    oled_draw_UTF8(8, 106, b);
}

/* ---- 测试发送回调 ---- */
static void ui_uart_tx_test(void)
{
    app_uart_send_test(s_uart_no);
    astra_push_info_bar("TX OK", 600);
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
    track_data_t *td_mut = (track_data_t *)td;  /* 仅用于兼容非 const API */

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
             track_is_lost(td_mut),
             track_is_cross(td_mut),
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

    /* ===== UART — 4开关，KEY2长按进详情 ===== */
    astra_list_item_t *u0 = astra_new_switch_item("UART0", &s_uart_en[0], ui_u0_in, ui_u0_sw, switch_icon);
    astra_list_item_t *u1 = astra_new_switch_item("UART1", &s_uart_en[1], ui_u1_in, ui_u1_sw, switch_icon);
    astra_list_item_t *u2 = astra_new_switch_item("UART2", &s_uart_en[2], ui_u2_in, ui_u2_sw, switch_icon);
    astra_list_item_t *u3 = astra_new_switch_item("UART3", &s_uart_en[3], ui_u3_in, ui_u3_sw, switch_icon);
    ui_push_item(uart_page, u0); ui_push_item(uart_page, u1);
    ui_push_item(uart_page, u2); ui_push_item(uart_page, u3);

    ui_push_item(u0, astra_new_button_item("Test TX", ui_uart_tx_test, flag_icon));
    ui_push_item(u0, astra_new_user_item("Params",   ui_u0_in, ui_uart_info_loop, ui_uart_exit, list_icon));

    ui_push_item(u1, astra_new_button_item("Test TX", ui_uart_tx_test, flag_icon));
    ui_push_item(u1, astra_new_user_item("Params",   ui_u1_in, ui_uart_info_loop, ui_uart_exit, list_icon));

    ui_push_item(u2, astra_new_button_item("Test TX", ui_uart_tx_test, flag_icon));
    ui_push_item(u2, astra_new_user_item("Params",   ui_u2_in, ui_uart_info_loop, ui_uart_exit, list_icon));

    ui_push_item(u3, astra_new_button_item("Test TX", ui_uart_tx_test, flag_icon));
    ui_push_item(u3, astra_new_user_item("Params",   ui_u3_in, ui_uart_info_loop, ui_uart_exit, list_icon));

    /* ===== IMU Gyro ===== */
    ui_push_item(imu_page,
        astra_new_user_item("IMU Data", ui_imu_init, ui_imu_loop, ui_imu_exit, slider_icon));

    /* ===== Track Sensor ===== */
    ui_push_item(track_page,
        astra_new_user_item("Sensor Bar", ui_track_init, ui_track_loop, ui_track_exit, flag_icon));
}

/*===========================================================================
 * 三键交互: KEY1=下一个, KEY2=进入/切换, KEY3=返回, KEY1长按=根菜单
 *===========================================================================*/

/**
 * @brief 按键事件处理
 */
void app_ui_handle_key(bsp_key_id_enum key, uint8 pressed, bsp_key_event_enum event, uint32 now_ms)
{
    (void)pressed;
    (void)now_ms;

    if (event == KEY_EVENT_PRESS)
    {
        switch (key)
        {
            case BSP_KEY_1:  /* KEY1 单击 → 下一个选项 */
                astra_selector_go_next_item();
                break;
            case BSP_KEY_2:  /* KEY2 单击 → 进入/确认 */
                astra_selector_jump_to_selected_item();
                break;
            case BSP_KEY_3:  /* KEY3 单击 → 返回上一级 */
                astra_selector_exit_current_item();
                break;
            default: break;
        }
    }
    else if (event == KEY_EVENT_LONG_PRESS)
    {
        switch (key)
        {
            case BSP_KEY_1:  /* KEY1 长按 → 回到根菜单 */
                astra_init_list();
                astra_push_info_bar("MAIN", 600);
                break;
            case BSP_KEY_2:  /* KEY2 长按 → 进入子页（不切开关） */
                if (astra_selector.selected_item != NULL
                    && astra_selector.selected_item->child_num > 0)
                    astra_enter_child_item(astra_selector.selected_item);
                break;
            default: break;
        }
    }
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
    ui_draw_boot_logo();

    /* 从 app_uart 同步初始状态到 UI */
    for (uint8 i = 0; i < 4; i++) {
        const app_uart_state_t *st = app_uart_get_state(i);
        if (st != NULL) s_uart_en[i] = st->enabled;
    }

    ui_build_astra_tree();
    in_astra = true;
    astra_init_core();
    astra_push_info_bar("2026 Dian_Sai_Demo", 800);
}

/**
 * @brief UI 任务主循环（约 20ms 调用一次）
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
