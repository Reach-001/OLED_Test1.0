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
 *  @file       dl_uart.h
 *  @brief      UART Driver Library
 *  @defgroup   UART Universal Asynchronous Receiver-Transmitter (UART)
 *
 *  @anchor ti_dl_dl_uart_Overview
 *  # Overview
 *
 *  The Universal Asynchronous Receiver-Transmitter Driver Library allows
 *  full configuration of the MSPM0 UART module.
 *  This module provides common functionality for UART-Main and UART-Extend, but
 *  developers should use the corresponding dl_uart_extend or dl_uart_main APIs
 *  directly.
 *
 *  <hr>
 ******************************************************************************
 *
 *  @par 文件用途
 *  UART 驱动公共头文件，定义 Main/Extend 两种模式共用的寄存器操作、
 *  中断/DMA 宏、枚举、配置结构体以及全部内联访问函数。
 */
/** @addtogroup UART
 * @{
 */
#ifndef ti_dl_dl_uart__include
#define ti_dl_dl_uart__include

#if defined(ti_dl_dl_uart_main__include) || \
    defined(ti_dl_dl_uart_extend__include) || defined(DOXYGEN__INCLUDE)

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#if defined(__MSPM0_HAS_UART_MAIN__) || defined(__MSPM0_HAS_UART_EXTD__)

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

/** @addtogroup DL_UART_INTERRUPT
 *  @{
 */
/*!
 * @brief DMA done on transmit interrupt, DMA 发送完成中断
 */
#define DL_UART_INTERRUPT_DMA_DONE_TX   (UART_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief DMA done on receive interrupt, DMA 接收完成中断
 */
#define DL_UART_INTERRUPT_DMA_DONE_RX   (UART_CPU_INT_IMASK_DMA_DONE_RX_SET)

/*!
 * @brief Clear to send interrupt, CTS 信号变化中断
 */
#define DL_UART_INTERRUPT_CTS_DONE              (UART_CPU_INT_IMASK_CTS_SET)

/*!
 * @brief 9-bit mode address match interrupt, 9 位模式地址匹配中断
 */
#define DL_UART_INTERRUPT_ADDRESS_MATCH  (UART_CPU_INT_IMASK_ADDR_MATCH_SET)

/*!
 * @brief LINC0 match interrupt, LIN 计数器 LINC0 匹配中断
 */
#define DL_UART_INTERRUPT_LINC0_MATCH         (UART_CPU_INT_IMASK_LINC0_SET)

/*!
 * @brief End of transmission interrupt, 发送完成（含 FIFO 空）中断
 */
#define DL_UART_INTERRUPT_EOT_DONE              (UART_CPU_INT_IMASK_EOT_SET)

/*!
 * @brief UART transmit interrupt, TX FIFO 达到阈值中断
 */
#define DL_UART_INTERRUPT_TX                  (UART_CPU_INT_IMASK_TXINT_SET)

/*!
 * @brief UART receive interrupt, RX FIFO 达到阈值中断
 */
#define DL_UART_INTERRUPT_RX                  (UART_CPU_INT_IMASK_RXINT_SET)

/*!
 * @brief LIN hardware counter overflow interrupt, LIN 硬件计数器溢出中断
 */
#define DL_UART_INTERRUPT_LIN_COUNTER_OVERFLOW                                \
                                             (UART_CPU_INT_IMASK_LINOVF_SET)

/*!
 * @brief LIN rising edge LINC1 interrupt, LIN 上升沿 LINC1 捕获中断
 */
#define DL_UART_INTERRUPT_LIN_RISING_EDGE                                     \
                                              (UART_CPU_INT_IMASK_LINC1_SET)

/*!
 * @brief LIN falling edge LINC0 interrupt, LIN 下降沿 LINC0 捕获中断
 */
#define DL_UART_INTERRUPT_LIN_FALLING_EDGE                                    \
                                              (UART_CPU_INT_IMASK_LINC0_SET)

/*!
 * @brief Positive Edge on UARTxRXD interrupt, RXD 引脚正边沿中断
 */
#define DL_UART_INTERRUPT_RXD_POS_EDGE         (UART_CPU_INT_IMASK_RXPE_SET)

/*!
 * @brief Negative Edge on UARTxRXD interrupt, RXD 引脚负边沿中断
 */
#define DL_UART_INTERRUPT_RXD_NEG_EDGE         (UART_CPU_INT_IMASK_RXNE_SET)

/*!
 * @brief Overrun error interrupt, 溢出错误中断
 */
#define DL_UART_INTERRUPT_OVERRUN_ERROR      (UART_CPU_INT_IMASK_OVRERR_SET)

/*!
 * @brief Break error interrupt, 间隔错误中断
 */
#define DL_UART_INTERRUPT_BREAK_ERROR        (UART_CPU_INT_IMASK_BRKERR_SET)

/*!
 * @brief Parity error interrupt, 校验错误中断
 */
#define DL_UART_INTERRUPT_PARITY_ERROR       (UART_CPU_INT_IMASK_PARERR_SET)

/*!
 * @brief Framing error interrupt, 帧格式错误中断
 */
#define DL_UART_INTERRUPT_FRAMING_ERROR      (UART_CPU_INT_IMASK_FRMERR_SET)

/*!
 * @brief Receive timeout interrupt, 接收超时中断
 */
#define DL_UART_INTERRUPT_RX_TIMEOUT_ERROR    (UART_CPU_INT_IMASK_RTOUT_SET)


/*!
 * @brief Noise error interrupt, 噪声错误中断
 */
#define DL_UART_INTERRUPT_NOISE_ERROR          (UART_CPU_INT_IMASK_NERR_SET)


/** @}*/

/*! @enum DL_UART_IIDX 中断索引枚举，用于 getPendingInterrupt 判断中断源 */
typedef enum {
    /*! DMA 发送完成中断索引 */
    DL_UART_IIDX_DMA_DONE_TX = UART_CPU_INT_IIDX_STAT_DMA_DONE_TX,
    /*! DMA 接收完成中断索引 */
    DL_UART_IIDX_DMA_DONE_RX = UART_CPU_INT_IIDX_STAT_DMA_DONE_RX,
    /*! CTS 信号中断索引 */
    DL_UART_IIDX_CTS_DONE = UART_CPU_INT_IIDX_STAT_CTS,
    /*! 9 位模式地址匹配中断索引 */
    DL_UART_IIDX_ADDRESS_MATCH = UART_CPU_INT_IIDX_STAT_MODE_9B,
    /*! 发送完成中断索引 */
    DL_UART_IIDX_EOT_DONE = UART_CPU_INT_IIDX_STAT_EOT,
    /*! TX FIFO 达到阈值中断索引 */
    DL_UART_IIDX_TX = UART_CPU_INT_IIDX_STAT_TXIFG,
    /*! RX FIFO 达到阈值中断索引 */
    DL_UART_IIDX_RX = UART_CPU_INT_IIDX_STAT_RXIFG,
    /*! LIN 计数器溢出中断索引 */
    DL_UART_IIDX_LIN_COUNTER_OVERFLOW = UART_CPU_INT_IIDX_STAT_LINOVF,
    /*! LIN 上升沿 LINC1 捕获中断索引 */
    DL_UART_IIDX_LIN_RISING_EDGE = UART_CPU_INT_IIDX_STAT_LINC1,
    /*! LIN 下降沿 LINC0 捕获中断索引 */
    DL_UART_IIDX_LIN_FALLING_EDGE = UART_CPU_INT_IIDX_STAT_LINC0,
    /*! RXD 正边沿中断索引 */
    DL_UART_IIDX_RXD_POS_EDGE = UART_CPU_INT_IIDX_STAT_RXPE,
    /*! RXD 负边沿中断索引 */
    DL_UART_IIDX_RXD_NEG_EDGE = UART_CPU_INT_IIDX_STAT_RXNE,
    /*! 溢出错误中断索引 */
    DL_UART_IIDX_OVERRUN_ERROR = UART_CPU_INT_IIDX_STAT_OEFG,
    /*! 间隔错误中断索引 */
    DL_UART_IIDX_BREAK_ERROR = UART_CPU_INT_IIDX_STAT_BEFG,
    /*! 校验错误中断索引 */
    DL_UART_IIDX_PARITY_ERROR = UART_CPU_INT_IIDX_STAT_PEFG,
    /*! 帧格式错误中断索引 */
    DL_UART_IIDX_FRAMING_ERROR = UART_CPU_INT_IIDX_STAT_FEFG,
    /*! 接收超时中断索引 */
    DL_UART_IIDX_RX_TIMEOUT_ERROR = UART_CPU_INT_IIDX_STAT_RTFG,


    /*! 噪声错误中断索引 */
    DL_UART_IIDX_NOISE_ERROR = UART_CPU_INT_IIDX_STAT_NERR_EVT,


    /*! 无中断 */
    DL_UART_IIDX_NO_INTERRUPT = UART_CPU_INT_IIDX_STAT_NO_INTR
} DL_UART_IIDX;

/*! @enum DL_UART_DMA_IIDX_RX DMA 接收触发源索引 */
typedef enum {
    /*! RX FIFO 达到阈值作为 DMA 接收触发 */
    DL_UART_DMA_IIDX_RX_TRIGGER = UART_DMA_TRIG_RX_IIDX_STAT_RXIFG,
    /*! 接收超时作为 DMA 接收触发 */
    DL_UART_DMA_IIDX_RX_TIMEOUT_TRIGGER = UART_DMA_TRIG_RX_IIDX_STAT_RTFG
} DL_UART_DMA_IIDX_RX;

/*! @enum DL_UART_DMA_IIDX_TX DMA 发送触发源索引 */
typedef enum {
    /*! TX FIFO 达到阈值作为 DMA 发送触发 */
    DL_UART_DMA_IIDX_TX_TRIGGER = UART_DMA_TRIG_TX_IIDX_STAT_TXIFG
} DL_UART_DMA_IIDX_TX;

/** @addtogroup DL_UART_DMA_INTERRUPT_RX
 *  @{
 */
/*!
 * @brief UART interrupt for enabling UART receive as DMA trigger
 *        RX FIFO 达到阈值触发 DMA 接收
 */
#define DL_UART_DMA_INTERRUPT_RX              (UART_DMA_TRIG_RX_IMASK_RXINT_SET)

/*!
 * @brief UART interrupt indicating DMA is done with the RX
 *        DMA 接收完成中断
 */
#define DL_UART_DMA_DONE_INTERRUPT_RX         (UART_CPU_INT_IMASK_DMA_DONE_RX_SET)

/*!
 * @brief UART interrupt for enabling UART receive timeout as DMA trigger
 *        接收超时触发 DMA 接收
 */
#define DL_UART_DMA_INTERRUPT_RX_TIMEOUT      (UART_DMA_TRIG_RX_IMASK_RTOUT_SET)

/** @}*/

/*!
 * @brief UART interrupt for enabling UART transmit as DMA trigger
 *        TX FIFO 达到阈值触发 DMA 发送
 */
#define DL_UART_DMA_INTERRUPT_TX              (UART_DMA_TRIG_TX_IMASK_TXINT_SET)

/*!
 * @brief UART interrupt indicating DMA is done with the TX
 *        DMA 发送完成中断
 */
#define DL_UART_DMA_DONE_INTERRUPT_TX         (UART_CPU_INT_IMASK_DMA_DONE_TX_SET)

/** @addtogroup DL_UART_ERROR
 *  @{
 */
/*!
 * @brief Overrun error ocurred, 溢出错误（RX FIFO 满时仍有新数据到达）
 */
#define DL_UART_ERROR_OVERRUN                          (UART_RXDATA_OVRERR_SET)

/*!
 * @brief Break error ocurred, 间隔错误（检测到过长低电平）
 */
#define DL_UART_ERROR_BREAK                            (UART_RXDATA_BRKERR_SET)

/*!
 * @brief Parity error ocurred, 校验错误（校验位不匹配）
 */
#define DL_UART_ERROR_PARITY                           (UART_RXDATA_PARERR_SET)

/*!
 * @brief Framing error ocurred, 帧格式错误（停止位不正确）
 */
#define DL_UART_ERROR_FRAMING                          (UART_RXDATA_FRMERR_SET)

/** @}*/

/*! @enum DL_UART_PULSE_WIDTH 模拟毛刺滤波脉宽选择 */
typedef enum {
    /*! 滤除短于 5ns 的脉冲 */
    DL_UART_PULSE_WIDTH_5_NS = UART_GFCTL_AGFSEL_AGLIT_5,
    /*! 滤除短于 10ns 的脉冲 */
    DL_UART_PULSE_WIDTH_10_NS = UART_GFCTL_AGFSEL_AGLIT_10,
    /*! 滤除短于 25ns 的脉冲 */
    DL_UART_PULSE_WIDTH_25_NS = UART_GFCTL_AGFSEL_AGLIT_25,
    /*! 滤除短于 50ns 的脉冲 */
    DL_UART_PULSE_WIDTH_50_NS = UART_GFCTL_AGFSEL_AGLIT_50
} DL_UART_PULSE_WIDTH;

/*! @enum DL_UART_OVERSAMPLING_RATE 过采样率选择 */
typedef enum {
    /*! 16 倍过采样 */
    DL_UART_OVERSAMPLING_RATE_16X = UART_CTL0_HSE_OVS16,
    /*! 8 倍过采样 */
    DL_UART_OVERSAMPLING_RATE_8X = UART_CTL0_HSE_OVS8,
    /*! 3 倍过采样。
     * IrDA、Manchester 和 DALI 模式不支持 3x 过采样。 */
    DL_UART_OVERSAMPLING_RATE_3X = UART_CTL0_HSE_OVS3
} DL_UART_OVERSAMPLING_RATE;

/*! @enum DL_UART_PARITY 校验模式 */
typedef enum {
    /*! 偶校验，1 的个数为偶数时校验位为 0 */
    DL_UART_PARITY_EVEN = (UART_LCRH_PEN_ENABLE | UART_LCRH_EPS_EVEN),
    /*! 奇校验，1 的个数为奇数时校验位为 0 */
    DL_UART_PARITY_ODD = (UART_LCRH_PEN_ENABLE | UART_LCRH_EPS_ODD),
    /*! 固定校验位为 1（发送时校验位恒为 1，接收时按 1 校验） */
    DL_UART_PARITY_STICK_ONE = (UART_LCRH_PEN_ENABLE | UART_LCRH_SPS_ENABLE | UART_LCRH_EPS_ODD),
    /*! 固定校验位为 0（发送时校验位恒为 0，接收时按 0 校验） */
    DL_UART_PARITY_STICK_ZERO = (UART_LCRH_PEN_ENABLE | UART_LCRH_SPS_ENABLE | UART_LCRH_EPS_EVEN),
    /*! 禁用校验 */
    DL_UART_PARITY_NONE = UART_LCRH_PEN_DISABLE
} DL_UART_PARITY;

/*! @enum DL_UART_WORD_LENGTH 数据位长度 */
typedef enum {
    /*! 5 位数据 */
    DL_UART_WORD_LENGTH_5_BITS = UART_LCRH_WLEN_DATABIT5,
    /*! 6 位数据 */
    DL_UART_WORD_LENGTH_6_BITS = UART_LCRH_WLEN_DATABIT6,
    /*! 7 位数据 */
    DL_UART_WORD_LENGTH_7_BITS = UART_LCRH_WLEN_DATABIT7,
    /*! 8 位数据 */
    DL_UART_WORD_LENGTH_8_BITS = UART_LCRH_WLEN_DATABIT8
} DL_UART_WORD_LENGTH;

/*! @enum DL_UART_MODE 通信模式 */
typedef enum {
    /*! 普通 UART 模式 */
    DL_UART_MODE_NORMAL = UART_CTL0_MODE_UART,
    /*! RS485 模式 */
    DL_UART_MODE_RS485 = UART_CTL0_MODE_RS485,
    /*! 空闲线多处理器模式 */
    DL_UART_MODE_IDLE_LINE = UART_CTL0_MODE_IDLELINE,
    /*! 9 位地址多处理器模式 */
    DL_UART_MODE_ADDR_9_BIT = UART_CTL0_MODE_ADDR9BIT,
    /*! ISO7816 智能卡模式 */
    DL_UART_MODE_SMART_CARD = UART_CTL0_MODE_SMART,
    /*! DALI 模式 */
    DL_UART_MODE_DALI = UART_CTL0_MODE_DALI
} DL_UART_MODE;

