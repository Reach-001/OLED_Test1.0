/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_lfss.h
 *  @brief      LFSS(Low Frequency Sub-System) Peripheral Interface
 *              LFSS(低频子系统)外设接口头文件，提供 RTC、防篡改检测(TIO)、
 *              独立异步看门狗(IWDT)及便签存储器(SPM) 的全部寄存器级操作 API。
 *  @defgroup   LFSS Low Frequeuncy Sub-System (LFSS)
 *
 *  @anchor ti_devices_msp_dl_lfss_Overview
 *  # Overview
 *  The Low Frequency Sub-System (LFSS) Driver Library allows full configuration
 *  of the MSPM0 LFSS module.
 *  The LFSS IP enables a separate, dedicated battery supply used for
 *  maintaining continuous operation of real time clock (RTC), tamper detection
 *  input / output (TIO) module, an independent asynchronous watchdog timer
 *  (IWDT) and a small scratchpad memory storage (SPM).
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup LFSS
 * @{
 */
#ifndef ti_dl_dl_lfss__include
#define ti_dl_dl_lfss__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#if defined(__MSPM0_HAS_LFSS__) || defined(DOXYGEN__INCLUDE)

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */


/** @addtogroup DL_LFSS_TAMPERIO_INTERRUPT
 *  @brief 防篡改 IO 中断掩码位定义
 *  @{
 */

/**
 * @brief LFSS Tamper IO 0 interrupt
 *        防篡改 IO 0 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_0                   (LFSS_CPU_INT_IMASK_TIO0_SET)

/**
 * @brief LFSS Tamper IO 1 interrupt
 *        防篡改 IO 1 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_1                   (LFSS_CPU_INT_IMASK_TIO1_SET)

/**
 * @brief LFSS Tamper IO 2 interrupt
 *        防篡改 IO 2 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_2                   (LFSS_CPU_INT_IMASK_TIO2_SET)

/**
 * @brief LFSS Tamper IO 3 interrupt
 *        防篡改 IO 3 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_3                   (LFSS_CPU_INT_IMASK_TIO3_SET)

/**
 * @brief LFSS Tamper IO 4 interrupt
 *        防篡改 IO 4 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_4                   (LFSS_CPU_INT_IMASK_TIO4_SET)

/**
 * @brief LFSS Tamper IO 5 interrupt
 *        防篡改 IO 5 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_5                   (LFSS_CPU_INT_IMASK_TIO6_SET)

/**
 * @brief LFSS Tamper IO 6 interrupt
 *        防篡改 IO 6 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_6                   (LFSS_CPU_INT_IMASK_TIO6_SET)

/**
 * @brief LFSS Tamper IO 7 interrupt
 *        防篡改 IO 7 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_7                   (LFSS_CPU_INT_IMASK_TIO7_SET)

/**
 * @brief LFSS Tamper IO 8 interrupt
 *        防篡改 IO 8 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_8                   (LFSS_CPU_INT_IMASK_TIO8_SET)

/**
 * @brief LFSS Tamper IO 9 interrupt
 *        防篡改 IO 9 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_9                   (LFSS_CPU_INT_IMASK_TIO9_SET)

/**
 * @brief LFSS Tamper IO 10 interrupt
 *        防篡改 IO 10 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_10                   (LFSS_CPU_INT_IMASK_TIO10_SET)

/**
 * @brief LFSS Tamper IO 11 interrupt
 *        防篡改 IO 11 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_11                   (LFSS_CPU_INT_IMASK_TIO11_SET)

/**
 * @brief LFSS Tamper IO 12 interrupt
 *        防篡改 IO 12 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_12                   (LFSS_CPU_INT_IMASK_TIO12_SET)

/**
 * @brief LFSS Tamper IO 13 interrupt
 *        防篡改 IO 13 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_13                   (LFSS_CPU_INT_IMASK_TIO13_SET)

/**
 * @brief LFSS Tamper IO 14 interrupt
 *        防篡改 IO 14 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_14                   (LFSS_CPU_INT_IMASK_TIO14_SET)

/**
 * @brief LFSS Tamper IO 15 interrupt
 *        防篡改 IO 15 中断
 */
#define DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_15                   (LFSS_CPU_INT_IMASK_TIO15_SET)

/** @}*/

/** @addtogroup DL_LFSS_TAMPERIO_EVENT
 *  @brief 防篡改 IO 事件掩码位定义
 *  @{
 */
/**
 * @brief LFSS Tamper IO 0 event
 *        防篡改 IO 0 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_0                   (LFSS_GEN_EVENT_IMASK_TIO0_SET)

/**
 * @brief LFSS Tamper IO 1 event
 *        防篡改 IO 1 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_1                   (LFSS_GEN_EVENT_IMASK_TIO1_SET)

/**
 * @brief LFSS Tamper IO 2 event
 *        防篡改 IO 2 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_2                   (LFSS_GEN_EVENT_IMASK_TIO2_SET)

/**
 * @brief LFSS Tamper IO 3 event
 *        防篡改 IO 3 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_3                   (LFSS_GEN_EVENT_IMASK_TIO3_SET)

/**
 * @brief LFSS Tamper IO 4 event
 *        防篡改 IO 4 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_4                   (LFSS_GEN_EVENT_IMASK_TIO4_SET)

/**
 * @brief LFSS Tamper IO 5 event
 *        防篡改 IO 5 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_5                   (LFSS_GEN_EVENT_IMASK_TIO5_SET)

/**
 * @brief LFSS Tamper IO 6 event
 *        防篡改 IO 6 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_6                   (LFSS_GEN_EVENT_IMASK_TIO6_SET)

/**
 * @brief LFSS Tamper IO 7 event
 *        防篡改 IO 7 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_7                   (LFSS_GEN_EVENT_IMASK_TIO7_SET)

/**
 * @brief LFSS Tamper IO 8 event
 *        防篡改 IO 8 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_8                   (LFSS_GEN_EVENT_IMASK_TIO8_SET)

/**
 * @brief LFSS Tamper IO 9 event
 *        防篡改 IO 9 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_9                   (LFSS_GEN_EVENT_IMASK_TIO9_SET)

/**
 * @brief LFSS Tamper IO 10 event
 *        防篡改 IO 10 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_10                   (LFSS_GEN_EVENT_IMASK_TIO10_SET)

/**
 * @brief LFSS Tamper IO 11 event
 *        防篡改 IO 11 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_11                   (LFSS_GEN_EVENT_IMASK_TIO11_SET)

/**
 * @brief LFSS Tamper IO 12 event
 *        防篡改 IO 12 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_12                   (LFSS_GEN_EVENT_IMASK_TIO12_SET)

/**
 * @brief LFSS Tamper IO 13 event
 *        防篡改 IO 13 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_13                   (LFSS_GEN_EVENT_IMASK_TIO13_SET)

/**
 * @brief LFSS Tamper IO 14 event
 *        防篡改 IO 14 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_14                   (LFSS_GEN_EVENT_IMASK_TIO14_SET)

/**
 * @brief LFSS Tamper IO 15 event
 *        防篡改 IO 15 事件
 */
#define DL_LFSS_TAMPERIO_EVENT_TAMPERIO_15                   (LFSS_GEN_EVENT_IMASK_TIO15_SET)

/** @}*/

/**
 * @brief Individual Tamper IO pin enable
 *        单个防篡改 IO 引脚使能值
 */
#define LFSS_TAMPER_IO_PIN_ENABLE 0x1

/**
 * @brief Individual Tamper IO pin mask
 *        单个防篡改 IO 引脚掩码
 */
#define LFSS_TAMPER_IO_PIN_MASK 0x1

/*! @enum DL_LFSS_TAMPERIO
 *  @brief 防篡改 IO 引脚编号枚举，共 16 个引脚(TIO0~TIO15)
 */
typedef enum {
    /*! LFSS Tamper IO 0 防篡改 IO 引脚 0 */
    DL_LFSS_TAMPERIO_0 = 0x0000,
    /*! LFSS Tamper IO 1 防篡改 IO 引脚 1 */
    DL_LFSS_TAMPERIO_1 = 0x0001,
    /*! LFSS Tamper IO 2 防篡改 IO 引脚 2 */
    DL_LFSS_TAMPERIO_2 = 0x0002,
    /*! LFSS Tamper IO 3 防篡改 IO 引脚 3 */
    DL_LFSS_TAMPERIO_3 = 0x0003,
    /*! LFSS Tamper IO 4 防篡改 IO 引脚 4 */
    DL_LFSS_TAMPERIO_4 = 0x0004,
    /*! LFSS Tamper IO 5 防篡改 IO 引脚 5 */
    DL_LFSS_TAMPERIO_5 = 0x0005,
    /*! LFSS Tamper IO 6 防篡改 IO 引脚 6 */
    DL_LFSS_TAMPERIO_6 = 0x0006,
    /*! LFSS Tamper IO 7 防篡改 IO 引脚 7 */
    DL_LFSS_TAMPERIO_7 = 0x0007,
    /*! LFSS Tamper IO 8 防篡改 IO 引脚 8 */
    DL_LFSS_TAMPERIO_8 = 0x0008,
    /*! LFSS Tamper IO 9 防篡改 IO 引脚 9 */
    DL_LFSS_TAMPERIO_9 = 0x0009,
    /*! LFSS Tamper IO 10 防篡改 IO 引脚 10 */
    DL_LFSS_TAMPERIO_10 = 0x000A,
    /*! LFSS Tamper IO 11 防篡改 IO 引脚 11 */
    DL_LFSS_TAMPERIO_11 = 0x000B,
    /*! LFSS Tamper IO 12 防篡改 IO 引脚 12 */
    DL_LFSS_TAMPERIO_12 = 0x000C,
    /*! LFSS Tamper IO 13 防篡改 IO 引脚 13 */
    DL_LFSS_TAMPERIO_13 = 0x000D,
    /*! LFSS Tamper IO 14 防篡改 IO 引脚 14 */
    DL_LFSS_TAMPERIO_14 = 0x000E,
    /*! LFSS Tamper IO 15 防篡改 IO 引脚 15 */
    DL_LFSS_TAMPERIO_15 = 0x000F,
} DL_LFSS_TAMPERIO;

/*! @enum DL_LFSS_TAMPERIO_VALUE
 *  @brief 防篡改 IO 数据值枚举
 */
typedef enum {
    /*! LFSS Tamper IO Value is 0 防篡改 IO 值为 0 */
    DL_LFSS_TAMPERIO_VALUE_0 = 0x00000,
    /*! LFSS Tamper IO Value is 1 防篡改 IO 值为 1 */
    DL_LFSS_TAMPERIO_VALUE_1 = 0x0001,
} DL_LFSS_TAMPERIO_VALUE;

/*! @enum DL_LFSS_TAMPERIO_OUTPUT_SOURCE
 *  @brief 防篡改 IO 输出源选择枚举
 */
typedef enum {
    /*! The TOUT register is the source for the tamper output (TOUT) control
     *  TOUT 寄存器作为输出源 */
    DL_LFSS_TAMPERIO_OUTPUT_SOURCE_TOUT = LFSS_TIOCTL_TOUTSEL_TOUT,
    /*! LFCLK is the source for the tamper output (TOUT) control
     *  低频时钟(LFCLK)作为输出源 */
    DL_LFSS_TAMPERIO_OUTPUT_SOURCE_LFCLK = LFSS_TIOCTL_TOUTSEL_LFCLKEXT,
    /*! The heart beat generator is the source for the tamper output (TOUT)
     *  control 心跳发生器作为输出源 */
    DL_LFSS_TAMPERIO_OUTPUT_SOURCE_HEARTBEAT = LFSS_TIOCTL_TOUTSEL_HEARTBEAT,
    /*! The time stamp event is the source for the tamper output (TOUT)
     *  control 时间戳事件作为输出源 */
    DL_LFSS_TAMPERIO_OUTPUT_SOURCE_TIME_STAMP_EVENT = LFSS_TIOCTL_TOUTSEL_TSEVTSTAT,
} DL_LFSS_TAMPERIO_OUTPUT_SOURCE;

/*! @enum DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH
 *  @brief 防篡改 IO 数字毛刺滤波宽度枚举，用于过滤短脉冲干扰
 */
typedef enum {
    /*! No filtering on the Tamper IO beyond CDS synchronization sample
     *  除 CDS 同步采样外无额外滤波 */
    DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_NONE = LFSS_TIOCTL_FILTEREN_NO_FLT,
    /*! Pulses on the Tamper IO less than 30us are filtered
     *  滤除宽度小于 30us 的脉冲 */
    DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_30_USEC = LFSS_TIOCTL_FILTEREN_FLT_1,
    /*! Pulses on the Tamper IO less than 100us are filtered
     *  滤除宽度小于 100us 的脉冲 */
    DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_100_USEC = LFSS_TIOCTL_FILTEREN_FLT_2,
    /*! Pulses on the Tamper IO less than 200us are filtered
     *  滤除宽度小于 200us 的脉冲 */
    DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_200_USEC = LFSS_TIOCTL_FILTEREN_FLT_3,
} DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH;

/*! @enum DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY
 *  @brief 防篡改 IO 边沿检测极性枚举
 */
typedef enum {
    /*! Edge detection polarity is disabled 禁用边沿检测 */
    DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_DISABLE = LFSS_TIOCTL_POLARITY_DISABLE,
    /*! Enables rising edge detection of input event 使能上升沿检测 */
    DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_RISE = LFSS_TIOCTL_POLARITY_RISE,
    /*! Enables falling edge detection of input event 使能下降沿检测 */
    DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_FALL = LFSS_TIOCTL_POLARITY_FALL,
    /*! Enables both rising and falling edge detection of input event
     *  使能双沿(上升沿+下降沿)检测 */
    DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_BOTH = LFSS_TIOCTL_POLARITY_BOTH,

} DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY;

/*! @enum DL_LFSS_TAMPERIO_MODE
 *  @brief 防篡改 IO 工作模式枚举
 */
typedef enum {
    /*! The Tamper IO is set to tamper mode 防篡改模式，由 VBAT 域控制 */
    DL_LFSS_TAMPERIO_MODE_TAMPER = LFSS_TIOCTL_IOMUX_TAMPER,
    /*! The Tamper IO is set to IOMUX mode IOMUX 复用模式，连接到 SoC */
    DL_LFSS_TAMPERIO_MODE_IOMUX = LFSS_TIOCTL_IOMUX_IOMUX
} DL_LFSS_TAMPERIO_MODE;

/*! @enum DL_LFSS_HEARTBEAT_INTERVAL
 *  @brief 心跳发生器周期间隔枚举
 */
typedef enum {
    /*! Heart beat generator interval is 0.125 sec 心跳间隔 0.125 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_0_125_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT0P125,
    /*! Heart beat generator interval is 0.25 sec 心跳间隔 0.25 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_0_25_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT0P25,
    /*! Heart beat generator interval is 0.5 sec 心跳间隔 0.5 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_05_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT0P5,
    /*! Heart beat generator interval is 1 sec 心跳间隔 1 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_1_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT1,
    /*! Heart beat generator interval is 2 sec 心跳间隔 2 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_2_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT2,
    /*! Heart beat generator interval is 4 sec 心跳间隔 4 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_4_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT4,
    /*! Heart beat generator interval is 8 sec 心跳间隔 8 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_8_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT8,
    /*! Heart beat generator interval is 16 sec 心跳间隔 16 秒 */
    DL_LFSS_HEARTBEAT_INTERVAL_16_SEC = LFSS_HEARTBEAT_HBINTERVAL_HBINT16,
} DL_LFSS_HEARTBEAT_INTERVAL;

/*! @enum DL_LFSS_HEARTBEAT_PULSE_WIDTH
 *  @brief 心跳发生器脉冲宽度枚举
 */
typedef enum {
    /*! Heart beat generator pulse width is 1 msec 心跳脉宽 1 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_1_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH1,
    /*! Heart beat generator pulse width is 2 msec 心跳脉宽 2 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_2_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH2,
    /*! Heart beat generator pulse width is 4 msec 心跳脉宽 4 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_4_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH4,
    /*! Heart beat generator pulse width is 8 msec 心跳脉宽 8 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_8_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH8,
    /*! Heart beat generator pulse width is 16 msec 心跳脉宽 16 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_16_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH16,
    /*! Heart beat generator pulse width is 32 msec 心跳脉宽 32 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_32_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH32,
    /*! Heart beat generator pulse width is 64 msec 心跳脉宽 64 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_64_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH64,
    /*! Heart beat generator pulse width is 128 msec 心跳脉宽 128 毫秒 */
    DL_LFSS_HEARTBEAT_PULSE_WIDTH_128_MSEC = LFSS_HEARTBEAT_HBWIDTH_HBPWDTH128,
} DL_LFSS_HEARTBEAT_PULSE_WIDTH;

/*! @enum DL_LFSS_HEARTBEAT_MODE
 *  @brief 心跳发生器工作模式枚举
 */
typedef enum {
    /*! Heart beat generator mode is disabled 禁用心跳发生器 */
    DL_LFSS_HEARTBEAT_MODE_DISABLED = LFSS_HEARTBEAT_HBMODE_HB_DIS,
    /*! Heart beat generator mode is always enabled 始终使能心跳 */
    DL_LFSS_HEARTBEAT_MODE_ALWAYS_ON = LFSS_HEARTBEAT_HBMODE_HB_ALLWAYS,
    /*! Heart beat generator mode is enabled when time stamp event is detected
     *  检测到时间戳事件时使能心跳 */
    DL_LFSS_HEARTBEAT_MODE_TIME_STAMP = LFSS_HEARTBEAT_HBMODE_HB_TS,
    /*! Heart beat generator mode is enabled when the main power supply fails
     *  主电源失效时使能心跳 */
    DL_LFSS_HEARTBEAT_MODE_POWER_FAIL = LFSS_HEARTBEAT_HBMODE_HB_VDDFAIL,
} DL_LFSS_HEARTBEAT_MODE;

/*! @enum DL_LFSS_TAMPERIO_IIDX
 *  @brief 防篡改 IO 中断索引枚举，用于读取最高优先级待处理中断
 */
typedef enum {
    /*! LFSS Tamper interrupt index for Tamper IO 0 防篡改 IO 0 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_0 = LFSS_CPU_INT_IIDX_STAT_TIO0,
    /*! LFSS Tamper interrupt index for Tamper IO 1 防篡改 IO 1 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_1 = LFSS_CPU_INT_IIDX_STAT_TIO1,
    /*! LFSS Tamper interrupt index for Tamper IO 2 防篡改 IO 2 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_2 = LFSS_CPU_INT_IIDX_STAT_TIO2,
    /*! LFSS Tamper interrupt index for Tamper IO 3 防篡改 IO 3 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_3 = LFSS_CPU_INT_IIDX_STAT_TIO3,
    /*! LFSS Tamper interrupt index for Tamper IO 4 防篡改 IO 4 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_4 = LFSS_CPU_INT_IIDX_STAT_TIO4,
    /*! LFSS Tamper interrupt index for Tamper IO 5 防篡改 IO 5 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_5 = LFSS_CPU_INT_IIDX_STAT_TIO5,
    /*! LFSS Tamper interrupt index for Tamper IO 6 防篡改 IO 6 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_6 = LFSS_CPU_INT_IIDX_STAT_TIO6,
    /*! LFSS Tamper interrupt index for Tamper IO 7 防篡改 IO 7 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_7 = LFSS_CPU_INT_IIDX_STAT_TIO7,
    /*! LFSS Tamper interrupt index for Tamper IO 8 防篡改 IO 8 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_8 = LFSS_CPU_INT_IIDX_STAT_TIO8,
    /*! LFSS Tamper interrupt index for Tamper IO 9 防篡改 IO 9 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_9 = LFSS_CPU_INT_IIDX_STAT_TIO9,
    /*! LFSS Tamper interrupt index for Tamper IO 10 防篡改 IO 10 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_10 = LFSS_CPU_INT_IIDX_STAT_TIO10,
    /*! LFSS Tamper interrupt index for Tamper IO 11 防篡改 IO 11 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_11 = LFSS_CPU_INT_IIDX_STAT_TIO11,
    /*! LFSS Tamper interrupt index for Tamper IO 12 防篡改 IO 12 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_12 = LFSS_CPU_INT_IIDX_STAT_TIO12,
    /*! LFSS Tamper interrupt index for Tamper IO 13 防篡改 IO 13 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_13 = LFSS_CPU_INT_IIDX_STAT_TIO13,
    /*! LFSS Tamper interrupt index for Tamper IO 14 防篡改 IO 14 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_14 = LFSS_CPU_INT_IIDX_STAT_TIO14,
    /*! LFSS Tamper interrupt index for Tamper IO 15 防篡改 IO 15 中断索引 */
    DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_15 = LFSS_CPU_INT_IIDX_STAT_TIO15,
} DL_LFSS_TAMPERIO_IIDX;

/*! @enum DL_LFSS_SCRATCHPAD_MEM_WORD
 *  @brief 便签存储器字索引枚举，每字 32 位
 */
typedef enum {
    /*! LFSS Scratch Pad memory word 0 便签存储器字 0 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_0 = 0x0000,
    /*! LFSS Scratch Pad memory word 1 便签存储器字 1 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_1 = 0x0001,
    /*! LFSS Scratch Pad memory word 2 便签存储器字 2 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_2 = 0x0002,
    /*! LFSS Scratch Pad memory word 3 便签存储器字 3 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_3 = 0x0003,
    /*! LFSS Scratch Pad memory word 4 便签存储器字 4 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_4 = 0x0004,
    /*! LFSS Scratch Pad memory word 5 便签存储器字 5 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_5 = 0x0005,
    /*! LFSS Scratch Pad memory word 6 便签存储器字 6 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_6 = 0x0006,
    /*! LFSS Scratch Pad memory word 7 便签存储器字 7 */
    DL_LFSS_SCRATCHPAD_MEM_WORD_7 = 0x0007,
} DL_LFSS_SCRATCHPAD_MEM_WORD;

/*! @enum DL_LFSS_SCRATCHPAD_MEM_BYTE
 *  @brief 便签存储器字节偏移枚举，每字含 4 字节
 */
typedef enum {
    /*! LFSS Scratch Pad memory byte 0 of the given Scratch Pad memory word
     *  便签存储器字内字节 0 */
    DL_LFSS_SCRATCHPAD_MEM_BYTE_0 = 0x0000,
    /*! LFSS Scratch Pad memory byte 1 of the given Scratch Pad memory word
     *  便签存储器字内字节 1 */
    DL_LFSS_SCRATCHPAD_MEM_BYTE_1 = 0x0001,
    /*! LFSS Scratch Pad memory byte 2 of the given Scratch Pad memory word
     *  便签存储器字内字节 2 */
    DL_LFSS_SCRATCHPAD_MEM_BYTE_2 = 0x0002,
    /*! LFSS Scratch Pad memory byte 3 of the given Scratch Pad memory word
     *  便签存储器字内字节 3 */
    DL_LFSS_SCRATCHPAD_MEM_BYTE_3 = 0x0003,
} DL_LFSS_SCRATCHPAD_MEM_BYTE;

/*! @enum DL_LFSS_IWDT_CLOCK_DIVIDE
 *  @brief 独立看门狗(IWDT)时钟分频枚举
 */
typedef enum {
    /*! WDT Clock divide by 1 看门狗时钟 1 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_1 = 0x00000000,
    /*! WDT Clock divide by 2 看门狗时钟 2 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_2 = 0x00000001,
    /*! WDT Clock divide by 3 看门狗时钟 3 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_3 = 0x00000002,
    /*! WDT Clock divide by 4 看门狗时钟 4 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_4 = 0x00000003,
    /*! WDT Clock divide by 5 看门狗时钟 5 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_5 = 0x00000004,
    /*! WDT Clock divide by 6 看门狗时钟 6 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_6 = 0x00000005,
    /*! WDT Clock divide by 7 看门狗时钟 7 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_7 = 0x00000006,
    /*! WDT Clock divide by 8 看门狗时钟 8 分频 */
    DL_LFSS_IWDT_CLOCK_DIVIDE_8 = 0x00000007
} DL_LFSS_IWDT_CLOCK_DIVIDE;

/*! @enum DL_LFSS_IWDT_TIMER_PERIOD
 *  @brief 独立看门狗定时器周期枚举，值越大超时时间越长
 */
typedef enum {
    /*! WDT 2^6 timer period count 看门狗定时周期 2^6 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_6_BITS = LFSS_WDTCTL_PER_PER_EN_6,
    /*! WDT 2^8 timer period count 看门狗定时周期 2^8 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_8_BITS = LFSS_WDTCTL_PER_PER_EN_8,
    /*! WDT 2^10 timer period count 看门狗定时周期 2^10 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_10_BITS = LFSS_WDTCTL_PER_PER_EN_10,
    /*! WDT 2^12 timer period count 看门狗定时周期 2^12 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_12_BITS = LFSS_WDTCTL_PER_PER_EN_12,
    /*! WDT 2^15 timer period count 看门狗定时周期 2^15 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_15_BITS = LFSS_WDTCTL_PER_PER_EN_15,
    /*! WDT 2^18 timer period count 看门狗定时周期 2^18 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_18_BITS = LFSS_WDTCTL_PER_PER_EN_18,
    /*! WDT 2^21 timer period count 看门狗定时周期 2^21 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_21_BITS = LFSS_WDTCTL_PER_PER_EN_21,
    /*! WDT 2^25 timer period count 看门狗定时周期 2^25 计数 */
    DL_LFSS_IWDT_TIMER_PERIOD_25_BITS = LFSS_WDTCTL_PER_PER_EN_25
} DL_LFSS_IWDT_TIMER_PERIOD;

/* clang-format on */

/**
 *  @brief      Enable output inversion for the specified Tamper IO
 *              使能指定防篡改 IO 的输出反转
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO.
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableOutputInversion(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 置位 OUTINV 位使能输出反转
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] |=
        LFSS_TIOCTL_OUTINV_ENABLE;
}

/**
 *  @brief      Checks if output inversion is enabled for the specified Tamper IO
 *              检查指定防篡改 IO 的输出反转是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     Returns if output inversion is enabled
 *              返回输出反转使能状态
 *
 *  @retval     true  Output inversion for the Tamper I/O is enabled 输出反转已使能
 *  @retval     false Output inversion for the Tamper I/O is disabled 输出反转已禁用
 */
__STATIC_INLINE bool DL_LFSS_TamperIO_isOutputInversionEnabled(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 OUTINV 位并与使能值比较
    return ((lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
                LFSS_TIOCTL_OUTINV_MASK) == LFSS_TIOCTL_OUTINV_ENABLE);
}

/**
 *  @brief      Disable output inversion for the specified Tamper IO
 *              禁用指定防篡改 IO 的输出反转
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableOutputInversion(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 清除 OUTINV 位禁用输出反转
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &=
        ~(LFSS_TIOCTL_OUTINV_MASK);
}

/**
 *  @brief      Selects the source for the tamper output (TOUT) control for
 *              the specified Tamper IO
 *              设置指定防篡改 IO 的输出源
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 *  @param[in]  source    The source for the tamper output control.
 *                        One of @ref DL_LFSS_TAMPERIO_OUTPUT_SOURCE.
 *                        输出源选择
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setOutputSource(LFSS_Regs *lfss,
    DL_LFSS_TAMPERIO tamperIO, DL_LFSS_TAMPERIO_OUTPUT_SOURCE source)
{
    // 用 TOUTSEL 掩码更新输出源选择位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO],
        (uint32_t) source, LFSS_TIOCTL_TOUTSEL_MASK);
}

/**
 *  @brief      Gets the source for the tamper output (TOUT) control for
 *              the specified Tamper IO
 *              获取指定防篡改 IO 的当前输出源
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     The output source of the tamper IO
 *              当前输出源
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_OUTPUT_SOURCE
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_OUTPUT_SOURCE
DL_LFSS_TamperIO_getOutputSource(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 TOUTSEL 位域并转换为枚举
    return ((DL_LFSS_TAMPERIO_OUTPUT_SOURCE)(
        lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
        LFSS_TIOCTL_TOUTSEL_MASK));
}

/**
 *  @brief      Selects the filter width for the digital glitch filter for
 *              the specified Tamper IO
 *              设置指定防篡改 IO 的数字毛刺滤波器宽度
 *
 *  The tamper event detection allows to configure one or more tamper IO’s to
 *  trigger a timestamp event and to generate an interrupt to the CPU. To
 *  minimize false triggers, a digital filter circuit is inserted into the IO
 *  path. Only pulses that are longer than the configured filter width will be
 *  registered as a tamper event.
 *  防篡改事件检测可配置一个或多个 IO 触发时间戳事件和 CPU 中断。
 *  为减少误触发，IO 路径中插入了数字滤波电路，
 *  仅宽度大于配置值的脉冲才会被识别为篡改事件。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 *  @param[in]  width     The width of the digital glitch filter. Pulses
 *                        smaller than this width will be filtered.
 *                        One of @ref DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH.
 *                        毛刺滤波宽度，小于此宽度的脉冲将被过滤
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setGlitchFilterWidth(LFSS_Regs *lfss,
    DL_LFSS_TAMPERIO tamperIO, DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH width)
{
    // 用 FILTEREN 掩码更新滤波宽度位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO],
        (uint32_t) width, LFSS_TIOCTL_FILTEREN_MASK);
}

/**
 *  @brief      Gets the filter width for the digital glitch filter for
 *              the specified Tamper IO
 *              获取指定防篡改 IO 的毛刺滤波器宽度
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     The digital glitch filter width
 *              当前毛刺滤波宽度
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH
DL_LFSS_TamperIO_getGlitchFilterWidth(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 FILTEREN 位域并转换为枚举
    return ((DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH)(
        lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
        LFSS_TIOCTL_FILTEREN_MASK));
}

/**
 *  @brief      Selects the edge detection polarity for the specified Tamper IO
 *              设置指定防篡改 IO 的边沿检测极性
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 *  @param[in]  polarity  The edge detection polarity.
 *                        One of @ref DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY.
 *                        边沿检测极性选择
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setEdgeDetectionPolarity(LFSS_Regs *lfss,
    DL_LFSS_TAMPERIO tamperIO,
    DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY polarity)
{
    // 用 POLARITY 掩码更新边沿检测极性位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO],
        (uint32_t) polarity, LFSS_TIOCTL_POLARITY_MASK);
}

/**
 *  @brief      Gets the edge detection polarity for the specified Tamper IO
 *              获取指定防篡改 IO 的边沿检测极性
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     The edge detection polarity for the specified Tamper IO
 *              当前边沿检测极性
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY
DL_LFSS_TamperIO_getEdgeDetectionPolarity(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 POLARITY 位域并转换为枚举
    return ((DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY)(
        lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
        LFSS_TIOCTL_POLARITY_MASK));
}

/**
 *  @brief      Enable internal pull-up resistor for the specified Tamper IO
 *              使能指定防篡改 IO 的内部上拉电阻
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO.
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableInternalPullUp(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 置位 PIPU 位使能内部上拉
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] |=
        LFSS_TIOCTL_PIPU_ENABLE;
}

/**
 *  @brief      Checks if internal pull-up resistor is enabled for the specified
 *              Tamper IO
 *              检查指定防篡改 IO 的内部上拉电阻是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     Returns the internal pull-up resistor enabled status
 *              返回内部上拉电阻使能状态
 *
 *  @retval     true  Internal pull-up resistor for the Tamper I/O is enabled 上拉已使能
 *  @retval     false Internal pull-up resistor for the Tamper I/O is disabled 上拉已禁用
 */
__STATIC_INLINE bool DL_LFSS_TamperIO_isInternalPullUpEnabled(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 PIPU 位并与使能值比较
    return ((lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
                LFSS_TIOCTL_PIPU_MASK) == LFSS_TIOCTL_PIPU_ENABLE);
}

/**
 *  @brief      Disable internal pull-up resistor for the specified Tamper IO
 *              禁用指定防篡改 IO 的内部上拉电阻
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableInternalPullUp(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 清除 PIPU 位禁用内部上拉
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &=
        ~(LFSS_TIOCTL_PIPU_MASK);
}

/**
 *  @brief      Enable internal pull-down resistor for the specified Tamper IO
 *              使能指定防篡改 IO 的内部下拉电阻
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO.
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableInternalPullDown(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 置位 PIPD 位使能内部下拉
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] |=
        LFSS_TIOCTL_PIPD_ENABLE;
}

/**
 *  @brief      Checks if internal pull-down resistor is enabled for the specified
 *              Tamper IO
 *              检查指定防篡改 IO 的内部下拉电阻是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     Returns the internal pull-down resistor enabled status
 *              返回内部下拉电阻使能状态
 *
 *  @retval     true  Internal pull-down resistor for the Tamper I/O is enabled 下拉已使能
 *  @retval     false Internal pull-down resistor for the Tamper I/O is disabled 下拉已禁用
 */
__STATIC_INLINE bool DL_LFSS_TamperIO_isInternalPullDownEnabled(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 PIPD 位并与使能值比较
    return ((lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
                LFSS_TIOCTL_PIPD_MASK) == LFSS_TIOCTL_PIPD_ENABLE);
}

/**
 *  @brief      Disable internal pull-down resistor for the specified Tamper IO
 *              禁用指定防篡改 IO 的内部下拉电阻
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableInternalPullDown(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 清除 PIPD 位禁用内部下拉
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &=
        ~(LFSS_TIOCTL_PIPD_MASK);
}

/**
 *  @brief      Enable input path for the specified Tamper IO
 *              使能指定防篡改 IO 的输入通路
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO.
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableInput(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 置位 INENA 位使能输入通路
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] |=
        LFSS_TIOCTL_INENA_ENABLE;
}

/**
 *  @brief      Checks if input is enabled for the specified Tamper IO
 *              检查指定防篡改 IO 的输入通路是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     Returns the input enabled status
 *              返回输入通路使能状态
 *
 *  @retval     true  Input for the Tamper I/O is enabled 输入已使能
 *  @retval     false Input for the Tamper I/O is disabled 输入已禁用
 */
__STATIC_INLINE bool DL_LFSS_TamperIO_isInputEnabled(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 INENA 位并与使能值比较
    return ((lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
                LFSS_TIOCTL_INENA_MASK) == LFSS_TIOCTL_INENA_ENABLE);
}

/**
 *  @brief      Disable input path for the specified Tamper IO
 *              禁用指定防篡改 IO 的输入通路
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableInput(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 清除 INENA 位禁用输入通路
    lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &=
        ~(LFSS_TIOCTL_INENA_MASK);
}

/**
 *  @brief      Selects the mode for the specified Tamper IO
 *              设置指定防篡改 IO 的工作模式
 *
 * The tamper IOs have two modes of operation:
 *     - In IOMUX mode (@ref DL_LFSS_TAMPERIO_MODE_IOMUX), the tamper IO input
 *         and output path are connected to the SoC IOMUX module and the IO can
 *         be used as a second function.
 *     - In tamper mode (@ref DL_LFSS_TAMPERIO_MODE_TAMPER), the tamper IO is
 *         completely controlled by the VBAT IP and will remain functional
 *         during the power loss of the main supply or during SHUTDOWN mode.
 *  防篡改 IO 有两种工作模式：
 *     - IOMUX 模式：IO 连接到 SoC IOMUX 模块，可作为第二功能引脚
 *     - 防篡改模式：由 VBAT IP 完全控制，主电源掉电或 SHUTDOWN 模式下仍可工作
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO to configure.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要配置的防篡改 IO 编号
 *  @param[in]  mode      The mode for the specified tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO_MODE.
 *                        工作模式选择
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setMode(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO, DL_LFSS_TAMPERIO_MODE mode)
{
    // 用 IOMUX 掩码更新模式选择位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO],
        (uint32_t) mode, LFSS_TIOCTL_IOMUX_MASK);
}

/**
 *  @brief      Gets the mode for the specified Tamper IO
 *              获取指定防篡改 IO 的当前工作模式
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     The mode for the specified Tamper IO
 *              当前工作模式
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_MODE
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_MODE DL_LFSS_TamperIO_getMode(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    // 读取 IOMUX 位域并转换为枚举
    return ((DL_LFSS_TAMPERIO_MODE)(
        lfss->IPSPECIFIC_TIO.TIOCTL[(uint32_t) tamperIO] &
        LFSS_TIOCTL_IOMUX_MASK));
}

/**
 *  @brief      Enable data output for the specified Tamper IO
 *              使能指定防篡改 IO 的数据输出
 *
 *  @param[in]  lfss       Pointer to the register overlay for the LFSS peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO   Specifies the tamper IO to configure.
 *                         One of @ref DL_LFSS_TAMPERIO.
 *                         要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableOutput(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_TIO.TOE3_0;

    /* Point to correct base register */
    // 每个寄存器管理 4 个 IO，根据编号偏移到对应寄存器
    pReg = (uint32_t *) ((uint8_t *) pReg + (uint8_t)(4 * (tamperIO / 4)));

    /* Set just the one bit for the specified tamper IO */
    // 每个 IO 占 8 位，计算位偏移并置位使能位
    *pReg |= LFSS_TAMPER_IO_PIN_ENABLE << (8 * (tamperIO % 4));
}

