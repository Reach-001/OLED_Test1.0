/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @file       dl_spgss.h
 *  @brief      Serial Peripheral Group (SPGSS) Driver Library 串行外设组(SPGSS)驱动库
 *  @defgroup   SPGSS Serial Peripheral Group (SPGSS)
 *
 *  @anchor ti_dl_dl_m0p_spgss_Overview
 *  # Overview
 *
 *  The Serial Peripheral Group (SPGSS) Driver Library allows full configuration
 *  of the SPG module.
 *  The SPG module combines one or more UNICOMM modules for special functions
 *  like I2C loopback.
 *
 *  本文件提供 SPGSS 模块的完整配置支持。SPGSS 模块将一个或多个 UNICOMM 模块
 *  组合在一起，用于 I2C 环回(Loopback)等特殊功能。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup SPGSS
 * @{
 */
#ifndef ti_dl_dl_spgss__include
#define ti_dl_dl_spgss__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MCU_HAS_SPGSS__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_SPGSS_INTERRUPTS
 *  @{
 */

/** @}*/

/*! @enum DL_SPGSS_PAIR_CONTROLLER_SEL 环回对控制器选择，指定哪个 UNICOMM 作为主控端 */
typedef enum {
    /*! Select UC0 as controller for loopback 选择 UC0 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC0_= SPGSS_PAIR0_CTL_SPG1,
    /*! Select UC1 as controller for loopback 选择 UC1 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC1_= SPGSS_PAIR0_CTL_SPG2,
    /*! Select UC2 as controller for loopback 选择 UC2 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC2_= SPGSS_PAIR0_CTL_SPG3,
    /*! Select UC3 as controller for loopback 选择 UC3 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC3_= SPGSS_PAIR0_CTL_SPG4,
    /*! Select UC4 as controller for loopback 选择 UC4 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC4_= SPGSS_PAIR0_CTL_SPG5,
    /*! Select UC5 as controller for loopback 选择 UC5 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC5_= SPGSS_PAIR0_CTL_SPG6,
    /*! Select UC6 as controller for loopback 选择 UC6 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC6_= SPGSS_PAIR0_CTL_SPG7,
    /*! Select UC7 as controller for loopback 选择 UC7 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC7_= SPGSS_PAIR0_CTL_SPG8,
    /*! Select UC8 as controller for loopback 选择 UC8 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC8_= SPGSS_PAIR0_CTL_SPG9,
    /*! Select UC9 as controller for loopback 选择 UC9 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC9_= SPGSS_PAIR0_CTL_SPG10,
    /*! Select UC10_as controller for loopback 选择 UC10 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC10_= SPGSS_PAIR0_CTL_SPG11,
    /*! Select UC11_as controller for loopback 选择 UC11 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC11_= SPGSS_PAIR0_CTL_SPG12,
    /*! Select UC12_as controller for loopback 选择 UC12 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC12_= SPGSS_PAIR0_CTL_SPG13,
    /*! Select UC13_as controller for loopback 选择 UC13 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC13_= SPGSS_PAIR0_CTL_SPG14,
    /*! Select UC14_as controller for loopback 选择 UC14 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC14_= SPGSS_PAIR0_CTL_SPG15,
    /*! Select UC15_as controller for loopback 选择 UC15 作为环回控制器 */
    DL_SPGSS_PAIR_CONTROLLER_UC15_= SPGSS_PAIR0_CTL_SPG16,
} DL_SPGSS_PAIR_CONTROLLER_SEL;

/*! @enum DL_SPGSS_PAIR_TARGET_SEL 环回对目标选择，指定哪个 UNICOMM 作为从属端 */
typedef enum {
    /*! Select UC0 as target for loopback 选择 UC0 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC0_= SPGSS_PAIR0_TARGET_SPG1,
    /*! Select UC1 as target for loopback 选择 UC1 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC1_= SPGSS_PAIR0_TARGET_SPG2,
    /*! Select UC2 as target for loopback 选择 UC2 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC2_= SPGSS_PAIR0_TARGET_SPG3,
    /*! Select UC3 as target for loopback 选择 UC3 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC3_= SPGSS_PAIR0_TARGET_SPG4,
    /*! Select UC4 as target for loopback 选择 UC4 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC4_= SPGSS_PAIR0_TARGET_SPG5,
    /*! Select UC5 as target for loopback 选择 UC5 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC5_= SPGSS_PAIR0_TARGET_SPG6,
    /*! Select UC6 as target for loopback 选择 UC6 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC6_= SPGSS_PAIR0_TARGET_SPG7,
    /*! Select UC7 as target for loopback 选择 UC7 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC7_= SPGSS_PAIR0_TARGET_SPG8,
    /*! Select UC8 as target for loopback 选择 UC8 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC8_= SPGSS_PAIR0_TARGET_SPG9,
    /*! Select UC9 as target for loopback 选择 UC9 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC9_= SPGSS_PAIR0_TARGET_SPG10,
    /*! Select UC10_as target for loopback 选择 UC10 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC10_= SPGSS_PAIR0_TARGET_SPG11,
    /*! Select UC11_as target for loopback 选择 UC11 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC11_= SPGSS_PAIR0_TARGET_SPG12,
    /*! Select UC12_as target for loopback 选择 UC12 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC12_= SPGSS_PAIR0_TARGET_SPG13,
    /*! Select UC13_as target for loopback 选择 UC13 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC13_= SPGSS_PAIR0_TARGET_SPG14,
    /*! Select UC14_as target for loopback 选择 UC14 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC14_= SPGSS_PAIR0_TARGET_SPG15,
    /*! Select UC15_as target for loopback 选择 UC15 作为环回目标 */
    DL_SPGSS_PAIR_TARGET_UC15_= SPGSS_PAIR0_TARGET_SPG16,
} DL_SPGSS_PAIR_TARGET_SEL;

/*! @enum DL_SPGSS_PAIR_INDEX 环回对索引，最多支持 8 对 */
typedef enum {
    /*! Loopback pair index 0 环回对索引 0 */
    DL_SPGSS_PAIR_INDEX_0 = 0,
    /*! Loopback pair index 1 环回对索引 1 */
    DL_SPGSS_PAIR_INDEX_1 = 1,
    /*! Loopback pair index 2 环回对索引 2 */
    DL_SPGSS_PAIR_INDEX_2 = 2,
    /*! Loopback pair index 3 环回对索引 3 */
    DL_SPGSS_PAIR_INDEX_3 = 3,
    /*! Loopback pair index 4 环回对索引 4 */
    DL_SPGSS_PAIR_INDEX_4 = 4,
    /*! Loopback pair index 5 环回对索引 5 */
    DL_SPGSS_PAIR_INDEX_5 = 5,
    /*! Loopback pair index 6 环回对索引 6 */
    DL_SPGSS_PAIR_INDEX_6 = 6,
    /*! Loopback pair index 7 环回对索引 7 */
    DL_SPGSS_PAIR_INDEX_7 = 7,
} DL_SPGSS_PAIR_INDEX;

/**
 * @brief  Configuration struct for @ref DL_SPGSS_setLoopbackConfig.
 *         环回配置结构体，用于 @ref DL_SPGSS_setLoopbackConfig。
 */
typedef struct {
    /*! SPGSS pairing controller selection. One of @ref DL_SPGSS_PAIR_CONTROLLER_SEL
     *  环回控制器选择 */
    DL_SPGSS_PAIR_CONTROLLER_SEL controller;
    /*! SPGSS pairing target selection. One of @ref DL_SPGSS_PAIR_TARGET_SEL
     *  环回目标选择 */
    DL_SPGSS_PAIR_TARGET_SEL target;
} DL_SPGSS_LoopbackConfig;

/**
 *  @brief      Enable SPGSS pair loopback 使能 SPGSS 环回对
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  index  Specifies pair index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 *                     指定要配置的环回对索引
 */
__STATIC_INLINE void DL_SPGSS_enableLoopback(
    SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    /* 按索引偏移寻址对应环回对寄存器，置位使能位 */
    *(pReg + (uint32_t) index) |= (SPGSS_PAIR0_EN_ENABLE);
}

/**
 *  @brief      Disable SPGSS pair loopback 禁用 SPGSS 环回对
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  index  Specifies pair index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 *                     指定要配置的环回对索引
 */
__STATIC_INLINE void DL_SPGSS_disableLoopback(
    SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    /* 按索引偏移寻址对应环回对寄存器，清除使能位 */
    *(pReg + (uint32_t) index) &= ~(SPGSS_PAIR0_EN_ENABLE);
}

/**
 *  @brief      Set SPG controller and target for loopback
 *              设置环回对的控制器和目标
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  index  Specifies pair index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 *                     指定要配置的环回对索引
 *  @param[in]  config  Pointer to the pair configuration struct
 *                      @ref DL_SPGSS_LoopbackConfig.
 *                     环回配置结构体指针
 */
__STATIC_INLINE void DL_SPGSS_setLoopbackConfig(SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index, DL_SPGSS_LoopbackConfig *config)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    /* 将控制器和目标字段合并后写入对应环回对寄存器 */
    DL_Common_updateReg((pReg + (uint32_t) index),
        ((uint32_t) config->controller | (uint32_t) config->target),
        (SPGSS_PAIR0_CTL_MASK | SPGSS_PAIR0_TARGET_MASK));
}

/**
 *  @brief      Get SPG controller and target for loopback
 *              获取环回对的当前控制器和目标配置
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  index  Specifies loopback index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 *                     指定要读取的环回对索引
 *  @param[in]  config  Pointer to the pair configuration struct
 *                      @ref DL_SPGSS_LoopbackConfig.
 *                     环回配置结构体指针，用于接收读取结果
 */
__STATIC_INLINE void DL_SPGSS_getLoopbackConfig(SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index, DL_SPGSS_LoopbackConfig *config)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    /* 分别用 CTL 和 TARGET 掩码提取控制器和目标字段 */
    config->controller = (DL_SPGSS_PAIR_CONTROLLER_SEL)(*(pReg + (uint32_t) index) & SPGSS_PAIR0_CTL_MASK);
    config->target = (DL_SPGSS_PAIR_TARGET_SEL)(*(pReg + (uint32_t) index) & SPGSS_PAIR0_TARGET_MASK);
}

