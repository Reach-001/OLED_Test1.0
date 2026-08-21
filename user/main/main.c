/**
 * @file    main.c
 * @brief   主程序入口 - 循迹小车
 *
 * @details 采用模块化设计：
 *          - config/  配置层：硬件引脚、参数定义
 *          - bsp/     驱动层：LED、电机、编码器、循迹传感器、按键
 *          - app/     应用层：控制算法、任务调度
 *
 * @note    修改硬件连接只需修改 board_config.h
 */

#include "zf_common_headfile.h"

/* BSP 层 */
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_track.h"

/* APP 层 */
#include "app_control.h"
#include "app_task.h"
#include "app_ui.h"

/*-----------------------------------------------------------
 * 系统初始化
 *-----------------------------------------------------------*/
static void system_init(void)
{
    /* 时钟初始化（必须第一个调用） */
    clock_init(SYSTEM_CLOCK);

    /* 最早启动指示：放在串口和外设初始化前，用于定位启动卡点。 */
    led_init();
    led_blink(LED_SELF_TEST_TIMES, LED_SELF_TEST_MS);

    /* 调试串口初始化 */
    debug_init();

    printf("\r\n");
    printf("========================================\r\n");
    printf("  MSPM0G3507 Line Follower Car\r\n");
    printf("  Build: %s %s\r\n", __DATE__, __TIME__);
    printf("========================================\r\n");
}

/*-----------------------------------------------------------
 * BSP 层初始化
 *-----------------------------------------------------------*/
static void bsp_init(void)
{
    printf("[BSP] Initializing...\r\n");

    led_init();
    bsp_key_init();
    motor_init();
#if ENCODER_ENABLE
    encoder_init();
#endif
#if TRACK_ENABLE
    track_init();
#endif

    printf("[BSP] Done.\r\n");
}

/*-----------------------------------------------------------
 * APP 层初始化
 *-----------------------------------------------------------*/
static void app_init(void)
{
    printf("[APP] Initializing...\r\n");

    control_init();
    task_init();
    app_ui_init();

    printf("[APP] Done.\r\n");
}

/*-----------------------------------------------------------
 * 开机自检
 *-----------------------------------------------------------*/
static void self_test(void)
{
    printf("[TEST] Self testing...\r\n");

#if TRACK_ENABLE
    /* 自检只打印实际配置的传感器数量，避免修改 TRACK_SENSOR_NUM 后越界。 */
    track_data_t track;
    track_read(&track);
    printf("[TEST] Track:");
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        printf(" %d", track.raw[i]);
    }
    printf("\r\n");
#else
    printf("[TEST] Track skipped.\r\n");
#endif

    printf("[TEST] Ready. Press KEY1 to start.\r\n");
}

/*-----------------------------------------------------------
 * 主函数
 *-----------------------------------------------------------*/
int main(void)
{
    /*==================== 初始化 ====================*/
    system_init();
    bsp_init();
    self_test();
    app_init();

    /*==================== 主循环 ====================*/
    while (1)
    {
        /* 任务模型：中断只置位，主循环执行。
         * 新增任务时，仿照下面的 if 块清标志并调用 task_xxx()。
         * 任务函数中不要写长时间阻塞，否则其他任务会被拖慢。
         */

        /* 循迹采样任务 */
        if (g_task_flag.track)
        {
            g_task_flag.track = 0;
            task_track();
        }

        /* 控制计算任务 */
        if (g_task_flag.control)
        {
            g_task_flag.control = 0;
            task_control();
        }

        /* 按键处理任务 */
        if (g_task_flag.key)
        {
            g_task_flag.key = 0;
            task_key();
        }

        /* UART 轮询任务 */
        if (g_task_flag.uart)
        {
            g_task_flag.uart = 0;
            task_uart();
        }

        if (g_task_flag.imu)
        {
            g_task_flag.imu = 0;
            task_imu();
        }

        /* UI 动画刷新任务 */
        if (g_task_flag.ui)
        {
            g_task_flag.ui = 0;
            task_ui();
        }

        /* 显示刷新任务 */
        if (g_task_flag.display)
        {
            g_task_flag.display = 0;
            task_display();
        }
    }
}
