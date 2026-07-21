/*
 * Copyright (c) 2022, Texas Instruments Incorporated
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
/** @file dl_opa.c
 *  @brief OPA驱动库实现 - 提供动态增益调整功能
 */

#include <ti/driverlib/dl_opa.h>

#ifdef __MSPM0_HAS_OA__

DL_OPA_GAIN DL_OPA_increaseGain(OA_Regs *opa)
{
    DL_OPA_GAIN currentGain = DL_OPA_getGain(opa); // 读取当前增益值

    /* if CFG.GAIN 0x5 do nothing / 如果增益已为最大值0x5则不做操作 */
    if (currentGain == DL_OPA_GAIN_N31_P32) {
        return DL_OPA_GAIN_N31_P32;
    } else {
        // 增益值加1（左移OA_CFG_GAIN_OFS位）
        DL_OPA_setGain(
            opa, (DL_OPA_GAIN)(
                     currentGain + (((uint32_t) 0x1) << OA_CFG_GAIN_OFS)));
        return DL_OPA_getGain(opa); // 返回更新后的增益值
    }
}

DL_OPA_GAIN DL_OPA_decreaseGain(OA_Regs *opa)
{
    DL_OPA_GAIN currentGain = DL_OPA_getGain(opa); // 读取当前增益值

    /* if CFG.GAIN is 0x1 do nothing / 如果增益已为最小值0x1则不做操作 */
    if (currentGain == DL_OPA_GAIN_N1_P2) {
        return DL_OPA_GAIN_N1_P2;
    } else {
        // 增益值减1（左移OA_CFG_GAIN_OFS位）
        DL_OPA_setGain(
            opa, (DL_OPA_GAIN)(
                     currentGain - (((uint32_t) 0x1) << OA_CFG_GAIN_OFS)));
        return DL_OPA_getGain(opa); // 返回更新后的增益值
    }
}

#endif /* __MSPM0_HAS_OA__ */
