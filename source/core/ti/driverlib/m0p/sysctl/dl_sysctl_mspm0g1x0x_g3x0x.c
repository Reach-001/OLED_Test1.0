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
 *
 *  MSPM0G1X0X_G3X0X 系统控制 (SysCtl) 模块驱动实现
 *  包含 PLL 配置、LFCLK/HFCLK 源切换、MCLK 源切换、FCC 配置、电源策略查询等功能
 */

#include <stdint.h>
#include <ti/devices/DeviceFamily.h>

#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G1X0X_G3X0X)

#include <ti/driverlib/m0p/dl_core.h>
#include <ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g1x0x_g3x0x.h>

/** @brief  配置系统 PLL 输出频率
 *  @param[in]  config  PLL 配置结构体指针 */
void DL_SYSCTL_configSYSPLL(const DL_SYSCTL_SYSPLLConfig *config)
{
    /* PLL 配置在低级复位中保持，先禁用 PLL 确保一致性 */
    DL_SYSCTL_disableSYSPLL();

    /* 等待 SYSPLL 确认已禁用 */
    while ((DL_SYSCTL_getClockStatus() & (DL_SYSCTL_CLK_STATUS_SYSPLL_OFF)) !=
           (DL_SYSCTL_CLK_STATUS_SYSPLL_OFF)) {
        ;
    }

    // 设置 SYSPLL 参考时钟源
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG0,
        ((uint32_t) config->sysPLLRef), SYSCTL_SYSPLLCFG0_SYSPLLREF_MASK);

    // 设置预分频器 PDIV（分频参考时钟）
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG1, ((uint32_t) config->pDiv),
        SYSCTL_SYSPLLCFG1_PDIV_MASK);

    // 保存 CPUSS CTL 状态并禁用缓存（从 Flash 读取 PLL 参数需要）
    uint32_t ctlTemp = DL_CORE_getInstructionConfig();
    DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
        DL_CORE_LITERAL_CACHE_ENABLED);

    // 根据输入频率从 Flash 加载 SYSPLLPARAM0/1 调谐寄存器
    SYSCTL->SOCLOCK.SYSPLLPARAM0 =
        *(volatile uint32_t *) ((uint32_t) config->inputFreq);
    SYSCTL->SOCLOCK.SYSPLLPARAM1 =
        *(volatile uint32_t *) ((uint32_t) config->inputFreq + (uint32_t) 0x4);

    // 恢复 CPUSS CTL 状态
    CPUSS->CTL = ctlTemp;

    // 设置反馈分频器 QDIV（乘法器，决定输出频率）
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG1,
        ((config->qDiv << SYSCTL_SYSPLLCFG1_QDIV_OFS) &
            SYSCTL_SYSPLLCFG1_QDIV_MASK),
        SYSCTL_SYSPLLCFG1_QDIV_MASK);

    // 写入输出分频器、使能输出、MCLK 源到 SYSPLLCFG0
    DL_Common_updateReg(&SYSCTL->SOCLOCK.SYSPLLCFG0,
        (((config->rDivClk2x << SYSCTL_SYSPLLCFG0_RDIVCLK2X_OFS) &
             SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK) |
            ((config->rDivClk1 << SYSCTL_SYSPLLCFG0_RDIVCLK1_OFS) &
                SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK) |
            ((config->rDivClk0 << SYSCTL_SYSPLLCFG0_RDIVCLK0_OFS) &
                SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK) |
            config->enableCLK2x | config->enableCLK1 | config->enableCLK0 |
            (uint32_t) config->sysPLLMCLK),
        (SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK | SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK |
            SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK2X_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK1_MASK |
            SYSCTL_SYSPLLCFG0_ENABLECLK0_MASK |
            SYSCTL_SYSPLLCFG0_MCLK2XVCO_MASK));

    // 使能 SYSPLL
    DL_SYSCTL_enableSYSPLL();

    // 等待 SYSPLL 启动稳定
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_SYSPLLGOOD_MASK) !=
           DL_SYSCTL_CLK_STATUS_SYSPLL_GOOD) {
        ;
    }
}

