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
/**
 * @brief 文件用途：MSPM0G3507 I2C 驱动库头文件。
 *        提供 I2C 控制器/目标模式的完整寄存器操作接口，包括：
 *        时钟配置、FIFO 读写、传输控制、地址管理、中断/DMA 事件、
 *        毛刺滤波、超时检测及 SMBus/PMBus PEC 校验等功能。
 */
/*!****************************************************************************
 *  @file       dl_i2c.h
 *  @brief      Inter-Integrated Circuit (I2C) Driver Library
 *  @defgroup   I2C  Inter-Integrated Circuit (I2C)
 *
 *  @anchor ti_dl_dl_i2c_Overview
 *  # Overview
 *
 *   The I2C Library allows full configuration of the MSPM0 I2C module.
 *   The I2C module provides a standardized serial interface to transfer data
 *   between MSP devices and other external I2C devices (such as a sensors,
 *   memory, or DACs).
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup I2C
 * @{
 */
#ifndef ti_dl_dl_i2c__include
#define ti_dl_dl_i2c__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_I2C__

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

/*!
 * @brief I2C number of bytes which could be put into the TX FIFO
 *        I2C TX FIFO 最大容量（字节数），由设备头文件中的 I2C_SYS_FENTRIES 计算得出。
 *
 * This variable is device specific and is calculated using the system parameter
 * I2C_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2C_TX_FIFO_COUNT_MAXIMUM          ((uint32_t)I2C_SYS_FENTRIES << 8)

/*!
 * @brief I2C number of bytes which could be put into the RX FIFO
 *        I2C RX FIFO 最大容量（字节数），由设备头文件中的 I2C_SYS_FENTRIES 计算得出。
 *
 * This variable is device specific and is calculated using the system parameter
 * I2C_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2C_RX_FIFO_COUNT_MAXIMUM               ((uint32_t)I2C_SYS_FENTRIES)

/** @addtogroup DL_I2C_CONTROLLER_STATUS
 *  @{
 */
/*!
 * @brief I2C is busy
 *        I2C 控制器忙标志，事务进行中置位（含 START/RESTART/地址/STOP 信号生成期间）。
 *
 * The BUSY bit is set during an ongoing transaction, so is set during the
 * transmit/receive of the amount of data set in WBLEN including START, RESTART,
 * Address and STOP signal generation when required for the current transaction
 */
#define DL_I2C_CONTROLLER_STATUS_BUSY                       (I2C_MSR_BUSY_MASK)

/*!
 * @brief I2C error detected
 *        I2C 控制器错误标志，目标地址或发送数据未应答时置位。
 *
 * The error can be from the target address not being acknowledged or the
 * transmit data not being acknowledged
 */
#define DL_I2C_CONTROLLER_STATUS_ERROR                       (I2C_MSR_ERR_MASK)

/*!
 * @brief I2C address acknowledged
 *        I2C 控制器地址应答标志，目标地址被应答时置位。
 */
#define DL_I2C_CONTROLLER_STATUS_ADDR_ACK                 (I2C_MSR_ADRACK_MASK)

/*!
 * @brief I2C data acknowledged
 *        I2C 控制器数据应答标志，发送的数据被应答时置位。
 */
#define DL_I2C_CONTROLLER_STATUS_DATA_ACK                 (I2C_MSR_DATACK_MASK)

/*!
 * @brief I2C arbitration lost
 *        I2C 控制器仲裁丢失标志，多控制器竞争时丢失总线控制权时置位。
 */
#define DL_I2C_CONTROLLER_STATUS_ARBITRATION_LOST         (I2C_MSR_ARBLST_MASK)

/*!
 * @brief I2C idle
 *        I2C 控制器空闲标志，无活跃事务时置位。
 */
#define DL_I2C_CONTROLLER_STATUS_IDLE                       (I2C_MSR_IDLE_MASK)

/*!
 * @brief I2C bus busy
 *        I2C 总线忙标志，根据 START 和 STOP 条件自动变化。
 *
 * The bit changes based on the START and STOP conditions
 */
#define DL_I2C_CONTROLLER_STATUS_BUSY_BUS                 (I2C_MSR_BUSBSY_MASK)

/** @}*/


/** @addtogroup DL_I2C_TARGET_STATUS
 *  @{
 */

/*!
 * @brief Indicates the address for which target address match happened
 *        目标地址匹配标志，指示匹配到的目标地址。
 */
#define DL_I2C_TARGET_STATUS_ADDRESS_MATCH             (I2C_SSR_ADDRMATCH_MASK)

/*!
 * @brief I2C Target Stale TX FIFO
 *        目标 TX FIFO 数据过期标志，上一次事务未清空 FIFO 时置位。
 *
 * Set = The target TX FIFO data is stale. This occurs when the TX FIFO was not
 *   emptied during the previous I2C transaction
 * Clear = The target TX FIFO data is not stale
 */
#define DL_I2C_TARGET_STATUS_STALE_TX_FIFO          (I2C_SSR_STALE_TXFIFO_MASK)

/*!
 * @brief I2C Target TX Mode
 *        目标发送模式标志，目标处于 TX_DATA/TX_ACK/TX_WAIT/ADDR_ACK 状态且总线方向为读时置位。
 *
 * Set = Target is in the TX_DATA, TX_ACK, TX_WAIT, or ADDR_ACK state with the
 *   bus direction set to read
 * Clear = Target is not in the TX_DATA, TX_ACK, TX_WAIT, or ADDR_ACK state
 *   with the bus direction set to read
 */
#define DL_I2C_TARGET_STATUS_TX_MODE                      (I2C_SSR_TXMODE_MASK)

/*!
 * @brief I2C Target Bus Busy
 *        目标总线忙标志，总线活跃时置位，检测到 STOP 或超时时清除。
 *
 * Set = The I2C bus is busy. This is cleared on a timeout
 * Clear = The I2C bus is not busy. This bit is cleared when a STOP condition is
 *   detected on the bus.
 */
#define DL_I2C_TARGET_STATUS_BUS_BUSY                     (I2C_SSR_BUSBSY_MASK)

/*!
 * @brief I2C Target RX Mode
 *        目标接收模式标志，目标处于 RX_DATA/RX_ACK/RX_WAIT/RX_ACK_WAIT/ADDR_ACK 状态且总线方向为写时置位。
 *
 * Set = Target is in the RX_DATA, RX_ACK, RX_WAIT, RX_ACK_WAIT or ADDR_ACK
 *   state with the bus direction set to write
 * Clear = Target is not in the RX_DATA, RX_ACK, RX_WAIT, RX_ACK_WAIT or ADDR_ACK
 *   state with the bus direction set to write
 */
#define DL_I2C_TARGET_STATUS_RX_MODE                      (I2C_SSR_RXMODE_MASK)


/*!
 * @brief Direction of I2C Quick Command
 *        I2C 快速命令方向标志，仅在 QCMDST 置位时有效。置位表示读，清除表示写。
 * Only valid when QCMDST is set. @sa DL_I2C_TARGET_STATUS_QUICK_COMMAND_STATUS.
 * Set = Quick command was a read
 * Clear = Quick command was a write
 */
#define DL_I2C_TARGET_STATUS_QUICK_COMMAND_READ_WRITE     (I2C_SSR_QCMDRW_MASK)

/*!
 * @brief Status of I2C Quick Command
 *        I2C 快速命令状态标志，上次事务为快速命令时置位。
 * @sa DL_I2C_TARGET_STATUS_QUICK_COMMAND_READ_WRITE.
 * Set = Last transaction was a Quick Comand transaction
 * Clear = Last transaction was normal or has not occurred
 */
#define DL_I2C_TARGET_STATUS_QUICK_COMMAND_STATUS         (I2C_SSR_QCMDST_MASK)

/*!
 * @brief I2C Target Own Address Alternate Matched
 *        目标备用地址匹配标志，匹配到备用地址（OAR2）时置位。
 */
#define DL_I2C_TARGET_STATUS_OWN_ADDR_ALTERNATE_MATCHED  (I2C_SSR_OAR2SEL_MASK)

/*!
 * @brief I2C Target Transmit Request
 *        目标发送请求标志，控制器发起读操作时置位，目标应准备发送数据。
 */
#define DL_I2C_TARGET_STATUS_TRANSMIT_REQUEST               (I2C_SSR_TREQ_MASK)

/*!
 * @brief I2C Target Receive Request
 *        目标接收请求标志，控制器发起写操作时置位，目标可读取接收数据。
 */
#define DL_I2C_TARGET_STATUS_RECEIVE_REQUEST                (I2C_SSR_RREQ_MASK)

/** @}*/

/** @addtogroup DL_I2C_INTERRUPT
 *  @{
 */
/*!
 * @brief Controller Receive Transaction completed Interrupt
 *        控制器接收事务完成中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_RX_DONE  (I2C_CPU_INT_IMASK_MRXDONE_SET)

/*!
 * @brief Controller Transmit Transaction completed Interrupt
 *        控制器发送事务完成中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_TX_DONE  (I2C_CPU_INT_IMASK_MTXDONE_SET)

/*!
 * @brief Controller Receive FIFO Trigger when >= defined bytes
 *        控制器 RX FIFO 触发中断，数据量达到阈值时触发。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER                            \
                                          (I2C_CPU_INT_IMASK_MRXFIFOTRG_SET)

/*!
 * @brief Controller Transmit FIFO Trigger when <= defined bytes
 *        控制器 TX FIFO 触发中断，数据量降至阈值时触发。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER                            \
                                          (I2C_CPU_INT_IMASK_MTXFIFOTRG_SET)

/*!
 * @brief Controller Receive FIFO is full
 *        控制器 RX FIFO 满中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_FULL                               \
                                         (I2C_CPU_INT_IMASK_MRXFIFOFULL_SET)

/*!
 * @brief Controller Transmit FIFO is empty
 *        控制器 TX FIFO 空中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_EMPTY                              \
                                            (I2C_CPU_INT_IMASK_MTXEMPTY_SET)

/*!
 * @brief Address/Data NACK Interrupt
 *        地址/数据无应答（NACK）中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_NACK       (I2C_CPU_INT_IMASK_MNACK_SET)

/*!
 * @brief START Detection Interrupt
 *        控制器 START 条件检测中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_START     (I2C_CPU_INT_IMASK_MSTART_SET)

/*!
 * @brief STOP Detection Interrupt
 *        控制器 STOP 条件检测中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_STOP       (I2C_CPU_INT_IMASK_MSTOP_SET)

/*!
 * @brief Arbitration Lost Interrupt
 *        控制器仲裁丢失中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_ARBITRATION_LOST                          \
                                            (I2C_CPU_INT_IMASK_MARBLOST_SET)

/*!
 * @brief Controller DMA Done on Event 1 publisher
 *        控制器 DMA 事件 1 完成中断（TX DMA 完成）。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_EVENT1_DMA_DONE                           \
                                       (I2C_CPU_INT_IMASK_MDMA_DONE_TX_SET)

/*!
 * @brief Controller DMA Done on Event 2 publisher
 *        控制器 DMA 事件 2 完成中断（RX DMA 完成）。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_EVENT2_DMA_DONE                           \
                                       (I2C_CPU_INT_IMASK_MDMA_DONE_RX_SET)


/*!
 * @brief Controller SMBus/PMBus PEC Receive Error Interrupt
 *        控制器 SMBus/PMBus PEC 接收错误中断。
 */
#define DL_I2C_INTERRUPT_CONTROLLER_PEC_RX_ERROR                              \
                                         (I2C_CPU_INT_IMASK_MPEC_RX_ERR_SET)


/*!
 * @brief Target Receive Data Interrupt (byte has been received)
 *        目标接收数据完成中断（收到 1 字节）。
 */
#define DL_I2C_INTERRUPT_TARGET_RX_DONE      (I2C_CPU_INT_IMASK_SRXDONE_SET)

/*!
 * @brief Target Transmit Transaction completed Interrupt
 *        目标发送事务完成中断。
 */
#define DL_I2C_INTERRUPT_TARGET_TX_DONE      (I2C_CPU_INT_IMASK_STXDONE_SET)

/*!
 * @brief Target Receive FIFO Trigger
 *        目标 RX FIFO 触发中断，数据量达到阈值时触发。
 */
#define DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER                                \
                                          (I2C_CPU_INT_IMASK_SRXFIFOTRG_SET)

/*!
 * @brief Target Transmit FIFO Trigger
 *        目标 TX FIFO 触发中断，数据量降至阈值时触发。
 */
#define DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER                                \
                                          (I2C_CPU_INT_IMASK_STXFIFOTRG_SET)

/*!
 * @brief Target RX FIFO full
 *        目标 RX FIFO 满中断。
 */
#define DL_I2C_INTERRUPT_TARGET_RXFIFO_FULL                                   \
                                         (I2C_CPU_INT_IMASK_SRXFIFOFULL_SET)

/*!
 * @brief Target TX FIFO empty. All data in Transmit FIFO shifted out and
 * transmit goes into idle mode.
 *        目标 TX FIFO 空中断，所有数据移出后发送进入空闲。
 */
#define DL_I2C_INTERRUPT_TARGET_TXFIFO_EMPTY                                  \
                                            (I2C_CPU_INT_IMASK_STXEMPTY_SET)

/*!
 * @brief Target Start Condition detected
 *        目标 START 条件检测中断。
 */
#define DL_I2C_INTERRUPT_TARGET_START                                         \
                                              (I2C_CPU_INT_IMASK_SSTART_SET)

/*!
 * @brief Target Stop Condition detected
 *        目标 STOP 条件检测中断。
 */
#define DL_I2C_INTERRUPT_TARGET_STOP           (I2C_CPU_INT_IMASK_SSTOP_SET)

/*!
 * @brief General Call Interrupt
 *        目标通用呼叫（General Call）中断。
 */
#define DL_I2C_INTERRUPT_TARGET_GENERAL_CALL                                  \
                                            (I2C_CPU_INT_IMASK_SGENCALL_SET)

/*!
 * @brief Target DMA Done on Event 1 Publisher
 *        目标 DMA 事件 1 完成中断（TX DMA 完成）。
 */
#define DL_I2C_INTERRUPT_TARGET_EVENT1_DMA_DONE                               \
                                        (I2C_CPU_INT_IMASK_SDMA_DONE_TX_SET)

/*!
 * @brief Target DMA Done on Event 2 Publisher
 *        目标 DMA 事件 2 完成中断（RX DMA 完成）。
 */
#define DL_I2C_INTERRUPT_TARGET_EVENT2_DMA_DONE                               \
                                        (I2C_CPU_INT_IMASK_SDMA_DONE_RX_SET)


/*!
 * @brief Target SMBus/PMBus PEC Receive Error Interrupt
 *        目标 SMBus/PMBus PEC 接收错误中断。
 */
#define DL_I2C_INTERRUPT_TARGET_PEC_RX_ERROR                                  \
                                         (I2C_CPU_INT_IMASK_SPEC_RX_ERR_SET)

/*!
 * @brief Target TX FIFO Underflow Interrupt
 *        目标 TX FIFO 下溢中断。
 */
#define DL_I2C_INTERRUPT_TARGET_TXFIFO_UNDERFLOW                              \
                                            (I2C_CPU_INT_IMASK_STX_UNFL_SET)

/*!
 * @brief Target RX FIFO Overflow Interrupt
 *        目标 RX FIFO 溢出中断。
 */
#define DL_I2C_INTERRUPT_TARGET_RXFIFO_OVERFLOW                              \
                                            (I2C_CPU_INT_IMASK_SRX_OVFL_SET)

/*!
 * @brief Target Arbitration Lost Interrupt
 *        目标仲裁丢失中断。
 */
#define DL_I2C_INTERRUPT_TARGET_ARBITRATION_LOST                              \
                                            (I2C_CPU_INT_IMASK_SARBLOST_SET)

/*!
 * @brief Interrupt Overflow Interrupt. Occurs when Target START or STOP
 *        interrupts overflow (i.e. occurs twice without being serviced)
 *        中断溢出中断，目标 START 或 STOP 中断连续发生两次未被服务时触发。
 */
#define DL_I2C_TARGET_INTERRUPT_OVERFLOW   (I2C_CPU_INT_IMASK_INTR_OVFL_SET)

/*!
 * @brief Timeout A Interrupt
 *        超时 A 中断，SCL 低电平超时时触发。
 */
#define DL_I2C_INTERRUPT_TIMEOUT_A          (I2C_CPU_INT_IMASK_TIMEOUTA_SET)

/*!
 * @brief Timeout B Interrupt
 *        超时 B 中断，SCL 高电平超时时触发。
 */
#define DL_I2C_INTERRUPT_TIMEOUT_B          (I2C_CPU_INT_IMASK_TIMEOUTB_SET)


/** @}*/

/** @addtogroup DL_I2C_DMA_INTERRUPT
 *  @{
 */
/*!
 * @brief Peripheral Transmit FIFO Trigger interrupt for DMA trigger
 *        目标 TX FIFO 触发 DMA 中断。
 */
#define DL_I2C_DMA_INTERRUPT_TARGET_TXFIFO_TRIGGER                            \
                                          (I2C_DMA_TRIG1_IMASK_STXFIFOTRG_SET)

/*!
 * @brief Peripheral Receive FIFO Trigger interrupt for DMA trigger
 *        目标 RX FIFO 触发 DMA 中断。
 */
#define DL_I2C_DMA_INTERRUPT_TARGET_RXFIFO_TRIGGER                            \
                                          (I2C_DMA_TRIG1_IMASK_SRXFIFOTRG_SET)

/*!
 * @brief Controller Transmit FIFO Trigger when <= defined bytes for DMA trigger
 *        控制器 TX FIFO 触发 DMA 中断。
 */
#define DL_I2C_DMA_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER                        \
                                          (I2C_DMA_TRIG1_IMASK_MTXFIFOTRG_SET)

/*!
 * @brief Controller Receive FIFO Trigger when >= defined bytes for DMA trigger
 *        控制器 RX FIFO 触发 DMA 中断。
 */
#define DL_I2C_DMA_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER                        \
                                          (I2C_DMA_TRIG1_IMASK_MRXFIFOTRG_SET)

/** @}*/

