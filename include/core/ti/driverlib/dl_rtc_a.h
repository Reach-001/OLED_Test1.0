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
 * PURPOSE ARE DISCLAIMED. IN NO SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY TfHEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_rtc_a.h
 *  @brief      Real Time Clock A (RTC_A) Peripheral Interface
 *  @defgroup   RTC_A Real Time Clock A (RTC_A)
 *
 *  @anchor ti_devices_msp_dl_rtc_a_Overview
 *  # Overview
 *  The RTC_A Driver Library allows full configuration of the MSPM0 RTC_A module.
 *  The real-time clock (RTC_A) module provides clock counters with calendar mode,
 *  a flexible programmable alarm, offset calibration, and a provision for
 *  temperature compensation. The module also provides a separate time stamp calendar
 *  capable of capturing a time stamp event upon loss of VDD or detection of a tamper event
 *  on a tamper I/O pin.
 *
 *  <hr>
 ******************************************************************************
 */
/**
 * @par 中文说明
 * MSPM0 RTC_A 增强型实时时钟驱动接口头文件。
 * 本文件将所有 RTC_A API 宏定义重定向到 dl_rtc_common 中的通用实现，
 * 除日历模式、闹钟、校准等基础功能外，还支持时间戳日历和防篡改检测。
 */
/** @addtogroup RTC_A
 * @{
 */
#ifndef ti_dl_dl_rtc_a__include
#define ti_dl_dl_rtc_a__include

#include <ti/driverlib/dl_rtc_common.h>

#ifdef __MSPM0_HAS_RTC_A__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_RTC_A_INTERRUPT
 *  @{
 *  @par 中文说明
 *  RTC_A 中断使能位掩码定义
 */

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM1 日历闹钟1中断
 */
#define DL_RTC_A_INTERRUPT_CALENDAR_ALARM1          (DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM2 日历闹钟2中断
 */
#define DL_RTC_A_INTERRUPT_CALENDAR_ALARM2          (DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_PRESCALER0 预分频器0中断
 */
#define DL_RTC_A_INTERRUPT_PRESCALER0               (DL_RTC_COMMON_INTERRUPT_PRESCALER0)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_PRESCALER1 预分频器1中断
 */
#define DL_RTC_A_INTERRUPT_PRESCALER1               (DL_RTC_COMMON_INTERRUPT_PRESCALER1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_INTERVAL_ALARM 间隔闹钟中断
 */
#define DL_RTC_A_INTERRUPT_INTERVAL_ALARM           (DL_RTC_COMMON_INTERRUPT_INTERVAL_ALARM)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_READY 就绪中断
 */
#define DL_RTC_A_INTERRUPT_READY                    (DL_RTC_COMMON_INTERRUPT_READY)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_READY 时间戳事件中断
 */
#define DL_RTC_A_INTERRUPT_TSEVT                    (DL_RTC_COMMON_INTERRUPT_TSEVT)


/** @}*/

/** @addtogroup DL_RTC_A_EVENT
 *  @{
 *  @par 中文说明
 *  RTC_A 事件使能位掩码定义（用于事件发布机制）
 */

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_CALENDAR_ALARM1 日历闹钟1事件
 */
#define DL_RTC_A_EVENT_CALENDAR_ALARM1          (DL_RTC_COMMON_EVENT_CALENDAR_ALARM1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_CALENDAR_ALARM2 日历闹钟2事件
 */
#define DL_RTC_A_EVENT_CALENDAR_ALARM2          (DL_RTC_COMMON_EVENT_CALENDAR_ALARM2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_PRESCALER0 预分频器0事件
 */
#define DL_RTC_A_EVENT_PRESCALER0               (DL_RTC_COMMON_EVENT_PRESCALER0)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_PRESCALER1 预分频器1事件
 */
#define DL_RTC_A_EVENT_PRESCALER1               (DL_RTC_COMMON_EVENT_PRESCALER1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_INTERVAL_ALARM 间隔闹钟事件
 */
#define DL_RTC_A_EVENT_INTERVAL_ALARM           (DL_RTC_COMMON_EVENT_INTERVAL_ALARM)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_READY 就绪事件
 */
#define DL_RTC_A_EVENT_READY                    (DL_RTC_COMMON_EVENT_READY)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_READY 时间戳事件
 */
#define DL_RTC_A_EVENT_TSEVT                    (DL_RTC_COMMON_EVENT_TSEVT)

/** @}*/

/** @addtogroup DL_RTC_A_TIME_STAMP_EVENT_CAUSE
 *  @{
 *  @par 中文说明
 *  时间戳事件触发原因，用于查询是哪个引脚或VDD掉电触发了时间戳
 */

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_0 防篡改引脚TIO_0触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_0       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_0)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_1 防篡改引脚TIO_1触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_1       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_2 防篡改引脚TIO_2触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_2       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_3 防篡改引脚TIO_3触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_3       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_3)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_4 防篡改引脚TIO_4触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_4       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_4)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_5 防篡改引脚TIO_5触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_5       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_5)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_6 防篡改引脚TIO_6触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_6       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_6)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_7 防篡改引脚TIO_7触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_7       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_7)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_8 防篡改引脚TIO_8触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_8       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_8)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_9 防篡改引脚TIO_9触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_9       (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_9)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_10 防篡改引脚TIO_10触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_10      (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_10)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_11 防篡改引脚TIO_11触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_11      (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_11)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_12 防篡改引脚TIO_12触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_12      (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_12)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_13 防篡改引脚TIO_13触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_13      (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_13)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_14 防篡改引脚TIO_14触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_14      (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_14)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_15 防篡改引脚TIO_15触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_15      (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_TIO_15)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_VDD_LOSS VDD掉电触发
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAUSE_VDD_LOSS    (DL_RTC_COMMON_TIME_STAMP_EVENT_CAUSE_VDD_LOSS)

