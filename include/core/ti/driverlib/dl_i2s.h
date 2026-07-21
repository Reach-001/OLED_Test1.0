/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @file       dl_i2s.h
 *  @brief      Inter-Integrated Circuit Sound (I2S) Driver Library
 *              I2S（集成电路间音频总线）驱动库头文件。
 *  @defgroup   I2S  Inter-Integrated Circuit Sound (I2S)
 *
 *  @anchor ti_dl_dl_i2s_Overview
 *  # Overview
 *
 *   The I2S Library allows full configuration of the MSPM0 I2S module.
 *   The I2S module provides a standardized serial interface to transfer audio
 *   data between MSP devices and other external devices (such as an audio
 *   amplifier).
 *
 *   I2S 库提供 MSPM0 I2S 模块的完整配置接口。I2S 模块通过标准化串行
 *   接口在 MSP 器件与外部音频器件（如音频功放）之间传输音频数据。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup I2S
 * @{
 */
#ifndef ti_dl_dl_i2s__include
#define ti_dl_dl_i2s__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_I2S__

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

#if (I2S0_SYS_SINGLE_PIN == 0)
/**
 *  @brief Device supports multiple data pins
 *         设备支持多数据引脚（AD0/AD1）。
 */
#define DEVICE_HAS_MULTIPLE_DATA_PIN
#endif

/**
 *  @brief I2S Minimum factor of the division ratio used to generate the
 *  controller clock (MCLK). The minimum is 2, since 0 is evaluated as
 *  1024, which is the maximum factor, and 1 is invalid.
 *         MCLK 分频系数最小值。0 解释为 1024（最大），1 为无效值。
 */
#define DL_I2S_MDIV_MINIMUM                                      ((uint32_t)2U)

/**
 *  @brief I2S Maximum factor of the division ratio used to generate the
 *  controller clock (MCLK). The maximum is 0, which is interpreted as 1024.
 *         MCLK 分频系数最大值（0 解释为 1024）。
 */
#define DL_I2S_MDIV_MAXIMUM                                      ((uint32_t)0U)

/**
 *  @brief I2S Invalid factor of the division ratio used to generate the
 *  controller clock (MCLK). This is 1.
 *         MCLK 分频系数无效值（1）。
 */
#define DL_I2S_MDIV_INVALID                                      ((uint32_t)1U)

/**
 *  @brief I2S Minimum factor of the division ratio used to generate BCLK. The
 *  minimum is 2, since the 0 is interpreted as 1024, which is the maximum
 *  factor, and 1 is invalid.
 *         BCLK 分频系数最小值。
 */
#define DL_I2S_BDIV_MINIMUM                                      ((uint32_t)2U)

/**
 *  @brief I2S Maximum factor of the division ratio used to generate BCLK.
 *  The maximum is 0, which is interpreted as 1024.
 *         BCLK 分频系数最大值（0 解释为 1024）。
 */
#define DL_I2S_BDIV_MAXIMUM                                      ((uint32_t)0U)

/**
 *  @brief I2S Invalid factor of the division ratio used to generate BCLK. This
 *  is 1.
 *         BCLK 分频系数无效值（1）。
 */
#define DL_I2S_BDIV_INVALID                                      ((uint32_t)1U)

/** @addtogroup DL_I2S_INTERRUPT
 *  @{
 */
/*!
 * @brief WCLK Error Interrupt
 *        WCLK 错误中断。
 */
#define DL_I2S_INTERRUPT_WCLK_ERROR              (I2S_CPU_INT_IMASK_WCLKERR_EN)

/*!
 * @brief RXFIFO Trigger when >= trigger level Interrupt
 *        RXFIFO 达到或超过触发阈值中断。
 */
#define DL_I2S_INTERRUPT_RXFIFO_TRIGGER           (I2S_CPU_INT_IMASK_RXINT_SET)

/*!
 * @brief TXFIFO when <= trigger level Interrupt
 *        TXFIFO 低于或等于触发阈值中断。
 */
#define DL_I2S_INTERRUPT_TXFIFO_TRIGGER           (I2S_CPU_INT_IMASK_TXINT_SET)

/*!
 * @brief DMA Done on RX Event Channel Interrupt
 *        DMA 接收通道完成中断。
 */
#define DL_I2S_INTERRUPT_DMA_DONE_RX        (I2S_CPU_INT_IMASK_DMA_DONE_RX_SET)

/*!
 * @brief DMA Done on TX Event Channel Interrupt
 *        DMA 发送通道完成中断。
 */
#define DL_I2S_INTERRUPT_DMA_DONE_TX        (I2S_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief RXFIFO Overflow Event Interrupt
 *        RXFIFO 溢出中断。
 */
#define DL_I2S_INTERRUPT_RXFIFO_OVERFLOW     (I2S_CPU_INT_IMASK_RXFIFO_OVF_SET)

/*!
 * @brief TXFIFO Underflow Event Interrupt
 *        TXFIFO 下溢中断。
 */
#define DL_I2S_INTERRUPT_TXFIFO_UNDERFLOW    (I2S_CPU_INT_IMASK_TXFIFO_UNF_SET)

/** @}*/

/** @addtogroup DL_I2S_DMA_INTERRUPT
 *  @{
 */
/*!
 * @brief Transmit interrupt for DMA trigger
 *        DMA 发送触发中断。
 */
#define DL_I2S_DMA_INTERRUPT_TX_TRIGGER       (I2S_DMA_TRIG_TX_IMASK_TXINT_SET)

/*!
 * @brief Receive interrupt for DMA trigger
 *        DMA 接收触发中断。
 */
#define DL_I2S_DMA_INTERRUPT_RX_TRIGGER       (I2S_DMA_TRIG_RX_IMASK_RXINT_SET)

/** @}*/

    /* clang-format on */

    /** @enum DL_I2S_WORD_BAUD_CLOCK_SOURCE
     *  音频时钟（WCLK/BCLK）来源选择。
     */
    typedef enum
    {
        /*! No source for audio clocks WCLK and BCLK *//*< 无时钟源 */
        DL_I2S_WORD_BAUD_CLOCK_SOURCE_NONE = I2S_WCLKSRC_WBCLKSRC_NONE,
        /*! External source for audio clocks WCLK and BCLK from pin *//*< 外部引脚输入 */
        DL_I2S_WORD_BAUD_CLOCK_SOURCE_EXTERNAL = I2S_WCLKSRC_WBCLKSRC_EXT,
        /*! Internal source for audio clocks WCLK and BCLK from module PRCM *//*< 内部 PRCM 模块提供 */
        DL_I2S_WORD_BAUD_CLOCK_SOURCE_INTERNAL = I2S_WCLKSRC_WBCLKSRC_INT,
    } DL_I2S_WORD_BAUD_CLOCK_SOURCE;

    /** @enum DL_I2S_CLOCK_SOURCE
     *  I2S 主时钟源选择。
     */
    typedef enum
    {
        /*! Select SYSOSC as the clock source for I2S *//*< 选择 SYSOSC */
        DL_I2S_CLOCK_SOURCE_SYSOSC = I2S_CLKCFG_DAICLK_SYSOSC,
        /*! Select HFXT as the clock source for I2S *//*< 选择 HFXT */
        DL_I2S_CLOCK_SOURCE_HFXT = I2S_CLKCFG_DAICLK_HFXT,
        /*! Select PLL as the clock source for I2S *//*< 选择 PLL */
        DL_I2S_CLOCK_SOURCE_PLL = I2S_CLKCFG_DAICLK_PLL,
    } DL_I2S_CLOCK_SOURCE;

    /** @enum DL_I2S_WCLK_INVERSION
     *  WCLK 极性反转控制。
     */
    typedef enum
    {
        /*! WCLK is not inverted *//*< 不反转 */
        DL_I2S_WCLK_INVERSION_DISABLED = I2S_WCLKSRC_WCLKINV_NOT_INVERTED,
        /*! WCLK is inverted *//*< 反转 */
        DL_I2S_WCLK_INVERSION_ENABLED = I2S_WCLKSRC_WCLKINV_INVERTED,
    } DL_I2S_WCLK_INVERSION;

    /** @enum DL_I2S_PHASE
     *  帧格式相位模式。
     */
    typedef enum
    {
        /*! I2S Single-Phase (DSP format) *//*< 单相位（DSP 格式） */
        DL_I2S_PHASE_SINGLE = I2S_FMTCFG_DUALPHASE_SINGLEPHASE,
        /*! I2S Dual-Phase (I2S, LJF and RJF) *//*< 双相位（I2S/LJF/RJF 格式） */
        DL_I2S_PHASE_DUAL = I2S_FMTCFG_DUALPHASE_DUALPHASE,
    } DL_I2S_PHASE;

    /** @enum DL_I2S_SAMPLE_EDGE
     *  数据采样边沿选择。
     */
    typedef enum
    {
        /*! Data sampled on negative edge, clocked out on positive edge *//*< 下降沿采样，上升沿输出 */
        DL_I2S_SAMPLE_EDGE_NEG = I2S_FMTCFG_SMPLEDGE_NEGEDGE,
        /*! Data sampled on positive edge, clocked out on negative edge *//*< 上升沿采样，下降沿输出 */
        DL_I2S_SAMPLE_EDGE_POS = I2S_FMTCFG_SMPLEDGE_POSEDGE,
    } DL_I2S_SAMPLE_EDGE;

    /** @enum DL_I2S_MEMORY_LENGTH
     *  每个样本的内存访问宽度。
     */
    typedef enum
    {
        /*! 16-bit memory access per sample *//*< 16 位 */
        DL_I2S_MEMORY_LENGTH_16_BIT = I2S_FMTCFG_MEMLEN32_16BIT,
        /*! 32-bit memory access per sample *//*< 32 位 */
        DL_I2S_MEMORY_LENGTH_32_BIT = I2S_FMTCFG_MEMLEN32_32BIT,
    } DL_I2S_MEMORY_LENGTH;

    /** @enum DL_I2S_DATA_DELAY
     *  WCLK 边沿到首字最高位之间的 BCLK 延迟周期数。
     */
    typedef enum
    {
        /*! Zero BCLK period delay between WCLK edge and MSB of first word *//*< 零延迟（LJF/DSP） */
        DL_I2S_DATA_DELAY_ZERO = I2S_FMTCFG_DATADLY_ZERO,
        /*! One BCLK period delay between WCLK edge and MSB of first word *//*< 1 周期延迟（I2S/DSP） */
        DL_I2S_DATA_DELAY_ONE = I2S_FMTCFG_DATADLY_ONE,
        /*! Two BCLK periods delay between WCLK edge and MSB of first word *//*< 2 周期延迟（RJF 最小值） */
        DL_I2S_DATA_DELAY_TWO = I2S_FMTCFG_DATADLY_TWO,
        /*! Max BCLK periods delay between WCLK edge and MSB of first word *//*< 最大延迟（RJF） */
        DL_I2S_DATA_DELAY_MAX = I2S_FMTCFG_DATADLY_MAX,
    } DL_I2S_DATA_DELAY;

    /** @enum DL_I2S_EMPTY_SLOT_OUTPUT
     *  空时隙期间数据引脚输出行为。
     */
    typedef enum
    {
        /*! Send out zeroes in empty slots *//*< 输出全零 */
        DL_I2S_EMPTY_SLOT_OUTPUT_ZERO = I2S_FMTCFG_EMPTYSLOTOUTPUT_ZERO,
        /*! Send out ones in empty slots *//*< 输出全一 */
        DL_I2S_EMPTY_SLOT_OUTPUT_ONE = I2S_FMTCFG_EMPTYSLOTOUTPUT_ONE,
        /*! Tristate data pin line during empty slot *//*< 三态高阻 */
        DL_I2S_EMPTY_SLOT_OUTPUT_TRISTATE = I2S_FMTCFG_EMPTYSLOTOUTPUT_TRISTATE,
    } DL_I2S_EMPTY_SLOT_OUTPUT;

    /** @enum DL_I2S_WCLK_PHASE
     *  WCLK 分频比计算模式。
     */
    typedef enum
    {
        /*! Calculate WCLK division ratio for single phase operation *//*< 按单相位计算分频比 */
        DL_I2S_WCLK_PHASE_SINGLE = (0U << I2S_CLKCTL_WCLKPHASE_OFS),
        /*! Calculate WCLK division ratio for dual phase operation *//*< 按双相位计算分频比 */
        DL_I2S_WCLK_PHASE_DUAL = (1U << I2S_CLKCTL_WCLKPHASE_OFS),
        /*! Calculate user-defined custom WCLK division ratio *//*< 用户自定义分频比 */
        DL_I2S_WCLK_PHASE_CUSTOM = (2U << I2S_CLKCTL_WCLKPHASE_OFS),
    } DL_I2S_WCLK_PHASE;

    /** @enum DL_I2S_MODE
     *  I2S 工作模式。
     */
    typedef enum
    {
        /*! I2S controller mode. Module outputs audio clock signals (WCLK, BCLK) *//*< 控制器模式：模块输出时钟 */
        DL_I2S_MODE_CONTROLLER = 0U,
        /*! I2S target mode. Audio clock signals are inputs (WCLK, BCLK) *//*< 目标模式：时钟由外部输入 */
        DL_I2S_MODE_TARGET = 1U,
    } DL_I2S_MODE;

    /** @enum DL_I2S_DATA_PIN_DIRECTION
     *  数据引脚方向配置。
     */
    typedef enum
    {
        /*! Data pin not in use (disabled) *//*< 未使用（禁用） */
        DL_I2S_DATA_PIN_DIRECTION_UNUSED = 0U,
        /*! Data pin acts as input *//*< 输入 */
        DL_I2S_DATA_PIN_DIRECTION_INPUT = 1U,
        /*! Data pin acts as output *//*< 输出 */
        DL_I2S_DATA_PIN_DIRECTION_OUTPUT = 2U,
    } DL_I2S_DATA_PIN_DIRECTION;

    /** @enum DL_I2S_TX_FIFO_LEVEL
     *  TX FIFO 中断触发阈值。
     */
    typedef enum
    {
        /*! Trigger when the TX FIFO <= 3/4 empty *//*< 空闲 <= 3/4 时触发 */
        DL_I2S_TX_FIFO_LEVEL_3_4_EMPTY = I2S_IFLS_TXIFLSEL_LVL_3_4,
        /*! Trigger when TX FIFO <= 1/2 empty (default) *//*< 空闲 <= 1/2 时触发（默认） */
        DL_I2S_TX_FIFO_LEVEL_1_2_EMPTY = I2S_IFLS_TXIFLSEL_LVL_1_2,
        /*! Trigger when TX FIFO <= 1/4 empty *//*< 空闲 <= 1/4 时触发 */
        DL_I2S_TX_FIFO_LEVEL_1_4_EMPTY = I2S_IFLS_TXIFLSEL_LVL_1_4,
        /*! Trigger when TX FIFO is empty *//*< FIFO 完全空时触发 */
        DL_I2S_TX_FIFO_LEVEL_EMPTY = I2S_IFLS_TXIFLSEL_LVL_EMPTY,
        /*! Trigger when TX FIFO <= 1 *//*< 仅剩 1 个条目时触发 */
        DL_I2S_TX_FIFO_LEVEL_ONE_ENTRY = I2S_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY,
        /*! Trigger when TX FIFO >= (MAX_FIFO_LEN - 1) *//*< 接近满时触发 */
        DL_I2S_TX_FIFO_LEVEL_ALMOST_FULL = I2S_IFLS_TXIFLSEL_LVL_ALMOST_FULL,
    } DL_I2S_TX_FIFO_LEVEL;

    /** @enum DL_I2S_RX_FIFO_LEVEL
     *  RX FIFO 中断触发阈值。
     */
    typedef enum
    {
        /*! Trigger when RX FIFO >= 1/4 full *//*< 满 >= 1/4 时触发 */
        DL_I2S_RX_FIFO_LEVEL_1_4_FULL = I2S_IFLS_RXIFLSEL_LVL_1_4,
        /*! Trigger when RX FIFO >= 1/2 full (default) *//*< 满 >= 1/2 时触发（默认） */
        DL_I2S_RX_FIFO_LEVEL_1_2_FULL = I2S_IFLS_RXIFLSEL_LVL_1_2,
        /*! Trigger when RX FIFO >= 3/4 full *//*< 满 >= 3/4 时触发 */
        DL_I2S_RX_FIFO_LEVEL_3_4_FULL = I2S_IFLS_RXIFLSEL_LVL_3_4,
        /*! Trigger when RX FIFO is full *//*< FIFO 完全满时触发 */
        DL_I2S_RX_FIFO_LEVEL_FULL = I2S_IFLS_RXIFLSEL_LVL_FULL,
        /*! Trigger when RX FIFO >= (MAX_FIFO_LEN - 1) *//*< 接近满时触发 */
        DL_I2S_RX_FIFO_LEVEL_ALMOST_FULL = I2S_IFLS_RXIFLSEL_LVL_ALMOST_FULL,
        /*! Trigger when RX FIFO <= 1 *//*< 仅剩 1 个条目时触发 */
        DL_I2S_RX_FIFO_LEVEL_ONE_ENTRY = I2S_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY,
    } DL_I2S_RX_FIFO_LEVEL;

    /*! @enum DL_I2S_IIDX
     *  中断索引，标识最高优先级待处理中断类型。
     */
    typedef enum
    {
        /*! Interrupt index for I2S if no interrupt is pending *//*< 无中断 */
        DL_I2S_IIDX_NO_INT = I2S_IIDX_STAT_NO_INTR,
        /*! Interrupt index for I2S WCLK Error *//*< WCLK 错误 */
        DL_I2S_IIDX_WCLK_ERR = I2S_IIDX_STAT_WCLKERR,
        /*! Interrupt index for I2S Receive Interrupt *//*< 接收完成 */
        DL_I2S_IIDX_RX_DONE = I2S_IIDX_STAT_RXIFG,
        /*! Interrupt index for I2S Transmit Interrupt *//*< 发送完成 */
        DL_I2S_IIDX_TX_DONE = I2S_IIDX_STAT_TXIFG,
        /*! Interrupt index for I2S RXFIFO Overflow Event *//*< RXFIFO 溢出 */
        DL_I2S_IIDX_RXFIFO_OVERFLOW = I2S_IIDX_STAT_RXFIFO_OVF_EVT,
        /*! Interrupt index for I2S TXFIFO Underflow Event *//*< TXFIFO 下溢 */
        DL_I2S_IIDX_TXFIFO_UNDERFLOW = I2S_IIDX_STAT_TXFIFO_UNF_EVT,
        /*! Interrupt index for DMA Done on RX *//*< DMA 接收完成 */
        DL_I2S_IIDX_DMA_DONE_RX = I2S_IIDX_STAT_DMA_DONE_RX,
        /*! Interrupt index for DMA Done on TX *//*< DMA 发送完成 */
        DL_I2S_IIDX_DMA_DONE_TX = I2S_IIDX_STAT_DMA_DONE_TX,
    } DL_I2S_IIDX;

    /**
     * @brief  Configuration struct for @ref DL_I2S_setClockConfig.
     *         I2S 音频时钟配置结构体。
     */
    typedef struct
    {
        /*! I2S clock source. One of @ref DL_I2S_CLOCK_SOURCE *//*< I2S 主时钟源 */
        DL_I2S_CLOCK_SOURCE clockSel;
        /*! Audio clocks (WCLK, BCLK) source. One of @ref DL_I2S_WORD_BAUD_CLOCK_SOURCE *//*< 音频时钟来源 */
        DL_I2S_WORD_BAUD_CLOCK_SOURCE wordBaudClockSource;
        /*! WCLK phase. One of @ref DL_I2S_WCLK_PHASE *//*< WCLK 分频计算模式 */
        DL_I2S_WCLK_PHASE wclkPhase;
        /*! WCLK divide ratio. [0x1, 0xFFFF] *//*< WCLK 分频系数 */
        uint32_t wclkDivider;
        /*! BCLK divide ratio. [ @ref DL_I2S_BDIV_MINIMUM, @ref DL_I2S_BDIV_MAXIMUM] *//*< BCLK 分频系数 */
        uint32_t bclkDivider;
    } DL_I2S_ClockConfig;

    /**
     * @brief  Configuration struct for @ref DL_I2S_init.
     *         I2S 初始化配置结构体。
     */
    typedef struct
    {
        /*! Controller or target mode. One of @ref DL_I2S_MODE *//*< 工作模式 */
        DL_I2S_MODE mode;
        /*! WCLK inversion. One of @ref DL_I2S_WCLK_INVERSION *//*< WCLK 极性 */
        DL_I2S_WCLK_INVERSION wclkInvert;
        /*! Single or dual-phase. One of @ref DL_I2S_PHASE *//*< 帧相位格式 */
        DL_I2S_PHASE phase;
        /*! Positive or negative sampling edge. One of @ref DL_I2S_SAMPLE_EDGE *//*< 采样边沿 */
        DL_I2S_SAMPLE_EDGE samplingEdge;
        /*! Number of bits per sample word. [7, 31]. See @ref DL_I2S_setSampleWordLength *//*< 样本字长（位） */
        uint32_t sampleWordLength;
        /*! Data delay. One of @ref DL_I2S_DATA_DELAY *//*< 数据延迟 */
        DL_I2S_DATA_DELAY dataDelay;
        /*! Empty slot output. One of @ref DL_I2S_EMPTY_SLOT_OUTPUT *//*< 空时隙输出行为 */
        DL_I2S_EMPTY_SLOT_OUTPUT emptySlotOutput;
        /*! Memory access length. One of @ref DL_I2S_MEMORY_LENGTH *//*< 内存访问宽度 */
        DL_I2S_MEMORY_LENGTH memoryAccessLength;
        /*! Data pin 0 (AD0) direction. One of @ref DL_I2S_DATA_PIN_DIRECTION *//*< AD0 引脚方向 */
        DL_I2S_DATA_PIN_DIRECTION dataPin0Direction;
        /*! Valid channel mask for a frame on a data pin. [0, 0xFF] *//*< AD0 有效通道掩码 */
        uint32_t dataPin0ValidChannelMask;
#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
        /*! Data pin 1 (AD1) direction. One of @ref DL_I2S_DATA_PIN_DIRECTION *//*< AD1 引脚方向 */
        DL_I2S_DATA_PIN_DIRECTION dataPin1Direction;
        /*! Valid channel mask for a frame on a data pin. [0, 0xFF] *//*< AD1 有效通道掩码 */
        uint32_t dataPin1ValidChannelMask;
#endif
        /*! MCLK divide ratio. [ @ref DL_I2S_MDIV_MINIMUM, @ref DL_I2S_MDIV_MAXIMUM] *//*< MCLK 分频系数 */
        uint32_t mclkDivider;
        /*! Enable the optional MCLK signal. Controller mode only *//*< 是否使能 MCLK 输出（仅控制器模式） */
        bool enableMCLK;
    } DL_I2S_Config;

    /**
     *  @brief      Configure I2S audio clocks (WCLK and BCLK)
     *  @details    功能：配置 I2S 音频时钟源及 WCLK/BCLK 分频参数。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the
     *                      peripheral
     *                      参数：I2S 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                      @ref DL_I2S_ClockConfig.
     *                      参数：时钟配置结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_I2S_setClockConfig(I2S_Regs *i2s, const DL_I2S_ClockConfig *config);

    /**
     *  @brief      Get I2S audio clocks configuration (WCLK and BCLK)
     *  @details    功能：读取当前 I2S 音频时钟配置。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the
     *                      peripheral
     *                      参数：I2S 外设寄存器基址。
     *  @param[out] config  Pointer to the clock configuration struct
     *                      @ref DL_I2S_ClockConfig.
     *                      参数：用于接收配置的输出结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_I2S_getClockConfig(const I2S_Regs *i2s, DL_I2S_ClockConfig *config);

    /**
     *  @brief     Initialize the I2S peripheral
     *
     *  Initializes all the common configurable options for the I2S peripheral. Any
     *  other custom configuration can be done after calling this API. The I2S is
     *  not enabled in this API.
     *  中文说明：完成 I2S 关键参数初始化，但不在此接口内使能外设。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      参数：I2S 外设寄存器基址。
     *  @param[in]  config  Configuration for I2S peripheral
     *                      参数：I2S 初始化配置结构体。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_I2S_init(I2S_Regs *i2s, const DL_I2S_Config *config);

    /**
     *  @brief      Writes 8-bit data into the TX FIFO for transmit
     *              向 TX FIFO 写入 8 位数据（不检查状态）。
     *
     *  Puts the data into the TX FIFO without checking its status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    待发送 8 位数据。
     *
     *  @sa         DL_I2S_transmitDataBlocking8
     *  @sa         DL_I2S_transmitDataCheck8
     */
    __STATIC_INLINE void DL_I2S_transmitData8(I2S_Regs *i2s, uint8_t data)
    {
        i2s->TXDATA = data; // 写入 TX FIFO
    }

    /**
     *  @brief      Writes 16-bit data into the TX FIFO for transmit
     *              向 TX FIFO 写入 16 位数据（不检查状态）。
     *
     *  Puts the data into the TX FIFO without checking its status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    待发送 16 位数据。
     *
     *  @sa         DL_I2S_transmitDataBlocking16
     *  @sa         DL_I2S_transmitDataCheck16
     */
    __STATIC_INLINE void DL_I2S_transmitData16(I2S_Regs *i2s, uint16_t data)
    {
        i2s->TXDATA = data; // 写入 TX FIFO
    }

    /**
     *  @brief      Writes 32-bit data into the TX FIFO for transmit
     *              向 TX FIFO 写入 32 位数据（不检查状态）。
     *
     *  Puts the data into the TX FIFO without checking its status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: A 32-bit write will be written as one FIFO entry. The application can
     *  choose to pack two 16-bit frames into one word.
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    待发送 32 位数据。
     *
     *  @sa         DL_I2S_transmitDataBlocking32
     *  @sa         DL_I2S_transmitDataCheck32
     */
    __STATIC_INLINE void DL_I2S_transmitData32(I2S_Regs *i2s, uint32_t data)
    {
        i2s->TXDATA = data; // 写入 TX FIFO
    }

    /**
     *  @brief      Reads 8-bit data from the RX FIFO
     *              从 RX FIFO 读取 8 位数据（不检查状态）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              读取到的 8 位样本。
     *
     *  @sa         DL_I2S_receiveDataBlocking8
     *  @sa         DL_I2S_receiveDataCheck8
     */
    __STATIC_INLINE uint8_t DL_I2S_receiveData8(const I2S_Regs *i2s)
    {
        return ((uint8_t)(i2s->RXDATA)); // 从 RX FIFO 读取
    }

    /**
     *  @brief      Reads 16-bit data from the RX FIFO
     *              从 RX FIFO 读取 16 位数据（不检查状态）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              读取到的 16 位样本。
     *
     *  @sa         DL_I2S_receiveDataBlocking16
     *  @sa         DL_I2S_receiveDataCheck16
     */
    __STATIC_INLINE uint16_t DL_I2S_receiveData16(const I2S_Regs *i2s)
    {
        return ((uint16_t)(i2s->RXDATA)); // 从 RX FIFO 读取
    }

    /**
     *  @brief      Reads 32-bit data from the RX FIFO
     *              从 RX FIFO 读取 32 位数据（不检查状态）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              读取到的 32 位样本。
     *
     *  @sa         DL_I2S_receiveDataBlocking32
     *  @sa         DL_I2S_receiveDataCheck32
     */
    __STATIC_INLINE uint32_t DL_I2S_receiveData32(const I2S_Regs *i2s)
    {
        return (i2s->RXDATA); // 从 RX FIFO 读取
    }

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts the data into the TX FIFO after blocking to ensure the TX FIFO is not
     *  full. Will wait indefinitely until there is space in the TX FIFO. See
     *  related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 8 位数据。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 8 位样本。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_I2S_transmitData8
     *  @sa         DL_I2S_transmitDataCheck8
     * @details    功能：参见上方英文说明。
     */
    void DL_I2S_transmitDataBlocking8(I2S_Regs *i2s, uint8_t data);

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts the data into the TX FIFO after blocking to ensure the TX FIFO is not
     *  full. Will wait indefinitely until there is space in the TX FIFO. See
     *  related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 16 位数据。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 16 位样本。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_I2S_transmitData16
     *  @sa         DL_I2S_transmitDataCheck16
     * @details    功能：参见上方英文说明。
     */
    void DL_I2S_transmitDataBlocking16(I2S_Regs *i2s, uint16_t data);

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts the data into the TX FIFO after blocking to ensure the TX FIFO is not
     *  full. Will wait indefinitely until there is space in the TX FIFO. See
     *  related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: A 32-bit write will be written as one FIFO entry. The application can
     *  choose to pack two 16-bit frames into one word.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 32 位数据。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 32 位样本。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_I2S_transmitData32
     *  @sa         DL_I2S_transmitDataCheck32
     * @details    功能：参见上方英文说明。
     */
    void DL_I2S_transmitDataBlocking32(I2S_Regs *i2s, uint32_t data);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefinitely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：阻塞等待 RX FIFO 有数据后读取 8 位样本。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的 8 位样本。
     *
     *  @sa         DL_I2S_receiveData16
     *  @sa         DL_I2S_receiveDataCheck16
     * @details    功能：参见上方英文说明。
     */
    uint8_t DL_I2S_receiveDataBlocking8(const I2S_Regs *i2s);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefinitely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：阻塞等待 RX FIFO 有数据后读取 16 位样本。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的 16 位样本。
     *
     *  @sa         DL_I2S_receiveData16
     *  @sa         DL_I2S_receiveDataCheck16
     * @details    功能：参见上方英文说明。
     */
    uint16_t DL_I2S_receiveDataBlocking16(const I2S_Regs *i2s);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefinitely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *  中文说明：阻塞等待 RX FIFO 有数据后读取 32 位样本。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的 32 位样本。
     *
     *  @sa         DL_I2S_receiveData32
     *  @sa         DL_I2S_receiveDataCheck32
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_I2S_receiveDataBlocking32(const I2S_Regs *i2s);

    /**
     *  @brief      Checks the TX FIFO before trying to transmit data
     *
     *  Checks if the TX FIFO is already full before trying to add new data to the
     *  FIFO. Exits immediately if full rather than trying to block. See related
     *  APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：非阻塞发送 8 位数据，FIFO 满时立即返回。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 8 位样本。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 成功写入，`false` 表示 FIFO 满。
     *
     *  @sa         DL_I2S_transmitData8
     *  @sa         DL_I2S_transmitDataBlocking8
     * @details    功能：参见上方英文说明。
     */
    bool DL_I2S_transmitDataCheck8(I2S_Regs *i2s, uint8_t data);

    /**
     *  @brief      Checks the TX FIFO before trying to transmit data
     *
     *  Checks if the TX FIFO is already full before trying to add new data to the
     *  FIFO. Exits immediately if full rather than trying to block. See related
     *  APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：非阻塞发送 16 位数据，FIFO 满时立即返回。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 16 位样本。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 成功写入，`false` 表示 FIFO 满。
     *
     *  @sa         DL_I2S_transmitData16
     *  @sa         DL_I2S_transmitDataBlocking16
     * @details    功能：参见上方英文说明。
     */
    bool DL_I2S_transmitDataCheck16(I2S_Regs *i2s, uint16_t data);

    /**
     *  @brief      Checks the TX FIFO before trying to transmit data
     *
     *  Checks if the TX FIFO is already full before trying to add new data to the
     *  FIFO. Exits immediately if full rather than trying to block. See related
     *  APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: A 32-bit write will be written as one FIFO entry. The application can
     *  choose to pack two 16-bit frames into one word.
     *  中文说明：非阻塞发送 32 位数据，FIFO 满时立即返回。
     *
     *  @param[in]  i2s   pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 32 位样本。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 成功写入，`false` 表示 FIFO 满。
     *
     *  @sa         DL_I2S_transmitData32
     *  @sa         DL_I2S_transmitDataBlocking32
     * @details    功能：参见上方英文说明。
     */
    bool DL_I2S_transmitDataCheck32(I2S_Regs *i2s, uint32_t data);

    /**
     *  @brief      Checks the RX FIFO before trying to receive data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：非阻塞读取 8 位数据，FIFO 空时立即返回。
     *
     *  @param[in]  i2s    pointer to the register overlay for the peripheral
     *                     参数：I2S 外设寄存器基址。
     *  @param[out]  buffer a buffer to write the received data into
     *                      参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @sa         DL_I2S_receiveData8
     *  @sa         DL_I2S_receiveDataBlocking8
     * @details    功能：参见上方英文说明。
     */
    bool DL_I2S_receiveDataCheck8(const I2S_Regs *i2s, uint8_t *buffer);

    /**
     *  @brief      Checks the RX FIFO before trying to receive data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：非阻塞读取 16 位数据，FIFO 空时立即返回。
     *
     *  @param[in]  i2s    pointer to the register overlay for the peripheral
     *                     参数：I2S 外设寄存器基址。
     *  @param[out]  buffer a buffer to write the received data into
     *                      参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @sa         DL_I2S_receiveData16
     *  @sa         DL_I2S_receiveDataBlocking16
     * @details    功能：参见上方英文说明。
     */
    bool DL_I2S_receiveDataCheck16(const I2S_Regs *i2s, uint16_t *buffer);

    /**
     *  @brief      Checks the RX FIFO before trying to receive data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *  中文说明：非阻塞读取 32 位数据，FIFO 空时立即返回。
     *
     *  @param[in]  i2s    pointer to the register overlay for the peripheral
     *                     参数：I2S 外设寄存器基址。
     *  @param[out]  buffer a buffer to write the received data into
     *                      参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @sa         DL_I2S_receiveData32
     *  @sa         DL_I2S_receiveDataBlocking32
     * @details    功能：参见上方英文说明。
     */
    bool DL_I2S_receiveDataCheck32(const I2S_Regs *i2s, uint32_t *buffer);

    /**
     *  @brief       Read all available data out of the RX FIFO using 8 bit access
     *  @details     功能：批量读取 RX FIFO 中可用的 8 位数据。
     *
     *  @param[in]   i2s       Pointer to the register overlay for the peripheral
     *                         参数：I2S 外设寄存器基址。
     *  @param[out]  buffer    Buffer to write received data into
     *                         参数：接收缓冲区。
     *  @param[in]   maxCount  Max number of halfwords to read from the RX FIFO
     *                         参数：最大读取数量。
     *
     *  @return      Number of bytes read from the RX FIFO
     *               返回值：实际读取的字节数。
     */
    uint32_t DL_I2S_drainRXFIFO8(
        const I2S_Regs *i2s, uint8_t *buffer, uint32_t maxCount);

    /**
     *  @brief       Read all available data out of the RX FIFO using 16 bit access
     *  @details     功能：批量读取 RX FIFO 中可用的 16 位数据。
     *
     *  @param[in]   i2s       Pointer to the register overlay for the peripheral
     *                         参数：I2S 外设寄存器基址。
     *  @param[out]  buffer    Buffer to write received data into
     *                         参数：接收缓冲区。
     *  @param[in]   maxCount  Max number of halfwords to read from the RX FIFO
     *                         参数：最大读取数量。
     *
     *  @return      Number of halfwords read from the RX FIFO
     *               返回值：实际读取的半字数。
     */
    uint32_t DL_I2S_drainRXFIFO16(
        const I2S_Regs *i2s, uint16_t *buffer, uint32_t maxCount);

    /**
     *  @brief       Read all available data out of the RX FIFO using 32 bit access
     *
     *  NOTE: A 32-bit write will be written as one FIFO entry. The application can
     *  choose to pack two 16-bit frames into one word.
     *  中文说明：批量读取 RX FIFO 中可用的 32 位数据。
     *
     *  @param[in]   i2s       Pointer to the register overlay for the peripheral
     *                         参数：I2S 外设寄存器基址。
     *  @param[out]  buffer    Buffer to write received data into
     *                         参数：接收缓冲区。
     *  @param[in]   maxCount  Max number of words to read from the RX FIFO
     *                         参数：最大读取数量。
     *
     *  @return      Number of words read from the RX FIFO
     *               返回值：实际读取的字数。
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_I2S_drainRXFIFO32(
        const I2S_Regs *i2s, uint32_t *buffer, uint32_t maxCount);

    /**
     *  @brief      Fill the TX FIFO using 8 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *  中文说明：批量写入 8 位数据到 TX FIFO，直到 FIFO 满或达到上限。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                     参数：I2S 外设寄存器基址。
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *                     参数：发送缓冲区。
     *  @param[in]  count   Max number of halfwords to write to the TX FIFO
     *                     参数：最大写入数量。
     *
     *  @return     Number of bytes written to the TX FIFO
     *              返回值：实际写入的字节数。
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_I2S_fillTXFIFO8(
        I2S_Regs *i2s, const uint8_t *buffer, uint32_t count);

    /**
     *  @brief      Fill the TX FIFO using 16 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *  中文说明：批量写入 16 位数据到 TX FIFO，直到 FIFO 满或达到上限。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                     参数：I2S 外设寄存器基址。
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *                     参数：发送缓冲区。
     *  @param[in]  count   Max number of halfwords to write to the TX FIFO
     *                     参数：最大写入数量。
     *
     *  @return     Number of halfwords written to the TX FIFO
     *              返回值：实际写入的半字数。
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_I2S_fillTXFIFO16(
        I2S_Regs *i2s, const uint16_t *buffer, uint32_t count);

    /**
     *  @brief      Fill the TX FIFO using 32 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *
     *  NOTE: A 32-bit write will be written as one FIFO entry. The application can
     *  choose to pack two 16-bit frames into one word.
     *  中文说明：批量写入 32 位数据到 TX FIFO，直到 FIFO 满或达到上限。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                     参数：I2S 外设寄存器基址。
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *                     参数：发送缓冲区。
     *  @param[in]  count   Max number of words to write to the TX FIFO
     *                     参数：最大写入数量。
     *
     *  @return     Number of words written to the TX FIFO
     *              返回值：实际写入的字数。
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_I2S_fillTXFIFO32(
        I2S_Regs *i2s, const uint32_t *buffer, uint32_t count);

    /**
     *  @brief      Clears contents of TX FIFO
     *
     *  @note When a FIFO clear has been initiated, do not try to re-clear again.
     *  中文说明：触发 TX FIFO 清空并等待硬件完成。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   参数：I2S 外设寄存器基址。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_I2S_clearTXFIFO(I2S_Regs *i2s);

    /**
     *  @brief      Clears contents of RX FIFO
     *
     *  @note When a FIFO clear has been initiated, do not try to re-clear again.
     *  中文说明：触发 RX FIFO 清空并等待硬件完成。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   参数：I2S 外设寄存器基址。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_I2S_clearRXFIFO(I2S_Regs *i2s);

    /**
     *  @brief      Checks if TX FIFO is full
     *              检查 TX FIFO 是否已满。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     If TX FIFO is full
     *              TX FIFO 是否已满。
     *
     *  @retval     true if TX FIFO is full
     *  @retval     false if TX FIFO is not full
     */
    __STATIC_INLINE bool DL_I2S_isTXFIFOFull(const I2S_Regs *i2s)
    {
        return ((i2s->STAT & I2S_STAT_TXFF_MASK) == I2S_STAT_TXFF_SET);
    }

    /**
     *  @brief      Checks if TX FIFO is empty
     *              检查 TX FIFO 是否为空。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     If TX FIFO is empty
     *              TX FIFO 是否为空。
     *
     *  @retval     true if TX FIFO is empty
     *  @retval     false if TX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2S_isTXFIFOEmpty(const I2S_Regs *i2s)
    {
        return ((i2s->STAT & I2S_STAT_TXFE_MASK) == I2S_STAT_TXFE_SET);
    }

    /**
     *  @brief      Checks if RX FIFO is full
     *              检查 RX FIFO 是否已满。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     If RX FIFO is full
     *              RX FIFO 是否已满。
     *
     *  @retval     true if RX FIFO is full
     *  @retval     false if RX FIFO is not full
     */
    __STATIC_INLINE bool DL_I2S_isRXFIFOFull(const I2S_Regs *i2s)
    {
        return ((i2s->STAT & I2S_STAT_RXFF_MASK) == I2S_STAT_RXFF_SET);
    }

    /**
     *  @brief      Checks if RX FIFO is empty
     *              检查 RX FIFO 是否为空。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     If RX FIFO is empty
     *              RX FIFO 是否为空。
     *
     *  @retval     true if RX FIFO is empty
     *  @retval     false if RX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2S_isRXFIFOEmpty(const I2S_Regs *i2s)
    {
        return ((i2s->STAT & I2S_STAT_RXFE_MASK) == I2S_STAT_RXFE_SET);
    }

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the I2S
     *        使能 I2S 外设电源写入寄存器。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_enablePower(I2S_Regs *i2s)
    {
        i2s->GPRCM.PWREN = (I2S_PWREN_KEY_UNLOCK_W | I2S_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the I2S
     *        禁用 I2S 外设电源写入寄存器。
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @note This API does not provide large power savings. For power savings,
     *  please refer to @ref DL_I2S_enable
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_disablePower(I2S_Regs *i2s)
    {
        i2s->GPRCM.PWREN = (I2S_PWREN_KEY_UNLOCK_W | I2S_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the I2S
     *        is enabled
     *        查询 I2S 外设电源是否已使能。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     Whether power is enabled
     *              电源是否已使能。
     *
     *  @retval     true if peripheral register access is enabled
     *  @retval     false if peripheral register access is disabled
     */
    __STATIC_INLINE bool DL_I2S_isPowerEnabled(const I2S_Regs *i2s)
    {
        return (
            (i2s->GPRCM.PWREN & I2S_PWREN_ENABLE_MASK) == I2S_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Resets I2S peripheral
     *        复位 I2S 外设。
     *
     * @param i2s        Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_reset(I2S_Regs *i2s)
    {
        i2s->GPRCM.RSTCTL =
            (I2S_RSTCTL_KEY_UNLOCK_W | I2S_RSTCTL_RESETSTKYCLR_CLR |
             I2S_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief Returns if I2S peripheral was reset
     *        查询 I2S 外设是否已复位。
     *
     * @param i2s        Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @return     Whether module was reset
     *              外设是否已复位。
     *
     *  @retval     true if module was reset
     *  @retval     false if module wasn't reset
     */
    __STATIC_INLINE bool DL_I2S_isReset(const I2S_Regs *i2s)
    {
        return ((i2s->GPRCM.STAT & I2S_GPRCM_STAT_RESETSTKY_MASK) ==
                I2S_GPRCM_STAT_RESETSTKY_RESET);
    }

    /**
     *  @brief      Enable the I2S peripheral
     *              使能 I2S 外设。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_enable(I2S_Regs *i2s)
    {
        i2s->FMTCFG |= I2S_FMTCFG_ENABLE_ENABLE;
    }

    /**
     *  @brief      Disable the I2S peripheral
     *              禁用 I2S 外设。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_disable(I2S_Regs *i2s)
    {
        i2s->FMTCFG &= ~(I2S_FMTCFG_ENABLE_MASK);
    }

    /**
     *  @brief      Checks if the I2S peripheral is enabled
     *              查询 I2S 外设是否已使能。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @return     Returns the enabled status of the I2S
     *              I2S 使能状态。
     *
     *  @retval     true  The I2S peripheral is enabled
     *  @retval     false The I2S peripheral is disabled
     */
    __STATIC_INLINE bool DL_I2S_isEnabled(const I2S_Regs *i2s)
    {
        return (
            (i2s->FMTCFG & I2S_FMTCFG_ENABLE_MASK) == I2S_FMTCFG_ENABLE_ENABLE);
    }

    /**
     *  @brief      Enable free run control
     *              使能自由运行模式，CPU 暂停调试时模块继续运行。
     *
     *  When enabled, the module will continue to free run and ignores the state
     *  of the CPU halted debug state
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_enableFreeRun(I2S_Regs *i2s)
    {
        DL_Common_updateReg(
            &i2s->PDBGCTL, I2S_PDBGCTL_FREE_RUN, I2S_PDBGCTL_FREE_MASK);
    }

    /**
     *  @brief      Disable free run control
     *              禁用自由运行模式。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_disableFreeRun(I2S_Regs *i2s)
    {
        DL_Common_updateReg(
            &i2s->PDBGCTL, I2S_PDBGCTL_FREE_STOP, I2S_PDBGCTL_FREE_MASK);
    }

    /**
     *  @brief      Returns if free run control is enabled
     *              查询自由运行模式是否已使能。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @return     Whether free run is enabled
     *              自由运行是否已使能。
     *
     *  @retval     true if free run is enabled
     *  @retval     false if free run is disabled
     */
    __STATIC_INLINE bool DL_I2S_isFreeRunEnabled(const I2S_Regs *i2s)
    {
        return ((i2s->PDBGCTL & I2S_PDBGCTL_FREE_MASK) == I2S_PDBGCTL_FREE_RUN);
    }

    /**
     *  @brief      Get the current mode for the I2S (controller/target)
     *  @details    功能：读取 I2S 当前工作模式（控制器/目标）。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      参数：I2S 外设寄存器基址。
     *
     *  @return     The currently configured mode for I2S (controller/target)
     *              返回值：当前 I2S 模式。
     *
     *  @retval     One of @ref DL_I2S_MODE
     */
    DL_I2S_MODE DL_I2S_getMode(const I2S_Regs *i2s);

    /**
     *  @brief      Set whether the device should be in controller/target mode
     *  @details    功能：设置 I2S 为控制器模式或目标模式。
     *
     *  @param[in]  i2s   Pointer to the register overlay for the peripheral
     *                    参数：I2S 外设寄存器基址。
     *  @param[in]  mode  Mode to configure the I2S into. One of @ref DL_I2S_MODE
     *                    参数：目标模式配置。
     *
     *  @return     返回值：无。
     *
     *  @sa DL_I2S_init
     */
    void DL_I2S_setMode(I2S_Regs *i2s, DL_I2S_MODE mode);

    /**
     *  @brief      Enable WCLK and BCLK generation
     *              使能 WCLK 和 BCLK 时钟输出（控制器模式）。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_enableWBCLKGeneration(I2S_Regs *i2s)
    {
        i2s->CLKCTL |= I2S_CLKCTL_WBEN_EN;
    }

    /**
     *  @brief      Disable WCLK and BCLK generation
     *              禁用 WCLK 和 BCLK 时钟输出。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_disableWBCLKGeneration(I2S_Regs *i2s)
    {
        i2s->CLKCTL &= ~(I2S_CLKCTL_WBEN_MASK);
    }

    /**
     *  @brief      Returns if WCLK and BCLK generation is enabled
     *              查询 WCLK/BCLK 时钟输出是否已使能。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @return     Whether clock generation is enabled
     *              时钟输出是否已使能。
     *
     *  @retval     true if clock generation is enabled
     *  @retval     false if clock generation is disabled
     *
     */
    __STATIC_INLINE bool DL_I2S_isWBCLKGenerationEnabled(const I2S_Regs *i2s)
    {
        return ((i2s->CLKCTL & I2S_CLKCTL_WBEN_MASK) == I2S_CLKCTL_WBEN_EN);
    }

    /**
     *  @brief Enable MCLK generation
     *         使能 MCLK 时钟输出（仅控制器模式）。
     *
     *  MCLK only applies when I2S is operating in controller mode. MCLK is an
     *  optional clock output signal
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @sa DL_I2S_init
     *  @sa DL_I2S_setMode
     */
    __STATIC_INLINE void DL_I2S_enableMCLKGeneration(I2S_Regs *i2s)
    {
        i2s->CLKCTL |= I2S_CLKCTL_MEN_EN;
    }

    /**
     *  @brief      Disable MCLK generation
     *              禁用 MCLK 时钟输出。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2S_disableMCLKGeneration(I2S_Regs *i2s)
    {
        i2s->CLKCTL &= ~(I2S_CLKCTL_MEN_MASK);
    }

    /**
     *  @brief      Returns if MCLK generation is enabled
     *              查询 MCLK 时钟输出是否已使能。
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @return     Whether clock generation is enabled
     *              时钟输出是否已使能。
     *
     *  @retval     true if clock generation is enabled
     *  @retval     false if clock generation is disabled
     */
    __STATIC_INLINE bool DL_I2S_isMCLKGenerationEnabled(const I2S_Regs *i2s)
    {
        return ((i2s->CLKCTL & I2S_CLKCTL_MEN_MASK) == I2S_CLKCTL_MEN_EN);
    }

    /**
     *  @brief      Get source of WCLK and BCLK
     *              获取 WCLK/BCLK 音频时钟源。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     Source of WCLK and BCLK
     *              当前音频时钟源。
     *
     *  @retval     One of @ref DL_I2S_WORD_BAUD_CLOCK_SOURCE
     */
    __STATIC_INLINE DL_I2S_WORD_BAUD_CLOCK_SOURCE DL_I2S_getWBCLKSource(
        const I2S_Regs *i2s)
    {
        uint32_t clockSource = i2s->WCLKSRC & I2S_WCLKSRC_WBCLKSRC_MASK;

        return (DL_I2S_WORD_BAUD_CLOCK_SOURCE)(clockSource);
    }

    /**
     *  @brief      Set source of WCLK and BCLK
     *              设置 WCLK/BCLK 音频时钟源。
     *
     *  @param[in]  i2s         Pointer to the register overlay for the peripheral
     *                          I2S 外设寄存器基址。
     *
     *  @param[in]  clockSource Source of WCLK and BCLK.
     *                          One of @ref DL_I2S_WORD_BAUD_CLOCK_SOURCE.
     *                          音频时钟源选择。
     */
    __STATIC_INLINE void DL_I2S_setWBCLKSource(
        I2S_Regs *i2s, DL_I2S_WORD_BAUD_CLOCK_SOURCE clockSource)
    {
        DL_Common_updateReg(
            &i2s->WCLKSRC, (uint32_t)clockSource, I2S_WCLKSRC_WBCLKSRC_MASK);
    }

    /**
     *  @brief      Get WCLK inversion status
     *              获取 WCLK 极性反转状态。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     WCLK inversion status
     *              WCLK 极性反转状态。
     *
     *  @retval     One of @ref DL_I2S_WCLK_INVERSION
     */
    __STATIC_INLINE DL_I2S_WCLK_INVERSION DL_I2S_getWCLKInversion(
        const I2S_Regs *i2s)
    {
        uint32_t wclkInversion = i2s->WCLKSRC & I2S_WCLKSRC_WCLKINV_MASK;

        return (DL_I2S_WCLK_INVERSION)(wclkInversion);
    }

    /**
     *  @brief      Invert WCLK
     *              设置 WCLK 极性反转。
     *
     *  @param[in]  i2s           Pointer to the register overlay for the peripheral
     *                            I2S 外设寄存器基址。
     *
     *  @param[in]  wclkInversion WCLK inversion status.
     *                            One of @ref DL_I2S_WCLK_INVERSION.
     *                            WCLK 极性反转配置。
     */
    __STATIC_INLINE void DL_I2S_setWCLKInversion(
        I2S_Regs *i2s, DL_I2S_WCLK_INVERSION wclkInversion)
    {
        DL_Common_updateReg(
            &i2s->WCLKSRC, (uint32_t)wclkInversion, I2S_WCLKSRC_WCLKINV_MASK);
    }

    /**
     *  @brief      Get WCLK phase
     *              获取 WCLK 分频比计算模式。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     WCLK phase
     *              WCLK 分频比计算模式。
     *
     *  @retval     One of @ref DL_I2S_WCLK_PHASE
     */
    __STATIC_INLINE DL_I2S_WCLK_PHASE DL_I2S_getWCLKPhase(const I2S_Regs *i2s)
    {
        uint32_t wclkPhase = i2s->CLKCTL & I2S_CLKCTL_WCLKPHASE_MASK;

        return (DL_I2S_WCLK_PHASE)(wclkPhase);
    }

    /**
     *  @brief      Set WCLK phase
     *              设置 WCLK 分频比计算模式。
     *
     *  @param[in]  i2s         Pointer to the register overlay for the peripheral
     *                          I2S 外设寄存器基址。
     *
     *  @param[in]  wclkPhase   WCLK phase. One of @ref DL_I2S_WCLK_PHASE.
     *                          WCLK 分频比计算模式。
     */
    __STATIC_INLINE void DL_I2S_setWCLKPhase(
        I2S_Regs *i2s, DL_I2S_WCLK_PHASE wclkPhase)
    {
        DL_Common_updateReg(
            &i2s->CLKCTL, (uint32_t)wclkPhase, I2S_CLKCTL_WCLKPHASE_MASK);
    }

    /**
     *  @brief Set MCLK divider
     *         设置 MCLK 分频系数。
     *
     *  @param[in]  i2s                     Pointer to the register overlay for the
     *                                      peripheral
     *                                      I2S 外设寄存器基址。
     *  @param[in]  mclkDivider             MCLK divider.
     *  [ @ref DL_I2S_MDIV_MINIMUM, @ref DL_I2S_MDIV_MAXIMUM ]
     *                                      MCLK 分频系数。
     *
     *  @sa         @ref DL_I2S_MDIV_INVALID
     */
    __STATIC_INLINE void DL_I2S_setMCLKDivider(I2S_Regs *i2s, uint32_t mclkDivider)
    {
        DL_Common_updateReg(&i2s->MCLKDIV, mclkDivider, I2S_MCLKDIV_MDIV_MASK);
    }

    /**
     *  @brief      Get MCLK divider
     *              获取 MCLK 分频系数。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     MCLK divider
     *              MCLK 分频系数。
     *
     *  @retval     [ @ref DL_I2S_MDIV_MINIMUM, @ref DL_I2S_MDIV_MAXIMUM ]
     *
     *  @sa         @ref DL_I2S_MDIV_INVALID
     */
    __STATIC_INLINE uint32_t DL_I2S_getMCLKDivider(const I2S_Regs *i2s)
    {
        uint32_t mclkDivider = i2s->MCLKDIV & I2S_MCLKDIV_MDIV_MASK;

        return (mclkDivider);
    }

    /**
     *  @brief Set WCLK divider.
     *         设置 WCLK 分频系数。
     *
     *  @note For single-phase and dual-phase frame formats, WDIV[9:0] are used for
     *  the clock calcuation, making 1023 the maximum factor of the division ratio.
     *  @note For user-defined frame formats such as PCM (long) and TDM formats,
     *  WCLK is high for WDIV[7:0] and low for WDIV[15:8], making both high and low
     *  portions have a maximum factor of 255.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  wclkDivider    WCLK divider to set. [0x1, 0xFFFF]
     *                             WCLK 分频系数。
     */
    __STATIC_INLINE void DL_I2S_setWCLKDivider(I2S_Regs *i2s, uint32_t wclkDivider)
    {
        DL_Common_updateReg(&i2s->WCLKDIV, wclkDivider, I2S_WCLKDIV_WDIV_MASK);
    }

    /**
     *  @brief      Get WCLK divider
     *              获取 WCLK 分频系数。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     WCLK divider
     *              WCLK 分频系数。
     *
     *  @retval     [0x1, 0xFFFF]
     */
    __STATIC_INLINE uint32_t DL_I2S_getWCLKDivider(const I2S_Regs *i2s)
    {
        uint32_t wclkDivider = i2s->WCLKDIV & I2S_WCLKDIV_WDIV_MASK;

        return (wclkDivider);
    }

    /**
     * @brief Set BCLK divider
     *        设置 BCLK 分频系数。
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  bclkDivider    BCLK divider. [ @ref DL_I2S_BDIV_MINIMUM, @ref DL_I2S_BDIV_MAXIMUM ]
     *                             BCLK 分频系数。
     *
     *  @sa         @ref DL_I2S_BDIV_INVALID
     */
    __STATIC_INLINE void DL_I2S_setBCLKDivider(I2S_Regs *i2s, uint32_t bclkDivider)
    {
        DL_Common_updateReg(&i2s->BCLKDIV, bclkDivider, I2S_BCLKDIV_BDIV_MASK);
    }

    /**
     *  @brief      Get BCLK divider
     *              获取 BCLK 分频系数。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     BCLK divider
     *              BCLK 分频系数。
     *
     *  @retval     [ @ref DL_I2S_BDIV_MINIMUM, @ref DL_I2S_BDIV_MAXIMUM ]
     *
     *  @sa         @ref DL_I2S_BDIV_INVALID
     */
    __STATIC_INLINE uint32_t DL_I2S_getBCLKDivider(const I2S_Regs *i2s)
    {
        uint32_t bclkDivider = i2s->BCLKDIV & I2S_BCLKDIV_BDIV_MASK;

        return (bclkDivider);
    }

    /**
     *  @brief Sets the sample word length in bits. In single-phase format, this is
     *  the exact number of bits per word. In dual-phase format, this is the
     *  maximum number of bits per word.
     *  设置每样本字长（位）。单相位为精确值，双相位为最大值。
     *
     *  @note Values written to the hardware should be 1 less than the desired
     *  sample word length e.g. a desired sample word length of 8 bits requires
     *  the value of 7 to be written. Sample word lengths less than 8 and above 32
     *  bits will result in undefined behavior.
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *  @param[in] sampleWordLength Sample word length (in bits) to set. [7, 31]
     *                              样本字长（硬件值 = 实际位数 - 1）。
     */
    __STATIC_INLINE void DL_I2S_setSampleWordLength(
        I2S_Regs *i2s, uint32_t sampleWordLength)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, sampleWordLength, I2S_FMTCFG_WORDLEN_MASK);
    }

    /**
     *  @brief Get the sample word length in bits
     *         获取每样本字长（位）。
     *
     *  @note Values read from the hardware will be 1 less than the true
     *  sample word length e.g. a value of 7 read from the hardware means that the
     *  sample word length is 8 bits.
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return Sample word length in bits
     *          样本字长（硬件值 = 实际位数 - 1）。
     *
     *  @retval Value in range [7, 31]
     */
    __STATIC_INLINE uint32_t DL_I2S_getSampleWordLength(const I2S_Regs *i2s)
    {
        uint32_t sampleWordLength = i2s->FMTCFG & I2S_FMTCFG_WORDLEN_MASK;

        return (sampleWordLength);
    }

    /**
     *  @brief Set between single or dual-phase format
     *         设置单相位或双相位帧格式。
     *
     *  @note Single-phase: DSP format
     *  @note Dual-phase: I2S, LJF and RJF formats
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *  @param[in] phase The sampling phase to use. One of @ref DL_I2S_PHASE
     *                   帧格式相位选择。
     */
    __STATIC_INLINE void DL_I2S_setFormatPhase(I2S_Regs *i2s, DL_I2S_PHASE phase)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, (uint32_t)phase, I2S_FMTCFG_DUALPHASE_MASK);
    }

    /**
     *  @brief Get the sampling phase being used
     *         获取当前帧格式相位模式。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return The sampling phase being used
     *          当前帧格式相位模式。
     *
     *  @retval One of @ref DL_I2S_PHASE
     */
    __STATIC_INLINE DL_I2S_PHASE DL_I2S_getFormatPhase(const I2S_Regs *i2s)
    {
        uint32_t phase = i2s->FMTCFG & I2S_FMTCFG_DUALPHASE_MASK;

        return ((DL_I2S_PHASE)(phase));
    }

    /**
     *  @brief Set the sample edge of data on BCLK.
     *         设置 BCLK 数据采样边沿。
     *
     *  @note On negative sample edge, data is sampled on the negative edge of
     *  BCLK and clocked out on the positive edge.
     *  @note On positive sample edge, data is sampled on the positive edge of
     *  BCLK and clocked out on the negative edge.
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *  @param[in] edge The sample edge to use. One of @ref DL_I2S_SAMPLE_EDGE
     *                  采样边沿选择。
     */
    __STATIC_INLINE void DL_I2S_setSampleEdge(
        I2S_Regs *i2s, DL_I2S_SAMPLE_EDGE edge)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, (uint32_t)edge, I2S_FMTCFG_SMPLEDGE_MASK);
    }

    /**
     *  @brief Gets the sample edge being used
     *         获取当前数据采样边沿。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return The sample edge being used
     *          当前采样边沿。
     *
     *  @retval One of @ref DL_I2S_SAMPLE_EDGE
     */
    __STATIC_INLINE DL_I2S_SAMPLE_EDGE DL_I2S_getSampleEdge(const I2S_Regs *i2s)
    {
        uint32_t edge = i2s->FMTCFG & I2S_FMTCFG_SMPLEDGE_MASK;

        return ((DL_I2S_SAMPLE_EDGE)(edge));
    }

    /**
     *  @brief Set the size of each sample word stored to or loaded from memory
     *         设置每样本的内存访问宽度。
     *
     *  @param[in] i2s    Pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *  @param[in] length The memory access length to use.
     *                    One of @ref DL_I2S_MEMORY_LENGTH
     *                    内存访问宽度。
     */
    __STATIC_INLINE void DL_I2S_setMemoryLength(
        I2S_Regs *i2s, DL_I2S_MEMORY_LENGTH length)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, (uint32_t)length, I2S_FMTCFG_MEMLEN32_MASK);
    }

    /**
     *  @brief Get the size of sample words stored to or loaded from memory
     *         获取当前内存访问宽度。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return The memory access length being used
     *          当前内存访问宽度。
     *
     *  @retval One of @ref DL_I2S_MEMORY_LENGTH
     */
    __STATIC_INLINE DL_I2S_MEMORY_LENGTH DL_I2S_getMemoryLength(
        const I2S_Regs *i2s)
    {
        uint32_t length = i2s->FMTCFG & I2S_FMTCFG_MEMLEN32_MASK;

        return ((DL_I2S_MEMORY_LENGTH)(length));
    }

    /**
     *  @brief Set the number of BCLK periods to wait between a WCLK edge and the
     *  MSB of the first word in a phase
     *         设置 WCLK 边沿到首字最高位之间的 BCLK 延迟周期数。
     *
     *  @note Data delay of 0 - LJF and DSP formats
     *  @note Data delay of 1 - I2S and DSP formats
     *  @note Data delay of 2 (minimum) - RJF format
     *  @note Data delay of 255 (maximum) - RJF format

     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *  @param[in] dataDelay The data delay to use. One of @ref DL_I2S_DATA_DELAY
     *                       数据延迟配置。
     */
    __STATIC_INLINE void DL_I2S_setDataDelay(
        I2S_Regs *i2s, DL_I2S_DATA_DELAY dataDelay)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, (uint32_t)dataDelay, I2S_FMTCFG_DATADLY_MASK);
    }

    /**
     *  @brief Gets the data delay being used
     *         获取当前数据延迟配置。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return The data delay being used
     *          当前数据延迟。
     *
     *  @retval One of @ref DL_I2S_DATA_DELAY
     */
    __STATIC_INLINE DL_I2S_DATA_DELAY DL_I2S_getDataDelay(const I2S_Regs *i2s)
    {
        uint32_t dataDelay = i2s->FMTCFG & I2S_FMTCFG_DATADLY_MASK;

        return ((DL_I2S_DATA_DELAY)(dataDelay));
    }

    /**
     *  @brief Set the data pin behavior during empty slots
     *         设置空时隙期间数据引脚输出行为。
     *
     *  @param[in] i2s    Pointer to the register overlay for the peripheral
     *                    I2S 外设寄存器基址。
     *  @param[in] output The empty slot output behavior.
     *                    One of @ref DL_I2S_EMPTY_SLOT_OUTPUT
     *                    空时隙输出行为。
     */
    __STATIC_INLINE void DL_I2S_setEmptySlotOutput(
        I2S_Regs *i2s, DL_I2S_EMPTY_SLOT_OUTPUT output)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, (uint32_t)output, I2S_FMTCFG_EMPTYSLOTOUTPUT_MASK);
    }

    /**
     *  @brief Get the data pin behavior during empty slots
     *         获取空时隙期间数据引脚输出行为。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return The empty slot output behavior being used
     *          当前空时隙输出行为。
     *
     *  @retval One of @ref DL_I2S_EMPTY_SLOT_OUTPUT
     */
    __STATIC_INLINE DL_I2S_EMPTY_SLOT_OUTPUT DL_I2S_getEmptySlotOutput(
        const I2S_Regs *i2s)
    {
        uint32_t output = i2s->FMTCFG & I2S_FMTCFG_EMPTYSLOTOUTPUT_MASK;

        return ((DL_I2S_EMPTY_SLOT_OUTPUT)(output));
    }

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    /**
     *  @brief Set the direction of the data pins
     *         设置 AD0/AD1 数据引脚方向。
     *
     *  @param[in] i2s               Pointer to the register overlay for the
     *                               peripheral
     *                               I2S 外设寄存器基址。
     *  @param[in] dataPin0Direction Data pin 0 (AD0) direction.
     *                               One of @ref DL_I2S_DATA_PIN_DIRECTION
     *                               AD0 方向。
     *  @param[in] dataPin1Direction Data pin 1 (AD1) direction.
     *                               One of @ref DL_I2S_DATA_PIN_DIRECTION
     *                               AD1 方向。
     */
    __STATIC_INLINE void DL_I2S_setDataPinsDirection(I2S_Regs *i2s,
                                                     DL_I2S_DATA_PIN_DIRECTION dataPin0Direction,
                                                     DL_I2S_DATA_PIN_DIRECTION dataPin1Direction)
    {
        DL_Common_updateReg(&i2s->DIRCFG,
                            ((((uint32_t)dataPin0Direction) << I2S_DIRCFG_AD0_OFS) |
                             (((uint32_t)dataPin1Direction) << I2S_DIRCFG_AD1_OFS)),
                            (I2S_DIRCFG_AD0_MASK | I2S_DIRCFG_AD1_MASK));
    }

