/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * 文件用途：防篡改 I/O (TIO) 驱动库头文件。
 * 本文件所有宏定义均为 DL_LFSS_TAMPERIO_* 的别名重定向，
 * 将 TIO 命名空间映射至 LFSS 底层实现，不包含独立函数。
 * TIO 引脚由 VBAT 电源域供电，主电源掉电或 SHUTDOWN 模式下仍可工作。
 */
/*!****************************************************************************
 *  @file       dl_tamperio.h
 *  @brief      Tamper Input/Output (TIO) Driver Library 防篡改输入/输出驱动库
 *  @defgroup   Tamper Input/Output (TIO)
 *
 *  @anchor ti_dl_dl_tio_Overview
 *  # Overview
 *
 *  The tamper I/O is a GPIO which is sourced by the VBAT power domain.
 *  The LFSS IP will contain up to 16 I/O’s of this type. The I/O’s have two modes of operation.
 *  In IOMUX mode the tamper I/O input and output path are connected to the SoC IOMUX module
 *  and the I/O’s can be used as a 2nd function like SPI, UART or Timer PWM. The second mode is
 *  the Tamper mode. In this mode the I/O is completely controlled by the LFSS IP and will remain
 *  functional during the power loss of the main supply or during SHUTDOWN mode.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup TAMPERIO
 * @{
 */
#ifndef ti_dl_dl_tio__include
#define ti_dl_dl_tio__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/driverlib/dl_lfss.h>

#ifdef __MSPM0_HAS_TIO__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_0
 *        防篡改 I/O 引脚 0，重定向至 LFSS 底层定义
 */
#define DL_TAMPERIO_0                               DL_LFSS_TAMPERIO_0

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_1
 *        防篡改 I/O 引脚 1
 */
#define DL_TAMPERIO_1                               DL_LFSS_TAMPERIO_1

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_2
 *        防篡改 I/O 引脚 2
 */
#define DL_TAMPERIO_2                               DL_LFSS_TAMPERIO_2

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_3
 *        防篡改 I/O 引脚 3
 */
#define DL_TAMPERIO_3                               DL_LFSS_TAMPERIO_3

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_4
 *        防篡改 I/O 引脚 4
 */
#define DL_TAMPERIO_4                               DL_LFSS_TAMPERIO_4

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_5
 *        防篡改 I/O 引脚 5
 */
#define DL_TAMPERIO_5                               DL_LFSS_TAMPERIO_5

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_6
 *        防篡改 I/O 引脚 6
 */
#define DL_TAMPERIO_6                               DL_LFSS_TAMPERIO_6

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_7
 *        防篡改 I/O 引脚 7
 */
#define DL_TAMPERIO_7                               DL_LFSS_TAMPERIO_7

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_8
 *        防篡改 I/O 引脚 8
 */
#define DL_TAMPERIO_8                               DL_LFSS_TAMPERIO_8

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_9
 *        防篡改 I/O 引脚 9
 */
#define DL_TAMPERIO_9                               DL_LFSS_TAMPERIO_9

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_10
 *        防篡改 I/O 引脚 10
 */
#define DL_TAMPERIO_10                              DL_LFSS_TAMPERIO_10

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_11
 *        防篡改 I/O 引脚 11
 */
#define DL_TAMPERIO_11                               DL_LFSS_TAMPERIO_11

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_12
 *        防篡改 I/O 引脚 12
 */
#define DL_TAMPERIO_12                              DL_LFSS_TAMPERIO_12

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_13
 *        防篡改 I/O 引脚 13
 */
#define DL_TAMPERIO_13                               DL_LFSS_TAMPERIO_13

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_14
 *        防篡改 I/O 引脚 14
 */
#define DL_TAMPERIO_14                              DL_LFSS_TAMPERIO_14

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_15
 *        防篡改 I/O 引脚 15
 */
#define DL_TAMPERIO_15                               DL_LFSS_TAMPERIO_15

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_VALUE_0
 *        输出值 0（低电平）
 */