/** @}*/

/** @addtogroup DL_RTC_A_TIME_STAMP_EVENT_SOURCE
 *  @{
 *  @par 中文说明
 *  时间戳事件源配置，选择哪个引脚或VDD掉电作为时间戳触发源
 */

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_0 选择TIO_0作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_0       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_0)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_1 选择TIO_1作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_1       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_2 选择TIO_2作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_2       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_3 选择TIO_3作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_3       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_3)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_4 选择TIO_4作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_4       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_4)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_5 选择TIO_5作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_5       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_5)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_6 选择TIO_6作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_6       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_6)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_7 选择TIO_7作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_7       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_7)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_8 选择TIO_8作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_8       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_8)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_9 选择TIO_9作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_9       (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_9)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_10 选择TIO_10作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_10      (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_10)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_11 选择TIO_11作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_11      (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_11)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_12 选择TIO_12作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_12      (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_12)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_13 选择TIO_13作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_13      (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_13)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_14 选择TIO_14作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_14      (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_14)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_15 选择TIO_15作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_15      (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_TIO_15)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_VDD_LOSS 选择VDD掉电作为时间戳源
 */
#define DL_RTC_A_TIME_STAMP_EVENT_SOURCE_VDD_LOSS    (DL_RTC_COMMON_TIME_STAMP_EVENT_SOURCE_VDD_LOSS)

/** @}*/

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_FORMAT_BINARY 二进制格式
 */
#define DL_RTC_A_FORMAT_BINARY                      DL_RTC_COMMON_FORMAT_BINARY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_FORMAT_BCD BCD码格式
 */
#define DL_RTC_A_FORMAT_BCD                         DL_RTC_COMMON_FORMAT_BCD

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_STATUS_READY 就绪状态
 */
#define DL_RTC_A_STATUS_READY                       DL_RTC_COMMON_STATUS_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_STATUS_NOT_READY 未就绪状态
 */
#define DL_RTC_A_STATUS_NOT_READY                   DL_RTC_COMMON_STATUS_NOT_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_STATUS_READY 补偿就绪
 */
#define DL_RTC_A_COMPENSATION_STATUS_READY          DL_RTC_COMMON_COMPENSATION_STATUS_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_STATUS_NOT_READY 补偿未就绪
 */
#define DL_RTC_A_COMPENSATION_STATUS_NOT_READY      DL_RTC_COMMON_COMPENSATION_STATUS_NOT_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_OK 补偿写入成功
 */
#define DL_RTC_A_COMPENSATION_WRITE_RESULT_OK       DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_OK

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_NOT_OK 补偿写入失败
 */
#define DL_RTC_A_COMPENSATION_WRITE_RESULT_NOT_OK   DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_NOT_OK

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_DOWN 偏移校准方向：减慢
 */
#define DL_RTC_A_OFFSET_CALIBRATION_SIGN_DOWN       DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_DOWN

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_UP 偏移校准方向：加快
 */
#define DL_RTC_A_OFFSET_CALIBRATION_SIGN_UP         DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_UP

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_32_KHZ 校准频率输出：32.768kHz
 */
#define DL_RTC_A_OFFSET_CALIBRATION_FREQUENCY_32_KHZ        DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_32_KHZ

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_512 校准频率输出：512Hz
 */
#define DL_RTC_A_OFFSET_CALIBRATION_FREQUENCY_512              DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_512

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_256 校准频率输出：256Hz
 */
#define DL_RTC_A_OFFSET_CALIBRATION_FREQUENCY_256              DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_256

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_1 校准频率输出：1Hz
 */
#define DL_RTC_A_OFFSET_CALIBRATION_FREQUENCY_1              DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_TEMP_CALIBRATION_DOWN1PPM 温度补偿：降低1ppm
 */
#define DL_RTC_A_TEMP_CALIBRATION_DOWN1PPM                  DL_RTC_COMMON_TEMP_CALIBRATION_DOWN1PPM

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_TEMP_CALIBRATION_UP1PPM 温度补偿：提高1ppm
 */
#define DL_RTC_A_TEMP_CALIBRATION_UP1PPM                    DL_RTC_COMMON_TEMP_CALIBRATION_UP1PPM

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_MINUTECHANGE 分钟变化间隔闹钟
 */
#define DL_RTC_A_INTERVAL_ALARM_MINUTECHANGE                DL_RTC_COMMON_INTERVAL_ALARM_MINUTECHANGE

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_HOURCHANGE 小时变化间隔闹钟
 */
#define DL_RTC_A_INTERVAL_ALARM_HOURCHANGE                  DL_RTC_COMMON_INTERVAL_ALARM_HOURCHANGE

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_NOON 正午间隔闹钟
 */
#define DL_RTC_A_INTERVAL_ALARM_NOON                        DL_RTC_COMMON_INTERVAL_ALARM_NOON

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_MIDNIGHT 午夜间隔闹钟
 */
#define DL_RTC_A_INTERVAL_ALARM_MIDNIGHT                    DL_RTC_COMMON_INTERVAL_ALARM_MIDNIGHT

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_8 预分频器0：8分频
 */
#define DL_RTC_A_PRESCALER0_DIVIDE_8                        DL_RTC_COMMON_PRESCALER0_DIVIDE_8

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_16 预分频器0：16分频
 */
#define DL_RTC_A_PRESCALER0_DIVIDE_16                       DL_RTC_COMMON_PRESCALER0_DIVIDE_16

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_32 预分频器0：32分频
 */
#define DL_RTC_A_PRESCALER0_DIVIDE_32                       DL_RTC_COMMON_PRESCALER0_DIVIDE_32

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_64 预分频器0：64分频
 */
