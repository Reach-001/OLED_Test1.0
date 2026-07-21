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
 *  @file       dl_npu.h
 *  @brief      Neural Processing Unit (NPU) Driver Library / 神经处理单元 (NPU) 驱动库
 *  @defgroup   Neural Processing Unit (NPU)
 *
 *  @anchor ti_dl_dl_npu_Overview
 *  # Overview
 *
 *  The Neural Processing Unit (NPU) Driver Library provides low level
 *  NPU drivers for power control, interrupt management, and access
 *  to NPU control registers as well as NPU memory.
 *  The power control and interrupt management registers are expected to be
 *  called as a part of the user application, but the NPU control register
 *  and NPU memory address accessor functions are only intended to be used by
 *  the NPU library.
 *
 *  <hr>
 ******************************************************************************
 */

/*
 * 文件用途：NPU（神经处理单元）驱动库头文件
 * 提供电源控制、中断管理、NPU 控制寄存器及 NPU 内存的底层访问接口。
 * 电源控制与中断管理函数供用户应用调用；NPU 控制寄存器和内存地址访问
 * 函数仅供 NPU 推理库内部使用。
 */

/** @addtogroup NPU
 * @{
 */
#ifndef ti_dl_dl_npu__include
#define ti_dl_dl_npu__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>

