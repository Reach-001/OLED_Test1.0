/*****************************************************************************

  Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

   Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

   Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/**
 * @file hw_mathacl.h
 * @brief MATHACL (数学加速器) 硬件寄存器定义
 *
 * 本文件定义了 MSPM0G3507 微控制器中数学加速器(MATHACL)模块的全部寄存器
 * 和位域。MATHACL 提供硬件加速的数学运算，包括：
 * - 三角函数：正弦/余弦(SINCOS)、反正切(ATAN2)
 * - 除法运算：支持有符号/无符号除法，结果包含商和余数
 * - 平方根：SQRT 运算，支持缩放因子
 * - 乘法运算：32位/64位乘法(MPY32/MPY64)、平方(SQUARE32/SQUARE64)
 * - 累加运算：乘累加(MAC)、平方累加(SAC)
 * - 支持 Q 格式定点数运算（Q0~Q31）
 * - 支持饱和使能控制
 */

#ifndef ti_devices_msp_peripherals_hw_mathacl__include
#define ti_devices_msp_peripherals_hw_mathacl__include

/* This preliminary header file does not have a version number */
/* MMR repo: https://bitbucket.itg.ti.com/projects/cmcu_msp65ip/repos/f65mspmathacl */
/* MMR revision: 9080fe468791d796cb4aed2ae9531011e6786af0 */
/* Generator revision: 77992b62fb4e9926f5a9143aae1e89fec6a84738
   (MInT: ec7ec7482a60c6871be32db8b378ec27aa4771f6) */

#ifndef __CORTEX_M
  #ifdef __cplusplus
    #define __I  volatile        /*!< Defines 'read only' permissions */
  #else
    #define __I  volatile const  /*!< Defines 'read only' permissions */
  #endif
  #define __O  volatile          /*!< Defines 'write only' permissions */
  #define __IO  volatile         /*!< Defines 'read / write' permissions */

  /* following defines should be used for structure members */
  #define __IM  volatile const   /*! Defines 'read only' structure member permissions */
  #define __OM  volatile         /*! Defines 'write only' structure member permissions */
  #define __IOM  volatile        /*! Defines 'read / write' structure member permissions */
#endif

/* Use standard integer types with explicit width */
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#if defined ( __CC_ARM )
#pragma anon_unions
#endif

/******************************************************************************
* MATHACL Registers - MATHACL 寄存器定义
******************************************************************************/

/* GPRCM 基地址偏移 */
#define MATHACL_GPRCM_OFS                        ((uint32_t)0x00000800U)


/** @addtogroup MATHACL_GPRCM
  @{
*/

/* GPRCM (通用电源/复位控制模块) 寄存器组 */
typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) 电源使能寄存器 */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) 复位控制寄存器 */
       uint32_t RESERVED0[3];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) 状态寄存器 */
} MATHACL_GPRCM_Regs;

/*@}*/ /* end of group MATHACL_GPRCM */

/** @addtogroup MATHACL
  @{
*/

/* MATHACL 主寄存器组 */
typedef struct {
       uint32_t RESERVED0[512];
  MATHACL_GPRCM_Regs  GPRCM;                             /* !< (@ 0x00000800) GPRCM寄存器组 */
       uint32_t RESERVED1[570];
  __IO uint32_t CTL;                               /* !< (@ 0x00001100) 控制寄存器 - 选择运算功能和操作数格式 */
       uint32_t RESERVED2[5];
  __IO uint32_t OP2;                               /* !< (@ 0x00001118) 操作数2寄存器 */
  __IO uint32_t OP1;                               /* !< (@ 0x0000111C) 操作数1寄存器（写入此寄存器启动运算） */
  __IO uint32_t RES1;                              /* !< (@ 0x00001120) 结果1寄存器（低32位） */
  __IO uint32_t RES2;                              /* !< (@ 0x00001124) 结果2寄存器（高32位，用于64位结果） */
       uint32_t RESERVED3[2];
  __I  uint32_t STATUS;                            /* !< (@ 0x00001130) 状态寄存器（溢出/下溢/忙等标志） */
       uint32_t RESERVED4[3];
  __O  uint32_t STATUSCLR;                         /* !< (@ 0x00001140) 状态标志清除寄存器（写1清除对应标志） */
} MATHACL_Regs;

