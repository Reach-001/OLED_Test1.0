/*****************************************************************************

  Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/

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
 * @file hw_cpuss.h
 * @brief MSPM0 CPUSS (CPU子系统) 外设寄存器定义
 *
 * CPUSS 模块提供以下功能:
 *   - 中断分组管理 (INT_GROUP): 中断索引、掩码、原始/屏蔽状态、置位/清除
 *   - 事件模式配置 (EVT_MODE): 中断/事件线模式选择 (禁用/软件/硬件)
 *   - 模块描述信息 (DESC): IP版本号、功能集、模块ID
 *   - 指令预取与缓存控制 (CTL): 指令预取(PREFETCH)、指令缓存(ICACHE)、字面量缓存(LITEN)
 *
 * @note 所有寄存器均为只读或只写，不支持读写操作
 * @note 中断分组支持8个中断源 (INT0-INT7)
 */

#ifndef ti_devices_msp_peripherals_m0p_hw_cpuss__include
#define ti_devices_msp_peripherals_m0p_hw_cpuss__include

/* Filename: hw_cpuss.h */
/* Revised: 2023-04-30 21:36:20 */
/* Revision: ec4f3de051dafa6135b4867f4e796eec5f1a339b */

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
* CPUSS Registers — CPU子系统寄存器结构体定义
* 包含: 中断分组 (INT_GROUP) 和顶层CPUSS控制/描述寄存器
******************************************************************************/
#define CPUSS_INT_GROUP_OFS                      ((uint32_t)0x00001100U)


/** @addtogroup CPUSS_INT_GROUP
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001100) Interrupt index — 中断索引，指示最高优先级待处理中断 */
       uint32_t RESERVED0;
  __I  uint32_t IMASK;                             /* !< (@ 0x00001108) Interrupt mask — 中断掩码状态 */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001110) Raw interrupt status — 原始中断状态 (未屏蔽) */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001118) Masked interrupt status — 屏蔽后中断状态 */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001120) Interrupt set — 软件置位中断 */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001128) Interrupt clear — 软件清除中断 */
       uint32_t RESERVED5;
} CPUSS_INT_GROUP_Regs;

/*@}*/ /* end of group CPUSS_INT_GROUP */

/** @addtogroup CPUSS
  @{
*/

typedef struct {
       uint32_t RESERVED0[1080];
  __I  uint32_t EVT_MODE;                          /* !< (@ 0x000010E0) Event Mode — 事件模式选择 */
       uint32_t RESERVED1[6];
  __I  uint32_t DESC;                              /* !< (@ 0x000010FC) Module Description — 模块描述 (版本号/模块ID) */
  CPUSS_INT_GROUP_Regs  INT_GROUP[5];                      /* !< (@ 0x00001100) 中断分组 (共5组) */
       uint32_t RESERVED2[68];
  __IO uint32_t CTL;                               /* !< (@ 0x00001300) Prefetch/Cache control — 指令预取与缓存控制 */
} CPUSS_Regs;

/*@}*/ /* end of group CPUSS */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* CPUSS Register Control Bits — CPU子系统寄存器位域定义
******************************************************************************/

/*—————————————————————————————————————————————————————————————————————————————
 * 中断分组 — IIDX (中断索引寄存器)
 * 读取此寄存器可获取最高优先级待处理中断编号，读取后自动清除对应RIS标志
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_INT_GROUP_IIDX[STAT] — 中断索引状态位 [7:0] */
#define CPUSS_INT_GROUP_IIDX_STAT_OFS            (0)                             /* !< STAT Offset */
#define CPUSS_INT_GROUP_IIDX_STAT_MASK           ((uint32_t)0x000000FFU)         /* !< 中断索引状态掩码，范围 0x00-0x08 */
#define CPUSS_INT_GROUP_IIDX_STAT_NO_INTR        ((uint32_t)0x00000000U)         /* !< 无待处理中断 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT0           ((uint32_t)0x00000001U)         /* !< 中断0 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT1           ((uint32_t)0x00000002U)         /* !< 中断1 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT2           ((uint32_t)0x00000003U)         /* !< 中断2 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT3           ((uint32_t)0x00000004U)         /* !< 中断3 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT4           ((uint32_t)0x00000005U)         /* !< 中断4 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT5           ((uint32_t)0x00000006U)         /* !< 中断5 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT6           ((uint32_t)0x00000007U)         /* !< 中断6 */
#define CPUSS_INT_GROUP_IIDX_STAT_INT7           ((uint32_t)0x00000008U)         /* !< 中断7 */

