/*****************************************************************************

  Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/ 

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
 * @file hw_npu.h
 * @brief NPU (神经网络处理单元) 硬件寄存器定义
 *
 * 本文件定义了 MSPM0G3507 微控制器中 NPU 模块的全部寄存器和位域。
 * NPU 提供硬件加速的神经网络推理能力，主要特性包括：
 * - 支持 AI/ML 模型推理加速
 * - 包含多个数据寄存器(DREG)用于配置和数据传输
 * - 支持中断通知（完成中断）
 * - 集成电源管理和复位控制
 */

#ifndef ti_devices_msp_peripherals_hw_npu__include
#define ti_devices_msp_peripherals_hw_npu__include

/* Filename: hw_npu.h */
/* Revised: 2025-06-16 15:14:44 */
/* Revision: 5443928412b4f047f17723d5fe92cb118ff208b6 */

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
* NPU Registers - NPU 寄存器定义
******************************************************************************/

/* 各寄存器组基地址偏移 */
#define NPU_INT_EVENT_OFS                        ((uint32_t)0x00001020U)  /* 中断事件寄存器偏移 */
#define NPU_GPRCM_OFS                            ((uint32_t)0x00000800U)  /* GPRCM寄存器偏移 */

/** @addtogroup NPU_INT_EVENT
  @{
*/

/* 中断事件寄存器组 */
typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) 中断索引寄存器 */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) 中断屏蔽寄存器 */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) 原始中断状态 */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) 屏蔽后中断状态 */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) 中断置位寄存器 */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) 中断清除寄存器 */
} NPU_INT_EVENT_Regs;

/*@}*/ /* end of group NPU_INT_EVENT */

/** @addtogroup NPU_GPRCM
  @{
*/

/* GPRCM (通用电源/复位控制模块) 寄存器组 */
typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) 电源使能寄存器 */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) 复位控制寄存器 */
       uint32_t RESERVED0[3];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) 状态寄存器 */
} NPU_GPRCM_Regs;

/*@}*/ /* end of group NPU_GPRCM */

/** @addtogroup NPU
  @{
*/

/* NPU 主寄存器组 - 包含数据寄存器和控制寄存器 */
typedef struct {
       uint32_t RESERVED0[512];
  NPU_GPRCM_Regs  GPRCM;                                /* !< (@ 0x00000800) GPRCM寄存器组 */
       uint32_t RESERVED1[514];
  NPU_INT_EVENT_Regs  CPU_INT;                          /* !< (@ 0x00001020) CPU中断事件寄存器 */
       uint32_t RESERVED2[37];
  __I  uint32_t EVT_MODE;                               /* !< (@ 0x000010E0) 事件模式 */
       uint32_t RESERVED3[6];
  __I  uint32_t DESC;                                   /* !< (@ 0x000010FC) 模块描述寄存器 */
       uint32_t RESERVED4[8];
  __IO uint32_t DREG0;                                  /* !< (@ 0x00001120) 数据寄存器0 */
  __IO uint32_t DREG1;                                  /* !< (@ 0x00001124) 数据寄存器1 */
  __IO uint32_t DREG2;                                  /* !< (@ 0x00001128) 数据寄存器2 */
  __IO uint32_t DREG3;                                  /* !< (@ 0x0000112C) 数据寄存器3 */
  __IO uint32_t DREG4;                                  /* !< (@ 0x00001130) 数据寄存器4 */
  __IO uint32_t DREG5;                                  /* !< (@ 0x00001134) 数据寄存器5 */
  __IO uint32_t DREG6[6];                               /* !< (@ 0x00001138) 数据寄存器6（数组） */
  __IO uint32_t DREG7;                                  /* !< (@ 0x00001150) 数据寄存器7 */
  __IO uint32_t DREG8[20];                              /* !< (@ 0x00001154) 数据寄存器8（数组） */
       uint32_t RESERVED5[347];
  __IO uint32_t DREG9[2];                               /* !< (@ 0x00001710) 数据寄存器9（数组） */
       uint32_t RESERVED6[2];
  __IO uint32_t DREG10;                                 /* !< (@ 0x00001720) 数据寄存器10 */
       uint32_t RESERVED7[3];
  __IO uint32_t DREG11;                                 /* !< (@ 0x00001730) 数据寄存器11 */
  __IO uint32_t DREG12;                                 /* !< (@ 0x00001734) 数据寄存器12 */
  __IO uint32_t DREG13;                                 /* !< (@ 0x00001738) 数据寄存器13 */
  __IO uint32_t DREG14;                                 /* !< (@ 0x0000173C) 数据寄存器14 */
       uint32_t RESERVED8[4];
  __IO uint32_t DREG15;                                 /* !< (@ 0x00001750) 数据寄存器15 */
  __IO uint32_t DREG16;                                 /* !< (@ 0x00001754) 数据寄存器16 */
  __IO uint32_t DREG17;                                 /* !< (@ 0x00001758) 数据寄存器17 */
  __IO uint32_t DREG18;                                 /* !< (@ 0x0000175C) 数据寄存器18 */
       uint32_t RESERVED9[296];
  __IO uint32_t DREG19[32];                             /* !< (@ 0x00001C00) 数据寄存器19（32位数组） */
       uint32_t RESERVED10[224];
  __IO uint32_t DREG20[256];                            /* !< (@ 0x00002000) 数据寄存器20（256位数组） */
       uint32_t RESERVED11[256];
  __IO uint32_t DREG21[512];                            /* !< (@ 0x00002800) 数据寄存器21（512位数组） */
} NPU_Regs;

