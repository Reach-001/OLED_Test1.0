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
 * @file hw_debugss.h
 * @brief MSPM0 DEBUGSS (调试子系统) 外设寄存器定义
 *
 * DEBUGSS 模块提供以下功能:
 *   - CPU中断管理 (CPU_INT): 调试相关中断的索引、掩码、状态、置位/清除
 *   - 调试数据通信 (TXD/RXD): 调试工具与CPU之间的数据收发通道
 *   - 收发控制 (TXCTL/RXCTL): 数据就绪标志和通用标志位
 *   - 特殊授权 (SPECIAL_AUTH): SEC-AP、SWD端口、DFT-AP、ET-AP、CFG-AP、AHB-AP、PWR-AP 的使能控制
 *   - 应用授权 (APP_AUTH): 侵入式/非侵入式调试、安全调试的使能控制
 *
 * @note 调试端口通过SWD (Serial Wire Debug) 协议与外部调试工具通信
 * @note SPECIAL_AUTH 和 APP_AUTH 寄存器控制调试访问权限，与芯片安全特性相关
 */

#ifndef ti_devices_msp_peripherals_m0p_hw_debugss__include
#define ti_devices_msp_peripherals_m0p_hw_debugss__include

/* Filename: hw_debugss.h */
/* Revised: 2023-05-10 21:16:53 */
/* Revision: 2f29710894b537f621c6ebedd563efb234664827 */

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
* DEBUGSS Registers — 调试子系统寄存器结构体定义
* 包含: CPU中断 (CPU_INT) 和顶层DEBUGSS控制/数据/授权寄存器
******************************************************************************/
#define DEBUGSS_CPU_INT_OFS                      ((uint32_t)0x00001020U)


/** @addtogroup DEBUGSS_CPU_INT
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) Interrupt index — 中断索引 */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) Interrupt mask — 中断掩码 */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) Raw interrupt status — 原始中断状态 */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) Masked interrupt status — 屏蔽后中断状态 */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) Interrupt set — 中断置位 */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) Interrupt clear — 中断清除 */
} DEBUGSS_CPU_INT_Regs;

/*@}*/ /* end of group DEBUGSS_CPU_INT */

/** @addtogroup DEBUGSS
  @{
*/

typedef struct {
       uint32_t RESERVED0[1032];
  DEBUGSS_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) 调试CPU中断控制 */
       uint32_t RESERVED1[37];
  __I  uint32_t EVT_MODE;                          /* !< (@ 0x000010E0) Event Mode — 事件模式选择 */
       uint32_t RESERVED2[6];
  __I  uint32_t DESC;                              /* !< (@ 0x000010FC) Module Description — 模块描述 (版本号/模块ID) */
  __I  uint32_t TXD;                               /* !< (@ 0x00001100) Transmit data — 调试发送数据 (外部工具写入) */
  __I  uint32_t TXCTL;                             /* !< (@ 0x00001104) Transmit control — 发送控制 (就绪/满标志) */
  __IO uint32_t RXD;                               /* !< (@ 0x00001108) Receive data — 调试接收数据 (软件写入) */
  __IO uint32_t RXCTL;                             /* !< (@ 0x0000110C) Receive control — 接收控制 (就绪/满标志) */
       uint32_t RESERVED3[60];
  __I  uint32_t SPECIAL_AUTH;                      /* !< (@ 0x00001200) Special authorization — 特殊调试端口授权 */
       uint32_t RESERVED4[3];
  __I  uint32_t APP_AUTH;                          /* !< (@ 0x00001210) Application authorization — 应用调试授权 */
} DEBUGSS_Regs;

/*@}*/ /* end of group DEBUGSS */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* DEBUGSS Register Control Bits — 调试子系统寄存器位域定义
******************************************************************************/

