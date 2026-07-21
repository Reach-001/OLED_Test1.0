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
 * @file dl_trng.c
 * @brief 真随机数发生器 (TRNG) 驱动库源文件，实现配置保存与恢复功能
 */

#include <ti/driverlib/dl_trng.h>

#ifdef __MSPM0_HAS_TRNG__

/**
 * @brief 进入掉电状态前保存 TRNG 配置
 *
 * @param trng  外设寄存器基地址指针
 * @param ptr   配置备份结构体指针
 *
 * @return 若已存在备份则返回 FALSE，成功保存返回 TRUE
 */
bool DL_TRNG_saveConfiguration(
    const TRNG_Regs *trng, DL_TRNG_backupConfig *ptr)
{
    /* 仅在未备份时执行保存 */
    bool stateSaved = !ptr->backupRdy;
    if (stateSaved) {
        ptr->controlWord   = trng->CTL;          /* 保存控制寄存器 */
        ptr->clockDivider  = trng->CLKDIVIDE;    /* 保存时钟分频 */
        ptr->interruptMask = trng->CPU_INT.IMASK; /* 保存中断掩码 */
        ptr->backupRdy     = true;                /* 标记备份就绪 */
    }
    return stateSaved;
}

/**
 * @brief 掉电恢复后还原 TRNG 配置
 *
 * @param trng  外设寄存器基地址指针
 * @param ptr   配置备份结构体指针
 *
 * @return 若无备份则返回 FALSE，成功恢复返回 TRUE
 */
bool DL_TRNG_restoreConfiguration(TRNG_Regs *trng, DL_TRNG_backupConfig *ptr)
{
    /* 仅在已备份时执行恢复 */
    bool stateRestored = ptr->backupRdy;
    if (stateRestored) {
        /* 恢复控制寄存器（仅抽取率字段） */
        DL_Common_updateReg(
            &trng->CTL, ptr->controlWord, TRNG_CTL_DECIM_RATE_MASK);
        trng->CLKDIVIDE     = ptr->clockDivider;    /* 恢复时钟分频 */
        trng->CPU_INT.IMASK = ptr->interruptMask;    /* 恢复中断掩码 */

        /* 若原始状态为 OFF，则发送关机命令 */
        if (((ptr->controlWord & TRNG_STAT_FSM_STATE_MASK) >>
                TRNG_STAT_FSM_STATE_OFS) == (uint32_t) DL_TRNG_STATE_OFF) {
            DL_TRNG_sendCommand(trng, DL_TRNG_CMD_PWROFF);
        }

        ptr->backupRdy = false; /* 清除备份就绪标志 */
    }
    return stateRestored;
}

#endif /* __MSPM0_HAS_TRNG__ */