/**
 *  @brief      Enable SPGSS interrupt 使能 SPGSS 中断
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *                             中断位掩码，可按位或组合
 */
__STATIC_INLINE void DL_SPGSS_enableInterrupt(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    spgss->CPU_INT.IMASK |= (interruptMask);
}

/**
 *  @brief      Disable SPGSS interrupt 禁用 SPGSS 中断
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *                             中断位掩码，可按位或组合
 *
 */
__STATIC_INLINE void DL_SPGSS_disableInterrupt(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    spgss->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which SPGSS interrupts are enabled
 *              查询哪些 SPGSS 中断已使能
 *
 *  @param[in]  spgss          Pointer to the register overlay for the
 *                             peripheral
 *                             外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *                             中断位掩码，可按位或组合
 *
 *  @return     Which of the requested SPGSS interrupts are enabled
 *              返回已使能的中断标志位
 *
 *  @retval     Bitwise OR of @ref DL_SPGSS_INTERRUPTS values
 */
__STATIC_INLINE uint32_t DL_SPGSS_getEnabledInterrupts(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    return (spgss->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled SPGSS interrupt
 *              检查已使能的 SPGSS 中断是否挂起
 *
 *  Checks if the SPGSS interrupt that was previously enabled is pending.
 *  查询之前已使能的 SPGSS 中断是否有待处理请求。
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *                             中断位掩码，可按位或组合
 *
 *  @return     If the enabled SPGSS interrupt is pending
 *              返回已使能且挂起的中断标志位
 *
 *  @sa         DL_SPGSS_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_SPGSS_getEnabledInterruptStatus(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    /* MIS = IMASK & RIS，仅返回已使能且挂起的中断 */
    return (spgss->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any SPGSS interrupt
 *              检查任意 SPGSS 中断的原始挂起状态
 *
 *  Checks if the SPGSS interrupt is pending. Interrupt does not have to
 *  be previously enabled.
 *  查询中断是否挂起，无需事先使能对应中断。
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *                             中断位掩码，可按位或组合
 *
 *  @return     If the SPGSS interrupt is pending
 *              返回原始中断挂起标志位
 *
 */
__STATIC_INLINE uint32_t DL_SPGSS_getRawInterruptStatus(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    return (spgss->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Clear pending SPGSS interrupt 清除挂起的 SPGSS 中断
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *                     外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *                             中断位掩码，可按位或组合
 *
 */
__STATIC_INLINE void DL_SPGSS_clearInterruptStatus(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    /* 写 1 清除对应中断标志 */
    spgss->CPU_INT.ICLR |= (interruptMask);
}

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_SPGSS__ */

#endif /* ti_dl_dl_spgss__include */
/** @}*/