#define DL_RTC_A_PRESCALER0_DIVIDE_64                       DL_RTC_COMMON_PRESCALER0_DIVIDE_64

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_128 预分频器0：128分频
 */
#define DL_RTC_A_PRESCALER0_DIVIDE_128                      DL_RTC_COMMON_PRESCALER0_DIVIDE_128

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_256 预分频器0：256分频
 */
#define DL_RTC_A_PRESCALER0_DIVIDE_256                      DL_RTC_COMMON_PRESCALER0_DIVIDE_256

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_2 预分频器1：2分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_2                        DL_RTC_COMMON_PRESCALER1_DIVIDE_2

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_4 预分频器1：4分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_4                        DL_RTC_COMMON_PRESCALER1_DIVIDE_4

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_8 预分频器1：8分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_8                        DL_RTC_COMMON_PRESCALER1_DIVIDE_8

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_16 预分频器1：16分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_16                       DL_RTC_COMMON_PRESCALER1_DIVIDE_16

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_32 预分频器1：32分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_32                       DL_RTC_COMMON_PRESCALER1_DIVIDE_32

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_64 预分频器1：64分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_64                       DL_RTC_COMMON_PRESCALER1_DIVIDE_64

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_128 预分频器1：128分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_128                      DL_RTC_COMMON_PRESCALER1_DIVIDE_128

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_256 预分频器1：256分频
 */
#define DL_RTC_A_PRESCALER1_DIVIDE_256                      DL_RTC_COMMON_PRESCALER1_DIVIDE_256

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_ALARM1 闹钟1中断索引
 */
#define DL_RTC_A_IIDX_ALARM1                                DL_RTC_COMMON_IIDX_ALARM1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_ALARM2 闹钟2中断索引
 */
#define DL_RTC_A_IIDX_ALARM2                                DL_RTC_COMMON_IIDX_ALARM2

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_PRESCALER0 预分频器0中断索引
 */
#define DL_RTC_A_IIDX_PRESCALER0                            DL_RTC_COMMON_IIDX_PRESCALER0

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_PRESCALER1 预分频器1中断索引
 */
#define DL_RTC_A_IIDX_PRESCALER1                            DL_RTC_COMMON_IIDX_PRESCALER1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_INTERVAL_TIMER 间隔定时器中断索引
 */
#define DL_RTC_A_IIDX_INTERVAL_TIMER                        DL_RTC_COMMON_IIDX_INTERVAL_TIMER

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_READY 就绪中断索引
 */
#define DL_RTC_A_IIDX_READY                                 DL_RTC_COMMON_IIDX_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TSEVT 时间戳事件中断索引
 */
#define DL_RTC_A_IIDX_TSEVT                                 DL_RTC_COMMON_IIDX_TSEVT

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_0 防篡改IO_0中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_0                           DL_RTC_COMMON_IIDX_TAMPER_IO_0

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_1 防篡改IO_1中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_1                           DL_RTC_COMMON_IIDX_TAMPER_IO_1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_2 防篡改IO_2中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_2                           DL_RTC_COMMON_IIDX_TAMPER_IO_2

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_3 防篡改IO_3中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_3                         DL_RTC_COMMON_IIDX_TAMPER_IO_3

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_4 防篡改IO_4中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_4                           DL_RTC_COMMON_IIDX_TAMPER_IO_4

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_5 防篡改IO_5中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_5                           DL_RTC_COMMON_IIDX_TAMPER_IO_5

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_6 防篡改IO_6中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_6                           DL_RTC_COMMON_IIDX_TAMPER_IO_6

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_7 防篡改IO_7中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_7                           DL_RTC_COMMON_IIDX_TAMPER_IO_7

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_8 防篡改IO_8中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_8                           DL_RTC_COMMON_IIDX_TAMPER_IO_8

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_9 防篡改IO_9中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_9                           DL_RTC_COMMON_IIDX_TAMPER_IO_9

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_10 防篡改IO_10中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_10                          DL_RTC_COMMON_IIDX_TAMPER_IO_10

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_11 防篡改IO_11中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_11                          DL_RTC_COMMON_IIDX_TAMPER_IO_11

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_12 防篡改IO_12中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_12                          DL_RTC_COMMON_IIDX_TAMPER_IO_12

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_13 防篡改IO_13中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_13                          DL_RTC_COMMON_IIDX_TAMPER_IO_13

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_14 防篡改IO_14中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_14                          DL_RTC_COMMON_IIDX_TAMPER_IO_14

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_TAMPER_IO_15 防篡改IO_15中断索引
 */
#define DL_RTC_A_IIDX_TAMPER_IO_15                          DL_RTC_COMMON_IIDX_TAMPER_IO_15

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAPTURE_FIRST 仅捕获首次时间戳
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAPTURE_FIRST             DL_RTC_COMMON_TIME_STAMP_EVENT_CAPTURE_FIRST

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_TIME_STAMP_EVENT_CAPTURE_LAST 仅捕获末次时间戳
 */
#define DL_RTC_A_TIME_STAMP_EVENT_CAPTURE_LAST              DL_RTC_COMMON_TIME_STAMP_EVENT_CAPTURE_LAST

/**
 * @brief Redirects to common @ref DL_RTC_Common_Calendar 日历数据结构体
 *
 */
typedef DL_RTC_Common_Calendar                      DL_RTC_A_Calendar;

/**
 * @brief Redirects to common @ref DL_RTC_Common_CalendarAlarm 日历闹钟数据结构体
 *
 */
typedef DL_RTC_Common_CalendarAlarm                 DL_RTC_A_CalendarAlarm;


/* --- 时钟源与复位控制 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isClockSourceLFCLK 查询时钟源是否为LFCLK
 */
