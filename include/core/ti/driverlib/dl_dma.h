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
 *  @file       dl_dma.h
 *  @brief      Direct Memory Access (DMA) Driver Library
 *  @defgroup   DMA Direct Memory Access (DMA)
 *
 *  @anchor ti_dl_dl_m0p_dma_Overview
 *  # Overview
 *
 *  The Direct Memory Access (DMA) Library allows full configuration of
 *  the MSPM0 DMA module.
 *  The DMA controller transfers data from one address to another, without CPU
 *  intervention, across the entire address range.
 *  DMA controllers have multiple channels that can be configured independently
 *
 ******************************************************************************
 */
/**
 * @brief MSPM0 DMA 直接内存访问驱动库头文件。
 *
 * 提供 DMA 通道配置、传输模式设置、触发源选择、中断/事件管理等全部 API。
 * 支持单次传输、块传输、重复模式（FULL 通道）、GATHER/FILL/TABLE 扩展模式。
 */
/** @addtogroup DMA
 * @{
 */
#ifndef ti_dl_dl_m0p_dma__include
#define ti_dl_dl_m0p_dma__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if (DMA_SYS_N_DMA_FULL_CHANNEL > 0)
/*!
 * @brief Device has support for DMA FULL channels. 设备支持 DMA FULL 通道。
 */
#define DEVICE_HAS_DMA_FULL_CHANNEL
#endif

#ifdef DMA_SYS_MMR_LLONG
/*!
 * @brief Device has support for DMA 128-bit access on all channels.
 * 设备支持所有通道的 128 位 DMA 访问。
 */
#define DEVICE_HAS_LLONG_ACCESS
/*!
 * @brief If device has LLONG support, it will also support AUTO reg and GATHER mode.
 * 若设备支持 LLONG，则同时支持自动寄存器和 GATHER 模式。
 */
#define DEVICE_HAS_AUTO_AND_GATHER
#endif

    /* clang-format off */

/** @addtogroup DL_DMA_INTERRUPT
 *  @{
 */
/*!
 * @brief DMA channel 0 interrupt. DMA 通道 0 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL0                        (DMA_CPU_INT_IMASK_DMACH0_SET)

/*!
 * @brief DMA channel 1 interrupt. DMA 通道 1 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL1                        (DMA_CPU_INT_IMASK_DMACH1_SET)

/*!
 * @brief DMA channel 2 interrupt. DMA 通道 2 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL2                        (DMA_CPU_INT_IMASK_DMACH2_SET)

/*!
 * @brief DMA channel 3 interrupt. DMA 通道 3 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL3                        (DMA_CPU_INT_IMASK_DMACH3_SET)

/*!
 * @brief DMA channel 4 interrupt. DMA 通道 4 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL4                        (DMA_CPU_INT_IMASK_DMACH4_SET)

/*!
 * @brief DMA channel 5 interrupt. DMA 通道 5 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL5                        (DMA_CPU_INT_IMASK_DMACH5_SET)

/*!
 * @brief DMA channel 6 interrupt. DMA 通道 6 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL6                        (DMA_CPU_INT_IMASK_DMACH6_SET)

/*!
 * @brief DMA channel 7 interrupt. DMA 通道 7 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL7                        (DMA_CPU_INT_IMASK_DMACH7_SET)

/*!
 * @brief DMA channel 8 interrupt. DMA 通道 8 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL8                        (DMA_CPU_INT_IMASK_DMACH8_SET)

/*!
 * @brief DMA channel 9 interrupt. DMA 通道 9 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL9                        (DMA_CPU_INT_IMASK_DMACH9_SET)

/*!
 * @brief DMA channel 10 interrupt. DMA 通道 10 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL10                      (DMA_CPU_INT_IMASK_DMACH10_SET)

/*!
 * @brief DMA channel 12 interrupt. DMA 通道 12 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL12                      (DMA_CPU_INT_IMASK_DMACH12_SET)

/*!
 * @brief DMA channel 13 interrupt. DMA 通道 13 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL13                      (DMA_CPU_INT_IMASK_DMACH13_SET)

/*!
 * @brief DMA channel 14 interrupt. DMA 通道 14 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL14                      (DMA_CPU_INT_IMASK_DMACH14_SET)

/*!
 * @brief DMA channel 15 interrupt. DMA 通道 15 中断。
 */
#define DL_DMA_INTERRUPT_CHANNEL15                      (DMA_CPU_INT_IMASK_DMACH15_SET)

