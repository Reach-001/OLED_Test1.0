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
 *  @file       dl_vref.h
 *  @brief      Voltage Reference (VREF) Driver Library
 *              电压基准 (VREF) 驱动库
 *  @defgroup   VREF Voltage Reference (VREF)
 *
 *  @anchor ti_dl_dl_vref_Overview
 *  # Overview
 *
 *  The Voltage Reference Driver Library allows full configuration of
 *  the MSPM0 VREF module.
 *  The VREF module contains a configurable voltage reference buffer which
 *  allows users to supply a stable internal reference to on-board analog
 *  peripherals. It also supports bringing in an external reference for
 *  applications where higher accuracy is required.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup VREF
 * @{
 */
#ifndef ti_dl_dl_m0p_vref__include
#define ti_dl_dl_m0p_vref__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_VREF__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_VREF_CTL1_READY
 * @{
 */

/*!
* @brief VREF bit in CTL1 indicating the module is ready is not set
* VREF CTL1 寄存器中指示模块未就绪
*/
#define DL_VREF_CTL1_READY_NOTRDY    (VREF_CTL1_READY_NOTRDY)

/*!
* @brief VREF bit in CTL1 indicating the module is ready is set
* VREF CTL1 寄存器中指示模块已就绪
*/
#define DL_VREF_CTL1_READY_RDY    (VREF_CTL1_READY_RDY)


/** @}*/

/** @addtogroup DL_VREF_SH
 * @{
 */

/*!
* @brief Minimum value for VREF sample and hold, if enabled
* VREF 采样保持的最小周期值（启用时生效）
*/
#define DL_VREF_SH_MIN             (VREF_CTL2_SHCYCLE_MINIMUM)

/*!
* @brief Maximum value for VREF sample and hold, if enabled
* VREF 采样保持的最大周期值（启用时生效）
*/
#define DL_VREF_SH_MAX             (VREF_CTL2_SHCYCLE_MAXIMUM)

/** @}*/

/** @addtogroup DL_VREF_HOLD
 * @{
 */

/*!
* @brief Minimum value for VREF hold, if enabled
* VREF 保持的最小周期值（启用时生效）
*/
#define DL_VREF_HOLD_MIN             (0x0000U)

/*!
* @brief Maximum value for VREF hold, if enabled
* VREF 保持的最大周期值（启用时生效）
*/
#define DL_VREF_HOLD_MAX             (0xFFFFU)

/** @}*/

/* clang-format on */

/*! @enum DL_VREF_CLOCK VREF 时钟源选择 */
typedef enum {
    /*! Set VREF clock source as LFCLK  设置 VREF 时钟源为低频时钟 (LFCLK) */
    DL_VREF_CLOCK_LFCLK = VREF_CLKSEL_LFCLK_SEL_MASK,
    /*! Set VREF clock source as MFCLK  设置 VREF 时钟源为中频时钟 (MFCLK) */
    DL_VREF_CLOCK_MFCLK = VREF_CLKSEL_MFCLK_SEL_MASK,
    /*! Set VREF clock source as BUSCLK  设置 VREF 时钟源为总线时钟 (BUSCLK) */
    DL_VREF_CLOCK_BUSCLK = VREF_CLKSEL_BUSCLK_SEL_MASK,
} DL_VREF_CLOCK;

/*! @enum DL_VREF_CLOCK_DIVIDE VREF 时钟分频系数 */
typedef enum {
    /*! Clock divide by 1  不分频 */
    DL_VREF_CLOCK_DIVIDE_1 = 0,
    /*! Clock divide by 2  2分频 */
    DL_VREF_CLOCK_DIVIDE_2 = 1,
    /*! Clock divide by 4  4分频 */
    DL_VREF_CLOCK_DIVIDE_4 = 3,
    /*! Clock divide by 6  6分频 */
    DL_VREF_CLOCK_DIVIDE_6 = 5,
    /*! Clock divide by 8  8分频 */
    DL_VREF_CLOCK_DIVIDE_8 = 7,
} DL_VREF_CLOCK_DIVIDE;

/*! @enum DL_VREF_ENABLE VREF 使能控制 */
typedef enum {
    /*! VREF control set disabled  禁用 VREF */
    DL_VREF_ENABLE_DISABLE = VREF_CTL0_ENABLE_DISABLE,
    /*! VREF control set enabled  使能 VREF */
    DL_VREF_ENABLE_ENABLE = VREF_CTL0_ENABLE_ENABLE,
} DL_VREF_ENABLE;

/*! @enum DL_VREF_BUFCONFIG VREF 输出缓冲区电压配置 */
typedef enum {
    /*! Configure VREF output buffer to 2.5V  配置 VREF 输出缓冲为 2.5V */
    DL_VREF_BUFCONFIG_OUTPUT_2_5V = VREF_CTL0_BUFCONFIG_OUTPUT2P5V,
    /*! Configure VREF output buffer to 1.4V  配置 VREF 输出缓冲为 1.4V */
    DL_VREF_BUFCONFIG_OUTPUT_1_4V = VREF_CTL0_BUFCONFIG_OUTPUT1P4V,
} DL_VREF_BUFCONFIG;

/*! @enum DL_VREF_SHMODE VREF 采样保持模式 */
typedef enum {
    /*! Set VREF sample and hold mode to disabled  禁用采样保持模式 */
    DL_VREF_SHMODE_DISABLE = VREF_CTL0_SHMODE_DISABLE,
    /*! Set VREF sample and hold mode to enabled  使能采样保持模式 */
    DL_VREF_SHMODE_ENABLE = VREF_CTL0_SHMODE_ENABLE,
} DL_VREF_SHMODE;

/*!
 *  @brief  Configuration struct for @ref DL_VREF_setClockConfig.
 *  @ref DL_VREF_setClockConfig 的时钟配置结构体
 */
typedef struct {
    /*! Selects VREF module clock source @ref DL_VREF_CLOCK
     *  VREF 模块时钟源选择 */
    DL_VREF_CLOCK clockSel;
    /*! Selects the divide ratio. One of @ref DL_VREF_CLOCK_DIVIDE
     *  时钟分频系数 */
    DL_VREF_CLOCK_DIVIDE divideRatio;
} DL_VREF_ClockConfig;

/**
 * @brief Configuration struct for @ref DL_VREF_Config.
 * VREF 参考电压配置结构体
 */
typedef struct {
    /*! VREF enable setting. Either @ref DL_VREF_ENABLE_DISABLE or @ref DL_VREF_ENABLE_ENABLE
     *  VREF 使能设置 */
    DL_VREF_ENABLE vrefEnable;
    /*! VREF buffer configuration setting. Either @ref DL_VREF_BUFCONFIG_OUTPUT_1_4V or @ref DL_VREF_BUFCONFIG_OUTPUT_2_5V
     *  VREF 输出缓冲电压配置 */
    DL_VREF_BUFCONFIG bufConfig;
    /*! VREF sample and hold enable configuration setting. Either @ref DL_VREF_SHMODE_DISABLE or @ref DL_VREF_SHMODE_ENABLE
     *  VREF 采样保持模式使能配置 */
    DL_VREF_SHMODE shModeEnable;
    /*! Number of cycles to sample and hold for, if sample and hold mode is enabled. Minimum @ref DL_VREF_SH_MIN, maximum @ref DL_VREF_SH_MAX
     *  采样保持周期数，需在 DL_VREF_SH_MIN 到 DL_VREF_SH_MAX 之间 */
    uint16_t shCycleCount;
    /*! Number of cycles to hold for, if sample and hold mode is enabled. Minimum @ref DL_VREF_HOLD_MIN, maximum @ref DL_VREF_HOLD_MAX
     *  保持周期数，需在 DL_VREF_HOLD_MIN 到 DL_VREF_HOLD_MAX 之间 */
    uint16_t holdCycleCount;
} DL_VREF_Config;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the VREF
 * 使能 VREF 外设电源写使能 (PWREN) 寄存器
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *  在软件配置任何外设寄存器之前，需先向 PWREN 寄存器写入 KEY 和 ENABLE 位来使能外设。
 *
 *  @note For power savings, please refer to @ref DL_VREF_enableInternalRef
 *  @note 如需节省功耗，请参考 @ref DL_VREF_enableInternalRef
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_enablePower(VREF_Regs *vref)
{
    vref->GPRCM.PWREN = VREF_PWREN_KEY_UNLOCK_W | VREF_PWREN_ENABLE_ENABLE;
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the VREF
 * 禁用 VREF 外设电源写使能 (PWREN) 寄存器
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *  清除 PWREN.ENABLE 位后，外设寄存器将不可读写。
 *
 *  @note This API does not provide large power savings. For power savings,
 *  please refer to @ref DL_VREF_disableInternalRef
 *  @note 此 API 节省的功耗有限。如需更多省电，请参考 @ref DL_VREF_disableInternalRef
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_disablePower(VREF_Regs *vref)
{
    vref->GPRCM.PWREN = VREF_PWREN_KEY_UNLOCK_W | VREF_PWREN_ENABLE_DISABLE;
}
/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the VREF
 *        is enabled
 * 查询 VREF 外设电源写使能 (PWREN) 寄存器是否已使能
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *  在软件配置任何外设寄存器之前，需先向 PWREN 寄存器写入 KEY 和 ENABLE 位来使能外设。
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *  清除 PWREN.ENABLE 位后，外设寄存器将不可读写。
 *
 * @param vref        Pointer to the register overlay for the peripheral
 *                    VREF 外设寄存器基地址指针
 *
 * @return true if peripheral register access is enabled  外设寄存器访问已使能
 * @return false if peripheral register access is disabled  外设寄存器访问已禁用
 */
__STATIC_INLINE bool DL_VREF_isPowerEnabled(VREF_Regs *vref)
{
    return ((vref->GPRCM.PWREN & VREF_PWREN_ENABLE_MASK) ==
            VREF_PWREN_ENABLE_ENABLE);
}

/**
 *  @brief      Enables Internal VREF
 *  使能内部 VREF 参考电压
 *
 *  This function only enables the internal VREF but doesn't configure all
 *  settings. It's recommended to use @ref DL_VREF_configReference to configure
 *  and enable VREF with all parameters.
 *  此函数仅使能内部 VREF，不会配置其他参数。建议使用 @ref DL_VREF_configReference
 *  一次性配置并使能 VREF。
 *
 *  @param vref       Pointer to the register overlay for the peripheral
 *                    VREF 外设寄存器基地址指针
 *
 */
__STATIC_INLINE void DL_VREF_enableInternalRef(VREF_Regs *vref)
{
    vref->CTL0 |= VREF_CTL0_ENABLE_ENABLE;
}

/**
 *  @brief      Disables Internal VREF, allows for External VREF
 *  禁用内部 VREF，以允许使用外部 VREF 输入
 *
 *  The internal VREF must be disabled when using external VREF input.
 *  使用外部 VREF 输入时，必须先禁用内部 VREF。
 *
 *  @param vref       Pointer to the register overlay for the peripheral
 *                    VREF 外设寄存器基地址指针
 *
 */
__STATIC_INLINE void DL_VREF_disableInternalRef(VREF_Regs *vref)
{
    vref->CTL0 &= ~VREF_CTL0_ENABLE_MASK;
}

/**
 *  @brief      Checks if the internal VREF is enabled
 *  检查内部 VREF 是否已使能
 *
 *  @param vref       Pointer to the register overlay for the peripheral
 *                    VREF 外设寄存器基地址指针
 *
 *  @return     Returns the enabled status of the vref peripheral
 *              返回 VREF 外设使能状态
 *
 *  @retval     true  The internal VREF is enabled  内部 VREF 已使能
 *  @retval     false The internal VREF is disabled, external VREF can be used  内部 VREF 已禁用，可使用外部 VREF
 */
__STATIC_INLINE bool DL_VREF_isEnabled(const VREF_Regs *vref)
{
    return ((vref->CTL0 & VREF_CTL0_ENABLE_MASK) == VREF_CTL0_ENABLE_ENABLE);
}

#if defined(VREF_SYS_INT_COMP_REF) && (VREF_SYS_INT_COMP_REF == 1)
/**
 *  @brief      Enable VREF buffer as internal reference input for comparator
 *  使能 VREF 缓冲区作为比较器的内部参考输入
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_enableInternalRefCOMP(VREF_Regs *vref)
{
    vref->CTL0 |= VREF_CTL0_COMP_VREF_ENABLE_ENABLE;
}

/**
 *  @brief      Checks if VREF buffer for comparator is enabled
 *  检查比较器的 VREF 缓冲区是否已使能
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 *
 *  @return     Returns if VREF buffer for comparator is enabled
 *              返回比较器 VREF 缓冲区使能状态
 *
 *  @retval     true  VREF buffer for comparator is enabled  比较器 VREF 缓冲区已使能
 *  @retval     false VREF buffer for comparator is disabled  比较器 VREF 缓冲区已禁用
 */
__STATIC_INLINE bool DL_VREF_isInternalRefCOMPEnabled(const VREF_Regs *vref)
{
    return ((vref->CTL0 & VREF_CTL0_COMP_VREF_ENABLE_MASK) ==
            VREF_CTL0_COMP_VREF_ENABLE_ENABLE);
}

/**
 *  @brief      Disable VREF buffer as internal reference input for comparator
 *  禁用 VREF 缓冲区作为比较器的内部参考输入
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_disableInternalRefCOMP(VREF_Regs *vref)
{
    vref->CTL0 &= ~(VREF_CTL0_COMP_VREF_ENABLE_MASK);
}
#endif

#if defined(VREF_SYS_INT_ADC_REF) && (VREF_SYS_INT_ADC_REF == 1)
/**
 *  @brief      Enable VREF buffer as internal reference input for ADC
 *  使能 VREF 缓冲区作为 ADC 的内部参考输入
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_enableInternalRefADC(VREF_Regs *vref)
{
    vref->CTL0 |= VREF_CTL0_ADC_VREF_ENABLE_ENABLE;
}

/**
 *  @brief      Checks if VREF buffer for ADC is enabled
 *  检查 ADC 的 VREF 缓冲区是否已使能
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 *
 *  @return     Returns if VREF buffer for ADC is enabled
 *              返回 ADC VREF 缓冲区使能状态
 *
 *  @retval     true  VREF buffer for ADC is enabled  ADC VREF 缓冲区已使能
 *  @retval     false VREF buffer for ADC is disabled  ADC VREF 缓冲区已禁用
 */
__STATIC_INLINE bool DL_VREF_isInternalRefADCEnabled(const VREF_Regs *vref)
{
    return ((vref->CTL0 & VREF_CTL0_ADC_VREF_ENABLE_MASK) ==
            VREF_CTL0_ADC_VREF_ENABLE_ENABLE);
}

/**
 *  @brief      Disable VREF buffer as internal reference input for ADC
 *  禁用 VREF 缓冲区作为 ADC 的内部参考输入
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_disableInternalRefADC(VREF_Regs *vref)
{
    vref->CTL0 &= ~(VREF_CTL0_ADC_VREF_ENABLE_MASK);
}
#endif

/**
 * @brief Set the clock select and clock divide fields in VREF
 * 设置 VREF 的时钟源选择和分频系数
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 *
 * @param config     Pointer to the configuration structure
 *                   时钟配置结构体指针
 */
void DL_VREF_setClockConfig(
    VREF_Regs *vref, const DL_VREF_ClockConfig *config);

/**
 * @brief Copy the clock select and clock divide fields in VREF to config
 * 读取 VREF 的时钟源选择和分频系数到配置结构体
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 *
 * @param config     Pointer to the configuration structure
 *                   时钟配置结构体指针（输出参数）
 */
void DL_VREF_getClockConfig(
    const VREF_Regs *vref, DL_VREF_ClockConfig *config);

/**
 * @brief Resets the VREF module
 * 复位 VREF 模块
 *
 * @param vref       Pointer to the register overlay for the peripheral
 *                   VREF 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_VREF_reset(VREF_Regs *vref)
{
    vref->GPRCM.RSTCTL =
        VREF_RSTCTL_RESETASSERT_MASK | VREF_RSTCTL_KEY_UNLOCK_W;
}

/**
 * @brief Returns if VREF peripheral was reset
 * 查询 VREF 外设是否已复位
 *
 * @param vref        Pointer to the register overlay for the VREF peripheral
 *                    VREF 外设寄存器基地址指针
 *
 * @return true if peripheral was reset  外设已复位
 * @return false if peripheral wasn't reset  外设未复位
 *
 */
__STATIC_INLINE bool DL_VREF_isReset(const VREF_Regs *vref)
{
    return (vref->GPRCM.STAT & VREF_STAT_RESETSTKY_MASK) ==
           VREF_STAT_RESETSTKY_RESET;
}

/**
 * @brief Returns VREF register status
 * 返回 VREF 寄存器状态
 *
 * @param vref        Pointer to the register overlay for the VREF peripheral
 *                    VREF 外设寄存器基地址指针
 *
 * @return VREF status bits set. Either @ref DL_VREF_CTL1_READY_NOTRDY or @ref DL_VREF_CTL1_READY_RDY
 *         VREF 状态位，为 DL_VREF_CTL1_READY_NOTRDY（未就绪）或 DL_VREF_CTL1_READY_RDY（已就绪）
 *
 */
__STATIC_INLINE uint32_t DL_VREF_getStatus(const VREF_Regs *vref)
{
    return vref->CTL1 & VREF_CTL1_READY_MASK;
}

/**
 * @brief Configure VREF enable, control registers
 * 配置 VREF 使能及控制寄存器
 *
 * @param vref        Pointer to the register overlay for the VREF peripheral
 *                    VREF 外设寄存器基地址指针
 *
 * @param config      Pointer to @ref DL_VREF_Config structure to configure the peripheral
 *                    VREF 配置结构体指针
 *
 */
void DL_VREF_configReference(VREF_Regs *vref, const DL_VREF_Config *config);

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_VREF__ */

#endif /* ti_dl_dl_m0p_vref__include */
/** @}*/
