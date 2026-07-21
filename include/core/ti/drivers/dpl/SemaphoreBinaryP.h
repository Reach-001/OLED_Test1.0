/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       SemaphoreBinaryP.h
 *
 *  @brief      Very lightweight binary Semaphore module for the RTOS Porting
 *              Interface
 *              RTOS移植接口的超轻量二值信号量模块
 *
 *  Binary semaphores can have only two states: available (count = 1) and
 *  unavailable (count = 0). They can be used to share a single resource
 *  between tasks. They can also be used for a basic signalling mechanism, where
 *  the semaphore can be posted multiple times. Binary semaphores do not keep
 *  track of the count; they simply track whether the semaphore has been posted
 *  or not.
 *
 *  ============================================================================
 */

#ifndef ti_dpl_SemaphoreBinaryP__include
#define ti_dpl_SemaphoreBinaryP__include

typedef bool SemaphoreBinaryP_Struct;

typedef void *SemaphoreBinaryP_Handle;

/*!
 *  @brief    Wait forever define / 永久等待
 */
#define SemaphoreBinaryP_WAIT_FOREVER ~(0)

/*!
 *  @brief    No wait define / 不等待
 */
#define SemaphoreBinaryP_NO_WAIT (0)

/*!
 *  @brief    Status codes for SemaphoreP APIs (for backwards compatibility) / 信号量API返回状态码（向后兼容）
 */
typedef enum {
    /*! API completed successfully / API执行成功 */
    SemaphoreBinaryP_OK = 0,
    /*! API failed because of a timeout / API因超时失败 */
    SemaphoreBinaryP_TIMEOUT = -1
} SemaphoreBinaryP_Status;

/*!
 *  @brief  Construct a binary semaphore (inline). / 构造二值信号量（内联）
 *
 *  @param  handle  Pointer to SemaphoreBinaryP_Struct. / 指向SemaphoreBinaryP_Struct的指针
 *  @param  count   Initial count (0 or 1). / 初始计数（0或1）
 *
 *  @return SemaphoreBinaryP_Handle / 返回信号量句柄
 */
static inline SemaphoreBinaryP_Handle SemaphoreBinaryP_construct(
    SemaphoreBinaryP_Struct *handle, unsigned int count)
{
    SemaphoreBinaryP_Struct *pSemaphore = (SemaphoreBinaryP_Struct *) handle;

    *pSemaphore = count > 0 ? true : false;

    return ((SemaphoreBinaryP_Handle) handle);
}

/** @brief Destruct a binary semaphore. / 析构二值信号量 */
void SemaphoreBinaryP_destruct(SemaphoreBinaryP_Struct *handle);

/** @brief Pend (wait) on a binary semaphore with timeout. / 等待二值信号量（带超时）
 *  @param  handle  Semaphore handle. / 信号量句柄
 *  @param  timeout Timeout in ticks. / 超时tick数
 *  @return SemaphoreBinaryP_OK or SemaphoreBinaryP_TIMEOUT. */
SemaphoreBinaryP_Status SemaphoreBinaryP_pend(
    SemaphoreBinaryP_Handle handle, uint32_t timeout);

/** @brief Pend (wait) on a binary semaphore indefinitely. / 永久等待二值信号量
 *  @param  handle  Semaphore handle. / 信号量句柄
 *  @return SemaphoreBinaryP_OK. */
SemaphoreBinaryP_Status SemaphoreBinaryP_pendNoTimeout(
    SemaphoreBinaryP_Handle handle);

/** @brief Post (signal) a binary semaphore from ISR context. / 在ISR中释放二值信号量
 *  @param  handle  Semaphore handle. / 信号量句柄 */
void SemaphoreBinaryP_postFromISR(SemaphoreBinaryP_Handle handle);

/** @brief Post (signal) a binary semaphore. / 释放二值信号量
 *  @param  handle  Semaphore handle. / 信号量句柄 */
void SemaphoreBinaryP_post(SemaphoreBinaryP_Handle handle);

#endif /* ti_dpl_SemaphoreBinaryP__include */