/*@}*/ /* end of group MATHACL */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* MATHACL Register Control Bits - MATHACL 寄存器控制位域
******************************************************************************/

/* ===== 电源使能寄存器 (PWREN) 位域 ===== */
/* MATHACL_PWREN Bits */
/* MATHACL_PWREN[ENABLE] 电源使能位 */
/* MATHACL_PWREN[ENABLE] Bits */
#define MATHACL_PWREN_ENABLE_OFS                 (0)                             /* !< ENABLE Offset */
#define MATHACL_PWREN_ENABLE_MASK                ((uint32_t)0x00000001U)         /* !< Enable the power */
#define MATHACL_PWREN_ENABLE_DISABLE             ((uint32_t)0x00000000U)         /* !< Disable Power */
#define MATHACL_PWREN_ENABLE_ENABLE              ((uint32_t)0x00000001U)         /* !< Enable Power */
/* MATHACL_PWREN[KEY] 写保护密钥（需写入0x26解锁） */
/* MATHACL_PWREN[KEY] Bits */
#define MATHACL_PWREN_KEY_OFS                    (24)                            /* !< KEY Offset */
#define MATHACL_PWREN_KEY_MASK                   ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define MATHACL_PWREN_KEY_UNLOCK_W               ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* ===== 复位控制寄存器 (RSTCTL) 位域 ===== */
/* MATHACL_RSTCTL Bits */
/* MATHACL_RSTCTL[RESETSTKYCLR] 复位粘滞标志清除位 */
/* MATHACL_RSTCTL[RESETSTKYCLR] Bits */
#define MATHACL_RSTCTL_RESETSTKYCLR_OFS          (1)                             /* !< RESETSTKYCLR Offset */
#define MATHACL_RSTCTL_RESETSTKYCLR_MASK         ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define MATHACL_RSTCTL_RESETSTKYCLR_NOP          ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define MATHACL_RSTCTL_RESETSTKYCLR_CLR          ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* MATHACL_RSTCTL[RESETASSERT] 复位置位位（写1触发模块复位） */
/* MATHACL_RSTCTL[RESETASSERT] Bits */
#define MATHACL_RSTCTL_RESETASSERT_OFS           (0)                             /* !< RESETASSERT Offset */
#define MATHACL_RSTCTL_RESETASSERT_MASK          ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define MATHACL_RSTCTL_RESETASSERT_NOP           ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define MATHACL_RSTCTL_RESETASSERT_ASSERT        ((uint32_t)0x00000001U)         /* !< Assert reset */
/* MATHACL_RSTCTL[KEY] 写保护密钥（需写入0xB1解锁） */
/* MATHACL_RSTCTL[KEY] Bits */
#define MATHACL_RSTCTL_KEY_OFS                   (24)                            /* !< KEY Offset */
#define MATHACL_RSTCTL_KEY_MASK                  ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define MATHACL_RSTCTL_KEY_UNLOCK_W              ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* ===== 状态寄存器 (STAT) 位域 ===== */
/* MATHACL_STAT Bits */
/* MATHACL_STAT[RESETSTKY] 复位粘滞标志（指示模块是否经历过复位） */
/* MATHACL_STAT[RESETSTKY] Bits */
#define MATHACL_STAT_RESETSTKY_OFS               (16)                            /* !< RESETSTKY Offset */
#define MATHACL_STAT_RESETSTKY_MASK              ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define MATHACL_STAT_RESETSTKY_NORES             ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define MATHACL_STAT_RESETSTKY_RESET             ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */

/* ===== 控制寄存器 (CTL) 位域 ===== */
/* MATHACL_CTL Bits */
/* MATHACL_CTL[FUNC] 运算功能选择（5位，范围0~11） */
/* MATHACL_CTL[FUNC] Bits */
#define MATHACL_CTL_FUNC_OFS                     (0)                             /* !< FUNC Offset */
#define MATHACL_CTL_FUNC_MASK                    ((uint32_t)0x0000001FU)         /* !< ULP_ADCHP Enable Conversions. */
#define MATHACL_CTL_FUNC_NOP                     ((uint32_t)0x00000000U)         /* !< No operation */
#define MATHACL_CTL_FUNC_SINCOS                  ((uint32_t)0x00000001U)         /* !< Sine and Cosine operation */
#define MATHACL_CTL_FUNC_ATAN2                   ((uint32_t)0x00000002U)         /* !< Arc tangent with x and y values as
                                                                                    operands. */
#define MATHACL_CTL_FUNC_DIV                     ((uint32_t)0x00000004U)         /* !< Divide, the operands are numerator,
                                                                                    denominator, and the divide type.
                                                                                    Result is the quotient and reminder. */
#define MATHACL_CTL_FUNC_SQRT                    ((uint32_t)0x00000005U)         /* !< Do square root. Operand is the
                                                                                    number whoose square root needs to be
                                                                                    computed. The number if outside the
                                                                                    range needs to be scaled up down by
                                                                                    2 power 2n to bring it with in the
                                                                                    range. */
#define MATHACL_CTL_FUNC_MPY32                   ((uint32_t)0x00000006U)         /* !< 32-bit Multiply Result */
#define MATHACL_CTL_FUNC_SQUARE32                ((uint32_t)0x00000007U)         /* !< 32-bit square result */
#define MATHACL_CTL_FUNC_MPY64                   ((uint32_t)0x00000008U)         /* !< 64-bit multiply result */
#define MATHACL_CTL_FUNC_SQUARE64                ((uint32_t)0x00000009U)         /* !< 64-bit multiply result */
#define MATHACL_CTL_FUNC_MAC                     ((uint32_t)0x0000000AU)         /* !< Multiply and accumulate operation */
#define MATHACL_CTL_FUNC_SAC                     ((uint32_t)0x0000000BU)         /* !< Square and accumulate operation */
/* MATHACL_CTL[QVAL] Q格式定点数小数位数（5位，范围0~31，用于除法运算） */
/* MATHACL_CTL[QVAL] Bits */
#define MATHACL_CTL_QVAL_OFS                     (8)                             /* !< QVAL Offset */
#define MATHACL_CTL_QVAL_MASK                    ((uint32_t)0x00001F00U)         /* !< Indicates the fractional bits in
                                                                                    the operands, ranges from 0 to 31.
                                                                                    Applicable to DIV function. */
