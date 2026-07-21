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
 *  @file       dl_common.c
 *  @brief      DriverLib Common APIs Implementation.
 *              DriverLib 公共 API 实现文件，包含寄存器掩码写入与 CPU 周期延时等功能。
 ******************************************************************************/

#include <stdint.h>

#include <ti/driverlib/dl_common.h>

/**
 * @brief   Consumes the number of CPU cycles specified.
 *          按请求值执行最少周期 CPU 忙等待延时。保证至少延时 cycles 个周期，但不保证精确等于该值。
 *
 * @param[in] cycles  Floor number of cycles to delay. 期望最小延时周期数；传入 0 将导致最大可实现延时。
 */
void DL_Common_delayCycles(uint32_t cycles)
{
        /* 临时寄存器：仅用于承接汇编循环计数，避免污染调用方变量 */
        uint32_t scratch;

        /* There will be a 2 cycle delay here to fetch & decode instructions
         * if branch and linking to this function */

        /* Subtract 2 net cycles for constant offset: +2 cycles for entry jump,
         * +2 cycles for exit, -1 cycle for a shorter loop cycle on the last loop,
         * -1 for this instruction */

        __asm volatile(
#ifdef __GNUC__
            ".syntax unified\n\t"
#endif
            /* 先扣除函数调用与尾次循环带来的固定周期偏移 */
            "SUBS %0, %[numCycles], #2; \n"
            "%=: \n\t"
            /* 每轮循环约消耗 4 周期，通过减 4 接近期望延时 */
            "SUBS %0, %0, #4; \n\t"
            "NOP; \n\t"
            "BHS  %=b;" /* 若仍大于等于 0，回跳继续延时 */
            /* Return: 2 cycles */
            : "=&r"(scratch)
            : [numCycles] "r"(cycles));
}
