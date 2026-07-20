/**
 * @file    board_config.h
 * @brief   硬件配置文件 - 集中管理所有引脚定义和参数配置
 *
 * @details 本文件是用户参数入口。新手改引脚、周期、PID、阈值时，
 *          优先只改这里，避免参数散落到各个模块。
 *
 * @note    引脚命名规范：模块名_功能_PIN
 */

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include "zf_common_headfile.h"

/*===========================================================================
 * 系统配置
 *===========================================================================*/
#define SYSTEM_CLOCK            SYSTEM_CLOCK_80M    /* 系统主频 */
#define DEBUG_UART_BAUD         115200              /* 调试串口波特率 */

/*===========================================================================
 * LED 指示灯配置
 *===========================================================================*/
#define LED_PIN                 B22                 /* 板载 LED */
#define LED_ON_LEVEL            1                   /* LED 点亮电平 (1=高电平亮) */
#define LED_SELF_TEST_TIMES     3                   /* 上电自检闪烁次数 */
#define LED_SELF_TEST_MS        100                 /* 单次亮灭时间 */
#define LED_RUN_TOGGLE_COUNT    10                  /* 运行状态 LED 翻转间隔，单位为控制周期 */

/*===========================================================================
 * 按键配置
 *===========================================================================*/
#define KEY1_PIN                B21                 /* 按键1 / UI */
#define KEY2_PIN                A1                  /* 按键2 */
#define KEY_ACTIVE_LEVEL        0                   /* 按键按下电平 (0=低电平触发) */
#define KEY_DEBOUNCE_COUNT      2                   /* 消抖次数，单位为按键扫描周期 */
#define KEY_LONG_PRESS_COUNT    50                  /* 长按次数，50 * 20ms = 1s */

/*===========================================================================
 * 电机配置 - 双路直流电机 (TB6612/DRV8833 驱动)
 *===========================================================================*/
#define MOTOR_ENABLE            0                   /* UI bring-up: keep motor pins inactive */

/* 电机1 (左电机) */
#define MOTOR_L_PWM             PWM_TIM_A0_CH0_B8   /* 左电机 PWM */
#define MOTOR_L_DIR1            B9                  /* 左电机方向1 */
#define MOTOR_L_DIR2            B10                 /* 左电机方向2 */

/* 电机2 (右电机) */
#define MOTOR_R_PWM             PWM_TIM_A0_CH1_B12  /* 右电机 PWM */
#define MOTOR_R_DIR1            B13                 /* 右电机方向1 */
#define MOTOR_R_DIR2            B14                 /* 右电机方向2 */

/* 电机参数 */
#define MOTOR_PWM_FREQ          10000               /* PWM 频率 (Hz) */
#define MOTOR_PWM_MAX           10000               /* PWM 最大值 (对应100%) */
#define MOTOR_DEADZONE          500                 /* 死区补偿 */

/*===========================================================================
 * 编码器配置 - 双路正交编码器
 *===========================================================================*/
/* 编码器1 (左轮) */
#define ENCODER_L_TIM           TIM_G8
#define ENCODER_L_CH1           TIMG8_ENCODER1_CH1_B6
#define ENCODER_L_CH2           TIMG8_ENCODER1_CH2_B7

/* 编码器2 (右轮) - 使用方向模式或另一组定时器 */
#define ENCODER_R_TIM           TIM_G7
#define ENCODER_R_CH1           TIMG7_ENCODER1_CH1_B15
#define ENCODER_R_DIR           B16

/* 编码器参数 */
#define ENCODER_RESOLUTION      1024                /* 编码器线数 * 4 (正交解码) */
#define WHEEL_DIAMETER          65                  /* 轮子直径 (mm) */
#define ENCODER_SAMPLE_MS       10                  /* 采样周期 (ms) */

/*===========================================================================
 * 循迹传感器配置 - 红外对管阵列
 *===========================================================================*/
/* 传感器数量。修改数量时，TRACK_WEIGHT_LIST 的数量必须同步。 */
#define TRACK_SENSOR_NUM        5