#endif
    /**
     *  @brief Set the direction of data pin 0
     *         设置数据引脚 0（AD0）方向。
     *
     *  @param[in] i2s       Pointer to the register overlay for the peripheral
     *                       I2S 外设寄存器基址。
     *  @param[in] direction Data pin direction.
     *                       One of @ref DL_I2S_DATA_PIN_DIRECTION
     *                       引脚方向。
     */
    __STATIC_INLINE void DL_I2S_setDataPin0Direction(
        I2S_Regs *i2s, DL_I2S_DATA_PIN_DIRECTION direction)
    {
        DL_Common_updateReg(&i2s->DIRCFG,
                            ((uint32_t)(direction)) << I2S_DIRCFG_AD0_OFS, I2S_DIRCFG_AD0_MASK);
    }

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    /**
     *  @brief Set the direction of data pin 1
     *         设置数据引脚 1（AD1）方向。
     *
     *  @param[in] i2s       Pointer to the register overlay for the peripheral
     *                       I2S 外设寄存器基址。
     *  @param[in] direction Data pin direction.
     *                       One of @ref DL_I2S_DATA_PIN_DIRECTION
     *                       引脚方向。
     */
    __STATIC_INLINE void DL_I2S_setDataPin1Direction(
        I2S_Regs *i2s, DL_I2S_DATA_PIN_DIRECTION direction)
    {
        DL_Common_updateReg(&i2s->DIRCFG,
                            ((uint32_t)(direction)) << I2S_DIRCFG_AD1_OFS, I2S_DIRCFG_AD1_MASK);
    }