#define DL_RTC_A_isClockSourceLFCLK                 DL_RTC_Common_isClockSourceLFCLK

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableClockControl 使能RTC时钟控制
 */
#define DL_RTC_A_enableClockControl                 DL_RTC_Common_enableClockControl

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableClockControl 关闭RTC时钟控制
 */
#define DL_RTC_A_disableClockControl                DL_RTC_Common_disableClockControl

/*!
 * @brief Redirects to common @ref DL_RTC_Common_reset 复位RTC模块
 */
#define DL_RTC_A_reset                              DL_RTC_Common_reset

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isReset 查询RTC是否处于复位状态
 */
#define DL_RTC_A_isReset                            DL_RTC_Common_isReset

/* --- 调试模式与时间格式 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableDebugInterrupts 使能调试中断
 */
#define DL_RTC_A_enableDebugInterrupts              DL_RTC_Common_enableDebugInterrupts

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableDebugInterrupts 关闭调试中断
 */
#define DL_RTC_A_disableDebugInterrupts             DL_RTC_Common_disableDebugInterrupts

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableDebugMode 使能调试模式
 */
#define DL_RTC_A_enableDebugMode                    DL_RTC_Common_enableDebugMode

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableDebugMode 关闭调试模式
 */
#define DL_RTC_A_disableDebugMode                   DL_RTC_Common_disableDebugMode

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setClockFormat 设置时钟格式（二进制/BCD）
 */
#define DL_RTC_A_setClockFormat                     DL_RTC_Common_setClockFormat

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getClockFormat 获取当前时钟格式
 */
#define DL_RTC_A_getClockFormat                     DL_RTC_Common_getClockFormat

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setIntervalAlarm 设置间隔闹钟
 */
#define DL_RTC_A_setIntervalAlarm                   DL_RTC_Common_setIntervalAlarm

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getIntervalAlarm 获取间隔闹钟配置
 */
#define DL_RTC_A_getIntervalAlarm                   DL_RTC_Common_getIntervalAlarm

/* --- 校准与温度补偿 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isSafeToRead 查询RTC寄存器是否可安全读取
 */
#define DL_RTC_A_isSafeToRead                       DL_RTC_Common_isSafeToRead

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isReadyToCalibrate 查询校准是否就绪
 */
#define DL_RTC_A_isReadyToCalibrate                 DL_RTC_Common_isReadyToCalibrate

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isCalibrationWriteResultOK 查询校准写入是否成功
 */
#define DL_RTC_A_isCalibrationWriteResultOK         DL_RTC_Common_isCalibrationWriteResultOK

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setOffsetCalibrationFrequency 设置偏移校准输出频率
 */
#define DL_RTC_A_setOffsetCalibrationFrequency      DL_RTC_Common_setOffsetCalibrationFrequency

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setOffsetCalibrationAdjValue 设置偏移校准调整值
 */
#define DL_RTC_A_setOffsetCalibrationAdjValue       DL_RTC_Common_setOffsetCalibrationAdjValue

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setOffsetCalibration 配置偏移校准（方向与值）
 */
#define DL_RTC_A_setOffsetCalibration               DL_RTC_Common_setOffsetCalibration

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getOffsetCalibrationFrequency 获取偏移校准输出频率
 */
#define DL_RTC_A_getOffsetCalibrationFrequency      DL_RTC_Common_getOffsetCalibrationFrequency

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getOffsetCalibrationSign 获取偏移校准方向
 */
#define DL_RTC_A_getOffsetCalibrationSign           DL_RTC_Common_getOffsetCalibrationSign

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setTemperatureCompensation 设置温度补偿
 */
#define DL_RTC_A_setTemperatureCompensation         DL_RTC_Common_setTemperatureCompensation

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTemperatureCompensation 获取温度补偿配置
 */
#define DL_RTC_A_getTemperatureCompensation         DL_RTC_Common_getTemperatureCompensation

/* --- 日历时间 BCD 格式读写 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarSecondsBCD 设置秒（BCD）
 */
#define DL_RTC_A_setCalendarSecondsBCD              DL_RTC_Common_setCalendarSecondsBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarSecondsBCD 获取秒（BCD）
 */
#define DL_RTC_A_getCalendarSecondsBCD              DL_RTC_Common_getCalendarSecondsBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMinutesBCD 设置分（BCD）
 */
#define DL_RTC_A_setCalendarMinutesBCD              DL_RTC_Common_setCalendarMinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMinutesBCD 获取分（BCD）
 */
#define DL_RTC_A_getCalendarMinutesBCD              DL_RTC_Common_getCalendarMinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarHoursBCD 设置时（BCD）
 */
#define DL_RTC_A_setCalendarHoursBCD                DL_RTC_Common_setCalendarHoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarHoursBCD 获取时（BCD）
 */
#define DL_RTC_A_getCalendarHoursBCD                DL_RTC_Common_getCalendarHoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfWeekBCD 设置星期（BCD）
 */
#define DL_RTC_A_setCalendarDayOfWeekBCD            DL_RTC_Common_setCalendarDayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfWeekBCD 获取星期（BCD）
 */
#define DL_RTC_A_getCalendarDayOfWeekBCD            DL_RTC_Common_getCalendarDayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfMonthBCD 设置日（BCD）
 */
#define DL_RTC_A_setCalendarDayOfMonthBCD           DL_RTC_Common_setCalendarDayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfMonthBCD 获取日（BCD）
 */
#define DL_RTC_A_getCalendarDayOfMonthBCD           DL_RTC_Common_getCalendarDayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMonthBCD 设置月（BCD）
 */
#define DL_RTC_A_setCalendarMonthBCD                DL_RTC_Common_setCalendarMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMonthBCD 获取月（BCD）
 */