/*!
 * @brief Backwards compatible redirect for @ref DL_I2C_getTargetOwnAddressAlternateMask
*/
#define I2C_getTargetOwnAddressAlternateMask DL_I2C_getTargetOwnAddressAlternateMask

    /* clang-format on */

    /*! @enum DL_I2C_DMA_IIDX I2C DMA 触发中断索引 */
    typedef enum
    {
        /*! I2C interrupt index for enabling I2C Target Transmit FIFO Trigger as DMA trigger
         *  目标 TX FIFO 触发 DMA 的中断索引 */
        DL_I2C_DMA_IIDX_TARGET_TXFIFO_TRIGGER = I2C_DMA_TRIG1_IIDX_STAT_MTXFIFOTRG,
        /*! I2C interrupt index for enabling I2C Target Receive FIFO Trigger as DMA trigger
         *  目标 RX FIFO 触发 DMA 的中断索引 */
        DL_I2C_DMA_IIDX_TARGET_RXFIFO_TRIGGER = I2C_DMA_TRIG1_IIDX_STAT_MRXFIFOTRG,
        /*! I2C interrupt index for enabling I2C Controller Transmit FIFO Trigger as DMA trigger
         *  控制器 TX FIFO 触发 DMA 的中断索引 */
        DL_I2C_DMA_IIDX_CONTROLLER_TXFIFO_TRIGGER =
            I2C_DMA_TRIG1_IIDX_STAT_STXFIFOTRG,
        /*! I2C interrupt index for enabling I2C Controller Receive FIFO Trigger as DMA trigger
         *  控制器 RX FIFO 触发 DMA 的中断索引 */
        DL_I2C_DMA_IIDX_CONTROLLER_RXFIFO_TRIGGER =
            I2C_DMA_TRIG1_IIDX_STAT_SRXFIFOTRG
    } DL_I2C_DMA_IIDX;

    /** @enum DL_I2C_EVENT_ROUTE I2C 事件路由选择 */
    typedef enum
    {
        /*! I2C event route 1 事件路由 1 */
        DL_I2C_EVENT_ROUTE_1 = 0,
        /*! I2C event route 2 事件路由 2 */
        DL_I2C_EVENT_ROUTE_2 = 12
    } DL_I2C_EVENT_ROUTE;

    /** @enum DL_I2C_CLOCK I2C 时钟源选择 */
    typedef enum
    {
        /*!  BUSCLK enabled as I2C clock source 总线时钟作为 I2C 时钟源 */
        DL_I2C_CLOCK_BUSCLK = I2C_CLKSEL_BUSCLK_SEL_ENABLE,
        /*!  MFCLK enabled as I2C clock source 主功能时钟作为 I2C 时钟源 */
        DL_I2C_CLOCK_MFCLK = I2C_CLKSEL_MFCLK_SEL_ENABLE,
    } DL_I2C_CLOCK;

    /** @enum DL_I2C_CLOCK_DIVIDE I2C 时钟分频比 */
    typedef enum
    {
        /*!  I2C Clock Divided by 1 不分频 */
        DL_I2C_CLOCK_DIVIDE_1 = I2C_CLKDIV_RATIO_DIV_BY_1,
        /*!  I2C Clock Divided by 2 2 分频 */
        DL_I2C_CLOCK_DIVIDE_2 = I2C_CLKDIV_RATIO_DIV_BY_2,
        /*!  I2C Clock Divided by 3 3 分频 */
        DL_I2C_CLOCK_DIVIDE_3 = I2C_CLKDIV_RATIO_DIV_BY_3,
        /*!  I2C Clock Divided by 4 4 分频 */
        DL_I2C_CLOCK_DIVIDE_4 = I2C_CLKDIV_RATIO_DIV_BY_4,
        /*!  I2C Clock Divided by 5 5 分频 */
        DL_I2C_CLOCK_DIVIDE_5 = I2C_CLKDIV_RATIO_DIV_BY_5,
        /*!  I2C Clock Divided by 6 6 分频 */
        DL_I2C_CLOCK_DIVIDE_6 = I2C_CLKDIV_RATIO_DIV_BY_6,
        /*!  I2C Clock Divided by 7 7 分频 */
        DL_I2C_CLOCK_DIVIDE_7 = I2C_CLKDIV_RATIO_DIV_BY_7,
        /*!  I2C Clock Divided by 8 8 分频 */
        DL_I2C_CLOCK_DIVIDE_8 = I2C_CLKDIV_RATIO_DIV_BY_8,
    } DL_I2C_CLOCK_DIVIDE;

    /** @enum DL_I2C_TARGET_ADDRESSING_MODE 目标寻址模式 */
    typedef enum
    {
        /*! Enable Target in 7-bit addressing mode 7 位寻址模式 */
        DL_I2C_TARGET_ADDRESSING_MODE_7_BIT = I2C_SOAR_SMODE_MODE7,
        /*! Enable Target in 10-bit addressing mode 10 位寻址模式 */
        DL_I2C_TARGET_ADDRESSING_MODE_10_BIT = I2C_SOAR_SMODE_MODE10,
    } DL_I2C_TARGET_ADDRESSING_MODE;

    /** @enum DL_I2C_TARGET_PEC_STATUS 目标 PEC 校验状态 */
    typedef enum
    {
        /*!  I2C Target SMBus/PMBus PEC was checked in the transaction that occurred
         *   before the last Stop 上次事务已执行 PEC 校验 */
        DL_I2C_TARGET_PEC_STATUS_CHECKED = I2C_TARGET_PECSR_PECSTS_CHECK_SET,
        /*!  I2C Target SMBus/PMBus PEC was not checked in the transaction that
         *   occurred before the last Stop 上次事务未执行 PEC 校验 */
        DL_I2C_TARGET_PEC_STATUS_NOT_CHECKED =
            I2C_TARGET_PECSR_PECSTS_CHECK_CLEARED,
    } DL_I2C_TARGET_PEC_STATUS;

    /** @enum DL_I2C_TARGET_PEC_CHECK_ERROR 目标 PEC 校验错误状态 */
    typedef enum
    {
        /*!  Indicates PEC check error did not occurr in the transaction that
         *   occurred before the last Stop 上次事务 PEC 校验无错误 */
        DL_I2C_TARGET_PEC_CHECK_ERROR_CLEARED =
            I2C_TARGET_PECSR_PECSTS_ERROR_CLEARED,
        /*!  Indicates PEC check error occurred in the transaction that
         *   occurred before the last Stop 上次事务 PEC 校验出错 */
        DL_I2C_TARGET_PEC_CHECK_ERROR_SET = I2C_TARGET_PECSR_PECSTS_ERROR_SET,
    } DL_I2C_TARGET_PEC_CHECK_ERROR;

    /** @enum DL_I2C_ANALOG_GLITCH_FILTER_WIDTH 模拟毛刺滤波宽度 */
    typedef enum
    {
        /*!  Pulses shorter than 5ns in length are filtered. 滤除 5ns 以下脉冲 */
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_5NS = I2C_GFCTL_AGFSEL_AGLIT_5,
        /*!  Pulses shorter than 10ns in length are filtered. 滤除 10ns 以下脉冲 */
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_10NS = I2C_GFCTL_AGFSEL_AGLIT_10,
        /*!  Pulses shorter than 25ns in length are filtered. 滤除 25ns 以下脉冲 */
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_25NS = I2C_GFCTL_AGFSEL_AGLIT_25,
        /*!  Pulses shorter than 50ns in length are filtered. 滤除 50ns 以下脉冲 */
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_50NS = I2C_GFCTL_AGFSEL_AGLIT_50,
    } DL_I2C_ANALOG_GLITCH_FILTER_WIDTH;

    /** @enum DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH 数字毛刺滤波宽度 */
    typedef enum
    {
        /*!  Pulses filtering disabled. 禁用数字毛刺滤波 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_DISABLED = I2C_GFCTL_DGFSEL_DISABLED,
        /*!  Pulses shorter than 1 functional clock tick in length are filtered. 滤除 1 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_1 = I2C_GFCTL_DGFSEL_CLK_1,
        /*!  Pulses shorter than 2 functional clock ticks in length are filtered. 滤除 2 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_2 = I2C_GFCTL_DGFSEL_CLK_2,
        /*!  Pulses shorter than 3 functional clock ticks in length are filtered. 滤除 3 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_3 = I2C_GFCTL_DGFSEL_CLK_3,
        /*!  Pulses shorter than 4 functional clock ticks in length are filtered. 滤除 4 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_4 = I2C_GFCTL_DGFSEL_CLK_4,
        /*!  Pulses shorter than 8 functional clock ticks in length are filtered. 滤除 8 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_8 = I2C_GFCTL_DGFSEL_CLK_8,
        /*!  Pulses shorter than 16 functional clock ticks in length are filtered. 滤除 16 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_16 = I2C_GFCTL_DGFSEL_CLK_16,
        /*!  Pulses shorter than 31 functional clock ticks in length are filtered. 滤除 31 个时钟周期以下脉冲 */
        DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_31 = I2C_GFCTL_DGFSEL_CLK_31,
    } DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH;

    /** @enum DL_I2C_CONTROLLER_DIRECTION 控制器传输方向 */
    typedef enum
    {
        /*! Set direction to controller transmitting to target 控制器发送方向 */
        DL_I2C_CONTROLLER_DIRECTION_TX = I2C_MSA_DIR_TRANSMIT,
        /*! Set direction to controller receiving from target 控制器接收方向 */
        DL_I2C_CONTROLLER_DIRECTION_RX = I2C_MSA_DIR_RECEIVE,
    } DL_I2C_CONTROLLER_DIRECTION;

    /** @enum DL_I2C_CONTROLLER_ADDRESSING_MODE 控制器寻址模式 */
    typedef enum
    {
        /*! Enable Controller in 7-bit addressing mode 7 位寻址模式 */
        DL_I2C_CONTROLLER_ADDRESSING_MODE_7_BIT = I2C_MSA_MMODE_MODE7,
        /*! Enable Controller in 10-bit addressing mode 10 位寻址模式 */
        DL_I2C_CONTROLLER_ADDRESSING_MODE_10_BIT = I2C_MSA_MMODE_MODE10,
    } DL_I2C_CONTROLLER_ADDRESSING_MODE;

    /** @enum DL_I2C_CONTROLLER_PEC_STATUS 控制器 PEC 校验状态 */
    typedef enum
    {
        /*!  I2C Controller SMBus/PMBus PEC was checked in the transaction that
         *   occurred before the last Stop 上次事务已执行 PEC 校验 */
        DL_I2C_CONTROLLER_PEC_STATUS_CHECKED =
            I2C_CONTROLLER_PECSR_PECSTS_CHECK_SET,
        /*!  I2C Controller SMBus/PMBus PEC was not checked in the transaction that
         *   occurred the last Stop 上次事务未执行 PEC 校验 */
        DL_I2C_CONTROLLER_PEC_STATUS_NOT_CHECKED =
            I2C_CONTROLLER_PECSR_PECSTS_CHECK_CLEARED,
    } DL_I2C_CONTROLLER_PEC_STATUS;

    /** @enum DL_I2C_CONTROLLER_PEC_CHECK_ERROR 控制器 PEC 校验错误状态 */
    typedef enum
    {
        /*! I2C Controller SMBus/PMBus PEC check error occurred in the transaction
         *  before the last stop 上次事务 PEC 校验出错 */
        DL_I2C_CONTROLLER_PEC_CHECK_ERROR_SET =
            I2C_CONTROLLER_PECSR_PECSTS_ERROR_SET,
        /*! I2C Controller SMBus/PMBus PEC check error did not occur in the
         *  transaction before the last stop 上次事务 PEC 校验无错误 */
        DL_I2C_CONTROLLER_PEC_CHECK_ERROR_CLEARED =
            I2C_CONTROLLER_PECSR_PECSTS_ERROR_CLEARED,
    } DL_I2C_CONTROLLER_PEC_CHECK_ERROR;

    /** @enum DL_I2C_CONTROLLER_SCL 控制器 SCL 信号状态 */
    typedef enum
    {
        /*!  I2C SCL signal high SCL 信号高电平 */
        DL_I2C_CONTROLLER_SCL_HIGH = I2C_MBMON_SCL_SET,
        /*!  I2C SCL signal low SCL 信号低电平 */
        DL_I2C_CONTROLLER_SCL_LOW = I2C_MBMON_SCL_CLEARED,
    } DL_I2C_CONTROLLER_SCL;

    /** @enum DL_I2C_CONTROLLER_SDA 控制器 SDA 信号状态 */
    typedef enum
    {
        /*!  I2C SDA signal high SDA 信号高电平 */
        DL_I2C_CONTROLLER_SDA_HIGH = I2C_MBMON_SDA_SET,
        /*!  I2C SDA signal low SDA 信号低电平 */
        DL_I2C_CONTROLLER_SDA_LOW = I2C_MBMON_SDA_CLEARED,
    } DL_I2C_CONTROLLER_SDA;

    /** @enum DL_I2C_CONTROLLER_START 控制器 START 条件生成 */
    typedef enum
    {
        /*!  Controller generates START condition 生成 START 条件 */
        DL_I2C_CONTROLLER_START_ENABLE = I2C_MCTR_START_ENABLE,
        /*!  Controller doesn't generate START condition 不生成 START 条件 */
        DL_I2C_CONTROLLER_START_DISABLE = I2C_MCTR_START_DISABLE,
    } DL_I2C_CONTROLLER_START;

    /** @enum DL_I2C_CONTROLLER_STOP 控制器 STOP 条件生成 */
    typedef enum
    {
        /*!  Controller generates STOP condition 生成 STOP 条件 */
        DL_I2C_CONTROLLER_STOP_ENABLE = I2C_MCTR_STOP_ENABLE,
        /*!  Controller doesn't generate STOP condition 不生成 STOP 条件 */
        DL_I2C_CONTROLLER_STOP_DISABLE = I2C_MCTR_STOP_DISABLE,
    } DL_I2C_CONTROLLER_STOP;

    /** @enum DL_I2C_CONTROLLER_ACK 控制器应答控制 */
    typedef enum
    {
        /*!  Last received data byte of a transaction is not ACKed automatically 最后一字节不自动应答（由软件控制） */
        DL_I2C_CONTROLLER_ACK_ENABLE = I2C_MCTR_ACK_ENABLE,
        /*!  Last received data byte of a transaction is ACKed automatically 最后一字节自动应答 */
        DL_I2C_CONTROLLER_ACK_DISABLE = I2C_MCTR_ACK_DISABLE,
    } DL_I2C_CONTROLLER_ACK;

    /** @enum DL_I2C_TX_FIFO_LEVEL TX FIFO 触发阈值 */
    typedef enum
    {
        /*!  Trigger when the TX FIFO is empty FIFO 空时触发 */
        DL_I2C_TX_FIFO_LEVEL_EMPTY = I2C_MFIFOCTL_TXTRIG_EMPTY,
        /*!  Trigger when TX FIFO contains 1 byte FIFO 含 1 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_1 = I2C_MFIFOCTL_TXTRIG_LEVEL_1,
        /*!  Trigger when TX FIFO contains 2 bytes FIFO 含 2 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_2 = I2C_MFIFOCTL_TXTRIG_LEVEL_2,
        /*!  Trigger when TX FIFO contains 3 bytes FIFO 含 3 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_3 = I2C_MFIFOCTL_TXTRIG_LEVEL_3,
        /*!  Trigger when TX FIFO contains 4 bytes FIFO 含 4 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_4 = I2C_MFIFOCTL_TXTRIG_LEVEL_4,
        /*!  Trigger when TX FIFO contains 5 bytes FIFO 含 5 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_5 = I2C_MFIFOCTL_TXTRIG_LEVEL_5,
        /*!  Trigger when TX FIFO contains 6 bytes FIFO 含 6 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_6 = I2C_MFIFOCTL_TXTRIG_LEVEL_6,
        /*!  Trigger when TX FIFO contains 7 bytes FIFO 含 7 字节时触发 */
        DL_I2C_TX_FIFO_LEVEL_BYTES_7 = I2C_MFIFOCTL_TXTRIG_LEVEL_7,
    } DL_I2C_TX_FIFO_LEVEL;

    /** @enum DL_I2C_RX_FIFO_LEVEL RX FIFO 触发阈值 */
    typedef enum
    {
        /*!  Trigger when RX FIFO contains at least 1 byte FIFO 至少含 1 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_1 = I2C_MFIFOCTL_RXTRIG_LEVEL_1,
        /*!  Trigger when RX FIFO contains at least 2 bytes FIFO 至少含 2 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_2 = I2C_MFIFOCTL_RXTRIG_LEVEL_2,
        /*!  Trigger when RX FIFO contains at least 3 bytes FIFO 至少含 3 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_3 = I2C_MFIFOCTL_RXTRIG_LEVEL_3,
        /*!  Trigger when RX FIFO contains at least 4 bytes FIFO 至少含 4 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_4 = I2C_MFIFOCTL_RXTRIG_LEVEL_4,
        /*!  Trigger when RX FIFO contains at least 5 bytes FIFO 至少含 5 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_5 = I2C_MFIFOCTL_RXTRIG_LEVEL_5,
        /*!  Trigger when RX FIFO contains at least 6 bytes FIFO 至少含 6 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_6 = I2C_MFIFOCTL_RXTRIG_LEVEL_6,
        /*!  Trigger when RX FIFO contains at least 7 bytes FIFO 至少含 7 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_7 = I2C_MFIFOCTL_RXTRIG_LEVEL_7,
        /*!  Trigger when RX FIFO contains at least 8 bytes FIFO 至少含 8 字节时触发 */
        DL_I2C_RX_FIFO_LEVEL_BYTES_8 = I2C_MFIFOCTL_RXTRIG_LEVEL_8,
    } DL_I2C_RX_FIFO_LEVEL;

    /** @enum DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE 目标应答覆盖值 */
    typedef enum
    {
        /*!  Target manual ACK for valid data or command. 手动应答 ACK */
        DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE_ACK = I2C_SACKCTL_ACKOVAL_DISABLE,
        /*!  Target manual NACK for invalid data or command. 手动应答 NACK */
        DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE_NACK = I2C_SACKCTL_ACKOVAL_ENABLE,
    } DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE;

    /*! @enum DL_I2C_IIDX I2C 中断索引 */
    typedef enum
    {
        /*! Interrupt index for I2C if no interrupt is pending 无中断挂起 */
        DL_I2C_IIDX_NO_INT = I2C_CPU_INT_IIDX_STAT_NO_INTR,
        /*! Interrupt index for I2C Controller Receive Transaction completed 控制器接收完成 */
        DL_I2C_IIDX_CONTROLLER_RX_DONE = I2C_CPU_INT_IIDX_STAT_MRXDONEFG,
        /*! Interrupt index for Controller Transmit Transaction completed 控制器发送完成 */
        DL_I2C_IIDX_CONTROLLER_TX_DONE = I2C_CPU_INT_IIDX_STAT_MTXDONEFG,
        /*! Interrupt index for I2C Controller Receive FIFO Trigger 控制器 RX FIFO 触发 */
        DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER = I2C_CPU_INT_IIDX_STAT_MRXFIFOTRG,
        /*! Interrupt index for I2C Controller Transmit FIFO Trigger 控制器 TX FIFO 触发 */
        DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER = I2C_CPU_INT_IIDX_STAT_MTXFIFOTRG,
        /*! Interrupt index for I2C Controller Receive when FIFO is full 控制器 RX FIFO 满 */
        DL_I2C_IIDX_CONTROLLER_RXFIFO_FULL = I2C_CPU_INT_IIDX_STAT_MRXFIFOFULL,
        /*! Interrupt index for I2C Controller when Transmit FIFO is empty 控制器 TX FIFO 空 */
        DL_I2C_IIDX_CONTROLLER_TXFIFO_EMPTY = I2C_CPU_INT_IIDX_STAT_MTX_EMPTY,
        /*! Interrupt index for Address/Data NACK 地址/数据无应答 */
        DL_I2C_IIDX_CONTROLLER_NACK = I2C_CPU_INT_IIDX_STAT_MNACKFG,
        /*! Interrupt index for I2C controller START Detection 控制器 START 检测 */
        DL_I2C_IIDX_CONTROLLER_START = I2C_CPU_INT_IIDX_STAT_MSTARTFG,
        /*! Interrupt index for I2C controller STOP Detection 控制器 STOP 检测 */
        DL_I2C_IIDX_CONTROLLER_STOP = I2C_CPU_INT_IIDX_STAT_MSTOPFG,
        /*! Interrupt index for I2C controller Arbitration Lost 控制器仲裁丢失 */
        DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST = I2C_CPU_INT_IIDX_STAT_MARBLOSTFG,
        /*! Interrupt index for I2C controller Event 1 DMA Done 控制器 DMA 事件 1 完成 */
        DL_I2C_IIDX_CONTROLLER_EVENT1_DMA_DONE =
            I2C_CPU_INT_IIDX_STAT_MDMA_DONE_TX,
        /*! Interrupt index for I2C controller Event 2 DMA Done 控制器 DMA 事件 2 完成 */
        DL_I2C_IIDX_CONTROLLER_EVENT2_DMA_DONE =
            I2C_CPU_INT_IIDX_STAT_MDMA_DONE_RX,

        /*! Interrupt index for I2C controller SMBus/PMBus PEC Receive Error Event 控制器 PEC 接收错误 */
        DL_I2C_IIDX_CONTROLLER_PEC_RX_ERROR = I2C_CPU_INT_IIDX_STAT_MPEC_RX_ERR,
        /*! Interrupt index for I2C Timeout A Event 超时 A 事件 */
        DL_I2C_IIDX_TIMEOUT_A = I2C_CPU_INT_IIDX_STAT_TIMEOUTA,
        /*! Interrupt index for I2C Timeout B Event 超时 B 事件 */
        DL_I2C_IIDX_TIMEOUT_B = I2C_CPU_INT_IIDX_STAT_TIMEOUTB,

        /*! Interrupt index for I2C Target Receive Data 目标接收数据完成 */
        DL_I2C_IIDX_TARGET_RX_DONE = I2C_CPU_INT_IIDX_STAT_SRXDONEFG,
        /*! Interrupt index for I2C Target Transmit Transaction completed 目标发送完成 */
        DL_I2C_IIDX_TARGET_TX_DONE = I2C_CPU_INT_IIDX_STAT_STXDONEFG,
        /*! Interrupt index for I2C Target Receive FIFO Trigger 目标 RX FIFO 触发 */
        DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER = I2C_CPU_INT_IIDX_STAT_SRXFIFOTRG,
        /*! Interrupt index for I2C Target Transmit FIFO Trigger 目标 TX FIFO 触发 */
        DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER = I2C_CPU_INT_IIDX_STAT_STXFIFOTRG,
        /*! Interrupt index for I2C Target RX FIFO full 目标 RX FIFO 满 */
        DL_I2C_IIDX_TARGET_RXFIFO_FULL = I2C_CPU_INT_IIDX_STAT_SRXFIFOFULL,
        /*! Interrupt index for I2C Target TX FIFO empty.
         * All data in Transmit FIFO shifted out and transmit goes into idle mode.
         * 目标 TX FIFO 空，所有数据移出后发送空闲 */
        DL_I2C_IIDX_TARGET_TXFIFO_EMPTY = I2C_CPU_INT_IIDX_STAT_STXEMPTY,
        /*! Interrupt index for I2C Target Start Condition detected 目标 START 检测 */
        DL_I2C_IIDX_TARGET_START = I2C_CPU_INT_IIDX_STAT_SSTARTFG,
        /*! Interrupt index for I2C Target Stop Condition detected 目标 STOP 检测 */
        DL_I2C_IIDX_TARGET_STOP = I2C_CPU_INT_IIDX_STAT_SSTOPFG,
        /*! Interrupt index for I2C General Call 通用呼叫中断 */
        DL_I2C_IIDX_TARGET_GENERAL_CALL = I2C_CPU_INT_IIDX_STAT_SGENCALL,
        /*! Interrupt index for I2C Target Event 1 DMA Done 目标 DMA 事件 1 完成 */
        DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE = I2C_CPU_INT_IIDX_STAT_SDMA_DONE_TX,
        /*! Interrupt index for I2C Target Event 2 DMA Done 目标 DMA 事件 2 完成 */
        DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE = I2C_CPU_INT_IIDX_STAT_SDMA_DONE_RX,

        /*! Interrupt index for I2C Target SMBus/PMBus PEC Receive Error Event 目标 PEC 接收错误 */
        DL_I2C_IIDX_TARGET_PEC_RX_ERROR = I2C_CPU_INT_IIDX_STAT_SPEC_RX_ERR,
        /*! Interrupt index for I2C Target TX FIFO underflow event 目标 TX FIFO 下溢 */
        DL_I2C_IIDX_TARGET_TXFIFO_UNDERFLOW = I2C_CPU_INT_IIDX_STAT_STX_UNFL,
        /*! Interrupt index for I2C Target RX FIFO overflow event 目标 RX FIFO 溢出 */
        DL_I2C_IIDX_TARGET_RXFIFO_OVERFLOW = I2C_CPU_INT_IIDX_STAT_SRX_OVFL,
        /*! Interrupt index for I2C Target arbitration lost event 目标仲裁丢失 */
        DL_I2C_IIDX_TARGET_ARBITRATION_LOST = I2C_CPU_INT_IIDX_STAT_SARBLOST,
        /*! Interrupt index for I2C interrupt overflow event 中断溢出 */
        DL_I2C_IIDX_INTERRUPT_OVERFLOW = I2C_CPU_INT_IIDX_STAT_INTR_OVFL,

    } DL_I2C_IIDX;

    /**
     * @brief  Configuration struct for @ref DL_I2C_setClockConfig.
     *         I2C 时钟配置结构体。
     */
    typedef struct
    {
        /*! I2C module clock source. One of @ref DL_I2C_CLOCK
         *  I2C 模块时钟源选择 */
        DL_I2C_CLOCK clockSel;
        /*! I2C clock divider selection. One of @ref DL_I2C_CLOCK_DIVIDE
         *  I2C 时钟分频比选择 */
        DL_I2C_CLOCK_DIVIDE divideRatio;
    } DL_I2C_ClockConfig;

    /**
     *  @brief      Configure I2C source clock
     *  @details    功能：配置 I2C 模块的时钟源与分频参数。
     *
     *  @param[in]  i2c    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                       @ref DL_I2C_ClockConfig.
     *                       参数：时钟配置结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_setClockConfig(I2C_Regs *i2c, const DL_I2C_ClockConfig *config);

    /**
     *  @brief      Get I2C source clock configuration
     *  @details    功能：读取 I2C 模块当前时钟源与分频参数。
     *
     *  @param[in]  i2c    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                      @ref DL_I2C_ClockConfig.
     *                      参数：用于回填的时钟配置结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_getClockConfig(const I2C_Regs *i2c, DL_I2C_ClockConfig *config);

    /**
     *  @brief      Fills the controller TX FIFO with data
     *  @details    功能：将缓存区数据写入控制器 TX FIFO，写满即停止。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  buffer  Pointer to buffer of bytes
     *                      参数：待发送数据缓冲区首地址。
     *  @param[in]  count   Number of bytes to fill controller TX FIFO from buffer
     *                      [0x00, 0xFFF]
     *                      参数：期望写入的字节数量。
     *
     *  @return     Number of bytes that were successfully written
     *              返回值：实际成功写入 FIFO 的字节数。
     */
    uint16_t DL_I2C_fillControllerTXFIFO(
        I2C_Regs *i2c, const uint8_t *buffer, uint16_t count);

    /**
     *  @brief      Flushes/removes all elements in the controller TX FIFO
     *  @details    功能：清空控制器 TX FIFO 并等待硬件完成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_flushControllerTXFIFO(I2C_Regs *i2c);

    /**
     *  @brief      Flushes/removes all elements in the controller RX FIFO
     *  @details    功能：清空控制器 RX FIFO 并等待硬件完成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_flushControllerRXFIFO(I2C_Regs *i2c);

    /**
     *  @brief      Checks if controller TX FIFO is full
     *              检查控制器 TX FIFO 是否已满。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller TX FIFO is full
     *              返回值：`true` 表示 FIFO 已满，`false` 表示未满。
     *
     *  @retval     true if controller TX FIFO is full
     *  @retval     false if controller TX FIFO is not full
     */
    __STATIC_INLINE bool DL_I2C_isControllerTXFIFOFull(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK) ==
                I2C_MFIFOSR_TXFIFOCNT_MINIMUM);
    }

    /**
     *  @brief      Checks if controller TX FIFO is empty
     *              检查控制器 TX FIFO 是否为空。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller TX FIFO is empty
     *              返回值：`true` 表示 FIFO 为空，`false` 表示非空。
     *
     *  @retval     true if controller TX FIFO is empty
     *  @retval     false if controller TX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2C_isControllerTXFIFOEmpty(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK) ==
                DL_I2C_TX_FIFO_COUNT_MAXIMUM);
    }

    /**
     *  @brief      Checks if controller RX FIFO is empty
     *              检查控制器 RX FIFO 是否为空。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller RX FIFO is empty
     *              返回值：`true` 表示 FIFO 为空，`false` 表示非空。
     *
     *  @retval     true if controller RX FIFO is empty
     *  @retval     false if controller RX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2C_isControllerRXFIFOEmpty(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MFIFOSR & I2C_MFIFOSR_RXFIFOCNT_MASK) ==
                I2C_MFIFOSR_RXFIFOCNT_MINIMUM);
    }

    /**
     *  @brief      Reset transfers from from I2C controller
     *              重置控制器传输寄存器，恢复 I2C 初始状态。
     *
     *  Resets transfer register to initialize I2C
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_resetControllerTransfer(I2C_Regs *i2c)
    {
        // Resets all Controller functionality
        i2c->MASTER.MCTR = 0x00;
    }

    /**
     *  @brief      Sets up a transfer from I2C controller
     *              启动控制器传输，自动设置目标地址、方向、突发长度及 START/STOP。
     *
     *  Set target address, transfer direction, burst length, START+STOP generation.
     *  @note   Reading/writing data must be done separately.
     *          数据读写需另行操作。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *  @param[in]  targetAddr  Target address [0x00, 0x3FF]
     *                          目标地址 [0x00, 0x3FF]。
     *  @param[in]  direction   One of @ref DL_I2C_CONTROLLER_DIRECTION
     *                          传输方向。
     *  @param[in]  length      Intended burst length in number of bytes
     *                          [0x00, 0xFFF]
     *                          突发传输字节数。
     */
    __STATIC_INLINE void DL_I2C_startControllerTransfer(I2C_Regs *i2c,
                                                        uint32_t targetAddr, DL_I2C_CONTROLLER_DIRECTION direction,
                                                        uint16_t length)
    {
        // Specify target address and read/write mode
        DL_Common_updateReg(&i2c->MASTER.MSA,
                            ((targetAddr << I2C_MSA_SADDR_OFS) | (uint32_t)direction),
                            (I2C_MSA_SADDR_MASK | I2C_MSA_DIR_MASK));

        // STOP bit is generated after burst length number of bytes transferred
        DL_Common_updateReg(&i2c->MASTER.MCTR,
                            (((uint32_t)length << I2C_MCTR_MBLEN_OFS) | I2C_MCTR_BURSTRUN_ENABLE |
                             I2C_MCTR_START_ENABLE | I2C_MCTR_STOP_ENABLE),
                            (I2C_MCTR_MBLEN_MASK | I2C_MCTR_BURSTRUN_MASK | I2C_MCTR_START_MASK |
                             I2C_MCTR_STOP_MASK));
    }

    /**
     *  @brief      Sets up a transfer from I2C controller with control of START,
     *  STOP and ACK
     *              启动控制器传输，可分别控制 START、STOP 和 ACK 行为。
     *
     *  @note   Reading/writing data must be done separately.
     *          数据读写需另行操作。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  targetAddr  7-bit target address [0x00, 0x7f]
     *                          7 位目标地址。
     *  @param[in]  direction   One of @ref DL_I2C_CONTROLLER_DIRECTION
     *                          传输方向。
     *  @param[in]  length      Intended burst length in number of bytes
     *                          突发传输字节数。
     *  @param[in]  start       One of @ref DL_I2C_CONTROLLER_START
     *                          START 条件使能。
     *  @param[in]  stop        One of @ref DL_I2C_CONTROLLER_STOP
     *                          STOP 条件使能。
     *  @param[in]  ack         One of @ref DL_I2C_CONTROLLER_ACK
     *                          ACK 控制。
     */
    __STATIC_INLINE void DL_I2C_startControllerTransferAdvanced(I2C_Regs *i2c,
                                                                uint32_t targetAddr, DL_I2C_CONTROLLER_DIRECTION direction,
                                                                uint16_t length, DL_I2C_CONTROLLER_START start,
                                                                DL_I2C_CONTROLLER_STOP stop, DL_I2C_CONTROLLER_ACK ack)
    {
        // Specify target address and read/write mode
        DL_Common_updateReg(&i2c->MASTER.MSA,
                            ((targetAddr << I2C_MSA_SADDR_OFS) | (uint32_t)direction),
                            (I2C_MSA_SADDR_MASK | I2C_MSA_DIR_MASK));

        DL_Common_updateReg(&i2c->MASTER.MCTR,
                            (((uint32_t)length << I2C_MCTR_MBLEN_OFS) | I2C_MCTR_BURSTRUN_ENABLE |
                             (uint32_t)start | (uint32_t)stop | (uint32_t)ack),
                            (I2C_MCTR_MBLEN_MASK | I2C_MCTR_BURSTRUN_MASK | I2C_MCTR_START_MASK |
                             I2C_MCTR_STOP_MASK | I2C_MCTR_ACK_MASK));
    }

    /**
     *  @brief      Checks if target TX FIFO is full
     *              检查目标 TX FIFO 是否已满。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target TX FIFO is full
     *              返回值：`true` 表示 FIFO 已满，`false` 表示未满。
     *
     *  @retval     true if target TX FIFO is full
     *  @retval     false if target TX FIFO is not full
     */
    __STATIC_INLINE bool DL_I2C_isTargetTXFIFOFull(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_TXFIFOCNT_MASK) ==
                I2C_SFIFOSR_TXFIFOCNT_MINIMUM);
    }

    /**
     *  @brief      Checks if target TX FIFO is empty
     *              检查目标 TX FIFO 是否为空。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target TX FIFO is empty
     *              返回值：`true` 表示 FIFO 为空，`false` 表示非空。
     *
     *  @retval     true if target TX FIFO is empty
     *  @retval     false if target TX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2C_isTargetTXFIFOEmpty(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_TXFIFOCNT_MASK) ==
                DL_I2C_TX_FIFO_COUNT_MAXIMUM);
    }

    /**
     *  @brief      Checks if target RX FIFO is empty
     *              检查目标 RX FIFO 是否为空。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target RX FIFO is empty
     *              返回值：`true` 表示 FIFO 为空，`false` 表示非空。
     *
     *  @retval     true if target RX FIFO is empty
     *  @retval     false if target RX FIFO is not empty
     */
    __STATIC_INLINE bool DL_I2C_isTargetRXFIFOEmpty(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_RXFIFOCNT_MASK) ==
                I2C_SFIFOSR_RXFIFOCNT_MINIMUM);
    }

    /**
     *  @brief      Fills the target TX FIFO with data
     *  @details    功能：将缓存区数据写入目标模式 TX FIFO，写满即停止。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  buffer  Pointer to buffer of bytes
     *                      参数：待发送数据缓冲区首地址。
     *  @param[in]  count   Number of bytes to fill target TX FIFO from buffer
     *                      参数：期望写入的字节数量。
     *
     *  @return     Number of bytes that were successfully written
     *              返回值：实际成功写入 FIFO 的字节数。
     */
    uint8_t DL_I2C_fillTargetTXFIFO(
        I2C_Regs *i2c, const uint8_t *buffer, uint8_t count);

    /**
     *  @brief      Flushes/removes all elements in the target TX FIFO
     *  @details    功能：清空目标模式 TX FIFO 并等待硬件完成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_flushTargetTXFIFO(I2C_Regs *i2c);

    /**
     *  @brief      Flushes/removes all elements in the target RX FIFO
     *  @details    功能：清空目标模式 RX FIFO 并等待硬件完成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_flushTargetRXFIFO(I2C_Regs *i2c);

    /**
     *  @brief  Transmit target data, blocking until transmit request is received.
     * 			Will wait indefintely until bus is not busy. Note that if data is
     *          already present in the TX FIFO when this API is called, it will
     *          block until all data is sent.
     *  @details 功能：阻塞等待发送请求后发送 1 字节，并等待总线空闲。
     *
     *  @note   Setting own target addresses and enabling target should be done
     *  separately.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  data    Byte of data to transmit
     *                      参数：待发送字节。
     *
     *  @return     返回值：无。
     */
    void DL_I2C_transmitTargetDataBlocking(I2C_Regs *i2c, uint8_t data);

    /**
     *  @brief  Transmit target data
     *  @details 功能：尝试发送 1 字节，若 TX FIFO 满则直接返回失败。
     *
     *  @note   Setting own target addresses and enabling target should be done
     *  separately.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  data    Byte of data to transmit
     *                      参数：待发送字节。
     *
     *  @return Whether data could be transmitted or not
     *  @retval true if data could be transmitted
     *  @retval false if data could not be transmitted
     *          返回值：`true` 表示发送成功，`false` 表示 FIFO 满。
     */
    bool DL_I2C_transmitTargetDataCheck(I2C_Regs *i2c, uint8_t data);

    /**
     *  @brief  Receive target data, waiting until receive request
     *  @details 功能：阻塞等待接收请求后读取 1 字节目标数据。
     *
     *  @note   Setting own target addresses and enabling target should be done
     *  separately.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *
     *  @return Data received from target
     *          返回值：读取到的目标数据字节。
     */
    uint8_t DL_I2C_receiveTargetDataBlocking(const I2C_Regs *i2c);

    /**
     *  @brief  Receive target data
     *  @details 功能：尝试读取 1 字节，若 RX FIFO 为空则返回失败。
     *
     *  @note   Setting own target addresses and enabling target should be done
     *  separately.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      参数：I2C 外设寄存器基址。
     *  @param[in]  buffer  Pointer to where byte of received data should be stored
     *                      参数：接收数据输出缓冲区指针。
     *
     *  @return Whether data could be received or not
     *  @retval true if data could be received
     *  @retval false if data could not be received
     *          返回值：`true` 表示读取成功，`false` 表示 FIFO 为空。
     */
    bool DL_I2C_receiveTargetDataCheck(const I2C_Regs *i2c, uint8_t *buffer);

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the I2C
     *        使能 I2C 外设电源写入使能寄存器。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     * @param i2c        Pointer to the register overlay for the peripheral
     *                   I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enablePower(I2C_Regs *i2c)
    {
        i2c->GPRCM.PWREN = (I2C_PWREN_KEY_UNLOCK_W | I2C_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the I2C
     *        禁用 I2C 外设电源写入使能寄存器。
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @note This API does not provide large power savings.
     *        该 API 节能效果有限。
     *
     * @param i2c        Pointer to the register overlay for the peripheral
     *                   I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disablePower(I2C_Regs *i2c)
    {
        i2c->GPRCM.PWREN = (I2C_PWREN_KEY_UNLOCK_W | I2C_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the I2C
     *        is enabled
     *        检查 I2C 外设电源写入使能是否已启用。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     * @param i2c        Pointer to the register overlay for the peripheral
     *                   I2C 外设寄存器基址。
     *
     * @return true if peripheral register access is enabled
     * @return false if peripheral register access is disabled
     *         返回值：`true` 表示已使能，`false` 表示已禁用。
     */
    __STATIC_INLINE bool DL_I2C_isPowerEnabled(const I2C_Regs *i2c)
    {
        return (
            (i2c->GPRCM.PWREN & I2C_PWREN_ENABLE_MASK) == I2C_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Resets i2c peripheral
     *        复位 I2C 外设。
     *
     * @param i2c        Pointer to the register overlay for the peripheral
     *                   I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_reset(I2C_Regs *i2c)
    {
        i2c->GPRCM.RSTCTL =
            (I2C_RSTCTL_KEY_UNLOCK_W | I2C_RSTCTL_RESETSTKYCLR_CLR |
             I2C_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief Returns if i2c peripheral was reset
     *        检查 I2C 外设是否已复位。
     *
     * @param i2c        Pointer to the register overlay for the peripheral
     *                   I2C 外设寄存器基址。
     *
     * @return true if peripheral was reset
     * @return false if peripheral wasn't reset
     *         返回值：`true` 表示已复位，`false` 表示未复位。
     */
    __STATIC_INLINE bool DL_I2C_isReset(const I2C_Regs *i2c)
    {
        return ((i2c->GPRCM.STAT & I2C_STAT_RESETSTKY_MASK) ==
                I2C_STAT_RESETSTKY_RESET);
    }

    /**
     * @brief Set Clock Source
     *        设置 I2C 时钟源。
     *
     *  Clock sources depend on device and clock should be enabled
     *
     *  @param[in]  i2c            Pointer to the register overlay for the
     *                             peripheral
     *                             I2C 外设寄存器基址。
     *  @param[in]  clockSource    One of @ref DL_I2C_CLOCK.
     *                             时钟源选择。
     */
    __STATIC_INLINE void DL_I2C_selectClockSource(
        I2C_Regs *i2c, DL_I2C_CLOCK clockSource)
    {
        DL_Common_updateReg(&i2c->CLKSEL, (uint32_t)clockSource,
                            I2C_CLKSEL_BUSCLK_SEL_MASK | I2C_CLKSEL_MFCLK_SEL_MASK);
    }

    /**
     * @brief Set Clock Divider
     *        设置 I2C 时钟分频比。
     *
     *  @param[in]  i2c            Pointer to the register overlay for the
     *                             peripheral
     *                             I2C 外设寄存器基址。
     *  @param[in]  clockDivider   One of @ref DL_I2C_CLOCK_DIVIDE.
     *                             时钟分频比。
     */
    __STATIC_INLINE void DL_I2C_selectClockDivider(
        I2C_Regs *i2c, DL_I2C_CLOCK_DIVIDE clockDivider)
    {
        DL_Common_updateReg(
            &i2c->CLKDIV, (uint32_t)clockDivider, I2C_CLKDIV_RATIO_MASK);
    }

    /**
     *  @brief      Get Analog Glitch Suppression Pulse Width
     *              获取模拟毛刺抑制脉冲宽度。
     *
     *  Pulse width for the analog glitch suppression on SCL/SDA lines.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Analog glitch suppression pulse width
     *              返回值：当前模拟毛刺滤波宽度。
     *
     *  @retval     One of @ref DL_I2C_ANALOG_GLITCH_FILTER_WIDTH.
     */
    __STATIC_INLINE DL_I2C_ANALOG_GLITCH_FILTER_WIDTH
    DL_I2C_getAnalogGlitchFilterPulseWidth(const I2C_Regs *i2c)
    {
        uint32_t filterWidth = i2c->GFCTL & I2C_GFCTL_AGFSEL_MASK;

        return (DL_I2C_ANALOG_GLITCH_FILTER_WIDTH)(filterWidth);
    }

    /**
     *  @brief      Set Analog Glitch Suppression Pulse Width
     *              设置模拟毛刺抑制脉冲宽度。
     *
     *  Pulse width for the analog glitch suppression on SCL/SDA lines.
     *
     *  @param[in]  i2c            Pointer to the register overlay for the
     *                             peripheral
     *                             I2C 外设寄存器基址。
     *  @param[in]  filterWidth    One of @ref DL_I2C_ANALOG_GLITCH_FILTER_WIDTH.
     *                             模拟毛刺滤波宽度。
     */
    __STATIC_INLINE void DL_I2C_setAnalogGlitchFilterPulseWidth(
        I2C_Regs *i2c, DL_I2C_ANALOG_GLITCH_FILTER_WIDTH filterWidth)
    {
        DL_Common_updateReg(
            &i2c->GFCTL, (uint32_t)filterWidth, I2C_GFCTL_AGFSEL_MASK);
    }

    /**
     *  @brief      Get Digital Glitch Suppression Pulse Width
     *              获取数字毛刺抑制脉冲宽度（单位：功能时钟周期）。
     *
     *  Pulse width for the digital glitch suppression on SCL/SDA lines.
     *  Values are in terms of functional clock ticks.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Digital glitch suppression pulse width
     *              返回值：当前数字毛刺滤波宽度。
     *
     *  @retval     One of @ref DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH.
     */
    __STATIC_INLINE DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH
    DL_I2C_getDigitalGlitchFilterPulseWidth(const I2C_Regs *i2c)
    {
        uint32_t filterWidth = i2c->GFCTL & I2C_GFCTL_DGFSEL_MASK;

        return (DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH)(filterWidth);
    }

    /**
     *  @brief      Set Digital Glitch Suppression Pulse Width
     *              设置数字毛刺抑制脉冲宽度（单位：功能时钟周期）。
     *
     *  Pulse width for the digital glitch suppression on SCL/SDA lines.
     *  Values are in terms of functional clock ticks.
     *
     *  @param[in]  i2c             Pointer to the register overlay for the
     *                              peripheral
     *                              I2C 外设寄存器基址。
     *  @param[in]  filterWidth     One of @ref DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH.
     *                              数字毛刺滤波宽度。
     */
    __STATIC_INLINE void DL_I2C_setDigitalGlitchFilterPulseWidth(
        I2C_Regs *i2c, DL_I2C_DIGITAL_GLITCH_FILTER_WIDTH filterWidth)
    {
        DL_Common_updateReg(
            &i2c->GFCTL, (uint32_t)filterWidth, I2C_GFCTL_DGFSEL_MASK);
    }

    /**
     *  @brief      Disable Analog Glitch Suppression
     *              禁用模拟毛刺抑制。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableAnalogGlitchFilter(I2C_Regs *i2c)
    {
        i2c->GFCTL &= ~(I2C_GFCTL_AGFEN_MASK);
    }

    /**
     *  @brief      Checks if analog glitch suppression is enabled
     *              检查模拟毛刺抑制是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If analog glitch suppression is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if analog glitch suppression is enabled
     *  @retval     false if analog glitch suppression is disabled
     */
    __STATIC_INLINE bool DL_I2C_isAnalogGlitchFilterEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->GFCTL & I2C_GFCTL_AGFEN_MASK) == I2C_GFCTL_AGFEN_ENABLE);
    }

    /**
     *  @brief      Enable Analog Glitch Suppression
     *              启用模拟毛刺抑制。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableAnalogGlitchFilter(I2C_Regs *i2c)
    {
        i2c->GFCTL |= I2C_GFCTL_AGFEN_ENABLE;
    }

    /**
     *  @brief      Get direction of next controller operation
     *              获取控制器下一次操作的传输方向。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Direction of next controller operation
     *              返回值：传输方向。
     *
     *  @retval     One of @ref DL_I2C_CONTROLLER_DIRECTION
     */
    __STATIC_INLINE DL_I2C_CONTROLLER_DIRECTION DL_I2C_getControllerDirection(
        const I2C_Regs *i2c)
    {
        uint32_t direction = i2c->MASTER.MSA & I2C_MSA_DIR_MASK;

        return (DL_I2C_CONTROLLER_DIRECTION)(direction);
    }

    /**
     *  @brief      Set direction of next controller operation
     *              设置控制器下一次操作的传输方向。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @param[in]  direction   Direction of next controller operation.
     *                          One of @ref DL_I2C_CONTROLLER_DIRECTION.
     *                          传输方向。
     */
    __STATIC_INLINE void DL_I2C_setControllerDirection(
        I2C_Regs *i2c, DL_I2C_CONTROLLER_DIRECTION direction)
    {
        DL_Common_updateReg(
            &i2c->MASTER.MSA, (uint32_t)direction, I2C_MSA_DIR_MASK);
    }

    /**
     *  @brief      Get the address of the target being addressed when configured
     *              as an I2C controller
     *              获取控制器模式下当前寻址的目标地址。
     *
     *  Specifies bits A9 through A0 of the target address.
     *  In 7-bit addressing mode as selected by @ref DL_I2C_setTargetAddressingMode,
     *  the top 3 bits are don't care.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     The target address
     *              返回值：目标地址 [0x00, 0x07FE]。
     *
     *  @retval     [0x00, 0x07FE]
     *
     *  @sa         DL_I2C_setTargetAddressingMode
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetAddress(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MSA & I2C_MSA_SADDR_MASK) >> I2C_MSA_SADDR_OFS);
    }

    /**
     *  @brief      Set the address of the target being addressed when configured
     *              as an I2C controller
     *              设置控制器模式下要寻址的目标地址。
     *
     *  Specifies bits A9 through A0 of the target address.
     *  In 7-bit addressing mode as selected by @ref DL_I2C_setTargetAddressingMode,
     *  the top 3 bits are don't care.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  targetAddress    Target address to set [0x00, 0x07FE]
     *                              目标地址 [0x00, 0x07FE]。
     *
     *  @sa         DL_I2C_setTargetAddressingMode
     */
    __STATIC_INLINE void DL_I2C_setTargetAddress(
        I2C_Regs *i2c, uint32_t targetAddress)
    {
        DL_Common_updateReg(&i2c->MASTER.MSA, (targetAddress << I2C_MSA_SADDR_OFS),
                            I2C_MSA_SADDR_MASK);
    }

    /**
     *  @brief      Get controller addressing mode
     *              获取控制器寻址模式（7 位或 10 位）。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Addressing mode the controller is set to
     *              返回值：当前寻址模式。
     *
     *  @retval     One of @ref DL_I2C_CONTROLLER_ADDRESSING_MODE
     */
    __STATIC_INLINE DL_I2C_CONTROLLER_ADDRESSING_MODE
    DL_I2C_getControllerAddressingMode(const I2C_Regs *i2c)
    {
        uint32_t mode = i2c->MASTER.MSA & I2C_MSA_MMODE_MASK;

        return (DL_I2C_CONTROLLER_ADDRESSING_MODE)(mode);
    }

    /**
     *  @brief      Set controller addressing mode between 7-bit and 10-bit mode
     *              设置控制器寻址模式（7 位或 10 位）。
     *
     *  Selects the addressing mode between 7-bit and 10-bit mode to be used when
     *  device is configured as a controller
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  mode    Addressing mode to set the target to.
     *                      One of @ref DL_I2C_CONTROLLER_ADDRESSING_MODE.
     *                      寻址模式。
     */
    __STATIC_INLINE void DL_I2C_setControllerAddressingMode(
        I2C_Regs *i2c, DL_I2C_CONTROLLER_ADDRESSING_MODE mode)
    {
        DL_Common_updateReg(&i2c->MASTER.MSA, (uint32_t)mode, I2C_MSA_MMODE_MASK);
    }

    /**
     *  @brief      Disable controller ACK override
     *              禁用控制器 ACK 覆盖。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableControllerACKOverride(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR &= ~(I2C_MCTR_MACKOEN_MASK);
    }

    /**
     *  @brief      Checks if controller ACK override is enabled
     *              检查控制器 ACK 覆盖是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller ACK override is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if controller ACK override is enabled
     *  @retval     false if controller ACK override is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerACKOverrideEnabled(const I2C_Regs *i2c)
    {
        return (
            (i2c->MASTER.MCTR & I2C_MCTR_MACKOEN_MASK) == I2C_MCTR_MACKOEN_ENABLE);
    }

    /**
     *  @brief      Enable controller ACK override
     *              启用控制器 ACK 覆盖，允许软件手动控制 ACK/NACK。
     *
     *  When enabled and the controller is receiving data and the number of bytes
     *  indicated in MCTR.MBLEN have been received, the state machine will generate
     *  an RXDONE interrupt and wait at the start of the ACK for FW to indicate if
     *  an ACK or NACK should be sent. The ACK or NACK is selected by writing the
     *  MCTR register and setting ACK accordingly. The other fields in this register
     *  can also be written at this time to continue on with the transaction. If a
     *  NACK is sent the state machine will automatically send a Stop.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     * @sa          DL_I2C_setTransactionLength
     */
    __STATIC_INLINE void DL_I2C_enableControllerACKOverride(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR |= I2C_MCTR_MACKOEN_ENABLE;
    }

    /**
     *  @brief      Disable controller read on TX empty
     *              禁用控制器 TX 空后自动读取功能。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableControllerReadOnTXEmpty(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR &= ~(I2C_MCTR_RD_ON_TXEMPTY_MASK);
    }

    /**
     *  @brief      Checks if controller read on TX empty is enabled
     *              检查控制器 TX 空后自动读取是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller read on TX empty is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if controller read on TX empty is enabled
     *  @retval     false if controller read on TX empty is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerReadOnTXEmptyEnabled(
        const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCTR & I2C_MCTR_RD_ON_TXEMPTY_MASK) ==
                I2C_MCTR_RD_ON_TXEMPTY_ENABLE);
    }

    /**
     *  @brief      Enable controller read on TX empty
     *              启用控制器 TX 空后自动切换为读模式。
     *
     *  When enabled, the controller will transmit all bytes from the TX FIFO
     *  before continuing with the programmed Burst Run Read. If the DIR is not
     *  set to Read in the MSA then this bit is ignored. The Start must be set in
     *  the MCTR for proper I2C protocol. The controller will first send the Start
     *  Condition, I2C Address with R/W bit set to write, before sending the bytes
     *  in the TX FIFO. When the TX FIFO is empty, the I2C transaction will
     *  continue as programmed in MTCR and MSA without sending a Stop Condition.
     *  This is intended to be used to perform simple I2C command based reads
     *  transition that will complete after initiating them without having to get
     *  an interrupt to turn the bus around.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableControllerReadOnTXEmpty(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR |= I2C_MCTR_RD_ON_TXEMPTY_ENABLE;
    }

    /**
     *  @brief      Get the SMBus/PMBus Packet Error Checking (PEC) count value
     *              获取控制器 PEC 计数值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     The value the PEC count is set to
     *              返回值：PEC 计数值 [0x0, 0x01FF]。
     *
     *  @retval     Value between [0x0, 0x01FF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getControllerPECCountValue(const I2C_Regs *i2c)
    {
        return (i2c->MASTER.CONTROLLER_I2CPECCTL &
                I2C_CONTROLLER_I2CPECCTL_PECCNT_MASK);
    }

    /**
     *  @brief      Set the SMBus/PMBus Packet Error Checking (PEC) count value
     *              设置控制器 PEC 计数值。
     *
     *  When this field is non zero, the number of I2C data bytes are counted.
     *  Refer to the device TRM for more details.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  count   The value to set the PEC count to.
     *                      Value between [0x0, 0x01FF]
     *                      PEC 计数值。
     */
    __STATIC_INLINE void DL_I2C_setControllerPECCountValue(
        I2C_Regs *i2c, uint32_t count)
    {
        DL_Common_updateReg(&i2c->MASTER.CONTROLLER_I2CPECCTL, count,
                            I2C_CONTROLLER_I2CPECCTL_PECCNT_MASK);
    }

    /**
     *  @brief      Disable controller SMBus/PMBus Packet Error Checking (PEC)
     *              禁用控制器 PEC 校验。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableControllerPEC(I2C_Regs *i2c)
    {
        i2c->MASTER.CONTROLLER_I2CPECCTL &= ~(I2C_CONTROLLER_I2CPECCTL_PECEN_MASK);
    }

    /**
     *  @brief      Checks if controller SMBus/PMBus Packet Error Checking (PEC)
     *              is enabled
     *              检查控制器 PEC 校验是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller PEC is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if controller PEC is enabled
     *  @retval     false if controller PEC is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerPECEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.CONTROLLER_I2CPECCTL &
                 I2C_CONTROLLER_I2CPECCTL_PECEN_MASK) ==
                I2C_CONTROLLER_I2CPECCTL_PECEN_ENABLE);
    }

    /**
     *  @brief      Enable controller SMBus/PMBus Packet Error Checking (PEC)
     *              启用控制器 PEC 校验，多项式为 x^8 + x^2 + x^1 + 1。
     *
     *  When enabled, the PEC is calculated on all bits accept the Start, Stop, ACK
     *  and NACK. The PEC LSFR and the Byte Counter is set to 0 when the State
     *  Machine is in the IDLE state, which occurs following a Stop or when a
     *  timeout occurs. The Counter is also set to 0 after the PEC byte is sent or
     *  received. Note that the NACK is automatically sent following a PEC byte
     *  that results in a PEC error.
     *  The PEC Polynomial is x^8 + x^2 + x^1 + 1.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableControllerPEC(I2C_Regs *i2c)
    {
        i2c->MASTER.CONTROLLER_I2CPECCTL |= I2C_CONTROLLER_I2CPECCTL_PECEN_ENABLE;
    }

    /**
     *  @brief      Get the current SMBus/PMBus PEC byte count of the controller
     *              state machine
     *              获取控制器状态机当前 PEC 字节计数。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     The current PEC byte count of the controller state machine
     *              返回值：当前 PEC 字节计数 [0x0, 0x01FF]。
     *
     *  @retval     Value between [0x0, 0x01FF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getControllerCurrentPECCount(
        const I2C_Regs *i2c)
    {
        return (
            i2c->MASTER.CONTROLLER_PECSR & I2C_CONTROLLER_PECSR_PECSTS_CHECK_MASK);
    }

    /**
     *  @brief      If controller SMBus/PMBus PEC was checked in last transaction
     *              查询上次事务是否执行了控制器 PEC 校验。
     *
     *  The status of if the controller PEC was checked in the transaction that
     *  occurred before the last Stop. Latched on Stop.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Status of controller PEC checked in last transaction
     *              返回值：上次事务 PEC 校验状态。
     *
     *  @retval     One of @ref DL_I2C_CONTROLLER_PEC_STATUS
     */
    __STATIC_INLINE DL_I2C_CONTROLLER_PEC_STATUS
    DL_I2C_getControllerPECCheckedStatus(const I2C_Regs *i2c)
    {
        uint32_t status =
            i2c->MASTER.CONTROLLER_PECSR & I2C_CONTROLLER_PECSR_PECSTS_CHECK_MASK;

        return (DL_I2C_CONTROLLER_PEC_STATUS)(status);
    }

    /**
     *  @brief      Get the status of the controller SMBus/PMBus PEC Check error
     *              查询上次事务的控制器 PEC 校验错误状态。
     *
     *  The status of if a PEC check error occurred in the transaction that
     *  occurred before the last Stop. Latched on Stop.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Status of controller PEC check error
     *              返回值：PEC 校验错误状态。
     *
     *  @retval     One of @ref DL_I2C_CONTROLLER_PEC_CHECK_ERROR
     */
    __STATIC_INLINE DL_I2C_CONTROLLER_PEC_CHECK_ERROR
    DL_I2C_getControllerPECCheckError(const I2C_Regs *i2c)
    {
        uint32_t error =
            i2c->MASTER.CONTROLLER_PECSR & I2C_CONTROLLER_PECSR_PECSTS_ERROR_MASK;

        return (DL_I2C_CONTROLLER_PEC_CHECK_ERROR)(error);
    }

    /**
     *  @brief      Disable I2C controller burst mode
     *              禁用控制器突发传输模式。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableControllerBurst(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR &= ~(I2C_MCTR_BURSTRUN_MASK);
    }

    /**
     *  @brief      Checks if I2C controller burst mode is enabled
     *              检查控制器突发传输模式是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If I2C controller burst mode is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if I2C controller burst mode is enabled
     *  @retval     false if I2C controller burst mode is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerBurstEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->GFCTL & I2C_MCTR_BURSTRUN_MASK) == I2C_MCTR_BURSTRUN_ENABLE);
    }

    /**
     *  @brief      Enable I2C controller burst mode
     *              启用控制器突发传输模式。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableControllerBurst(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR |= I2C_MCTR_BURSTRUN_ENABLE;
    }

    /**
     *  @brief      Disable I2C START generation
     *              禁用 I2C START 条件生成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableStartCondition(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR &= ~(I2C_MCTR_START_MASK);
    }

    /**
     *  @brief      Checks if I2C START generation is enabled
     *              检查 I2C START 条件生成是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If I2C START generation is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if I2C START generation is enabled
     *  @retval     false if I2C START generation is disabled
     */
    __STATIC_INLINE bool DL_I2C_isStartConditionEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCTR & I2C_MCTR_START_MASK) == I2C_MCTR_START_ENABLE);
    }

    /**
     *  @brief      Enable I2C START generation
     *              启用 I2C START 条件生成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableStartCondition(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR |= I2C_MCTR_START_ENABLE;
    }

    /**
     *  @brief      Disable I2C STOP generation
     *              禁用 I2C STOP 条件生成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableStopCondition(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR &= ~(I2C_MCTR_STOP_MASK);
    }

    /**
     *  @brief      Checks if I2C STOP generation is enabled
     *              检查 I2C STOP 条件生成是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If I2C STOP generation is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if I2C STOP generation is enabled
     *  @retval     false if I2C STOP generation is disabled
     */
    __STATIC_INLINE bool DL_I2C_isStopConditionEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCTR & I2C_MCTR_STOP_MASK) == I2C_MCTR_STOP_ENABLE);
    }

    /**
     *  @brief      Enable I2C STOP generation
     *              启用 I2C STOP 条件生成。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableStopCondition(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR |= I2C_MCTR_STOP_ENABLE;
    }

    /**
     *  @brief      Disable I2C controller data acknowledge (ACK or NACK)
     *              禁用控制器数据应答（ACK），收到数据后不自动应答。
     *
     *  When the I2C module operates in Controller receiver mode, the ACK bit can be
     *  cleared when no further data needs to be received from the target
     *  transmitter.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableControllerACK(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR &= ~(I2C_MCTR_ACK_MASK);
    }

    /**
     *  @brief      Checks if I2C controller data acknowledge (ACK or NACK) is
     *              enabled
     *              检查控制器数据应答是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If I2C controller data acknowledge is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if I2C controller data acknowledge is enabled
     *  @retval     false if I2C controller data acknowledge is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerACKEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCTR & I2C_MCTR_ACK_MASK) == I2C_MCTR_ACK_ENABLE);
    }

    /**
     *  @brief      Enable I2C controller data acknowledge (ACK or NACK)
     *              启用控制器数据应答（ACK），收到数据后自动应答。
     *
     *  When the I2C module operates in Controller receiver mode, the ACK bit is
     *  normally set causing the I2C bus controller to transmit an acknowledge
     *  automatically after each byte.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableControllerACK(I2C_Regs *i2c)
    {
        i2c->MASTER.MCTR |= I2C_MCTR_ACK_MASK;
    }

    /**
     *  @brief      Get transaction length in bytes
     *              获取当前事务长度（字节数）。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Length of I2C transaction in bytes
     *              返回值：事务长度 [0x000, 0xfff]。
     *
     *  @retval     [0x000, 0xfff]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTransactionLength(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCTR & I2C_MCTR_MBLEN_MASK) >> I2C_MCTR_MBLEN_OFS);
    }

    /**
     *  @brief      Set transaction length in bytes
     *              设置事务长度（字节数）。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @param[in]  length      Length of I2C transaction in bytes [0x000, 0xfff]
     *                          事务长度。
     */
    __STATIC_INLINE void DL_I2C_setTransactionLength(
        I2C_Regs *i2c, uint32_t length)
    {
        DL_Common_updateReg(&i2c->MASTER.MCTR, (length << I2C_MCTR_MBLEN_OFS),
                            I2C_MCTR_MBLEN_MASK);
    }

    /**
     *  @brief      Get status of I2C bus controller for controller
     *              获取控制器状态寄存器值。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Status of I2C bus controller for controller
     *              返回值：控制器状态，@ref DL_I2C_CONTROLLER_STATUS 的按位或。
     *
     *  @retval     Bitwise OR of @ref DL_I2C_CONTROLLER_STATUS
     */
    __STATIC_INLINE uint32_t DL_I2C_getControllerStatus(const I2C_Regs *i2c)
    {
        return (i2c->MASTER.MSR);
    }

    /**
     *  @brief      Get transaction count in bytes
     *              获取事务剩余字节倒计数值。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Current 12-bit countdown value of the transaction
     *              返回值：12 位倒计数值 [0x000, 0xfff]。
     *
     *  @retval     [0x000, 0xfff]
     */
    __STATIC_INLINE uint16_t DL_I2C_getTransactionCount(const I2C_Regs *i2c)
    {
        return ((uint16_t)((i2c->MASTER.MSR & I2C_MSR_MBCNT_MASK) >> I2C_MSR_MBCNT_OFS));
    }

    /**
     *  @brief      Get byte of data from I2C controller
     *              从控制器 RX FIFO 读取 1 字节数据。
     *
     *  If using FIFO, it is first byte from the RX FIFO.
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Last received byte of data
     *              返回值：接收到的数据字节 [0x00, 0xff]。
     *
     *  @retval     [0x00, 0xff]
     */
    __STATIC_INLINE uint8_t DL_I2C_receiveControllerData(const I2C_Regs *i2c)
    {
        return ((uint8_t)(i2c->MASTER.MRXDATA & I2C_MRXDATA_VALUE_MASK));
    }

    /**
     *  @brief      Set next byte to be transferred during the next transaction
     *              写入控制器 TX 数据寄存器，待启动传输后发送。
     *
     *  Does not transmit data until @ref DL_I2C_startControllerTransfer
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  data    Data to be transferred during the next transaction.
     *                      [0x00, 0xff]
     *                      待发送数据字节。
     */
    __STATIC_INLINE void DL_I2C_transmitControllerData(I2C_Regs *i2c, uint8_t data)
    {
        i2c->MASTER.MTXDATA = data;
    }

    /**
     *  @brief      Get timer period
     *              获取 SCL 定时器周期值，用于配置 I2C 时钟速率。
     *  This field is used in the equation to configure SCL_PERIOD:
     *
     *  SCL_PERIOD = (1 + TPR) * (SCL_LP + SCL_HP) * INT_CLK_PRD
     *
     *  where:
     *  SCL_PRD is the SCL line period (I2C clock)
     *
     *  TPR is the Timer Period register value (range of 1 to 127)
     *
     *  SCL_LP is the SCL Low period (fixed at 6)
     *  SCL_HP is the SCL High period (fixed at 4)
     *
     *  CLK_PRD is the functional clock period in ns
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     Timer period
     *              返回值：定时器周期 [0x00, 0x7f]。
     *
     *  @retval     [0x00, 0x7f]
     */
    __STATIC_INLINE uint8_t DL_I2C_getTimerPeriod(const I2C_Regs *i2c)
    {
        return ((uint8_t)(i2c->MASTER.MTPR & I2C_MTPR_TPR_MASK));
    }

    /**
     *  @brief      Set timer period
     *              设置 SCL 定时器周期值，用于配置 I2C 时钟速率。
     *  This field is used in the equation to configure SCL_PERIOD:
     *
     *  SCL_PERIOD = (1 + TPR) * (SCL_LP + SCL_HP) * INT_CLK_PRD
     *
     *  where:
     *  SCL_PRD is the SCL line period (I2C clock)
     *
     *  TPR is the Timer Period register value (range of 1 to 127)
     *
     *  SCL_LP is the SCL Low period (fixed at 6)
     *  SCL_HP is the SCL High period (fixed at 4)
     *
     *  CLK_PRD is the functional clock period in ns
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  period  Timer period [0x00, 0x7f]
     *                      定时器周期值。
     */
    __STATIC_INLINE void DL_I2C_setTimerPeriod(I2C_Regs *i2c, uint8_t period)
    {
        i2c->MASTER.MTPR = period;
    }

    /**
     *  @brief      Disable loopback mode
     *              禁用回环测试模式。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableLoopbackMode(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR &= ~(I2C_MCR_LPBK_MASK);
    }

    /**
     *  @brief      Checks if loopback mode is enabled
     *              检查回环测试模式是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If loopback mode is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if loopback mode is enabled
     *  @retval     false if loopback mode is disabled
     */
    __STATIC_INLINE bool DL_I2C_isLoopbackModeEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCR & I2C_MCR_LPBK_MASK) == I2C_MCR_LPBK_ENABLE);
    }

    /**
     *  @brief      Enable loopback mode
     *              启用回环测试模式。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableLoopbackMode(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR |= I2C_MCR_LPBK_ENABLE;
    }

    /**
     *  @brief      Disable multicontroller mode
     *              禁用多控制器模式。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableMultiControllerMode(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR &= ~(I2C_MCR_MMST_MASK);
    }

    /**
     *  @brief      Checks if multicontroller mode is enabled
     *              检查多控制器模式是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If multicontroller mode is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if multicontroller mode is enabled
     *  @retval     false if multicontroller mode is disabled
     */
    __STATIC_INLINE bool DL_I2C_isMultiControllerModeEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCR & I2C_MCR_MMST_MASK) == I2C_MCR_MMST_ENABLE);
    }

    /**
     *  @brief      Enable multicontroller mode
     *              启用多控制器模式，SCL 高电平计时从检测到 SCL 高电平开始。
     *
     *  In multicontroller mode, the SCL high time counts once the SCL line has been
     *  detected high. If this is not enabled, the high time counts as soon as the
     *  SCL line has been set high by the I2C controller
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableMultiControllerMode(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR |= I2C_MCR_MMST_ENABLE;
    }

    /**
     *  @brief      Disable controller
     *              禁用控制器。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableController(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR &= ~(I2C_MCR_ACTIVE_MASK);
    }

    /**
     *  @brief      Checks if controller is enabled
     *              检查控制器是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if controller is enabled
     *  @retval     false if controller is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCR & I2C_MCR_ACTIVE_MASK) == I2C_MCR_ACTIVE_ENABLE);
    }

    /**
     *  @brief      Enable controller
     *              启用控制器。启用后需先禁用或复位才能重新启用，否则可能导致传输失败。
     *
     *  After controller has enabled, it should not be re-enabled unless it has been
     *  disabled or by a reset, otherwise transfer failures may occur.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableController(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR |= I2C_MCR_ACTIVE_ENABLE;
    }

    /**
     *  @brief      Disable controller clock stretching
     *              禁用控制器时钟延展。
     *
     *  Clock stretching can be disabled if no target on the bus supports clock
     *  stretching; however, it should be typically enabled to be compliant with
     *  I2C specification.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableControllerClockStretching(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR &= ~(I2C_MCR_CLKSTRETCH_MASK);
    }

    /**
     *  @brief      Checks if controller clock stretching is enabled
     *              检查控制器时钟延展是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller clock stretching is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if controller clock stretching is enabled
     *  @retval     false if controller clock stretching is disabled
     */
    __STATIC_INLINE bool DL_I2C_isControllerClockStretchingEnabled(
        const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MCR & I2C_MCR_CLKSTRETCH_MASK) ==
                I2C_MCR_CLKSTRETCH_ENABLE);
    }

    /**
     *  @brief      Enable controller clock stretching
     *              启用控制器时钟延展。
     *
     *  Clock stretching can be disabled if no target on the bus supports clock
     *  stretching; however, it should be typically enabled to be compliant with I2C
     *  specification.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableControllerClockStretching(I2C_Regs *i2c)
    {
        i2c->MASTER.MCR |= I2C_MCR_CLKSTRETCH_ENABLE;
    }

    /**
     *  @brief      Get SCL signal status
     *              获取 SCL 信号电平状态。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Status of SCL signal
     *              返回值：SCL 信号状态。
     *
     *  @retval     One of @ref DL_I2C_CONTROLLER_SCL
     */
    __STATIC_INLINE DL_I2C_CONTROLLER_SCL DL_I2C_getSCLStatus(const I2C_Regs *i2c)
    {
        uint32_t sclStatus = i2c->MASTER.MBMON & I2C_MBMON_SCL_MASK;

        return (DL_I2C_CONTROLLER_SCL)(sclStatus);
    }

    /**
     *  @brief      Get SDA signal status
     *              获取 SDA 信号电平状态。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Status of SDA signal
     *              返回值：SDA 信号状态。
     *
     *  @retval     One of @ref DL_I2C_CONTROLLER_SDA
     */
    __STATIC_INLINE DL_I2C_CONTROLLER_SDA DL_I2C_getSDAStatus(const I2C_Regs *i2c)
    {
        uint32_t sdaStatus = i2c->MASTER.MBMON & I2C_MBMON_SDA_MASK;

        return (DL_I2C_CONTROLLER_SDA)(sdaStatus);
    }

    /**
     *  @brief      Get controller TX FIFO threshold level
     *              获取控制器 TX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     Indicates at what fill level in the TX FIFO a threshold will be
     *              generated
     *              返回值：TX FIFO 触发阈值。
     *
     *  @retval     One of @ref DL_I2C_TX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2C_TX_FIFO_LEVEL DL_I2C_getControllerTXFIFOThreshold(
        const I2C_Regs *i2c)
    {
        uint32_t level = i2c->MASTER.MFIFOCTL & I2C_MFIFOCTL_TXTRIG_MASK;

        return (DL_I2C_TX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set controller TX FIFO threshold level
     *              设置控制器 TX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  level   Indicates at what fill level in the TX FIFO a threshold
     *                      will be generated.
     *                      One of @ref DL_I2C_TX_FIFO_LEVEL.
     *                      TX FIFO 触发阈值。
     */
    __STATIC_INLINE void DL_I2C_setControllerTXFIFOThreshold(
        I2C_Regs *i2c, DL_I2C_TX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(
            &i2c->MASTER.MFIFOCTL, (uint32_t)level, I2C_MFIFOCTL_TXTRIG_MASK);
    }

    /**
     *  @brief      Stop controller TX FIFO flush
     *              停止控制器 TX FIFO 刷新。
     *
     *  Before stopping the flush, check if @ref DL_I2C_isControllerTXFIFOEmpty,
     *  indicating flush is complete.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_stopFlushControllerTXFIFO(I2C_Regs *i2c)
    {
        i2c->MASTER.MFIFOCTL &= ~(I2C_MFIFOCTL_TXFLUSH_MASK);
    }

    /**
     *  @brief      Start controller TX FIFO flush
     *              启动控制器 TX FIFO 刷新。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_startFlushControllerTXFIFO(I2C_Regs *i2c)
    {
        i2c->MASTER.MFIFOCTL |= I2C_MFIFOCTL_TXFLUSH_MASK;
    }

    /**
     *  @brief      Get controller RX FIFO threshold level
     *              获取控制器 RX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     Indicates at what fill level in the RX FIFO a threshold will be
     *              generated
     *              返回值：RX FIFO 触发阈值。
     *
     *  @retval     One of @ref DL_I2C_RX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2C_RX_FIFO_LEVEL DL_I2C_getControllerRXFIFOThreshold(
        const I2C_Regs *i2c)
    {
        uint32_t level = i2c->MASTER.MFIFOCTL & I2C_MFIFOCTL_RXTRIG_MASK;

        return (DL_I2C_RX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set controller RX FIFO threshold level
     *              设置控制器 RX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  level   Indicates at what fill level in the RX FIFO a threshold
     *                      will be generated.
     *                      One of @ref DL_I2C_RX_FIFO_LEVEL.
     *                      RX FIFO 触发阈值。
     */
    __STATIC_INLINE void DL_I2C_setControllerRXFIFOThreshold(
        I2C_Regs *i2c, DL_I2C_RX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(
            &i2c->MASTER.MFIFOCTL, (uint32_t)level, I2C_MFIFOCTL_RXTRIG_MASK);
    }

    /**
     *  @brief      Stop controller RX FIFO flush
     *              停止控制器 RX FIFO 刷新。
     *
     *  Before stopping the flush, check if @ref DL_I2C_isControllerRXFIFOEmpty,
     *  indicating flush is complete.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_stopFlushControllerRXFIFO(I2C_Regs *i2c)
    {
        i2c->MASTER.MFIFOCTL &= ~(I2C_MFIFOCTL_RXFLUSH_MASK);
    }

    /**
     *  @brief      Start controller RX FIFO flush
     *              启动控制器 RX FIFO 刷新。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_startFlushControllerRXFIFO(I2C_Regs *i2c)
    {
        i2c->MASTER.MFIFOCTL |= I2C_MFIFOCTL_RXFLUSH_MASK;
    }

    /**
     *  @brief      Get number of bytes which can be read from RX FIFO
     *              获取控制器 RX FIFO 中可读取的字节数。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Number of bytes which can be read from RX FIFO
     *              返回值：可读字节数 [0x0, 0x8]。
     *
     *  @retval     [0x0, 0x8]
     */
    __STATIC_INLINE uint32_t DL_I2C_getControllerRXFIFOCounter(const I2C_Regs *i2c)
    {
        return (i2c->MASTER.MFIFOSR & I2C_MFIFOSR_RXFIFOCNT_MASK);
    }

    /**
     *  @brief      Get number of bytes which can be put into TX FIFO
     *              获取控制器 TX FIFO 中可写入的字节数。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Number of bytes which can be put into TX FIFO
     *              返回值：可写字节数 [0x0, 0x8]。
     *
     *  @retval     [0x0, 0x8]
     */
    __STATIC_INLINE uint32_t DL_I2C_getControllerTXFIFOCounter(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK) >>
                I2C_MFIFOSR_TXFIFOCNT_OFS);
    }

    /**
     *  @brief      Checks if controller RX FIFO flush is active
     *              检查控制器 RX FIFO 刷新是否正在进行。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller RX FIFO flush is active
     *              返回值：`true` 表示刷新中，`false` 表示未刷新。
     *
     *  @retval     true if controller RX FIFO flush is active
     *  @retval     false if controller RX FIFO flush is not active
     */
    __STATIC_INLINE bool DL_I2C_isControllerRXFIFOFlushActive(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MFIFOSR & I2C_MFIFOSR_RXFLUSH_MASK) ==
                I2C_MFIFOSR_RXFLUSH_ACTIVE);
    }

    /**
     *  @brief      Checks if controller TX FIFO flush is active
     *              检查控制器 TX FIFO 刷新是否正在进行。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If controller TX FIFO flush is active
     *              返回值：`true` 表示刷新中，`false` 表示未刷新。
     *
     *  @retval     true if controller TX FIFO flush is active
     *  @retval     false if controller TX FIFO flush is not active
     */
    __STATIC_INLINE bool DL_I2C_isControllerTXFIFOFlushActive(const I2C_Regs *i2c)
    {
        return ((i2c->MASTER.MFIFOSR & I2C_MFIFOSR_TXFLUSH_MASK) ==
                I2C_MFIFOSR_TXFLUSH_ACTIVE);
    }

    /**
     *  @brief      Set target own address
     *              设置目标自身地址。
     *
     * This field specifies bits A9 through A0 of the target own address.
     *
     * In 7-bit addressing mode as selected by @ref DL_I2C_setTargetAddressingMode,
     * the top 3 bits are don't care
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  addr    Value of target own address [0x00, 0x3FF]
     *                      目标地址值。
     */
    __STATIC_INLINE void DL_I2C_setTargetOwnAddress(I2C_Regs *i2c, uint32_t addr)
    {
        DL_Common_updateReg(&i2c->SLAVE.SOAR, addr, I2C_SOAR_OAR_MASK);
    }

    /**
     *  @brief      Get target own address
     *              获取目标自身地址。
     *
     * Get bits A9 through A0 of the target own address.
     *
     * In 7-bit addressing mode as selected by @ref DL_I2C_setTargetAddressingMode,
     * the top 3 bits are don't care
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     Target own address
     *              返回值：目标地址 [0x00, 0x3FF]。
     *
     *  @retval     [0x00, 0x3FF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetOwnAddress(const I2C_Regs *i2c)
    {
        return (i2c->SLAVE.SOAR & I2C_SOAR_OAR_MASK);
    }

    /**
     *  @brief      Enable target own address
     *              启用目标自身地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa DL_I2C_setTargetOwnAddress
     */
    __STATIC_INLINE void DL_I2C_enableTargetOwnAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SOAR |= I2C_SOAR_OAREN_ENABLE;
    }

    /**
     *  @brief      Disable target own address
     *              禁用目标自身地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetOwnAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SOAR &= ~(I2C_SOAR_OAREN_MASK);
    }

    /**
     *  @brief      Checks if target own address is enabled
     *              检查目标自身地址是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target own address is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target own address is enabled
     *  @retval     false if target own address is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetOwnAddressEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SOAR & I2C_SOAR_OAREN_MASK) == I2C_SOAR_OAREN_ENABLE);
    }

    /**
     *  @brief      Set target addressing mode
     *              设置目标寻址模式（7 位或 10 位）。
     *
     *  Selects the addressing mode to be used when device is configured as a
     *  target
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  mode    Addressing mode to set the target to.
     *                      One of @ref DL_I2C_TARGET_ADDRESSING_MODE.
     *                      寻址模式。
     */
    __STATIC_INLINE void DL_I2C_setTargetAddressingMode(
        I2C_Regs *i2c, DL_I2C_TARGET_ADDRESSING_MODE mode)
    {
        DL_Common_updateReg(
            &i2c->SLAVE.SOAR, (uint32_t)mode, I2C_SOAR_SMODE_MASK);
    }

    /**
     *  @brief      Get target addressing mode
     *              获取目标寻址模式。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Addressing mode the target is set to
     *              返回值：当前目标寻址模式。
     *
     *  @retval     One of @ref DL_I2C_TARGET_ADDRESSING_MODE
     */
    __STATIC_INLINE DL_I2C_TARGET_ADDRESSING_MODE DL_I2C_getTargetAddressingMode(
        const I2C_Regs *i2c)
    {
        uint32_t mode = i2c->SLAVE.SOAR & I2C_SOAR_SMODE_MASK;

        return (DL_I2C_TARGET_ADDRESSING_MODE)(mode);
    }

    /**
     *  @brief      Get target own address alternate
     *              获取目标备用自身地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     Target own address alternate
     *              返回值：备用地址 [0x00, 0x7f]。
     *
     *  @retval     [0x00, 0x7f]
     */
    __STATIC_INLINE uint32_t I2C_getTargetOwnAddressAlternate(const I2C_Regs *i2c)
    {
        return (i2c->SLAVE.SOAR2 & I2C_SOAR2_OAR2_MASK);
    }

    /**
     *  @brief      Set target own address alternate
     *              设置目标备用自身地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  addr    Value of target own address alternate [0x00, 0x7f]
     *                      备用地址值。
     */
    __STATIC_INLINE void DL_I2C_setTargetOwnAddressAlternate(
        I2C_Regs *i2c, uint32_t addr)
    {
        DL_Common_updateReg(&i2c->SLAVE.SOAR2, addr, I2C_SOAR2_OAR2_MASK);
    }

    /**
     *  @brief      Get target own address alternate mask
     *              获取目标备用地址掩码。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Which bits of the target address are matched
     *              返回值：地址匹配位掩码 [0x00, 0x7F]。
     *
     *  @retval     Bit mask with each bit corresponding to bits A6 through A0 of
     *              the target address. Value between [0x00, 0x7F]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetOwnAddressAlternateMask(
        const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SOAR2 & I2C_SOAR2_OAR2_MASK_MASK) >>
                I2C_SOAR2_OAR2_MASK_OFS);
    }

    /**
     *  @brief      Set target own address alternate mask
     *              设置目标备用地址掩码。掩码为 1 的位将忽略对应地址位。
     *
     * This field specifies bits A6 through A0 of the target address. The bits with
     * a value of 1 in the SOAR2.OAR2_MASK field will make the corresponding
     * incoming address bits to match by default regardless of the value inside
     * SOAR2.OAR2 i.e. corresponding SOAR2.OAR2 bit is a don't care.
     *
     *  @param[in]  i2c           Pointer to the register overlay for the peripheral
     *                            I2C 外设寄存器基址。
     *  @param[in]  addressMask   Bit mask of address bits to match.
     *                            Value between [0x00, 0x7F]
     *                            地址匹配位掩码。
     */
    __STATIC_INLINE void DL_I2C_setTargetOwnAddressAlternateMask(
        I2C_Regs *i2c, uint32_t addressMask)
    {
        DL_Common_updateReg(&i2c->SLAVE.SOAR2,
                            addressMask << I2C_SOAR2_OAR2_MASK_OFS, I2C_SOAR2_OAR2_MASK_MASK);
    }

    /**
     *  @brief      Disable usage of target own address alternate
     *              禁用目标备用自身地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetOwnAddressAlternate(I2C_Regs *i2c)
    {
        i2c->SLAVE.SOAR2 &= ~(I2C_SOAR2_OAR2EN_MASK);
    }

    /**
     *  @brief      Checks if target own address alternate is enabled
     *              检查目标备用自身地址是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target own address alternate is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target own address alternate is enabled
     *  @retval     false if target own address alternate is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetOwnAddressAlternateEnabled(
        const I2C_Regs *i2c)
    {
        return (
            (i2c->SLAVE.SOAR2 & I2C_SOAR2_OAR2EN_MASK) == I2C_SOAR2_OAR2EN_ENABLE);
    }

    /**
     *  @brief      Enable usage of target own address alternate
     *              启用目标备用自身地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetOwnAddressAlternate(I2C_Regs *i2c)
    {
        i2c->SLAVE.SOAR2 |= I2C_SOAR2_OAR2EN_ENABLE;
    }

    /**
     *  @brief      Get the address for which address match happened
     *              获取地址匹配到的目标地址。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Matched Target address
     *              返回值：匹配到的地址。
     *
     *  @retval     Bit mask with each bit corresponding to the target address.
     *  Value between [0x00, 0x7F] in 7-bit mode, or [0x00, 0x3FF] in 10-bit mode.
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetAddressMatch(const I2C_Regs *i2c)
    {
        return (
            (i2c->SLAVE.SSR & I2C_SSR_ADDRMATCH_MASK) >> I2C_SSR_ADDRMATCH_OFS);
    }

    /**
     *  @brief      Disable target clock stretching
     *              禁用目标时钟延展。
     *
     *  Clock stretching should be enabled to be compliant with I2C specification
     *  and the SCL will be stretched when data must be read or written from the
     *  FIFO. It can optionally be disabled if a controller does not support it or
     *  to achieve maximum I2C frequency; however the developer should ensure that
     *  the FIFO is written or read on time.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetClockStretching(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_SCLKSTRETCH_MASK);
    }

    /**
     *  @brief      Checks if target clock stretching is enabled
     *              检查目标时钟延展是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target clock stretching is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target clock stretching is enabled
     *  @retval     false if target clock stretching is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetClockStretchingEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_SCLKSTRETCH_MASK) ==
                I2C_SCTR_SCLKSTRETCH_ENABLE);
    }

    /**
     *  @brief      Enable target clock stretching
     *              启用目标时钟延展。
     *
     *  Clock stretching should be enabled to be compliant with I2C specification
     *  and the SCL will be stretched when data must be read or written from the
     *  FIFO. It can optionally be disabled if a controller does not support it or
     *  to achieve maximum I2C frequency; however the developer should ensure that
     *  the FIFO is written or read on time.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetClockStretching(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_SCLKSTRETCH_ENABLE;
    }

    /**
     *  @brief      Disable target TX empty interrupt on transmit request
     *              禁用目标发送请求时的 TX FIFO 空中断。
     *
     * When disabled, RIS.STXEMPTY will be set when only the Target TX FIFO is
     * empty. This allows the STXEMPTY interrupt to be used to indicate that the
     * I2C bus is being clock stretched and that Target TX data is required.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa         DL_I2C_isTargetClockStretchEnabled
     */
    __STATIC_INLINE void DL_I2C_disableTargetTXEmptyOnTXRequest(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_TXEMPTY_ON_TREQ_MASK);
    }

    /**
     *  @brief      Checks if target TX empty interrupt on transmit request is
     *              enabled
     *              检查目标发送请求时 TX FIFO 空中断是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If TX empty interrupt on transmit request is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if TX empty interrupt on transmit request is enabled
     *  @retval     false if TX empty interrupt on transmit request disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetTXEmptyOnTXRequestEnabled(
        const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_TXEMPTY_ON_TREQ_MASK) ==
                I2C_SCTR_TXEMPTY_ON_TREQ_ENABLE);
    }

    /**
     *  @brief      Enable target TX empty interrupt on transmit request
     *              启用目标发送请求时的 TX FIFO 空中断。
     *
     *  When enabled, RIS.STXEMPTY will be set when the Target state machine is in
     *  the TX_WAIT state which occurs when the TX FIFO is empty AND the I2C
     *  transaction is clock stretched waiting for the FIFO to receive data.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetTXEmptyOnTXRequest(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_TXEMPTY_ON_TREQ_ENABLE;
    }

    /**
     *  @brief      Disable target TX trigger in TX mode
     *              禁用目标 TX 模式下的 TX FIFO 触发。
     *
     * When disabled, RIS.TXFIFOTRG will only be set when the Target TX FIFO is at
     * or above the trigger level.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetTXTriggerInTXMode(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_TXTRIG_TXMODE_MASK);
    }

    /**
     *  @brief      Checks if target TX trigger in TX mode is enabled
     *              检查目标 TX 模式下 TX FIFO 触发是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target TX trigger in TX mode is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target TX trigger in TX mode is enabled
     *  @retval     false if target TX trigger in TX mode is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetTXTriggerInTXModeEnabled(
        const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_TXTRIG_TXMODE_MASK) ==
                I2C_SCTR_TXTRIG_TXMODE_ENABLE);
    }

    /**
     *  @brief      Enable TX trigger when target is in TX mode
     *              启用目标 TX 模式下的 TX FIFO 触发。
     *
     *  When enabled, RIS.TXFIFOTRG will be set when the Target TX FIFO has reached
     *  the trigger level AND the Target state machine is in the TXMODE as defined
     *  in the SSR register. When cleared RIS.TXFIFOTRG will be set when the Target
     *  TX FIFO is at or above the trigger level. This setting can be used to hold
     *  off the TX DMA until a transaction starts. This allows the DMA to be
     *  configured when the I2C is idle but have it wait till the transaction
     *  starts to load the Target TX FIFO, so it can load from a memory buffer that
     *  might be changing over time.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa                 DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER
     */
    __STATIC_INLINE void DL_I2C_enableTargetTXTriggerInTXMode(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_TXTRIG_TXMODE_ENABLE;
    }

    /**
     *  @brief      Disable target TX transfer waits when stale data in TX FIFO
     *              禁用目标 TX FIFO 数据过期时的传输等待。
     *
     *  When disabled, the TX FIFO empty signal to the Target state machine
     *  indicates that the TX FIFO is empty.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetTXWaitWhenTXFIFOStale(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_TXWAIT_STALE_TXFIFO_MASK);
    }

    /**
     *  @brief      Checks if target TX transfer waits when stale data in TX FIFO
     *              is enabled
     *              检查目标 TX FIFO 数据过期等待是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target TX transfer waits when stale data in TX FIFO is
     *              enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true   if target TX transfer waits when stale data in TX FIFO
     *                     is enabled
     *  @retval     false  if target TX transfer waits when stale data in TX FIFO
     *                     is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetTXWaitWhenTXFIFOStaleEnabled(
        const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_TXWAIT_STALE_TXFIFO_MASK) ==
                I2C_SCTR_TXWAIT_STALE_TXFIFO_ENABLE);
    }

    /**
     *  @brief      Enable target TX transfer waits when stale data in TX FIFO
     *              启用目标 TX FIFO 数据过期时的传输等待。
     *
     *  When enabled, the TX FIFO empty signal to the Target state machine will
     *  indicate that the TX FIFO is empty or that the TX FIFO data is stale. The
     *  TX FIFO data is determined to be stale when there is data in the TX FIFO
     *  when the Target state machine leaves the TXMODE as defined in the SSR
     *  register. This can occur is a Stop or timeout occur when there are bytes
     *  left in the TX FIFO.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetTXWaitWhenTXFIFOStale(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_TXWAIT_STALE_TXFIFO_ENABLE;
    }

    /**
     *  @brief      Disable target RX full interrupt on receive request
     *              禁用目标接收请求时的 RX FIFO 满中断。
     *
     *  When disabled, RIS.SRXFULL will be set when only the Target RX FIFO is
     *  full. This allows the SRXFULL interrupt to be used to indicate that the
     *  I2C bus is being clock stretched and that the FW must either read the RX
     *  FIFO or ACK/NACK the current RX byte.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa         DL_I2C_isTargetClockStretchEnabled
     */
    __STATIC_INLINE void DL_I2C_disableTargetRXFullOnRXRequest(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_RXFULL_ON_RREQ_MASK);
    }

    /**
     *  @brief      Checks if target RX full interrupt on receive request is enabled
     *              检查目标接收请求时 RX FIFO 满中断是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target RX full interrupt on receive request enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true   if target RX full interrupt on receive request enabled
     *  @retval     false  if target RX full interrupt on receive request disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetRXFullOnRXRequestEnabled(
        const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_RXFULL_ON_RREQ_MASK) ==
                I2C_SCTR_RXFULL_ON_RREQ_ENABLE);
    }

    /**
     *  @brief      Enable target RX full interrupt on receive request
     *              启用目标接收请求时的 RX FIFO 满中断。
     *
     *  When enabled, RIS.SRXFULL will be set when the Target state machine is in
     *  the RX_WAIT or RX_ACK_WAIT states which occurs when the I2C transaction is
     *  clock stretched because the RX FIFO is full or the ACKOEN has been set and
     *  the state machine is waiting for FW to ACK/NACK the current byte.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetRXFullOnRXRequest(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_RXFULL_ON_RREQ_ENABLE;
    }

    /**
     *  @brief      Disable SMBus/PMBus default host address of 000 1000b
     *              禁用 SMBus/PMBus 默认主机地址 0x08。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableDefaultHostAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_EN_DEFHOSTADR_MASK);
    }

    /**
     *  @brief      Checks if SMBus/PMBus default host address of 000 1000b is
     *              enabled
     *              检查 SMBus/PMBus 默认主机地址是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If default host address is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true   if default host address is enabled
     *  @retval     false  if default host address disabled
     */
    __STATIC_INLINE bool DL_I2C_isDefaultHostAddressEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_EN_DEFHOSTADR_MASK) ==
                I2C_SCTR_EN_DEFHOSTADR_ENABLE);
    }

    /**
     *  @brief      Enable SMBus/PMBus default host address of 000 1000b
     *              启用 SMBus/PMBus 默认主机地址 0x08。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableDefaultHostAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_EN_DEFHOSTADR_ENABLE;
    }

    /**
     *  @brief      Disable SMBus/PMBus Alert response address (ARA) of 000 1100b
     *              禁用 SMBus/PMBus 告警响应地址 0x0C。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableAlertResponseAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_EN_ALRESPADR_MASK);
    }

    /**
     *  @brief      Checks if SMBus/PMBus Alert response address (ARA) of 000 1100b
     *              is enabled
     *              检查 SMBus/PMBus 告警响应地址是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If alert response address is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true   if alert response address is enabled
     *  @retval     false  if alert response address disabled
     */
    __STATIC_INLINE bool DL_I2C_isAlertResponseAddressEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_EN_ALRESPADR_MASK) ==
                I2C_SCTR_EN_ALRESPADR_ENABLE);
    }

    /**
     *  @brief      Enable SMBus/PMBus Alert response address (ARA) of 000 1100b
     *              启用 SMBus/PMBus 告警响应地址 0x0C。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableAlertResponseAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_EN_ALRESPADR_ENABLE;
    }

    /**
     *  @brief      Disable SMBus/PMBus default device address of 110 0001b
     *              禁用 SMBus/PMBus 默认设备地址 0x61（ARP 用）。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableDefaultDeviceAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_EN_DEFDEVADR_MASK);
    }

    /**
     *  @brief      Checks SMBus/PMBus default device address of 110 0001b is
     *              enabled
     *              检查 SMBus/PMBus 默认设备地址是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If default device address is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true   if default device address is enabled
     *  @retval     false  if default device address disabled
     */
    __STATIC_INLINE bool DL_I2C_isDefaultDeviceAddressEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_EN_DEFDEVADR_MASK) ==
                I2C_SCTR_EN_DEFDEVADR_ENABLE);
    }

    /**
     *  @brief      Enable SMBus/PMBus default device address of 110 0001b
     *              启用 SMBus/PMBus 默认设备地址 0x61（ARP 用）。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableDefaultDeviceAddress(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_EN_DEFDEVADR_ENABLE;
    }

    /**
     *  @brief      Disable target wakeup
     *              禁用目标唤醒功能。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetWakeup(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_SWUEN_MASK);
    }

    /**
     *  @brief      Checks if target wakeup is enabled
     *              检查目标唤醒功能是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target wakeup is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true   if target wakeup is enabled
     *  @retval     false  if target wakeup disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetWakeupEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SCTR & I2C_SCTR_SWUEN_MASK) == I2C_SCTR_SWUEN_ENABLE);
    }

    /**
     *  @brief      Enable target wakeup
     *              启用目标唤醒功能，建议同时启用时钟延展以等待模块唤醒。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa         DL_I2C_enableTargetClockStretching
     */
    __STATIC_INLINE void DL_I2C_enableTargetWakeup(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_SWUEN_ENABLE;
    }

    /**
     *  @brief      Disable target functionality
     *              禁用目标功能。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTarget(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_ACTIVE_MASK);
    }

    /**
     *  @brief      Checks if target functionality is enabled
     *              检查目标功能是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target functionality is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target functionality is enabled
     *  @retval     false if target functionality is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetEnabled(const I2C_Regs *i2c)
    {
        return (
            (i2c->SLAVE.SCTR & I2C_SCTR_ACTIVE_MASK) == I2C_SCTR_ACTIVE_ENABLE);
    }

    /**
     *  @brief      Enable usage of target functionality
     *              启用目标功能。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTarget(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_ACTIVE_ENABLE;
    }

    /**
     *  @brief      Disable general call address of 000 0000b
     *              禁用通用呼叫地址 0x00。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableGeneralCall(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR &= ~(I2C_SCTR_GENCALL_MASK);
    }

    /**
     *  @brief      Checks if general call address of 000 0000b is enabled
     *              检查通用呼叫地址 0x00 是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If general call is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if general call is enabled
     *  @retval     false if general call is disabled
     */
    __STATIC_INLINE bool DL_I2C_isGeneralCallEnabled(const I2C_Regs *i2c)
    {
        return (
            (i2c->SLAVE.SCTR & I2C_SCTR_GENCALL_MASK) == I2C_SCTR_GENCALL_ENABLE);
    }

    /**
     *  @brief      Enable usage of general call address of 000 0000b
     *              启用通用呼叫地址 0x00。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableGeneralCall(I2C_Regs *i2c)
    {
        i2c->SLAVE.SCTR |= I2C_SCTR_GENCALL_ENABLE;
    }

    /**
     *  @brief      Get status of I2C bus controller for target
     *              获取目标状态寄存器值。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Status of I2C bus controller for target
     *              返回值：目标状态，@ref DL_I2C_TARGET_STATUS 的按位或。
     *
     *  @retval     Bitwise OR of @ref DL_I2C_TARGET_STATUS
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetStatus(const I2C_Regs *i2c)
    {
        return (i2c->SLAVE.SSR);
    }

    /**
     *  @brief      Get byte of data from I2C target
     *              从目标 RX 数据寄存器读取 1 字节。
     *
     *  If using FIFO, it is first byte from the RX FIFO.
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *
     *  @return     Last received byte of data
     *              返回值：接收到的数据字节 [0x00, 0xff]。
     *
     *  @retval     [0x00, 0xff]
     */
    __STATIC_INLINE uint8_t DL_I2C_receiveTargetData(const I2C_Regs *i2c)
    {
        return (uint8_t)(i2c->SLAVE.SRXDATA & I2C_SRXDATA_VALUE_MASK);
    }

    /**
     *  @brief      Set next byte to be transferred during the next transaction
     *              写入目标 TX 数据寄存器，待控制器读取时发送。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  data    Byte of data to be transferred during the next
     *                      transaction. [0x00, 0xff]
     *                      待发送数据字节。
     */
    __STATIC_INLINE void DL_I2C_transmitTargetData(I2C_Regs *i2c, uint8_t data)
    {
        i2c->SLAVE.STXDATA = data;
    }

    /**
     *  @brief      Disable target ACK override
     *              禁用目标 ACK 覆盖，恢复自动应答。
     *
     *  Disable manual ACK override to automatically ACK all received bytes until
     *  the RX FIFO is full.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetACKOverride(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL &= ~(I2C_SACKCTL_ACKOEN_MASK);
    }

    /**
     *  @brief      Checks if target ACK override is enabled
     *              检查目标 ACK 覆盖是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target ACK override is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target ACK override is enabled
     *  @retval     false if target ACK override is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetACKOverrideEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SACKCTL & I2C_SACKCTL_ACKOEN_MASK) ==
                I2C_SACKCTL_ACKOEN_ENABLE);
    }

    /**
     *  @brief      Enable target ACK override
     *              启用目标 ACK 覆盖，SCL 将被拉低直到软件写入应答值。
     *
     *  When manual ACK override is enabled, the I2C target SCL is pulled low after
     *  the last data is received until the ACK override value (through
     *  @ref DL_I2C_setTargetACKOverrideValue) is written.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetACKOverride(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL |= I2C_SACKCTL_ACKOEN_ENABLE;
    }

    /**
     *  @brief      Get target acknowledge override value
     *              获取目标应答覆盖值（ACK 或 NACK）。
     *
     *  @note for General Call this bit will be ignored if set to NACK and target
     *  continues to receive data.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     What type of response will be generated
     *              返回值：应答覆盖值。
     *
     *  @retval     One of @ref DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE.
     */
    __STATIC_INLINE DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE
    DL_I2C_getTargetACKOverrideValue(const I2C_Regs *i2c)
    {
        uint32_t value = i2c->SLAVE.SACKCTL & I2C_SACKCTL_ACKOVAL_MASK;

        return (DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE)(value);
    }

    /**
     *  @brief      Set target acknowledge override value
     *              设置目标应答覆盖值（ACK 或 NACK）。
     *
     *  @note for General Call this bit will be ignored if set to NACK and target
     *  continues to receive data.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  value   Indicates what type of response will be generated.
     *                      One of @ref DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE.
     *                      应答覆盖值。
     *
     *  @sa         DL_I2C_enableTargetACKOverride
     */
    __STATIC_INLINE void DL_I2C_setTargetACKOverrideValue(
        I2C_Regs *i2c, DL_I2C_TARGET_RESPONSE_OVERRIDE_VALUE value)
    {
        DL_Common_updateReg(
            &i2c->SLAVE.SACKCTL, (uint32_t)value, I2C_SACKCTL_ACKOVAL_MASK);
    }

    /**
     *  @brief      Disable target ACK override on Start Condition
     *              禁用目标 START 条件后的 ACK 覆盖。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableACKOverrideOnStart(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL &= ~(I2C_SACKCTL_ACKOEN_ON_START_MASK);
    }

    /**
     *  @brief      Checks if target ACK override on Start condition is enabled
     *              检查目标 START 条件后的 ACK 覆盖是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target ACK override on Start condition is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target ACK override on Start condition is enabled
     *  @retval     false if target ACK override on Start condition is disabled
     */
    __STATIC_INLINE bool DL_I2C_isACKOverrideOnStartEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SACKCTL & I2C_SACKCTL_ACKOEN_ON_START_MASK) ==
                I2C_SACKCTL_ACKOEN_ON_START_ENABLE);
    }

    /**
     *  @brief      Enable target ACK override on Start condition
     *              启用目标 START 条件后的 ACK 覆盖。
     *
     *  When enabled, this bit will automatically turn on the Target ACKOEN field
     *  following a Start condition.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa                 DL_I2C_enableTargetACKOverride
     */
    __STATIC_INLINE void DL_I2C_enableACKOverrideOnStart(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL |= I2C_SACKCTL_ACKOEN_ON_START_ENABLE;
    }

    /**
     *  @brief      Disable target ACK override when SMBus/PMBus PEC is next byte
     *              禁用 PEC 字节前的 ACK 覆盖。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableACKOverrideOnPECNext(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL &= ~(I2C_SACKCTL_ACKOEN_ON_PECNEXT_MASK);
    }

    /**
     *  @brief      Checks if target ACK override when SMBus/PMBus PEC is next byte
     *              is enabled
     *              检查 PEC 字节前的 ACK 覆盖是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target ACK override when PEC is next byte is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target ACK override when PEC is next byte is enabled
     *  @retval     false if target ACK override when PEC is next byte is disabled
     */
    __STATIC_INLINE bool DL_I2C_isACKOverrideOnPECNextEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SACKCTL & I2C_SACKCTL_ACKOEN_ON_PECNEXT_MASK) ==
                I2C_SACKCTL_ACKOEN_ON_PECNEXT_ENABLE);
    }

    /**
     *  @brief      Enable target ACK override when SMBus/PMBus PEC is next byte
     *              启用 PEC 字节前的 ACK 覆盖。
     *
     *  When enabled, this bit will automatically turn on the Target ACKOEN field
     *  following the ACK/NACK of the byte received just prior to the PEC byte.
     *  Note that when ACKOEN is set, the PEC byte will not automatically be
     *  ACKed/NACKed by the state machine and FW must perform this function by
     *  writing SLAVE_SACKCTL.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa                 DL_I2C_enableTargetACKOverride
     */
    __STATIC_INLINE void DL_I2C_enableACKOverrideOnPECNext(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL |= I2C_SACKCTL_ACKOEN_ON_PECNEXT_ENABLE;
    }

    /**
     *  @brief      Disable target ACK override when SMBus/PMBus PEC is next byte
     *              禁用 PEC 字节接收完成后的 ACK 覆盖。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableACKOverrideOnPECDone(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL &= ~(I2C_SACKCTL_ACKOEN_ON_PECDONE_MASK);
    }

    /**
     *  @brief      Checks if target ACK override when SMBus/PMBus PEC is next byte
     *              is enabled
     *              检查 PEC 字节接收完成后的 ACK 覆盖是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target ACK override when PEC is next byte is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target ACK override when PEC is next byte is enabled
     *  @retval     false if target ACK override when PEC is next byte is disabled
     */
    __STATIC_INLINE bool DL_I2C_isACKOverrideOnPECDoneEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SACKCTL & I2C_SACKCTL_ACKOEN_ON_PECDONE_MASK) ==
                I2C_SACKCTL_ACKOEN_ON_PECDONE_ENABLE);
    }

    /**
     *  @brief      Enable target ACK override when SMBus/PMBus PEC is done
     *              启用 PEC 字节接收完成后的 ACK 覆盖。
     *
     *  When enabled, this bit will automatically turn on the Target ACKOEN field
     * following the ACK/NACK of the received PEC byte.
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @sa                 DL_I2C_enableTargetACKOverride
     */
    __STATIC_INLINE void DL_I2C_enableACKOverrideOnPECDone(I2C_Regs *i2c)
    {
        i2c->SLAVE.SACKCTL |= I2C_SACKCTL_ACKOEN_ON_PECDONE_ENABLE;
    }

    /**
     *  @brief      Get the target SMBus/PMBus Packet Error Checking (PEC) count
     *              value
     *              获取目标 PEC 计数值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     The value the PEC count is set to
     *              返回值：PEC 计数值 [0x0, 0x01FF]。
     *
     *  @retval     Value between [0x0, 0x01FF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetPECCountValue(const I2C_Regs *i2c)
    {
        return (i2c->SLAVE.TARGET_PECCTL & I2C_TARGET_PECCTL_PECCNT_MASK);
    }

    /**
     *  @brief      Set the target SMBus/PMBus Packet Error Checking (PEC) count
     *              value
     *              设置目标 PEC 计数值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  count   The value to set the PEC count to.
     *                      Value between [0x0, 0x01FF]
     *                      PEC 计数值。
     */
    __STATIC_INLINE void DL_I2C_setTargetPECCountValue(
        I2C_Regs *i2c, uint32_t count)
    {
        DL_Common_updateReg(
            &i2c->SLAVE.TARGET_PECCTL, count, I2C_TARGET_PECCTL_PECCNT_MASK);
    }

    /**
     *  @brief      Disable target SMBus/PMBus Packet Error Checking (PEC)
     *              禁用目标 PEC 校验。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTargetPEC(I2C_Regs *i2c)
    {
        i2c->SLAVE.TARGET_PECCTL &= ~(I2C_TARGET_PECCTL_PECEN_MASK);
    }

    /**
     *  @brief      Checks if target SMBus/PMBus Packet Error Checking (PEC) is
     *              enabled
     *              检查目标 PEC 校验是否已启用。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     If target PEC is enabled
     *              返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if target PEC is enabled
     *  @retval     false if target PEC is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTargetPECEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.TARGET_PECCTL & I2C_TARGET_PECCTL_PECEN_MASK) ==
                I2C_TARGET_PECCTL_PECEN_ENABLE);
    }

    /**
     *  @brief      Enable target SMBus/PMBus Packet Error Checking (PEC)
     *              启用目标 PEC 校验，多项式为 x^8 + x^2 + x^1 + 1。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTargetPEC(I2C_Regs *i2c)
    {
        i2c->SLAVE.TARGET_PECCTL |= I2C_TARGET_PECCTL_PECEN_ENABLE;
    }

    /**
     *  @brief      Get the current SMBus/PMBus PEC byte count of the Target state
     *              machine
     *              获取目标状态机当前 PEC 字节计数。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     The current PEC byte count of the target state machine
     *              返回值：当前 PEC 字节计数 [0x0, 0x01FF]。
     *
     *  @retval     Value between [0x0, 0x01FF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetCurrentPECCount(const I2C_Regs *i2c)
    {
        return (i2c->SLAVE.TARGET_PECSR & I2C_TARGET_PECSR_PECBYTECNT_MASK);
    }

    /**
     *  @brief      Get status if SMBus/PMBus target PEC was checked in last
     *              transaction
     *              查询上次事务是否执行了目标 PEC 校验。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Status of PEC target checked
     *              返回值：PEC 校验状态。
     *
     *  @retval     One of @ref DL_I2C_TARGET_PEC_STATUS
     */
    __STATIC_INLINE DL_I2C_TARGET_PEC_STATUS DL_I2C_getTargetPECCheckedStatus(
        const I2C_Regs *i2c)
    {
        uint32_t status =
            i2c->SLAVE.TARGET_PECSR & I2C_TARGET_PECSR_PECSTS_CHECK_MASK;

        return (DL_I2C_TARGET_PEC_STATUS)(status);
    }

    /**
     *  @brief      Get status if SMBus/PMBus target PEC had an error
     *              查询上次事务的目标 PEC 校验错误状态。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *
     *  @return     Status of target PEC error check
     *              返回值：PEC 校验错误状态。
     *
     *  @retval     One of @ref DL_I2C_TARGET_PEC_CHECK_ERROR
     */
    __STATIC_INLINE DL_I2C_TARGET_PEC_CHECK_ERROR DL_I2C_getTargetPECCheckError(
        const I2C_Regs *i2c)
    {
        uint32_t status =
            i2c->SLAVE.TARGET_PECSR & I2C_TARGET_PECSR_PECSTS_ERROR_MASK;

        return (DL_I2C_TARGET_PEC_CHECK_ERROR)(status);
    }

    /**
     *  @brief      Get target TX FIFO threshold level
     *              获取目标 TX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     返回值：TX FIFO 触发阈值。
     *
     *  @retval     One of @ref DL_I2C_TX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2C_TX_FIFO_LEVEL DL_I2C_getTargetTXFIFOThreshold(
        const I2C_Regs *i2c)
    {
        uint32_t level = i2c->SLAVE.SFIFOCTL & I2C_SFIFOCTL_TXTRIG_MASK;

        return (DL_I2C_TX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set target TX FIFO threshold level
     *              设置目标 TX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  level   TX FIFO 触发阈值，@ref DL_I2C_TX_FIFO_LEVEL。
     */
    __STATIC_INLINE void DL_I2C_setTargetTXFIFOThreshold(
        I2C_Regs *i2c, DL_I2C_TX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(&i2c->SLAVE.SFIFOCTL, (uint32_t)level,
                            (uint32_t)I2C_SFIFOCTL_TXTRIG_MASK);
    }

    /**
     *  @brief      Stop target TX FIFO flush
     *              停止目标 TX FIFO 刷新。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_stopFlushTargetTXFIFO(I2C_Regs *i2c)
    {
        i2c->SLAVE.SFIFOCTL &= ~(I2C_SFIFOCTL_TXFLUSH_MASK);
    }

    /**
     *  @brief      Start target TX FIFO flush
     *              启动目标 TX FIFO 刷新。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_startFlushTargetTXFIFO(I2C_Regs *i2c)
    {
        i2c->SLAVE.SFIFOCTL |= I2C_SFIFOCTL_TXFLUSH_MASK;
    }

    /**
     *  @brief      Stop target RX FIFO flush
     *              停止目标 RX FIFO 刷新。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_stopFlushTargetRXFIFO(I2C_Regs *i2c)
    {
        i2c->SLAVE.SFIFOCTL &= ~(I2C_SFIFOCTL_RXFLUSH_MASK);
    }

    /**
     *  @brief      Start target RX FIFO flush
     *              启动目标 RX FIFO 刷新。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_startFlushTargetRXFIFO(I2C_Regs *i2c)
    {
        i2c->SLAVE.SFIFOCTL |= I2C_SFIFOCTL_RXFLUSH_MASK;
    }

    /**
     *  @brief      Get target RX FIFO threshold level
     *              获取目标 RX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     返回值：RX FIFO 触发阈值。
     *
     *  @retval     One of @ref DL_I2C_RX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_I2C_RX_FIFO_LEVEL DL_I2C_getTargetRXFIFOThreshold(
        const I2C_Regs *i2c)
    {
        uint32_t level = i2c->SLAVE.SFIFOCTL & I2C_SFIFOCTL_RXTRIG_MASK;

        return (DL_I2C_RX_FIFO_LEVEL)(level);
    }

    /**
     *  @brief      Set target RX FIFO threshold level
     *              设置目标 RX FIFO 触发阈值。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @param[in]  level   RX FIFO 触发阈值，@ref DL_I2C_RX_FIFO_LEVEL。
     */
    __STATIC_INLINE void DL_I2C_setTargetRXFIFOThreshold(
        I2C_Regs *i2c, DL_I2C_RX_FIFO_LEVEL level)
    {
        DL_Common_updateReg(
            &i2c->SLAVE.SFIFOCTL, (uint32_t)level, I2C_SFIFOCTL_RXTRIG_MASK);
    }

    /**
     *  @brief      Get number of bytes which can be read from RX FIFO
     *              获取目标 RX FIFO 中可读取的字节数。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *  @return     返回值：可读字节数 [0x0, 0x8]。
     *
     *  @retval     [0x0, 0x8]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetRXFIFOCounter(const I2C_Regs *i2c)
    {
        return (i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_RXFIFOCNT_MASK);
    }

    /**
     *  @brief      Get number of bytes which can be put into TX FIFO
     *              获取目标 TX FIFO 中可写入的字节数。
     *
     *  @param[in]  i2c         Pointer to the register overlay for the peripheral
     *                          I2C 外设寄存器基址。
     *  @return     返回值：可写字节数 [0x0, 0x8]。
     *
     *  @retval     [0x0, 0x8]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTargetTXFIFOCounter(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_TXFIFOCNT_MASK) >>
                I2C_SFIFOSR_TXFIFOCNT_OFS);
    }

    /**
     *  @brief      Checks if target RX FIFO flush is active
     *              检查目标 RX FIFO 刷新是否正在进行。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     返回值：`true` 表示刷新中，`false` 表示未刷新。
     *
     *  @retval     true if target RX FIFO flush is active
     *  @retval     false if target RX FIFO flush is not active
     */
    __STATIC_INLINE bool DL_I2C_isTargetRXFIFOFlushActive(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_RXFLUSH_MASK) ==
                I2C_SFIFOSR_RXFLUSH_ACTIVE);
    }

    /**
     *  @brief      Checks if target TX FIFO flush is active
     *              检查目标 TX FIFO 刷新是否正在进行。
     *
     *  @param[in]  i2c     Pointer to the register overlay for the peripheral
     *                      I2C 外设寄存器基址。
     *  @return     返回值：`true` 表示刷新中，`false` 表示未刷新。
     *
     *  @retval     true if target TX FIFO flush is active
     *  @retval     false if target TX FIFO flush is not active
     */
    __STATIC_INLINE bool DL_I2C_isTargetTXFIFOFlushActive(const I2C_Regs *i2c)
    {
        return ((i2c->SLAVE.SFIFOSR & I2C_SFIFOSR_TXFLUSH_MASK) ==
                I2C_SFIFOSR_TXFLUSH_ACTIVE);
    }

    /**
     *  @brief      Enable I2C interrupts
     *              启用 I2C 中断。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  interruptMask  中断位掩码，@ref DL_I2C_INTERRUPT 的按位或。
     */
    __STATIC_INLINE void DL_I2C_enableInterrupt(
        I2C_Regs *i2c, uint32_t interruptMask)
    {
        i2c->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable I2C interrupts
     *              禁用 I2C 中断。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  interruptMask  中断位掩码，@ref DL_I2C_INTERRUPT 的按位或。
     */
    __STATIC_INLINE void DL_I2C_disableInterrupt(
        I2C_Regs *i2c, uint32_t interruptMask)
    {
        i2c->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which I2C interrupts are enabled
     *              查询哪些 I2C 中断已启用。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  interruptMask  中断位掩码。
     *
     *  @return     返回值：已启用的中断位掩码。
     *
     *  @retval     Bitwise OR of @ref DL_I2C_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2C_getEnabledInterrupts(
        const I2C_Regs *i2c, uint32_t interruptMask)
    {
        return (i2c->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled I2C interrupts
     *              查询已启用的 I2C 中断是否有挂起。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  interruptMask  中断位掩码。
     *
     *  @return     返回值：挂起的中断位掩码。
     *
     *  @retval     Bitwise OR of @ref DL_I2C_INTERRUPT values
     *
     *  @sa         DL_I2C_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_I2C_getEnabledInterruptStatus(
        const I2C_Regs *i2c, uint32_t interruptMask)
    {
        return (i2c->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any I2C interrupt
     *              查询任意 I2C 中断是否有挂起（无需先启用）。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  interruptMask  中断位掩码。
     *
     *  @return     返回值：挂起的中断位掩码。
     *
     *  @retval     Bitwise OR of @ref DL_I2C_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_I2C_getRawInterruptStatus(
        const I2C_Regs *i2c, uint32_t interruptMask)
    {
        return (i2c->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending I2C interrupt
     *              获取最高优先级的挂起 I2C 中断索引。
     *
     *  @param[in]  i2c  I2C 外设寄存器基址。
     *
     *  @return     返回值：最高优先级中断索引。
     *
     *  @retval     One of @ref DL_I2C_IIDX
     */
    __STATIC_INLINE DL_I2C_IIDX DL_I2C_getPendingInterrupt(const I2C_Regs *i2c)
    {
        return ((DL_I2C_IIDX)i2c->CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending I2C interrupts
     *              清除挂起的 I2C 中断标志。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  interruptMask  中断位掩码。
     */
    __STATIC_INLINE void DL_I2C_clearInterruptStatus(
        I2C_Regs *i2c, uint32_t interruptMask)
    {
        i2c->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief Enables I2C interrupt for triggering DMA events
     *         启用 I2C 中断作为 DMA 触发条件。
     *
     * @note Only one interrupt source should be enabled at a time.
     *       每次应仅启用一个中断源。
     *
     *  @param[in]  i2c        I2C 外设寄存器基址。
     *  @param[in]  index      事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *  @param[in]  interrupt  DMA 触发中断，@ref DL_I2C_DMA_INTERRUPT。
     */
    __STATIC_INLINE void DL_I2C_enableDMAEvent(
        I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index, uint32_t interrupt)
    {
        switch (index)
        {
        case DL_I2C_EVENT_ROUTE_1:
            i2c->DMA_TRIG1.IMASK = interrupt;
            break;
        case DL_I2C_EVENT_ROUTE_2:
            i2c->DMA_TRIG0.IMASK = interrupt;
            break;
        default:
            break;
        }
    }

    /**
     *  @brief Disables I2C interrupt from triggering DMA events
     *         禁用 I2C 中断作为 DMA 触发条件。
     *
     *  @param[in]  i2c        I2C 外设寄存器基址。
     *  @param[in]  index      事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *  @param[in]  interrupt  DMA 触发中断，@ref DL_I2C_DMA_INTERRUPT。
     */
    __STATIC_INLINE void DL_I2C_disableDMAEvent(
        I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index, uint32_t interrupt)
    {
        switch (index)
        {
        case DL_I2C_EVENT_ROUTE_1:
            i2c->DMA_TRIG1.IMASK &= ~(interrupt);
            break;
        case DL_I2C_EVENT_ROUTE_2:
            i2c->DMA_TRIG0.IMASK &= ~(interrupt);
            break;
        default:
            break;
        }
    }

    /**
     *  @brief Check which I2C interrupt for DMA receive events is enabled
     *         查询哪些 DMA 事件中断已启用。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  index          事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *  @param[in]  interruptMask  中断位掩码。
     *
     *  @return     返回值：已启用的 DMA 事件中断位掩码。
     */
    __STATIC_INLINE uint32_t DL_I2C_getEnabledDMAEvents(
        I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index, uint32_t interruptMask)
    {
        volatile uint32_t *pReg = &i2c->DMA_TRIG1.IMASK;

        return ((*(pReg + (uint32_t)index) & interruptMask));
    }

    /**
     *  @brief Check interrupt flag of enabled I2C interrupt for DMA event
     *         查询已启用的 DMA 事件中断是否有挂起。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  index          事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *  @param[in]  interruptMask  中断位掩码。
     *
     *  @return     返回值：挂起的 DMA 事件中断位掩码。
     *
     *  @sa         DL_I2C_enableDMAEvent
     */
    __STATIC_INLINE uint32_t DL_I2C_getEnabledDMAEventStatus(
        const I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index, uint32_t interruptMask)
    {
        const volatile uint32_t *pReg = &i2c->DMA_TRIG1.MIS;

        return ((*(pReg + (uint32_t)index) & interruptMask));
    }

    /**
     *  @brief Check interrupt flag of any I2C interrupt for DMA event
     *         查询任意 DMA 事件中断是否有挂起（无需先启用）。
     *
     *  @param[in]  i2c            I2C 外设寄存器基址。
     *  @param[in]  index          事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *  @param[in]  interruptMask  中断位掩码。
     *
     *  @return     返回值：挂起的 DMA 事件中断位掩码。
     */
    __STATIC_INLINE uint32_t DL_I2C_getRawDMAEventStatus(
        const I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index, uint32_t interruptMask)
    {
        const volatile uint32_t *pReg = &i2c->DMA_TRIG1.RIS;

        return ((*(pReg + (uint32_t)index) & interruptMask));
    }

    /**
     *  @brief Get highest priority pending I2C interrupt for DMA event
     *         获取最高优先级的挂起 DMA 事件中断索引。
     *
     *  @param[in]  i2c        I2C 外设寄存器基址。
     *  @param[in]  index      事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *
     *  @return     返回值：最高优先级 DMA 事件中断索引。
     *
     *  @retval     One of @ref DL_I2C_DMA_IIDX
     */
    __STATIC_INLINE DL_I2C_DMA_IIDX DL_I2C_getPendingDMAEvent(
        const I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index)
    {
        const volatile uint32_t *pReg = &i2c->DMA_TRIG1.IIDX;

        return (DL_I2C_DMA_IIDX)((*(pReg + (uint32_t)index)));
    }

    /**
     *  @brief Clear pending I2C interrupts for DMA events
     *         清除挂起的 DMA 事件中断标志。
     *
     *  @param[in]  i2c        I2C 外设寄存器基址。
     *  @param[in]  index      事件路由选择，@ref DL_I2C_EVENT_ROUTE。
     *  @param[in]  interrupt  要清除的中断，@ref DL_I2C_DMA_INTERRUPT。
     */
    __STATIC_INLINE void DL_I2C_clearDMAEvent(
        I2C_Regs *i2c, DL_I2C_EVENT_ROUTE index, uint32_t interrupt)
    {
        switch (index)
        {
        case DL_I2C_EVENT_ROUTE_1:
            i2c->DMA_TRIG1.ICLR |= interrupt;
            break;
        case DL_I2C_EVENT_ROUTE_2:
            i2c->DMA_TRIG0.ICLR |= interrupt;
            break;
        default:
            break;
        }
    }

    /**
     *  @brief      Disable analog and digital glitch filter chaining
     *              禁用模拟和数字毛刺滤波器串联。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableGlitchFilterChaining(I2C_Regs *i2c)
    {
        i2c->GFCTL &= ~(I2C_GFCTL_CHAIN_MASK);
    }

    /**
     *  @brief      Checks if analog and digital glitch filter chaining is enabled
     *              检查毛刺滤波器串联是否已启用。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *
     *  @return     返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if glitch filter chaining is enabled
     *  @retval     false if glitch filter chaining is disabled
     */
    __STATIC_INLINE bool DL_I2C_isGlitchFilterChainingEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->GFCTL & I2C_GFCTL_CHAIN_MASK) == I2C_GFCTL_CHAIN_ENABLE);
    }

    /**
     *  @brief      Enable analog and digital glitch filter chaining
     *              启用模拟和数字毛刺滤波器串联。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableGlitchFilterChaining(I2C_Regs *i2c)
    {
        i2c->GFCTL |= I2C_GFCTL_CHAIN_ENABLE;
    }

    /**
     *  @brief      Get the Timeout Counter A value
     *              获取超时计数器 A 预加载值。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @return     返回值：超时 A 计数值 [0x0, 0xFF]。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTimeoutACount(const I2C_Regs *i2c)
    {
        return (i2c->TIMEOUT_CTL & I2C_TIMEOUT_CTL_TCNTLA_MASK);
    }

    /**
     *  @brief      Set the Timeout Counter A value
     *              设置超时计数器 A 预加载值，用于 SCL 低电平超时检测。
     *
     *  NOTE: 计数值必须大于 1 才能启用超时。每次计数等于 520 倍功能时钟周期。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @param[in]  count   超时 A 计数值 [0x0, 0xFF]。
     */
    __STATIC_INLINE void DL_I2C_setTimeoutACount(I2C_Regs *i2c, uint32_t count)
    {
        DL_Common_updateReg(&i2c->TIMEOUT_CTL, count, I2C_TIMEOUT_CTL_TCNTLA_MASK);
    }

    /**
     *  @brief      Disable Timeout Counter A
     *              禁用超时计数器 A。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTimeoutA(I2C_Regs *i2c)
    {
        i2c->TIMEOUT_CTL &= ~(I2C_TIMEOUT_CTL_TCNTAEN_MASK);
    }

    /**
     *  @brief      Checks if Timeout Counter A is enabled
     *              检查超时计数器 A 是否已启用。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @return     返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if Timeout Counter A is enabled
     *  @retval     false if Timeout Counter A is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTimeoutAEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->TIMEOUT_CTL & I2C_TIMEOUT_CTL_TCNTAEN_MASK) ==
                I2C_TIMEOUT_CTL_TCNTAEN_ENABLE);
    }

    /**
     *  @brief      Enable Timeout Counter A
     *              启用超时计数器 A。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTimeoutA(I2C_Regs *i2c)
    {
        i2c->TIMEOUT_CTL |= I2C_TIMEOUT_CTL_TCNTAEN_ENABLE;
    }

    /**
     *  @brief      Get the current Timer Counter A value
     *              获取超时计数器 A 当前计数值。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @return     返回值：当前计数值 [0x0, 0xFF]。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getCurrentTimeoutACounter(const I2C_Regs *i2c)
    {
        return (i2c->TIMEOUT_CNT & I2C_TIMEOUT_CNT_TCNTA_MASK);
    }

    /**
     *  @brief      Get the Timeout Counter B value
     *              获取超时计数器 B 预加载值。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @return     返回值：超时 B 计数值 [0x0, 0xFF]。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getTimeoutBCount(const I2C_Regs *i2c)
    {
        return ((i2c->TIMEOUT_CTL & I2C_TIMEOUT_CTL_TCNTLB_MASK) >>
                I2C_TIMEOUT_CTL_TCNTLB_OFS);
    }

    /**
     *  @brief      Set the Timeout Counter B value
     *              设置超时计数器 B 预加载值，用于 SCL 高电平超时检测。
     *
     *  NOTE: 计数值必须大于 1 才能启用超时。每次计数等于 1 倍功能时钟周期。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @param[in]  count   超时 B 计数值 [0x0, 0xFF]。
     */
    __STATIC_INLINE void DL_I2C_setTimeoutBCount(I2C_Regs *i2c, uint32_t count)
    {
        DL_Common_updateReg(&i2c->TIMEOUT_CTL,
                            (count << I2C_TIMEOUT_CTL_TCNTLB_OFS), I2C_TIMEOUT_CTL_TCNTLB_MASK);
    }

    /**
     *  @brief      Disable Timeout Counter B
     *              禁用超时计数器 B。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_disableTimeoutB(I2C_Regs *i2c)
    {
        i2c->TIMEOUT_CTL &= ~(I2C_TIMEOUT_CTL_TCNTBEN_MASK);
    }

    /**
     *  @brief      Checks if Timeout Counter B is enabled
     *              检查超时计数器 B 是否已启用。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @return     返回值：`true` 表示已启用，`false` 表示已禁用。
     *
     *  @retval     true if Timeout Counter B is enabled
     *  @retval     false if Timeout Counter B is disabled
     */
    __STATIC_INLINE bool DL_I2C_isTimeoutBEnabled(const I2C_Regs *i2c)
    {
        return ((i2c->TIMEOUT_CTL & I2C_TIMEOUT_CTL_TCNTBEN_MASK) ==
                I2C_TIMEOUT_CTL_TCNTBEN_ENABLE);
    }

    /**
     *  @brief      Enable Timeout Counter B
     *              启用超时计数器 B。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     */
    __STATIC_INLINE void DL_I2C_enableTimeoutB(I2C_Regs *i2c)
    {
        i2c->TIMEOUT_CTL |= I2C_TIMEOUT_CTL_TCNTBEN_ENABLE;
    }

    /**
     *  @brief      Get the current Timer Counter B value
     *              获取超时计数器 B 当前计数值。
     *
     *  @param[in]  i2c     I2C 外设寄存器基址。
     *  @return     返回值：当前计数值 [0x0, 0xFF]。
     *
     *  @retval     Value between [0x0, 0xFF]
     */
    __STATIC_INLINE uint32_t DL_I2C_getCurrentTimeoutBCounter(const I2C_Regs *i2c)
    {
        return (i2c->TIMEOUT_CNT & I2C_TIMEOUT_CNT_TCNTB_MASK);
    }

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_I2C__ */

#endif /* ti_dl_dl_i2c__include */
/** @}*/