#endif
    /**
     *  @brief Get the direction of data pin 0
     *         获取数据引脚 0（AD0）方向。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return Direction of the data pin
     *          引脚方向。
     *
     *  @retval One of @ref DL_I2S_DATA_PIN_DIRECTION
     */
    __STATIC_INLINE DL_I2S_DATA_PIN_DIRECTION DL_I2S_getDataPin0Direction(
        const I2S_Regs *i2s)
    {
        uint32_t direction = i2s->DIRCFG & I2S_DIRCFG_AD0_MASK;

        return ((DL_I2S_DATA_PIN_DIRECTION)(direction >> I2S_DIRCFG_AD0_OFS));
    }

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    /**
     *  @brief Get the direction of data pin 1
     *         获取数据引脚 1（AD1）方向。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return Direction of the data pin
     *          引脚方向。
     *
     *  @retval One of @ref DL_I2S_DATA_PIN_DIRECTION
     */
    __STATIC_INLINE DL_I2S_DATA_PIN_DIRECTION DL_I2S_getDataPin1Direction(
        const I2S_Regs *i2s)
    {
        uint32_t direction = i2s->DIRCFG & I2S_DIRCFG_AD1_MASK;

        return ((DL_I2S_DATA_PIN_DIRECTION)(direction >> I2S_DIRCFG_AD1_OFS));
    }

