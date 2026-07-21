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
 *  @file       dl_opa.h
 *  @brief      Op-Amp (OPA) Driver Library / 运算放大器(OPA)驱动库
 *  @defgroup   OPA Op-Amp (OPA)
 *
 *  @anchor ti_dl_dl_opa_Overview
 *  # Overview / 概述
 *
 *  The OP-Amp Driver Library allows full configuration of the MSPM0 OPA module.
 *  The OPA is a zero-drift chopper stabilized operational amplifier with a
 *  programmable gain stage.
 *  OPA驱动库允许对MSPM0 OPA模块进行完整配置。OPA是一个零漂移斩波稳定运算放大器，
 *  具有可编程增益级。
 *
 *  <hr>

 ******************************************************************************
 */
/** @addtogroup OPA
 * @{
 */
#ifndef ti_dl_dl_opa__include
#define ti_dl_dl_opa__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_OA__

#ifdef __cplusplus
extern "C" {
#endif

/*! @enum DL_OPA_CHOPPING_MODE 斩波模式选择 */
typedef enum {
    /*! Disable chopping mode / 禁用斩波模式 */
    DL_OPA_CHOPPING_MODE_DISABLE = OA_CFG_CHOP_OFF,
    /*! Enable standard chopping mode / 启用标准斩波模式 */
    DL_OPA_CHOPPING_MODE_STANDARD = OA_CFG_CHOP_ON,
    /*! Enable ADC assisted chopping mode. Requires OPA output to be
     connected to an ADC input with hardware averaging.
     启用ADC辅助斩波模式，需要OPA输出连接到带硬件平均的ADC输入。 */
    DL_OPA_CHOPPING_MODE_ADC_AVERAGING = OA_CFG_CHOP_AVGON,
} DL_OPA_CHOPPING_MODE;

/*! @enum DL_OPA_OUTPUT_PIN_STATE 输出引脚状态 */
typedef enum {

    /*! Output pin is enabled / 输出引脚使能 */
    DL_OPA_OUTPUT_PIN_ENABLED = OA_CFG_OUTPIN_ENABLED,
    /*! Outpin pin is disabled / 输出引脚禁用 */
    DL_OPA_OUTPUT_PIN_DISABLED = OA_CFG_OUTPIN_DISABLED,

} DL_OPA_OUTPUT_PIN_STATE;

/*! @enum DL_OPA_PSEL 同相输入通道选择 */
typedef enum {
    /*! Non-inverting input channel is Open / 同相输入通道开路 */
    DL_OPA_PSEL_OPEN = OA_CFG_PSEL_NC,
    /*!  Non-inverting input is OPAx_IN0+ / 同相输入为OPAx_IN0+ */
    DL_OPA_PSEL_IN0_POS = OA_CFG_PSEL_EXTPIN0,
    /*!  Non-inverting input is OPAx_IN1+ / 同相输入为OPAx_IN1+ */
    DL_OPA_PSEL_IN1_POS = OA_CFG_PSEL_EXTPIN1,
    /*!  Non-inverting input is DAC_OUT / 同相输入为DAC_OUT */
    DL_OPA_PSEL_DAC_OUT = OA_CFG_PSEL_DAC12OUT,
    /*!  Non-inverting input is DAC8.x_OUT / 同相输入为DAC8.x_OUT */
    DL_OPA_PSEL_DAC8_OUT = OA_CFG_PSEL_DAC8OUT,
    /*!  Non-inverting input is VREF / 同相输入为VREF */
    DL_OPA_PSEL_VREF = OA_CFG_PSEL_VREF,
    /*!  Non-inverting input is OPA[x-1]_RTOP / 同相输入为OPA[x-1]_RTOP */
    DL_OPA_PSEL_RTOP = OA_CFG_PSEL_OANM1RTOP,

    /*!  Non-inverting input is GPAMP OUT / 同相输入为GPAMP输出 */
    DL_OPA_PSEL_GPAMP_OUT = OA_CFG_PSEL_GPAMP_OUT_INT,
    /*!  Non-inverting input is GND / 同相输入为GND */
    DL_OPA_PSEL_GND = OA_CFG_PSEL_VSS,

} DL_OPA_PSEL;

/*! @enum DL_OPA_NSEL 反相输入通道选择 */
typedef enum {
    /*! Inverting input channel is Open / 反相输入通道开路 */
    DL_OPA_NSEL_OPEN = OA_CFG_NSEL_NC,
    /*! Inverting input channel is OPAx_IN0- / 反相输入为OPAx_IN0- */
    DL_OPA_NSEL_IN0_NEG = OA_CFG_NSEL_EXTPIN0,
    /*! Inverting input channel is OPAx_IN1- / 反相输入为OPAx_IN1- */
    DL_OPA_NSEL_IN1_NEG = OA_CFG_NSEL_EXTPIN1,
    /*! Inverting input channel is OPA[x-1]_RBOT / 反相输入为OPA[x-1]_RBOT */
    DL_OPA_NSEL_RBOT = OA_CFG_NSEL_OANP1RBOT,
    /*! Inverting input channel is RTAP / 反相输入为RTAP */
    DL_OPA_NSEL_RTAP = OA_CFG_NSEL_OANRTAP,
    /*! Inverting input channel is RTOP / 反相输入为RTOP */
    DL_OPA_NSEL_RTOP = OA_CFG_NSEL_OANRTOP,
    /*! Spare input for inverting channel / 反相通道备用输入 */
    DL_OPA_NSEL_SPARE = OA_CFG_NSEL_SPARE,
} DL_OPA_NSEL;

/*! @enum DL_OPA_MSEL M-MUX输入通道选择 */
typedef enum {
    /*! M-MUX input channel is Open / M-MUX输入通道开路 */
    DL_OPA_MSEL_OPEN = OA_CFG_MSEL_NC,
    /*! M-MUX input channel is OPAx_IN1- / M-MUX输入为OPAx_IN1- */
    DL_OPA_MSEL_IN1_NEG = OA_CFG_MSEL_EXTNPIN1,
    /*! M-MUX input channel is GND / M-MUX输入为GND */
    DL_OPA_MSEL_GND = OA_CFG_MSEL_VSS,
    /*! M-MUX input channel is DAC_OUT / M-MUX输入为DAC_OUT */
    DL_OPA_MSEL_DAC_OUT = OA_CFG_MSEL_DAC12OUT,
    /*! M-MUX input channel is OPA[x-1]_RTOP / M-MUX输入为OPA[x-1]_RTOP */
    DL_OPA_MSEL_RTOP = OA_CFG_MSEL_OANM1RTOP,
} DL_OPA_MSEL;

/*! @enum DL_OPA_GBW 增益带宽积选择 */
typedef enum {
    /*! High gain bandwidth (GBW). See device specific datasheet for values.
     *  高增益带宽积(GBW)，具体值请参考器件数据手册。 */
    DL_OPA_GBW_HIGH = OA_CFGBASE_GBW_HIGHGAIN,
    /*! Low gain bandwidth (GBW). See device specific datasheet for values.
     *  低增益带宽积(GBW)，具体值请参考器件数据手册。 */
    DL_OPA_GBW_LOW = OA_CFGBASE_GBW_LOWGAIN,
} DL_OPA_GBW;

/*! @enum DL_OPA_GAIN 可编程增益级增益选择 */
typedef enum {
    /*! Gain value is Not Valid in Non-inverting and inverting PGA modes
     *  在同相和反相PGA模式下增益值无效 */
    DL_OPA_GAIN_N0_P1 = (((uint32_t) 0x0) << OA_CFG_GAIN_OFS),
    /*! Gain value is -1x in Inverting PGA mode, and 2x in Non-inverting
    * PGA mode
    *  反相PGA模式下增益为-1x，同相PGA模式下增益为2x */
    DL_OPA_GAIN_N1_P2 = (((uint32_t) 0x1) << OA_CFG_GAIN_OFS),
    /*! Gain value is -3x in Inverting PGA mode, and 4x in Non-inverting
    * PGA mode
    *  反相PGA模式下增益为-3x，同相PGA模式下增益为4x */
    DL_OPA_GAIN_N3_P4 = (((uint32_t) 0x2) << OA_CFG_GAIN_OFS),
    /*! Gain value is -7x in Inverting PGA mode, and 8x in Non-inverting
    * PGA mode
    *  反相PGA模式下增益为-7x，同相PGA模式下增益为8x */
    DL_OPA_GAIN_N7_P8 = (((uint32_t) 0x3) << OA_CFG_GAIN_OFS),
    /*! Gain value is -15x in Inverting PGA mode, and 16x in Non-inverting
    * PGA mode
    *  反相PGA模式下增益为-15x，同相PGA模式下增益为16x */
    DL_OPA_GAIN_N15_P16 = (((uint32_t) 0x4) << OA_CFG_GAIN_OFS),
    /*! Gain value is -31x in Inverting PGA mode, and 32x in Non-inverting
      * PGA mode
    *  反相PGA模式下增益为-31x，同相PGA模式下增益为32x */
    DL_OPA_GAIN_N31_P32 = (((uint32_t) 0x5) << OA_CFG_GAIN_OFS),
} DL_OPA_GAIN;

/*!
 *  @brief  Configuration struct for @ref DL_OPA_init
 *  @brief  OPA初始化配置结构体 / DL_OPA_init的配置参数
 */
typedef struct {
    /*! The chopping mode to set. One of @ref DL_OPA_CHOPPING_MODE
     *  斩波模式设置，取值参考 DL_OPA_CHOPPING_MODE */
    DL_OPA_CHOPPING_MODE choppingMode;
    /*! The state of the output pin. One of @ref DL_OPA_OUTPUT_PIN_STATE.
     *  输出引脚状态，取值参考 DL_OPA_OUTPUT_PIN_STATE */
    DL_OPA_OUTPUT_PIN_STATE outputPinState;
    /*! The non-inverting input channel to set. One of @ref DL_OPA_PSEL
     *  同相输入通道选择，取值参考 DL_OPA_PSEL */
    DL_OPA_PSEL pselChannel;
    /*! The inverting input channel to set. One of @ref DL_OPA_NSEL
     *  反相输入通道选择，取值参考 DL_OPA_NSEL */
    DL_OPA_NSEL nselChannel;
    /*! The M-MUX input channel to set. One of @ref DL_OPA_MSEL
     *  M-MUX输入通道选择，取值参考 DL_OPA_MSEL */
    DL_OPA_MSEL mselChannel;
    /*! The gain to set. Value between 0-7.
     *  增益设置，取值0-7 */
    DL_OPA_GAIN gain;
} DL_OPA_Config;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the OPA
 * @brief 使能OPA外设的PWREN寄存器
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *  在软件配置外设寄存器之前，必须先向PWREN寄存器写入ENABLE位和正确的KEY值来使能外设。
 *
 *  @note For power savings, please refer to @ref DL_OPA_enable
 *  @note 如需省电，请参考 DL_OPA_enable
 *
 * @param[in]  opa  Pointer to the register overlay for the peripheral
 * @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_enablePower(OA_Regs *opa)
{
    opa->GPRCM.PWREN = (OA_PWREN_KEY_UNLOCK_W | OA_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the OPA
 * @brief 禁用OPA外设的PWREN寄存器
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *  当PWREN.ENABLE位被清零时，外设寄存器不可读写。
 *
 *  @note This API does not provide large power savings. For power savings,
 *  please refer to @ref DL_OPA_enable
 *  @note 此API省电效果有限，如需省电请参考 DL_OPA_enable
 *
 * @param[in]  opa  Pointer to the register overlay for the peripheral
 * @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_disablePower(OA_Regs *opa)
{
    opa->GPRCM.PWREN = (OA_PWREN_KEY_UNLOCK_W | OA_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the OPA
 *        is enabled
 * @brief 检查OPA外设的PWREN寄存器是否已使能
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 * @param[in]  opa  Pointer to the register overlay for the peripheral
 * @param[in]  opa  外设寄存器基地址指针
 *
 * @return true if peripheral register access is enabled / 外设寄存器可访问时返回true
 * @return false if peripheral register access is disabled / 外设寄存器不可访问时返回false
 */
__STATIC_INLINE bool DL_OPA_isPowerEnabled(const OA_Regs *opa)
{
    return (
        (opa->GPRCM.PWREN & OA_PWREN_ENABLE_MASK) == OA_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets OPA peripheral
 * @brief 复位OPA外设
 *
 * @param[in]  opa  Pointer to the register overlay for the peripheral
 * @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_reset(OA_Regs *opa)
{
    opa->GPRCM.RSTCTL = (OA_RSTCTL_KEY_UNLOCK_W | OA_RSTCTL_RESETSTKYCLR_CLR |
                         OA_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Returns if OPA peripheral was reset
 * @brief 检查OPA外设是否已复位
 *
 * @param[in]  opa  Pointer to the register overlay for the peripheral
 * @param[in]  opa  外设寄存器基地址指针
 *
 * @return true if peripheral was reset / 外设已复位时返回true
 * @return false if peripheral wasn't reset / 外设未复位时返回false
 *
 */
__STATIC_INLINE bool DL_OPA_isReset(const OA_Regs *opa)
{
    return ((opa->GPRCM.STAT & OA_GPRCM_STAT_RESETSTKY_MASK) ==
            OA_GPRCM_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      Enable the OPA peripheral
 *  @brief      使能OPA外设
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_enable(OA_Regs *opa)
{
    opa->CTL |= OA_CTL_ENABLE_ON;
}

/**
 *  @brief      Checks if the OPA peripheral is enabled
 *  @brief      检查OPA外设是否已使能
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 *
 *  @return     Returns the enabled status of the OPA
 *  @return     返回OPA的使能状态
 *
 *  @retval     true  The OPA peripheral is enabled / OPA外设已使能
 *  @retval     false The OPA peripheral is disabled / OPA外设已禁用

 */
__STATIC_INLINE bool DL_OPA_isEnabled(const OA_Regs *opa)
{
    return ((opa->CTL & OA_CTL_ENABLE_MASK) == OA_CTL_ENABLE_ON);
}

/**
 *  @brief      Disable the OPA peripheral
 *  @brief      禁用OPA外设
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_disable(OA_Regs *opa)
{
    opa->CTL &= ~(OA_CTL_ENABLE_MASK);
}

/**
 *  @brief      Initialize the OPA peripheral
 *  @brief      初始化OPA外设
 *
 *  Initializes all the common configurable options for the OPA peripheral. Any
 *  other custom configuration can be done after calling this API. The OPA is
 *  not enabled in this API.
 *  初始化OPA外设的所有通用配置选项。调用此API后可进行其他自定义配置。此API不会使能OPA。
 *
 *  @param[in]  opa    Pointer to the register overlay for the peripheral
 *  @param[in]  opa    外设寄存器基地址指针
 *  @param[in]  config  Configuration for OPA peripheral
 *  @param[in]  config  OPA外设配置参数
 */
__STATIC_INLINE void DL_OPA_init(OA_Regs *opa, const DL_OPA_Config *config)
{
    DL_Common_updateReg(&opa->CFG,
        (uint32_t) config->choppingMode | (uint32_t) config->outputPinState |
            (uint32_t) config->pselChannel | (uint32_t) config->nselChannel |
            (uint32_t) config->mselChannel | (uint32_t) config->gain,
        OA_CFG_CHOP_MASK | OA_CFG_OUTPIN_MASK | OA_CFG_PSEL_MASK |
            OA_CFG_NSEL_MASK | OA_CFG_MSEL_MASK | OA_CFG_GAIN_MASK);
}

/**
 *  @brief      Set the gain bandwidth (GBW)
 *  @brief      设置增益带宽积(GBW)
 *
 *  @param[in]  opa        Pointer to the register overlay for the peripheral
 *  @param[in]  opa        外设寄存器基地址指针
 *  @param[in]  bandwidth  The gain bandwidth to set. One of @ref DL_OPA_GBW
 *  @param[in]  bandwidth  增益带宽积设置，取值参考 DL_OPA_GBW
 *
 *
 *  @pre The OPA must be ready (STAT.RDY == 1) before setting
 *       this configuration.
 *  @pre 设置此配置前，OPA必须就绪(STAT.RDY == 1)。
 *
 */
__STATIC_INLINE void DL_OPA_setGainBandwidth(
    OA_Regs *opa, DL_OPA_GBW bandwidth)
{
    DL_Common_updateReg(
        &opa->CFGBASE, (uint32_t) bandwidth, OA_CFGBASE_GBW_MASK);
}

/**
 *  @brief      Get the gain bandwidth (GBW)
 *  @brief      获取增益带宽积(GBW)
 *
 *  @param[in]  opa   Pointer to the register overlay for the peripheral
 *  @param[in]  opa   外设寄存器基地址指针
 *
 *  @return     The current gain bandwidth
 *  @return     当前增益带宽积
 *
 *  @retval     One of @ref DL_OPA_GBW
 */
__STATIC_INLINE DL_OPA_GBW DL_OPA_getGainBandwidth(const OA_Regs *opa)
{
    uint32_t bandwidth = (opa->CFGBASE & OA_CFGBASE_GBW_MASK);

    return (DL_OPA_GBW)(bandwidth);
}

/**
 *  @brief      Enable the rail-to-rail input (RRI)
 *  @brief      使能轨到轨输入(RRI)
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 *
 *
 *  @pre The OPA must be ready (STAT.RDY == 1) before setting
 *       this configuration.
 *  @pre 设置此配置前，OPA必须就绪(STAT.RDY == 1)。
 *
 */
__STATIC_INLINE void DL_OPA_enableRailToRailInput(OA_Regs *opa)
{
    opa->CFGBASE |= OA_CFGBASE_RRI_ON;
}

/**
 *  @brief      Checks if the the rail-to-rail input (RRI) is enabled
 *  @brief      检查轨到轨输入(RRI)是否已使能
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 *
 *  @return     Returns the rail-to-rail input status of the OPA
 *  @return     返回OPA的轨到轨输入状态
 *
 *  @retval     true  The rail-to-rail input is enabled / 轨到轨输入已使能
 *  @retval     false The rail-to-rail input is disabled / 轨到轨输入已禁用

 */
__STATIC_INLINE bool DL_OPA_isRailToRailInputEnabled(OA_Regs *opa)
{
    return ((opa->CFGBASE &= OA_CFGBASE_RRI_MASK) == OA_CFGBASE_RRI_ON);
}

/**
 *  @brief      Disable the rail-to-rail input (RRI)
 *  @brief      禁用轨到轨输入(RRI)
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_disableRailToRailInput(OA_Regs *opa)
{
    opa->CFGBASE &= ~(OA_CFGBASE_RRI_MASK);
}

/**
 *  @brief      Set and enable the chopping mode
 *  @brief      设置并使能斩波模式
 *
 *  @param[in]  opa     Pointer to the register overlay for the peripheral
 *  @param[in]  opa     外设寄存器基地址指针
 *  @param[in]  mode    The chopping mode to set.
 *                      One of @ref DL_OPA_CHOPPING_MODE
 *  @param[in]  mode    斩波模式设置，取值参考 DL_OPA_CHOPPING_MODE
 */
__STATIC_INLINE void DL_OPA_setChoppingMode(
    OA_Regs *opa, DL_OPA_CHOPPING_MODE mode)
{
    DL_Common_updateReg(&opa->CFG, (uint32_t) mode, OA_CFG_CHOP_MASK);
}

/**
 *  @brief      Get the chopping mode
 *  @brief      获取斩波模式
 *
 *  @param[in]  opa     Pointer to the register overlay for the peripheral
 *  @param[in]  opa     外设寄存器基地址指针
 *
 *  @return     The current chopping mode
 *  @return     当前斩波模式
 *
 *  @retval     One of @ref DL_OPA_CHOPPING_MODE
 */
__STATIC_INLINE DL_OPA_CHOPPING_MODE DL_OPA_getChoppingMode(const OA_Regs *opa)
{
    uint32_t mode = (opa->CFG & OA_CFG_CHOP_MASK);

    return (DL_OPA_CHOPPING_MODE)(mode);
}

/**
 *  @brief      Set the state of the output pin
 *  @brief      设置输出引脚状态
 *
 * When the output pin is enabled, the OPA output goes to the device pin while
 * still maintaining connection to the programmable gain stage and other analog
 * peripherals.
 * When disabled, the OPA output is a purely internal signal and is internally
 * connected to the programmable gain stage and capable of being routed to
 * other on-board analog peripherals.
 * 输出引脚使能时，OPA输出连接到器件引脚，同时保持与可编程增益级和其他模拟外设的连接。
 * 禁用时，OPA输出为纯内部信号，可内部连接到可编程增益级并路由到其他片上模拟外设。
 *
 *  @param[in]  opa     Pointer to the register overlay for the peripheral
 *  @param[in]  opa     外设寄存器基地址指针
 *  @param[in]  state   The state to set the output pin to
 *                      One of @ref DL_OPA_OUTPUT_PIN_STATE
 *  @param[in]  state   输出引脚状态设置，取值参考 DL_OPA_OUTPUT_PIN_STATE
 */
__STATIC_INLINE void DL_OPA_setOutputPinState(
    OA_Regs *opa, DL_OPA_OUTPUT_PIN_STATE state)
{
    DL_Common_updateReg(&opa->CFG, (uint32_t) state, OA_CFG_OUTPIN_MASK);
}

/**
 *  @brief      Get the state of the output pin
 *  @brief      获取输出引脚状态
 *
 * When the output pin is enabled, the OPA output goes to the device pin while
 * still maintaining connection to the programmable gain stage and other analog
 * peripherals.
 * When disabled, the OPA output is a purely internal signal and is internally
 * connected to the programmable gain stage and capable of being routed to
 * other on-board analog peripherals.
 *
 *  @param[in]  opa     Pointer to the register overlay for the peripheral
 *  @param[in]  opa     外设寄存器基地址指针
 *
 *  @return     The state of the output pin
 *  @return     输出引脚状态
 *
 *  @retval     One of @ref DL_OPA_OUTPUT_PIN_STATE
 */
__STATIC_INLINE DL_OPA_OUTPUT_PIN_STATE DL_OPA_getOutputPinState(
    const OA_Regs *opa)
{
    uint32_t state = (opa->CFG & OA_CFG_OUTPIN_MASK);

    return (DL_OPA_OUTPUT_PIN_STATE)(state);
}

/**
 *  @brief      Disable the OPA output signal to be accessed by a device pin
 *  @brief      禁用OPA输出信号到器件引脚的连接
 *
 * When disabled, the OPA output is a purely internal signal and is internally
 * connected to the programmable gain stage and capable of being routed to
 * other on-board analog peripherals.
 * 禁用后，OPA输出为纯内部信号，可内部连接到可编程增益级并路由到其他片上模拟外设。
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 */
__STATIC_INLINE void DL_OPA_disableOutputPin(OA_Regs *opa)
{
    opa->CFG &= ~(OA_CFG_OUTPIN_MASK);
}

/**
 *  @brief      Set the non-inverting input channel
 *  @brief      设置同相输入通道
 *
 *  This API sets the PSEL control bit to select the non-inverting input
 *  channel of the amplifier.
 *  此API设置PSEL控制位以选择放大器的同相输入通道。
 *
 *  @param[in]  opa           Pointer to the register overlay for the peripheral
 *  @param[in]  opa           外设寄存器基地址指针
 *  @param[in]  inputChannel  The non-inverting input channel to set.
 *                            One of @ref DL_OPA_PSEL.
 *  @param[in]  inputChannel  同相输入通道设置，取值参考 DL_OPA_PSEL
 */
__STATIC_INLINE void DL_OPA_setNonInvertingInputChannel(
    OA_Regs *opa, DL_OPA_PSEL inputChannel)
{
    DL_Common_updateReg(&opa->CFG, (uint32_t) inputChannel, OA_CFG_PSEL_MASK);
}

/**
 *  @brief      Get the non-inverting input channel
 *  @brief      获取同相输入通道
 *
 *  @param[in]  opa     Pointer to the register overlay for the peripheral
 *  @param[in]  opa     外设寄存器基地址指针
 *
 *  @return     The current non-inverting input channel
 *  @return     当前同相输入通道
 *
 *  @retval     One of @ref DL_OPA_PSEL
 */
__STATIC_INLINE DL_OPA_PSEL DL_OPA_getNonInvertingInputChannel(
    const OA_Regs *opa)
{
    uint32_t inputChannel = (opa->CFG & OA_CFG_PSEL_MASK);

    return (DL_OPA_PSEL)(inputChannel);
}

/**
 *  @brief      Set the inverting input channel
 *  @brief      设置反相输入通道
 *
 *  This API sets the NSEL control bit to select the inverting input
 *  channel of the amplifier.
 *  此API设置NSEL控制位以选择放大器的反相输入通道。
 *
 *  @param[in]  opa           Pointer to the register overlay for the peripheral
 *  @param[in]  opa           外设寄存器基地址指针
 *  @param[in]  inputChannel  The inverting input channel to set.
 *                            One of @ref DL_OPA_NSEL.
 *  @param[in]  inputChannel  反相输入通道设置，取值参考 DL_OPA_NSEL
 */
__STATIC_INLINE void DL_OPA_setInvertingInputChannel(
    OA_Regs *opa, DL_OPA_NSEL inputChannel)
{
    DL_Common_updateReg(&opa->CFG, (uint32_t) inputChannel, OA_CFG_NSEL_MASK);
}

/**
 *  @brief      Get the inverting input channel
 *  @brief      获取反相输入通道
 *
 *  @param[in]  opa     Pointer to the register overlay for the peripheral
 *  @param[in]  opa     外设寄存器基地址指针
 *
 *  @return     The current inverting input channel
 *  @return     当前反相输入通道
 *
 *  @retval     One of @ref DL_OPA_NSEL
 */
__STATIC_INLINE DL_OPA_NSEL DL_OPA_getInvertingInputChannel(const OA_Regs *opa)
{
    uint32_t inputChannel = (opa->CFG & OA_CFG_NSEL_MASK);

    return (DL_OPA_NSEL)(inputChannel);
}

/**
 *  @brief      Set the M-MUX input channel
 *  @brief      设置M-MUX输入通道
 *
 *  This API sets the MSEL control bit to select the input channel for M-MUX,
 *  which is connected to the programmable gain stage resistor ladder.
 *  此API设置MSEL控制位以选择M-MUX的输入通道，M-MUX连接到可编程增益级电阻梯形网络。
 *
 *  @param[in]  opa           Pointer to the register overlay for the peripheral
 *  @param[in]  opa           外设寄存器基地址指针
 *  @param[in]  inputChannel  The M-MUX input channel to set.
 *                            One of @ref DL_OPA_MSEL.
 *  @param[in]  inputChannel  M-MUX输入通道设置，取值参考 DL_OPA_MSEL
 */
__STATIC_INLINE void DL_OPA_setMMUXInputChannel(
    OA_Regs *opa, DL_OPA_MSEL inputChannel)
{
    DL_Common_updateReg(&opa->CFG, (uint32_t) inputChannel, OA_CFG_MSEL_MASK);
}

/**
 *  @brief      Get the M-MUX input channel
 *  @brief      获取M-MUX输入通道
 *
 *  @param[in]  opa   Pointer to the register overlay for the peripheral
 *  @param[in]  opa   外设寄存器基地址指针
 *
 *  @return     The current M-MUX input channel
 *  @return     当前M-MUX输入通道
 *
 *  @retval     One of @ref DL_OPA_MSEL
 */
__STATIC_INLINE DL_OPA_MSEL DL_OPA_getMMUXInputChannel(const OA_Regs *opa)
{
    uint32_t inputChannel = (opa->CFG & OA_CFG_MSEL_MASK);

    return (DL_OPA_MSEL)(inputChannel);
}

/**
 *  @brief      Set the gain for the programmable gain stage
 *  @brief      设置可编程增益级的增益
 *
 * The OPA integrates a programmable gain stage in the feedback loop to
 * configure the OPA as a PGA(programmable gain amplifier). The gain stage
 * is a feedback resistance ladder and it supports up to 32x amplification.
 * OPA在反馈环路中集成了可编程增益级，可配置为PGA(可编程增益放大器)。
 * 增益级为反馈电阻梯形网络，支持最高32倍放大。
 *
 * Refer to the TRM for gain enumeration information.
 *
 *  @param[in]  opa   Pointer to the register overlay for the peripheral
 *  @param[in]  opa   外设寄存器基地址指针
 *  @param[in]  gain  The gain to set. One of @ref DL_OPA_GAIN.
 *  @param[in]  gain  增益设置，取值参考 DL_OPA_GAIN
 */
__STATIC_INLINE void DL_OPA_setGain(OA_Regs *opa, DL_OPA_GAIN gain)
{
    DL_Common_updateReg(&opa->CFG, (uint32_t) gain, OA_CFG_GAIN_MASK);
}

/**
 *  @brief      Get the gain from the programmable gain stage
 *  @brief      获取可编程增益级的增益
 *
 * Refer to the TRM for gain enumeration information.
 *
 *  @param[in]  opa   Pointer to the register overlay for the peripheral
 *  @param[in]  opa   外设寄存器基地址指针
 *
 *  @return     The gain
 *  @return     当前增益值
 *
 *  @retval     The gain. One of @ref DL_OPA_GAIN
 */
__STATIC_INLINE DL_OPA_GAIN DL_OPA_getGain(const OA_Regs *opa)
{
    uint32_t gain = (opa->CFG & OA_CFG_GAIN_MASK);

    return (DL_OPA_GAIN)(gain);
}

/**
 *  @brief      Increment gain to the next @ref DL_OPA_GAIN enum value
 *  @brief      将增益递增到下一个 DL_OPA_GAIN 枚举值
 *
 * The OPA allows dynamic gain changes. If the gain is already at the maximum
 * setting DL_OPA_GAIN_N31_P32 (CFG.GAIN = 0x5), this function does nothing.
 * OPA允许动态增益更改。如果增益已达到最大值DL_OPA_GAIN_N31_P32 (CFG.GAIN = 0x5)，
 * 此函数不执行任何操作。
 *
 * Refer to the TRM for more information about changing gain dynamically.
 *
 *  @param[in]  opa   Pointer to the register overlay for the peripheral
 *  @param[in]  opa   外设寄存器基地址指针
 *
 *  @return     The gain at the end of the operation.
 *  @return     操作结束后的增益值
 *
 *  @retval     The applied gain. One of @ref DL_OPA_GAIN
 */
DL_OPA_GAIN DL_OPA_increaseGain(OA_Regs *opa);

/**
 *  @brief      Decrement gain to the next @ref DL_OPA_GAIN enum value
 *  @brief      将增益递减到下一个 DL_OPA_GAIN 枚举值
 *
 * The OPA allows dynamic gain changes. If the gain is at the minimum setting
 * DL_OPA_GAIN_N1_P2 (CFG.GAIN = 0x1), this function does nothing.
 * OPA允许动态增益更改。如果增益已达到最小值DL_OPA_GAIN_N1_P2 (CFG.GAIN = 0x1)，
 * 此函数不执行任何操作。
 *
 * Refer to the TRM for more information about changing gain dynamically.
 *
 *  @param[in]  opa   Pointer to the register overlay for the peripheral
 *  @param[in]  opa   外设寄存器基地址指针
 *
 *  @return     The gain at the end of the operation.
 *  @return     操作结束后的增益值
 *
 *  @retval     The applied gain. One of @ref DL_OPA_GAIN
 */
DL_OPA_GAIN DL_OPA_decreaseGain(OA_Regs *opa);

/**
 *  @brief      Checks if the OPA is ready
 *  @brief      检查OPA是否就绪
 *
 *  @param[in]  opa  Pointer to the register overlay for the peripheral
 *  @param[in]  opa  外设寄存器基地址指针
 *
 *  @return     The status of the OPA ready bit
 *  @return     OPA就绪位的状态
 *
 *  @retval     true   The OPA is ready / OPA已就绪
 *  @retval     false  The OPA is not ready / OPA未就绪
 *
 */
__STATIC_INLINE bool DL_OPA_isReady(const OA_Regs *opa)
{
    return ((opa->STAT & OA_STAT_RDY_MASK) == OA_STAT_RDY_TRUE);
}

#ifdef __cplusplus
}
#endif
#endif /* __MSPM0_HAS_OA__ */

#endif /* ti_dl_dl_opa__include */
/** @}*/