/*—————————————————————————————————————————————————————————————————————————————
 * IIDX (中断索引寄存器)
 * 指示最高优先级待处理中断，读取后自动清除对应RIS标志
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_IIDX[STAT] — 中断索引状态 [7:0] */
#define DEBUGSS_IIDX_STAT_OFS                    (0)                             /* !< STAT Offset */
#define DEBUGSS_IIDX_STAT_MASK                   ((uint32_t)0x000000FFU)         /* !< 中断索引状态，范围 0x00-0x04 */
#define DEBUGSS_IIDX_STAT_NO_INTR                ((uint32_t)0x00000000U)         /* !< 无待处理中断 */
#define DEBUGSS_IIDX_STAT_TXIFG                  ((uint32_t)0x00000001U)         /* !< TX中断: 数据已发送 */
#define DEBUGSS_IIDX_STAT_RXIFG                  ((uint32_t)0x00000002U)         /* !< RX中断: 数据已接收 */
#define DEBUGSS_IIDX_STAT_PWRUP                  ((uint32_t)0x00000003U)         /* !< 上电中断: 调试会话已开始 */
#define DEBUGSS_IIDX_STAT_PWRDWN                 ((uint32_t)0x00000004U)         /* !< 断电中断: 调试会话已结束 */

/*—————————————————————————————————————————————————————————————————————————————
 * IMASK (中断掩码寄存器)
 * 控制各中断源的屏蔽状态，读写
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_IMASK[TXIFG] — TX中断掩码 [0] */
#define DEBUGSS_IMASK_TXIFG_OFS                  (0)                             /* !< TXIFG Offset */
#define DEBUGSS_IMASK_TXIFG_MASK                 ((uint32_t)0x00000001U)         /* !< TX中断掩码 */
#define DEBUGSS_IMASK_TXIFG_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt is masked out */
#define DEBUGSS_IMASK_TXIFG_SET                  ((uint32_t)0x00000001U)         /* !< Interrupt will request an interrupt
                                                                                    service routine and corresponding bit
                                                                                    in MIS will be set */
/* DEBUGSS_IMASK[RXIFG] Bits */
#define DEBUGSS_IMASK_RXIFG_OFS                  (1)                             /* !< RXIFG Offset */
#define DEBUGSS_IMASK_RXIFG_MASK                 ((uint32_t)0x00000002U)         /* !< Masks RXIFG in MIS register */
#define DEBUGSS_IMASK_RXIFG_CLR                  ((uint32_t)0x00000000U)         /* !< Interrupt is masked out */
#define DEBUGSS_IMASK_RXIFG_SET                  ((uint32_t)0x00000002U)         /* !< Interrupt will request an interrupt
                                                                                    service routine and corresponding bit
                                                                                    in MIS will be set */
/* DEBUGSS_IMASK[PWRUPIFG] Bits */
#define DEBUGSS_IMASK_PWRUPIFG_OFS               (2)                             /* !< PWRUPIFG Offset */
#define DEBUGSS_IMASK_PWRUPIFG_MASK              ((uint32_t)0x00000004U)         /* !< Masks PWRUPIFG in MIS register */
#define DEBUGSS_IMASK_PWRUPIFG_CLR               ((uint32_t)0x00000000U)         /* !< Interrupt is masked out */
#define DEBUGSS_IMASK_PWRUPIFG_SET               ((uint32_t)0x00000004U)         /* !< Interrupt will request an interrupt
                                                                                    service routine and corresponding bit
                                                                                    in MIS will be set */
/* DEBUGSS_IMASK[PWRDWNIFG] Bits */
#define DEBUGSS_IMASK_PWRDWNIFG_OFS              (3)                             /* !< PWRDWNIFG Offset */
#define DEBUGSS_IMASK_PWRDWNIFG_MASK             ((uint32_t)0x00000008U)         /* !< Masks PWRDWNIFG in MIS register */
#define DEBUGSS_IMASK_PWRDWNIFG_CLR              ((uint32_t)0x00000000U)         /* !< Interrupt is masked out */
#define DEBUGSS_IMASK_PWRDWNIFG_SET              ((uint32_t)0x00000008U)         /* !< Interrupt will request an interrupt
                                                                                    service routine and corresponding bit
                                                                                    in MIS will be set */

