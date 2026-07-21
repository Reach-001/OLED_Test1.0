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
 *  @file       dl_interrupt.h
 *  @brief      Interrupt Management Driver Library / 中断管理驱动库
 *  @defgroup   INTERRUPT M0P Interrupts
 *
 *  @anchor ti_dl_m0p_dl_interrupt_Overview
 *  # Overview
 *
 *  The Cortex-M0+ architecture is limited to 32 device interrupts. To enable
 *  interrupt handling on devices that need more than 32 interrupts, the
 *  interrupts are grouped together.
 *
 *  For example, "Group 0" contains the interrupt flags for the WWDT0, WWDT1,
 *  DEBUGSS, FLASH, and SYSCTL peripherals. Once the interrupt fires for Group 0,
 *  the application must check to see which peripheral from Group 0 was the
 *  source using the APIs in this module.
 *
 *  This PI module is not for controlling the NVIC or the I-bit in the CPSR
 *  register. It's best to use the CMSIS-Core APIs that are delivered as part
 *  of CMSIS-Core in the `source/third_party/CMSIS/Core/Include/core_cm0plus.h`
 *  file within the SDK.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup INTERRUPT
 * @{
 */
#ifndef ti_dl_m0p_dl_interrupt__include
#define ti_dl_m0p_dl_interrupt__include

#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/devices/msp/peripherals/m0p/hw_cpuss.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* clang-format off */

/** @addtogroup DL_INTERRUPT_GROUP_IIDX
 *  @{
 */

/*!
 *  @brief WWDT0 Interrupt in Group 0
 *         第0组 WWDT0 中断
 */
#define DL_INTERRUPT_GROUP0_IIDX_WWDT0          (CPUSS_INT_GROUP_IIDX_STAT_INT0)

/*!
 *  @brief WWDT1 Interrupt in Group 0.
 *         第0组 WWDT1 中断
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 0 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第0组。
 */
#define DL_INTERRUPT_GROUP0_IIDX_WWDT1          (CPUSS_INT_GROUP_IIDX_STAT_INT1)

/*!
 *  @brief DEBUGSS Interrupt in Group 0
 *         第0组 DEBUGSS 中断
 */
#define DL_INTERRUPT_GROUP0_IIDX_DEBUGSS        (CPUSS_INT_GROUP_IIDX_STAT_INT2)

/*!
 *  @brief FLASH Interrupt in Group 0
 *         第0组 FLASH 中断
 */
#define DL_INTERRUPT_GROUP0_IIDX_FLASH          (CPUSS_INT_GROUP_IIDX_STAT_INT3)

/*!
 *  @brief WUC FSUB0 Interrupt in Group 0
 *         第0组 WUC FSUB0 中断
 */
#define DL_INTERRUPT_GROUP0_IIDX_WUC_FSUB0      (CPUSS_INT_GROUP_IIDX_STAT_INT4)

/*!
 *  @brief WUC FSUB1 Interrupt in Group 0
 *         第0组 WUC FSUB1 中断
 */
#define DL_INTERRUPT_GROUP0_IIDX_WUC_FSUB1      (CPUSS_INT_GROUP_IIDX_STAT_INT5)

/*!
 *  @brief SYSCTL (PMCU) Interrupt in Group 0
 *         第0组 SYSCTL (PMCU) 中断
 */
#define DL_INTERRUPT_GROUP0_IIDX_SYSCTL         (CPUSS_INT_GROUP_IIDX_STAT_INT6)

/*!
 *  @brief GPIOA Interrupt in Group 1
 *         第1组 GPIOA 中断
 */
#define DL_INTERRUPT_GROUP1_IIDX_GPIOA          (CPUSS_INT_GROUP_IIDX_STAT_INT0)

/*!
 *  @brief GPIOB Interrupt in Group 1
 *         第1组 GPIOB 中断
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_IIDX_GPIOB          (CPUSS_INT_GROUP_IIDX_STAT_INT1)

/*!
 *  @brief COMP0 Interrupt in Group 1
 *         第1组 COMP0 中断
 */
#define DL_INTERRUPT_GROUP1_IIDX_COMP0          (CPUSS_INT_GROUP_IIDX_STAT_INT2)

/*!
 *  @brief COMP1 Interrupt in Group 1
 *         第1组 COMP1 中断
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_IIDX_COMP1          (CPUSS_INT_GROUP_IIDX_STAT_INT3)

/*!
 *  @brief COMP2 Interrupt in Group 1
 *         第1组 COMP2 中断
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_IIDX_COMP2          (CPUSS_INT_GROUP_IIDX_STAT_INT4)

/*!
 *  @brief TRNG Interrupt in Group 1
 *         第1组 TRNG 中断
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_IIDX_TRNG           (CPUSS_INT_GROUP_IIDX_STAT_INT5)

/*!
 *  @brief GPIOC Interrupt in Group 1
 *         第1组 GPIOC 中断
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_IIDX_GPIOC          (CPUSS_INT_GROUP_IIDX_STAT_INT6)

/** @}*/

/** @addtogroup DL_INTERRUPT_GROUP_INTERRUPTS
 *  @{
 */
/*!
 *  @brief WWDT0 Interrupt in Group 0
 *         第0组 WWDT0 中断掩码位
 */
#define DL_INTERRUPT_GROUP0_WWDT0               (CPUSS_INT_GROUP_IMASK_INT_INT0)
/*!
 *  @brief WWDT1 Interrupt in Group 0
 *         第0组 WWDT1 中断掩码位
 */
#define DL_INTERRUPT_GROUP0_WWDT1               (CPUSS_INT_GROUP_IMASK_INT_INT1)
/*!
 *  @brief DEBUGSS Interrupt in Group 0
 *         第0组 DEBUGSS 中断掩码位
 */
#define DL_INTERRUPT_GROUP0_DEBUGSS             (CPUSS_INT_GROUP_IMASK_INT_INT2)
/*!
 *  @brief FLASH Interrupt in Group 0
 *         第0组 FLASH 中断掩码位
 */
#define DL_INTERRUPT_GROUP0_FLASH               (CPUSS_INT_GROUP_IMASK_INT_INT3)
/*!
 *  @brief Generic event subscriber 0 in Group 0
 *         第0组通用事件订阅者0
 */
#define DL_INTERRUPT_GROUP0_WUC_FSUB0           (CPUSS_INT_GROUP_IMASK_INT_INT4)
/*!
 *  @brief Generic event subscriber 1 in Group 0
 *         第0组通用事件订阅者1
 */
#define DL_INTERRUPT_GROUP0_WUC_FSUB1           (CPUSS_INT_GROUP_IMASK_INT_INT5)
/*!
 *  @brief PMCU (system controller) in Group 0
 *         第0组 PMCU（系统控制器）中断掩码位
 */
#define DL_INTERRUPT_GROUP0_PMCU                (CPUSS_INT_GROUP_IMASK_INT_INT6)

/*!
 *  @brief GPIOA Interrupt in Group 1
 *         第1组 GPIOA 中断掩码位
 */
#define DL_INTERRUPT_GROUP1_GPIOA               (CPUSS_INT_GROUP_IMASK_INT_INT0)
/*!
 *  @brief GPIOB Interrupt in Group 1
 *         第1组 GPIOB 中断掩码位
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_GPIOB               (CPUSS_INT_GROUP_IMASK_INT_INT1)
/*!
 *  @brief COMP0 Interrupt in Group 1
 *         第1组 COMP0 中断掩码位
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_COMP0               (CPUSS_INT_GROUP_IMASK_INT_INT2)
/*!
 *  @brief COMP1 Interrupt in Group 1
 *         第1组 COMP1 中断掩码位
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_COMP1               (CPUSS_INT_GROUP_IMASK_INT_INT3)
/*!
 *  @brief COMP2 Interrupt in Group 1
 *         第1组 COMP2 中断掩码位
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_COMP2               (CPUSS_INT_GROUP_IMASK_INT_INT4)
/*!
 *  @brief TRNG Interrupt in Group 1
 *         第1组 TRNG 中断掩码位
 *  @note This is a device specific interrupt. Please consult the device TRM to
 *        determine if the interrupt is enabled as part of Group 1 interrupts.
 *        这是设备特定中断，请查阅设备TRM确认该中断是否属于第1组。
 */
#define DL_INTERRUPT_GROUP1_TRNG                (CPUSS_INT_GROUP_IMASK_INT_INT5)

/** @}*/

    /* clang-format on */

    /*! @enum DL_INTERRUPT_GROUP 中断组选择枚举 */
    typedef enum
    {
        /*! Selects interrupt group 0 *//* 选择中断组0 */
        DL_INTERRUPT_GROUP_0 = 0,
        /*! Selects interrupt group 1 *//* 选择中断组1 */
        DL_INTERRUPT_GROUP_1 = 1
    } DL_INTERRUPT_GROUP;

    /**
     *  @brief      Device default Handler
     *
     *  This function is defined as part of the device startup file.
     *
     *  @par 中文说明
     *  设备默认中断处理函数，通常用于未注册中断的兜底入口。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  无返回值。
     */
    extern void Default_Handler(void);

    /**
     *  @brief      Checks interrupt flag status in the selected group of interrupts
     *
     *  Checks if any of the interrupts in the group are pending. When calling the
     *  function, ensure that the interrupts passed in the mask are applicable to
     *  the requested interrupt group.
     *
     *  @param[in]  group          Group of interrupts to check.
     *                             @ref DL_INTERRUPT_GROUP
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_INTERRUPT_GROUP_INTERRUPTS.
     *
     *  @return     Which of the requested group interrupts are pending
     *
     *  @retval     Bitwise OR of @ref DL_INTERRUPT_GROUP_INTERRUPTS values
     *
     *  @par 中文说明
     *  读取指定中断组内的挂起状态位。
     *
     *  @par 中文参数
     *  - group：目标中断组。
     *  - interruptMask：待检查中断位掩码。
     *
     *  @par 中文返回值
     *  返回请求掩码内当前处于挂起状态的中断位。
     */
    static inline uint32_t DL_Interrupt_getStatusGroup(
        DL_INTERRUPT_GROUP group, uint32_t interruptMask)
    {
        return (CPUSS->INT_GROUP[(uint32_t)group].RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority interrupt pending in the selected interrupt
     *              group
     *
     *  @param[in]  group  Group of interrupts to check
     *
     *  @return     The highest priority pending interrupt for the group requested.
     *              One of @ref DL_INTERRUPT_GROUP_IIDX
     *
     *  @par 中文说明
     *  获取指定中断组当前最高优先级的挂起中断索引。
     *
     *  @par 中文参数
     *  - group：目标中断组。
     *
     *  @par 中文返回值
     *  返回该组的最高优先级挂起中断编号（IIDX）。
     */
    __STATIC_INLINE uint32_t DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP group)
    {
        return (CPUSS->INT_GROUP[group].IIDX);
    }

    /**
     *  @brief      Clear selected interrupt flags in the selected interrupt group
     *
     *  Clears any of the desired interrupts in the group. When calling the
     *  function, ensure that the interrupts passed in the mask are applicable to
     *  the requested interrupt group.
     *
     *  @param[in]  group          Group of interrupts to check.
     *                             @ref DL_INTERRUPT_GROUP
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_INTERRUPT_GROUP_INTERRUPTS.
     *
     *  @par 中文说明
     *  清除指定中断组中的一个或多个中断标志。
     *
     *  @par 中文参数
     *  - group：目标中断组。
     *  - interruptMask：待清除中断位掩码。
     *
     *  @par 中文返回值
     *  无返回值。
     */
    static inline void DL_Interrupt_clearGroup(
        DL_INTERRUPT_GROUP group, uint32_t interruptMask)
    {
        CPUSS->INT_GROUP[(uint32_t)group].ICLR |= interruptMask;
    }

    /**
     *  @brief     Register a function to be called when an interrupt occurs
     *
     *  This allows you to register a function to be called during an interrupt,
     *  allowing you to override the interrupt handler at run time. The function
     *  moves the interrupt vector table from Flash to SRAM so that the interrupt
     *  handler functions can be overwritten.
     *
     *  This function should only be used if the application requires specifying
     *  the interrupt handler at run time. If it does not need to be changed at
     *  run time consider placing the ISR function pointer in the startup file to
     *  link the function into the interrupt table.
     *
     *  Example of usage:
     *  @code
     *  DL_Interrupt_registerInterrupt(TIMA0_INT_IRQn, redirectedInterruptHandler);
     *   @endcode
     *
     *  @param[in] exceptionNumber  The interrupt number to register the handler.
     *                              Interrupt numbers are device specific, please
     *                              consult the device datasheet for valid
     *                              interrupt numbers.
     *  @param[in] intHandler       A pointer to a function to call for the desired
     *                              interrupt
     *
     *  @par 中文说明
     *  注册中断服务函数，并在首次调用时将向量表从 Flash 迁移到 SRAM。
     *
     *  @par 中文参数
     *  - exceptionNumber：设备中断号。
     *  - intHandler：中断触发后执行的处理函数指针。
     *
     *  @par 中文返回值
     *  无返回值。
     */
    void DL_Interrupt_registerInterrupt(
        uint32_t exceptionNumber, void (*intHandler)(void));

    /**
     *  @brief      Unregister a function to be called when an interrupt occurs
     *
     *  Overrides the registered interrupt function to jump into a simple while(1)
     *  loop rather than the previously registered interrupt function. Requires
     *  that @ref DL_Interrupt_registerInterrupt was previously called so that the
     *  interrupt vector table has been moved to RAM.
     *
     *  Example of usage:
     *  @code DL_Interrupt_unregisterInterrupt(TIMA0_INT_IRQn); @endcode
     *
     *  @param[in]  exceptionNumber  The interrupt number to unregister.
     *                               Interrupt numbers are device specific, please
     *                               consult the device datasheet for valid
     *                               interrupt numbers.
     *
     *  @par 中文说明
     *  取消已注册的中断处理函数，并恢复为默认处理入口。
     *
     *  @par 中文参数
     *  - exceptionNumber：需要取消注册的设备中断号。
     *
     *  @par 中文返回值
     *  无返回值。
     */
    void DL_Interrupt_unregisterInterrupt(uint32_t exceptionNumber);

#ifdef __cplusplus
}
#endif

#endif /* ti_dl_m0p_dl_interrupt__include */
/** @}*/