/**
 *  @brief      Disable data output for the specified Tamper IO
 *              禁用指定防篡改 IO 的数据输出
 *
 *  @param[in]  lfss       Pointer to the register overlay for the LFSS peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO   Specifies the tamper IO to configure.
 *                         One of @ref DL_LFSS_TAMPERIO.
 *                         要配置的防篡改 IO 编号
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableOutput(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_TIO.TOE3_0;

    /* Point to correct base register */
    // 每个寄存器管理 4 个 IO，根据编号偏移到对应寄存器
    pReg = (uint32_t *) ((uint8_t *) pReg + (uint8_t)(4 * (tamperIO / 4)));

    /* Clear just the one bit for the specified tamper IO */
    // 计算位偏移并清除使能位
    *pReg &= ~(LFSS_TAMPER_IO_PIN_ENABLE << (8 * (tamperIO % 4)));
}

/**
 *  @brief      Checks if data output for the specified Tamper IO is enabled
 *              检查指定防篡改 IO 的数据输出是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     If data output for the specified Tamper IO is enabled
 *              返回数据输出使能状态
 *
 *  @retval     true   Data output for the tamper IO is enabled 输出已使能
 *  @retval     false  Data output for the tamper IO is disabled 输出已禁用
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_isOutputEnabled(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_TIO.TOE3_0;

    /* Point to correct base register */
    // 每个寄存器管理 4 个 IO，根据编号偏移到对应寄存器
    pReg = (uint32_t *) ((uint8_t *) pReg + (uint8_t)(4 * (tamperIO / 4)));

    volatile uint32_t pinMask = LFSS_TAMPER_IO_PIN_MASK
                                << (8 * (tamperIO % 4));
    volatile uint32_t enableMask = LFSS_TAMPER_IO_PIN_ENABLE
                                   << (8 * (tamperIO % 4));

    /* Get just the one bit for the specified tamper IO */
    // 提取对应位并与使能值比较
    return ((*pReg & pinMask) == enableMask);
}