#endif
    /**
     *  @brief Set the valid channel mask of data pin 0
     *         设置数据引脚 0（AD0）有效通道掩码。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *  @param[in] channelMask Valid channel mask of the data pin
     *                         有效通道掩码 [0, 0xFF]。
     */
    __STATIC_INLINE void DL_I2S_setDataPin0ChannelMask(
        I2S_Regs *i2s, uint32_t channelMask)
    {
        i2s->WMASK0 = channelMask;
    }

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    /**
     *  @brief Set the valid channel mask of data pin 1
     *         设置数据引脚 1（AD1）有效通道掩码。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *  @param[in] channelMask Valid channel mask of the data pin
     *                         有效通道掩码 [0, 0xFF]。
     */
    __STATIC_INLINE void DL_I2S_setDataPin1ChannelMask(
        I2S_Regs *i2s, uint32_t channelMask)
    {
        i2s->WMASK1 = channelMask;
    }

#endif
    /**
     *  @brief Get the valid channel mask of data pin 0
     *         获取数据引脚 0（AD0）有效通道掩码。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return Valid channel mask of the data pin
     *          有效通道掩码。
     *
     *  @retval The valid channel mask of the data pin with the range [0, 0xFF].
     *          For single-phase mode, each bit represents 1 channel. For
     *          dual-phase mode, only the first 2 LSBs are considered.
     */
    __STATIC_INLINE uint32_t DL_I2S_getDataPin0ChannelMask(const I2S_Regs *i2s)
    {
        uint32_t channelMask = i2s->WMASK0 & I2S_WMASK0_MASK_MASK;

        return (channelMask);
    }

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    /**
     *  @brief Get the valid channel mask of data pin 1
     *         获取数据引脚 1（AD1）有效通道掩码。
     *
     *  @param[in] i2s Pointer to the register overlay for the peripheral
     *                 I2S 外设寄存器基址。
     *
     *  @return Valid channel mask of the data pin
     *          有效通道掩码。
     *
     *  @retval The valid channel mask of the data pin with the range [0, 0xFF].
     *          For single-phase mode, each bit represents 1 channel. For
     *          dual-phase mode, only the first 2 LSBs are considered.
     */
    __STATIC_INLINE uint32_t DL_I2S_getDataPin1ChannelMask(const I2S_Regs *i2s)
    {
        uint32_t channelMask = i2s->WMASK1 & I2S_WMASK1_MASK_MASK;

        return (channelMask);
    }