/*—————————————————————————————————————————————————————————————————————————————
 * RIS (原始中断状态寄存器)
 * 指示未屏蔽的中断状态，只读
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_RIS[TXIFG] — TX原始中断状态 [0] */
#define DEBUGSS_RIS_TXIFG_OFS                    (0)                             /* !< TXIFG Offset */
#define DEBUGSS_RIS_TXIFG_MASK                   ((uint32_t)0x00000001U)         /* !< TX原始中断状态 */
#define DEBUGSS_RIS_TXIFG_CLR                    ((uint32_t)0x00000000U)         /* !< TXIFG did not occur */
#define DEBUGSS_RIS_TXIFG_SET                    ((uint32_t)0x00000001U)         /* !< TXIFG occurred */
/* DEBUGSS_RIS[RXIFG] Bits */
#define DEBUGSS_RIS_RXIFG_OFS                    (1)                             /* !< RXIFG Offset */
#define DEBUGSS_RIS_RXIFG_MASK                   ((uint32_t)0x00000002U)         /* !< Raw interrupt status for RXIFG */
#define DEBUGSS_RIS_RXIFG_CLR                    ((uint32_t)0x00000000U)         /* !< RXIFG did not occur */
#define DEBUGSS_RIS_RXIFG_SET                    ((uint32_t)0x00000002U)         /* !< RXIFG occurred */
/* DEBUGSS_RIS[PWRUPIFG] Bits */
#define DEBUGSS_RIS_PWRUPIFG_OFS                 (2)                             /* !< PWRUPIFG Offset */
#define DEBUGSS_RIS_PWRUPIFG_MASK                ((uint32_t)0x00000004U)         /* !< Raw interrupt status for PWRUPIFG */
#define DEBUGSS_RIS_PWRUPIFG_CLR                 ((uint32_t)0x00000000U)         /* !< PWRUPIFG did not occur */
#define DEBUGSS_RIS_PWRUPIFG_SET                 ((uint32_t)0x00000004U)         /* !< PWRUPIFG occurred */
/* DEBUGSS_RIS[PWRDWNIFG] Bits */
#define DEBUGSS_RIS_PWRDWNIFG_OFS                (3)                             /* !< PWRDWNIFG Offset */
#define DEBUGSS_RIS_PWRDWNIFG_MASK               ((uint32_t)0x00000008U)         /* !< Raw interrupt status for PWRDWNIFG */
#define DEBUGSS_RIS_PWRDWNIFG_CLR                ((uint32_t)0x00000000U)         /* !< PWRUPIFG did not occur */
#define DEBUGSS_RIS_PWRDWNIFG_SET                ((uint32_t)0x00000008U)         /* !< PWRUPIFG occurred */

/*—————————————————————————————————————————————————————————————————————————————
 * MIS (屏蔽后中断状态寄存器)
 * RIS & IMASK 结果，只读
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_MIS[TXIFG] — TX屏蔽后中断状态 [0] */
#define DEBUGSS_MIS_TXIFG_OFS                    (0)                             /* !< TXIFG Offset */
#define DEBUGSS_MIS_TXIFG_MASK                   ((uint32_t)0x00000001U)         /* !< TX屏蔽后中断状态 */
#define DEBUGSS_MIS_TXIFG_CLR                    ((uint32_t)0x00000000U)         /* !< TXIFG did not request an interrupt
                                                                                    service routine */
#define DEBUGSS_MIS_TXIFG_SET                    ((uint32_t)0x00000001U)         /* !< TXIFG requests an interrupt service
                                                                                    routine */
/* DEBUGSS_MIS[RXIFG] Bits */
#define DEBUGSS_MIS_RXIFG_OFS                    (1)                             /* !< RXIFG Offset */
#define DEBUGSS_MIS_RXIFG_MASK                   ((uint32_t)0x00000002U)         /* !< Masked interrupt status for RXIFG */
#define DEBUGSS_MIS_RXIFG_CLR                    ((uint32_t)0x00000000U)         /* !< RXIFG did not request an interrupt
                                                                                    service routine */
#define DEBUGSS_MIS_RXIFG_SET                    ((uint32_t)0x00000002U)         /* !< RXIFG requests an interrupt service
                                                                                    routine */
/* DEBUGSS_MIS[PWRUPIFG] Bits */
#define DEBUGSS_MIS_PWRUPIFG_OFS                 (2)                             /* !< PWRUPIFG Offset */
#define DEBUGSS_MIS_PWRUPIFG_MASK                ((uint32_t)0x00000004U)         /* !< Masked interrupt status for
                                                                                    PWRUPIFG */
#define DEBUGSS_MIS_PWRUPIFG_CLR                 ((uint32_t)0x00000000U)         /* !< PWRUPIFG did not request an
                                                                                    interrupt service routine */