/**
 *  @brief      Enable data output value as zero for the specified Tamper IO
 *              设置指定防篡改 IO 的输出数据值
 *
 *  @param[in]  lfss       Pointer to the register overlay for the LFSS peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO   Specifies the tamper IO to configure.
 *                         One of @ref DL_LFSS_TAMPERIO.
 *                         要配置的防篡改 IO 编号
 *  @param[in]  outVal     The data value to set the tamper IO output to.
 *                         One of @ref DL_LFSS_TAMPERIO_VALUE.
 *                         输出数据值(0 或 1)
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setOutputValue(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO, DL_LFSS_TAMPERIO_VALUE outVal)
{
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_TIO.TOUT3_0;

    /* Point to correct base register */
    // 每个寄存器管理 4 个 IO，根据编号偏移到对应寄存器
    pReg = (uint32_t *) ((uint8_t *) pReg + (uint8_t)(4 * (tamperIO / 4)));

    volatile uint32_t valueMask = outVal << (8 * (tamperIO % 4));
    volatile uint32_t pinMask   = LFSS_TAMPER_IO_PIN_MASK
                                << (8 * (tamperIO % 4));

    // 用掩码更新对应 IO 的输出值位
    DL_Common_updateReg(pReg, valueMask, pinMask);
}

/**
 *  @brief      Gets the output data value for the specified Tamper IO
 *              获取指定防篡改 IO 的输出数据值
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     The output data value for the specified tamper IO
 *              当前输出数据值
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_VALUE
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_VALUE DL_LFSS_TamperIO_getOutputValue(
    LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_TIO.TOUT3_0;

    /* Point to correct base register */
    // 每个寄存器管理 4 个 IO，根据编号偏移到对应寄存器
    pReg = (uint32_t *) ((uint8_t *) pReg + (uint8_t)(4 * (tamperIO / 4)));

    volatile uint32_t pinMask = LFSS_TAMPER_IO_PIN_MASK
                                << (8 * (tamperIO % 4));

    // 提取对应位并右移到最低位
    uint32_t outVal = (*pReg & pinMask) >> (8 * (tamperIO % 4));

    return (DL_LFSS_TAMPERIO_VALUE)(outVal);
}