/*! @enum DL_UART_DIRECTION 收发方向 */
typedef enum {
    /*! 仅发送 */
    DL_UART_DIRECTION_TX = UART_CTL0_TXE_ENABLE,
    /*! 仅接收 */
    DL_UART_DIRECTION_RX = UART_CTL0_RXE_ENABLE,
    /*! 收发双向 */
    DL_UART_DIRECTION_TX_RX = (UART_CTL0_RXE_ENABLE | UART_CTL0_TXE_ENABLE),
    /*! 禁用收发 */
    DL_UART_DIRECTION_NONE = (UART_CTL0_RXE_DISABLE | UART_CTL0_TXE_DISABLE)
} DL_UART_DIRECTION;

/*! @enum DL_UART_CLOCK 时钟源选择 */
typedef enum {
    /*! 选择 BUSCLK 作为时钟源 */
    DL_UART_CLOCK_BUSCLK = UART_CLKSEL_BUSCLK_SEL_ENABLE,
    /*! 选择 MFCLK 作为时钟源 */
    DL_UART_CLOCK_MFCLK = UART_CLKSEL_MFCLK_SEL_ENABLE,
    /*! 选择 LFCLK 作为时钟源 */
    DL_UART_CLOCK_LFCLK = UART_CLKSEL_LFCLK_SEL_ENABLE
} DL_UART_CLOCK;

/*! @enum DL_UART_FLOW_CONTROL 流控制模式 */
typedef enum {
    /*! 启用 RTS（请求发送） */
    DL_UART_FLOW_CONTROL_RTS = UART_CTL0_RTSEN_ENABLE,
    /*! 启用 CTS（清除发送） */
    DL_UART_FLOW_CONTROL_CTS = UART_CTL0_CTSEN_ENABLE,
    /*! 同时启用 RTS 和 CTS */
    DL_UART_FLOW_CONTROL_RTS_CTS = (UART_CTL0_RTSEN_ENABLE | UART_CTL0_CTSEN_ENABLE),
    /*! 禁用流控制 */
    DL_UART_FLOW_CONTROL_NONE = (UART_CTL0_CTSEN_DISABLE | UART_CTL0_RTSEN_DISABLE)
} DL_UART_FLOW_CONTROL;

/*! @enum DL_UART_RTS 输出信号 */
typedef enum {
    /*! RTS 置位（低电平），表示 RX FIFO 低于阈值可接收 */
    DL_UART_RTS_ASSERT = UART_CTL0_RTS_SET,
    /*! RTS 解除（高电平），表示 RX FIFO 达到或超过阈值 */
    DL_UART_RTS_DEASSERT = UART_CTL0_RTS_CLR
} DL_UART_RTS;

/*! @enum DL_UART_STOP_BITS 停止位 */
typedef enum {
    /*! 1 位停止位 */
    DL_UART_STOP_BITS_ONE = UART_LCRH_STP2_DISABLE,
    /*! 2 位停止位 */
    DL_UART_STOP_BITS_TWO = UART_LCRH_STP2_ENABLE
} DL_UART_STOP_BITS;

/*! @enum DL_UART_TXD_OUT TXD 引脚手动输出电平 */
typedef enum {
    /*! TXD 输出低电平 */
    DL_UART_TXD_OUT_LOW = UART_CTL0_TXD_OUT_LOW,
    /*! TXD 输出高电平 */
    DL_UART_TXD_OUT_HIGH = UART_CTL0_TXD_OUT_HIGH
} DL_UART_TXD_OUT;

/*! @enum DL_UART_TX_FIFO_LEVEL TX FIFO 中断阈值 */
typedef enum {
    /*! FIFO 剩余 <= 3/4 时触发中断 */
    DL_UART_TX_FIFO_LEVEL_3_4_EMPTY = UART_IFLS_TXIFLSEL_LVL_3_4,
    /*! FIFO 剩余 <= 1/2 时触发中断 */
    DL_UART_TX_FIFO_LEVEL_1_2_EMPTY = UART_IFLS_TXIFLSEL_LVL_1_2,
    /*! FIFO 剩余 <= 1/4 时触发中断 */
    DL_UART_TX_FIFO_LEVEL_1_4_EMPTY = UART_IFLS_TXIFLSEL_LVL_1_4,
    /*! FIFO 完全为空时触发中断 */
    DL_UART_TX_FIFO_LEVEL_EMPTY = UART_IFLS_TXIFLSEL_LVL_EMPTY,
    /*! FIFO 中 >= 1 条数据时触发中断 */
    DL_UART_TX_FIFO_LEVEL_ONE_ENTRY = UART_IFLS_TXIFLSEL_LVL_1
} DL_UART_TX_FIFO_LEVEL;

/*! @enum DL_UART_RX_FIFO_LEVEL RX FIFO 中断阈值 */
typedef enum {
    /*! FIFO 中 >= 1 条数据时触发中断（DMA 触发必需） */
    DL_UART_RX_FIFO_LEVEL_ONE_ENTRY = UART_IFLS_RXIFLSEL_LVL_1,
    /*! FIFO 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_FULL = UART_IFLS_RXIFLSEL_LVL_FULL,
    /*! FIFO >= 3/4 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_3_4_FULL = UART_IFLS_RXIFLSEL_LVL_3_4,
    /*! FIFO >= 1/2 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_1_2_FULL = UART_IFLS_RXIFLSEL_LVL_1_2,
    /*! FIFO >= 1/4 满时触发中断 */
    DL_UART_RX_FIFO_LEVEL_1_4_FULL = UART_IFLS_RXIFLSEL_LVL_1_4,
} DL_UART_RX_FIFO_LEVEL;

/*! @enum DL_UART_IRDA_CLOCK IrDA 发射脉冲时钟源 */
typedef enum {
    /*! 基于波特率时钟 */
    DL_UART_IRDA_CLOCK_BAUD_RATE = UART_IRCTL_IRTXCLK_BRCLK,
    /*! 基于功能时钟 */
    DL_UART_IRDA_CLOCK_FUNCTIONAL = UART_IRCTL_IRTXCLK_BITCLK
} DL_UART_IRDA_CLOCK;

/*! @enum DL_UART_IRDA_POLARITY IrDA 接收极性 */
typedef enum {
    /*! 收到光脉冲时输出低电平 */
    DL_UART_IRDA_POLARITY_LOW = UART_IRCTL_IRRXPL_LOW,
    /*! 收到光脉冲时输出高电平 */
    DL_UART_IRDA_POLARITY_HIGH = UART_IRCTL_IRRXPL_HIGH
} DL_UART_IRDA_POLARITY;

/*!
 * @brief Sets the IrDA pulse width to 3/16 bit period when using the BITCLK16
 */
#define DL_UART_PULSE_WIDTH_3_16_BIT_PERIOD           ((uint32_t) 0x00000000U)

/*! @enum DL_UART_CLOCK_DIVIDE_RATIO 时钟分频比 */
typedef enum {
    /*! 分频比 1（不分频） */
    DL_UART_CLOCK_DIVIDE_RATIO_1 = UART_CLKDIV_RATIO_DIV_BY_1,
    /*! 分频比 2 */
    DL_UART_CLOCK_DIVIDE_RATIO_2 = UART_CLKDIV_RATIO_DIV_BY_2,
    /*! 分频比 3 */
    DL_UART_CLOCK_DIVIDE_RATIO_3 = UART_CLKDIV_RATIO_DIV_BY_3,
    /*! 分频比 4 */
    DL_UART_CLOCK_DIVIDE_RATIO_4 = UART_CLKDIV_RATIO_DIV_BY_4,
    /*! 分频比 5 */
    DL_UART_CLOCK_DIVIDE_RATIO_5 = UART_CLKDIV_RATIO_DIV_BY_5,
    /*! 分频比 6 */
    DL_UART_CLOCK_DIVIDE_RATIO_6 = UART_CLKDIV_RATIO_DIV_BY_6,
    /*! 分频比 7 */
    DL_UART_CLOCK_DIVIDE_RATIO_7 = UART_CLKDIV_RATIO_DIV_BY_7,
    /*! 分频比 8 */
    DL_UART_CLOCK_DIVIDE_RATIO_8 = UART_CLKDIV_RATIO_DIV_BY_8
} DL_UART_CLOCK_DIVIDE_RATIO;

/*! @enum DL_UART_CLOCK_DIVIDE2_RATIO 第二级时钟分频比（IrDA 专用） */
typedef enum {
    /*! 第二级分频比 1 */
    DL_UART_CLOCK_DIVIDE2_RATIO_1 = UART_CLKDIV2_RATIO_DIV_BY_1,
    /*! 第二级分频比 2 */
    DL_UART_CLOCK_DIVIDE2_RATIO_2 = UART_CLKDIV2_RATIO_DIV_BY_2,
    /*! 第二级分频比 3 */
    DL_UART_CLOCK_DIVIDE2_RATIO_3 = UART_CLKDIV2_RATIO_DIV_BY_3,
    /*! 第二级分频比 4 */
    DL_UART_CLOCK_DIVIDE2_RATIO_4 = UART_CLKDIV2_RATIO_DIV_BY_4,
    /*! 第二级分频比 5 */
    DL_UART_CLOCK_DIVIDE2_RATIO_5 = UART_CLKDIV2_RATIO_DIV_BY_5,
    /*! 第二级分频比 6 */
    DL_UART_CLOCK_DIVIDE2_RATIO_6 = UART_CLKDIV2_RATIO_DIV_BY_6,
    /*! 第二级分频比 7 */
    DL_UART_CLOCK_DIVIDE2_RATIO_7 = UART_CLKDIV2_RATIO_DIV_BY_7,
    /*! 第二级分频比 8 */
    DL_UART_CLOCK_DIVIDE2_RATIO_8 = UART_CLKDIV2_RATIO_DIV_BY_8
} DL_UART_CLOCK_DIVIDE2_RATIO;

    /* clang-format on */

    /*!
     *  @brief  Configuration struct for @ref DL_UART_init
     *          UART 初始化配置结构体
     */
    typedef struct
    {
        /*! 通信模式，取值见 @ref DL_UART_MODE */
        DL_UART_MODE mode;

        /*! 收发方向，取值见 @ref DL_UART_DIRECTION */
        DL_UART_DIRECTION direction;

        /*! 流控制配置，取值见 @ref DL_UART_FLOW_CONTROL */
        DL_UART_FLOW_CONTROL flowControl;

        /*! 校验模式，取值见 @ref DL_UART_PARITY */
        DL_UART_PARITY parity;

        /*! 数据位长度，取值见 @ref DL_UART_WORD_LENGTH */
        DL_UART_WORD_LENGTH wordLength;

        /*! 停止位，取值见 @ref DL_UART_STOP_BITS */
        DL_UART_STOP_BITS stopBits;

    } DL_UART_Config;

    /*!
     *  @brief  Configuration struct for @ref DL_UART_setClockConfig.
     *          UART 时钟配置结构体
     */
    typedef struct
    {
        /*! 时钟源选择，取值见 @ref DL_UART_CLOCK */
        DL_UART_CLOCK clockSel;

        /*! 分频比，取值见 @ref DL_UART_CLOCK_DIVIDE_RATIO */
        DL_UART_CLOCK_DIVIDE_RATIO divideRatio;

    } DL_UART_ClockConfig;

#ifdef __MSPM0_HAS_UART_MAIN__

    /**
     * @brief Configuration structure to backup UART Main peripheral state before
     *        going to STOP/STANDBY mode. Used by
     *        @ref DL_UART_Main_saveConfiguration and
     *        @ref DL_UART_Main_restoreConfiguration
     *        UART Main 外设备份配置结构体，用于 STOP/STANDBY 前保存状态
     */
    typedef struct
    {
        /*! CTL0 控制寄存器备份值 */
        uint32_t controlWord;

        /*! 时钟源选择备份，取值见 @ref DL_UART_CLOCK */
        uint32_t clockSel;

        /*! 分频比备份，取值见 @ref DL_UART_CLOCK_DIVIDE_RATIO */
        uint32_t divideRatio;

        /*! FIFO 中断阈值寄存器备份 */
        uint32_t interruptFifoLevelSelectWord;

        /*! 波特率整数分频值，范围 0-65535 */
        uint32_t ibrd;

        /*! 波特率小数分频值，范围 0-63 */
        uint32_t fbrd;

        /*! 线路控制寄存器 LCRH 备份 */
        uint32_t lineControlRegisterWord;

        /*! 毛刺滤波控制寄存器备份 */
        uint32_t glitchFilterControlWord;

        /*! CPU 中断掩码（EVENT0）备份，@ref DL_UART_INTERRUPT 按位或 */
        uint32_t interruptMask0;

        /*! DMA 接收触发掩码（EVENT1）备份 */
        uint32_t interruptMask1;

        /*! DMA 发送触发掩码（EVENT2）备份 */
        uint32_t interruptMask2;

        /*! 备份有效标志，用户不应修改 */
        bool backupRdy;
    } DL_UART_Main_backupConfig;

#endif /* __MSPM0_HAS_UART_MAIN__ */

#ifdef __MSPM0_HAS_UART_EXTD__

    /**
     * @brief Configuration structure to backup UART Extend peripheral state before
     *        going to STOP/STANDBY mode. Used by
     *        @ref DL_UART_Extend_saveConfiguration and
     *        @ref DL_UART_Extend_restoreConfiguration
     *        UART Extend 外设备份配置结构体，用于 STOP/STANDBY 前保存状态
     */
    typedef struct
    {
        /*! CTL0 控制寄存器备份值 */
        uint32_t controlWord;

        /*! 时钟源选择备份，取值见 @ref DL_UART_CLOCK */
        uint32_t clockSel;

        /*! 分频比备份，取值见 @ref DL_UART_CLOCK_DIVIDE_RATIO */
        uint32_t divideRatio;

        /*! 线路控制寄存器 LCRH 备份 */
        uint32_t lineControlRegisterWord;

        /*! FIFO 中断阈值寄存器备份 */
        uint32_t interruptFifoLevelSelectWord;

        /*! 波特率整数分频值，范围 0-65535 */
        uint32_t ibrd;

        /*! 波特率小数分频值，范围 0-63 */
        uint32_t fbrd;

        /*! 毛刺滤波控制寄存器备份 */
        uint32_t glitchFilterControlWord;

        /*! LIN 控制寄存器备份 */
        uint32_t linControlWord;

        /*! IrDA 控制寄存器备份 */
        uint32_t irdaControlWord;

        /*! 地址掩码备份（9 位/空闲线/DALI 模式用） */
        uint32_t addressMask;

        /*! 地址值备份（9 位/空闲线/DALI 模式用） */
        uint32_t address;

        /*! CPU 中断掩码（EVENT0）备份 */
        uint32_t interruptMask0;

        /*! DMA 接收触发掩码（EVENT1）备份 */
        uint32_t interruptMask1;

        /*! DMA 发送触发掩码（EVENT2）备份 */
        uint32_t interruptMask2;

        /*! 备份有效标志，用户不应修改 */
        bool backupRdy;
    } DL_UART_Extend_backupConfig;