/** @brief  将 LFCLK 源切换为外部晶振 LFXT
 *  @param[in]  config  LFCLK 配置结构体指针 */
void DL_SYSCTL_setLFCLKSourceLFXT(const DL_SYSCTL_LFCLKConfig *config)
{
    // 配置低电容选项和驱动强度
    DL_Common_updateReg(&SYSCTL->SOCLOCK.LFCLKCFG,
        ((uint32_t) config->lowCap << SYSCTL_LFCLKCFG_LOWCAP_OFS) |
            (uint32_t) config->xt1Drive,
        (SYSCTL_LFCLKCFG_XT1DRIVE_MASK | SYSCTL_LFCLKCFG_LOWCAP_MASK));
    // 启动 LFXT 振荡器
    SYSCTL->SOCLOCK.LFXTCTL =
        (SYSCTL_LFXTCTL_KEY_VALUE | SYSCTL_LFXTCTL_STARTLFXT_TRUE);
    // 等待 LFXT 振荡器稳定，若无法稳定请检查硬件/IOMUX 设置
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_LFXTGOOD_MASK) !=
           DL_SYSCTL_CLK_STATUS_LFXT_GOOD) {
        ;
    }
    if (config->monitor) {
        // 使能 LFCLK 故障监控
        SYSCTL->SOCLOCK.LFCLKCFG |= SYSCTL_LFCLKCFG_MONITOR_ENABLE;
    }

    // 将 LFCLK 源从 LFOSC 切换为 LFXT
    SYSCTL->SOCLOCK.LFXTCTL =
        (SYSCTL_LFXTCTL_KEY_VALUE | SYSCTL_LFXTCTL_SETUSELFXT_TRUE);
}

/** @brief  将 MCLK 源从 SYSOSC 切换为 LFCLK
 *  @param[in]  disableSYSOSC  是否关闭 SYSOSC */
void DL_SYSCTL_switchMCLKfromSYSOSCtoLFCLK(bool disableSYSOSC)
{
    if (disableSYSOSC == false) {
        // 若保留 SYSOSC，将其恢复为基频 32MHz
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
        SYSCTL->SOCLOCK.SYSOSCCFG &= ~SYSCTL_SYSOSCCFG_DISABLE_ENABLE;
    } else {
        // 关闭 SYSOSC
        SYSCTL->SOCLOCK.SYSOSCCFG |= SYSCTL_SYSOSCCFG_DISABLE_ENABLE;
    }
    // 切换 MCLK 源为 LFCLK
    SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USELFCLK_ENABLE;

    // 验证 MCLK 源已切换为 LFCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_CURMCLKSEL_MASK) !=
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_LFCLK) {
        ;
    }
}

/** @brief  将 MCLK 源从 LFCLK 切换为 SYSOSC */
void DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC(void)
{
    // 清除 SYSOSCCFG.DISABLE 使 SYSOSC 恢复运行
    // 清除 MCLKCFG.USELFCLK 将 MCLK 源从 LFCLK 切回 SYSOSC
    SYSCTL->SOCLOCK.SYSOSCCFG &= ~SYSCTL_SYSOSCCFG_DISABLE_ENABLE;
    SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_USELFCLK_ENABLE;

    // 验证 MCLK 源已不再是 LFCLK
    while (((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_CURMCLKSEL_MASK) ==
            DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_LFCLK)) {
        ;
    }
}

/** @brief  将 MCLK 源从 SYSOSC 切换为 HSCLK
 *  @param[in]  source  期望的高速时钟源 */
void DL_SYSCTL_switchMCLKfromSYSOSCtoHSCLK(DL_SYSCTL_HSCLK_SOURCE source)
{
    // 假设所需的高速时钟源已按要求使能（SYSPLL、HFXT、HFCLK_IN）
    // 选择期望的 HSCLK 源
    DL_SYSCTL_setHSCLKSource(source);

    // 验证 HSCLK 源有效
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HSCLKGOOD_MASK) !=
           DL_SYSCTL_CLK_STATUS_HSCLK_GOOD) {
        ;
    }

    // 切换 MCLK 为 HSCLK
    SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

    // 验证 MCLK 源已切换为 HSCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HSCLKMUX_MASK) !=
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_HSCLK) {
        ;
    }
}

