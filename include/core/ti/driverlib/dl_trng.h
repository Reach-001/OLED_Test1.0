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
 *  @file       dl_trng.h
 *  @brief      True Random Number Generator (TRNG) Driver Library
 *              真随机数发生器 (TRNG) 驱动库头文件
 *  @defgroup   TRNG True Random Number Generator (TRNG)
 *
 *  @anchor ti_dl_dl_m0p_trng_Overview
 *  # Overview 概述
 *
 *  The True Random Number Generator Driver Library allows full configuration of
 *  the MSPM0 TRNG module.
 *  The true random number generator (TRNG) block is an entropy source which
 *  can be used to generate random bit sequences.
 *
 *  真随机数发生器驱动库支持 MSPM0 TRNG 模块的完整配置。
 *  TRNG 模块是一个熵源，可用于生成随机比特序列。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup TRNG
 * @{
 */
#ifndef ti_dl_dl_trng__include
#define ti_dl_dl_trng__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_TRNG__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_TRNG_INTERRUPT
 *  @{
 */

/*!
* @brief TRNG interrupt definition for health test failed
*        健康测试失败中断定义
*/
#define DL_TRNG_INTERRUPT_HEALTH_FAIL_EVENT   (TRNG_IMASK_IRQ_HEALTH_FAIL_MASK)

/*!
* @brief TRNG interrupt definition for commafnd done
*        命令完成中断定义
*/
#define DL_TRNG_INTERRUPT_CMD_DONE_EVENT         (TRNG_IMASK_IRQ_CMD_DONE_MASK)

/*!
* @brief TRNG interrupt definition for command fail
*        命令失败中断定义
*/
#define DL_TRNG_INTERRUPT_CMD_FAIL_EVENT         (TRNG_IMASK_IRQ_CMD_FAIL_MASK)

/*!
* @brief TRNG interrupt definition for capture ready
*        捕获就绪中断定义
*/
#define DL_TRNG_INTERRUPT_CAPTURE_RDY_EVENT  (TRNG_IMASK_IRQ_CAPTURED_RDY_MASK)
/** @}*/

/** @addtogroup DL_TRNG_HEALTH_TEST_SUCCESS
 *  @{
 */

/*!
* @brief TRNG Digital health test success value
*        数字健康测试成功值，所有 8 项测试均通过时为 0xFF
*/
#define DL_TRNG_DIGITAL_HEALTH_TEST_SUCCESS                              (0xFF)

/*!
* @brief TRNG Analog health test success value
*        模拟健康测试成功值，通过时为 0x1
*/
#define DL_TRNG_ANALOG_HEALTH_TEST_SUCCESS                                (0x1)
/** @}*/

/* clang-format on */

/*! @enum DL_TRNG_IIDX TRNG 中断索引枚举 */
typedef enum {
    /*! TRNG interrupt index for health fail 健康测试失败中断索引 */
    DL_TRNG_IIDX_HEALTH_FAIL = TRNG_IIDX_STAT_IRQ_HEALTH_FAIL,
    /*! TRNG interrupt index for command done 命令完成中断索引 */
    DL_TRNG_IIDX_CMD_DONE = TRNG_IIDX_STAT_IRQ_CMD_DONE,
    /*! TRNG interrupt index for command fail 命令失败中断索引 */
    DL_TRNG_IIDX_CMD_FAIL = TRNG_IIDX_STAT_IRQ_CMD_FAIL,
    /*! TRNG interrupt index indicating the capture buffer is ready 捕获缓冲区就绪中断索引 */
    DL_TRNG_IIDX_CAPTURED_READY = TRNG_IIDX_STAT_IRQ_CAPTURED_RDY,
} DL_TRNG_IIDX;

/*! @enum DL_TRNG_DECIMATION_RATE 抽取率枚举 */
typedef enum {
    /*! Decimation by 1 (no decimation) 抽取率为 1（不抽取） */
    DL_TRNG_DECIMATION_RATE_1 = 0x0,
    /*! Decimation rate 2 -- take every other sample 抽取率为 2，每隔一个采样取一个 */
    DL_TRNG_DECIMATION_RATE_2 = 0x1,
    /*! Decimation rate 3 -- take every third sample 抽取率为 3，每隔两个采样取一个 */
    DL_TRNG_DECIMATION_RATE_3 = 0x2,
    /*! Decimation rate 4 -- take every fourth sample 抽取率为 4，每隔三个采样取一个 */
    DL_TRNG_DECIMATION_RATE_4 = 0x3,
    /*! Decimation rate 5 -- take every fifth sample 抽取率为 5，每隔四个采样取一个 */
    DL_TRNG_DECIMATION_RATE_5 = 0x4,
    /*! Decimation rate 6 -- take every sixth sample 抽取率为 6，每隔五个采样取一个 */
    DL_TRNG_DECIMATION_RATE_6 = 0x5,
    /*! Decimation rate 7 -- take every seventh sample 抽取率为 7，每隔六个采样取一个 */
    DL_TRNG_DECIMATION_RATE_7 = 0x6,
    /*! Decimation rate 8 -- take every eighth sample 抽取率为 8，每隔七个采样取一个 */
    DL_TRNG_DECIMATION_RATE_8 = 0x7,
} DL_TRNG_DECIMATION_RATE;

/*! @enum DL_TRNG_CMD TRNG 命令枚举 */
typedef enum {
    /*! TRNG command to power off analog blocks and digital blocks are gated 关闭电源命令，模拟模块断电，数字模块门控 */
    DL_TRNG_CMD_PWROFF = TRNG_CTL_CMD_PWR_OFF,
    /*! TRNG command to execute digital power-on self-tests 执行数字模块上电自检命令 */
    DL_TRNG_CMD_TEST_DIG = TRNG_CTL_CMD_PWRUP_DIG,
    /*! TRNG command to execute analog power-on self-tests 执行模拟模块上电自检命令 */
    DL_TRNG_CMD_TEST_ANA = TRNG_CTL_CMD_PWRUP_ANA,
    /*! TRNG command to go into normal operating mode 进入正常工作模式命令 */
    DL_TRNG_CMD_NORM_FUNC = TRNG_CTL_CMD_NORM_FUNC,
} DL_TRNG_CMD;

/*! @enum DL_TRNG_CLOCK_DIVIDE TRNG 时钟分频枚举 */
typedef enum {
    /*! Clock divide by 1 时钟不分频 */
    DL_TRNG_CLOCK_DIVIDE_1 = TRNG_CLKDIVIDE_RATIO_DIV_BY_1,
    /*! Clock divide by 2 时钟 2 分频 */
    DL_TRNG_CLOCK_DIVIDE_2 = TRNG_CLKDIVIDE_RATIO_DIV_BY_2,
    /*! Clock divide by 4 时钟 4 分频 */
    DL_TRNG_CLOCK_DIVIDE_4 = TRNG_CLKDIVIDE_RATIO_DIV_BY_4,
    /*! Clock divide by 6 时钟 6 分频 */
    DL_TRNG_CLOCK_DIVIDE_6 = TRNG_CLKDIVIDE_RATIO_DIV_BY_6,
    /*! Clock divide by 8 时钟 8 分频 */
    DL_TRNG_CLOCK_DIVIDE_8 = TRNG_CLKDIVIDE_RATIO_DIV_BY_8,
} DL_TRNG_CLOCK_DIVIDE;

/*! @enum DL_TRNG_STATE TRNG 状态枚举 */
typedef enum {
    /*! Current state of the TRNG is OFF 关闭状态 */
    DL_TRNG_STATE_OFF = 0x0,
    /*! Current state of the TRNG is PWRUP_ES 熵源上电中 */
    DL_TRNG_STATE_PWRUP_ES = 0x1,
    /*! Current state of the TRNG is NORM_FUNC 正常工作状态 */
    DL_TRNG_STATE_NORM_FUNC = 0x3,
    /*! Current state of the TRNG is TEST_DIG 数字自检状态 */
    DL_TRNG_STATE_TEST_DIG = 0x7,
    /*! Current state of the TRNG is TEST_ANA 模拟自检状态 */
    DL_TRNG_STATE_TEST_ANA = 0xB,
    /*! Current state of the TRNG is ERROR 错误状态 */
    DL_TRNG_STATE_ERROR = 0xA,
    /*! Current state of the TRNG is PWRDOWN_ES 熵源掉电中 */
    DL_TRNG_STATE_PWRDOWN = 0x2,
} DL_TRNG_STATE;

/**
 * @brief Configuration structure to backup TRNG peripheral state before
 *        going to STOP/STANDBY mode. Used by @ref DL_TRNG_saveConfiguration
 *        进入 STOP/STANDBY 模式前备份 TRNG 外设状态的配置结构体
 */
typedef struct {
    /*! TRNG control word. Combination of @ref DL_TRNG_DECIMATION_RATE and
     *  @ref DL_TRNG_CMD
     *  控制字，包含抽取率和命令的组合 */
    uint32_t controlWord;
    /*! TRNG clock divider. Must be one of @ref DL_TRNG_CLOCK_DIVIDE
     *  时钟分频值 */
    uint32_t clockDivider;
    /*! TRNG interrupt status. Must be one of @ref DL_TRNG_IIDX
     *  中断掩码 */
    uint32_t interruptMask;
    /*! Boolean flag indicating whether or not a valid configuration structure
       exists. Should not be modified by the user.
     *  备份就绪标志，用户不应修改 */
    bool backupRdy;
} DL_TRNG_backupConfig;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the TRNG
 *        使能 TRNG 外设电源写使能寄存器
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 */
__STATIC_INLINE void DL_TRNG_enablePower(TRNG_Regs *trng)
{
    trng->GPRCM.PWREN = TRNG_PWREN_KEY_UNLOCK_W | TRNG_PWREN_ENABLE_ENABLE;
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the TRNG
 *        禁用 TRNG 外设电源写使能寄存器
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 *  @note This API does not provide large power savings.
 *        注意：此 API 不会显著节省功耗
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 */
__STATIC_INLINE void DL_TRNG_disablePower(TRNG_Regs *trng)
{
    trng->GPRCM.PWREN = TRNG_PWREN_KEY_UNLOCK_W | TRNG_PWREN_ENABLE_DISABLE;
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the TRNG
 *        is enabled
 *        查询 TRNG 外设电源是否已使能
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 * @param trng        Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 * @return true if peripheral register access is enabled 外设寄存器可访问
 * @return false if peripheral register access is disabled 外设寄存器不可访问
 */
__STATIC_INLINE bool DL_TRNG_isPowerEnabled(TRNG_Regs *trng)
{
    return ((trng->GPRCM.PWREN & TRNG_PWREN_ENABLE_MASK) ==
            TRNG_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Get the clock divider on the TRNG module
 *        获取 TRNG 模块的时钟分频值
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return      Clock divider value for the TRNG module
 *              时钟分频值
 */
__STATIC_INLINE DL_TRNG_CLOCK_DIVIDE DL_TRNG_getClockDivider(
    const TRNG_Regs *trng)
{
    return (DL_TRNG_CLOCK_DIVIDE) trng->CLKDIVIDE;
}

/**
 * @brief Set the clock divider on the TRNG module
 *        设置 TRNG 模块的时钟分频值
 *
 * @param trng          Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 * @param clockDivider  Clock divider value for the TRNG module.
 *                      One of @ref DL_TRNG_CLOCK_DIVIDE
 *                      时钟分频值
 */
__STATIC_INLINE void DL_TRNG_setClockDivider(
    TRNG_Regs *trng, DL_TRNG_CLOCK_DIVIDE clockDivider)
{
    trng->CLKDIVIDE = (uint32_t) clockDivider;
}

/**
 * @brief Resets the TRNG module
 *        复位 TRNG 模块
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 */
__STATIC_INLINE void DL_TRNG_reset(TRNG_Regs *trng)
{
    trng->GPRCM.RSTCTL =
        TRNG_RSTCTL_RESETASSERT_ASSERT | TRNG_RSTCTL_KEY_UNLOCK_W;
}

/**
 * @brief Returns if TRNG peripheral was reset
 *        查询 TRNG 外设是否已复位
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return true if peripheral was reset 外设已复位
 * @return false if peripheral wasn't reset 外设未复位
 */
__STATIC_INLINE bool DL_TRNG_isReset(const TRNG_Regs *trng)
{
    return (trng->GPRCM.STAT & TRNG_GPRCM_STAT_RESETSTKY_MASK) ==
           TRNG_GPRCM_STAT_RESETSTKY_RESET;
}

/**
 * @brief Returns if the data capture is ready
 *        查询数据捕获是否就绪
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return true if the data capture is ready 数据捕获就绪
 * @return false if the data capture is not ready 数据捕获未就绪
 */
__STATIC_INLINE bool DL_TRNG_isCaptureReady(const TRNG_Regs *trng)
{
    return ((trng->CPU_INT.RIS & TRNG_RIS_IRQ_CAPTURED_RDY_MASK)) ==
           TRNG_RIS_IRQ_CAPTURED_RDY_SET;
}

/**
 * @brief Returns if the issued TRNG command is done
 *        查询 TRNG 命令是否已完成
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return true if the issued command is done 命令已完成
 * @return false if the issued command is not done 命令未完成
 */
__STATIC_INLINE bool DL_TRNG_isCommandDone(const TRNG_Regs *trng)
{
    return ((trng->CPU_INT.RIS & TRNG_RIS_IRQ_CMD_DONE_MASK)) ==
           TRNG_RIS_IRQ_CMD_DONE_SET;
}

/**
 * @brief Returns if the issued TRNG command failed
 *        查询 TRNG 命令是否失败
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return true if the issued command failed 命令已失败
 * @return false if the issued command did not fail 命令未失败
 */
__STATIC_INLINE bool DL_TRNG_isCommandFail(const TRNG_Regs *trng)
{
    return ((trng->CPU_INT.RIS & TRNG_RIS_IRQ_CMD_FAIL_MASK)) ==
           TRNG_RIS_IRQ_CMD_FAIL_SET;
}

/**
 * @brief Returns if a health test failed
 *        查询健康测试是否失败
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return true if a health test is failed 健康测试失败
 * @return false if a health test did not fail 健康测试未失败
 */
__STATIC_INLINE bool DL_TRNG_isHealthTestFail(const TRNG_Regs *trng)
{
    return ((trng->CPU_INT.RIS & TRNG_RIS_IRQ_HEALTH_FAIL_MASK)) ==
           TRNG_RIS_IRQ_HEALTH_FAIL_SET;
}

/**
 * @brief Set the decimation rate
 *        设置抽取率
 *
 * @param trng            Pointer to the register overlay for the peripheral
 *                        外设寄存器基地址指针
 *
 * @param decimationRate  Decimation rate to set.
 *                        One of @ref DL_TRNG_DECIMATION_RATE
 *                        抽取率值
 */
__STATIC_INLINE void DL_TRNG_setDecimationRate(
    TRNG_Regs *trng, DL_TRNG_DECIMATION_RATE decimationRate)
{
    DL_Common_updateReg(&trng->CTL,
        ((uint32_t) decimationRate << TRNG_CTL_DECIM_RATE_OFS),
        TRNG_CTL_DECIM_RATE_MASK);
}

/**
 *  @brief      Get the decimation rate
 *              获取抽取率
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @return     The decimation rate of the TRNG module
 *              TRNG 模块的抽取率
 *
 *  @retval     One of @ref DL_TRNG_DECIMATION_RATE
 */
__STATIC_INLINE DL_TRNG_DECIMATION_RATE DL_TRNG_getDecimationRate(
    const TRNG_Regs *trng)
{
    uint32_t decimationRate =
        ((trng->CTL & TRNG_CTL_DECIM_RATE_MASK) >> TRNG_CTL_DECIM_RATE_OFS);

    return (DL_TRNG_DECIMATION_RATE)(decimationRate);
}

/**
 * @brief Get the digital health test results
 *        获取数字健康测试结果
 *
 * The test sequence includes eight tests, and each test reports its status in
 * an individual result bit. As each test passes, the corresponding bit is set.
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return The results of the digital health tests
 *         数字健康测试结果
 *
 * @retval DL_TRNG_DIGITAL_HEALTH_TEST_SUCCESS if all tests passed,
 *         otherwise a bitmask of which tests passed
 *         全部通过返回 0xFF，否则返回各测试位掩码
 */
__STATIC_INLINE uint8_t DL_TRNG_getDigitalHealthTestResults(
    const TRNG_Regs *trng)
{
    return (uint8_t)(trng->TEST_RESULTS & TRNG_TEST_RESULTS_DIG_TEST_MASK);
}

/**
 * @brief Get the analog health test results
 *        获取模拟健康测试结果
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return The results of the analog health test
 *         模拟健康测试结果
 *
 * @retval DL_TRNG_ANALOG_HEALTH_TEST_SUCCESS if success 测试通过返回 0x1
 * @retval 0 if test failed 测试失败返回 0
 */
__STATIC_INLINE uint8_t DL_TRNG_getAnalogHealthTestResults(
    const TRNG_Regs *trng)
{
    return (uint8_t)((trng->TEST_RESULTS & TRNG_TEST_RESULTS_ANA_TEST_MASK) >>
                     TRNG_TEST_RESULTS_ANA_TEST_OFS);
}

/**
 * @brief Get the current state of the TRNG front end FSM
 *        获取 TRNG 前端状态机的当前状态
 *
 * This API should be called twice since two reads of the state are required as
 * there is a chance of metastability when reading this register.
 * 注意：由于读取该寄存器存在亚稳态风险，需调用两次以确保读取正确
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return The current state of the TRNG
 *         TRNG 当前状态
 *
 * @retval One of @ref DL_TRNG_STATE
 */
__STATIC_INLINE uint32_t DL_TRNG_getCurrentState(const TRNG_Regs *trng)
{
    return ((trng->STAT & TRNG_STAT_FSM_STATE_MASK)) >>
           TRNG_STAT_FSM_STATE_OFS;
}

/**
 * @brief Get the last accepted command that was issued to the TRNG
 *        获取最近一次已接受的 TRNG 命令
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return The last issued accepted command
 *         最近一次已接受的命令
 *
 * @retval One of @ref DL_TRNG_CMD
 */
__STATIC_INLINE uint32_t DL_TRNG_getIssuedCommand(const TRNG_Regs *trng)
{
    return ((trng->STAT & TRNG_STAT_ISSUED_CMD_MASK)) >>
           TRNG_STAT_ISSUED_CMD_OFS;
}

/**
 * @brief Indicates if the repetition counter test caused the most recent
 *        failure
 *        查询最近一次失败是否由重复计数器测试引起
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return The status of the repetition test
 *         重复测试状态
 *
 * @retval true if the repetition test failed 重复测试失败
 * @retval false if the repetition test did not fail 重复测试未失败
 */
__STATIC_INLINE bool DL_TRNG_isRepetitionTestFail(const TRNG_Regs *trng)
{
    return ((trng->STAT & TRNG_STAT_REP_FAIL_MASK)) == TRNG_STAT_REP_FAIL_MASK;
}

/**
 * @brief Indicates if the Adaptive Proportion Test (1,2,3, or 4-bit counters)
 *        caused the most recent failure
 *        查询最近一次失败是否由自适应比例测试引起
 *
 * @param trng  Pointer to the register overlay for the peripheral
 *              外设寄存器基地址指针
 *
 * @return The status of the Adaptive Proportion Test
 *         自适应比例测试状态
 *
 * @retval true if the adaptive proportion test failed 自适应比例测试失败
 * @retval false if the adaptive proportion test did not fail 自适应比例测试未失败
 */
__STATIC_INLINE bool DL_TRNG_isAdaptiveTestFail(const TRNG_Regs *trng)
{
    return ((trng->STAT & TRNG_STAT_ADAP_FAIL_MASK)) ==
           TRNG_STAT_ADAP_FAIL_MASK;
}

/**
 *  @brief      Enable TRNG interrupt
 *              使能 TRNG 中断
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_TRNG_INTERRUPT.
 *                             中断位掩码，可按位或组合
 */
__STATIC_INLINE void DL_TRNG_enableInterrupt(
    TRNG_Regs *trng, uint32_t interruptMask)
{
    trng->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable TRNG interrupt
 *              禁用 TRNG 中断
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_TRNG_INTERRUPT.
 *                             中断位掩码，可按位或组合
 */
__STATIC_INLINE void DL_TRNG_disableInterrupt(
    TRNG_Regs *trng, uint32_t interruptMask)
{
    trng->CPU_INT.IMASK &= ~interruptMask;
}

/**
 *  @brief      Check which TRNG interrupts are enabled
 *              查询哪些 TRNG 中断已使能
 *
 *  @param[in]  trng           Pointer to the register overlay for the
 *                             peripheral
 *                             外设寄存器基地址指针
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_TRNG_IIDX.
 *                             中断位掩码，可按位或组合
 *
 *  @return     Which of the requested TRNG interrupts are enabled
 *              已使能的中断位
 *
 *  @retval     Bitwise OR of @ref DL_TRNG_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_TRNG_getEnabledInterrupts(
    const TRNG_Regs *trng, uint32_t interruptMask)
{
    return trng->CPU_INT.IMASK & interruptMask;
}

/**
 *  @brief      Check interrupt flag of enabled TRNG interrupt
 *              查询已使能的 TRNG 中断标志
 *
 *  Checks if the TRNG interrupt that was previously enabled is pending.
 *
 *  @param[in]  trng           Pointer to the register overlay for the
 *                             peripheral
 *                             外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_TRNG_INTERRUPT.
 *                             中断位掩码，可按位或组合
 *
 *  @return     If the enabled TRNG interrupt is pending
 *              已使能中断的挂起状态
 *
 *  @sa         DL_TRNG_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_TRNG_getEnabledInterruptStatus(
    const TRNG_Regs *trng, uint32_t interruptMask)
{
    return (trng->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any TRNG interrupt
 *              查询任意 TRNG 中断的原始状态
 *
 *  Checks if the TRNG interrupt is pending. Interrupt does not have to
 *  be previously enabled.
 *  无需事先使能中断即可查询
 *
 *  @param[in]  trng           Pointer to the register overlay for the
 *                             peripheral
 *                             外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_TRNG_INTERRUPT.
 *                             中断位掩码，可按位或组合
 *
 *  @return     If the trng interrupt is pending
 *              中断原始挂起状态
 */
__STATIC_INLINE uint32_t DL_TRNG_getRawInterruptStatus(
    const TRNG_Regs *trng, uint32_t interruptMask)
{
    return trng->CPU_INT.RIS & interruptMask;
}

/**
 *  @brief      Get highest priority pending TRNG interrupt
 *              获取最高优先级的挂起 TRNG 中断
 *
 *  Checks if any of the TRNG interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *  无需事先使能中断即可查询
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @return     The highest priority pending TRNG interrupt
 *              最高优先级的挂起中断
 *
 *  @retval     One of @ref DL_TRNG_IIDX
 */
__STATIC_INLINE DL_TRNG_IIDX DL_TRNG_getPendingInterrupt(const TRNG_Regs *trng)
{
    return (DL_TRNG_IIDX) trng->CPU_INT.IIDX;
}

/**
 *  @brief      Clear pending TRNG interrupt
 *              清除挂起的 TRNG 中断
 *
 *  @param[in]  trng           Pointer to the register overlay for the
 *                             peripheral
 *                             外设寄存器基地址指针
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_TRNG_INTERRUPT.
 *                             中断位掩码，可按位或组合
 */
__STATIC_INLINE void DL_TRNG_clearInterruptStatus(
    TRNG_Regs *trng, uint32_t interruptMask)
{
    trng->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Update control register's command bits to send a TRNG command
 *              更新控制寄存器的命令位以发送 TRNG 命令
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @param[in]  cmd   Command to update the control register with
 *                    要发送的命令
 */
__STATIC_INLINE void DL_TRNG_sendCommand(TRNG_Regs *trng, DL_TRNG_CMD cmd)
{
    DL_Common_updateReg(&trng->CTL, (uint32_t) cmd, TRNG_CTL_CMD_MASK);
}

/**
 *  @brief      Get word capture from TRNG
 *              从 TRNG 获取捕获数据
 *
 *  Gets the captured data from the decimation block.
 *  从抽取模块获取已捕获的数据
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @retval     TRNG capture value
 *              TRNG 捕获值
 *
 *  @pre The IRQ_CAPTURED_RDY IRQ should be asserted first, indicating that
 *  random bits are available for read-out.
 *  前置条件：需先确认 IRQ_CAPTURED_RDY 中断已触发，表示随机比特可读取
 *
 *  @sa         DL_TRNG_isCaptureReady
 */
__STATIC_INLINE uint32_t DL_TRNG_getCapture(const TRNG_Regs *trng)
{
    return trng->DATA_CAPTURE;
}

/**
 *  @brief      Save TRNG configuration before entering a power loss state.
 *              进入掉电状态前保存 TRNG 配置
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @param[in]  ptr   Configuration backup setup structure. See
 *                    @ref DL_TRNG_backupConfig.
 *                    配置备份结构体指针
 *
 *  @retval     FALSE if a configuration already exists in ptr (will not be
 *              overwritten). TRUE if a configuration was successfully saved
 *              若已存在备份则返回 FALSE（不覆盖），成功保存返回 TRUE
 */
bool DL_TRNG_saveConfiguration(
    const TRNG_Regs *trng, DL_TRNG_backupConfig *ptr);

/**
 *  @brief      Restore TRNG configuration after leaving a power loss state.
 *              Upon restoration, if the TRNG was not originally in OFF state,
 *              then the TRNG will be set to the normal operating mode.
 *              掉电恢复后还原 TRNG 配置。若原始状态非 OFF，则恢复为正常工作模式
 *
 *  @param[in]  trng  Pointer to the register overlay for the peripheral
 *                    外设寄存器基地址指针
 *
 *  @param[in]  ptr   Configuration backup setup structure. See
 *                    @ref DL_TRNG_backupConfig.
 *                    配置备份结构体指针
 *
 *  @retval     FALSE if a configuration does not exist in ptr (will not be
 *              loaded). TRUE if a configuration successfully loaded
 *              若无备份则返回 FALSE，成功恢复返回 TRUE
 */
bool DL_TRNG_restoreConfiguration(TRNG_Regs *trng, DL_TRNG_backupConfig *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_TRNG__ */

#endif /* ti_dl_dl_trng__include */
/** @}*/
