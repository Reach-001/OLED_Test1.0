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
 *  @file       dl_comp.h
 *  @brief      Comparator (COMP) 比较器 (COMP) 外设驱动库头文件。
 *  @defgroup   COMP Comparator (COMP) 比较器 (COMP)
 *
 *  @anchor ti_dl_dl_comp_Overview
 *  # Overview
 *
 *  The COMP DriverLib allows full configuration of the MSPM0 comparator module.
 *  The comparator module (COMP) is an analog voltage comparator with general
 *  comparator functionality.
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup COMP
 * @{
 */
#ifndef ti_dl_m0p_dl_comp__include
#define ti_dl_m0p_dl_comp__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_COMP__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef COMP_SYS_DACOUT_EN
/*!
 * @brief Device has support for 8-bit DAC output to pin. 设备支持 8-bit DAC 输出到引脚。
 */
#define DEVICE_HAS_DAC_OUT_TO_PIN
#endif /* COMP_SYS_DACOUT_EN */

    /* clang-format off */

/** @addtogroup DL_COMP_INTERRUPT
 *  @{
 */
/*!
 * @brief Comparator output ready interrupt. 比较器输出就绪中断。
 */
#define DL_COMP_INTERRUPT_OUTPUT_READY               (COMP_CPU_INT_IMASK_OUTRDYIFG_SET)

/*!
 * @brief Rising or falling edge of comparator output (selected by IES bit)
 *        interrupt. 比较器输出上升沿或下降沿中断（由 IES 位选择）。
 */
#define DL_COMP_INTERRUPT_OUTPUT_EDGE                  (COMP_CPU_INT_IMASK_COMPIFG_SET)

/*!
 * @brief Rising or falling edge of comparator inverted output (selected by
 *        IES bit) interrupt. 比较器反相输出上升沿或下降沿中断（由 IES 位选择）。
 */
#define DL_COMP_INTERRUPT_OUTPUT_EDGE_INV           (COMP_CPU_INT_IMASK_COMPINVIFG_SET)

/** @}*/

/** @addtogroup DL_COMP_EVENT
 *  @{
 */
/*!
 * @brief Comparator output ready event. 比较器输出就绪事件。
 */
#define DL_COMP_EVENT_OUTPUT_READY               (COMP_GEN_EVENT_IMASK_OUTRDYIFG_SET)

/*!
 * @brief Rising or falling edge of comparator output (selected by IES bit)
 *        event. 比较器输出上升沿或下降沿事件（由 IES 位选择）。
 */
#define DL_COMP_EVENT_OUTPUT_EDGE                  (COMP_GEN_EVENT_IMASK_COMPIFG_SET)

/*!
 * @brief Rising or falling edge of comparator inverted output (selected by
 *        IES bit) event. 比较器反相输出上升沿或下降沿事件（由 IES 位选择）。
 */
#define DL_COMP_EVENT_OUTPUT_EDGE_INV           (COMP_GEN_EVENT_IMASK_COMPINVIFG_SET)

/** @}*/

    /* clang-format on */

    /*! @enum DL_COMP_IIDX 中断索引枚举 */
    typedef enum
    {
        /*! COMP interrupt index for no interrupt. 无中断。 */
        DL_COMP_IIDX_NO_INTERRUPT = COMP_CPU_INT_IIDX_STAT_NO_INTR,
        /*! COMP interrupt index for comparator output ready interrupt. 比较器输出就绪中断。 */
        DL_COMP_IIDX_OUTPUT_READY = COMP_CPU_INT_IIDX_STAT_OUTRDYIFG,
        /*! COMP interrupt index for rising or falling edge of comparator output
         * (selected by IES bit) interrupt. 比较器输出边沿中断。 */
        DL_COMP_IIDX_OUTPUT_EDGE = COMP_CPU_INT_IIDX_STAT_COMPIFG,
        /*! COMP interrupt index for rising or falling edge of comparator inverted
         * output (selected by IES bit) interrupt. 比较器反相输出边沿中断。 */
        DL_COMP_IIDX_OUTPUT_EDGE_INV = COMP_CPU_INT_IIDX_STAT_COMPINVIFG,
    } DL_COMP_IIDX;

    /*! @enum DL_COMP_MODE 比较器工作模式枚举 */
    typedef enum
    {
        /*! Comparator is in fast mode. 快速模式。 */
        DL_COMP_MODE_FAST = COMP_CTL1_MODE_FAST,
        /*! Comparator is in ultra-low power (ULP) mode. 超低功耗模式。 */
        DL_COMP_MODE_ULP = COMP_CTL1_MODE_ULP,
    } DL_COMP_MODE;

    /*! @enum DL_COMP_OUTPUT_INT_EDGE 输出中断边沿选择枚举 */
    typedef enum
    {
        /*! Rising edge sets COMPIFG and falling edge sets COMPINVIFG. 上升沿置位 COMPIFG，下降沿置位 COMPINVIFG。 */
        DL_COMP_OUTPUT_INT_EDGE_RISING = COMP_CTL1_IES_RISING,
        /*! Falling edge sets COMPIFG and rising edge sets COMPINVIFG. 下降沿置位 COMPIFG，上升沿置位 COMPINVIFG。 */
        DL_COMP_OUTPUT_INT_EDGE_FALLING = COMP_CTL1_IES_FALLING,
    } DL_COMP_OUTPUT_INT_EDGE;

    /*! @enum DL_COMP_HYSTERESIS 迟滞电压枚举 */
    typedef enum
    {
        /*! No hysteresis. 无迟滞。 */
        DL_COMP_HYSTERESIS_NONE = COMP_CTL1_HYST_NO_HYS,
        /*! Typical hysteresis voltage generated is 10mV. 典型迟滞电压 10mV。 */
        DL_COMP_HYSTERESIS_10 = COMP_CTL1_HYST_LOW_HYS,
        /*! Typical hysteresis voltage generated is 20mV. 典型迟滞电压 20mV。 */
        DL_COMP_HYSTERESIS_20 = COMP_CTL1_HYST_MED_HYS,
        /*! Typical hysteresis voltage generated is 30mV. 典型迟滞电压 30mV。 */
        DL_COMP_HYSTERESIS_30 = COMP_CTL1_HYST_HIGH_HYS,
    } DL_COMP_HYSTERESIS;

    /*! @enum DL_COMP_POLARITY 输出极性枚举 */
    typedef enum
    {
        /*! Comparator output is non-inverted. When comparator is off, OUT is low. 输出不反相，关闭时 OUT 为低。 */
        DL_COMP_POLARITY_NON_INV = COMP_CTL1_OUTPOL_NON_INV,
        /*! Comparator output is inverted. When comparator is off, OUT is high. 输出反相，关闭时 OUT 为高。 */
        DL_COMP_POLARITY_INV = COMP_CTL1_OUTPOL_INV,
    } DL_COMP_POLARITY;

    /*! @enum DL_COMP_FILTER_DELAY 滤波延时枚举 */
    typedef enum
    {
        /*! Typical filter delay of 70ns. 典型滤波延时 70ns。 */
        DL_COMP_FILTER_DELAY_70 = COMP_CTL1_FLTDLY_DLY_0,
        /*! Typical filter delay of 500ns. 典型滤波延时 500ns。 */
        DL_COMP_FILTER_DELAY_500 = COMP_CTL1_FLTDLY_DLY_1,
        /*! Typical filter delay of 1200ns. 典型滤波延时 1200ns。 */
        DL_COMP_FILTER_DELAY_1200 = COMP_CTL1_FLTDLY_DLY_2,
        /*! Typical filter delay of 2700ns. 典型滤波延时 2700ns。 */
        DL_COMP_FILTER_DELAY_2700 = COMP_CTL1_FLTDLY_DLY_3,
    } DL_COMP_FILTER_DELAY;

    /*! @enum DL_COMP_IPSEL_CHANNEL 正端输入通道选择枚举 */
    typedef enum
    {
        /*! IPSEL is channel 0. 正端输入通道 0，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_0 = COMP_CTL0_IPSEL_CH_0,
        /*! IPSEL is channel 1. 正端输入通道 1，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_1 = COMP_CTL0_IPSEL_CH_1,
        /*! IPSEL is channel 2. 正端输入通道 2，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_2 = COMP_CTL0_IPSEL_CH_2,
        /*! IPSEL is channel 3. 正端输入通道 3，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_3 = COMP_CTL0_IPSEL_CH_3,
        /*! IPSEL is channel 4. 正端输入通道 4，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_4 = COMP_CTL0_IPSEL_CH_4,
        /*! IPSEL is channel 5. 正端输入通道 5，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_5 = COMP_CTL0_IPSEL_CH_5,
        /*! IPSEL is channel 6. 正端输入通道 6，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_6 = COMP_CTL0_IPSEL_CH_6,
        /*! IPSEL is channel 7. 正端输入通道 7，详见器件数据手册。 */
        DL_COMP_IPSEL_CHANNEL_7 = COMP_CTL0_IPSEL_CH_7,
    } DL_COMP_IPSEL_CHANNEL;

    /*! @enum DL_COMP_IMSEL_CHANNEL 负端输入通道选择枚举 */
    typedef enum
    {
        /*! IMSEL is channel 0. 负端输入通道 0，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_0 = COMP_CTL0_IMSEL_CH_0,
        /*! IMSEL is channel 1. 负端输入通道 1，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_1 = COMP_CTL0_IMSEL_CH_1,
        /*! IMSEL is channel 2. 负端输入通道 2，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_2 = COMP_CTL0_IMSEL_CH_2,
        /*! IMSEL is channel 3. 负端输入通道 3，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_3 = COMP_CTL0_IMSEL_CH_3,
        /*! IMSEL is channel 4. 负端输入通道 4，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_4 = COMP_CTL0_IMSEL_CH_4,
        /*! IMSEL is channel 5. 负端输入通道 5，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_5 = COMP_CTL0_IMSEL_CH_5,
        /*! IMSEL is channel 6. 负端输入通道 6，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_6 = COMP_CTL0_IMSEL_CH_6,
        /*! IMSEL is channel 7. 负端输入通道 7，详见器件数据手册。 */
        DL_COMP_IMSEL_CHANNEL_7 = COMP_CTL0_IMSEL_CH_7,
    } DL_COMP_IMSEL_CHANNEL;

    /*! @enum DL_COMP_ENABLE_CHANNEL 输入通道使能枚举 */
    typedef enum
    {
        /*! Channel input disabled for both the positive and negative terminals. 正负端通道均禁用。 */
        DL_COMP_ENABLE_CHANNEL_NONE =
            (COMP_CTL0_IPEN_DISABLE | COMP_CTL0_IMEN_DISABLE),
        /*! Channel input enabled for the positive terminal. 正端通道使能。 */
        DL_COMP_ENABLE_CHANNEL_POS = COMP_CTL0_IPEN_ENABLE,
        /*! Channel input enabled for the negative terminal. 负端通道使能。 */
        DL_COMP_ENABLE_CHANNEL_NEG = COMP_CTL0_IMEN_ENABLE,
        /*! Channel input enabled for both the positive and negative terminals. 正负端通道均使能。 */
        DL_COMP_ENABLE_CHANNEL_POS_NEG =
            (COMP_CTL0_IPEN_ENABLE | COMP_CTL0_IMEN_ENABLE),
    } DL_COMP_ENABLE_CHANNEL;

    /*! @enum DL_COMP_REF_MODE 参考电压工作模式枚举 */
    typedef enum
    {
        /*! ULP_REF bandgap, local reference buffer, and 8-bit DAC inside
         * comparator operate in static mode. 静态模式，精度高但功耗较大。 */
        DL_COMP_REF_MODE_STATIC = COMP_CTL2_REFMODE_STATIC,
        /*! ULP_REF bandgap, local reference buffer, and 8-bit DAC inside
         * comparator operate in sampled mode. 采样模式，功耗低但精度稍低。 */
        DL_COMP_REF_MODE_SAMPLED = COMP_CTL2_REFMODE_SAMPLED,
    } DL_COMP_REF_MODE;

    /*! @enum DL_COMP_REF_SOURCE 参考电压源选择枚举 */
    typedef enum
    {
        /*! Reference voltage generator is disabled (local reference buffer as
         * well as DAC). 参考电压发生器关闭。 */
        DL_COMP_REF_SOURCE_NONE = COMP_CTL2_REFSRC_OFF,
        /*! VDDA selected as the reference source to DAC and DAC output applied
         * as reference to comparator. VDDA 经 DAC 输出作为比较器参考。 */
        DL_COMP_REF_SOURCE_VDDA_DAC = COMP_CTL2_REFSRC_VDDA_DAC,
        /*! VREF selected as the reference source to DAC and DAC output applied
         * as reference to comparator. VREF 经 DAC 输出作为比较器参考。 */
        DL_COMP_REF_SOURCE_VREF_DAC = COMP_CTL2_REFSRC_VREF_DAC,
        /*! VREF applied as reference to comparator. For some devices, DAC is
         * switched off. VREF 直接作为比较器参考（部分器件 DAC 关闭）。 */
        DL_COMP_REF_SOURCE_VREF = COMP_CTL2_REFSRC_VREF,
        /*! VDDA is used as comparator reference. For some devices, DAC is
         * switched off. VDDA 直接作为比较器参考（部分器件 DAC 关闭）。 */
        DL_COMP_REF_SOURCE_VDDA = COMP_CTL2_REFSRC_VDDA,
        /*! Internal reference selected as the reference source to the DAC and DAC
         * output applied as reference to comparator. 内部参考经 DAC 输出作为比较器参考。 */
        DL_COMP_REF_SOURCE_INT_VREF_DAC = COMP_CTL2_REFSRC_INTVREF_DAC,
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0L122X_L222X)
        /*! Internal VREF used as reference source of comparator. 内部 VREF 直接作为比较器参考。 */
        DL_COMP_REF_SOURCE_INT_VREF = COMP_CTL2_REFSRC_INTVREF,
#endif
    } DL_COMP_REF_SOURCE;

    /*! @enum DL_COMP_REF_TERMINAL_SELECT 参考电压端子选择枚举 */
    typedef enum
    {
        /*! Reference applied to positive terminal. If EXCH bit is 1, reference
         * applied to negative terminal. 参考电压施加到正端（EXCH=1 时施加到负端）。 */
        DL_COMP_REF_TERMINAL_SELECT_POS = COMP_CTL2_REFSEL_POSITIVE,
        /*! Reference applied to negative terminal. If EXCH bit is 0, reference
         * applied to positive terminal. 参考电压施加到负端（EXCH=0 时施加到正端）。 */
        DL_COMP_REF_TERMINAL_SELECT_NEG = COMP_CTL2_REFSEL_NEGATIVE,
    } DL_COMP_REF_TERMINAL_SELECT;

    /*! @enum DL_COMP_BLANKING_SOURCE 消隐源选择枚举 */
    typedef enum
    {
        /*! Disable use of blanking source. 禁用消隐源。 */
        DL_COMP_BLANKING_SOURCE_DISABLE = COMP_CTL2_BLANKSRC_DISABLE,
        /*! Blanking source 1. 消隐源 1。 */
        DL_COMP_BLANKING_SOURCE_1 = COMP_CTL2_BLANKSRC_BLANKSRC1,
        /*! Blanking source 2. 消隐源 2。 */
        DL_COMP_BLANKING_SOURCE_2 = COMP_CTL2_BLANKSRC_BLANKSRC2,
        /*! Blanking source 3. 消隐源 3。 */
        DL_COMP_BLANKING_SOURCE_3 = COMP_CTL2_BLANKSRC_BLANKSRC3,
        /*! Blanking source 4. 消隐源 4。 */
        DL_COMP_BLANKING_SOURCE_4 = COMP_CTL2_BLANKSRC_BLANKSRC4,
        /*! Blanking source 5. 消隐源 5。 */
        DL_COMP_BLANKING_SOURCE_5 = COMP_CTL2_BLANKSRC_BLANKSRC5,
        /*! Blanking source 6. 消隐源 6。 */
        DL_COMP_BLANKING_SOURCE_6 = COMP_CTL2_BLANKSRC_BLANKSRC6,
    } DL_COMP_BLANKING_SOURCE;

    /*! @enum DL_COMP_DAC_CONTROL DAC 控制源选择枚举 */
    typedef enum
    {
        /*! Comparator output controls selection betwen DACCODE0 and DACCODE1. 比较器输出控制 DACCODE0/DACCODE1 切换。 */
        DL_COMP_DAC_CONTROL_COMP_OUT = COMP_CTL2_DACCTL_COMPOUT_SEL,
        /*! DACSW bit controls selection betwen DACCODE0 and DACCODE1. 软件 DACSW 位控制 DACCODE0/DACCODE1 切换。 */
        DL_COMP_DAC_CONTROL_SW = COMP_CTL2_DACCTL_DACSW_SEL,
    } DL_COMP_DAC_CONTROL;

    /*! @enum DL_COMP_DAC_INPUT DAC 输入选择枚举 */
    typedef enum
    {
        /*! DACCODE0 selected as input for 8-bit DAC when DACCTL bit is 1. DACCTL=1 时选择 DACCODE0 作为 DAC 输入。 */
        DL_COMP_DAC_INPUT_DACCODE0 = COMP_CTL2_DACSW_DACCODE0_SEL,
        /*! DACCODE1 selected as input for 8-bit DAC when DACCTL bit is 1. DACCTL=1 时选择 DACCODE1 作为 DAC 输入。 */
        DL_COMP_DAC_INPUT_DACCODE1 = COMP_CTL2_DACSW_DACCODE1_SEL,
    } DL_COMP_DAC_INPUT;

    /*! @enum DL_COMP_OUTPUT 比较器输出状态枚举 */
    typedef enum
    {
        /*! Comparator output is low. 比较器输出低电平。 */
        DL_COMP_OUTPUT_LOW = COMP_STAT_OUT_LOW,
        /*! Comparator output is high. 比较器输出高电平。 */
        DL_COMP_OUTPUT_HIGH = COMP_STAT_OUT_HIGH,
    } DL_COMP_OUTPUT;

    /*! @enum DL_COMP_SUBSCRIBER_INDEX 订阅器索引枚举 */
    typedef enum
    {
        /*! COMP Subscriber index 0. 订阅器索引 0。 */
        DL_COMP_SUBSCRIBER_INDEX_0 = 0,
        /*! COMP Subscriber index 1. 订阅器索引 1。 */
        DL_COMP_SUBSCRIBER_INDEX_1 = 1
    } DL_COMP_SUBSCRIBER_INDEX;

    /*!
     *  @brief  Configuration struct for @ref DL_COMP_init. 比较器初始化配置结构体。
     */
    typedef struct
    {
        /*! The operating mode of the comparator. 比较器工作模式。 */
        DL_COMP_MODE mode;
        /*! The channel inputs to the comparator terminals to enable. 使能的输入通道。 */
        DL_COMP_ENABLE_CHANNEL channelEnable;
        /*! The channel input for the positive terminal of the comparator. 正端输入通道。 */
        DL_COMP_IPSEL_CHANNEL posChannel;
        /*! The channel input for the negative terminal of the comparator. 负端输入通道。 */
        DL_COMP_IMSEL_CHANNEL negChannel;
        /*! The polarity of the comparator output. 输出极性。 */
        DL_COMP_POLARITY polarity;
        /*! The hysteresis of the comparator. 迟滞电压。 */
        DL_COMP_HYSTERESIS hysteresis;
    } DL_COMP_Config;

    /*!
     *  @brief  Configuration struct for @ref DL_COMP_refVoltageInit. 参考电压配置结构体。
     */
    typedef struct
    {
        /*! The operating mode of the reference voltage generator. 参考电压工作模式。 */
        DL_COMP_REF_MODE mode;
        /*! The reference source of the comparator. 参考电压源。 */
        DL_COMP_REF_SOURCE source;
        /*! The terminals the reference voltage is applied to. 参考电压施加端子。 */
        DL_COMP_REF_TERMINAL_SELECT terminalSelect;
        /*! Determines what controls the selection between DACCODE0 and DACCODE1. DAC 输入切换控制源。 */
        DL_COMP_DAC_CONTROL controlSelect;
        /*! If DL_COMP_DAC_CONTROL_SW is enabled, selects between DACCODE0
         * and DACCODE1. 软件控制时的 DAC 输入选择。 */
        DL_COMP_DAC_INPUT inputSelect;
    } DL_COMP_RefVoltageConfig;

    /**
     * @brief Enables the Peripheral Write Enable (PWREN) register for the COMP. 使能 COMP 外设电源与寄存器访问。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  @note For power savings, please refer to @ref DL_COMP_enable
     *
     * @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enablePower(COMP_Regs *comp)
    {
        comp->GPRCM.PWREN = (COMP_PWREN_KEY_UNLOCK_W | COMP_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief Disables the Peripheral Write Enable (PWREN) register for the COMP. 关闭 COMP 外设电源与寄存器访问。
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     *  @note This API does not provide large power savings. For power savings,
     *  please refer to @ref DL_COMP_enable
     *
     * @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disablePower(COMP_Regs *comp)
    {
        comp->GPRCM.PWREN = (COMP_PWREN_KEY_UNLOCK_W | COMP_PWREN_ENABLE_DISABLE);
    }

    /**
     * @brief Returns if the Peripheral Write Enable (PWREN) register for the COMP
     *        is enabled. 查询 COMP 外设电源是否已使能。
     *
     *  Before any peripheral registers can be configured by software, the
     *  peripheral itself must be enabled by writing the ENABLE bit together with
     *  the appropriate KEY value to the peripheral's PWREN register.
     *
     *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
     *  accessible for read/write operations.
     *
     * @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return    The status of the peripheral power. 外设电源状态。
     *
     * @return true if peripheral register access is enabled. 已使能时返回 true。
     * @return false if peripheral register access is disabled. 未使能时返回 false。
     */
    __STATIC_INLINE bool DL_COMP_isPowerEnabled(COMP_Regs *comp)
    {
        return ((comp->GPRCM.PWREN & COMP_PWREN_ENABLE_MASK) ==
                COMP_PWREN_ENABLE_ENABLE);
    }

    /**
     * @brief      Resets comparator peripheral. 复位比较器外设。
     *
     * @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_reset(COMP_Regs *comp)
    {
        comp->GPRCM.RSTCTL =
            (COMP_RSTCTL_KEY_UNLOCK_W | COMP_RSTCTL_RESETSTKYCLR_CLR |
             COMP_RSTCTL_RESETASSERT_ASSERT);
    }

    /**
     * @brief      Returns if comparator was reset. 查询比较器是否经历过复位。
     *
     * @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     * @return true  If peripheral was reset. 已复位时返回 true。
     * @return false If peripheral wasn't reset. 未复位时返回 false。
     */
    __STATIC_INLINE bool DL_COMP_isReset(COMP_Regs *comp)
    {
        return ((comp->GPRCM.STAT & COMP_GPRCM_STAT_RESETSTKY_MASK) ==
                COMP_GPRCM_STAT_RESETSTKY_RESET);
    }

    /**
     *  @brief      Initialize the COMP peripheral. 初始化比较器外设。
     *
     *  Initializes all the common configurable options for the COMP peripheral. Any
     *  other custom configuration can be done after calling this API. The COMP is
     *  not enabled in this API.
     *
     *  @param[in]  comp    Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  config  Configuration for COMP peripheral. COMP 初始化配置结构体。
     */
    __STATIC_INLINE void DL_COMP_init(COMP_Regs *comp, DL_COMP_Config *config)
    {
        DL_Common_updateReg(&comp->CTL0,
                            (uint32_t)config->posChannel | (uint32_t)config->negChannel |
                                (uint32_t)config->channelEnable,
                            COMP_CTL0_IMSEL_MASK | COMP_CTL0_IPSEL_MASK | COMP_CTL0_IPEN_MASK |
                                COMP_CTL0_IMEN_MASK);

        DL_Common_updateReg(&comp->CTL1,
                            (uint32_t)config->mode | (uint32_t)config->polarity |
                                (uint32_t)config->hysteresis,
                            COMP_CTL1_MODE_MASK | COMP_CTL1_HYST_MASK | COMP_CTL1_OUTPOL_MASK);
    }

    /**
     *  @brief      Initialize the comparator reference voltage generator. 初始化比较器参考电压发生器。
     *
     * Initializes all the common configurable options for the comparator
     * reference voltage generator. Any other custom configuration can be done
     * after calling this API.
     *
     *  @param[in]  comp    Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  config  Configuration for the reference voltage generator. 参考电压配置结构体。
     */
    __STATIC_INLINE void DL_COMP_refVoltageInit(
        COMP_Regs *comp, DL_COMP_RefVoltageConfig *config)
    {
        DL_Common_updateReg(&comp->CTL2,
                            (uint32_t)config->mode | (uint32_t)config->source |
                                (uint32_t)config->terminalSelect |
                                (uint32_t)config->controlSelect | (uint32_t)config->inputSelect,
                            COMP_CTL2_REFMODE_MASK | COMP_CTL2_REFSRC_MASK |
                                COMP_CTL2_REFSEL_MASK | COMP_CTL2_DACCTL_MASK |
                                COMP_CTL2_DACSW_MASK);
    }

    /**
     *  @brief      Enable the comparator peripheral. 使能比较器。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enable(COMP_Regs *comp)
    {
        comp->CTL1 |= COMP_CTL1_ENABLE_ON;
    }

    /**
     *  @brief      Checks if the comparator peripheral is enabled. 查询比较器是否已使能。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns the enabled status of the comparator. 比较器使能状态。
     *
     *  @retval     true  The comparator peripheral is enabled. 已使能。
     *  @retval     false The comparator peripheral is disabled. 未使能。
     */
    __STATIC_INLINE bool DL_COMP_isEnabled(COMP_Regs *comp)
    {
        return ((comp->CTL1 & COMP_CTL1_ENABLE_MASK) == COMP_CTL1_ENABLE_ON);
    }

    /**
     *  @brief      Disable the comparator peripheral. 关闭比较器。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disable(COMP_Regs *comp)
    {
        comp->CTL1 &= ~(COMP_CTL1_ENABLE_MASK);
    }

    /**
     *  @brief      Set the comparator operating mode. 设置比较器工作模式。
     *
     * The clock control for comparator is managed by SYSCTL, SYSCTL knows if
     * comparator module is enabled and at also knows if it is in ultra-low power
     * mode or fast mode. User needs to ensure the proper bus clock is selected
     * for different comparator operation mode.
     *   - In @ref DL_COMP_MODE_ULP, bus clock can be LFCLK or any of the high
     *     speed clocks.
     *   - In @ref DL_COMP_MODE_FAST, bus clock cannot be LFCLK.
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  mode  Mode to set the comparator to.
     *                    One of @ref DL_COMP_MODE. 目标比较器模式。
     */
    __STATIC_INLINE void DL_COMP_setCompMode(COMP_Regs *comp, DL_COMP_MODE mode)
    {
        DL_Common_updateReg(&comp->CTL1, (uint32_t)mode, COMP_CTL1_MODE_MASK);
    }

    /**
     *  @brief      Get the mode the comparator is set to. 获取当前比较器工作模式。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The mode the comparator is set to. 当前比较器模式。
     *
     *  @retval     One of @ref DL_COMP_MODE. 比较器模式枚举值。
     */
    __STATIC_INLINE DL_COMP_MODE DL_COMP_getCompMode(COMP_Regs *comp)
    {
        uint32_t mode = (comp->CTL1 & COMP_CTL1_MODE_MASK);

        return (DL_COMP_MODE)(mode);
    }

    /**
     *  @brief      Enable exchange of the comparator inputs, and invert output. 使能输入端互换并反转输出。
     *
     * When enabled, the input signals of comparator positive and negative
     * terminals are exchanged. Additionally, the output signal from the comparator
     * is inverted too. This allows the user to determine or compensate for the
     * comparator input offset voltage.
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enableExchangeInputs(COMP_Regs *comp)
    {
        comp->CTL1 |= COMP_CTL1_EXCH_EXC;
    }

    /**
     *  @brief      Checks if the inputs are exchanged and if output is inverted. 查询输入互换是否已启用。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns if the inputs are exchanged and if output is inverted. 输入互换状态。
     *
     *  @retval     true  The inputs are exchanged and output is inverted. 已启用。
     *  @retval     false The inputs are not exchanged and output is not inverted. 未启用。
     */
    __STATIC_INLINE bool DL_COMP_isExchangeInputsEnabled(COMP_Regs *comp)
    {
        return ((comp->CTL1 & COMP_CTL1_EXCH_MASK) == COMP_CTL1_EXCH_EXC);
    }

    /**
     *  @brief      Disable input exchange and invert output. 关闭输入互换并恢复正常输出。
     *
     * The input signals of the comparator positive and negative terminals are not
     * exchanged, and the output signal from the comparator is not inverted.
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disableExchangeInputs(COMP_Regs *comp)
    {
        comp->CTL1 &= ~(COMP_CTL1_EXCH_MASK);
    }

    /**
     *  @brief      Enable shorting of the comparator inputs. 使能比较器输入端短接。
     *
     * When enabled, shorts the positive and negative input terminals of the
     * comparator.
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enableShortInputTerminals(COMP_Regs *comp)
    {
        comp->CTL1 |= COMP_CTL1_SHORT_SHT;
    }

    /**
     *  @brief      Checks if the comparator inputs are shorted. 查询比较器输入端是否被短接。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns if the comparator inputs are shorted. 输入端短接状态。
     *
     *  @retval     true  The comparator inputs are shorted. 已短接。
     *  @retval     false The comparator inputs are not shorted. 未短接。
     */
    __STATIC_INLINE bool DL_COMP_isInputTerminalsShorted(COMP_Regs *comp)
    {
        return ((comp->CTL1 & COMP_CTL1_SHORT_MASK) == COMP_CTL1_SHORT_SHT);
    }

    /**
     *  @brief      Disable shorting the comparator inputs. 关闭比较器输入端短接。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disableShortInputTerminals(COMP_Regs *comp)
    {
        comp->CTL1 &= ~(COMP_CTL1_SHORT_MASK);
    }

    /**
     *  @brief      Set the interrupt edge for the comparator output. 设置比较器输出中断触发边沿。
     *
     * The interrupt flags COMPIFG and COMPINVIFG are set either on the rising or
     * falling edge of the comparator output.
     *   - In @ref DL_COMP_OUTPUT_INT_EDGE_RISING, rising edge of the comparator
     *     output sets the COMPIFG and falling edge sets the COMPINVIFG.
     *   - In @ref DL_COMP_OUTPUT_INT_EDGE_FALLING, falling edge of the comparator
     *     output sets the COMPIFG and rising edge sets the COMPINVIFG.
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  edge  Interrupt edge for the comparator output.
     *                    One of @ref DL_COMP_OUTPUT_INT_EDGE. 中断边沿选择。
     */
    __STATIC_INLINE void DL_COMP_setOutputInterruptEdge(
        COMP_Regs *comp, DL_COMP_OUTPUT_INT_EDGE edge)
    {
        DL_Common_updateReg(&comp->CTL1, (uint32_t)edge, COMP_CTL1_IES_MASK);
    }

    /**
     *  @brief      Get the interrupt edge for the comparator output. 获取比较器输出中断边沿配置。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The interrupt edge for the comparator output. 中断边沿配置。
     *
     *  @retval     One of @ref DL_COMP_OUTPUT_INT_EDGE. 中断边沿枚举值。
     */
    __STATIC_INLINE DL_COMP_OUTPUT_INT_EDGE DL_COMP_getOutputInterruptEdge(
        COMP_Regs *comp)
    {
        uint32_t interruptEdge = (comp->CTL1 & COMP_CTL1_IES_MASK);

        return (DL_COMP_OUTPUT_INT_EDGE)(interruptEdge);
    }

    /**
     *  @brief      Set the hysteresis voltage. 设置比较器迟滞电压。
     *
     *  @param[in]  comp     Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  voltage  Voltage value to set the hysteresis to. 目标迟滞电压枚举值。
     */
    __STATIC_INLINE void DL_COMP_setHysteresis(
        COMP_Regs *comp, DL_COMP_HYSTERESIS voltage)
    {
        DL_Common_updateReg(&comp->CTL1, (uint32_t)voltage, COMP_CTL1_HYST_MASK);
    }

    /**
     *  @brief      Get the hysteresis voltage. 获取当前比较器迟滞电压。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The hysteresis voltage. 迟滞电压配置。
     *
     *  @retval     One of @ref DL_COMP_HYSTERESIS. 迟滞电压枚举值。
     */
    __STATIC_INLINE DL_COMP_HYSTERESIS DL_COMP_getHysteresis(COMP_Regs *comp)
    {
        uint32_t hysteresis = (comp->CTL1 & COMP_CTL1_HYST_MASK);

        return (DL_COMP_HYSTERESIS)(hysteresis);
    }

    /**
     *  @brief      Set the output polarity. 设置比较器输出极性。
     *
     *  @param[in]  comp      Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  polarity  The polarity of the comparator output. 输出极性选择。
     */
    __STATIC_INLINE void DL_COMP_setOutputPolarity(
        COMP_Regs *comp, DL_COMP_POLARITY polarity)
    {
        DL_Common_updateReg(
            &comp->CTL1, (uint32_t)polarity, COMP_CTL1_OUTPOL_MASK);
    }

    /**
     *  @brief      Get the output polarity. 获取比较器输出极性。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The output polarity. 输出极性。
     *
     *  @retval     One of @ref DL_COMP_POLARITY. 输出极性枚举值。
     */
    __STATIC_INLINE DL_COMP_POLARITY DL_COMP_getOutputPolarity(COMP_Regs *comp)
    {
        uint32_t polarity = (comp->CTL1 & COMP_CTL1_OUTPOL_MASK);

        return (DL_COMP_POLARITY)(polarity);
    }

    /**
     *  @brief      Enable output filter. 使能输出滤波。
     *
     * The output is filtered with an on-chip analog filter. The delay of the
     * filter can be adjusted in four different steps. Refer to the device
     * specific datasheet for the comparator propagation delay with different
     * filter delay settings.
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  delay  Delay of the filter. One of @ref DL_COMP_FILTER_DELAY. 滤波延时等级。
     */
    __STATIC_INLINE void DL_COMP_enableOutputFilter(
        COMP_Regs *comp, DL_COMP_FILTER_DELAY delay)
    {
        comp->CTL1 |= (COMP_CTL1_FLTEN_ENABLE | (uint32_t)delay);
    }

    /**
     *  @brief      Checks if output filter is enabled. 查询输出滤波是否已使能。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns if the output is filtered. 滤波使能状态。
     *
     *  @retval     true  The output is filtered. 已使能。
     *  @retval     false The output is not filtered. 未使能。
     */
    __STATIC_INLINE bool DL_COMP_isOutputFilterEnabled(COMP_Regs *comp)
    {
        return ((comp->CTL1 & COMP_CTL1_FLTEN_MASK) == COMP_CTL1_FLTEN_ENABLE);
    }

    /**
     *  @brief      Disable output filter using on-chip analog filter. 关闭输出滤波。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disableOutputFilter(COMP_Regs *comp)
    {
        comp->CTL1 &= ~(COMP_CTL1_FLTEN_MASK);
    }

    /**
     *  @brief      Get the output filter delay. 获取输出滤波延时。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The output filter delay. 滤波延时配置。
     *
     *  @retval     One of @ref DL_COMP_FILTER_DELAY. 滤波延时枚举值。
     */
    __STATIC_INLINE DL_COMP_FILTER_DELAY DL_COMP_getOutputFilterDelay(
        COMP_Regs *comp)
    {
        uint32_t delay = (comp->CTL1 & COMP_CTL1_FLTDLY_MASK);

        return (DL_COMP_FILTER_DELAY)(delay);
    }

    /**
     *  @brief      Enable the window comparator. 使能窗口比较器。
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enableWindowComparator(COMP_Regs *comp)
    {
        comp->CTL1 |= COMP_CTL1_WINCOMPEN_ON;
    }

    /**
     *  @brief      Checks if the window comparator is enabled. 查询窗口比较器是否已使能。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns if the window comparator is enabled. 窗口比较器使能状态。
     *
     *  @retval     true  The window comparator is enabled. 已使能。
     *  @retval     false The window comparator is not enabled. 未使能。
     */
    __STATIC_INLINE bool DL_COMP_isWindowComparatorEnabled(COMP_Regs *comp)
    {
        return ((comp->CTL1 & COMP_CTL1_WINCOMPEN_MASK) == COMP_CTL1_WINCOMPEN_ON);
    }

    /**
     *  @brief      Disable the window comparator. 关闭窗口比较器。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disableWindowComparator(COMP_Regs *comp)
    {
        comp->CTL1 &= ~(COMP_CTL1_WINCOMPEN_MASK);
    }

#ifdef DEVICE_HAS_DAC_OUT_TO_PIN
    /**
     *  @brief      Enable 8-bit DAC output to pin. 使能 8-bit DAC 输出到引脚。
     *
     * Please check your device datasheet for which pin the 8-bit DAC output is
     * connected to.
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enableDACOutputToPin(COMP_Regs *comp)
    {
        comp->CTL1 |= COMP_CTL1_DACOUTEN_ENABLE;
    }

    /**
     *  @brief      Checks if 8-bit DAC output to pin is enabled. 查询 DAC 输出到引脚是否已使能。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns if the 8-bit DAC output is enabled. DAC 输出使能状态。
     *
     *  @retval     true  The 8-bit DAC output is enabled. 已使能。
     *  @retval     false The 8-bit DAC output is not enabled. 未使能。
     */
    __STATIC_INLINE bool DL_COMP_isDACOutputToPinEnabled(COMP_Regs *comp)
    {
        return (
            (comp->CTL1 & COMP_CTL1_DACOUTEN_MASK) == COMP_CTL1_DACOUTEN_ENABLE);
    }

    /**
     *  @brief      Disable 8-bit DAC output to pin. 关闭 DAC 输出到引脚。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disableDACOutputToPin(COMP_Regs *comp)
    {
        comp->CTL1 &= ~(COMP_CTL1_DACOUTEN_MASK);
    }

#endif /* DEVICE_HAS_DAC_OUT_TO_PIN */

    /**
     *  @brief      Set the enabled channels for the comparator terminals. 设置比较器输入通道使能。
     *
     * Enables the selected analog input channels for the positive and
     * negative comparator terminals.
     *
     *  @param[in]  comp      Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  channels  Input channels to enable. 输入通道使能位组合。
     */
    __STATIC_INLINE void DL_COMP_setEnabledInputChannels(
        COMP_Regs *comp, DL_COMP_ENABLE_CHANNEL channels)
    {
        DL_Common_updateReg(&comp->CTL0, (uint32_t)channels,
                            (COMP_CTL0_IMEN_MASK | COMP_CTL0_IPEN_MASK));
    }

    /**
     *  @brief      Get the enabled input channels. 获取已使能的输入通道。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The enabled input channels. 已使能的输入通道。
     *
     *  @retval     One of @ref DL_COMP_ENABLE_CHANNEL. 输入通道使能枚举值。
     */
    __STATIC_INLINE DL_COMP_ENABLE_CHANNEL DL_COMP_getEnabledInputChannels(
        COMP_Regs *comp)
    {
        uint32_t channel =
            (comp->CTL0 & (COMP_CTL0_IMEN_MASK | COMP_CTL0_IPEN_MASK));

        return (DL_COMP_ENABLE_CHANNEL)(channel);
    }

    /**
     *  @brief      Set the channel input for the positive terminal. 设置比较器正端输入通道。
     *
     * This API sets the IPSEL control bit to select the channel input for the
     * positive terminal. Please see the device datasheet for more details on each
     * channel input for each comparator instance.
     *
     * Note this API does not enable the positive terminal. Call
     * @ref DL_COMP_setEnabledInputChannels to enable the terminals.
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  input  Channel input for the positive terminal. 正端通道选择。
     */
    __STATIC_INLINE void DL_COMP_setPositiveChannelInput(
        COMP_Regs *comp, DL_COMP_IPSEL_CHANNEL input)
    {
        DL_Common_updateReg(&comp->CTL0, (uint32_t)input, COMP_CTL0_IPSEL_MASK);
    }

    /**
     *  @brief      Get the channel input for the positive terminal. 获取比较器正端输入通道。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The channel input for the positive terminal. 正端通道。
     *
     *  @retval     One of @ref DL_COMP_IPSEL_CHANNEL. 正端通道枚举值。
     */
    __STATIC_INLINE DL_COMP_IPSEL_CHANNEL DL_COMP_getPositiveChannelInput(
        COMP_Regs *comp)
    {
        uint32_t channel = (comp->CTL0 & COMP_CTL0_IPSEL_MASK);

        return (DL_COMP_IPSEL_CHANNEL)(channel);
    }

    /**
     *  @brief      Set the channel input for the negative terminal. 设置比较器负端输入通道。
     *
     * This API sets the IMSEL control bit to select the channel input for the
     * negative terminal. Please see the device TRM for more details on each
     * channel input for each comparator instance.
     *
     * Note this API does not enable the negative terminal. Call
     * @ref DL_COMP_setEnabledInputChannels to enable the terminals.
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  input  Channel input for the negative terminal. 负端通道选择。
     */
    __STATIC_INLINE void DL_COMP_setNegativeChannelInput(
        COMP_Regs *comp, DL_COMP_IMSEL_CHANNEL input)
    {
        DL_Common_updateReg(&comp->CTL0, (uint32_t)input, COMP_CTL0_IMSEL_MASK);
    }

    /**
     *  @brief      Get the channel input for the negative terminal. 获取比较器负端输入通道。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The channel input for the negative terminal. 负端通道。
     *
     *  @retval     One of @ref DL_COMP_IMSEL_CHANNEL. 负端通道枚举值。
     */
    __STATIC_INLINE DL_COMP_IMSEL_CHANNEL DL_COMP_getNegativeChannelInput(
        COMP_Regs *comp)
    {
        uint32_t channel = (comp->CTL0 & COMP_CTL0_IMSEL_MASK);

        return (DL_COMP_IMSEL_CHANNEL)(channel);
    }

    /**
     *  @brief      Set the mode for the reference voltage. 设置参考电压工作模式。
     *
     * This bit requests ULP_REF bandgap operation in static mode or sampled mode.
     * The local reference buffer and 8-bit DAC inside comparator module are also
     * configured accordingly.
     *     - In @ref DL_COMP_REF_MODE_STATIC, operation offers higher accuracy but
     *       consumes higher current.
     *     - In @ref DL_COMP_REF_MODE_SAMPLED, operation consumes lower current but
     *       with relaxed reference voltage accuracy.
     * Comparator requests for reference voltage from ULP_REF only when REFLVL > 0.
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  mode   Mode to set the reference voltage to. 参考电压模式。
     */
    __STATIC_INLINE void DL_COMP_setReferenceMode(
        COMP_Regs *comp, DL_COMP_REF_MODE mode)
    {
        DL_Common_updateReg(&comp->CTL2, (uint32_t)mode, COMP_CTL2_REFMODE_MASK);
    }

    /**
     *  @brief      Get the mode for the reference voltage. 获取参考电压工作模式。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The mode of the reference voltage. 参考电压模式。
     *
     *  @retval     One of @ref DL_COMP_REF_MODE. 参考电压模式枚举值。
     */
    __STATIC_INLINE DL_COMP_REF_MODE DL_COMP_getReferenceMode(COMP_Regs *comp)
    {
        uint32_t mode = (comp->CTL2 & COMP_CTL2_REFMODE_MASK);

        return (DL_COMP_REF_MODE)(mode);
    }

    /**
     *  @brief      Set the reference source for the comparator. 设置比较器参考电压源。
     *
     *  @param[in]  comp     Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  source   Reference source for the comparator. 参考源选择。
     */
    __STATIC_INLINE void DL_COMP_setReferenceSource(
        COMP_Regs *comp, DL_COMP_REF_SOURCE source)
    {
        DL_Common_updateReg(&comp->CTL2, (uint32_t)source, COMP_CTL2_REFSRC_MASK);
    }

    /**
     *  @brief      Get the reference source for the comparator. 获取比较器参考电压源。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The reference source. 参考源。
     *
     *  @retval     One of @ref DL_COMP_REF_SOURCE. 参考源枚举值。
     */
    __STATIC_INLINE DL_COMP_REF_SOURCE DL_COMP_getReferenceSource(COMP_Regs *comp)
    {
        uint32_t source = (comp->CTL2 & COMP_CTL2_REFSRC_MASK);

        return (DL_COMP_REF_SOURCE)(source);
    }

    /**
     *  @brief      Set the comparator terminal the reference voltage is applied to. 设置参考电压施加端子。
     *
     * The reference voltage generator output can be applied to either the positive
     * terminal or negative terminal of the comparator. When the reference voltage
     * generator output is applied on a comparator terminal using REFSEL bit and
     * the comparator channel (from device pins or from internal analog modules) is
     * also selected on the same terminal using IPSEL/IPEN or IMSEL/IMEN bits, then
     * the comparator channel selection takes precedence.
     *
     *  @param[in]  comp      Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  terminal  Terminal the reference voltage is applied to. 参考电压目标端子。
     */
    __STATIC_INLINE void DL_COMP_setReferenceCompTerminal(
        COMP_Regs *comp, DL_COMP_REF_TERMINAL_SELECT terminal)
    {
        DL_Common_updateReg(
            &comp->CTL2, (uint32_t)terminal, COMP_CTL2_REFSEL_MASK);
    }

    /**
     *  @brief      Get the comparator terminal the reference voltage is applied to. 获取参考电压施加端子。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The terminal the reference voltage is applied to. 参考电压施加端子。
     *
     *  @retval     One of @ref DL_COMP_REF_TERMINAL_SELECT. 端子选择枚举值。
     */
    __STATIC_INLINE DL_COMP_REF_TERMINAL_SELECT DL_COMP_getReferenceCompTerminal(
        COMP_Regs *comp)
    {
        uint32_t terminal = (comp->CTL2 & COMP_CTL2_REFSEL_MASK);

        return (DL_COMP_REF_TERMINAL_SELECT)(terminal);
    }

    /**
     *  @brief      Set the blanking source for the comparator. 设置比较器消隐源。
     *
     *  @param[in]  comp     Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  source   Blanking source for the comparator. 消隐源选择。
     */
    __STATIC_INLINE void DL_COMP_setBlankingSource(
        COMP_Regs *comp, DL_COMP_BLANKING_SOURCE source)
    {
        DL_Common_updateReg(
            &comp->CTL2, (uint32_t)source, COMP_CTL2_BLANKSRC_MASK);
    }

    /**
     *  @brief      Get the blanking source for the comparator. 获取比较器消隐源。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The blanking source. 消隐源。
     *
     *  @retval     One of @ref DL_COMP_BLANKING_SOURCE. 消隐源枚举值。
     */
    __STATIC_INLINE DL_COMP_BLANKING_SOURCE DL_COMP_getBlankingSource(
        COMP_Regs *comp)
    {
        uint32_t source = (comp->CTL2 & COMP_CTL2_BLANKSRC_MASK);

        return (DL_COMP_BLANKING_SOURCE)(source);
    }

    /**
     *  @brief      Select the source for DAC control. 选择 DAC 控制源。
     *
     * The DACCTL bit determines if the comparator output or a software control
     * bit, DACSW, selects between DACCODE0 and DACCODE1 bits as the input to
     * the DAC.
     *
     *  @param[in]  comp     Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  control  What controls the inputs to the DAC.
     *                       One of @ref DL_COMP_DAC_CONTROL. DAC 控制源选择。
     */
    __STATIC_INLINE void DL_COMP_setDACControl(
        COMP_Regs *comp, DL_COMP_DAC_CONTROL control)
    {
        DL_Common_updateReg(
            &comp->CTL2, (uint32_t)control, COMP_CTL2_DACCTL_MASK);
    }

    /**
     *  @brief      Get what controls the input to the DAC. 获取 DAC 控制源。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The control input. DAC 控制源。
     *
     *  @retval     One of @ref DL_COMP_DAC_CONTROL. DAC 控制源枚举值。
     */
    __STATIC_INLINE DL_COMP_DAC_CONTROL DL_COMP_getDACControl(COMP_Regs *comp)
    {
        uint32_t control = (comp->CTL2 & COMP_CTL2_DACCTL_MASK);

        return (DL_COMP_DAC_CONTROL)(control);
    }

    /**
     *  @brief      Set whether DACCODE0 or DACCODE1 is the input to the DAC. 设置 DAC 输入码来源。
     *
     * @pre The DACCTL bit must be set to 1 in order to program the DACSW bit to
     * select between DACCODE0 or DACCODE1.
     *
     * @sa          DL_COMP_setDACControl
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  input  The input to the DAC. One of @ref DL_COMP_DAC_INPUT. DAC 输入选择。
     */
    __STATIC_INLINE void DL_COMP_setDACInput(
        COMP_Regs *comp, DL_COMP_DAC_INPUT input)
    {
        DL_Common_updateReg(&comp->CTL2, (uint32_t)input, COMP_CTL2_DACSW_MASK);
    }

    /**
     *  @brief      Get whether DACCODE0 or DACCODE1 is the input to the DAC. 获取 DAC 输入来源。
     *
     * @pre The DACCTL bit must be set to 1 in order to program the DACSW bit to
     * select between DACCODE0 or DACCODE1.
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The input to the DAC. DAC 输入来源。
     *
     *  @retval     One of @ref DL_COMP_DAC_INPUT. DAC 输入选择枚举值。
     */
    __STATIC_INLINE DL_COMP_DAC_INPUT DL_COMP_getDACInput(COMP_Regs *comp)
    {
        uint32_t input = (comp->CTL2 & COMP_CTL2_DACSW_MASK);

        return (DL_COMP_DAC_INPUT)(input);
    }

    /**
     *  @brief      Set the 8-bit DAC input code through DACCODE0. 设置 DACCODE0 的 8-bit 输入码。
     *
     * Sets the first 8-bit DAC code through DACCODE0.  When the DAC code is 0x0
     * the DAC output will be selected reference voltage x 1/256 V. When the DAC
     * code is 0xFF the DAC output will be selected reference voltage x 255/256.
     *
     * @pre The DACCTL bit determines what controls the selection between
     * DACCODE0 and DACCODE1.
     * @pre If DACCTL is 1, then the DACSW bit is used to select between DACCODE0
     * or DACCODE1.
     *
     * @sa          DL_COMP_setDACControl
     * @sa          DL_COMP_setDACInput
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  value  Code to set for codeDACCODE0.
     *                     Value between 0x0 - 0xFF. DACCODE0 码值（0x00~0xFF）。
     */
    __STATIC_INLINE void DL_COMP_setDACCode0(COMP_Regs *comp, uint32_t value)
    {
        DL_Common_updateReg(&comp->CTL3, value, COMP_CTL3_DACCODE0_MASK);
    }

    /**
     *  @brief      Get the code value of DACCODE0. 读取 DACCODE0 码值。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The code value that DACCODE0 is set to. DACCODE0 码值。
     *
     *  @retval     Value between 0x0 - 0xFF. 码值范围 0x00~0xFF。
     */
    __STATIC_INLINE uint32_t DL_COMP_getDACCode0(COMP_Regs *comp)
    {
        return (comp->CTL3 & COMP_CTL3_DACCODE0_MASK);
    }

    /**
     *  @brief      Set the 8-bit DAC input code through DACCODE1. 设置 DACCODE1 的 8-bit 输入码。
     *
     * Sets the second 8-bit DAC code through DACCODE1.  When the DAC code is 0x0
     * the DAC output will be selected reference voltage x 1/256 V. When the DAC
     * code is 0xFF the DAC output will be selected reference voltage x 255/256.
     *
     * @pre The DACCTL bit determines what controls the selection between
     * DACCODE0 and DACCODE1.
     * @pre If DACCTL is 1, then the DACSW bit is used to select between DACCODE0
     * or DACCODE1.
     *
     * @sa          DL_COMP_setDACControl
     * @sa          DL_COMP_setDACInput
     *
     *  @param[in]  comp   Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  value  Code to set for codeDACCODE1.
     *                     Value between 0x0 - 0xFF. DACCODE1 码值（0x00~0xFF）。
     */
    __STATIC_INLINE void DL_COMP_setDACCode1(COMP_Regs *comp, uint32_t value)
    {
        DL_Common_updateReg(&comp->CTL3, (value << COMP_CTL3_DACCODE1_OFS),
                            COMP_CTL3_DACCODE1_MASK);
    }

    /**
     *  @brief      Get the code value of DACCODE1. 读取 DACCODE1 码值。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     The code value that DACCODE1 is set to. DACCODE1 码值。
     *
     *  @retval     Value between 0x0 - 0xFF. 码值范围 0x00~0xFF。
     */
    __STATIC_INLINE uint32_t DL_COMP_getDACCode1(COMP_Regs *comp)
    {
        return ((comp->CTL3 & COMP_CTL3_DACCODE1_MASK) >> COMP_CTL3_DACCODE1_OFS);
    }

    /**
     *  @brief      Gets the comparator output. 获取比较器输出状态。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns the output status of the comparator. One of @ref DL_COMP_OUTPUT. 比较器输出状态。
     */
    __STATIC_INLINE DL_COMP_OUTPUT DL_COMP_getComparatorOutput(COMP_Regs *comp)
    {
        uint32_t output = (comp->STAT & COMP_STAT_OUT_MASK);

        return (DL_COMP_OUTPUT)(output);
    }

    /**
     *  @brief      Enable sampled mode of the comparator. 使能比较器采样模式。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_enableSampledMode(COMP_Regs *comp)
    {
        comp->CTL2 |= COMP_CTL2_SAMPMODE_ENABLE;
    }

    /**
     *  @brief      Checks if sampled mode is enabled. 查询采样模式是否已启用。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *
     *  @return     Returns if sampled mode is enabled. 采样模式状态。
     *
     *  @retval     true  Sampled mode is enabled. 已启用。
     *  @retval     false Sampled mode is disabled. 未启用。
     */
    __STATIC_INLINE bool DL_COMP_isSampledModeEnabled(COMP_Regs *comp)
    {
        return (
            (comp->CTL2 & COMP_CTL2_SAMPMODE_MASK) == COMP_CTL2_SAMPMODE_ENABLE);
    }

    /**
     *  @brief      Disable sampled mode. 关闭采样模式。
     *
     *  @param[in]  comp  Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     */
    __STATIC_INLINE void DL_COMP_disableSampledMode(COMP_Regs *comp)
    {
        comp->CTL2 &= ~(COMP_CTL2_SAMPMODE_MASK);
    }

    /**
     *  @brief Sets the event subscriber channel id. 设置事件订阅通道号。
     *
     *  @param[in]  comp Pointer to the register overlay for the
     *                      peripheral. COMP 外设寄存器基址。
     *  @param[in]  index   Specifies the register event index to be configured. 订阅器索引。
     *  @param[in]  chanID  Channel ID number. Valid range 0-15. If ChanID == 0
     *                      subscriber is disconnected. 事件通道号（0 表示断开）。
     */
    __STATIC_INLINE void DL_COMP_setSubscriberChanID(
        COMP_Regs *comp, DL_COMP_SUBSCRIBER_INDEX index, uint8_t chanID)
    {
        volatile uint32_t *pReg = &comp->FSUB_0;

        *(pReg + (uint32_t)index) = (chanID & COMP_FSUB_0_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event subscriber channel id. 获取事件订阅通道号。
     *
     *  @param[in]  comp Pointer to the register overlay for the peripheral. COMP 外设寄存器基址。
     *  @param[in]  index  Specifies the register event index to be configured. 订阅器索引。
     *
     *  @return     Event subscriber channel ID. 事件订阅通道号。
     */
    __STATIC_INLINE uint8_t DL_COMP_getSubscriberChanID(
        COMP_Regs *comp, DL_COMP_SUBSCRIBER_INDEX index)
    {
        volatile uint32_t *pReg = &comp->FSUB_0;

        return ((uint8_t)(*(pReg + (uint32_t)index) & COMP_FSUB_0_CHANID_MASK));
    }

    /**
     *  @brief      Enable COMP interrupts. 使能 COMP 中断。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_COMP_INTERRUPT. 中断位掩码。
     */
    __STATIC_INLINE void DL_COMP_enableInterrupt(
        COMP_Regs *comp, uint32_t interruptMask)
    {
        comp->CPU_INT.IMASK |= interruptMask;
    }

    /**
     *  @brief      Disable COMP interrupts. 禁用 COMP 中断。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
     *                             @ref DL_COMP_INTERRUPT. 中断位掩码。
     */
    __STATIC_INLINE void DL_COMP_disableInterrupt(
        COMP_Regs *comp, uint32_t interruptMask)
    {
        comp->CPU_INT.IMASK &= ~(interruptMask);
    }

    /**
     *  @brief      Check which COMP interrupts are enabled. 查询已使能的 COMP 中断。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_COMP_INTERRUPT. 待查询中断位掩码。
     *
     *  @return     Which of the requested COMP interrupts are enabled. 已使能的中断位图。
     *
     *  @retval     Bitwise OR of @ref DL_COMP_INTERRUPT values. 已使能中断位图。
     */
    __STATIC_INLINE uint32_t DL_COMP_getEnabledInterrupts(
        COMP_Regs *comp, uint32_t interruptMask)
    {
        return (comp->CPU_INT.IMASK & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of enabled COMP interrupts. 查询已使能中断的挂起状态。
     *
     *  Checks if any of the COMP interrupts that were previously enabled are
     *  pending.
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_COMP_INTERRUPT. 待检查中断位掩码。
     *
     *  @return     Which of the requested COMP interrupts are pending. 挂起中断位图。
     *
     *  @retval     Bitwise OR of @ref DL_COMP_INTERRUPT values. 挂起中断位图。
     *
     *  @sa         DL_COMP_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_COMP_getEnabledInterruptStatus(
        COMP_Regs *comp, uint32_t interruptMask)
    {
        return (comp->CPU_INT.MIS & interruptMask);
    }

    /**
     *  @brief      Check interrupt flag of any COMP interrupt. 获取原始中断挂起状态。
     *
     *  Checks if any of the COMP interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_COMP_INTERRUPT. 待检查中断位掩码。
     *
     *  @return     Which of the requested COMP interrupts are pending. 原始挂起中断位图。
     *
     *  @retval     Bitwise OR of @ref DL_COMP_INTERRUPT values. 原始挂起中断位图。
     */
    __STATIC_INLINE uint32_t DL_COMP_getRawInterruptStatus(
        COMP_Regs *comp, uint32_t interruptMask)
    {
        return (comp->CPU_INT.RIS & interruptMask);
    }

    /**
     *  @brief      Get highest priority pending COMP interrupt. 获取最高优先级挂起中断。
     *
     *  Checks if any of the COMP interrupts are pending. Interrupts do not have to
     *  be previously enabled.
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @return     The highest priority pending COMP interrupt. One of @ref
     *               DL_COMP_IIDX. 最高优先级挂起中断索引。
     */
    __STATIC_INLINE DL_COMP_IIDX DL_COMP_getPendingInterrupt(COMP_Regs *comp)
    {
        return (DL_COMP_IIDX)(comp->CPU_INT.IIDX);
    }

    /**
     *  @brief      Clear pending COMP interrupts. 清除 COMP 中断挂起标志。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_COMP_INTERRUPT. 待清除中断位掩码。
     */
    __STATIC_INLINE void DL_COMP_clearInterruptStatus(
        COMP_Regs *comp, uint32_t interruptMask)
    {
        comp->CPU_INT.ICLR = interruptMask;
    }

    /**
     *  @brief Sets the COMP event publisher channel ID. 设置事件发布通道号。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  chanID         Channel ID number. Valid range 0-15.
     *                             If ChanID == 0, publisher is disconnected. 发布通道号（0 表示断开）。
     */
    __STATIC_INLINE void DL_COMP_setPublisherChanID(
        COMP_Regs *comp, uint8_t chanID)
    {
        comp->FPUB_1 = (chanID & COMP_FPUB_1_CHANID_MAXIMUM);
    }

    /**
     *  @brief Gets the event publisher channel ID. 获取事件发布通道号。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @return     Event publisher channel ID. 事件发布通道号。
     */
    __STATIC_INLINE uint8_t DL_COMP_getPublisherChanID(COMP_Regs *comp)
    {
        return ((uint8_t)((comp->FPUB_1) & COMP_FPUB_1_CHANID_MAXIMUM));
    }

    /**
     *  @brief      Enable COMP event. 使能 COMP 事件。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_COMP_EVENT. 事件位掩码。
     */
    __STATIC_INLINE void DL_COMP_enableEvent(COMP_Regs *comp, uint32_t eventMask)
    {
        comp->GEN_EVENT.IMASK |= (eventMask);
    }

    /**
     *  @brief      Disable COMP event. 禁用 COMP 事件。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of interrupts to enable. Bitwise OR of
     *                             @ref DL_COMP_EVENT. 事件位掩码。
     */
    __STATIC_INLINE void DL_COMP_disableEvent(COMP_Regs *comp, uint32_t eventMask)
    {
        comp->GEN_EVENT.IMASK &= ~(eventMask);
    }

    /**
     *  @brief      Check which COMP events are enabled. 查询已使能的 COMP 事件。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_COMP_EVENT. 待查询事件位掩码。
     *
     *  @return     Which of the requested COMP interrupts are enabled. 已使能的事件位图。
     *
     *  @retval     Bitwise OR of @ref DL_COMP_EVENT values. 已使能事件位图。
     */
    __STATIC_INLINE uint32_t DL_COMP_getEnabledEvents(
        COMP_Regs *comp, uint32_t eventMask)
    {
        return ((comp->GEN_EVENT.IMASK) & (eventMask));
    }

    /**
     *  @brief      Check event flag of enabled COMP event. 查询已使能事件的挂起状态。
     *
     *  Checks if any of the COMP events that were previously enabled are
     *  pending.
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_COMP_EVENT. 待检查事件位掩码。
     *
     *  @return     Which of the requested COMP interrupts are pending. 挂起事件位图。
     *
     *  @retval     Bitwise OR of @ref DL_COMP_EVENT values. 挂起事件位图。
     *
     *  @sa         DL_COMP_enableInterrupt
     */
    __STATIC_INLINE uint32_t DL_COMP_getEnabledEventStatus(
        COMP_Regs *comp, uint32_t eventMask)
    {
        return ((comp->GEN_EVENT.MIS) & eventMask);
    }

    /**
     *  @brief      Check interrupt flag of any COMP event. 获取原始事件挂起状态。
     *
     *  Checks if any events are pending. Events do not have to
     *  be previously enabled.
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of interrupts to check. Bitwise OR of
     *                             @ref DL_COMP_EVENT. 待检查事件位掩码。
     *
     *  @return     Which of the requested COMP interrupts are pending. 原始挂起事件位图。
     *
     *  @retval     Bitwise OR of @ref DL_COMP_EVENT values. 原始挂起事件位图。
     */
    __STATIC_INLINE uint32_t DL_COMP_getRawEventsStatus(
        COMP_Regs *comp, uint32_t eventMask)
    {
        return ((comp->GEN_EVENT.RIS) & eventMask);
    }

    /**
     *  @brief      Clear pending COMP events. 清除 COMP 事件挂起标志。
     *
     *  @param[in]  comp           Pointer to the register overlay for the
     *                             peripheral. COMP 外设寄存器基址。
     *  @param[in]  eventMask      Bit mask of interrupts to clear. Bitwise OR of
     *                             @ref DL_COMP_EVENT. 待清除事件位掩码。
     */
    __STATIC_INLINE void DL_COMP_clearEventsStatus(
        COMP_Regs *comp, uint32_t eventMask)
    {
        comp->GEN_EVENT.ICLR |= (eventMask);
    }

    /* clang-format on */

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_COMP__ */

#endif /* ti_dl_m0p_dl_comp__include */
/** @}*/
