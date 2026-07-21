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

/* 电压基准 (VREF) 驱动库实现文件
 * 提供 VREF 模块的配置、时钟设置等功能实现 */

#include <ti/driverlib/dl_vref.h>

#ifdef __MSPM0_HAS_VREF__

void DL_VREF_configReference(VREF_Regs *vref, const DL_VREF_Config *config)
{
    /* 配置使能、缓冲输出电压和采样保持模式 */
    vref->CTL0 = (uint32_t) config->vrefEnable | (uint32_t) config->bufConfig |
                 (uint32_t) config->shModeEnable;
    /* 配置采样保持周期和保持周期 */
    vref->CTL2 = (config->shCycleCount << VREF_CTL2_SHCYCLE_OFS) |
                 (config->holdCycleCount << VREF_CTL2_HCYCLE_OFS);
}

void DL_VREF_setClockConfig(VREF_Regs *vref, const DL_VREF_ClockConfig *config)
{
    /* 设置时钟源 */
    vref->CLKSEL = (uint32_t) config->clockSel;
    /* 设置分频系数 */
    vref->CLKDIV = (uint32_t) config->divideRatio;
}

void DL_VREF_getClockConfig(const VREF_Regs *vref, DL_VREF_ClockConfig *config)
{
    /* 读取当前时钟源配置 */
    config->clockSel    = (DL_VREF_CLOCK) vref->CLKSEL;
    /* 读取当前分频系数配置 */
    config->divideRatio = (DL_VREF_CLOCK_DIVIDE) vref->CLKDIV;
}

#endif /* __MSPM0_HAS_VREF__ */