#endif
    /**
     *  @brief      Get TX FIFO threshold level
     *              获取 TX FIFO 中断触发阈值。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     Indicates at what fill level in the TX FIFO a threshold will be
     *              generated
     *              当前 TX FIFO 触发阈值。
     *
     *  @retval     One of @ref DL_I2S_TX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2S_TX_FIFO_LEVEL DL_I2S_getTXFIFOThreshold(
        const I2S_Regs *i2s)
    {
        uint32_t level = i2s->IFLS & I2S_IFLS_TXIFLSEL_MASK;

        return (DL_I2S_TX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set TX FIFO threshold level
     *              设置 TX FIFO 中断触发阈值。
     *
     *  @note Randomly changing FIFO levels when FIFO has contents might lead to
     *  unexpected behavior. It is recommended to clear the FIFO before changing
     *  FIFO threshold levels.
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *  @param[in]  level   Indicates at what fill level in the TX FIFO a threshold
     *                      will be generated.
     *                      One of @ref DL_I2S_TX_FIFO_LEVEL.
     *                      TX FIFO 触发阈值。
     *
     *  @sa         DL_I2S_clearTXFIFO
     */
    __STATIC_INLINE void DL_I2S_setTXFIFOThreshold(
        I2S_Regs *i2s, DL_I2S_TX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(&i2s->IFLS, (uint32_t)level, I2S_IFLS_TXIFLSEL_MASK);
    }

    /**
     *  @brief      Get RX FIFO threshold level
     *              获取 RX FIFO 中断触发阈值。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     Indicates at what fill level in the RX FIFO a threshold will be
     *              generated
     *              当前 RX FIFO 触发阈值。
     *
     *  @retval     One of @ref DL_I2S_RX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2S_RX_FIFO_LEVEL DL_I2S_getRXFIFOThreshold(
        const I2S_Regs *i2s)
    {
        uint32_t level = i2s->IFLS & I2S_IFLS_RXIFLSEL_MASK;

        return (DL_I2S_RX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set RX FIFO threshold level
     *              设置 RX FIFO 中断触发阈值。
     *
     *  @note Randomly changing FIFO levels when FIFO has contents might lead to
     *  unexpected behavior. It is recommended to clear the FIFO before changing
     *  FIFO threshold levels.
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *  @param[in]  level   Indicates at what fill level in the RX FIFO a threshold
     *                      will be generated.
     *                      One of @ref DL_I2S_RX_FIFO_LEVEL.
     *                      RX FIFO 触发阈值。
     *
     *  @sa         DL_I2S_clearRXFIFO
     */
    __STATIC_INLINE void DL_I2S_setRXFIFOThreshold(
        I2S_Regs *i2s, DL_I2S_RX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(&i2s->IFLS, (uint32_t)level, I2S_IFLS_RXIFLSEL_MASK);
    }

    /**
     *  @brief      Checks if RX FIFO clear is complete
     *              查询 RX FIFO 清除是否完成。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     If RX FIFO clear is complete
     *              RX FIFO 清除是否完成。
     *
     *  @retval     true if RX FIFO clear is complete
     *  @retval     false if RX FIFO clear is complete
     */
    __STATIC_INLINE bool DL_I2S_isRXFIFOClearComplete(const I2S_Regs *i2s)
    {
        return ((i2s->STAT & I2S_STAT_RXCLR_MASK) == I2S_STAT_RXCLR_SET);
    }

    /**
     *  @brief      Checks if TX FIFO clear is complete
     *              查询 TX FIFO 清除是否完成。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     If TX FIFO clear is complete
     *              TX FIFO 清除是否完成。
     *
     *  @retval     true if TX FIFO clear is complete
     *  @retval     false if TX FIFO clear is complete
     */
    __STATIC_INLINE bool DL_I2S_isTXFIFOClearComplete(const I2S_Regs *i2s)
    {
        return ((i2s->STAT & I2S_STAT_TXCLR_MASK) == I2S_STAT_TXCLR_SET);
    }

    /**
     *  @brief      Enable I2S interrupts
     *              使能 I2S 中断。
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_I2S_INTERRUPT.
     *                             中断掩码。
     */
    __STATIC_INLINE void DL_I2S_enableInterrupt(
        I2S_Regs *i2s, uint32_t interruptMask)
    {
        i2s->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable I2S interrupts
     *              禁用 I2S 中断。
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
     *                             @ref DL_I2S_INTERRUPT.
     *                             中断掩码。
     */
    __STATIC_INLINE void DL_I2S_disableInterrupt(
        I2S_Regs *i2s, uint32_t interruptMask)
    {
        i2s->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which I2S interrupts are enabled
     *              查询哪些 I2S 中断已使能。
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_INTERRUPT.
     *                             中断掩码。
     *
     *  @return     Which of the requested I2S interrupts are enabled
     *              已使能的中断位。
     *
     *  @retval     Bitwise OR of @ref DL_I2S_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2S_getEnabledInterrupts(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled I2S interrupts
     *              查询已使能的 I2S 中断标志。
     *
     *  Checks if any of the I2S interrupts that were previously enabled are
     *  pending.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_INTERRUPT.
     *                             中断掩码。
     *
     *  @return     Which of the requested I2S interrupts are pending
     *              挂起的中断标志位。
     *
     *  @retval     Bitwise OR of @ref DL_I2S_INTERRUPT values
     *
     *  @sa         DL_I2S_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_I2S_getEnabledInterruptStatus(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any I2S interrupt
     *              查询任意 I2S 中断原始标志（无需先使能）。
     *
     *  Checks if any of the I2S interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_INTERRUPT.
     *                             中断掩码。
     *
     *  @return     Which of the requested I2S interrupts are pending
     *              挂起的原始中断标志位。
     *
     *  @retval     Bitwise OR of @ref DL_I2S_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2S_getRawInterruptStatus(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending I2S interrupt
     *              获取最高优先级待处理 I2S 中断索引。
     *
     *  Checks if any of the I2S interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @return     The highest priority pending I2S interrupt
     *              最高优先级待处理中断索引。
     *
     *  @retval     One of @ref DL_I2S_IIDX
     */
    __STATIC_INLINE DL_I2S_IIDX DL_I2S_getPendingInterrupt(const I2S_Regs *i2s)
    {
        return ((DL_I2S_IIDX)i2s->CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending I2S interrupts
     *              清除挂起的 I2S 中断标志。
     *
     *  @param[in]  i2s            Pointer to the register overlay for the
     *                             peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_I2S_INTERRUPT.
     *                             中断掩码。
     */
    __STATIC_INLINE void DL_I2S_clearInterruptStatus(
        I2S_Regs *i2s, uint32_t interruptMask)
    {
        i2s->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief Enables I2S interrupt for triggering the DMA receive event
     *         使能 I2S DMA 接收触发中断。
     *
     * Enables the I2S interrupt to be used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_RX
     * register, which is the event publisher used for triggering the DMA to do a
     * receive data transfer.
     *
     * @note Only one interrupt source should be enabled at a time.
     *
     *  @param[in]  i2s        Pointer to the register overlay for the peripheral
     *                         I2S 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
     *                         the DMA. One of @ref DL_I2S_DMA_INTERRUPT.
     *                         DMA 触发中断源。
     */
    __STATIC_INLINE void DL_I2S_enableDMAReceiveEvent(
        I2S_Regs *i2s, uint32_t interrupt)
    {
        i2s->DMA_TRIG_RX.IMASK = interrupt;
    }

    /**
     *  @brief Enables I2S interrupt for triggering the DMA transmit event
     *         使能 I2S DMA 发送触发中断。
     *
     * Enables the I2S interrupt to be used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_TX
     * register, which is the event publisher used for triggering the DMA to do a
     * transmit data transfer.
     *
     * @note Only one interrupt source should be enabled at a time.
     *
     *  @param[in]  i2s        Pointer to the register overlay for the peripheral
     *                         I2S 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
     *                         the DMA. One of @ref DL_I2S_DMA_INTERRUPT.
     *                         DMA 触发中断源。
     */
    __STATIC_INLINE void DL_I2S_enableDMATransmitEvent(
        I2S_Regs *i2s, uint32_t interrupt)
    {
        i2s->DMA_TRIG_TX.IMASK = interrupt;
    }

    /**
     *  @brief Disables I2S interrupt from triggering the DMA receive event
     *         禁用 I2S DMA 接收触发中断。
     *
     * Disables the I2S interrupt from being used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_RX
     * register, which is the event publisher used for triggering the DMA to do a
     * receive data transfer.
     *
     *  @param[in]  i2s        Pointer to the register overlay for the peripheral
     *                         I2S 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
     *                         the DMA. One of @ref DL_I2S_DMA_INTERRUPT.
     *                         DMA 触发中断源。
     */
    __STATIC_INLINE void DL_I2S_disableDMAReceiveEvent(
        I2S_Regs *i2s, uint32_t interrupt)
    {
        i2s->DMA_TRIG_RX.IMASK &= ~(interrupt);
    }

    /**
     *  @brief Disables I2S interrupt from triggering the DMA transmit event
     *         禁用 I2S DMA 发送触发中断。
     *
     * Disables the I2S interrupt from being used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_TX
     * register, which is the event publisher used for triggering the DMA to do a
     * transmit data transfer.
     *
     *  @param[in]  i2s        Pointer to the register overlay for the peripheral
     *                         I2S 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
     *                         the DMA. One of @ref DL_I2S_DMA_INTERRUPT.
     *                         DMA 触发中断源。
     */
    __STATIC_INLINE void DL_I2S_disableDMATransmitEvent(
        I2S_Regs *i2s, uint32_t interrupt)
    {
        i2s->DMA_TRIG_TX.IMASK &= ~(interrupt);
    }

    /**
     *  @brief Check which I2S interrupt for DMA receive events is enabled
     *         查询哪些 DMA 接收触发中断已使能。
     *
     *  This API checks the DMA_TRIG_RX register, which is used for triggering the
     *  DMA to do a receive data transfer.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_DMA_INTERRUPT.
     *                             中断掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested I2S interrupt status
     *              已使能的 DMA 接收触发中断。
     *
     *  @retval     One of @ref DL_I2S_DMA_INTERRUPT
     */
    __STATIC_INLINE uint32_t DL_I2S_getEnabledDMAReceiveEvent(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->DMA_TRIG_RX.IMASK & interruptMask);
    }

    /**
     *  @brief Check which I2S interrupt for DMA transmit events is enabled
     *         查询哪些 DMA 发送触发中断已使能。
     *
     *  This API checks the DMA_TRIG_TX register, which is used for triggering the
     *  DMA to do a transmit data transfer.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_DMA_INTERRUPT.
     *                             中断掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested I2S interrupt status
     *              已使能的 DMA 发送触发中断。
     *
     *  @retval     One of @ref DL_I2S_DMA_INTERRUPT
     */
    __STATIC_INLINE uint32_t DL_I2S_getEnabledDMATransmitEvent(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->DMA_TRIG_TX.IMASK & interruptMask);
    }

    /**
     *  @brief Check interrupt flag of enabled I2S interrupt for DMA receive event
     *         查询已使能的 DMA 接收触发中断标志。
     *
     *  Checks if any of the I2S interrupts for the DMA receive event that were
     *  previously enabled are pending.
     *  This API checks the DMA_TRIG_RX register, which is  used for triggering the
     *  DMA to do a receive event.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_DMA_INTERRUPT.
     *                             中断掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested I2S interrupt status
     *              挂起的 DMA 接收触发中断标志。
     *
     *  @retval     One of @ref DL_I2S_DMA_INTERRUPT
     *
     *  @sa         DL_I2S_enableDMAReceiveEvent
     */
    __STATIC_INLINE uint32_t DL_I2S_getEnabledDMAReceiveEventStatus(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->DMA_TRIG_RX.MIS & interruptMask);
    }

    /**
     *  @brief Check interrupt flag of enabled I2S interrupt for DMA transmit event
     *         查询已使能的 DMA 发送触发中断标志。
     *
     *  Checks if any of the I2S interrupts for the DMA transmit event that were
     *  previously enabled are pending.
     *  This API checks the DMA_TRIG_TX register, which is  used for triggering the
     *  DMA to do a transmit event.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_DMA_INTERRUPT.
     *                             中断掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested I2S interrupt status
     *              挂起的 DMA 发送触发中断标志。
     *
     *  @retval     One of @ref DL_I2S_DMA_INTERRUPT
     *
     *  @sa         DL_I2S_enableDMATransmitEvent
     */
    __STATIC_INLINE uint32_t DL_I2S_getEnabledDMATransmitEventStatus(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->DMA_TRIG_TX.MIS & interruptMask);
    }

    /**
     *  @brief Check interrupt flag of any I2S interrupt for DMA receive event
     *         查询 DMA 接收触发中断原始标志（无需先使能）。
     *
     *  Checks if any of the I2S interrupts for DMA receive event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_RX register, which is used for triggering the
     *  DMA to do a receive event.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_DMA_INTERRUPT.
     *                             中断掩码。
     *
     *  @return     Which of the requested I2S interrupts are pending
     *              挂起的原始 DMA 接收触发中断标志。
     *
     *  @retval     Bitwise OR of @ref DL_I2S_DMA_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2S_getRawDMAReceiveEventStatus(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->DMA_TRIG_RX.RIS & interruptMask);
    }

    /**
     *  @brief Check interrupt flag of any I2S interrupt for DMA transmit event
     *         查询 DMA 发送触发中断原始标志（无需先使能）。
     *
     *  Checks if any of the I2S interrupts for DMA transmit event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_TX register, which is used for triggering the
     *  DMA to do a transmit event.
     *
     *  @param[in]  i2s            Pointer to the register overlay for the peripheral
     *                             I2S 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2S_DMA_INTERRUPT.
     *                             中断掩码。
     *
     *  @return     Which of the requested I2S interrupts are pending
     *              挂起的原始 DMA 发送触发中断标志。
     *
     *  @retval     Bitwise OR of @ref DL_I2S_DMA_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2S_getRawDMATransmitEventStatus(
        const I2S_Regs *i2s, uint32_t interruptMask)
    {
        return (i2s->DMA_TRIG_TX.RIS & interruptMask);
    }

    /**
     *  @brief      Suspend external communication
     *              挂起外部通信，当前传输完成后停止，寄存器配置保留。
     *
     *  Ongoing communication will complete, and further external communications
     *  are stopped. Tranmit line(s) will be driven to idle state, and further
     *  toggles on the receive line(s) will not be processed. Once suspend is
     *  requested, the CPU should poll to ensure that the device has reached idle.
     *  When the device is idle, the transmit FIFO should be flushed, and the
     *  receive FIFO should be drained prior to disabling the module.
     *  After suspending the module, the I2S register configurations are retained.
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @sa DL_I2S_disable
     *  @sa DL_I2S_resume
     */
    __STATIC_INLINE void DL_I2S_suspend(I2S_Regs *i2s)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, I2S_FMTCFG_SUSPEND_ENABLE, I2S_FMTCFG_SUSPEND_MASK);
    }

    /**
     *  @brief      Resume functional mode
     *              恢复正常通信模式。
     *
     *  Functional communication can be resumed by calling this API, and enabling
     *  the module
     *
     *  @param[in]  i2s  Pointer to the register overlay for the peripheral
     *                   I2S 外设寄存器基址。
     *
     *  @sa DL_I2S_enable
     *  @sa DL_I2S_suspend
     */
    __STATIC_INLINE void DL_I2S_resume(I2S_Regs *i2s)
    {
        DL_Common_updateReg(
            &i2s->FMTCFG, I2S_FMTCFG_SUSPEND_DISABLE, I2S_FMTCFG_SUSPEND_MASK);
    }

    /**
     *  @brief      Set the clock source for the I2S peripheral
     *              设置 I2S 外设主时钟源。
     *
     *  @param[in]  i2s         Pointer to the register overlay for the peripheral
     *                          I2S 外设寄存器基址。
     *
     *  @param[in]  clockSource Source of the clock for I2S.
     *                          One of @ref DL_I2S_CLOCK_SOURCE.
     *                          主时钟源选择。
     */
    __STATIC_INLINE void DL_I2S_setClockSource(
        I2S_Regs *i2s, DL_I2S_CLOCK_SOURCE clockSource)
    {
        i2s->GPRCM.CLKCFG = (I2S_CLKCFG_KEY_UNLOCK | (uint32_t)clockSource);
    }

    /**
     *  @brief      Get I2S audio clock configuration
     *              获取 I2S 当前主时钟源配置。
     *
     *  @param[in]  i2s     Pointer to the register overlay for the peripheral
     *                      I2S 外设寄存器基址。
     *
     *  @return     The currently configured clock for I2S
     *              当前主时钟源。
     *
     *  @retval     One of @ref DL_I2S_CLOCK_SOURCE
     */
    __STATIC_INLINE DL_I2S_CLOCK_SOURCE DL_I2S_getClockSource(const I2S_Regs *i2s)
    {
        uint32_t clockSource = i2s->GPRCM.CLKCFG & I2S_CLKCFG_DAICLK_MASK;

        return ((DL_I2S_CLOCK_SOURCE)(clockSource));
    }

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_I2S__ */

#endif /* ti_dl_dl_i2s__include */
/** @}*/