#define DEBUGSS_MIS_PWRUPIFG_SET                 ((uint32_t)0x00000004U)         /* !< PWRUPIFG requests an interrupt
                                                                                    service routine */
/* DEBUGSS_MIS[PWRDWNIFG] Bits */
#define DEBUGSS_MIS_PWRDWNIFG_OFS                (3)                             /* !< PWRDWNIFG Offset */
#define DEBUGSS_MIS_PWRDWNIFG_MASK               ((uint32_t)0x00000008U)         /* !< Masked interrupt status for
                                                                                    PWRDWNIFG */
#define DEBUGSS_MIS_PWRDWNIFG_CLR                ((uint32_t)0x00000000U)         /* !< PWRUPIFG did not request an
                                                                                    interrupt service routine */
#define DEBUGSS_MIS_PWRDWNIFG_SET                ((uint32_t)0x00000008U)         /* !< PWRUPIFG requests an interrupt
                                                                                    service routine */

/*—————————————————————————————————————————————————————————————————————————————
 * ISET (中断置位寄存器)
 * 写1置位对应RIS位，只写
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_ISET[TXIFG] — TX中断置位 [0] */
#define DEBUGSS_ISET_TXIFG_OFS                   (0)                             /* !< TXIFG Offset */
#define DEBUGSS_ISET_TXIFG_MASK                  ((uint32_t)0x00000001U)         /* !< 置位TX中断 */
#define DEBUGSS_ISET_TXIFG_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ISET_TXIFG_SET                   ((uint32_t)0x00000001U)         /* !< RIS bit corresponding to TXIFG is
                                                                                    set */
/* DEBUGSS_ISET[RXIFG] Bits */
#define DEBUGSS_ISET_RXIFG_OFS                   (1)                             /* !< RXIFG Offset */
#define DEBUGSS_ISET_RXIFG_MASK                  ((uint32_t)0x00000002U)         /* !< Sets RXIFG in RIS register */
#define DEBUGSS_ISET_RXIFG_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ISET_RXIFG_SET                   ((uint32_t)0x00000002U)         /* !< RIS bit corresponding to RXIFG is
                                                                                    set */
/* DEBUGSS_ISET[PWRUPIFG] Bits */
#define DEBUGSS_ISET_PWRUPIFG_OFS                (2)                             /* !< PWRUPIFG Offset */
#define DEBUGSS_ISET_PWRUPIFG_MASK               ((uint32_t)0x00000004U)         /* !< Sets PWRUPIFG in RIS register */
#define DEBUGSS_ISET_PWRUPIFG_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ISET_PWRUPIFG_SET                ((uint32_t)0x00000004U)         /* !< RIS bit corresponding to PWRUPIFG
                                                                                    is set */
/* DEBUGSS_ISET[PWRDWNIFG] Bits */
#define DEBUGSS_ISET_PWRDWNIFG_OFS               (3)                             /* !< PWRDWNIFG Offset */
#define DEBUGSS_ISET_PWRDWNIFG_MASK              ((uint32_t)0x00000008U)         /* !< Sets PWRDWNIFG in RIS register */
#define DEBUGSS_ISET_PWRDWNIFG_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ISET_PWRDWNIFG_SET               ((uint32_t)0x00000008U)         /* !< RIS bit corresponding to PWRUPIFG
                                                                                    is set */

/*—————————————————————————————————————————————————————————————————————————————
 * ICLR (中断清除寄存器)
 * 写1清除对应RIS位，只写
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_ICLR[TXIFG] — TX中断清除 [0] */
#define DEBUGSS_ICLR_TXIFG_OFS                   (0)                             /* !< TXIFG Offset */
#define DEBUGSS_ICLR_TXIFG_MASK                  ((uint32_t)0x00000001U)         /* !< 清除TX中断 */
#define DEBUGSS_ICLR_TXIFG_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ICLR_TXIFG_CLR                   ((uint32_t)0x00000001U)         /* !< RIS bit corresponding to TXIFG is
                                                                                    cleared */
/* DEBUGSS_ICLR[RXIFG] Bits */
#define DEBUGSS_ICLR_RXIFG_OFS                   (1)                             /* !< RXIFG Offset */
#define DEBUGSS_ICLR_RXIFG_MASK                  ((uint32_t)0x00000002U)         /* !< Clears RXIFG in RIS register */
#define DEBUGSS_ICLR_RXIFG_NO_EFFECT             ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ICLR_RXIFG_CLR                   ((uint32_t)0x00000002U)         /* !< RIS bit corresponding to RXIFG is
                                                                                    cleared */