/**
 *  @brief      Gets the input data value for the specified Tamper IO
 *              获取指定防篡改 IO 的输入数据值
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  tamperIO  Specifies the tamper IO.
 *                        One of @ref DL_LFSS_TAMPERIO
 *                        要查询的防篡改 IO 编号
 *
 *  @return     The input data value for the specified tamper IO
 *              当前输入数据值
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_VALUE
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_VALUE DL_LFSS_TamperIO_getInputValue(
    const LFSS_Regs *lfss, DL_LFSS_TAMPERIO tamperIO)
{
    const volatile uint32_t *pReg = &lfss->IPSPECIFIC_TIO.TIN3_0;

    /* Point to correct base register */
    // 每个寄存器管理 4 个 IO，根据编号偏移到对应寄存器
    pReg = (uint32_t *) ((uint8_t *) pReg + (uint8_t)(4 * (tamperIO / 4)));

    volatile uint32_t pinMask = LFSS_TAMPER_IO_PIN_MASK
                                << (8 * (tamperIO % 4));

    // 提取对应位并右移到最低位
    uint32_t inputVal = (*pReg & pinMask) >> (8 * (tamperIO % 4));

    return (DL_LFSS_TAMPERIO_VALUE)(inputVal);
}

/**
 *  @brief      Sets the interval for the heart beat generator
 *              设置心跳发生器的周期间隔
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  interval  The interval of the heart beat generator.
 *                        One of @ref DL_LFSS_HEARTBEAT_INTERVAL
 *                        心跳周期间隔
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setHeartBeatInterval(
    LFSS_Regs *lfss, DL_LFSS_HEARTBEAT_INTERVAL interval)
{
    // 用 HBINTERVAL 掩码更新心跳间隔位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.HEARTBEAT, (uint32_t) interval,
        LFSS_HEARTBEAT_HBINTERVAL_MASK);
}

/**
 *  @brief      Gets the interval for the heart beat generator
 *              获取心跳发生器的当前周期间隔
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *
 *  @return     The interval for the heart beat generator.
 *              当前心跳周期间隔
 *
 *  @retval     One of @ref DL_LFSS_HEARTBEAT_INTERVAL
 */
