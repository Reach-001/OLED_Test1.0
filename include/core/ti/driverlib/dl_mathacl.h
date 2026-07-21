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
 *  @file       dl_mathacl.h
 *  @brief      Math Accelerator Driver Library
 *              数学加速器驱动库，提供对 MATHACL 硬件的软件控制接口。
 *              支持三角函数(sin/cos/atan2)、除法、开方、乘法、平方、
 *              累加乘(MAC)、累加平方(SAC)等硬件加速运算。
 *  @defgroup   MATHACL Math Accelerator (MATHACL)
 *
 *  @anchor ti_dl_dl_m0p_mathacl_Overview
 *  # Overview
 *
 *  The Math Accelerator Driver Library provides software to control the
 *  onboard Math Accelerator hardware.
 *
 ******************************************************************************
 */
/** @addtogroup MATHACL
 * @{
 */
#ifndef ti_dl_dl_mathacl__include
#define ti_dl_dl_mathacl__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_MATHACL__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_MATHACL_STATUS
 *  @{
 */

/*!
 * @brief MATHACL operation underflow status
 *        运算下溢状态标志，当运算结果低于可表示范围时置位
 */
#define DL_MATHACL_STATUS_UNDERFLOW               (MATHACL_STATUS_UF_UNDERFLOW)

/*!
 * @brief MATHACL generic underflow bitmask for MATHACL Status register comparison
 *        下溢状态掩码，用于读取 STATUS 寄存器中的下溢位
 */
#define DL_MATHACL_STATUS_UF_MASK                      (MATHACL_STATUS_UF_MASK)

/*!
 * @brief MATHACL operation overflow status
 *        运算溢出状态标志，当运算结果超过可表示范围时置位
 */
#define DL_MATHACL_STATUS_OVERFLOW                (MATHACL_STATUS_OVF_OVERFLOW)

/*!
 * @brief MATHACL generic overflow bitmask for MATHACL Status register comparison
 *        溢出状态掩码，用于读取 STATUS 寄存器中的溢出位
 */
#define DL_MATHACL_STATUS_OVF_MASK                    (MATHACL_STATUS_OVF_MASK)

/*!
 * @brief MATHACL divide by error status
 *        除零错误状态标志，当除法运算的分母为零时置位
 */
#define DL_MATHACL_STATUS_ERR_DIVBY0                (MATHACL_STATUS_ERR_DIVBY0)

/*!
 * @brief MATHACL generic error bits mask
 *        错误位掩码，用于读取 STATUS 寄存器中的所有错误位
 */
#define DL_MATHACL_STATUS_ERR_MASK                    (MATHACL_STATUS_ERR_MASK)

/*!
 * @brief Alias for one-shot sin/cos operation
 *        单次 sin/cos 运算的便捷宏
 */
#define DL_MathACL_startSinCosOperation(MATHACL, config, op)    DL_MathACL_configOperation(MATHACL, config, op, 0)

/*!
 * @brief Alias for one-shot inverse 4-quadrant arctan operation
 *        单次四象限 arctan2 运算的便捷宏
 */
#define DL_MathACL_startArcTan2Operation(MATHACL, config, x, y)    DL_MathACL_configOperation(MATHACL, config, x, y)

/*!
 * @brief Alias for one-shot arctan operation
 *        单次 arctan 运算的便捷宏
 */
#define DL_MathACL_startArcTanOperation(MATHACL, config, x)    DL_MathACL_configOperation(MATHACL, config, x, 0)

/*!
 * @brief Alias for one-shot divide
 *        单次除法运算的便捷宏
 */
#define DL_MathACL_startDivOperation(MATHACL, config, num, den)    DL_MathACL_configOperation(MATHACL, config, num, den)

/*!
 * @brief Alias for one-shot square root operation
 *        单次开方运算的便捷宏
 */
#define DL_MathACL_startSqrtOperation(MATHACL, config, sqrt)    DL_MathACL_configOperation(MATHACL, config, sqrt, 0)

/*!
 * @brief Alias for one-shot multiply operation
 *        单次乘法运算的便捷宏
 */
#define DL_MathACL_startMpyOperation(MATHACL, config, multicand, multiplier)    DL_MathACL_configOperation(MATHACL, config, multicand, multiplier)

/** @}*/

/* clang-format on */

/*! @enum DL_MATHACL_OP_TYPE 运算类型枚举 */
typedef enum {
    /*! Sin or cosine operation */
    /*! 正弦/余弦运算 */
    DL_MATHACL_OP_TYPE_SINCOS = MATHACL_CTL_FUNC_SINCOS,
    /*! Arc tangent 2 operation -- takes in X, Y values */
    /*! 四象限反正切运算，输入 X、Y 值 */
    DL_MATHACL_OP_TYPE_ARCTAN2 = MATHACL_CTL_FUNC_ATAN2,
    /*! Divide operation -- takes in numerator, denominator, division type */
    /*! 除法运算，输入分子、分母 */
    DL_MATHACL_OP_TYPE_DIV = MATHACL_CTL_FUNC_DIV,
    /*! Square root operation -- requires that a scaling factor be provided as well  */
    /*! 开方运算，需要提供缩放因子 */
    DL_MATHACL_OP_TYPE_SQRT = MATHACL_CTL_FUNC_SQRT,
    /*! Multiply operation returns 32-bit result -- takes in two operands */
    /*! 32位乘法运算，输入两个操作数，返回32位结果 */
    DL_MATHACL_OP_TYPE_MPY_32 = MATHACL_CTL_FUNC_MPY32,
    /*! Square operation returns 32-bit result -- takes in one operand. Supports
     * Q and int data formats. Enable saturation with @ref
     * DL_MathACL_enableSaturation and check @ref DL_MATHACL_STATUS_OVERFLOW
     * for overflow. */
    /*! 32位平方运算，输入一个操作数。支持Q格式和整数格式。
     *  通过 @ref DL_MathACL_enableSaturation 使能饱和，溢出检查 @ref DL_MATHACL_STATUS_OVERFLOW */
    DL_MATHACL_OP_TYPE_SQUARE_32 = MATHACL_CTL_FUNC_SQUARE32,
    /*! Multiply operation returns 64-bit result -- takes in two operands */
    /*! 64位乘法运算，输入两个操作数，返回64位结果 */
    DL_MATHACL_OP_TYPE_MPY_64 = MATHACL_CTL_FUNC_MPY64,
    /*! Square operation returns 64-bit result -- takes in one operand */
    /*! 64位平方运算，输入一个操作数，返回64位结果 */
    DL_MATHACL_OP_TYPE_SQUARE_64 = MATHACL_CTL_FUNC_SQUARE64,
    /*! Multiply and Accumulate operation -- takes in two operands. Supports Q
     * and int data formats. Enable saturation with @ref
     * DL_MathACL_enableSaturation and check @ref DL_MATHACL_STATUS_OVERFLOW
     * for overflow. */
    /*! 累加乘运算(MAC)，输入两个操作数。支持Q格式和整数格式。
     *  通过 @ref DL_MathACL_enableSaturation 使能饱和，溢出检查 @ref DL_MATHACL_STATUS_OVERFLOW */
    DL_MATHACL_OP_TYPE_MAC = MATHACL_CTL_FUNC_MAC,
    /*! Square and Accumulate operation -- takes in one operand. Supports Q and
     * int data formats. Enable saturation with @ref
     * DL_MathACL_enableSaturation and check @ref DL_MATHACL_STATUS_OVERFLOW
     * for overflow. */
    /*! 累加平方运算(SAC)，输入一个操作数。支持Q格式和整数格式。
     *  通过 @ref DL_MathACL_enableSaturation 使能饱和，溢出检查 @ref DL_MATHACL_STATUS_OVERFLOW */
    DL_MATHACL_OP_TYPE_SAC = MATHACL_CTL_FUNC_SAC,
} DL_MATHACL_OP_TYPE;

/*! @enum DL_MATHACL_Q_TYPE Q格式定点数类型枚举，Qn表示n位小数位 */
typedef enum {
    /*! Q0 Operand Type */
    /*! Q0格式：纯整数，无小数位 */
    DL_MATHACL_Q_TYPE_Q0 = MATHACL_CTL_QVAL_Q0,
    /*! Q1 Operand Type */
    DL_MATHACL_Q_TYPE_Q1 = MATHACL_CTL_QVAL_Q1,
    /*! Q2 Operand Type */
    DL_MATHACL_Q_TYPE_Q2 = MATHACL_CTL_QVAL_Q2,
    /*! Q3 Operand Type */
    DL_MATHACL_Q_TYPE_Q3 = MATHACL_CTL_QVAL_Q3,
    /*! Q4 Operand Type */
    DL_MATHACL_Q_TYPE_Q4 = MATHACL_CTL_QVAL_Q4,
    /*! Q5 Operand Type */
    DL_MATHACL_Q_TYPE_Q5 = MATHACL_CTL_QVAL_Q5,
    /*! Q6 Operand Type */
    DL_MATHACL_Q_TYPE_Q6 = MATHACL_CTL_QVAL_Q6,
    /*! Q7 Operand Type */
    DL_MATHACL_Q_TYPE_Q7 = MATHACL_CTL_QVAL_Q7,
    /*! Q8 Operand Type */
    DL_MATHACL_Q_TYPE_Q8 = MATHACL_CTL_QVAL_Q8,
    /*! Q9 Operand Type */
    DL_MATHACL_Q_TYPE_Q9 = MATHACL_CTL_QVAL_Q9,
    /*! Q10 Operand Type */
    DL_MATHACL_Q_TYPE_Q10 = MATHACL_CTL_QVAL_Q10,
    /*! Q11 Operand Type */
    DL_MATHACL_Q_TYPE_Q11 = MATHACL_CTL_QVAL_Q11,
    /*! Q12 Operand Type */
    DL_MATHACL_Q_TYPE_Q12 = MATHACL_CTL_QVAL_Q12,
    /*! Q13 Operand Type */
    DL_MATHACL_Q_TYPE_Q13 = MATHACL_CTL_QVAL_Q13,
    /*! Q14 Operand Type */
    DL_MATHACL_Q_TYPE_Q14 = MATHACL_CTL_QVAL_Q14,
    /*! Q15 Operand Type */
    DL_MATHACL_Q_TYPE_Q15 = MATHACL_CTL_QVAL_Q15,
    /*! Q16 Operand Type */
    DL_MATHACL_Q_TYPE_Q16 = MATHACL_CTL_QVAL_Q16,
    /*! Q17 Operand Type */
    DL_MATHACL_Q_TYPE_Q17 = MATHACL_CTL_QVAL_Q17,
    /*! Q18 Operand Type */
    DL_MATHACL_Q_TYPE_Q18 = MATHACL_CTL_QVAL_Q18,
    /*! Q19 Operand Type */
    DL_MATHACL_Q_TYPE_Q19 = MATHACL_CTL_QVAL_Q19,
    /*! Q20 Operand Type */
    DL_MATHACL_Q_TYPE_Q20 = MATHACL_CTL_QVAL_Q20,
    /*! Q21 Operand Type */
    DL_MATHACL_Q_TYPE_Q21 = MATHACL_CTL_QVAL_Q21,
    /*! Q22 Operand Type */
    DL_MATHACL_Q_TYPE_Q22 = MATHACL_CTL_QVAL_Q22,
    /*! Q23 Operand Type */
    DL_MATHACL_Q_TYPE_Q23 = MATHACL_CTL_QVAL_Q23,
    /*! Q24 Operand Type */
    DL_MATHACL_Q_TYPE_Q24 = MATHACL_CTL_QVAL_Q24,
    /*! Q25 Operand Type */
    DL_MATHACL_Q_TYPE_Q25 = MATHACL_CTL_QVAL_Q25,
    /*! Q26 Operand Type */
    DL_MATHACL_Q_TYPE_Q26 = MATHACL_CTL_QVAL_Q26,
    /*! Q27 Operand Type */
    DL_MATHACL_Q_TYPE_Q27 = MATHACL_CTL_QVAL_Q27,
    /*! Q28 Operand Type */
    DL_MATHACL_Q_TYPE_Q28 = MATHACL_CTL_QVAL_Q28,
    /*! Q29 Operand Type */
    DL_MATHACL_Q_TYPE_Q29 = MATHACL_CTL_QVAL_Q29,
    /*! Q30 Operand Type */
    DL_MATHACL_Q_TYPE_Q30 = MATHACL_CTL_QVAL_Q30,
    /*! Q31 Operand Type */
    DL_MATHACL_Q_TYPE_Q31 = MATHACL_CTL_QVAL_Q31,
} DL_MATHACL_Q_TYPE;

/*! @enum DL_MATHACL_OPSIGN 操作数符号类型枚举 */
typedef enum {
    /*! Unsigned operation type */
    /*! 无符号运算 */
    DL_MATHACL_OPSIGN_UNSIGNED = MATHACL_CTL_OPTYPE_UNSIGNED,
    /*! Signed operation type */
    /*! 有符号运算 */
    DL_MATHACL_OPSIGN_SIGNED = MATHACL_CTL_OPTYPE_SIGNED,
} DL_MATHACL_OPSIGN;

/**
 * @brief Configuration struct for @ref DL_MathACL_configOperation.
 *        运算配置结构体，用于配置 MATHACL 的运算参数
 */
typedef struct {
    /*! Operation type. Set to one of @ref DL_MATHACL_OP_TYPE */
    /*! 运算类型，取值为 @ref DL_MATHACL_OP_TYPE 中的枚举值 */
    DL_MATHACL_OP_TYPE opType;
    /*! Operation sign type. One of @ref DL_MATHACL_OPSIGN */
    /*! 操作数符号类型，取值为 @ref DL_MATHACL_OPSIGN 中的枚举值 */
    DL_MATHACL_OPSIGN opSign;
    /*! Number of iterations, for applicable operation types. Supported types
        include trigonometric operations and square root. */
    /*! 迭代次数，适用于三角函数和开方运算，影响计算精度 */
    uint32_t iterations;
    /*! Scaling factor on the input in division operations. Leave as 0
        for non-division operations. */
    /*! 除法运算的输入缩放因子，非除法运算时设为0 */
    uint32_t scaleFactor;
    /*! Q value for the operand inputs for the operation */
    /*! 操作数的Q格式定点数类型 */
    DL_MATHACL_Q_TYPE qType;
} DL_MathACL_operationConfig;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the MATHACL
 *        使能 MATHACL 外设电源，写入 ENABLE 位和 KEY 值到 PWREN 寄存器
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 * @note If using compiler TI Clang LTS 4.0.0+ with the --mathacl flag, do not reset
 * the MATHACL then perform a math divide. The peripheral will need to be reenabled
 * with @ref DL_MathACL_enablePower; before performing a division.
 *
 * @param mathacl       Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 */
__STATIC_INLINE void DL_MathACL_enablePower(MATHACL_Regs *mathacl)
{
    mathacl->GPRCM.PWREN =
        MATHACL_PWREN_KEY_UNLOCK_W | MATHACL_PWREN_ENABLE_ENABLE;
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the MATHACL
 *        禁用 MATHACL 外设电源，清除 PWREN.ENABLE 位后外设寄存器不可访问
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 *  @note This API does not provide large power savings.
 *
 * @param mathacl       Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 */
__STATIC_INLINE void DL_MathACL_disablePower(MATHACL_Regs *mathacl)
{
    mathacl->GPRCM.PWREN =
        MATHACL_PWREN_KEY_UNLOCK_W | MATHACL_PWREN_ENABLE_DISABLE;
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the MATHACL
 *        is enabled
 *        查询 MATHACL 外设电源是否已使能
 *
 *  Before any peripheral registers can be configured by software, the
 *  peripheral itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the peripheral's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the peripheral's registers are not
 *  accessible for read/write operations.
 *
 * @param mathacl        Pointer to the register overlay for the peripheral
 *                       外设寄存器基地址指针
 *
 * @return true if peripheral register access is enabled 外设已使能
 * @return false if peripheral register access is disabled 外设已禁用
 */
__STATIC_INLINE bool DL_MathACL_isPowerEnabled(MATHACL_Regs *mathacl)
{
    return ((mathacl->GPRCM.PWREN & MATHACL_PWREN_ENABLE_MASK) ==
            MATHACL_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets the MATHACL module
 *        复位 MATHACL 模块
 *
 * @param mathacl       Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 */
__STATIC_INLINE void DL_MathACL_reset(MATHACL_Regs *mathacl)
{
    mathacl->GPRCM.RSTCTL =
        MATHACL_RSTCTL_RESETASSERT_ASSERT | MATHACL_RSTCTL_KEY_UNLOCK_W;
}

/**
 * @brief Returns if MATHACL peripheral was reset
 *        查询 MATHACL 外设是否已复位
 *
 * @param mathacl        Pointer to the register overlay for the MATHACL peripheral
 *                       外设寄存器基地址指针
 *
 * @return true if peripheral was reset 外设已复位
 * @return false if peripheral wasn't reset 外设未复位
 *
 */
__STATIC_INLINE bool DL_MathACL_isReset(const MATHACL_Regs *mathacl)
{
    return (mathacl->GPRCM.STAT & MATHACL_STAT_RESETSTKY_MASK) ==
           MATHACL_STAT_RESETSTKY_RESET;
}

/**
 * @brief Enable saturation
 *        使能饱和模式，溢出时结果钳位到最大值而非回绕
 *
 * This bit is shared among @ref DL_MATHACL_OP_TYPE_DIV, @ref
 * DL_MATHACL_OP_TYPE_SQUARE_32, @ref DL_MATHACL_OP_TYPE_MPY_32, @ref
 * DL_MATHACL_OP_TYPE_MAC, and @ref DL_MATHACL_OP_TYPE_SAC operations.
 * When saturation is enabled, if there is an overflow event during an
 * operation, then the result will saturate to the maximum value.
 *
 * @param mathacl       Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 * @sa    DL_MATHACL_STATUS_OVERFLOW
 */
__STATIC_INLINE void DL_MathACL_enableSaturation(MATHACL_Regs *mathacl)
{
    mathacl->CTL |= MATHACL_CTL_SATEN_ENABLE;
}

/**
 * @brief Disable saturation
 *        禁用饱和模式，溢出时结果将回绕到未知值
 *
 * When saturation is disabled, if there is an overflow event during an
 * operation, then the result will overflow to an unknown value.
 *
 * @param mathacl       Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 */
__STATIC_INLINE void DL_MathACL_disableSaturation(MATHACL_Regs *mathacl)
{
    mathacl->CTL &= ~(MATHACL_CTL_SATEN_MASK);
}

/**
 * @brief Returns if saturation is enabled
 *        查询饱和模式是否已使能
 *
 * @param mathacl     Pointer to the register overlay for the MATHACL peripheral
 *                    外设寄存器基地址指针
 *
 * @return true if saturation is enabled 饱和模式已使能
 * @return false if saturation is not enabled 饱和模式未使能
 *
 */
__STATIC_INLINE bool DL_MathACL_isSaturationEnabled(
    const MATHACL_Regs *mathacl)
{
    return (
        (mathacl->CTL & MATHACL_CTL_SATEN_MASK) == MATHACL_CTL_SATEN_ENABLE);
}

/**
 *  @brief      Load Operand one into the math accelerator
 *              向数学加速器加载操作数1
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @param[in]  op      Operand one to load into the Math Accelerator
 *                      要加载的操作数1
 *
 */
__STATIC_INLINE void DL_MathACL_setOperandOne(
    MATHACL_Regs *mathacl, uint32_t op)
{
    mathacl->OP1 = op;
}

/**
 *  @brief      Load Operand two into the math accelerator
 *              向数学加速器加载操作数2
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @param[in]  op  Operand two to load into the Math Accelerator
 *                  要加载的操作数2
 *
 */
__STATIC_INLINE void DL_MathACL_setOperandTwo(
    MATHACL_Regs *mathacl, uint32_t op)
{
    mathacl->OP2 = op;
}

/**
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @brief Returns MATHACL result one register
 *         读取 MATHACL 结果寄存器1(RES1)
 *
 *  @return Value stored in MATHACUL Result 1 status
 *          RES1 寄存器中存储的值
 */
__STATIC_INLINE uint32_t DL_MathACL_getResultOne(const MATHACL_Regs *mathacl)
{
    return mathacl->RES1;
}

/**
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @brief Returns MATHACL result two register
 *         读取 MATHACL 结果寄存器2(RES2)
 *
 *  @return Value stored in MATHACUL Result 2 status
 *          RES2 寄存器中存储的值
 *
 */
__STATIC_INLINE uint32_t DL_MathACL_getResultTwo(const MATHACL_Regs *mathacl)
{
    return mathacl->RES2;
}

/**
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @brief Returns MATHACL operation status
 *         读取 MATHACL 运行状态寄存器
 *
 *  @return One of @ref DL_MATHACL_STATUS
 *          状态值，可与 @ref DL_MATHACL_STATUS 宏进行比较
 *
 */
__STATIC_INLINE uint32_t DL_MathACL_getStatus(const MATHACL_Regs *mathacl)
{
    return mathacl->STATUS;
}

/**
 *  @brief      Clear the overflow status bit
 *              清除溢出状态位
 *
 *  Calling this API clears @ref DL_MATHACL_STATUS_OVERFLOW
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @sa         DL_MATHACL_STATUS_OVERFLOW
 */
__STATIC_INLINE void DL_MathACL_clearOverflowStatus(MATHACL_Regs *mathacl)
{
    mathacl->STATUSCLR |= MATHACL_STATUSCLR_CLR_OVF_CLR;
}

/**
 *  @brief      Clear the underflow status bit
 *              清除下溢状态位
 *
 *  Calling this API clears @ref DL_MATHACL_STATUS_UNDERFLOW
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @sa         DL_MATHACL_STATUS_UNDERFLOW
 */
__STATIC_INLINE void DL_MathACL_clearUnderflowStatus(MATHACL_Regs *mathacl)
{
    mathacl->STATUSCLR |= MATHACL_STATUSCLR_CLR_UF_CLR;
}

/**
 *  @brief      Clear the error status bit
 *              清除错误状态位
 *
 *  Calling this API clears @ref DL_MATHACL_STATUS_ERR_MASK
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 *  @sa         DL_MATHACL_STATUS_ERR_MASK
 */
__STATIC_INLINE void DL_MathACL_clearErrorStatus(MATHACL_Regs *mathacl)
{
    mathacl->STATUSCLR |= MATHACL_STATUSCLR_CLR_ERR_CLR;
}

/**
 *  @brief      Block until MATHACL_STATUS_BUSY_DONE is active in MATHACL
 *              阻塞等待 MATHACL 运算完成（BUSY 位变为 DONE）
 *
 *  @param[in]  mathacl  Pointer to the register overlay for the peripheral
 *                       外设寄存器基地址指针
 *
 */
__STATIC_INLINE void DL_MathACL_waitForOperation(const MATHACL_Regs *mathacl)
{
    // 轮询 STATUS 寄存器的 BUSY 位，直到运算完成
    while ((mathacl->STATUS & MATHACL_STATUS_BUSY_MASK) ==
           MATHACL_STATUS_BUSY_NOTDONE) {
        ;
    }
}

/**
 *  @brief      Configure and execute a Math accelerator operation
 *              配置并执行一次数学加速器运算
 *
 *  @pre        Call @ref DL_MathACL_clearResults when configuring the MATHACL
 *              for performing @ref DL_MATHACL_OP_TYPE_MAC and
 *              @ref DL_MATHACL_OP_TYPE_SAC operations.
 *              执行 MAC/SAC 运算前需先调用 @ref DL_MathACL_clearResults 清除结果寄存器
 *
 *  @param[in]  mathacl   Pointer to the register overlay for the peripheral
 *                        外设寄存器基地址指针
 *
 *  @param[in]  opConfig  Pointer to the configuration structure for the
 *                        operation
 *                        运算配置结构体指针
 *
 *  @param[in]  op1       Operand one for the operation
 *                        操作数1
 *
 *  @param[in]  op2       Operand two for the operation. May be left as 0
 *                        if unused
 *                        操作数2，不使用时设为0
 *
 */
void DL_MathACL_configOperation(MATHACL_Regs *mathacl,
    const DL_MathACL_operationConfig *opConfig, uint32_t op1, uint32_t op2);

/**
 *  @brief      Clears the RES1 register
 *              清除结果寄存器1(RES1)
 *
 *  Clears the Result 1 (RES1) register
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 */
__STATIC_INLINE void DL_MathACL_clearResultOne(MATHACL_Regs *mathacl)
{
    mathacl->RES1 = 0;
}

/**
 *  @brief      Clears the RES2 register
 *              清除结果寄存器2(RES2)
 *
 *  Clears the Result 2 (RES2) register
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 */
__STATIC_INLINE void DL_MathACL_clearResultTwo(MATHACL_Regs *mathacl)
{
    mathacl->RES2 = 0;
}

/**
 *  @brief      Clears the RES1 and RES2 registers
 *              同时清除结果寄存器1(RES1)和结果寄存器2(RES2)
 *
 *  Clears the Result 1 (RES1) and Result 2 (RES2) registers
 *
 *  @param[in]  mathacl Pointer to the register overlay for the peripheral
 *                      外设寄存器基地址指针
 *
 */
__STATIC_INLINE void DL_MathACL_clearResults(MATHACL_Regs *mathacl)
{
    mathacl->RES1 = 0;
    mathacl->RES2 = 0;
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_MATHACL__ */

#endif /* ti_dl_mathacl__include */
/** @}*/
