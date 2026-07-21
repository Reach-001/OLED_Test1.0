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
 *  @file dl_dac12.h
 *  @brief 12-bit DAC Driver Library 12位数模转换器驱动库
 *  @defgroup   DAC12 Digital to Analog Converter (DAC12)
 *
 *  @anchor ti_dl_dl_dac12_Overview
 *  # Overview
 *
 *  The Digital to Analog Converter Driver Library allows full configuration of
 *  the MSPM0 DAC12 module. The DAC module is a 12-bit voltage-output
 *  digital-to-analog converter (DAC).
 *
 *  <hr>
 ******************************************************************************
 *  @par 中文文件说明
 *  MSPM0G3507 12位 DAC 驱动库头文件。
 *  提供 DAC12 模块的初始化、输出控制、FIFO 管理、DMA 触发、
 *  采样定时器、校准、中断/事件等完整 API 声明。
 */
/** @addtogroup DAC12
 * @{
 */
#ifndef ti_dl_dl_dac12__include
#define ti_dl_dl_dac12__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_DAC12__

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

/** @enum DL_DAC12_OUTPUT DAC 输出引脚连接配置 */
typedef enum {
    /*!  DAC output is disconnected from output pin DAC 输出与引脚断开 */
    DL_DAC12_OUTPUT_DISABLED = DAC12_CTL1_OPS_NOC0,
    /*!  DAC output is available on output pin DAC 输出连接到输出引脚 */
    DL_DAC12_OUTPUT_ENABLED = DAC12_CTL1_OPS_OUT0,
} DL_DAC12_OUTPUT;

/** @enum DL_DAC12_REPRESENTATION 输入数据格式选择 */
typedef enum {
    /*!  Binary representation 二进制格式 */
    DL_DAC12_REPRESENTATION_BINARY = DAC12_CTL0_DFM_BINARY,
    /*!  Two's complement representation 二补码格式 */
    DL_DAC12_REPRESENTATION_TWOS_COMPLEMENT = DAC12_CTL0_DFM_TWOS_COMP,
} DL_DAC12_REPRESENTATION;

/** @enum DL_DAC12_RESOLUTION 输入数据分辨率 */
typedef enum {
    /*!  12-bit input resolution 12位输入分辨率 */
    DL_DAC12_RESOLUTION_12BIT = DAC12_CTL0_RES__12BITS,
    /*!  8-bit input resolution 8位输入分辨率 */
    DL_DAC12_RESOLUTION_8BIT = DAC12_CTL0_RES__8BITS,
} DL_DAC12_RESOLUTION;

/** @enum DL_DAC12_AMP 输出放大器工作模式 */
typedef enum {
    /*!  Output amplifier off with high impedance tristate output 放大器关闭，输出高阻三态 */
    DL_DAC12_AMP_OFF_TRISTATE = DAC12_CTL1_AMPHIZ_HIZ,
    /*!  Output amplifier off with output pulled to ground 放大器关闭，输出拉低至地 */
    DL_DAC12_AMP_OFF_0V = DAC12_CTL1_AMPHIZ_PULLDOWN,
    /*!  Output amplifier enabled 放大器开启 */
    DL_DAC12_AMP_ON = DAC12_CTL1_AMPEN_ENABLE,
} DL_DAC12_AMP;

/** @enum DL_DAC12_VREF_SOURCE 正负参考电压源选择 */
typedef enum {
    /*! Positive reference is VDDA, negative reference is VeREF- 正参考VDDA，负参考VeREF- */
    DL_DAC12_VREF_SOURCE_VDDA_VEREFN = (DAC12_CTL1_REFSP_VDDA |
                                        DAC12_CTL1_REFSN_VEREFN) ,
    /*! Positive reference is VeREF+, negative reference is VeREF- 正参考VeREF+，负参考VeREF- */
    DL_DAC12_VREF_SOURCE_VEREFP_VEREFN = (DAC12_CTL1_REFSP_VEREFP |
                                          DAC12_CTL1_REFSN_VEREFN),
    /*! Positive reference is VDDA, negative reference is VSSA 正参考VDDA，负参考VSSA */
    DL_DAC12_VREF_SOURCE_VDDA_VSSA = (DAC12_CTL1_REFSP_VDDA |
                                      DAC12_CTL1_REFSN_VSSA),
    /*! Positive reference is VeREF+, negative reference is VSSA 正参考VeREF+，负参考VSSA */
    DL_DAC12_VREF_SOURCE_VEREFP_VSSA = (DAC12_CTL1_REFSP_VEREFP |
                                        DAC12_CTL1_REFSN_VSSA),
} DL_DAC12_VREF_SOURCE;

/** @enum DL_DAC12_SAMPLETIMER 采样定时器使能控制 */
typedef enum {
    /*!  DAC sample timer generator is disabled 采样定时器禁用 */
    DL_DAC12_SAMPLETIMER_DISABLE = DAC12_CTL3_STIMEN_CLR,
    /*!  DAC sample timer generator is enabled 采样定时器使能 */
    DL_DAC12_SAMPLETIMER_ENABLE = DAC12_CTL3_STIMEN_SET,
} DL_DAC12_SAMPLETIMER;


/** @enum DL_DAC12_SAMPLES_PER_SECOND 采样率配置（每秒采样数） */
typedef enum {
    /*!  Sample Time Trigger rate of 500 Samples Per Second 500 SPS */
    DL_DAC12_SAMPLES_PER_SECOND_500 = DAC12_CTL3_STIMCONFIG__500SPS,
    /*!  Sample Time Trigger rate of 1,000 Samples Per Second 1 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_1K = DAC12_CTL3_STIMCONFIG__1KSPS,
    /*!  Sample Time Trigger rate of 2,000 Samples Per Second 2 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_2K = DAC12_CTL3_STIMCONFIG__2KSPS,
    /*!  Sample Time Trigger rate of 4,000 Samples Per Second 4 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_4K = DAC12_CTL3_STIMCONFIG__4KSPS,
    /*!  Sample Time Trigger rate of 8,000 Samples Per Second 8 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_8K = DAC12_CTL3_STIMCONFIG__8KSPS,
    /*!  Sample Time Trigger rate of 16,000 Samples Per Second 16 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_16K = DAC12_CTL3_STIMCONFIG__16KSPS,
    /*!  Sample Time Trigger rate of 100,000 Samples Per Second 100 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_100K = DAC12_CTL3_STIMCONFIG__100KSPS,
    /*!  Sample Time Trigger rate of 200,000 Samples Per Second 200 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_200K = DAC12_CTL3_STIMCONFIG__200KSPS,
    /*!  Sample Time Trigger rate of 500,000 Samples Per Second 500 KSPS */
    DL_DAC12_SAMPLES_PER_SECOND_500K = DAC12_CTL3_STIMCONFIG__500KSPS,
    /*!  Sample Time Trigger rate of 1,000,000 Samples Per Second 1 MSPS */
    DL_DAC12_SAMPLES_PER_SECOND_1M = DAC12_CTL3_STIMCONFIG__1MSPS,
} DL_DAC12_SAMPLES_PER_SECOND;