#define MATHACL_CTL_QVAL_Q0                      ((uint32_t)0x00000000U)         /* !< Q0 operands */
#define MATHACL_CTL_QVAL_Q1                      ((uint32_t)0x00000100U)         /* !< Q1 operands */
#define MATHACL_CTL_QVAL_Q2                      ((uint32_t)0x00000200U)         /* !< Q2 operands */
#define MATHACL_CTL_QVAL_Q3                      ((uint32_t)0x00000300U)         /* !< Q3 operands */
#define MATHACL_CTL_QVAL_Q4                      ((uint32_t)0x00000400U)         /* !< Q4 operands */
#define MATHACL_CTL_QVAL_Q5                      ((uint32_t)0x00000500U)         /* !< Q5 operands */
#define MATHACL_CTL_QVAL_Q6                      ((uint32_t)0x00000600U)         /* !< Q6 operands */
#define MATHACL_CTL_QVAL_Q7                      ((uint32_t)0x00000700U)         /* !< Q7 operands */
#define MATHACL_CTL_QVAL_Q8                      ((uint32_t)0x00000800U)         /* !< Q8 operands */
#define MATHACL_CTL_QVAL_Q9                      ((uint32_t)0x00000900U)         /* !< Q9 operands */
#define MATHACL_CTL_QVAL_Q10                     ((uint32_t)0x00000A00U)         /* !< Q10 operands */
#define MATHACL_CTL_QVAL_Q11                     ((uint32_t)0x00000B00U)         /* !< Q11 operands */
#define MATHACL_CTL_QVAL_Q12                     ((uint32_t)0x00000C00U)         /* !< Q12 operands */
#define MATHACL_CTL_QVAL_Q13                     ((uint32_t)0x00000D00U)         /* !< Q13 operands */
#define MATHACL_CTL_QVAL_Q14                     ((uint32_t)0x00000E00U)         /* !< Q14 operands */
#define MATHACL_CTL_QVAL_Q15                     ((uint32_t)0x00000F00U)         /* !< Q15 operands */
#define MATHACL_CTL_QVAL_Q16                     ((uint32_t)0x00001000U)         /* !< Q16 operands */
#define MATHACL_CTL_QVAL_Q17                     ((uint32_t)0x00001100U)         /* !< Q17 operands */
#define MATHACL_CTL_QVAL_Q18                     ((uint32_t)0x00001200U)         /* !< Q18 operands */
#define MATHACL_CTL_QVAL_Q19                     ((uint32_t)0x00001300U)         /* !< Q19 operands */
#define MATHACL_CTL_QVAL_Q20                     ((uint32_t)0x00001400U)         /* !< Q20 operands */
#define MATHACL_CTL_QVAL_Q21                     ((uint32_t)0x00001500U)         /* !< Q21 operands */
#define MATHACL_CTL_QVAL_Q22                     ((uint32_t)0x00001600U)         /* !< Q22 operands */
#define MATHACL_CTL_QVAL_Q23                     ((uint32_t)0x00001700U)         /* !< Q23 operands */
#define MATHACL_CTL_QVAL_Q24                     ((uint32_t)0x00001800U)         /* !< Q24 operands */
#define MATHACL_CTL_QVAL_Q25                     ((uint32_t)0x00001900U)         /* !< Q25 operands */
#define MATHACL_CTL_QVAL_Q26                     ((uint32_t)0x00001A00U)         /* !< Q26 operands */
#define MATHACL_CTL_QVAL_Q27                     ((uint32_t)0x00001B00U)         /* !< Q27 operands */
#define MATHACL_CTL_QVAL_Q28                     ((uint32_t)0x00001C00U)         /* !< Q28 operands */
#define MATHACL_CTL_QVAL_Q29                     ((uint32_t)0x00001D00U)         /* !< Q29 operands */
#define MATHACL_CTL_QVAL_Q30                     ((uint32_t)0x00001E00U)         /* !< Q30 operands */
#define MATHACL_CTL_QVAL_Q31                     ((uint32_t)0x00001F00U)         /* !< Q31 operands */
/* MATHACL_CTL[OPTYPE] 操作数类型（0=无符号，1=有符号，用于除法运算） */
/* MATHACL_CTL[OPTYPE] Bits */
#define MATHACL_CTL_OPTYPE_OFS                   (5)                             /* !< OPTYPE Offset */
#define MATHACL_CTL_OPTYPE_MASK                  ((uint32_t)0x00000020U)         /* !< Operand type, could signed or
                                                                                    unsigned. applicable to DIV function. */
#define MATHACL_CTL_OPTYPE_UNSIGNED              ((uint32_t)0x00000000U)         /* !< Unsigned operands */
#define MATHACL_CTL_OPTYPE_SIGNED                ((uint32_t)0x00000020U)         /* !< Signed operands. */
/* MATHACL_CTL[NUMITER] 迭代次数（5位，用于SINCOS/ATAN2/SQRT，0表示31次） */
/* MATHACL_CTL[NUMITER] Bits */
#define MATHACL_CTL_NUMITER_OFS                  (24)                            /* !< NUMITER Offset */
#define MATHACL_CTL_NUMITER_MASK                 ((uint32_t)0x1F000000U)         /* !< Number of iterations, applicable if
                                                                                    the function does the computations
                                                                                    iteratively, for example
                                                                                    sine/cosine/atan2/sqrt. Note: A value
                                                                                    of 0 is interpreted as 31. */
/* MATHACL_CTL[SFACTOR] 缩放因子（6位，用于SQRT运算，输入需缩放到2^(+/-n)范围） */
/* MATHACL_CTL[SFACTOR] Bits */
#define MATHACL_CTL_SFACTOR_OFS                  (16)                            /* !< SFACTOR Offset */
#define MATHACL_CTL_SFACTOR_MASK                 ((uint32_t)0x003F0000U)         /* !< Scaling factor. In case of SQRT
                                                                                    function, the input operand needs to
                                                                                    be in a range. If not it has to be
                                                                                    scaled to 2^(+/-n). This field should
                                                                                    be written with the value 'n'. */