__STATIC_INLINE DL_LFSS_HEARTBEAT_INTERVAL
DL_LFSS_TamperIO_getHeartBeatInterval(const LFSS_Regs *lfss)
{
    // 读取 HBINTERVAL 位域并转换为枚举
    uint32_t interval =
        lfss->IPSPECIFIC_TIO.HEARTBEAT & LFSS_HEARTBEAT_HBINTERVAL_MASK;

    return (DL_LFSS_HEARTBEAT_INTERVAL)(interval);
}

/**
 *  @brief      Sets the pulse width for the heart beat generator
 *              设置心跳发生器的脉冲宽度
 *
 *  @param[in]  lfss   Pointer to the register overlay for the LFSS peripheral
 *                     LFSS 外设寄存器基地址指针
 *  @param[in]  width  The pulse width of the heart beat generator.
 *                     One of @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH
 *                     心跳脉冲宽度
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setHeartBeatPulseWidth(
    LFSS_Regs *lfss, DL_LFSS_HEARTBEAT_PULSE_WIDTH width)
{
    // 用 HBWIDTH 掩码更新脉冲宽度位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.HEARTBEAT, (uint32_t) width,
        LFSS_HEARTBEAT_HBWIDTH_MASK);
}

/**
 *  @brief      Gets the pulse width for the heart beat generator
 *              获取心跳发生器的当前脉冲宽度
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *
 *  @return     The pulse width for the heart beat generator.
 *              当前心跳脉冲宽度
 *
 *  @retval     One of @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH
 */
__STATIC_INLINE DL_LFSS_HEARTBEAT_PULSE_WIDTH
DL_LFSS_TamperIO_getHeartBeatPulseWidth(const LFSS_Regs *lfss)
{
    // 读取 HBWIDTH 位域并转换为枚举
    uint32_t width =
        (lfss->IPSPECIFIC_TIO.HEARTBEAT & LFSS_HEARTBEAT_HBWIDTH_MASK);

    return (DL_LFSS_HEARTBEAT_PULSE_WIDTH)(width);
}

/**
 *  @brief      Sets the mode for the heart beat generator
 *              设置心跳发生器的工作模式
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS peripheral
 *                    LFSS 外设寄存器基地址指针
 *  @param[in]  mode  The mode of the heart beat generator.
 *                    One of @ref DL_LFSS_HEARTBEAT_MODE
 *                    心跳工作模式
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setHeartBeatMode(
    LFSS_Regs *lfss, DL_LFSS_HEARTBEAT_MODE mode)
{
    // 用 HBMODE 掩码更新工作模式位
    DL_Common_updateReg(&lfss->IPSPECIFIC_TIO.HEARTBEAT, (uint32_t) mode,
        LFSS_HEARTBEAT_HBMODE_MASK);
}

/**
 *  @brief      Gets the pulse mode for the heart beat generator
 *              获取心跳发生器的当前工作模式
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *
 *  @return     The pulse mode for the heart beat generator.
 *              当前心跳工作模式
 *
 *  @retval     One of @ref DL_LFSS_HEARTBEAT_MODE
 */
__STATIC_INLINE DL_LFSS_HEARTBEAT_MODE DL_LFSS_TamperIO_getHeartBeatMode(
    const LFSS_Regs *lfss)
{
    // 读取 HBMODE 位域并转换为枚举
    uint32_t mode =
        (lfss->IPSPECIFIC_TIO.HEARTBEAT & LFSS_HEARTBEAT_HBMODE_MASK);

    return (DL_LFSS_HEARTBEAT_MODE)(mode);
}

/**
 *  @brief      Enable write protection lock of the TIOCTL and HEARTBEAT
 *              registers from accidental updates
 *              使能 TIOCTL 和 HEARTBEAT 寄存器的写保护锁
 *
 *  When enabled, the TIOCTL and HEARTBEAT registers will have read-only access.
 *  使能后，TIOCTL 和 HEARTBEAT 寄存器将变为只读。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableWriteProtectLock(LFSS_Regs *lfss)
{
    // 写入解锁密钥并置位保护位
    lfss->IPSPECIFIC_TIO.TIOLOCK =
        (LFSS_TIOLOCK_PROTECT_SET | LFSS_TIOLOCK_KEY_UNLOCK_W);
}

/**
 *  @brief      Checks if write protection lock of the TIOCTL and HEARTBEAT
 *              registers is enabled for the specified Tamper IO
 *              检查 TIOCTL 和 HEARTBEAT 寄存器的写保护锁是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *
 *  @return     Returns the enabled status of the write protection lock
 *              返回写保护锁使能状态
 *
 *  @retval     true  Write protection lock is enabled 写保护已使能
 *  @retval     false Write protection lock is disabled 写保护已禁用
 */
__STATIC_INLINE bool DL_LFSS_TamperIO_isWriteProtectLockEnabled(
    const LFSS_Regs *lfss)
{
    // 读取 PROTECT 位并与使能值比较
    return ((lfss->IPSPECIFIC_TIO.TIOLOCK & LFSS_TIOLOCK_PROTECT_MASK) ==
            LFSS_TIOLOCK_PROTECT_SET);
}

/**
 *  @brief      Disable write protection lock of the TIOCTL and HEARTBEAT
 *              registers
 *              禁用 TIOCTL 和 HEARTBEAT 寄存器的写保护锁
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableWriteProtectLock(LFSS_Regs *lfss)
{
    // 写入解锁密钥并清除保护位
    lfss->IPSPECIFIC_TIO.TIOLOCK =
        (LFSS_TIOLOCK_PROTECT_CLR | LFSS_TIOLOCK_KEY_UNLOCK_W);
}

/**
 * @brief Enables the Watchdog module
 *        使能独立看门狗模块
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_enableModule(LFSS_Regs *lfss)
{
    // 写入解锁密钥并置位使能位
    lfss->IPSPECIFIC_WDT.WDTEN =
        (LFSS_WDTEN_KEY_UNLOCK_W | LFSS_WDTEN_ENABLE_SET);
}

/**
 * @brief Disables the Watchdog module
 *        禁用独立看门狗模块
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_disableModule(LFSS_Regs *lfss)
{
    // 写入解锁密钥并清除使能位
    lfss->IPSPECIFIC_WDT.WDTEN =
        (LFSS_WDTEN_KEY_UNLOCK_W | LFSS_WDTEN_ENABLE_CLR);
}

/**
 * @brief Returns if Watchdog module is on
 *        检查独立看门狗模块是否已使能
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 * @return true  Watchdog is enabled 看门狗已使能
 * @return false Watchdog is disabled 看门狗已禁用
 */
__STATIC_INLINE bool DL_LFSS_IWDT_isModuleEnabled(const LFSS_Regs *lfss)
{
    // 读取 ENABLE 位并与使能值比较
    return ((lfss->IPSPECIFIC_WDT.WDTEN & LFSS_WDTEN_ENABLE_MASK) ==
            LFSS_WDTEN_ENABLE_SET);
}

/**
 * @brief Enables the Watchdog free run control
 *        使能看门狗自由运行模式
 *
 *  When enabled, the Watchdog will continue to free run and ignores the state
 *  of the CPU halted debug state
 *  使能后，看门狗在 CPU 暂停调试时仍继续运行。
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_enableFreeRun(LFSS_Regs *lfss)
{
    // 设置自由运行模式，调试暂停时看门狗不暂停
    lfss->IPSPECIFIC_WDT.WDTDBGCTL = LFSS_WDTDBGCTL_FREE_RUN;
}

/**
 * @brief Disables the Watchdog free run control
 *        禁用看门狗自由运行模式
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_disableFreeRun(LFSS_Regs *lfss)
{
    // 设置停止模式，调试暂停时看门狗也暂停
    lfss->IPSPECIFIC_WDT.WDTDBGCTL = LFSS_WDTDBGCTL_FREE_STOP;
}

/**
 * @brief Returns if Watchdog free run control is enabled
 *        检查看门狗自由运行模式是否已使能
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 * @return true  Watchdog free run control is enabled 自由运行已使能
 * @return false Watchdog free run control is disabled 自由运行已禁用
 */
__STATIC_INLINE bool DL_LFSS_IWDT_isFreeRunEnabled(const LFSS_Regs *lfss)
{
    // 读取 FREE 位并与自由运行值比较
    return ((lfss->IPSPECIFIC_WDT.WDTDBGCTL & LFSS_WDTDBGCTL_FREE_MASK) ==
            LFSS_WDTDBGCTL_FREE_RUN);
}

