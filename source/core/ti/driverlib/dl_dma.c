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
 * @brief MSPM0 DMA 直接内存访问驱动库实现。
 *
 * 提供 DMA 通道初始化等非内联函数的实现。
 */

#include <ti/driverlib/dl_dma.h>

/**
 * @brief 初始化 DMA 通道基础配置。
 *
 * @param[in] dma DMA 外设寄存器基址。
 * @param[in] channelNum DMA 通道号。
 * @param[in] config DMA 通道配置结构体指针。
 *
 * @return 无返回值。
 */
void DL_DMA_initChannel(
    DMA_Regs *dma, uint8_t channelNum, const DL_DMA_Config *config)
{
    /* 先配置传输属性（宽度、步进、模式），再配置触发源 */
    DL_DMA_configTransfer(dma, channelNum, config->transferMode,
                          config->extendedMode, config->srcWidth, config->destWidth,
                          config->srcIncrement, config->destIncrement);

    /* 最后写入触发配置，避免通道提前进入错误触发条件 */
    DL_DMA_setTrigger(dma, channelNum, config->trigger, config->triggerType);
}