#ifdef DEVICE_HAS_DMA_FULL_CHANNEL
/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 0 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 0 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL0        (DMA_CPU_INT_IMASK_PREIRQCH0_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 1 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 1 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL1        (DMA_CPU_INT_IMASK_PREIRQCH1_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 2 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 2 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL2        (DMA_CPU_INT_IMASK_PREIRQCH2_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 3 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 3 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL3        (DMA_CPU_INT_IMASK_PREIRQCH3_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 4 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 4 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL4        (DMA_CPU_INT_IMASK_PREIRQCH4_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 5 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 5 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL5        (DMA_CPU_INT_IMASK_PREIRQCH5_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 6 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 6 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL6        (DMA_CPU_INT_IMASK_PREIRQCH6_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 7 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 7 提前中断，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL7        (DMA_CPU_INT_IMASK_PREIRQCH7_SET)
#endif /* DEVICE_HAS_DMA_FULL_CHANNEL */

/*!
 * @brief DMA address error, source address not reachable.
 * DMA 地址错误，源地址不可达。
 */
#define DL_DMA_INTERRUPT_ADDR_ERROR                      (DMA_CPU_INT_IMASK_ADDRERR_SET)
/*!
 * @brief DMA data error, source data might be corrupted (PAR or ECC error).
 * DMA 数据错误，源数据可能已损坏（PAR 或 ECC 错误）。
 */
#define DL_DMA_INTERRUPT_DATA_ERROR                      (DMA_CPU_INT_IMASK_DATAERR_SET)

/** @}*/

/** @addtogroup DL_DMA_EVENT
 *  @{
 */
/*!
 * @brief DMA channel 0 event. DMA 通道 0 事件。
 */
#define DL_DMA_EVENT_CHANNEL0                             (DMA_GEN_EVENT_IMASK_DMACH0_SET)

/*!
 * @brief DMA channel 1 event. DMA 通道 1 事件。
 */
#define DL_DMA_EVENT_CHANNEL1                             (DMA_GEN_EVENT_IMASK_DMACH1_SET)

/*!
 * @brief DMA channel 2 event. DMA 通道 2 事件。
 */
#define DL_DMA_EVENT_CHANNEL2                             (DMA_GEN_EVENT_IMASK_DMACH2_SET)

/*!
 * @brief DMA channel 3 event. DMA 通道 3 事件。
 */
#define DL_DMA_EVENT_CHANNEL3                             (DMA_GEN_EVENT_IMASK_DMACH3_SET)

/*!
 * @brief DMA channel 4 event. DMA 通道 4 事件。
 */
#define DL_DMA_EVENT_CHANNEL4                             (DMA_GEN_EVENT_IMASK_DMACH4_SET)

/*!
 * @brief DMA channel 5 event. DMA 通道 5 事件。
 */
#define DL_DMA_EVENT_CHANNEL5                             (DMA_GEN_EVENT_IMASK_DMACH5_SET)

/*!
 * @brief DMA channel 6 event. DMA 通道 6 事件。
 */
#define DL_DMA_EVENT_CHANNEL6                             (DMA_GEN_EVENT_IMASK_DMACH6_SET)

/*!
 * @brief DMA channel 7 event. DMA 通道 7 事件。
 */
#define DL_DMA_EVENT_CHANNEL7                             (DMA_GEN_EVENT_IMASK_DMACH7_SET)

/*!
 * @brief DMA channel 8 event. DMA 通道 8 事件。
 */
#define DL_DMA_EVENT_CHANNEL8                             (DMA_GEN_EVENT_IMASK_DMACH8_SET)

/*!
 * @brief DMA channel 9 event. DMA 通道 9 事件。
 */
#define DL_DMA_EVENT_CHANNEL9                             (DMA_GEN_EVENT_IMASK_DMACH9_SET)

/*!
 * @brief DMA channel 10 event. DMA 通道 10 事件。
 */
#define DL_DMA_EVENT_CHANNEL10                           (DMA_GEN_EVENT_IMASK_DMACH10_SET)

/*!
 * @brief DMA channel 12 event. DMA 通道 12 事件。
 */
#define DL_DMA_EVENT_CHANNEL12                           (DMA_GEN_EVENT_IMASK_DMACH12_SET)

/*!
 * @brief DMA channel 13 event. DMA 通道 13 事件。
 */
#define DL_DMA_EVENT_CHANNEL13                           (DMA_GEN_EVENT_IMASK_DMACH13_SET)

/*!
 * @brief DMA channel 14 event. DMA 通道 14 事件。
 */
#define DL_DMA_EVENT_CHANNEL14                           (DMA_GEN_EVENT_IMASK_DMACH14_SET)

/*!
 * @brief DMA channel 15 event. DMA 通道 15 事件。
 */
#define DL_DMA_EVENT_CHANNEL15                           (DMA_GEN_EVENT_IMASK_DMACH15_SET)

#ifdef DEVICE_HAS_DMA_FULL_CHANNEL
/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 0 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 0 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL0            (DMA_GEN_EVENT_IMASK_PREIRQCH0_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 1 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 1 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL1            (DMA_GEN_EVENT_IMASK_PREIRQCH1_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 2 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 2 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL2            (DMA_GEN_EVENT_IMASK_PREIRQCH2_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 3 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 3 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL3            (DMA_GEN_EVENT_IMASK_PREIRQCH3_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 4 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 4 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL4            (DMA_GEN_EVENT_IMASK_PREIRQCH4_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 5 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 5 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL5            (DMA_GEN_EVENT_IMASK_PREIRQCH5_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 6 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 6 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL6            (DMA_GEN_EVENT_IMASK_PREIRQCH6_SET)

/*!
 * @brief Available for FULL-channel configuration only. Early IRQ for DMA
 * channel 7 interrupt. Size counter has reached early IRQ threshold.
 * 仅 FULL 通道可用。DMA 通道 7 提前事件，计数器到达提前中断阈值。
 */
#define DL_DMA_FULL_CH_EVENT_EARLY_CHANNEL7            (DMA_GEN_EVENT_IMASK_PREIRQCH7_SET)
#endif /* DEVICE_HAS_DMA_FULL_CHANNEL */

/*!
 * @brief DMA address error, source address not reachable.
 * DMA 地址错误，源地址不可达。
 */
#define DL_DMA_EVENT_ADDR_ERROR                     (DMA_GEN_EVENT_IMASK_ADDRERR_SET)
/*!
 * @brief DMA data error, source data might be corrupted (PAR or ECC error).
 * DMA 数据错误，源数据可能已损坏（PAR 或 ECC 错误）。
 */
#define DL_DMA_EVENT_DATA_ERROR                     (DMA_GEN_EVENT_IMASK_DATAERR_SET)

/** @}*/

/*! @enum DL_DMA_TRANSFER_MODE DMA 传输模式。 */
typedef enum {
    /*! Each DMA trigger results in a single data transfer, once. 单次单元素传输。 */
    DL_DMA_SINGLE_TRANSFER_MODE = DMA_DMACTL_DMATM_SINGLE,
    /*! Each DMA trigger results in a transfer of a block of data, once. 单次块传输。 */
    DL_DMA_SINGLE_BLOCK_TRANSFER_MODE = DMA_DMACTL_DMATM_BLOCK,
#ifdef DEVICE_HAS_DMA_FULL_CHANNEL
    /*! Available for FULL-channel configuration only. Each DMA trigger results
     *  in a single data transfer, repeating.
     *  仅 FULL 通道可用。重复单元素传输。 */
    DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE = DMA_DMACTL_DMATM_RPTSNGL,
    /*! Available for FULL-channel configuration only. Each DMA trigger results
     *  in a transfer of a block of data, repeating.
     *  仅 FULL 通道可用。重复块传输。 */
    DL_DMA_FULL_CH_REPEAT_BLOCK_TRANSFER_MODE = DMA_DMACTL_DMATM_RPTBLCK,
#endif /* DEVICE_HAS_DMA_FULL_CHANNEL */
} DL_DMA_TRANSFER_MODE;

/*! @enum DL_DMA_EXTENDED_MODE DMA 扩展模式。 */
typedef enum {
    /*! Normal operation. 正常模式。 */
    DL_DMA_NORMAL_MODE = DMA_DMACTL_DMAEM_NORMAL,
#ifdef DEVICE_HAS_DMA_FULL_CHANNEL
#ifdef DEVICE_HAS_AUTO_AND_GATHER
    /*! Available for FULL-channel configuration only. Reads data from
     *  address table located at source address and transfers data to
     *  destination address.
     *  仅 FULL 通道可用。GATHER 模式，从源地址的地址表读取数据并传输到目标地址。 */
    DL_DMA_FULL_CH_GATHER_MODE = DMA_DMACTL_DMAEM_GATHERMODE,
#endif /* DEVICE_HAS_AUTO_AND_GATHER */
    /*! Available for FULL-channel configuration only. Fills the destination
     *  with a specific value.
     *  仅 FULL 通道可用。FILL 模式，用指定值填充目标地址。 */
    DL_DMA_FULL_CH_FILL_MODE = DMA_DMACTL_DMAEM_FILLMODE,
    /*! Available for FULL-channel configuration only. The source data contains
     *  addresses and data.
     *  仅 FULL 通道可用。TABLE 模式，源数据包含地址和数据。 */
    DL_DMA_FULL_CH_TABLE_MODE = DMA_DMACTL_DMAEM_TABLEMODE,
#endif /* DEVICE_HAS_DMA_FULL_CHANNEL */
} DL_DMA_EXTENDED_MODE;

/*! @enum DL_DMA_INCREMENT DMA 地址递增/递减模式。 */
typedef enum {
    /*! Do not change address after each transfer. 每次传输后地址不变。 */
    DL_DMA_ADDR_UNCHANGED = DMA_DMACTL_DMASRCINCR_UNCHANGED,
    /*! Decrement address by 1 * DL_DMA_WIDTH after each transfer. 每次传输后地址递减 1 个宽度。 */
    DL_DMA_ADDR_DECREMENT = DMA_DMACTL_DMASRCINCR_DECREMENT,
    /*! Increment address by 1 * DL_DMA_WIDTH after each transfer. 每次传输后地址递增 1 个宽度。 */
    DL_DMA_ADDR_INCREMENT = DMA_DMACTL_DMASRCINCR_INCREMENT,
    /*! Stride mode 2, increment address by 2 * DL_DMA_WIDTH (skip over every
     * other element). 步进模式 2，每次递增 2 个宽度（隔一个元素）。 */
    DL_DMA_ADDR_STRIDE_2 = DMA_DMACTL_DMASRCINCR_STRIDE_2,
    /*! Stride mode 3, increment address by 3 * DL_DMA_WIDTH (skip over two
     * elements. 步进模式 3，每次递增 3 个宽度（跳过两个元素）。 */
    DL_DMA_ADDR_STRIDE_3 = DMA_DMACTL_DMASRCINCR_STRIDE_3,
    /*! Stride mode 4, increment address by 4 * DL_DMA_WIDTH (skip over three
     * elements. 步进模式 4，每次递增 4 个宽度（跳过三个元素）。 */
    DL_DMA_ADDR_STRIDE_4 = DMA_DMACTL_DMASRCINCR_STRIDE_4,
    /*! Stride mode 5, increment address by 5 * DL_DMA_WIDTH (skip over four
     * elements. 步进模式 5，每次递增 5 个宽度（跳过四个元素）。 */
    DL_DMA_ADDR_STRIDE_5 = DMA_DMACTL_DMASRCINCR_STRIDE_5,
    /*! Stride mode 6, increment address by 6 * DL_DMA_WIDTH (skip over five
     * elements. 步进模式 6，每次递增 6 个宽度（跳过五个元素）。 */
    DL_DMA_ADDR_STRIDE_6 = DMA_DMACTL_DMASRCINCR_STRIDE_6,
    /*! Stride mode 7, increment address by 7 * DL_DMA_WIDTH (skip over six
     * elements. 步进模式 7，每次递增 7 个宽度（跳过六个元素）。 */
    DL_DMA_ADDR_STRIDE_7 = DMA_DMACTL_DMASRCINCR_STRIDE_7,
    /*! Stride mode 8, increment address by 8 * DL_DMA_WIDTH (skip over seven
     * elements. 步进模式 8，每次递增 8 个宽度（跳过七个元素）。 */
    DL_DMA_ADDR_STRIDE_8 = DMA_DMACTL_DMASRCINCR_STRIDE_8,
    /*! Stride mode 9, increment address by 9 * DL_DMA_WIDTH (skip over eight
     * elements. 步进模式 9，每次递增 9 个宽度（跳过八个元素）。 */
    DL_DMA_ADDR_STRIDE_9 = DMA_DMACTL_DMASRCINCR_STRIDE_9,
} DL_DMA_INCREMENT;

/*! @enum DL_DMA_EARLY_INTERRUPT_THRESHOLD DMA 提前中断阈值。 */
typedef enum {
    /*! Disable early interrupt events. 禁用提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_DISABLED = DMA_DMACTL_DMAPREIRQ_PREIRQ_DISABLE,
    /*! Generate Early-IRQ event with one transfer pending (DMASZ=1). 剩余 1 次传输时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_1 = DMA_DMACTL_DMAPREIRQ_PREIRQ_1,
    /*! Generate Early-IRQ event with two transfers pending (DMASZ=2). 剩余 2 次传输时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_2 = DMA_DMACTL_DMAPREIRQ_PREIRQ_2,
    /*! Generate Early-IRQ event with three transfers pending (DMASZ=4). 剩余 4 次传输时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_4 = DMA_DMACTL_DMAPREIRQ_PREIRQ_4,
    /*! Generate Early-IRQ event with eight transfers pending (DMASZ=8). 剩余 8 次传输时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_8 = DMA_DMACTL_DMAPREIRQ_PREIRQ_8,
    /*! Generate Early-IRQ event with 32 transfers pending (DMASZ=32). 剩余 32 次传输时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_32 = DMA_DMACTL_DMAPREIRQ_PREIRQ_32,
    /*! Generate Early-IRQ event with 64 transfers pending (DMASZ=64). 剩余 64 次传输时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_64 = DMA_DMACTL_DMAPREIRQ_PREIRQ_64,
    /*! Generate Early-IRQ event when DMASZ reaches the half size point of the
     * original transfer size. 当 DMASZ 到达原始传输大小的一半时触发提前中断。 */
    DL_DMA_EARLY_INTERRUPT_THRESHOLD_HALF = DMA_DMACTL_DMAPREIRQ_PREIRQ_HALF,
} DL_DMA_EARLY_INTERRUPT_THRESHOLD;

/*! @enum DL_DMA_BURST_SIZE DMA 突发传输大小。 */
typedef enum {
    /*! No burst interruption. The block transfer always transfers all elements
     * defined in the DMASZ register before priority is newly evaluated.
     * 无突发中断，块传输完成后才重新评估优先级。 */
    DL_DMA_BURST_SIZE_INFINITY = DMA_DMAPRIO_BURSTSZ_INFINITI,
    /*! Burst size of a block transfer is 8. 突发大小为 8。 */
    DL_DMA_BURST_SIZE_8 = DMA_DMAPRIO_BURSTSZ_BURST_8,
    /*! Burst size of a block transfer is 16. 突发大小为 16。 */
    DL_DMA_BURST_SIZE_16 = DMA_DMAPRIO_BURSTSZ_BUSRT_16,
    /*! Burst size of a block transfer is 32. 突发大小为 32。 */
    DL_DMA_BURST_SIZE_32 = DMA_DMAPRIO_BURSTSZ_BURST_32,
} DL_DMA_BURST_SIZE;

/*! @enum DL_DMA_TRIGGER_TYPE DMA 触发类型。 */
typedef enum {
    /*! Internal DMA channel is selected as the DMA trigger. 选择内部 DMA 通道作为触发源。 */
    DL_DMA_TRIGGER_TYPE_INTERNAL = DMA_DMATCTL_DMATINT_INTERNAL,
    /*! External DMA channel is selected as the DMA trigger. 选择外部 DMA 通道作为触发源。 */
    DL_DMA_TRIGGER_TYPE_EXTERNAL = DMA_DMATCTL_DMATINT_EXTERNAL,
} DL_DMA_TRIGGER_TYPE;

/*! @enum DL_DMA_WIDTH DMA 数据宽度。 */
typedef enum {
    /*! Byte Acccess (8-bit). 字节访问（8 位）。 */
    DL_DMA_WIDTH_BYTE = DMA_DMACTL_DMASRCWDTH_BYTE,
    /*! Half Word Acccess (16-bit). 半字访问（16 位）。 */
    DL_DMA_WIDTH_HALF_WORD = DMA_DMACTL_DMASRCWDTH_HALF,
    /*! Word Acccess (32-bit). 字访问（32 位）。 */
    DL_DMA_WIDTH_WORD = DMA_DMACTL_DMASRCWDTH_WORD,
    /*! Long Acccess (64-bit). 长字访问（64 位）。 */
    DL_DMA_WIDTH_LONG = DMA_DMACTL_DMASRCWDTH_LONG,
    #ifdef DEVICE_HAS_LLONG_ACCESS
    /*! Long-long Acccess (128-bit). 双长字访问（128 位）。 */
    DL_DMA_WIDTH_LONG_LONG = DMA_DMACTL_DMASRCWDTH_LONGLONG,
    #endif /* DEVICE_HAS_LLONG_ACCESS*/
} DL_DMA_WIDTH;

/*! @enum DL_DMA_EVENT_IIDX DMA 事件中断索引。 */
typedef enum {
    /*! Enum to indicate that no DMA event has taken place. 无 DMA 事件。 */
    DL_DMA_EVENT_IIDX_NO_INTR = DMA_GEN_EVENT_IIDX_STAT_NO_INTR,
    /*! Enum to indicate that the channel 0 interrupt has fired. 通道 0 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH0 = DMA_GEN_EVENT_IIDX_STAT_DMACH0,
    /*! Enum to indicate that the channel 1 interrupt has fired. 通道 1 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH1 = DMA_GEN_EVENT_IIDX_STAT_DMACH1,
    /*! Enum to indicate that the channel 2 interrupt has fired. 通道 2 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH2 = DMA_GEN_EVENT_IIDX_STAT_DMACH2,
    /*! Enum to indicate that the channel 3 interrupt has fired. 通道 3 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH3 = DMA_GEN_EVENT_IIDX_STAT_DMACH3,
    /*! Enum to indicate that the channel 4 interrupt has fired. 通道 4 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH4 = DMA_GEN_EVENT_IIDX_STAT_DMACH4,
    /*! Enum to indicate that the channel 5 interrupt has fired. 通道 5 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH5 = DMA_GEN_EVENT_IIDX_STAT_DMACH5,
    /*! Enum to indicate that the channel 6 interrupt has fired. 通道 6 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH6 = DMA_GEN_EVENT_IIDX_STAT_DMACH6,
    /*! Enum to indicate that the channel 7 interrupt has fired. 通道 7 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH7 = DMA_GEN_EVENT_IIDX_STAT_DMACH7,
    /*! Enum to indicate that the channel 8 interrupt has fired. 通道 8 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH8 = DMA_GEN_EVENT_IIDX_STAT_DMACH8,
    /*! Enum to indicate that the channel 9 interrupt has fired. 通道 9 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH9 = DMA_GEN_EVENT_IIDX_STAT_DMACH9,
    /*! Enum to indicate that the channel 10 interrupt has fired. 通道 10 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH10 = DMA_GEN_EVENT_IIDX_STAT_DMACH10,
    /*! Enum to indicate that the channel 11 interrupt has fired. 通道 11 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH11 = DMA_GEN_EVENT_IIDX_STAT_DMACH11,
    /*! Enum to indicate that the channel 12 interrupt has fired. 通道 12 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH12 = DMA_GEN_EVENT_IIDX_STAT_DMACH12,
    /*! Enum to indicate that the channel 13 interrupt has fired. 通道 13 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH13 = DMA_GEN_EVENT_IIDX_STAT_DMACH13,
    /*! Enum to indicate that the channel 14 interrupt has fired. 通道 14 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH14 = DMA_GEN_EVENT_IIDX_STAT_DMACH14,
    /*! Enum to indicate that the channel 15 interrupt has fired. 通道 15 中断已触发。 */
    DL_DMA_EVENT_IIDX_DMACH15 = DMA_GEN_EVENT_IIDX_STAT_DMACH15,
#ifdef DEVICE_HAS_DMA_FULL_CHANNEL
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 0 interrupt has fired.
     *  仅 FULL 通道可用。通道 0 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH0 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH0,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 1 interrupt has fired.
     *  仅 FULL 通道可用。通道 1 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH1 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH1,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 2 interrupt has fired.
     *  仅 FULL 通道可用。通道 2 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH2 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH2,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 3 interrupt has fired.
     *  仅 FULL 通道可用。通道 3 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH3 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH3,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 4 interrupt has fired.
     *  仅 FULL 通道可用。通道 4 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH4 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH4,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 5 interrupt has fired.
     *  仅 FULL 通道可用。通道 5 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH5 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH5,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 6 interrupt has fired.
     *  仅 FULL 通道可用。通道 6 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH6 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH6,
    /*! Available for FULL-channel configuration only. Enum to indicate that the
     *  early interrupt event for channel 7 interrupt has fired.
     *  仅 FULL 通道可用。通道 7 提前中断已触发。 */
    DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH7 = DMA_GEN_EVENT_IIDX_STAT_PREIRQCH7,
#endif /* DEVICE_HAS_DMA_FULL_CHANNEL */
    /*! Enum to indicate that a DMA address error has occurred. DMA 地址错误已发生。 */
    DL_DMA_EVENT_IIDX_ADDR_ERROR = DMA_GEN_EVENT_IIDX_STAT_ADDRERR,

    /*! Enum to indicate that a DMA data error has occurred. DMA 数据错误已发生。 */
    DL_DMA_EVENT_IIDX_DATA_ERROR = DMA_GEN_EVENT_IIDX_STAT_DATAERR
} DL_DMA_EVENT_IIDX;

/*! @enum DL_DMA_PUBLISHER_INDEX DMA 事件发布者索引。 */
typedef enum {
    /*! DMA Publisher index 0. DMA 发布者索引 0。 */
    DL_DMA_PUBLISHER_INDEX_0 = 0,
} DL_DMA_PUBLISHER_INDEX;

/*! @enum DL_DMA_SUBSCRIBER_INDEX DMA 事件订阅者索引。 */
typedef enum {
    /*! DMA Subscriber index 0. DMA 订阅者索引 0。 */
    DL_DMA_SUBSCRIBER_INDEX_0 = 0,
    /*! DMA Subscriber index 1. DMA 订阅者索引 1。 */
    DL_DMA_SUBSCRIBER_INDEX_1 = 1
} DL_DMA_SUBSCRIBER_INDEX;

/*! @enum DL_DMA_AUTOEN DMA 自动使能模式。 */
typedef enum {
    /*! No automatic DMA enable. 禁用自动 DMA 使能。 */
    DL_DMA_AUTOEN_DISABLE = DMA_DMACTL_DMAAUTOEN_DISABLE,
    #ifdef DEVICE_HAS_AUTO_AND_GATHER
    /*! Automatic DMA enable on DMASA register write. 写入 DMASA 寄存器时自动使能 DMA。 */
    DL_DMA_AUTOEN_DMASA = DMA_DMACTL_DMAAUTOEN_DMASA,
    /*! Automatic DMA enable on DMADA register write. 写入 DMADA 寄存器时自动使能 DMA。 */
    DL_DMA_AUTOEN_DMADA = DMA_DMACTL_DMAAUTOEN_DMADA,
    /*! Automatic DMA enable on DMASZ register write. 写入 DMASZ 寄存器时自动使能 DMA。 */
    DL_DMA_AUTOEN_DMASZ = DMA_DMACTL_DMAAUTOEN_DMASZ,
    #endif /* DEVICE_HAS_AUTO_AND_GATHER*/
} DL_DMA_AUTOEN;

    /* clang-format on */

    /*!
     *  @brief Configuration struct for @ref DL_DMA_initChannel.
     *  DMA 通道初始化配置结构体。
     */
    typedef struct
    {
        /*!
         *  The event that should trigger a DMA transfer. Refer to the datasheet of
         *  the device for which DMA trigger values map to which events.
         *  触发 DMA 传输的事件值，具体映射请参考设备数据手册。
         */
        uint8_t trigger;

        /*!
         *  Configure whether the DMA selects an internal or external channel as
         *  the DMA trigger.
         *  触发类型（内部或外部通道）。
         */
        DL_DMA_TRIGGER_TYPE triggerType;

        /*!
         * The transfer mode to use. Refer to the device datasheet to determine
         * which modes are supported in the selected channel.
         * One of @ref DL_DMA_TRANSFER_MODE.
         * 传输模式（单次/块/重复单次/重复块）。
         */
        DL_DMA_TRANSFER_MODE transferMode;

        /*! The extended mode to use. One of @ref DL_DMA_EXTENDED_MODE.
         *  扩展模式（正常/GATHER/FILL/TABLE）。 */
        DL_DMA_EXTENDED_MODE extendedMode;

        /*! The width of the DMA source. One of @ref DL_DMA_WIDTH.
         *  源数据宽度。 */
        DL_DMA_WIDTH srcWidth;

        /*! The width of the DMA destination. One of @ref DL_DMA_WIDTH.
         *  目标数据宽度。 */
        DL_DMA_WIDTH destWidth;

        /*!
         *  Amount to increment/decrement the DMA source address by. One of
         *  @ref DL_DMA_INCREMENT.
         *  源地址递增/递减步进。
         */
        DL_DMA_INCREMENT srcIncrement;

        /*!
         *  Amount to increment/decrement the DMA destination address by. One of
         *  @ref DL_DMA_INCREMENT.
         *  目标地址递增/递减步进。
         */
        DL_DMA_INCREMENT destIncrement;
    } DL_DMA_Config;

    /**
     *  @brief      Initialize a DMA channel. 初始化 DMA 通道。
     *
     *  Initializes all the configurable options for a DMA channel. The DMA channel
     *  is not enabled in this API.
     *  一次性配置 DMA 通道的传输模式与触发源，不在本接口内使能通道。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *  @param[in]  config        Pointer to DMA channel configuration settings. 通道配置结构体指针。
     */
    void DL_DMA_initChannel(
        DMA_Regs *dma, uint8_t channelNum, const DL_DMA_Config *config);

    /**
     *  @brief      Configure a DMA channel for a transfer. 配置 DMA 通道传输参数。
     *
     *  Configures the transfer settings for a DMA channel. The DMA channel is
     *  not enabled in this API.
     *  配置 DMA 通道的传输参数，不在本接口内使能通道。
     *
     *  @param[in]  dma             Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum      DMA channel to operate on. 目标通道号。
     *  @param[in]  transferMode    The transfer mode to use. One of @ref DL_DMA_TRANSFER_MODE. 传输模式。
     *  @param[in]  extendedMode    The extended mode to use. One of @ref DL_DMA_EXTENDED_MODE. 扩展模式。
     *  @param[in]  srcWidth        The width of the DMA source. One of @ref DL_DMA_WIDTH. 源数据宽度。
     *  @param[in]  destWidth       The width of the DMA destination. One of @ref DL_DMA_WIDTH. 目标数据宽度。
     *  @param[in]  srcIncrement    Amount to increment/decrement the DMA source address. One of @ref DL_DMA_INCREMENT. 源地址步进。
     *  @param[in]  destIncrement   Amount to increment/decrement the DMA destination address. One of @ref DL_DMA_INCREMENT. 目标地址步进。
     */
    __STATIC_INLINE void DL_DMA_configTransfer(DMA_Regs *dma, uint8_t channelNum,
                                               DL_DMA_TRANSFER_MODE transferMode, DL_DMA_EXTENDED_MODE extendedMode,
                                               DL_DMA_WIDTH srcWidth, DL_DMA_WIDTH destWidth,
                                               DL_DMA_INCREMENT srcIncrement, DL_DMA_INCREMENT destIncrement)
    {
        dma->DMACHAN[channelNum].DMACTL =
            ((uint32_t)transferMode | (uint32_t)extendedMode |
             (((uint32_t)destIncrement) << 4) | (uint32_t)srcIncrement |
             ((uint32_t)destWidth << 4) | (uint32_t)srcWidth);
    }

    /**
     *  @brief      Configure the DMA for round-robin priority. 启用 DMA 轮询优先级。
     *
     *  When round-robin priority is enabled, the channel that completes a transfer
     *  becomes the lowest priority. If multiple triggers happen simultaneously or
     *  are pending, the channel that transferred least recently will transfer
     *  first. Once it's complete the next highest priority channel will transfer.
     *  启用轮询优先级后，完成传输的通道变为最低优先级，最近最少传输的通道优先。
     *
     *  @param[in]  dma  Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     */
    __STATIC_INLINE void DL_DMA_enableRoundRobinPriority(DMA_Regs *dma)
    {
        dma->DMAPRIO |= DMA_DMAPRIO_ROUNDROBIN_ENABLE;
    }

    /**
     *  @brief      Disable round-robin priority for the DMA. 禁用 DMA 轮询优先级。
     *
     *  When round-robin priority is disabled, the channel priorities are fixed
     *  in ascending order (Channel 0 is the lowed priority). If multiple triggers
     *  happen simultaneously or are pending, the channel with the highest priority
     *  completes its transfer before the next-highest transfer can start.
     *  禁用后通道优先级固定，通道 0 最低，高优先级通道先传输。
     *
     *  @param[in]  dma  Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     */
    __STATIC_INLINE void DL_DMA_disableRoundRobinPriority(DMA_Regs *dma)
    {
        dma->DMAPRIO &= ~(DMA_DMAPRIO_ROUNDROBIN_MASK);
    }

    /**
     *  @brief      Check if round-robin priority is enabled for the DMA.
     *              查询 DMA 轮询优先级是否启用。
     *
     *  @param[in]  dma  Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *
     *  @return     The status of round-robin priority. 轮询优先级状态。
     *
     *  @retval     true  Round-robin priority is enabled. 已启用。
     *  @retval     false Round-robin priority is disabled. 已禁用。
     */
    __STATIC_INLINE bool DL_DMA_isRoundRobinPriorityEnabled(const DMA_Regs *dma)
    {
        return ((dma->DMAPRIO & DMA_DMAPRIO_ROUNDROBIN_MASK) ==
                DMA_DMAPRIO_ROUNDROBIN_ENABLE);
    }

    /**
     *  @brief      Set the burst size for block transfers. 设置块传输突发大小。
     *
     *  After the DMA transfers the amount of transfers defined by
     *  @ref DL_DMA_BURST_SIZE, the ongoing block transfer is interrupted and the
     *  priority encoder has the chance to assign a higher priority channel.
     *  达到突发大小后，当前块传输被中断，优先级编码器可分配更高优先级通道。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  burstSize     The burst size to set. One of @ref DL_DMA_BURST_SIZE. 突发大小。
     *
     *  @sa         DL_DMA_configTransfer
     */
    __STATIC_INLINE void DL_DMA_setBurstSize(
        DMA_Regs *dma, DL_DMA_BURST_SIZE burstSize)
    {
        DL_Common_updateReg(
            &dma->DMAPRIO, (uint32_t)burstSize, DMA_DMAPRIO_BURSTSZ_MASK);
    }

    /**
     *  @brief      Get the burst size for block transfers. 获取块传输突发大小。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *
     *  @return     The burst size for block transfers. 当前突发大小。
     *
     *  @retval     One of @ref DL_DMA_BURST_SIZE
     */
    __STATIC_INLINE DL_DMA_BURST_SIZE DL_DMA_getBurstSize(const DMA_Regs *dma)
    {
        uint32_t burstSize = dma->DMAPRIO & DMA_DMAPRIO_BURSTSZ_MASK;

        return (DL_DMA_BURST_SIZE)(burstSize);
    }

    /**
     *  @brief      Enable a DMA channel for transfers. 使能 DMA 通道。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     */
    __STATIC_INLINE void DL_DMA_enableChannel(DMA_Regs *dma, uint8_t channelNum)
    {
        dma->DMACHAN[channelNum].DMACTL |= DMA_DMACTL_DMAEN_ENABLE;
    }

    /**
     *  @brief      Disable a DMA channel for transfers. 禁用 DMA 通道。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     */
    __STATIC_INLINE void DL_DMA_disableChannel(DMA_Regs *dma, uint8_t channelNum)
    {
        dma->DMACHAN[channelNum].DMACTL &= ~(DMA_DMACTL_DMAEN_MASK);
    }

    /**
     *  @brief      Check if a DMA channel is enabled for transfers.
     *              查询 DMA 通道是否已使能。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The status of the DMA channel. 通道使能状态。
     *
     *  @retval     true  The DMA channel is enabled. 已使能。
     *  @retval     false The DMA channel is disabled. 已禁用。
     */
    __STATIC_INLINE bool DL_DMA_isChannelEnabled(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        return ((dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMAEN_MASK) ==
                DMA_DMACTL_DMAEN_ENABLE);
    }

    /**
     *  @brief      Configure the mode for a DMA channel. 配置 DMA 通道的传输和扩展模式。
     *
     *  @param[in]  dma          Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum   DMA channel to operate on. 目标通道号。
     *  @param[in]  transferMode  The transfer mode to set. One of @ref DL_DMA_TRANSFER_MODE. 传输模式。
     *  @param[in]  extendedMode The extended operation mode to set. One of @ref DL_DMA_EXTENDED_MODE. 扩展模式。
     *
     *  @sa         DL_DMA_configTransfer
     */
    __STATIC_INLINE void DL_DMA_configMode(DMA_Regs *dma, uint8_t channelNum,
                                           DL_DMA_TRANSFER_MODE transferMode, DL_DMA_EXTENDED_MODE extendedMode)
    {
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL,
                            (uint32_t)transferMode | (uint32_t)extendedMode,
                            DMA_DMACTL_DMATM_MASK | DMA_DMACTL_DMAEM_MASK);
    }

    /**
     *  @brief      Set a DMA channel's transfer mode. 设置 DMA 通道传输模式。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *  @param[in]  transferMode  The transfer mode to use. One of @ref DL_DMA_TRANSFER_MODE. 传输模式。
     *
     *  @sa         DL_DMA_configMode
     *  @sa         DL_DMA_configTransfer
     */
    __STATIC_INLINE void DL_DMA_setTransferMode(
        DMA_Regs *dma, uint8_t channelNum, DL_DMA_TRANSFER_MODE transferMode)
    {
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL,
                            (uint32_t)transferMode, DMA_DMACTL_DMATM_MASK);
    }

    /**
     *  @brief      Get a DMA channel's transfer mode. 获取 DMA 通道传输模式。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The channel's transfer mode. 当前传输模式。
     *
     *  @retval     One of @ref DL_DMA_TRANSFER_MODE
     */
    __STATIC_INLINE DL_DMA_TRANSFER_MODE DL_DMA_getTransferMode(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        uint32_t mode = (dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMATM_MASK);

        return (DL_DMA_TRANSFER_MODE)(mode);
    }

    /**
     *  @brief      Set a DMA channel's extended mode. 设置 DMA 通道扩展模式。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *  @param[in]  extendedMode  The extended mode to use. One of @ref DL_DMA_EXTENDED_MODE. 扩展模式。
     *
     *  @sa         DL_DMA_configMode
     *  @sa         DL_DMA_configTransfer
     */
    __STATIC_INLINE void DL_DMA_setExtendedMode(
        DMA_Regs *dma, uint8_t channelNum, DL_DMA_EXTENDED_MODE extendedMode)
    {
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL,
                            (uint32_t)extendedMode, DMA_DMACTL_DMAEM_MASK);
    }

    /**
     *  @brief      Get a DMA channel's extended mode. 获取 DMA 通道扩展模式。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The channel's extended mode. 当前扩展模式。
     *
     *  @retval     One of @ref DL_DMA_EXTENDED_MODE
     */
    __STATIC_INLINE DL_DMA_EXTENDED_MODE DL_DMA_getExtendedMode(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        uint32_t mode = (dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMAEM_MASK);

        return (DL_DMA_EXTENDED_MODE)(mode);
    }

    /**
     *  @brief      Start a DMA transfer using software. 软件触发 DMA 传输。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     */
    __STATIC_INLINE void DL_DMA_startTransfer(DMA_Regs *dma, uint8_t channelNum)
    {
        dma->DMACHAN[channelNum].DMACTL |= DMA_DMACTL_DMAREQ_REQUEST;
    }

    /**
     *  @brief      Set a channel's trigger for a DMA transfer. 设置 DMA 通道触发源。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *  @param[in]  trigger     What should trigger a DMA transfer. Refer to the device datasheet. 触发事件值。
     *  @param[in]  triggerType Whether an internal or external DMA channel is selected. One of @ref DL_DMA_TRIGGER_TYPE. 触发类型。
     */
    __STATIC_INLINE void DL_DMA_setTrigger(DMA_Regs *dma, uint8_t channelNum,
                                           uint8_t trigger, DL_DMA_TRIGGER_TYPE triggerType)
    {
        DL_Common_updateReg(&dma->DMATRIG[channelNum].DMATCTL,
                            trigger | (uint32_t)triggerType,
                            DMA_DMATCTL_DMATSEL_MASK | DMA_DMATCTL_DMATINT_MASK);
    }

    /**
     *  @brief      Get the current trigger for a DMA channel. 获取 DMA 通道当前触发事件值。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     What is configured to trigger a DMA transfer. 当前触发事件值。
     *
     *  @retval     Check the device datasheet for what values are mapped to on your device.
     */
    __STATIC_INLINE uint32_t DL_DMA_getTrigger(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        return (dma->DMATRIG[channelNum].DMATCTL & DMA_DMATCTL_DMATSEL_MASK);
    }

    /**
     *  @brief      Get the current trigger type for a DMA channel. 获取 DMA 通道触发类型。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     Whether an internal or external DMA channel is selected as the DMA trigger. 触发类型。
     *
     *  @retval     One of @ref DL_DMA_TRIGGER_TYPE
     */
    __STATIC_INLINE DL_DMA_TRIGGER_TYPE DL_DMA_getTriggerType(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        uint32_t triggerType =
            (dma->DMATRIG[channelNum].DMATCTL & DMA_DMATCTL_DMATINT_MASK);

        return (DL_DMA_TRIGGER_TYPE)(triggerType);
    }

    /**
     *  @brief      Set a DMA channel's source address. 设置 DMA 通道源地址。
     *
     *  Set the source address for a DMA channel for transferring data from. This
     *  address can be automatically incremented/decremented after the completion
     *  of a transfer by using the @ref DL_DMA_setSrcIncrement function.
     *  设置 DMA 数据传输的源地址，可通过 @ref DL_DMA_setSrcIncrement 设置自动递增/递减。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *  @param[in]  srcAddr     Address to set as the DMA source. 源地址。
     *
     *  @sa         DL_DMA_setSrcIncrement
     */
    __STATIC_INLINE void DL_DMA_setSrcAddr(
        DMA_Regs *dma, uint8_t channelNum, uint32_t srcAddr)
    {
        dma->DMACHAN[channelNum].DMASA = srcAddr;
    }

    /**
     *  @brief      Get a DMA channel's source address. 获取 DMA 通道源地址。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     Source address for the DMA channel. 源地址。
     */
    __STATIC_INLINE uint32_t DL_DMA_getSrcAddr(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        return dma->DMACHAN[channelNum].DMASA;
    }

    /**
     *  @brief      Set a DMA channel's destination address. 设置 DMA 通道目标地址。
     *
     *  Set the destination address for a DMA channel for transferring data to.
     *  This address can be automatically incremented/decremented after the
     *  completion of a transfer by using the @ref DL_DMA_setDestIncrement function.
     *  设置 DMA 数据传输的目标地址，可通过 @ref DL_DMA_setDestIncrement 设置自动递增/递减。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *  @param[in]  destAddr    Address to set as the DMA destination. 目标地址。
     *
     *  @sa         DL_DMA_setDestIncrement
     */
    __STATIC_INLINE void DL_DMA_setDestAddr(
        DMA_Regs *dma, uint8_t channelNum, uint32_t destAddr)
    {
        dma->DMACHAN[channelNum].DMADA = destAddr;
    }

    /**
     *  @brief      Get a DMA channel's destination address. 获取 DMA 通道目标地址。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     Destination address for the DMA channel. 目标地址。
     */
    __STATIC_INLINE uint32_t DL_DMA_getDestAddr(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        return dma->DMACHAN[channelNum].DMADA;
    }

    /**
     *  @brief      Set the size of a block for a DMA transfer. 设置 DMA 传输块大小。
     *
     *  Defines the size of the block of data to transfer.
     *  块传输模式下为每次触发传输的数据量；单次传输模式下为完成传输所需的触发次数。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *  @param[in]  size          The size of the block of data to transfer. Value between 0 - 65535. 传输大小（0-65535）。
     */
    __STATIC_INLINE void DL_DMA_setTransferSize(
        DMA_Regs *dma, uint8_t channelNum, uint16_t size)
    {
        dma->DMACHAN[channelNum].DMASZ = size;
    }

    /**
     *  @brief      Get a channel's size of block of data for a DMA transfer.
     *              获取 DMA 通道传输块大小。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *
     *  @return     The channel's size of block of data to transfer. 传输块大小。
     *
     *  @retval     A value between 0 - 65535.
     */
    __STATIC_INLINE uint16_t DL_DMA_getTransferSize(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        return (uint16_t)(dma->DMACHAN[channelNum].DMASZ & DMA_DMASZ_SIZE_MASK);
    }

    /**
     *  @brief      Set a channel's source address increment amount.
     *              设置 DMA 通道源地址步进。
     *
     *  After each DMA transfer the channel source address can be incremented,
     *  decremented or remain unchanged.
     *  每次 DMA 传输后，源地址可递增、递减或保持不变。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *  @param[in]  srcIncrement  Amount to increment/decrement the DMA source address. One of @ref DL_DMA_INCREMENT. 源地址步进模式。
     *
     *  @sa         DL_DMA_configTransfer
     *  @sa         DL_DMA_setSrcAddr
     *  @sa         DL_DMA_setSrcWidth
     */
    __STATIC_INLINE void DL_DMA_setSrcIncrement(
        DMA_Regs *dma, uint8_t channelNum, DL_DMA_INCREMENT srcIncrement)
    {
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL,
                            (uint32_t)srcIncrement, DMA_DMACTL_DMASRCINCR_MASK);
    }

    /**
     *  @brief      Return a channel's source address increment amount.
     *              获取 DMA 通道源地址步进。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The source address increment amount for selected channel. 源地址步进模式。
     *
     *  @retval     One of @ref DL_DMA_INCREMENT.
     */
    __STATIC_INLINE DL_DMA_INCREMENT DL_DMA_getSrcIncrement(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        uint32_t incrementAmount =
            (dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMASRCINCR_MASK);

        return (DL_DMA_INCREMENT)(incrementAmount);
    }

    /**
     *  @brief      Set a channel's destination address increment amount.
     *              设置 DMA 通道目标地址步进。
     *
     *  After each DMA transfer the channel destination address can be incremented,
     *  decremented or remain unchanged.
     *  每次 DMA 传输后，目标地址可递增、递减或保持不变。
     *
     *  @param[in]  dma           Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum    DMA channel to operate on. 目标通道号。
     *  @param[in]  destIncrement Amount to increment/decrement the DMA destination address. One of @ref DL_DMA_INCREMENT. 目标地址步进模式。
     *
     *  @sa         DL_DMA_configTransfer
     *  @sa         DL_DMA_setDestAddr
     *  @sa         DL_DMA_setDestWidth
     */
    __STATIC_INLINE void DL_DMA_setDestIncrement(
        DMA_Regs *dma, uint8_t channelNum, DL_DMA_INCREMENT destIncrement)
    {
        /* Left shifted by 4 so the defines align with bitfield */
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL,
                            ((uint32_t)destIncrement) << 4, DMA_DMACTL_DMADSTINCR_MASK);
    }

    /**
     *  @brief      Return a channel's destination address increment amount.
     *              获取 DMA 通道目标地址步进。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The destination address increment amount for selected channel. 目标地址步进模式。
     *
     *  @retval     One of @ref DL_DMA_INCREMENT.
     */
    __STATIC_INLINE DL_DMA_INCREMENT DL_DMA_getDestIncrement(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        /* Right shifted by 4 to align with provided defines */
        uint32_t incrementAmount =
            (dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMADSTINCR_MASK) >> 4;

        return (DL_DMA_INCREMENT)(incrementAmount);
    }

    /**
     *  @brief      Set the width of the DMA source address for a channel.
     *              设置 DMA 通道源数据宽度。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *  @param[in]  srcWidth    The width of the DMA source. One of @ref DL_DMA_WIDTH. 源数据宽度。
     *
     *  @sa         DL_DMA_configTransfer
     */
    __STATIC_INLINE void DL_DMA_setSrcWidth(
        DMA_Regs *dma, uint8_t channelNum, DL_DMA_WIDTH srcWidth)
    {
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL, (uint32_t)srcWidth,
                            DMA_DMACTL_DMASRCWDTH_MASK);
    }

    /**
     *  @brief      Get the width of the DMA source address for a channel.
     *              获取 DMA 通道源数据宽度。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The width of the DMA source for selected channel. 源数据宽度。
     *
     *  @retval     One of @ref DL_DMA_WIDTH.
     */
    __STATIC_INLINE DL_DMA_WIDTH DL_DMA_getSrcWidth(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        uint32_t width =
            (dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMASRCWDTH_MASK);

        return (DL_DMA_WIDTH)(width);
    }

    /**
     *  @brief      Set the width of the DMA destination address for a channel.
     *              设置 DMA 通道目标数据宽度。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *  @param[in]  destWidth   The width of the DMA destination. One of @ref DL_DMA_WIDTH. 目标数据宽度。
     *
     *  @sa         DL_DMA_configTransfer
     */
    __STATIC_INLINE void DL_DMA_setDestWidth(
        DMA_Regs *dma, uint8_t channelNum, DL_DMA_WIDTH destWidth)
    {
        /* Left shifted by 4 to align with provided defines */
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL,
                            ((uint32_t)destWidth) << 4, DMA_DMACTL_DMADSTWDTH_MASK);
    }

    /**
     *  @brief      Get the width of the DMA destination address for a channel.
     *              获取 DMA 通道目标数据宽度。
     *
     *  @param[in]  dma         Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The width of the DMA destination for selected channel. 目标数据宽度。
     *
     *  @retval     One of @ref DL_DMA_WIDTH.
     */
    __STATIC_INLINE DL_DMA_WIDTH DL_DMA_getDestWidth(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        /* Right shifted by 4 to align with provided defines */
        uint32_t width =
            (dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMADSTWDTH_MASK) >> 4;

        return (DL_DMA_WIDTH)(width);
    }
#ifdef DEVICE_HAS_DMA_FULL_CHANNEL
    /**
     *  @brief      Set the early interrupt event. 设置提前中断阈值。
     *
     *  This functionality is available for FULL-channel configuration only.
     *  仅 FULL 通道可用。DMA 可在完成中断前指定传输次数时产生提前中断，
     *  使 ISR 上下文切换与 DMA 完成并行执行，补偿任务切换延迟。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum     DMA channel to operate on. 目标通道号。
     *  @param[in]  threshold      When to generate the early interrupt. One of @ref DL_DMA_EARLY_INTERRUPT_THRESHOLD. 提前中断阈值。
     */
    __STATIC_INLINE void DL_DMA_Full_Ch_setEarlyInterruptThreshold(DMA_Regs *dma,
                                                                   uint8_t channelNum, DL_DMA_EARLY_INTERRUPT_THRESHOLD threshold)
    {
        DL_Common_updateReg(&dma->DMACHAN[channelNum].DMACTL, (uint32_t)threshold,
                            DMA_DMACTL_DMAPREIRQ_MASK);
    }

    /**
     *  @brief      Get the early interrupt event threshold. 获取提前中断阈值。
     *
     *  This functionality is available for FULL-channel configuration only.
     *  仅 FULL 通道可用。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum     DMA channel to operate on. 目标通道号。
     *
     *  @return     The early interrupt event threshold. 当前提前中断阈值。
     *
     *  @retval     One of @ref DL_DMA_EARLY_INTERRUPT_THRESHOLD.
     */
    __STATIC_INLINE DL_DMA_EARLY_INTERRUPT_THRESHOLD
    DL_DMA_Full_Ch_getEarlyInterruptThreshold(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        uint32_t threshold =
            dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMAPREIRQ_MASK;

        return (DL_DMA_EARLY_INTERRUPT_THRESHOLD)(threshold);
    }
#endif /* DEVICE_HAS_DMA_FULL_CHANNEL */

    /**
     *  @brief      Enable DMA interrupts. 使能 DMA 中断。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of @ref DL_DMA_INTERRUPT. 中断掩码。
     */
    __STATIC_INLINE void DL_DMA_enableInterrupt(
        DMA_Regs *dma, uint32_t interruptMask)
    {
        dma->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable DMA interrupts. 禁用 DMA 中断。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of @ref DL_DMA_INTERRUPT. 中断掩码。
     */
    __STATIC_INLINE void DL_DMA_disableInterrupt(
        DMA_Regs *dma, uint32_t interruptMask)
    {
        dma->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which DMA interrupts are enabled. 查询已使能的 DMA 中断。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of @ref DL_DMA_INTERRUPT. 中断掩码。
     *
     *  @return     Which of the requested DMA interrupts are enabled. 已使能的中断位。
     *
     *  @retval     Bitwise OR of @ref DL_DMA_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_DMA_getEnabledInterrupts(
        const DMA_Regs *dma, uint32_t interruptMask)
    {
        return (dma->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled DMA interrupts.
     *              查询已使能 DMA 中断的挂起状态。
     *
     *  Checks if any of the DMA interrupts that were previously enabled are pending.
     *  检查已使能的 DMA 中断是否处于挂起状态。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of @ref DL_DMA_INTERRUPT. 中断掩码。
     *
     *  @return     Which of the requested DMA interrupts are pending. 挂起的中断位。
     *
     *  @retval     Bitwise OR of @ref DL_DMA_INTERRUPT values
     *
     *  @sa         DL_DMA_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_DMA_getEnabledInterruptStatus(
        const DMA_Regs *dma, uint32_t interruptMask)
    {
        return (dma->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any DMA interrupt.
     *              查询任意 DMA 中断的原始挂起状态（无需事先使能）。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of @ref DL_DMA_INTERRUPT. 中断掩码。
     *
     *  @return     Which of the requested DMA interrupts are pending. 原始挂起中断位。
     *
     *  @retval     Bitwise OR of @ref DL_DMA_INTERRUPT values
     */
    __STATIC_INLINE uint32_t DL_DMA_getRawInterruptStatus(
        const DMA_Regs *dma, uint32_t interruptMask)
    {
        return (dma->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending DMA interrupt.
     *              获取最高优先级的挂起 DMA 中断。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *
     *  @return     The highest priority pending DMA interrupt. 最高优先级挂起中断索引。
     *
     *  @retval     One of @ref DL_DMA_EVENT_IIDX
     */
    __STATIC_INLINE DL_DMA_EVENT_IIDX DL_DMA_getPendingInterrupt(
        const DMA_Regs *dma)
    {
        return (DL_DMA_EVENT_IIDX)dma->CPU_INT.IIDX;
    }

    /**
     *  @brief      Clear pending DMA interrupts. 清除挂起的 DMA 中断。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of @ref DL_DMA_INTERRUPT. 中断掩码。
     */
    __STATIC_INLINE void DL_DMA_clearInterruptStatus(
        DMA_Regs *dma, uint32_t interruptMask)
    {
        dma->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief Sets the event publisher channel id. 设置事件发布者通道 ID。
     *
     *  @param[in]  dma     Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  index   Specifies the register event index to be configured. 发布者索引。
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0 publisher is disconnected. 通道 ID（0-15），0 表示断开。
     */
    __STATIC_INLINE void DL_DMA_setPublisherChanID(
        DMA_Regs *dma, DL_DMA_PUBLISHER_INDEX index, uint8_t chanID)
    {
        volatile uint32_t *pReg = &dma->FPUB_1;

        *(pReg + (uint32_t)index) = (chanID & DMA_FPUB_1_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event publisher channel id. 获取事件发布者通道 ID。
     *
     *  @param[in]  dma     Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  index   Specifies the register event index to be configured. 发布者索引。
     *
     *  @return     Event publisher channel ID. 发布者通道 ID。
     */
    __STATIC_INLINE uint8_t DL_DMA_getPublisherChanID(
        DMA_Regs *dma, DL_DMA_PUBLISHER_INDEX index)
    {
        volatile uint32_t *pReg = &dma->FPUB_1;

        return ((uint8_t)(*(pReg + (uint32_t)index) & DMA_FPUB_1_CHANID_MASK));
    }

    /**
     *  @brief Sets the event subscriber channel id. 设置事件订阅者通道 ID。
     *
     *  @param[in]  dma     Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  index   Specifies the register event index to be configured. 订阅者索引。
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0 subscriber is disconnected. 通道 ID（0-15），0 表示断开。
     */
    __STATIC_INLINE void DL_DMA_setSubscriberChanID(
        DMA_Regs *dma, DL_DMA_SUBSCRIBER_INDEX index, uint8_t chanID)
    {
        volatile uint32_t *pReg = &dma->FSUB_0;

        *(pReg + (uint32_t)index) = (chanID & DMA_FSUB_0_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event subscriber channel id. 获取事件订阅者通道 ID。
     *
     *  @param[in]  dma     Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  index   Specifies the register event index to be configured. 订阅者索引。
     *
     *  @return     Event subscriber channel ID. 订阅者通道 ID。
     */
    __STATIC_INLINE uint8_t DL_DMA_getSubscriberChanID(
        DMA_Regs *dma, DL_DMA_SUBSCRIBER_INDEX index)
    {
        volatile uint32_t *pReg = &dma->FSUB_0;

        return ((uint8_t)(*(pReg + (uint32_t)index) & DMA_FSUB_0_CHANID_MASK));
    }

    /**
     *  @brief      Enable DMA event. 使能 DMA 事件。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of events to enable. Bitwise OR of @ref DL_DMA_EVENT. 事件掩码。
     */
    __STATIC_INLINE void DL_DMA_enableEvent(DMA_Regs *dma, uint32_t eventMask)
    {
        dma->GEN_EVENT.IMASK |= (eventMask);
    }

    /**
     *  @brief      Disable DMA event. 禁用 DMA 事件。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of events to disable. Bitwise OR of @ref DL_DMA_EVENT. 事件掩码。
     */
    __STATIC_INLINE void DL_DMA_disableEvent(DMA_Regs *dma, uint32_t eventMask)
    {
        dma->GEN_EVENT.IMASK &= ~(eventMask);
    }

    /**
     *  @brief      Check which DMA events triggers are enabled. 查询已使能的 DMA 事件。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of events to check. Bitwise OR of @ref DL_DMA_EVENT. 事件掩码。
     *
     *  @return     Which of the requested DMA events are enabled. 已使能的事件位。
     *
     *  @retval     Bitwise OR of @ref DL_DMA_EVENT values
     */
    __STATIC_INLINE uint32_t DL_DMA_getEnabledEvents(
        const DMA_Regs *dma, uint32_t eventMask)
    {
        return (dma->GEN_EVENT.IMASK & eventMask);
    }

    /**
     *  @brief      Check event flag of enabled DMA event. 查询已使能 DMA 事件的挂起状态。
     *
     *  Checks if any of the DMA events that were previously enabled are pending.
     *  检查已使能的 DMA 事件是否处于挂起状态。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of events to check. Bitwise OR of @ref DL_DMA_EVENT. 事件掩码。
     *
     *  @return     Which of the requested DMA events are pending. 挂起的事件位。
     *
     *  @retval     Bitwise OR of @ref DL_DMA_EVENT values
     *
     *  @sa         DL_DMA_enableEvent
     */
    __STATIC_INLINE uint32_t DL_DMA_getEnabledEventStatus(
        const DMA_Regs *dma, uint32_t eventMask)
    {
        return (dma->GEN_EVENT.MIS & ~(eventMask));
    }

    /**
     *  @brief      Check event flag of any DMA event. 查询任意 DMA 事件的原始挂起状态。
     *
     *  Checks if any events are pending. Events do not have to be previously enabled.
     *  检查是否有事件挂起，无需事先使能。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of event to check. Bitwise OR of @ref DL_DMA_EVENT. 事件掩码。
     *
     *  @return     Which of the requested DMA event are pending. 原始挂起事件位。
     *
     *  @retval     Bitwise OR of @ref DL_DMA_EVENT values
     */
    __STATIC_INLINE uint32_t DL_DMA_getRawEventsStatus(
        const DMA_Regs *dma, uint32_t eventMask)
    {
        return (dma->GEN_EVENT.RIS & ~(eventMask));
    }

    /**
     *  @brief      Clear pending DMA events. 清除挂起的 DMA 事件。
     *
     *  @param[in]  dma            Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of events to clear. Bitwise OR of @ref DL_DMA_EVENT. 事件掩码。
     */
    __STATIC_INLINE void DL_DMA_clearEventsStatus(
        DMA_Regs *dma, uint32_t eventMask)
    {
        dma->GEN_EVENT.ICLR |= (eventMask);
    }

#ifdef DEVICE_HAS_AUTO_AND_GATHER
    /**
     *  @brief      Configure the DMA for auto-enable. 配置 DMA 自动使能。
     *
     *  @param[in]  dma             Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  mode            Enable auto-enable for DMA on a specified register write. One of @ref DL_DMA_AUTOEN. 自动使能触发模式。
     *  @param[in]  channelNum      DMA channel to operate on. 目标通道号。
     */
    __STATIC_INLINE void DL_DMA_enableAutoEnable(
        DMA_Regs *dma, DL_DMA_AUTOEN mode, uint8_t channelNum)
    {
        dma->DMACHAN[channelNum].DMACTL |= mode;
    }

    /**
     *  @brief      Disable auto-enable for DMA. 禁用 DMA 自动使能。
     *
     *  @param[in]  dma             Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum      DMA channel to operate on. 目标通道号。
     */
    __STATIC_INLINE void DL_DMA_disableAutoEnable(
        DMA_Regs *dma, uint8_t channelNum)
    {
        dma->DMACHAN[channelNum].DMACTL &= ~(DMA_DMACTL_DMAAUTOEN_MASK);
    }

    /**
     *  @brief      Check if auto-enable is enabled for the DMA. 查询 DMA 自动使能状态。
     *
     *  @param[in]  dma  Pointer to the register overlay for the peripheral. DMA 外设寄存器基址。
     *  @param[in]  channelNum  DMA channel to operate on. 目标通道号。
     *
     *  @return     The status of auto-enable. 自动使能状态。
     *
     *  @retval     true    auto-enable is enabled. 已启用。
     *  @retval     false   auto-enable is disabled. 已禁用。
     */
    __STATIC_INLINE bool DL_DMA_isAutoEnableEnabled(
        const DMA_Regs *dma, uint8_t channelNum)
    {
        return ((dma->DMACHAN[channelNum].DMACTL & DMA_DMACTL_DMAAUTOEN_MASK) !=
                DMA_DMACTL_DMAAUTOEN_DISABLE);
    }
#endif /* DEVICE_HAS_AUTO_AND_GATHER*/

#ifdef __cplusplus
}
#endif

#endif /* ti_dl_dl_m0p_dma__include */
/** @}*/