#define DL_TAMPERIO_VALUE_0                          DL_LFSS_TAMPERIO_VALUE_0

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_VALUE_1
 *        输出值 1（高电平）
 */
#define DL_TAMPERIO_VALUE_1                          DL_LFSS_TAMPERIO_VALUE_1

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_0
 *        防篡改 I/O 0 中断位掩码
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_0                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_0

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_1
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_1                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_1

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_2
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_2                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_2

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_3
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_3                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_3

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_4
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_4                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_4

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_5
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_5                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_5

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_6
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_6                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_6

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_7
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_7                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_8

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_8
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_8                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_8

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_9
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_9                    DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_9

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_10
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_10                   DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_10

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_11
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_11                   DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_11

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_12
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_12                   DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_12

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_13
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_13                   DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_13

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_14
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_14                   DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_14

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_15
 */
#define DL_TAMPERIO_INTERRUPT_TAMPERIO_15                   DL_LFSS_TAMPERIO_INTERRUPT_TAMPERIO_15

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_0
 *        防篡改 I/O 0 事件位掩码
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_0                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_0

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_1
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_1                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_1

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_2
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_2                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_2

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_3
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_3                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_3

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_4
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_4                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_4

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_5
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_5                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_5

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_6
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_6                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_6

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_7
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_7                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_8

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_8
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_8                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_8

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_9
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_9                        DL_LFSS_TAMPERIO_EVENT_TAMPERIO_9

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_10
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_10                       DL_LFSS_TAMPERIO_EVENT_TAMPERIO_10

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_11
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_11                       DL_LFSS_TAMPERIO_EVENT_TAMPERIO_11

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_12
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_12                       DL_LFSS_TAMPERIO_EVENT_TAMPERIO_12

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_13
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_13                       DL_LFSS_TAMPERIO_EVENT_TAMPERIO_13

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_14
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_14                       DL_LFSS_TAMPERIO_EVENT_TAMPERIO_14

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EVENT_TAMPERIO_15
 */
#define DL_TAMPERIO_EVENT_TAMPERIO_15                       DL_LFSS_TAMPERIO_EVENT_TAMPERIO_15

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_0
 *        防篡改 I/O 0 中断索引，用于读取 IIDX 寄存器
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_0                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_0

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_1
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_1                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_1

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_2
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_2                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_2

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_3
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_3                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_3

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_4
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_4                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_4

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_5
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_5                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_5

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_6
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_6                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_6

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_7
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_7                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_7

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_8
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_8                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_8

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_9
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_9                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_9

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_10
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_10                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_10

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_11
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_11                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_11

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_12
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_12                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_12

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_13
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_13                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_13

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_14
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_14                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_14

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_15
 */
#define DL_TAMPERIO_IIDX_TAMPER_IO_15                       DL_LFSS_TAMPERIO_IIDX_TAMPER_IO_15

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_OUTPUT_SOURCE_TOUT
 *        输出源选择：TOUT 寄存器值
 */
#define DL_TAMPERIO_OUTPUT_SOURCE_TOUT                      DL_LFSS_TAMPERIO_OUTPUT_SOURCE_TOUT

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_OUTPUT_SOURCE_LFCLK
 *        输出源选择：低频时钟 (LFCLK)
 */
#define DL_TAMPERIO_OUTPUT_SOURCE_LFCLK                     DL_LFSS_TAMPERIO_OUTPUT_SOURCE_LFCLK

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_OUTPUT_SOURCE_HEARTBEAT
 *        输出源选择：心跳信号
 */
#define DL_TAMPERIO_OUTPUT_SOURCE_HEARTBEAT                 DL_LFSS_TAMPERIO_OUTPUT_SOURCE_HEARTBEAT

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_OUTPUT_SOURCE_TIME_STAMP_EVENT
 *        输出源选择：时间戳事件
 */
