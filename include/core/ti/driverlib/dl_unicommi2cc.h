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
 *  @file       dl_unicommi2cc.h
 *  @brief      Unified Communication Module (UNICOMM) - Inter-Integrated Circuit Controller (I2CC) Driver Library
 *              统一通信模块（UNICOMM）I2C 控制器（I2CC）驱动库，提供 I2C 主机模式的完整配置与数据传输接口。
 *  @defgroup   UNICOMMI2CC Unified Communication Module (UNICOMM) - Inter-Integrated Circuit Controller (I2CC)
 *
 *  @anchor ti_dl_dl_m0p_unicommi2cc_Overview
 *  # Overview
 *
 *  Unified Communication Module Inter-Integrated Circuit Controller Driver Library allows
 *  full configuration of the UNICOMM I2CC module.
 *
 *  This Inter-Integrated Circuit Controller (I2CC) module provides a standardized
 *  interface to transfer data, as a controller, between devices and other external
 *  devices with the I2C interface.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup UNICOMMI2CC
 * @{
 */
#ifndef ti_dl_dl_unicommi2cc__include
#define ti_dl_dl_unicommi2cc__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/dl_unicomm.h>

#ifdef __MCU_HAS_UNICOMMI2CC__

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

/*!
 * @brief I2CC number of bytes which could be put into the TX FIFO
 *        TX FIFO 最大容量（字节数），由器件头文件中的 UNICOMMI2CC_SYS_FENTRIES 决定。
 *
 * This variable is device specific and is calculated using the system parameter
 * UNICOMMI2CC_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2CC_TX_FIFO_COUNT_MAXIMUM          ((uint32_t)UNICOMMI2CC_SYS_FENTRIES << 8)

/*!
 * @brief I2CC number of bytes which could be put into the RX FIFO
 *        RX FIFO 最大容量（字节数），由器件头文件中的 UNICOMMI2CC_SYS_FENTRIES 决定。
 *
 * This variable is device specific and is calculated using the system parameter
 * UNICOMMI2CC_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2CC_RX_FIFO_COUNT_MAXIMUM               ((uint32_t)UNICOMMI2CC_SYS_FENTRIES)

/** @addtogroup DL_I2CC_STATUS
 *  @{
 */
/*!
 * @brief I2CC is busy
 *        I2CC 忙标志，事务进行中置位（含 START/RESTART/地址/STOP 等信号）。
 *
 * The BUSY bit is set during an ongoing transaction, so is set during the
 * transmit/receive of the amount of data set in WBLEN including START, RESTART,
 * Address and STOP signal generation when required for the current transaction
 */
#define DL_I2CC_STATUS_BUSY                       (UNICOMMI2CC_SR_BUSY_MASK)

/*!
 * @brief I2CC error detected
 *        I2CC 错误标志，目标地址或发送数据未被应答时置位。
 *
 * The error can be from the target address not being acknowledged or the
 * transmit data not being acknowledged
 */
#define DL_I2CC_STATUS_ERROR                       (UNICOMMI2CC_SR_ERR_MASK)

/*!
 * @brief I2CC address acknowledged
 *        I2CC 地址应答标志。
 */
#define DL_I2CC_STATUS_ADDR_ACK                 (UNICOMMI2CC_SR_ADRACK_MASK)

/*!
 * @brief I2CC data acknowledged
 *        I2CC 数据应答标志。
 */
#define DL_I2CC_STATUS_DATA_ACK                 (UNICOMMI2CC_SR_DATACK_MASK)

/*!
 * @brief I2CC arbitration lost
 *        I2CC 仲裁丢失标志。
 */
#define DL_I2CC_STATUS_ARBITRATION_LOST         (UNICOMMI2CC_SR_ARBLST_MASK)

/*!
 * @brief I2CC idle
 *        I2CC 空闲标志。
 */
#define DL_I2CC_STATUS_IDLE                       (UNICOMMI2CC_SR_IDLE_MASK)

/*!
 * @brief I2CC bus busy
 *        I2CC 总线忙标志，随 START/STOP 条件变化。
 *
 * The bit changes based on the START and STOP conditions
 */
#define DL_I2CC_STATUS_BUSY_BUS                 (UNICOMMI2CC_SR_BUSBSY_MASK)

/** @}*/


/** @addtogroup DL_I2CC_INTERRUPT
 *  @{
 */
/*!
 * @brief Receive Transaction completed Interrupt
 *        接收事务完成中断。
 */
#define DL_I2CC_INTERRUPT_RX_DONE  (UNICOMMI2CC_CPU_INT_IMASK_RXDONE_SET)

/*!
 * @brief Transmit Transaction completed Interrupt
 *        发送事务完成中断。
 */
#define DL_I2CC_INTERRUPT_TX_DONE  (UNICOMMI2CC_CPU_INT_IMASK_TXDONE_SET)

/*!
 * @brief Receive FIFO Trigger when >= defined bytes
 *        RX FIFO 触发中断（数据量达到阈值时触发）。
 */
#define DL_I2CC_INTERRUPT_RXFIFO_TRIGGER                            \
                                          (UNICOMMI2CC_CPU_INT_IMASK_RXTRG_SET)

/*!
 * @brief Transmit FIFO Trigger when <= defined bytes
 *        TX FIFO 触发中断（数据量低于阈值时触发）。
 */
#define DL_I2CC_INTERRUPT_TXFIFO_TRIGGER                            \
                                          (UNICOMMI2CC_CPU_INT_IMASK_TXTRG_SET)

/*!
 * @brief Receive FIFO is full
 *        RX FIFO 满中断。
 */
#define DL_I2CC_INTERRUPT_RXFIFO_FULL                               \
                                         (UNICOMMI2CC_CPU_INT_IMASK_RXFULL_SET)

/*!
 * @brief Transmit FIFO is empty
 *        TX FIFO 空中断。
 */
#define DL_I2CC_INTERRUPT_TXFIFO_EMPTY                              \
                                            (UNICOMMI2CC_CPU_INT_IMASK_TXEMPTY_SET)

/*!
 * @brief Address/Data NACK Interrupt
 *        地址/数据 NACK 中断。
 */
#define DL_I2CC_INTERRUPT_NACK       (UNICOMMI2CC_CPU_INT_IMASK_NACK_SET)

/*!
 * @brief START Detection Interrupt
 *        START 条件检测中断。
 */
#define DL_I2CC_INTERRUPT_START     (UNICOMMI2CC_CPU_INT_IMASK_START_SET)

/*!
 * @brief STOP Detection Interrupt
 *        STOP 条件检测中断。
 */
#define DL_I2CC_INTERRUPT_STOP       (UNICOMMI2CC_CPU_INT_IMASK_STOP_SET)

/*!
 * @brief Arbitration Lost Interrupt
 *        仲裁丢失中断。
 */
#define DL_I2CC_INTERRUPT_ARBITRATION_LOST                          \
                                            (UNICOMMI2CC_CPU_INT_IMASK_ARBLOST_SET)

/*!
 * @brief DMA Done on Event 1 publisher
 *        事件 1 DMA 完成中断（发送方向）。
 */
#define DL_I2CC_INTERRUPT_EVENT1_DMA_DONE                           \
                                       (UNICOMMI2CC_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief DMA Done on Event 2 publisher
 *        事件 2 DMA 完成中断（接收方向）。
 */
#define DL_I2CC_INTERRUPT_EVENT2_DMA_DONE                           \
                                       (UNICOMMI2CC_CPU_INT_IMASK_DMA_DONE_RX_SET)


/*!
 * @brief SMBus/PMBus PEC Receive Error Interrupt
 *        SMBus/PMBus PEC 接收错误中断。
 */
#define DL_I2CC_INTERRUPT_PEC_RX_ERROR                              \
                                         (UNICOMMI2CC_CPU_INT_IMASK_PEC_RX_ERR_SET)

/*!
 * @brief Timeout A Interrupt
 *        超时 A 中断（用于 SCL 低电平检测）。
 */
#define DL_I2CC_INTERRUPT_TIMEOUT_A          (UNICOMMI2CC_CPU_INT_IMASK_TIMEOUTA_SET)

/*!
 * @brief Timeout B Interrupt
 *        超时 B 中断（用于 SCL 高电平检测）。
 */
#define DL_I2CC_INTERRUPT_TIMEOUT_B          (UNICOMMI2CC_CPU_INT_IMASK_TIMEOUTB_SET)


/** @}*/

/** @addtogroup DL_I2CC_DMA_INTERRUPT
 *  @{
 */
/*!
 * @brief Controller Transmit FIFO Trigger interrupt for DMA trigger
 *        TX FIFO 触发 DMA 中断。
 */
#define DL_I2CC_DMA_INTERRUPT_TXFIFO_TRIGGER                            \
                                          (UNICOMMI2CC_DMA_TRIG0_IMASK_TXTRG_SET)

/*!
 * @brief Controller Receive FIFO Trigger interrupt for DMA trigger
 *        RX FIFO 触发 DMA 中断。
 */
#define DL_I2CC_DMA_INTERRUPT_RXFIFO_TRIGGER                        \
                                          (UNICOMMI2CC_DMA_TRIG1_IMASK_RXTRG_SET)

/** @}*/

    /* clang-format on */

    /** @enum DL_I2CC_EVENT_ROUTE I2CC 事件路由选择 */
    typedef enum
    {
        /*! I2CC event route 1 事件路由 1 */
        DL_I2CC_EVENT_ROUTE_1 = 0,
        /*! I2CC event route 2 事件路由 2 */
        DL_I2CC_EVENT_ROUTE_2 = 12
    } DL_I2CC_EVENT_ROUTE;

    /** @enum DL_I2CC_CLOCK I2CC 时钟源选择 */
    typedef enum
    {
        /*!  BUSCLK enabled as I2CC clock source 总线时钟 */
        DL_I2CC_CLOCK_BUSCLK = UNICOMMI2CC_CLKSEL_BUSCLK_SEL_ENABLE,
        /*!  MFCLK enabled as I2CC clock source 主功能时钟 */
        DL_I2CC_CLOCK_MFCLK = UNICOMMI2CC_CLKSEL_MFCLK_SEL_ENABLE,
        /*!  Asynchronous SYSCLK enabled as I2CC clock source 异步系统时钟 */
        DL_I2CC_CLOCK_ASYNC_SYSCLK = UNICOMMI2CC_CLKSEL_ASYNC_SYSCLK_SEL_ENABLE,
        /*!  Asynchronous HFCLK enabled as I2CC clock source 异步高频时钟 */
        DL_I2CC_CLOCK_ASYNC_HFCLK = UNICOMMI2CC_CLKSEL_ASYNC_HFCLK_SEL_ENABLE,
        /*!  Asynchronous PLL enabled as I2CC clock source 异步 PLL 时钟 */
        DL_I2CC_CLOCK_ASYNC_PLL = UNICOMMI2CC_CLKSEL_ASYNC_PLL_SEL_ENABLE,
    } DL_I2CC_CLOCK;

    /** @enum DL_I2CC_CLOCK_DIVIDE I2CC 时钟分频系数 */
    typedef enum
    {
        /*!  I2CC Clock Divided by 1 */
        DL_I2CC_CLOCK_DIVIDE_1 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_1,
        /*!  I2CC Clock Divided by 2 */
        DL_I2CC_CLOCK_DIVIDE_2 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_2,
        /*!  I2CC Clock Divided by 3 */
        DL_I2CC_CLOCK_DIVIDE_3 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_3,
        /*!  I2CC Clock Divided by 4 */
        DL_I2CC_CLOCK_DIVIDE_4 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_4,
        /*!  I2CC Clock Divided by 5 */
        DL_I2CC_CLOCK_DIVIDE_5 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_5,
        /*!  I2CC Clock Divided by 6 */
        DL_I2CC_CLOCK_DIVIDE_6 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_6,
        /*!  I2CC Clock Divided by 7 */
        DL_I2CC_CLOCK_DIVIDE_7 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_7,
        /*!  I2CC Clock Divided by 8 */
        DL_I2CC_CLOCK_DIVIDE_8 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_8,
    } DL_I2CC_CLOCK_DIVIDE;

    /** @enum DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH I2CC 数字毛刺滤波宽度（功能时钟周期数） */
    typedef enum
    {
        /*!  Pulses filtering disabled. 禁用毛刺滤波 */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_DISABLED =
            UNICOMMI2CC_GFCTL_DGFSEL_DISABLED,
        /*!  Pulses shorter than 1 functional clock tick in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_1 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_1,
        /*!  Pulses shorter than 2 functional clock ticks in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_2 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_2,
        /*!  Pulses shorter than 3 functional clock ticks in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_3 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_3,
        /*!  Pulses shorter than 4 functional clock ticks in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_4 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_4,
        /*!  Pulses shorter than 8 functional clock ticks in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_8 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_8,
        /*!  Pulses shorter than 16 functional clock ticks in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_16 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_16,
        /*!  Pulses shorter than 31 functional clock ticks in length are filtered.  */
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_31 =
            UNICOMMI2CC_GFCTL_DGFSEL_CLK_31,
    } DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH;

    /** @enum DL_I2CC_DIRECTION I2CC 数据传输方向 */
    typedef enum
    {
        /*! Set direction to transmitting to target 发送方向（写） */
        DL_I2CC_DIRECTION_TX = UNICOMMI2CC_TA_DIR_TRANSMIT,
        /*! Set direction to receiving from target 接收方向（读） */
        DL_I2CC_DIRECTION_RX = UNICOMMI2CC_TA_DIR_RECEIVE,
    } DL_I2CC_DIRECTION;

    /** @enum DL_I2CC_ADDRESSING_MODE I2CC 寻址模式 */
    typedef enum
    {
        /*! Enable in 7-bit addressing mode 7 位地址模式 */
        DL_I2CC_ADDRESSING_MODE_7_BIT = UNICOMMI2CC_TA_MODE_MODE7,
        /*! Enable in 10-bit addressing mode 10 位地址模式 */
        DL_I2CC_ADDRESSING_MODE_10_BIT = UNICOMMI2CC_TA_MODE_MODE10,
    } DL_I2CC_ADDRESSING_MODE;

    /** @enum DL_I2CC_PEC_STATUS I2CC PEC 校验状态 */
    typedef enum
    {
        /*!  I2CC SMBus/PMBus PEC was checked in the transaction that
         *   occurred before the last Stop 上次事务已进行 PEC 校验 */
        DL_I2CC_PEC_STATUS_CHECKED = UNICOMMI2CC_PECSR_PECSTS_CHECK_SET,
        /*!  I2CC SMBus/PMBus PEC was not checked in the transaction that
         *   occurred the last Stop 上次事务未进行 PEC 校验 */
        DL_I2CC_PEC_STATUS_NOT_CHECKED = UNICOMMI2CC_PECSR_PECSTS_CHECK_CLEARED,
    } DL_I2CC_PEC_STATUS;

    /** @enum DL_I2CC_PEC_CHECK_ERROR I2CC PEC 校验错误状态 */
    typedef enum
    {
        /*! I2CC SMBus/PMBus PEC check error occurred in the transaction
         *  before the last stop 上次事务 PEC 校验出错 */
        DL_I2CC_PEC_CHECK_ERROR_SET = UNICOMMI2CC_PECSR_PECSTS_ERROR_SET,
        /*! I2CC SMBus/PMBus PEC check error did not occur in the
         *  transaction before the last stop 上次事务 PEC 校验无误 */
        DL_I2CC_PEC_CHECK_ERROR_CLEARED = UNICOMMI2CC_PECSR_PECSTS_ERROR_CLEARED,
    } DL_I2CC_PEC_CHECK_ERROR;

    /** @enum DL_I2CC_SCL I2CC SCL 信号电平 */
    typedef enum
    {
        /*!  I2CC SCL signal high SCL 高电平 */
        DL_I2CC_SCL_HIGH = UNICOMMI2CC_BMON_SCL_SET,
        /*!  I2CC SCL signal low SCL 低电平 */
        DL_I2CC_SCL_LOW = UNICOMMI2CC_BMON_SCL_CLEARED,
    } DL_I2CC_SCL;

    /** @enum DL_I2CC_SDA I2CC SDA 信号电平 */
    typedef enum
    {
        /*!  I2CC SDA signal high SDA 高电平 */
        DL_I2CC_SDA_HIGH = UNICOMMI2CC_BMON_SDA_SET,
        /*!  I2CC SDA signal low SDA 低电平 */
        DL_I2CC_SDA_LOW = UNICOMMI2CC_BMON_SDA_CLEARED,
    } DL_I2CC_SDA;

    /** @enum DL_I2CC_START I2CC START 条件控制 */
    typedef enum
    {
        /*!  generates START condition 生成 START 条件 */
        DL_I2CC_START_ENABLE = UNICOMMI2CC_CTR_START_ENABLE,
        /*!  doesn't generate START condition 不生成 START 条件 */
        DL_I2CC_START_DISABLE = UNICOMMI2CC_CTR_START_DISABLE,
    } DL_I2CC_START;

    /** @enum DL_I2CC_STOP I2CC STOP 条件控制 */
    typedef enum
    {
        /*!  generates STOP condition 生成 STOP 条件 */
        DL_I2CC_STOP_ENABLE = UNICOMMI2CC_CTR_STOP_ENABLE,
        /*!  doesn't generate STOP condition 不生成 STOP 条件 */
        DL_I2CC_STOP_DISABLE = UNICOMMI2CC_CTR_STOP_DISABLE,
    } DL_I2CC_STOP;

    /** @enum DL_I2CC_ACK I2CC 应答控制 */
    typedef enum
    {
        /*!  Last received data byte of a transaction is not ACKed automatically 启用 ACK（最后字节不自动应答） */
        DL_I2CC_ACK_ENABLE = UNICOMMI2CC_CTR_ACK_ENABLE,
        /*!  Last received data byte of a transaction is ACKed automatically 禁用 ACK（所有字节自动应答） */
        DL_I2CC_ACK_DISABLE = UNICOMMI2CC_CTR_ACK_DISABLE,
    } DL_I2CC_ACK;

    /** @enum DL_I2CC_TX_FIFO_LEVEL I2CC TX FIFO 中断触发阈值 */
    typedef enum
    {
        /*! Interrupt triggers when FIFO <= 3/4 empty */
        DL_I2CC_TX_FIFO_LEVEL_3_4_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_3_4,
        /*! Interrupt triggers when FIFO <= 1/2 empty */
        DL_I2CC_TX_FIFO_LEVEL_1_2_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_1_2,
        /*! Interrupt triggers when FIFO <= 1/4 empty */
        DL_I2CC_TX_FIFO_LEVEL_1_4_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_1_4,
        /*! Interrupt triggers when FIFO is not full */
        DL_I2CC_TX_FIFO_LEVEL_ONE_ENTRY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_NOT_FULL,
        /*! Interrupt triggers when FIFO is empty */
        DL_I2CC_TX_FIFO_LEVEL_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_EMPTY,
        /*! Interrupt triggers when FIFO is almost empty (FIFO <= 1) */
        DL_I2CC_TX_FIFO_LEVEL_ALMOST_EMPTY =
            UNICOMMI2CC_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY,
        /*! Interrupt triggers when FIFO is almost full (FIFO >= (MAX_FIFO_LEN - 1)) */
        DL_I2CC_TX_FIFO_LEVEL_ALMOST_FULL =
            UNICOMMI2CC_IFLS_TXIFLSEL_LVL_ALMOST_FULL
    } DL_I2CC_TX_FIFO_LEVEL;

    /** @enum DL_I2CC_RX_FIFO_LEVEL I2CC RX FIFO 中断触发阈值 */
    typedef enum
    {
        /*! Interrupt triggers when FIFO >= 3/4 full */
        DL_I2CC_RX_FIFO_LEVEL_3_4_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_3_4,
        /*! Interrupt triggers when FIFO >= 1/2 full */
        DL_I2CC_RX_FIFO_LEVEL_1_2_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_1_2,
        /*! Interrupt triggers when FIFO >= 1/4 full */
        DL_I2CC_RX_FIFO_LEVEL_1_4_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_1_4,
        /*! Interrupt triggers when FIFO is not empty */
        DL_I2CC_RX_FIFO_LEVEL_ONE_ENTRY = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_NOT_EMPTY,
        /*! Interrupt triggers when FIFO is full */
        DL_I2CC_RX_FIFO_LEVEL_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_FULL,
        /*! Interrupt triggers when FIFO is almost full (FIFO >= (MAX_FIFO_LEN - 1)) */
        DL_I2CC_RX_FIFO_LEVEL_ALMOST_FULL =
            UNICOMMI2CC_IFLS_RXIFLSEL_LVL_ALMOST_FULL,
        /*! Interrupt triggers when FIFO is almost empty (FIFO <= 1) */
        DL_I2CC_RX_FIFO_LEVEL_ALMOST_EMPTY =
            UNICOMMI2CC_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY,
    } DL_I2CC_RX_FIFO_LEVEL;

    /*! @enum DL_I2CC_IIDX I2CC 中断索引 */
    typedef enum
    {
        /*! Interrupt index for I2CC if no interrupt is pending 无中断 */
        DL_I2CC_IIDX_NO_INT = UNICOMMI2CC_IIDX_STAT_NO_INTR,
        /*! Interrupt index for I2CC Receive Transaction completed 接收完成 */
        DL_I2CC_IIDX_RX_DONE = UNICOMMI2CC_IIDX_STAT_RXDONEFG,
        /*! Interrupt index for Transmit Transaction completed 发送完成 */
        DL_I2CC_IIDX_TX_DONE = UNICOMMI2CC_IIDX_STAT_TXDONEFG,
        /*! Interrupt index for I2CC Receive FIFO Trigger RX FIFO 触发 */
        DL_I2CC_IIDX_RXFIFO_TRIGGER = UNICOMMI2CC_IIDX_STAT_RXTRG,
        /*! Interrupt index for I2CC Transmit FIFO Trigger TX FIFO 触发 */
        DL_I2CC_IIDX_TXFIFO_TRIGGER = UNICOMMI2CC_IIDX_STAT_TXTRG,
        /*! Interrupt index for I2CC Receive when FIFO is full RX FIFO 满 */
        DL_I2CC_IIDX_RXFIFO_FULL = UNICOMMI2CC_IIDX_STAT_RXFULL,
        /*! Interrupt index for I2CC when Transmit FIFO is empty TX FIFO 空 */
        DL_I2CC_IIDX_TXFIFO_EMPTY = UNICOMMI2CC_IIDX_STAT_TXEMPTY,
        /*! Interrupt index for Address/Data NACK 地址/数据无应答 */
        DL_I2CC_IIDX_NACK = UNICOMMI2CC_IIDX_STAT_NACKFG,
        /*! Interrupt index for I2CC START Detection START 条件检测 */
        DL_I2CC_IIDX_START = UNICOMMI2CC_IIDX_STAT_STARTFG,
        /*! Interrupt index for I2CC STOP Detection STOP 条件检测 */
        DL_I2CC_IIDX_STOP = UNICOMMI2CC_IIDX_STAT_STOPFG,
        /*! Interrupt index for I2CC Arbitration Lost 仲裁丢失 */
        DL_I2CC_IIDX_ARBITRATION_LOST = UNICOMMI2CC_IIDX_STAT_ARBLOSTFG,
        /*! Interrupt index for I2CC Event 1 DMA Done 事件 1 DMA 完成 */
        DL_I2CC_IIDX_EVENT1_DMA_DONE = UNICOMMI2CC_IIDX_STAT_DMA_DONE_TX,
        /*! Interrupt index for I2CC Event 2 DMA Done 事件 2 DMA 完成 */
        DL_I2CC_IIDX_EVENT2_DMA_DONE = UNICOMMI2CC_IIDX_STAT_DMA_DONE_RX,
        /*! Interrupt index for I2CC receive pre-emptive interrupt 发送预抢占中断 */
        DL_I2CC_IIDX_PREEMPT_INT_TX = UNICOMMI2CC_IIDX_STAT_DMA_PREIRQ_TX,
        /*! Interrupt index for I2CC receive pre-emptive interrupt 接收预抢占中断 */
        DL_I2CC_IIDX_PREEMPT_INT_RX = UNICOMMI2CC_IIDX_STAT_DMA_PREIRQ_RX,
        /*! Interrupt index for I2CC SMBus/PMBus PEC Receive Error Event PEC 接收错误 */
        DL_I2CC_IIDX_PEC_RX_ERROR = UNICOMMI2CC_IIDX_STAT_PEC_RX_ERR,
        /*! Interrupt index for I2CC Timeout A Event 超时 A */
        DL_I2CC_IIDX_TIMEOUT_A = UNICOMMI2CC_IIDX_STAT_TIMEOUTA,
        /*! Interrupt index for I2CC Timeout B Event 超时 B */
        DL_I2CC_IIDX_TIMEOUT_B = UNICOMMI2CC_IIDX_STAT_TIMEOUTB,
    } DL_I2CC_IIDX;

    /**
     * @brief  Configuration struct for @ref DL_I2CC_setClockConfig.
     *         I2CC 时钟配置结构体。
     */
    typedef struct
    {
        /*! I2CC module clock source. One of @ref DL_I2CC_CLOCK 时钟源选择 */
        DL_I2CC_CLOCK clockSel;
        /*! I2CC clock divider selection. One of @ref DL_I2CC_CLOCK_DIVIDE 时钟分频系数 */
        DL_I2CC_CLOCK_DIVIDE divideRatio;
    } DL_I2CC_ClockConfig;

    /**
     *  @brief      Configure I2CC source clock
     *  @details    功能：配置 UNICOMM I2CC 模式的时钟源与分频参数。
     *
     *  @param[in]  unicomm    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：UNICOMM 外设寄存器实例。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                       @ref DL_I2CC_ClockConfig.
     *                       参数：时钟配置结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_I2CC_setClockConfig(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_ClockConfig *config);

    /**
     *  @brief      Get I2CC source clock configuration
     *  @details    功能：读取 UNICOMM I2CC 模式当前时钟配置。
     *
     *  @param[in]  unicomm    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：UNICOMM 外设寄存器实例。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                      @ref DL_I2CC_ClockConfig.
     *                      参数：用于回填配置的结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_I2CC_getClockConfig(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_ClockConfig *config);

    /**
     *  @brief      Fills the TX FIFO with data
     *  @details    功能：批量写入 I2CC TX FIFO，直到 FIFO 满或达到上限。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  buffer  Pointer to buffer of bytes
     *                      参数：发送缓冲区。
     *  @param[in]  count   Number of bytes to fill TX FIFO from buffer
     *                      [0x00, 0xFFF]
     *                      参数：最大写入数量。
     *
     *  @return     Number of bytes that were successfully written
     *              返回值：实际写入字节数。
     */
    uint16_t DL_I2CC_fillTXFIFO(
        UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint16_t count);

    /**
     *  @brief      Flushes/removes all elements in the TX FIFO
     *  @details    功能：清空 I2CC TX FIFO 并等待硬件完成。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     返回值：无。
     */
    void DL_I2CC_flushTXFIFO(UNICOMM_Inst_Regs *unicomm);

    /**
     *  @brief      Flushes/removes all elements in the RX FIFO
     *  @details    功能：清空 I2CC RX FIFO 并等待硬件完成。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     返回值：无。
     */
    void DL_I2CC_flushRXFIFO(UNICOMM_Inst_Regs *unicomm);

    /**
     *  @brief      Stop TX FIFO flush
     *              停止 TX FIFO 刷新。
     *
     *  Before stopping the flush, check if @ref DL_I2CC_isTXFIFOEmpty,
     *  indicating flush is complete.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_stopFlushTXFIFO(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->IFLS &= ~(UNICOMMI2CC_IFLS_TXCLR_MASK);
    }

    /**
     *  @brief      Start TX FIFO flush
     *              启动 TX FIFO 刷新。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_startFlushTXFIFO(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->IFLS |= UNICOMMI2CC_IFLS_TXCLR_MASK;
    }

    /**
     *  @brief      Stop RX FIFO flush
     *              停止 RX FIFO 刷新。
     *
     *  Before stopping the flush, check if @ref DL_I2CC_isRXFIFOEmpty,
     *  indicating flush is complete.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_stopFlushRXFIFO(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->IFLS &= ~(UNICOMMI2CC_IFLS_RXCLR_MASK);
    }

    /**
     *  @brief      Start RX FIFO flush
     *              启动 RX FIFO 刷新。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_startFlushRXFIFO(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->IFLS |= UNICOMMI2CC_IFLS_RXCLR_MASK;
    }

    /**
     *  @brief      Checks if TX FIFO is full
     *              检查 TX FIFO 是否已满。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If TX FIFO is full
     *              返回值：TX FIFO 是否已满。
     *
     *  @retval     true if TX FIFO is full
     *  @retval     false if TX FIFO is not full
     */
    __STATIC_INLINE bool DL_I2CC_isTXFIFOFull(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_TXFF_MASK) ==
                UNICOMMI2CC_SR_TXFF_SET);
    }

    /**
     *  @brief      Checks if TX FIFO is empty
     *              检查 TX FIFO 是否为空。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If TX FIFO is empty
     *              返回值：TX FIFO 是否为空。
     *
     *  @retval     true if TX FIFO is empty
     *  @retval     false if TX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2CC_isTXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_TXFE_MASK) ==
                UNICOMMI2CC_SR_TXFE_SET);
    }

    /**
     *  @brief      Checks if RX FIFO is full
     *              检查 RX FIFO 是否已满。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If RX FIFO is full
     *              返回值：RX FIFO 是否已满。
     *
     *  @retval     true if RX FIFO is full
     *  @retval     false if RX FIFO is not full
     */
    __STATIC_INLINE bool DL_I2CC_isRXFIFOFull(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_RXFF_MASK) ==
                UNICOMMI2CC_SR_RXFF_SET);
    }

    /**
     *  @brief      Checks if RX FIFO is empty
     *              检查 RX FIFO 是否为空。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If RX FIFO is empty
     *              返回值：RX FIFO 是否为空。
     *
     *  @retval     true if RX FIFO is empty
     *  @retval     false if RX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2CC_isRXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_RXFE_MASK) ==
                UNICOMMI2CC_SR_RXFE_SET);
    }

    /**
     *  @brief      Reset transfers from from UNICOMMI2CC
     *              复位 I2CC 传输控制寄存器。
     *
     *  Resets transfer register to initialize UNICOMMI2CC
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_resetTransfer(UNICOMM_Inst_Regs *unicomm)
    {
        // Resets all functionality
        unicomm->i2cc->CTR = 0x00;
    }

    /**
     *  @brief      Sets up a transfer from UNICOMMI2CC
     *              配置 I2CC 传输参数（地址、方向、突发长度、START/STOP）。
     *
     *  Set target address, transfer direction, burst length, START+STOP generation.
     *  @note   Reading/writing data must be done separately.
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *  @param[in]  targetAddr  Target address [0x00, 0x3FF]
     *                          参数：目标从机地址。
     *  @param[in]  direction   One of @ref DL_I2CC_DIRECTION
     *                          参数：传输方向。
     *  @param[in]  length      Intended burst length in number of bytes
     *                          [0x00, 0xFFF]
     *                          参数：突发传输字节数。
     */
    __STATIC_INLINE void DL_I2CC_startTransfer(UNICOMM_Inst_Regs *unicomm,
                                               uint32_t targetAddr, DL_I2CC_DIRECTION direction, uint16_t length)
    {
        // Specify target address and read/write mode 设置目标地址和读写方向
        DL_Common_updateReg(&unicomm->i2cc->TA,
                            ((targetAddr << UNICOMMI2CC_TA_ADDR_OFS) | (uint32_t)direction),
                            (UNICOMMI2CC_TA_ADDR_MASK | UNICOMMI2CC_TA_DIR_MASK));

        // STOP bit is generated after burst length number of bytes transferred 传输完成后自动生成 STOP
        DL_Common_updateReg(&unicomm->i2cc->CTR,
                            (((uint32_t)length << UNICOMMI2CC_CTR_BLEN_OFS) |
                             UNICOMMI2CC_CTR_FRM_START_ENABLE | UNICOMMI2CC_CTR_START_ENABLE |
                             UNICOMMI2CC_CTR_STOP_ENABLE),
                            (UNICOMMI2CC_CTR_BLEN_MASK | UNICOMMI2CC_CTR_FRM_START_MASK |
                             UNICOMMI2CC_CTR_START_MASK | UNICOMMI2CC_CTR_STOP_MASK));
    }

    /**
     *  @brief      Sets up a transfer from I2CC with control of START,
     *  STOP and ACK
     *              配置 I2CC 高级传输参数（可单独控制 START/STOP/ACK）。
     *
     *  @note   Reading/writing data must be done separately.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  targetAddr  7-bit target address [0x00, 0x7f]
     *                          参数：目标从机地址。
     *  @param[in]  direction   One of @ref DL_I2CC_DIRECTION
     *                          参数：传输方向。
     *  @param[in]  length      Intended burst length in number of bytes
     *                          参数：突发传输字节数。
     *  @param[in]  start       One of @ref DL_I2CC_START
     *                          参数：START 条件控制。
     *  @param[in]  stop        One of @ref DL_I2CC_STOP
     *                          参数：STOP 条件控制。
     *  @param[in]  ack         One of @ref DL_I2CC_ACK
     *                          参数：ACK 控制。
     */
    __STATIC_INLINE void DL_I2CC_startTransferAdvanced(UNICOMM_Inst_Regs *unicomm,
                                                       uint32_t targetAddr, DL_I2CC_DIRECTION direction, uint16_t length,
                                                       DL_I2CC_START start, DL_I2CC_STOP stop, DL_I2CC_ACK ack)
    {
        // Specify target address and read/write mode 设置目标地址和读写方向
        DL_Common_updateReg(&unicomm->i2cc->TA,
                            ((targetAddr << UNICOMMI2CC_TA_ADDR_OFS) | (uint32_t)direction),
                            (UNICOMMI2CC_TA_ADDR_MASK | UNICOMMI2CC_TA_DIR_MASK));

        // 配置突发长度、帧起始、START/STOP/ACK 控制位
        DL_Common_updateReg(&unicomm->i2cc->CTR,
                            (((uint32_t)length << UNICOMMI2CC_CTR_BLEN_OFS) |
                             UNICOMMI2CC_CTR_FRM_START_ENABLE | (uint32_t)start |
                             (uint32_t)stop | (uint32_t)ack),
                            (UNICOMMI2CC_CTR_BLEN_MASK | UNICOMMI2CC_INTCTL_INTEVAL_MASK |
                             UNICOMMI2CC_CTR_START_MASK | UNICOMMI2CC_CTR_STOP_MASK |
                             UNICOMMI2CC_CTR_ACK_MASK));
    }

    /**
     * @brief Enables power on UNICOMM I2C module
     *        使能 UNICOMM I2C 模块电源。
     *
     * @param[in] unicomm        Pointer to the register overlay for the peripheral
     *                           参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enablePower(UNICOMM_Inst_Regs *unicomm)
    {
        DL_UNICOMM_enablePower(unicomm);
        if (unicomm->fixedMode == false)
        {
            DL_UNICOMM_setIPMode(unicomm, DL_UNICOMM_I2C_CONTROLLER);
        }
    }

    /**
     * @brief Disables power on UNICOMM I2C module
     *        关闭 UNICOMM I2C 模块电源。
     *
     * @param[in] unicomm        Pointer to the register overlay for the peripheral
     *                           参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disablePower(UNICOMM_Inst_Regs *unicomm)
    {
        DL_UNICOMM_disablePower(unicomm);
    }

    /**
     * @brief Returns if  power on UNICOMM I2C module
     *        查询 UNICOMM I2C 模块电源是否已使能。
     *
     * @param[in] unicomm        Pointer to the register overlay for the peripheral
     *                           参数：UNICOMM 外设寄存器实例。
     *
     * @return true if power is enabled
     * @return false if power is disabled
     *         返回值：`true` 已使能，`false` 已关闭。
     */
    __STATIC_INLINE bool DL_I2CC_isPowerEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return DL_UNICOMM_isPowerEnabled(unicomm);
    }

    /**
     * @brief Resets UNICOMM I2C peripheral
     *        复位 UNICOMM I2C 外设。
     *
     * @param[in] unicomm        Pointer to the register overlay for the peripheral
     *                           参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_reset(UNICOMM_Inst_Regs *unicomm)
    {
        DL_UNICOMM_reset(unicomm);
    }

    /**
     * @brief Returns if UNICOMM I2C peripheral was reset
     *        查询 UNICOMM I2C 外设是否已完成复位。
     *
     * @param[in] unicomm        Pointer to the register overlay for the peripheral
     *                           参数：UNICOMM 外设寄存器实例。
     *
     * @return true if peripheral was reset
     * @return false if peripheral wasn't reset
     *         返回值：`true` 已复位，`false` 未复位。
     */
    __STATIC_INLINE bool DL_I2CC_isReset(UNICOMM_Inst_Regs *unicomm)
    {
        return DL_UNICOMM_isReset(unicomm);
    }

    /**
     * @brief Set Clock Source
     *        设置 I2CC 时钟源。
     *
     *  Clock sources depend on device and clock should be enabled
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  clockSource    One of @ref DL_I2CC_CLOCK.
     *                             参数：时钟源选择。
     *
     */
    __STATIC_INLINE void DL_I2CC_selectClockSource(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_CLOCK clockSource)
    {
        DL_Common_updateReg(&unicomm->i2cc->CLKSEL, (uint32_t)clockSource,
                            UNICOMMI2CC_CLKSEL_BUSCLK_SEL_MASK |
                                UNICOMMI2CC_CLKSEL_MFCLK_SEL_MASK);
    }

    /**
     * @brief Set Clock Divider
     *        设置 I2CC 时钟分频系数。
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  clockDivider   One of @ref DL_I2CC_CLOCK_DIVIDE.
     *                             参数：时钟分频系数。
     *
     */
    __STATIC_INLINE void DL_I2CC_selectClockDivider(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_CLOCK_DIVIDE clockDivider)
    {
        DL_Common_updateReg(&unicomm->i2cc->CLKDIV, (uint32_t)clockDivider,
                            UNICOMMI2CC_CLKDIV_RATIO_MASK);
    }

    /**
     *  @brief      Get Digital Glitch Suppression Pulse Width
     *              获取 SCL/SDA 数字毛刺滤波宽度。
     *
     *  Pulse width for the digital glitch suppression on SCL/SDA lines.
     *  Values are in terms of functional clock ticks.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Digital glitch suppression pulse width
     *              返回值：数字毛刺滤波宽度枚举值。
     *
     *  @retval     One of @ref DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH.
     */
    __STATIC_INLINE DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH
    DL_I2CC_getDigitalGlitchFilterPulseWidth(UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t filterWidth =
            unicomm->i2cc->GFCTL & UNICOMMI2CC_GFCTL_DGFSEL_MASK;

        return (DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH)(filterWidth);
    }

    /**
     *  @brief      Set Digital Glitch Suppression Pulse Width
     *              设置 SCL/SDA 数字毛刺滤波宽度。
     *
     *  Pulse width for the digital glitch suppression on SCL/SDA lines.
     *  Values are in terms of functional clock ticks.
     *
     *  @param[in]  unicomm             Pointer to the register overlay for the
     *                              peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *  @param[in]  filterWidth     One of @ref DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH.
     *                              参数：滤波宽度枚举值。
     */
    __STATIC_INLINE void DL_I2CC_setDigitalGlitchFilterPulseWidth(
        UNICOMM_Inst_Regs *unicomm,
        DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH filterWidth)
    {
        DL_Common_updateReg(&unicomm->i2cc->GFCTL, (uint32_t)filterWidth,
                            UNICOMMI2CC_GFCTL_DGFSEL_MASK);
    }

    /**
     *  @brief      Disable Analog Glitch Suppression
     *              禁用 SCL/SDA 模拟毛刺抑制。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableAnalogGlitchFilter(
        UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->GFCTL &= ~(UNICOMMI2CC_GFCTL_AGFEN_MASK);
    }

    /**
     *  @brief      Checks if analog glitch suppression is enabled
     *              检查模拟毛刺抑制是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If analog glitch suppression is enabled
     *              返回值：模拟毛刺抑制是否已使能。
     *
     *  @retval     true if analog glitch suppression is enabled
     *  @retval     false if analog glitch suppression is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isAnalogGlitchFilterEnabled(
        UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->GFCTL & UNICOMMI2CC_GFCTL_AGFEN_MASK) ==
                UNICOMMI2CC_GFCTL_AGFEN_ENABLE);
    }

    /**
     *  @brief      Enable Analog Glitch Suppression
     *              使能 SCL/SDA 模拟毛刺抑制。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableAnalogGlitchFilter(
        UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->GFCTL |= UNICOMMI2CC_GFCTL_AGFEN_ENABLE;
    }

    /**
     *  @brief      Get direction of next operation
     *              获取下一次传输方向。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Direction of next operation
     *              返回值：传输方向枚举值。
     *
     *  @retval     One of @ref DL_I2CC_DIRECTION
     */
    __STATIC_INLINE DL_I2CC_DIRECTION DL_I2CC_getDirection(
        UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t direction = unicomm->i2cc->TA & UNICOMMI2CC_TA_DIR_MASK;

        return (DL_I2CC_DIRECTION)(direction);
    }

    /**
     *  @brief      Set direction of next operation
     *              设置下一次传输方向。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @param[in]  direction   Direction of next operation.
     *                          One of @ref DL_I2CC_DIRECTION.
     *                          参数：传输方向枚举值。
     */
    __STATIC_INLINE void DL_I2CC_setDirection(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_DIRECTION direction)
    {
        DL_Common_updateReg(
            &unicomm->i2cc->TA, (uint32_t)direction, UNICOMMI2CC_TA_DIR_MASK);
    }

    /**
     *  @brief      Get the address of the target being addressed when configured
     *              as an UNICOMMI2CC
     *              获取目标从机地址。
     *
     *  Specifies bits A9 through A0 of the target address.
     *  In 7-bit addressing mode as selected by @ref DL_I2CC_setAddressingMode,
     *  the top 3 bits are don't care.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The target address
     *              返回值：目标从机地址。
     *
     *  @retval     [0x00, 0x07FE]
     *
     *  @sa         DL_I2CC_setAddressingMode
     */
    __STATIC_INLINE uint32_t DL_I2CC_getTargetAddress(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->TA & UNICOMMI2CC_TA_ADDR_MASK) >>
                UNICOMMI2CC_TA_ADDR_OFS);
    }

    /**
     *  @brief      Set the address of the target being addressed when configured
     *              as an UNICOMMI2CC
     *              设置目标从机地址。
     *
     *  Specifies bits A9 through A0 of the target address.
     *  In 7-bit addressing mode as selected by @ref DL_I2CC_setAddressingMode,
     *  the top 3 bits are don't care.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  targetAddress    Target address to set [0x00, 0x07FE]
     *                              参数：目标从机地址。
     *
     *  @sa         DL_I2CC_setAddressingMode
     */
    __STATIC_INLINE void DL_I2CC_setTargetAddress(
        UNICOMM_Inst_Regs *unicomm, uint32_t targetAddress)
    {
        DL_Common_updateReg(&unicomm->i2cc->TA,
                            (targetAddress << UNICOMMI2CC_TA_ADDR_OFS), UNICOMMI2CC_TA_ADDR_MASK);
    }

    /**
     *  @brief      Get addressing mode
     *              获取当前寻址模式。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Addressing mode the is set to
     *              返回值：寻址模式枚举值。
     *
     *  @retval     One of @ref DL_I2CC_ADDRESSING_MODE
     *
     */
    __STATIC_INLINE DL_I2CC_ADDRESSING_MODE DL_I2CC_getAddressingMode(
        UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t mode = unicomm->i2cc->TA & UNICOMMI2CC_TA_MODE_MASK;

        return (DL_I2CC_ADDRESSING_MODE)(mode);
    }

    /**
     *  @brief      Set addressing mode between 7-bit and 10-bit mode
     *              设置寻址模式（7 位或 10 位）。
     *
     *  Selects the addressing mode between 7-bit and 10-bit mode to be used when
     *  device is configured as a
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  mode    Addressing mode to set the target to.
     *                      One of @ref DL_I2CC_ADDRESSING_MODE.
     *                      参数：寻址模式枚举值。
     */
    __STATIC_INLINE void DL_I2CC_setAddressingMode(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_ADDRESSING_MODE mode)
    {
        DL_Common_updateReg(
            &unicomm->i2cc->TA, (uint32_t)mode, UNICOMMI2CC_TA_MODE_MASK);
    }

    /**
     *  @brief      Disable ACK override
     *              禁用 ACK 覆盖模式。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableACKOverride(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_ACKOEN_MASK);
    }

    /**
     *  @brief      Checks if ACK override is enabled
     *              检查 ACK 覆盖模式是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If ACK override is enabled
     *              返回值：ACK 覆盖模式是否已使能。
     *
     *  @retval     true if ACK override is enabled
     *  @retval     false if ACK override is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isACKOverrideEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_ACKOEN_MASK) ==
                UNICOMMI2CC_CTR_ACKOEN_ENABLE);
    }

    /**
     *  @brief      Enable ACK override
     *              使能 ACK 覆盖模式，接收指定字节数后由固件决定 ACK/NACK。
     *
     *  When enabled and the is receiving data and the number of bytes
     *  indicated in CTR.MBLEN have been received, the state machine will generate
     *  an RXDONE interrupt and wait at the start of the ACK for FW to indicate if
     *  an ACK or NACK should be sent. The ACK or NACK is selected by writing the
     *  CTR register and setting ACK accordingly. The other fields in this register
     *  can also be written at this time to continue on with the transaction. If a
     *  NACK is sent the state machine will automatically send a Stop.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     * @sa          DL_I2CC_setTransactionLength
     */
    __STATIC_INLINE void DL_I2CC_enableACKOverride(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_ACKOEN_ENABLE;
    }

    /**
     *  @brief      Disable read on TX empty
     *              禁用 TX 空后自动切换读模式。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableReadOnTXEmpty(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_RD_ON_TXEMPTY_MASK);
    }

    /**
     *  @brief      Checks if read on TX empty is enabled
     *              检查 TX 空后自动切换读模式是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If read on TX empty is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if read on TX empty is enabled
     *  @retval     false if read on TX empty is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isReadOnTXEmptyEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_RD_ON_TXEMPTY_MASK) ==
                UNICOMMI2CC_CTR_RD_ON_TXEMPTY_ENABLE);
    }

    /**
     *  @brief      Enable read on TX empty
     *              使能 TX 空后自动切换读模式（用于命令-读取场景）。
     *
     *  When enabled, the will transmit all bytes from the TX FIFO
     *  before continuing with the programmed Burst Run Read. If the DIR is not
     *  set to Read in the MSA then this bit is ignored. The Start must be set in
     *  the CTR for proper I2CC protocol. The will first send the Start
     *  Condition, I2CC Address with R/W bit set to write, before sending the bytes
     *  in the TX FIFO. When the TX FIFO is empty, the I2CC transaction will
     *  continue as programmed in MTCR and MSA without sending a Stop Condition.
     *  This is intended to be used to perform simple I2CC command based reads
     *  transition that will complete after initiating them without having to get
     *  an interrupt to turn the bus around.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     */
    __STATIC_INLINE void DL_I2CC_enableReadOnTXEmpty(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_RD_ON_TXEMPTY_ENABLE;
    }

    /**
     *  @brief      Get the SMBus/PMBus Packet Error Checking (PEC) count value
     *              获取 PEC 计数预设值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The value the PEC count is set to
     *              返回值：PEC 计数预设值。
     *
     *  @retval     Value between [0x0, 0x01FF]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getPECCountValue(UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->PECCTL & UNICOMMI2CC_PECCTL_PECCNT_MASK);
    }

    /**
     *  @brief      Set the SMBus/PMBus Packet Error Checking (PEC) count value
     *              设置 PEC 计数预设值。
     *
     *  When this field is non zero, the number of I2CC data bytes are counted.
     *  Refer to the device TRM for more details.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  count   The value to set the PEC count to.
     *                      Value between [0x0, 0x01FF]
     *                      参数：PEC 计数值。
     */
    __STATIC_INLINE void DL_I2CC_setPECCountValue(
        UNICOMM_Inst_Regs *unicomm, uint32_t count)
    {
        DL_Common_updateReg(
            &unicomm->i2cc->PECCTL, count, UNICOMMI2CC_PECCTL_PECCNT_MASK);
    }

    /**
     *  @brief      Disable SMBus/PMBus Packet Error Checking (PEC)
     *              禁用 PEC 校验。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disablePEC(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->PECCTL &= ~(UNICOMMI2CC_PECCTL_PECEN_MASK);
    }

    /**
     *  @brief      Checks if SMBus/PMBus Packet Error Checking (PEC)
     *              is enabled
     *              检查 PEC 校验是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If PEC is enabled
     *              返回值：PEC 是否已使能。
     *
     *  @retval     true if PEC is enabled
     *  @retval     false if PEC is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isPECEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->PECCTL & UNICOMMI2CC_PECCTL_PECEN_MASK) ==
                UNICOMMI2CC_PECCTL_PECEN_ENABLE);
    }

    /**
     *  @brief      Enable SMBus/PMBus Packet Error Checking (PEC)
     *              使能 PEC 校验，多项式为 x^8 + x^2 + x^1 + 1。
     *
     *  When enabled, the PEC is calculated on all bits accept the Start, Stop, ACK
     *  and NACK. The PEC LSFR and the Byte Counter is set to 0 when the State
     *  Machine is in the IDLE state, which occurs following a Stop or when a
     *  timeout occurs. The Counter is also set to 0 after the PEC byte is sent or
     *  received. Note that the NACK is automatically sent following a PEC byte
     *  that results in a PEC error.
     *  The PEC Polynomial is x^8 + x^2 + x^1 + 1.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     */
    __STATIC_INLINE void DL_I2CC_enablePEC(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->PECCTL |= UNICOMMI2CC_PECCTL_PECEN_ENABLE;
    }

    /**
     *  @brief      Get the current SMBus/PMBus PEC byte count of the
     *              state machine
     *              获取当前 PEC 字节计数。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The current PEC byte count of the state machine
     *              返回值：当前 PEC 字节计数。
     *
     *  @retval     Value between [0x0, 0x01FF]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getCurrentPECCount(UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->PECSR & UNICOMMI2CC_PECSR_PECSTS_CHECK_MASK);
    }

    /**
     *  @brief      If SMBus/PMBus PEC was checked in last transaction
     *              查询上次事务是否进行了 PEC 校验。
     *
     *  The status of if the PEC was checked in the transaction that
     *  occurred before the last Stop. Latched on Stop.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Status of PEC checked in last transaction
     *              返回值：PEC 校验状态。
     *
     *  @retval     One of @ref DL_I2CC_PEC_STATUS
     */
    __STATIC_INLINE DL_I2CC_PEC_STATUS DL_I2CC_getPECCheckedStatus(
        UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t status =
            unicomm->i2cc->PECSR & UNICOMMI2CC_PECSR_PECSTS_CHECK_MASK;

        return (DL_I2CC_PEC_STATUS)(status);
    }

    /**
     *  @brief      Get the status of the SMBus/PMBus PEC Check error
     *              查询上次事务 PEC 校验是否出错。
     *
     *  The status of if a PEC check error occurred in the transaction that
     *  occurred before the last Stop. Latched on Stop.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Status of PEC check error
     *              返回值：PEC 校验错误状态。
     *
     *  @retval     One of @ref DL_I2CC_PEC_CHECK_ERROR
     */
    __STATIC_INLINE DL_I2CC_PEC_CHECK_ERROR DL_I2CC_getPECCheckError(
        UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t error =
            unicomm->i2cc->PECSR & UNICOMMI2CC_PECSR_PECSTS_ERROR_MASK;

        return (DL_I2CC_PEC_CHECK_ERROR)(error);
    }

    /**
     *  @brief      Disable I2CC START generation
     *              禁用 START 条件生成。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableStartCondition(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_START_MASK);
    }

    /**
     *  @brief      Checks if I2CC START generation is enabled
     *              检查 START 条件生成是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If I2CC START generation is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if I2CC START generation is enabled
     *  @retval     false if I2CC START generation is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isStartConditionEnabled(
        UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_START_MASK) ==
                UNICOMMI2CC_CTR_START_ENABLE);
    }

    /**
     *  @brief      Enable I2CC START generation
     *              使能 START 条件生成。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableStartCondition(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_START_ENABLE;
    }

    /**
     *  @brief      Disable I2CC STOP generation
     *              禁用 STOP 条件生成。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableStopCondition(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_STOP_MASK);
    }

    /**
     *  @brief      Checks if I2CC STOP generation is enabled
     *              检查 STOP 条件生成是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If I2CC STOP generation is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if I2CC STOP generation is enabled
     *  @retval     false if I2CC STOP generation is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isStopConditionEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_STOP_MASK) ==
                UNICOMMI2CC_CTR_STOP_ENABLE);
    }

    /**
     *  @brief      Enable I2CC STOP generation
     *              使能 STOP 条件生成。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableStopCondition(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_STOP_ENABLE;
    }

    /**
     *  @brief      Disable I2CC data acknowledge (ACK or NACK)
     *              禁用数据应答（发送 NACK）。
     *
     *  When the I2CC module operates in receiver mode, the ACK bit can be
     *  cleared when no further data needs to be received from the target
     *  transmitter.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableACK(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_ACK_MASK);
    }

    /**
     *  @brief      Checks if I2CC data acknowledge (ACK or NACK) is
     *              enabled
     *              检查数据应答是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If I2CC data acknowledge is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if I2CC data acknowledge is enabled
     *  @retval     false if I2CC data acknowledge is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isACKEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_ACK_MASK) ==
                UNICOMMI2CC_CTR_ACK_ENABLE);
    }

    /**
     *  @brief      Enable I2CC data acknowledge (ACK or NACK)
     *              使能数据应答（发送 ACK）。
     *
     *  When the I2CC module operates in receiver mode, the ACK bit is
     *  normally set causing the I2CC bus to transmit an acknowledge
     *  automatically after each byte.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableACK(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_ACK_MASK;
    }

    /**
     *  @brief      Get transaction length in bytes
     *              获取事务长度（字节数）。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Length of I2CC transaction in bytes
     *              返回值：事务长度。
     *
     *  @retval     [0x000, 0xfff]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getTransactionLength(
        UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_BLEN_MASK) >>
                UNICOMMI2CC_CTR_BLEN_OFS);
    }

    /**
     *  @brief      Set transaction length in bytes
     *              设置事务长度（字节数）。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @param[in]  length      Length of I2CC transaction in bytes [0x000, 0xfff]
     *                          参数：事务长度。
     */
    __STATIC_INLINE void DL_I2CC_setTransactionLength(
        UNICOMM_Inst_Regs *unicomm, uint32_t length)
    {
        DL_Common_updateReg(&unicomm->i2cc->CTR,
                            (length << UNICOMMI2CC_CTR_BLEN_OFS), UNICOMMI2CC_CTR_BLEN_MASK);
    }

    /**
     *  @brief      Get status of I2CC bus for
     *              获取 I2CC 总线状态。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Status of I2CC bus for
     *              返回值：状态寄存器值（@ref DL_I2CC_STATUS 位掩码）。
     *
     *  @retval     Bitwise OR of @ref DL_I2CC_STATUS
     */
    __STATIC_INLINE uint32_t DL_I2CC_getStatus(UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->SR);
    }

    /**
     *  @brief      Get transaction count in bytes
     *              获取事务字节计数（倒计数值）。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Current 12-bit countdown value of the transaction
     *              返回值：当前 12 位倒计数值。
     *
     *  @retval     [0x000, 0xfff]
     */
    __STATIC_INLINE uint16_t DL_I2CC_getTransactionCount(
        UNICOMM_Inst_Regs *unicomm)
    {
        return ((uint16_t)((unicomm->i2cc->SR & UNICOMMI2CC_SR_BCNT_MASK) >>
                           UNICOMMI2CC_SR_BCNT_OFS));
    }

    /**
     *  @brief      Get byte of data from UNICOMMI2CC
     *              从 RX FIFO 读取 1 字节数据。
     *
     *  If using FIFO, it is first byte from the RX FIFO.
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Last received byte of data
     *              返回值：接收到的字节数据。
     *
     *  @retval     [0x00, 0xff]
     */
    __STATIC_INLINE uint8_t DL_I2CC_receiveData(UNICOMM_Inst_Regs *unicomm)
    {
        return ((uint8_t)(unicomm->i2cc->RXDATA & UNICOMMI2CC_RXDATA_DATA_MASK));
    }

    /**
     *  @brief      Set next byte to be transferred during the next transaction
     *              向 TX FIFO 写入 1 字节待发送数据。
     *
     *  Does not transmit data until @ref DL_I2CC_startTransfer
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  data    Data to be transferred during the next transaction.
     *                      [0x00, 0xff]
     *                      参数：待发送字节。
     */
    __STATIC_INLINE void DL_I2CC_transmitData(
        UNICOMM_Inst_Regs *unicomm, uint8_t data)
    {
        unicomm->i2cc->TXDATA = data;
    }

    /**
     *  @brief      Get timer period
     *              获取 SCL 定时器周期值。
     *  This field is used in the equation to configure SCL_PERIOD:
     *
     *  SCL_PERIOD = (1 + TPR) * (SCL_LP + SCL_HP) * INT_CLK_PRD
     *
     *  where:
     *  SCL_PRD is the SCL line period (I2CC clock)
     *
     *  TPR is the Timer Period register value (range of 1 to 127)
     *
     *  SCL_LP is the SCL Low period (fixed at 6)
     *  SCL_HP is the SCL High period (fixed at 4)
     *
     *  CLK_PRD is the functional clock period in ns
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @return     Timer period
     *              返回值：定时器周期值。
     *
     *  @retval     [0x00, 0x7f]
     */
    __STATIC_INLINE uint8_t DL_I2CC_getTimerPeriod(UNICOMM_Inst_Regs *unicomm)
    {
        return ((uint8_t)(unicomm->i2cc->TPR & UNICOMMI2CC_TPR_TPR_MASK));
    }

    /**
     *  @brief      Set timer period
     *              设置 SCL 定时器周期值。
     *  This field is used in the equation to configure SCL_PERIOD:
     *
     *  SCL_PERIOD = (1 + TPR) * (SCL_LP + SCL_HP) * INT_CLK_PRD
     *
     *  where:
     *  SCL_PRD is the SCL line period (I2CC clock)
     *
     *  TPR is the Timer Period register value (range of 1 to 127)
     *
     *  SCL_LP is the SCL Low period (fixed at 6)
     *  SCL_HP is the SCL High period (fixed at 4)
     *
     *  CLK_PRD is the functional clock period in ns
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  period  Timer period [0x00, 0x7f]
     *                      参数：定时器周期值。
     */
    __STATIC_INLINE void DL_I2CC_setTimerPeriod(
        UNICOMM_Inst_Regs *unicomm, uint8_t period)
    {
        unicomm->i2cc->TPR = period;
    }

    /**
     *  @brief      Disable multimode
     *              禁用多主机模式。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableMultiMode(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CR &= ~(UNICOMMI2CC_CR_MCTL_MASK);
    }

    /**
     *  @brief      Checks if multimode is enabled
     *              检查多主机模式是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If multimode is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if multimode is enabled
     *  @retval     false if multimode is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isMultiModeEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CR & UNICOMMI2CC_CR_MCTL_MASK) ==
                UNICOMMI2CC_CR_MCTL_ENABLE);
    }

    /**
     *  @brief      Enable multimode
     *              使能多主机模式（SCL 高电平由实际线路检测决定）。
     *
     *  In multimode, the SCL high time counts once the SCL line has been
     *  detected high. If this is not enabled, the high time counts as soon as the
     *  SCL line has been set high by the UNICOMMI2CC
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableMultiMode(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CR |= UNICOMMI2CC_CR_MCTL_ENABLE;
    }

    /**
     *  @brief      Disable controller
     *              禁用 I2CC 控制器。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disable(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CR &= ~(UNICOMMI2CC_CR_ENABLE_MASK);
    }

    /**
     *  @brief      Checks if controller is enabled
     *              检查 I2CC 控制器是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if is enabled
     *  @retval     false if is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CR & UNICOMMI2CC_CR_ENABLE_MASK) ==
                UNICOMMI2CC_CR_ENABLE_ENABLE);
    }

    /**
     *  @brief      Enable controller
     *              使能 I2CC 控制器。
     *
     *  After has enabled, it should not be re-enabled unless it has been
     *  disabled or by a reset, otherwise transfer failures may occur.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enable(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CR |= UNICOMMI2CC_CR_ENABLE_ENABLE;
    }

    /**
     *  @brief      Disable clock stretching
     *              禁用时钟拉伸。
     *
     *  Clock stretching can be disabled if no target on the bus supports clock
     *  stretching; however, it should be typically enabled to be compliant with
     *  I2CC specification.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableClockStretching(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CR &= ~(UNICOMMI2CC_CR_CLKSTRETCH_MASK);
    }

    /**
     *  @brief      Checks if clock stretching is enabled
     *              检查时钟拉伸是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If clock stretching is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if clock stretching is enabled
     *  @retval     false if clock stretching is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isClockStretchingEnabled(
        UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->CR & UNICOMMI2CC_CR_CLKSTRETCH_MASK) ==
                UNICOMMI2CC_CR_CLKSTRETCH_ENABLE);
    }

    /**
     *  @brief      Enable clock stretching
     *              使能时钟拉伸（符合 I2C 规范）。
     *
     *  Clock stretching can be disabled if no target on the bus supports clock
     *  stretching; however, it should be typically enabled to be compliant with UNICOMMI2CC
     *  specification.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableClockStretching(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->CR |= UNICOMMI2CC_CR_CLKSTRETCH_ENABLE;
    }

    /**
     *  @brief      Get SCL signal status
     *              获取 SCL 信号电平状态。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Status of SCL signal
     *              返回值：SCL 信号电平。
     *
     *  @retval     One of @ref DL_I2CC_SCL
     */
    __STATIC_INLINE DL_I2CC_SCL DL_I2CC_getSCLStatus(UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t sclStatus = unicomm->i2cc->BMON & UNICOMMI2CC_BMON_SCL_MASK;

        return (DL_I2CC_SCL)(sclStatus);
    }

    /**
     *  @brief      Get SDA signal status
     *              获取 SDA 信号电平状态。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Status of SDA signal
     *              返回值：SDA 信号电平。
     *
     *  @retval     One of @ref DL_I2CC_SDA
     */
    __STATIC_INLINE DL_I2CC_SDA DL_I2CC_getSDAStatus(UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t sdaStatus = unicomm->i2cc->BMON & UNICOMMI2CC_BMON_SDA_MASK;

        return (DL_I2CC_SDA)(sdaStatus);
    }

    /**
     *  @brief      Get TX FIFO threshold level
     *              获取 TX FIFO 中断触发阈值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @return     Indicates at what fill level in the TX FIFO a threshold will be
     *              generated
     *              返回值：TX FIFO 阈值枚举值。
     *
     *  @retval     One of @ref DL_I2CC_TX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2CC_TX_FIFO_LEVEL DL_I2CC_getTXFIFOThreshold(
        UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t level = unicomm->i2cc->IFLS & UNICOMMI2CC_IFLS_TXIFLSEL_MASK;

        return (DL_I2CC_TX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set TX FIFO threshold level
     *              设置 TX FIFO 中断触发阈值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  level   Indicates at what fill level in the TX FIFO a threshold
     *                      will be generated.
     *                      One of @ref DL_I2CC_TX_FIFO_LEVEL.
     *                      参数：TX FIFO 阈值枚举值。
     */
    __STATIC_INLINE void DL_I2CC_setTXFIFOThreshold(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_TX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(&unicomm->i2cc->IFLS, (uint32_t)level,
                            UNICOMMI2CC_IFLS_TXIFLSEL_MASK);
    }

    /**
     *  @brief      Get RX FIFO threshold level
     *              获取 RX FIFO 中断触发阈值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @return     Indicates at what fill level in the RX FIFO a threshold will be
     *              generated
     *              返回值：RX FIFO 阈值枚举值。
     *
     *  @retval     One of @ref DL_I2CC_RX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2CC_RX_FIFO_LEVEL DL_I2CC_getRXFIFOThreshold(
        UNICOMM_Inst_Regs *unicomm)
    {
        uint32_t level = unicomm->i2cc->IFLS & UNICOMMI2CC_IFLS_RXIFLSEL_MASK;

        return (DL_I2CC_RX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set RX FIFO threshold level
     *              设置 RX FIFO 中断触发阈值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *  @param[in]  level   Indicates at what fill level in the RX FIFO a threshold
     *                      will be generated.
     *                      One of @ref DL_I2CC_RX_FIFO_LEVEL.
     *                      参数：RX FIFO 阈值枚举值。
     */
    __STATIC_INLINE void DL_I2CC_setRXFIFOThreshold(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_RX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(&unicomm->i2cc->IFLS, (uint32_t)level,
                            UNICOMMI2CC_IFLS_RXIFLSEL_MASK);
    }

    /**
     *  @brief      Get number of bytes which can be read from RX FIFO
     *              获取 RX FIFO 可读字节数。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Number of bytes which can be read from RX FIFO
     *              返回值：可读字节数。
     *
     *  @retval     [0x0, 0x8]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getRXFIFOCounter(UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->SR & UNICOMMI2CC_SR_BCNT_MASK);
    }

    /**
     *  @brief      Get number of bytes which can be put into TX FIFO
     *              获取 TX FIFO 可写字节数。
     *
     *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
     *                              参数：UNICOMM 外设寄存器实例。
     *
     *  @return     Number of bytes which can be put into TX FIFO
     *              返回值：可写字节数。
     *
     *  @retval     [0x0, 0x8]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getTXFIFOCounter(UNICOMM_Inst_Regs *unicomm)
    {
        return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_BCNT_MASK) >>
                UNICOMMI2CC_SR_BCNT_OFS);
    }

    /**
     *  @brief      Enable I2CC interrupts
     *              使能 I2CC 中断。
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_I2CC_INTERRUPT.
     *                             参数：中断位掩码。
     */
    __STATIC_INLINE void DL_I2CC_enableInterrupt(
        UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
    {
        unicomm->i2cc->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable I2CC interrupts
     *              禁用 I2CC 中断。
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
     *                             @ref DL_I2CC_INTERRUPT.
     *                             参数：中断位掩码。
     */
    __STATIC_INLINE void DL_I2CC_disableInterrupt(
        UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
    {
        unicomm->i2cc->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which I2CC interrupts are enabled
     *              查询已使能的 I2CC 中断。
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2CC_INTERRUPT.
     *                             参数：中断位掩码。
     *
     *  @return     Which of the requested I2CC interrupts are enabled
     *              返回值：已使能的中断位。
     *
     *  @retval     Bitwise OR of @ref DL_I2CC_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2CC_getEnabledInterrupts(
        UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
    {
        return (unicomm->i2cc->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled I2CC interrupts
     *              查询已使能中断的挂起状态（屏蔽后）。
     *
     *  Checks if any of the I2CC interrupts that were previously enabled are
     *  pending.
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2CC_INTERRUPT.
     *                             参数：中断位掩码。
     *
     *  @return     Which of the requested I2CC interrupts are pending
     *              返回值：挂起的中断位。
     *
     *  @retval     Bitwise OR of @ref DL_I2CC_INTERRUPT values
     *
     *  @sa         DL_I2CC_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_I2CC_getEnabledInterruptStatus(
        UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
    {
        return (unicomm->i2cc->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any I2CC interrupt
     *              查询原始中断挂起状态（不受屏蔽影响）。
     *
     *  Checks if any of the I2CC interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2CC_INTERRUPT.
     *                             参数：中断位掩码。
     *
     *  @return     Which of the requested I2CC interrupts are pending
     *              返回值：挂起的中断位。
     *
     *  @retval     Bitwise OR of @ref DL_I2CC_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2CC_getRawInterruptStatus(
        UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
    {
        return (unicomm->i2cc->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending I2CC interrupt
     *              获取最高优先级的挂起中断索引。
     *
     *  Checks if any of the I2CC interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
     *                       参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The highest priority pending I2CC interrupt
     *              返回值：中断索引枚举值。
     *
     *  @retval     One of @ref DL_I2CC_IIDX
     */
    __STATIC_INLINE DL_I2CC_IIDX DL_I2CC_getPendingInterrupt(
        UNICOMM_Inst_Regs *unicomm)
    {
        return ((DL_I2CC_IIDX)unicomm->i2cc->CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending I2CC interrupts
     *              清除指定的中断标志。
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_I2CC_INTERRUPT.
     *                             参数：待清除的中断位掩码。
     */
    __STATIC_INLINE void DL_I2CC_clearInterruptStatus(
        UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
    {
        unicomm->i2cc->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief Enables I2CC interrupt for triggering DMA events
     *         使能 I2CC DMA 事件触发中断。
     *
     * Enables the I2CC interrupt to be used as the condition to generate an
     * event to directly trigger the DMA.
     *
     * Each event publisher @ref DL_I2CC_EVENT_ROUTE can set any one of
     * @ref DL_I2CC_DMA_INTERRUPT.
     *
     * @note Only one interrupt source should be enabled at a time.
     *
     *  @param[in]  unicomm        Pointer to the register overlay for the peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  index      Specifies the register event publisher to configure
     *                         参数：事件路由选择。
     *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
     *                         the DMA. One of @ref DL_I2CC_DMA_INTERRUPT.
     *                         参数：DMA 触发中断源。
     */
    __STATIC_INLINE void DL_I2CC_enableDMAEvent(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index, uint32_t interrupt)
    {
        switch (index)
        {
        case DL_I2CC_EVENT_ROUTE_1:
            unicomm->i2cc->DMA_TRIG1.IMASK = interrupt;
            break;
        case DL_I2CC_EVENT_ROUTE_2:
            unicomm->i2cc->DMA_TRIG0.IMASK = interrupt;
            break;
        default:
            break;
        }
    }

    /**
     *  @brief Disables I2CC interrupt from triggering DMA events
     *         禁用 I2CC DMA 事件触发中断。
     *
     * Disables the I2CC interrupt from being used as the condition to generate an
     * event to directly trigger the DMA.
     *
     * Each event publisher @ref DL_I2CC_EVENT_ROUTE can set any one of
     * @ref DL_I2CC_DMA_INTERRUPT.
     *
     *  @param[in]  unicomm        Pointer to the register overlay for the peripheral
     *                             参数：UNICOMM 外设寄存器实例。
     *  @param[in]  index      Specifies the register event publisher to configure
     *                         参数：事件路由选择。
     *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
     *                         the DMA. One of @ref DL_I2CC_DMA_INTERRUPT.
     *                         参数：DMA 触发中断源。
     */
    __STATIC_INLINE void DL_I2CC_disableDMAEvent(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index, uint32_t interrupt)
    {
        switch (index)
        {
        case DL_I2CC_EVENT_ROUTE_1:
            unicomm->i2cc->DMA_TRIG1.IMASK &= ~(interrupt);
            break;
        case DL_I2CC_EVENT_ROUTE_2:
            unicomm->i2cc->DMA_TRIG0.IMASK &= ~(interrupt);
            break;
        default:
            break;
        }
    }

    /**
     *  @brief Check which I2CC interrupt for DMA receive events is enabled
     *         查询已使能的 DMA 事件中断。
     *
     *  This API checks the event publisher register as selected by
     *  @ref DL_I2CC_EVENT_ROUTE, which are used for triggering the DMA for
     *  or Target and receive or transmit events.
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
     *                                 参数：UNICOMM 外设寄存器实例。
     *  @param[in]  index          Specifies the register event publisher to configure
     *                             参数：事件路由选择。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2CC_DMA_INTERRUPT.
     *                             参数：中断位掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested I2CC interrupt status
     *              返回值：已使能的 DMA 中断位。
     *
     *  @retval     One of @ref DL_I2CC_DMA_INTERRUPT
     */
    __STATIC_INLINE uint32_t DL_I2CC_getEnabledDMAEvents(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index,
        uint32_t interruptMask)
    {
        volatile uint32_t *pReg = &unicomm->i2cc->DMA_TRIG1.IMASK;

        return ((*(pReg + (uint32_t)index) & interruptMask));
    }

    /**
     *  @brief Check interrupt flag of enabled I2CC interrupt for DMA event
     *         查询已使能 DMA 事件中断的挂起状态。
     *
     *  Checks if any of the I2CC interrupts for the DMA receive event that were
     *  previously enabled are pending.
     *  This API checks the event publisher register as selected by
     *  @ref DL_I2CC_EVENT_ROUTE, which are used for triggering the DMA for
     *  or Target and receive or transmit events.
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
     *                                 参数：UNICOMM 外设寄存器实例。
     *  @param[in]  index          Specifies the register event publisher to configure
     *                             参数：事件路由选择。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2CC_DMA_INTERRUPT.
     *                             参数：中断位掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested I2CC interrupt status
     *              返回值：挂起的 DMA 中断位。
     *
     *  @retval     One of @ref DL_I2CC_DMA_INTERRUPT
     *
     *  @sa         DL_I2CC_enableDMAEvent
     */
    __STATIC_INLINE uint32_t DL_I2CC_getEnabledDMAEventStatus(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index,
        uint32_t interruptMask)
    {
        const volatile uint32_t *pReg = &unicomm->i2cc->DMA_TRIG1.MIS;

        return ((*(pReg + (uint32_t)index) & interruptMask));
    }

    /**
     *  @brief Check interrupt flag of any I2CC interrupt for DMA event
     *         查询 DMA 事件原始中断挂起状态（不受屏蔽影响）。
     *
     *  Checks if any of the I2CC interrupts for DMA receive event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the event publisher register as selected by
     *  @ref DL_I2CC_EVENT_ROUTE, which are used for triggering the DMA for
     *  or Target and receive or transmit events.
     *
     *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
     *                                 参数：UNICOMM 外设寄存器实例。
     *  @param[in]  index          Specifies the register event publisher to configure
     *                             参数：事件路由选择。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_I2CC_DMA_INTERRUPT.
     *                             参数：中断位掩码。
     *
     *  @return     Which of the requested I2CC interrupts are pending
     *              返回值：挂起的 DMA 中断位。
     *
     *  @retval     Bitwise OR of @ref DL_I2CC_DMA_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2CC_getRawDMAEventStatus(
        UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index,
        uint32_t interruptMask)
    {
        const volatile uint32_t *pReg = &unicomm->i2cc->DMA_TRIG1.RIS;

        return ((*(pReg + (uint32_t)index) & interruptMask));
    }

    /**
     *  @brief      Get the Timeout Counter A value
     *              获取超时计数器 A 预设值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The Timeout A counter value
     *              返回值：超时 A 预设值。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getTimeoutACount(UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTLA_MASK);
    }

    /**
     *  @brief      Set the Timeout Counter A value
     *              设置超时计数器 A 预设值（用于 SCL 低电平检测）。
     *
     *  Timeout A is used for SCL low detection. This field contains the upper 8
     *  bits of a 12-bit pre-load value for the Timeout A count.
     *  NOTE: The value of the counter must be greater than 1 to enable the
     *  timeout. Each count is equal to 520 times the timeout period of the
     *  functional clock. For example, with 8MHz functional clock and a
     *  100KHz operating I2CC clock, one timeout period will be equal to
     *  (1 / 8MHz) * 520 = 65us.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the
     *                      peripheral
     *                      参数：UNICOMM 外设寄存器实例。
     *  @param[in]  count   The value to set the Timeout A counter to.
     *                      Value between [0x0, 0xFF]
     *                      参数：超时 A 预设值。
     */
    __STATIC_INLINE void DL_I2CC_setTimeoutACount(
        UNICOMM_Inst_Regs *unicomm, uint32_t count)
    {
        DL_Common_updateReg(&unicomm->i2cc->TIMEOUT_CTL, count,
                            UNICOMMI2CC_TIMEOUT_CTL_TCNTLA_MASK);
    }

    /**
     *  @brief      Disable Timeout Counter A
     *              禁用超时计数器 A。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableTimeoutA(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->TIMEOUT_CTL &= ~(UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_MASK);
    }

    /**
     *  @brief      Checks if Timeout Counter A is enabled
     *              检查超时计数器 A 是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If Timeout Counter A is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if Timeout Counter A is enabled
     *  @retval     false if Timeout Counter A is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isTimeoutAEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return (
            (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_MASK) ==
            UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_ENABLE);
    }

    /**
     *  @brief      Enable Timeout Counter A
     *              使能超时计数器 A。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableTimeoutA(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->TIMEOUT_CTL |= UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_ENABLE;
    }

    /**
     *  @brief      Get the current Timer Counter A value
     *              获取超时计数器 A 当前值。
     *
     *  This field contains the upper 8 bits of a 12-bit current counter for
     *  Timeout Counter A.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The Timeout A counter value
     *              返回值：超时 A 当前计数值。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getCurrentTimeoutACounter(
        UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->TIMEOUT_CNT & UNICOMMI2CC_TIMEOUT_CNT_TCNTA_MASK);
    }

    /**
     *  @brief      Get the Timeout Counter B value
     *              获取超时计数器 B 预设值。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The Timeout B counter value
     *              返回值：超时 B 预设值。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getTimeoutBCount(UNICOMM_Inst_Regs *unicomm)
    {
        return (
            (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_MASK) >>
            UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_OFS);
    }

    /**
     *  @brief      Set the Timeout Counter B value
     *              设置超时计数器 B 预设值（用于 SCL 高电平检测）。
     *
     *  Timeout B is used for SCL high detection. This field contains the upper 8
     *  bits of a 12-bit pre-load value for the Timeout A count.
     *  NOTE: The value of the counter must be greater than 1 to enable the
     *  timeout. Each count is equal to 1* clock period. For example, with 10MHz
     *  functional clock one timeout period will be equal to 1 * 10ns.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the
     *                      peripheral
     *                      参数：UNICOMM 外设寄存器实例。
     *  @param[in]  count   The value to set the Timeout A counter to.
     *                      Value between [0x0, 0xFF]
     *                      参数：超时 B 预设值。
     */
    __STATIC_INLINE void DL_I2CC_setTimeoutBCount(
        UNICOMM_Inst_Regs *unicomm, uint32_t count)
    {
        DL_Common_updateReg(&unicomm->i2cc->TIMEOUT_CTL,
                            (count << UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_OFS),
                            UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_MASK);
    }

    /**
     *  @brief      Disable Timeout Counter B
     *              禁用超时计数器 B。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_disableTimeoutB(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->TIMEOUT_CTL &= ~(UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_MASK);
    }

    /**
     *  @brief      Checks if Timeout Counter B is enabled
     *              检查超时计数器 B 是否已使能。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     If Timeout Counter B is enabled
     *              返回值：是否已使能。
     *
     *  @retval     true if Timeout Counter B is enabled
     *  @retval     false if Timeout Counter B is disabled
     */
    __STATIC_INLINE bool DL_I2CC_isTimeoutBEnabled(UNICOMM_Inst_Regs *unicomm)
    {
        return (
            (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_MASK) ==
            UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_ENABLE);
    }

    /**
     *  @brief      Enable Timeout Counter B
     *              使能超时计数器 B。
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     */
    __STATIC_INLINE void DL_I2CC_enableTimeoutB(UNICOMM_Inst_Regs *unicomm)
    {
        unicomm->i2cc->TIMEOUT_CTL |= UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_ENABLE;
    }

    /**
     *  @brief      Get the current Timer Counter B value
     *              获取超时计数器 B 当前值。
     *
     *  This field contains the upper 8 bits of a 12-bit current counter for
     *  Timeout Counter B.
     *
     *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
     *                          参数：UNICOMM 外设寄存器实例。
     *
     *  @return     The Timeout B counter value
     *              返回值：超时 B 当前计数值。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2CC_getCurrentTimeoutBCounter(
        UNICOMM_Inst_Regs *unicomm)
    {
        return (unicomm->i2cc->TIMEOUT_CNT & UNICOMMI2CC_TIMEOUT_CNT_TCNTB_MASK);
    }

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_UNICOMMI2CC__ */

#endif /* ti_dl_dl_unicomm__include */
/** @}*/