/* DEBUGSS_ICLR[PWRUPIFG] Bits */
#define DEBUGSS_ICLR_PWRUPIFG_OFS                (2)                             /* !< PWRUPIFG Offset */
#define DEBUGSS_ICLR_PWRUPIFG_MASK               ((uint32_t)0x00000004U)         /* !< Clears PWRUPIFG in RIS register */
#define DEBUGSS_ICLR_PWRUPIFG_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ICLR_PWRUPIFG_CLR                ((uint32_t)0x00000004U)         /* !< RIS bit corresponding to PWRUPIFG
                                                                                    is cleared */
/* DEBUGSS_ICLR[PWRDWNIFG] Bits */
#define DEBUGSS_ICLR_PWRDWNIFG_OFS               (3)                             /* !< PWRDWNIFG Offset */
#define DEBUGSS_ICLR_PWRDWNIFG_MASK              ((uint32_t)0x00000008U)         /* !< Clears PWRDWNIFG in RIS register */
#define DEBUGSS_ICLR_PWRDWNIFG_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing a 0 has no effect */
#define DEBUGSS_ICLR_PWRDWNIFG_CLR               ((uint32_t)0x00000008U)         /* !< RIS bit corresponding to PWRUPIFG
                                                                                    is cleared */

/*—————————————————————————————————————————————————————————————————————————————
 * EVT_MODE (事件模式寄存器)
 * 选择调试中断/事件线的处理模式
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_EVT_MODE[INT0_CFG] — 事件线模式选择 [1:0] */
#define DEBUGSS_EVT_MODE_INT0_CFG_OFS            (0)                             /* !< INT0_CFG Offset */
#define DEBUGSS_EVT_MODE_INT0_CFG_MASK           ((uint32_t)0x00000003U)         /* !< 事件线模式选择，范围 0-2 */
#define DEBUGSS_EVT_MODE_INT0_CFG_DISABLE        ((uint32_t)0x00000000U)         /* !< 中断/事件线禁用 */
#define DEBUGSS_EVT_MODE_INT0_CFG_SOFTWARE       ((uint32_t)0x00000001U)         /* !< 软件模式: 软件清除RIS标志 */
#define DEBUGSS_EVT_MODE_INT0_CFG_HARDWARE       ((uint32_t)0x00000002U)         /* !< 硬件模式: 硬件自动清除RIS标志 */

/*—————————————————————————————————————————————————————————————————————————————
 * DESC (模块描述寄存器，只读)
 * 包含IP版本号、实例编号、功能集版本、模块ID
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_DESC[MINREV] — IP次版本号 [3:0] */
#define DEBUGSS_DESC_MINREV_OFS                  (0)                             /* !< MINREV Offset */
#define DEBUGSS_DESC_MINREV_MASK                 ((uint32_t)0x0000000FU)         /* !< IP次版本号，范围 0-15 */
/* DEBUGSS_DESC[MAJREV] — IP主版本号 [7:4] */
#define DEBUGSS_DESC_MAJREV_OFS                  (4)                             /* !< MAJREV Offset */
#define DEBUGSS_DESC_MAJREV_MASK                 ((uint32_t)0x000000F0U)         /* !< IP主版本号，范围 0-15 */
/* DEBUGSS_DESC[INSTNUM] — 实例编号 [11:8] */
#define DEBUGSS_DESC_INSTNUM_OFS                 (8)                             /* !< INSTNUM Offset */
#define DEBUGSS_DESC_INSTNUM_MASK                ((uint32_t)0x00000F00U)         /* !< 设备内模块实例编号，范围 0-15 */
/* DEBUGSS_DESC[FEATUREVER] — 功能集版本 [15:12] */
#define DEBUGSS_DESC_FEATUREVER_OFS              (12)                            /* !< FEATUREVER Offset */
#define DEBUGSS_DESC_FEATUREVER_MASK             ((uint32_t)0x0000F000U)         /* !< 模块实例的功能集版本号 */
/* DEBUGSS_DESC[MODULEID] — 模块标识 [31:16] */
#define DEBUGSS_DESC_MODULEID_OFS                (16)                            /* !< MODULEID Offset */
#define DEBUGSS_DESC_MODULEID_MASK               ((uint32_t)0xFFFF0000U)         /* !< 模块唯一标识号，由中心数据库统一分配 */

