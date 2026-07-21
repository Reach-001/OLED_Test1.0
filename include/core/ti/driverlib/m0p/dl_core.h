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
 *  @file       dl_core.h
 *  @brief      CPU Core Driver Library / CPU 核心驱动库
 *  @defgroup   CORE M0P Core
 *
 *  @anchor ti_dl_m0p_dl_core_Overview
 *  # Overview
 *
 *  The Core module enables software to read from core registers of the CPU to
 *  get more information about the device at runtime.
 *  核心模块允许软件读取 CPU 核心寄存器，以在运行时获取设备信息（如处理器型号、
 *  架构版本、缓存/预取配置等）。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup CORE
 * @{
 */
#ifndef ti_dl_m0p_dl_core__include
#define ti_dl_m0p_dl_core__include

#include <stdint.h>

#include <ti/devices/msp/msp.h>

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_CORE_CACHE
 *  @{
 */

/*!
 * @brief Enables instruction caching on flash access / 使能 Flash 访问的指令缓存
 */
#define DL_CORE_CACHE_ENABLED                          (CPUSS_CTL_ICACHE_ENABLE)

/*!
 * @brief Disables instruction caching on flash access / 禁用 Flash 访问的指令缓存
 */
#define DL_CORE_CACHE_DISABLED                        (CPUSS_CTL_ICACHE_DISABLE)

/** @}*/

/** @addtogroup DL_CORE_PREFETCH
 *  @{
 */

/*!
 * @brief Enables instruction prefetch to Flash / 使能 Flash 指令预取
 */
#define DL_CORE_PREFETCH_ENABLED                     (CPUSS_CTL_PREFETCH_ENABLE)

/*!
 * @brief Disables instruction prefetch to Flash / 禁用 Flash 指令预取
 */
#define DL_CORE_PREFETCH_DISABLED                   (CPUSS_CTL_PREFETCH_DISABLE)

/** @}*/

/** @addtogroup DL_CORE_LITERAL_CACHE
 *  @{
 */

/*!
 * @brief Enables caching and prefetching of literals.
 * This is set only if the ICACHE or PREFETCH bits have been set respectively.
 * 使能常量缓存和预取，仅在 ICACHE 或 PREFETCH 位已设置时生效。
 */
#define DL_CORE_LITERAL_CACHE_ENABLED                   (CPUSS_CTL_LITEN_ENABLE)

/*!
 * @brief Disables caching and prefetching of literals / 禁用常量缓存和预取
 */
#define DL_CORE_LITERAL_CACHE_DISABLED                 (CPUSS_CTL_LITEN_DISABLE)

/** @}*/


/**
 * @brief Alias for DL_Common_delayCycles / DL_Common_delayCycles 的便捷别名
 *        参数 cycles 为需要延时的 CPU 时钟周期数
 */
#define delay_cycles(cycles) DL_Common_delayCycles(cycles)

/** @}*/

/* clang-format on  */


/**
 *  @brief   Get the implementer code for the processor
 *           获取处理器制造商代码
 *
 *  @return  The implementer code / 制造商代码
 *
 *  @retval  0x41 for ARM / 0x41 表示 ARM
 */
__STATIC_INLINE uint32_t DL_CORE_getImplementer(void)
{
    return (
        (SCB->CPUID & SCB_CPUID_IMPLEMENTER_Msk) >> SCB_CPUID_IMPLEMENTER_Pos);
}

/**
 *  @brief   Get the major revision number 'n' in the 'npm' revision status
 *           获取处理器修订号中的主版本号 'n'（'npm' 格式）
 *
 *  @return  The major revision number / 主版本号
 *
 *  @retval  0x00 for revision 0 / 0x00 表示修订版本 0
 */
__STATIC_INLINE uint32_t DL_CORE_getVariant(void)
{
    return ((SCB->CPUID & SCB_CPUID_VARIANT_Msk) >> SCB_CPUID_VARIANT_Pos);
}

/**
 *  @brief   Get the architecture of the processor
 *           获取处理器架构版本
 *
 *  @return  Value that defines the architecture of the processor / 架构标识值
 *
 *  @retval  0x0C for ARMv6-M architecture / 0x0C 表示 ARMv6-M 架构
 */
__STATIC_INLINE uint32_t DL_CORE_getArchitecture(void)
{
    return ((SCB->CPUID & SCB_CPUID_ARCHITECTURE_Msk) >>
            SCB_CPUID_ARCHITECTURE_Pos);
}

/**
 *  @brief   Get part number of the processor (not the device)
 *           获取处理器部件号（非芯片型号）
 *
 *  @return  Value that defines the processor / 处理器部件号
 *
 *  @retval  0x0C60 for Cortex-M0+ / 0x0C60 表示 Cortex-M0+
 */
__STATIC_INLINE uint32_t DL_CORE_getPartNumber(void)
{
    return ((SCB->CPUID & SCB_CPUID_PARTNO_Msk) >> SCB_CPUID_PARTNO_Pos);
}

/**
 *  @brief   Get the minor revision number 'm' in the 'npm' revision status
 *           获取处理器修订号中的次版本号 'm'（'npm' 格式）
 *
 *  @return  The minor revision number / 次版本号
 *
 *  @retval  0x01 for patch 1 / 0x01 表示补丁版本 1
 */
__STATIC_INLINE uint32_t DL_CORE_getRevision(void)
{
    return ((SCB->CPUID & (uint32_t)SCB_CPUID_REVISION_Msk) >> (uint32_t)SCB_CPUID_REVISION_Pos);
}

/**
 * @brief Configures instruction caching in flash accesses and instruction
 *        prefetch  to flash
 *        配置 Flash 访问的指令缓存、指令预取和常量缓存
 *
 * @param icache [in]    Instruction cache option @ref DL_CORE_CACHE
 *                       指令缓存选项 @ref DL_CORE_CACHE
 * @param prefetch [in]  Instruction prefetch option @ref DL_CORE_PREFETCH
 *                       指令预取选项 @ref DL_CORE_PREFETCH
 * @param litCache [in]  Literal cache option @ref DL_CORE_LITERAL_CACHE
 *                       常量缓存选项 @ref DL_CORE_LITERAL_CACHE
 *
 */
__STATIC_INLINE void DL_CORE_configInstruction(uint32_t icache, uint32_t prefetch, uint32_t litCache)
{
    CPUSS->CTL = (icache | prefetch | litCache);
}

/**
 * @brief Returns instruction caching, prefetch, and literal cache configuration
 *        获取当前指令缓存、预取和常量缓存的配置
 *
 * @return Bitwise OR of @ref DL_CORE_CACHE and @ref DL_CORE_PREFETCH and
 * @ref DL_CORE_LITERAL_CACHE
 *         @ref DL_CORE_CACHE、@ref DL_CORE_PREFETCH 和
 *         @ref DL_CORE_LITERAL_CACHE 的按位或结果
 */
__STATIC_INLINE uint32_t DL_CORE_getInstructionConfig(void)
{
    return(CPUSS->CTL & (CPUSS_CTL_ICACHE_MASK | CPUSS_CTL_PREFETCH_MASK | CPUSS_CTL_LITEN_MASK));
}

#ifdef __cplusplus
}

#endif

#endif /* ti_dl_m0p_dl_core__include */
/** @}*/