/*@}*/ /* end of group NPU */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* NPU Register Control Bits - NPU 寄存器控制位域
******************************************************************************/

/* ===== 中断索引寄存器位域 ===== */
/* NPU_IIDX Bits */
/* NPU_IIDX[STAT] 中断索引状态 */
/* NPU_IIDX[STAT] Bits */
#define NPU_IIDX_STAT_OFS                        (0)                             /* !< STAT Offset */
#define NPU_IIDX_STAT_MASK                       ((uint32_t)0x000000FFU)         /* !< Interrupt index status */
#define NPU_IIDX_STAT_NO_INTR                    ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define NPU_IIDX_STAT_DONE                       ((uint32_t)0x00000001U)         /* !< Done indication interrupt */

/* NPU_IMASK Bits */
/* NPU_IMASK[DONE] Bits */
#define NPU_IMASK_DONE_OFS                       (0)                             /* !< DONE Offset */
#define NPU_IMASK_DONE_MASK                      ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_IMASK_DONE_CLR                       ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_IMASK_DONE_SET                       ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_RIS Bits */
/* NPU_RIS[DONE] Bits */
#define NPU_RIS_DONE_OFS                         (0)                             /* !< DONE Offset */
#define NPU_RIS_DONE_MASK                        ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_RIS_DONE_CLR                         ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_RIS_DONE_SET                         ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_MIS Bits */
/* NPU_MIS[DONE] Bits */
#define NPU_MIS_DONE_OFS                         (0)                             /* !< DONE Offset */
#define NPU_MIS_DONE_MASK                        ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_MIS_DONE_CLR                         ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_MIS_DONE_SET                         ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_ISET Bits */
/* NPU_ISET[DONE] Bits */
#define NPU_ISET_DONE_OFS                        (0)                             /* !< DONE Offset */
#define NPU_ISET_DONE_MASK                       ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_ISET_DONE_CLR                        ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_ISET_DONE_SET                        ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_ICLR Bits */
/* NPU_ICLR[DONE] Bits */
#define NPU_ICLR_DONE_OFS                        (0)                             /* !< DONE Offset */
#define NPU_ICLR_DONE_MASK                       ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_ICLR_DONE_CLR                        ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_ICLR_DONE_SET                        ((uint32_t)0x00000001U)         /* !< Enable Event */

