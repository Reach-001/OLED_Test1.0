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
#define KEY2_PIN                A28                 /* 按键2 */
#define KEY3_PIN                A0                  /* 按键3 */
#define KEY_ACTIVE_LEVEL        0                   /* 按键按下电平 (0=低电平触发) */
#define KEY_DEBOUNCE_COUNT      2                   /* 消抖次数，单位为按键扫描周期 */
#define KEY_LONG_PRESS_COUNT    30                  /* 长按次数，50 * 20ms = 1s */

/*===========================================================================
 * 电机配置 - 双路直流电机 (TB6612/DRV8833 驱动)
 *===========================================================================*/
#define MOTOR_ENABLE            0                   /* UI bring-up: keep motor pins inactive */
#define ENCODER_ENABLE          0                   /* 编码器未接时可改 0，系统仍正常运行 */
#define TRACK_ENABLE            1                   /* 循迹板未接时可改 0，跳过 ADC 采样 */
#define LCD_ENABLE              1                   /* 屏幕未接时可改 0，跳过 ST7789 初始化和刷新 */
#define APP_UART_ENABLE         1                   /* 外部通信 UART 未接时可改 0，跳过应用串口轮询 */

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
 * 循迹传感器配置 - ADS7830 8 路 IIC 灰度采样
 *===========================================================================*/
#define TRACK_SENSOR_NUM        8
#define TRACK_ADS7830_FILTER_COUNT  3               /* 单通道均值滤波采样次数 */
#define TRACK_LOST_MAX_COUNT    50                  /* 连续丢线保护次数，50 * 10ms = 500ms */
#define TRACK_WEIGHT_LIST       {-100, -75, -50, -25, 25, 50, 75, 100}

/* ADS7830 输出为 8 位，阈值范围 0~255。
 * ADS7830 独立使用 B24/B25，避免与 BNO085 的 B17/B18 软件 IIC 冲突。 */
#define ADS7830_SCL_PIN         B24
#define ADS7830_SDA_PIN         B25
#define ADS7830_SOFT_IIC_DELAY  80
#define TRACK_ADS7830_POWER_MODE ADS7830_INTERNAL_REF_OFF_ADC_ON
#define TRACK_ADS7830_THRESHOLD 128

/*===========================================================================
 * 串口通信配置
 *===========================================================================*/
/* 无线串口/蓝牙模块 */
#define WIRELESS_UART           UART_1
#define WIRELESS_UART_BAUD      115200
#define WIRELESS_UART_TX        UART1_TX_B4
#define WIRELESS_UART_RX        UART1_RX_B5

/*===========================================================================
 * BNO085 IMU 配置
 *===========================================================================*/
#define BNO085_ENABLE                   1             /* 未接或初始化失败时自动跳过，不阻塞系统 */
#define BNO085_INIT_MAX_RETRY           1             /* 初始化失败后跳过，避免主循环每秒阻塞重试 */
#define BNO085_INIT_RETRY_INTERVAL_MS   1000U         /* 仅在允许多次重试时生效，单位 ms */
#define BNO085_USE_SOFT_IIC             1             /* 默认软件 IIC，避开硬件 I2C1 状态机兼容问题 */
#define BNO085_SOFT_IIC_DELAY           80            /* 软件 IIC 延时，优先保证 BNO085 启动阶段稳定 */
#define BNO085_STARTUP_DELAY_MS         300U          /* 上电等待，过短会漏启动包，过长会拖慢 APP 初始化 */
#define BNO085_STARTUP_FLUSH_MS         80U           /* 清启动包时间；软件 IIC 下不宜长时间占用主循环 */
#define BNO085_FEATURE_FLUSH_MS         80U           /* SetFeature 后清控制包时间 */
#define BNO085_IIC_SPEED                (100 * 1000)  /* BNO085_USE_SOFT_IIC=0 时生效 */
#define BNO085_SCL_PIN                  B17           /* 软件 IIC 默认接线: BNO085 SCL */
#define BNO085_SDA_PIN                  B18           /* 软件 IIC 默认接线: BNO085 SDA */
#define BNO085_SCL_AF                   GPIO_AF4      /* 硬件 I2C1 时需把引脚改为 B2/PB2 */
#define BNO085_SDA_AF                   GPIO_AF4      /* 硬件 I2C1 时需把引脚改为 B3/PB3 */

/* BNO085 各传感器报告输出频率，单位 Hz。
 * 软件 IIC 下报告频率过高会抢占控制循环，先用低频稳定读取。
 * 扩展报告设为 0 表示不发送 SetFeature，避免启动阶段一次堆积过多长包。 */
#define BNO085_GYRO_RATE_HZ             20
#define BNO085_ROTATION_VECTOR_RATE_HZ  10
#define BNO085_ACCEL_RATE_HZ            0
#define BNO085_LINEAR_ACCEL_RATE_HZ     0
#define BNO085_MAG_RATE_HZ              0
#define BNO085_STEP_RATE_HZ             0
#define BNO085_STABILITY_RATE_HZ        0
#define BNO085_ACTIVITY_RATE_HZ         0

/* IMU 轮询任务周期，应高于最高报告频率，避免 BNO085 FIFO 堆积。 */
#define TASK_IMU_PERIOD         20                  /* BNO085 轮询: 20ms (50Hz) */
#define TASK_STATUS_PRINT_PERIOD_MS 200U            /* 串口状态输出降频，避免 printf 阻塞主循环 */
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
#define TASK_UART_PERIOD        5                   /* UART 轮询: 5ms */
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
