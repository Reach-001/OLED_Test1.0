/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 * @brief Timer B (基础定时器) 驱动库源文件
 *        本文件包含 BTimer 特有的函数实现（如 initTimer），
 *        大部分内联函数在头文件中定义。
 */

#include <ti/driverlib/dl_timerb.h>

#ifdef __MSPM0_HAS_TIMER_B__

/**
 *  @brief      Initializes all the configurable options for the Basic Timer
 *              初始化基础定时器的所有可配置选项
 *
 *  @param[in]  timerb       Pointer to the register overlay for the peripheral
 *                           外设寄存器基地址指针
 *  @param[in]  config       Pointer to the mode configuration struct
 *                           定时器配置结构体指针
 *  @param[in]  idx          Counter index
 *                           计数器索引
 */
void DL_TimerB_initTimer(BTIMER_Regs* const timerb,
    DL_TimerB_TimerConfig* const config, DL_TIMERB_COUNTER_IDX idx)
{
    // 设置计数器装载值
    DL_TimerB_setLoadValue(timerb, idx, config->loadValue);
    // 设置时钟源
    DL_TimerB_setTimerClockSource(timerb, idx, config->clockSource);
    // 设置复位源
    DL_TimerB_setTimerResetSource(timerb, idx, config->resetSource);
    // 设置停止源
    DL_TimerB_setTimerStopSource(timerb, idx, config->stopSource);
    // 设置启动源
    DL_TimerB_setTimerStartSource(timerb, idx, config->startSource);
    // 根据配置决定是否立即启动定时器
    if (config->startTimer) {
        DL_TimerB_startCounter(timerb, idx);
    }
}

#endif /* __MSPM0_HAS_TIMER_B__ */