/* 传感器引脚 (从左到右) */
#define TRACK_SENSOR_1          ADC0_CH0_A27        /* 最左 */
#define TRACK_SENSOR_2          ADC0_CH1_A26
#define TRACK_SENSOR_3          ADC0_CH2_A25        /* 中间 */
#define TRACK_SENSOR_4          ADC0_CH3_A24
#define TRACK_SENSOR_5          ADC0_CH4_B25        /* 最右 */
#define TRACK_SENSOR_6          ADC0_CH5_B24        /* 扩展预留 */
#define TRACK_SENSOR_7          ADC0_CH6_B20        /* 扩展预留 */
#define TRACK_SENSOR_8          ADC0_CH7_A22        /* 扩展预留 */

/* 传感器参数 */
#define TRACK_THRESHOLD         2000                /* 黑白阈值 (需实际标定) */
#define TRACK_ADC_RESOLUTION    ADC_12BIT           /* ADC 分辨率 */
#define TRACK_ADC_FILTER_COUNT  3                   /* ADC 均值滤波采样次数 */
#define TRACK_LOST_MAX_COUNT    50                  /* 连续丢线保护次数，50 * 10ms = 500ms */
#define TRACK_WEIGHT_LIST       {-100, -50, 0, 50, 100}

/*===========================================================================
 * 串口通信配置
 *===========================================================================*/
/* 无线串口/蓝牙模块 */
#define WIRELESS_UART           UART_1
#define WIRELESS_UART_BAUD      115200
#define WIRELESS_UART_TX        UART1_TX_B4
#define WIRELESS_UART_RX        UART1_RX_B5

/*===========================================================================
 * OLED/LCD 显示屏配置 (可选)
 *===========================================================================*/
#define LCD_SPI_INDEX           SPI_1
#define LCD_SPI_SPEED           (30 * 1000 * 1000)
#define LCD_SCL_SPI_PIN         SPI1_SCK_B9
#define LCD_SDA_SPI_PIN         SPI1_MOSI_B8
#define LCD_SCL_PIN             B9
#define LCD_SDA_PIN             B8
#define LCD_RES_PIN             B10
#define LCD_DC_PIN              B11
#define LCD_CS_PIN              B14
#define LCD_BLK_PIN             B26

#define LCD_PANEL_114_ST7789    1                   /* 1.14 寸 ST7789: 240x135 */
#define LCD_PANEL_147_ST7789    2                   /* 1.47 寸 ST7789: 320x172 */
#define LCD_PANEL_TYPE          LCD_PANEL_114_ST7789

/*===========================================================================
 * 定时任务配置
 *===========================================================================*/
#define TASK_PIT                PIT_TIM_A1          /* 任务调度定时器（TIMA0 已被电机 PWM 占用，改用 TIMA1） */
#define TASK_PERIOD_MS          1                   /* 基础调度周期 (ms) */

/* 各任务执行周期。实际周期 = TASK_PERIOD_MS * TASK_xxx_PERIOD */
#define TASK_TRACK_PERIOD       2                   /* 循迹采样: 2ms */
#define TASK_CONTROL_PERIOD     5                   /* 控制计算: 5ms */
#define TASK_DISPLAY_PERIOD     50                  /* 显示刷新: 50ms */
#define TASK_UI_PERIOD          20                  /* UI 动画刷新: 20ms (50Hz) */
#define TASK_KEY_PERIOD         10                  /* 按键扫描: 10ms */
/* 新增任务时，在这里添加 TASK_xxx_PERIOD，再到 app_task.h/c 和 main.c 接入。 */
#define TASK_TICK_MAX           10000               /* 调度计数器回绕阈值 */

/*===========================================================================
 * PID 参数默认值 (可通过上位机/按键调参)
 *===========================================================================*/
/* 转向 PD 控制 */
#define PID_STEER_KP            50.0f
#define PID_STEER_KI            0.0f
#define PID_STEER_KD            10.0f
#define PID_STEER_INTEGRAL_MAX  1000.0f
#define PID_STEER_OUTPUT_MAX    5000.0f

/* 速度 PI 控制 */
#define PID_SPEED_KP            20.0f
#define PID_SPEED_KI            5.0f
#define PID_SPEED_KD            0.0f
#define PID_SPEED_INTEGRAL_MAX  5000.0f
#define PID_SPEED_OUTPUT_MAX    ((float)MOTOR_PWM_MAX)

/* 目标速度 */
#define TARGET_SPEED_DEFAULT    100                 /* 默认目标速度 */
#define TARGET_SPEED_MAX        300                 /* 最大速度限制 */
#define TARGET_SPEED_STEP       20                  /* 按键调速步进 */

#endif /* _BOARD_CONFIG_H_ */
