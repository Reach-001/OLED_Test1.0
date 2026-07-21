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
 *  @file       dl_gpamp.h
 *  @brief      General Purpose Amplifier (GPAMP)
 *  @defgroup   GPAMP General Purpose Amplifier (GPAMP)
 *
 ******************************************************************************
 *  @brief 中文说明：通用运算放大器 (GPAMP) 驱动头文件。
 *         提供 GPAMP 使能/禁止、输入通道选择、轨到轨输入模式、
 *         输出到 PAD、斩波频率/模式等配置接口。
 */
/** @addtogroup GPAMP
 * @{
 */
#ifndef ti_dl_m0p_dl_gpamp__include
#define ti_dl_m0p_dl_gpamp__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_GPAMP__

#ifdef __cplusplus
extern "C"
{
#endif

    /*! @enum DL_GPAMP_NSEL 反相输入通道选择 */
    typedef enum
    {
        /*! Inverting input channel is the output pin. 反相输入接输出引脚（跟随器接法） */
        DL_GPAMP_NSEL_OUTPUT_PIN = SYSCTL_PMUOPAMP_NSEL_SEL0,
        /*! Inverting input channel is the IN- pin. 反相输入接 IN- 引脚 */
        DL_GPAMP_NSEL_IN_NEG = SYSCTL_PMUOPAMP_NSEL_SEL1,
        /*! Inverting input channel is internal GPAMP output. 反相输入接内部 GPAMP 输出（片内反馈） */
        DL_GPAMP_NSEL_INTERNAL_OUTPUT = SYSCTL_PMUOPAMP_NSEL_SEL2,
        /*! No channel selected for the inverting input channel. 反相输入通道未选择（开路） */
        DL_GPAMP_NSEL_OPEN = SYSCTL_PMUOPAMP_NSEL_SEL3,
    } DL_GPAMP_NSEL;

    /*! @enum DL_GPAMP_PSEL 正输入通道选择 */
    typedef enum
    {
        /*! Non-Inverting input channel is disabled. 正输入通道禁用 */
        DL_GPAMP_PSEL_OPEN = SYSCTL_PMUOPAMP_PCHENABLE_FALSE,
        /*! Non-Inverting input channel is enabled. 正输入通道启用 */
        DL_GPAMP_PSEL_IN_POS = SYSCTL_PMUOPAMP_PCHENABLE_TRUE,
    } DL_GPAMP_PSEL;

    /*! @enum DL_GPAMP_OUTPUT_PIN_STATE 输出引脚状态 */
    typedef enum
    {
        /*! Output pin is disabled. 输出引脚禁用 */
        DL_GPAMP_OUTPUT_PIN_STATE_DISABLED = SYSCTL_PMUOPAMP_OUTENABLE_FALSE,
        /*! Output pin is enabled. 输出引脚使能 */
        DL_GPAMP_OUTPUT_PIN_STATE_ENABLED = SYSCTL_PMUOPAMP_OUTENABLE_TRUE,
    } DL_GPAMP_OUTPUT_PIN_STATE;

    /*! @enum DL_GPAMP_RRI_MODE 轨到轨输入模式 */
    typedef enum
    {
        /*! RRI mode is PMOS input pairs mode. PMOS 输入对模式（接近正轨） */
        DL_GPAMP_RRI_MODE_PMOS_INPUT = SYSCTL_PMUOPAMP_RRI_MODE0,
        /*! RRI mode is NMOS input pairs. NMOS 输入对模式（接近负轨） */
        DL_GPAMP_RRI_MODE_NMOS_INPUT = SYSCTL_PMUOPAMP_RRI_MODE1,
        /*! RRI mode is rail to rail. 轨到轨模式（全范围输入） */
        DL_GPAMP_RRI_MODE_RAIL_TO_RAIL = SYSCTL_PMUOPAMP_RRI_MODE2,
        /*! RRI mode is sample channel 0. 采样通道 0 模式 */
        DL_GPAMP_RRI_MODE_SAMPLE_CHANNEL_0 = SYSCTL_PMUOPAMP_RRI_MODE3,

    } DL_GPAMP_RRI_MODE;

    /*! @enum DL_GPAMP_CHOPPING_FREQ 斩波时钟频率 */
    typedef enum
    {
        /*! Chopping clock frequency is 16KHZ. 斩波时钟 16 kHz */
        DL_GPAMP_CHOPPING_FREQ_16KHZ = SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK16KHZ,
        /*! Chopping clock frequency is 8KHZ. 斩波时钟 8 kHz */
        DL_GPAMP_CHOPPING_FREQ_8KHZ = SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK8KHZ,
        /*! Chopping clock frequency is 4KHZ. 斩波时钟 4 kHz */
        DL_GPAMP_CHOPPING_FREQ_4KHZ = SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK4KHZ,
        /*! Chopping clock frequency is 2KHZ. 斩波时钟 2 kHz */
        DL_GPAMP_CHOPPING_FREQ_2KHZ = SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK2KHZ,
    } DL_GPAMP_CHOPPING_FREQ;

    /*! @enum DL_GPAMP_CHOPPING_MODE 斩波工作模式 */
    typedef enum
    {
        /*! Chopping is disabled. 斩波功能禁用 */
        DL_GPAMP_CHOPPING_MODE_DISABLED = SYSCTL_PMUOPAMP_CHOPCLKMODE_CHOPDISABLED,
        /*! Chopping mode is standard chop. 标准斩波模式 */
        DL_GPAMP_CHOPPING_MODE_STANDARD = SYSCTL_PMUOPAMP_CHOPCLKMODE_REGCHOP,
#if defined(GPAMP_SYS_ADC_CHOPPING) && (GPAMP_SYS_ADC_CHOPPING == 1)
        /*! Chopping mode is ADC assisted chop. ADC 协同斩波模式 */
        DL_GPAMP_CHOPPING_MODE_ADC_ASSISTED =
            SYSCTL_PMUOPAMP_CHOPCLKMODE_ADCASSIST,
#endif
    } DL_GPAMP_CHOPPING_MODE;

    /*!
     *  @brief  Configuration struct for @ref DL_GPAMP_init
     *          DL_GPAMP_init 初始化配置结构体
     */
    typedef struct
    {
        /*! The chopping mode to set. One of @ref DL_GPAMP_CHOPPING_MODE. 斩波工作模式 */
        DL_GPAMP_CHOPPING_MODE choppingMode;
        /*! The chopping frequency to set. One of @ref DL_GPAMP_CHOPPING_FREQ. 斩波时钟频率 */
        DL_GPAMP_CHOPPING_FREQ choppingFreq;
        /*! The state of the output pin. One of @ref DL_GPAMP_OUTPUT_PIN_STATE. 输出引脚使能状态 */
        DL_GPAMP_OUTPUT_PIN_STATE outputPinState;
        /*! The non-inverting input channel to set. One of @ref DL_GPAMP_PSEL. 正输入通道选择 */
        DL_GPAMP_PSEL pselChannel;
        /*! The inverting input channel to set. One of @ref DL_GPAMP_NSEL. 反相输入通道选择 */
        DL_GPAMP_NSEL nselChannel;
    } DL_GPAMP_Config;

    /* clang-format on */

    /**
     *  @brief      Enable the GPAMP peripheral. 使能通用运放外设
     */
    __STATIC_INLINE void DL_GPAMP_enable(void)
    {
        SYSCTL->SOCLOCK.PMUOPAMP |= SYSCTL_PMUOPAMP_ENABLE_TRUE; // 置位使能位
    }

    /**
     *  @brief      Checks if the GPAMP peripheral is enabled. 查询 GPAMP 是否已使能
     *
     *  @return     Returns the enabled status of the GPAMP. true 表示已使能，false 表示未使能
     *
     *  @retval     true  The GPAMP peripheral is enabled
     *  @retval     false The GPAMP peripheral is disabled
     */
    __STATIC_INLINE bool DL_GPAMP_isEnabled(void)
    {
        // 读取使能位并与真值比较
        return ((SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_ENABLE_MASK) ==
                SYSCTL_PMUOPAMP_ENABLE_TRUE);
    }

    /**
     *  @brief      Disable the GPAMP peripheral. 禁用通用运放外设
     */
    __STATIC_INLINE void DL_GPAMP_disable(void)
    {
        SYSCTL->SOCLOCK.PMUOPAMP &= ~(SYSCTL_PMUOPAMP_ENABLE_MASK); // 清除使能位
    }

    /**
     *  @brief      Initialize the GPAmp peripheral. 按配置结构初始化 GPAMP（不自动使能）
     *
     *  Initializes all the common configurable options for the GPAmp peripheral.
     *  Any other custom configuration can be done after calling this API.
     *  The GPAmp is not enabled in this API.
     *
     *  @param[in]  config  Configuration for gpamp peripheral. GPAMP 初始化配置结构体指针
     */
    __STATIC_INLINE void DL_GPAMP_init(DL_GPAMP_Config *config)
    {
        // 将所有配置字段合并后一次性写入寄存器，仅更新对应掩码位域
        DL_Common_updateReg(&SYSCTL->SOCLOCK.PMUOPAMP,
                            (uint32_t)config->choppingMode | (uint32_t)config->choppingFreq |
                                (uint32_t)config->outputPinState |
                                (uint32_t)config->pselChannel | (uint32_t)config->nselChannel,
                            SYSCTL_PMUOPAMP_CHOPCLKMODE_MASK | SYSCTL_PMUOPAMP_CHOPCLKFREQ_MASK |
                                SYSCTL_PMUOPAMP_OUTENABLE_MASK | SYSCTL_PMUOPAMP_PCHENABLE_MASK |
                                SYSCTL_PMUOPAMP_NSEL_MASK);
    }

    /**
     *  @brief      Enable the non-inverting input channel. 使能正输入通道（IN+）
     */
    __STATIC_INLINE void DL_GPAMP_enableNonInvertingInputChannel(void)
    {
        SYSCTL->SOCLOCK.PMUOPAMP |= SYSCTL_PMUOPAMP_PCHENABLE_TRUE; // 置位正输入使能
    }

    /**
     *  @brief      Checks if the non-inverting input channel is enabled. 查询正输入通道是否已启用
     *
     *  @return     Returns the enabled status of the GPAMP. true 表示已启用，false 表示未启用
     *
     *  @retval     true  The non-inverting input channel is enabled
     *  @retval     false The non-inverting input channel is disabled
     */
    __STATIC_INLINE bool DL_GPAMP_isNonInvertingInputChannelEnabled(void)
    {
        // 读取正输入使能标志位
        return ((SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_PCHENABLE_MASK) ==
                SYSCTL_PMUOPAMP_PCHENABLE_TRUE);
    }

    /**
     *  @brief      Disable the non-inverting input channel. 禁用正输入通道（IN+）
     */
    __STATIC_INLINE void DL_GPAMP_disableNonInvertingInputChannel(void)
    {
        SYSCTL->SOCLOCK.PMUOPAMP &= ~(SYSCTL_PMUOPAMP_PCHENABLE_MASK); // 清除正输入使能位
    }

    /**
     *  @brief      Set the inverting input channel. 设置反相输入端通道来源
     *
     *  @param[in]  inputChannel  The inverting input channel to set.
     *                            One of @ref DL_GPAMP_NSEL. 反相输入端通道选择
     */
    __STATIC_INLINE void DL_GPAMP_setInvertingInputChannel(
        DL_GPAMP_NSEL inputChannel)
    {
        // 仅更新 NSEL 位域
        DL_Common_updateReg(&SYSCTL->SOCLOCK.PMUOPAMP, (uint32_t)inputChannel,
                            SYSCTL_PMUOPAMP_NSEL_MASK);
    }

    /**
     *  @brief      Get the inverting input channel. 读取当前反相输入端通道选择
     *
     *  @return     The current inverting input channel. 返回当前反相输入通道枚举值
     *
     *  @retval     One of @ref DL_GPAMP_NSEL
     */
    __STATIC_INLINE DL_GPAMP_NSEL DL_GPAMP_getInvertingInputChannel(void)
    {
        uint32_t nsel = (SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_NSEL_MASK); // 提取 NSEL 位域

        return (DL_GPAMP_NSEL)(nsel);
    }

    /**
     *  @brief      Set the rail-to-rail input (RRI) mode. 设置轨到轨输入模式
     *
     *  @param[in]  mode  The RRI mode to set. One of @ref DL_GPAMP_RRI_MODE. RRI 模式选择
     */
    __STATIC_INLINE void DL_GPAMP_setRailToRailInputMode(DL_GPAMP_RRI_MODE mode)
    {
        // 仅更新 RRI 位域
        DL_Common_updateReg(
            &SYSCTL->SOCLOCK.PMUOPAMP, (uint32_t)mode, SYSCTL_PMUOPAMP_RRI_MASK);
    }

    /**
     *  @brief      Get the rail-to-rail input (RRI) mode. 读取当前轨到轨输入模式
     *
     *  @return     The current RRI mode. 返回当前 RRI 模式枚举值
     *
     *  @retval     One of @ref DL_GPAMP_RRI_MODE
     */
    __STATIC_INLINE DL_GPAMP_RRI_MODE DL_GPAMP_getRailToRailInputMode(void)
    {
        uint32_t mode = (SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_RRI_MASK); // 提取 RRI 位域

        return (DL_GPAMP_RRI_MODE)(mode);
    }

    /**
     *  @brief      Enable output to pad. 使能 GPAMP 输出到外部引脚
     */
    __STATIC_INLINE void DL_GPAMP_enableOutputToPad(void)
    {
        SYSCTL->SOCLOCK.PMUOPAMP |= SYSCTL_PMUOPAMP_OUTENABLE_TRUE; // 置位输出使能
    }

    /**
     *  @brief      Checks if output to pad is enabled. 查询输出到引脚是否已使能
     *
     *  @return     Returns if output to pad is enabled. true 表示已使能，false 表示未使能
     *
     *  @retval     true  Output to pad is enabled
     *  @retval     false Output to pad is disabled
     */
    __STATIC_INLINE bool DL_GPAMP_isOutputToPadEnabled(void)
    {
        // 读取输出使能标志位
        return ((SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_OUTENABLE_MASK) ==
                SYSCTL_PMUOPAMP_OUTENABLE_TRUE);
    }

    /**
     *  @brief      Disable output to pad. 禁用 GPAMP 输出到外部引脚
     */
    __STATIC_INLINE void DL_GPAMP_disableOutputToPad(void)
    {
        SYSCTL->SOCLOCK.PMUOPAMP &= ~(SYSCTL_PMUOPAMP_OUTENABLE_MASK); // 清除输出使能位
    }

    /**
     *  @brief      Set the chopping clock frequency. 设置斩波时钟频率（用于抑制失调与低频噪声）
     *
     *  @param[in]  frequency  The chopping frequency to set.
     *                         One of @ref DL_GPAMP_CHOPPING_FREQ. 斩波频率枚举值
     */
    __STATIC_INLINE void DL_GPAMP_setChoppingFrequency(
        DL_GPAMP_CHOPPING_FREQ frequency)
    {
        // 仅更新斩波频率位域
        DL_Common_updateReg(&SYSCTL->SOCLOCK.PMUOPAMP, (uint32_t)frequency,
                            SYSCTL_PMUOPAMP_CHOPCLKFREQ_MASK);
    }

    /**
     *  @brief      Get the chopping clock frequency. 读取当前斩波时钟频率
     *
     *  @return     The current chopping frequency. 返回当前斩波频率枚举值
     *
     *  @retval     One of @ref DL_GPAMP_CHOPPING_FREQ
     */
    __STATIC_INLINE DL_GPAMP_CHOPPING_FREQ DL_GPAMP_getChoppingFrequency(void)
    {
        // 提取斩波频率位域
        uint32_t freq =
            (SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_CHOPCLKFREQ_MASK);

        return (DL_GPAMP_CHOPPING_FREQ)(freq);
    }

    /**
     *  @brief      Set the chopping mode. 设置斩波工作模式（禁用/常规/ADC 协同）
     *
     *  @param[in]  mode  The chopping mode to set.
     *                         One of @ref DL_GPAMP_CHOPPING_MODE. 斩波模式枚举值
     */
    __STATIC_INLINE void DL_GPAMP_setChoppingMode(DL_GPAMP_CHOPPING_MODE mode)
    {
        // 仅更新斩波模式位域
        DL_Common_updateReg(&SYSCTL->SOCLOCK.PMUOPAMP, (uint32_t)mode,
                            SYSCTL_PMUOPAMP_CHOPCLKMODE_MASK);
    }

    /**
     *  @brief      Get the chopping mode. 读取当前斩波模式配置
     *
     *  @return     The current chopping mode. 返回当前斩波模式枚举值
     *
     *  @retval     One of @ref DL_GPAMP_CHOPPING_MODE
     */
    __STATIC_INLINE DL_GPAMP_CHOPPING_MODE DL_GPAMP_getChoppingMode(void)
    {
        // 提取斩波模式位域
        uint32_t mode =
            (SYSCTL->SOCLOCK.PMUOPAMP & SYSCTL_PMUOPAMP_CHOPCLKMODE_MASK);

        return (DL_GPAMP_CHOPPING_MODE)(mode);
    }

    /* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_GPAMP__ */

#endif /* ti_dl_m0p_dl_gpamp__include */
/** @}*/