#define DL_TAMPERIO_OUTPUT_SOURCE_TIME_STAMP_EVENT          DL_LFSS_TAMPERIO_OUTPUT_SOURCE_TIME_STAMP_EVENT

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_NONE
 *        毛刺滤波器：禁用（无滤波）
 */
#define DL_TAMPERIO_GLITCH_FILTER_WIDTH_NONE                DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_NONE

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_30_USEC
 *        毛刺滤波器宽度：30 微秒
 */
#define DL_TAMPERIO_GLITCH_FILTER_WIDTH_30_USEC             DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_30_USEC

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_100_USEC
 *        毛刺滤波器宽度：100 微秒
 */
#define DL_TAMPERIO_GLITCH_FILTER_WIDTH_100_USEC            DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_100_USEC

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_200_USEC
 *        毛刺滤波器宽度：200 微秒
 */
#define DL_TAMPERIO_GLITCH_FILTER_WIDTH_200_USEC            DL_LFSS_TAMPERIO_GLITCH_FILTER_WIDTH_200_USEC

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_DISABLE
 *        边沿检测极性：禁用
 */
#define DL_TAMPERIO_EDGE_DETECTION_POLARITY_DISABLE            DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_DISABLE

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_RISE
 *        边沿检测极性：上升沿
 */
#define DL_TAMPERIO_EDGE_DETECTION_POLARITY_RISE                DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_RISE

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_FALL
 *        边沿检测极性：下降沿
 */
#define DL_TAMPERIO_EDGE_DETECTION_POLARITY_FALL                DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_FALL

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_BOTH
 *        边沿检测极性：双沿（上升沿和下降沿）
 */
#define DL_TAMPERIO_EDGE_DETECTION_POLARITY_BOTH            DL_LFSS_TAMPERIO_EDGE_DETECTION_POLARITY_BOTH

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_MODE_TAMPER
 *        工作模式：防篡改模式，I/O 由 LFSS 完全控制
 */
#define DL_TAMPERIO_MODE_TAMPER                             DL_LFSS_TAMPERIO_MODE_TAMPER

/*!
 * @brief Redirects to common @ref DL_LFSS_TAMPERIO_MODE_IOMUX
 *        工作模式：IOMUX 模式，I/O 复用为 SPI/UART/Timer 等第二功能
 */
#define DL_TAMPERIO_MODE_IOMUX                              DL_LFSS_TAMPERIO_MODE_IOMUX

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_0_125_SEC
 *        心跳间隔：0.125 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_0_125_SEC            DL_LFSS_HEARTBEAT_INTERVAL_0_125_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_0_25_SEC
 *        心跳间隔：0.25 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_0_25_SEC             DL_LFSS_HEARTBEAT_INTERVAL_0_25_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_05_SEC
 *        心跳间隔：0.5 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_05_SEC                DL_LFSS_HEARTBEAT_INTERVAL_05_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_1_SEC
 *        心跳间隔：1 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_1_SEC                 DL_LFSS_HEARTBEAT_INTERVAL_1_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_2_SEC
 *        心跳间隔：2 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_2_SEC                 DL_LFSS_HEARTBEAT_INTERVAL_2_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_4_SEC
 *        心跳间隔：4 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_4_SEC                  DL_LFSS_HEARTBEAT_INTERVAL_4_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_8_SEC
 *        心跳间隔：8 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_8_SEC                  DL_LFSS_HEARTBEAT_INTERVAL_8_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_INTERVAL_16_SEC
 *        心跳间隔：16 秒
 */