#define DL_RTC_A_getCalendarMonthBCD                DL_RTC_Common_getCalendarMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarYearBCD 设置年（BCD）
 */
#define DL_RTC_A_setCalendarYearBCD                 DL_RTC_Common_setCalendarYearBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarYearBCD 获取年（BCD）
 */
#define DL_RTC_A_getCalendarYearBCD                 DL_RTC_Common_getCalendarYearBCD

/* --- 闹钟1 BCD 格式配置 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1MinutesBCD 使能闹钟1分钟匹配（BCD）
 */
#define DL_RTC_A_enableAlarm1MinutesBCD             DL_RTC_Common_enableAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1MinutesBCD 关闭闹钟1分钟匹配（BCD）
 */
#define DL_RTC_A_disableAlarm1MinutesBCD            DL_RTC_Common_disableAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1MinutesBCD 设置闹钟1分钟（BCD）
 */
#define DL_RTC_A_setAlarm1MinutesBCD                DL_RTC_Common_setAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1MinutesBCD 获取闹钟1分钟（BCD）
 */
#define DL_RTC_A_getAlarm1MinutesBCD                DL_RTC_Common_getAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1HoursBCD 使能闹钟1小时匹配（BCD）
 */
#define DL_RTC_A_enableAlarm1HoursBCD               DL_RTC_Common_enableAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1HoursBCD 关闭闹钟1小时匹配（BCD）
 */
#define DL_RTC_A_disableAlarm1HoursBCD              DL_RTC_Common_disableAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1HoursBCD 设置闹钟1小时（BCD）
 */
#define DL_RTC_A_setAlarm1HoursBCD                  DL_RTC_Common_setAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1HoursBCD 获取闹钟1小时（BCD）
 */
#define DL_RTC_A_getAlarm1HoursBCD                  DL_RTC_Common_getAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfWeekBCD 使能闹钟1星期匹配（BCD）
 */
#define DL_RTC_A_enableAlarm1DayOfWeekBCD           DL_RTC_Common_enableAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfWeekBCD 关闭闹钟1星期匹配（BCD）
 */
#define DL_RTC_A_disableAlarm1DayOfWeekBCD          DL_RTC_Common_disableAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfWeekBCD 设置闹钟1星期（BCD）
 */
#define DL_RTC_A_setAlarm1DayOfWeekBCD              DL_RTC_Common_setAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfWeekBCD 获取闹钟1星期（BCD）
 */
#define DL_RTC_A_getAlarm1DayOfWeekBCD              DL_RTC_Common_getAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfMonthBCD 使能闹钟1日期匹配（BCD）
 */
#define DL_RTC_A_enableAlarm1DayOfMonthBCD          DL_RTC_Common_enableAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfMonthBCD 关闭闹钟1日期匹配（BCD）
 */
#define DL_RTC_A_disableAlarm1DayOfMonthBCD         DL_RTC_Common_disableAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfMonthBCD 设置闹钟1日期（BCD）
 */
#define DL_RTC_A_setAlarm1DayOfMonthBCD             DL_RTC_Common_setAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfMonthBCD 获取闹钟1日期（BCD）
 */
#define DL_RTC_A_getAlarm1DayOfMonthBCD             DL_RTC_Common_getAlarm1DayOfMonthBCD

/* --- 闹钟2 BCD 格式配置 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2MinutesBCD 使能闹钟2分钟匹配（BCD）
 */
#define DL_RTC_A_enableAlarm2MinutesBCD             DL_RTC_Common_enableAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2MinutesBCD 关闭闹钟2分钟匹配（BCD）
 */
#define DL_RTC_A_disableAlarm2MinutesBCD            DL_RTC_Common_disableAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2MinutesBCD 设置闹钟2分钟（BCD）
 */
#define DL_RTC_A_setAlarm2MinutesBCD                DL_RTC_Common_setAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2MinutesBCD 获取闹钟2分钟（BCD）
 */
#define DL_RTC_A_getAlarm2MinutesBCD                DL_RTC_Common_getAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2HoursBCD 使能闹钟2小时匹配（BCD）
 */
#define DL_RTC_A_enableAlarm2HoursBCD               DL_RTC_Common_enableAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2HoursBCD 关闭闹钟2小时匹配（BCD）
 */
#define DL_RTC_A_disableAlarm2HoursBCD              DL_RTC_Common_disableAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2HoursBCD 设置闹钟2小时（BCD）
 */
#define DL_RTC_A_setAlarm2HoursBCD                  DL_RTC_Common_setAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2HoursBCD 获取闹钟2小时（BCD）
 */
#define DL_RTC_A_getAlarm2HoursBCD                  DL_RTC_Common_getAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfWeekBCD 使能闹钟2星期匹配（BCD）
 */
#define DL_RTC_A_enableAlarm2DayOfWeekBCD           DL_RTC_Common_enableAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfWeekBCD 关闭闹钟2星期匹配（BCD）
 */
#define DL_RTC_A_disableAlarm2DayOfWeekBCD          DL_RTC_Common_disableAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfWeekBCD 设置闹钟2星期（BCD）
 */
#define DL_RTC_A_setAlarm2DayOfWeekBCD              DL_RTC_Common_setAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfWeekBCD 获取闹钟2星期（BCD）
 */
#define DL_RTC_A_getAlarm2DayOfWeekBCD              DL_RTC_Common_getAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfMonthBCD 使能闹钟2日期匹配（BCD）
 */
#define DL_RTC_A_enableAlarm2DayOfMonthBCD          DL_RTC_Common_enableAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfMonthBCD 关闭闹钟2日期匹配（BCD）
 */
#define DL_RTC_A_disableAlarm2DayOfMonthBCD         DL_RTC_Common_disableAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfMonthBCD 设置闹钟2日期（BCD）
 */
