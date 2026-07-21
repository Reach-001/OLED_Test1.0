/**
 * @file    app_task.c
 * @brief   任务调度模块实现
 */

#include "app_task.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_track.h"
#include "app_control.h"
#include "app_ui.h"
#include "app_uart.h"

/* 任务标志由 PIT 中断置位，在 main.c 主循环中清零并执行。 */
volatile task_flag_t g_task_flag = {0};

/* 任务计数器 */
static volatile uint16 s_task_tick = 0;
static volatile uint32 s_task_ms = 0;

/* 系统状态 */
static sys_state_enum s_sys_state = SYS_STATE_IDLE;

/* 循迹数据（非 static，允许 UI 等模块读取） */
track_data_t s_track_data;

/* 编码器数据 */
static int16 s_encoder_l = 0;
static int16 s_encoder_r = 0;

/* 丢线计数 */
static uint16 s_lost_count = 0;
/* BUG FIX: 与 track_get_error 返回类型对齐，改为 int16 防止截断 */
static int16 s_last_error = 0;

/*-----------------------------------------------------------
 * 定时器回调 (PIT 中断中调用)
 *-----------------------------------------------------------*/
void task_timer_callback(void)
{
    s_task_tick++;
    s_task_ms += TASK_PERIOD_MS;

    /* 循迹采样任务 */
    if (s_task_tick % TASK_TRACK_PERIOD == 0)
    {
        g_task_flag.track = 1;
    }

    /* 控制计算任务 */
    if (s_task_tick % TASK_CONTROL_PERIOD == 0)
    {
        g_task_flag.control = 1;
    }

    /* 显示刷新任务 */
    if (s_task_tick % TASK_DISPLAY_PERIOD == 0)
    {
        g_task_flag.display = 1;
    }

    /* UI 动画刷新任务 */
    if (s_task_tick % TASK_UI_PERIOD == 0)
    {
        g_task_flag.ui = 1;
    }

    /* 按键扫描任务 */
    if (s_task_tick % TASK_KEY_PERIOD == 0)
    {
        g_task_flag.key = 1;
    }

    if (s_task_tick % TASK_UART_PERIOD == 0)
    {
        g_task_flag.uart = 1;
    }

    /* 计数器溢出处理 */
    if (s_task_tick >= TASK_TICK_MAX)
    {
        s_task_tick = 0;
    }
}

const track_data_t* task_get_track_data(void)
{
    return &s_track_data;
}

uint32 task_get_ms(void)
{
    return s_task_ms;
}

/*-----------------------------------------------------------
 * PIT 中断回调包装
 *-----------------------------------------------------------*/
static void pit_callback(uint32 flag, void *param)
{
    (void)flag;
    (void)param;
    task_timer_callback();
}

/*-----------------------------------------------------------
 * 任务调度初始化
 *-----------------------------------------------------------*/
void task_init(void)
{
    app_uart_init();

    /* 初始化定时器 */
    pit_ms_init(TASK_PIT, TASK_PERIOD_MS, pit_callback, NULL);

    /* 初始化状态 */
    s_sys_state = SYS_STATE_READY;
}

/*-----------------------------------------------------------
 * 循迹采样任务
 *-----------------------------------------------------------*/
void task_track(void)
{
    /* 读取循迹传感器 */
    track_read(&s_track_data);
}

/*-----------------------------------------------------------
 * 控制计算任务
 *-----------------------------------------------------------*/
void task_control(void)
{
    /* 读取编码器 */
    s_encoder_l = encoder_get_and_clear(ENCODER_LEFT);
    s_encoder_r = encoder_get_and_clear(ENCODER_RIGHT);

    /* 非运行状态强制关电机，避免上电或暂停时误输出 PWM。 */
    if (s_sys_state != SYS_STATE_RUNNING)
    {
        motor_stop_all();
        return;
    }

    /* 检测丢线 */
    if (track_is_lost(&s_track_data))
    {
        s_lost_count++;
        if (s_lost_count > TRACK_LOST_MAX_COUNT)
        {
            /* 停车或按上次方向继续 */
            task_stop();
            return;
        }
    }
    else
    {
        s_lost_count = 0;
    }

    /* 计算循迹偏差
     * BUG FIX: int8 改为 int16，与 track_get_error 返回类型一致，防止截断 */
    int16 error;
    if (track_is_lost(&s_track_data))
    {
        /* 丢线时使用上次的偏差 (保持方向) */
        error = s_last_error;
    }
    else
    {
        error = track_get_error(&s_track_data);
        s_last_error = error;
    }

    /* 控制计算 */
    int16 motor_l, motor_r;
    control_run(error, s_encoder_l, s_encoder_r, &motor_l, &motor_r);

    /* 输出到电机 */
    motor_set_speed_both(motor_l, motor_r);

    /* LED 指示运行状态 */
    static uint8 led_cnt = 0;
    if (++led_cnt >= LED_RUN_TOGGLE_COUNT)
    {
        led_cnt = 0;
        led_toggle();
    }
}

/*-----------------------------------------------------------
 * 显示刷新任务
 *-----------------------------------------------------------*/
void task_display(void)
{
    /* BUG FIX: 原代码重新调用 track_get_error，丢线时返回 0，与控制逻辑
     *          使用的 s_last_error（保持上次有效值）不一致，调试时误导。
     *          改为直接打印 s_last_error，与控制逻辑保持一致。 */
    printf("S:%d E:%d L:%d R:%d\r\n",
           s_sys_state,
           s_last_error,
           s_encoder_l,
           s_encoder_r);
}

/*-----------------------------------------------------------
 * UI 动画刷新任务
 *-----------------------------------------------------------*/
void task_ui(void)
{
    app_ui_task();
}

/*-----------------------------------------------------------
 * 按键处理任务
 *-----------------------------------------------------------*/
void task_key(void)
{
    bsp_key_scan();
    app_ui_handle_key(
        bsp_key_get_state(BSP_KEY_1),
        bsp_key_get_event(BSP_KEY_1),
        task_get_ms());
    bsp_key_clear_event(BSP_KEY_1);

    if (bsp_key_get_event(BSP_KEY_2) == KEY_EVENT_PRESS)
    {
        bsp_key_clear_event(BSP_KEY_2);
    }

    if (bsp_key_get_event(BSP_KEY_3) == KEY_EVENT_PRESS)
    {
        bsp_key_clear_event(BSP_KEY_3);
    }
}

/*-----------------------------------------------------------
 * UART 轮询任务
 *-----------------------------------------------------------*/
void task_uart(void)
{
    app_uart_task();
}

/*-----------------------------------------------------------
 * 设置系统状态
 *-----------------------------------------------------------*/
void task_set_state(sys_state_enum state)
{
    s_sys_state = state;
}

/*-----------------------------------------------------------
 * 获取系统状态
 *-----------------------------------------------------------*/
sys_state_enum task_get_state(void)
{
    return s_sys_state;
}

/*-----------------------------------------------------------
 * 系统启动
 *-----------------------------------------------------------*/
void task_start(void)
{
    if (s_sys_state == SYS_STATE_READY || s_sys_state == SYS_STATE_STOP)
    {
        control_reset();
        control_enable(1);
        s_lost_count = 0;
        s_last_error = 0;
        s_sys_state = SYS_STATE_RUNNING;
        led_on();
        printf("System Started!\r\n");
    }
}

/*-----------------------------------------------------------
 * 系统停止
 *-----------------------------------------------------------*/
void task_stop(void)
{
    control_enable(0);
    motor_stop_all();
    s_sys_state = SYS_STATE_STOP;
    led_off();
    printf("System Stopped!\r\n");
}
