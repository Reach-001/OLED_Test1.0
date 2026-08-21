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
#include "zf_device_bno085.h"

/* 任务标志由 PIT 中断置位，在 main.c 主循环中清零并执行。 */
volatile task_flag_t g_task_flag = {0};

/* 任务计数器 */
static volatile uint16 s_task_tick = 0;
static volatile uint32 s_task_ms = 0;

/* 系统状态 */
static sys_state_enum s_sys_state = SYS_STATE_IDLE;

static uint8 s_competition_task_id = 1;
static competition_task_param_t s_competition_task_params[COMPETITION_TASK_COUNT] = {
    { TARGET_SPEED_DEFAULT, 0, 0, 0 },
    { TARGET_SPEED_DEFAULT, 0, 0, 0 },
    { TARGET_SPEED_DEFAULT, 0, 0, 0 },
    { TARGET_SPEED_DEFAULT, 0, 0, 0 },
};

/* 循迹数据（非 static，允许 UI 等模块读取） */
track_data_t s_track_data;

/* 编码器数据 */
static int16 s_encoder_l = 0;
static int16 s_encoder_r = 0;

/* BNO085 不参与系统启动门控：IIC 异常时只影响 IMU 数据，不阻塞主循环。 */
static uint8 s_imu_ready = 0;
static uint8 s_imu_retrying = 0;
static uint8 s_imu_retry_count = 0;
static uint8 s_imu_disabled = 0;
static uint32 s_imu_next_retry_ms = 0;

/* 丢线计数 */
static uint16 s_lost_count = 0;
/* BUG FIX: 与 track_get_error 返回类型对齐，改为 int16 防止截断 */
static int16 s_last_error = 0;

static void task_imu_init_once(uint8 allow_disable);

/*-----------------------------------------------------------
 * 定时器回调 (PIT 中断中调用)
 *-----------------------------------------------------------*/
void task_timer_callback(void)
{
    s_task_tick++;
    s_task_ms += TASK_PERIOD_MS;

    /* 循迹采样任务 */
#if TRACK_ENABLE
    if (s_task_tick % TASK_TRACK_PERIOD == 0)
    {
        g_task_flag.track = 1;
    }
#endif

    /* 控制计算任务 */
    if (s_task_tick % TASK_CONTROL_PERIOD == 0)
    {
        g_task_flag.control = 1;
    }

    /* 显示刷新任务 */
#if LCD_ENABLE
    if (s_task_tick % TASK_DISPLAY_PERIOD == 0)
    {
        g_task_flag.display = 1;
    }
#endif

    /* UI 动画刷新任务 */
#if LCD_ENABLE
    if (s_task_tick % TASK_UI_PERIOD == 0)
    {
        g_task_flag.ui = 1;
    }
#endif

    /* 按键扫描任务 */
    if (s_task_tick % TASK_KEY_PERIOD == 0)
    {
        g_task_flag.key = 1;
    }

#if APP_UART_ENABLE
    if (s_task_tick % TASK_UART_PERIOD == 0)
    {
        g_task_flag.uart = 1;
    }
#endif

#if BNO085_ENABLE
    if (s_task_tick % TASK_IMU_PERIOD == 0)
    {
        g_task_flag.imu = 1;
    }
#endif

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

    s_imu_ready = 0;
    s_imu_retrying = 0;
    s_imu_retry_count = 0;
    s_imu_disabled = 0;
    s_imu_next_retry_ms = BNO085_INIT_RETRY_INTERVAL_MS;

#if BNO085_ENABLE
    /* BNO085 初始化包含上电等待和启动包清理，必须放在 PIT 启动前，
     * 避免运行期一次性阻塞循迹、按键和 UI 任务。 */
    task_imu_init_once(1);
#endif

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
#if TRACK_ENABLE
    /* 读取循迹传感器 */
    track_read(&s_track_data);
#else
    memset(&s_track_data, 0, sizeof(s_track_data));
#endif
}

/*-----------------------------------------------------------
 * 控制计算任务
 *-----------------------------------------------------------*/