#define DL_RTC_A_setAlarm2DayOfMonthBCD             DL_RTC_Common_setAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfMonthBCD 获取闹钟2日期（BCD）
 */
#define DL_RTC_A_getAlarm2DayOfMonthBCD             DL_RTC_Common_getAlarm2DayOfMonthBCD

/* --- 日历时间二进制格式读写 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarSecondsBinary 设置秒（二进制）
 */
#define DL_RTC_A_setCalendarSecondsBinary           DL_RTC_Common_setCalendarSecondsBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarSecondsBinary 获取秒（二进制）
 */
#define DL_RTC_A_getCalendarSecondsBinary           DL_RTC_Common_getCalendarSecondsBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMinutesBinary 设置分（二进制）
 */
#define DL_RTC_A_setCalendarMinutesBinary           DL_RTC_Common_setCalendarMinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMinutesBinary 获取分（二进制）
 */
#define DL_RTC_A_getCalendarMinutesBinary           DL_RTC_Common_getCalendarMinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarHoursBinary 设置时（二进制）
 */
#define DL_RTC_A_setCalendarHoursBinary             DL_RTC_Common_setCalendarHoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarHoursBinary 获取时（二进制）
 */
#define DL_RTC_A_getCalendarHoursBinary             DL_RTC_Common_getCalendarHoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfWeekBinary 设置星期（二进制）
 */
#define DL_RTC_A_setCalendarDayOfWeekBinary         DL_RTC_Common_setCalendarDayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfWeekBinary 获取星期（二进制）
 */
#define DL_RTC_A_getCalendarDayOfWeekBinary         DL_RTC_Common_getCalendarDayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfMonthBinary 设置日（二进制）
 */
#define DL_RTC_A_setCalendarDayOfMonthBinary        DL_RTC_Common_setCalendarDayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfMonthBinary 获取日（二进制）
 */
#define DL_RTC_A_getCalendarDayOfMonthBinary        DL_RTC_Common_getCalendarDayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMonthBinary 设置月（二进制）
 */
#define DL_RTC_A_setCalendarMonthBinary             DL_RTC_Common_setCalendarMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMonthBinary 获取月（二进制）
 */
#define DL_RTC_A_getCalendarMonthBinary             DL_RTC_Common_getCalendarMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarYearBinary 设置年（二进制）
 */
#define DL_RTC_A_setCalendarYearBinary              DL_RTC_Common_setCalendarYearBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarYearBinary 获取年（二进制）
 */
#define DL_RTC_A_getCalendarYearBinary              DL_RTC_Common_getCalendarYearBinary

/* --- 闹钟1 二进制格式配置 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1MinutesBinary 使能闹钟1分钟匹配（二进制）
 */
#define DL_RTC_A_enableAlarm1MinutesBinary          DL_RTC_Common_enableAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1MinutesBinary 关闭闹钟1分钟匹配（二进制）
 */
#define DL_RTC_A_disableAlarm1MinutesBinary         DL_RTC_Common_disableAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1MinutesBinary 设置闹钟1分钟（二进制）
 */
#define DL_RTC_A_setAlarm1MinutesBinary             DL_RTC_Common_setAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1MinutesBinary 获取闹钟1分钟（二进制）
 */
#define DL_RTC_A_getAlarm1MinutesBinary             DL_RTC_Common_getAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1HoursBinary 使能闹钟1小时匹配（二进制）
 */
#define DL_RTC_A_enableAlarm1HoursBinary            DL_RTC_Common_enableAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1HoursBinary 关闭闹钟1小时匹配（二进制）
 */
#define DL_RTC_A_disableAlarm1HoursBinary           DL_RTC_Common_disableAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1HoursBinary 设置闹钟1小时（二进制）
 */
#define DL_RTC_A_setAlarm1HoursBinary               DL_RTC_Common_setAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1HoursBinary 获取闹钟1小时（二进制）
 */
#define DL_RTC_A_getAlarm1HoursBinary               DL_RTC_Common_getAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfWeekBinary 使能闹钟1星期匹配（二进制）
 */
#define DL_RTC_A_enableAlarm1DayOfWeekBinary        DL_RTC_Common_enableAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfWeekBinary 关闭闹钟1星期匹配（二进制）
 */
#define DL_RTC_A_disableAlarm1DayOfWeekBinary       DL_RTC_Common_disableAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfWeekBinary 设置闹钟1星期（二进制）
 */
#define DL_RTC_A_setAlarm1DayOfWeekBinary           DL_RTC_Common_setAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfWeekBinary 获取闹钟1星期（二进制）
 */
#define DL_RTC_A_getAlarm1DayOfWeekBinary           DL_RTC_Common_getAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfMonthBinary 使能闹钟1日期匹配（二进制）
 */
#define DL_RTC_A_enableAlarm1DayOfMonthBinary       DL_RTC_Common_enableAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfMonthBinary 关闭闹钟1日期匹配（二进制）
 */
#define DL_RTC_A_disableAlarm1DayOfMonthBinary      DL_RTC_Common_disableAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfMonthBinary 设置闹钟1日期（二进制）
 */
#define DL_RTC_A_setAlarm1DayOfMonthBinary          DL_RTC_Common_setAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfMonthBinary 获取闹钟1日期（二进制）
 */
#define DL_RTC_A_getAlarm1DayOfMonthBinary          DL_RTC_Common_getAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2MinutesBinary 使能闹钟2分钟匹配（二进制）
 */
#define DL_RTC_A_enableAlarm2MinutesBinary          DL_RTC_Common_enableAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2MinutesBinary 关闭闹钟2分钟匹配（二进制）
 */
#define DL_RTC_A_disableAlarm2MinutesBinary         DL_RTC_Common_disableAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2MinutesBinary 设置闹钟2分钟（二进制）
 */