/* ===== 电源使能寄存器位域 ===== */
/* NPU_PWREN Bits */
/* NPU_PWREN[ENABLE] 电源使能位 */
/* NPU_PWREN[ENABLE] Bits */
#define NPU_PWREN_ENABLE_OFS                     (0)                             /* !< ENABLE Offset */
#define NPU_PWREN_ENABLE_MASK                    ((uint32_t)0x00000001U)         /* !< Enable the power */
#define NPU_PWREN_ENABLE_DISABLE                 ((uint32_t)0x00000000U)         /* !< Disable Power */
#define NPU_PWREN_ENABLE_ENABLE                  ((uint32_t)0x00000001U)         /* !< Enable Power */
/* NPU_PWREN[KEY] 写保护密钥（需写入0x26解锁） */
/* NPU_PWREN[KEY] Bits */
#define NPU_PWREN_KEY_OFS                        (24)                            /* !< KEY Offset */
#define NPU_PWREN_KEY_MASK                       ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define NPU_PWREN_KEY_UNLOCK_W                   ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* ===== 复位控制寄存器位域 ===== */
/* NPU_RSTCTL Bits */
/* NPU_RSTCTL[RESETSTKYCLR] 复位粘滞标志清除位 */
/* NPU_RSTCTL[RESETSTKYCLR] Bits */
#define NPU_RSTCTL_RESETSTKYCLR_OFS              (1)                             /* !< RESETSTKYCLR Offset */
#define NPU_RSTCTL_RESETSTKYCLR_MASK             ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define NPU_RSTCTL_RESETSTKYCLR_NOP              ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define NPU_RSTCTL_RESETSTKYCLR_CLR              ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* NPU_RSTCTL[RESETASSERT] 复位置位位（写1触发模块复位） */
/* NPU_RSTCTL[RESETASSERT] Bits */
#define NPU_RSTCTL_RESETASSERT_OFS               (0)                             /* !< RESETASSERT Offset */
#define NPU_RSTCTL_RESETASSERT_MASK              ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define NPU_RSTCTL_RESETASSERT_NOP               ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define NPU_RSTCTL_RESETASSERT_ASSERT            ((uint32_t)0x00000001U)         /* !< Assert reset */
/* NPU_RSTCTL[KEY] 写保护密钥（需写入0xB1解锁） */
/* NPU_RSTCTL[KEY] Bits */
#define NPU_RSTCTL_KEY_OFS                       (24)                            /* !< KEY Offset */
#define NPU_RSTCTL_KEY_MASK                      ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define NPU_RSTCTL_KEY_UNLOCK_W                  ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* ===== 状态寄存器位域 ===== */
/* NPU_STAT Bits */
/* NPU_STAT[RESETSTKY] 复位粘滞标志 */
/* NPU_STAT[RESETSTKY] Bits */
#define NPU_STAT_RESETSTKY_OFS                   (16)                            /* !< RESETSTKY Offset */
#define NPU_STAT_RESETSTKY_MASK                  ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define NPU_STAT_RESETSTKY_NORES                 ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define NPU_STAT_RESETSTKY_RESET                 ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */

/* ===== 模块描述寄存器位域 ===== */
/* NPU_DESC Bits */
/* NPU_DESC[MODULEID] 模块ID */
/* NPU_DESC[MODULEID] Bits */
#define NPU_DESC_MODULEID_OFS                    (16)                            /* !< MODULEID Offset */
#define NPU_DESC_MODULEID_MASK                   ((uint32_t)0xFFFF0000U)         /* !< specifies module id */
/* NPU_DESC[FEATUREVER] Bits */
#define NPU_DESC_FEATUREVER_OFS                  (12)                            /* !< FEATUREVER Offset */
#define NPU_DESC_FEATUREVER_MASK                 ((uint32_t)0x0000F000U)         /* !< specifies feature version info */
/* NPU_DESC[INSTNUM] Bits */
#define NPU_DESC_INSTNUM_OFS                     (8)                             /* !< INSTNUM Offset */
#define NPU_DESC_INSTNUM_MASK                    ((uint32_t)0x00000F00U)         /* !< specifies module instance number */
/* NPU_DESC[MAJREV] Bits */
#define NPU_DESC_MAJREV_OFS                      (4)                             /* !< MAJREV Offset */
#define NPU_DESC_MAJREV_MASK                     ((uint32_t)0x000000F0U)         /* !< specifies major revision id */
/* NPU_DESC[MINREV] Bits */
#define NPU_DESC_MINREV_OFS                      (0)                             /* !< MINREV Offset */
#define NPU_DESC_MINREV_MASK                     ((uint32_t)0x0000000FU)         /* !< specifies minor revision id */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_npu__include */