/** @brief  将 MCLK 源从 HSCLK 切换为 SYSOSC */
void DL_SYSCTL_switchMCLKfromHSCLKtoSYSOSC(void)
{
    // 切换 MCLK 为 SYSOSC
    SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

    // 验证 MCLK 源已不再是 HSCLK
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HSCLKMUX_MASK) ==
           DL_SYSCTL_CLK_STATUS_MCLK_SOURCE_HSCLK) {
        ;
    }
}

/** @brief  将 HFCLK 源切换为外部高频晶振 HFXT（使用默认参数）
 *  @param[in]  range  HFXT 频率范围 */
void DL_SYSCTL_setHFCLKSourceHFXT(DL_SYSCTL_HFXT_RANGE range)
{
    /* 某些晶振配置在低级复位中保持，先禁用 HFXT 确保一致性 */
    DL_SYSCTL_disableHFXT();

    DL_SYSCTL_setHFXTFrequencyRange(range);
    /* 根据数据手册推荐设置启动时间 ~0.512ms */
    DL_SYSCTL_setHFXTStartupTime(8);
    SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_HFXTEN_ENABLE;
    DL_SYSCTL_enableHFCLKStartupMonitor();
    /* 等待 HFXT 振荡器稳定，若无法稳定请检查硬件/IOMUX 设置 */
    while ((DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HFCLKGOOD_MASK) !=
           DL_SYSCTL_CLK_STATUS_HFCLK_GOOD) {
        ;
    }
}

/** @brief  将 HFCLK 源切换为外部高频晶振 HFXT（自定义参数）
 *  @param[in]  range           HFXT 频率范围
 *  @param[in]  startupTime     HFXT 启动时间
 *  @param[in]  monitorEnable   是否使能 HFCLK 启动监控 */
void DL_SYSCTL_setHFCLKSourceHFXTParams(
    DL_SYSCTL_HFXT_RANGE range, uint32_t startupTime, bool monitorEnable)
{
    /* 某些晶振配置在低级复位中保持，先禁用 HFXT 确保一致性 */
    DL_SYSCTL_disableHFXT();

    DL_SYSCTL_setHFXTFrequencyRange(range);
    DL_SYSCTL_setHFXTStartupTime(startupTime);
    SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_HFXTEN_ENABLE;

    if (monitorEnable == true) {
        DL_SYSCTL_enableHFCLKStartupMonitor();
        /* 等待 HFXT 振荡器稳定，若无法稳定请检查硬件/IOMUX 设置 */
        while (
            (DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_HFCLKGOOD_MASK) !=
            DL_SYSCTL_CLK_STATUS_HFCLK_GOOD) {
            ;
        }
    } else {
        DL_SYSCTL_disableHFCLKStartupMonitor();
    }
}

/** @brief  配置频率时钟计数器 (FCC)
 *  @param[in]  trigLvl  触发类型（电平/边沿）
 *  @param[in]  trigSrc  触发源
 *  @param[in]  clkSrc   待测时钟源 */
void DL_SYSCTL_configFCC(DL_SYSCTL_FCC_TRIG_TYPE trigLvl,
    DL_SYSCTL_FCC_TRIG_SOURCE trigSrc, DL_SYSCTL_FCC_CLOCK_SOURCE clkSrc)
{
    DL_Common_updateReg(&SYSCTL->SOCLOCK.GENCLKCFG,
        (uint32_t) trigLvl | (uint32_t) trigSrc | (uint32_t) clkSrc,
        SYSCTL_GENCLKCFG_FCCLVLTRIG_MASK | SYSCTL_GENCLKCFG_FCCTRIGSRC_MASK |
            SYSCTL_GENCLKCFG_FCCSELCLK_MASK);
}

/** @brief  获取当前 RUN/SLEEP 电源策略
 *  @return  当前 RUN/SLEEP 电源策略 */
