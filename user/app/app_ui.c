/**
 * @file    app_ui.c
 * @brief   B21 按键到 Astra UI 的适配层。
 * @details 本文件负责：
 *            1. 构建比赛 TASK 入口、任务参数和通用调试页面
 *            2. 将单键物理按键事件映射为 UI 操作
 *            3. 驱动 UI 的刷新循环
 *
 * @par 操作约定：
 *   - 单击：     切换下一个选项
 *   - KEY2 单击：进入/确认；根菜单 TASK 项为直接启动
 *   - 长按：     返回上一级
 *   - KEY2 长按：进入 TASK 参数页或子菜单
 *   - KEY1 长按：返回根菜单
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
#include "zf_device_bno085.h"
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

/** PID 调参值，使用整数显示，和当前控制模块参数保持一致 */
static int16_t s_ui_steer_kp = (int16_t)PID_STEER_KP;
static int16_t s_ui_steer_kd = (int16_t)PID_STEER_KD;
static int16_t s_ui_speed_kp = (int16_t)PID_SPEED_KP;
static int16_t s_ui_speed_ki = (int16_t)PID_SPEED_KI;

static astra_list_item_t *s_task_item[COMPETITION_TASK_COUNT] = {};
static char s_task_status_text[24] = {};

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

    /* 基于屏幕高度垂直居中内容块（约100px高），适配 1.14" 和 1.47" 两种面板 */
    int16_t _y0 = (OLED_HEIGHT - 100) / 2 + 8;  /* 首行基线 */

    /* 标题: UARTx + 开关状态 */
    oled_set_draw_color(UI_COLOR_WHITE);
    snprintf(b, sizeof(b), "UART%d  %s", c, st->enabled ? "ON" : "OFF");
    oled_draw_UTF8(8, _y0, b);

    /* 波特率 + 引脚 */
    oled_set_draw_color(UI_COLOR_GRAY);
    snprintf(b, sizeof(b), "BAUD: %u", st->baud);
    oled_draw_UTF8(8, _y0 + 20, b);
    snprintf(b, sizeof(b), "TX:%s RX:%s", app_uart_pin_name(st->tx_pin), app_uart_pin_name(st->rx_pin));
    oled_draw_UTF8(8, _y0 + 36, b);

    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_H_line(8, _y0 + 50, OLED_WIDTH - 16);

    /* UI 不显示累计字节数，避免长时间运行后数字变长挤占版面。 */
    snprintf(b, sizeof(b), "LAST RX: 0x%02X", st->last_rx);
    oled_draw_UTF8(8, _y0 + 66, b);
}

/* ---- 测试发送回调 ---- */
static void ui_uart_tx_test(void)
{
    app_uart_send_test(s_uart_no);
    astra_push_info_bar("TX OK", 600);
}

/*===========================================================================
 * 陀螺仪页面 — BNO085 数据显示
 *=========================================================================*/

static char s_imu_gyro_str[48] = {};
static char s_imu_angle_str[48] = {};
static char s_imu_info_str[48] = {};

/* RPY 零点偏移，单位：毫度（md），按 Roll/Pitch/Yaw 顺序 */
static int32 s_imu_rpy_offset_md[3] = {0};
static uint8 s_imu_page_active = 0;

/**
 * @brief 将角度差规范化到 [-180000, 180000] 毫度区间
 * @param angle_md  输入角度差，单位毫度
 * @return int32    规范化后的角度差
 */
static int32 ui_imu_wrap_angle_md(int32 angle_md)
{
    while (angle_md > 180000)
    {
        angle_md -= 360000;
    }
    while (angle_md < -180000)
    {
        angle_md += 360000;
    }
    return angle_md;
}

/**
 * @brief KEY1 短按归零 RPY：把当前 Roll/Pitch/Yaw 记为新的零点
 */
static void ui_imu_zero_rpy(void)
{
    const bno085_imu_data_t *imu = bno085_get_imu_data();

    s_imu_rpy_offset_md[0] = imu->roll_md;
    s_imu_rpy_offset_md[1] = imu->pitch_md;
    s_imu_rpy_offset_md[2] = imu->yaw_md;

    astra_push_info_bar("RPY ZERO", 600);
}

static void ui_imu_init(void)
{
    s_imu_page_active = 1;
}