#define DL_TAMPERIO_HEARTBEAT_INTERVAL_16_SEC                 DL_LFSS_HEARTBEAT_INTERVAL_16_SEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_1_MSEC
 *        心跳脉冲宽度：1 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_1_MSEC              DL_LFSS_HEARTBEAT_PULSE_WIDTH_1_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_2_MSEC
 *        心跳脉冲宽度：2 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_2_MSEC               DL_LFSS_HEARTBEAT_PULSE_WIDTH_2_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_4_MSEC
 *        心跳脉冲宽度：4 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_4_MSEC               DL_LFSS_HEARTBEAT_PULSE_WIDTH_4_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_8_MSEC
 *        心跳脉冲宽度：8 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_8_MSEC                     DL_LFSS_HEARTBEAT_PULSE_WIDTH_8_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_16_MSEC
 *        心跳脉冲宽度：16 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_16_MSEC                    DL_LFSS_HEARTBEAT_PULSE_WIDTH_16_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_32_MSEC
 *        心跳脉冲宽度：32 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_32_MSEC                    DL_LFSS_HEARTBEAT_PULSE_WIDTH_32_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_64_MSEC
 *        心跳脉冲宽度：64 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_64_MSEC                    DL_LFSS_HEARTBEAT_PULSE_WIDTH_64_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_PULSE_WIDTH_128_MSEC
 *        心跳脉冲宽度：128 毫秒
 */
#define DL_TAMPERIO_HEARTBEAT_PULSE_WIDTH_128_MSEC                   DL_LFSS_HEARTBEAT_PULSE_WIDTH_128_MSEC

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_MODE_DISABLED
 *        心跳模式：禁用
 */
#define DL_TAMPERIO_HEARTBEAT_MODE_DISABLED                          DL_LFSS_HEARTBEAT_MODE_DISABLED

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_MODE_ALWAYS_ON
 *        心跳模式：常开，持续输出心跳信号
 */
#define DL_TAMPERIO_HEARTBEAT_MODE_ALWAYS_ON                         DL_LFSS_HEARTBEAT_MODE_ALWAYS_ON

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_MODE_TIME_STAMP
 *        心跳模式：时间戳触发，检测到篡改事件时输出心跳
 */
#define DL_TAMPERIO_HEARTBEAT_MODE_TIME_STAMP                        DL_LFSS_HEARTBEAT_MODE_TIME_STAMP

/*!
 * @brief Redirects to common @ref DL_LFSS_HEARTBEAT_MODE_POWER_FAIL
 *        心跳模式：掉电触发，主电源失效时输出心跳
 */
#define DL_TAMPERIO_HEARTBEAT_MODE_POWER_FAIL               DL_LFSS_HEARTBEAT_MODE_POWER_FAIL

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableOutputInversion
 *        使能输出反转功能
 */
#define DL_TamperIO_enableOutputInversion                   DL_LFSS_TamperIO_enableOutputInversion

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_isOutputInversionEnabled
 *        查询输出反转是否已使能
 */
#define DL_TamperIO_isOutputInversionEnabled                DL_LFSS_TamperIO_isOutputInversionEnabled

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableOutputInversion
 *        禁用输出反转功能
 */
#define DL_TamperIO_disableOutputInversion                  DL_LFSS_TamperIO_disableOutputInversion

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setOutputSource
 *        设置输出信号源（TOUT/LFCLK/心跳/时间戳事件）
 */
#define DL_TamperIO_setOutputSource                         DL_LFSS_TamperIO_setOutputSource

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getOutputSource
 *        获取当前输出信号源配置
 */
#define DL_TamperIO_getOutputSource                         DL_LFSS_TamperIO_getOutputSource

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setGlitchFilterWidth
 *        设置输入毛刺滤波器宽度
 */
#define DL_TamperIO_setGlitchFilterWidth                    DL_LFSS_TamperIO_setGlitchFilterWidth

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getGlitchFilterWidth
 *        获取当前毛刺滤波器宽度配置
 */
#define DL_TamperIO_getGlitchFilterWidth                    DL_LFSS_TamperIO_getGlitchFilterWidth

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setEdgeDetectionPolarity
 *        设置边沿检测极性（禁用/上升沿/下降沿/双沿）
 */
#define DL_TamperIO_setEdgeDetectionPolarity                DL_LFSS_TamperIO_setEdgeDetectionPolarity

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getEdgeDetectionPolarity
 *        获取当前边沿检测极性配置
 */
