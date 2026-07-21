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
 *  @file       dl_wwdt.h
 *  @brief      Window Watchdog Timer (WWDT) Driver Library
 *              窗口看门狗定时器 (WWDT) 驱动库
 *  @defgroup   WWDT Window Watchdog Timer (WWDT)
 *
 *  @anchor ti_dl_dl_wwdt_Overview
 *  # Overview
 *
 *  The Window Watchdog Timer Driver Library allows full configuration of
 *  the MSPM0 WWDT module.
 *  The window watchdog timer (WWDT) supervises code execution.
 *  If the application software does not successfully reset the window
 *  watchdog within the programmed open time window, the window watchdog
 *  generates a SYSRST.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup WWDT
 * @{
 */
#ifndef ti_dl_dl_wwdt__include
#define ti_dl_dl_wwdt__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>

#ifdef __MSPM0_HAS_WWDT__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/*! @enum DL_WWDT_CLOCK_DIVIDE WWDT 时钟分频系数 */
typedef enum {
    /*! Clock source divide by 1  不分频 */
    DL_WWDT_CLOCK_DIVIDE_1 = 0x00000000,
    /*! Clock source divide by 2  2分频 */
    DL_WWDT_CLOCK_DIVIDE_2 = 0x00000001,
    /*! Clock source divide by 3  3分频 */
    DL_WWDT_CLOCK_DIVIDE_3 = 0x00000002,
    /*! Clock source divide by 4  4分频 */
    DL_WWDT_CLOCK_DIVIDE_4 = 0x00000003,
    /*! Clock source divide by 5  5分频 */
    DL_WWDT_CLOCK_DIVIDE_5 = 0x00000004,
    /*! Clock source divide by 6  6分频 */
    DL_WWDT_CLOCK_DIVIDE_6 = 0x00000005,
    /*! Clock source divide by 7  7分频 */
    DL_WWDT_CLOCK_DIVIDE_7 = 0x00000006,
    /*! Clock source divide by 8  8分频 */
    DL_WWDT_CLOCK_DIVIDE_8 = 0x00000007
} DL_WWDT_CLOCK_DIVIDE;

/*! @enum DL_WWDT_WINDOW WWDT 窗口选择 */
typedef enum {
    /*! Window 0 active  使用窗口 0 */
    DL_WWDT_WINDOW0 = WWDT_WWDTCTL1_WINSEL_WIN0,
    /*! Window 1 active  使用窗口 1 */
    DL_WWDT_WINDOW1 = WWDT_WWDTCTL1_WINSEL_WIN1
} DL_WWDT_WINDOW;

/*! @enum DL_WWDT_WINDOW_PERIOD WWDT 关闭窗口周期百分比 */
typedef enum {
    /*! 0% closed window period  关闭窗口 0% */
    DL_WWDT_WINDOW_PERIOD_0 = WWDT_WWDTCTL0_WINDOW0_SIZE_0,
    /*! 12% closed window period  关闭窗口 12% */
    DL_WWDT_WINDOW_PERIOD_12 = WWDT_WWDTCTL0_WINDOW0_SIZE_12,
    /*! 18% closed window period  关闭窗口 18% */
    DL_WWDT_WINDOW_PERIOD_18 = WWDT_WWDTCTL0_WINDOW0_SIZE_18,
    /*! 25% closed window period  关闭窗口 25% */
    DL_WWDT_WINDOW_PERIOD_25 = WWDT_WWDTCTL0_WINDOW0_SIZE_25,
    /*! 50% closed window period  关闭窗口 50% */
    DL_WWDT_WINDOW_PERIOD_50 = WWDT_WWDTCTL0_WINDOW0_SIZE_50,
    /*! 75% closed window period  关闭窗口 75% */
    DL_WWDT_WINDOW_PERIOD_75 = WWDT_WWDTCTL0_WINDOW0_SIZE_75,
    /*! 81% closed window period  关闭窗口 81% */
    DL_WWDT_WINDOW_PERIOD_81 = WWDT_WWDTCTL0_WINDOW0_SIZE_81,
    /*! 87% closed window period  关闭窗口 87% */
    DL_WWDT_WINDOW_PERIOD_87 = WWDT_WWDTCTL0_WINDOW0_SIZE_87
} DL_WWDT_WINDOW_PERIOD;

/*! @enum DL_WWDT_TIMER_PERIOD WWDT 定时器周期计数位数 */
typedef enum {
    /*! 2^6 timer period count  2^6 计数周期 */
    DL_WWDT_TIMER_PERIOD_6_BITS = WWDT_WWDTCTL0_PER_EN_6,
    /*! 2^8 timer period count  2^8 计数周期 */
    DL_WWDT_TIMER_PERIOD_8_BITS = WWDT_WWDTCTL0_PER_EN_8,
    /*! 2^10 timer period count  2^10 计数周期 */
    DL_WWDT_TIMER_PERIOD_10_BITS = WWDT_WWDTCTL0_PER_EN_10,
    /*! 2^12 timer period count  2^12 计数周期 */
    DL_WWDT_TIMER_PERIOD_12_BITS = WWDT_WWDTCTL0_PER_EN_12,
    /*! 2^15 timer period count  2^15 计数周期 */
    DL_WWDT_TIMER_PERIOD_15_BITS = WWDT_WWDTCTL0_PER_EN_15,
    /*! 2^18 timer period count  2^18 计数周期 */
    DL_WWDT_TIMER_PERIOD_18_BITS = WWDT_WWDTCTL0_PER_EN_18,
    /*! 2^21 timer period count  2^21 计数周期 */
    DL_WWDT_TIMER_PERIOD_21_BITS = WWDT_WWDTCTL0_PER_EN_21,
    /*! 2^25 timer period count  2^25 计数周期 */
    DL_WWDT_TIMER_PERIOD_25_BITS = WWDT_WWDTCTL0_PER_EN_25
} DL_WWDT_TIMER_PERIOD;

/*! @enum DL_WWDT_SLEEP_MODE WWDT 睡眠模式行为 */
typedef enum {
    /*! Stop the watchdog while in sleep  睡眠时停止看门狗 */
    DL_WWDT_STOP_IN_SLEEP = WWDT_WWDTCTL0_STISM_STOP,
    /*! Keep the watchdog running in sleep  睡眠时保持看门狗运行 */
    DL_WWDT_RUN_IN_SLEEP = WWDT_WWDTCTL0_STISM_CONT
} DL_WWDT_SLEEP_MODE;

/*! @enum DL_WWDT_MODE WWDT 工作模式 */
typedef enum {
    /*! The watchdog is in watchdog mode  看门狗模式 */
    DL_WWDT_WATCHDOG_MODE = 0x0,
    /*! The watchdog is in interval timer mode  间隔定时器模式 */
    DL_WWDT_INTERVAL_TIMER_MODE = WWDT_WWDTCTL0_MODE_INTERVAL
} DL_WWDT_MODE;

/*! @enum DL_WWDT_IIDX WWDT 中断索引 */
typedef enum {
    /*! Interrupt index for WWDT if no interrupt is pending  无中断挂起 */
    DL_WWDT_IIDX_NO_INT = WWDT_IIDX_STAT_NO_INTR,
    /*! WWDT interrupt index for interval timer  间隔定时器中断 */
    DL_WWDT_IIDX_INTERVAL_TIMER = WWDT_IIDX_STAT_INTTIM
} DL_WWDT_IIDX;

/*! @enum DL_WWDT_CORE_HALT WWDT 内核暂停时的行为 */
typedef enum {
    /*! WWDT will halt with core  内核暂停时 WWDT 也暂停 */
    DL_WWDT_CORE_HALT_STOP     = WWDT_PDBGCTL_FREE_STOP,
    /*! WWDT ignores the state of the core halted input  内核暂停时 WWDT 继续运行 */
    DL_WWDT_CORE_HALT_FREE_RUN = WWDT_PDBGCTL_FREE_RUN,
} DL_WWDT_CORE_HALT;

/* clang-format on */

/*!
 *  @brief      Initializes the Window Watchdog Timer in watchdog mode
 *  以看门狗模式初始化窗口看门狗定时器
 *
 *  While in watchdog mode, the Window Watchdog Timer must be serviced
 *  periodically or the device will be reset. After calling this initialization
 *  API the watchdog will automatically start running with whatever window is
 *  currently selected. See @ref DL_WWDT_setActiveWindow for setting the active
 *  window, this is typically window 0 upon startup.
 *  看门狗模式下必须周期性喂狗，否则设备将复位。调用此 API 后看门狗将使用当前选择的窗口自动启动。
 *  通常启动时使用窗口 0，可通过 @ref DL_WWDT_setActiveWindow 切换。
 *
 *  Once running, the watchdog will need to be restarted periodically with @ref
 *  DL_WWDT_restart.
 *  运行后需周期性调用 @ref DL_WWDT_restart 重启看门狗。
 *
 *  The watchdog can only be initialized once, so this API can only be called
 *  once, and you may not later call @ref DL_WWDT_initIntervalTimerMode.
 *  看门狗只能初始化一次，此 API 只能调用一次，之后不能调用 @ref DL_WWDT_initIntervalTimerMode。
 *
 *  @param[in]  wwdt                Pointer to the register overlay for
 *                                  peripheral
 *                                  WWDT 外设寄存器基地址指针
 *  @param[in]  divider             Clock divider for the timer. One of @ref
 *                                  DL_WWDT_CLOCK_DIVIDE.
 *                                  时钟分频系数
 *  @param[in]  period              Period for the timer. One of @ref
 *                                  DL_WWDT_TIMER_PERIOD.
 *                                  定时器周期
 *  @param[in]  sleepMode           Enable/disable the timer running in sleep
 *                                  mode. One of @ref DL_WWDT_SLEEP_MODE.
 *                                  睡眠模式下是否保持运行
 *  @param[in]  window0ClosedPeriod The percentage of the total period that
 *                                  reset window 0 should be closed. One of
 *                                  @ref DL_WWDT_WINDOW_PERIOD.
 *                                  窗口 0 关闭周期百分比
 *  @param[in]  window1ClosedPeriod The percentage of the total period that
 *                                  reset window 1 should be closed. One of
 *                                  @ref DL_WWDT_WINDOW_PERIOD.
 *                                  窗口 1 关闭周期百分比
 *
 *  @sa         DL_WWDT_initIntervalTimerMode
 *  @sa         DL_WWDT_setActiveWindow
 *  @sa         DL_WWDT_restart
 *
 */
__STATIC_INLINE void DL_WWDT_initWatchdogMode(WWDT_Regs *wwdt,
    DL_WWDT_CLOCK_DIVIDE divider, DL_WWDT_TIMER_PERIOD period,
    DL_WWDT_SLEEP_MODE sleepMode, DL_WWDT_WINDOW_PERIOD window0ClosedPeriod,
    DL_WWDT_WINDOW_PERIOD window1ClosedPeriod)
{
    /* Window1 is shifted because the #defines used as an input are shared
       between window 0 and window 1 */
    /* 窗口1左移4位，因为窗口0和窗口1共享相同的宏定义 */
    wwdt->WWDTCTL0 =
        (WWDT_WWDTCTL0_KEY_UNLOCK_W | (uint32_t) divider | (uint32_t) period |
            (uint32_t) sleepMode | (uint32_t) window0ClosedPeriod |
            ((uint32_t) window1ClosedPeriod << 4));
}

/*!
 *  @brief      Initializes the Window Watchdog Timer in interval timer mode
 *  以间隔定时器模式初始化窗口看门狗定时器
 *
 *  The Window Watchdog Timer can be used as a simple interval timer if you do
 *  not need watchdog/reset functionality. After calling this initialization
 *  API the timer will automatically start running and start interrupting
 *  periodically.
 *  如果不需要看门狗/复位功能，可将 WWDT 用作简单间隔定时器。
 *  调用此 API 后定时器将自动启动并周期性产生中断。
 *
 *  The WWDT can only be initialized once, so this API can only be called once,
 *  and you may not later call @ref DL_WWDT_initWatchdogMode.
 *  WWDT 只能初始化一次，此 API 只能调用一次，之后不能调用 @ref DL_WWDT_initWatchdogMode。
 *
 *  @param[in]  wwdt      Pointer to the register overlay for peripheral
 *                        WWDT 外设寄存器基地址指针
 *  @param[in]  divider   Clock divider for the timer. One of
 *                        @ref DL_WWDT_CLOCK_DIVIDE.
 *                        时钟分频系数
 *  @param[in]  period    Period for the timer. One of @ref
 *                        DL_WWDT_TIMER_PERIOD.
 *                        定时器周期
 *  @param[in]  sleepMode Enable/disable the timer running in sleep mode. One
 *                        of @ref DL_WWDT_SLEEP_MODE.
 *                        睡眠模式下是否保持运行
 *
 *  @sa         DL_WWDT_initWatchdogMode
 */
__STATIC_INLINE void DL_WWDT_initIntervalTimerMode(WWDT_Regs *wwdt,
    DL_WWDT_CLOCK_DIVIDE divider, DL_WWDT_TIMER_PERIOD period,
    DL_WWDT_SLEEP_MODE sleepMode)
{
    wwdt->WWDTCTL0 =
        (WWDT_WWDTCTL0_KEY_UNLOCK_W | (uint32_t) divider | (uint32_t) period |
            (uint32_t) sleepMode | WWDT_WWDTCTL0_MODE_INTERVAL);
}

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the WWDT
 * 使能 WWDT 外设电源写使能 (PWREN) 寄存器
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *  在软件配置任何外设寄存器之前，需先向 PWREN 寄存器写入 KEY 和 ENABLE 位来使能外设。
 *
 * @param wwdt        Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_enablePower(WWDT_Regs *wwdt)
{
    wwdt->GPRCM.PWREN = (WWDT_PWREN_KEY_UNLOCK_W | WWDT_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the WWDT
 * 禁用 WWDT 外设电源写使能 (PWREN) 寄存器
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *  清除 PWREN.ENABLE 位后，外设寄存器将不可读写。
 *
 *  @note This API does not provide large power savings.
 *  @note 此 API 节省的功耗有限。
 *
 * @param wwdt        Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_disablePower(WWDT_Regs *wwdt)
{
    wwdt->GPRCM.PWREN = (WWDT_PWREN_KEY_UNLOCK_W | WWDT_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the WWDT
 *        is enabled
 * 查询 WWDT 外设电源写使能 (PWREN) 寄存器是否已使能
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
 * @param wwdt        Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 * @return true if peripheral register access is enabled  外设寄存器访问已使能
 * @return false if peripheral register access is disabled  外设寄存器访问已禁用
 */
__STATIC_INLINE bool DL_WWDT_isPowerEnabled(WWDT_Regs *wwdt)
{
    return ((wwdt->GPRCM.PWREN & WWDT_PWREN_ENABLE_MASK) ==
            WWDT_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets wwdt peripheral
 * 复位 WWDT 外设
 *
 * @param wwdt        Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_reset(WWDT_Regs *wwdt)
{
    wwdt->GPRCM.RSTCTL =
        (WWDT_RSTCTL_KEY_UNLOCK_W | WWDT_RSTCTL_RESETSTKYCLR_CLR |
            WWDT_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Returns if wwdt peripheral was reset
 * 查询 WWDT 外设是否已复位
 *
 * @param wwdt        Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 * @return true if peripheral was reset  外设已复位
 * @return false if peripheral wasn't reset  外设未复位
 *
 */
__STATIC_INLINE bool DL_WWDT_isReset(WWDT_Regs *wwdt)
{
    return ((wwdt->GPRCM.STAT & WWDT_STAT_RESETSTKY_MASK) ==
            WWDT_STAT_RESETSTKY_RESET);
}

/*!
 *  @brief      Restarts the Window Watchdog
 *  重启窗口看门狗（喂狗）
 *
 *  When the Window Watchdog Timer is in watchdog mode it must be periodically
 *  serviced to avoid a hardware reset. This API must be called after the
 *  closed window of the watchdog has passed but before the watchdog resets the
 *  device.
 *  看门狗模式下必须周期性喂狗以避免硬件复位。
 *  需在关闭窗口结束之后、看门狗复位之前调用此 API。
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_restart(WWDT_Regs *wwdt)
{
    /* 写入 0x00A7 重启看门狗计数器 */
    wwdt->WWDTCNTRST = 0x00A7;
}

/*!
 *  @brief      Checks if the Window Watchdog Timer is actively running
 *  检查窗口看门狗定时器是否正在运行
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     Returns the running status of the Window Watchdog Timer
 *              返回窗口看门狗定时器运行状态
 *
 *  @retval     true  The Window Watchdog Timer is running  看门狗正在运行
 *  @retval     false The Window Watchdog Timer is not running  看门狗未运行
 */
__STATIC_INLINE bool DL_WWDT_isRunning(WWDT_Regs *wwdt)
{
    return (wwdt->WWDTSTAT == WWDT_WWDTSTAT_RUN_ON);
}

/*!
 *  @brief Set the active window being used
 *  设置当前活动窗口
 *
 *  The Window Watchdog Timer module allows you to switch between two different
 *  configured windows. This allows you to configure the watchdog with two
 *  different window sizes and switch between them during your application.
 *  WWDT 支持在两个已配置的窗口之间切换，可在运行时更改窗口大小。
 *
 *  @param[in]  wwdt    Pointer to the register overlay for peripheral
 *                      WWDT 外设寄存器基地址指针
 *  @param[in]  window  The window to set as active. One of @ref
 *                      DL_WWDT_WINDOW.
 *                      要激活的窗口
 *
 */
__STATIC_INLINE void DL_WWDT_setActiveWindow(
    WWDT_Regs *wwdt, DL_WWDT_WINDOW window)
{
    wwdt->WWDTCTL1 = (WWDT_WWDTCTL1_KEY_UNLOCK_W | (uint32_t) window);
}

/*!
 *  @brief      Get the active window being used
 *  获取当前活动窗口
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     The active window being used
 *              当前活动窗口
 *
 *  @retval     One of @ref DL_WWDT_WINDOW
 */
__STATIC_INLINE DL_WWDT_WINDOW DL_WWDT_getActiveWindow(WWDT_Regs *wwdt)
{
    uint32_t window = (wwdt->WWDTCTL1 & WWDT_WWDTCTL1_WINSEL_MASK);

    return (DL_WWDT_WINDOW)(window);
}

/*!
 *  @brief      Get the sleep mode being used
 *  获取当前睡眠模式配置
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     The current sleep mode
 *              当前睡眠模式
 *
 *  @retval     One of @ref DL_WWDT_SLEEP_MODE
 */
__STATIC_INLINE DL_WWDT_SLEEP_MODE DL_WWDT_getSleepMode(WWDT_Regs *wwdt)
{
    uint32_t mode = (wwdt->WWDTCTL0 & WWDT_WWDTCTL0_STISM_MASK);

    return (DL_WWDT_SLEEP_MODE)(mode);
}

/*!
 *  @brief      Get the mode the watchdog is running in
 *  获取看门狗当前工作模式
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     The current mode the watchdog is running in
 *              看门狗当前工作模式
 *
 *  @retval     One of @ref DL_WWDT_MODE
 */
__STATIC_INLINE DL_WWDT_MODE DL_WWDT_getMode(WWDT_Regs *wwdt)
{
    uint32_t mode = (wwdt->WWDTCTL0 & WWDT_WWDTCTL0_MODE_MASK);

    return (DL_WWDT_MODE)(mode);
}

/*!
 *  @brief      Get the window closed percentage for a watchdog window
 *  获取指定窗口的关闭窗口百分比
 *
 *  @param[in]  wwdt    Pointer to the register overlay for peripheral
 *                      WWDT 外设寄存器基地址指针
 *  @param[in]  window  The window to get the closed window percentage of. One
 *                      of @ref DL_WWDT_WINDOW.
 *                      要查询的窗口编号
 *
 *  @return     The desired window's closed percentage
 *              指定窗口的关闭百分比
 *
 *  @retval     One of @ref DL_WWDT_WINDOW_PERIOD
 */
__STATIC_INLINE DL_WWDT_WINDOW_PERIOD DL_WWDT_getWindowPeriod(
    WWDT_Regs *wwdt, DL_WWDT_WINDOW window)
{
    uint32_t period;

    /* 根据窗口编号读取对应位域，窗口1需右移4位 */
    if (window == DL_WWDT_WINDOW0) {
        period = (wwdt->WWDTCTL0 & WWDT_WWDTCTL0_WINDOW0_MASK);
    } else {
        period = ((wwdt->WWDTCTL0 & WWDT_WWDTCTL0_WINDOW1_MASK) >> 4);
    }

    return (DL_WWDT_WINDOW_PERIOD)(period);
}

/*!
 *  @brief      Get the timer period for the watchdog
 *  获取看门狗定时器周期配置
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     The current timer period for the watchdog
 *              当前定时器周期
 *
 *  @retval     One of @ref DL_WWDT_TIMER_PERIOD
 */
__STATIC_INLINE DL_WWDT_TIMER_PERIOD DL_WWDT_getTimerPeriod(WWDT_Regs *wwdt)
{
    uint32_t period = (wwdt->WWDTCTL0 & WWDT_WWDTCTL0_PER_MASK);

    return (DL_WWDT_TIMER_PERIOD)(period);
}

/*!
 *  @brief      Get the clock divider for the watchdog
 *  获取看门狗时钟分频系数
 *
 *  @param[in]  wwdt  Pointer to the register overlay for peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     The current clock divider used
 *              当前时钟分频系数
 *
 *  @retval     One of @ref DL_WWDT_CLOCK_DIVIDE
 */
__STATIC_INLINE DL_WWDT_CLOCK_DIVIDE DL_WWDT_getClockDivider(WWDT_Regs *wwdt)
{
    uint32_t clockDivider = (wwdt->WWDTCTL0 & WWDT_WWDTCTL0_CLKDIV_MASK);

    return (DL_WWDT_CLOCK_DIVIDE)(clockDivider);
}

/**
 *  @brief      Enable WWDT interrupt
 *  使能 WWDT 中断
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_enableInterrupt(WWDT_Regs *wwdt)
{
    wwdt->CPU_INT.IMASK = WWDT_IMASK_INTTIM_SET;
}

/**
 *  @brief      Disable WWDT interrupt
 *  禁用 WWDT 中断
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_disableInterrupt(WWDT_Regs *wwdt)
{
    wwdt->CPU_INT.IMASK = WWDT_IMASK_INTTIM_CLR;
}

/**
 *  @brief      Check if WWDT interrupt is enabled
 *  检查 WWDT 中断是否已使能
 *
 *  @param[in]  wwdt Pointer to the register overlay for the peripheral
 *                   WWDT 外设寄存器基地址指针
 *
 *  @return     If the WWDT interrupt is enabled
 *              WWDT 中断是否已使能
 *
 *  @retval     true  The WWDT interrupt is enabled  WWDT 中断已使能
 *  @retval     false The WWDT interrupt is disabled  WWDT 中断已禁用
 */
__STATIC_INLINE bool DL_WWDT_isInterruptEnabled(WWDT_Regs *wwdt)
{
    return ((wwdt->CPU_INT.IMASK & WWDT_IMASK_INTTIM_SET) ==
            WWDT_IMASK_INTTIM_SET);
}

/**
 *  @brief      Check interrupt flag of enabled WWDT interrupt
 *  检查已使能的 WWDT 中断标志位
 *
 *  Checks if the WWDT interrupt that was previously enabled is pending.
 *  检查之前已使能的 WWDT 中断是否处于挂起状态。
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     If the enabled WWDT interrupt is pending
 *              已使能的 WWDT 中断是否挂起
 *
 *  @retval     true  The WWDT interrupt is pending  WWDT 中断挂起
 *  @retval     false The WWDT interrupt is not pending  WWDT 中断未挂起
 *
 *  @sa         DL_WWDT_enableInterrupt
 */
__STATIC_INLINE bool DL_WWDT_getEnabledInterruptStatus(WWDT_Regs *wwdt)
{
    return ((wwdt->CPU_INT.MIS & WWDT_MIS_INTTIM_SET) == WWDT_MIS_INTTIM_SET);
}

/**
 *  @brief      Check interrupt flag of any WWDT interrupt
 *  检查任意 WWDT 中断的原始标志位
 *
 *  Checks if the WWDT interrupt is pending. Interrupt does not have to
 *  be previously enabled.
 *  检查 WWDT 中断是否挂起，无需预先使能中断。
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     If the WWDT interrupt is pending
 *              WWDT 中断是否挂起
 *
 *  @retval     true  The WWDT interrupt is pending  WWDT 中断挂起
 *  @retval     false The WWDT interrupt is not pending  WWDT 中断未挂起
 */
__STATIC_INLINE uint32_t DL_WWDT_getRawInterruptStatus(WWDT_Regs *wwdt)
{
    return (wwdt->CPU_INT.RIS & WWDT_RIS_INTTIM_SET);
}

/**
 *  @brief      Get highest priority pending WWDT interrupt
 *  获取最高优先级的挂起 WWDT 中断
 *
 *  Checks if any of the WWDT interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *  检查是否有 WWDT 中断挂起，无需预先使能中断。
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     The highest priority pending WWDT interrupt
 *              最高优先级的挂起 WWDT 中断
 *
 *  @retval     One of @ref DL_WWDT_IIDX
 */
__STATIC_INLINE DL_WWDT_IIDX DL_WWDT_getPendingInterrupt(WWDT_Regs *wwdt)
{
    return (DL_WWDT_IIDX)(wwdt->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending WWDT interrupt
 *  清除挂起的 WWDT 中断
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 */
__STATIC_INLINE void DL_WWDT_clearInterruptStatus(WWDT_Regs *wwdt)
{
    wwdt->CPU_INT.ICLR = WWDT_ICLR_INTTIM_CLR;
}

/**
 *  @brief      Configures WWDT behavior when the core is halted.
 *  配置内核暂停时 WWDT 的行为
 *
 *  @param[in]  wwdt     Pointer to the register overlay for the peripheral
 *                       WWDT 外设寄存器基地址指针
 *
 *  @param[in]  haltMode WWDT halt behavior. One of @ref DL_WWDT_CORE_HALT.
 *                       内核暂停时 WWDT 行为
 *
 */
__STATIC_INLINE void DL_WWDT_setCoreHaltBehavior(
    WWDT_Regs *wwdt, DL_WWDT_CORE_HALT haltMode)
{
    wwdt->PDBGCTL = (uint32_t) haltMode;
}

/**
 *  @brief      Get WWDT behavior when the core is halted.
 *  获取内核暂停时 WWDT 的行为配置
 *
 *  @param[in]  wwdt  Pointer to the register overlay for the peripheral
 *                    WWDT 外设寄存器基地址指针
 *
 *  @return     WWDT bahvior when core is halted. One of
 *              @ref DL_WWDT_CORE_HALT
 *              内核暂停时 WWDT 的行为
 *
 */
__STATIC_INLINE DL_WWDT_CORE_HALT DL_WWDT_getCoreHaltBehavior(WWDT_Regs *wwdt)
{
    uint32_t haltMode = (wwdt->PDBGCTL & WWDT_PDBGCTL_FREE_MASK);
    return (DL_WWDT_CORE_HALT) haltMode;
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_WWDT__ */

#endif /* ti_dl_dl_wwdt__include */
/** @}*/