static void ui_imu_loop(void)
{
    const bno085_imu_data_t *imu = bno085_get_imu_data();
    uint8 imu_ready = bno085_is_ready();
    uint8 zero_active = (s_imu_rpy_offset_md[0] != 0 ||
                         s_imu_rpy_offset_md[1] != 0 ||
                         s_imu_rpy_offset_md[2] != 0);

    st7789_set_font(astra_default_font);

    /* 第1行只保留固定长度状态，避免计数增长挤占后续显示。 */
    oled_set_draw_color(imu_ready ? UI_COLOR_MINT : UI_COLOR_AMBER);
    snprintf(s_imu_info_str, sizeof(s_imu_info_str),
             "BNO085%s %s A:%02X E:%u",
             zero_active ? "[Z]" : "",
             imu_ready ? "READY" : "NO IMU",
             bno085_get_i2c_addr(),
             bno085_get_last_error());
    oled_draw_UTF8(2, 16, s_imu_info_str);

    /* 第2行：陀螺仪角速度 dps */
    oled_set_draw_color(UI_COLOR_WHITE);
    snprintf(s_imu_gyro_str, sizeof(s_imu_gyro_str),
             "G:%6.2f %6.2f %6.2f",
             (float)imu->gyro.mdps_x / 1000.0f,
             (float)imu->gyro.mdps_y / 1000.0f,
             (float)imu->gyro.mdps_z / 1000.0f);
    oled_draw_UTF8(2, 32, s_imu_gyro_str);

    /* 第3行：加速度计 g */
    snprintf(s_imu_angle_str, sizeof(s_imu_angle_str),
             "A:%6.2f %6.2f %6.2f",
             (float)imu->accel_raw_x / 256.0f,
             (float)imu->accel_raw_y / 256.0f,
             (float)imu->accel_raw_z / 256.0f);
    oled_draw_UTF8(2, 48, s_imu_angle_str);

    /* 第4行：线性加速度计 g（去重力） */
    snprintf(s_imu_info_str, sizeof(s_imu_info_str),
             "L:%6.2f %6.2f %6.2f",
             (float)imu->lin_accel_raw_x / 256.0f,
             (float)imu->lin_accel_raw_y / 256.0f,
             (float)imu->lin_accel_raw_z / 256.0f);
    oled_draw_UTF8(2, 64, s_imu_info_str);

    /* 第5行：欧拉角 deg（减去零点偏移并做 [-180,180] 规范化） */
    snprintf(s_imu_angle_str, sizeof(s_imu_angle_str),
             "RPY:%6.2f %6.2f %6.2f",
             (float)ui_imu_wrap_angle_md(imu->roll_md - s_imu_rpy_offset_md[0]) / 1000.0f,
             (float)ui_imu_wrap_angle_md(imu->pitch_md - s_imu_rpy_offset_md[1]) / 1000.0f,
             (float)ui_imu_wrap_angle_md(imu->yaw_md - s_imu_rpy_offset_md[2]) / 1000.0f);
    oled_draw_UTF8(2, 80, s_imu_angle_str);

    /* 第6行：磁力计 uT */
    snprintf(s_imu_info_str, sizeof(s_imu_info_str),
             "M:%6.2f %6.2f %6.2f",
             (float)imu->mag_raw_x / 16.0f,
             (float)imu->mag_raw_y / 16.0f,
             (float)imu->mag_raw_z / 16.0f);
    oled_draw_UTF8(2, 96, s_imu_info_str);

    /* 第7行去掉累计计步，避免数字增长后遮挡边界。 */
    oled_set_draw_color(UI_COLOR_GRAY);
    snprintf(s_imu_info_str, sizeof(s_imu_info_str),
             "SB:%u AC:%u",
             (unsigned int)imu->stability_classifier,
             (unsigned int)imu->activity_classifier);
    oled_draw_UTF8(2, 112, s_imu_info_str);
}

static void ui_imu_exit(void)
{
    s_imu_page_active = 0;
}

/*===========================================================================
 * 循迹传感器页面 — 5路传感器 0/1 状态统计图
 *=========================================================================*/

/* 循迹数据通过 task_get_track_data() 获取，无需 extern */

static uint16_t ui_track_adc_max(void)
{
    return 255U;
}