/* MATHACL_CTL[SATEN] 饱和使能位（溢出时结果饱和到最大值，适用于DIV/SQUARE32/MPY32/MAC/SAC） */
/* MATHACL_CTL[SATEN] Bits */
#define MATHACL_CTL_SATEN_OFS                    (22)                            /* !< SATEN Offset */
#define MATHACL_CTL_SATEN_MASK                   ((uint32_t)0x00400000U)         /* !< Saturation enable This bit is
                                                                                    shared among DIV, SQUARE32, MPY32,
                                                                                    MAC and SAC functions. When enabled,
                                                                                    it will make the result to saturate
                                                                                    to maximum value in case of an
                                                                                    overflow event When disabled, the
                                                                                    result will overflow to an unknown
                                                                                    value. */
#define MATHACL_CTL_SATEN_DISABLE                ((uint32_t)0x00000000U)         /* !< Saturation is disabled */
#define MATHACL_CTL_SATEN_ENABLE                 ((uint32_t)0x00400000U)         /* !< Saturation is enabled */

/* ===== 操作数/结果寄存器位域 ===== */
/* MATHACL_OP2 Bits - 操作数2 */
/* MATHACL_OP2[DATA] Bits */
#define MATHACL_OP2_DATA_OFS                     (0)                             /* !< DATA Offset */
#define MATHACL_OP2_DATA_MASK                    ((uint32_t)0xFFFFFFFFU)         /* !< Operand 2 Register */

/* MATHACL_OP1 Bits - 操作数1（写入此寄存器触发运算开始） */
/* MATHACL_OP1[DATA] Bits */
#define MATHACL_OP1_DATA_OFS                     (0)                             /* !< DATA Offset */
#define MATHACL_OP1_DATA_MASK                    ((uint32_t)0xFFFFFFFFU)         /* !< Operand 1 register. */

/* MATHACL_RES1 Bits - 结果1（低32位，或除法的商） */
/* MATHACL_RES1[DATA] Bits */
#define MATHACL_RES1_DATA_OFS                    (0)                             /* !< DATA Offset */
#define MATHACL_RES1_DATA_MASK                   ((uint32_t)0xFFFFFFFFU)         /* !< Result 1 register */

/* MATHACL_RES2 Bits - 结果2（高32位，或除法的余数） */
/* MATHACL_RES2[DATA] Bits */
#define MATHACL_RES2_DATA_OFS                    (0)                             /* !< DATA Offset */
#define MATHACL_RES2_DATA_MASK                   ((uint32_t)0xFFFFFFFFU)         /* !< Result 2 register */

/* ===== 状态寄存器 (STATUS) 位域 ===== */
/* MATHACL_STATUS Bits */
/* MATHACL_STATUS[UF] 下溢标志位 */
/* MATHACL_STATUS[UF] Bits */
#define MATHACL_STATUS_UF_OFS                    (0)                             /* !< UF Offset */
#define MATHACL_STATUS_UF_MASK                   ((uint32_t)0x00000001U)         /* !< Underflow Flag */
#define MATHACL_STATUS_UF_NO_UNDERFLOW           ((uint32_t)0x00000000U)         /* !< No undreflow error. */
#define MATHACL_STATUS_UF_UNDERFLOW              ((uint32_t)0x00000001U)         /* !< Underflow error. */
/* MATHACL_STATUS[OVF] 溢出标志位（适用于MPY32/SQUARE32/DIV/MAC/SAC） */
/* MATHACL_STATUS[OVF] Bits */
#define MATHACL_STATUS_OVF_OFS                   (1)                             /* !< OVF Offset */
#define MATHACL_STATUS_OVF_MASK                  ((uint32_t)0x00000002U)         /* !< Overflow bit for MPY32, SQUARE32,
                                                                                    DIV, MAC, and SAC functions This bit
                                                                                    will be set on overflow and will
                                                                                    retain its value until cleared by
                                                                                    writing 1 into CLR.CLR_OVF */