#define DL_TamperIO_getEdgeDetectionPolarity                DL_LFSS_TamperIO_getEdgeDetectionPolarity

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableInternalPullUp
 *        使能内部上拉电阻
 */
#define DL_TamperIO_enableInternalPullUp                    DL_LFSS_TamperIO_enableInternalPullUp

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_isInternalPullUpEnabled
 *        查询内部上拉电阻是否已使能
 */
#define DL_TamperIO_isInternalPullUpEnabled                 DL_LFSS_TamperIO_isInternalPullUpEnabled

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableInternalPullUp
 *        禁用内部上拉电阻
 */
#define DL_TamperIO_disableInternalPullUp                   DL_LFSS_TamperIO_disableInternalPullUp

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableInternalPullDown
 *        使能内部下拉电阻
 */
#define DL_TamperIO_enableInternalPullDown                  DL_LFSS_TamperIO_enableInternalPullDown

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_isInternalPullDownEnabled
 *        查询内部下拉电阻是否已使能
 */
#define DL_TamperIO_isInternalPullDownEnabled               DL_LFSS_TamperIO_isInternalPullDownEnabled

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableInternalPullDown
 *        禁用内部下拉电阻
 */
#define DL_TamperIO_disableInternalPullDown                 DL_LFSS_TamperIO_disableInternalPullDown

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableInput
 *        使能输入路径
 */
#define DL_TamperIO_enableInput                             DL_LFSS_TamperIO_enableInput

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_isInputEnabled
 *        查询输入路径是否已使能
 */
#define DL_TamperIO_isInputEnabled                          DL_LFSS_TamperIO_isInputEnabled

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableInput
 *        禁用输入路径
 */
#define DL_TamperIO_disableInput                            DL_LFSS_TamperIO_disableInput

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setMode
 *        设置工作模式（防篡改模式 或 IOMUX 模式）
 */
#define DL_TamperIO_setMode                                 DL_LFSS_TamperIO_setMode

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getMode
 *        获取当前工作模式
 */
#define DL_TamperIO_getMode                                 DL_LFSS_TamperIO_getMode

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableOutput
 *        使能输出路径
 */
#define DL_TamperIO_enableOutput                            DL_LFSS_TamperIO_enableOutput

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableOutput
 *        禁用输出路径
 */
#define DL_TamperIO_disableOutput                           DL_LFSS_TamperIO_disableOutput

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_isOutputEnabled
 *        查询输出路径是否已使能
 */
#define DL_TamperIO_isOutputEnabled                         DL_LFSS_TamperIO_isOutputEnabled

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setOutputValue
 *        设置输出电平值（0 或 1）
 */
#define DL_TamperIO_setOutputValue                          DL_LFSS_TamperIO_setOutputValue

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getOutputValue
 *        获取当前输出电平值
 */
#define DL_TamperIO_getOutputValue                          DL_LFSS_TamperIO_getOutputValue

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getInputValue
 *        获取当前输入电平值
 */
#define DL_TamperIO_getInputValue                           DL_LFSS_TamperIO_getInputValue

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setHeartBeatInterval
 *        设置心跳信号输出间隔
 */
#define DL_TamperIO_setHeartBeatInterval                    DL_LFSS_TamperIO_setHeartBeatInterval

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getHeartBeatInterval
 *        获取当前心跳信号输出间隔
 */
#define DL_TamperIO_getHeartBeatInterval                    DL_LFSS_TamperIO_getHeartBeatInterval

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setHeartBeatPulseWidth
 *        设置心跳信号脉冲宽度
 */
#define DL_TamperIO_setHeartBeatPulseWidth                  DL_LFSS_TamperIO_setHeartBeatPulseWidth

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getHeartBeatPulseWidth
 *        获取当前心跳信号脉冲宽度
 */
#define DL_TamperIO_getHeartBeatPulseWidth                  DL_LFSS_TamperIO_getHeartBeatPulseWidth

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setHeartBeatMode
 *        设置心跳信号模式（禁用/常开/时间戳/掉电）
 */
