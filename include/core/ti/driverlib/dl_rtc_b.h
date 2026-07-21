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
 *  @file       dl_rtc_b.h
 *  @brief      Real Time Clock A (RTC_a) Peripheral Interface / RTC_B 外设接口头文件
 *  @defgroup   RTC_B Real Time Clock A (RTC_B)
 *
 *  @anchor ti_devices_msp_dl_rtc_b_Overview
 *  # Overview
 *  The RTC_B Driver Library allows full configuration of the MSPM0 RTC_B module.
 *  The real-time clock (RTC_B) module provides clock counters with calendar mode,
 *  a flexible programmable alarm, offset calibration, and a provision for
 *  temperature compensation. The module also provides a separate time stamp calendar
 *  capable of capturing a time stamp event upon loss of VDD or detection of a tamper event
 *  on a tamper I/O pin.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup RTC_B
 * @{
 */
#ifndef ti_dl_dl_rtc_b__include
#define ti_dl_dl_rtc_b__include

#include <ti/driverlib/dl_rtc_common.h>

#ifdef __MSPM0_HAS_RTC_B__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_RTC_B_INTERRUPT
 *  @{
 */

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM1 / 日历闹钟1中断掩码
 */
#define DL_RTC_B_INTERRUPT_CALENDAR_ALARM1          (DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM2 / 日历闹钟2中断掩码
 */
#define DL_RTC_B_INTERRUPT_CALENDAR_ALARM2          (DL_RTC_COMMON_INTERRUPT_CALENDAR_ALARM2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_PRESCALER0 / 预分频器0中断掩码
 */
#define DL_RTC_B_INTERRUPT_PRESCALER0               (DL_RTC_COMMON_INTERRUPT_PRESCALER0)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_PRESCALER1 / 预分频器1中断掩码
 */
#define DL_RTC_B_INTERRUPT_PRESCALER1               (DL_RTC_COMMON_INTERRUPT_PRESCALER1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_PRESCALER2 / 预分频器2中断掩码
 */
#define DL_RTC_B_INTERRUPT_PRESCALER2               (DL_RTC_COMMON_INTERRUPT_PRESCALER2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_INTERVAL_ALARM / 间隔闹钟中断掩码
 */
#define DL_RTC_B_INTERRUPT_INTERVAL_ALARM           (DL_RTC_COMMON_INTERRUPT_INTERVAL_ALARM)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERRUPT_READY / 就绪中断掩码
 */
#define DL_RTC_B_INTERRUPT_READY                    (DL_RTC_COMMON_INTERRUPT_READY)

/** @}*/

/** @addtogroup DL_RTC_B_EVENT
 *  @{
 */

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_CALENDAR_ALARM1 / 日历闹钟1事件掩码
 */
#define DL_RTC_B_EVENT_CALENDAR_ALARM1          (DL_RTC_COMMON_EVENT_CALENDAR_ALARM1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_CALENDAR_ALARM2 / 日历闹钟2事件掩码
 */
#define DL_RTC_B_EVENT_CALENDAR_ALARM2          (DL_RTC_COMMON_EVENT_CALENDAR_ALARM2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_PRESCALER0 / 预分频器0事件掩码
 */
#define DL_RTC_B_EVENT_PRESCALER0               (DL_RTC_COMMON_EVENT_PRESCALER0)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_PRESCALER1 / 预分频器1事件掩码
 */
#define DL_RTC_B_EVENT_PRESCALER1               (DL_RTC_COMMON_EVENT_PRESCALER1)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_PRESCALER2 / 预分频器2事件掩码
 */
#define DL_RTC_B_EVENT_PRESCALER2               (DL_RTC_COMMON_EVENT_PRESCALER2)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_INTERVAL_ALARM / 间隔闹钟事件掩码
 */
#define DL_RTC_B_EVENT_INTERVAL_ALARM           (DL_RTC_COMMON_EVENT_INTERVAL_ALARM)

/**
 * @brief Redirects to common @ref DL_RTC_COMMON_EVENT_READY / 就绪事件掩码
 */
#define DL_RTC_B_EVENT_READY                    (DL_RTC_COMMON_EVENT_READY)

/** @}*/

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_FORMAT_BINARY / 二进制格式
 */
#define DL_RTC_B_FORMAT_BINARY                      DL_RTC_COMMON_FORMAT_BINARY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_FORMAT_BCD / BCD格式
 */
#define DL_RTC_B_FORMAT_BCD                         DL_RTC_COMMON_FORMAT_BCD

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_STATUS_READY / RTC就绪，可安全读取
 */
#define DL_RTC_B_STATUS_READY                       DL_RTC_COMMON_STATUS_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_STATUS_NOT_READY / RTC未就绪
 */
#define DL_RTC_B_STATUS_NOT_READY                   DL_RTC_COMMON_STATUS_NOT_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_STATUS_READY / 补偿就绪
 */
#define DL_RTC_B_COMPENSATION_STATUS_READY          DL_RTC_COMMON_COMPENSATION_STATUS_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_STATUS_NOT_READY / 补偿未就绪
 */
#define DL_RTC_B_COMPENSATION_STATUS_NOT_READY      DL_RTC_COMMON_COMPENSATION_STATUS_NOT_READY

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_OK / 补偿写入成功
 */
#define DL_RTC_B_COMPENSATION_WRITE_RESULT_OK       DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_OK

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_NOT_OK / 补偿写入失败
 */
#define DL_RTC_B_COMPENSATION_WRITE_RESULT_NOT_OK   DL_RTC_COMMON_COMPENSATION_WRITE_RESULT_NOT_OK

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_DOWN / 校准方向：频率下调
 */
#define DL_RTC_B_OFFSET_CALIBRATION_SIGN_DOWN       DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_DOWN

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_UP / 校准方向：频率上调
 */
#define DL_RTC_B_OFFSET_CALIBRATION_SIGN_UP         DL_RTC_COMMON_OFFSET_CALIBRATION_SIGN_UP

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_512 / 校准输出512Hz
 */
#define DL_RTC_B_OFFSET_CALIBRATION_FREQUENCY_512              DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_512

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_256 / 校准输出256Hz
 */
#define DL_RTC_B_OFFSET_CALIBRATION_FREQUENCY_256              DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_256

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_1 / 校准输出1Hz
 */
#define DL_RTC_B_OFFSET_CALIBRATION_FREQUENCY_1              DL_RTC_COMMON_OFFSET_CALIBRATION_FREQUENCY_1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_TEMP_CALIBRATION_DOWN1PPM / 温度补偿：-1ppm步进
 */
#define DL_RTC_B_TEMP_CALIBRATION_DOWN1PPM                  DL_RTC_COMMON_TEMP_CALIBRATION_DOWN1PPM

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_TEMP_CALIBRATION_UP1PPM / 温度补偿：+1ppm步进
 */
#define DL_RTC_B_TEMP_CALIBRATION_UP1PPM                    DL_RTC_COMMON_TEMP_CALIBRATION_UP1PPM

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_MINUTECHANGE / 间隔闹钟：每分钟触发
 */
#define DL_RTC_B_INTERVAL_ALARM_MINUTECHANGE                DL_RTC_COMMON_INTERVAL_ALARM_MINUTECHANGE

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_HOURCHANGE / 间隔闹钟：每小时触发
 */
#define DL_RTC_B_INTERVAL_ALARM_HOURCHANGE                  DL_RTC_COMMON_INTERVAL_ALARM_HOURCHANGE

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_NOON / 间隔闹钟：中午12点触发
 */
#define DL_RTC_B_INTERVAL_ALARM_NOON                        DL_RTC_COMMON_INTERVAL_ALARM_NOON

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_INTERVAL_ALARM_MIDNIGHT / 间隔闹钟：午夜0点触发
 */
#define DL_RTC_B_INTERVAL_ALARM_MIDNIGHT                    DL_RTC_COMMON_INTERVAL_ALARM_MIDNIGHT

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_8 / 预分频器0：8分频
 */
#define DL_RTC_B_PRESCALER0_DIVIDE_8                        DL_RTC_COMMON_PRESCALER0_DIVIDE_8

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_16 / 预分频器0：16分频
 */
#define DL_RTC_B_PRESCALER0_DIVIDE_16                       DL_RTC_COMMON_PRESCaLER0_DIVIDE_16

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_32 / 预分频器0：32分频
 */
#define DL_RTC_B_PRESCALER0_DIVIDE_32                       DL_RTC_COMMON_PRESCALER0_DIVIDE_32

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_64 / 预分频器0：64分频
 */
#define DL_RTC_B_PRESCALER0_DIVIDE_64                       DL_RTC_COMMON_PRESCALER0_DIVIDE_64

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_128 / 预分频器0：128分频
 */
#define DL_RTC_B_PRESCALER0_DIVIDE_128                      DL_RTC_COMMON_PRESCALER0_DIVIDE_128

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER0_DIVIDE_256 / 预分频器0：256分频
 */
#define DL_RTC_B_PRESCALER0_DIVIDE_256                      DL_RTC_COMMON_PRESCALER0_DIVIDE_256

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_2 / 预分频器1：2分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_2                        DL_RTC_COMMON_PRESCALER1_DIVIDE_2

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_4 / 预分频器1：4分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_4                        DL_RTC_COMMON_PRESCALER1_DIVIDE_4

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_8 / 预分频器1：8分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_8                        DL_RTC_COMMON_PRESCALER1_DIVIDE_8

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_16 / 预分频器1：16分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_16                       DL_RTC_COMMON_PRESCALER1_DIVIDE_16

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_32 / 预分频器1：32分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_32                       DL_RTC_COMMON_PRESCALER1_DIVIDE_32

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_64 / 预分频器1：64分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_64                       DL_RTC_COMMON_PRESCALER1_DIVIDE_64

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_128 / 预分频器1：128分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_128                      DL_RTC_COMMON_PRESCALER1_DIVIDE_128

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER1_DIVIDE_256 / 预分频器1：256分频
 */
#define DL_RTC_B_PRESCALER1_DIVIDE_256                      DL_RTC_COMMON_PRESCALER1_DIVIDE_256

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER2_4_SEC / 预分频器2：每4秒间隔
 */
#define DL_RTC_B_PRESCALER2_4_SEC                           DL_RTC_COMMON_PRESCALER2_4_SEC

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER2_8_SEC / 预分频器2：每8秒间隔
 */
#define DL_RTC_B_PRESCALER2_8_SEC                           DL_RTC_COMMON_PRESCALER2_8_SEC

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_PRESCALER2_16_SEC / 预分频器2：每16秒间隔
 */
#define DL_RTC_B_PRESCALER2_16_SEC                          DL_RTC_COMMON_PRESCALER2_16_SEC

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_ALARM1 / 中断索引：闹钟1
 */
#define DL_RTC_B_IIDX_ALARM1                                DL_RTC_COMMON_IIDX_ALARM1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_ALARM2 / 中断索引：闹钟2
 */
#define DL_RTC_B_IIDX_ALARM2                                DL_RTC_COMMON_IIDX_ALARM2

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_PRESCALER0 / 中断索引：预分频器0
 */
#define DL_RTC_B_IIDX_PRESCALER0                            DL_RTC_COMMON_IIDX_PRESCALER0

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_PRESCALER1 / 中断索引：预分频器1
 */
#define DL_RTC_B_IIDX_PRESCALER1                            DL_RTC_COMMON_IIDX_PRESCALER1

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_PRESCALER2 / 中断索引：预分频器2
 */
#define DL_RTC_B_IIDX_PRESCALER2                            DL_RTC_COMMON_IIDX_PRESCALER2

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_INTERVAL_TIMER / 中断索引：间隔定时器
 */
#define DL_RTC_B_IIDX_INTERVAL_TIMER                        DL_RTC_COMMON_IIDX_INTERVAL_TIMER

/*!
 * @brief Redirects to common @ref DL_RTC_COMMON_IIDX_READY / 中断索引：就绪
 */
#define DL_RTC_B_IIDX_READY                                 DL_RTC_COMMON_IIDX_READY

/**
 * @brief Redirects to common @ref DL_RTC_Common_Calendar / 日历时间结构体
 *
 */
typedef DL_RTC_Common_Calendar                      DL_RTC_B_Calendar;

/**
 * @brief Redirects to common @ref DL_RTC_Common_CalendarAlarm / 日历闹钟结构体
 *
 */
typedef DL_RTC_Common_CalendarAlarm                 DL_RTC_B_CalendarAlarm;


/*!
 * @brief Redirects to common @ref DL_RTC_Common_isClockSourceLFCLK / 检查时钟源是否为LFCLK
 */
#define DL_RTC_B_isClockSourceLFCLK                 DL_RTC_Common_isClockSourceLFCLK

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableClockControl / 使能32kHz时钟供给
 */
#define DL_RTC_B_enableClockControl                 DL_RTC_Common_enableClockControl

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableClockControl / 禁用32kHz时钟供给
 */
#define DL_RTC_B_disableClockControl                DL_RTC_Common_disableClockControl

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableDebugInterrupts / 使能调试模式中断
 */
#define DL_RTC_B_enableDebugInterrupts              DL_RTC_Common_enableDebugInterrupts

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableDebugInterrupts / 禁用调试模式中断
 */
#define DL_RTC_B_disableDebugInterrupts             DL_RTC_Common_disableDebugInterrupts

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableDebugMode / 使能调试模式
 */
#define DL_RTC_B_enableDebugMode                    DL_RTC_Common_enableDebugMode

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableDebugMode / 禁用调试模式
 */
#define DL_RTC_B_disableDebugMode                   DL_RTC_Common_disableDebugMode

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setClockFormat / 设置时钟格式（二进制/BCD）
 */
#define DL_RTC_B_setClockFormat                     DL_RTC_Common_setClockFormat

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getClockFormat / 获取当前时钟格式
 */
#define DL_RTC_B_getClockFormat                     DL_RTC_Common_getClockFormat

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setIntervalAlarm / 设置间隔闹钟条件
 */
#define DL_RTC_B_setIntervalAlarm                   DL_RTC_Common_setIntervalAlarm

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getIntervalAlarm / 获取间隔闹钟条件
 */
#define DL_RTC_B_getIntervalAlarm                   DL_RTC_Common_getIntervalAlarm

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isSafeToRead / 检查RTC是否可安全读取
 */
#define DL_RTC_B_isSafeToRead                       DL_RTC_Common_isSafeToRead

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isReadyToCalibrate / 检查是否可校准
 */
#define DL_RTC_B_isReadyToCalibrate                 DL_RTC_Common_isReadyToCalibrate

/*!
 * @brief Redirects to common @ref DL_RTC_Common_isCalibrationWriteResultOK / 检查校准写入是否成功
 */
#define DL_RTC_B_isCalibrationWriteResultOK         DL_RTC_Common_isCalibrationWriteResultOK

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setOffsetCalibrationFrequency / 设置偏移校准输出频率
 */
#define DL_RTC_B_setOffsetCalibrationFrequency      DL_RTC_Common_setOffsetCalibrationFrequency

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setOffsetCalibrationAdjValue / 设置偏移校准调整值
 */
#define DL_RTC_B_setOffsetCalibrationAdjValue       DL_RTC_Common_setOffsetCalibrationAdjValue

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setOffsetCalibration / 设置偏移校准参数
 */
#define DL_RTC_B_setOffsetCalibration               DL_RTC_Common_setOffsetCalibration

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getOffsetCalibrationFrequency / 获取偏移校准频率
 */
#define DL_RTC_B_getOffsetCalibrationFrequency      DL_RTC_Common_getOffsetCalibrationFrequency

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getOffsetCalibrationSign / 获取偏移校准方向
 */
#define DL_RTC_B_getOffsetCalibrationSign           DL_RTC_Common_getOffsetCalibrationSign

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setTemperatureCompensation / 设置温度补偿
 */
#define DL_RTC_B_setTemperatureCompensation         DL_RTC_Common_setTemperatureCompensation

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTemperatureCompensation / 获取温度补偿值
 */
#define DL_RTC_B_getTemperatureCompensation         DL_RTC_Common_getTemperatureCompensation

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarSecondsBCD / 设置日历秒（BCD）
 */
#define DL_RTC_B_setCalendarSecondsBCD              DL_RTC_Common_setCalendarSecondsBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarSecondsBCD / 获取日历秒（BCD）
 */
#define DL_RTC_B_getCalendarSecondsBCD              DL_RTC_Common_getCalendarSecondsBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMinutesBCD / 设置日历分（BCD）
 */
#define DL_RTC_B_setCalendarMinutesBCD              DL_RTC_Common_setCalendarMinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMinutesBCD / 获取日历分（BCD）
 */
#define DL_RTC_B_getCalendarMinutesBCD              DL_RTC_Common_getCalendarMinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarHoursBCD / 设置日历时（BCD）
 */
#define DL_RTC_B_setCalendarHoursBCD                DL_RTC_Common_setCalendarHoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarHoursBCD / 获取日历时（BCD）
 */
#define DL_RTC_B_getCalendarHoursBCD                DL_RTC_Common_getCalendarHoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfWeekBCD / 设置日历星期（BCD）
 */
#define DL_RTC_B_setCalendarDayOfWeekBCD            DL_RTC_Common_setCalendarDayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfWeekBCD / 获取日历星期（BCD）
 */
#define DL_RTC_B_getCalendarDayOfWeekBCD            DL_RTC_Common_getCalendarDayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfMonthBCD / 设置日历日（BCD）
 */
#define DL_RTC_B_setCalendarDayOfMonthBCD           DL_RTC_Common_setCalendarDayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfMonthBCD / 获取日历日（BCD）
 */
#define DL_RTC_B_getCalendarDayOfMonthBCD           DL_RTC_Common_getCalendarDayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMonthBCD / 设置日历月（BCD）
 */
#define DL_RTC_B_setCalendarMonthBCD                DL_RTC_Common_setCalendarMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMonthBCD / 获取日历月（BCD）
 */
#define DL_RTC_B_getCalendarMonthBCD                DL_RTC_Common_getCalendarMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarYearBCD / 设置日历年（BCD）
 */
#define DL_RTC_B_setCalendarYearBCD                 DL_RTC_Common_setCalendarYearBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarYearBCD / 获取日历年（BCD）
 */
#define DL_RTC_B_getCalendarYearBCD                 DL_RTC_Common_getCalendarYearBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1MinutesBCD / 使能闹钟1分匹配（BCD）
 */
#define DL_RTC_B_enableAlarm1MinutesBCD             DL_RTC_Common_enableAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1MinutesBCD / 禁用闹钟1分匹配（BCD）
 */
#define DL_RTC_B_disableAlarm1MinutesBCD            DL_RTC_Common_disableAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1MinutesBCD / 设置闹钟1分（BCD）
 */
#define DL_RTC_B_setAlarm1MinutesBCD                DL_RTC_Common_setAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1MinutesBCD / 获取闹钟1分（BCD）
 */
#define DL_RTC_B_getAlarm1MinutesBCD                DL_RTC_Common_getAlarm1MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1HoursBCD / 使能闹钟1时匹配（BCD）
 */
#define DL_RTC_B_enableAlarm1HoursBCD               DL_RTC_Common_enableAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1HoursBCD / 禁用闹钟1时匹配（BCD）
 */
#define DL_RTC_B_disableAlarm1HoursBCD              DL_RTC_Common_disableAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1HoursBCD / 设置闹钟1时（BCD）
 */
#define DL_RTC_B_setAlarm1HoursBCD                  DL_RTC_Common_setAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1HoursBCD / 获取闹钟1时（BCD）
 */
#define DL_RTC_B_getAlarm1HoursBCD                  DL_RTC_Common_getAlarm1HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfWeekBCD / 使能闹钟1星期匹配（BCD）
 */
#define DL_RTC_B_enableAlarm1DayOfWeekBCD           DL_RTC_Common_enableAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfWeekBCD / 禁用闹钟1星期匹配（BCD）
 */
#define DL_RTC_B_disableAlarm1DayOfWeekBCD          DL_RTC_Common_disableAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfWeekBCD / 设置闹钟1星期（BCD）
 */
#define DL_RTC_B_setAlarm1DayOfWeekBCD              DL_RTC_Common_setAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfWeekBCD / 获取闹钟1星期（BCD）
 */
#define DL_RTC_B_getAlarm1DayOfWeekBCD              DL_RTC_Common_getAlarm1DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfMonthBCD / 使能闹钟1日匹配（BCD）
 */
#define DL_RTC_B_enableAlarm1DayOfMonthBCD          DL_RTC_Common_enableAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfMonthBCD / 禁用闹钟1日匹配（BCD）
 */
#define DL_RTC_B_disableAlarm1DayOfMonthBCD         DL_RTC_Common_disableAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfMonthBCD / 设置闹钟1日（BCD）
 */
#define DL_RTC_B_setAlarm1DayOfMonthBCD             DL_RTC_Common_setAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfMonthBCD / 获取闹钟1日（BCD）
 */
#define DL_RTC_B_getAlarm1DayOfMonthBCD             DL_RTC_Common_getAlarm1DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2MinutesBCD / 使能闹钟2分匹配（BCD）
 */
#define DL_RTC_B_enableAlarm2MinutesBCD             DL_RTC_Common_enableAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2MinutesBCD / 禁用闹钟2分匹配（BCD）
 */
#define DL_RTC_B_disableAlarm2MinutesBCD            DL_RTC_Common_disableAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2MinutesBCD / 设置闹钟2分（BCD）
 */
#define DL_RTC_B_setAlarm2MinutesBCD                DL_RTC_Common_setAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2MinutesBCD / 获取闹钟2分（BCD）
 */
#define DL_RTC_B_getAlarm2MinutesBCD                DL_RTC_Common_getAlarm2MinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2HoursBCD / 使能闹钟2时匹配（BCD）
 */
#define DL_RTC_B_enableAlarm2HoursBCD               DL_RTC_Common_enableAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2HoursBCD / 禁用闹钟2时匹配（BCD）
 */
#define DL_RTC_B_disableAlarm2HoursBCD              DL_RTC_Common_disableAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2HoursBCD / 设置闹钟2时（BCD）
 */
#define DL_RTC_B_setAlarm2HoursBCD                  DL_RTC_Common_setAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2HoursBCD / 获取闹钟2时（BCD）
 */
#define DL_RTC_B_getAlarm2HoursBCD                  DL_RTC_Common_getAlarm2HoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfWeekBCD / 使能闹钟2星期匹配（BCD）
 */
#define DL_RTC_B_enableAlarm2DayOfWeekBCD           DL_RTC_Common_enableAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfWeekBCD / 禁用闹钟2星期匹配（BCD）
 */
#define DL_RTC_B_disableAlarm2DayOfWeekBCD          DL_RTC_Common_disableAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfWeekBCD / 设置闹钟2星期（BCD）
 */
#define DL_RTC_B_setAlarm2DayOfWeekBCD              DL_RTC_Common_setAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfWeekBCD / 获取闹钟2星期（BCD）
 */
#define DL_RTC_B_getAlarm2DayOfWeekBCD              DL_RTC_Common_getAlarm2DayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfMonthBCD / 使能闹钟2日匹配（BCD）
 */
#define DL_RTC_B_enableAlarm2DayOfMonthBCD          DL_RTC_Common_enableAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfMonthBCD / 禁用闹钟2日匹配（BCD）
 */
#define DL_RTC_B_disableAlarm2DayOfMonthBCD         DL_RTC_Common_disableAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfMonthBCD / 设置闹钟2日（BCD）
 */
#define DL_RTC_B_setAlarm2DayOfMonthBCD             DL_RTC_Common_setAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfMonthBCD / 获取闹钟2日（BCD）
 */
#define DL_RTC_B_getAlarm2DayOfMonthBCD             DL_RTC_Common_getAlarm2DayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarSecondsBinary / 设置日历秒（二进制）
 */
#define DL_RTC_B_setCalendarSecondsBinary           DL_RTC_Common_setCalendarSecondsBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarSecondsBinary / 获取日历秒（二进制）
 */
#define DL_RTC_B_getCalendarSecondsBinary           DL_RTC_Common_getCalendarSecondsBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMinutesBinary / 设置日历分（二进制）
 */
#define DL_RTC_B_setCalendarMinutesBinary           DL_RTC_Common_setCalendarMinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMinutesBinary / 获取日历分（二进制）
 */
#define DL_RTC_B_getCalendarMinutesBinary           DL_RTC_Common_getCalendarMinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarHoursBinary / 设置日历时（二进制）
 */
#define DL_RTC_B_setCalendarHoursBinary             DL_RTC_Common_setCalendarHoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarHoursBinary / 获取日历时（二进制）
 */
#define DL_RTC_B_getCalendarHoursBinary             DL_RTC_Common_getCalendarHoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfWeekBinary / 设置日历星期（二进制）
 */
#define DL_RTC_B_setCalendarDayOfWeekBinary         DL_RTC_Common_setCalendarDayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfWeekBinary / 获取日历星期（二进制）
 */
#define DL_RTC_B_getCalendarDayOfWeekBinary         DL_RTC_Common_getCalendarDayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarDayOfMonthBinary / 设置日历日（二进制）
 */
#define DL_RTC_B_setCalendarDayOfMonthBinary        DL_RTC_Common_setCalendarDayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarDayOfMonthBinary / 获取日历日（二进制）
 */
#define DL_RTC_B_getCalendarDayOfMonthBinary        DL_RTC_Common_getCalendarDayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarMonthBinary / 设置日历月（二进制）
 */
#define DL_RTC_B_setCalendarMonthBinary             DL_RTC_Common_setCalendarMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarMonthBinary / 获取日历月（二进制）
 */
#define DL_RTC_B_getCalendarMonthBinary             DL_RTC_Common_getCalendarMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarYearBinary / 设置日历年（二进制）
 */
#define DL_RTC_B_setCalendarYearBinary              DL_RTC_Common_setCalendarYearBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarYearBinary / 获取日历年（二进制）
 */
#define DL_RTC_B_getCalendarYearBinary              DL_RTC_Common_getCalendarYearBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1MinutesBinary / 使能闹钟1分匹配（二进制）
 */
#define DL_RTC_B_enableAlarm1MinutesBinary          DL_RTC_Common_enableAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1MinutesBinary / 禁用闹钟1分匹配（二进制）
 */
#define DL_RTC_B_disableAlarm1MinutesBinary         DL_RTC_Common_disableAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1MinutesBinary / 设置闹钟1分（二进制）
 */
#define DL_RTC_B_setAlarm1MinutesBinary             DL_RTC_Common_setAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1MinutesBinary / 获取闹钟1分（二进制）
 */
#define DL_RTC_B_getAlarm1MinutesBinary             DL_RTC_Common_getAlarm1MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1HoursBinary / 使能闹钟1时匹配（二进制）
 */
#define DL_RTC_B_enableAlarm1HoursBinary            DL_RTC_Common_enableAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1HoursBinary / 禁用闹钟1时匹配（二进制）
 */
#define DL_RTC_B_disableAlarm1HoursBinary           DL_RTC_Common_disableAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1HoursBinary / 设置闹钟1时（二进制）
 */
#define DL_RTC_B_setAlarm1HoursBinary               DL_RTC_Common_setAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1HoursBinary / 获取闹钟1时（二进制）
 */
#define DL_RTC_B_getAlarm1HoursBinary               DL_RTC_Common_getAlarm1HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfWeekBinary / 使能闹钟1星期匹配（二进制）
 */
#define DL_RTC_B_enableAlarm1DayOfWeekBinary        DL_RTC_Common_enableAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfWeekBinary / 禁用闹钟1星期匹配（二进制）
 */
#define DL_RTC_B_disableAlarm1DayOfWeekBinary       DL_RTC_Common_disableAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfWeekBinary / 设置闹钟1星期（二进制）
 */
#define DL_RTC_B_setAlarm1DayOfWeekBinary           DL_RTC_Common_setAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfWeekBinary / 获取闹钟1星期（二进制）
 */
#define DL_RTC_B_getAlarm1DayOfWeekBinary           DL_RTC_Common_getAlarm1DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm1DayOfMonthBinary / 使能闹钟1日匹配（二进制）
 */
#define DL_RTC_B_enableAlarm1DayOfMonthBinary       DL_RTC_Common_enableAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm1DayOfMonthBinary / 禁用闹钟1日匹配（二进制）
 */
#define DL_RTC_B_disableAlarm1DayOfMonthBinary      DL_RTC_Common_disableAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm1DayOfMonthBinary / 设置闹钟1日（二进制）
 */
#define DL_RTC_B_setAlarm1DayOfMonthBinary          DL_RTC_Common_setAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm1DayOfMonthBinary / 获取闹钟1日（二进制）
 */
#define DL_RTC_B_getAlarm1DayOfMonthBinary          DL_RTC_Common_getAlarm1DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2MinutesBinary / 使能闹钟2分匹配（二进制）
 */
#define DL_RTC_B_enableAlarm2MinutesBinary          DL_RTC_Common_enableAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2MinutesBinary / 禁用闹钟2分匹配（二进制）
 */
#define DL_RTC_B_disableAlarm2MinutesBinary         DL_RTC_Common_disableAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2MinutesBinary / 设置闹钟2分（二进制）
 */
#define DL_RTC_B_setAlarm2MinutesBinary             DL_RTC_Common_setAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2MinutesBinary / 获取闹钟2分（二进制）
 */
#define DL_RTC_B_getAlarm2MinutesBinary             DL_RTC_Common_getAlarm2MinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2HoursBinary / 使能闹钟2时匹配（二进制）
 */
#define DL_RTC_B_enableAlarm2HoursBinary            DL_RTC_Common_enableAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2HoursBinary / 禁用闹钟2时匹配（二进制）
 */
#define DL_RTC_B_disableAlarm2HoursBinary           DL_RTC_Common_disableAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2HoursBinary / 设置闹钟2时（二进制）
 */
#define DL_RTC_B_setAlarm2HoursBinary               DL_RTC_Common_setAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2HoursBinary / 获取闹钟2时（二进制）
 */
#define DL_RTC_B_getAlarm2HoursBinary               DL_RTC_Common_getAlarm2HoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfWeekBinary / 使能闹钟2星期匹配（二进制）
 */
#define DL_RTC_B_enableAlarm2DayOfWeekBinary        DL_RTC_Common_enableAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfWeekBinary / 禁用闹钟2星期匹配（二进制）
 */
#define DL_RTC_B_disableAlarm2DayOfWeekBinary       DL_RTC_Common_disableAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfWeekBinary / 设置闹钟2星期（二进制）
 */
#define DL_RTC_B_setAlarm2DayOfWeekBinary           DL_RTC_Common_setAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfWeekBinary / 获取闹钟2星期（二进制）
 */
#define DL_RTC_B_getAlarm2DayOfWeekBinary           DL_RTC_Common_getAlarm2DayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableAlarm2DayOfMonthBinary / 使能闹钟2日匹配（二进制）
 */
#define DL_RTC_B_enableAlarm2DayOfMonthBinary       DL_RTC_Common_enableAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableAlarm2DayOfMonthBinary / 禁用闹钟2日匹配（二进制）
 */
#define DL_RTC_B_disableAlarm2DayOfMonthBinary      DL_RTC_Common_disableAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setAlarm2DayOfMonthBinary / 设置闹钟2日（二进制）
 */
#define DL_RTC_B_setAlarm2DayOfMonthBinary          DL_RTC_Common_setAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getAlarm2DayOfMonthBinary / 获取闹钟2日（二进制）
 */
#define DL_RTC_B_getAlarm2DayOfMonthBinary          DL_RTC_Common_getAlarm2DayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPrescalerEvents / 设置预分频器事件
 */
#define DL_RTC_B_setPrescalerEvents                 DL_RTC_Common_setPrescalerEvents

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPeriodicAlarm0 / 设置周期闹钟0
 */
#define DL_RTC_B_setPeriodicAlarm0                  DL_RTC_Common_setPeriodicAlarm0

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPeriodicAlarm1 / 设置周期闹钟1
 */
#define DL_RTC_B_setPeriodicAlarm1                  DL_RTC_Common_setPeriodicAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPeriodicAlarm2 / 设置周期闹钟2
 */
#define DL_RTC_B_setPeriodicAlarm2                  DL_RTC_Common_setPeriodicAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPrescaler0 / 获取预分频器0值
 */
#define DL_RTC_B_getPrescaler0                      DL_RTC_Common_getPrescaler0

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPrescaler1 / 获取预分频器1值
 */
#define DL_RTC_B_getPrescaler1                      DL_RTC_Common_getPrescaler1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPrescaler2 / 获取预分频器2值
 */
#define DL_RTC_B_getPrescaler2                      DL_RTC_Common_getPrescaler2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampSecondsBCD / 获取时间戳秒（BCD）
 */
#define DL_RTC_B_getTimeStampSecondsBCD             DL_RTC_Common_getTimeStampSecondsBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMinutesBCD / 获取时间戳分（BCD）
 */
#define DL_RTC_B_getTimeStampMinutesBCD             DL_RTC_Common_getTimeStampMinutesBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampHoursBCD / 获取时间戳时（BCD）
 */
#define DL_RTC_B_getTimeStampHoursBCD               DL_RTC_Common_getTimeStampHoursBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfWeekBCD / 获取时间戳星期（BCD）
 */
#define DL_RTC_B_getTimeStampDayOfWeekBCD           DL_RTC_Common_getTimeStampDayOfWeekBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfMonthBCD / 获取时间戳日（BCD）
 */
#define DL_RTC_B_getTimeStampDayOfMonthBCD          DL_RTC_Common_getTimeStampDayOfMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMonthBCD / 获取时间戳月（BCD）
 */
#define DL_RTC_B_getTimeStampMonthBCD               DL_RTC_Common_getTimeStampMonthBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampYearBCD / 获取时间戳年（BCD）
 */
#define DL_RTC_B_getTimeStampYearBCD                DL_RTC_Common_getTimeStampYearBCD

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampSecondsBinary / 获取时间戳秒（二进制）
 */
#define DL_RTC_B_getTimeStampSecondsBinary          DL_RTC_Common_getTimeStampSecondsBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMinutesBinary / 获取时间戳分（二进制）
 */
#define DL_RTC_B_getTimeStampMinutesBinary          DL_RTC_Common_getTimeStampMinutesBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampHoursBinary / 获取时间戳时（二进制）
 */
#define DL_RTC_B_getTimeStampHoursBinary            DL_RTC_Common_getTimeStampHoursBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfWeekBinary / 获取时间戳星期（二进制）
 */
#define DL_RTC_B_getTimeStampDayOfWeekBinary        DL_RTC_Common_getTimeStampDayOfWeekBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampDayOfMonthBinary / 获取时间戳日（二进制）
 */
#define DL_RTC_B_getTimeStampDayOfMonthBinary       DL_RTC_Common_getTimeStampDayOfMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampMonthBinary / 获取时间戳月（二进制）
 */
#define DL_RTC_B_getTimeStampMonthBinary            DL_RTC_Common_getTimeStampMonthBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getTimeStampYearBinary / 获取时间戳年（二进制）
 */
#define DL_RTC_B_getTimeStampYearBinary             DL_RTC_Common_getTimeStampYearBinary

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableInterrupt / 使能RTC中断
 */
#define DL_RTC_B_enableInterrupt                    DL_RTC_Common_enableInterrupt

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableInterrupt / 禁用RTC中断
 */
#define DL_RTC_B_disableInterrupt                   DL_RTC_Common_disableInterrupt

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledInterrupts / 获取已使能的中断
 */
#define DL_RTC_B_getEnabledInterrupts               DL_RTC_Common_getEnabledInterrupts

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledInterruptStatus / 获取已使能中断的状态
 */
#define DL_RTC_B_getEnabledInterruptStatus          DL_RTC_Common_getEnabledInterruptStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getRawInterruptStatus / 获取原始中断状态
 */
#define DL_RTC_B_getRawInterruptStatus              DL_RTC_Common_getRawInterruptStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPendingInterrupt / 获取最高优先级待处理中断
 */
#define DL_RTC_B_getPendingInterrupt                DL_RTC_Common_getPendingInterrupt

/*!
 * @brief Redirects to common @ref DL_RTC_Common_clearInterruptStatus / 清除中断标志
 */
#define DL_RTC_B_clearInterruptStatus               DL_RTC_Common_clearInterruptStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableEvent / 使能RTC事件
 */
#define DL_RTC_B_enableEvent                        DL_RTC_Common_enableEvent

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableEvent / 禁用RTC事件
 */
#define DL_RTC_B_disableEvent                       DL_RTC_Common_disableEvent

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledEvents / 获取已使能的事件
 */
#define DL_RTC_B_getEnabledEvents                   DL_RTC_Common_getEnabledEvents

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getEnabledEventStatus / 获取已使能事件的状态
 */
#define DL_RTC_B_getEnabledEventStatus              DL_RTC_Common_getEnabledEventStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getRawEventsStatus / 获取原始事件状态
 */
#define DL_RTC_B_getRawEventsStatus                 DL_RTC_Common_getRawEventsStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_clearEventsStatus / 清除事件标志
 */
#define DL_RTC_B_clearEventsStatus                  DL_RTC_Common_clearEventsStatus

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setPublisherChanID / 设置事件发布通道ID
 */
#define DL_RTC_B_setPublisherChanID                 DL_RTC_Common_setPublisherChanID

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getPublisherChanID / 获取事件发布通道ID
 */
#define DL_RTC_B_getPublisherChanID                 DL_RTC_Common_getPublisherChanID

/*!
 * @brief Redirects to common @ref DL_RTC_Common_initCalendar / 初始化日历
 */
#define DL_RTC_B_initCalendar                       DL_RTC_Common_initCalendar

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarTime / 获取日历时间
 */
#define DL_RTC_B_getCalendarTime                    DL_RTC_Common_getCalendarTime

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarAlarm1 / 设置日历闹钟1
 */
#define DL_RTC_B_setCalendarAlarm1                  DL_RTC_Common_setCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarAlarm1 / 获取日历闹钟1
 */
#define DL_RTC_B_getCalendarAlarm1                  DL_RTC_Common_getCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableCalendarAlarm1 / 使能日历闹钟1
 */
#define DL_RTC_B_enableCalendarAlarm1               DL_RTC_Common_enableCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableCalendarAlarm1 / 禁用日历闹钟1
 */
#define DL_RTC_B_disableCalendarAlarm1              DL_RTC_Common_disableCalendarAlarm1

/*!
 * @brief Redirects to common @ref DL_RTC_Common_setCalendarAlarm2 / 设置日历闹钟2
 */
#define DL_RTC_B_setCalendarAlarm2                  DL_RTC_Common_setCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_getCalendarAlarm2 / 获取日历闹钟2
 */
#define DL_RTC_B_getCalendarAlarm2                  DL_RTC_Common_getCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_enableCalendarAlarm2 / 使能日历闹钟2
 */
#define DL_RTC_B_enableCalendarAlarm2               DL_RTC_Common_enableCalendarAlarm2

/*!
 * @brief Redirects to common @ref DL_RTC_Common_disableCalendarAlarm2 / 禁用日历闹钟2
 */
#define DL_RTC_B_disableCalendarAlarm2              DL_RTC_Common_disableCalendarAlarm2

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_RTC_B__ */

#endif /* ti_dl_dl_rtc_b__include */
/** @}*/