/*—————————————————————————————————————————————————————————————————————————————
 * TXD (调试发送数据寄存器，只读)
 * 外部调试工具写入的数据，软件通过读取此寄存器获取
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_TXD[TX_DATA] — 发送数据 [31:0] */
#define DEBUGSS_TXD_TX_DATA_OFS                  (0)                             /* !< TX_DATA Offset */
#define DEBUGSS_TXD_TX_DATA_MASK                 ((uint32_t)0xFFFFFFFFU)         /* !< 外部调试工具写入的32位数据 */

/*—————————————————————————————————————————————————————————————————————————————
 * TXCTL (调试发送控制寄存器)
 * 指示TXD数据就绪状态，软件读取TXD后自动清除
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_TXCTL[TRANSMIT] — 发送状态 [0] */
#define DEBUGSS_TXCTL_TRANSMIT_OFS               (0)                             /* !< TRANSMIT Offset */
#define DEBUGSS_TXCTL_TRANSMIT_MASK              ((uint32_t)0x00000001U)         /* !< TXD数据就绪标志，软件读TXD后自动清除 */
#define DEBUGSS_TXCTL_TRANSMIT_EMPTY             ((uint32_t)0x00000000U)         /* !< TXD为空 (无数据) */
#define DEBUGSS_TXCTL_TRANSMIT_FULL              ((uint32_t)0x00000001U)         /* !< TXD已满 (有数据待读) */
/* DEBUGSS_TXCTL[TRANSMIT_FLAGS] — 通用发送标志 [31:1] */
#define DEBUGSS_TXCTL_TRANSMIT_FLAGS_OFS         (1)                             /* !< TRANSMIT_FLAGS Offset */
#define DEBUGSS_TXCTL_TRANSMIT_FLAGS_MASK        ((uint32_t)0xFFFFFFFEU)         /* !< 通用TX标志，由外部调试工具设置，功能由软件定义 */

/*—————————————————————————————————————————————————————————————————————————————
 * RXD (调试接收数据寄存器，读写)
 * 软件写入数据，外部调试工具通过SWD读取
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_RXD[RX_DATA] — 接收数据 [31:0] */
#define DEBUGSS_RXD_RX_DATA_OFS                  (0)                             /* !< RX_DATA Offset */
#define DEBUGSS_RXD_RX_DATA_MASK                 ((uint32_t)0xFFFFFFFFU)         /* !< 软件写入的32位数据 */

/*—————————————————————————————————————————————————————————————————————————————
 * RXCTL (调试接收控制寄存器)
 * 指示RXD数据就绪状态，SWD AP读取RXD后自动清除
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_RXCTL[RECEIVE] — 接收状态 [0] */
#define DEBUGSS_RXCTL_RECEIVE_OFS                (0)                             /* !< RECEIVE Offset */
#define DEBUGSS_RXCTL_RECEIVE_MASK               ((uint32_t)0x00000001U)         /* !< RXD数据就绪标志，SWD AP读取后自动清除 */
#define DEBUGSS_RXCTL_RECEIVE_EMPTY              ((uint32_t)0x00000000U)         /* !< RXD为空 (无数据) */
#define DEBUGSS_RXCTL_RECEIVE_FULL               ((uint32_t)0x00000001U)         /* !< RXD已满 (有数据待读) */
/* DEBUGSS_RXCTL[RECEIVE_FLAGS] — 通用接收标志 [7:1] */
#define DEBUGSS_RXCTL_RECEIVE_FLAGS_OFS          (1)                             /* !< RECEIVE_FLAGS Offset */
#define DEBUGSS_RXCTL_RECEIVE_FLAGS_MASK         ((uint32_t)0x000000FEU)         /* !< 通用RX标志，由软件设置，外部调试工具读取 */