DL_SYSCTL_POWER_POLICY_RUN_SLEEP DL_SYSCTL_getPowerPolicyRUNSLEEP(void)
{
    DL_SYSCTL_POWER_POLICY_RUN_SLEEP policy =
        DL_SYSCTL_POWER_POLICY_RUN_SLEEP_NOT_ENABLED;

    // 检查是否处于 SLEEP 模式（非深度睡眠）
    if ((SCB->SCR & SCB_SCR_SLEEPDEEP_Msk) != SCB_SCR_SLEEPDEEP_Msk) {
        // 根据寄存器判断具体策略
        if ((SYSCTL->SOCLOCK.MCLKCFG & SYSCTL_MCLKCFG_USELFCLK_MASK) ==
            SYSCTL_MCLKCFG_USELFCLK_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_RUN_SLEEP1;  // LFCLK 运行，SYSOSC 保持
        } else if ((SYSCTL->SOCLOCK.SYSOSCCFG &
                       SYSCTL_SYSOSCCFG_DISABLESTOP_MASK) ==
                   SYSCTL_SYSOSCCFG_DISABLESTOP_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_RUN_SLEEP2;  // LFCLK 运行，SYSOSC 关闭
        } else {
            policy = DL_SYSCTL_POWER_POLICY_RUN_SLEEP0;  // 全速运行
        }
    }
    return policy;
}

/** @brief  获取当前 STOP 电源策略
 *  @return  当前 STOP 电源策略 */
DL_SYSCTL_POWER_POLICY_STOP DL_SYSCTL_getPowerPolicySTOP(void)
{
    DL_SYSCTL_POWER_POLICY_STOP policy =
        DL_SYSCTL_POWER_POLICY_STOP_NOT_ENABLED;

    // 检查是否处于 STOP 模式
    if ((SYSCTL->SOCLOCK.PMODECFG == SYSCTL_PMODECFG_DSLEEP_STOP) &&
        (SCB->SCR & SCB_SCR_SLEEPDEEP_Msk) == SCB_SCR_SLEEPDEEP_Msk) {
        // 根据寄存器判断具体策略
        if ((SYSCTL->SOCLOCK.SYSOSCCFG & SYSCTL_SYSOSCCFG_USE4MHZSTOP_MASK) ==
            SYSCTL_SYSOSCCFG_USE4MHZSTOP_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_STOP1;  // SYSOSC 降频至 4MHz
        } else if ((SYSCTL->SOCLOCK.SYSOSCCFG &
                       SYSCTL_SYSOSCCFG_DISABLESTOP_MASK) ==
                   SYSCTL_SYSOSCCFG_DISABLESTOP_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_STOP2;  // SYSOSC 关闭
        } else {
            policy = DL_SYSCTL_POWER_POLICY_STOP0;  // SYSOSC 保持当前频率
        }
    }
    return policy;
}

/** @brief  获取当前 STANDBY 电源策略
 *  @return  当前 STANDBY 电源策略 */
DL_SYSCTL_POWER_POLICY_STANDBY DL_SYSCTL_getPowerPolicySTANDBY(void)
{
    DL_SYSCTL_POWER_POLICY_STANDBY policy =
        DL_SYSCTL_POWER_POLICY_STANDBY_NOT_ENABLED;

    // 检查是否处于 STANDBY 模式
    if ((SYSCTL->SOCLOCK.PMODECFG == SYSCTL_PMODECFG_DSLEEP_STANDBY) &&
        (SCB->SCR & SCB_SCR_SLEEPDEEP_Msk) == SCB_SCR_SLEEPDEEP_Msk) {
        // 根据寄存器判断具体策略
        if ((SYSCTL->SOCLOCK.MCLKCFG & SYSCTL_MCLKCFG_STOPCLKSTBY_MASK) ==
            SYSCTL_MCLKCFG_STOPCLKSTBY_ENABLE) {
            policy = DL_SYSCTL_POWER_POLICY_STANDBY1;  // 仅 TIMG0/TIMG1 接收时钟
        } else {
            policy = DL_SYSCTL_POWER_POLICY_STANDBY0;  // 所有 PD0 外设接收时钟
        }
    }
    return policy;
}

#endif /* DeviceFamily_PARENT_MSPM0G1X0X_G3X0X */