/**
 *  @brief      Sets the clock divider for the WDT module
 *              设置看门狗模块的时钟分频系数
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  divider   The WDT clock divider to set.
 *                        One of @ref DL_LFSS_IWDT_CLOCK_DIVIDE.
 *                        时钟分频系数
 */
__STATIC_INLINE void DL_LFSS_IWDT_setClockDivider(
    LFSS_Regs *lfss, DL_LFSS_IWDT_CLOCK_DIVIDE divider)
{
    // 用 CLKDIV 掩码更新分频位，同时写入解锁密钥
    DL_Common_updateReg(&lfss->IPSPECIFIC_WDT.WDTCTL,
        ((uint32_t) divider | LFSS_WDTCTL_KEY_UNLOCK_W),
        LFSS_WDTCTL_CLKDIV_MASK | LFSS_WDTCTL_KEY_MASK);
}

/**
 *  @brief      Gets the clock divider for the WDT module
 *              获取看门狗模块的当前时钟分频系数
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *
 *  @return     The clock divider for the WDT module
 *              当前时钟分频系数
 *
 *  @retval     One of @ref DL_LFSS_IWDT_CLOCK_DIVIDE
 */
__STATIC_INLINE DL_LFSS_IWDT_CLOCK_DIVIDE DL_LFSS_IWDT_getClockDivider(
    const LFSS_Regs *lfss)
{
    // 读取 CLKDIV 位域并转换为枚举
    uint32_t divider = (lfss->IPSPECIFIC_WDT.WDTCTL & LFSS_WDTCTL_CLKDIV_MASK);

    return (DL_LFSS_IWDT_CLOCK_DIVIDE)(divider);
}

/**
 *  @brief      Sets the timer period for the WDT module
 *              设置看门狗模块的定时器周期
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  period    The WDT timer period to set.
 *                        One of @ref DL_LFSS_IWDT_TIMER_PERIOD.
 *                        定时器周期选择
 */
__STATIC_INLINE void DL_LFSS_IWDT_setTimerPeriod(
    LFSS_Regs *lfss, DL_LFSS_IWDT_TIMER_PERIOD period)
{
    // 用 PER 掩码更新周期位，同时写入解锁密钥
    DL_Common_updateReg(&lfss->IPSPECIFIC_WDT.WDTCTL,
        ((uint32_t) period | LFSS_WDTCTL_KEY_UNLOCK_W),
        LFSS_WDTCTL_PER_MASK | LFSS_WDTCTL_KEY_MASK);
}

/*!
 *  @brief      Get the timer period for the WDT module
 *              获取看门狗模块的当前定时器周期
 *
 *  @param[in]  lfss  Pointer to the register overlay for peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @return     The current timer period for the WDT module
 *              当前定时器周期
 *
 *  @retval     One of @ref DL_LFSS_IWDT_TIMER_PERIOD
 */
__STATIC_INLINE DL_LFSS_IWDT_TIMER_PERIOD DL_LFSS_IWDT_getTimerPeriod(
    const LFSS_Regs *lfss)
{
    // 读取 PER 位域并转换为枚举
    uint32_t period = (lfss->IPSPECIFIC_WDT.WDTCTL & LFSS_WDTCTL_PER_MASK);

    return (DL_LFSS_IWDT_TIMER_PERIOD)(period);
}

/*!
 *  @brief      Restarts the Watchdog
 *              重启看门狗计数器(喂狗)
 *
 *  @param[in]  lfss  Pointer to the register overlay for peripheral
 *                    LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_restart(LFSS_Regs *lfss)
{
    // 写入重启值清零计数器，防止看门狗超时复位
    lfss->IPSPECIFIC_WDT.WDTCNTRST = LFSS_WDTCNTRST_RESTART_VALUE;
}

/**
 * @brief Returns if Watchdog module is running
 *        检查看门狗模块是否正在运行
 *
 * @param lfss        Pointer to the register overlay for the peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 * @return true  Watchdog module is running 看门狗正在运行
 * @return false Watchdog counter has stopped 看门狗计数器已停止
 */
__STATIC_INLINE bool DL_LFSS_IWDT_isWatchdogRunning(const LFSS_Regs *lfss)
{
    // 读取 RUN 位并与运行值比较
    return ((lfss->IPSPECIFIC_WDT.WDTSTAT & LFSS_WDTSTAT_RUN_MASK) ==
            LFSS_WDTSTAT_RUN_RUN);
}

/**
 *  @brief      Enable write protection lock of the WDTEN and WDTCTL
 *              registers from accidental updates
 *              使能 WDTEN 和 WDTCTL 寄存器的写保护锁
 *
 *  When enabled, the WDTEN and WDTCTL registers will have read-only access.
 *  使能后，WDTEN 和 WDTCTL 寄存器将变为只读。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_enableWriteProtect(LFSS_Regs *lfss)
{
    // 写入解锁密钥并置位保护位
    lfss->IPSPECIFIC_WDT.WDTLOCK =
        (LFSS_WDTLOCK_PROTECT_SET | LFSS_WDTLOCK_KEY_UNLOCK_W);
}

/**
 *  @brief      Checks if write protection lock of the WDTEN and WDTCTL
 *              registers is enabled
 *              检查 WDTEN 和 WDTCTL 寄存器的写保护锁是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *
 *  @return     Returns the enabled status of the write protection lock
 *              返回写保护锁使能状态
 *
 *  @retval     true  Write protection lock is enabled 写保护已使能
 *  @retval     false Write protection lock is disabled 写保护已禁用
 */
__STATIC_INLINE bool DL_LFSS_IWDT_isWriteProtectEnabled(const LFSS_Regs *lfss)
{
    // 读取 PROTECT 位并与使能值比较
    return ((lfss->IPSPECIFIC_WDT.WDTLOCK & LFSS_WDTLOCK_PROTECT_MASK) ==
            LFSS_WDTLOCK_PROTECT_SET);
}

/**
 *  @brief      Disable write protection lock of the WDTEN and WDTCTL
 *              registers
 *              禁用 WDTEN 和 WDTCTL 寄存器的写保护锁
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_LFSS_IWDT_disableWriteProtect(LFSS_Regs *lfss)
{
    // 写入解锁密钥并清除保护位
    lfss->IPSPECIFIC_WDT.WDTLOCK =
        (LFSS_WDTLOCK_PROTECT_CLR | LFSS_WDTLOCK_KEY_UNLOCK_W);
}

/**
 *  @brief      Write a word in scratch pad memory
 *              向便签存储器写入一个 32 位字
 *
 *  The VBAT scratch pad memory is register based memory that will retain data
 *  as long as VBAT is supplied. Refer to the device TRM for details on the
 *  specific size and implementation of the scratch pad memory.
 *  VBAT 便签存储器是基于寄存器的存储器，只要 VBAT 供电即可保持数据。
 *
 *  This API wil write 32-bit data to the selected word in the scratch pad
 *  memory.
 *  本 API 将 32 位数据写入选定的便签存储器字。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word to write to.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        要写入的便签存储器字索引
 *  @param[in]  data      The 32-bit data to write to the specified scratch pad
 *                        memory word. Value in range of [0x0, 0xFFFFFFF].
 *                        要写入的 32 位数据
 */
__STATIC_INLINE void DL_LFSS_writeScratchPadData32(
    LFSS_Regs *lfss, DL_LFSS_SCRATCHPAD_MEM_WORD memIndex, uint32_t data)
{
    // 直接写入 SPMEM 对应字
    lfss->IPSPECIFIC_MEM.SPMEM[(uint32_t) memIndex] = data;
}

/**
 *  @brief      Write to a single byte in scratch pad memory
 *              向便签存储器写入单个字节
 *
 *  The VBAT scratch pad memory is register based memory that will retain data
 *  as long as VBAT is supplied. Refer to the device TRM for details on the
 *  specific size and implementation of the scratch pad memory.
 *  VBAT 便签存储器是基于寄存器的存储器，只要 VBAT 供电即可保持数据。
 *
 *  This API will write 8-bit data to the selected byte in the scratch pad
 *  memory. The byte to write to is given in context of the scratch pad memory
 *  word that contains that byte.
 *      For example, to write to BYTE5 of scratch pad memory, the user will call
 *      this API with parameters @ref DL_LFSS_SCRATCHPAD_MEM_WORD_1 and
 *      @ref DL_LFSS_SCRATCHPAD_MEM_BYTE_1.
 *  本 API 向便签存储器选定字节写入 8 位数据。
 *  例如：写入 BYTE5 时，使用 WORD_1 + BYTE_1 参数组合。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word to write to.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        要写入的便签存储器字索引
 *  @param[in]  byteIndex The specified scratch pad memory byte to write to.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *                        要写入的字节偏移
 *  @param[in]  data      The 8-bit data to write to the specified scratch pad
 *                        memory byte. Value in range of [0x0, 0xFF].
 *                        要写入的 8 位数据
 */

__STATIC_INLINE void DL_LFSS_writeScratchPadData8(LFSS_Regs *lfss,
    DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex, uint32_t data)
{
    /* Point to correct SPMEM word */
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMEM[(uint32_t) memIndex];

    /* Get the correct bit based on the byteIndex */
    // 计算字节在字内的位偏移(每字节 8 位)
    uint32_t offset = ((uint32_t) byteIndex * (uint32_t) 8);
    data            = data << offset;
    uint32_t mask   = ((uint32_t) 0x000000FFU) << offset;

    // 用掩码更新对应字节
    DL_Common_updateReg(pReg, data, mask);
}

/**
 *  @brief      Read the specified word in scratch pad memory
 *              从便签存储器读取指定的 32 位字
 *
 *  The VBAT scratch pad memory is register based memory that will retain data
 *  as long as VBAT is supplied. Refer to the device TRM for details on the
 *  specific size and implementation of the scratch pad memory.
 *  VBAT 便签存储器是基于寄存器的存储器，只要 VBAT 供电即可保持数据。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word to read.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        要读取的便签存储器字索引
 *
 *  @return     The 32-bit data in the specified scratch pad memory word
 *              读取到的 32 位数据
 */
__STATIC_INLINE uint32_t DL_LFSS_readScratchPadDataWord(
    const LFSS_Regs *lfss, DL_LFSS_SCRATCHPAD_MEM_WORD memIndex)
{
    // 直接读取 SPMEM 对应字
    return (lfss->IPSPECIFIC_MEM.SPMEM[(uint32_t) memIndex]);
}