/*—————————————————————————————————————————————————————————————————————————————
 * SPECIAL_AUTH (特殊调试端口授权寄存器，只读)
 * 控制各调试访问端口 (AP) 的使能状态
 * 禁用时DAPBUS防火墙将隔离对应AP
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_SPECIAL_AUTH[SECAPEN] — 安全AP使能 [0] */
#define DEBUGSS_SPECIAL_AUTH_SECAPEN_OFS         (0)                             /* !< SECAPEN Offset */
#define DEBUGSS_SPECIAL_AUTH_SECAPEN_MASK        ((uint32_t)0x00000001U)         /* !< SEC-AP使能，需同时允许SWD访问 */
#define DEBUGSS_SPECIAL_AUTH_SECAPEN_DISABLE     ((uint32_t)0x00000000U)         /* !< 禁用SEC-AP */
#define DEBUGSS_SPECIAL_AUTH_SECAPEN_ENABLE      ((uint32_t)0x00000001U)         /* !< 启用SEC-AP */
/* DEBUGSS_SPECIAL_AUTH[SWDPORTEN] — SWD端口使能 [1] */
#define DEBUGSS_SPECIAL_AUTH_SWDPORTEN_OFS       (1)                             /* !< SWDPORTEN Offset */
#define DEBUGSS_SPECIAL_AUTH_SWDPORTEN_MASK      ((uint32_t)0x00000002U)         /* !< SWD端口使能，禁用后禁止所有外部调试访问 */
#define DEBUGSS_SPECIAL_AUTH_SWDPORTEN_DISABLE   ((uint32_t)0x00000000U)         /* !< 禁用SWD端口 */
#define DEBUGSS_SPECIAL_AUTH_SWDPORTEN_ENABLE    ((uint32_t)0x00000002U)         /* !< 启用SWD端口 */
/* DEBUGSS_SPECIAL_AUTH[DFTAPEN] — DFT-AP使能 [2] */
#define DEBUGSS_SPECIAL_AUTH_DFTAPEN_OFS         (2)                             /* !< DFTAPEN Offset */
#define DEBUGSS_SPECIAL_AUTH_DFTAPEN_MASK        ((uint32_t)0x00000004U)         /* !< DFT-AP使能 (可测试性设计访问端口) */
#define DEBUGSS_SPECIAL_AUTH_DFTAPEN_DISABLE     ((uint32_t)0x00000000U)         /* !< 禁用DFT-AP */
#define DEBUGSS_SPECIAL_AUTH_DFTAPEN_ENABLE      ((uint32_t)0x00000004U)         /* !< 启用DFT-AP */
/* DEBUGSS_SPECIAL_AUTH[ETAPEN] — ET-AP使能 [3] */
#define DEBUGSS_SPECIAL_AUTH_ETAPEN_OFS          (3)                             /* !< ETAPEN Offset */
#define DEBUGSS_SPECIAL_AUTH_ETAPEN_MASK         ((uint32_t)0x00000008U)         /* !< ET-AP使能 (嵌入式追踪访问端口) */
#define DEBUGSS_SPECIAL_AUTH_ETAPEN_DISABLE      ((uint32_t)0x00000000U)         /* !< 禁用ET-AP */
#define DEBUGSS_SPECIAL_AUTH_ETAPEN_ENABLE       ((uint32_t)0x00000008U)         /* !< 启用ET-AP */
/* DEBUGSS_SPECIAL_AUTH[CFGAPEN] — CFG-AP使能 [4] */
#define DEBUGSS_SPECIAL_AUTH_CFGAPEN_OFS         (4)                             /* !< CFGAPEN Offset */
#define DEBUGSS_SPECIAL_AUTH_CFGAPEN_MASK        ((uint32_t)0x00000010U)         /* !< CFG-AP使能 (配置访问端口，读取设备配置) */
#define DEBUGSS_SPECIAL_AUTH_CFGAPEN_DISABLE     ((uint32_t)0x00000000U)         /* !< 禁用CFG-AP */
#define DEBUGSS_SPECIAL_AUTH_CFGAPEN_ENABLE      ((uint32_t)0x00000010U)         /* !< 启用CFG-AP */
/* DEBUGSS_SPECIAL_AUTH[AHBAPEN] — AHB-AP使能 [5] */
#define DEBUGSS_SPECIAL_AUTH_AHBAPEN_OFS         (5)                             /* !< AHBAPEN Offset */
#define DEBUGSS_SPECIAL_AUTH_AHBAPEN_MASK        ((uint32_t)0x00000020U)         /* !< AHB-AP使能 (通过DAP总线隔离控制M0+ Core调试访问) */
#define DEBUGSS_SPECIAL_AUTH_AHBAPEN_DISABLE     ((uint32_t)0x00000000U)         /* !< 禁用AHB-AP */
#define DEBUGSS_SPECIAL_AUTH_AHBAPEN_ENABLE      ((uint32_t)0x00000020U)         /* !< 启用AHB-AP */
/* DEBUGSS_SPECIAL_AUTH[PWRAPEN] — PWR-AP使能 [6] */
#define DEBUGSS_SPECIAL_AUTH_PWRAPEN_OFS         (6)                             /* !< PWRAPEN Offset */
#define DEBUGSS_SPECIAL_AUTH_PWRAPEN_MASK        ((uint32_t)0x00000040U)         /* !< PWR-AP使能 (电源/复位状态访问端口) */
#define DEBUGSS_SPECIAL_AUTH_PWRAPEN_DISABLE     ((uint32_t)0x00000000U)         /* !< 禁用PWR-AP */
#define DEBUGSS_SPECIAL_AUTH_PWRAPEN_ENABLE      ((uint32_t)0x00000040U)         /* !< 启用PWR-AP */