/*—————————————————————————————————————————————————————————————————————————————
 * 中断分组 — IMASK (中断掩码寄存器)
 * 每位对应一个中断源的掩码状态 (只读)
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_INT_GROUP_IMASK[INT] — 中断掩码位 [7:0]，每位对应一个中断源 */
#define CPUSS_INT_GROUP_IMASK_INT_OFS            (0)                             /* !< INT Offset */
#define CPUSS_INT_GROUP_IMASK_INT_MASK           ((uint32_t)0x000000FFU)         /* !< 对应中断的掩码状态 */
#define CPUSS_INT_GROUP_IMASK_INT_INT0           ((uint32_t)0x00000001U)         /* !< Interrupt 0 */
#define CPUSS_INT_GROUP_IMASK_INT_INT1           ((uint32_t)0x00000002U)         /* !< Interrupt 1 */
#define CPUSS_INT_GROUP_IMASK_INT_INT2           ((uint32_t)0x00000004U)         /* !< Interrupt 2 */
#define CPUSS_INT_GROUP_IMASK_INT_INT3           ((uint32_t)0x00000008U)         /* !< Interrupt 3 */
#define CPUSS_INT_GROUP_IMASK_INT_INT4           ((uint32_t)0x00000010U)         /* !< Interrupt 4 */
#define CPUSS_INT_GROUP_IMASK_INT_INT5           ((uint32_t)0x00000020U)         /* !< Interrupt 5 */
#define CPUSS_INT_GROUP_IMASK_INT_INT6           ((uint32_t)0x00000040U)         /* !< Interrupt 6 */
#define CPUSS_INT_GROUP_IMASK_INT_INT7           ((uint32_t)0x00000080U)         /* !< Interrupt 7 */

/*—————————————————————————————————————————————————————————————————————————————
 * 中断分组 — RIS (原始中断状态寄存器)
 * 指示未被屏蔽的中断状态 (只读)
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_INT_GROUP_RIS[INT] — 原始中断状态位 [7:0] */
#define CPUSS_INT_GROUP_RIS_INT_OFS              (0)                             /* !< INT Offset */
#define CPUSS_INT_GROUP_RIS_INT_MASK             ((uint32_t)0x000000FFU)         /* !< 原始中断状态 */
#define CPUSS_INT_GROUP_RIS_INT_INT0             ((uint32_t)0x00000001U)         /* !< Interrupt 0 */
#define CPUSS_INT_GROUP_RIS_INT_INT1             ((uint32_t)0x00000002U)         /* !< Interrupt 1 */
#define CPUSS_INT_GROUP_RIS_INT_INT2             ((uint32_t)0x00000004U)         /* !< Interrupt 2 */
#define CPUSS_INT_GROUP_RIS_INT_INT3             ((uint32_t)0x00000008U)         /* !< Interrupt 3 */
#define CPUSS_INT_GROUP_RIS_INT_INT4             ((uint32_t)0x00000010U)         /* !< Interrupt 4 */
#define CPUSS_INT_GROUP_RIS_INT_INT5             ((uint32_t)0x00000020U)         /* !< Interrupt 5 */
#define CPUSS_INT_GROUP_RIS_INT_INT6             ((uint32_t)0x00000040U)         /* !< Interrupt 6 */
#define CPUSS_INT_GROUP_RIS_INT_INT7             ((uint32_t)0x00000080U)         /* !< Interrupt 7 */

/*—————————————————————————————————————————————————————————————————————————————
 * 中断分组 — MIS (屏蔽后中断状态寄存器)
 * RIS & IMASK 的结果 (只读)
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_INT_GROUP_MIS[INT] — 屏蔽后中断状态位 [7:0] */
#define CPUSS_INT_GROUP_MIS_INT_OFS              (0)                             /* !< INT Offset */
#define CPUSS_INT_GROUP_MIS_INT_MASK             ((uint32_t)0x000000FFU)         /* !< 屏蔽后中断状态 */
#define CPUSS_INT_GROUP_MIS_INT_INT0             ((uint32_t)0x00000001U)         /* !< Interrupt 0 */
#define CPUSS_INT_GROUP_MIS_INT_INT1             ((uint32_t)0x00000002U)         /* !< Interrupt 1 */
#define CPUSS_INT_GROUP_MIS_INT_INT2             ((uint32_t)0x00000004U)         /* !< Interrupt 2 */
#define CPUSS_INT_GROUP_MIS_INT_INT3             ((uint32_t)0x00000008U)         /* !< Interrupt 3 */
#define CPUSS_INT_GROUP_MIS_INT_INT4             ((uint32_t)0x00000010U)         /* !< Interrupt 4 */
#define CPUSS_INT_GROUP_MIS_INT_INT5             ((uint32_t)0x00000020U)         /* !< Interrupt 5 */
#define CPUSS_INT_GROUP_MIS_INT_INT6             ((uint32_t)0x00000040U)         /* !< Interrupt 6 */
#define CPUSS_INT_GROUP_MIS_INT_INT7             ((uint32_t)0x00000080U)         /* !< Interrupt 7 */