static void ui_track_draw_centered_ascii(uint16_t x, uint16_t w, uint16_t y, const char *text)
{
    int16_t text_w = oled_get_str_width(text);
    int16_t text_x = (int16_t)x + ((int16_t)w - text_w) / 2;

    if (text_x < 0)
    {
        text_x = 0;
    }
    else if ((text_x + text_w) > OLED_WIDTH)
    {
        text_x = OLED_WIDTH - text_w;
    }

    oled_draw_str(text_x, y, text);
}

static void ui_track_init(void)
{
    /* 进入页面时无需特殊操作 */
}

static void ui_track_loop(void)
{
    const track_data_t *td = task_get_track_data();
    track_data_t *td_mut = (track_data_t *)td;  /* 仅用于兼容非 const API */
    uint16_t adc_max = ui_track_adc_max();
    uint16_t threshold = track_get_threshold();
    char digital_text[TRACK_SENSOR_NUM + 1] = {};

    st7789_set_font(astra_default_font);

    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(8, 18, "Track ADC");

    st7789_set_font((const void *)&font_8x16);

    oled_set_draw_color(UI_COLOR_GRAY);
    char _range[24] = {};
    snprintf(_range, sizeof(_range), "MAX:%u TH:%u",
             (unsigned int)adc_max,
             (unsigned int)threshold);
    oled_draw_str(120, 18, _range);

    /* ---- 原始 ADC 柱状图 ---- */
    uint16_t bar_area_w = OLED_WIDTH - 16;
    uint16_t bar_w      = bar_area_w / TRACK_SENSOR_NUM - 4;
    uint16_t bar_max_h  = 54;
    uint16_t bar_top_y  = 28;
    uint16_t label_y    = bar_top_y + bar_max_h + 18;
    uint16_t value_y    = label_y + 16;
    uint16_t th_h       = (uint16_t)((uint32_t)threshold * bar_max_h / adc_max);

    if (th_h > bar_max_h)
    {
        th_h = bar_max_h;
    }

    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        uint16_t bx = 8 + i * (bar_w + 4);
        uint16_t raw = td->raw[i];
        uint16_t raw_h;
        char _val[6] = {};

        if (raw > adc_max)
        {
            raw = adc_max;
        }

        raw_h = (uint16_t)((uint32_t)raw * bar_max_h / adc_max);
        if ((raw > 0U) && (raw_h == 0U))
        {
            raw_h = 1U;
        }

        oled_set_draw_color(UI_COLOR_WHITE);
        oled_draw_frame(bx, bar_top_y, bar_w, bar_max_h);

        if (raw_h > 0U)
        {
            oled_set_draw_color(td->digital[i] ? UI_COLOR_MINT : UI_COLOR_SKY);
            oled_draw_box(bx + 1,
                          bar_top_y + bar_max_h - raw_h,
                          bar_w - 2,
                          raw_h);
        }

        /* 阈值线按当前 ADC 量程映射到柱状图，便于直接判断黑白分界。 */
        if ((th_h > 0U) && (th_h < bar_max_h))
        {
            oled_set_draw_color(UI_COLOR_AMBER);
            oled_draw_H_line(bx, bar_top_y + bar_max_h - th_h, bar_w);
        }

        oled_set_draw_color(UI_COLOR_GRAY);
        char _lbl[4] = {};
        snprintf(_lbl, sizeof(_lbl), "%d", i);
        ui_track_draw_centered_ascii(bx, bar_w, label_y, _lbl);

        snprintf(_val, sizeof(_val), "%u", (unsigned int)td->raw[i]);
        oled_set_draw_color(td->digital[i] ? UI_COLOR_MINT : UI_COLOR_WHITE);
        ui_track_draw_centered_ascii(bx, bar_w, value_y, _val);
    }

    /* ---- 底部状态 ---- */
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        digital_text[i] = td->digital[i] ? '1' : '0';
    }

    oled_set_draw_color(UI_COLOR_WHITE);
    char _st[40] = {};
    snprintf(_st, sizeof(_st), "D:%s L:%d X:%d",
             digital_text,
             track_is_lost(td_mut),
             track_is_cross(td_mut));
    oled_draw_UTF8(8, OLED_HEIGHT - 6, _st);

    st7789_set_font(astra_default_font);
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

    s_ui_steer_kp = (int16_t)param->steer_pid.kp;
    s_ui_steer_kd = (int16_t)param->steer_pid.kd;
    s_ui_speed_kp = (int16_t)param->speed_pid.kp;
    s_ui_speed_ki = (int16_t)param->speed_pid.ki;
}