/*—————————————————————————————————————————————————————————————————————————————
 * APP_AUTH (应用调试授权寄存器，只读)
 * 控制CPU0的调试权限: 侵入式/非侵入式、安全/非安全调试
 *—————————————————————————————————————————————————————————————————————————————*/
/* DEBUGSS_APP_AUTH[DBGEN] — 侵入式调试使能 [0] */
#define DEBUGSS_APP_AUTH_DBGEN_OFS               (0)                             /* !< DBGEN Offset */
#define DEBUGSS_APP_AUTH_DBGEN_MASK              ((uint32_t)0x00000001U)         /* !< 侵入式调试使能 (断点/单步等) */
#define DEBUGSS_APP_AUTH_DBGEN_DISABLE           ((uint32_t)0x00000000U)         /* !< 禁用侵入式调试 */
#define DEBUGSS_APP_AUTH_DBGEN_ENABLE            ((uint32_t)0x00000001U)         /* !< 启用侵入式调试 */
/* DEBUGSS_APP_AUTH[NIDEN] — 非侵入式调试使能 [1] */
#define DEBUGSS_APP_AUTH_NIDEN_OFS               (1)                             /* !< NIDEN Offset */
#define DEBUGSS_APP_AUTH_NIDEN_MASK              ((uint32_t)0x00000002U)         /* !< 非侵入式调试使能 (ETM跟踪等) */
#define DEBUGSS_APP_AUTH_NIDEN_DISABLE           ((uint32_t)0x00000000U)         /* !< 禁用非侵入式调试 */
#define DEBUGSS_APP_AUTH_NIDEN_ENABLE            ((uint32_t)0x00000002U)         /* !< 启用非侵入式调试 */
/* DEBUGSS_APP_AUTH[SPIDEN] — 安全侵入式调试使能 [2] */
#define DEBUGSS_APP_AUTH_SPIDEN_OFS              (2)                             /* !< SPIDEN Offset */
#define DEBUGSS_APP_AUTH_SPIDEN_MASK             ((uint32_t)0x00000004U)         /* !< 安全侵入式调试使能 */
#define DEBUGSS_APP_AUTH_SPIDEN_DISABLE          ((uint32_t)0x00000000U)         /* !< 禁用安全侵入式调试 */
#define DEBUGSS_APP_AUTH_SPIDEN_ENABLE           ((uint32_t)0x00000004U)         /* !< 启用安全侵入式调试 */
/* DEBUGSS_APP_AUTH[SPNIDEN] — 安全非侵入式调试使能 [3] */
#define DEBUGSS_APP_AUTH_SPNIDEN_OFS             (3)                             /* !< SPNIDEN Offset */
#define DEBUGSS_APP_AUTH_SPNIDEN_MASK            ((uint32_t)0x00000008U)         /* !< 安全非侵入式调试使能 */
#define DEBUGSS_APP_AUTH_SPNIDEN_DISABLE         ((uint32_t)0x00000000U)         /* !< 禁用安全非侵入式调试 */
#define DEBUGSS_APP_AUTH_SPNIDEN_ENABLE          ((uint32_t)0x00000008U)         /* !< 启用安全非侵入式调试 */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_m0p_hw_debugss__include */
