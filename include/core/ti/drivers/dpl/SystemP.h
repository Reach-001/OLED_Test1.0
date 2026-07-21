/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
/** ===========================================================================
 *  @file       SystemP.h
 *
 *  @brief      System module for the RTOS Porting Interface
 *              RTOS移植接口的系统模块
 *
 *  Basic system services for supporting printf-like output.
 *
 *  ===========================================================================
 */

#ifndef ti_dpl_SystemP__include
#define ti_dpl_SystemP__include

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Formatted output to a buffer (variadic). / 格式化输出到缓冲区（可变参数）
 *  @param  buf     Output buffer. / 输出缓冲区
 *  @param  n       Buffer size. / 缓冲区大小
 *  @param  format  printf format string. / printf格式字符串
 *  @return Number of characters written (excluding null terminator). / 写入字符数（不含终止符） */
extern int SystemP_snprintf(char *buf, size_t n, const char *format,...);

/** @brief Formatted output to a buffer (va_list). / 格式化输出到缓冲区（va_list）
 *  @param  buf     Output buffer. / 输出缓冲区
 *  @param  n       Buffer size. / 缓冲区大小
 *  @param  format  printf format string. / printf格式字符串
 *  @param  va      va_list of arguments. / 参数va_list
 *  @return Number of characters written (excluding null terminator). / 写入字符数（不含终止符） */
extern int SystemP_vsnprintf(char *buf, size_t n, const char *format, va_list va);

/*!
 * @brief    Start command for the RTOS scheduler / 启动RTOS调度器
 *
 * This function will never return. Call only once, from the application's
 * main function. / 此函数不会返回，仅在main函数中调用一次
 */
extern void SystemP_startScheduler(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_dpl_SemaphoreP__include */
