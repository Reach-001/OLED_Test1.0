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
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       dl_adc12.h
 *  @brief      Analog to Digital Converter (ADC) / ADC12 模数转换器驱动库
 *  @defgroup   ADC12 Analog to Digital Converter (ADC12)
 *
 *  @anchor ti_dl_dl_adc12_Overview
 *  # Overview
 *
 *  The Analog to Digital Converter Driver Library allows full configuration of
 *  the MSPM0 ADC module.
 *  The ADC is a high-performance successive-approximation-register (SAR)
 *  analog-to-digital converter.
 *
 *  文件用途说明：
 *  MSPM0G3507 ADC12 外设驱动库，提供以下功能：
 *  - 外设电源管理与复位
 *  - 单次/序列采样模式初始化与配置
 *  - 时钟源、分频比、采样时间设置
 *  - 窗口比较器阈值配置
 *  - DMA 与 FIFO 模式控制
 *  - 中断、事件、DMA 触发管理
 *  - 转换结果读取
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup ADC12
 * @{
 */
#ifndef ti_dl_dl_adc12__include
#define ti_dl_dl_adc12__include

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/m0p/dl_factoryregion.h>

#ifdef __MSPM0_HAS_ADC12__

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

#if (ADC_SYS_NUM_ANALOG_CHAN > 16)
/*
 * @brief Device has more than 16 ADC input channels. 设备支持超过 16 个 ADC 输入通道
 */
#define DEVICE_HAS_GREATER_THAN_16_INPUT_CHAN
#endif

/** @addtogroup DL_ADC12_SEQ_END_ADDR
* @{
*/

/*!
 * @brief Sequence end address set to 00
 */
#define DL_ADC12_SEQ_END_ADDR_00                     (ADC12_CTL2_ENDADD_ADDR_00)

/*!
 * @brief Sequence end address set to 01
 */
#define DL_ADC12_SEQ_END_ADDR_01                     (ADC12_CTL2_ENDADD_ADDR_01)

/*!
 * @brief Sequence end address set to 02
 */
#define DL_ADC12_SEQ_END_ADDR_02                     (ADC12_CTL2_ENDADD_ADDR_02)

/*!
 * @brief Sequence end address set to 03
 */
#define DL_ADC12_SEQ_END_ADDR_03                     (ADC12_CTL2_ENDADD_ADDR_03)

/*!
 * @brief Sequence end address set to 04
 */
#define DL_ADC12_SEQ_END_ADDR_04                     (ADC12_CTL2_ENDADD_ADDR_04)

/*!
 * @brief Sequence end address set to 05
 */
#define DL_ADC12_SEQ_END_ADDR_05                     (ADC12_CTL2_ENDADD_ADDR_05)

/*!
 * @brief Sequence end address set to 06
 */
#define DL_ADC12_SEQ_END_ADDR_06                     (ADC12_CTL2_ENDADD_ADDR_06)

/*!
 * @brief Sequence end address set to 07
 */
#define DL_ADC12_SEQ_END_ADDR_07                     (ADC12_CTL2_ENDADD_ADDR_07)

/*!
 * @brief Sequence end address set to 08
 */
#define DL_ADC12_SEQ_END_ADDR_08                     (ADC12_CTL2_ENDADD_ADDR_08)

/*!
 * @brief Sequence end address set to 09
 */
#define DL_ADC12_SEQ_END_ADDR_09                     (ADC12_CTL2_ENDADD_ADDR_09)

/*!
 * @brief Sequence end address set to 10
 */
#define DL_ADC12_SEQ_END_ADDR_10                     (ADC12_CTL2_ENDADD_ADDR_10)

/*!
 * @brief Sequence end address set to 11
 */
#define DL_ADC12_SEQ_END_ADDR_11                     (ADC12_CTL2_ENDADD_ADDR_11)

/** @}*/

/** @addtogroup DL_ADC12_SEQ_START_ADDR
 * @{
 */

/*!
 * @brief Sequence start address set to 00
 */
#define DL_ADC12_SEQ_START_ADDR_00                 (ADC12_CTL2_STARTADD_ADDR_00)

/*!
 * @brief Sequence start address set to 01
 */
#define DL_ADC12_SEQ_START_ADDR_01                 (ADC12_CTL2_STARTADD_ADDR_01)

/*!
 * @brief Sequence start address set to 02
 */
#define DL_ADC12_SEQ_START_ADDR_02                 (ADC12_CTL2_STARTADD_ADDR_02)

/*!
 * @brief Sequence start address set to 03
 */
#define DL_ADC12_SEQ_START_ADDR_03                 (ADC12_CTL2_STARTADD_ADDR_03)

/*!
 * @brief Sequence start address set to 04
 */
#define DL_ADC12_SEQ_START_ADDR_04                 (ADC12_CTL2_STARTADD_ADDR_04)

/*!
 * @brief Sequence start address set to 05
 */
#define DL_ADC12_SEQ_START_ADDR_05                 (ADC12_CTL2_STARTADD_ADDR_05)

/*!
 * @brief Sequence start address set to 06
 */
#define DL_ADC12_SEQ_START_ADDR_06                 (ADC12_CTL2_STARTADD_ADDR_06)

/*!
 * @brief Sequence start address set to 07
 */
#define DL_ADC12_SEQ_START_ADDR_07                 (ADC12_CTL2_STARTADD_ADDR_07)

/*!
 * @brief Sequence start address set to 08
 */
#define DL_ADC12_SEQ_START_ADDR_08                 (ADC12_CTL2_STARTADD_ADDR_08)

/*!
 * @brief Sequence start address set to 09
 */
#define DL_ADC12_SEQ_START_ADDR_09                 (ADC12_CTL2_STARTADD_ADDR_09)

/*!
 * @brief Sequence start address set to 10
 */
#define DL_ADC12_SEQ_START_ADDR_10                 (ADC12_CTL2_STARTADD_ADDR_10)

/*!
 * @brief Sequence start address set to 11
 */
#define DL_ADC12_SEQ_START_ADDR_11                 (ADC12_CTL2_STARTADD_ADDR_11)

/** @}*/

/** @addtogroup DL_ADC12_SAMP_MODE
 * @{
 */

/*!
 * @brief Single sample mode selected. 单次采样模式
 */
#define DL_ADC12_SAMP_MODE_SINGLE                     (ADC12_CTL1_CONSEQ_SINGLE)

/*!
 * @brief Repeat single sample mode selected. 重复单次采样模式
 */
#define DL_ADC12_SAMP_MODE_SINGLE_REPEAT        (ADC12_CTL1_CONSEQ_REPEATSINGLE)

/*!
 * @brief Sequence sample mode selected. 序列采样模式
 */
#define DL_ADC12_SAMP_MODE_SEQUENCE                 (ADC12_CTL1_CONSEQ_SEQUENCE)

/*!
 * @brief Repeat sequence sample mode selected. 重复序列采样模式
 */
#define DL_ADC12_SAMP_MODE_SEQUENCE_REPEAT    (ADC12_CTL1_CONSEQ_REPEATSEQUENCE)

/** @}*/


/** @addtogroup DL_ADC12_HW_AVG_NUM
 * @{
 */

/*!
 * @brief Doesn't accumulate conversions. 禁用累加
 */
#define DL_ADC12_HW_AVG_NUM_ACC_DISABLED               (ADC12_CTL1_AVGN_DISABLE)

/*!
 * @brief Accumulates 2 conversions and then is divided by the
 *        denominator selected. 累加 2 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_2                        (ADC12_CTL1_AVGN_AVG_2)

/*!
 * @brief Accumulates 4 conversions and then is divided by the
 *        denominator selected. 累加 4 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_4                        (ADC12_CTL1_AVGN_AVG_4)

/*!
 * @brief Accumulates 8 conversions and then is divided by the
 *        denominator selected. 累加 8 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_8                        (ADC12_CTL1_AVGN_AVG_8)

/*!
 * @brief Accumulates 16 conversions and then is divided by the
 *        denominator selected. 累加 16 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_16                      (ADC12_CTL1_AVGN_AVG_16)

/*!
 * @brief Accumulates 32 conversions and then is divided by the
 *        denominator selected. 累加 32 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_32                      (ADC12_CTL1_AVGN_AVG_32)

/*!
 * @brief Accumulates 64 conversions and then is divided by the
 *        denominator selected. 累加 64 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_64                      (ADC12_CTL1_AVGN_AVG_64)

/*!
 * @brief Accumulates 128 conversions and then is divided by the
 *        denominator selected. 累加 128 次转换后除以所选分母
 */
#define DL_ADC12_HW_AVG_NUM_ACC_128                    (ADC12_CTL1_AVGN_AVG_128)

/** @}*/

/** @addtogroup DL_ADC12_HW_AVG_DEN
 * @{
 */

/*!
 * @brief Accumulated conversions are divided by 1. 累加结果除以 1
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_1                    (ADC12_CTL1_AVGD_SHIFT0)

/*!
 * @brief Accumulated conversions are divided by 2. 累加结果除以 2
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_2                    (ADC12_CTL1_AVGD_SHIFT1)

/*!
 * @brief Accumulated conversions are divided by 4. 累加结果除以 4
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_4                    (ADC12_CTL1_AVGD_SHIFT2)

/*!
 * @brief Accumulated conversions are divided by 8. 累加结果除以 8
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_8                    (ADC12_CTL1_AVGD_SHIFT3)

/*!
 * @brief Accumulated conversions are divided by 16. 累加结果除以 16
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_16                   (ADC12_CTL1_AVGD_SHIFT4)

/*!
 * @brief Accumulated conversions are divided by 32. 累加结果除以 32
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_32                   (ADC12_CTL1_AVGD_SHIFT5)

/*!
 * @brief Accumulated conversions are divided by 64. 累加结果除以 64
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_64                   (ADC12_CTL1_AVGD_SHIFT6)

/*!
 * @brief Accumulated conversions are divided by 128. 累加结果除以 128
 */
#define DL_ADC12_HW_AVG_DEN_DIV_BY_128                  (ADC12_CTL1_AVGD_SHIFT7)

/** @}*/

/** @addtogroup DL_ADC12_POWER_DOWN_MODE
 * @{
 */

/*!
 * @brief ADC12 power down mode auto. ADC12 自动掉电模式
 */
#define DL_ADC12_POWER_DOWN_MODE_AUTO                    (ADC12_CTL0_PWRDN_AUTO)

/*!
 * @brief ADC12 power down mode manual. ADC12 手动掉电模式
 */
#define DL_ADC12_POWER_DOWN_MODE_MANUAL                (ADC12_CTL0_PWRDN_MANUAL)

/** @}*/

/** @addtogroup DL_ADC12_INPUT_CHAN
 *  @{
 */
/*!
 * @brief ADC12 input channel 0 selected
 */
#define DL_ADC12_INPUT_CHAN_0                      (ADC12_MEMCTL_CHANSEL_CHAN_0)

/*!
 * @brief ADC12 input channel 1 selected
 */
#define DL_ADC12_INPUT_CHAN_1                      (ADC12_MEMCTL_CHANSEL_CHAN_1)

/*!
 * @brief ADC12 input channel 2 selected
 */
#define DL_ADC12_INPUT_CHAN_2                      (ADC12_MEMCTL_CHANSEL_CHAN_2)

/*!
 * @brief ADC12 input channel 3 selected
 */
#define DL_ADC12_INPUT_CHAN_3                      (ADC12_MEMCTL_CHANSEL_CHAN_3)

/*!
 * @brief ADC12 input channel 4 selected
 */
#define DL_ADC12_INPUT_CHAN_4                      (ADC12_MEMCTL_CHANSEL_CHAN_4)

/*!
 * @brief ADC12 input channel 5 selected
 */
#define DL_ADC12_INPUT_CHAN_5                      (ADC12_MEMCTL_CHANSEL_CHAN_5)

/*!
 * @brief ADC12 input channel 6 selected
 */
#define DL_ADC12_INPUT_CHAN_6                      (ADC12_MEMCTL_CHANSEL_CHAN_6)

/*!
 * @brief ADC12 input channel 7 selected
 */
#define DL_ADC12_INPUT_CHAN_7                      (ADC12_MEMCTL_CHANSEL_CHAN_7)

/*!
 * @brief ADC12 input channel 8 selected
 */
#define DL_ADC12_INPUT_CHAN_8                      (ADC12_MEMCTL_CHANSEL_CHAN_8)

/*!
 * @brief ADC12 input channel 9 selected
 */
#define DL_ADC12_INPUT_CHAN_9                      (ADC12_MEMCTL_CHANSEL_CHAN_9)

/*!
 * @brief ADC12 input channel 10 selected
 */
#define DL_ADC12_INPUT_CHAN_10                    (ADC12_MEMCTL_CHANSEL_CHAN_10)

/*!
 * @brief ADC12 input channel 11 selected
 */
#define DL_ADC12_INPUT_CHAN_11                    (ADC12_MEMCTL_CHANSEL_CHAN_11)

/*!
 * @brief ADC12 input channel 12 selected
 */
#define DL_ADC12_INPUT_CHAN_12                    (ADC12_MEMCTL_CHANSEL_CHAN_12)

/*!
 * @brief ADC12 input channel 13 selected
 */
#define DL_ADC12_INPUT_CHAN_13                    (ADC12_MEMCTL_CHANSEL_CHAN_13)

/*!
 * @brief ADC12 input channel 14 selected
 */
#define DL_ADC12_INPUT_CHAN_14                    (ADC12_MEMCTL_CHANSEL_CHAN_14)

/*!
 * @brief ADC12 input channel 15 selected
 */
#define DL_ADC12_INPUT_CHAN_15                    (ADC12_MEMCTL_CHANSEL_CHAN_15)

#ifdef DEVICE_HAS_GREATER_THAN_16_INPUT_CHAN
/*!
 * @brief ADC12 input channel 16 selected
 */
#define DL_ADC12_INPUT_CHAN_16                    (ADC12_MEMCTL_CHANSEL_CHAN_16)

/*!
 * @brief ADC12 input channel 17 selected
 */
#define DL_ADC12_INPUT_CHAN_17                    (ADC12_MEMCTL_CHANSEL_CHAN_17)

/*!
 * @brief ADC12 input channel 18 selected
 */
#define DL_ADC12_INPUT_CHAN_18                    (ADC12_MEMCTL_CHANSEL_CHAN_18)

/*!
 * @brief ADC12 input channel 19 selected
 */
#define DL_ADC12_INPUT_CHAN_19                    (ADC12_MEMCTL_CHANSEL_CHAN_19)

/*!
 * @brief ADC12 input channel 20 selected
 */
#define DL_ADC12_INPUT_CHAN_20                    (ADC12_MEMCTL_CHANSEL_CHAN_20)

/*!
 * @brief ADC12 input channel 21 selected
 */
#define DL_ADC12_INPUT_CHAN_21                    (ADC12_MEMCTL_CHANSEL_CHAN_21)

/*!
 * @brief ADC12 input channel 22 selected
 */
#define DL_ADC12_INPUT_CHAN_22                    (ADC12_MEMCTL_CHANSEL_CHAN_22)

/*!
 * @brief ADC12 input channel 23 selected
 */
#define DL_ADC12_INPUT_CHAN_23                    (ADC12_MEMCTL_CHANSEL_CHAN_23)

/*!
 * @brief ADC12 input channel 24 selected
 */
#define DL_ADC12_INPUT_CHAN_24                    (ADC12_MEMCTL_CHANSEL_CHAN_24)

/*!
 * @brief ADC12 input channel 25 selected
 */
#define DL_ADC12_INPUT_CHAN_25                    (ADC12_MEMCTL_CHANSEL_CHAN_25)

/*!
 * @brief ADC12 input channel 26 selected
 */
#define DL_ADC12_INPUT_CHAN_26                    (ADC12_MEMCTL_CHANSEL_CHAN_26)

/*!
 * @brief ADC12 input channel 27 selected
 */
#define DL_ADC12_INPUT_CHAN_27                    (ADC12_MEMCTL_CHANSEL_CHAN_27)

/*!
 * @brief ADC12 input channel 28 selected
 */
#define DL_ADC12_INPUT_CHAN_28                    (ADC12_MEMCTL_CHANSEL_CHAN_28)

/*!
 * @brief ADC12 input channel 29 selected
 */
#define DL_ADC12_INPUT_CHAN_29                    (ADC12_MEMCTL_CHANSEL_CHAN_29)

/*!
 * @brief ADC12 input channel 30 selected
 */
#define DL_ADC12_INPUT_CHAN_30                    (ADC12_MEMCTL_CHANSEL_CHAN_30)

/*!
 * @brief ADC12 input channel 31 selected
 */
#define DL_ADC12_INPUT_CHAN_31                    (ADC12_MEMCTL_CHANSEL_CHAN_31)

#endif /* DEVICE_HAS_GREATER_THAN_16_INPUT_CHAN */

/** @}*/

/** @addtogroup DL_ADC12_REFERENCE_VOLTAGE
 *  @{
 */

/*!
 * @brief ADC12 voltage reference VDDA. ADC12 参考电压 VDDA
 */
#define DL_ADC12_REFERENCE_VOLTAGE_VDDA                (ADC12_MEMCTL_VRSEL_VDDA_VSSA)

/*!
 * @brief ADC12 voltage reference external. ADC12 外部参考电压
 */
#define DL_ADC12_REFERENCE_VOLTAGE_EXTREF            (ADC12_MEMCTL_VRSEL_EXTREF_VREFM)

/*!
 * @brief ADC12 voltage reference internal. ADC12 内部参考电压
 */
#define DL_ADC12_REFERENCE_VOLTAGE_INTREF            (ADC12_MEMCTL_VRSEL_INTREF_VSSA)

#ifndef __MSPM0_HAS_LEGACY_ADC_REFERENCE__
/*!
 * @brief ADC12 voltage reference VDDA
 */
#define DL_ADC12_REFERENCE_VOLTAGE_VDDA_VSSA               (ADC12_MEMCTL_VRSEL_VDDA_VSSA)

/*!
 * @brief ADC12 voltage reference external
 */
#define DL_ADC12_REFERENCE_VOLTAGE_EXTREF_VREFM            (ADC12_MEMCTL_VRSEL_EXTREF_VREFM)

/*!
 * @brief ADC12 voltage reference internal
 */
#define DL_ADC12_REFERENCE_VOLTAGE_INTREF_VSSA            (ADC12_MEMCTL_VRSEL_INTREF_VSSA)

/*!
 * @brief ADC12 voltage reference VDDA and VREFM connected to VREF+.
 *        ADC12 参考电压 VDDA 与 VREFM 连接至 VREF+
 */
#define DL_ADC12_REFERENCE_VOLTAGE_VDDA_VREFM            (ADC12_MEMCTL_VRSEL_VDDA_VREFM)

/*!
 * @brief ADC12 voltage reference INTREF and VREFM connected to VREF+ and VREF-.
 *        ADC12 参考电压 INTREF 与 VREFM 连接至 VREF+ 和 VREF-
 */
#define DL_ADC12_REFERENCE_VOLTAGE_INTREF_VREFM            (ADC12_MEMCTL_VRSEL_INTREF_VREFM)

#endif
/** @}*/

/** @addtogroup DL_ADC12_SAMPLE_TIMER_SOURCE
 *  @{
 */

/*!
 * @brief ADC12 sample adc12 source 0
 */
#define DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0      (ADC12_MEMCTL_STIME_SEL_SCOMP0)

/*!
 * @brief ADC12 sample adc12 source 1
 */
#define DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP1      (ADC12_MEMCTL_STIME_SEL_SCOMP1)

/** @}*/

/** @addtogroup DL_ADC12_AVERAGING_MODE
 *  @{
 */

/*!
 * @brief ADC12 averaging mode enabled. ADC12 均值模式已启用
 */
#define DL_ADC12_AVERAGING_MODE_ENABLED              (ADC12_MEMCTL_AVGEN_ENABLE)

/*!
 * @brief ADC12 averaging mode disabled. ADC12 均值模式已禁用
 */
#define DL_ADC12_AVERAGING_MODE_DISABLED            (ADC12_MEMCTL_AVGEN_DISABLE)

/** @}*/

/** @addtogroup DL_ADC12_BURN_OUT_SOURCE
 *  @{
 */

/*!
 * @brief ADC12 burn out current source enabled. ADC12 烧毁电流源已启用
 */
#define DL_ADC12_BURN_OUT_SOURCE_ENABLED             (ADC12_MEMCTL_BCSEN_ENABLE)

/*!
 * @brief ADC12 burn out current source disabled. ADC12 烧毁电流源已禁用
 */
#define DL_ADC12_BURN_OUT_SOURCE_DISABLED           (ADC12_MEMCTL_BCSEN_DISABLE)

/** @}*/

/** @addtogroup DL_ADC12_TRIGGER_MODE
 *  @{
 */

/*!
 * @brief ADC12 trigger automatically step to next memory conversion register.
 *        ADC12 触发后自动切换到下一个内存转换寄存器
 */
#define DL_ADC12_TRIGGER_MODE_AUTO_NEXT           (ADC12_MEMCTL_TRIG_AUTO_NEXT)

/*!
 * @brief ADC12 valid trigger will step to next memory conversion register.
 *        ADC12 有效触发后切换到下一个内存转换寄存器
 */
#define DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT      (ADC12_MEMCTL_TRIG_TRIGGER_NEXT)

/** @}*/

/** @addtogroup DL_ADC12_WINDOWS_COMP_MODE
 *  @{
 */

/*!
 * @brief ADC12 window comparator enabled. ADC12 窗口比较器已启用
 */
#define DL_ADC12_WINDOWS_COMP_MODE_ENABLED         (ADC12_MEMCTL_WINCOMP_ENABLE)

/*!
 * @brief ADC12 window comparator disabled. ADC12 窗口比较器已禁用
 */
#define DL_ADC12_WINDOWS_COMP_MODE_DISABLED       (ADC12_MEMCTL_WINCOMP_DISABLE)

/** @}*/

/** @addtogroup DL_ADC12_STATUS_CONVERSION
 *  @{
 */

/*!
 * @brief Indicates sample or conversion is in progress. 采样或转换正在进行
 */
#define DL_ADC12_STATUS_CONVERSION_ACTIVE             (ADC12_STATUS_BUSY_ACTIVE)

/*!
 * @brief Indicates sample or conversion is not in progress. 采样或转换未在进行
 */
#define DL_ADC12_STATUS_CONVERSION_IDLE                 (ADC12_STATUS_BUSY_IDLE)

/** @}*/

/** @addtogroup DL_ADC12_STATUS_REFERENCE
 *  @{
 */

/*!
 * @brief Indicates reference buffer is powered up and ready. 参考缓冲区已上电就绪
 */
#define DL_ADC12_STATUS_REFERENCE_READY           (ADC12_STATUS_REFBUFRDY_READY)

/*!
 * @brief Indicates reference buffer is not ready. 参考缓冲区未就绪
 */
#define DL_ADC12_STATUS_REFERENCE_NOTREADY     (ADC12_STATUS_REFBUFRDY_NOTREADY)

/** @}*/

/** @addtogroup DL_ADC12_INTERRUPTS
 *  @{
 */

/*!
 * @brief ADC12 MEMRESX overflow. ADC12 转换结果溢出
 */
#define DL_ADC12_INTERRUPT_OVERFLOW           (ADC12_CPU_INT_IMASK_OVIFG_SET)

/*!
 * @brief ADC12 sequence conversion trigger overflow. ADC12 序列转换触发溢出
 */
#define DL_ADC12_INTERRUPT_TRIG_OVF      (ADC12_CPU_INT_IMASK_TOVIFG_SET)

/*!
 * @brief ADC12 MEMRESx result higher than window comparator high threshold.
 *        ADC12 转换结果高于窗口比较器高阈值
 */
#define DL_ADC12_INTERRUPT_WINDOW_COMP_HIGH (ADC12_CPU_INT_IMASK_HIGHIFG_SET)

/*!
 * @brief ADC12 MEMRESx result lower than window comparator low threshold.
 *        ADC12 转换结果低于窗口比较器低阈值
 */
#define DL_ADC12_INTERRUPT_WINDOW_COMP_LOW   (ADC12_CPU_INT_IMASK_LOWIFG_SET)

/*!
 * @brief ADC12 MEMRESx result is between low and high window comparator
 *        threshold. ADC12 转换结果在窗口比较器高低阈值之间
 */
#define DL_ADC12_INTERRUPT_INIFG              (ADC12_CPU_INT_IMASK_INIFG_SET)

/*!
 * @brief ADC12 DMA done. ADC12 DMA 传输完成
 */
#define DL_ADC12_INTERRUPT_DMA_DONE         (ADC12_CPU_INT_IMASK_DMADONE_SET)

/*!
 * @brief ADC12 MEMRESX underflow. ADC12 转换结果下溢
 */
#define DL_ADC12_INTERRUPT_UNDERFLOW          (ADC12_CPU_INT_IMASK_UVIFG_SET)

/*!
 * @brief ADC12 MEM0 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG0_SET)

/*!
 * @brief ADC12 MEM1 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG1_SET)

/*!
 * @brief ADC12 MEM2 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM2_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG2_SET)

/*!
 * @brief ADC12 MEM3 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM3_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG3_SET)

/*!
 * @brief ADC12 MEM4 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM4_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG4_SET)

/*!
 * @brief ADC12 MEM5 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM5_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG5_SET)

/*!
 * @brief ADC12 MEM6 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM6_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG6_SET)

/*!
 * @brief ADC12 MEM7 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM7_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG7_SET)

/*!
 * @brief ADC12 MEM8 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM8_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG8_SET)

/*!
 * @brief ADC12 MEM9 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM9_RESULT_LOADED \
                                         (ADC12_CPU_INT_IMASK_MEMRESIFG9_SET)

/*!
 * @brief ADC12 MEM10 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM10_RESULT_LOADED \
                                        (ADC12_CPU_INT_IMASK_MEMRESIFG10_SET)

/*!
 * @brief ADC12 MEM12 result loaded interrupt
 */
#define DL_ADC12_INTERRUPT_MEM11_RESULT_LOADED \
                                        (ADC12_CPU_INT_IMASK_MEMRESIFG11_SET)

/** @}*/

/** @addtogroup DL_ADC12_EVENT
 *  @{
 */

/*!
 * @brief ADC12 MEMRESx result higher than window comparator high threshold.
 *        ADC12 转换结果高于窗口比较器高阈值事件
 */
#define DL_ADC12_EVENT_WINDOW_COMP_HIGH     (ADC12_GEN_EVENT_IMASK_HIGHIFG_SET)

/*!
 * @brief ADC12 MEMRESx result lower than window comparator low threshold.
 *        ADC12 转换结果低于窗口比较器低阈值事件
 */
#define DL_ADC12_EVENT_WINDOW_COMP_LOW       (ADC12_GEN_EVENT_IMASK_LOWIFG_SET)

/*!
 * @brief ADC12 MEMRESx result between high and low window comparator threshold.
 *        ADC12 转换结果在窗口比较器阈值之间事件
 */
#define DL_ADC12_EVENT_INIFG                  (ADC12_GEN_EVENT_IMASK_INIFG_SET)

/*!
 * @brief ADC12 MEM0 result loaded interrupt. ADC12 MEM0 结果加载完成事件
 */
#define DL_ADC12_EVENT_MEM0_RESULT_LOADED \
                                         (ADC12_GEN_EVENT_IMASK_MEMRESIFG0_SET)

/** @}*/

/** @addtogroup DL_ADC12_DMA
 *  @{
 */

/*!
 * @brief ADC12 MEM0 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM0_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG0_SET)

/*!
 * @brief ADC12 MEM1 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM1_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG1_SET)

/*!
 * @brief ADC12 MEM2 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM2_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG2_SET)

/*!
 * @brief ADC12 MEM3 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM3_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG3_SET)

/*!
 * @brief ADC12 MEM4 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM4_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG4_SET)

/*!
 * @brief ADC12 MEM5 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM5_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG5_SET)

/*!
 * @brief ADC12 MEM6 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM6_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG6_SET)

/*!
 * @brief ADC12 MEM7 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM7_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG7_SET)

/*!
 * @brief ADC12 MEM8 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM8_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG8_SET)

/*!
 * @brief ADC12 MEM9 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM9_RESULT_LOADED  (ADC12_DMA_TRIG_IMASK_MEMRESIFG9_SET)

/*!
 * @brief ADC12 MEM10 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM10_RESULT_LOADED \
                                        (ADC12_DMA_TRIG_IMASK_MEMRESIFG10_SET)

/*!
 * @brief ADC12 MEM11 result loaded interrupt
 */
#define DL_ADC12_DMA_MEM11_RESULT_LOADED \
                                        (ADC12_DMA_TRIG_IMASK_MEMRESIFG11_SET)

/** @}*/

/*!
 * @brief  This is an internal macro is used to resolve the offset to ADC12 SVT.
 *         内部宏，用于计算 ADC12 SVT 寄存器偏移量
 *
 * The offset from ADC12 base to ADC12 SVT is 0x556000. However the FIFODATA
 * and MEMRES registers within SVT are offset by an additional 0x1000. For example,
 * the FIFODATA register from ADC12 base has offset 0x1160. The same register
 * in ADC12 SVT has offset 0x0160.
 *
 * This 0x1000 difference is taken in to consideration by DL_ADC12_SVT_OFFSET.
 * Formula: (ADC12 SVT - ADC12 BASE) - 0x1000
 *
 * Used by the following APIs:
 *  @ref DL_ADC12_getFIFOData
 *  @ref DL_ADC12_getFIFOAddress
 *  @ref DL_ADC12_getMemResult
 *  @ref DL_ADC12_getMemResultAddress
 *
 */
#define DL_ADC12_SVT_OFFSET                  ((uint32_t)0x555000 >> (uint32_t)2)

    /* clang-format on */

    /*! @enum DL_ADC12_MEM_IDX  ADC12 内存转换索引 */
    typedef enum
    {

        /*! ADC12 Memory conversion index 0. ADC12 内存转换索引 0 */
        DL_ADC12_MEM_IDX_0 = 0,

        /*! ADC12 Memory conversion index 1. ADC12 内存转换索引 1 */
        DL_ADC12_MEM_IDX_1 = 1,

        /*! ADC12 Memory conversion index 2. ADC12 内存转换索引 2 */
        DL_ADC12_MEM_IDX_2 = 2,

        /*! ADC12 Memory conversion index 3. ADC12 内存转换索引 3 */
        DL_ADC12_MEM_IDX_3 = 3,

        /*! ADC12 Memory conversion index 4. ADC12 内存转换索引 4 */
        DL_ADC12_MEM_IDX_4 = 4,

        /*! ADC12 Memory conversion index 5. ADC12 内存转换索引 5 */
        DL_ADC12_MEM_IDX_5 = 5,

        /*! ADC12 Memory conversion index 6. ADC12 内存转换索引 6 */
        DL_ADC12_MEM_IDX_6 = 6,

        /*! ADC12 Memory conversion index 7. ADC12 内存转换索引 7 */
        DL_ADC12_MEM_IDX_7 = 7,

        /*! ADC12 Memory conversion index 8. ADC12 内存转换索引 8 */
        DL_ADC12_MEM_IDX_8 = 8,

        /*! ADC12 Memory conversion index 9. ADC12 内存转换索引 9 */
        DL_ADC12_MEM_IDX_9 = 9,

        /*! ADC12 Memory conversion index 10. ADC12 内存转换索引 10 */
        DL_ADC12_MEM_IDX_10 = 10,

        /*! ADC12 Memory conversion index 11. ADC12 内存转换索引 11 */
        DL_ADC12_MEM_IDX_11 = 11,

    } DL_ADC12_MEM_IDX;

    /*! @enum DL_ADC12_REPEAT_MODE  ADC12 重复模式 */
    typedef enum
    {

        /*! Sequence mode single repeat. 重复单次采样模式 */
        DL_ADC12_REPEAT_MODE_ENABLED = ADC12_CTL1_CONSEQ_REPEATSINGLE,

        /*! Sequence mode single. 单次采样模式 */
        DL_ADC12_REPEAT_MODE_DISABLED = ADC12_CTL1_CONSEQ_SINGLE

    } DL_ADC12_REPEAT_MODE;

    /*! @enum DL_ADC12_SAMPLING_SOURCE  ADC12 采样信号源 */
    typedef enum
    {
        /*! Timer high phase is used as source of the sample signal. 自动采样模式 */
        DL_ADC12_SAMPLING_SOURCE_AUTO = ADC12_CTL1_SAMPMODE_AUTO,

        /*! External or software trigger is used as source of the sample signal.
         *  手动采样模式（外部或软件触发） */
        DL_ADC12_SAMPLING_SOURCE_MANUAL = ADC12_CTL1_SAMPMODE_MANUAL
    } DL_ADC12_SAMPLING_SOURCE;

    /*! @enum DL_ADC12_TRIG_SRC  ADC12 触发源 */
    typedef enum
    {
        /*! Conversion is triggered by software. 软件触发转换 */
        DL_ADC12_TRIG_SRC_SOFTWARE = ADC12_CTL1_TRIGSRC_SOFTWARE,

        /*! Conversion is triggered by hardware. 硬件触发转换 */
        DL_ADC12_TRIG_SRC_EVENT = ADC12_CTL1_TRIGSRC_EVENT

    } DL_ADC12_TRIG_SRC;

    /*! @enum DL_ADC12_SAMP_CONV_RES  ADC12 采样转换分辨率 */
    typedef enum
    {
        /*! 12-bits resolution. 12 位分辨率 */
        DL_ADC12_SAMP_CONV_RES_12_BIT = ADC12_CTL2_RES_BIT_12,
        /*! 10-bits resolution. 10 位分辨率 */
        DL_ADC12_SAMP_CONV_RES_10_BIT = ADC12_CTL2_RES_BIT_10,
        /*! 8-bits resolution. 8 位分辨率 */
        DL_ADC12_SAMP_CONV_RES_8_BIT = ADC12_CTL2_RES_BIT_8
    } DL_ADC12_SAMP_CONV_RES;

    /*! @enum DL_ADC12_SAMP_CONV_DATA_FORMAT  ADC12 转换结果数据格式 */
    typedef enum
    {
        /*! Results are read as binary unsigned, right aligned.
         *  无符号二进制右对齐 */
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED = ADC12_CTL2_DF_UNSIGNED,

        /*! Result are read as signed binary (2s complement), left aligned.
         *  有符号二进制（补码）左对齐 */
        DL_ADC12_SAMP_CONV_DATA_FORMAT_SIGNED = ADC12_CTL2_DF_SIGNED

    } DL_ADC12_SAMP_CONV_DATA_FORMAT;

    /*! @enum DL_ADC12_IIDX  ADC12 中断索引 */
    typedef enum
    {
        /*! ADC12 interrupt index for MEMRESX overflow */
        DL_ADC12_IIDX_OVERFLOW = ADC12_CPU_INT_IIDX_STAT_OVIFG,

        /*! ADC12 interrupt index for sequence conversion trigger overflow */
        DL_ADC12_IIDX_TRIG_OVERFLOW = ADC12_CPU_INT_IIDX_STAT_TOVIFG,

        /*! ADC12 interrupt index for MEMRESx result higher than window
         * comparator high threshold */
        DL_ADC12_IIDX_WINDOW_COMP_HIGH = ADC12_CPU_INT_IIDX_STAT_HIGHIFG,

        /*! ADC12 interrupt index for MEMRESx result lower than window
         *  comparator low threshold */
        DL_ADC12_IIDX_WINDOW_COMP_LOW = ADC12_CPU_INT_IIDX_STAT_LOWIFG,

        /*! ADC12 interrupt index for result in range */
        DL_ADC12_IIDX_INIFG = ADC12_CPU_INT_IIDX_STAT_INIFG,

        /*! ADC12 interrupt index for DMA done */
        DL_ADC12_IIDX_DMA_DONE = ADC12_CPU_INT_IIDX_STAT_DMADONE,

        /*! ADC12 interrupt index for MEMRESX underflow */
        DL_ADC12_IIDX_UNDERFLOW = ADC12_CPU_INT_IIDX_STAT_UVIFG,

        /*! ADC12 interrupt index for MEM0 result loaded interrupt */
        DL_ADC12_IIDX_MEM0_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG0,

        /*! ADC12 interrupt index for MEM1 result loaded interrupt */
        DL_ADC12_IIDX_MEM1_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG1,

        /*! ADC12 interrupt index for MEM2 result loaded interrupt */
        DL_ADC12_IIDX_MEM2_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG2,

        /*! ADC12 interrupt index for MEM3 result loaded interrupt */
        DL_ADC12_IIDX_MEM3_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG3,

        /*! ADC12 interrupt index for MEM4 result loaded interrupt */
        DL_ADC12_IIDX_MEM4_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG4,

        /*! ADC12 interrupt index for MEM5 result loaded interrupt */
        DL_ADC12_IIDX_MEM5_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG5,

        /*! ADC12 interrupt index for MEM6 result loaded interrupt */
        DL_ADC12_IIDX_MEM6_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG6,

        /*! ADC12 interrupt index for MEM7 result loaded interrupt */
        DL_ADC12_IIDX_MEM7_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG7,

        /*! ADC12 interrupt index for MEM8 result loaded interrupt */
        DL_ADC12_IIDX_MEM8_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG8,

        /*! ADC12 interrupt index for MEM9 result loaded interrupt */
        DL_ADC12_IIDX_MEM9_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG9,

        /*! ADC12 interrupt index for MEM10 result loaded interrupt */
        DL_ADC12_IIDX_MEM10_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG10,

        /*! ADC12 interrupt index for MEM10 result loaded interrupt */
        DL_ADC12_IIDX_MEM11_RESULT_LOADED = ADC12_CPU_INT_IIDX_STAT_MEMRESIFG11,
    } DL_ADC12_IIDX;

    /* clang-format on */
    /*! @enum DL_ADC12_CLOCK  ADC12 采样时钟源 */
    typedef enum
    {
        /*! SYSOSC is the source of ADC sample clock. SYSOSC 作为 ADC 采样时钟源 */
        DL_ADC12_CLOCK_SYSOSC = ADC12_CLKCFG_SAMPCLK_SYSOSC,
        /*! ULPCLK is the source of ADC sample clock. ULPCLK 作为 ADC 采样时钟源 */
        DL_ADC12_CLOCK_ULPCLK = ADC12_CLKCFG_SAMPCLK_ULPCLK,
        /*! HFCLK is the source of ADC sample clock. HFCLK 作为 ADC 采样时钟源 */
        DL_ADC12_CLOCK_HFCLK = ADC12_CLKCFG_SAMPCLK_HFCLK,
    } DL_ADC12_CLOCK;

    /*! @enum DL_ADC12_CLOCK_DIVIDE  ADC12 采样时钟分频比 */
    typedef enum
    {

        /*! Divide sample clock source by 1. 采样时钟 1 分频 */
        DL_ADC12_CLOCK_DIVIDE_1 = ADC12_CTL0_SCLKDIV_DIV_BY_1,

        /*! Divide sample clock source by 2. 采样时钟 2 分频 */
        DL_ADC12_CLOCK_DIVIDE_2 = ADC12_CTL0_SCLKDIV_DIV_BY_2,

        /*! Divide sample clock source by 4. 采样时钟 4 分频 */
        DL_ADC12_CLOCK_DIVIDE_4 = ADC12_CTL0_SCLKDIV_DIV_BY_4,

        /*! Divide sample clock source by 8. 采样时钟 8 分频 */
        DL_ADC12_CLOCK_DIVIDE_8 = ADC12_CTL0_SCLKDIV_DIV_BY_8,

        /*! Divide sample clock source by 16. 采样时钟 16 分频 */
        DL_ADC12_CLOCK_DIVIDE_16 = ADC12_CTL0_SCLKDIV_DIV_BY_16,

        /*! Divide sample clock source by 24. 采样时钟 24 分频 */
        DL_ADC12_CLOCK_DIVIDE_24 = ADC12_CTL0_SCLKDIV_DIV_BY_24,

        /*! Divide sample clock source by 32. 采样时钟 32 分频 */
        DL_ADC12_CLOCK_DIVIDE_32 = ADC12_CTL0_SCLKDIV_DIV_BY_32,

        /*! Divide sample clock source by 48. 采样时钟 48 分频 */
        DL_ADC12_CLOCK_DIVIDE_48 = ADC12_CTL0_SCLKDIV_DIV_BY_48,

    } DL_ADC12_CLOCK_DIVIDE;

    /*! @enum DL_ADC12_CLOCK_FREQ_RANGE  ADC12 时钟频率范围 */
    typedef enum
    {
        /*! Specifies ADC clock (ADCCLK) frequency range is from 1 MHz to 4 MHz.
         *  ADCCLK 频率范围 1~4 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_1_TO_4 = ADC12_CLKFREQ_FRANGE_RANGE1TO4,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 4 MHz to 8 MHz.
         *  ADCCLK 频率范围 4~8 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_4_TO_8 = ADC12_CLKFREQ_FRANGE_RANGE4TO8,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 8 MHz to 16 MHz.
         *  ADCCLK 频率范围 8~16 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_8_TO_16 = ADC12_CLKFREQ_FRANGE_RANGE8TO16,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 16 MHz to 20 MHz.
         *  ADCCLK 频率范围 16~20 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_16_TO_20 = ADC12_CLKFREQ_FRANGE_RANGE16TO20,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 20 MHz to 24 MHz.
         *  ADCCLK 频率范围 20~24 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_20_TO_24 = ADC12_CLKFREQ_FRANGE_RANGE20TO24,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 24 MHz to 32 MHz.
         *  ADCCLK 频率范围 24~32 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32 = ADC12_CLKFREQ_FRANGE_RANGE24TO32,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 32 MHz to 40 MHz.
         *  ADCCLK 频率范围 32~40 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_32_TO_40 = ADC12_CLKFREQ_FRANGE_RANGE32TO40,

        /*! Specifies ADC clock (ADCCLK) frequency range is from 40 MHz to 48 MHz.
         *  ADCCLK 频率范围 40~48 MHz */
        DL_ADC12_CLOCK_FREQ_RANGE_40_TO_48 = ADC12_CLKFREQ_FRANGE_RANGE40TO48,

    } DL_ADC12_CLOCK_FREQ_RANGE;

    /**
     * @brief  Configuration struct for @ref DL_ADC12_setClockConfig.
     *         ADC12 时钟配置结构体
     */
    typedef struct
    {
        /*! ADC12 clock source selection. This sources the ADCCLK. One of
         * @ref DL_ADC12_CLOCK */
        DL_ADC12_CLOCK clockSel;
        /*! ADC12 clock source frequency range. One of
         * @ref DL_ADC12_CLOCK_FREQ_RANGE */
        DL_ADC12_CLOCK_FREQ_RANGE freqRange;
        /*! ADC12 sample clock divider selection. This is used to generate the ADC12
         * Sample Clock frequency. One of @ref DL_ADC12_CLOCK_DIVIDE */
        DL_ADC12_CLOCK_DIVIDE divideRatio;
    } DL_ADC12_ClockConfig;

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the ADC12.
     *        使能 ADC12 外设电源写保护寄存器
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  @note For power savings, please refer to @ref DL_ADC12_setPowerDownMode
     *
     * @param adc12        Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_enablePower(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.GPRCM.PWREN =
            (ADC12_PWREN_KEY_UNLOCK_W | ADC12_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the ADC12.
     *        禁用 ADC12 外设电源写保护寄存器
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @note This API does not provide large power savings. For power savings
     *  please refer to @ref DL_ADC12_setPowerDownMode
     *
     * @param adc12        Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_disablePower(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.GPRCM.PWREN =
            (ADC12_PWREN_KEY_UNLOCK_W | ADC12_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the ADC12
     *        is enabled. 检查 ADC12 外设电源是否已使能
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     * @param adc12        Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     * @return true if peripheral register access is enabled. 外设已使能返回 true
     * @return false if peripheral register access is disabled. 外设已禁用返回 false
     */
    __STATIC_INLINE bool DL_ADC12_isPowerEnabled(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.GPRCM.PWREN & ADC12_PWREN_ENABLE_MASK) ==
                ADC12_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Resets adc12 peripheral. 复位 ADC12 外设
     *
     * @param adc12        Pointer to the register overlay for the ADC12 peripheral.
     *                     ADC12 外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_reset(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.GPRCM.RSTCTL =
            (ADC12_RSTCTL_KEY_UNLOCK_W | ADC12_RSTCTL_RESETSTKYCLR_CLR |
             ADC12_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief Returns if adc12 peripheral was reset. 检查 ADC12 外设是否已复位
     *
     * @param adc12        Pointer to the register overlay for the ADC12 peripheral.
     *                     ADC12 外设寄存器基地址指针
     *
     * @return true if peripheral was reset. 已复位返回 true
     * @return false if peripheral wasn't reset. 未复位返回 false
     *
     */
    __STATIC_INLINE bool DL_ADC12_isReset(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.GPRCM.STAT & ADC12_STAT_RESETSTKY_MASK) ==
                ADC12_STAT_RESETSTKY_RESET);
    }

    /**
     * @brief Initializes ADC12 for single sampling mode operation. This
     * initialization configures MEMCTL0 as the default memory control register for
     * the conversion. If the conversion needs use a different memory control
     * register the user can call @ref DL_ADC12_setStartAddress to specify a
     * different control register.
     *        初始化 ADC12 单次采样模式。默认使用 MEMCTL0 作为转换控制寄存器。
     *        如需使用其他寄存器，可调用 DL_ADC12_setStartAddress 指定。
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        外设寄存器基地址指针
     * @param[in] repeatMode  Specifies repeat configuration. One of
     *                    @ref DL_ADC12_REPEAT_MODE. 重复模式配置
     * @param[in] sampleMode  Specifies source of the sampling signal. One of
     *                    @ref DL_ADC12_SAMPLING_SOURCE. 采样信号源
     * @param[in] trigSrc     Specifies sampling trigger source. One of
     *                    @ref DL_ADC12_TRIG_SRC. 采样触发源
     * @param[in] resolution  Specifies sample conversion resolution. One of
     *                    @ref DL_ADC12_SAMP_CONV_RES. 转换分辨率
     * @param[in] dataFormat  Specifies sample conversion data format. One of
     *                    @ref DL_ADC12_SAMP_CONV_DATA_FORMAT. 转换数据格式
     *
     */
    __STATIC_INLINE void DL_ADC12_initSingleSample(ADC12_Regs *adc12,
                                                   DL_ADC12_REPEAT_MODE repeatMode, DL_ADC12_SAMPLING_SOURCE sampleMode,
                                                   DL_ADC12_TRIG_SRC trigSrc, DL_ADC12_SAMP_CONV_RES resolution,
                                                   DL_ADC12_SAMP_CONV_DATA_FORMAT dataFormat)
    {
        DL_Common_updateReg(&adc12->ULLMEM.CTL1,
                            (repeatMode | sampleMode | trigSrc),
                            (ADC12_CTL1_SAMPMODE_MASK | ADC12_CTL1_CONSEQ_MASK |
                             ADC12_CTL1_TRIGSRC_MASK));

        DL_Common_updateReg(&adc12->ULLMEM.CTL2,
                            (ADC12_CTL2_STARTADD_ADDR_00 | ADC12_CTL2_ENDADD_ADDR_00 | resolution |
                             dataFormat),
                            (ADC12_CTL2_STARTADD_MASK | ADC12_CTL2_ENDADD_MASK |
                             ADC12_CTL2_RES_MASK | ADC12_CTL2_DF_MASK));
    }

    /**
     *  @brief Sets the start address for ADC conversion.
     *         设置 ADC 转换起始地址
     *
     *  @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                         外设寄存器基地址指针
     *  @param[in] startAdd    If ADC has been initialized in Single Sample mode,
     *                         startAdd specifies the memory control register to be
     *                         used during conversion. If ADC is initialized in
     *                         sequence sampling mode, it specifies the first memory
     *                         control register in the sequence. One of
     *                         @ref DL_ADC12_SEQ_START_ADDR.
     *                         转换起始地址，单次模式指定内存控制寄存器，
     *                         序列模式指定序列首地址
     *
     */
    __STATIC_INLINE void DL_ADC12_setStartAddress(
        ADC12_Regs *adc12, uint32_t startAdd)
    {
        DL_Common_updateReg(
            &adc12->ULLMEM.CTL2, startAdd, ADC12_CTL2_STARTADD_MASK);
    }

    /**
     *  @brief Gets start address for ADC conversion.
     *         获取 ADC 转换起始地址
     *
     *  @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                         外设寄存器基地址指针
     *
     *  @return One of @ref DL_ADC12_SEQ_START_ADDR. 返回转换起始地址
     *
     */
    __STATIC_INLINE uint32_t DL_ADC12_getStartAddress(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL2 & ADC12_CTL2_STARTADD_MASK);
    }

    /**
     *  @brief Sets the end address for ADC conversion.
     *         设置 ADC 转换结束地址
     *
     *  @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                         外设寄存器基地址指针
     *  @param[in] endAdd      When ADC is initialized in sequence sampling  mode it
     *                         specifies the last memory control register in the
     *                         sequence. One of @ref DL_ADC12_SEQ_END_ADDR.
     *                         序列采样模式下的序列末地址
     *
     */
    __STATIC_INLINE void DL_ADC12_setEndAddress(ADC12_Regs *adc12, uint32_t endAdd)
    {
        DL_Common_updateReg(&adc12->ULLMEM.CTL2, endAdd, ADC12_CTL2_ENDADD_MASK);
    }

    /**
     *  @brief Gets end address for ADC conversion.
     *         获取 ADC 转换结束地址
     *
     *  @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                         外设寄存器基地址指针
     *
     *  @return One of @ref DL_ADC12_SEQ_END_ADDR. 返回转换结束地址
     *
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEndAddress(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL2 & ADC12_CTL2_ENDADD_MASK);
    }

    /**
     * @brief Initializes ADC12 for sequence sampling mode operation.
     *        初始化 ADC12 序列采样模式
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        外设寄存器基地址指针
     * @param[in] repeatMode  Specifies repeat configuration. One of
     *                        @ref DL_ADC12_REPEAT_MODE. 重复模式配置
     * @param[in] sampleMode  Specifies source of the sampling signal. One of
     *                        @ref DL_ADC12_SAMPLING_SOURCE. 采样信号源
     * @param[in] trigSrc     Specifies sampling trigger source. One of
     *                        @ref DL_ADC12_TRIG_SRC. 采样触发源
     * @param[in] startAdd    Specifies the starting address to sequence conversion.
     *                        One of @ref DL_ADC12_SEQ_START_ADDR. 序列起始地址
     * @param[in] endAdd      Specifies the ending address to sequence conversion.
     *                        One of @ref DL_ADC12_SEQ_END_ADDR. 序列结束地址
     * @param[in] resolution  Specifies sample conversion resolution. One of
     *                        @ref DL_ADC12_SAMP_CONV_RES. 转换分辨率
     * @param[in] dataFormat  Specifies sample conversion data format. One of
     *                        @ref DL_ADC12_SAMP_CONV_DATA_FORMAT. 转换数据格式
     */
    __STATIC_INLINE void DL_ADC12_initSeqSample(ADC12_Regs *adc12,
                                                DL_ADC12_REPEAT_MODE repeatMode, DL_ADC12_SAMPLING_SOURCE sampleMode,
                                                DL_ADC12_TRIG_SRC trigSrc, uint32_t startAdd, uint32_t endAdd,
                                                DL_ADC12_SAMP_CONV_RES resolution,
                                                DL_ADC12_SAMP_CONV_DATA_FORMAT dataFormat)
    {
        DL_Common_updateReg(&adc12->ULLMEM.CTL1,
                            (ADC12_CTL1_CONSEQ_SEQUENCE | repeatMode | sampleMode | trigSrc),
                            (ADC12_CTL1_SAMPMODE_MASK | ADC12_CTL1_CONSEQ_MASK |
                             ADC12_CTL1_TRIGSRC_MASK));

        DL_Common_updateReg(&adc12->ULLMEM.CTL2,
                            (startAdd | endAdd | resolution | dataFormat),
                            (ADC12_CTL2_ENDADD_MASK | ADC12_CTL2_STARTADD_MASK |
                             ADC12_CTL2_RES_MASK | ADC12_CTL2_DF_MASK));
    }

    /**
     * @brief Returns ADC12 resolution. 获取 ADC12 转换分辨率
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        外设寄存器基地址指针
     *
     * @return One of @ref DL_ADC12_SAMP_CONV_RES. 返回当前分辨率配置
     */
    __STATIC_INLINE uint32_t DL_ADC12_getResolution(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL2 & ADC12_CTL2_RES_MASK);
    }

    /**
     * @brief Returns ADC12 data format. 获取 ADC12 转换数据格式
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        外设寄存器基地址指针
     *
     * @return One of @ref DL_ADC12_SAMP_CONV_DATA_FORMAT. 返回当前数据格式配置
     */
    __STATIC_INLINE uint32_t DL_ADC12_getDataFormat(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL2 & ADC12_CTL2_DF_MASK);
    }

    /**
     * @brief  Returns ADC12 sampling source. 获取 ADC12 采样信号源
     *
     * @param[in] adc12   Pointer to the register overlay for the peripheral.
     *                    外设寄存器基地址指针
     *
     * @return  One of @ref DL_ADC12_SAMPLING_SOURCE. 返回当前采样信号源配置
     */
    __STATIC_INLINE uint32_t DL_ADC12_getSamplingSource(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL1 & ADC12_CTL1_SAMPMODE_MASK);
    }

    /**
     * @brief Returns ADC12 sampling mode. 获取 ADC12 采样模式
     *
     * @param[in] adc12   Pointer to the register overlay for the peripheral.
     *                    外设寄存器基地址指针
     *
     * @return One of @ref DL_ADC12_SAMP_MODE. 返回当前采样模式配置
     */
    __STATIC_INLINE uint32_t DL_ADC12_getSampleMode(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL1 & ADC12_CTL1_CONSEQ_MASK);
    }

    /**
     * @brief Returns ADC12 trigger mode. 获取 ADC12 触发源
     *
     * @param[in] adc12   Pointer to the register overlay for the peripheral.
     *                    外设寄存器基地址指针
     *
     * @return One of @ref DL_ADC12_TRIG_SRC. 返回当前触发源配置
     */
    __STATIC_INLINE DL_ADC12_TRIG_SRC DL_ADC12_getTriggerSource(
        const ADC12_Regs *adc12)
    {
        uint32_t trigSrc = adc12->ULLMEM.CTL1 & ADC12_CTL1_TRIGSRC_MASK;

        return (DL_ADC12_TRIG_SRC)(trigSrc);
    }

    /**
     * @brief Start ADC12 conversion. 启动 ADC12 转换
     *
     * @param[in] adc12  Pointer to the register overlay for the peripheral.
     *                   外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_startConversion(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL1 |= (ADC12_CTL1_SC_START);
    }

    /**
     * @brief Stop ADC12 conversion. 停止 ADC12 转换
     *
     * @param[in] adc12  Pointer to the register overlay for the peripheral.
     *                   外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_stopConversion(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL1 &= ~(ADC12_CTL1_SC_START);
    }

    /**
     * @brief Check if ADC12 conversion is started. 检查 ADC12 转换是否已启动
     *
     * @param[in] adc12  Pointer to the register overlay for the peripheral.
     *                   外设寄存器基地址指针
     *
     * @return If ADC12 conversion is started. 转换是否已启动
     *
     * @retval  true  ADC12 conversion is started. 转换已启动
     * @retval  false ADC12 conversion is stopped. 转换已停止
     */
    __STATIC_INLINE bool DL_ADC12_isConversionStarted(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.CTL1 & ADC12_CTL1_SC_MASK) == ADC12_CTL1_SC_START);
    }

    /**
     * @brief Enables DMA for data transfer. 使能 DMA 数据传输
     *
     * @param[in] adc12 Pointer to the register overlay for the peripheral.
     *                   外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_enableDMA(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL2 |= (ADC12_CTL2_DMAEN_ENABLE);
    }

    /**
     * @brief Disables DMA for data transfer. 禁用 DMA 数据传输
     *
     * @param[in] adc12 Pointer to the register overlay for the peripheral.
     *                   外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_disableDMA(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL2 &= ~(ADC12_CTL2_DMAEN_ENABLE);
    }

    /**
     * @brief Check if DMA is enabled. 检查 DMA 是否已使能
     *
     * @param[in] adc12  Pointer to the register overlay for the peripheral.
     *                   外设寄存器基地址指针
     *
     * @return If DMA is enabled. DMA 是否已使能
     *
     * @retval  true  DMA is enabled. DMA 已使能
     * @retval  false DMA is disabled. DMA 已禁用
     */
    __STATIC_INLINE bool DL_ADC12_isDMAEnabled(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.CTL2 & ADC12_CTL2_DMAEN_ENABLE) ==
                ADC12_CTL2_DMAEN_ENABLE);
    }

    /**
     * @brief Set number of ADC results to be transfer on a DMA trigger.
     *        设置每次 DMA 触发传输的 ADC 结果数量
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     * @param[in] sampCnt  Number of ADC results to be transfer on a DMA trigger.
     *                     Valid range 0 - 24. 每次 DMA 触发传输的结果数量，有效范围 0~24
     *
     */
    __STATIC_INLINE void DL_ADC12_setDMASamplesCnt(
        ADC12_Regs *adc12, uint8_t sampCnt)
    {
        DL_Common_updateReg(&adc12->ULLMEM.CTL2, (((uint32_t)sampCnt) << 11),
                            ADC12_CTL2_SAMPCNT_MASK);
    }

    /**
     * @brief Get number of ADC results to be transfer on a DMA trigger.
     *        获取每次 DMA 触发传输的 ADC 结果数量
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     * @return Number of ADC results to be transfer on a DMA trigger.
     *         返回每次 DMA 触发传输的结果数量
     */
    __STATIC_INLINE uint8_t DL_ADC12_getDMASampleCnt(const ADC12_Regs *adc12)
    {
        return (uint8_t)((adc12->ULLMEM.CTL2 & ADC12_CTL2_SAMPCNT_MASK) >> 11);
    }

    /**
     * @brief Enables FIFO mode. 使能 FIFO 模式
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_enableFIFO(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL2 |= (ADC12_CTL2_FIFOEN_ENABLE);
    }

    /**
     * @brief Disables FIFO mode. 禁用 FIFO 模式
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     */
    __STATIC_INLINE void DL_ADC12_disableFIFO(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL2 &= ~(ADC12_CTL2_FIFOEN_ENABLE);
    }

    /**
     * @brief Checks if FIFO mode is enabled. 检查 FIFO 模式是否已使能
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     * @return If FIFO mode is enabled. FIFO 模式是否已使能
     *
     * @retval  true  FIFO mode is enabled. FIFO 模式已使能
     * @retval  false FIFO mode is disabled. FIFO 模式已禁用
     *
     */
    __STATIC_INLINE bool DL_ADC12_isFIFOEnabled(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.CTL2 & ADC12_CTL2_FIFOEN_MASK) ==
                ADC12_CTL2_FIFOEN_ENABLE);
    }

    /**
     * @brief Configures ADC12 sample clock divider and sample clock frequency range.
     *        配置 ADC12 采样时钟来源、分频和频段范围
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        ADC12 外设寄存器基地址指针
     * @param[in] config      Pointer to the clock configuration struct
     *                        @ref DL_ADC12_ClockConfig. 时钟配置结构体指针
     */
    void DL_ADC12_setClockConfig(
        ADC12_Regs *adc12, const DL_ADC12_ClockConfig *config);

    /**
     * @brief Returns ADC12 sample clock configuration.
     *        读取当前 ADC12 采样时钟配置并回填到结构体
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        ADC12 外设寄存器基地址指针
     * @param[in] config      Pointer to the clock configuration struct
     *                        @ref DL_ADC12_ClockConfig. 用于接收结果的时钟配置结构体指针
     */
    void DL_ADC12_getClockConfig(
        const ADC12_Regs *adc12, DL_ADC12_ClockConfig *config);

    /**
     * @brief Configures ADC12 power down mode. 配置 ADC12 掉电模式
     *
     * @param[in] adc12         Pointer to the register overlay for the peripheral.
     *                          外设寄存器基地址指针
     * @param[in] powerDownMode Specifies the power down mode. One of
     *                          @ref DL_ADC12_POWER_DOWN_MODE. 掉电模式选择
     */
    __STATIC_INLINE void DL_ADC12_setPowerDownMode(
        ADC12_Regs *adc12, uint32_t powerDownMode)
    {
        DL_Common_updateReg(
            &adc12->ULLMEM.CTL0, powerDownMode, ADC12_CTL0_PWRDN_MASK);
    }

    /**
     * @brief Returns ADC power down mode. 获取 ADC 掉电模式
     *
     * @param[in] adc12         Pointer to the register overlay for the peripheral.
     *                          外设寄存器基地址指针
     *
     * @return  The current ADC12 power down mode. 当前 ADC12 掉电模式
     *
     * @retval  One of @ref DL_ADC12_POWER_DOWN_MODE. 返回掉电模式配置值
     */
    __STATIC_INLINE uint32_t DL_ADC12_getPowerDownMode(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.CTL0 & ADC12_CTL0_PWRDN_MASK);
    }

    /**
     *  @brief      Enable ADC12 conversion. 使能 ADC12 转换
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_enableConversions(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL0 |= (ADC12_CTL0_ENC_ON);
    }

    /**
     *  @brief      Disable ADC12 conversion. 禁用 ADC12 转换
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_disableConversions(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL0 &= ~(ADC12_CTL0_ENC_ON);
    }

    /**
     *  @brief      Check if ADC12 conversion is enabled. 检查 ADC12 转换是否已使能
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @return     If the ADC12 conversion is enabled. ADC12 转换是否已使能
     *
     *  @retval     true  The ADC12 conversion is enabled. 转换已使能
     *  @retval     false The ADC12 conversion is disabled. 转换已禁用
     *
     */
    __STATIC_INLINE bool DL_ADC12_isConversionsEnabled(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.CTL0 & ADC12_CTL0_ENC_MASK) == ADC12_CTL0_ENC_ON);
    }

    /**
     * @brief Configure ADC12 hardware average. 配置 ADC12 硬件均值滤波
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        外设寄存器基地址指针
     * @param[in] numerator   Specifies the number of conversion to accumulate. One
     *                        of @ref DL_ADC12_HW_AVG_NUM. 累加转换次数
     * @param[in] denominator Specifies the number to divide the accumulated value
     *                        by. One of @ref DL_ADC12_HW_AVG_DEN. 累加分母
     */
    __STATIC_INLINE void DL_ADC12_configHwAverage(
        ADC12_Regs *adc12, uint32_t numerator, uint32_t denominator)
    {
        DL_Common_updateReg(&adc12->ULLMEM.CTL1, (numerator | denominator),
                            (ADC12_CTL1_AVGN_MASK | ADC12_CTL1_AVGD_MASK));
    }

    /**
     * @brief Return the hardware average configuration. 获取硬件均值滤波配置
     *
     * @param[in] adc12       Pointer to the register overlay for the peripheral.
     *                        外设寄存器基地址指针
     *
     * @return Bitwise OR of @ref DL_ADC12_HW_AVG_NUM and @ref DL_ADC12_HW_AVG_DEN.
     *         返回累加次数与分母的按位或值
     */
    __STATIC_INLINE uint32_t DL_ADC12_getHwAverageConfig(const ADC12_Regs *adc12)
    {
        return (
            adc12->ULLMEM.CTL1 & (ADC12_CTL1_AVGN_MASK | ADC12_CTL1_AVGD_MASK));
    }

    /**
     *  @brief      Set sample time 0. 设置采样时间 0
     *
     *  @param[in]  adc12    Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     *  @param[in]  adcclks  The sample time in number of ADCCLK cycles.
     *                       以 ADCCLK 周期数表示的采样时间
     *
     * Actual sample time is calculated as follows:
     *
     * If adcclks (SCOMP register) = 0 or 1, then:
     *     number of sampling clocks = sample clock divide value
     *                                 (actual divide value of SCLKDIV register)
     * Ex. If adcclks = 0, and SCLKDIV = 2 (implies divide value of 4), then:
     *     the total number of sampling clocks is 4.
     *
     * If adcclks (SCOMP register) >= 1, then:
     *     number of sampling clocks = adcclks * sample clock divide value
     *                                           (actual divide value of SCLKDIV)
     * Ex. If adcclks = 5, and SCLKDIV = 6 (implies divide value of 16), then:
     *     5 * 16 = 80 sampling clocks.
     *
     */
    __STATIC_INLINE void DL_ADC12_setSampleTime0(
        ADC12_Regs *adc12, uint16_t adcclks)
    {
        adc12->ULLMEM.SCOMP0 = (adcclks);
    }

    /**
     * @brief Get sample time 0. 获取采样时间 0
     *
     * @param[in]   adc12   Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     *
     * @return Sample time 0 in ADCCLKS. 以 ADCCLK 周期数返回的采样时间 0
     */
    __STATIC_INLINE uint16_t DL_ADC12_getSampleTime0(const ADC12_Regs *adc12)
    {
        return (uint16_t)(adc12->ULLMEM.SCOMP0 + (uint32_t)1);
    }

    /**
     *  @brief      Set sample time 1. 设置采样时间 1
     *
     *  @param[in]  adc12    Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     *  @param[in]  adcclks  Specifies the sample time in number of ADCCLK cycles.
     *                       Actual sample time is (adcclks + 1).
     *                       以 ADCCLK 周期数表示的采样时间，实际时间为 adcclks + 1
     *
     */
    __STATIC_INLINE void DL_ADC12_setSampleTime1(
        ADC12_Regs *adc12, uint16_t adcclks)
    {
        adc12->ULLMEM.SCOMP1 = (adcclks);
    }

    /**
     * @brief Get sample time 1. 获取采样时间 1
     *
     * @param[in]   adc12   Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     *
     * @return Sample time 1 in ADCCLKS. 以 ADCCLK 周期数返回的采样时间 1
     */
    __STATIC_INLINE uint16_t DL_ADC12_getSampleTime1(const ADC12_Regs *adc12)
    {
        return (uint16_t)(adc12->ULLMEM.SCOMP1 + (uint32_t)1);
    }

    /**
     * @brief Configures window comparator low threshold.
     *        配置窗口比较器低阈值
     *
     * @param[in] adc12          Pointer to the register overlay for the peripheral.
     *                           外设寄存器基地址指针
     * @param[in] threshold      Window comparator low threshold value. Threshold
     *                           value must take into account result data format and
     *                           resolution configured via
     *                           @ref DL_ADC12_initSingleSample or
     *                           @ref DL_ADC12_initSeqSample.
     *                           窗口比较器低阈值，需考虑数据格式和分辨率
     */
    __STATIC_INLINE void DL_ADC12_configWinCompLowThld(
        ADC12_Regs *adc12, uint16_t threshold)
    {
        adc12->ULLMEM.WCLOW = (threshold);
    }

    /**
     * @brief Configures window comparator high threshold.
     *        配置窗口比较器高阈值
     *
     * @param[in] adc12          Pointer to the register overlay for the peripheral.
     *                           外设寄存器基地址指针
     * @param[in] threshold      Window comparator high threshold value. Threshold
     *                           value must take into account result data format and
     *                           resolution configured via
     *                           @ref DL_ADC12_initSingleSample or
     *                           @ref DL_ADC12_initSeqSample.
     *                           窗口比较器高阈值，需考虑数据格式和分辨率
     */
    __STATIC_INLINE void DL_ADC12_configWinCompHighThld(
        ADC12_Regs *adc12, uint16_t threshold)
    {
        adc12->ULLMEM.WCHIGH = (threshold);
    }

    /**
     * @brief Returns the data from the top of FIFO. 读取 FIFO 栈顶数据
     *
     * @param[in] adc12         Pointer to the register overlay for the peripheral.
     *                          外设寄存器基地址指针
     *
     * @return Data from the top of FIFO. FIFO 栈顶数据
     */
    __STATIC_INLINE uint32_t DL_ADC12_getFIFOData(const ADC12_Regs *adc12)
    {
        volatile const uint32_t *pReg = &adc12->ULLMEM.FIFODATA;

        return (uint32_t)(*(pReg + DL_ADC12_SVT_OFFSET));
    }

    /**
     * @brief Returns the address of FIFO data register. 获取 FIFO 数据寄存器地址
     *
     * @param[in] adc12         Pointer to the register overlay for the peripheral.
     *                          外设寄存器基地址指针
     *
     * @return Address of FIFO data register. FIFO 数据寄存器地址
     */
    __STATIC_INLINE uint32_t DL_ADC12_getFIFOAddress(const ADC12_Regs *adc12)
    {
        return ((uint32_t)(&adc12->ULLMEM.FIFODATA + DL_ADC12_SVT_OFFSET));
    }

    /**
     * @brief Configures conversion memory. 配置转换内存控制寄存器
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     * @param[in] idx      Memory conversion index. @ref DL_ADC12_MEM_IDX.
     *                     内存转换索引
     * @param[in] chansel  Selects input channel. One of @ref DL_ADC12_INPUT_CHAN.
     *                     输入通道选择
     * @param[in] vref     Selects reference voltage. One of
     *                 @ref DL_ADC12_REFERENCE_VOLTAGE. 参考电压选择
     * @param[in] stime    Selects source of sample adc12 period. One of
     *                 @ref DL_ADC12_SAMPLE_TIMER_SOURCE. 采样定时器源选择
     * @param[in] avgen    Selects averaging mode. One of
     *                 @ref DL_ADC12_AVERAGING_MODE. 均值模式选择
     * @param[in] bcsen    Selects burn out current source selection. One of
     *                 @ref DL_ADC12_BURN_OUT_SOURCE. 烧毁电流源选择
     * @param[in] trig     Selects trigger mode. One of
     *                 @ref DL_ADC12_TRIGGER_MODE. 触发模式选择
     * @param[in] wincomp  Selects window comparator mode. One of
     *                 @ref DL_ADC12_WINDOWS_COMP_MODE. 窗口比较器模式选择
     */
    __STATIC_INLINE void DL_ADC12_configConversionMem(ADC12_Regs *adc12,
                                                      DL_ADC12_MEM_IDX idx, uint32_t chansel, uint32_t vref, uint32_t stime,
                                                      uint32_t avgen, uint32_t bcsen, uint32_t trig, uint32_t wincomp)
    {
        adc12->ULLMEM.MEMCTL[idx] =
            (chansel | vref | stime | avgen | bcsen | trig | wincomp);
    }

    /**
     * @brief Returns conversion memory configuration.
     *        获取转换内存控制寄存器配置
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     * @param[in] idx      Memory conversion index. @ref DL_ADC12_MEM_IDX.
     *                     内存转换索引
     * @return Bitwise OR of @ref DL_ADC12_INPUT_CHAN,
     *         @ref DL_ADC12_REFERENCE_VOLTAGE, @ref DL_ADC12_SAMPLE_TIMER_SOURCE,
     *         @ref DL_ADC12_AVERAGING_MODE, @ref DL_ADC12_BURN_OUT_SOURCE,
     *         @ref DL_ADC12_TRIGGER_MODE, @ref DL_ADC12_WINDOWS_COMP_MODE.
     *         返回各配置项的按位或值
     *
     */
    __STATIC_INLINE uint32_t DL_ADC12_getConversionMemConfig(
        const ADC12_Regs *adc12, DL_ADC12_MEM_IDX idx)
    {
        return (adc12->ULLMEM.MEMCTL[idx]);
    }

    /**
     * @brief Returns the conversion result for the selected memory index.
     *        获取指定内存索引的转换结果
     *
     * @param[in] adc12     Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     * @param[in] idx       Memory conversion index. @ref DL_ADC12_MEM_IDX.
     *                      内存转换索引
     *
     * @return Conversion result. 转换结果
     *
     */
    __STATIC_INLINE uint16_t DL_ADC12_getMemResult(
        const ADC12_Regs *adc12, DL_ADC12_MEM_IDX idx)
    {
        volatile const uint32_t *pReg = &adc12->ULLMEM.MEMRES[idx];

        return (uint16_t)(*(pReg + DL_ADC12_SVT_OFFSET));
    }

    /**
     * @brief Returns the conversion result memory address.
     *        获取转换结果内存地址
     *
     * @param[in] adc12     Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     * @param[in] idx       Memory conversion index. @ref DL_ADC12_MEM_IDX.
     *                      内存转换索引
     *
     * @return Conversion result memory address. 转换结果内存地址
     *
     */
    __STATIC_INLINE uint32_t DL_ADC12_getMemResultAddress(
        const ADC12_Regs *adc12, DL_ADC12_MEM_IDX idx)
    {
        return ((uint32_t)(&adc12->ULLMEM.MEMRES[idx] + DL_ADC12_SVT_OFFSET));
    }

    /**
     * @brief Returns ADC12 status. 获取 ADC12 状态
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     * @return Bitwise OR of @ref DL_ADC12_STATUS_CONVERSION and
     *         @ref DL_ADC12_STATUS_REFERENCE. 转换状态与参考状态的按位或值
     *
     */
    __STATIC_INLINE uint32_t DL_ADC12_getStatus(const ADC12_Regs *adc12)
    {
        return (adc12->ULLMEM.STATUS);
    }

    /**
     * @brief Allows SYSOSC to not run at base frequency when device is in RUN mode.
     *        允许 SYSOSC 在 RUN 模式下不以基频运行
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *
     */
    __STATIC_INLINE void DL_ADC12_disableForcingSYSOSCOnInRunMode(
        ADC12_Regs *adc12)
    {
        DL_Common_updateReg(&adc12->ULLMEM.GPRCM.CLKCFG,
                            (ADC12_CLKCFG_CCONRUN_DISABLE | ADC12_CLKCFG_KEY_UNLOCK_W),
                            (ADC12_CLKCFG_CCONRUN_MASK | ADC12_CLKCFG_KEY_MASK));
    }

    /**
     * @brief Forces SYSOSC to run at base frequency when device is in RUN mode.
     *        强制 SYSOSC 在 RUN 模式下以基频运行
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *
     */
    __STATIC_INLINE void DL_ADC12_forceSYSOSCOnInRunMode(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.GPRCM.CLKCFG |=
            (ADC12_CLKCFG_KEY_UNLOCK_W | ADC12_CLKCFG_CCONRUN_ENABLE);
    }

    /**
     * @brief Allows SYSOSC to not run at base frequency when device is in STOP mode.
     *        允许 SYSOSC 在 STOP 模式下不以基频运行
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *
     */
    __STATIC_INLINE void DL_ADC12_disableForcingSYSOSCOnInStopMode(
        ADC12_Regs *adc12)
    {
        DL_Common_updateReg(&adc12->ULLMEM.GPRCM.CLKCFG,
                            (ADC12_CLKCFG_CCONSTOP_DISABLE | ADC12_CLKCFG_KEY_UNLOCK_W),
                            (ADC12_CLKCFG_CCONSTOP_MASK | ADC12_CLKCFG_KEY_MASK));
    }

    /**
     * @brief Forces SYSOSC to run at base frequency when device is in STOP mode.
     *        强制 SYSOSC 在 STOP 模式下以基频运行
     *
     * @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *
     */
    __STATIC_INLINE void DL_ADC12_forceSYSOSCOnInStopMode(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.GPRCM.CLKCFG |=
            (ADC12_CLKCFG_KEY_UNLOCK_W | ADC12_CLKCFG_CCONSTOP_ENABLE);
    }

    /**
     *  @brief      Enable ADC12 interrupt. 使能 ADC12 中断
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_ADC12_INTERRUPTS. 中断掩码
     */
    __STATIC_INLINE void DL_ADC12_enableInterrupt(
        ADC12_Regs *adc12, uint32_t interruptMask)
    {
        adc12->ULLMEM.CPU_INT.IMASK |= (interruptMask);
    }

    /**
     *  @brief      Disable ADC12 interrupt. 禁用 ADC12 中断
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_ADC12_INTERRUPTS. 中断掩码
     *
     */
    __STATIC_INLINE void DL_ADC12_disableInterrupt(
        ADC12_Regs *adc12, uint32_t interruptMask)
    {
        adc12->ULLMEM.CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which ADC12 interrupts are enabled.
     *              查询哪些 ADC12 中断已使能
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_ADC12_INTERRUPTS. 中断掩码
     *
     *  @return     Which of the requested ADC12 interrupts are enabled.
     *              返回已使能的中断位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_INTERRUPTS values
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEnabledInterrupts(
        const ADC12_Regs *adc12, uint32_t interruptMask)
    {
        return (adc12->ULLMEM.CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled ADC12 interrupt.
     *              查询已使能的 ADC12 中断是否挂起
     *
     *  Checks if the ADC12 interrupt that was previously enabled is pending.
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_ADC12_INTERRUPTS. 中断掩码
     *
     *  @return     If the enabled ADC12 interrupt is pending. 已使能的中断是否挂起
     *
     *  @sa         DL_ADC12_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEnabledInterruptStatus(
        const ADC12_Regs *adc12, uint32_t interruptMask)
    {
        return (adc12->ULLMEM.CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any ADC12 interrupt.
     *              查询任意 ADC12 中断的原始标志
     *
     *  Checks if the ADC12 interrupt is pending. Interrupt does not have to
     *  be previously enabled.
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_ADC12_INTERRUPTS. 中断掩码
     *
     *  @return     If the ADC12 interrupt is pending. 中断是否挂起
     *
     */
    __STATIC_INLINE uint32_t DL_ADC12_getRawInterruptStatus(
        const ADC12_Regs *adc12, uint32_t interruptMask)
    {
        return (adc12->ULLMEM.CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending ADC12 interrupt.
     *              获取最高优先级的待处理 ADC12 中断
     *
     *  Checks if any of the ADC12 interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @return     The highest priority pending ADC12 interrupt.
     *              最高优先级的待处理中断索引
     *
     *  @retval     One of @ref DL_ADC12_IIDX
     */
    __STATIC_INLINE DL_ADC12_IIDX DL_ADC12_getPendingInterrupt(
        const ADC12_Regs *adc12)
    {
        return ((DL_ADC12_IIDX)adc12->ULLMEM.CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending ADC12 interrupt. 清除待处理的 ADC12 中断
     *
     *  @param[in]  adc12  Pointer to the register overlay for the peripheral.
     *                     外设寄存器基地址指针
     *
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_ADC12_INTERRUPTS. 中断掩码
     *
     */
    __STATIC_INLINE void DL_ADC12_clearInterruptStatus(
        ADC12_Regs *adc12, uint32_t interruptMask)
    {
        adc12->ULLMEM.CPU_INT.ICLR |= (interruptMask);
    }

    /**
     *  @brief Sets the event publisher channel id. 设置事件发布者通道 ID
     *
     *  @param[in]  adc12 Pointer to the register overlay for the
     *                      peripheral. 外设寄存器基地址指针
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
     *                      publisher is disconnected. 通道 ID，有效范围 0~15，
     *                      为 0 时断开发布者
     */
    __STATIC_INLINE void DL_ADC12_setPublisherChanID(
        ADC12_Regs *adc12, uint8_t chanID)
    {
        adc12->ULLMEM.FPUB_1 = (chanID & ADC12_FPUB_1_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event publisher channel id. 获取事件发布者通道 ID
     *
     *  @param[in]  adc12 Pointer to the register overlay for the
     *                      peripheral. 外设寄存器基地址指针
     *
     *  @return     Event publisher channel ID. 事件发布者通道 ID
     *
     */
    __STATIC_INLINE uint8_t DL_ADC12_getPublisherChanID(const ADC12_Regs *adc12)
    {
        return (uint8_t)(adc12->ULLMEM.FPUB_1 & ADC12_FPUB_1_CHANID_MAXIMUM);
    }

    /**
     *  @brief Sets the event subscriber channel id. 设置事件订阅者通道 ID
     *
     *  @param[in]  adc12 Pointer to the register overlay for the
     *                      peripheral. 外设寄存器基地址指针
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
     *                      subscriber is disconnected. 通道 ID，有效范围 0~15，
     *                      为 0 时断开订阅者
     */
    __STATIC_INLINE void DL_ADC12_setSubscriberChanID(
        ADC12_Regs *adc12, uint8_t chanID)
    {
        adc12->ULLMEM.FSUB_0 = (chanID & ADC12_FSUB_0_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event subscriber channel id. 获取事件订阅者通道 ID
     *
     *  @param[in]  adc12 Pointer to the register overlay for the
     *                      peripheral. 外设寄存器基地址指针
     *
     *  @return     Event subscriber channel ID. 事件订阅者通道 ID
     *
     */
    __STATIC_INLINE uint8_t DL_ADC12_getSubscriberChanID(const ADC12_Regs *adc12)
    {
        return (uint8_t)(adc12->ULLMEM.FSUB_0 & ADC12_FSUB_0_CHANID_MAXIMUM);
    }

    /**
     *  @brief      Enable ADC12 event. 使能 ADC12 事件
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  eventMask      Bit mask of events to enable. Bitwise OR of
     *                             @ref DL_ADC12_EVENT. 事件掩码
     */
    __STATIC_INLINE void DL_ADC12_enableEvent(
        ADC12_Regs *adc12, uint32_t eventMask)
    {
        adc12->ULLMEM.GEN_EVENT.IMASK |= (eventMask);
    }

    /**
     *  @brief      Disable ADC12 event. 禁用 ADC12 事件
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  eventMask      Bit mask of events to disable. Bitwise OR of
     *                             @ref DL_ADC12_EVENT. 事件掩码
     */
    __STATIC_INLINE void DL_ADC12_disableEvent(
        ADC12_Regs *adc12, uint32_t eventMask)
    {
        adc12->ULLMEM.GEN_EVENT.IMASK &= ~(eventMask);
    }

    /**
     *  @brief      Check which ADC12 events are enabled.
     *              查询哪些 ADC12 事件已使能
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  eventMask      Bit mask of events to check. Bitwise OR of
     *                             @ref DL_ADC12_EVENT. 事件掩码
     *
     *  @return     Which of the requested adc12 events are enabled.
     *              返回已使能的事件位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_EVENT values
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEnabledEvents(
        const ADC12_Regs *adc12, uint32_t eventMask)
    {
        return (adc12->ULLMEM.GEN_EVENT.IMASK & eventMask);
    }

    /**
     *  @brief      Check event flag of enabled adc12 event.
     *              查询已使能的 ADC12 事件是否挂起
     *
     *  Checks if any of the adc12 events that were previously enabled are
     *  pending.
     *
     *  @param[in]  adc12        Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  eventMask      Bit mask of events to check. Bitwise OR of
     *                             @ref DL_ADC12_EVENT. 事件掩码
     *
     *  @return     Which of the requested adc12 events are pending.
     *              返回挂起的事件位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_EVENT values
     *
     *  @sa         DL_ADC12_enableEvent
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEnabledEventStatus(
        const ADC12_Regs *adc12, uint32_t eventMask)
    {
        return (adc12->ULLMEM.GEN_EVENT.MIS & ~(eventMask));
    }

    /**
     *  @brief      Check event flag of any adc12 event.
     *              查询任意 ADC12 事件的原始标志
     *
     *  Checks if any events are pending. Events do not have to
     *  be previously enabled.
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  eventMask      Bit mask of event to check. Bitwise OR of
     *                             @ref DL_ADC12_EVENT. 事件掩码
     *
     *  @return     Which of the requested adc12 event are pending.
     *              返回挂起的事件位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_EVENT values
     */
    __STATIC_INLINE uint32_t DL_ADC12_getRawEventsStatus(
        const ADC12_Regs *adc12, uint32_t eventMask)
    {
        return (adc12->ULLMEM.GEN_EVENT.RIS & ~(eventMask));
    }

    /**
     *  @brief      Clear pending adc12 events. 清除待处理的 ADC12 事件
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  eventMask      Bit mask of events to clear. Bitwise OR of
     *                             @ref DL_ADC12_EVENT. 事件掩码
     */
    __STATIC_INLINE void DL_ADC12_clearEventsStatus(
        ADC12_Regs *adc12, uint32_t eventMask)
    {
        adc12->ULLMEM.GEN_EVENT.ICLR |= (eventMask);
    }

    /**
     *  @brief      Enable ADC12 DMA triggers. 使能 ADC12 DMA 触发
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  dmaMask      Bit mask of DMA triggers to enable. Bitwise OR of
     *                             @ref DL_ADC12_DMA. DMA 触发掩码
     */
    __STATIC_INLINE void DL_ADC12_enableDMATrigger(
        ADC12_Regs *adc12, uint32_t dmaMask)
    {
        adc12->ULLMEM.DMA_TRIG.IMASK |= (dmaMask);
    }

    /**
     *  @brief      Disable ADC12 DMA triggers. 禁用 ADC12 DMA 触发
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  dmaMask        Bit mask of DMA triggers to disable. Bitwise OR of
     *                             @ref DL_ADC12_DMA. DMA 触发掩码
     */
    __STATIC_INLINE void DL_ADC12_disableDMATrigger(
        ADC12_Regs *adc12, uint32_t dmaMask)
    {
        adc12->ULLMEM.DMA_TRIG.IMASK &= ~(dmaMask);
    }

    /**
     *  @brief      Check which adc12 DMA triggers are enabled.
     *              查询哪些 ADC12 DMA 触发已使能
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  dmaMask        Bit mask of DMA triggers to check. Bitwise OR of
     *                             @ref DL_ADC12_DMA. DMA 触发掩码
     *
     *  @return     Which of the requested adc12 DMA triggers are enabled.
     *              返回已使能的 DMA 触发位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_DMA values
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEnabledDMATrigger(
        const ADC12_Regs *adc12, uint32_t dmaMask)
    {
        return (adc12->ULLMEM.DMA_TRIG.IMASK & dmaMask);
    }

    /**
     *  @brief      Check event flag of enabled adc12 DMA triggers.
     *              查询已使能的 ADC12 DMA 触发是否挂起
     *
     *  Checks if any of the adc12 DMA triggers that were previously enabled are
     *  pending.
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  dmaMask      Bit mask of DMA triggers to check. Bitwise OR of
     *                             @ref DL_ADC12_DMA. DMA 触发掩码
     *
     *  @return     Which of the requested adc12 events are pending.
     *              返回挂起的 DMA 触发位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_DMA values
     *
     *  @sa         DL_ADC12_enableDMATrigger
     */
    __STATIC_INLINE uint32_t DL_ADC12_getEnabledDMATriggerStatus(
        const ADC12_Regs *adc12, uint32_t dmaMask)
    {
        return (adc12->ULLMEM.DMA_TRIG.MIS & ~(dmaMask));
    }

    /**
     *  @brief      Check DMA triggers flag of any adc12 dma trigger.
     *              查询任意 ADC12 DMA 触发的原始标志
     *
     *  Checks if any dma trigger are pending. DMA triggers do not have to
     *  be previously enabled.
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  dmaMask        Bit mask of DMA triggers to check. Bitwise OR of
     *                             @ref DL_ADC12_DMA. DMA 触发掩码
     *
     *  @return     Which of the requested adc12 dma triggers are pending.
     *              返回挂起的 DMA 触发位
     *
     *  @retval     Bitwise OR of @ref DL_ADC12_DMA values
     */
    __STATIC_INLINE uint32_t DL_ADC12_getRawDMATriggerStatus(
        const ADC12_Regs *adc12, uint32_t dmaMask)
    {
        return (adc12->ULLMEM.DMA_TRIG.RIS & ~(dmaMask));
    }

    /**
     *  @brief      Clear pending adc12 DMA triggers. 清除待处理的 ADC12 DMA 触发
     *
     *  @param[in]  adc12          Pointer to the register overlay for the
     *                             peripheral. 外设寄存器基地址指针
     *  @param[in]  dmaMask        Bit mask of DMA triggers to clear. Bitwise OR of
     *                             @ref DL_ADC12_DMA. DMA 触发掩码
     */
    __STATIC_INLINE void DL_ADC12_clearDMATriggerStatus(
        ADC12_Regs *adc12, uint32_t dmaMask)
    {
        adc12->ULLMEM.DMA_TRIG.ICLR |= (dmaMask);
    }

#ifdef __MSPM0_HAS_ADC12_SH_CAP_DISCH__
    /**
     *  @brief Enables sample and hold capacitor discharge.
     *         使能采样保持电容放电
     *
     *  Sample and hold capacitor is discharged at end of conversion.
     *
     *  @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     */
    __STATIC_INLINE void DL_ADC12_enableSAMPCAP(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL2 |= (ADC12_CTL2_RSTSAMPCAPEN_ENABLE);
    }

    /**
     *  @brief Disables sample and hold capacitor discharge.
     *         禁用采样保持电容放电
     *
     *  Sample and hold capacitor is discharged at end of conversion. This incurs one additional clock cycle.
     *
     *  @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     *
     */
    __STATIC_INLINE void DL_ADC12_disableSAMPCAP(ADC12_Regs *adc12)
    {
        adc12->ULLMEM.CTL2 &= ~(ADC12_CTL2_RSTSAMPCAPEN_ENABLE);
    }

    /**
     *  @brief Checks if sample and hold capacitor discharge is enabled.
     *         检查采样保持电容放电是否已使能
     *
     *  @param[in] adc12    Pointer to the register overlay for the peripheral.
     *                      外设寄存器基地址指针
     *
     *  @return If sample and hold capacitor discharge enabled.
     *          采样保持电容放电是否已使能
     *
     *  @retval  true  sample and hold capacitor discharge is enabled. 已使能
     *  @retval  false sample and hold capacitor discharge is disabled. 已禁用
     *
     */
    __STATIC_INLINE bool DL_ADC12_isSAMPCAPEnabled(const ADC12_Regs *adc12)
    {
        return ((adc12->ULLMEM.CTL2 & ADC12_CTL2_RSTSAMPCAPEN_MASK) ==
                ADC12_CTL2_RSTSAMPCAPEN_ENABLE);
    }
#endif /* ADC HAS SAMPLE AND HOLD CAPACITOR DISCHARGE*/

#ifdef __MSPM0C110X_ADC_ERR_06__
    /**
     *  @brief Get calibration ADC offset value.
     *         获取 ADC 校准偏移值
     *
     *  Workaround for errata on MSPM0C110x devices: ADC_ERR_06
     *
     *  Offset needs to be calibrated each time @ref DL_ADC12_configConversionMem
     *  is configured with a new voltage reference.
     *
     *  User needs to add calibrated offset to the result of @ref DL_ADC12_getMemResult
     *  to get correct ADC value.
     *
     *  @param[in] userRef  Reference voltage. 用户参考电压
     *
     *  @return Calibrated ADC offset value. 校准后的 ADC 偏移值
     */
    __STATIC_INLINE int16_t DL_ADC12_getADCOffsetCalibration(float userRef)
    {
        float adcBuff = DL_FactoryRegion_getADCOffset() * (3.3 / userRef);
        return (int16_t)(round(adcBuff));
    }
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_ADC12__ */

#endif /* ti_dl_dl_adc12__include */
/** @}*/
