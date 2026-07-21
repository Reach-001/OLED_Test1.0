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
 *  @brief 中断管理驱动实现
 *         提供中断向量表 RAM 迁移、中断注册/注销等功能。
 */

#include <stdint.h>

#include <ti/driverlib/m0p/dl_interrupt.h>

/* 32 Device Peripheral Exceptions + 16 reserved M0+ Exceptions */
/* Cortex-M0+ 保留异常数量（系统异常） */
#define NUM_RES_M0_EXC ((uint32_t)16)
/* 设备外设中断数量 */
#define NUM_DEV_PERIPH_EXC ((uint32_t)32)
/* 向量表总条目数 = 16 系统异常 + 32 设备中断 */
#define NUM_INTERRUPTS (NUM_RES_M0_EXC + NUM_DEV_PERIPH_EXC)

/* RAM 中的中断向量表副本，256 字节对齐以满足 VTOR 寄存器要求 */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 256
static __no_init void (*InterruptRAMVectors[NUM_INTERRUPTS])(void) @ "VTABLE";
#else
static __attribute__((section(".vtable"))) void (
    *InterruptRAMVectors[NUM_INTERRUPTS])(void) __attribute__((aligned(256)));
#endif

/**
 * @brief 注册中断处理函数，并在需要时将向量表复制到 SRAM。
 *
 * @param[in] exceptionNumber 设备中断号（不含 Cortex-M 保留异常偏移）。
 * @param[in] intHandler      用户中断处理函数指针。
 *
 * @return 无返回值。
 */
void DL_Interrupt_registerInterrupt(
    uint32_t exceptionNumber, void (*intHandler)(void))
{
    void (**baseAddress)(void);
    uint32_t i;

    /* 判断当前 VTOR 是否已指向 RAM 向量表，避免重复拷贝 */
    if (SCB->VTOR != (uint32_t)InterruptRAMVectors)
    {
        baseAddress = (void (**)(void))SCB->VTOR;

        /* 首次注册：将 Flash 向量表完整复制到 RAM，后续可按需改写某个中断入口 */
        for (i = 0; i < NUM_INTERRUPTS; i++)
        {
            InterruptRAMVectors[i] = baseAddress[i];
        }

        /* 切换 VTOR 到 RAM 向量表，确保中断分发使用可写表项 */
        SCB->VTOR = (uint32_t)InterruptRAMVectors;
    }

    /* +16 跳过 Cortex-M0+ 的系统异常向量，只覆盖设备中断向量 */
    InterruptRAMVectors[exceptionNumber + NUM_RES_M0_EXC] = intHandler;
}

/**
 * @brief 取消中断处理函数注册并恢复默认处理函数。
 *
 * @param[in] exceptionNumber 设备中断号（不含 Cortex-M 保留异常偏移）。
 *
 * @return 无返回值。
 */
void DL_Interrupt_unregisterInterrupt(uint32_t exceptionNumber)
{
    InterruptRAMVectors[exceptionNumber + NUM_RES_M0_EXC] = Default_Handler;
}