static void ui_show_state_action(void)
{
    const char *state = "?";

    switch (task_get_state())
    {
        case SYS_STATE_IDLE:    state = "IDLE";  break;
        case SYS_STATE_READY:   state = "READY"; break;
        case SYS_STATE_RUNNING: state = "RUN";   break;
        case SYS_STATE_STOP:    state = "STOP";  break;
        case SYS_STATE_ERROR:   state = "ERROR"; break;
        default: break;
    }

    snprintf(s_task_status_text, sizeof(s_task_status_text),
             "TASK %u %s",
             task_get_competition_task(),
             state);
    astra_push_pop_up(s_task_status_text, 1000);
}

static void ui_apply_pid_values(void)
{
    control_set_steer_pid((float)s_ui_steer_kp, PID_STEER_KI, (float)s_ui_steer_kd);
    control_set_speed_pid((float)s_ui_speed_kp, (float)s_ui_speed_ki, PID_SPEED_KD);
    astra_push_info_bar(control_save_tune_params() ? "SAVE ERR" : "PID SAVED", 800);
}

static uint8 ui_task_id_from_item(astra_list_item_t *item)
{
    while (item != NULL && item->parent != NULL && item->parent->parent != NULL)
    {
        item = item->parent;
    }

    for (uint8 i = 0; i < COMPETITION_TASK_COUNT; i++)
    {
        if (item == s_task_item[i])
            return (uint8)(i + 1U);
    }

    return 0;
}

static void ui_apply_competition_param(void)
{
    uint8 task_id = ui_task_id_from_item(astra_selector.selected_item);

    if (task_id != 0U)
    {
        competition_task_param_t *param = task_get_competition_param(task_id);
        if (task_get_competition_task() == task_id)
            control_set_target_speed(param->speed);
    }

    astra_push_info_bar("TASK PARAM OK", 600);
}

static bool ui_start_selected_competition_task(void)
{
    uint8 task_id = 0;

    for (uint8 i = 0; i < COMPETITION_TASK_COUNT; i++)
    {
        if (astra_selector.selected_item == s_task_item[i])
        {
            task_id = (uint8)(i + 1U);
            break;
        }
    }

    if (task_id == 0U)
        return false;

    if (task_get_state() == SYS_STATE_RUNNING)
        task_stop();

    task_set_competition_task(task_id);
    task_start();
    ui_show_state_action();
    return true;
}

static void ui_push_item(astra_list_item_t *parent, astra_list_item_t *child)
{
    if (!astra_push_item_to_list(parent, child))
        astra_push_info_bar("MENU FULL", 1000);
}

/*===========================================================================
 * UI 页面树构建（比赛入口 + 通用设置）
 *=========================================================================*/

/**
 * @brief 构建 Astra UI 的页面树
 * @note 根菜单只保留比赛 TASK 和 Setting，减少赛场误操作。
 */