#define DL_RTC_A_setAlarm2MinutesBinary             DL_RTC_Common_setAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2MinutesBinary 获取闹钟2分钟（二进制）
 */
#define DL_RTC_A_getAlarm2MinutesBinary             DL_RTC_Common_getAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2HoursBinary 使能闹钟2小时匹配（二进制）
 */
#define DL_RTC_A_enableAlarm2HoursBinary            DL_RTC_Common_enableAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2HoursBinary 关闭闹钟2小时匹配（二进制）
 */
#define DL_RTC_A_disableAlarm2HoursBinary           DL_RTC_Common_disableAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2HoursBinary 设置闹钟2小时（二进制）
 */
#define DL_RTC_A_setAlarm2HoursBinary               DL_RTC_Common_setAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2HoursBinary 获取闹钟2小时（二进制）
 */
#define DL_RTC_A_getAlarm2HoursBinary               DL_RTC_Common_getAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfWeekBinary 使能闹钟2星期匹配（二进制）
 */
#define DL_RTC_A_enableAlarm2DayOfWeekBinary        DL_RTC_Common_enableAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfWeekBinary 关闭闹钟2星期匹配（二进制）
 */
#define DL_RTC_A_disableAlarm2DayOfWeekBinary       DL_RTC_Common_disableAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfWeekBinary 设置闹钟2星期（二进制）
 */
#define DL_RTC_A_setAlarm2DayOfWeekBinary           DL_RTC_Common_setAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfWeekBinary 获取闹钟2星期（二进制）
 */
#define DL_RTC_A_getAlarm2DayOfWeekBinary           DL_RTC_Common_getAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfMonthBinary 使能闹钟2日期匹配（二进制）
 */
#define DL_RTC_A_enableAlarm2DayOfMonthBinary       DL_RTC_Common_enableAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfMonthBinary 关闭闹钟2日期匹配（二进制）
 */
#define DL_RTC_A_disableAlarm2DayOfMonthBinary      DL_RTC_Common_disableAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfMonthBinary 设置闹钟2日期（二进制）
 */
#define DL_RTC_A_setAlarm2DayOfMonthBinary          DL_RTC_Common_setAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfMonthBinary 获取闹钟2日期（二进制）
 */
#define DL_RTC_A_getAlarm2DayOfMonthBinary          DL_RTC_Common_getAlarm2DayOfMonthBinary

/* --- 预分频器与周期性闹钟 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPrescalerEvents 设置预分频器事件
 */
#define DL_RTC_A_setPrescalerEvents                 DL_RTC_Common_setPrescalerEvents

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPeriodicAlarm0 设置周期性闹钟0
 */
#define DL_RTC_A_setPeriodicAlarm0                  DL_RTC_Common_setPeriodicAlarm0

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPeriodicAlarm1 设置周期性闹钟1
 */
#define DL_RTC_A_setPeriodicAlarm1                  DL_RTC_Common_setPeriodicAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPrescaler0 获取预分频器0当前值
 */
#define DL_RTC_A_getPrescaler0                      DL_RTC_Common_getPrescaler0

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPrescaler1 获取预分频器1当前值
 */
#define DL_RTC_A_getPrescaler1                      DL_RTC_Common_getPrescaler1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampSecondsBCD 获取时间戳秒（BCD）
 */
#define DL_RTC_A_getTimeStampSecondsBCD             DL_RTC_Common_getTimeStampSecondsBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMinutesBCD 获取时间戳分（BCD）
 */
#define DL_RTC_A_getTimeStampMinutesBCD             DL_RTC_Common_getTimeStampMinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampHoursBCD 获取时间戳时（BCD）
 */
#define DL_RTC_A_getTimeStampHoursBCD               DL_RTC_Common_getTimeStampHoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfWeekBCD 获取时间戳星期（BCD）
 */
#define DL_RTC_A_getTimeStampDayOfWeekBCD           DL_RTC_Common_getTimeStampDayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfMonthBCD 获取时间戳日（BCD）
 */
#define DL_RTC_A_getTimeStampDayOfMonthBCD          DL_RTC_Common_getTimeStampDayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMonthBCD 获取时间戳月（BCD）
 */
#define DL_RTC_A_getTimeStampMonthBCD               DL_RTC_Common_getTimeStampMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampYearBCD 获取时间戳年（BCD）
 */
#define DL_RTC_A_getTimeStampYearBCD                DL_RTC_Common_getTimeStampYearBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampSecondsBinary 获取时间戳秒（二进制）
 */
#define DL_RTC_A_getTimeStampSecondsBinary          DL_RTC_Common_getTimeStampSecondsBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMinutesBinary 获取时间戳分（二进制）
 */
#define DL_RTC_A_getTimeStampMinutesBinary          DL_RTC_Common_getTimeStampMinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampHoursBinary 获取时间戳时（二进制）
 */
#define DL_RTC_A_getTimeStampHoursBinary            DL_RTC_Common_getTimeStampHoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfWeekBinary 获取时间戳星期（二进制）
 */
#define DL_RTC_A_getTimeStampDayOfWeekBinary        DL_RTC_Common_getTimeStampDayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfMonthBinary 获取时间戳日（二进制）
 */
#define DL_RTC_A_getTimeStampDayOfMonthBinary       DL_RTC_Common_getTimeStampDayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMonthBinary 获取时间戳月（二进制）
 */
#define DL_RTC_A_getTimeStampMonthBinary            DL_RTC_Common_getTimeStampMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampYearBinary 获取时间戳年（二进制）
 */
#define DL_RTC_A_getTimeStampYearBinary             DL_RTC_Common_getTimeStampYearBinary

/* --- 中断控制 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableInterrupt 使能RTC中断
 */