#define MATHACL_STATUS_OVF_NO_OVERFLOW           ((uint32_t)0x00000000U)         /* !< No overflow error. */
#define MATHACL_STATUS_OVF_OVERFLOW              ((uint32_t)0x00000000U)         /* !< Overflow error. */
/* MATHACL_STATUS[ERR] 错误标志位（2位，指示输入/输出错误） */
/* MATHACL_STATUS[ERR] Bits */
#define MATHACL_STATUS_ERR_OFS                   (2)                             /* !< ERR Offset */
#define MATHACL_STATUS_ERR_MASK                  ((uint32_t)0x0000000CU)         /* !< Incorrect inputs/outputs. */
#define MATHACL_STATUS_ERR_NOERROR               ((uint32_t)0x00000000U)         /* !< No Error in computation. */
#define MATHACL_STATUS_ERR_DIVBY0                ((uint32_t)0x00000004U)         /* !< DIVBY0 error */
/* MATHACL_STATUS[BUSY] 忙标志位（1=运算进行中，0=运算完成） */
/* MATHACL_STATUS[BUSY] Bits */
#define MATHACL_STATUS_BUSY_OFS                  (8)                             /* !< BUSY Offset */
#define MATHACL_STATUS_BUSY_MASK                 ((uint32_t)0x00000100U)         /* !< MATHACL busy bit. */
#define MATHACL_STATUS_BUSY_DONE                 ((uint32_t)0x00000000U)         /* !< Compute has completed. */
#define MATHACL_STATUS_BUSY_NOTDONE              ((uint32_t)0x00000100U)         /* !< Compute ongoing */

/* ===== 状态清除寄存器 (STATUSCLR) 位域 ===== */
/* MATHACL_STATUSCLR Bits */
/* MATHACL_STATUSCLR[CLR_OVF] 溢出标志清除位（写1清除STATUS.OVF） */
/* MATHACL_STATUSCLR[CLR_OVF] Bits */
#define MATHACL_STATUSCLR_CLR_OVF_OFS            (1)                             /* !< CLR_OVF Offset */
#define MATHACL_STATUSCLR_CLR_OVF_MASK           ((uint32_t)0x00000002U)         /* !< Write 1 to this bit to clear
                                                                                    STATUS.OVF bit */
#define MATHACL_STATUSCLR_CLR_OVF_NO_EFFECT      ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define MATHACL_STATUSCLR_CLR_OVF_CLR            ((uint32_t)0x00000002U)         /* !< Clear STATUS.OVF */
/* MATHACL_STATUSCLR[CLR_UF] 下溢标志清除位（写1清除STATUS.UF） */
/* MATHACL_STATUSCLR[CLR_UF] Bits */
#define MATHACL_STATUSCLR_CLR_UF_OFS             (0)                             /* !< CLR_UF Offset */
#define MATHACL_STATUSCLR_CLR_UF_MASK            ((uint32_t)0x00000001U)         /* !< Write 1 to this bit to clear
                                                                                    STATUS.UF bit */
#define MATHACL_STATUSCLR_CLR_UF_NO_EFFECT       ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define MATHACL_STATUSCLR_CLR_UF_CLR             ((uint32_t)0x00000001U)         /* !< Clear STATUS.UF */
/* MATHACL_STATUSCLR[CLR_ERR] 错误标志清除位（写1清除STATUS.ERR） */
/* MATHACL_STATUSCLR[CLR_ERR] Bits */
#define MATHACL_STATUSCLR_CLR_ERR_OFS            (2)                             /* !< CLR_ERR Offset */
#define MATHACL_STATUSCLR_CLR_ERR_MASK           ((uint32_t)0x00000004U)         /* !< Write 1 to this bit to clear
                                                                                    STATUS.ERR field */
#define MATHACL_STATUSCLR_CLR_ERR_NO_EFFECT      ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define MATHACL_STATUSCLR_CLR_ERR_CLR            ((uint32_t)0x00000004U)         /* !< Clear STATUS.ERR */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_mathacl__include */