static void ui_build_astra_tree(void)
{
    static uint8_t built = 0;
    if (built) return;
    built = 1;

    ui_sync_control_values();

    astra_list_item_t *root = astra_get_root_list();

    astra_list_item_t *setting_page = astra_new_list_item("Setting", list_icon);
    astra_list_item_t *pid_page     = astra_new_list_item("PID Tune", slider_icon);
    astra_list_item_t *sensors_page = astra_new_list_item("Sensors", flag_icon);
    astra_list_item_t *uart_page    = astra_new_list_item("UART", switch_icon);
    astra_list_item_t *imu_page     = astra_new_list_item("IMU Gyro", slider_icon);
    astra_list_item_t *track_page   = astra_new_list_item("Track Sensor", flag_icon);

    s_task_item[0] = astra_new_list_item("TASK: 1", flag_icon);
    s_task_item[1] = astra_new_list_item("TASK: 2", flag_icon);
    s_task_item[2] = astra_new_list_item("TASK: 3", flag_icon);
    s_task_item[3] = astra_new_list_item("TASK: 4", flag_icon);

    for (uint8 i = 0; i < COMPETITION_TASK_COUNT; i++)
    {
        competition_task_param_t *param = task_get_competition_param((uint8)(i + 1U));

        ui_push_item(root, s_task_item[i]);
        ui_push_item(s_task_item[i],
            astra_new_slider_item("Speed", &param->speed,
                                  TARGET_SPEED_STEP, 0, TARGET_SPEED_MAX,
                                  NULL, ui_apply_competition_param, slider_icon));
        ui_push_item(s_task_item[i],
            astra_new_slider_item("Parameter 1", &param->parameter1,
                                  1, -1000, 1000,
                                  NULL, ui_apply_competition_param, slider_icon));
        ui_push_item(s_task_item[i],
            astra_new_slider_item("Parameter 2", &param->parameter2,
                                  1, -1000, 1000,
                                  NULL, ui_apply_competition_param, slider_icon));
        ui_push_item(s_task_item[i],
            astra_new_slider_item("Parameter 3", &param->parameter3,
                                  1, -1000, 1000,
                                  NULL, ui_apply_competition_param, slider_icon));
    }

    ui_push_item(root, setting_page);
    ui_push_item(setting_page, pid_page);
    ui_push_item(setting_page, sensors_page);
    ui_push_item(setting_page, uart_page);

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

    /* ===== Sensors ===== */
    ui_push_item(sensors_page, track_page);
    ui_push_item(sensors_page, imu_page);

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
#if !LCD_ENABLE
    (void)key;
    (void)pressed;
    (void)event;
    (void)now_ms;
    return;
#else
    (void)pressed;
    (void)now_ms;

    if (event == KEY_EVENT_PRESS)
    {
        switch (key)
        {
            case BSP_KEY_1:  /* KEY1 单击 */
                if (s_imu_page_active)
                {
                    /* 在 IMU 页面短按 KEY1：归零 RPY */
                    ui_imu_zero_rpy();
                }
                else
                {
                    /* 其他页面：下一个选项 */
                    astra_selector_go_next_item();
                }
                break;
            case BSP_KEY_2:  /* KEY2 单击 → 进入/确认 */
                if (!ui_start_selected_competition_task())
                    astra_selector_jump_to_selected_item();
                break;
            case BSP_KEY_3:  /* KEY3 单击 → 上一个选项 */
                astra_selector_go_prev_item();
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
            case BSP_KEY_2:  /* KEY2 长按 → 进入子页 */
                if (astra_selector.selected_item != NULL
                    && astra_selector.selected_item->child_num > 0)
                    astra_enter_child_item(astra_selector.selected_item);
                break;
            case BSP_KEY_3:  /* KEY3 长按 → 返回上一级 */
                astra_selector_exit_current_item();
                break;
            default: break;
        }
    }
#endif
}

/*===========================================================================
 * 公共 API
 *=========================================================================*/

/*===========================================================================
 * 自定义标题回调 — 在 app_ui.c 统一管理所有页面标题
 *===========================================================================*/

static const char *ui_custom_title(astra_list_item_t *parent)
{
    /* 根菜单 / 无父节点 → 自定义根标题 */
    if (parent == NULL || parent->layer == 0)
        return "2026 电·赛!";

    /* 子菜单标题：根据父节点 content 映射更多样化的显示名。
     * 此处可直接改字符串，也可用 if/switch 按菜单分支返回不同文字。 */
    return parent->content;
}

/**
 * @brief UI 模块初始化
 */
void app_ui_init(void)
{
#if !LCD_ENABLE
    in_astra = false;
    return;
#else
    astra_ui_driver_init();

    /* 注册标题回调 — 在 build tree 之前设置 */
    astra_custom_title_cb = ui_custom_title;

    ui_draw_boot_logo();

    /* 从 app_uart 同步初始状态到 UI */
    for (uint8 i = 0; i < 4; i++) {
        const app_uart_state_t *st = app_uart_get_state(i);
        if (st != NULL) s_uart_en[i] = st->enabled;
    }

    ui_build_astra_tree();
    in_astra = true;
    astra_init_core();
    astra_push_info_bar("2026 电·赛", 800);
#endif
}

/**
 * @brief UI 任务主循环（约 20ms 调用一次）
 */
void app_ui_task(void)
{
#if !LCD_ENABLE
    return;
#else
    /* 仅在当前处于 Astra UI 模式时才刷新屏幕。
     * 用户可通过 ALLOW_EXIT_ASTRA_UI_BY_USER 退出 UI，
     * 退出后不再清屏和绘制，恢复外部显示内容。 */
    if (!in_astra) return;

    oled_clear_buffer();
    astra_ui_main_core();
    astra_ui_widget_core();
    oled_send_buffer();
    astra_draw_color_overlay();
#endif
}