/** @enum DL_DAC12_FIFO FIFO 模块使能控制 */
typedef enum {
    /*!  DAC FIFO is disabled FIFO 禁用 */
    DL_DAC12_FIFO_DISABLED = DAC12_CTL2_FIFOEN_CLR,
    /*!  DAC FIFO is enabled FIFO 使能 */
    DL_DAC12_FIFO_ENABLED = DAC12_CTL2_FIFOEN_SET,
} DL_DAC12_FIFO;


/** @enum DL_DAC12_FIFO_THRESHOLD FIFO 空闲阈值（用于 DMA 触发生成） */
typedef enum {
    /*!  DAC FIFO threshold for DMA Trigger Generation where 1/4th of location are empty FIFO 1/4 空位触发 */
    DL_DAC12_FIFO_THRESHOLD_ONE_QTR_EMPTY = DAC12_CTL2_FIFOTH_LOW,
    /*!  DAC FIFO threshold for DMA Trigger Generation where 2/4th of locations are empty FIFO 1/2 空位触发 */
    DL_DAC12_FIFO_THRESHOLD_TWO_QTRS_EMPTY = DAC12_CTL2_FIFOTH_MED,
    /*!  DAC FIFO threshold for DMA Trigger Generation where 3/4th of locations are empty FIFO 3/4 空位触发 */
    DL_DAC12_FIFO_THRESHOLD_THREE_QTRS_EMPTY = DAC12_CTL2_FIFOTH_HIGH,
} DL_DAC12_FIFO_THRESHOLD;

/** @enum DL_DAC12_FIFO_TRIGGER FIFO 数据读取触发源 */
typedef enum {
    /*!  DAC FIFO read trigger sourced from the sample time generator 采样定时器触发 FIFO 读取 */
    DL_DAC12_FIFO_TRIGGER_SAMPLETIMER = DAC12_CTL2_FIFOTRIGSEL_STIM,
    /*!  DAC FIFO read trigger sourced from the hardware trigger 0 event fabric 硬件触发0触发 FIFO 读取 */
    DL_DAC12_FIFO_TRIGGER_HWTRIG0 = DAC12_CTL2_FIFOTRIGSEL_TRIG0,
} DL_DAC12_FIFO_TRIGGER;

/** @enum DL_DAC12_DMA_TRIGGER DMA 触发机制使能控制 */
typedef enum {
    /*!  DMA trigger mechanism for DAC is disabled DMA 触发禁用 */
    DL_DAC12_DMA_TRIGGER_DISABLED = DAC12_CTL2_DMATRIGEN_CLR,
    /*!  DMA trigger mechanism for DAC is enabled DMA 触发使能 */
    DL_DAC12_DMA_TRIGGER_ENABLED = DAC12_CTL2_DMATRIGEN_SET,
} DL_DAC12_DMA_TRIGGER;

/** @enum DL_DAC12_CALIBRATION 校准数据源选择 */
typedef enum {

    /*! Factory Trim 出厂校准值 */
    DL_DAC12_CALIBRATION_FACTORY = DAC12_CALCTL_CALSEL_FACTORYTRIM,
    /*! Self Trim (last calibration result) 自校准值（上次校准结果） */
    DL_DAC12_CALIBRATION_SELF = DAC12_CALCTL_CALSEL_SELFCALIBRATIONTRIM,

} DL_DAC12_CALIBRATION;

/** @addtogroup DL_DAC12_INTERRUPT
 *  @{
 */

/*!
 *  @brief Interrupt raised when the module ready event has occurred 模块就绪中断
 */
#define DL_DAC12_INTERRUPT_MODULE_READY             (DAC12_GEN_EVENT_IMASK_MODRDYIFG_SET)

/*!
 *  @brief Interrupt raised when the FIFO is empty FIFO 空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_EMPTY            (DAC12_GEN_EVENT_IMASK_FIFOEMPTYIFG_SET)

/*!
 *  @brief Interrupt raised when the FIFO is 3/4th empty FIFO 3/4 空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_THREE_QTRS_EMPTY   (DAC12_GEN_EVENT_IMASK_FIFO3B4IFG_SET)

/*!
 *  @brief Interrupt raised when the FIFO is 2/4ths empty FIFO 1/2 空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_TWO_QTRS_EMPTY     (DAC12_GEN_EVENT_IMASK_FIFO1B2IFG_SET)

/*!
 *  @brief Interrupt raised when the FIFO is 1/4th empty FIFO 1/4 空中断
 */
#define DL_DAC12_INTERRUPT_FIFO_ONE_QTR_EMPTY      (DAC12_GEN_EVENT_IMASK_FIFO1B4IFG_SET)

/*!
 *  @brief Interrupt raised when the FIFO is full FIFO 满中断
 */
#define DL_DAC12_INTERRUPT_FIFO_FULL              (DAC12_GEN_EVENT_IMASK_FIFOFULLIFG_SET)

/*!
 *  @brief Interrupt raised when the FIFO is underrun
 *         (tried to read from an empty FIFO) FIFO 下溢中断（尝试读取空FIFO）
 */
#define DL_DAC12_INTERRUPT_FIFO_UNDERRUN          (DAC12_GEN_EVENT_IMASK_FIFOURUNIFG_SET)

/*!
 *  @brief Interrupt raised as an Acknowledgement to the DMA completing a
 *         transfer DMA 传输完成中断
 *
 *  During DMA operation only, once the DMA has performed its desired number
 *  of transfers to the FIFO, the DMA will assert a DMA done event. The DMA will
 *  also send a status, which is 0 if there is more data to send. If there is a
 *  nonzero status, and the DMA is done sending information, the DMATrigger will
 *  be disabled and this interrupt will be asserted.
 */
#define DL_DAC12_INTERRUPT_DMA_DONE                (DAC12_GEN_EVENT_IMASK_DMADONEIFG_SET)

/** @}*/

/** @addtogroup DL_DAC12_EVENT
 *  @{
 */

/*!
 *  @brief Event raised when the module ready event has occurred 模块就绪事件
 */
#define DL_DAC12_EVENT_MODULE_READY             (DAC12_GEN_EVENT_IMASK_MODRDYIFG_SET)

/*!
 *  @brief Event raised when the FIFO is empty FIFO 空事件
 */
#define DL_DAC12_EVENT_FIFO_EMPTY            (DAC12_GEN_EVENT_IMASK_FIFOEMPTYIFG_SET)

/*!
 *  @brief Event raised when the FIFO is 3/4th empty FIFO 3/4 空事件
 */
#define DL_DAC12_EVENT_FIFO_THREE_QTRS_EMPTY   (DAC12_GEN_EVENT_IMASK_FIFO3B4IFG_SET)