#define DL_RTC_A_enableInterrupt                    DL_RTC_Common_enableInterrupt

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableInterrupt 关闭RTC中断
 */
#define DL_RTC_A_disableInterrupt                   DL_RTC_Common_disableInterrupt

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledInterrupts 获取已使能的中断掩码
 */
#define DL_RTC_A_getEnabledInterrupts               DL_RTC_Common_getEnabledInterrupts

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledInterruptStatus 获取已使能中断的状态
 */
#define DL_RTC_A_getEnabledInterruptStatus          DL_RTC_Common_getEnabledInterruptStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getRawInterruptStatus 获取原始中断状态
 */
#define DL_RTC_A_getRawInterruptStatus              DL_RTC_Common_getRawInterruptStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPendingInterrupt 获取待处理中断索引
 */
#define DL_RTC_A_getPendingInterrupt                DL_RTC_Common_getPendingInterrupt

/*!
 * @brief Redirects to common @ref DL_RTC_Common_clearInterruptStatus 清除中断状态
 */
#define DL_RTC_A_clearInterruptStatus               DL_RTC_Common_clearInterruptStatus

/* --- 事件控制与发布通道 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableEvent 使能RTC事件
 */
#define DL_RTC_A_enableEvent                        DL_RTC_Common_enableEvent

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableEvent 关闭RTC事件
 */
#define DL_RTC_A_disableEvent                       DL_RTC_Common_disableEvent

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledEvents 获取已使能的事件掩码
 */
#define DL_RTC_A_getEnabledEvents                   DL_RTC_Common_getEnabledEvents

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledEventStatus 获取已使能事件的状态
 */
#define DL_RTC_A_getEnabledEventStatus              DL_RTC_Common_getEnabledEventStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getRawEventsStatus 获取原始事件状态
 */
#define DL_RTC_A_getRawEventsStatus                 DL_RTC_Common_getRawEventsStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_clearEventsStatus 清除事件状态
 */
#define DL_RTC_A_clearEventsStatus                  DL_RTC_Common_clearEventsStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPublisherChanID 设置事件发布通道ID
 */
#define DL_RTC_A_setPublisherChanID                 DL_RTC_Common_setPublisherChanID

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPublisherChanID 获取事件发布通道ID
 */
#define DL_RTC_A_getPublisherChanID                 DL_RTC_Common_getPublisherChanID

/* --- 日历初始化、读取与闹钟配置 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_initCalendar 初始化日历
 */
#define DL_RTC_A_initCalendar                       DL_RTC_Common_initCalendar

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarTime 获取当前日历时间
 */
#define DL_RTC_A_getCalendarTime                    DL_RTC_Common_getCalendarTime

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarAlarm1 设置日历闹钟1
 */
#define DL_RTC_A_setCalendarAlarm1                  DL_RTC_Common_setCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarAlarm1 获取日历闹钟1配置
 */
#define DL_RTC_A_getCalendarAlarm1                  DL_RTC_Common_getCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableCalendarAlarm1 使能日历闹钟1
 */
#define DL_RTC_A_enableCalendarAlarm1               DL_RTC_Common_enableCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableCalendarAlarm1 关闭日历闹钟1
 */
#define DL_RTC_A_disableCalendarAlarm1              DL_RTC_Common_disableCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarAlarm2 设置日历闹钟2
 */
#define DL_RTC_A_setCalendarAlarm2                  DL_RTC_Common_setCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarAlarm2 获取日历闹钟2配置
 */
#define DL_RTC_A_getCalendarAlarm2                  DL_RTC_Common_getCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableCalendarAlarm2 使能日历闹钟2
 */
#define DL_RTC_A_enableCalendarAlarm2               DL_RTC_Common_enableCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableCalendarAlarm2 关闭日历闹钟2
 */
#define DL_RTC_A_disableCalendarAlarm2              DL_RTC_Common_disableCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setTimeStampEventSource 设置时间戳事件源
 */
#define DL_RTC_A_setTimeStampEventSource            DL_RTC_Common_setTimeStampEventSource

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampEventSource 获取时间戳事件源配置
 */
#define DL_RTC_A_getTimeStampEventSource            DL_RTC_Common_getTimeStampEventSource

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampEventCause 获取时间戳事件触发原因
 */
#define DL_RTC_A_getTimeStampEventCause             DL_RTC_Common_getTimeStampEventCause

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setTimeStampEventCapture 设置时间戳捕获模式（首次/末次）
 */
#define DL_RTC_A_setTimeStampEventCapture           DL_RTC_Common_setTimeStampEventCapture

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampEventCapture 获取时间戳捕获模式
 */
#define DL_RTC_A_getTimeStampEventCapture           DL_RTC_Common_getTimeStampEventCapture

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampCalendar 获取时间戳日历完整数据
 */
#define DL_RTC_A_getTimeStampCalendar               DL_RTC_Common_getTimeStampCalendar

/*!
 * @brief Redirects to common @ref DL_RTC_Common_clearTimeStamp 清除时间戳数据
 */
#define DL_RTC_A_clearTimeStamp                     DL_RTC_Common_clearTimeStamp

/* --- 写保护控制 --- */

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableWriteProtect 使能写保护
 */
#define DL_RTC_A_enableWriteProtect                 DL_RTC_Common_enableWriteProtect

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isWriteProtectEnabled 查询写保护是否使能
 */
#define DL_RTC_A_isWriteProtectEnabled              DL_RTC_Common_isWriteProtectEnabled

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableWriteProtect 关闭写保护
 */
#define DL_RTC_A_disableWriteProtect                DL_RTC_Common_disableWriteProtect

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_RTC_A__ */

#endif /* ti_dl_dl_rtc_a__include */
/** @}*/
