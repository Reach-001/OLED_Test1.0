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
#include "zf_driver_uart.h"
#include <stdbool.h>
#include <stdio.h>

/*===========================================================================
 * 开机 Logo 动画
 *===========================================================================*/

/**
 * @brief 绘制开机 Logo — 赛车 + 标题 + 进度条
 * @note 调用后阻塞约 2 秒，展示动画后自动进入主菜单
 */
static void ui_draw_boot_logo(void)
{
    int16_t cx = OLED_WIDTH  / 2;   /**< 屏幕水平中点 */
    int16_t cy = OLED_HEIGHT / 2;   /**< 屏幕垂直中点 */

    /* ---- 第1帧: 标题文字浮现 ---- */
    oled_clear_buffer();
    oled_set_draw_color(UI_COLOR_WHITE);

    /* 项目名称 */
    st7789_set_font(astra_default_font);
    oled_draw_UTF8(cx - 48, cy - 48, "Dian Sai");
    oled_draw_UTF8(cx - 32, cy - 28, "2026");

    /* 分割线 */
    oled_draw_H_line(cx - 60, cy - 12, 120);

    /* 平台信息 */
    st7789_set_font(astra_default_font);
    oled_draw_str(cx - 32, cy + 6, "MSPM0G3507");

    oled_send_buffer();
    delay(600);

    /* ---- 第2帧: 赛车图标出现 ---- */
    oled_clear_buffer();
    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(cx - 48, cy - 48, "Dian Sai");
    oled_draw_UTF8(cx - 32, cy - 28, "2026");
    oled_draw_H_line(cx - 60, cy - 12, 120);

    /* === 车身（简化赛车俯视图）=== */
    int16_t car_y = cy + 10;

    /* 底盘 */
    oled_draw_R_box(cx - 50, car_y - 12, 100, 24, 6);

    /* 车头（梯形，用横线叠加模拟） */
    oled_draw_H_line(cx + 38, car_y - 15, 14);
    oled_draw_H_line(cx + 36, car_y - 14, 16);
    oled_draw_H_line(cx + 34, car_y - 13, 16);
    oled_draw_H_line(cx + 38, car_y - 16, 12);

    /* 尾翼 */
    oled_draw_box(cx - 60, car_y - 18, 16, 4);

    /* 前轮 */
    oled_draw_box(cx + 34, car_y + 12, 12, 6);
    oled_set_draw_color(UI_COLOR_BLACK);
    oled_draw_box(cx + 37, car_y + 13, 6, 4);
    oled_set_draw_color(UI_COLOR_WHITE);

    /* 后轮 */
    oled_draw_box(cx - 46, car_y + 12, 12, 6);
    oled_set_draw_color(UI_COLOR_BLACK);
    oled_draw_box(cx - 43, car_y + 13, 6, 4);
    oled_set_draw_color(UI_COLOR_WHITE);

    /* 车窗 */
    oled_draw_box(cx - 10, car_y - 9, 18, 8);

    oled_send_buffer();
    delay(600);

    /* ---- 第3帧: 进度条动画 ---- */
    uint8_t bar_target = 120;

    for (uint8_t step = 0; step <= bar_target; step += 4)
    {
        oled_clear_buffer();

        /* 标题区 */
        oled_set_draw_color(UI_COLOR_WHITE);
        oled_draw_UTF8(cx - 48, cy - 48, "Dian Sai");
        oled_draw_UTF8(cx - 32, cy - 28, "2026");
        oled_draw_H_line(cx - 60, cy - 12, 120);

        /* 赛车图标（简化版，复用帧2逻辑） */
        oled_draw_R_box(cx - 50, car_y - 12, 100, 24, 6);
        oled_draw_H_line(cx + 38, car_y - 15, 14);
        oled_draw_H_line(cx + 36, car_y - 14, 16);
        oled_draw_H_line(cx + 34, car_y - 13, 16);
        oled_draw_H_line(cx + 38, car_y - 16, 12);
        oled_draw_box(cx - 60, car_y - 18, 16, 4);
        oled_draw_box(cx + 34, car_y + 12, 12, 6);
        oled_set_draw_color(UI_COLOR_BLACK);
        oled_draw_box(cx + 37, car_y + 13, 6, 4);
        oled_set_draw_color(UI_COLOR_WHITE);
        oled_draw_box(cx - 46, car_y + 12, 12, 6);
        oled_set_draw_color(UI_COLOR_BLACK);
        oled_draw_box(cx - 43, car_y + 13, 6, 4);
        oled_set_draw_color(UI_COLOR_WHITE);
        oled_draw_box(cx - 10, car_y - 9, 18, 8);

        /* 进度条背景 */
        oled_set_draw_color(UI_COLOR_GRAY);
        oled_draw_frame(cx - 62, OLED_HEIGHT - 24, 124, 10);
        oled_set_draw_color(UI_COLOR_BLACK);
        oled_draw_box(cx - 60, OLED_HEIGHT - 22, 120, 6);

        /* 进度条填充（渐变增长） */
        oled_set_draw_color(UI_COLOR_WHITE);
        oled_draw_box(cx - 60, OLED_HEIGHT - 22, step, 6);

        oled_send_buffer();

        /* 进度条速度控制: 前快后慢 ~500ms */
        if (step < 60)      delay(15);
        else if (step < 100) delay(20);
        else                 delay(30);
    }

    /* 进度条填满后短暂停留 */
    delay(200);

    /* 清屏过渡 */
    oled_clear_buffer();
    oled_send_buffer();
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
 * UART 四通道独立控制
 *===========================================================================*/

/** 常用波特率列表 */
static const uint32 s_uart_baud_list[] = {
    9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
};
#define UART_BAUD_COUNT (sizeof(s_uart_baud_list) / sizeof(s_uart_baud_list[0]))

/** 单个 UART 通道的状态 */
typedef struct {
    bool     enabled;       /**< 通道使能 */
    uint8    baud_idx;      /**< 波特率在 s_uart_baud_list 中的索引 */
    uint32   tx_count;      /**< TX 字节计数 */
    uint32   rx_count;      /**< RX 字节计数 */
    uint32   tx_pin;        /**< TX 引脚枚举值 */
    uint32   rx_pin;        /**< RX 引脚枚举值 */
    bool     inited;        /**< 是否已调用 uart_init */
} ui_uart_chan_t;

/* 默认引脚配置: UART0=A0/A1, UART1=B4/B5, UART2=A21/A22, UART3=B12/B13 */
static ui_uart_chan_t s_uart_chan[4] = {
    { false, 4, 0, 0, UART0_TX_A0,  UART0_RX_A1,  false },
    { false, 4, 0, 0, UART1_TX_B4,  UART1_RX_B5,  false },
    { false, 4, 0, 0, UART2_TX_A21, UART2_RX_A22, false },
    { false, 4, 0, 0, UART3_TX_B12, UART3_RX_B13, false },
};

/** 当前正在显示的 UART 通道索引 (0-3)，供各控件的回调使用 */
static uint8 s_uart_cur_chan = 0;

/* ---- 引脚名映射 ---- */
static const char *ui_uart_pin_name(uint32 pin)
{
    uint16 idx = pin & UART_PIN_INDEX_MASK;
    switch (idx) {
        case A0:  return "A0";  case A1:  return "A1";
        case B4:  return "B4";  case B5:  return "B5";
        case A21: return "A21"; case A22: return "A22";
        case B12: return "B12"; case B13: return "B13";
        default:  return "??";
    }
}

/* ---- 各通道的 init / sync 回调 ---- */
static void ui_uart0_sync(void) { s_uart_cur_chan = 0; }
static void ui_uart1_sync(void) { s_uart_cur_chan = 1; }
static void ui_uart2_sync(void) { s_uart_cur_chan = 2; }
static void ui_uart3_sync(void) { s_uart_cur_chan = 3; }
static void (*s_uart_sync_list[4])(void) = {
    ui_uart0_sync, ui_uart1_sync, ui_uart2_sync, ui_uart3_sync
};

/* ---- 通用 Enable 开关回调 ---- */
static void ui_uart_apply_enable(void)
{
    ui_uart_chan_t *ch = &s_uart_chan[s_uart_cur_chan];
    if (ch->enabled && !ch->inited) {
        uart_init((uart_index_enum)s_uart_cur_chan,
                  s_uart_baud_list[ch->baud_idx],
                  (uart_tx_pin_enum)ch->tx_pin,
                  (uart_rx_pin_enum)ch->rx_pin);
        ch->inited = true;
    }
    astra_push_info_bar(ch->enabled ? "UART ON" : "UART OFF", 600);
}

/* ---- UART 实时信息页 loop ---- */
static void ui_uart_info_init(void) {}
static void ui_uart_info_exit(void) {}

static void ui_uart_info_loop(void)
{
    ui_uart_chan_t *ch = &s_uart_chan[s_uart_cur_chan];

    st7789_set_font(astra_default_font);
    oled_set_draw_color(UI_COLOR_WHITE);

    char _b[40] = {};
    snprintf(_b, sizeof(_b), "UART%d  %s",
             s_uart_cur_chan,
             ch->enabled ? "ON" : "OFF");
    oled_draw_UTF8(8, 22, _b);

    oled_set_draw_color(UI_COLOR_GRAY);
    snprintf(_b, sizeof(_b), "BAUD: %u", s_uart_baud_list[ch->baud_idx]);
    oled_draw_UTF8(8, 42, _b);

    snprintf(_b, sizeof(_b), "TX:%s  RX:%s",
             ui_uart_pin_name(ch->tx_pin),
             ui_uart_pin_name(ch->rx_pin));
    oled_draw_UTF8(8, 60, _b);

    /* 分隔线 */
    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_H_line(8, 72, OLED_WIDTH - 16);

    /* TX 统计 */
    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(8,  88, "TX Bytes:");
    snprintf(_b, sizeof(_b), "%u", ch->tx_count);
    oled_set_draw_color(UI_COLOR_MINT);
    oled_draw_UTF8(80, 88, _b);

    /* RX 统计 */
    oled_set_draw_color(UI_COLOR_WHITE);
    oled_draw_UTF8(8,  106, "RX Bytes:");
    snprintf(_b, sizeof(_b), "%u", ch->rx_count);
    oled_set_draw_color(UI_COLOR_MINT);
    oled_draw_UTF8(80, 106, _b);
}

/* ---- 波特率切换按钮 ---- */
static void ui_uart_baud_next(void)
{
    ui_uart_chan_t *ch = &s_uart_chan[s_uart_cur_chan];
    ch->baud_idx = (ch->baud_idx + 1) % UART_BAUD_COUNT;
    ch->inited = false;  /* 波特率变了下次使能时需要重新 init */
    char _m[32] = {};
    snprintf(_m, sizeof(_m), "BAUD %u", s_uart_baud_list[ch->baud_idx]);
    astra_push_info_bar(_m, 800);
}

/* ---- TX 测试发送按钮 ---- */
static void ui_uart_test_tx(void)
{
    ui_uart_chan_t *ch = &s_uart_chan[s_uart_cur_chan];
    if (!ch->enabled || !ch->inited) {
        astra_push_info_bar("UART OFF", 600);
        return;
    }
    uart_write_string((uart_index_enum)s_uart_cur_chan, "Hello UART\r\n");
    ch->tx_count += 12;
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

    /* ===== UART — 四通道独立控制 ===== */
    /* 每个 UARTx 子页面: Enable开关 + Baud切换 + 实时信息 + 测试发送 */
    astra_list_item_t *uart0_page = astra_new_list_item("UART0 A0/A1", switch_icon);
    astra_list_item_t *uart1_page = astra_new_list_item("UART1 B4/B5", switch_icon);
    astra_list_item_t *uart2_page = astra_new_list_item("UART2 A21/A22", switch_icon);
    astra_list_item_t *uart3_page = astra_new_list_item("UART3 B12/B13", switch_icon);
    ui_push_item(uart_page, uart0_page);
    ui_push_item(uart_page, uart1_page);
    ui_push_item(uart_page, uart2_page);
    ui_push_item(uart_page, uart3_page);

    /* UART0 */
    ui_push_item(uart0_page,
        astra_new_switch_item("Enable", &s_uart_chan[0].enabled,
                              ui_uart0_sync, ui_uart_apply_enable, switch_icon));
    ui_push_item(uart0_page,
        astra_new_button_item("Switch Baud", ui_uart_baud_next, plus_icon));
    ui_push_item(uart0_page,
        astra_new_user_item("Live Info", ui_uart_info_init, ui_uart_info_loop,
                            ui_uart_info_exit, list_icon));
    ui_push_item(uart0_page,
        astra_new_button_item("Test TX", ui_uart_test_tx, flag_icon));

    /* UART1 */
    ui_push_item(uart1_page,
        astra_new_switch_item("Enable", &s_uart_chan[1].enabled,
                              ui_uart1_sync, ui_uart_apply_enable, switch_icon));
    ui_push_item(uart1_page,
        astra_new_button_item("Switch Baud", ui_uart_baud_next, plus_icon));
    ui_push_item(uart1_page,
        astra_new_user_item("Live Info", ui_uart_info_init, ui_uart_info_loop,
                            ui_uart_info_exit, list_icon));
    ui_push_item(uart1_page,
        astra_new_button_item("Test TX", ui_uart_test_tx, flag_icon));

    /* UART2 */
    ui_push_item(uart2_page,
        astra_new_switch_item("Enable", &s_uart_chan[2].enabled,
                              ui_uart2_sync, ui_uart_apply_enable, switch_icon));
    ui_push_item(uart2_page,
        astra_new_button_item("Switch Baud", ui_uart_baud_next, plus_icon));
    ui_push_item(uart2_page,
        astra_new_user_item("Live Info", ui_uart_info_init, ui_uart_info_loop,
                            ui_uart_info_exit, list_icon));
    ui_push_item(uart2_page,
        astra_new_button_item("Test TX", ui_uart_test_tx, flag_icon));

    /* UART3 */
    ui_push_item(uart3_page,
        astra_new_switch_item("Enable", &s_uart_chan[3].enabled,
                              ui_uart3_sync, ui_uart_apply_enable, switch_icon));
    ui_push_item(uart3_page,
        astra_new_button_item("Switch Baud", ui_uart_baud_next, plus_icon));
    ui_push_item(uart3_page,
        astra_new_user_item("Live Info", ui_uart_info_init, ui_uart_info_loop,
                            ui_uart_info_exit, list_icon));
    ui_push_item(uart3_page,
        astra_new_button_item("Test TX", ui_uart_test_tx, flag_icon));

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

    /* 开机 Logo 动画：赛车图标 + 进度条，约 2 秒 */
    ui_draw_boot_logo();

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