#define DL_TamperIO_setHeartBeatMode                        DL_LFSS_TamperIO_setHeartBeatMode

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getHeartBeatMode
 *        获取当前心跳信号模式
 */
#define DL_TamperIO_getHeartBeatMode                        DL_LFSS_TamperIO_getHeartBeatMode

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableWriteProtectLock
 *        使能写保护锁定，锁定后寄存器不可写入
 */
#define DL_TamperIO_enableWriteProtectLock                  DL_LFSS_TamperIO_enableWriteProtectLock

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_isWriteProtectLockEnabled
 *        查询写保护锁定是否已使能
 */
#define DL_TamperIO_isWriteProtectLockEnabled               DL_LFSS_TamperIO_isWriteProtectLockEnabled

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableWriteProtectLock
 *        禁用写保护锁定
 */
#define DL_TamperIO_disableWriteProtectLock                 DL_LFSS_TamperIO_disableWriteProtectLock

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableInterrupt
 *        使能指定中断源
 */
#define DL_TamperIO_enableInterrupt                         DL_LFSS_TamperIO_enableInterrupt

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableInterrupt
 *        禁用指定中断源
 */
#define DL_TamperIO_disableInterrupt                        DL_LFSS_TamperIO_disableInterrupt

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getEnabledInterrupts
 *        获取所有已使能的中断掩码
 */
#define DL_TamperIO_getEnabledInterrupts                    DL_LFSS_TamperIO_getEnabledInterrupts

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getEnabledInterruptStatus
 *        获取已使能中断的挂起状态
 */
#define DL_TamperIO_getEnabledInterruptStatus               DL_LFSS_TamperIO_getEnabledInterruptStatus

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getRawInterruptStatus
 *        获取原始中断状态（不受使能掩码影响）
 */
#define DL_TamperIO_getRawInterruptStatus                   DL_LFSS_TamperIO_getRawInterruptStatus

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getPendingInterrupt
 *        获取最高优先级的待处理中断索引
 */
#define DL_TamperIO_getPendingInterrupt                     DL_LFSS_TamperIO_getPendingInterrupt

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_clearInterruptStatus
 *        清除指定中断挂起状态
 */
#define DL_TamperIO_clearInterruptStatus                    DL_LFSS_TamperIO_clearInterruptStatus

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_enableEvent
 *        使能指定事件
 */
#define DL_TamperIO_enableEvent                             DL_LFSS_TamperIO_enableEvent

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_disableEvent
 *        禁用指定事件
 */
#define DL_TamperIO_disableEvent                            DL_LFSS_TamperIO_disableEvent

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getEnabledEvents
 *        获取所有已使能的事件掩码
 */
#define DL_TamperIO_getEnabledEvents                        DL_LFSS_TamperIO_getEnabledEvents

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getEnabledEventStatus
 *        获取已使能事件的挂起状态
 */
#define DL_TamperIO_getEnabledEventStatus                   DL_LFSS_TamperIO_getEnabledEventStatus

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getRawEventsStatus
 *        获取原始事件状态（不受使能掩码影响）
 */
#define DL_TamperIO_getRawEventsStatus                      DL_LFSS_TamperIO_getRawEventsStatus

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_clearEventsStatus
 *        清除指定事件挂起状态
 */
#define DL_TamperIO_clearEventsStatus                       DL_LFSS_TamperIO_clearEventsStatus

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_setPublisherChanID
 *        设置事件发布者通道 ID
 */
#define DL_TamperIO_setPublisherChanID                    DL_LFSS_TamperIO_setPublisherChanID

/*!
 * @brief Redirects to common @ref DL_LFSS_TamperIO_getPublisherChanID
 *        获取事件发布者通道 ID
 */
#define DL_TamperIO_getPublisherChanID                    DL_LFSS_TamperIO_getPublisherChanID

/* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_TIO__ */

#endif /* ti_dl_dl_tio__include */
/** @}*/
