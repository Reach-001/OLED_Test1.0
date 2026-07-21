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
 *  @file       dl_spi.h
 *  @brief      SPI Driver Library
 *  @defgroup   SPI Serial Peripheral Interface (SPI)
 *
 *  @anchor ti_dl_dl_spi_Overview
 *  # Overview
 *
 *  The Serial Peripheral Interface Driver Library allows full configuration of
 *  the MSPM0 SPI module.
 *  The serial peripheral interface (SPI) module provides a standardized
 *  serial interface to transfer data between MSPM0 devices and other external
 *  devices with SPI interface.
 *
 *  文件用途：MSPM0G3507 SPI 外设驱动库头文件，提供 SPI 模块的初始化、
 *  数据收发、中断管理、DMA 触发及低功耗配置备份/恢复等完整 API 声明。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup SPI
 * @{
 */
#ifndef ti_dl_dl_spi__include
#define ti_dl_dl_spi__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_SPI__

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

/** @addtogroup DL_SPI_CD_MODE
 *  @{
 */
/*!
 * @brief Data mode
 * @brief 中文：数据模式，C/D 引脚高电平。
 */
#define DL_SPI_CD_MODE_DATA        (SPI_CTL1_CDMODE_DATA >> SPI_CTL1_CDMODE_OFS)

/*!
 * @brief Command mode
 * @brief 中文：命令模式，C/D 引脚低电平。
 */
#define DL_SPI_CD_MODE_COMMAND  (SPI_CTL1_CDMODE_COMMAND >> SPI_CTL1_CDMODE_OFS)


/** @}*/

/** @addtogroup DL_SPI_INTERRUPT
 *  @{
 */

/*!
 * @brief DMA done 1 event for transmit interrupt
 * @brief 中文：发送 DMA 完成中断。
 */
#define DL_SPI_INTERRUPT_DMA_DONE_TX    (SPI_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief DMA done 1 event for receive interrupt
 * @brief 中文：接收 DMA 完成中断。
 */
#define DL_SPI_INTERRUPT_DMA_DONE_RX    (SPI_CPU_INT_IMASK_DMA_DONE_RX_SET)
/*!
 * @brief SPI has finished transfers and changed into idle mode interrupt
 * @brief 中文：SPI 传输完成进入空闲模式中断。
 */
#define DL_SPI_INTERRUPT_IDLE                   (SPI_CPU_INT_IMASK_IDLE_SET)

/*!
 * @brief Transmit FIFO empty interrupt
 * @brief 中文：发送 FIFO 空中断。
 */
#define DL_SPI_INTERRUPT_TX_EMPTY            (SPI_CPU_INT_IMASK_TXEMPTY_SET)

/*!
 * @brief Transmit FIFO interrupt
 * @brief 中文：发送 FIFO 中断（达到阈值触发）。
 */
#define DL_SPI_INTERRUPT_TX                       (SPI_CPU_INT_IMASK_TX_SET)

/*!
 * @brief Receive FIFO interrupt
 * @brief 中文：接收 FIFO 中断（达到阈值触发）。
 */
#define DL_SPI_INTERRUPT_RX                       (SPI_CPU_INT_IMASK_RX_SET)

/*!
 * @brief Receive timeout interrupt
 * @brief 中文：接收超时中断。
 */
#define DL_SPI_INTERRUPT_RX_TIMEOUT            (SPI_CPU_INT_IMASK_RTOUT_SET)

/*!
 * @brief Receive FIFO full interrupt
 * @brief 中文：接收 FIFO 满中断。
 */
#define DL_SPI_INTERRUPT_RX_FULL              (SPI_CPU_INT_IMASK_RXFULL_SET)

/*!
 * @brief Transmit FIFO underflow interrupt
 * @brief 中文：发送 FIFO 下溢中断。
 */
#define DL_SPI_INTERRUPT_TX_UNDERFLOW     (SPI_CPU_INT_IMASK_TXFIFO_UNF_SET)

/*!
 * @brief Parity error
 * @brief 中文：奇偶校验错误中断。
 */
#define DL_SPI_INTERRUPT_PARITY_ERROR            (SPI_CPU_INT_IMASK_PER_SET)

/*!
 * @brief Receive FIFO overflow interrupt
 * @brief 中文：接收 FIFO 上溢中断。
 */
#define DL_SPI_INTERRUPT_RX_OVERFLOW      (SPI_CPU_INT_IMASK_RXFIFO_OVF_SET)

/** @}*/

/*! @enum DL_SPI_DMA_IIDX_RX 中文：SPI 接收 DMA 触发中断索引 */
typedef enum {
    /*! SPI interrupt index for enabling SPI receive as DMA trigger */
    /*! 中文：SPI 接收 DMA 触发索引 */
    DL_SPI_DMA_IIDX_RX_TRIGGER = SPI_DMA_TRIG_RX_IIDX_STAT_RX_EVT,
    /*! SPI interrupt index for enabling SPI receive timeout as DMA trigger */
    /*! 中文：SPI 接收超时 DMA 触发索引 */
    DL_SPI_DMA_IIDX_RX_TIMEOUT_TRIGGER = SPI_DMA_TRIG_RX_IIDX_STAT_RTOUT_EVT
} DL_SPI_DMA_IIDX_RX;

/*! @enum DL_SPI_DMA_IIDX_TX 中文：SPI 发送 DMA 触发中断索引 */
typedef enum {
    /*! SPI interrupt index for enabling SPI transmit as DMA trigger */
    /*! 中文：SPI 发送 DMA 触发索引 */
    DL_SPI_DMA_IIDX_TX_TRIGGER = SPI_DMA_TRIG_TX_IIDX_STAT_TX_EVT
} DL_SPI_DMA_IIDX_TX;

/** @addtogroup DL_SPI_DMA_INTERRUPT_RX
 *  @{
 */
/*!
 * @brief SPI interrupt for enabling SPI receive as DMA trigger
 * @brief 中文：SPI 接收事件作为 DMA 触发源。
 */
#define DL_SPI_DMA_INTERRUPT_RX               (SPI_DMA_TRIG_RX_IMASK_RX_SET)

/*!
 * @brief SPI interrupt for enabling SPI receive timeout as DMA trigger
 * @brief 中文：SPI 接收超时作为 DMA 触发源。
 */
#define DL_SPI_DMA_INTERRUPT_RX_TIMEOUT       (SPI_DMA_TRIG_RX_IMASK_RTOUT_SET)

/** @}*/

/*!
 * @brief SPI interrupt for enabling SPI transmit as DMA trigger
 * @brief 中文：SPI 发送事件作为 DMA 触发源。
 */
#define DL_SPI_DMA_INTERRUPT_TX               (SPI_DMA_TRIG_TX_IMASK_TX_SET)

    /* clang-format on */

    /*! @enum DL_SPI_PARITY 中文：SPI 奇偶校验配置 */
    typedef enum
    {
        /*! Use even parity, enable transmit and receive parity */
        /*! 中文：偶校验，使能收发校验 */
        DL_SPI_PARITY_EVEN =
            (SPI_CTL1_PES_ENABLE | SPI_CTL1_PREN_ENABLE | SPI_CTL1_PTEN_ENABLE),
        /*! Use odd parity, enable transmit and receive parity  */
        /*! 中文：奇校验，使能收发校验 */
        DL_SPI_PARITY_ODD =
            (SPI_CTL1_PES_DISABLE | SPI_CTL1_PREN_ENABLE | SPI_CTL1_PTEN_ENABLE),
        /*! Disable receive and transmit parity */
        /*! 中文：禁用收发校验 */
        DL_SPI_PARITY_NONE = (SPI_CTL1_PREN_DISABLE | SPI_CTL1_PTEN_DISABLE)
    } DL_SPI_PARITY;

    /*! @enum DL_SPI_FRAME_FORMAT 中文：SPI 帧格式配置 */
    typedef enum
    {
        /*! Motorola 3 Wire with Polarity 0, Phase 0 */
        /*! 中文：Motorola 三线，极性 0 相位 0 */
        DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA0 =
            (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_3WIRE),
        /*! Motorola 3 Wire with Polarity 0, Phase 1 */
        /*! 中文：Motorola 三线，极性 0 相位 1 */
        DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1 =
            (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_SECOND | SPI_CTL0_FRF_MOTOROLA_3WIRE),
        /*! Motorola 3 Wire with Polarity 1, Phase 0 */
        /*! 中文：Motorola 三线，极性 1 相位 0 */
        DL_SPI_FRAME_FORMAT_MOTO3_POL1_PHA0 =
            (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_3WIRE),
        /*! Motorola 3 Wire with Polarity 1, Phase 1 */
        /*! 中文：Motorola 三线，极性 1 相位 1 */
        DL_SPI_FRAME_FORMAT_MOTO3_POL1_PHA1 =
            (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_SECOND |
             SPI_CTL0_FRF_MOTOROLA_3WIRE),
        /*! Motorola 4 Wire with Polarity 0, Phase 0 */
        /*! 中文：Motorola 四线，极性 0 相位 0 */
        DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0 =
            (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_4WIRE),
        /*! Motorola 4 Wire with Polarity 0, Phase 1 */
        /*! 中文：Motorola 四线，极性 0 相位 1 */
        DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA1 =
            (SPI_CTL0_SPO_LOW | SPI_CTL0_SPH_SECOND | SPI_CTL0_FRF_MOTOROLA_4WIRE),
        /*! Motorola 4 Wire with Polarity 1, Phase 0 */
        /*! 中文：Motorola 四线，极性 1 相位 0 */
        DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA0 =
            (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_FIRST | SPI_CTL0_FRF_MOTOROLA_4WIRE),
        /*! Motorola 4 Wire with Polarity 1, Phase 1 */
        /*! 中文：Motorola 四线，极性 1 相位 1 */
        DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA1 =
            (SPI_CTL0_SPO_HIGH | SPI_CTL0_SPH_SECOND |
             SPI_CTL0_FRF_MOTOROLA_4WIRE),
        /*! TI Sync Frame Format */
        /*! 中文：TI 同步帧格式 */
        DL_SPI_FRAME_FORMAT_TI_SYNC = (SPI_CTL0_FRF_TI_SYNC),
    } DL_SPI_FRAME_FORMAT;

    /*! @enum DL_SPI_MODE 中文：SPI 主从模式 */
    typedef enum
    {
        /*! Controller mode */
        /*! 中文：主机模式 */
        DL_SPI_MODE_CONTROLLER = (SPI_CTL1_CP_ENABLE),
        /*! Peripheral mode */
        /*! 中文：从机模式 */
        DL_SPI_MODE_PERIPHERAL = (SPI_CTL1_CP_DISABLE)
    } DL_SPI_MODE;

    /*! @enum DL_SPI_BIT_ORDER 中文：SPI 位序 */
    typedef enum
    {
        /*! MSB First */
        /*! 中文：高位在前 */
        DL_SPI_BIT_ORDER_MSB_FIRST = (SPI_CTL1_MSB_ENABLE),
        /*! LSB First */
        /*! 中文：低位在前 */
        DL_SPI_BIT_ORDER_LSB_FIRST = (SPI_CTL1_MSB_DISABLE)
    } DL_SPI_BIT_ORDER;

    /*! @enum DL_SPI_DATA_SIZE 中文：SPI 数据位宽 */
    typedef enum
    {
        /*! Data size 4 bits */
        /*! 中文：4 位数据 */
        DL_SPI_DATA_SIZE_4 = (SPI_CTL0_DSS_DSS_4),
        /*! Data size 5 bits */   /*! 中文：5 位数据 */
        DL_SPI_DATA_SIZE_5 = (SPI_CTL0_DSS_DSS_5),
        /*! Data size 6 bits */   /*! 中文：6 位数据 */
        DL_SPI_DATA_SIZE_6 = (SPI_CTL0_DSS_DSS_6),
        /*! Data size 7 bits */   /*! 中文：7 位数据 */
        DL_SPI_DATA_SIZE_7 = (SPI_CTL0_DSS_DSS_7),
        /*! Data size 8 bits */   /*! 中文：8 位数据 */
        DL_SPI_DATA_SIZE_8 = (SPI_CTL0_DSS_DSS_8),
        /*! Data size 9 bits */   /*! 中文：9 位数据 */
        DL_SPI_DATA_SIZE_9 = (SPI_CTL0_DSS_DSS_9),
        /*! Data size 10 bits */  /*! 中文：10 位数据 */
        DL_SPI_DATA_SIZE_10 = (SPI_CTL0_DSS_DSS_10),
        /*! Data size 11 bits */  /*! 中文：11 位数据 */
        DL_SPI_DATA_SIZE_11 = (SPI_CTL0_DSS_DSS_11),
        /*! Data size 12 bits */  /*! 中文：12 位数据 */
        DL_SPI_DATA_SIZE_12 = (SPI_CTL0_DSS_DSS_12),
        /*! Data size 13 bits */  /*! 中文：13 位数据 */
        DL_SPI_DATA_SIZE_13 = (SPI_CTL0_DSS_DSS_13),
        /*! Data size 14 bits */  /*! 中文：14 位数据 */
        DL_SPI_DATA_SIZE_14 = (SPI_CTL0_DSS_DSS_14),
        /*! Data size 15 bits */  /*! 中文：15 位数据 */
        DL_SPI_DATA_SIZE_15 = (SPI_CTL0_DSS_DSS_15),
        /*! Data size 16 bits */  /*! 中文：16 位数据 */
        DL_SPI_DATA_SIZE_16 = (SPI_CTL0_DSS_DSS_16),
    } DL_SPI_DATA_SIZE;

    /*! @enum DL_SPI_CHIP_SELECT 中文：SPI 片选引脚选择 */
    typedef enum
    {
        /*! Chip Select 0 */   /*! 中文：片选 0 */
        DL_SPI_CHIP_SELECT_0 = (SPI_CTL0_CSSEL_CSSEL_0),
        /*! Chip Select 1 */   /*! 中文：片选 1 */
        DL_SPI_CHIP_SELECT_1 = (SPI_CTL0_CSSEL_CSSEL_1),
        /*! Chip Select 2 */   /*! 中文：片选 2 */
        DL_SPI_CHIP_SELECT_2 = (SPI_CTL0_CSSEL_CSSEL_2),
        /*! Chip Select 3 */   /*! 中文：片选 3 */
        DL_SPI_CHIP_SELECT_3 = (SPI_CTL0_CSSEL_CSSEL_3),
        /*! No chip select */  /*! 中文：不使用片选 */
        DL_SPI_CHIP_SELECT_NONE = (0)
    } DL_SPI_CHIP_SELECT;

    /*! @enum DL_SPI_TX_FIFO_LEVEL 中文：发送 FIFO 中断阈值 */
    typedef enum
    {
        /*! Interrupt triggers when TX FIFO <= 3/4 empty */
        /*! 中文：FIFO 空 3/4 时触发中断 */
        DL_SPI_TX_FIFO_LEVEL_3_4_EMPTY = SPI_IFLS_TXIFLSEL_LVL_3_4,
        /*! Interrupt triggers when TX FIFO <= 1/2 empty (default) */
        /*! 中文：FIFO 空 1/2 时触发中断（默认） */
        DL_SPI_TX_FIFO_LEVEL_1_2_EMPTY = SPI_IFLS_TXIFLSEL_LVL_1_2,
        /*! Interrupt triggers when TX FIFO <= 1/4 empty */
        /*! 中文：FIFO 空 1/4 时触发中断 */
        DL_SPI_TX_FIFO_LEVEL_1_4_EMPTY = SPI_IFLS_TXIFLSEL_LVL_1_4,
        /*! Interrupt triggers when TX FIFO is empty */
        /*! 中文：FIFO 完全为空时触发中断 */
        DL_SPI_TX_FIFO_LEVEL_EMPTY = SPI_IFLS_TXIFLSEL_LVL_EMPTY,
        /*! Interrupt triggers when TX FIFO has >= 1 frame free. Should be used with DMA */
        /*! 中文：FIFO 至少空 1 帧时触发中断，配合 DMA 使用 */
        DL_SPI_TX_FIFO_LEVEL_ONE_FRAME = SPI_IFLS_TXIFLSEL_LEVEL_1
    } DL_SPI_TX_FIFO_LEVEL;

    /*! @enum DL_SPI_RX_FIFO_LEVEL 中文：接收 FIFO 中断阈值 */
    typedef enum
    {
        /*! Interrupt triggers when RX FIFO contains >= 1 frame. Should be used with DMA */
        /*! 中文：FIFO 至少 1 帧时触发中断，配合 DMA 使用 */
        DL_SPI_RX_FIFO_LEVEL_ONE_FRAME = SPI_IFLS_RXIFLSEL_LEVEL_1,
        /*! Interrupt triggers when RX FIFO is full */
        /*! 中文：FIFO 满时触发中断 */
        DL_SPI_RX_FIFO_LEVEL_FULL = SPI_IFLS_RXIFLSEL_LVL_FULL,
        /*! Interrupt triggers when RX FIFO >= 3/4 full */
        /*! 中文：FIFO 满 3/4 时触发中断 */
        DL_SPI_RX_FIFO_LEVEL_3_4_FULL = SPI_IFLS_RXIFLSEL_LVL_3_4,
        /*! Interrupt triggers when RX FIFO >= 1/2 full (default) */
        /*! 中文：FIFO 满 1/2 时触发中断（默认） */
        DL_SPI_RX_FIFO_LEVEL_1_2_FULL = SPI_IFLS_RXIFLSEL_LVL_1_2,
        /*! Interrupt triggers when RX FIFO >= 1/4 full */
        /*! 中文：FIFO 满 1/4 时触发中断 */
        DL_SPI_RX_FIFO_LEVEL_1_4_FULL = SPI_IFLS_RXIFLSEL_LVL_1_4,
    } DL_SPI_RX_FIFO_LEVEL;

    /*! @enum DL_SPI_IIDX 中文：SPI 中断索引 */
    typedef enum
    {

        /*! SPI interrupt index for DMA Done 1 event for transmit */
        /*! 中文：发送 DMA 完成事件索引 */
        DL_SPI_IIDX_DMA_DONE_TX = SPI_CPU_INT_IIDX_STAT_DMA_DONE_TX_EVT,
        /*! SPI interrupt index for DMA Done 1 event for receive */
        /*! 中文：接收 DMA 完成事件索引 */
        DL_SPI_IIDX_DMA_DONE_RX = SPI_CPU_INT_IIDX_STAT_DMA_DONE_RX_EVT,
        /*! SPI interrupt index for SPI to signal it has finished transfers and
         * changed into idle mode */
        /*! 中文：SPI 空闲事件索引 */

        DL_SPI_IIDX_IDLE = SPI_CPU_INT_IIDX_STAT_IDLE_EVT,
        /*! SPI interrupt index for transmit FIFO empty */
        /*! 中文：发送 FIFO 空事件索引 */
        DL_SPI_IIDX_TX_EMPTY = SPI_CPU_INT_IIDX_STAT_TX_EMPTY,
        /*! SPI interrupt index for transmit FIFO */
        /*! 中文：发送 FIFO 事件索引 */
        DL_SPI_IIDX_TX = SPI_CPU_INT_IIDX_STAT_TX_EVT,
        /*! SPI interrupt index for receive FIFO */
        /*! 中文：接收 FIFO 事件索引 */
        DL_SPI_IIDX_RX = SPI_CPU_INT_IIDX_STAT_RX_EVT,
        /*! SPI interrupt index for receive timeout */
        /*! 中文：接收超时事件索引 */
        DL_SPI_IIDX_RX_TIMEOUT = SPI_CPU_INT_IIDX_STAT_RTOUT_EVT,

        /*! SPI interrupt index for receive FIFO full  */
        /*! 中文：接收 FIFO 满事件索引 */
        DL_SPI_IIDX_RX_FULL = SPI_CPU_INT_IIDX_STAT_RXFULL_EVT,
        /*! SPI interrupt index for transmit FIFO underflow  */
        /*! 中文：发送 FIFO 下溢事件索引 */
        DL_SPI_IIDX_TX_UNDERFLOW = SPI_CPU_INT_IIDX_STAT_TXFIFO_UNF_EVT,

        /*! SPI interrupt index for parity error */
        /*! 中文：奇偶校验错误事件索引 */
        DL_SPI_IIDX_PARITY_ERROR = SPI_CPU_INT_IIDX_STAT_PER_EVT,
        /*! SPI interrupt index for receive FIFO overflow */
        /*! 中文：接收 FIFO 上溢事件索引 */
        DL_SPI_IIDX_RX_OVERFLOW = SPI_CPU_INT_IIDX_STAT_RXFIFO_OFV_EVT
    } DL_SPI_IIDX;

    /*! @enum DL_SPI_CLOCK_DIVIDE_RATIO 中文：SPI 时钟分频系数 */
    typedef enum
    {
        /*! SPI source clock divide ratio set to 1 */
        /*! 中文：分频系数 1 */
        DL_SPI_CLOCK_DIVIDE_RATIO_1 = SPI_CLKDIV_RATIO_DIV_BY_1,
        /*! SPI source clock divide ratio set to 2 */  /*! 中文：分频系数 2 */
        DL_SPI_CLOCK_DIVIDE_RATIO_2 = SPI_CLKDIV_RATIO_DIV_BY_2,
        /*! SPI source clock divide ratio set to 3 */  /*! 中文：分频系数 3 */
        DL_SPI_CLOCK_DIVIDE_RATIO_3 = SPI_CLKDIV_RATIO_DIV_BY_3,
        /*! SPI source clock divide ratio set to 4 */  /*! 中文：分频系数 4 */
        DL_SPI_CLOCK_DIVIDE_RATIO_4 = SPI_CLKDIV_RATIO_DIV_BY_4,
        /*! SPI source clock divide ratio set to 5 */  /*! 中文：分频系数 5 */
        DL_SPI_CLOCK_DIVIDE_RATIO_5 = SPI_CLKDIV_RATIO_DIV_BY_5,
        /*! SPI source clock divide ratio set to 6 */  /*! 中文：分频系数 6 */
        DL_SPI_CLOCK_DIVIDE_RATIO_6 = SPI_CLKDIV_RATIO_DIV_BY_6,
        /*! SPI source clock divide ratio set to 7 */  /*! 中文：分频系数 7 */
        DL_SPI_CLOCK_DIVIDE_RATIO_7 = SPI_CLKDIV_RATIO_DIV_BY_7,
        /*! SPI source clock divide ratio set to 8 */  /*! 中文：分频系数 8 */
        DL_SPI_CLOCK_DIVIDE_RATIO_8 = SPI_CLKDIV_RATIO_DIV_BY_8
    } DL_SPI_CLOCK_DIVIDE_RATIO;

    /*! @enum DL_SPI_CLOCK 中文：SPI 时钟源选择 */
    typedef enum
    {
        /*! Selects BUSCLK as the clock source */
        /*! 中文：选择总线时钟 (BUSCLK) */
        DL_SPI_CLOCK_BUSCLK = SPI_CLKSEL_SYSCLK_SEL_ENABLE,
        /*! Selects MFCLK as the clock source */
        /*! 中文：选择中频时钟 (MFCLK) */
        DL_SPI_CLOCK_MFCLK = SPI_CLKSEL_MFCLK_SEL_ENABLE,
        /*! Selects LFCLK as the clock source */
        /*! 中文：选择低频时钟 (LFCLK) */
        DL_SPI_CLOCK_LFCLK = SPI_CLKSEL_LFCLK_SEL_ENABLE
    } DL_SPI_CLOCK;

    /*!
     *  @brief  Configuration struct for @ref DL_SPI_init.
     *  @brief 中文：SPI 初始化配置结构体。
     */
    typedef struct
    {
        /*! The controller/peripheral mode configuration. One of @ref DL_SPI_MODE */
        /*! 中文：主从模式配置 */
        DL_SPI_MODE mode;

        /*!
         *  The frame format to use for data transfer. One of @ref
         *  DL_SPI_FRAME_FORMAT
         */
        /*! 中文：帧格式配置 */
        DL_SPI_FRAME_FORMAT frameFormat;

        /*!
         *  The parity configuration to use for data transfer. One of @ref
         *  DL_SPI_PARITY.
         */
        /*! 中文：奇偶校验配置 */
        DL_SPI_PARITY parity;

        /*! The size of the data transfer. One of @ref DL_SPI_DATA_SIZE */
        /*! 中文：数据位宽 */
        DL_SPI_DATA_SIZE dataSize;

        /*! The order of bits during data transfer. One of @ref DL_SPI_BIT_ORDER */
        /*! 中文：位序配置 */
        DL_SPI_BIT_ORDER bitOrder;

        /*! The pin to use for chip select. Used in Controller or Peripheral modes
         *  with Motorola 4-Wire or TI Sync frame formats. One of
         *  @ref DL_SPI_CHIP_SELECT.
         */
        /*! 中文：片选引脚，仅四线 Motorola 或 TI Sync 帧格式时有效 */
        DL_SPI_CHIP_SELECT chipSelectPin;

    } DL_SPI_Config;

    /*!
     *  @brief  Configuration struct for @ref DL_SPI_setClockConfig.
     *  @brief 中文：SPI 时钟配置结构体。
     */
    typedef struct
    {
        /*! Selects SPI module clock source @ref DL_SPI_CLOCK */
        /*! 中文：时钟源选择 */
        DL_SPI_CLOCK clockSel;

        /*! Selects the divide ratio. One of @ref DL_SPI_CLOCK_DIVIDE_RATIO */
        /*! 中文：分频系数 */
        DL_SPI_CLOCK_DIVIDE_RATIO divideRatio;

    } DL_SPI_ClockConfig;

    /**
     * @brief Configuration structure to backup SPI peripheral state before going
     *        to STOP/STANDBY mode. Used by @ref DL_SPI_saveConfiguration and
     *        @ref DL_SPI_restoreConfiguration
     * @brief 中文：SPI 低功耗配置备份结构体，用于 STOP/STANDBY 前后保存与恢复。
     */
    typedef struct
    {
        /*! Combination of basic SPI control configurations that are
         *  compressed to a single word as they are stored in the SPI
         *  registers See @ref DL_SPI_init for how the peripheral control word 0
         *  is created. */
        /*! 中文：CTL0 寄存器备份 */
        uint32_t controlWord0;

        /*! Combination of basic SPI control configurations that are
         *  compressed to a single word as they are stored in the SPI
         *  registers. See @ref DL_SPI_init for how the peripheral control word 1
         *  is created. */
        /*! 中文：CTL1 寄存器备份 */
        uint32_t controlWord1;

        /*! Combination of serial clock divider and delayed sampling settings
         *  compressed to a single word as they are stored in the SPI registers. */
        /*! 中文：CLKCTL 寄存器备份 */
        uint32_t clockControl;

        /*! SPI module clock source. One of  @ref DL_SPI_CLOCK */
        /*! 中文：时钟源备份 */
        uint32_t clockSel;

        /*! SPI clock divider. One of @ref DL_SPI_CLOCK_DIVIDE_RATIO */
        /*! 中文：分频系数备份 */
        uint32_t divideRatio;

        /*! Combination of SPI interrupt FIFO level select configurations that are
         *  compressed to a single word as they are stored in the SPI
         *  registers. */
        /*! 中文：IFLS 寄存器备份 */
        uint32_t interruptFifoLevelSelectWord;

        /*! SPI interrupt status for EVENT0.
         *  Bitwise OR of @ref DL_SPI_INTERRUPT */
        /*! 中文：CPU 中断掩码备份 */
        uint32_t interruptMask0;

        /*! SPI interrupt status for EVENT1.
         *  Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_RX */
        /*! 中文：DMA 接收触发中断掩码备份 */
        uint32_t interruptMask1;

        /*! SPI interrupt status for EVENT2.
         *  Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_TX */
        /*! 中文：DMA 发送触发中断掩码备份 */
        uint32_t interruptMask2;

        /*! Boolean flag indicating whether or not a valid configuration structure
         *  exists. Should not be modified by the user. */
        /*! 中文：备份有效标志，用户不应修改 */
        bool backupRdy;
    } DL_SPI_backupConfig;

    /**
     *  @brief      Initialize the SPI peripheral
     *
     *  Initializes all the common configurable options for the SPI peripheral. Any
     *  other custom configuration can be done after calling this API. The SPI is
     *  not enabled in this API.
     *  中文说明：完成 SPI 关键参数初始化，但不在本接口内使能外设。
     *
     *  @param[in]  spi     Pointer to the register overlay for the peripheral
     *                      参数：SPI 外设寄存器基址。
     *  @param[in]  config  Configuration for SPI peripheral
     *                      参数：SPI 初始化配置结构体。
     *
     *  @return     返回值：无。
     * @details    功能：参见上方英文说明。
     */
    void DL_SPI_init(SPI_Regs *spi, const DL_SPI_Config *config);

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the SPI
     * @brief 中文：使能 SPI 外设电源写保护寄存器。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  @note For power savings, please refer to @ref DL_SPI_enable
     *
     * @param spi        Pointer to the register overlay for the peripheral
     * @param spi        中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enablePower(SPI_Regs *spi)
    {
        spi->GPRCM.PWREN = (SPI_PWREN_KEY_UNLOCK_W | SPI_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the SPI
     * @brief 中文：关闭 SPI 外设电源写保护寄存器。
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @note This API does not provide large power savings. For power savings,
     *  please refer to @ref DL_SPI_enable
     *
     * @param spi        Pointer to the register overlay for the peripheral
     * @param spi        中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disablePower(SPI_Regs *spi)
    {
        spi->GPRCM.PWREN = (SPI_PWREN_KEY_UNLOCK_W | SPI_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the SPI
     *        is enabled
     * @brief 中文：查询 SPI 外设电源写保护是否已使能。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     * @param spi        Pointer to the register overlay for the peripheral
     * @param spi        中文：SPI 外设寄存器基址。
     *
     * @return true if peripheral register access is enabled
     * @return false if peripheral register access is disabled
     * @return 中文：`true` 表示已使能，`false` 表示已关闭。
     */
    __STATIC_INLINE bool DL_SPI_isPowerEnabled(const SPI_Regs *spi)
    {
        return (
            (spi->GPRCM.PWREN & SPI_PWREN_ENABLE_MASK) == SPI_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Resets spi peripheral
     * @brief 中文：复位 SPI 外设。
     *
     * @param spi        Pointer to the register overlay for the peripheral
     * @param spi        中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_reset(SPI_Regs *spi)
    {
        spi->GPRCM.RSTCTL =
            (SPI_RSTCTL_KEY_UNLOCK_W | SPI_RSTCTL_RESETSTKYCLR_CLR |
             SPI_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief Returns if spi peripheral was reset
     * @brief 中文：查询 SPI 外设是否已复位。
     *
     * @param spi        Pointer to the register overlay for the peripheral
     * @param spi        中文：SPI 外设寄存器基址。
     *
     * @return true if peripheral was reset
     * @return false if peripheral wasn't reset
     * @return 中文：`true` 表示已复位，`false` 表示未复位。
     */
    __STATIC_INLINE bool DL_SPI_isReset(const SPI_Regs *spi)
    {
        return ((spi->GPRCM.STAT & SPI_GPRCM_STAT_RESETSTKY_MASK) ==
                SPI_GPRCM_STAT_RESETSTKY_RESET);
    }

    /**
     *  @brief      Enable the SPI peripheral
     *  @brief      中文：使能 SPI 外设。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enable(SPI_Regs *spi)
    {
        spi->CTL1 |= SPI_CTL1_ENABLE_ENABLE;
    }

    /**
     *  @brief      Checks if the SPI peripheral is enabled
     *  @brief      中文：查询 SPI 外设是否已使能。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     Returns the enabled status of the SPI
     *
     *  @retval     true  The SPI peripheral is enabled
     *  @retval     false The SPI peripheral is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isEnabled(const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_ENABLE_MASK) == SPI_CTL1_ENABLE_ENABLE);
    }

    /**
     *  @brief      Disable the SPI peripheral
     *  @brief      中文：禁用 SPI 外设。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disable(SPI_Regs *spi)
    {
        spi->CTL1 &= ~(SPI_CTL1_ENABLE_MASK);
    }

    /**
     *  @brief      Configure SPI source clock
     *  @details    功能：配置 SPI 时钟源与分频值。
     *
     *  @param[in]  spi    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：SPI 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                       @ref DL_SPI_ClockConfig.
     *                       参数：时钟配置结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_SPI_setClockConfig(SPI_Regs *spi, const DL_SPI_ClockConfig *config);

    /**
     *  @brief      Get SPI source clock configuration
     *  @details    功能：读取 SPI 当前时钟源与分频配置。
     *
     *  @param[in]  spi    Pointer to the register overlay for the
     *                      peripheral
     *                      参数：SPI 外设寄存器基址。
     *  @param[in]  config  Pointer to the clock configuration struct
     *                      @ref DL_SPI_ClockConfig.
     *                      参数：用于回填配置的结构体指针。
     *
     *  @return     返回值：无。
     */
    void DL_SPI_getClockConfig(const SPI_Regs *spi, DL_SPI_ClockConfig *config);

    /**
     *  @brief      Checks if the SPI is busy transmitting
     *  @brief      中文：查询 SPI 是否正在传输。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     Returns the busy status of the SPI
     *
     *  @retval     true  The SPI is transmitting
     *  @retval     false The SPI is idle
     *  @return     中文：`true` 忙碌，`false` 空闲。
     */
    __STATIC_INLINE bool DL_SPI_isBusy(const SPI_Regs *spi)
    {
        return ((spi->STAT & SPI_STAT_BUSY_MASK) == SPI_STAT_BUSY_ACTIVE);
    }

    /**
     *  @brief      Checks if the TX FIFO is empty
     *  @brief      中文：查询发送 FIFO 是否为空。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     Returns the empty status of the TX FIFO
     *
     *  @retval     true  The TX FIFO is empty
     *  @retval     false The TX FIFO is not empty
     *  @return     中文：`true` 为空，`false` 非空。
     */
    __STATIC_INLINE bool DL_SPI_isTXFIFOEmpty(const SPI_Regs *spi)
    {
        return ((spi->STAT & SPI_STAT_TFE_MASK) == SPI_STAT_TFE_EMPTY);
    }

    /**
     *  @brief      Checks if the TX FIFO is full
     *  @brief      中文：查询发送 FIFO 是否已满。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     Returns the full status of the TX FIFO
     *
     *  @retval     true  The TX FIFO is full
     *  @retval     false The TX FIFO is not full
     *  @return     中文：`true` 已满，`false` 未满。
     */
    __STATIC_INLINE bool DL_SPI_isTXFIFOFull(const SPI_Regs *spi)
    {
        return ((spi->STAT & SPI_STAT_TNF_MASK) == SPI_STAT_TNF_FULL);
    }

    /**
     *  @brief      Checks if the RX FIFO is empty
     *  @brief      中文：查询接收 FIFO 是否为空。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     Returns the empty status of the RX FIFO
     *
     *  @retval     true  The RX FIFO is empty
     *  @retval     false The RX FIFO is not empty
     *  @return     中文：`true` 为空，`false` 非空。
     */
    __STATIC_INLINE bool DL_SPI_isRXFIFOEmpty(const SPI_Regs *spi)
    {
        return ((spi->STAT & SPI_STAT_RFE_MASK) == SPI_STAT_RFE_EMPTY);
    }

    /**
     *  @brief      Checks if the RX FIFO is full
     *  @brief      中文：查询接收 FIFO 是否已满。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     Returns the full status of the RX FIFO
     *
     *  @retval     true  The RX FIFO is full
     *  @retval     false The RX FIFO is not full
     *  @return     中文：`true` 已满，`false` 未满。
     */
    __STATIC_INLINE bool DL_SPI_isRXFIFOFull(const SPI_Regs *spi)
    {
        return ((spi->STAT & SPI_STAT_RNF_MASK) == SPI_STAT_RNF_FULL);
    }

    /**
     *  @brief      Sets the parity configuration used for transactions
     *  @brief      中文：设置收发奇偶校验配置。
     *
     *  This API sets the configuration for both receive parity and transmit parity.
     *  This includes which bit is used, and whether even or odd parity is set.
     *
     *  To individually enable or dsiable the receive or transmit parity, refer to
     *  the APIs listed below.
     *
     *  @param[in]  spi     Pointer to the register overlay for the peripheral
     *  @param[in]  spi     中文：SPI 外设寄存器基址。
     *  @param[in]  parity  Parity configuration to use. One of @ref DL_SPI_PARITY.
     *  @param[in]  parity  中文：奇偶校验模式。
     *
     *  @sa         DL_SPI_init
     *  @sa         DL_SPI_enableReceiveParity
     *  @sa         DL_SPI_disableReceiveParity
     *  @sa         DL_SPI_enableTransmitParity
     *  @sa         DL_SPI_disableTransmitParity
     */
    __STATIC_INLINE void DL_SPI_setParity(SPI_Regs *spi, DL_SPI_PARITY parity)
    {
        DL_Common_updateReg(&spi->CTL1, (uint32_t)parity,
                            (SPI_CTL1_PREN_MASK | SPI_CTL1_PTEN_MASK | SPI_CTL1_PES_MASK));
    }

    /**
     *  @brief      Get the current receive and transmit parity configuration
     *  @brief      中文：获取当前奇偶校验配置。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The current parity configuration being used
     *
     *  @retval     One of @ref DL_SPI_PARITY
     *  @return     中文：当前校验模式。
     */
    __STATIC_INLINE DL_SPI_PARITY DL_SPI_getParity(const SPI_Regs *spi)
    {
        uint32_t parity = spi->CTL1 & (SPI_CTL1_PES_MASK | SPI_CTL1_PREN_MASK |
                                       SPI_CTL1_PTEN_MASK);

        return (DL_SPI_PARITY)(parity);
    }

    /**
     *  @brief      Enables receive parity
     *  @brief      中文：使能接收奇偶校验。
     *
     *  This API only enables receive parity, it does not configure the parity mode
     *  used.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @sa         DL_SPI_setParity
     */
    __STATIC_INLINE void DL_SPI_enableReceiveParity(SPI_Regs *spi)
    {
        spi->CTL1 |= SPI_CTL1_PREN_ENABLE;
    }

    /**
     *  @brief      Disables receive parity
     *  @brief      中文：禁用接收奇偶校验。
     *
     *  This API only disable receive parity, it does not configure the parity mode
     *  used.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @sa         DL_SPI_setParity
     */
    __STATIC_INLINE void DL_SPI_disableReceiveParity(SPI_Regs *spi)
    {
        spi->CTL1 &= ~(SPI_CTL1_PREN_MASK);
    }

    /**
     *  @brief      Checks if receive parity is enabled
     *  @brief      中文：查询接收奇偶校验是否已使能。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     If receive parity is enabled
     *
     *  @retval     true   Receive parity is enabled
     *  @retval     false  Receive parity is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isReceiveParityEnabled(const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_PREN_MASK) == SPI_CTL1_PREN_ENABLE);
    }

    /**
     *  @brief      Enables transmit parity
     *  @brief      中文：使能发送奇偶校验。
     *
     *  This API only enables transmit parity, it does not configure the parity mode
     *  used.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @sa         DL_SPI_setParity
     */
    __STATIC_INLINE void DL_SPI_enableTransmitParity(SPI_Regs *spi)
    {
        spi->CTL1 |= SPI_CTL1_PTEN_ENABLE;
    }

    /**
     *  @brief      Disables transmit parity
     *  @brief      中文：禁用发送奇偶校验。
     *
     *  This API only disables transmit parity, it does not configure the parity
     *  mode used.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @sa         DL_SPI_setParity
     */
    __STATIC_INLINE void DL_SPI_disableTransmitParity(SPI_Regs *spi)
    {
        spi->CTL1 &= ~(SPI_CTL1_PTEN_MASK);
    }

    /**
     *  @brief      Checks if transmit parity is enabled
     *  @brief      中文：查询发送奇偶校验是否已使能。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     If transmit parity is enabled
     *
     *  @retval     true   Transmit parity is enabled
     *  @retval     false  Transmit parity is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isTransmitParityEnabled(const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_PTEN_MASK) == SPI_CTL1_PTEN_ENABLE);
    }

    /**
     *  @brief      Set the frame format to use
     *  @brief      中文：设置 SPI 帧格式。
     *
     *  Configures the frame format to use for transactions. If you are using chip
     *  select you must use one of the Motorola 4 wire frame formats.
     *
     *  @param[in]  spi          Pointer to the register overlay for the peripheral
     *  @param[in]  spi          中文：SPI 外设寄存器基址。
     *  @param[in]  frameFormat  Frame format to use. One of @ref
     *                           DL_SPI_FRAME_FORMAT.
     *  @param[in]  frameFormat  中文：帧格式。
     *
     *  @sa         DL_SPI_init
     */
    __STATIC_INLINE void DL_SPI_setFrameFormat(
        SPI_Regs *spi, DL_SPI_FRAME_FORMAT frameFormat)
    {
        DL_Common_updateReg(&spi->CTL0, (uint32_t)frameFormat,
                            (SPI_CTL0_FRF_MASK | SPI_CTL0_SPO_MASK | SPI_CTL0_SPH_MASK));
    }

    /**
     *  @brief      Get the frame format configuration
     *  @brief      中文：获取当前帧格式配置。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The current frame format being used
     *
     *  @retval     One of @ref DL_SPI_FRAME_FORMAT
     *  @return     中文：当前帧格式。
     */
    __STATIC_INLINE DL_SPI_FRAME_FORMAT DL_SPI_getFrameFormat(const SPI_Regs *spi)
    {
        uint32_t frameFormat = spi->CTL0 & (SPI_CTL0_FRF_MASK | SPI_CTL0_SPO_MASK |
                                            SPI_CTL0_SPH_MASK);

        return (DL_SPI_FRAME_FORMAT)(frameFormat);
    }

    /**
     *  @brief      Set the size for transfers
     *  @brief      中文：设置数据传输位宽。
     *
     *  @param[in]  spi       Pointer to the register overlay for the peripheral
     *  @param[in]  spi       中文：SPI 外设寄存器基址。
     *  @param[in]  dataSize  Number of bits used in a transfer.
     *                        One of @ref DL_SPI_DATA_SIZE
     *  @param[in]  dataSize  中文：数据位宽。
     *
     *  @sa         DL_SPI_init
     */
    __STATIC_INLINE void DL_SPI_setDataSize(
        SPI_Regs *spi, DL_SPI_DATA_SIZE dataSize)
    {
        DL_Common_updateReg(&spi->CTL0, (uint32_t)dataSize, SPI_CTL0_DSS_MASK);
    }

    /**
     *  @brief      Get the configured size for transfers
     *  @brief      中文：获取当前数据传输位宽。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The currently configured size for transfers
     *
     *  @retval     One of @ref DL_SPI_DATA_SIZE
     *  @return     中文：当前数据位宽。
     */
    __STATIC_INLINE DL_SPI_DATA_SIZE DL_SPI_getDataSize(const SPI_Regs *spi)
    {
        uint32_t dataSize = spi->CTL0 & SPI_CTL0_DSS_MASK;

        return (DL_SPI_DATA_SIZE)(dataSize);
    }

    /**
     *  @brief      Set whether the device should be in controller/peripheral mode
     *  @brief      中文：设置 SPI 主机/从机模式。
     *
     *  @param[in]  spi   Pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *  @param[in]  mode  Mode to configure the SPI into. One of @ref DL_SPI_MODE.
     *  @param[in]  mode  中文：主从模式。
     *
     *  @sa         DL_SPI_init
     */
    __STATIC_INLINE void DL_SPI_setMode(SPI_Regs *spi, DL_SPI_MODE mode)
    {
        DL_Common_updateReg(&spi->CTL1, (uint32_t)mode, SPI_CTL1_CP_MASK);
    }

    /**
     *  @brief      Get the current mode for the SPI (controller/peripheral)
     *  @brief      中文：获取当前主从模式。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The currently configured mode for the SPI (controller/peripheral)
     *
     *  @retval     One of @ref DL_SPI_MODE.
     *  @return     中文：当前主从模式。
     */
    __STATIC_INLINE DL_SPI_MODE DL_SPI_getMode(const SPI_Regs *spi)
    {
        uint32_t mode = spi->CTL1 & SPI_CTL1_CP_MASK;

        return (DL_SPI_MODE)(mode);
    }

    /**
     *  @brief      Set the bit order used for transfers
     *  @brief      中文：设置传输位序。
     *
     *  @param[in]  spi       Pointer to the register overlay for the peripheral
     *  @param[in]  spi       中文：SPI 外设寄存器基址。
     *  @param[in]  bitOrder  Order for bits to be sent out during transfer. One of
     *                        @ref DL_SPI_BIT_ORDER.
     *  @param[in]  bitOrder  中文：位序（高位/低位在前）。
     *
     *  @sa         DL_SPI_init
     */
    __STATIC_INLINE void DL_SPI_setBitOrder(
        SPI_Regs *spi, DL_SPI_BIT_ORDER bitOrder)
    {
        DL_Common_updateReg(&spi->CTL1, (uint32_t)bitOrder, SPI_CTL1_MSB_MASK);
    }

    /**
     *  @brief      Get the current bit order used for transfers
     *  @brief      中文：获取当前位序配置。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The currently configured bit order
     *
     *  @retval     One of @ref DL_SPI_BIT_ORDER.
     *  @return     中文：当前位序。
     */
    __STATIC_INLINE DL_SPI_BIT_ORDER DL_SPI_getBitOrder(const SPI_Regs *spi)
    {
        uint32_t bitOrder = spi->CTL1 & SPI_CTL1_MSB_MASK;

        return (DL_SPI_BIT_ORDER)(bitOrder);
    }

    /**
     *  @brief      Enables loopback mode
     *  @brief      中文：使能回环模式。
     *
     *  Enables the loopback mode. When enabled, the output of the transmit serial
     *  shifter is connected to the input of the receive serial shifter internally.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enableLoopbackMode(SPI_Regs *spi)
    {
        spi->CTL1 |= SPI_CTL1_LBM_ENABLE;
    }

    /**
     *  @brief      Disables loopback mode
     *  @brief      中文：禁用回环模式。
     *
     *  Disables the loopback mode. When disabled, the transmit serial shifter and
     *  receive serial shifter are not connected internally.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disableLoopbackMode(SPI_Regs *spi)
    {
        spi->CTL1 &= ~(SPI_CTL1_LBM_MASK);
    }

    /**
     *  @brief      Checks if the loopback mode is enabled
     *  @brief      中文：查询回环模式是否已使能。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The status of the loopback mode
     *
     *  @retval     true if loopback mode is enabled
     *  @retval     false if loopback mode is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isLoopbackModeEnabled(const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_LBM_MASK) == SPI_CTL1_LBM_ENABLE);
    }

    /**
     *  @brief      Set counter for repeated transmit
     *  @brief      中文：设置重复发送计数器。
     *
     *  Repeated transmit allows you to send the same data multiple times. This is
     *  essentially the same as writing the data into the transmit buffer
     *  repeatedly.
     *
     *  @param[in]  spi         pointer to the register overlay for the peripheral
     *  @param[in]  spi         中文：SPI 外设寄存器基址。
     *  @param[in]  numRepeats  number of times to repeat the transfer. Should be a
     *                          value between 0-255.
     *                          @arg 0 disables the repeated transfer
     *                          @arg 1-255 repeats that many times. So will be sent
     *                               numRepeats + 1 times in total.
     *  @param[in]  numRepeats  中文：重复次数（0-255），0 禁用，1-255 表示额外重复次数。
     */
    __STATIC_INLINE void DL_SPI_setRepeatTransmit(
        SPI_Regs *spi, uint32_t numRepeats)
    {
        DL_Common_updateReg(&spi->CTL1, numRepeats << SPI_CTL1_REPEATTX_OFS,
                            SPI_CTL1_REPEATTX_MASK);
    }

    /**
     *  @brief      Get counter for repeated transmit
     *  @brief      中文：获取重复发送计数器值。
     *
     *  Repeated transmit allows you to send the same data multiple times. This is
     *  essentially the same as writing the data into the transmit buffer
     *  repeatedly.
     *
     *  @param[in]  spi    pointer to the register overlay for the peripheral
     *  @param[in]  spi    中文：SPI 外设寄存器基址。
     *
     *  @return     Number of repeats used for transfer
     *
     *  @retval     0      Repeated transfer is disabled
     *  @retval     1-255  Repeat that many times.
     *  @return     中文：重复次数（0-255）。
     */
    __STATIC_INLINE uint32_t DL_SPI_getRepeatTransmit(const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_REPEATTX_MASK) >> SPI_CTL1_REPEATTX_OFS);
    }

    /**
     *  @brief      Enables data alignment on chip select for peripherals
     *  @brief      中文：使能从机片选数据对齐。
     *
     *  When enabled, the receieve bit counter is cleared automatically when the
     *  chip select gets set inactive. This helps the peripheral synchronize again to
     *  the controller in case of a disturbance or glitch on the clock line or during
     *  initialization.
     *
     *  This is only relevant when in peripheral mode.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enablePeripheralAlignDataOnChipSelect(
        SPI_Regs *spi)
    {
        spi->CTL0 |= SPI_CTL0_CSCLR_ENABLE;
    }

    /**
     *  @brief      Disables data alignment on chip select for peripherals
     *  @brief      中文：禁用从机片选数据对齐。
     *
     *  When disable, the receieve bit counter is not cleared automatically when
     *  the chip select gets set inactive.
     *
     *  This is only relevant when in peripheral mode.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disablePeripheralAlignDataOnChipSelect(
        SPI_Regs *spi)
    {
        spi->CTL0 &= ~(SPI_CTL0_CSCLR_MASK);
    }

    /**
     *  @brief      Checks if data alignment on chip select for peripherals is enabled
     *  @brief      中文：查询从机片选数据对齐是否已使能。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The status of data alignment on chip select
     *
     *  @retval     true   Data alignment on chip select is enabled
     *  @retval     false  Data alignment on chip select is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isPeripheralAlignDataOnChipSelectEnabled(
        const SPI_Regs *spi)
    {
        return ((spi->CTL0 & SPI_CTL0_CSCLR_MASK) == SPI_CTL0_CSCLR_ENABLE);
    }

    /**
     *  @brief      Enables packing feature
     *  @brief      中文：使能 FIFO 打包功能。
     *
     *  When enabled, two entries of the RX FIFO are returned as a 32-bit value.
     *  When reading the TX FIFO, if the last write to that field was a 32-bit
     *  write, those 32-bits will be returned.
     *  When writing to the TX FIFO, a 32-bit write will be written as one FIFO
     *  entry.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enablePacking(SPI_Regs *spi)
    {
        spi->CTL0 |= SPI_CTL0_PACKEN_ENABLED;
    }

    /**
     *  @brief      Disables packing feature
     *  @brief      中文：禁用 FIFO 打包功能。
     *
     *  When disabled, 1 entry of the RX FIFO is returned as a 16-bit value.
     *  When reading the TX FIFO, if the last write to that field was a 16-bit
     *  write, those 16-bits wil be returned.
     *  When writing to the TX FIFO, a 32-bit write will be written as two FIFO
     *  entries.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disablePacking(SPI_Regs *spi)
    {
        spi->CTL0 &= ~(SPI_CTL0_PACKEN_MASK);
    }

    /**
     *  @brief      Checks if packing feature is enabled
     *  @brief      中文：查询 FIFO 打包功能是否已使能。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     If packing is enabled
     *
     *  @retval     true   Packing is enabled
     *  @retval     false  Packing is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isPackingEnabled(const SPI_Regs *spi)
    {
        return ((spi->CTL0 & SPI_CTL0_PACKEN_MASK) == SPI_CTL0_PACKEN_ENABLED);
    }

    /**
     *  @brief      Set chip select used for controller or peripheral mode
     *  @brief      中文：设置片选引脚。
     *
     *  Choose which chip select should be used for data transfer. User must ensure
     *  one of the 4-wire frame formats were selected using @ref DL_SPI_init or
     *  @ref DL_SPI_setFrameFormat.
     *
     * This API is for both controller and peripheral modes, and the chip select
     * can be changed in the application by calling this API.
     *
     *  @param[in]  spi         pointer to the register overlay for the peripheral
     *  @param[in]  spi         中文：SPI 外设寄存器基址。
     *  @param[in]  chipSelect  the chip select pin to use. One of @ref
     *                          DL_SPI_CHIP_SELECT.
     *  @param[in]  chipSelect  中文：片选引脚编号。
     */
    __STATIC_INLINE void DL_SPI_setChipSelect(
        SPI_Regs *spi, DL_SPI_CHIP_SELECT chipSelect)
    {
        DL_Common_updateReg(
            &spi->CTL0, (uint32_t)chipSelect, SPI_CTL0_CSSEL_MASK);
    }

    /**
     *  @brief      Get chip select used for controller or peripheral mode
     *  @brief      中文：获取当前片选引脚配置。
     *
     *  This API is for both controller and peripheral modes.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The current chip selected used for data transfer
     *
     *  @retval     One of @ref DL_SPI_CHIP_SELECT.
     *  @return     中文：当前片选引脚。
     */
    __STATIC_INLINE DL_SPI_CHIP_SELECT DL_SPI_getChipSelect(const SPI_Regs *spi)
    {
        uint32_t chipSelect = spi->CTL0 & SPI_CTL0_CSSEL_MASK;

        return (DL_SPI_CHIP_SELECT)(chipSelect);
    }

    /**
     *  @brief      Set peripheral receive timeout
     *  @brief      中文：设置从机接收超时周期。
     *
     *  Sets the number of clock cycles before a receive timeout occurs.
     *
     *  This is only relevant when in peripheral mode.
     *
     *  @param[in]  spi      pointer to the register overlay for the peripheral
     *  @param[in]  spi      中文：SPI 外设寄存器基址。
     *  @param[in]  timeout  Number of clock cycles before a receive timeout
     *                       occurs. Must be between 0-63.
     *                       @arg 0 Disables receive timeout
     *                       @arg 1-63 Number of clock cycles before timeout
     *  @param[in]  timeout  中文：超时周期数（0-63），0 禁用超时。
     */
    __STATIC_INLINE void DL_SPI_setPeripheralReceiveTimeout(
        SPI_Regs *spi, uint32_t timeout)
    {
        DL_Common_updateReg(&spi->CTL1, timeout << SPI_CTL1_RXTIMEOUT_OFS,
                            SPI_CTL1_RXTIMEOUT_MASK);
    }

    /**
     *  @brief      Get peripheral receive timeout
     *  @brief      中文：获取从机接收超时周期配置。
     *
     *  This is only relevant when in peripheral mode.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *
     *  @return     The current amount of cycles used for a peripheral timeout
     *
     *  @retval     0     indicating that receive timeout is disabled
     *  @retval     1-63  number of clock cycles before a receive timeout occurs
     *  @return     中文：超时周期数（0-63）。
     */
    __STATIC_INLINE uint32_t DL_SPI_getPeripheralReceiveTimeout(
        const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_RXTIMEOUT_MASK) >> SPI_CTL1_RXTIMEOUT_OFS);
    }

    /**
     *  @brief      Configure the command/data mode
     *  @brief      中文：配置主机命令/数据模式。
     *
     *  Command/data mode allows the hardware to control the C/D pin to indicate
     *  whether the data being transferred is a command or just data. The C/D pin
     *  is low for commands and high for data.
     *
     *  You can use this API to manually set the C/D pin by passing in @ref
     *  DL_SPI_CD_MODE_DATA or @ref DL_SPI_CD_MODE_COMMAND. You can also have the
     *  C/D pin switch automatically by passing in how many bytes are part of the
     *  command transfer. The C/D pin will stay low until that many bytes are sent
     *  out and then automatically switch to high.
     *
     *  This is only relevant when in controller mode.
     *
     *  @param[in]  spi    pointer to the register overlay for the peripheral
     *  @param[in]  spi    中文：SPI 外设寄存器基址。
     *  @param[in]  config configuration for command/data mode. One of:
     *                     @arg DL_SPI_CD_MODE_DATA
     *                     @arg DL_SPI_CD_MODE_COMMAND
     *                     @arg A value between 1-14 to indicate how many bytes
     *                          should be sent as command
     *  @param[in]  config 中文：C/D 模式配置（DATA/COMMAND 或 1-14 字节数）。
     */
    __STATIC_INLINE void DL_SPI_setControllerCommandDataModeConfig(
        SPI_Regs *spi, uint32_t config)
    {
        DL_Common_updateReg(
            &spi->CTL1, config << SPI_CTL1_CDMODE_OFS, SPI_CTL1_CDMODE_MASK);
    }

    /**
     *  @brief      Get the command/data mode configuration
     *  @brief      中文：获取主机命令/数据模式配置。
     *
     *  Returns the current configuration for command/data mode. When using the
     *  automatic C/D pin the value read back will be the number of command bytes
     *  remaining to be sent.
     *
     *  @param[in]  spi    pointer to the register overlay for the peripheral
     *  @param[in]  spi    中文：SPI 外设寄存器基址。
     *
     *  @return     The current configuration for command/data mode
     *
     *  @retval     DL_SPI_CD_MODE_DATA    if in manual data mode
     *  @retval     DL_SPI_CD_MODE_COMMAND if in manual command mode
     *  @retval     1-14 indicating how many command bytes still need to be sent
     *  @return     中文：C/D 模式配置值。
     */
    __STATIC_INLINE uint32_t DL_SPI_getControllerCommandDataModeConfig(
        const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_CDMODE_MASK) >> SPI_CTL1_CDMODE_OFS);
    }

    /**
     *  @brief      Enables command/data mode
     *  @brief      中文：使能主机命令/数据模式。
     *
     *  When command/data mode is enabled, the C/D pin is used to differentiate
     *  between command and data during the transaction.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @sa         DL_SPI_setControllerCommandDataModeConfig
     */
    __STATIC_INLINE void DL_SPI_enableControllerCommandDataMode(SPI_Regs *spi)
    {
        spi->CTL1 |= SPI_CTL1_CDENABLE_ENABLE;
    }

    /**
     *  @brief      Disables command/data mode
     *  @brief      中文：禁用主机命令/数据模式。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disableControllerCommandDataMode(SPI_Regs *spi)
    {
        spi->CTL1 &= ~(SPI_CTL1_CDENABLE_MASK);
    }

    /**
     *  @brief      Checks if command/data mode is enabled
     *  @brief      中文：查询主机命令/数据模式是否已使能。
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The status of command/data mode
     *
     *  @retval     true  Command/data mode is enabled
     *  @retval     false Command/data mode is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isControllerCommandDataModeEnabled(
        const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_CDENABLE_MASK) == SPI_CTL1_CDENABLE_ENABLE);
    }

    /**
     *  @brief      Enables peripheral data output
     *  @brief      中文：使能从机数据输出。
     *
     *  When peripheral data output is enabled, the peripheral can drive the POCI output pin.
     *  This will cause problems if all peripherals have their RXD lines tied together
     *  and the controller is trying to broadcast a message to all peripherals while
     *  ensuring only one peripheral drives data onto its serial output line.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enablePeripheralDataOutput(SPI_Regs *spi)
    {
        spi->CTL1 &= ~(SPI_CTL1_POD_MASK);
    }

    /**
     *  @brief      Disables peripheral data output
     *  @brief      中文：禁用从机数据输出。
     *
     *  When peripheral data output is disabled, the peripheral cannot drive the POCI output
     *  pin. This allows multiple peripherals that have their RXD lines tied together to
     *  receive a broadcasted message from a controller.
     *
     *  This is only relevant when in peripheral mode.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disablePeripheralDataOutput(SPI_Regs *spi)
    {
        spi->CTL1 |= SPI_CTL1_POD_ENABLE;
    }

    /**
     *  @brief      Checks if peripheral data output is enabled
     *  @brief      中文：查询从机数据输出是否已使能。
     *
     *  This is only relevant when in peripheral mode.
     *
     *  @param[in]  spi  pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     If peripheral data output is enabled
     *
     *  @retval     true if peripheral data output is enabled
     *  @retval     false if peripheral data output is disabled
     *  @return     中文：`true` 已使能，`false` 已禁用。
     */
    __STATIC_INLINE bool DL_SPI_isPeripheralDataOutputEnabled(const SPI_Regs *spi)
    {
        return ((spi->CTL1 & SPI_CTL1_POD_MASK) == SPI_CTL1_POD_DISABLE);
    }

    /**
     *  @brief      Set the delay sampling
     *  @brief      中文：设置延迟采样周期。
     *
     *  In controller mode only, the data on the input pin will be delayed sampled
     *  by the defined SPI clock cycles. The delay can be adjusted in steps of SPI
     *  input clock steps. The maximum allowed delay should not exceed the length
     *  of one data frame
     *
     *  @param[in]  spi     Pointer to the register overlay for the peripheral
     *  @param[in]  spi     中文：SPI 外设寄存器基址。
     *  @param[in]  delay   The number of SPI clock cycles to delay sampling on
     *                      input pin. Value between 0-15.
     *  @param[in]  delay   中文：延迟采样周期数（0-15）。
     */
    __STATIC_INLINE void DL_SPI_setDelayedSampling(SPI_Regs *spi, uint32_t delay)
    {
        DL_Common_updateReg(&spi->CLKCTL, delay << SPI_CLKCTL_DSAMPLE_OFS,
                            SPI_CLKCTL_DSAMPLE_MASK);
    }

    /**
     *  @brief      Get the delay sampling
     *  @brief      中文：获取延迟采样周期配置。
     *
     *  In controller mode only, the data on the input pin will be delayed sampled
     *  by the defined SPI clock cycles. The delay can be adjusted in steps of SPI
     *  input clock steps. The maximum allowed delay should not exceed the length
     *  of one data frame
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The amount of delay sampling on the input pin in SPI
     *              clock cycles.
     *
     *  @retval     0 - 15. The amount of delay sampling in SPI clock cycles.
     *  @return     中文：延迟采样周期数（0-15）。
     */
    __STATIC_INLINE uint32_t DL_SPI_getDelayedSampling(const SPI_Regs *spi)
    {
        return ((spi->CLKCTL & SPI_CLKCTL_DSAMPLE_MASK) >> SPI_CLKCTL_DSAMPLE_OFS);
    }

    /**
     *  @brief      Set the RX and TX FIFO interrupt threshold level
     *  @brief      中文：设置收发 FIFO 中断阈值。
     *
     *  Select the threshold for the receive and transmit FIFO interrupts. The
     *  interrupts are generated based on a transition through a level rather
     *  than being based on the level. That is, the interrupts are generated when
     *  the fill level progresses through the trigger level.
     *
     *  For example, if the trigger level is set to the half-way mark, the interrupt
     *  is triggered when the FIFO becomes half empty/full.
     *
     *  Out of reset, the FIFOs are triggered to interrupt at half-way mark.
     *
     *  @param[in]  spi             Pointer to the register overlay for the peripheral
     *  @param[in]  spi             中文：SPI 外设寄存器基址。
     *  @param[in]  rxThreshold     One of @ref DL_SPI_RX_FIFO_LEVEL
     *  @param[in]  rxThreshold     中文：接收 FIFO 中断阈值。
     *  @param[in]  txThreshold     One of @ref DL_SPI_TX_FIFO_LEVEL
     *  @param[in]  txThreshold     中文：发送 FIFO 中断阈值。
     *
     */
    __STATIC_INLINE void DL_SPI_setFIFOThreshold(SPI_Regs *spi,
                                                 DL_SPI_RX_FIFO_LEVEL rxThreshold, DL_SPI_TX_FIFO_LEVEL txThreshold)
    {
        DL_Common_updateReg(&spi->IFLS,
                            (uint32_t)rxThreshold | (uint32_t)txThreshold,
                            SPI_IFLS_RXIFLSEL_MASK | SPI_IFLS_TXIFLSEL_MASK);
    }

    /**
     *  @brief      Get the TX FIFO interrupt threshold level
     *  @brief      中文：获取发送 FIFO 中断阈值。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The TX FIFO interrupt threshold level
     *
     *  @retval     One of @ref DL_SPI_TX_FIFO_LEVEL
     *  @return     中文：发送 FIFO 中断阈值。
     */
    __STATIC_INLINE DL_SPI_TX_FIFO_LEVEL DL_SPI_getTXFIFOThreshold(
        const SPI_Regs *spi)
    {
        uint32_t txThreshold = spi->IFLS & SPI_IFLS_TXIFLSEL_MASK;

        return (DL_SPI_TX_FIFO_LEVEL)(txThreshold);
    }

    /**
     *  @brief      Get the RX FIFO interrupt threshold level
     *  @brief      中文：获取接收 FIFO 中断阈值。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The RX FIFO interrupt threshold level
     *
     *  @retval     One of @ref DL_SPI_RX_FIFO_LEVEL
     *  @return     中文：接收 FIFO 中断阈值。
     */
    __STATIC_INLINE DL_SPI_RX_FIFO_LEVEL DL_SPI_getRXFIFOThreshold(
        const SPI_Regs *spi)
    {
        uint32_t rxThreshold = spi->IFLS & SPI_IFLS_RXIFLSEL_MASK;

        return (DL_SPI_RX_FIFO_LEVEL)(rxThreshold);
    }

    /**
     *  @brief      Set the SPI bit rate serial clock divider (SCR)
     *  @brief      中文：设置 SPI 波特率串行时钟分频器 (SCR)。
     *
     * The SPI includes a programmable bit rate clock divider and prescaler to
     * generate the serial output clock. The bit rates are supported up to
     * FUNCCLK/2. The functional clock selection depends on the specific device,
     * please refer to the device datasheet and PMU/Clock section.
     *
     * The SPI output bit rate is calculated with the following formula:
     * SPI bit rate = (SPI functional clock) / ((1 + SCR)*2)
     * Given this formula, the SCR can be calculated:
     * SCR = (SPI functional clock) / ((2 * SPI bit rate) - 1)
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *  @param[in]  SCR  The SPI serial clock divider. Value between 0-1023.
     *  @param[in]  SCR  中文：串行时钟分频值（0-1023）。
     */
    __STATIC_INLINE void DL_SPI_setBitRateSerialClockDivider(
        SPI_Regs *spi, uint32_t SCR)
    {
        DL_Common_updateReg(&spi->CLKCTL, SCR, SPI_CLKCTL_SCR_MASK);
    }

    /**
     *  @brief      Get the SPI bit rate serial clock divider (SCR)
     *  @brief      中文：获取 SPI 波特率串行时钟分频器 (SCR) 值。
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *  @param[in]  spi  中文：SPI 外设寄存器基址。
     *
     *  @return     The current bit rate serial clock divider
     *
     *  @retval     The SPI SCR. Value from 0-1023
     *  @return     中文：当前 SCR 值（0-1023）。
     */
    __STATIC_INLINE uint32_t DL_SPI_getBitRateSerialClockDivider(
        const SPI_Regs *spi)
    {
        return (spi->CLKCTL & SPI_CLKCTL_SCR_MASK);
    }

    /**
     *  @brief      Writes 8-bit data into the TX FIFO for transmit
     *  @brief      中文：向发送 FIFO 写入 8 位数据（无状态检查）。
     *
     *  Puts the data into the TX FIFO without checking its status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *  @param[in]  data  中文：待发送 8 位数据。
     *
     *  @sa         DL_SPI_transmitDataBlocking8
     *  @sa         DL_SPI_transmitDataCheck8
     */
    __STATIC_INLINE void DL_SPI_transmitData8(SPI_Regs *spi, uint8_t data)
    {
        spi->TXDATA = data;
    }

    /**
     *  @brief      Writes 16-bit data into the TX FIFO for transmit
     *  @brief      中文：向发送 FIFO 写入 16 位数据（无状态检查）。
     *
     *  Puts the data into the TX FIFO without checking its status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *  @param[in]  data  中文：待发送 16 位数据。
     *
     *  @sa         DL_SPI_transmitDataBlocking16
     *  @sa         DL_SPI_transmitDataCheck16
     */
    __STATIC_INLINE void DL_SPI_transmitData16(SPI_Regs *spi, uint16_t data)
    {
        spi->TXDATA = data;
    }

    /**
     *  @brief      Writes 32-bit data into the TX FIFO for transmit
     *  @brief      中文：向发送 FIFO 写入 32 位数据（无状态检查）。
     *
     *  Puts the data into the TX FIFO without checking its status. Use if already
     *  sure the TX FIFO has space for the write. See related APIs for additional
     *  transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: If packing is enabled by calling @ref DL_SPI_enablePacking prior to
     *  calling this API, then a 32-bit write will be written as one FIFO entry. If
     *  packing is disabled, then a 32-bit write will be written as two FIFO
     *  entries.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *  @param[in]  data  中文：待发送 32 位数据。
     *
     *  @sa         DL_SPI_enablePacking
     *  @sa         DL_SPI_transmitDataBlocking32
     *  @sa         DL_SPI_transmitDataCheck32
     */
    __STATIC_INLINE void DL_SPI_transmitData32(SPI_Regs *spi, uint32_t data)
    {
        spi->TXDATA = data;
    }

    /**
     *  @brief      Reads 8-bit data from the RX FIFO
     *  @brief      中文：从接收 FIFO 读取 8 位数据（无状态检查）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *  @return     中文：读取到的 8 位数据。
     *
     *  @sa         DL_SPI_receiveDataBlocking8
     *  @sa         DL_SPI_receiveDataCheck8
     */
    __STATIC_INLINE uint8_t DL_SPI_receiveData8(const SPI_Regs *spi)
    {
        return ((uint8_t)(spi->RXDATA));
    }

    /**
     *  @brief      Reads 16-bit data from the RX FIFO
     *  @brief      中文：从接收 FIFO 读取 16 位数据（无状态检查）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *  @return     中文：读取到的 16 位数据。
     *
     *  @sa         DL_SPI_receiveDataBlocking16
     *  @sa         DL_SPI_receiveDataCheck16
     */
    __STATIC_INLINE uint16_t DL_SPI_receiveData16(const SPI_Regs *spi)
    {
        return ((uint16_t)(spi->RXDATA));
    }

    /**
     *  @brief      Reads 32-bit data from the RX FIFO
     *  @brief      中文：从接收 FIFO 读取 32 位数据（需先使能打包）。
     *
     *  Reads the data from the RX FIFO without checking its status. Use if
     *  already sure the RX FIFO has data available. See related APIs for
     *  additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: Requires that packing has been enabled by calling
     *  @ref DL_SPI_enablePacking prior to calling this API. When packing is
     *  enabled, two entries of the RX FIFO are returned as a 32-bit value.
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *  @param[in]  spi   中文：SPI 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *  @return     中文：读取到的 32 位数据。
     *
     *  @pre         DL_SPI_enablePacking
     *
     *  @sa         DL_SPI_receiveDataBlocking32
     *  @sa         DL_SPI_receiveDataCheck32
     */
    __STATIC_INLINE uint32_t DL_SPI_receiveData32(const SPI_Regs *spi)
    {
        return spi->RXDATA;
    }

    /**
     *  @brief      Enable SPI interrupts
     *  @brief      中文：使能 SPI 中断。
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_SPI_INTERRUPT.
     *  @param[in]  interruptMask  中文：中断掩码（按位或）。
     */
    __STATIC_INLINE void DL_SPI_enableInterrupt(
        SPI_Regs *spi, uint32_t interruptMask)
    {
        spi->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable SPI interrupts
     *  @brief      中文：禁用 SPI 中断。
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
     *                             @ref DL_SPI_INTERRUPT.
     *  @param[in]  interruptMask  中文：中断掩码（按位或）。
     */
    __STATIC_INLINE void DL_SPI_disableInterrupt(
        SPI_Regs *spi, uint32_t interruptMask)
    {
        spi->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which SPI interrupts are enabled
     *  @brief      中文：查询哪些 SPI 中断已使能。
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_SPI_INTERRUPT.
     *  @param[in]  interruptMask  中文：待查询中断掩码。
     *
     *  @return     Which of the requested SPI interrupts are enabled
     *
     *  @retval     Bitwise OR of @ref DL_SPI_INTERRUPT values
     *  @return     中文：已使能的中断位掩码。
     */
    __STATIC_INLINE uint32_t DL_SPI_getEnabledInterrupts(
        const SPI_Regs *spi, uint32_t interruptMask)
    {
        return (spi->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled SPI interrupts
     *  @brief      中文：查询已使能 SPI 中断的挂起状态。
     *
     *  Checks if any of the SPI interrupts that were previously enabled are
     *  pending.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_SPI_INTERRUPT.
     *  @param[in]  interruptMask  中文：待查询中断掩码。
     *
     *  @return     Which of the requested SPI interrupts are pending
     *
     *  @retval     Bitwise OR of @ref DL_SPI_INTERRUPT values
     *  @return     中文：挂起的中断位掩码。
     *
     *  @sa         DL_SPI_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_SPI_getEnabledInterruptStatus(
        const SPI_Regs *spi, uint32_t interruptMask)
    {
        return (spi->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any SPI interrupt
     *  @brief      中文：查询 SPI 中断原始挂起状态（无需先使能）。
     *
     *  Checks if any of the SPI interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_SPI_INTERRUPT.
     *  @param[in]  interruptMask  中文：待查询中断掩码。
     *
     *  @return     Which of the requested SPI interrupts are pending
     *
     *  @retval     Bitwise OR of @ref DL_SPI_INTERRUPT values
     *  @return     中文：原始挂起中断位掩码。
     */
    __STATIC_INLINE uint32_t DL_SPI_getRawInterruptStatus(
        const SPI_Regs *spi, uint32_t interruptMask)
    {
        return (spi->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending SPI interrupt
     *  @brief      中文：获取最高优先级挂起中断索引。
     *
     *  Checks if any of the SPI interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *
     *  @return     The highest priority pending SPI interrupt. One of @ref
     *               DL_SPI_IIDX
     *  @return     中文：最高优先级挂起中断索引。
     */
    __STATIC_INLINE DL_SPI_IIDX DL_SPI_getPendingInterrupt(const SPI_Regs *spi)
    {
        return ((DL_SPI_IIDX)spi->CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending SPI interrupts
     *  @brief      中文：清除挂起的 SPI 中断标志。
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_SPI_INTERRUPT.
     *  @param[in]  interruptMask  中文：待清除中断掩码。
     */
    __STATIC_INLINE void DL_SPI_clearInterruptStatus(
        SPI_Regs *spi, uint32_t interruptMask)
    {
        spi->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts data into the TX FIFO, waiting indefintely until there is space in the
     *  the TX FIFO. After placing data, waits indefintely until data is sent and
     *  the SPI busy is no longer busy. Note that if data is already present in the
     *  TX FIFO when this API is called, it will block until all data is sent.
     *  See related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 8 位数据，并等待总线空闲。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 8 位数据。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_SPI_transmitData8
     *  @sa         DL_SPI_transmitDataCheck8
     * @details    功能：参见上方英文说明。
     */
    void DL_SPI_transmitDataBlocking8(SPI_Regs *spi, uint8_t data);

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts data into the TX FIFO, waiting indefintely until there is space in the
     *  the TX FIFO. After placing data, waits indefintely until data is sent and
     *  the SPI busy is no longer busy. Note that if data is already present in the
     *  TX FIFO when this API is called, it will block until all data is sent.
     *  See related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 16 位数据，并等待总线空闲。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 16 位数据。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_SPI_transmitData16
     *  @sa         DL_SPI_transmitDataCheck16
     * @details    功能：参见上方英文说明。
     */
    void DL_SPI_transmitDataBlocking16(SPI_Regs *spi, uint16_t data);

    /**
     *  @brief      Blocks to ensure transmit is ready before sending data
     *
     *  Puts data into the TX FIFO, waiting indefintely until there is space in the
     *  the TX FIFO. After placing data, waits indefintely until data is sent and
     *  the SPI busy is no longer busy. Note that if data is already present in the
     *  TX FIFO when this API is called, it will block until all data is sent.
     *  See related APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: If packing is enabled by calling @ref DL_SPI_enablePacking prior to
     *  calling this API, then a 32-bit write will be written as one FIFO entry. If
     *  packing is disabled, then a 32-bit write will be written as two FIFO
     *  entries.
     *  中文说明：阻塞等待 TX FIFO 可写后发送 32 位数据，并等待总线空闲。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 32 位数据。
     *
     *  @return     返回值：无。
     *
     *  @sa         DL_SPI_enablePacking
     *  @sa         DL_SPI_transmitData32
     *  @sa         DL_SPI_transmitDataCheck32
     * @details    功能：参见上方英文说明。
     */
    void DL_SPI_transmitDataBlocking32(SPI_Regs *spi, uint32_t data);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefinitely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：阻塞等待 RX FIFO 非空后读取 8 位数据。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的 8 位数据。
     *
     *  @sa         DL_SPI_transmitData8
     *  @sa         DL_SPI_transmitDataCheck8
     * @details    功能：参见上方英文说明。
     */
    uint8_t DL_SPI_receiveDataBlocking8(const SPI_Regs *spi);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefinitely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：阻塞等待 RX FIFO 非空后读取 16 位数据。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的 16 位数据。
     *
     *  @sa         DL_SPI_transmitData16
     *  @sa         DL_SPI_transmitDataCheck16
     * @details    功能：参见上方英文说明。
     */
    uint16_t DL_SPI_receiveDataBlocking16(const SPI_Regs *spi);

    /**
     *  @brief      Blocks to ensure receive is ready before reading data
     *
     *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
     *  empty. Will wait indefinitely until there is data in the RX FIFO. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: Requires that packing has been enabled by calling
     *  @ref DL_SPI_enablePacking prior to calling this API. When packing is
     *  enabled, two entries of the RX FIFO are returned as a 32-bit value.
     *  中文说明：阻塞等待 RX FIFO 非空后读取 32 位数据。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *
     *  @return     The data in the RX FIFO
     *              返回值：读取到的 32 位数据。
     *
     *  @pre         DL_SPI_enablePacking
     *
     *  @sa         DL_SPI_transmitData32
     *  @sa         DL_SPI_transmitDataCheck32
     * @details    功能：参见上方英文说明。
     */
    uint32_t DL_SPI_receiveDataBlocking32(const SPI_Regs *spi);

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
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 8 位数据。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 发送成功，`false` 表示 FIFO 满。
     *
     *  @sa         DL_SPI_transmitData8
     *  @sa         DL_SPI_transmitDataBlocking8
     * @details    功能：参见上方英文说明。
     */
    bool DL_SPI_transmitDataCheck8(SPI_Regs *spi, uint8_t data);

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
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 16 位数据。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 发送成功，`false` 表示 FIFO 满。
     *
     *  @sa         DL_SPI_transmitData16
     *  @sa         DL_SPI_transmitDataBlocking16
     * @details    功能：参见上方英文说明。
     */
    bool DL_SPI_transmitDataCheck16(SPI_Regs *spi, uint16_t data);

    /**
     *  @brief      Checks the TX FIFO before trying to transmit data
     *
     *  Checks if the TX FIFO is already full before trying to add new data to the
     *  FIFO. Exits immediately if full rather than trying to block. See related
     *  APIs for additional transmit options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: If packing is enabled by calling @ref DL_SPI_enablePacking prior to
     *  calling this API, then a 32-bit write will be written as one FIFO entry. If
     *  packing is disabled, then a 32-bit write will be written as two FIFO
     *  entries.
     *  中文说明：非阻塞发送 32 位数据，FIFO 满时立即返回。
     *
     *  @param[in]  spi   pointer to the register overlay for the peripheral
     *                    参数：SPI 外设寄存器基址。
     *  @param[in]  data  data to send
     *                    参数：待发送 32 位数据。
     *
     *  @return     If the transmit occurred
     *
     *  @retval     true  if data was added to the TX FIFO
     *  @retval     false if the TX FIFO was full and data was not added
     *              返回值：`true` 发送成功，`false` 表示 FIFO 满。
     *
     *  @sa         DL_SPI_enablePacking
     *  @sa         DL_SPI_transmitData32
     *  @sa         DL_SPI_transmitDataBlocking32
     * @details    功能：参见上方英文说明。
     */
    bool DL_SPI_transmitDataCheck32(SPI_Regs *spi, uint32_t data);

    /**
     *  @brief      Checks the RX FIFO before trying to transmit data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 8 bits.
     *  中文说明：非阻塞读取 8 位数据，FIFO 空时立即返回。
     *
     *  @param[in]  spi    pointer to the register overlay for the peripheral
     *                     参数：SPI 外设寄存器基址。
     *  @param[in]  buffer a buffer to write the received data into
     *                     参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @sa         DL_SPI_receiveData8
     *  @sa         DL_SPI_receiveDataBlocking8
     * @details    功能：参见上方英文说明。
     */
    bool DL_SPI_receiveDataCheck8(const SPI_Regs *spi, uint8_t *buffer);

    /**
     *  @brief      Checks the RX FIFO before trying to transmit data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 16 bits.
     *  中文说明：非阻塞读取 16 位数据，FIFO 空时立即返回。
     *
     *  @param[in]  spi    pointer to the register overlay for the peripheral
     *                     参数：SPI 外设寄存器基址。
     *  @param[in]  buffer a buffer to write the received data into
     *                     参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @sa         DL_SPI_receiveData16
     *  @sa         DL_SPI_receiveDataBlocking16
     * @details    功能：参见上方英文说明。
     */
    bool DL_SPI_receiveDataCheck16(const SPI_Regs *spi, uint16_t *buffer);

    /**
     *  @brief      Checks the RX FIFO before trying to transmit data
     *
     *  Checks if the RX FIFO is already empty before trying to read new data from
     *  the FIFO. Exits immediately if empty rather than trying to block. See
     *  related APIs for additional receive options.
     *
     *  Can be used for any data transfers that are less than or equal to 32 bits.
     *
     *  NOTE: Requires that packing has been enabled by calling
     *  @ref DL_SPI_enablePacking prior to calling this API. When packing is
     *  enabled, two entries of the RX FIFO are returned as a 32-bit value.
     *  中文说明：非阻塞读取 32 位数据，FIFO 空时立即返回。
     *
     *  @param[in]  spi    pointer to the register overlay for the peripheral
     *                     参数：SPI 外设寄存器基址。
     *  @param[in]  buffer a buffer to write the received data into
     *                     参数：接收输出缓冲区。
     *
     *  @return     If the receive occurred
     *
     *  @retval     true  if data was read from the RX FIFO
     *  @retval     false if the RX FIFO was empty and data was not read
     *              返回值：`true` 读取成功，`false` 表示 FIFO 空。
     *
     *  @pre         DL_SPI_enablePacking
     *
     *  @sa         DL_SPI_receiveData32
     *  @sa         DL_SPI_receiveDataBlocking32
     * @details    功能：参见上方英文说明。
     */
    bool DL_SPI_receiveDataCheck32(const SPI_Regs *spi, uint32_t *buffer);

    /**
     *  @brief       Read all available data out of the RX FIFO using 8 bit access
     *
     *  @param[in]   spi       Pointer to the register overlay for the peripheral
     *  @param[out]  buffer    Buffer to write received data into
     *  @param[in]   maxCount  Max number of bytes to read from the RX FIFO
     *
     *  @return      Number of bytes read from the RX FIFO
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    uint32_t DL_SPI_drainRXFIFO8(
        const SPI_Regs *spi, uint8_t *buffer, uint32_t maxCount);

    /**
     *  @brief       Read all available data out of the RX FIFO using 16 bit access
     *
     *  @param[in]   spi       Pointer to the register overlay for the peripheral
     *  @param[out]  buffer    Buffer to write received data into
     *  @param[in]   maxCount  Max number of halfwords to read from the RX FIFO
     *
     *  @return      Number of halfwords read from the RX FIFO
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    uint32_t DL_SPI_drainRXFIFO16(
        const SPI_Regs *spi, uint16_t *buffer, uint32_t maxCount);

    /**
     *  @brief       Read all available data out of the RX FIFO using 32 bit access
     *
     *  NOTE: Requires that packing has been enabled by calling
     *  @ref DL_SPI_enablePacking prior to calling this API. When packing is
     *  enabled, two entries of the RX FIFO are returned as a 32-bit value.
     *
     *  @param[in]   spi       Pointer to the register overlay for the peripheral
     *  @param[out]  buffer    Buffer to write received data into
     *  @param[in]   maxCount  Max number of words to read from the RX FIFO
     *
     *  @return      Number of words read from the RX FIFO
     *
     *  @pre         DL_SPI_enablePacking
     *
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    uint32_t DL_SPI_drainRXFIFO32(
        const SPI_Regs *spi, uint32_t *buffer, uint32_t maxCount);

    /**
     *  @brief      Fill the TX FIFO using 8 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *
     *  @param[in]  spi     Pointer to the register overlay for the peripheral
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *  @param[in]  count   Max number of bytes to write to the TX FIFO
     *
     *  @return     Number of bytes written to the TX FIFO
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    uint32_t DL_SPI_fillTXFIFO8(
        SPI_Regs *spi, const uint8_t *buffer, uint32_t count);

    /**
     *  @brief      Fill the TX FIFO using 16 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *
     *  @param[in]  spi     Pointer to the register overlay for the peripheral
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *  @param[in]  count   Max number of halfwords to write to the TX FIFO
     *
     *  @return     Number of halfwords written to the TX FIFO
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    uint32_t DL_SPI_fillTXFIFO16(
        SPI_Regs *spi, const uint16_t *buffer, uint32_t count);

    /**
     *  @brief      Fill the TX FIFO using 32 bit access
     *
     *  Continuously write data into the TX FIFO until it is filled up or count has
     *  been reached.
     *
     *  NOTE: If packing is enabled by calling @ref DL_SPI_enablePacking prior to
     *  calling this API, then a 32-bit write will be written as one FIFO entry. If
     *  packing is disabled, then a 32-bit write will be written as two FIFO
     *  entries.
     *
     *  @param[in]  spi     Pointer to the register overlay for the peripheral
     *  @param[in]  buffer  Buffer of data to write to the TX FIFO
     *  @param[in]  count   Max number of words to write to the TX FIFO
     *
     *  @return     Number of words written to the TX FIFO
     *
     *  @sa         DL_SPI_enablePacking
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    uint32_t DL_SPI_fillTXFIFO32(
        SPI_Regs *spi, const uint32_t *buffer, uint32_t count);

    /**
     *  @brief      Enable SPI interrupt for triggering the DMA receive event
     *  @brief      中文：使能 SPI 接收 DMA 触发事件。
     *
     * Enables the SPI interrupt to be used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_RX
     * register, which is the event publisher used for triggering the DMA to do
     * a receive data transfer.
     *
     * @note Only one interrupt source should be enabled at a time.
     *
     *  @param[in]  spi       Pointer to the register overlay for the
     *                         peripheral
     *  @param[in]  spi       中文：SPI 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
     *                         the DMA. One of @ref DL_SPI_DMA_INTERRUPT_RX.
     *  @param[in]  interrupt  中文：DMA 触发中断源。
     */
    __STATIC_INLINE void DL_SPI_enableDMAReceiveEvent(
        SPI_Regs *spi, uint32_t interrupt)
    {
        spi->DMA_TRIG_RX.IMASK = interrupt;
    }

    /**
     *  @brief      Enable SPI interrupt for triggering the DMA transmit event
     *  @brief      中文：使能 SPI 发送 DMA 触发事件。
     *
     * Enables the SPI interrupt to be used as the condition to generate an
     * event to directly trigger the DMA. This API configures the DMA_TRIG_TX
     * register, which is the event publisher used for triggering the DMA to do
     * a transmit data transfer.
     *
     * @note DMA_TRIG_TX only has one transmit interrupt source
     *
     *  @param[in]  spi       Pointer to the register overlay for the
     *                         peripheral
     *  @param[in]  spi       中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_enableDMATransmitEvent(SPI_Regs *spi)
    {
        spi->DMA_TRIG_TX.IMASK = SPI_DMA_TRIG_TX_IMASK_TX_SET;
    }

    /**
     *  @brief      Disables SPI interrupt from triggering the DMA receive event
     *  @brief      中文：禁用 SPI 接收 DMA 触发事件。
     *
     * Disables the SPI interrupt as the condition to generate an event to
     * directly trigger the DMA. This API configures the DMA_TRIG_RX
     * register, which is the event publisher used for triggering the DMA to do
     * a receive data transfer.
     *
     *  @param[in]  spi       Pointer to the register overlay for the
     *                         peripheral
     *  @param[in]  spi       中文：SPI 外设寄存器基址。
     *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
     *                         the DMA. One of @ref DL_SPI_DMA_INTERRUPT_RX.
     *  @param[in]  interrupt  中文：待禁用的 DMA 触发中断源。
     */
    __STATIC_INLINE void DL_SPI_disableDMAReceiveEvent(
        SPI_Regs *spi, uint32_t interrupt)
    {
        spi->DMA_TRIG_RX.IMASK &= ~(interrupt);
    }

    /**
     *  @brief      Disables SPI interrupt from triggering the DMA transmit event
     *  @brief      中文：禁用 SPI 发送 DMA 触发事件。
     *
     * Disables the SPI interrupt as the condition to generate an event to
     * directly trigger the DMA. This API configures the DMA_TRIG_TX
     * register, which is the event publisher used for triggering the DMA to do
     * a transmit data transfer.
     *
     * @note DMA_TRIG_TX only has one transmit interrupt source
     *
     * @param[in]  spi       Pointer to the register overlay for the
     *                       peripheral
     * @param[in]  spi       中文：SPI 外设寄存器基址。
     */
    __STATIC_INLINE void DL_SPI_disableDMATransmitEvent(SPI_Regs *spi)
    {
        spi->DMA_TRIG_TX.IMASK = SPI_DMA_TRIG_TX_IMASK_TX_CLR;
    }

    /**
     *  @brief      Check which SPI interrupt for DMA receive events is enabled
     *  @brief      中文：查询哪些接收 DMA 触发中断已使能。
     *
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_SPI_DMA_INTERRUPT_RX.
     *  @param[in]  interruptMask  中文：待查询中断掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     Which of the requested SPI interrupts is enabled
     *
     *  @retval     One of @ref DL_SPI_DMA_INTERRUPT_RX
     *  @return     中文：已使能的接收 DMA 触发中断。
     */
    __STATIC_INLINE uint32_t DL_SPI_getEnabledDMAReceiveEvent(
        const SPI_Regs *spi, uint32_t interruptMask)
    {
        return (spi->DMA_TRIG_RX.IMASK & interruptMask);
    }

    /**
     *  @brief      Check if SPI interrupt for DMA transmit event is enabled
     *  @brief      中文：查询发送 DMA 触发中断是否已使能。
     *
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  spi           Pointer to the register overlay for the
     *                            peripheral
     *  @param[in]  spi           中文：SPI 外设寄存器基址。
     *
     *  @return     The requested SPI interrupt status
     *
     *  @retval     DL_SPI_DMA_INTERRUPT_TX if enabled, 0 if not enabled
     *  @return     中文：已使能返回 DL_SPI_DMA_INTERRUPT_TX，否则返回 0。
     */
    __STATIC_INLINE uint32_t DL_SPI_getEnabledDMATransmitEvent(const SPI_Regs *spi)
    {
        return (spi->DMA_TRIG_TX.IMASK & SPI_DMA_TRIG_TX_IMASK_TX_MASK);
    }

    /**
     *  @brief      Check interrupt flag of enabled SPI interrupt for DMA receive event
     *  @brief      中文：查询已使能接收 DMA 触发中断的挂起状态。
     *
     * Checks if any of the SPI interrupts for the DMA receive event that were
     * previously enabled are pending.
     * This API checks the DMA_TRIG_RX register, which is the event publisher used
     * for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_SPI_DMA_INTERRUPT_RX.
     *  @param[in]  interruptMask  中文：待查询中断掩码。
     *
     *  @note Only one interrupt source should be enabled at a time.
     *
     *  @return     The requested SPI interrupt status
     *
     *  @retval     One of @ref DL_SPI_DMA_INTERRUPT_RX
     *  @return     中文：挂起的接收 DMA 触发中断。
     *
     *  @sa         DL_SPI_enableDMAReceiveEvent
     */
    __STATIC_INLINE uint32_t DL_SPI_getEnabledDMAReceiveEventStatus(
        const SPI_Regs *spi, uint32_t interruptMask)
    {
        return (spi->DMA_TRIG_RX.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled SPI interrupt for DMA transmit event
     *  @brief      中文：查询已使能发送 DMA 触发中断的挂起状态。
     *
     * Checks if the SPI interrupt for the DMA transmit event that was
     * previously enabled is pending.
     * This API checks the DMA_TRIG_TX register, which is the event publisher used
     * for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  spi           Pointer to the register overlay for the
     *                            peripheral
     *  @param[in]  spi           中文：SPI 外设寄存器基址。
     *
     *  @return     The requested SPI interrupt status
     *
     *  @retval     DL_SPI_DMA_INTERRUPT_TX if enabled, 0 if not enabled
     *  @return     中文：挂起返回 DL_SPI_DMA_INTERRUPT_TX，否则返回 0。
     *
     *  @sa         DL_SPI_enableDMATransmitEvent
     */
    __STATIC_INLINE uint32_t DL_SPI_getEnabledDMATransmitEventStatus(
        const SPI_Regs *spi)
    {
        return (spi->DMA_TRIG_TX.MIS & SPI_DMA_TRIG_TX_MIS_TX_MASK);
    }

    /**
     *  @brief      Check interrupt flag of any SPI interrupt for DMA receive event
     *  @brief      中文：查询接收 DMA 触发中断原始挂起状态。
     *
     *  Checks if any of the SPI interrupts for DMA receive event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_SPI_DMA_INTERRUPT_RX.
     *  @param[in]  interruptMask  中文：待查询中断掩码。
     *
     *  @return     Which of the requested SPI interrupts are pending
     *
     *  @retval     Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_RX values
     *  @return     中文：原始挂起中断位掩码。
     */
    __STATIC_INLINE uint32_t DL_SPI_getRawDMAReceiveEventStatus(
        const SPI_Regs *spi, uint32_t interruptMask)
    {
        return (spi->DMA_TRIG_RX.RIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any SPI interrupt for DMA transmit event
     *  @brief      中文：查询发送 DMA 触发中断原始挂起状态。
     *
     *  Checks if any of the SPI interrupts for DMA transmit event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  spi           Pointer to the register overlay for the
     *                            peripheral
     *  @param[in]  spi           中文：SPI 外设寄存器基址。
     *
     *  @return     The requested SPI interrupt status
     *
     *  @retval     DL_SPI_DMA_INTERRUPT_TX if enabled, 0 if not enabled
     *  @return     中文：挂起返回 DL_SPI_DMA_INTERRUPT_TX，否则返回 0。
     */
    __STATIC_INLINE uint32_t DL_SPI_getRawDMATransmitEventStatus(
        const SPI_Regs *spi)
    {
        return (spi->DMA_TRIG_TX.RIS & SPI_DMA_TRIG_TX_RIS_TX_MASK);
    }

    /**
     *  @brief      Get highest priority pending SPI interrupt for DMA receive event
     *  @brief      中文：获取接收 DMA 最高优先级挂起中断索引。
     *
     *  Checks if any of the SPI interrupts for DMA receive event are pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *
     *  @param[in]  spi           Pointer to the register overlay for the
     *                            peripheral
     *  @param[in]  spi           中文：SPI 外设寄存器基址。
     *
     *  @return     The highest priority pending SPI interrupt
     *
     *  @retval     One of @ref DL_SPI_DMA_IIDX_RX
     *  @return     中文：最高优先级挂起中断索引。
     */
    __STATIC_INLINE DL_SPI_DMA_IIDX_RX DL_SPI_getPendingDMAReceiveEvent(
        const SPI_Regs *spi)
    {
        return (DL_SPI_DMA_IIDX_RX)(spi->DMA_TRIG_RX.IIDX);
    }

    /**
     *  @brief      Get highest priority pending SPI interrupt for DMA transmit event
     *  @brief      中文：获取发送 DMA 最高优先级挂起中断索引。
     *
     *  Checks if the SPI interrupt for DMA transmit event is pending.
     *  Interrupts do not have to be previously enabled.
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *
     *  @param[in]  spi           Pointer to the register overlay for the
     *                            peripheral
     *  @param[in]  spi           中文：SPI 外设寄存器基址。
     *
     *  @return     The highest priority pending SPI interrupt
     *
     *  @retval     DL_SPI_DMA_IIDX_TX if pending, 0 if not pending
     *  @return     中文：挂起返回 DL_SPI_DMA_IIDX_TX，否则返回 0。
     */
    __STATIC_INLINE DL_SPI_DMA_IIDX_TX DL_SPI_getPendingDMATransmitEvent(
        const SPI_Regs *spi)
    {
        return (DL_SPI_DMA_IIDX_TX)(spi->DMA_TRIG_TX.IIDX);
    }

    /**
     *  @brief      Clear pending SPI interrupts for DMA receive event
     *  @brief      中文：清除接收 DMA 触发中断挂起标志。
     *
     *  This API checks the DMA_TRIG_RX register, which is the event publisher used
     *  for triggering the DMA to do a receive data transfer.
     *
     *  @param[in]  spi            Pointer to the register overlay for the
     *                             peripheral
     *  @param[in]  spi            中文：SPI 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_SPI_DMA_INTERRUPT_RX.
     *  @param[in]  interruptMask  中文：待清除中断掩码。
     */
    __STATIC_INLINE void DL_SPI_clearDMAReceiveEventStatus(
        SPI_Regs *spi, uint32_t interruptMask)
    {
        spi->DMA_TRIG_RX.ICLR = interruptMask;
    }

    /**
     *  @brief      Clear pending SPI interrupt for DMA transmit event
     *  @brief      中文：清除发送 DMA 触发中断挂起标志。
     *
     *  This API checks the DMA_TRIG_TX register, which is the event publisher used
     *  for triggering the DMA to do a transmit data transfer.
     *
     *  @param[in]  spi           Pointer to the register overlay for the
     *                            peripheral
     *  @param[in]  spi           中文：SPI 外设寄存器基址。
     * @note DMA_TRIG_TX only has one transmit interrupt source
     */
    __STATIC_INLINE void DL_SPI_clearDMATransmitEventStatus(SPI_Regs *spi)
    {
        spi->DMA_TRIG_TX.ICLR = SPI_DMA_TRIG_TX_ICLR_TX_CLR;
    }

    /**
     *  @brief      Save SPI configuration before entering a power loss state.
     *
     *  Some MSPM0G peripherals residing in PD1 domain do not retain register
     *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
     *  for the full list of peripheral instances that exhibit this behavior.
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *
     *  @param[in]  ptr  Configuration backup setup structure. See
     *                  @ref DL_SPI_backupConfig.
     *
     *  @retval     FALSE if a configuration already exists in ptr (will not be
     *              overwritten). TRUE if a configuration was successfully saved
     *
     *  @sa         DL_SPI_restoreConfiguration
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    bool DL_SPI_saveConfiguration(const SPI_Regs *spi, DL_SPI_backupConfig *ptr);

    /**
     *  @brief      Restore SPI configuration after leaving a power loss state.
     *
     *  Some MSPM0G peripherals residing in PD1 domain do not retain register
     *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
     *  for the full list of peripheral instances that exhibit this behavior.
     *
     *  @param[in]  spi  Pointer to the register overlay for the peripheral
     *
     *  @param[in]  ptr   Configuration backup setup structure. See
     *                    @ref DL_SPI_backupConfig.
     *
     *  @retval     FALSE if a configuration does not exist in ptr (will not be
     *              loaded). TRUE if a configuration successfully loaded
     *
     *  @sa         DL_SPI_saveConfiguration
     * @details    功能：参见上方英文说明。
     *             参数：参见 @param 英文说明。
     *             返回值：参见 @return 英文说明。
     */
    bool DL_SPI_restoreConfiguration(SPI_Regs *spi, DL_SPI_backupConfig *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_SPI__ */

#endif /* ti_dl_dl_spi__include */
/** @}*/