#ifdef __MSPM0_HAS_NPU__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_NPU_INTERRUPT
 *  @{
 */

/*!
 * @brief NPU done interrupt / NPU 完成中断
 * 当 NPU 推理完成时触发此中断
 */
#define DL_NPU_INTERRUPT_DONE (NPU_IMASK_DONE_SET)

/** @}*/

/* clang-format on */

/*! @enum DL_NPU_IIDX / NPU 中断索引枚举 */
typedef enum {
    /*! NPU interrupt index for no interrupt / 无中断 */
    DL_NPU_IIDX_NO_INTERRUPT = NPU_IIDX_STAT_NO_INTR,
    /*! NPU interrupt index for NPU done interrupt / NPU 推理完成中断 */
    DL_NPU_IIDX_OUTPUT_READY = NPU_IIDX_STAT_DONE
} DL_NPU_IIDX;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the NPU
 *        使能 NPU 外设电源写使能 (PWREN) 寄存器
 *
 *  Before any NPU registers can be configured by software, the
 *  NPU itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the NPU's PWREN register.
 *  在软件配置 NPU 寄存器之前，必须先通过写入 ENABLE 位和正确的 KEY 值来使能 NPU。
 *
 * @param npu        Pointer to the register overlay for the NPU
 *                   NPU 寄存器基地址指针
 */
__STATIC_INLINE void DL_NPU_enablePower(NPU_Regs *npu)
{
    // 解锁并使能 NPU 电源
    npu->GPRCM.PWREN = (NPU_PWREN_KEY_UNLOCK_W | NPU_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the NPU
 *        禁用 NPU 外设电源写使能 (PWREN) 寄存器
 *
 *  When the PWREN.ENABLE bit is cleared, the NPU's registers and memory are
 *  not accessible for read/write operations.
 *  清除 PWREN.ENABLE 位后，NPU 的寄存器和内存将不可读写。
 *
 *  @note This API does not provide large power savings.
 *  @note 此 API 不会带来显著的功耗节省。
 *
 * @param npu        Pointer to the register overlay for the NPU
 *                   NPU 寄存器基地址指针
 */
__STATIC_INLINE void DL_NPU_disablePower(NPU_Regs *npu)
{
    // 解锁并禁用 NPU 电源
    npu->GPRCM.PWREN = (NPU_PWREN_KEY_UNLOCK_W | NPU_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the NPU
 *        is enabled
 *        查询 NPU 外设电源写使能 (PWREN) 寄存器是否已使能
 *
 *  Before any NPU registers can be configured by software, the
 *  NPU itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the NPU's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the NPU's registers are not
 *  accessible for read/write operations.
 *
 * @param npu        Pointer to the register overlay for the NPU
 *                   NPU 寄存器基地址指针
 *
 * @return true if NPU register access is enabled / NPU 寄存器可访问时返回 true
 * @return false if NPU register access is disabled / NPU 寄存器不可访问时返回 false
 */
__STATIC_INLINE bool DL_NPU_isPowerEnabled(const NPU_Regs *npu)
{
    // 读取 PWREN 寄存器 ENABLE 字段，判断 NPU 电源是否使能
    return (
        (npu->GPRCM.PWREN & NPU_PWREN_ENABLE_MASK) == NPU_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets NPU peripheral / 复位 NPU 外设
 *
 * @param npu        Pointer to the register overlay for the NPU
 *                   NPU 寄存器基地址指针
 */
__STATIC_INLINE void DL_NPU_reset(NPU_Regs *npu)
{
    // 解锁复位控制寄存器，清除复位粘滞标志并发起复位
    npu->GPRCM.RSTCTL =
        (NPU_RSTCTL_KEY_UNLOCK_W | NPU_RSTCTL_RESETSTKYCLR_CLR |
            NPU_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Returns if NPU was reset / 查询 NPU 是否已复位
 *
 * @param npu        Pointer to the register overlay for the NPU
 *                   NPU 寄存器基地址指针
 *
 * @return true if NPU was reset / NPU 已复位时返回 true
 * @return false if NPU wasn't reset / NPU 未复位时返回 false
 *
 */
__STATIC_INLINE bool DL_NPU_isReset(const NPU_Regs *npu)
{
    // 读取状态寄存器复位粘滞位，判断是否发生过复位
    return ((npu->GPRCM.STAT & NPU_STAT_RESETSTKY_MASK) ==
            NPU_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      Enable NPU interrupts / 使能 NPU 中断
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *                             要使能的中断位掩码，由 @ref DL_NPU_INTERRUPT 按位或组合
 */
__STATIC_INLINE void DL_NPU_enableInterrupt(
    NPU_Regs *npu, uint32_t interruptMask)
{
    // 将指定中断位写入中断屏蔽寄存器，使能对应中断
    npu->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable NPU interrupts / 禁用 NPU 中断
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *                             要禁用的中断位掩码，由 @ref DL_NPU_INTERRUPT 按位或组合
 */
__STATIC_INLINE void DL_NPU_disableInterrupt(
    NPU_Regs *npu, uint32_t interruptMask)
{
    // 清除中断屏蔽寄存器中对应的中断位，禁用指定中断
    npu->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which NPU interrupts are enabled / 查询哪些 NPU 中断已使能
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *                             要查询的中断位掩码，由 @ref DL_NPU_INTERRUPT 按位或组合
 *
 *  @return     Which of the requested NPU interrupts are enabled
 *              已使能的 NPU 中断位
 *
 *  @retval     Bitwise OR of @ref DL_NPU_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_NPU_getEnabledInterrupts(
    NPU_Regs *npu, uint32_t interruptMask)
{
    // 读取中断屏蔽寄存器，返回已使能的中断位
    return (npu->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled NPU interrupts
 *              查询已使能的 NPU 中断标志
 *
 *  Checks if any of the NPU interrupts that were previously enabled are
 *  pending.
 *  检查先前已使能的 NPU 中断是否有挂起。
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *                             要查询的中断位掩码，由 @ref DL_NPU_INTERRUPT 按位或组合
 *
 *  @return     Which of the requested NPU interrupts are pending
 *              挂起的 NPU 中断位
 *
 *  @retval     Bitwise OR of @ref DL_NPU_INTERRUPT values
 *
 *  @sa         DL_NPU_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_NPU_getEnabledInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    // 读取屏蔽后中断状态寄存器 (MIS)，返回已使能且挂起的中断位
    return (npu->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any NPU interrupt
 *              查询任意 NPU 中断的原始状态
 *
 *  Checks if any of the NPU interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *  检查 NPU 中断是否有挂起，无需事先使能中断。
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *                             要查询的中断位掩码，由 @ref DL_NPU_INTERRUPT 按位或组合
 *
 *  @return     Which of the requested NPU interrupts are pending
 *              挂起的 NPU 中断位
 *
 *  @retval     Bitwise OR of @ref DL_NPU_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_NPU_getRawInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    // 读取原始中断状态寄存器 (RIS)，返回所有挂起的中断位（不考虑屏蔽）
    return (npu->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending NPU interrupt
 *              获取最高优先级的挂起 NPU 中断
 *
 *  Checks if any of the NPU interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *  检查是否有 NPU 中断挂起，无需事先使能中断。
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @return     The highest priority pending NPU interrupt. One of @ref
 *              DL_NPU_IIDX
 *              最高优先级的挂起 NPU 中断，取值为 @ref DL_NPU_IIDX 之一
 */
__STATIC_INLINE DL_NPU_IIDX DL_NPU_getPendingInterrupt(NPU_Regs *npu)
{
    // 读取中断索引寄存器 (IIDX)，返回最高优先级挂起中断的索引值
    return (DL_NPU_IIDX)(npu->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending NPU interrupts / 清除挂起的 NPU 中断
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *                             要清除的中断位掩码，由 @ref DL_NPU_INTERRUPT 按位或组合
 */
__STATIC_INLINE void DL_NPU_clearInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    // 写入中断清除寄存器 (ICLR)，清除指定的挂起中断标志
    npu->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Get the physical starting address of the NPU instruction memory
 *              获取 NPU 指令存储器的物理起始地址
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @return     Address of the instruction memory / 指令存储器地址
 */
__STATIC_INLINE uint32_t DL_NPU_getInstrMemAddress(const NPU_Regs *npu)
{
    // 返回 DREG20 寄存器地址，即指令存储器的物理起始地址
    return ((uint32_t)(&npu->DREG20));
}

/**
 *  @brief      Get the physical starting address of the NPU parameter memory
 *              获取 NPU 参数存储器的物理起始地址
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @return     Address of the parameter memory / 参数存储器地址
 */
__STATIC_INLINE uint32_t DL_NPU_getParamMemAddress(const NPU_Regs *npu)
{
    // 返回 DREG21 寄存器地址，即参数存储器的物理起始地址
    return ((uint32_t)(&npu->DREG21));
}

/**
 *  @brief      Get the physical starting address of the NPU control registers
 *              获取 NPU 控制寄存器的物理起始地址
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @return     Address of the NPU CTL register / NPU 控制寄存器地址
 */
__STATIC_INLINE uint32_t DL_NPU_getCtlRegAddress(const NPU_Regs *npu)
{
    // 返回 DREG0 寄存器地址，即 NPU 控制寄存器的物理起始地址
    return ((uint32_t)(&npu->DREG0));
}

/**
 *  @brief      Get the physical address of the NPU ACC register
 *              获取 NPU 累加器 (ACC) 寄存器的物理地址
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *                             NPU 寄存器基地址指针
 *
 *  @return     Address of the ACCVAL register / 累加器值寄存器地址
 */
__STATIC_INLINE uint32_t DL_NPU_getAccRegAddress(const NPU_Regs *npu)
{
    // 返回 DREG19 寄存器地址，即 NPU 累加器值寄存器的物理地址
    return ((uint32_t)(&npu->DREG19));
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_NPU__ */

#endif /* ti_dl_dl_npu__include */
/** @}*/