void task_control(void)
{
#if ENCODER_ENABLE
    /* 读取编码器 */
    s_encoder_l = encoder_get_and_clear(ENCODER_LEFT);
    s_encoder_r = encoder_get_and_clear(ENCODER_RIGHT);
#else
    s_encoder_l = 0;
    s_encoder_r = 0;
#endif

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
    static uint32 s_last_status_print_ms = 0;

    /* UART0 被关时跳过 printf，否则 DL_UART_isBusy 永远忙等待导致卡死 */
    if (app_uart_get_state(0) == NULL || !app_uart_get_state(0)->enabled)
        return;

    if ((task_get_ms() - s_last_status_print_ms) < TASK_STATUS_PRINT_PERIOD_MS)
        return;
    s_last_status_print_ms = task_get_ms();

#if BNO085_ENABLE
    if (bno085_is_ready())
    {
        const bno085_imu_data_t *imu = bno085_get_imu_data();
        printf("S:%d E:%d L:%d R:%d IMU:%lu G:%lu Q:%lu Y:%ld\r\n",
               s_sys_state,
               s_last_error,
               s_encoder_l,
               s_encoder_r,
               (unsigned long)bno085_get_rx_count(),
               (unsigned long)bno085_get_gyro_update_count(),
               (unsigned long)bno085_get_quat_update_count(),
               (long)imu->yaw_md);
    }
    else
    {
        printf("S:%d E:%d L:%d R:%d IMU:E%u\r\n",
               s_sys_state,
               s_last_error,
               s_encoder_l,
               s_encoder_r,
               bno085_get_last_error());
    }
#else
    printf("S:%d E:%d L:%d R:%d\r\n",
           s_sys_state,
           s_last_error,
           s_encoder_l,
           s_encoder_r);
#endif
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

    /* 三个按键统一路由到 UI 处理 */
    for (uint8 i = BSP_KEY_1; i < BSP_KEY_NUM; i++)
    {
        bsp_key_event_enum evt = bsp_key_get_event((bsp_key_id_enum)i);
        if (evt != KEY_EVENT_NONE)
        {
            app_ui_handle_key((bsp_key_id_enum)i,
                              bsp_key_get_state((bsp_key_id_enum)i),
                              evt,
                              task_get_ms());
            bsp_key_clear_event((bsp_key_id_enum)i);
        }
    }
}

/*-----------------------------------------------------------
 * UART 轮询任务
 *-----------------------------------------------------------*/
void task_uart(void)
{
#if APP_UART_ENABLE
    app_uart_task();
#endif
}

static void task_imu_init_once(uint8 allow_disable)
{
#if BNO085_ENABLE
    if (bno085_init())
    {
        s_imu_retry_count++;
        if (allow_disable && (s_imu_retry_count >= BNO085_INIT_MAX_RETRY))
        {
            s_imu_disabled = 1;
            printf("[IMU] BNO085 absent, skipped, retry=%u/%u, err=%u\r\n",
                   s_imu_retry_count,
                   BNO085_INIT_MAX_RETRY,
                   bno085_get_last_error());
            return;
        }

        s_imu_retrying = 1;
        s_imu_next_retry_ms = task_get_ms() + BNO085_INIT_RETRY_INTERVAL_MS;
        printf("[IMU] BNO085 init failed, retry=%u/%u, err=%u\r\n",
               s_imu_retry_count,
               BNO085_INIT_MAX_RETRY,
               bno085_get_last_error());
        return;
    }

    s_imu_ready = 1;
    s_imu_retrying = 0;
    printf("[IMU] BNO085 ready, addr=0x%02X, retry=%u\r\n",
           bno085_get_i2c_addr(),
           s_imu_retry_count);
#else
    (void)allow_disable;
#endif
}

/*-----------------------------------------------------------
 * BNO085 轮询任务
 *-----------------------------------------------------------*/
void task_imu(void)
{
#if !BNO085_ENABLE
    return;
#else
    if (s_imu_disabled)
    {
        return;
    }

    if (!s_imu_ready)
    {
        if (s_imu_retrying && (task_get_ms() < s_imu_next_retry_ms))
        {
            return;
        }

        task_imu_init_once(1);
        if (!s_imu_ready)
            return;
    }

    (void)bno085_update();
#endif
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

void task_set_competition_task(uint8 task_id)
{
    if ((task_id < 1U) || (task_id > COMPETITION_TASK_COUNT))
        return;

    s_competition_task_id = task_id;
    control_set_target_speed(s_competition_task_params[task_id - 1U].speed);
}

uint8 task_get_competition_task(void)
{
    return s_competition_task_id;
}

competition_task_param_t *task_get_competition_param(uint8 task_id)
{
    if ((task_id < 1U) || (task_id > COMPETITION_TASK_COUNT))
        task_id = s_competition_task_id;

    return &s_competition_task_params[task_id - 1U];
}

/*-----------------------------------------------------------
 * 系统启动
 *-----------------------------------------------------------*/
void task_start(void)
{
    if (s_sys_state == SYS_STATE_READY || s_sys_state == SYS_STATE_STOP)
    {
        control_set_target_speed(s_competition_task_params[s_competition_task_id - 1U].speed);
        control_reset();
        control_enable(1);
        s_lost_count = 0;
        s_last_error = 0;
        s_sys_state = SYS_STATE_RUNNING;
        led_on();
        if (app_uart_get_state(0)->enabled) printf("TASK %u Started!\r\n", s_competition_task_id);
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
    if (app_uart_get_state(0)->enabled) printf("System Stopped!\r\n");
}