/*!
 *  @brief Event raised when the FIFO is 2/4ths empty FIFO 1/2 空事件
 */
#define DL_DAC12_EVENT_FIFO_TWO_QTRS_EMPTY     (DAC12_GEN_EVENT_IMASK_FIFO1B2IFG_SET)

/*!
 *  @brief Event raised when the FIFO is 1/4th empty FIFO 1/4 空事件
 */
#define DL_DAC12_EVENT_FIFO_ONE_QTR_EMPTY      (DAC12_GEN_EVENT_IMASK_FIFO1B4IFG_SET)

/*!
 *  @brief Event raised when the FIFO is full FIFO 满事件
 */
#define DL_DAC12_EVENT_FIFO_FULL              (DAC12_GEN_EVENT_IMASK_FIFOFULLIFG_SET)

/*!
 *  @brief Event raised when the FIFO is underrun
 *         (tried to read from an empty FIFO) FIFO 下溢事件（尝试读取空FIFO）
 */
#define DL_DAC12_EVENT_FIFO_UNDERRUN          (DAC12_GEN_EVENT_IMASK_FIFOURUNIFG_SET)

/*!
 *  @brief Event raised as an Acknowledgement to the DMA completing a
 *         transfer DMA 传输完成事件
 *
 *  During DMA operation only, once the DMA has performed its desired number
 *  of transfers to the FIFO, the DMA will assert a DMA done event. The DMA will
 *  also send a status, which is 0 if there is more data to send. If there is a
 *  nonzero status, and the DMA is done sending information, the DMATrigger will
 *  be disabled and this interrupt will be asserted.
 */
#define DL_DAC12_EVENT_DMA_DONE                (DAC12_GEN_EVENT_IMASK_DMADONEIFG_SET)