/**
 *  @brief      Read the specified byte in scratch pad memory
 *              从便签存储器读取指定的单个字节
 *
 *  The VBAT scratch pad memory is register based memory that will retain data
 *  as long as VBAT is supplied. Refer to the device TRM for details on the
 *  specific size and implementation of the scratch pad memory.
 *  VBAT 便签存储器是基于寄存器的存储器，只要 VBAT 供电即可保持数据。
 *
 *  This API will read 8-bit data from the selected byte in the scratch pad
 *  memory. The byte to read to is given in context of the scratch pad memory
 *  word that contains that byte.
 *      For example, to read BYTE5 of scratch pad memory, the user will call
 *      this API with parameters @ref DL_LFSS_SCRATCHPAD_MEM_WORD_1 and
 *      @ref DL_LFSS_SCRATCHPAD_MEM_BYTE_1.
 *  本 API 从便签存储器选定字节读取 8 位数据。
 *  例如：读取 BYTE5 时，使用 WORD_1 + BYTE_1 参数组合。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word to read.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        要读取的便签存储器字索引
 *  @param[in]  byteIndex The specified scratch pad memory byte to read.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *                        要读取的字节偏移
 *
 *  @return     The 8-bit data in the specified scratch pad memory byte
 *              读取到的 8 位数据
 */
__STATIC_INLINE uint8_t DL_LFSS_readScratchPadDataByte(LFSS_Regs *lfss,
    DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMEM word */
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMEM[(uint32_t) memIndex];

    /* Get the correct bit based on the byteIndex */
    // 计算字节在字内的位偏移并构造掩码
    uint32_t offset = ((uint32_t) byteIndex * (uint32_t) 8);
    uint32_t mask   = ((uint32_t) 0x000000FFU) << offset;

    // 提取对应字节并右移到最低位
    return (((*pReg & mask)) >> offset);
}

/**
 *  @brief     Enable write protection lock of the specified byte in scratch pad
 *             memory from accidental updates
 *             使能便签存储器指定字节的写保护
 *
 *  When enabled, the specified byte in scratch pad memory will have read-only
 *  access.
 *  使能后，指定字节将变为只读。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        便签存储器字索引
 *  @param[in]  byteIndex The specified byte in scratch pad memory word to
 *                        enable write protection for
 *                        要使能写保护的字节偏移
 */
__STATIC_INLINE void DL_LFSS_enableScratchPadWriteProtectByte(LFSS_Regs *lfss,
    DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMWPROT register */
    // 每个保护寄存器管理 4 个字(16 字节)，右移 2 位得到寄存器索引
    uint32_t regIndex       = (uint32_t) memIndex >> (uint32_t) 2;
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMWPROT0 + regIndex;

    /* Get the correct bit based on the byteIndex */
    // 计算目标字节在保护寄存器中的位位置
    uint32_t mask = (uint32_t) 1 << ((((uint32_t) memIndex % (uint32_t) 4)
                                         << (uint32_t) 2) +
                                     (uint32_t) byteIndex);

    // 写入解锁密钥并置位对应保护位
    *pReg |= (LFSS_SPMWPROT0_KEY_UNLOCK_W | mask);
}

/**
 *  @brief      Checks if write protection is enabled for the specified byte in
 *              scratch pad memory
 *              检查便签存储器指定字节的写保护是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        便签存储器字索引
 *  @param[in]  byteIndex The specified scratch pad memory byte.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *                        要查询的字节偏移
 *
 *  @return     Returns if write protection is enabled for the specified byte
 *              返回写保护使能状态
 *
 *  @retval     true  Write protection for the specified byte is enabled 写保护已使能
 *  @retval     false Write protection for the specified byte is disabled 写保护已禁用
 */
__STATIC_INLINE bool DL_LFSS_isScratchPadWriteProtectByteEnabled(
    LFSS_Regs *lfss, DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMWPROT index */
    // 每个保护寄存器管理 4 个字，右移 2 位得到寄存器索引
    uint32_t regIndex       = (uint32_t) memIndex >> (uint32_t) 2;
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMWPROT0 + regIndex;

    /* Get the correct bit based on the byteIndex */
    // 计算目标字节在保护寄存器中的位位置
    uint32_t mask = (uint32_t) 1 << ((((uint32_t) memIndex % (uint32_t) 4)
                                         << (uint32_t) 2) +
                                     (uint32_t) byteIndex);

    // 检查对应保护位是否置位
    return ((*pReg & mask) == mask);
}

/**
 *  @brief     Disable write protection lock of the specified byte in scratch
 *             pad memory
 *             禁用便签存储器指定字节的写保护
 *
 *  When disables, the specified byte in scratch pad memory will have read-only
 *  access.
 *  禁用后，指定字节可恢复读写访问。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        便签存储器字索引
 *  @param[in]  byteIndex The specified byte in scratch pad memory word to
 *                        disable write protection for.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *                        要禁用写保护的字节偏移
 */
__STATIC_INLINE void DL_LFSS_disableScratchPadWriteProtectByte(LFSS_Regs *lfss,
    DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMWPROT index */
    // 每个保护寄存器管理 4 个字，右移 2 位得到寄存器索引
    uint32_t regIndex       = (uint32_t) memIndex >> (uint32_t) 2;
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMWPROT0 + regIndex;

    /* Get the correct bit based on the byteIndex */
    // 计算目标字节在保护寄存器中的位位置
    uint32_t mask = (uint32_t) 1 << ((((uint32_t) memIndex % (uint32_t) 4)
                                         << (uint32_t) 2) +
                                     (uint32_t) byteIndex);

    // 写入解锁密钥并清除对应保护位
    *pReg = ((*pReg & (~mask)) | LFSS_SPMWPROT1_KEY_UNLOCK_W);
}

/**
 *  @brief     Enable tamper erase of the specified byte in scratch pad
 *             memory
 *             使能便签存储器指定字节的防篡改擦除功能
 *
 *  When enabled, the specified byte in scratch pad memory will be erased in the
 *  event of a tamper detect.
 *  使能后，检测到篡改事件时将自动擦除该字节。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        便签存储器字索引
 *  @param[in]  byteIndex The specified byte in scratch pad memory word to
 *                        enable tamper erase for.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *                        要使能篡改擦除的字节偏移
 */
__STATIC_INLINE void DL_LFSS_enableScratchPadTamperEraseByte(LFSS_Regs *lfss,
    DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMTERASE index */
    // 每个擦除寄存器管理 4 个字，右移 2 位得到寄存器索引
    uint32_t regIndex       = (uint32_t) memIndex >> (uint32_t) 2;
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMTERASE0 + regIndex;

    /* Get the correct bit based on the byteIndex */
    // 计算目标字节在擦除寄存器中的位位置
    uint32_t mask = (uint32_t) 1 << ((((uint32_t) memIndex % (uint32_t) 4)
                                         << (uint32_t) 2) +
                                     (uint32_t) byteIndex);

    // 写入解锁密钥并置位对应擦除使能位
    *pReg |= (LFSS_SPMTERASE0_KEY_UNLOCK_W | mask);
}

/**
 *  @brief      Checks if tamper erase is enabled for the specified byte in
 *              scratch pad memory
 *              检查便签存储器指定字节的篡改擦除是否已使能
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *  @param[in]  memIndex  The specified scratch pad memory word.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *  @param[in]  byteIndex The specified byte in scratch pad memory byte.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *
 *  @return     Returns if tamper erase is enabled for the specified byte
 *
 *  @retval     true  Tamper erase for the specified byte is enabled 篡改擦除已使能
 *  @retval     false tamper erase for the specified byte is disabled 篡改擦除已禁用
 */
__STATIC_INLINE bool DL_LFSS_isScratchPadTamperEraseByteEnabled(
    LFSS_Regs *lfss, DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMTERASE index */
    // 每个擦除寄存器管理 4 个字，右移 2 位得到寄存器索引
    uint32_t regIndex       = (uint32_t) memIndex >> (uint32_t) 2;
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMTERASE0 + regIndex;

    /* Get the correct bit based on the byteIndex */
    // 计算目标字节在擦除寄存器中的位位置
    uint32_t mask = (uint32_t) 1 << ((((uint32_t) memIndex % (uint32_t) 4)
                                         << (uint32_t) 2) +
                                     (uint32_t) byteIndex);

    // 检查对应擦除使能位是否置位
    return ((*pReg & mask) == mask);
}

/**
 *  @brief     Disable tamper erase of the specified byte in scratch pad memory
 *             禁用便签存储器指定字节的篡改擦除功能
 *
 *  The specified byte in scratch pad memory will not be erased in the event of
 *  a tamper detect.
 *  禁用后，篡改事件不会擦除该字节。
 *
 *  @param[in]  lfss      Pointer to the register overlay for the LFSS peripheral
 *                        LFSS 外设寄存器基地址指针
 *  @param[in]  memIndex  The specified scratch pad memory word to write to.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_WORD
 *                        便签存储器字索引
 *  @param[in]  byteIndex The specified byte in scratch pad memory word to
 *                        disable tamper erase for.
 *                        One of @ref DL_LFSS_SCRATCHPAD_MEM_BYTE
 *                        要禁用篡改擦除的字节偏移
 */
__STATIC_INLINE void DL_LFSS_disableScratchPadTamperEraseByte(LFSS_Regs *lfss,
    DL_LFSS_SCRATCHPAD_MEM_WORD memIndex,
    DL_LFSS_SCRATCHPAD_MEM_BYTE byteIndex)
{
    /* Point to correct SPMTERASE index */
    // 每个擦除寄存器管理 4 个字，右移 2 位得到寄存器索引
    uint32_t regIndex       = (uint32_t) memIndex >> (uint32_t) 2;
    volatile uint32_t *pReg = &lfss->IPSPECIFIC_MEM.SPMTERASE0 + regIndex;

    /* Get the correct bit based on the byteIndex */
    // 计算目标字节在擦除寄存器中的位位置
    uint32_t mask = (uint32_t) 1 << ((((uint32_t) memIndex % (uint32_t) 4)
                                         << (uint32_t) 2) +
                                     (uint32_t) byteIndex);

    // 写入解锁密钥并清除对应擦除使能位
    *pReg = ((*pReg & (~mask)) | LFSS_SPMWPROT2_KEY_UNLOCK_W);
}