/*—————————————————————————————————————————————————————————————————————————————
 * 中断分组 — ISET (中断置位寄存器)
 * 写1置位对应RIS位 (只写)
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_INT_GROUP_ISET[INT] — 中断置位位 [7:0] */
#define CPUSS_INT_GROUP_ISET_INT_OFS             (0)                             /* !< INT Offset */
#define CPUSS_INT_GROUP_ISET_INT_MASK            ((uint32_t)0x000000FFU)         /* !< 置位RIS中对应中断 */
#define CPUSS_INT_GROUP_ISET_INT_INT0            ((uint32_t)0x00000001U)         /* !< Interrupt 0 */
#define CPUSS_INT_GROUP_ISET_INT_INT1            ((uint32_t)0x00000002U)         /* !< Interrupt 1 */
#define CPUSS_INT_GROUP_ISET_INT_INT2            ((uint32_t)0x00000004U)         /* !< Interrupt 2 */
#define CPUSS_INT_GROUP_ISET_INT_INT3            ((uint32_t)0x00000008U)         /* !< Interrupt 3 */
#define CPUSS_INT_GROUP_ISET_INT_INT4            ((uint32_t)0x00000010U)         /* !< Interrupt 4 */
#define CPUSS_INT_GROUP_ISET_INT_INT5            ((uint32_t)0x00000020U)         /* !< Interrupt 5 */
#define CPUSS_INT_GROUP_ISET_INT_INT6            ((uint32_t)0x00000040U)         /* !< Interrupt 6 */
#define CPUSS_INT_GROUP_ISET_INT_INT7            ((uint32_t)0x00000080U)         /* !< Interrupt 7 */

/*—————————————————————————————————————————————————————————————————————————————
 * 中断分组 — ICLR (中断清除寄存器)
 * 写1清除对应RIS位 (只写)
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_INT_GROUP_ICLR[INT] — 中断清除位 [7:0] */
#define CPUSS_INT_GROUP_ICLR_INT_OFS             (0)                             /* !< INT Offset */
#define CPUSS_INT_GROUP_ICLR_INT_MASK            ((uint32_t)0x000000FFU)         /* !< 清除RIS中对应中断 */
#define CPUSS_INT_GROUP_ICLR_INT_INT0            ((uint32_t)0x00000001U)         /* !< Interrupt 0 */
#define CPUSS_INT_GROUP_ICLR_INT_INT1            ((uint32_t)0x00000002U)         /* !< Interrupt 1 */
#define CPUSS_INT_GROUP_ICLR_INT_INT2            ((uint32_t)0x00000004U)         /* !< Interrupt 2 */
#define CPUSS_INT_GROUP_ICLR_INT_INT3            ((uint32_t)0x00000008U)         /* !< Interrupt 3 */
#define CPUSS_INT_GROUP_ICLR_INT_INT4            ((uint32_t)0x00000010U)         /* !< Interrupt 4 */
#define CPUSS_INT_GROUP_ICLR_INT_INT5            ((uint32_t)0x00000020U)         /* !< Interrupt 5 */
#define CPUSS_INT_GROUP_ICLR_INT_INT6            ((uint32_t)0x00000040U)         /* !< Interrupt 6 */
#define CPUSS_INT_GROUP_ICLR_INT_INT7            ((uint32_t)0x00000080U)         /* !< Interrupt 7 */