#endif /* __MSPM0_HAS_UART_EXTD__ */

    /**
     *  @brief      Initialize the UART peripheral
     *
     *  Initializes all the common configurable options for the UART peripheral. Any
     *  other custom configuration can be done after calling this API. The UART is
     *  not enabled in this API.
     *  中文说明：完成 UART 关键参数初始化，但不在本接口内使能外设。
     *
     *  @param[in]  uart    Pointer to the register overlay for the peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  config  Configuration for UART peripheral
     *                      参数：UART 初始化配置结构体。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_UART_init(UART_Regs *uart, const DL_UART_Config *config);

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the UART
     *        使能 UART 外设电源，在配置寄存器前必须先调用。
     *
     * @param uart        Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enablePower(UART_Regs *uart)
    {
        uart->GPRCM.PWREN = (UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the UART
     *        禁用 UART 外设电源，之后寄存器不可读写。
     *
     * @param uart        Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disablePower(UART_Regs *uart)
    {
        uart->GPRCM.PWREN = (UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the UART
     *        is enabled
     *        查询 UART 外设电源是否已使能。
     *
     * @param uart        Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     * @return true if peripheral register access is enabled, 已使能
     * @return false if peripheral register access is disabled, 未使能
     */
    __STATIC_INLINE bool DL_UART_isPowerEnabled(const UART_Regs *uart)
    {
        return ((uart->GPRCM.PWREN & UART_PWREN_ENABLE_MASK) ==
                UART_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Resets uart peripheral
     *        复位 UART 外设。
     *
     * @param uart        Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_reset(UART_Regs *uart)
    {
        uart->GPRCM.RSTCTL =
            (UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETSTKYCLR_CLR |
             UART_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief Returns if uart peripheral was reset
     *        查询 UART 外设是否已复位。
     *
     * @param uart        Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     * @return true if peripheral was reset, 已复位
     * @return false if peripheral wasn't reset, 未复位
     */
    __STATIC_INLINE bool DL_UART_isReset(const UART_Regs *uart)
    {
        return ((uart->GPRCM.STAT & UART_GPRCM_STAT_RESETSTKY_MASK) ==
                UART_GPRCM_STAT_RESETSTKY_RESET);
    }

    /**
     *  @brief      Enable the UART peripheral
     *              使能 UART 外设。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enable(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_ENABLE_ENABLE;
    }

    /**
     *  @brief      Checks if the UART peripheral is enabled
     *              查询 UART 外设是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the enabled status of the UART
     *              返回值：使能状态。
     *
     *  @retval     true  The UART peripheral is enabled, 已使能
     *  @retval     false The UART peripheral is disabled, 未使能
     */
    __STATIC_INLINE bool DL_UART_isEnabled(const UART_Regs *uart)
    {
        return ((uart->CTL0 & UART_CTL0_ENABLE_MASK) == UART_CTL0_ENABLE_ENABLE);
    }

    /**
     *  @brief      Disable the UART peripheral
     *              禁用 UART 外设。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disable(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_ENABLE_MASK);
    }

    /**
     *  @brief      Configure UART source clock
     *  @details    功能：配置 UART 时钟源与分频值。
     *
     *  @param[in]  uart    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                       @ref DL_UART_ClockConfig.
     *                       参数：时钟配置结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_UART_setClockConfig(
        UART_Regs *uart, const DL_UART_ClockConfig *config);

    /**
     *  @brief      Get UART source clock configuration
     *  @details    功能：读取 UART 当前时钟源与分频配置。
     *
     *  @param[in]  uart    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                      @ref DL_UART_ClockConfig.
     *                      参数：用于回填配置的结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_UART_getClockConfig(
        const UART_Regs *uart, DL_UART_ClockConfig *config);

    /**
     *  @brief      Configure the baud rate
     *
     *  Given the target baud rate and the frequency of the UART clock source, this
     *  API determines and sets the recommended oversampling setting, and then
     *  calculates and sets the required baud rate divisors.
     *
     * The oversampling rate that will be set is the highest possible oversampling
     * rate given the target baud rate and UART clock frequency.
     *
     * If the user wishes to avoid having the CPU calculate the baud rate divisors
     * or not use the recommended calculated values, the user can call
     * @ref DL_UART_setOversampling and @ref DL_UART_setBaudRateDivisor directly.
     * 中文说明：根据输入时钟自动选择过采样率并计算整数/小数分频值。
     *
     *  @param[in]  uart       Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  clockFreq  The clock frequency in Hz of the UART clock source
     *                         参数：UART 时钟频率（Hz）。
     *  @param[in]  baudRate   The target baud rate
     *                         参数：目标波特率（bps）。
     *
     *  @return     返回值：无。
     *
     * @sa          DL_UART_setOversampling
     * @sa          DL_UART_setBaudRateDivisor
     * @details    功能：参见上方英文说明。
     */
    void DL_UART_configBaudRate(
        UART_Regs *uart, uint32_t clockFreq, uint32_t baudRate);

    /**
     *  @brief      Set the oversampling rate
     *              设置过采样率。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  rate  The oversampling rate to use.
     *                    One of @ref DL_UART_OVERSAMPLING_RATE
     *                    参数：过采样率。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_setOversampling(
        UART_Regs *uart, DL_UART_OVERSAMPLING_RATE rate)
    {
        DL_Common_updateReg(&uart->CTL0, (uint32_t)rate, UART_CTL0_HSE_MASK);
    }

    /**
     *  @brief      Get the oversampling rate
     *              获取当前过采样率。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The current oversampling rate
     *              返回值：当前过采样率。
     *
     *  @retval     One of @ref DL_UART_OVERSAMPLING_RATE
     *
     */
    __STATIC_INLINE DL_UART_OVERSAMPLING_RATE DL_UART_getOversampling(
        const UART_Regs *uart)
    {
        uint32_t rate = uart->CTL0 & UART_CTL0_HSE_MASK;

        return (DL_UART_OVERSAMPLING_RATE)(rate);
    }

    /**
     *  @brief      Enable loopback mode
     *              使能回环模式，TXD 输出内部连接到 RXD 输入。
     *
     *  Enables the loopback mode. When enabled, the UARTxTXD path is fed through
     *  the UARTxRXD path.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_enableLoopbackMode(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_LBE_ENABLE;
    }

    /**
     *  @brief      Check if loopback mode is enabled
     *              查询回环模式是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of loopback mode
     *              返回值：回环模式状态。
     *
     *  @retval     true  Loopback mode is enabled
     *  @retval     false Loopback mode is disabled
     */
    __STATIC_INLINE bool DL_UART_isLoopbackModeEnabled(const UART_Regs *uart)
    {
        return ((uart->CTL0 & UART_CTL0_LBE_MASK) == UART_CTL0_LBE_ENABLE);
    }

    /**
     *  @brief      Disable loopback mode
     *              禁用回环模式。
     *
     *  Disables the loopback mode. When disabled, the UARTxTXD path is not fed through
     *  the UARTxRXD path.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_disableLoopbackMode(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_LBE_MASK);
    }

    /**
     *  @brief      Set the direction of the UART communication
     *              设置 UART 收发方向。
     *
     *  If the UART is disabled in the middle of a transmit or receive, it
     *  completes the current character before stopping.
     *
     *  @param[in]  uart       Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  direction  Direction to set UART communication to.
     *                         One of @ref DL_UART_DIRECTION.
     *                         参数：收发方向。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     *
     */
    __STATIC_INLINE void DL_UART_setDirection(
        UART_Regs *uart, DL_UART_DIRECTION direction)
    {
        DL_Common_updateReg(&uart->CTL0, (uint32_t)direction,
                            UART_CTL0_TXE_MASK | UART_CTL0_RXE_MASK);
    }

    /**
     *  @brief      Get the direction of the UART communication
     *              获取当前 UART 收发方向。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The direction of UART communication.
     *              返回值：收发方向。
     *
     *  @retval     One of @ref DL_UART_DIRECTION
     */
    __STATIC_INLINE DL_UART_DIRECTION DL_UART_getDirection(const UART_Regs *uart)
    {
        uint32_t direction =
            uart->CTL0 & (UART_CTL0_TXE_MASK | UART_CTL0_RXE_MASK);

        return (DL_UART_DIRECTION)(direction);
    }

    /**
     *  @brief      Enable majority voting control
     *              使能多数表决采样，用 3 个中心采样点投票决定位值。
     *
     *  When enabled, the three center bits are used to determine received sample
     *  value. The value corresponding to at least two of the three samples is
     *  considered to be the received value.In case of error (i.e. all 3 bits are
     *  not the same), noise error is detected and bits RIS.NERR and register
     *  RXDATA.NERR are set.
     *
     *  When enabled with oversampling of 16, samples 7, 8, and 9 are
     *  majority voted to decide the sampled bit value.
     *
     *  When enabled with oversampling of 8, samples 3, 4, and 5 are majority
     *  voted to decide the sampled bit value. The value corresponding to at least 2
     *  of the 3 samples is considered to be the received value.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     *
     *  @sa          DL_UART_setOversampling
     */
    __STATIC_INLINE void DL_UART_enableMajorityVoting(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_MAJVOTE_ENABLE;
    }

    /**
     *  @brief      Check if majority voting is enabled
     *              查询多数表决采样是否已使能。
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @return     The status of majority control feature
     *              返回值：多数表决状态。
     *
     *  @retval     true  Majority voting is enabled
     *  @retval     false Majority voting is disabled
     */
    __STATIC_INLINE bool DL_UART_isMajorityVotingEnabled(const UART_Regs *uart)
    {
        return ((uart->CTL0 & UART_CTL0_MAJVOTE_MASK) == UART_CTL0_MAJVOTE_ENABLE);
    }

    /**
     *  @brief      Disable majority voting control
     *              禁用多数表决采样，仅取单次采样值。
     *
     *  When disabled, only a single sample of the received bit of is taken.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_disableMajorityVoting(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_MAJVOTE_MASK);
    }

    /**
     *  @brief      Enable most significant bit (MSB) first
     *              使能 MSB 先发模式，影响收发两个方向的位序。
     *
     *  When enabled, the most significant bit (MSB) is sent first in the protocol
     *  packet. This bit has effect on both the way the protocol byte is
     *  transmitted and received.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_enableMSBFirst(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_MSBFIRST_ENABLE;
    }

    /**
     *  @brief      Check if most significant bit (MSB) first is enabled
     *              查询 MSB 先发模式是否已使能。
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @return     The status of majority control feature
     *              返回值：MSB 先发状态。
     *
     *  @retval     true  MSB first is enabled
     *  @retval     false MSB first is disabled
     */
    __STATIC_INLINE bool DL_UART_isMSBFirstEnabled(const UART_Regs *uart)
    {
        return (
            (uart->CTL0 & UART_CTL0_MSBFIRST_MASK) == UART_CTL0_MSBFIRST_ENABLE);
    }

    /**
     *  @brief      Disable most significant bit (MSB) first
     *              禁用 MSB 先发模式，恢复默认 LSB 先发。
     *
     *  When disabled, the least significant bit (LSB) is sent first in the protocol
     *  packet. This bit has effect on both the way the protocol byte is
     *  transmitted and received.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_disableMSBFirst(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_MSBFIRST_MASK);
    }

    /**
     *  @brief      Enable control of the TXD pin
     *              使能 TXD 引脚手动控制（需先禁用发送）。
     *
     *  When enabled, the TXD pin can be controlled by the TXD_OUT bit. The UART
     *  transmit section must first be disabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     *
     *  @sa          DL_UART_setDirection
     *  @sa          DL_UART_setTXDPin
     */
    __STATIC_INLINE void DL_UART_enableTransmitPinManualControl(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_TXD_OUT_EN_ENABLE;
    }

    /**
     *  @brief      Check if control of the TXD pin is enabled
     *              查询 TXD 引脚手动控制是否已使能。
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @return     The status of control of the TXD pin
     *              返回值：TXD 手动控制状态。
     *
     *  @retval     true  Control of the TXD pin is enabled
     *  @retval     false Control of the TXD pin is disabled
     */
    __STATIC_INLINE bool DL_UART_isTransmitPinManualControlEnabled(
        const UART_Regs *uart)
    {
        return ((uart->CTL0 & UART_CTL0_TXD_OUT_EN_MASK) ==
                UART_CTL0_TXD_OUT_EN_ENABLE);
    }

    /**
     *  @brief      Disable control of the TXD pin
     *              禁用 TXD 引脚手动控制。
     *
     *  When disabled, the TXD pin can not be controlled by the TXD_OUT bit
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_disableTransmitPinManualControl(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_TXD_OUT_EN_MASK);
    }

    /**
     *  @brief      Set the output of the TXD pin
     *              设置 TXD 引脚手动输出电平。
     *
     *  Control the output transmit data pin only when TXD_OUT_EN is enabled and
     *  TXE is disabled.
     *
     *  The TXD pin is set to manual control if it hadn't been
     *  previously set.
     *
     *  @param[in]  uart       Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  txdOutVal  Value to set the TXD pin output to.
     *                         One of @ref DL_UART_TXD_OUT
     *                         参数：TXD 输出电平。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     *
     *  @sa          DL_UART_disableTransmitPinManualControl
     *  @sa          DL_UART_enableTransmitPinManualControl
     */
    __STATIC_INLINE void DL_UART_setTransmitPinManualOutput(
        UART_Regs *uart, DL_UART_TXD_OUT txdOutVal)
    {
        DL_Common_updateReg(&uart->CTL0,
                            UART_CTL0_TXD_OUT_EN_ENABLE | (uint32_t)txdOutVal,
                            UART_CTL0_TXD_OUT_EN_MASK | UART_CTL0_TXD_OUT_MASK);
    }

    /**
     *  @brief      Get the output value of the TXD pin
     *              获取 TXD 引脚当前手动输出电平。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The value of the TXD pin
     *              返回值：TXD 输出电平。
     *
     *  @retval     One of @ref DL_UART_TXD_OUT
     */
    __STATIC_INLINE DL_UART_TXD_OUT DL_UART_getTransmitPinManualOutput(
        const UART_Regs *uart)
    {
        uint32_t txdOutVal = uart->CTL0 & UART_CTL0_TXD_OUT_MASK;

        return (DL_UART_TXD_OUT)(txdOutVal);
    }

    /**
     *  @brief      Enable Manchester encoding
     *              使能 Manchester 编解码。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableManchesterEncoding(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_MENC_ENABLE;
    }

    /**
     *  @brief      Disable Manchester encoding
     *              禁用 Manchester 编解码。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableManchesterEncoding(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_MENC_MASK);
    }

    /**
     *  @brief      Check if Manchester encoding is enabled
     *              查询 Manchester 编解码是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of Manchester encode
     *              返回值：Manchester 编解码状态。
     *
     *  @retval     true  Manchester encoding is enabled
     *  @retval     false Manchester encoding is disabled
     */
    __STATIC_INLINE bool DL_UART_isManchesterEncodingEnabled(const UART_Regs *uart)
    {
        return ((uart->CTL0 & UART_CTL0_MENC_MASK) == UART_CTL0_MENC_ENABLE);
    }

    /**
     *  @brief      Set the communication mode/protocol to use
     *              设置通信模式（UART/RS485/空闲线/9位等）。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  mode  Value to set the UART communication protocol to.
     *                     One of @ref DL_UART_MODE
     *                    参数：通信模式。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_setCommunicationMode(
        UART_Regs *uart, DL_UART_MODE mode)
    {
        DL_Common_updateReg(&uart->CTL0, (uint32_t)mode, UART_CTL0_MODE_MASK);
    }

    /**
     *  @brief      Get the communication mode/protocol being used
     *              获取当前通信模式。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The communication mode/protocol being used by the UART
     *              返回值：当前通信模式。
     *
     *  @retval     One of @ref DL_UART_MODE
     */
    __STATIC_INLINE DL_UART_MODE DL_UART_getCommunicationMode(
        const UART_Regs *uart)
    {
        uint32_t mode = uart->CTL0 & UART_CTL0_MODE_MASK;

        return (DL_UART_MODE)(mode);
    }

    /**
     *  @brief      Set the flow control configuration
     *              设置流控制模式（RTS/CTS/两者/无）。
     *
     *  @param[in]  uart    Pointer to the register overlay for the peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  config  The flow control configuration to use.
     *                      One of @ref DL_UART_FLOW_CONTROL.
     *                      参数：流控制配置。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_setFlowControl(
        UART_Regs *uart, DL_UART_FLOW_CONTROL config)
    {
        DL_Common_updateReg(&uart->CTL0, (uint32_t)config,
                            UART_CTL0_RTSEN_MASK | UART_CTL0_CTSEN_MASK);
    }

    /**
     *  @brief      Check the flow control configuration
     *              获取当前流控制配置。
     *
     *  @param[in]  uart  Pointer to the register overlay for the
     *                    peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The flow control configuration
     *              返回值：流控制配置。
     *
     *  @retval     One of @ref DL_UART_FLOW_CONTROL values
     */
    __STATIC_INLINE DL_UART_FLOW_CONTROL DL_UART_getFlowControl(
        const UART_Regs *uart)
    {
        uint32_t config =
            uart->CTL0 & (UART_CTL0_RTSEN_MASK | UART_CTL0_CTSEN_MASK);

        return (DL_UART_FLOW_CONTROL)(config);
    }

    /**
     *  @brief      Set the request to send output signal
     *              手动设置 RTS 输出信号电平。
     *
     *  The RTS output signal indicates the state of the RX FIFO, and is
     *  linked to the programmable receive FIFO threshold levels. When RTS flow
     *  control is enabled, the RTS signal is asserted (low) when the data in the
     *  RX FIFO is less than the threshold level. When the RX FIFO threshold level
     *  is reached, the RTS signal is deasserted (high). The RTS signal is
     *  reasserted (low) when data has been read out  of the RX FIFO so it is less
     *  than the threshold.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  val   The RTS output signal. One of @ref DL_UART_RTS
     *                    参数：RTS 输出电平。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     *
     *  @sa         DL_UART_setTXFIFOThreshold
     */
    __STATIC_INLINE void DL_UART_setRTSOutput(UART_Regs *uart, DL_UART_RTS val)
    {
        DL_Common_updateReg(&uart->CTL0, (uint32_t)val, UART_CTL0_RTS_MASK);
    }

    /**
     *  @brief      Get the request to send output signal
     *              获取 RTS 输出信号当前状态。
     *
     *  The RTS output signal indicates the state of the RX FIFO, and is
     *  linked to the programmable receive FIFO threshold levels. When RTS flow
     *  control is enabled, the RTS signal is asserted (low) when the data in the
     *  RX FIFO is less than the threshold level. When the RX FIFO threshold level
     *  is reached, the RTS signal is deasserted (high). The RTS signal is
     *  reasserted (low) when data has been read out  of the RX FIFO so it is less
     *  than the threshold.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The RTS signal status
     *              返回值：RTS 信号状态。
     *
     *  @retval     One of @ref DL_UART_RTS
     *
     *  @sa         DL_UART_setTXFIFOThreshold
     */
    __STATIC_INLINE DL_UART_RTS DL_UART_getRTSOutput(const UART_Regs *uart)
    {
        uint32_t val = uart->CTL0 & UART_CTL0_RTS_MASK;

        return (DL_UART_RTS)(val);
    }

    /**
     *  @brief      Enable FIFOs
     *              使能 TX/RX FIFO 缓冲。
     *
     *  Enables the transmit and receive FIFO buffers.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @sa         DL_UART_init
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_enableFIFOs(UART_Regs *uart)
    {
        uart->CTL0 |= UART_CTL0_FEN_ENABLE;
    }

    /**
     *  @brief      Disable FIFOs
     *              禁用 FIFO，接收器退化为 1 字节深度。
     *
     *  Disables the transmit and receive FIFO buffers. The receiver will now
     *  only hold 1-byte of data.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @pre  If the UART has already been enabled, then it must be made ready for
     *        configuration by first calling @ref DL_UART_changeConfig
     *  @post If @ref DL_UART_changeConfig was called, then the UART must be
     *        re-enabled by calling @ref DL_UART_enable
     */
    __STATIC_INLINE void DL_UART_disableFIFOs(UART_Regs *uart)
    {
        uart->CTL0 &= ~(UART_CTL0_FEN_MASK);
    }

    /**
     *  @brief      Check if FIFOs are enabled
     *              查询 FIFO 是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of the FIFOs
     *              返回值：FIFO 使能状态。
     *
     *  @retval     true  FIFOs are enabled
     *  @retval     false FIFOs are disabled
     */
    __STATIC_INLINE bool DL_UART_isFIFOsEnabled(const UART_Regs *uart)
    {
        return ((uart->CTL0 & UART_CTL0_FEN_MASK) == UART_CTL0_FEN_ENABLE);
    }

    /**
     *  @brief      Enable send break (for LIN protocol)
     *              使能 LIN 间隔信号发送（持续低电平至少 2 帧）。
     *
     *  When enabled, a low level is continually output on the TXD signal after completing
     *  transmission of the current character. For the proper execution of the
     *  break command, software must set this bit for at least two frames (character periods).
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableLINSendBreak(UART_Regs *uart)
    {
        uart->LCRH |= UART_LCRH_BRK_ENABLE;
    }

    /**
     *  @brief      Disable send break
     *              禁用 LIN 间隔信号发送。
     *
     *  When disabled, a low level is not continually output on the TXD signal
     *  after completing transmission of the current character.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINSendBreak(UART_Regs *uart)
    {
        uart->LCRH &= ~(UART_LCRH_BRK_MASK);
    }

    /**
     *  @brief      Check if send break is enabled
     *              查询 LIN 间隔信号发送是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of send break
     *              返回值：间隔信号发送状态。
     *
     *  @retval     true  Send break is enabled
     *  @retval     false Send break is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINSendBreakEnabled(const UART_Regs *uart)
    {
        return ((uart->LCRH & UART_LCRH_BRK_MASK) == UART_LCRH_BRK_ENABLE);
    }

    /**
     *  @brief      Check if parity is enabled
     *              查询校验功能是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of parity
     *              返回值：校验使能状态。
     *
     *  @retval     true  Parity is enabled
     *  @retval     false Parity is disabled
     */
    __STATIC_INLINE bool DL_UART_isParityEnabled(const UART_Regs *uart)
    {
        return ((uart->LCRH & UART_LCRH_PEN_MASK) == UART_LCRH_PEN_ENABLE);
    }

    /**
     *  @brief      Set the parity mode
     *              设置校验模式（偶/奇/固定1/固定0/无）。
     *
     *  For 9-bit UART mode transmissions, the parity mode affects the address
     *  byte and data byte indication (9th bit). If DL_UART_PARITY_EVEN or
     *  DL_UART_PARITY_STICK_ZERO is enabled, then the transferred byte is an
     *  address byte with Parity bit '1'. If DL_UART_PARITY_EVEN or
     *  DL_UART_PARITY_STICK_ZERO is not enabled, then the transferred byte is an
     *  address byte with Parity bit '0'.
     *
     *  @param[in]  uart    Pointer to the register overlay for the peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  parity  Parity mode to set UART to.
     *                      One of @ref DL_UART_PARITY
     *                      参数：校验模式。
     */
    __STATIC_INLINE void DL_UART_setParityMode(
        UART_Regs *uart, DL_UART_PARITY parity)
    {
        DL_Common_updateReg(&uart->LCRH, (uint32_t)parity,
                            (UART_LCRH_PEN_MASK | UART_LCRH_EPS_MASK | UART_LCRH_SPS_MASK));
    }

    /**
     *  @brief      Get parity mode
     *              获取当前校验模式。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The current parity mode being used
     *              返回值：当前校验模式。
     *
     *  @retval     One of @ref DL_UART_PARITY
     *
     */
    __STATIC_INLINE DL_UART_PARITY DL_UART_getParityMode(const UART_Regs *uart)
    {
        uint32_t parity = uart->LCRH & (UART_LCRH_PEN_MASK | UART_LCRH_EPS_MASK |
                                        UART_LCRH_SPS_MASK);

        return (DL_UART_PARITY)(parity);
    }

    /**
     *  @brief      Set the number of stop bits
     *              设置停止位数量（ISO7816 模式强制 2 位）。
     *
     *  When in 7816 smart code mode (DL_UART_MODE_SMART_CARD mode), the number of
     *  stop bits is forced to 2
     *
     *  @param[in]  uart         Pointer to the register overlay for the peripheral
     *                           参数：UART 外设寄存器基址。
     *  @param[in]  numStopBits  The number of stop bits transmitted.
     *                           One of @ref DL_UART_STOP_BITS
     *                           参数：停止位数量。
     */
    __STATIC_INLINE void DL_UART_setStopBits(
        UART_Regs *uart, DL_UART_STOP_BITS numStopBits)
    {
        DL_Common_updateReg(
            &uart->LCRH, (uint32_t)numStopBits, UART_LCRH_STP2_MASK);
    }

    /**
     *  @brief      Get the number of stop bits
     *              获取当前停止位数量。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The number of stop bits transmitted.
     *              返回值：停止位数量。
     *
     *  @retval     One of @ref DL_UART_STOP_BITS
     */
    __STATIC_INLINE DL_UART_STOP_BITS DL_UART_getStopBits(const UART_Regs *uart)
    {
        uint32_t numStopBits = uart->LCRH & UART_LCRH_STP2_MASK;

        return (DL_UART_STOP_BITS)(numStopBits);
    }

    /**
     *  @brief      Set the word length
     *              设置数据位长度（5/6/7/8 位）。
     *
     *  @param[in]  uart        Pointer to the register overlay for the peripheral
     *                          参数：UART 外设寄存器基址。
     *  @param[in]  wordLength  The number of data bits transmitted or received in
     *                           a frame. One of @ref DL_UART_WORD_LENGTH
     *                          参数：数据位长度。
     */
    __STATIC_INLINE void DL_UART_setWordLength(
        UART_Regs *uart, DL_UART_WORD_LENGTH wordLength)
    {
        DL_Common_updateReg(
            &uart->LCRH, (uint32_t)wordLength, UART_LCRH_WLEN_MASK);
    }

    /**
     *  @brief      Get the word length
     *              获取当前数据位长度。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The length of the data bits transmitted or received in a frame
     *              返回值：数据位长度。
     *
     *  @retval     One of @ref DL_UART_WORD_LENGTH
     */
    __STATIC_INLINE DL_UART_WORD_LENGTH DL_UART_getWordLength(
        const UART_Regs *uart)
    {
        uint32_t wordLength = uart->LCRH & UART_LCRH_WLEN_MASK;

        return (DL_UART_WORD_LENGTH)(wordLength);
    }

    /**
     *  @brief      Send idle pattern
     *              发送 11 位时间的空闲模式，硬件自动清除。
     *
     *  When enabled, a SENDIDLE period of 11 bit times will be sent on the TX
     *  line. The bit is cleared by hardware afterwards.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableSendIdlePattern(UART_Regs *uart)
    {
        uart->LCRH |= UART_LCRH_SENDIDLE_ENABLE;
    }

    /**
     *  @brief      Disable send idle pattern
     *              禁用空闲模式发送。
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableSendIdlePattern(UART_Regs *uart)
    {
        uart->LCRH &= ~(UART_LCRH_SENDIDLE_MASK);
    }

    /**
     *  @brief      Check if send idle pattern is enabled
     *              查询空闲模式发送是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of the send idle pattern
     *              返回值：空闲模式发送状态。
     *
     *  @retval     true  Send idle pattern is enabled
     *  @retval     false Send idle pattern is disabled
     */
    __STATIC_INLINE bool DL_UART_isSendIdlePatternEnabled(const UART_Regs *uart)
    {
        return (
            (uart->LCRH & UART_LCRH_SENDIDLE_MASK) == UART_LCRH_SENDIDLE_ENABLE);
    }

    /**
     *  @brief      Set external driver setup value
     *              设置 RS485 外部驱动建立时间（0-31 个时钟周期）。
     *
     *  Defines the number of UARTclk ticks the signal to control the external
     *  driver for the RS485 will be set before the START bit is sent
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  val   The number of UARTclk ticks the signal before the RS485
     *                    is setup. Value between 0 - 31.
     *                    参数：建立时间。
     */
    __STATIC_INLINE void DL_UART_setExternalDriverSetup(
        UART_Regs *uart, uint32_t val)
    {
        DL_Common_updateReg(&uart->LCRH, val << UART_LCRH_EXTDIR_SETUP_OFS,
                            UART_LCRH_EXTDIR_SETUP_MASK);
    }

    /**
     *  @brief      Get the external driver setup value
     *              获取 RS485 外部驱动建立时间。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return    The number of UARTclk ticks the signal to control the
     *             external driver
     *             返回值：建立时间（0-31）。
     *
     *  @retval     0 - 31 The number of UARTclk ticks
     */
    __STATIC_INLINE uint32_t DL_UART_getExternalDriverSetup(const UART_Regs *uart)
    {
        return ((uart->LCRH &
                 UART_LCRH_EXTDIR_SETUP_MASK) >>
                UART_LCRH_EXTDIR_SETUP_OFS);
    }

    /**
     *  @brief      Set external driver setup hold
     *              设置 RS485 外部驱动保持时间（0-31 个时钟周期）。
     *
     *  Defines the number of UARTclk ticks the signal to control the external
     *  driver for the RS485 will be reset after the beginning of the stop bit.
     *  If 2 STOP bits are enabled, the RS485 will be reset at the beginning of
     *  the 2nd STOP bit.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  val   The number of UARTclk ticks the signal to hold the
     *                    external driver before the RS485 will be reset.
     *                    Value between 0 - 31.
     *                    参数：保持时间。
     */
    __STATIC_INLINE void DL_UART_setExternalDriverHold(
        UART_Regs *uart, uint32_t val)
    {
        DL_Common_updateReg(&uart->LCRH, val << UART_LCRH_EXTDIR_HOLD_OFS,
                            UART_LCRH_EXTDIR_HOLD_MASK);
    }

    /**
     *  @brief      Get the external driver setup hold
     *              获取 RS485 外部驱动保持时间。
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *
     *  @return     The number of UARTclk ticks the signal to hold the
     *              external driver before the RS485 will be reset
     *              返回值：保持时间（0-31）。
     *
     *  @retval     0 - 31 The number of UARTclk ticks
     */
    __STATIC_INLINE uint32_t DL_UART_getExternalDriverHold(const UART_Regs *uart)
    {
        return ((
                    uart->LCRH & UART_LCRH_EXTDIR_HOLD_MASK) >>
                UART_LCRH_EXTDIR_HOLD_OFS);
    }

    /**
     *  @brief      Checks if the UART is busy
     *              查询 UART 是否处于忙状态（正在发送或接收）。
     *
     *  This bit is set as soon as the transmit FIFO or TXDATA register becomes
     *  non-empty (regardless of whether UART is enabled) or if a receive data is
     *  currently ongoing (after the start edge have been detected until a complete
     *  byte, including all stop bits, has been received by the shift register).
     *
     *  In IDLE Line mode the Busy signal also stays set during the idle time
     *  generation.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of the UART Busy bit
     *              返回值：忙状态。
     *
     *  @retval     true   The UART is busy
     *  @retval     false  The UART is not busy
     *
     */
    __STATIC_INLINE bool DL_UART_isBusy(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_BUSY_MASK) == UART_STAT_BUSY_SET);
    }

    /**
     *  @brief      Checks if the RX FIFO is empty
     *              查询 RX FIFO 是否为空。
     *
     *  The meaning of this bit depends on if the FIFOs were enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the empty status of the RX FIFO
     *              返回值：RX FIFO 空状态。
     *
     *  @retval     true   If the FIFO is enabled, the receive FIFO is empty.
     *                     If the FIFO is disabled, the receiver has no data.
     *  @retval     false  The receiver is not empty
     *
     *  @sa         DL_UART_enableFIFOs
     */
    __STATIC_INLINE bool DL_UART_isRXFIFOEmpty(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_RXFE_MASK) == UART_STAT_RXFE_SET);
    }

    /**
     *  @brief      Checks if the RX FIFO is full
     *              查询 RX FIFO 是否已满。
     *
     *  The meaning of this bit depends on if the FIFOs were enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the full status of the RX FIFO
     *              返回值：RX FIFO 满状态。
     *
     *  @retval     true   If the FIFO is enabled, the receive FIFO is full.
     *                     If the FIFO is disabled, the receiver has 1-byte
     *                     of data.
     *  @retval     false  The receiver is not full
     *
     *  @sa         DL_UART_enableFIFOs
     */
    __STATIC_INLINE bool DL_UART_isRXFIFOFull(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_RXFF_MASK) == UART_STAT_RXFF_SET);
    }

    /**
     *  @brief      Checks if the TX FIFO is empty
     *              查询 TX FIFO 是否为空。
     *
     *  The meaning of this bit depends on if the FIFOs were enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the empty status of the TX FIFO
     *              返回值：TX FIFO 空状态。
     *
     *  @retval     true   If the FIFO is enabled, the transmit FIFO is empty.
     *                     If the FIFO is disabled, the transmitter has no data.
     *  @retval     false  The transmitter is not empty
     *
     *  @sa         DL_UART_enableFIFOs
     */
    __STATIC_INLINE bool DL_UART_isTXFIFOEmpty(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_TXFE_MASK) == UART_STAT_TXFE_SET);
    }

    /**
     *  @brief      Checks if the TX FIFO is full
     *              查询 TX FIFO 是否已满。
     *
     *  The meaning of this bit depends on if the FIFOs were enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the full status of the TX FIFO
     *              返回值：TX FIFO 满状态。
     *
     *  @retval     true   If the FIFO is enabled, the transmit FIFO is full.
     *                     If the FIFO is disabled, the transmitter has 1-byte
     *                     of data.
     *  @retval     false  The transmitter is not full
     *
     *  @sa         DL_UART_enableFIFOs
     */
    __STATIC_INLINE bool DL_UART_isTXFIFOFull(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_TXFF_MASK) == UART_STAT_TXFF_SET);
    }

    /**
     *  @brief      Checks if UART is clear to send
     *              查询 CTS 信号是否有效（低电平）。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the status of the CTS signal
     *              返回值：CTS 信号状态。
     *
     *  @retval     true   The CTS signal is asserted (low)
     *  @retval     false  The CTS signal is not asserted (high)
     *
     *  @sa         DL_UART_isClearToSendEnabled
     */
    __STATIC_INLINE bool DL_UART_isClearToSend(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_CTS_MASK) == UART_STAT_CTS_SET);
    }

    /**
     *  @brief      Checks if Idle mode has been detected
     *              查询空闲线多处理器模式中是否检测到空闲状态。
     *
     *  Idle mode has been detected in Idleline-Multiprocessor-Mode. The IDLE bit
     *  is used as an address tag for each block of characters. In idle-line
     *  multiprocessor format, this bit is set when a received character is an
     *  address.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     Returns the status if Idle mode has been detected
     *              返回值：空闲检测状态。
     *
     *  @retval     true  Idle has been detected before last received character
     *  @retval     false Idle has not been detected before last received character
     *
     */
    __STATIC_INLINE bool DL_UART_isIdleModeDetected(const UART_Regs *uart)
    {
        return ((uart->STAT & UART_STAT_IDLE_MASK) == UART_STAT_IDLE_SET);
    }

    /**
     *  @brief      Set the TX FIFO interrupt threshold level
     *              设置 TX FIFO 中断触发阈值。
     *
     *  Select the threshold for the transmit FIFO interrupt. The interrupts are
     *  generated based on a transition through a level rather than being based on
     *  the level. That is, the interrupts are generated when the fill level
     *  progresses through the trigger level. For example, if the transmit trigger
     *  level is set to the half-way mark, the interrupt is triggered when the
     *  transmit FIFO becomes half empty. In other words, if the transmit FIFO was
     *  filled with four characters, the interrupt would trigger once there are
     *  two or less characters after transmitting.
     *
     *  @param[in]  uart       Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  threshold  One of @ref DL_UART_TX_FIFO_LEVEL
     *                         参数：TX FIFO 阈值。
     *
     */
    __STATIC_INLINE void DL_UART_setTXFIFOThreshold(
        UART_Regs *uart, DL_UART_TX_FIFO_LEVEL threshold)
    {
        DL_Common_updateReg(
            &uart->IFLS, (uint32_t)threshold, UART_IFLS_TXIFLSEL_MASK);
    }

    /**
     *  @brief      Get the TX FIFO interrupt threshold level
     *              获取 TX FIFO 中断触发阈值。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The TX FIFO interrupt threshold level
     *              返回值：TX FIFO 阈值。
     *
     *  @retval     One of @ref DL_UART_TX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_UART_TX_FIFO_LEVEL DL_UART_getTXFIFOThreshold(
        const UART_Regs *uart)
    {
        uint32_t threshold = uart->IFLS & UART_IFLS_TXIFLSEL_MASK;

        return (DL_UART_TX_FIFO_LEVEL)(threshold);
    }

    /**
     *  @brief      Set the RX FIFO interrupt threshold level.
     *              设置 RX FIFO 中断触发阈值。The interrupts are
     *  generated based on a transition through a level rather than being based on
     *  the level. That is, the interrupts are generated when the fill level
     *  progresses through the trigger level. For example, if the receive trigger
     *  level is set to the half-way mark, the interrupt is triggered when the
     *  receive FIFO becomes half full. In other words, the interrupt is triggered
     *  after the receive FIFO is filled with two or more characters.
     *
     *  @param[in]  uart       Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  threshold  One of @ref DL_UART_RX_FIFO_LEVEL
     *                         参数：RX FIFO 阈值。
     */
    __STATIC_INLINE void DL_UART_setRXFIFOThreshold(
        UART_Regs *uart, DL_UART_RX_FIFO_LEVEL threshold)
    {
        DL_Common_updateReg(
            &uart->IFLS, (uint32_t)threshold, UART_IFLS_RXIFLSEL_MASK);
    }

    /**
     *  @brief      Get the RX FIFO interrupt threshold level
     *              获取 RX FIFO 中断触发阈值。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The RX FIFO interrupt threshold level
     *              返回值：RX FIFO 阈值。
     *
     *  @retval     One of @ref DL_UART_RX_FIFO_LEVEL
     */
    __STATIC_INLINE DL_UART_RX_FIFO_LEVEL DL_UART_getRXFIFOThreshold(
        const UART_Regs *uart)
    {
        uint32_t threshold = uart->IFLS & UART_IFLS_RXIFLSEL_MASK;

        return (DL_UART_RX_FIFO_LEVEL)(threshold);
    }

    /**
     *  @brief      Set the RX interrupt timeout
     *              设置接收超时值（0-15），超时后即使 FIFO 未达阈值也触发中断。
     *
     *  When an additional character has not been received within the set
     *  timeout, a RX interrupt will still trigger even if the FIFO level has not
     *  been reached. A value of 0 disables this function.
     *
     *  @param[in]  uart     Pointer to the register overlay for the peripheral
     *                       参数：UART 外设寄存器基址。
     *  @param[in]  timeout  Timeout to set the RX interrupt to.
     *                       Value between 0 - 15
     *                       参数：超时值。
     */
    __STATIC_INLINE void DL_UART_setRXInterruptTimeout(
        UART_Regs *uart, uint32_t timeout)
    {
        DL_Common_updateReg(
            &uart->IFLS, timeout << UART_IFLS_RXTOSEL_OFS, UART_IFLS_RXTOSEL_MASK);
    }

    /**
     *  @brief      Get the RX interrupt timeout
     *              获取接收超时值。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The RX interrupt timeout value
     *              返回值：超时值（0-15）。
     *
     *  @retval     0 - 15  The RX interrupt timeout value
     *
     */
    __STATIC_INLINE uint32_t DL_UART_getRXInterruptTimeout(const UART_Regs *uart)
    {
        return ((uart->IFLS & UART_IFLS_RXTOSEL_MASK) >> UART_IFLS_RXTOSEL_OFS);
    }

    /**
     *  @brief      Get Integer Baud-Rate Divisor
     *              获取波特率整数分频值。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The integer component of the baud rate divisor
     *              返回值：整数分频值。
     *
     *  @retval     0 - 65535 The integer baud date divisor
     */
    __STATIC_INLINE uint32_t DL_UART_getIntegerBaudRateDivisor(
        const UART_Regs *uart)
    {
        return (uart->IBRD & UART_IBRD_DIVINT_MASK);
    }

    /**
     *  @brief      Get Fractional Baud-Rate Divisor
     *              获取波特率小数分频值。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The fractional component of the baud rate divisor
     *              返回值：小数分频值。
     *
     *  @retval     0 - 63  The fractional baud date divisor
     */
    __STATIC_INLINE uint32_t DL_UART_getFractionalBaudRateDivisor(
        const UART_Regs *uart)
    {
        return (uart->FBRD & UART_FBRD_DIVFRAC_MASK);
    }

    /**
     *  @brief      Set the baud rate divisor
     *              设置波特率整数和小数分频值。
     *
     *  Set the integer baud rate divisor and fractional baud rate divisor
     *  components of the baud rate divisor
     *
     *  @param[in]  uart               Pointer to the register overlay for the
     *                                 peripheral
     *                                 参数：UART 外设寄存器基址。
     *  @param[in]  integerDivisor     The integer component of the baud rate
     *                                 divisor
     *                                 参数：整数分频值。
     *  @param[in]  fractionalDivisor  The fractional component of the baud rate
     *                                 divisor
     *                                 参数：小数分频值。
     */
    __STATIC_INLINE void DL_UART_setBaudRateDivisor(
        UART_Regs *uart, uint32_t integerDivisor, uint32_t fractionalDivisor)
    {
        DL_Common_updateReg(&uart->IBRD, integerDivisor, UART_IBRD_DIVINT_MASK);
        DL_Common_updateReg(
            &uart->FBRD, fractionalDivisor, UART_FBRD_DIVFRAC_MASK);

        // When updating the baud-rate divisor (UARTIBRD or UARTIFRD),
        // the LCRH register must also be written to (any bit in LCRH can
        // be written to for updating the baud-rate divisor).
        DL_Common_updateReg(
            &uart->LCRH, (uart->LCRH & UART_LCRH_BRK_MASK), UART_LCRH_BRK_MASK);
    }

    /**
     *  @brief      Set the baud rate divisor for IrDA mode
     *              设置 IrDA 模式波特率分频值（含第二级分频）。
     *
     *  Set the integer baud rate divisor and fractional baud rate divisor
     *  components of the baud rate divisor
     *  Divide integerDivisor by clkDivisor2 + 1 as a way of reducing the UART
     *  clock frequency, which in turn reduces the baud rate divisor further
     *  in accordance to IrDA standards
     *
     *  @param[in]  uart               Pointer to the register overlay for the
     *                                 peripheral
     *                                 参数：UART 外设寄存器基址。
     *  @param[in]  integerDivisor     The integer component of the baud rate
     *                                 divisor
     *                                 参数：整数分频值。
     *  @param[in]  fractionalDivisor  The fractional component of the baud rate
     *                                 divisor
     *                                 参数：小数分频值。
     *  @param[in]  clkDivisor2        The additional factor to divide the clock,
     *                                 One of @ref DL_UART_CLOCK_DIVIDE2_RATIO
     *                                 参数：第二级分频比。
     *
     */
    __STATIC_INLINE void DL_UART_setIrDABaudRateDivisor(UART_Regs *uart,
                                                        uint32_t integerDivisor, uint32_t fractionalDivisor,
                                                        DL_UART_CLOCK_DIVIDE2_RATIO clkDivisor2)
    {
        DL_Common_updateReg(&uart->IBRD,
                            (integerDivisor / ((uint32_t)clkDivisor2 + 1)),
                            UART_IBRD_DIVINT_MASK);
        DL_Common_updateReg(
            &uart->FBRD, fractionalDivisor, UART_FBRD_DIVFRAC_MASK);

        // When updating the baud-rate divisor (UARTIBRD or UARTIFRD),
        // the LCRH register must also be written to (any bit in LCRH can
        // be written to for updating the baud-rate divisor).
        DL_Common_updateReg(
            &uart->LCRH, (uart->LCRH & UART_LCRH_BRK_MASK), UART_LCRH_BRK_MASK);
    }

    /**
     *  @brief      Set the pulse width select for the digital glitch suppresion
     *              设置数字毛刺滤波脉宽（0-63 个功能时钟周期）。
     *
     *  Controls the pulse width select for glitch suppression on the RX line.
     *  The glitch suppression values are in terms of functional clocks.
     *
     *  In IrDA mode, this sets the receive filter length. The minimum pulse
     *  length for receive is given by: t(MIN) = (DGFSEL) / f(IRTXCLK)
     *
     *  @param[in]  uart        Pointer to the register overlay for the peripheral
     *                          参数：UART 外设寄存器基址。
     *  @param[in]  pulseWidth  Pulse width select for the glitch suppresion.
     *                          Value between 0 - 63.
     *                          参数：脉宽值。
     */
    __STATIC_INLINE void DL_UART_setDigitalPulseWidth(
        UART_Regs *uart, uint32_t pulseWidth)
    {
        DL_Common_updateReg(&uart->GFCTL, pulseWidth, UART_GFCTL_DGFSEL_MASK);
    }

    /**
     *  @brief      Get the pulse width select for the digital glitch suppresion
     *              获取数字毛刺滤波脉宽。
     *
     *  Gets the pulse width select for glitch suppression on the RX line.
     *  The glitch suppression values are in terms of functional clocks.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The digital glitch suppression pulse width
     *              返回值：脉宽值。
     *
     *  @retval     0 indicating digital glitch suppression is disabled
     *  @retval     1 - 63 the digital glitch suppression pulse width
     */
    __STATIC_INLINE uint32_t DL_UART_getDigitalPulseWidth(const UART_Regs *uart)
    {
        return (uart->GFCTL & UART_GFCTL_DGFSEL_MASK);
    }

    /**
     *  @brief      Writes data into the TX FIFO to transmit
     *              向 TX FIFO 写入 1 字节数据（不检查状态）。
     *
     *  Puts the data into the TX FIFO without checking it's status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  For transmitted data, if the FIFO is enabled, data written to this
     *  location is pushed onto the transmit FIFO. If the FIFO is disabled,
     *  data is stored in the 1-byte deep transmitter.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  data  The data to transmit
     *                    参数：待发送字节。
     *
     *
     *  @sa         DL_UART_transmitDataBlocking
     *  @sa         DL_UART_transmitDataCheck
     */
    __STATIC_INLINE void DL_UART_transmitData(UART_Regs *uart, uint8_t data)
    {
        uart->TXDATA = data;
    }

    /**
     *  @brief      Reads data from the RX FIFO
     *              从 RX FIFO 读取 1 字节数据（不检查状态）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  @note:      As a result of reading the RX FIFO data, the corresponding
     *              error status in the RXDATA register (OVRERR, BRKERR, PARERR,
     *              FRMERR bits) will be dropped.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：接收到的字节。
     *
     *  @sa         DL_UART_receiveDataBlocking
     *  @sa         DL_UART_receiveDataCheck
     */
    __STATIC_INLINE uint8_t DL_UART_receiveData(const UART_Regs *uart)
    {
        return ((uint8_t)(uart->RXDATA & UART_RXDATA_DATA_MASK));
    }

    /**
     *  @brief      Gets the status of the error flags of the received data
     *              获取接收数据的错误状态标志。
     *
     *  @note:      As a result of reading the error status, the corresponding
     *              RX FIFO data in the RXDATA.DATA bit field will be dropped.
     *
     *  @param[in]  uart       Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  errorMask  Bit mask of error flags to check. Bitwise OR of
     *                         @ref DL_UART_ERROR.
     *                         参数：错误掩码。
     *
     *  @return     The status of the requested UART error flags
     *              返回值：错误标志。
     *
     *  @retval     Bitwise OR of @ref DL_UART_ERROR values
     */
    __STATIC_INLINE uint32_t DL_UART_getErrorStatus(
        const UART_Regs *uart, uint32_t errorMask)
    {
        return (uart->RXDATA & errorMask);
    }

    /**
     *  @brief      Set the LIN counter value
     *              设置 LIN 计数器值。
     *
     *  The LIN counter is a 16 bit up counter clocked by the functional clock of
     *  the UART
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *  @param[in]  value  Pulse width select for the glitch suppresion.
     *                     参数：计数器值。
     */
    __STATIC_INLINE void DL_UART_setLINCounterValue(
        UART_Regs *uart, uint16_t value)
    {
        DL_Common_updateReg(&uart->LINCNT, value, UART_LINCNT_VALUE_MASK);
    }

    /**
     *  @brief      Get the LIN counter value
     *              获取 LIN 计数器当前值。
     *
     *  The LIN counter is a 16 bit up counter clocked by the module clock of
     *  the UART
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The clock counter value
     *              返回值：计数器值。
     *
     *  @retval     0 - 65535  The clock counter value
     *
     */
    __STATIC_INLINE uint16_t DL_UART_getLINCounterValue(const UART_Regs *uart)
    {
        return ((uint16_t)(uart->LINCNT & UART_LINCNT_VALUE_MASK));
    }

    /**
     *  @brief      Enable the LIN counter
     *              使能 LIN 计数器。
     *
     *  The LIN counter will only count when enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableLINCounter(UART_Regs *uart)
    {
        uart->LINCTL |= UART_LINCTL_CTRENA_ENABLE;
    }

    /**
     *  @brief      Check if the LIN counter is enabled
     *              查询 LIN 计数器是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of the LIN counter
     *              返回值：LIN 计数器状态。
     *
     *  @retval     true   LIN counter is enabled
     *  @retval     false  LIN counter is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINCounterEnabled(const UART_Regs *uart)
    {
        return (
            (uart->LINCTL & UART_LINCTL_CTRENA_MASK) == UART_LINCTL_CTRENA_ENABLE);
    }

    /**
     *  @brief      Disable the LIN counter
     *              禁用 LIN 计数器。
     *
     *  LIN counter will only count when enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINCounter(UART_Regs *uart)
    {
        uart->LINCTL &= ~(UART_LINCTL_CTRENA_MASK);
    }

    /**
     *  @brief   Enable LIN counter clear and start counting on falling edge of RXD
     *           使能 RXD 下降沿清除并启动 LIN 计数器。
     *
     *  When enabled, the counter is set to 0 and starts counting on the LIN counter
     *  on a falling edge of RXD.
     *
     *  The LIN counter will only count when it is enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @sa  DL_UART_enableLINCounter
     */
    __STATIC_INLINE void DL_UART_enableLINCounterClearOnFallingEdge(
        UART_Regs *uart)
    {
        uart->LINCTL |= UART_LINCTL_ZERONE_ENABLE;
    }

    /**
     *  @brief      Check if LIN counting on falling edge of RXD is enabled
     *              查询 RXD 下降沿清除计数器功能是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of counting on falling edge of RXD
     *              返回值：下降沿清除状态。
     *
     *  @retval     true   Counting on falling edge is enabled
     *  @retval     false  Counting on falling edge is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINCounterClearOnFallingEdge(
        const UART_Regs *uart)
    {
        return (
            (uart->LINCTL & UART_LINCTL_ZERONE_MASK) == UART_LINCTL_ZERONE_ENABLE);
    }

    /**
     *  @brief      Disable LIN counting on falling edge of RXD
     *              禁用 RXD 下降沿清除计数器功能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINCounterClearOnFallingEdge(
        UART_Regs *uart)
    {
        uart->LINCTL &= ~(UART_LINCTL_ZERONE_MASK);
    }

    /**
     *  @brief      Enable LIN counter incrementing while RXD signal is low
     *              使能 RXD 低电平期间 LIN 计数器持续递增。
     *
     *  When LIN counter is enabled and the signal on RXD is low, the counter
     *  increments
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @sa  DL_UART_enableLINCounter
     */
    __STATIC_INLINE void DL_UART_enableLINCountWhileLow(UART_Regs *uart)
    {
        uart->LINCTL |= UART_LINCTL_CNTRXLOW_ENABLE;
    }

    /**
     *  @brief   Check if LIN counter increments while RXD signal is low is enabled
     *           查询 RXD 低电平计数功能是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of counter increments while RXD signal is low
     *              返回值：低电平计数状态。
     *
     *  @retval     true   Counter increments while RXD signal is low is enabled
     *  @retval     false  Counter increments while RXD signal is low is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINCountWhileLowEnabled(const UART_Regs *uart)
    {
        return ((uart->LINCTL & UART_LINCTL_CNTRXLOW_MASK) ==
                UART_LINCTL_CNTRXLOW_ENABLE);
    }

    /**
     *  @brief      Disable LIN counter increments while RXD signal is low
     *              禁用 RXD 低电平计数功能。
     *
     *  LIN counter will not increment while the RXD signal is low
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINCountWhileLow(UART_Regs *uart)
    {
        uart->LINCTL &= ~(UART_LINCTL_CNTRXLOW_MASK);
    }

    /**
     *  @brief      Enable capture of the LIN counter on a falling edge
     *              使能 RXD 下降沿捕获 LIN 计数器值到 LINC0。
     *
     *  When enabled, the LIN counter value is captured to the LINC0 register on
     *  each falling RXD edge. A LINC0 interrupt is triggered when enabled.
     *  Disables counter compare match mode if enabled.
     *
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @sa  DL_UART_configLINMode
     */
    __STATIC_INLINE void DL_UART_enableLINFallingEdgeCapture(UART_Regs *uart)
    {
        DL_Common_updateReg(&uart->LINCTL,
                            UART_LINCTL_LINC0CAP_ENABLE | UART_LINCTL_LINC0_MATCH_DISABLE,
                            UART_LINCTL_LINC0CAP_MASK | UART_LINCTL_LINC0_MATCH_MASK);
    }

    /**
     *  @brief      Check status of capture of LIN counter on a falling edge
     *              查询 RXD 下降沿捕获功能是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of capture the LIN counter on a falling edge
     *              返回值：下降沿捕获状态。
     *
     *  @retval     true   Capture to LINC0 on falling RXD edge is enabled
     *  @retval     false  Capture to LINC0 on falling RXD edge is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINFallingEdgeCaptureEnabled(
        const UART_Regs *uart)
    {
        return ((uart->LINCTL & UART_LINCTL_LINC0CAP_MASK) ==
                UART_LINCTL_LINC0CAP_ENABLE);
    }

    /**
     *  @brief      Disable capture of LIN counter on a falling edge
     *              禁用 RXD 下降沿捕获功能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINFallingEdgeCapture(UART_Regs *uart)
    {
        uart->LINCTL &= ~(UART_LINCTL_LINC0CAP_MASK);
    }

    /**
     *  @brief      Enable capture of the LIN counter on a rising edge
     *              使能 RXD 上升沿捕获 LIN 计数器值到 LINC1。
     *
     *  When enabled the LIN counter value is captured to LINC1 register on each
     *  rising RXD edge. A LINC1 interrupt is triggered when enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     */
    __STATIC_INLINE void DL_UART_enableLINRisingEdgeCapture(UART_Regs *uart)
    {
        uart->LINCTL |= UART_LINCTL_LINC1CAP_ENABLE;
    }

    /**
     *  @brief      Check status of capture of LIN counter on a rising edge
     *              查询 RXD 上升沿捕获功能是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of capture of LIN counter on a rising edge
     *              返回值：上升沿捕获状态。
     *
     *  @retval     true   Capture to LINC1 on rising RXD edge is enabled
     *  @retval     false  Capture to LINC1 on rising RXD edge is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINRisingEdgeCaptureEnabled(
        const UART_Regs *uart)
    {
        return ((uart->LINCTL & UART_LINCTL_LINC1CAP_MASK) ==
                UART_LINCTL_LINC1CAP_ENABLE);
    }

    /**
     *  @brief      Disable capture of LIN counter on a rising edge
     *              禁用 RXD 上升沿捕获功能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINRisingEdgeCapture(UART_Regs *uart)
    {
        uart->LINCTL &= ~(UART_LINCTL_LINC1CAP_MASK);
    }

    /**
     *  @brief      Enable LIN counter compare match mode
     *              使能 LIN 计数器比较匹配模式（LINC0 与计数器匹配触发中断）。
     *
     *  When enabled, a match between a value in LINC0 and the LIN counter can
     *  trigger a LINC0 interrupt. Disables capture on falling edge if enabled.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableLINCounterCompareMatch(UART_Regs *uart)
    {
        DL_Common_updateReg(&uart->LINCTL,
                            UART_LINCTL_LINC0_MATCH_ENABLE | UART_LINCTL_LINC0CAP_DISABLE,
                            UART_LINCTL_LINC0CAP_MASK | UART_LINCTL_LINC0_MATCH_MASK);
    }

    /**
     *  @brief      Setup LIN counter control for sync field validation
     *              配置 LIN 计数器用于同步场验证（上升/下降沿捕获+下降沿清除+使能）。
     *
     *  Enable LIN counter capture on rising RX edge. Enable LIN counter capture on falling RX edge.
     *  Enable LIN counter clearing on RX falling edge. Enable LIN counter.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableLINSyncFieldValidationCounterControl(
        UART_Regs *uart)
    {
        DL_Common_updateReg(&uart->LINCTL,
                            UART_LINCTL_LINC0CAP_ENABLE | UART_LINCTL_LINC1CAP_ENABLE |
                                UART_LINCTL_ZERONE_ENABLE | UART_LINCTL_CTRENA_ENABLE,
                            UART_LINCTL_LINC0CAP_MASK | UART_LINCTL_LINC1CAP_MASK |
                                UART_LINCTL_ZERONE_MASK | UART_LINCTL_CTRENA_MASK);
    }

    /**
     *  @brief      Setup LIN counter control for LIN reception
     *              配置 LIN 计数器用于接收（低电平计数+下降沿清除+使能）。
     *
     * Enable count while low signal on RXD. Enable LIN counter clearing on RXD falling edge.
     * Enable LIN counter.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableLINReceptionCountControl(UART_Regs *uart)
    {
        DL_Common_updateReg(&uart->LINCTL,
                            UART_LINCTL_CNTRXLOW_ENABLE | UART_LINCTL_ZERONE_ENABLE |
                                UART_LINCTL_CTRENA_ENABLE,
                            UART_LINCTL_CNTRXLOW_MASK | UART_LINCTL_ZERONE_MASK |
                                UART_LINCTL_CTRENA_MASK);
    }

    /**
     *  @brief      Check if LIN counter compare match mode is enabled
     *              查询 LIN 计数器比较匹配模式是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of counter compare match mode
     *              返回值：比较匹配模式状态。
     *
     *  @retval     true   LIN counter compare match mode is enabled
     *  @retval     false  LIN counter compare match mode is disabled
     */
    __STATIC_INLINE bool DL_UART_isLINCounterCompareMatchEnabled(
        const UART_Regs *uart)
    {
        return ((uart->LINCTL & UART_LINCTL_LINC0_MATCH_MASK) ==
                UART_LINCTL_LINC0_MATCH_ENABLE);
    }

    /**
     *  @brief      Disable LIN counter compare match mode
     *              禁用 LIN 计数器比较匹配模式。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableLINCounterCompareMatch(UART_Regs *uart)
    {
        uart->LINCTL &= ~(UART_LINCTL_LINC0_MATCH_MASK);
    }

    /**
     *  @brief      Set the value to be compared to the LIN counter
     *              设置 LINC0 比较值（0-65535）。
     *
     *  Sets the value of LINC0 to be used to compare to the LIN counter. For use
     *  when LIN counter compare match mode is enabled.
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *  @param[in]  value  Value to be compared to the LIN counter for matching.
     *                     Value between 0 - 65535.
     *                     参数：比较值。
     *
     *  @sa  DL_UART_enableLINCounterCompareMatch
     */
    __STATIC_INLINE void DL_UART_setLINCounterCompareValue(
        UART_Regs *uart, uint16_t value)
    {
        DL_Common_updateReg(&uart->LINC0, value, UART_LINC0_DATA_MASK);
    }

    /**
     *  @brief      Get the LINC0 counter value
     *              获取 LINC0 捕获/比较值。
     *
     *  Captures current LINCTR value on RXD falling edge.
     *
     *  If capture is enabled with @ref DL_UART_enableLINFallingEdgeCapture, a
     *  capture can generate a LINC0 interrupt.
     *  If compare mode is enabled with @ref DL_UART_enableLINCounterCompareMatch,
     *  a counter match can generate a LINC0 interrupt.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The LINC0 counter value
     *              返回值：LINC0 值。
     *
     *  @retval     0 - 65535  The LINC0 counter value
     *
     *  @sa  DL_UART_enableLINFallingEdgeCapture
     *  @sa  DL_UART_enableLINCounterCompareMatch
     */
    __STATIC_INLINE uint16_t DL_UART_getLINFallingEdgeCaptureValue(
        const UART_Regs *uart)
    {
        return ((uint16_t)(uart->LINC0 & UART_LINC0_DATA_MASK));
    }

    /**
     *  @brief      Get the LINC1 counter value
     *              获取 LINC1 捕获值。
     *
     *  Captures current LINCTR value on RXD rising edge. For use when LIN rising
     *  edge capture is enabled. It can generate a LINC1 interrupt on capture.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The LINC1 counter value
     *              返回值：LINC1 值。
     *
     *  @retval     0 - 65535  The LINC1 counter value
     *
     *  @sa  DL_UART_enableLINRisingEdgeCapture
     */
    __STATIC_INLINE uint16_t DL_UART_getLINRisingEdgeCaptureValue(
        const UART_Regs *uart)
    {
        return ((uint16_t)(uart->LINC1 & UART_LINC1_DATA_MASK));
    }

    /**
     *  @brief      Enable the IrDA encoder/decoder
     *              使能 IrDA 编解码器。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableIrDAMode(UART_Regs *uart)
    {
        uart->IRCTL |= UART_IRCTL_IREN_ENABLE;
    }

    /**
     *  @brief      Check if the IrDA encoder/decoder is enabled
     *              查询 IrDA 模式是否已使能。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The status of IrDA mode
     *              返回值：IrDA 模式状态。
     *
     *  @retval     true   IrDA mode is enabled
     *  @retval     false  IrDA mode is disabled
     */
    __STATIC_INLINE bool DL_UART_isIrDAModeEnabled(const UART_Regs *uart)
    {
        return ((uart->IRCTL & UART_IRCTL_IREN_MASK) == UART_IRCTL_IREN_ENABLE);
    }

    /**
     *  @brief      Disable the IrDA encoder/decoder
     *              禁用 IrDA 编解码器。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableIrDAMode(UART_Regs *uart)
    {
        uart->IRCTL &= ~(UART_IRCTL_IREN_MASK);
    }

    /**
     *  @brief      Set the IrDA transmit pulse clock select
     *              设置 IrDA 发射脉冲时钟源。
     *
     *  @param[in]  uart   Pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *  @param[in]  uartClock  The clock select for the IrDA pulse data to be based on.
     *                     One of @ref DL_UART_IRDA_CLOCK
     *                     参数：IrDA 时钟源。
     */
    __STATIC_INLINE void DL_UART_setIrDATXPulseClockSelect(
        UART_Regs *uart, DL_UART_IRDA_CLOCK uartClock)
    {
        DL_Common_updateReg(
            &uart->IRCTL, (uint32_t)uartClock, UART_IRCTL_IRTXCLK_MASK);
    }

    /**
     *  @brief      Get the IrDA transmit pulse clock select
     *              获取 IrDA 发射脉冲时钟源。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The clock select that the IrDA pulse data is based on
     *              返回值：IrDA 时钟源。
     *
     *  @retval     One of @ref DL_UART_IRDA_CLOCK
     *
     */
    __STATIC_INLINE DL_UART_IRDA_CLOCK DL_UART_getIrDATXPulseClockSelect(
        const UART_Regs *uart)
    {
        uint32_t uartClock = uart->IRCTL & UART_IRCTL_IRTXCLK_MASK;

        return (DL_UART_IRDA_CLOCK)(uartClock);
    }

    /**
     *  @brief      Set the IrDA configurations
     *
     *  Calculates the baud rate divisor given the clock output of the UART clock
     *  source and the target baud rate. This API also enables IrDA mode.
     *  中文说明：设置 IrDA 接收极性与发射脉宽，并自动使能 IrDA 功能。
     *
     *  @param[in]  uart         Pointer to the register overlay for the peripheral
     *                           参数：UART 外设寄存器基址。
     *  @param[in]  polarity     The receive input polarity.
     *                           One of @ref DL_UART_IRDA_POLARITY.
     *                           参数：IrDA 接收极性。
     *  @param[in]  pulseLength  The length of the IrDA transmit pulse.
     *                           参数：IrDA 发射脉宽。
     *  @param[in]  irdaClk      The clock used for the transmit pulse.
     *                           One of @ref DL_UART_IRDA_CLOCK.
     *                           参数：IrDA 脉冲时钟源。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_UART_configIrDAMode(UART_Regs *uart, DL_UART_IRDA_POLARITY polarity,
                                uint32_t pulseLength, DL_UART_IRDA_CLOCK irdaClk);

    /**
     *  @brief      Set the IrDA transmit pulse length
     *
     *  The pulse length can be calculated with the following equation:
     *      IRTXPLx = pulseLength * 2 * irdaClk - 1
     *      (IRTXCLK = functional clock of the UART)
     *
     *  To set the pulse time of 3/16 bit period required by the IrDA period,
     *  16-bit oversampling is selected with HSE = 0, the baud rate clock is
     *  selected with IRTXCLK = 1, and the pulse length is set to six one-half
     *  clock cycles with IRTXPLx = 6 - 1 - 5.
     *  中文说明：根据脉宽与时钟源计算并写入 IRTXPL 字段。
     *
     *  @param[in]  uart         Pointer to the register overlay for the peripheral
     *                           参数：UART 外设寄存器基址。
     *  @param[in]  pulseLength  The length of the IrDA transmit pulse.
     *                           参数：IrDA 发射脉宽。
     *  @param[in]  irdaClk      The clock used for the transmit pulse.
     *                           One of @ref DL_UART_IRDA_CLOCK.
     *                           参数：IrDA 脉冲时钟源。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_UART_setIrDAPulseLength(
        UART_Regs *uart, uint32_t pulseLength, DL_UART_IRDA_CLOCK irdaClk);

    /**
     *  @brief      Get the IrDA transmit pulse length
     *              获取 IrDA 发射脉宽。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The IrDA transmit pulse length is set to
     *              返回值：脉宽值。
     *
     *  @retval     0 - 63  The IrDA transmit pulse length
     */
    __STATIC_INLINE uint32_t DL_UART_getIrDATXPulseLength(const UART_Regs *uart)
    {
        return (uart->IRCTL & UART_IRCTL_IRTXPL_MASK);
    }

    /**
     *  @brief      Set the IrDA receive input UCAxRXD polarity
     *              设置 IrDA 接收极性。
     *
     *  @param[in]  uart      Pointer to the register overlay for the peripheral
     *                        参数：UART 外设寄存器基址。
     *  @param[in]  polarity  The value to set the IrDA RX pulse polarity to.
     *                        One of @ref DL_UART_IRDA_POLARITY
     *                        参数：接收极性。
     */
    __STATIC_INLINE void DL_UART_setIrDARXPulsePolarity(
        UART_Regs *uart, DL_UART_IRDA_POLARITY polarity)
    {
        DL_Common_updateReg(
            &uart->IRCTL, (uint32_t)polarity, UART_IRCTL_IRRXPL_MASK);
    }

    /**
     *  @brief      Get the IrDA receive input UCAxRXD polarity
     *              获取 IrDA 接收极性。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The IrDA RX pulse polarity
     *              返回值：接收极性。
     *
     *  @retval     One of @ref DL_UART_IRDA_POLARITY
     */
    __STATIC_INLINE DL_UART_IRDA_POLARITY DL_UART_getIrDARXPulsePolarity(
        const UART_Regs *uart)
    {
        uint32_t polarity = uart->IRCTL & UART_IRCTL_IRRXPL_MASK;

        return (DL_UART_IRDA_POLARITY)(polarity);
    }

    /**
     *  @brief      Set the address mask for DALI, 9-bit, or Idle-Line mode
     *              设置地址掩码（DALI/9位/空闲线模式用）。
     *
     *  The address bits are masked to create a set of addresses to be matched
     *  with the received address byte.
     *
     *  A 0 bit in the MSK bitfield configures that the corresponding bit in the
     *  ADDR bitfield of the ADDR register is don't care.
     *  A 1 bit in the MSK bitfield configures, that the corresponding bit in the
     *  ADDR bitfield of the ADDR register must match.
     *
     *  Used in DALI, UART 9-Bit or Idle-Line mode.
     *
     *  @param[in]  uart         Pointer to the register overlay for the peripheral
     *                            参数：UART 外设寄存器基址。
     *  @param[in]  addressMask  The address mask to set
     *                            参数：地址掩码。
     */
    __STATIC_INLINE void DL_UART_setAddressMask(
        UART_Regs *uart, uint32_t addressMask)
    {
        DL_Common_updateReg(&uart->AMASK, addressMask, UART_AMASK_VALUE_MASK);
    }

    /**
     *  @brief      Get the address mask being used
     *              获取当前地址掩码。
     *
     *  The address bits are masked to create a set of addresses to be matched
     *  with the received address byte.
     *
     *  A 0 bit in the MSK bitfield configures that the corresponding bit in the
     *  ADDR bitfield of the ADDR register is don't care.
     *  A 1 bit in the MSK bitfield configures, that the corresponding bit in the
     *  ADDR bitfield of the ADDR register must match.
     *
     *  Used in DALI, UART 9-Bit or Idle-Line mode.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The address mask being used
     *              返回值：地址掩码。
     *
     *  @retval     0-255  The address mask
     *
     */
    __STATIC_INLINE uint32_t DL_UART_getAddressMask(const UART_Regs *uart)
    {
        return (uart->AMASK & UART_AMASK_VALUE_MASK);
    }

    /**
     *  @brief      Set the address
     *              设置本机地址（DALI/9位/空闲线模式用）。
     *
     *  Used to write the specific address that should be matched with the
     *  receiving byte when the Address Mask (AMASK) is set to FFh. This register
     *  is used in conjunction with AMASK to form a match for address-byte
     *  received.
     *
     *  Used in DALI, UART 9-Bit or Idle-Line mode.
     *
     *  @param[in]  uart     Pointer to the register overlay for the peripheral
     *                       参数：UART 外设寄存器基址。
     *  @param[in]  address  The address to set
     *                       参数：地址值。
     */
    __STATIC_INLINE void DL_UART_setAddress(UART_Regs *uart, uint32_t address)
    {
        DL_Common_updateReg(&uart->ADDR, address, UART_ADDR_VALUE_MASK);
    }

    /**
     *  @brief      Get the address being used
     *              获取当前本机地址。
     *
     *  Used to write the specific address that should be matched with the
     *  receiving byte when the Address Mask (AMASK) is set to FFh. This register
     *  is used in conjunction with AMASK to form a match for address-byte
     *  received.
     *
     *  Used in DALI, UART 9-Bit or Idle-Line mode.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The address being used
     *              返回值：地址值。
     *
     *  @retval     0-255  The address being used
     *
     */
    __STATIC_INLINE uint32_t DL_UART_getAddress(const UART_Regs *uart)
    {
        return (uart->ADDR & UART_ADDR_VALUE_MASK);
    }

    /**
     *  @brief      Enable UART interrupts
     *              使能 UART 中断。
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_UART_INTERRUPT.
     *                             参数：中断掩码。
     */
    __STATIC_INLINE void DL_UART_enableInterrupt(
        UART_Regs *uart, uint32_t interruptMask)
    {
        uart->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable UART interrupts
     *              禁用 UART 中断。
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
     *                             @ref DL_UART_INTERRUPT.
     *                             参数：中断掩码。
     */
    __STATIC_INLINE void DL_UART_disableInterrupt(
        UART_Regs *uart, uint32_t interruptMask)
    {
        uart->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which UART interrupts are enabled
     *              查询哪些 UART 中断已使能。
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_UART_INTERRUPT.
     *                             参数：中断掩码。
     *
     *  @return     Which of the requested UART interrupts are enabled
     *              返回值：已使能的中断。
     *
     *  @retval     Bitwise OR of @ref DL_UART_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_UART_getEnabledInterrupts(
        const UART_Regs *uart, uint32_t interruptMask)
    {
        return (uart->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled UART interrupts
     *              查询已使能的 UART 中断是否有挂起。
     *
     *  Checks if any of the UART interrupts that were previously enabled are
     *  pending.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_UART_INTERRUPT.
     *
     *  @return     Which of the requested UART interrupts are pending
     *
     *  @retval     Bitwise OR of @ref DL_UART_INTERRUPT values
     *
     *  @sa         DL_UART_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_UART_getEnabledInterruptStatus(
        const UART_Regs *uart, uint32_t interruptMask)
    {
        return (uart->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any UART interrupt
     *              查询原始中断状态（无需先使能）。
     *
     *  Checks if any of the UART interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_UART_INTERRUPT.
     *                             参数：中断掩码。
     *
     *  @return     Which of the requested UART interrupts are pending
     *              返回值：挂起的中断。
     *
     *  @retval     Bitwise OR of @ref DL_UART_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_UART_getRawInterruptStatus(
        const UART_Regs *uart, uint32_t interruptMask)
    {
        return (uart->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending UART interrupt
     *              获取最高优先级的挂起中断索引。
     *
     *  Checks if any of the UART interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *
     *  @return     The highest priority pending UART interrupt
     *              返回值：中断索引。
     *
     *  @retval     TBD
     */
    __STATIC_INLINE DL_UART_IIDX DL_UART_getPendingInterrupt(const UART_Regs *uart)
    {
        return (DL_UART_IIDX)(uart->CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending UART interrupts
     *              清除挂起的 UART 中断。
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_UART_INTERRUPT.
     *                             参数：中断掩码。
     */
    __STATIC_INLINE void DL_UART_clearInterruptStatus(
        UART_Regs *uart, uint32_t interruptMask)
    {
        uart->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief      Prepares the UART to change the configuration
     *              准备 UART 配置变更（禁用外设→等待空闲→清 FIFO）。
     *
     *  If the UART has already been enabled, then it is recommended to call this
     *  function before calling other APIs that make changes to the CTL0 register.
     *  If changes are made to the CTL0 register without disabling the UART, then
     *  results are unpredictable. This API performs the following:
     *  1. Disable the UART.
     *  2. Wait for the end of transmission or reception of the current character.
     *  3. Flush the transmit FIFO by clearing bit FEN in the UART control
     *  register CTL0.
     *
     *  @post After calling this API, the user must be re-enabled by calling
     *        @ref DL_UART_enable.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_changeConfig(UART_Regs *uart)
    {
        DL_UART_disable(uart);
        while (DL_UART_isBusy(uart))
        {
            ;
        }
        DL_UART_disableFIFOs(uart);
    }

    /**
     *  @brief      Enable the analog glitch filter on the RX input
     *              使能 RX 输入模拟毛刺滤波器。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableAnalogGlitchFilter(UART_Regs *uart)
    {
        uart->GFCTL |= UART_GFCTL_AGFEN_ENABLE;
    }

    /**
     *  @brief      Disable the analog glitch filter on the RX input
     *              禁用 RX 输入模拟毛刺滤波器。
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableAnalogGlitchFilter(UART_Regs *uart)
    {
        uart->GFCTL &= ~(UART_GFCTL_AGFEN_MASK);
    }

    /**
     * @brief      Returns if analog glitch filter is enabled
     *             查询模拟毛刺滤波器是否已使能。
     *
     * @param[in]  uart  Pointer to the register overlay for the peripheral
     *                   参数：UART 外设寄存器基址。
     *
     * @return true   if analog glitch filter is enabled
     * @return false  if analog glitch filter is disabled
     */
    __STATIC_INLINE bool DL_UART_isAnalogGlitchFilterEnabled(const UART_Regs *uart)
    {
        return ((uart->GFCTL & UART_GFCTL_AGFEN_MASK) == UART_GFCTL_AGFEN_ENABLE);
    }

    /**
     *  @brief      Enable analog and digital noise glitch filter chaining
     *              使能模拟+数字毛刺滤波器级联。
     *
     * When enabled, analog and digital glitch filters are chained and the output
     * of the combination is made available to the IP logic for sampling.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableGlitchFilterChaining(UART_Regs *uart)
    {
        uart->GFCTL |= UART_GFCTL_CHAIN_ENABLED;
    }

    /**
     *  @brief      Disable analog and digital noise glitch filter chaining
     *              禁用模拟+数字毛刺滤波器级联。
     *
     * When disabled, only digital filter output is available to the IP logic
     * for sampling.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableGlitchFilterChaining(UART_Regs *uart)
    {
        uart->GFCTL &= ~(UART_GFCTL_CHAIN_MASK);
    }

    /**
     * @brief      Returns if glitch filter chaining enabled
     *             查询毛刺滤波器级联是否已使能。
     *
     * @param[in]  uart  Pointer to the register overlay for the peripheral
     *                   参数：UART 外设寄存器基址。
     *
     * @return true   if glitch filter chaining is enabled
     * @return false  if glitch filter chaining is disabled
     */
    __STATIC_INLINE bool DL_UART_isGlitchFilterChainingEnabled(
        const UART_Regs *uart)
    {
        return ((uart->GFCTL & UART_GFCTL_CHAIN_MASK) == UART_GFCTL_CHAIN_ENABLED);
    }

    /**
     *  @brief      Set the pulse width select for the analog glitch suppresion
     *              设置模拟毛刺滤波脉宽。
     *
     *  Sets the pulse width select for the analog glitch
     *  suppression on the RX line. See device datasheet for exact values.
     *
     *  This only applies to Power Domain 0 (PD0).
     *
     *  @param[in]  uart        Pointer to the register overlay for the peripheral
     *                          参数：UART 外设寄存器基址。
     *  @param[in]  pulseWidth  Pulse width select for the glitch suppresion.
                                One of @ref DL_UART_PULSE_WIDTH
     *                          参数：脉宽选择。
     */
    __STATIC_INLINE void DL_UART_setAnalogPulseWidth(
        UART_Regs *uart, DL_UART_PULSE_WIDTH pulseWidth)
    {
        DL_Common_updateReg(
            &uart->GFCTL, (uint32_t)pulseWidth, UART_GFCTL_AGFSEL_MASK);
    }

    /**
     *  @brief      Get the pulse width select for the glitch suppresion
     *              获取模拟毛刺滤波脉宽。
     *
     *  Gets the pulse width select for the analog glitch
     *  suppression on the RX line. See device datasheet for exact values.
     *
     *  This only applies to Power Domain 0 (PD0).
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The pulse width select for the glitch suppression
     *              返回值：脉宽选择。
     *
     *  @retval     One of @ref DL_UART_PULSE_WIDTH

     */
    __STATIC_INLINE DL_UART_PULSE_WIDTH DL_UART_getAnalogPulseWidth(
        const UART_Regs *uart)
    {
        uint32_t pulseWidth = uart->GFCTL & UART_GFCTL_AGFSEL_MASK;

        return (DL_UART_PULSE_WIDTH)(pulseWidth);
    }

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts data into the TX FIFO, waiting indefintely until there is space in the
     *  the TX FIFO. After placing data, waits indefintely until data is sent and
     *  the entire TX FIFO is empty. Note that if data is already present in the TX
     *  FIFO when this API is called, it will block until all data is sent.
     *  See related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 1 字节，并等待发送完成。
     *
     *  @param[in]  uart  pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送字节。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_UART_transmitData
     *  @sa         DL_UART_transmitDataCheck
     * @details    功能：参见上方英文说明。
     */
    void DL_UART_transmitDataBlocking(UART_Regs *uart, uint8_t data);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefintely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：阻塞等待 RX FIFO 非空后读取 1 字节。
     *
     *  @note:      As a result of reading the RX FIFO data, the corresponding
     *              error status in the RXDATA register (OVRERR, BRKERR, PARERR,
     *              FRMERR bits) will be dropped.
     *
     *  @param[in]  uart  pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的字节数据。
     *
     *  @sa         DL_UART_transmitData
     *  @sa         DL_UART_transmitDataCheck
     * @details    功能：参见上方英文说明。
     */
    uint8_t DL_UART_receiveDataBlocking(const UART_Regs *uart);

    /**
     *  @brief      Checks the TX FIFO before trying to transmit data
     *
     *  Checks if the TX FIFO is already full before trying to add new data to the
     *  FIFO. Exits immediately if full rather than trying to block. See related
     *  APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：非阻塞发送 1 字节，FIFO 满时立即返回。
     *
     *  @param[in]  uart  pointer to the register overlay for the peripheral
     *                    参数：UART 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送字节。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 发送成功，`false` 表示 FIFO 满。
     *
     *  @sa         DL_UART_transmitData
     *  @sa         DL_UART_transmitDataBlocking
     * @details    功能：参见上方英文说明。
     */
    bool DL_UART_transmitDataCheck(UART_Regs *uart, uint8_t data);

    /**
     *  @brief      Checks the RX FIFO before trying to transmit data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：非阻塞接收 1 字节，FIFO 空时立即返回。
     *
     *  @note:      As a result of reading the RX FIFO data, the corresponding
     *              error status in the RXDATA register (OVRERR, BRKERR, PARERR,
     *              FRMERR bits) will be dropped.
     *
     *  @param[in]  uart   pointer to the register overlay for the peripheral
     *                     参数：UART 外设寄存器基址。
     *  @param[in]  buffer a buffer to write the received data into
     *                     参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @sa         DL_UART_receiveData
     *  @sa         DL_UART_receiveDataBlocking
     * @details    功能：参见上方英文说明。
     */
    bool DL_UART_receiveDataCheck(const UART_Regs *uart, uint8_t *buffer);

    /**
     *  @brief       Read all available data out of the RX FIFO using 8 bit access
     *  @details     功能：批量读取 RX FIFO 数据到软件缓冲区。
     *
     *  @param[in]   uart      Pointer to the register overlay for the peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[out]  buffer    Buffer to write received data into
     *                         参数：接收缓冲区。
     *  @param[in]   maxCount  Max number of bytes to read from the RX FIFO
     *                         参数：最大读取数量。
     *
     *  @return      Number of bytes read from the RX FIFO
     *               返回值：实际读取字节数。
     */
    uint32_t DL_UART_drainRXFIFO(
        const UART_Regs *uart, uint8_t *buffer, uint32_t maxCount);

    /**
     *  @brief      Fill the TX FIFO until full using 8 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *  中文说明：批量写入 TX FIFO，直到 FIFO 满或达到上限。
     *
     *  @param[in]  uart    Pointer to the register overlay for the peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *                      参数：发送缓冲区。
     *  @param[in]  count   Max number of bytes to write to the TX FIFO
     *                      参数：最大写入数量。
     *
     *  @return     Number of bytes written to the TX FIFO
     *              返回值：实际写入字节数。
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_UART_fillTXFIFO(
        UART_Regs *uart, const uint8_t *buffer, uint32_t count);

    /**
     *  @brief      Enable UART interrupt for triggering the DMA receive event
     *              使能 UART 中断作为 DMA 接收触发条件。
     *
     * Enables the UART interrupt to be used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_RX
     * register, which is the event publisher used for triggering the DMA to do
     * a receive data transfer.
     *
     * @note Only one interrupt source should be enabled at a time.
     *
     *  @param[in]  uart       Pointer to the register overlay for the
     *                         peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
     *                         the DMA. One of @ref DL_UART_DMA_INTERRUPT_RX.
     *                         参数：DMA 接收触发中断源。
     */
    __STATIC_INLINE void DL_UART_enableDMAReceiveEvent(
        UART_Regs *uart, uint32_t interrupt)
    {
        uart->DMA_TRIG_RX.IMASK = interrupt;
    }

    /**
     *  @brief      Enable UART interrupt for triggering the DMA transmit event
     *              使能 UART 中断作为 DMA 发送触发条件。
     *
     * Enables the UART interrupt to be used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_TX
     * register, which is the event publisher used for triggering the DMA to do
     * a transmit data transfer.
     *
     * @note DMA_TRIG_TX only has one transmit interrupt source
     *
     *  @param[in]  uart       Pointer to the register overlay for the
     *                         peripheral
     *                         参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_enableDMATransmitEvent(UART_Regs *uart)
    {
        uart->DMA_TRIG_TX.IMASK = UART_DMA_TRIG_TX_IMASK_TXINT_SET;
    }

    /**
     *  @brief      Disables UART interrupt from triggering the DMA receive event
     *              禁用 UART 中断作为 DMA 接收触发条件。
     *
     * Disables the UART interrupt as the condition to generate an event to
     * directly trigger the DMA. This API configures the DMA_TRIG_RX
     * register, which is the event publisher used for triggering the DMA to do
     * a receive data transfer.
     *
     *  @param[in]  uart       Pointer to the register overlay for the
     *                         peripheral
     *                         参数：UART 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
     *                         the DMA. One of @ref DL_UART_DMA_INTERRUPT_RX.
     *                         参数：DMA 接收触发中断源。
     */
    __STATIC_INLINE void DL_UART_disableDMAReceiveEvent(
        UART_Regs *uart, uint32_t interrupt)
    {
        uart->DMA_TRIG_RX.IMASK &= ~(interrupt);
    }

    /**
     *  @brief      Disables UART interrupt from triggering the DMA transmit event
     *              禁用 UART 中断作为 DMA 发送触发条件。
     *
     * Disables the UART interrupt as the condition to generate an event to
     * directly trigger the DMA. This API configures the DMA_TRIG_TX
     * register, which is the event publisher used for triggering the DMA to do
     * a transmit data transfer.
     *
     * @note DMA_TRIG_TX only has one transmit interrupt source
     *
     *  @param[in]  uart       Pointer to the register overlay for the
     *                         peripheral
     *                         参数：UART 外设寄存器基址。
     */
    __STATIC_INLINE void DL_UART_disableDMATransmitEvent(UART_Regs *uart)
    {
        uart->DMA_TRIG_TX.IMASK = UART_DMA_TRIG_TX_IMASK_TXINT_CLR;
    }

    /**
     *  @brief      Check which UART interrupt for DMA receive events is enabled
     *              查询 DMA 接收触发中断是否已使能。
     *
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check Bitwise OR of
     *                             @ref DL_UART_DMA_INTERRUPT_RX.
     *                             参数：中断掩码。
     *
     *  @return     Which of the requested UART interrupts is enabled
     *              返回值：已使能的 DMA 接收触发中断。
     *
     *  @retval     One of @ref DL_UART_DMA_INTERRUPT_RX
     */
    __STATIC_INLINE uint32_t DL_UART_getEnabledDMAReceiveEvent(
        const UART_Regs *uart, uint32_t interruptMask)
    {
        return (uart->DMA_TRIG_RX.IMASK & interruptMask);
    }

    /**
     *  @brief      Check if UART interrupt for DMA transmit event is enabled
     *              查询 DMA 发送触发中断是否已使能。
     *
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *
     *  @return     The requested UART interrupt status
     *              返回值：DMA 发送触发中断状态。
     *
     *  @retval     DL_UART_DMA_INTERRUPT_TX if enabled, 0 if not enabled
     */
    __STATIC_INLINE uint32_t DL_UART_getEnabledDMATransmitEvent(
        const UART_Regs *uart)
    {
        return (uart->DMA_TRIG_TX.IMASK & UART_DMA_TRIG_TX_IMASK_TXINT_MASK);
    }

    /**
     *  @brief      Check interrupt flag of enabled UART interrupt for DMA receive event
     *              查询已使能的 DMA 接收触发中断是否有挂起。
     *
     * Checks if any of the UART interrupts for the DMA receive event that were
     * previously enabled are pending.
     * This API checks the DMA_TRIG_RX register, which is the event publisher used
     * for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_UART_DMA_INTERRUPT_RX.
     *
     *  @return     The requested UART interrupt status
     *
     *  @retval     One of @ref DL_UART_DMA_INTERRUPT_RX
     *
     *  @sa         DL_UART_enableDMAReceiveEvent
     */
    __STATIC_INLINE uint32_t DL_UART_getEnabledDMAReceiveEventStatus(
        const UART_Regs *uart, uint32_t interruptMask)
    {
        return (uart->DMA_TRIG_RX.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled UART interrupt for DMA transmit event
     *              查询已使能的 DMA 发送触发中断是否有挂起。
     *
     * Checks if the UART interrupt for the DMA transmit event that was
     * previously enabled is pending.
     * This API checks the DMA_TRIG_TX register, which is the event publisher used
     * for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *
     *  @return     The requested UART interrupt status
     *
     *  @retval     DL_UART_DMA_INTERRUPT_TX if enabled, 0 if not enabled
     *
     *  @sa         DL_UART_enableDMATransmitEvent
     */
    __STATIC_INLINE uint32_t DL_UART_getEnabledDMATransmitEventStatus(
        const UART_Regs *uart)
    {
        return (uart->DMA_TRIG_TX.MIS & UART_DMA_TRIG_TX_MIS_TXINT_MASK);
    }

    /**
     *  @brief      Check interrupt flag of any UART interrupt for DMA receive event
     *              查询原始 DMA 接收触发中断状态（无需先使能）。
     *
     *  Checks if any of the UART interrupts for DMA receive event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_UART_DMA_INTERRUPT_RX.
     *
     *  @return     Which of the requested UART interrupts are pending
     *
     *  @retval     Bitwise OR of @ref DL_UART_DMA_INTERRUPT_RX values
     */
    __STATIC_INLINE uint32_t DL_UART_getRawDMAReceiveEventStatus(
        const UART_Regs *uart, uint32_t interruptMask)
    {
        return (uart->DMA_TRIG_RX.RIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any UART interrupt for DMA transmit event
     *              查询原始 DMA 发送触发中断状态（无需先使能）。
     *
     *  Checks if any of the UART interrupts for DMA transmit event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *
     *  @return     The requested UART interrupt status
     *
     *  @retval     DL_UART_DMA_INTERRUPT_TX if enabled, 0 if not enabled
     */
    __STATIC_INLINE uint32_t DL_UART_getRawDMATransmitEventStatus(
        const UART_Regs *uart)
    {
        return (uart->DMA_TRIG_TX.RIS & UART_DMA_TRIG_TX_RIS_TXINT_MASK);
    }

    /**
     *  @brief      Get highest priority pending UART interrupt for DMA receive event
     *              获取 DMA 接收触发最高优先级挂起中断。
     *
     *  Checks if any of the UART interrupts for DMA receive event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *
     *  @return     The highest priority pending UART interrupt
     *
     *  @retval     One of @ref DL_UART_DMA_IIDX_RX
     */
    __STATIC_INLINE DL_UART_DMA_IIDX_RX DL_UART_getPendingDMAReceiveEvent(
        const UART_Regs *uart)
    {
        return (DL_UART_DMA_IIDX_RX)(uart->DMA_TRIG_RX.IIDX);
    }

    /**
     *  @brief      Get highest priority pending UART interrupt for DMA transmit event
     *              获取 DMA 发送触发最高优先级挂起中断。
     *
     *  Checks if the UART interrupt for DMA transmit event is pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *
     *  @return     The highest priority pending UART interrupt
     *
     *  @retval     DL_UART_DMA_IIDX_TX if pending, 0 if not pending
     */
    __STATIC_INLINE DL_UART_DMA_IIDX_TX DL_UART_getPendingDMATransmitEvent(
        const UART_Regs *uart)
    {
        return (DL_UART_DMA_IIDX_TX)(uart->DMA_TRIG_TX.IIDX);
    }

    /**
     *  @brief      Clear pending UART interrupts for DMA receive event
     *              清除 DMA 接收触发挂起中断。
     *
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_UART_DMA_INTERRUPT_RX.
     *                             参数：中断掩码。
     */
    __STATIC_INLINE void DL_UART_clearDMAReceiveEventStatus(
        UART_Regs *uart, uint32_t interruptMask)
    {
        uart->DMA_TRIG_RX.ICLR = interruptMask;
    }

    /**
     *  @brief      Clear pending UART interrupt for DMA transmit event
     *              清除 DMA 发送触发挂起中断。
     *
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  uart           Pointer to the register overlay for the
     *                             peripheral
     *                             参数：UART 外设寄存器基址。
     * @note DMA_TRIG_TX only has one transmit interrupt source
     */
    __STATIC_INLINE void DL_UART_clearDMATransmitEventStatus(UART_Regs *uart)
    {
        uart->DMA_TRIG_TX.ICLR = UART_DMA_TRIG_TX_ICLR_TXINT_CLR;
    }

    /*!
     *  @brief Sets the second clock divider ratio
     *         设置第二级时钟分频比（IrDA 专用）。
     *
     *  This API sets the CLKDIV2 register, which stores the clock divider ratio
     *  used to further divide the UART function clock in IrDA UART mode
     *
     *  @param[in]  uart    Pointer to the register overlay for the peripheral
     *                      参数：UART 外设寄存器基址。
     *  @param[in]  ratio   The CLKDIV2 value.  One of @ref DL_UART_CLOCK_DIVIDE_RATIO
     *                      参数：分频比。
     */
    __STATIC_INLINE void DL_UART_setClockDivider2(
        UART_Regs *uart, DL_UART_CLOCK_DIVIDE2_RATIO ratio)
    {
        uart->CLKDIV2 = (uint32_t)ratio;
    }

    /*!
     *  @brief Gets the value of CLKDIV2
     *         获取第二级时钟分频比。
     *
     *  This API gets the value stored in the CLKDIV2 register, which stores the
     *  clock divider ratio to further divide the UART function clock in IrDA Mode
     *
     *  @param[in]  uart    Pointer to the register overlay for the peripheral
     *                      参数：UART 外设寄存器基址。
     *
     *  @return     The clock divider ratio stored in the CLKDIV2 register
     *              返回值：第二级分频比。
     *
     *  @retval     The CLKDIV2 value.  One of @ref DL_UART_CLOCK_DIVIDE2_RATIO
     */

    __STATIC_INLINE DL_UART_CLOCK_DIVIDE2_RATIO DL_UART_getClockDivider2(
        const UART_Regs *uart)
    {
        uint32_t ratio = uart->CLKDIV2;
        return (DL_UART_CLOCK_DIVIDE2_RATIO)ratio;
    }
#ifdef __MSPM0_HAS_UART_MAIN__

    /**
     *  @brief      Save UART Main configuration before entering a power loss state.
     *
     *  Some MSPM0G peripherals residing in PD1 domain do not retain register
     *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
     *  for the full list of peripheral instances that exhibit this behavior.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *
     *  @param[in]  ptr  Configuration backup setup structure. See
     *                  @ref DL_UART_Main_backupConfig.
     *
     *  @retval     FALSE if a configuration already exists in ptr (will not be
     *              overwritten). TRUE if a configuration was successfully saved
     *
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    bool DL_UART_Main_saveConfiguration(
        const UART_Regs *uart, DL_UART_Main_backupConfig *ptr);

    /**
     *  @brief      Restore UART Main configuration after leaving a power loss state.
     *
     *  Some MSPM0G peripherals residing in PD1 domain do not retain register
     *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
     *  for the full list of peripheral instances that exhibit this behavior.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *
     *  @param[in]  ptr   Configuration backup setup structure. See
     *                    @ref DL_UART_Main_backupConfig.
     *
     *  @retval     FALSE if a configuration does not exist in ptr (will not be
     *              loaded). TRUE if a configuration successfully loaded
     *
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    bool DL_UART_Main_restoreConfiguration(
        UART_Regs *uart, DL_UART_Main_backupConfig *ptr);

#endif /* __MSPM0_HAS_UART_MAIN__ */

#ifdef __MSPM0_HAS_UART_EXTD__

    /**
     *  @brief      Save UART Extend configuration before entering a power loss
     *              state.
     *
     *  Some MSPM0G peripherals residing in PD1 domain do not retain register
     *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
     *  for the full list of peripheral instances that exhibit this behavior.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *
     *  @param[in]  ptr   Configuration backup setup structure. See
     *                    @ref DL_UART_Extend_backupConfig.
     *
     *  @retval     FALSE if a configuration already exists in ptr (will not be
     *              overwritten). TRUE if a configuration was successfully saved
     *
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    bool DL_UART_Extend_saveConfiguration(
        const UART_Regs *uart, DL_UART_Extend_backupConfig *ptr);

    /**
     *  @brief      Restore UART Extend configuration after leaving a power loss
     *              state.
     *
     *  Some MSPM0G peripherals residing in PD1 domain do not retain register
     *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
     *  for the full list of peripheral instances that exhibit this behavior.
     *
     *  @param[in]  uart  Pointer to the register overlay for the peripheral
     *
     *  @param[in]  ptr   Configuration backup setup structure. See
     *                    @ref DL_UART_Extend_backupConfig.
     *
     *  @retval     FALSE if a configuration does not exist in ptr (will not be
     *              loaded). TRUE if a configuration successfully loaded
     *
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    bool DL_UART_Extend_restoreConfiguration(
        UART_Regs *uart, DL_UART_Extend_backupConfig *ptr);

#endif /* __MSPM0_HAS_UART_EXTD__ */

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_UART_MAIN__ || __MSPM0_HAS_UART_EXTD__ */

#else
#warning \
    "TI highly recommends accessing uart with dl_uart_main, dl_uart_extend.h only."
#endif /* ti_dl_dl_uart_main__include ti_dl_dl_uart_extend__include */

#endif /* ti_dl_dl_uart__include */
/** @}*/