/** @}*/

    /* clang-format on */

    /*! @enum DL_DAC12_IIDX 中断索引号（用于读取最高优先级待处理中断） */
    typedef enum
    {
        /*! DAC interrupt index for no interrupt pending 无待处理中断 */
        DL_DAC12_IIDX_NO_INT = DAC12_CPU_INT_IIDX_STAT_NO_INTR,
        /*! DAC interrupt index when module is ready 模块就绪 */
        DL_DAC12_IIDX_MODULE_READY = DAC12_CPU_INT_IIDX_STAT_MODRDYIFG,
        /*! DAC interrupt index when FIFO is full FIFO 满 */
        DL_DAC12_IIDX_FIFO_FULL = DAC12_CPU_INT_IIDX_STAT_FIFOFULLIFG,
        /*! DAC interrupt index when FIFO is 1/4 empty FIFO 1/4 空 */
        DL_DAC12_IIDX_FIFO_1_4_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFO1B4IFG,
        /*! DAC interrupt index when FIFO is 1/2 empty FIFO 1/2 空 */
        DL_DAC12_IIDX_FIFO_1_2_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFO1B2IFG,
        /*! DAC interrupt index when FIFO is 3/4 empty FIFO 3/4 空 */
        DL_DAC12_IIDX_FIFO_3_4_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFO3B4IFG,
        /*! DAC interrupt index when FIFO is empty FIFO 空 */
        DL_DAC12_IIDX_FIFO_EMPTY = DAC12_CPU_INT_IIDX_STAT_FIFOEMPTYIFG,
        /*! DAC interrupt index if there is FIFO underrun FIFO 下溢（读空FIFO） */
        DL_DAC12_IIDX_FIFO_UNDERRUN = DAC12_CPU_INT_IIDX_STAT_FIFOURUNIFG,
        /*! DAC interrupt index for DMA transfer done DMA 传输完成 */
        DL_DAC12_IIDX_DMA_DONE = DAC12_CPU_INT_IIDX_STAT_DMADONEIFG
    } DL_DAC12_IIDX;

    /*! @enum DL_DAC12_SUBSCRIBER_INDEX 事件订阅通道索引 */
    typedef enum
    {
        /*! DAC12 Subscriber index 0 订阅通道0 */
        DL_DAC12_SUBSCRIBER_INDEX_0 = 0,
    } DL_DAC12_SUBSCRIBER_INDEX;

    /*! @enum DL_DAC12_EVENT_ROUTE 事件输出路由 */
    typedef enum
    {
        /*! DAC12 event route 1 事件路由1 */
        DL_DAC12_EVENT_ROUTE_1 = 0,
    } DL_DAC12_EVENT_ROUTE;

    /**
     *  @brief  Configuration struct for @ref DL_DAC12_init
     */
    typedef struct
    {
        /*! Enables the DAC output on the device OUT pin. One of @ref DL_DAC12_OUTPUT */
        DL_DAC12_OUTPUT outputEnable;

        /*! The bit resolution. One of @ref DL_DAC12_RESOLUTION */
        DL_DAC12_RESOLUTION resolution;

        /*! The input data representation. One of @ref DL_DAC12_REPRESENTATION */
        DL_DAC12_REPRESENTATION representation;

        /*! The voltage reference. One of @ref DL_DAC12_VREF_SOURCE */
        DL_DAC12_VREF_SOURCE voltageReferenceSource;

        /*! Output amplifier setting. One of @ref DL_DAC12_AMP */
        DL_DAC12_AMP amplifierSetting;

        /*! The FIFO enable. One of @ref DL_DAC12_FIFO */
        DL_DAC12_FIFO fifoEnable;

        /*! The Read Fifo Trigger source. One of @ref DL_DAC12_FIFO_TRIGGER */
        DL_DAC12_FIFO_TRIGGER fifoTriggerSource;

        /*! The bit to enable a DMA trigger source to use the DMA instead of
         *  the CPU to input data.
         *  One of @ref DL_DAC12_DMA_TRIGGER
         */
        DL_DAC12_DMA_TRIGGER dmaTriggerEnable;

        /*! The FIFO Threshold at which the DMA Trigger is asserted.
         *  One of @ref DL_DAC12_FIFO_THRESHOLD
         */
        DL_DAC12_FIFO_THRESHOLD dmaTriggerThreshold;

        /*! Enables the sample time generator as the FIFO trigger.
         *  One of @ref DL_DAC12_SAMPLETIMER
         */
        DL_DAC12_SAMPLETIMER sampleTimeGeneratorEnable;

        /*! Rate of the sample time generator. One of @ref
         *  DL_DAC12_SAMPLES_PER_SECOND
         */
        DL_DAC12_SAMPLES_PER_SECOND sampleRate;
    } DL_DAC12_Config;

    /**
     *  @brief      Initialize the DAC module 初始化 DAC12 模块
     *
     *  Initializes all the common configurable options for the DAC module. Any
     *  other custom configuration can be done after this initialization.
     *  按配置结构体初始化 DAC12 模块的关键工作模式，不在该接口内使能模块。
     *  @post DAC is not enabled
     *
     *  @param[in]  dac12   Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  config  Configuration for DAC12 peripheral DAC12配置结构体指针
     */
    void DL_DAC12_init(DAC12_Regs *dac12, const DL_DAC12_Config *config);

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the DAC12 使能DAC12外设电源写入权限
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  @note For power savings, please refer to @ref DL_DAC12_enable
     *
     * @param dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_enablePower(DAC12_Regs *dac12)
    {
        dac12->GPRCM.PWREN =
            (DAC12_PWREN_KEY_UNLOCK_W | DAC12_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the DAC12 禁用DAC12外设电源写入权限
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @note This API does not provide large power savings. For power savings,
     *  please refer to @ref DL_DAC12_enable
     *
     * @param dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_disablePower(DAC12_Regs *dac12)
    {
        dac12->GPRCM.PWREN =
            (DAC12_PWREN_KEY_UNLOCK_W | DAC12_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the DAC12
     *        is enabled 查询DAC12外设电源是否已使能
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *
     * @param dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *
     * @return true if peripheral register access is enabled 外设寄存器可访问时返回true
     * @return false if peripheral register access is disabled 外设寄存器不可访问时返回false
     */
    __STATIC_INLINE bool DL_DAC12_isPowerEnabled(const DAC12_Regs *dac12)
    {
        return ((dac12->GPRCM.PWREN & DAC12_PWREN_ENABLE_MASK) ==
                DAC12_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Resets dac12 peripheral 复位DAC12外设
     *
     * @param dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_reset(DAC12_Regs *dac12)
    {
        dac12->GPRCM.RSTCTL =
            (DAC12_RSTCTL_KEY_UNLOCK_W | DAC12_RSTCTL_RESETSTKYCLR_CLR |
             DAC12_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief Returns if dac12 peripheral was reset 查询DAC12是否已复位
     *
     * @param dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *
     * @return true if peripheral was reset 外设已复位时返回true
     * @return false if peripheral wasn't reset 外设未复位时返回false
     *
     */
    __STATIC_INLINE bool DL_DAC12_isReset(const DAC12_Regs *dac12)
    {
        return ((dac12->GPRCM.STAT & DAC12_STAT_RESETSTKY_MASK) ==
                DAC12_STAT_RESETSTKY_RESET);
    }

    /**
     *  @brief      Enables the DAC module 使能DAC模块
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_enable(DAC12_Regs *dac12)
    {
        dac12->CTL0 |= DAC12_CTL0_ENABLE_SET;
    }

    /**
     *  @brief      Disables the DAC Module 禁用DAC模块
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_disable(DAC12_Regs *dac12)
    {
        dac12->CTL0 &= ~DAC12_CTL0_ENABLE_MASK;
    }

    /**
     *  @brief      Checks the enable bit of the DAC 查询DAC是否已使能
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Whether the DAC module is enabled DAC模块使能状态
     *
     *  @retval     true    Module enabled 模块已使能
     *  @retval     false   Module disabled 模块已禁用
     */
    __STATIC_INLINE bool DL_DAC12_isEnabled(const DAC12_Regs *dac12)
    {
        uint32_t t = (dac12->CTL0 & DAC12_CTL0_ENABLE_MASK);
        return (t == DAC12_CTL0_ENABLE_SET);
    }

    /**
     *  @brief      Sets all elements of the input data format at once 配置输入数据格式（表示方式和分辨率）
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  rep      Data Representation. One of
     *                       @ref DL_DAC12_REPRESENTATION 数据表示格式
     *  @param[in]  res      Data Resolution. One of @ref DL_DAC12_RESOLUTION 数据分辨率
     */
    __STATIC_INLINE void DL_DAC12_configDataFormat(
        DAC12_Regs *dac12, DL_DAC12_REPRESENTATION rep, DL_DAC12_RESOLUTION res)
    {
        DL_Common_updateReg(&dac12->CTL0, ((uint32_t)rep | (uint32_t)res),
                            DAC12_CTL0_RES_MASK | DAC12_CTL0_DFM_MASK);
    }

    /**
     *  @brief      Gets the currently configured amplifier setting 获取当前放大器配置
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Currently configured amplifier setting 当前放大器配置值
     *
     *  @retval     One of @ref DL_DAC12_AMP
     */
    __STATIC_INLINE DL_DAC12_AMP DL_DAC12_getAmplifier(const DAC12_Regs *dac12)
    {
        uint32_t ampVal =
            (dac12->CTL1 & (DAC12_CTL1_AMPEN_MASK | DAC12_CTL1_AMPHIZ_MASK));

        return (DL_DAC12_AMP)(ampVal);
    }

    /**
     *  @brief      Sets the DAC and output amplifer setting 设置输出放大器模式
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  ampVal   amplifier configuration value. One of @ref DL_DAC12_AMP 放大器配置值
     */
    __STATIC_INLINE void DL_DAC12_setAmplifier(
        DAC12_Regs *dac12, DL_DAC12_AMP ampVal)
    {
        DL_Common_updateReg(&dac12->CTL1, (uint32_t)ampVal,
                            (DAC12_CTL1_AMPEN_MASK | DAC12_CTL1_AMPHIZ_MASK));
    }

    /**
     *  @brief      Gets the currently configured reference voltage source 获取当前参考电压源配置
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Currently configured reference voltage 当前参考电压源
     *
     *  @retval     Bitwise OR of @ref DL_DAC12_VREF_SOURCE
     */
    __STATIC_INLINE DL_DAC12_VREF_SOURCE DL_DAC12_getReferenceVoltageSource(
        const DAC12_Regs *dac12)
    {
        uint32_t refsVal =
            (dac12->CTL1 & (DAC12_CTL1_REFSP_MASK | DAC12_CTL1_REFSN_MASK));

        return (DL_DAC12_VREF_SOURCE)(refsVal);
    }

    /**
     *  @brief      Set the reference voltage source of the DAC 设置DAC参考电压源
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  refsVal  reference voltage source. Bitwise OR of
     *                       @ref DL_DAC12_VREF_SOURCE 参考电压源配置值
     */
    __STATIC_INLINE void DL_DAC12_setReferenceVoltageSource(
        DAC12_Regs *dac12, DL_DAC12_VREF_SOURCE refsVal)
    {
        DL_Common_updateReg(&dac12->CTL1, (uint32_t)refsVal,
                            (DAC12_CTL1_REFSP_MASK | DAC12_CTL1_REFSN_MASK));
    }

    /**
     *  @brief      Enables the DAC output by connecting it to the OUT0 pin 使能DAC输出到OUT0引脚
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_enableOutputPin(DAC12_Regs *dac12)
    {
        dac12->CTL1 |= DAC12_CTL1_OPS_OUT0;
    }

    /**
     *  @brief      Disable the DAC output by disconnecting it from the OUT0 pin 断开DAC输出与OUT0引脚
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_disableOutputPin(DAC12_Regs *dac12)
    {
        dac12->CTL1 &= ~DAC12_CTL1_OPS_MASK;
    }

    /**
     *  @brief      Checks to see whether the output is connected 查询输出引脚是否已连接
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Status of output connection 输出引脚连接状态
     *
     *  @retval     true  Output is connected 输出已连接
     *  @retval     false Output is not connected 输出未连接
     */
    __STATIC_INLINE bool DL_DAC12_isOutputPinEnabled(const DAC12_Regs *dac12)
    {
        return ((dac12->CTL1 & DAC12_CTL1_OPS_MASK) == DAC12_CTL1_OPS_OUT0);
    }

    /**
     *  @brief      Enables the FIFO module 使能FIFO模块
     *
     *  Enables the FIFO and the FIFO hardware control state machine
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_enableFIFO(DAC12_Regs *dac12)
    {
        /* Insert value */
        dac12->CTL2 |= DAC12_CTL2_FIFOEN_SET;
    }

    /**
     *  @brief      Disables the FIFO 禁用FIFO
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @sa         DL_DAC12_enableFIFO
     */
    __STATIC_INLINE void DL_DAC12_disableFIFO(DAC12_Regs *dac12)
    {
        /* Clear out the bit */
        dac12->CTL2 &= ~DAC12_CTL2_FIFOEN_MASK;
    }

    /**
     *  @brief      Checks whether the FIFO is enabled 查询FIFO是否已使能
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Current status of the FIFO FIFO使能状态
     *
     *  @retval     true    FIFO is enabled FIFO已使能
     *  @retval     false   FIFO is not enabled FIFO未使能
     */
    __STATIC_INLINE bool DL_DAC12_isFIFOEnabled(const DAC12_Regs *dac12)
    {
        uint32_t t = (dac12->CTL2 & DAC12_CTL2_FIFOEN_MASK);
        return (t == DAC12_CTL2_FIFOEN_SET);
    }

    /**
     *  @brief      Gets the FIFO threshold 获取FIFO阈值
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Currently configured FIFO Threshold 当前FIFO阈值
     *
     *  @retval     One of @ref DL_DAC12_FIFO_THRESHOLD
     *
     *  @sa         DL_DAC12_setFIFOThreshold
     */
    __STATIC_INLINE DL_DAC12_FIFO_THRESHOLD DL_DAC12_getFIFOThreshold(
        const DAC12_Regs *dac12)
    {
        uint32_t fifoThreshold = (dac12->CTL2 & DAC12_CTL2_FIFOTH_MASK);

        return (DL_DAC12_FIFO_THRESHOLD)(fifoThreshold);
    }

    /**
     *  @brief      Sets the FIFO threshold 设置FIFO阈值
     *
     *  Determines the FIFO threshold In case of DMA based operation,
     *  DAC generates new DMA trigger when the number of empty locations in FIFO
     *  match the selected FIFO threshold level.
     *  In the case of CPU based operation, the FIFO threshold bits are don't care
     *  and FIFO level is directly indicated through the respective bits in the
     *  RIS register
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  fifoThreshold  Threshold value. One of @ref DL_DAC12_FIFO_THRESHOLD FIFO阈值
     */
    __STATIC_INLINE void DL_DAC12_setFIFOThreshold(
        DAC12_Regs *dac12, DL_DAC12_FIFO_THRESHOLD fifoThreshold)
    {
        DL_Common_updateReg(
            &dac12->CTL2, (uint32_t)fifoThreshold, DAC12_CTL2_FIFOTH_MASK);
    }

    /**
     *  @brief      Gets the FIFO read trigger source 获取FIFO读触发源
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Currently configured trigger source 当前FIFO读触发源
     *
     *  @retval     One of @ref DL_DAC12_FIFO_TRIGGER
     *
     *  @sa         DL_DAC12_setFIFOTriggerSource
     */
    __STATIC_INLINE DL_DAC12_FIFO_TRIGGER DL_DAC12_getFIFOTriggerSource(
        const DAC12_Regs *dac12)
    {
        uint32_t fifoTrig = (dac12->CTL2 & DAC12_CTL2_FIFOTRIGSEL_MASK);

        return (DL_DAC12_FIFO_TRIGGER)(fifoTrig);
    }

    /**
     *  @brief      Sets the FIFO read trigger source 设置FIFO读触发源
     *
     *  Selects the source for FIFO read trigger by the DAC. When the selected FIFO
     *  read trigger is asserted, the data from FIFO (as indicated by read pointer)
     *  is moved into internal DAC data register
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  fifoTrig    Read Trigger Source. One of
     *                          @ref DL_DAC12_FIFO_TRIGGER FIFO读触发源
     */
    __STATIC_INLINE void DL_DAC12_setFIFOTriggerSource(
        DAC12_Regs *dac12, DL_DAC12_FIFO_TRIGGER fifoTrig)
    {
        DL_Common_updateReg(
            &dac12->CTL2, (uint32_t)fifoTrig, DAC12_CTL2_FIFOTRIGSEL_MASK);
    }

    /**
     *  @brief      Enables the DMA trigger generator 使能DMA触发生成器
     *
     *  When this is and the FIFO are enabled, the DMA trigger is generated based
     *  on the empty FIFO locations qualified by the selected FIFO Threshold
     *  settings. This bit is automatically cleared by hardware when the DMA done
     *  event is asserted with DMA status signal value being nonzero
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @sa         DL_DAC12_enableFIFO
     *  @sa         DL_DAC12_setFIFOThreshold
     */
    __STATIC_INLINE void DL_DAC12_enableDMATrigger(DAC12_Regs *dac12)
    {
        /* Insert value */
        dac12->CTL2 |= DAC12_CTL2_DMATRIGEN_SET;
    }

    /**
     *  @brief      Disables the DMA trigger generator 禁用DMA触发生成器
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @sa         DL_DAC12_enableDMATriggerGenerator
     */
    __STATIC_INLINE void DL_DAC12_disableDMATrigger(DAC12_Regs *dac12)
    {
        /* Clear out the bit */
        dac12->CTL2 &= ~DAC12_CTL2_DMATRIGEN_MASK;
    }

    /**
     *  @brief      Checks whether the DMA trigger generator is enabled 查询DMA触发是否已使能
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Current status of the DMA trigger generator DMA触发使能状态
     *
     *  @retval     true   DMA Trigger enabled DMA触发已使能
     *  @retval     false  DMA Trigger not enabled DMA触发未使能
     */
    __STATIC_INLINE bool DL_DAC12_isDMATriggerEnabled(const DAC12_Regs *dac12)
    {
        uint32_t t = (dac12->CTL2 & DAC12_CTL2_DMATRIGEN_MASK);
        return (t == DAC12_CTL2_DMATRIGEN_SET);
    }

    /**
     *  @brief      Enables the sample time generator 使能采样定时器
     *
     *  The sample time generator can be selected as the FIFO Trigger @ref
     *  DL_DAC12_setFIFOTriggerSource and control the rate at which the DAC will
     *  consume inputs from the FIFO
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_enableSampleTimeGenerator(DAC12_Regs *dac12)
    {
        /* Insert value */
        dac12->CTL3 |= DAC12_CTL3_STIMEN_SET;
    }

    /**
     *  @brief      Disables the sample time generator 禁用采样定时器
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @sa         DL_DAC12_enableSampleTimeGenerator
     */
    __STATIC_INLINE void DL_DAC12_disableSampleTimeGenerator(DAC12_Regs *dac12)
    {
        /* Clear out the bit */
        dac12->CTL3 &= ~DAC12_CTL3_STIMEN_MASK;
    }

    /**
     *  @brief      Checks whether the sample time trigger generator is enabled 查询采样定时器是否已使能
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Current status of the sample time generator 采样定时器使能状态
     *
     *  @retval     true  Sample time generator is enabled 采样定时器已使能
     *  @retval     false Sample time generator is not enabled 采样定时器未使能
     */
    __STATIC_INLINE bool DL_DAC12_isSampleTimeGeneratorEnabled(
        const DAC12_Regs *dac12)
    {
        uint32_t t = (dac12->CTL3 & DAC12_CTL3_STIMEN_MASK);
        return (t == DAC12_CTL3_STIMEN_SET);
    }

    /**
     *  @brief      Gets the sample trigger rate of the sample time generator 获取采样率
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Currently configured sample rate 当前采样率
     *
     *  @retval     One of @ref DL_DAC12_SAMPLES_PER_SECOND
     */
    __STATIC_INLINE DL_DAC12_SAMPLES_PER_SECOND DL_DAC12_getSampleRate(
        const DAC12_Regs *dac12)
    {
        uint32_t sampleRate = (dac12->CTL3 & DAC12_CTL3_STIMCONFIG_MASK);

        return (DL_DAC12_SAMPLES_PER_SECOND)(sampleRate);
    }

    /**
     *  @brief      Sets the sample triggering rate of the sample time generator 设置采样率
     *
     *  Sets the sample trigger rate of the sample time generator.
     *  The sample time generator can be selected as the FIFO Trigger @ref
     *  DL_DAC12_setFIFOTriggerSource and control the rate at which the DAC will
     *  consume inputs from the FIFO
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  sampleRate  Desired sample rate. One of
     *                          @ref DL_DAC12_SAMPLES_PER_SECOND 目标采样率
     */
    __STATIC_INLINE void DL_DAC12_setSampleRate(
        DAC12_Regs *dac12, DL_DAC12_SAMPLES_PER_SECOND sampleRate)
    {
        DL_Common_updateReg(
            &dac12->CTL3, (uint32_t)sampleRate, DAC12_CTL3_STIMCONFIG_MASK);
    }

    /**
     *  @brief      Checks whether a calibration sequence is currently running 查询校准是否正在运行
     *
     *  @param[in]  dac12    pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Current status of an active calibration sequence 校准运行状态
     *
     *  @retval     true  Calibration Sequence is currently running 校准正在进行
     *  @retval     false Calibration Sequence is not running 校准未在运行
     *
     *  @sa         DL_DAC12_startCalibration
     */
    __STATIC_INLINE bool DL_DAC12_isCalibrationRunning(const DAC12_Regs *dac12)
    {
        uint32_t t = (dac12->CALCTL & DAC12_CALCTL_CALON_MASK);
        return (t == DAC12_CALCTL_CALON_ACTIVE);
    }

    /**
     *  @brief      Initiates the DAC offset error calibration sequence 启动DAC偏移误差自校准
     *
     *  Initiates the DAC offset error calibration sequence and
     *  the CALON_ACTIVE bit is automatically reset when the offset error
     *  calibration completes. Upon completion, the correct calibration will be put
     *  into the CALDATA register. This sequence can be started either on the fly
     *  (once the module is ready) or during setup (where this is called directy
     *  after enabling the DAC).
     *
     *  In order for the calibration to be successful, the AMP must already be
     *  configured and the LOCK bit must be cleared for the value to be written
     *  successfully. The output is also tri-stated during calibration.
     *
     *  A negative offset will mean that low input values will have a output of 0.
     *  A positive offset will mean that an input of 0 will result in a non-zero
     *  output.
     *
     *  @param[in]  dac12   pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    __STATIC_INLINE void DL_DAC12_startCalibration(DAC12_Regs *dac12)
    {
        dac12->CALCTL =
            (DAC12_CALCTL_CALON_ACTIVE | DAC12_CALCTL_CALSEL_SELFCALIBRATIONTRIM);
    }

    /**
     *  @brief      Gets the DAC Calibration offset 获取校准偏移数据
     *
     *  Reads of the CALDATA register should only be performed after the
     *  calibration sequence has completed. During calibration, this register is
     *  continuously written to, and inaccurate values could be obtained.
     *
     *  @param[in]  dac12 pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     Current error calibration offset 当前校准偏移值
     *
     *  @retval     -64 to +63 in two's complement 二补码范围 -64 ~ +63
     */
    __STATIC_INLINE uint32_t DL_DAC12_getCalibrationData(const DAC12_Regs *dac12)
    {
        return (dac12->CALDATA & DAC12_CALDATA_DATA_MASK);
    }

    /**
     *  @brief      Perform calibration sequence 执行自校准并阻塞等待完成
     *
     *  Enables writes, and then starts the calibration sequence. Should only be
     *  called after the DAC module is enabled. This will only return when the
     *  calibration sequence has completed.
     *  执行一次完整自校准流程，并阻塞等待校准完成。
     *
     *  @pre Amplifier settings should be configured. @ref DL_DAC12_setAmplifier
     *  @post Calibration is complete, and CALDATA is locked to prevent writes.
     *
     *  @param[in]  dac12   pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     */
    void DL_DAC12_performSelfCalibrationBlocking(DAC12_Regs *dac12);

    /**
     *  @brief      Outputs an 8-bit data value 输出8位数据到DAC
     *
     *  Using the CPU to control the DAC, this register can be written to if a
     *  fixed output voltage is desired (ex. DC Generation) with the FIFO disabled,
     *  or it can write with variable output (ex. AC Generation) by enabling the
     *  FIFO and writing to this register. The value will then be written to the
     *  FIFO internally. With FIFO enabled with @ref DL_DAC12_enableFIFO, the
     *  FIFO-specific interrupts will be generated inside the DAC.
     *
     *  The DMA trigger generation mechanism must be kept disabled when CPU is used
     *  to load data into DAC
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  dataValue   value to be written to the bit-field from 0x0 to
     *                          0xFF. It can be either binary or two's complement 待写入的8位数据（0x00~0xFF）
     */
    __STATIC_INLINE void DL_DAC12_output8(DAC12_Regs *dac12, uint8_t dataValue)
    {
        dac12->DATA0 = dataValue;
    }

    /**
     *  @brief      Outputs a 12-bit Data Value 输出12位数据到DAC
     *
     *  Using the CPU to control the DAC, this register can be written to if a
     *  fixed output voltage is desired (ex. DC Generation) with the FIFO disabled,
     *  or it can write with variable output (ex. AC Generation) by enabling the
     *  FIFO and writing to this register. The value will then be written to the
     *  FIFO internally. With FIFO enabled with @ref DL_DAC12_enableFIFO, the
     *  FIFO-specific interrupts will be generated inside the DAC.
     *
     *  The DMA trigger generation mechanism must be kept disabled when CPU is used
     *  to load data into DAC
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  dataValue   value to be written to the bit-field from 0x0 to
     *                          0xFFF. It can be either binary or two's complement 待写入的12位数据（0x000~0xFFF）
     */
    __STATIC_INLINE void DL_DAC12_output12(DAC12_Regs *dac12, uint32_t dataValue)
    {
        dac12->DATA0 = (dataValue & DAC12_DATA0_DATA_VALUE_MASK);
    }

    /**
     *  @brief      Fills the DAC fifo with 8-bit data values from the buffer 批量写入8位数据到FIFO
     *
     *  Writes as many values from the data buffer to the FIFO as possible,
     *  and then exits when either the count is matched or the FIFO is full.
     *  尝试将 8 位数据批量写入 FIFO，直到 FIFO 满或达到 count。
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  buffer      array containing the data values (0x00 - 0xFF) to
     *                          be written 源数据缓冲区（0x00~0xFF）
     *  @param[in]  count       length of the buffer, or maximum desired transfer 最大写入样本数
     *
     *  @return     Count successfully transferred to the FIFO 实际成功写入FIFO的样本数
     *
     *  @retval     0 - max(number of empty fifo slots, count)
     */
    uint32_t DL_DAC12_fillFIFO8(
        DAC12_Regs *dac12, const uint8_t *buffer, uint32_t count);

    /**
     *  @brief      Fills the DAC fifo with 12-bit data values from the buffer 批量写入12位数据到FIFO
     *
     *  Writes as many values from the data buffer to the FIFO as possible,
     *  and then exits when either the count is matched or the FIFO is full.
     *  尝试将 12 位数据批量写入 FIFO，直到 FIFO 满或达到 count。
     *
     *  @param[in]  dac12       pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  buffer      array containing the data values (0x00 - 0xFFF) to
     *                          be written 源数据缓冲区（0x000~0xFFF）
     *  @param[in]  count       length of the buffer, or maximum desired transfer 最大写入样本数
     *
     *  @return     Count successfully transfered to the FIFO 实际成功写入FIFO的样本数
     *
     *  @retval     0 - max(number of empty fifo slots, count)
     */
    uint32_t DL_DAC12_fillFIFO12(
        DAC12_Regs *dac12, const uint16_t *buffer, uint32_t count);

    /**
     *  @brief      Blocking 8-bit output to the DAC FIFO 阻塞写入8位数据到FIFO
     *
     *  Waits until the FIFO is not full, then writes a single value
     *  阻塞等待 FIFO 有空位后写入 1 个 8 位样本。
     *
     *  @param[in]  dac12        pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  data         data value (0x00 - 0xFF) to be written 待输出的8位数据
     */
    void DL_DAC12_outputBlocking8(DAC12_Regs *dac12, uint8_t data);

    /**
     *  @brief      Blocking 12-bit output to the DAC FIFO 阻塞写入12位数据到FIFO
     *
     *  Waits until the FIFO is not full, then writes a single value
     *  阻塞等待 FIFO 有空位后写入 1 个 12 位样本。
     *
     *  @param[in]  dac12        pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  data         data value (0x000 - 0xFFF) to be written 待输出的12位数据
     */
    void DL_DAC12_outputBlocking12(DAC12_Regs *dac12, uint16_t data);

    /**
     *  @brief      Checks the raw interrupt status of one or more interrupts 查询原始中断状态
     *
     *  The raw interrupt status is independent of whether a specific interrupt is
     *  enabled.
     *
     *  @param[in]  dac12         pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  interruptMask the desired interrupt(s). One or more of
     *                            @ref DL_DAC12_INTERRUPT 目标中断掩码
     *
     *  @return     currently asserted interrupts (whether enabled or not) bit-wise
     *              AND with the inputted interrupts 当前已触发的中断（与输入掩码按位与）
     *
     *  @retval     0       no interrupts that were inputted are currently asserted 无匹配中断
     *  @retval     not 0   a bit-wise OR of the inputted @ref DL_DAC12_INTERRUPT
     *                      registers that are currently asserted. 已触发中断的按位或
     */
    __STATIC_INLINE uint32_t DL_DAC12_getInterruptStatus(
        const DAC12_Regs *dac12, uint32_t interruptMask)
    {
        return (dac12->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Clears the interrupt status of one or more interrupts 清除指定中断状态
     *
     *  Accesses the write only interrupt clear (ICLR) register and writes a 1 to
     *  the bits specified. Any bit that has 1 written to it will clear that pending
     *  interrupt if it is currently pending.
     *
     *  @param[in]  dac12         pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  interruptMask the desired interrupt(s). One or more of
     *                            @ref DL_DAC12_INTERRUPT 目标中断掩码
     */
    __STATIC_INLINE void DL_DAC12_clearInterruptStatus(
        DAC12_Regs *dac12, uint32_t interruptMask)
    {
        dac12->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief      Enables one or more interrupts 使能指定中断
     *
     *  Access and write to the IMASK register to enable the interrupts specified.
     *
     *  @param[in]  dac12         pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  interruptMask the desired interrupt(s). One or more of
     *                            @ref DL_DAC12_INTERRUPT 目标中断掩码
     */
    __STATIC_INLINE void DL_DAC12_enableInterrupt(
        DAC12_Regs *dac12, uint32_t interruptMask)
    {
        dac12->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disables one or more interrupts 禁用指定中断
     *
     *  Access and write to the IMASK register to disable the interrupts specified.
     *
     *  @param[in]  dac12         pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  interruptMask the desired interrupt(s). One or more of
     *                            @ref DL_DAC12_INTERRUPT 目标中断掩码
     */
    __STATIC_INLINE void DL_DAC12_disableInterrupt(
        DAC12_Regs *dac12, uint32_t interruptMask)
    {
        dac12->CPU_INT.IMASK &= ~interruptMask;
    }

    /**
     *  @brief      Gets the highest priority pending interrupt 获取最高优先级待处理中断
     *
     *  Accesses and reads the IIDX register to obtain the highest currently
     *  pending interrupt. Interrupts with lower offsets (and smaller values) are
     *  higher priority than interrupts with higher offsets (and larger values).
     *  In order for the interrupt to be pending, it must have been enabled and
     *  asserted. Clearing the interrupt should will un-pend the interrupt.
     *
     *  @param[in]  dac12        pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     highest priority pending interrupt 最高优先级中断索引
     *
     *  @retval     One of @ref DL_DAC12_IIDX
     */
    __STATIC_INLINE DL_DAC12_IIDX DL_DAC12_getPendingInterrupt(
        const DAC12_Regs *dac12)
    {
        return ((DL_DAC12_IIDX)dac12->CPU_INT.IIDX);
    }

    /**
     *  @brief      Checks if the DAC FIFO is currently full 查询FIFO是否已满
     *
     *  Accesses the raw interrupt status (RIS) register in order to get the
     *  current state of the FIFOFull Interrupt flag.
     *
     *  @param[in]  dac12        pointer to the register overlay of the peripheral 指向DAC12外设寄存器的指针
     *
     *  @return     status of FIFO full interrupt flag FIFO满状态
     *
     *  @retval     true   FIFO is currently full FIFO已满
     *  @retval     false  FIFO is not full FIFO未满
     */
    __STATIC_INLINE bool DL_DAC12_isFIFOFull(const DAC12_Regs *dac12)
    {
        uint32_t t =
            DL_DAC12_getInterruptStatus(dac12, DL_DAC12_INTERRUPT_FIFO_FULL);
        return (t == DL_DAC12_INTERRUPT_FIFO_FULL);
    }

    /**
     *  @brief Sets the event publisher channel id 设置事件发布通道ID
     *
     *  @param[in]  dac12 Pointer to the register overlay for the
     *                      peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
     *                      publisher is disconnected. 通道ID（0-15，0表示断开）
     */
    __STATIC_INLINE void DL_DAC12_setPublisherChanID(
        DAC12_Regs *dac12, uint8_t chanID)
    {
        dac12->FPUB_1 = (chanID & DAC12_FPUB_1_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event publisher channel id 获取事件发布通道ID
     *
     *  @param[in]  dac12 Pointer to the register overlay for the
     *                      peripheral 指向DAC12外设寄存器的指针
     *  @return     Event publisher channel ID 事件发布通道ID
     *
     */
    __STATIC_INLINE uint8_t DL_DAC12_getPublisherChanID(const DAC12_Regs *dac12)
    {
        return ((uint8_t)((dac12->FPUB_1) & DAC12_FPUB_1_CHANID_MASK));
    }

    /**
     *  @brief Sets the event subscriber channel id 设置事件订阅通道ID
     *
     *  @param[in]  dac12 Pointer to the register overlay for the
     *                      peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  index   Specifies the register event index to be configured 事件寄存器索引
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
     *                      subscriber is disconnected. 通道ID（0-15，0表示断开）
     */
    __STATIC_INLINE void DL_DAC12_setSubscriberChanID(
        DAC12_Regs *dac12, DL_DAC12_SUBSCRIBER_INDEX index, uint8_t chanID)
    {
        volatile uint32_t *pReg = &dac12->FSUB_0;

        *(pReg + (uint32_t)index) = (chanID & DAC12_FSUB_0_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event subscriber channel id 获取事件订阅通道ID
     *
     *  @param[in]  dac12 Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  index  Specifies the register event index to be configured 事件寄存器索引
     *
     *  @return     Event subscriber channel ID 事件订阅通道ID
     *
     */
    __STATIC_INLINE uint8_t DL_DAC12_getSubscriberChanID(
        DAC12_Regs *dac12, DL_DAC12_SUBSCRIBER_INDEX index)
    {
        volatile uint32_t *pReg = &dac12->FSUB_0;

        return ((uint8_t)(*(pReg + (uint32_t)index) & DAC12_FSUB_0_CHANID_MASK));
    }

    /**
     *  @brief      Enable DAC event 使能DAC事件
     *
     *  @param[in]  dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_DAC12_EVENT. 事件掩码
     */
    __STATIC_INLINE void DL_DAC12_enableEvent(
        DAC12_Regs *dac12, uint32_t eventMask)
    {
        dac12->GEN_EVENT.IMASK |= (eventMask);
    }

    /**
     *  @brief      Disable DAC event 禁用DAC事件
     *
     *  @param[in]  dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_DAC12_EVENT. 事件掩码
     */
    __STATIC_INLINE void DL_DAC12_disableEvent(
        DAC12_Regs *dac12, uint32_t eventMask)
    {
        dac12->GEN_EVENT.IMASK &= ~(eventMask);
    }

    /**
     *  @brief      Check which DAC events are enabled 查询已使能的DAC事件
     *
     *  @param[in]  dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  eventMask      Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_DAC12_EVENT. 事件掩码
     *
     *  @return     Which of the requested DAC interrupts are enabled 已使能的事件
     *
     *  @retval     Bitwise OR of @ref DL_DAC12_EVENT values
     */
    __STATIC_INLINE uint32_t DL_DAC12_getEnabledEvents(
        const DAC12_Regs *dac12, uint32_t eventMask)
    {
        return ((dac12->GEN_EVENT.IMASK) & (eventMask));
    }

    /**
     *  @brief      Check event flag of enabled DAC event 查询已使能事件的待处理状态
     *
     *  Checks if any of the DAC events that were previously enabled are
     *  pending.
     *
     *  @param[in]  dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  eventMask      Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_DAC12_EVENT. 事件掩码
     *
     *  @return     Which of the requested DAC interrupts are pending 待处理的事件
     *
     *  @retval     Bitwise OR of @ref DL_DAC12_EVENT values
     *
     *  @sa         DL_DAC12_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_DAC12_getEnabledEventStatus(
        const DAC12_Regs *dac12, uint32_t eventMask)
    {
        return ((dac12->GEN_EVENT.MIS) & eventMask);
    }

    /**
     *  @brief      Check interrupt flag of any DAC event 查询任意DAC事件的原始状态
     *
     *  Checks if any events are pending. Events do not have to
     *  be previously enabled.
     *
     *  @param[in]  dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  eventMask      Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_DAC12_EVENT. 事件掩码
     *
     *  @return     Which of the requested DAC interrupts are pending 待处理的事件
     *
     *  @retval     Bitwise OR of @ref DL_DAC12_EVENT values
     */
    __STATIC_INLINE uint32_t DL_DAC12_getRawEventsStatus(
        const DAC12_Regs *dac12, uint32_t eventMask)
    {
        return ((dac12->GEN_EVENT.RIS) & eventMask);
    }

    /**
     *  @brief      Clear pending DAC events 清除待处理DAC事件
     *
     *  @param[in]  dac12        Pointer to the register overlay for the peripheral 指向DAC12外设寄存器的指针
     *  @param[in]  eventMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_DAC12_EVENT. 待清除的事件掩码
     */
    __STATIC_INLINE void DL_DAC12_clearEventsStatus(
        DAC12_Regs *dac12, uint32_t eventMask)
    {
        dac12->GEN_EVENT.ICLR |= (eventMask);
    }

#ifdef __cplusplus
}
#endif
#endif /* __MSPM0_HAS_DAC12__ */

#endif /* ti_dl_dl_dac12__include */
/** @}*/