/**
 *  @brief      Enable LFSS Tamper Interrupts
 *              使能 LFSS 防篡改中断
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Logical OR of
 *                             @ref DL_LFSS_TAMPERIO_INTERRUPT.
 *                             中断掩码，可按位或组合
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableInterrupt(
    LFSS_Regs *lfss, uint32_t interruptMask)
{
    // 置位 IMASK 中对应中断使能位
    lfss->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable LFSS Tamper interrupts
 *              禁用 LFSS 防篡改中断
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Logical OR of
 *                             @ref DL_LFSS_TAMPERIO_INTERRUPT
 *                             中断掩码，可按位或组合
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableInterrupt(
    LFSS_Regs *lfss, uint32_t interruptMask)
{
    // 清除 IMASK 中对应中断使能位
    lfss->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which LFSS Tamper interrupts are enabled
 *              查询哪些 LFSS 防篡改中断已使能
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_LFSS_TAMPERIO_INTERRUPT
 *                             要查询的中断掩码
 *
 *  @return     Which of the requested LFSS interrupts are enabled
 *              返回已使能的中断位
 *
 *  @retval     Logical OR of @ref DL_LFSS_TAMPERIO_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_getEnabledInterrupts(
    const LFSS_Regs *lfss, uint32_t interruptMask)
{
    // 读取 IMASK 并与查询掩码按位与
    return (lfss->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled LFSS Tamper interrupts
 *              查询已使能的 LFSS 防篡改中断的挂起状态
 *
 *  Checks if any of the LFSS Tamper interrupts that were previously enabled are
 *  pending.
 *  检查先前已使能的防篡改中断是否有待处理的中断请求。
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_LFSS_TAMPERIO_INTERRUPT
 *                             要查询的中断掩码
 *
 *  @return     Which of the requested LFSS Tamper interrupts are pending
 *              返回挂起的中断位
 *
 *  @retval     Logical OR of @ref DL_LFSS_TAMPERIO_INTERRUPT values
 *
 *  @sa         DL_LFSS_TAMPERIO_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_getEnabledInterruptStatus(
    const LFSS_Regs *lfss, uint32_t interruptMask)
{
    // 读取 MIS(已使能中断状态)寄存器并与查询掩码按位与
    return (lfss->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any LFSS Tamper interrupt
 *              查询任意 LFSS 防篡改中断的原始挂起状态
 *
 *  Checks if any of the LFSS Tamper interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *  检查防篡改中断是否有待处理请求，无需事先使能。
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_LFSS_TAMPERIO_INTERRUPT
 *                             要查询的中断掩码
 *
 *  @return     Which of the requested LFSS Tamper interrupts are pending
 *              返回原始挂起的中断位
 *
 *  @retval     Logical OR of @ref DL_LFSS_TAMPERIO_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_getRawInterruptStatus(
    const LFSS_Regs *lfss, uint32_t interruptMask)
{
    // 读取 RIS(原始中断状态)寄存器并与查询掩码按位与
    return (lfss->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending LFSS interrupt
 *              获取最高优先级的待处理 LFSS 中断
 *
 *  Checks if any of the LFSS interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *  检查是否有待处理的 LFSS 中断，无需事先使能。
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @return     The highest priority pending LFSS Tamper interrupt
 *              最高优先级待处理中断的索引
 *
 *  @retval     One of @ref DL_LFSS_TAMPERIO_IIDX
 */
__STATIC_INLINE DL_LFSS_TAMPERIO_IIDX DL_LFSS_TamperIO_getPendingInterrupt(
    const LFSS_Regs *lfss)
{
    // 读取 IIDX 寄存器获取最高优先级中断索引
    return (DL_LFSS_TAMPERIO_IIDX)(lfss->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending LFSS Tamper interrupts
 *              清除待处理的 LFSS 防篡改中断标志
 *
 *  @param[in]  lfss  Pointer to the register overlay for the LFSS Tamper peripheral
 *                    LFSS 外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Logical OR of
 *                             @ref DL_LFSS_TAMPERIO_INTERRUPT
 *                             要清除的中断掩码
 */
__STATIC_INLINE void DL_LFSS_TamperIO_clearInterruptStatus(
    LFSS_Regs *lfss, uint32_t interruptMask)
{
    // 写入 ICLR 寄存器清除对应中断标志
    lfss->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Enable LFSS Tamper event
 *              使能 LFSS 防篡改事件
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  eventMask   Bit mask of interrupts to enable. Bitwise OR of
 *                          @ref DL_LFSS_TAMPERIO_EVENT.
 *                          事件掩码，可按位或组合
 */
__STATIC_INLINE void DL_LFSS_TamperIO_enableEvent(
    LFSS_Regs *lfss, uint32_t eventMask)
{
    // 置位 GEN_EVENT.IMASK 中对应事件使能位
    lfss->GEN_EVENT.IMASK |= (eventMask);
}

/**
 *  @brief      Disable LFSS Tamper event
 *              禁用 LFSS 防篡改事件
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  eventMask   Bit mask of interrupts to enable. Bitwise OR of
 *                          @ref DL_LFSS_TAMPERIO_EVENT.
 *                          事件掩码，可按位或组合
 */
__STATIC_INLINE void DL_LFSS_TamperIO_disableEvent(
    LFSS_Regs *lfss, uint32_t eventMask)
{
    // 清除 GEN_EVENT.IMASK 中对应事件使能位
    lfss->GEN_EVENT.IMASK &= ~(eventMask);
}

/**
 *  @brief      Check which LFSS Tamper events are enabled
 *              查询哪些 LFSS 防篡改事件已使能
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  eventMask   Bit mask of interrupts to check. Bitwise OR of
 *                          @ref DL_LFSS_TAMPERIO_EVENT.
 *                          要查询的事件掩码
 *
 *  @return     Which of the requested LFSS Tamper interrupts are enabled
 *              返回已使能的事件位
 *
 *  @retval     Bitwise OR of @ref DL_LFSS_TAMPERIO_EVENT values
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_getEnabledEvents(
    const LFSS_Regs *lfss, uint32_t eventMask)
{
    // 读取 GEN_EVENT.IMASK 并与查询掩码按位与
    return ((lfss->GEN_EVENT.IMASK) & (eventMask));
}

/**
 *  @brief      Check event flag of enabled LFSS Tamper event
 *              查询已使能的 LFSS 防篡改事件的挂起状态
 *
 *  Checks if any of the LFSS Tamper events that were previously enabled are
 *  pending.
 *  检查先前已使能的防篡改事件是否有待处理的事件。
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  eventMask   Bit mask of interrupts to check. Bitwise OR of
 *                          @ref DL_LFSS_TAMPERIO_EVENT.
 *                          要查询的事件掩码
 *
 *  @return     Which of the requested LFSS Tamper interrupts are pending
 *              返回挂起的事件位
 *
 *  @retval     Bitwise OR of @ref DL_LFSS_TAMPERIO_EVENT values
 *
 *  @sa         DL_LFSS_TamperIO_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_getEnabledEventStatus(
    const LFSS_Regs *lfss, uint32_t eventMask)
{
    // 读取 GEN_EVENT.MIS(已使能事件状态)并按位与
    return ((lfss->GEN_EVENT.MIS) & eventMask);
}

/**
 *  @brief      Check interrupt flag of any LFSS Tamper event
 *              查询任意 LFSS 防篡改事件的原始挂起状态
 *
 *  Checks if any events are pending. Events do not have to
 *  be previously enabled.
 *  检查防篡改事件是否有待处理请求，无需事先使能。
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  eventMask   Bit mask of interrupts to check. Bitwise OR of
 *                          @ref DL_LFSS_TAMPERIO_EVENT.
 *                          要查询的事件掩码
 *
 *  @return     Which of the requested LFSS Tamper interrupts are pending
 *              返回原始挂起的事件位
 *
 *  @retval     Bitwise OR of @ref DL_LFSS_TAMPERIO_EVENT values
 */
__STATIC_INLINE uint32_t DL_LFSS_TamperIO_getRawEventsStatus(
    const LFSS_Regs *lfss, uint32_t eventMask)
{
    // 读取 GEN_EVENT.RIS(原始事件状态)并按位与
    return ((lfss->GEN_EVENT.RIS) & eventMask);
}

/**
 *  @brief      Clear pending LFSS Tamper events
 *              清除待处理的 LFSS 防篡改事件标志
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  eventMask   Bit mask of interrupts to clear. Bitwise OR of
 *                          @ref DL_LFSS_TAMPERIO_EVENT.
 *                          要清除的事件掩码
 */
__STATIC_INLINE void DL_LFSS_TamperIO_clearEventsStatus(
    LFSS_Regs *lfss, uint32_t eventMask)
{
    // 写入 GEN_EVENT.ICLR 寄存器清除对应事件标志
    lfss->GEN_EVENT.ICLR |= (eventMask);
}

/**
 *  @brief Sets the LFSS Tamper event publisher channel ID
 *         设置 LFSS 防篡改事件发布者通道 ID
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
 *                      publisher is disconnected.
 *                      通道 ID 号(0-15)，为 0 时断开发布者连接
 */
__STATIC_INLINE void DL_LFSS_TamperIO_setPublisherChanID(
    LFSS_Regs *lfss, uint8_t chanID)
{
    // 写入通道 ID 到 FPUB_0 寄存器
    lfss->FPUB_0 = (chanID & LFSS_FPUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Gets the event publisher channel ID
 *         获取 LFSS 防篡改事件发布者通道 ID
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *
 *  @return     Event publisher channel ID
 *              当前事件发布者通道 ID
 *
 */
__STATIC_INLINE uint8_t DL_LFSS_TamperIO_getPublisherChanID(
    const LFSS_Regs *lfss)
{
    // 读取 FPUB_0 寄存器并提取通道 ID
    return (uint8_t)(lfss->FPUB_0 & LFSS_FPUB_0_CHANID_MAXIMUM);
}

/**
 *  @brief Requests a reset to the LFSS module
 *         请求 LFSS 模块复位
 *
 *  @param[in]  lfss       Pointer to the register overlay for the peripheral
 *                         LFSS 外设寄存器基地址指针
 *
 *
 */
__STATIC_INLINE void DL_LFSS_reset(LFSS_Regs *lfss)
{
    // 写入解锁密钥并置位 VBATPOR 复位位
    lfss->IPSPECIFIC_RTC.LFSSRST =
        (LFSS_LFSSRST_KEY_UNLOCK_W | LFSS_LFSSRST_VBATPOR_SET);
}

#ifdef __cplusplus
}
#endif
#endif /* __MSPM0_HAS_LFSS__ */
#endif /* ti_dl_dl_lfss__include */
/** @}*/