/*—————————————————————————————————————————————————————————————————————————————
 * EVT_MODE (事件模式寄存器)
 * 选择中断/事件线的处理模式
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_EVT_MODE[INT_CFG] — 事件线模式选择 [1:0] */
#define CPUSS_EVT_MODE_INT_CFG_OFS               (0)                             /* !< INT_CFG Offset */
#define CPUSS_EVT_MODE_INT_CFG_MASK              ((uint32_t)0x00000003U)         /* !< 事件线模式选择，范围 0-2 */
#define CPUSS_EVT_MODE_INT_CFG_DISABLE           ((uint32_t)0x00000000U)         /* !< 中断/事件线禁用 */
#define CPUSS_EVT_MODE_INT_CFG_SOFTWARE          ((uint32_t)0x00000001U)         /* !< 软件模式: 由软件清除RIS标志 */
#define CPUSS_EVT_MODE_INT_CFG_HARDWARE          ((uint32_t)0x00000002U)         /* !< 硬件模式: 硬件自动清除RIS标志 */

/*—————————————————————————————————————————————————————————————————————————————
 * DESC (模块描述寄存器，只读)
 * 包含IP版本号、功能集版本、模块ID等信息
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_DESC[MINREV] — IP次版本号 [3:0] */
#define CPUSS_DESC_MINREV_OFS                    (0)                             /* !< MINREV Offset */
#define CPUSS_DESC_MINREV_MASK                   ((uint32_t)0x0000000FU)         /* !< IP次版本号，范围 0-15 */
/* CPUSS_DESC[MAJREV] — IP主版本号 [7:4] */
#define CPUSS_DESC_MAJREV_OFS                    (4)                             /* !< MAJREV Offset */
#define CPUSS_DESC_MAJREV_MASK                   ((uint32_t)0x000000F0U)         /* !< IP主版本号，范围 0-15 */
/* CPUSS_DESC[FEATUREVER] — 功能集版本 [15:12] */
#define CPUSS_DESC_FEATUREVER_OFS                (12)                            /* !< FEATUREVER Offset */
#define CPUSS_DESC_FEATUREVER_MASK               ((uint32_t)0x0000F000U)         /* !< 模块实例的功能集版本号 */
/* CPUSS_DESC[MODULEID] — 模块标识 [31:16] */
#define CPUSS_DESC_MODULEID_OFS                  (16)                            /* !< MODULEID Offset */
#define CPUSS_DESC_MODULEID_MASK                 ((uint32_t)0xFFFF0000U)         /* !< 模块唯一标识号，由中心数据库统一分配 */

/*—————————————————————————————————————————————————————————————————————————————
 * CTL (预取/缓存控制寄存器，读写)
 * 控制Flash指令预取和缓存行为，优化代码执行速度
 *—————————————————————————————————————————————————————————————————————————————*/
/* CPUSS_CTL[PREFETCH] — Flash指令预取使能 [0] */
#define CPUSS_CTL_PREFETCH_OFS                   (0)                             /* !< PREFETCH Offset */
#define CPUSS_CTL_PREFETCH_MASK                  ((uint32_t)0x00000001U)         /* !< Flash指令预取使能/禁用 */
#define CPUSS_CTL_PREFETCH_DISABLE               ((uint32_t)0x00000000U)         /* !< 禁用指令预取 */
#define CPUSS_CTL_PREFETCH_ENABLE                ((uint32_t)0x00000001U)         /* !< 启用指令预取 */
/* CPUSS_CTL[ICACHE] — 指令缓存使能 [1] */
#define CPUSS_CTL_ICACHE_OFS                     (1)                             /* !< ICACHE Offset */
#define CPUSS_CTL_ICACHE_MASK                    ((uint32_t)0x00000002U)         /* !< Flash指令缓存使能/禁用 */
#define CPUSS_CTL_ICACHE_DISABLE                 ((uint32_t)0x00000000U)         /* !< 禁用指令缓存 */
#define CPUSS_CTL_ICACHE_ENABLE                  ((uint32_t)0x00000002U)         /* !< 启用指令缓存 */
/* CPUSS_CTL[LITEN] — 字面量缓存/预取使能 [2]
 * 仅在 ICACHE 或 PREFETCH 启用时生效，控制字面量的缓存和预取行为 */
#define CPUSS_CTL_LITEN_OFS                      (2)                             /* !< LITEN Offset */
#define CPUSS_CTL_LITEN_MASK                     ((uint32_t)0x00000004U)         /* !< 字面量缓存和预取使能 */
#define CPUSS_CTL_LITEN_DISABLE                  ((uint32_t)0x00000000U)         /* !< 禁用字面量缓存 */
#define CPUSS_CTL_LITEN_ENABLE                   ((uint32_t)0x00000004U)         /* !< 启用字面量缓存 */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_m0p_hw_cpuss__include */
