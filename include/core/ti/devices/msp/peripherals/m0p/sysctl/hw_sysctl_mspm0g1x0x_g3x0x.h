/*****************************************************************************

  Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/ 

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
 * @file hw_sysctl_mspm0g1x0x_g3x0x.h
 * @brief MSPM0G1x0x/G3x0x SYSCTL (系统控制) 外设寄存器定义
 *
 * SYSCTL 模块管理系统时钟、电源模式、复位和系统配置:
 *
 * 中断管理:
 *   - IIIDX/IMASK/RIS/MIS/ISET/ICLR: 系统控制中断 (LFOSCGOOD/ANACLKERR/FlashSEC/SRAMSEC等)
 *   - NMIIIDX/NMIRIS/NMIISET/NMIICLR: NMI (不可屏蔽中断) (BOR/WWDT/LFCLKFAIL/FlashDED/SRAMDED)
 *
 * 时钟系统:
 *   - SYSOSCCFG: 系统振荡器 (SYSOSC) 配置 (频率/STOP模式策略/异步请求阻断)
 *   - MCLKCFG: 主时钟 (MCLK) 配置 (分频/Flash等待状态/HSCLK/LFCLK选择)
 *   - HSCLKEN/HSCLKCFG: 高速时钟源 (HFXT/SYSPLL) 使能与选择
 *   - HFCLKCLKCFG: 高频时钟 (HFXT) 启动时间和监控配置
 *   - LFCLKCFG: 低频时钟 (LFXT) 驱动强度/监控/低功耗模式
 *   - SYSPLLCFG0/1, SYSPLLPARAM0/1: 系统PLL配置 (参考/反馈分频/输出分频/环路滤波器)
 *   - GENCLKCFG/GENCLKEN: 通用时钟 (CLK_OUT/MFPCLK/CANCLK/FCC) 配置
 *   - FCC: 频率时钟计数器
 *
 * 电源与复位:
 *   - PMODECFG: 深睡眠模式选择 (STOP/STANDBY/SHUTDOWN)
 *   - RESETLEVEL/RESETCMD: 软件复位 (CPU/BOOT/BSL/POR)
 *   - BORTHRESHOLD/BORCLRCMD: 欠压复位 (BOR) 阈值配置
 *   - CLKSTATUS/SYSSTATUS: 时钟和系统状态指示
 *   - RSTCAUSE: 复位原因记录 (读清)
 *
 * 系统配置:
 *   - WRITELOCK: 寄存器写保护
 *   - SRAMBOUNDARY: SRAM读写/只执行边界
 *   - SYSTEMCFG: 系统配置 (ECC/WWDT复位行为)
 *   - SWDCFG/EXRSTPIN: SWD引脚/NRST引脚功能禁用
 *   - SHUTDNSTORE0-3: SHUTDOWN模式数据保持
 *   - PMUOPAMP: GPAMP (通用运放) 控制
 */

#ifndef ti_devices_msp_peripherals_m0p_hw_sysctl_mspm0g1x0x_g3x0x__include
#define ti_devices_msp_peripherals_m0p_hw_sysctl_mspm0g1x0x_g3x0x__include

/* Filename: hw_sysctl_mspm0g1x0x_g3x0x.h */
/* Revised: 2024-10-15 01:23:05 */
/* Revision: 9bc476ca81dfb607b7fffe21bf712d6d17d66845 */

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
* SYSCTL Registers — 系统控制寄存器结构体定义
* SOCLOCK区域包含: 中断控制、时钟配置、电源模式、复位控制、系统状态
******************************************************************************/
#define SYSCTL_SOCLOCK_OFS                       ((uint32_t)0x00001000U)


/** @addtogroup SYSCTL_SOCLOCK
  @{
*/

typedef struct {
       uint32_t RESERVED0[8];
  /*—— 中断控制 (SYSCTL中断) ——*/
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) SYSCTL中断索引 */
       uint32_t RESERVED1;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) SYSCTL中断掩码 */
       uint32_t RESERVED2;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) SYSCTL原始中断状态 */
       uint32_t RESERVED3;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) SYSCTL屏蔽后中断状态 */
       uint32_t RESERVED4;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) SYSCTL中断置位 */
       uint32_t RESERVED5;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) SYSCTL中断清除 */
       uint32_t RESERVED6;
  /*—— NMI (不可屏蔽中断) 控制 ——*/
  __I  uint32_t NMIIIDX;                           /* !< (@ 0x00001050) NMI中断索引 */
       uint32_t RESERVED7[3];
  __I  uint32_t NMIRIS;                            /* !< (@ 0x00001060) NMI原始中断状态 */
       uint32_t RESERVED8[3];
  __O  uint32_t NMIISET;                           /* !< (@ 0x00001070) NMI中断置位 */
       uint32_t RESERVED9;
  __O  uint32_t NMIICLR;                           /* !< (@ 0x00001078) NMI中断清除 */
       uint32_t RESERVED10[33];
  /*—— 时钟配置 ——*/
  __IO uint32_t SYSOSCCFG;                         /* !< (@ 0x00001100) SYSOSC配置 (频率/STOP模式策略) */
  __IO uint32_t MCLKCFG;                           /* !< (@ 0x00001104) 主时钟MCLK配置 (分频/Flash等待/源选择) */
  __IO uint32_t HSCLKEN;                           /* !< (@ 0x00001108) 高速时钟源使能 (HFXT/SYSPLL/HFCLK_IN) */
  __IO uint32_t HSCLKCFG;                          /* !< (@ 0x0000110C) 高速时钟源选择 (SYSPLL或HFCLK) */
  __IO uint32_t HFCLKCLKCFG;                       /* !< (@ 0x00001110) HFCLK配置 (HFXT启动时间/频率范围/监控) */
  __IO uint32_t LFCLKCFG;                          /* !< (@ 0x00001114) LFCLK配置 (LFXT驱动强度/监控/低功耗) */
       uint32_t RESERVED11[2];
  __IO uint32_t SYSPLLCFG0;                        /* !< (@ 0x00001120) SYSPLL参考和输出配置 */
  __IO uint32_t SYSPLLCFG1;                        /* !< (@ 0x00001124) SYSPLL参考和反馈分频 */
  __IO uint32_t SYSPLLPARAM0;                      /* !< (@ 0x00001128) SYSPLL参数0 (从工厂区域加载) */
  __IO uint32_t SYSPLLPARAM1;                      /* !< (@ 0x0000112C) SYSPLL参数1 (从工厂区域加载) */
       uint32_t RESERVED12[2];
  __IO uint32_t GENCLKCFG;                         /* !< (@ 0x00001138) 通用时钟配置 (CLK_OUT/MFPCLK/CANCLK/FCC) */
  __IO uint32_t GENCLKEN;                          /* !< (@ 0x0000113C) 通用时钟使能 (CLK_OUT/MFPCLK) */
  __IO uint32_t PMODECFG;                          /* !< (@ 0x00001140) 电源模式配置 (深睡眠目标模式) */
       uint32_t RESERVED13[3];
  __I  uint32_t FCC;                               /* !< (@ 0x00001150) 频率时钟计数器 (FCC) 计数值 */
       uint32_t RESERVED14[7];
  __IO uint32_t SYSOSCTRIMUSER;                    /* !< (@ 0x00001170) SYSOSC用户自定义频率校准参数 */
       uint32_t RESERVED15;
  __IO uint32_t SRAMBOUNDARY;                      /* !< (@ 0x00001178) SRAM读写/只执行边界配置 */
       uint32_t RESERVED16;
  __IO uint32_t SYSTEMCFG;                         /* !< (@ 0x00001180) 系统配置 (ECC/WWDT复位行为) */
       uint32_t RESERVED17[31];
  /*—— 状态与复位控制 ——*/
  __IO uint32_t WRITELOCK;                         /* !< (@ 0x00001200) SYSCTL寄存器写保护锁 */
  __I  uint32_t CLKSTATUS;                         /* !< (@ 0x00001204) 时钟模块状态 (各时钟源就绪/错误指示) */
  __I  uint32_t SYSSTATUS;                         /* !< (@ 0x00001208) 系统状态 (ECC/BOR/SWD/MCAN等) */
  __I  uint32_t DEDERRADDR;                        /* !< (@ 0x0000120C) 存储器双位错误地址 */
       uint32_t RESERVED18[4];
  __I  uint32_t RSTCAUSE;                          /* !< (@ 0x00001220) 复位原因 (读清) */
       uint32_t RESERVED19[55];
  __IO uint32_t RESETLEVEL;                        /* !< (@ 0x00001300) 软件复位级别选择 (CPU/BOOT/BSL/POR) */
  __O  uint32_t RESETCMD;                          /* !< (@ 0x00001304) 执行软件复位命令 */
  __IO uint32_t BORTHRESHOLD;                      /* !< (@ 0x00001308) BOR阈值选择 */
  __O  uint32_t BORCLRCMD;                         /* !< (@ 0x0000130C) 清除BOR状态并设置阈值 */
  __O  uint32_t SYSOSCFCLCTL;                      /* !< (@ 0x00001310) SYSOSC频率校准环 (FCL) 使能 */
  __O  uint32_t LFXTCTL;                           /* !< (@ 0x00001314) LFXT和LFCLK控制 */
  __O  uint32_t EXLFCTL;                           /* !< (@ 0x00001318) LFCLK_IN和LFCLK控制 */
  __O  uint32_t SHDNIOREL;                         /* !< (@ 0x0000131C) SHUTDOWN模式IO释放控制 */
  __O  uint32_t EXRSTPIN;                          /* !< (@ 0x00001320) 禁用NRST引脚的复位功能 */
  __O  uint32_t SYSSTATUSCLR;                      /* !< (@ 0x00001324) 清除SYSSTATUS粘滞位 */
  __O  uint32_t SWDCFG;                            /* !< (@ 0x00001328) 禁用SWD引脚上的SWD功能 */
  __O  uint32_t FCCCMD;                            /* !< (@ 0x0000132C) 频率时钟计数器启动捕获 */
       uint32_t RESERVED20[20];
  /*—— 模拟外设控制 ——*/
  __IO uint32_t PMUOPAMP;                          /* !< (@ 0x00001380) GPAMP (通用运放) 控制 */
       uint32_t RESERVED21[31];
  /*—— SHUTDOWN数据保持 ——*/
  __IO uint32_t SHUTDNSTORE0;                      /* !< (@ 0x00001400) SHUTDOWN数据保持字节0 */
  __IO uint32_t SHUTDNSTORE1;                      /* !< (@ 0x00001404) SHUTDOWN数据保持字节1 */
  __IO uint32_t SHUTDNSTORE2;                      /* !< (@ 0x00001408) SHUTDOWN数据保持字节2 */
  __IO uint32_t SHUTDNSTORE3;                      /* !< (@ 0x0000140C) SHUTDOWN数据保持字节3 */
} SYSCTL_SOCLOCK_Regs;

/*@}*/ /* end of group SYSCTL_SOCLOCK */

/** @addtogroup SYSCTL
  @{
*/

typedef struct {
       uint32_t RESERVED0[1024];
  SYSCTL_SOCLOCK_Regs  SOCLOCK;                           /* !< (@ 0x00001000) SYSCTL SOCLOCK Region */
} SYSCTL_Regs;

/*@}*/ /* end of group SYSCTL */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* SYSCTL Register Control Bits — 系统控制寄存器位域定义
******************************************************************************/

/*—————————————————————————————————————————————————————————————————————————————
 * 系统控制中断 — IIDX (中断索引寄存器)
 * 最高优先级待处理中断编号，读取后自动清除对应RIS标志
 * 中断源: LFOSCGOOD / ANACLKERR / FLASHSEC / SRAMSEC / LFXTGOOD / HFCLKGOOD / SYSPLLGOOD / HSCLKGOOD
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_IIDX[STAT] — 中断索引状态 [3:0] */
#define SYSCTL_IIDX_STAT_OFS                     (0)                             /* !< STAT Offset */
#define SYSCTL_IIDX_STAT_MASK                    ((uint32_t)0x0000000FU)         /* !< The SYSCTL interrupt index (IIDX)
                                                                                    register generates a value
                                                                                    corresponding to the highest priority
                                                                                    pending interrupt source.  This value
                                                                                    may be used as an address offset for
                                                                                    fast, deterministic handling in the
                                                                                    interrupt service routine.  A read of
                                                                                    the IIDX register will clear the
                                                                                    corresponding interrupt status in the
                                                                                    RIS and MIS registers. */
#define SYSCTL_IIDX_STAT_NO_INTR                 ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define SYSCTL_IIDX_STAT_LFOSCGOOD               ((uint32_t)0x00000001U)         /* !< LFOSCGOOD interrupt pending */
#define SYSCTL_IIDX_STAT_ANACLKERR               ((uint32_t)0x00000002U)
#define SYSCTL_IIDX_STAT_FLASHSEC                ((uint32_t)0x00000003U)
#define SYSCTL_IIDX_STAT_SRAMSEC                 ((uint32_t)0x00000004U)
#define SYSCTL_IIDX_STAT_LFXTGOOD                ((uint32_t)0x00000005U)
#define SYSCTL_IIDX_STAT_HFCLKGOOD               ((uint32_t)0x00000006U)
#define SYSCTL_IIDX_STAT_SYSPLLGOOD              ((uint32_t)0x00000007U)
#define SYSCTL_IIDX_STAT_HSCLKGOOD               ((uint32_t)0x00000008U)

/*—————————————————————————————————————————————————————————————————————————————
 * SYSCTL中断 — IMASK (中断掩码) / RIS (原始状态) / MIS (屏蔽后状态) / ISET (置位) / ICLR (清除)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_IMASK[LFOSCGOOD] — LFOSC就绪中断掩码 [0] */
#define SYSCTL_IMASK_LFOSCGOOD_OFS               (0)                             /* !< LFOSCGOOD Offset */
#define SYSCTL_IMASK_LFOSCGOOD_MASK              ((uint32_t)0x00000001U)         /* !< Enable or disable the LFOSCGOOD
                                                                                    interrupt. LFOSCGOOD indicates that
                                                                                    the LFOSC has started successfully. */
#define SYSCTL_IMASK_LFOSCGOOD_DISABLE           ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define SYSCTL_IMASK_LFOSCGOOD_ENABLE            ((uint32_t)0x00000001U)         /* !< Interrupt enabled */
/* SYSCTL_IMASK[HFCLKGOOD] Bits */
#define SYSCTL_IMASK_HFCLKGOOD_OFS               (5)                             /* !< HFCLKGOOD Offset */
#define SYSCTL_IMASK_HFCLKGOOD_MASK              ((uint32_t)0x00000020U)         /* !< HFCLK GOOD */
#define SYSCTL_IMASK_HFCLKGOOD_DISABLE           ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_HFCLKGOOD_ENABLE            ((uint32_t)0x00000020U)
/* SYSCTL_IMASK[SRAMSEC] Bits */
#define SYSCTL_IMASK_SRAMSEC_OFS                 (3)                             /* !< SRAMSEC Offset */
#define SYSCTL_IMASK_SRAMSEC_MASK                ((uint32_t)0x00000008U)         /* !< SRAM Single Error Correct */
#define SYSCTL_IMASK_SRAMSEC_DISABLE             ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_SRAMSEC_ENABLE              ((uint32_t)0x00000008U)
/* SYSCTL_IMASK[LFXTGOOD] Bits */
#define SYSCTL_IMASK_LFXTGOOD_OFS                (4)                             /* !< LFXTGOOD Offset */
#define SYSCTL_IMASK_LFXTGOOD_MASK               ((uint32_t)0x00000010U)         /* !< LFXT GOOD */
#define SYSCTL_IMASK_LFXTGOOD_DISABLE            ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_LFXTGOOD_ENABLE             ((uint32_t)0x00000010U)
/* SYSCTL_IMASK[HSCLKGOOD] Bits */
#define SYSCTL_IMASK_HSCLKGOOD_OFS               (7)                             /* !< HSCLKGOOD Offset */
#define SYSCTL_IMASK_HSCLKGOOD_MASK              ((uint32_t)0x00000080U)         /* !< HSCLK GOOD */
#define SYSCTL_IMASK_HSCLKGOOD_DISABLE           ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_HSCLKGOOD_ENABLE            ((uint32_t)0x00000080U)
/* SYSCTL_IMASK[SYSPLLGOOD] Bits */
#define SYSCTL_IMASK_SYSPLLGOOD_OFS              (6)                             /* !< SYSPLLGOOD Offset */
#define SYSCTL_IMASK_SYSPLLGOOD_MASK             ((uint32_t)0x00000040U)         /* !< SYSPLL GOOD */
#define SYSCTL_IMASK_SYSPLLGOOD_DISABLE          ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_SYSPLLGOOD_ENABLE           ((uint32_t)0x00000040U)
/* SYSCTL_IMASK[ANACLKERR] Bits */
#define SYSCTL_IMASK_ANACLKERR_OFS               (1)                             /* !< ANACLKERR Offset */
#define SYSCTL_IMASK_ANACLKERR_MASK              ((uint32_t)0x00000002U)         /* !< Analog Clocking Consistency Error */
#define SYSCTL_IMASK_ANACLKERR_DISABLE           ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_ANACLKERR_ENABLE            ((uint32_t)0x00000002U)
/* SYSCTL_IMASK[FLASHSEC] Bits */
#define SYSCTL_IMASK_FLASHSEC_OFS                (2)                             /* !< FLASHSEC Offset */
#define SYSCTL_IMASK_FLASHSEC_MASK               ((uint32_t)0x00000004U)         /* !< Flash Single Error Correct */
#define SYSCTL_IMASK_FLASHSEC_DISABLE            ((uint32_t)0x00000000U)
#define SYSCTL_IMASK_FLASHSEC_ENABLE             ((uint32_t)0x00000004U)

/* SYSCTL_RIS Bits */
/* SYSCTL_RIS[LFOSCGOOD] Bits */
#define SYSCTL_RIS_LFOSCGOOD_OFS                 (0)                             /* !< LFOSCGOOD Offset */
#define SYSCTL_RIS_LFOSCGOOD_MASK                ((uint32_t)0x00000001U)         /* !< Raw status of the LFOSCGOOD
                                                                                    interrupt. */
#define SYSCTL_RIS_LFOSCGOOD_FALSE               ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define SYSCTL_RIS_LFOSCGOOD_TRUE                ((uint32_t)0x00000001U)         /* !< Interrupt pending */
/* SYSCTL_RIS[HFCLKGOOD] Bits */
#define SYSCTL_RIS_HFCLKGOOD_OFS                 (5)                             /* !< HFCLKGOOD Offset */
#define SYSCTL_RIS_HFCLKGOOD_MASK                ((uint32_t)0x00000020U)         /* !< HFCLK GOOD */
#define SYSCTL_RIS_HFCLKGOOD_FALSE               ((uint32_t)0x00000000U)
#define SYSCTL_RIS_HFCLKGOOD_TRUE                ((uint32_t)0x00000020U)
/* SYSCTL_RIS[SRAMSEC] Bits */
#define SYSCTL_RIS_SRAMSEC_OFS                   (3)                             /* !< SRAMSEC Offset */
#define SYSCTL_RIS_SRAMSEC_MASK                  ((uint32_t)0x00000008U)         /* !< SRAM Single Error Correct */
#define SYSCTL_RIS_SRAMSEC_FALSE                 ((uint32_t)0x00000000U)
#define SYSCTL_RIS_SRAMSEC_TRUE                  ((uint32_t)0x00000008U)
/* SYSCTL_RIS[LFXTGOOD] Bits */
#define SYSCTL_RIS_LFXTGOOD_OFS                  (4)                             /* !< LFXTGOOD Offset */
#define SYSCTL_RIS_LFXTGOOD_MASK                 ((uint32_t)0x00000010U)         /* !< LFXT GOOD */
#define SYSCTL_RIS_LFXTGOOD_FALSE                ((uint32_t)0x00000000U)
#define SYSCTL_RIS_LFXTGOOD_TRUE                 ((uint32_t)0x00000010U)
/* SYSCTL_RIS[HSCLKGOOD] Bits */
#define SYSCTL_RIS_HSCLKGOOD_OFS                 (7)                             /* !< HSCLKGOOD Offset */
#define SYSCTL_RIS_HSCLKGOOD_MASK                ((uint32_t)0x00000080U)         /* !< HSCLK GOOD */
#define SYSCTL_RIS_HSCLKGOOD_FALSE               ((uint32_t)0x00000000U)
#define SYSCTL_RIS_HSCLKGOOD_TRUE                ((uint32_t)0x00000080U)
/* SYSCTL_RIS[SYSPLLGOOD] Bits */
#define SYSCTL_RIS_SYSPLLGOOD_OFS                (6)                             /* !< SYSPLLGOOD Offset */
#define SYSCTL_RIS_SYSPLLGOOD_MASK               ((uint32_t)0x00000040U)         /* !< SYSPLL GOOD */
#define SYSCTL_RIS_SYSPLLGOOD_FALSE              ((uint32_t)0x00000000U)
#define SYSCTL_RIS_SYSPLLGOOD_TRUE               ((uint32_t)0x00000040U)
/* SYSCTL_RIS[ANACLKERR] Bits */
#define SYSCTL_RIS_ANACLKERR_OFS                 (1)                             /* !< ANACLKERR Offset */
#define SYSCTL_RIS_ANACLKERR_MASK                ((uint32_t)0x00000002U)         /* !< Analog Clocking Consistency Error */
#define SYSCTL_RIS_ANACLKERR_FALSE               ((uint32_t)0x00000000U)
#define SYSCTL_RIS_ANACLKERR_TRUE                ((uint32_t)0x00000002U)
/* SYSCTL_RIS[FLASHSEC] Bits */
#define SYSCTL_RIS_FLASHSEC_OFS                  (2)                             /* !< FLASHSEC Offset */
#define SYSCTL_RIS_FLASHSEC_MASK                 ((uint32_t)0x00000004U)         /* !< Flash Single Error Correct */
#define SYSCTL_RIS_FLASHSEC_FALSE                ((uint32_t)0x00000000U)
#define SYSCTL_RIS_FLASHSEC_TRUE                 ((uint32_t)0x00000004U)

/* SYSCTL_MIS Bits */
/* SYSCTL_MIS[LFOSCGOOD] Bits */
#define SYSCTL_MIS_LFOSCGOOD_OFS                 (0)                             /* !< LFOSCGOOD Offset */
#define SYSCTL_MIS_LFOSCGOOD_MASK                ((uint32_t)0x00000001U)         /* !< Masked status of the LFOSCGOOD
                                                                                    interrupt. */
#define SYSCTL_MIS_LFOSCGOOD_FALSE               ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define SYSCTL_MIS_LFOSCGOOD_TRUE                ((uint32_t)0x00000001U)         /* !< Interrupt pending */
/* SYSCTL_MIS[HFCLKGOOD] Bits */
#define SYSCTL_MIS_HFCLKGOOD_OFS                 (5)                             /* !< HFCLKGOOD Offset */
#define SYSCTL_MIS_HFCLKGOOD_MASK                ((uint32_t)0x00000020U)         /* !< HFCLK GOOD */
#define SYSCTL_MIS_HFCLKGOOD_FALSE               ((uint32_t)0x00000000U)
#define SYSCTL_MIS_HFCLKGOOD_TRUE                ((uint32_t)0x00000020U)
/* SYSCTL_MIS[SRAMSEC] Bits */
#define SYSCTL_MIS_SRAMSEC_OFS                   (3)                             /* !< SRAMSEC Offset */
#define SYSCTL_MIS_SRAMSEC_MASK                  ((uint32_t)0x00000008U)         /* !< SRAM Single Error Correct */
#define SYSCTL_MIS_SRAMSEC_FALSE                 ((uint32_t)0x00000000U)
#define SYSCTL_MIS_SRAMSEC_TRUE                  ((uint32_t)0x00000008U)
/* SYSCTL_MIS[LFXTGOOD] Bits */
#define SYSCTL_MIS_LFXTGOOD_OFS                  (4)                             /* !< LFXTGOOD Offset */
#define SYSCTL_MIS_LFXTGOOD_MASK                 ((uint32_t)0x00000010U)         /* !< LFXT GOOD */
#define SYSCTL_MIS_LFXTGOOD_FALSE                ((uint32_t)0x00000000U)
#define SYSCTL_MIS_LFXTGOOD_TRUE                 ((uint32_t)0x00000010U)
/* SYSCTL_MIS[HSCLKGOOD] Bits */
#define SYSCTL_MIS_HSCLKGOOD_OFS                 (7)                             /* !< HSCLKGOOD Offset */
#define SYSCTL_MIS_HSCLKGOOD_MASK                ((uint32_t)0x00000080U)         /* !< HSCLK GOOD */
#define SYSCTL_MIS_HSCLKGOOD_FALSE               ((uint32_t)0x00000000U)
#define SYSCTL_MIS_HSCLKGOOD_TRUE                ((uint32_t)0x00000080U)
/* SYSCTL_MIS[SYSPLLGOOD] Bits */
#define SYSCTL_MIS_SYSPLLGOOD_OFS                (6)                             /* !< SYSPLLGOOD Offset */
#define SYSCTL_MIS_SYSPLLGOOD_MASK               ((uint32_t)0x00000040U)         /* !< SYSPLL GOOD */
#define SYSCTL_MIS_SYSPLLGOOD_FALSE              ((uint32_t)0x00000000U)
#define SYSCTL_MIS_SYSPLLGOOD_TRUE               ((uint32_t)0x00000040U)
/* SYSCTL_MIS[ANACLKERR] Bits */
#define SYSCTL_MIS_ANACLKERR_OFS                 (1)                             /* !< ANACLKERR Offset */
#define SYSCTL_MIS_ANACLKERR_MASK                ((uint32_t)0x00000002U)         /* !< Analog Clocking Consistency Error */
#define SYSCTL_MIS_ANACLKERR_FALSE               ((uint32_t)0x00000000U)
#define SYSCTL_MIS_ANACLKERR_TRUE                ((uint32_t)0x00000002U)
/* SYSCTL_MIS[FLASHSEC] Bits */
#define SYSCTL_MIS_FLASHSEC_OFS                  (2)                             /* !< FLASHSEC Offset */
#define SYSCTL_MIS_FLASHSEC_MASK                 ((uint32_t)0x00000004U)         /* !< Flash Single Error Correct */
#define SYSCTL_MIS_FLASHSEC_FALSE                ((uint32_t)0x00000000U)
#define SYSCTL_MIS_FLASHSEC_TRUE                 ((uint32_t)0x00000004U)

/* SYSCTL_ISET Bits */
/* SYSCTL_ISET[LFOSCGOOD] Bits */
#define SYSCTL_ISET_LFOSCGOOD_OFS                (0)                             /* !< LFOSCGOOD Offset */
#define SYSCTL_ISET_LFOSCGOOD_MASK               ((uint32_t)0x00000001U)         /* !< Set the LFOSCGOOD interrupt. */
#define SYSCTL_ISET_LFOSCGOOD_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0h hs no effect */
#define SYSCTL_ISET_LFOSCGOOD_SET                ((uint32_t)0x00000001U)         /* !< Set interrupt */
/* SYSCTL_ISET[HFCLKGOOD] Bits */
#define SYSCTL_ISET_HFCLKGOOD_OFS                (5)                             /* !< HFCLKGOOD Offset */
#define SYSCTL_ISET_HFCLKGOOD_MASK               ((uint32_t)0x00000020U)         /* !< HFCLK GOOD */
#define SYSCTL_ISET_HFCLKGOOD_NO_EFFECT          ((uint32_t)0x00000000U)
#define SYSCTL_ISET_HFCLKGOOD_SET                ((uint32_t)0x00000020U)
/* SYSCTL_ISET[SRAMSEC] Bits */
#define SYSCTL_ISET_SRAMSEC_OFS                  (3)                             /* !< SRAMSEC Offset */
#define SYSCTL_ISET_SRAMSEC_MASK                 ((uint32_t)0x00000008U)         /* !< SRAM Single Error Correct */
#define SYSCTL_ISET_SRAMSEC_NO_EFFECT            ((uint32_t)0x00000000U)
#define SYSCTL_ISET_SRAMSEC_SET                  ((uint32_t)0x00000008U)
/* SYSCTL_ISET[LFXTGOOD] Bits */
#define SYSCTL_ISET_LFXTGOOD_OFS                 (4)                             /* !< LFXTGOOD Offset */
#define SYSCTL_ISET_LFXTGOOD_MASK                ((uint32_t)0x00000010U)         /* !< LFXT GOOD */
#define SYSCTL_ISET_LFXTGOOD_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_ISET_LFXTGOOD_SET                 ((uint32_t)0x00000010U)
/* SYSCTL_ISET[HSCLKGOOD] Bits */
#define SYSCTL_ISET_HSCLKGOOD_OFS                (7)                             /* !< HSCLKGOOD Offset */
#define SYSCTL_ISET_HSCLKGOOD_MASK               ((uint32_t)0x00000080U)         /* !< HSCLK GOOD */
#define SYSCTL_ISET_HSCLKGOOD_NO_EFFECT          ((uint32_t)0x00000000U)
#define SYSCTL_ISET_HSCLKGOOD_SET                ((uint32_t)0x00000080U)
/* SYSCTL_ISET[SYSPLLGOOD] Bits */
#define SYSCTL_ISET_SYSPLLGOOD_OFS               (6)                             /* !< SYSPLLGOOD Offset */
#define SYSCTL_ISET_SYSPLLGOOD_MASK              ((uint32_t)0x00000040U)         /* !< SYSPLL GOOD */
#define SYSCTL_ISET_SYSPLLGOOD_NO_EFFECT         ((uint32_t)0x00000000U)
#define SYSCTL_ISET_SYSPLLGOOD_SET               ((uint32_t)0x00000040U)
/* SYSCTL_ISET[ANACLKERR] Bits */
#define SYSCTL_ISET_ANACLKERR_OFS                (1)                             /* !< ANACLKERR Offset */
#define SYSCTL_ISET_ANACLKERR_MASK               ((uint32_t)0x00000002U)         /* !< Analog Clocking Consistency Error */
#define SYSCTL_ISET_ANACLKERR_NO_EFFECT          ((uint32_t)0x00000000U)
#define SYSCTL_ISET_ANACLKERR_SET                ((uint32_t)0x00000002U)
/* SYSCTL_ISET[FLASHSEC] Bits */
#define SYSCTL_ISET_FLASHSEC_OFS                 (2)                             /* !< FLASHSEC Offset */
#define SYSCTL_ISET_FLASHSEC_MASK                ((uint32_t)0x00000004U)         /* !< Flash Single Error Correct */
#define SYSCTL_ISET_FLASHSEC_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_ISET_FLASHSEC_SET                 ((uint32_t)0x00000004U)

/* SYSCTL_ICLR Bits */
/* SYSCTL_ICLR[LFOSCGOOD] Bits */
#define SYSCTL_ICLR_LFOSCGOOD_OFS                (0)                             /* !< LFOSCGOOD Offset */
#define SYSCTL_ICLR_LFOSCGOOD_MASK               ((uint32_t)0x00000001U)         /* !< Clear the LFOSCGOOD interrupt. */
#define SYSCTL_ICLR_LFOSCGOOD_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0h has no effect */
#define SYSCTL_ICLR_LFOSCGOOD_CLR                ((uint32_t)0x00000001U)         /* !< Clear interrupt */
/* SYSCTL_ICLR[HFCLKGOOD] Bits */
#define SYSCTL_ICLR_HFCLKGOOD_OFS                (5)                             /* !< HFCLKGOOD Offset */
#define SYSCTL_ICLR_HFCLKGOOD_MASK               ((uint32_t)0x00000020U)         /* !< HFCLK GOOD */
#define SYSCTL_ICLR_HFCLKGOOD_NO_EFFECT          ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_HFCLKGOOD_CLR                ((uint32_t)0x00000020U)
/* SYSCTL_ICLR[SRAMSEC] Bits */
#define SYSCTL_ICLR_SRAMSEC_OFS                  (3)                             /* !< SRAMSEC Offset */
#define SYSCTL_ICLR_SRAMSEC_MASK                 ((uint32_t)0x00000008U)         /* !< SRAM Single Error Correct */
#define SYSCTL_ICLR_SRAMSEC_NO_EFFECT            ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_SRAMSEC_CLR                  ((uint32_t)0x00000008U)
/* SYSCTL_ICLR[LFXTGOOD] Bits */
#define SYSCTL_ICLR_LFXTGOOD_OFS                 (4)                             /* !< LFXTGOOD Offset */
#define SYSCTL_ICLR_LFXTGOOD_MASK                ((uint32_t)0x00000010U)         /* !< LFXT GOOD */
#define SYSCTL_ICLR_LFXTGOOD_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_LFXTGOOD_CLR                 ((uint32_t)0x00000010U)
/* SYSCTL_ICLR[HSCLKGOOD] Bits */
#define SYSCTL_ICLR_HSCLKGOOD_OFS                (7)                             /* !< HSCLKGOOD Offset */
#define SYSCTL_ICLR_HSCLKGOOD_MASK               ((uint32_t)0x00000080U)         /* !< HSCLK GOOD */
#define SYSCTL_ICLR_HSCLKGOOD_NO_EFFECT          ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_HSCLKGOOD_CLR                ((uint32_t)0x00000080U)
/* SYSCTL_ICLR[SYSPLLGOOD] Bits */
#define SYSCTL_ICLR_SYSPLLGOOD_OFS               (6)                             /* !< SYSPLLGOOD Offset */
#define SYSCTL_ICLR_SYSPLLGOOD_MASK              ((uint32_t)0x00000040U)         /* !< SYSPLL GOOD */
#define SYSCTL_ICLR_SYSPLLGOOD_NO_EFFECT         ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_SYSPLLGOOD_CLR               ((uint32_t)0x00000040U)
/* SYSCTL_ICLR[ANACLKERR] Bits */
#define SYSCTL_ICLR_ANACLKERR_OFS                (1)                             /* !< ANACLKERR Offset */
#define SYSCTL_ICLR_ANACLKERR_MASK               ((uint32_t)0x00000002U)         /* !< Analog Clocking Consistency Error */
#define SYSCTL_ICLR_ANACLKERR_NO_EFFECT          ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_ANACLKERR_CLR                ((uint32_t)0x00000002U)
/* SYSCTL_ICLR[FLASHSEC] Bits */
#define SYSCTL_ICLR_FLASHSEC_OFS                 (2)                             /* !< FLASHSEC Offset */
#define SYSCTL_ICLR_FLASHSEC_MASK                ((uint32_t)0x00000004U)         /* !< Flash Single Error Correct */
#define SYSCTL_ICLR_FLASHSEC_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_ICLR_FLASHSEC_CLR                 ((uint32_t)0x00000004U)

/*—————————————————————————————————————————————————————————————————————————————
 * NMI (不可屏蔽中断) — NMIIIDX / NMIRIS / NMIISET / NMIICLR
 * NMI源: BORLVL / WWDT0 / WWDT1 / LFCLKFAIL / FLASHDED / SRAMDED
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_NMIIIDX[STAT] — NMI中断索引 [3:0] */
#define SYSCTL_NMIIIDX_STAT_OFS                  (0)                             /* !< STAT Offset */
#define SYSCTL_NMIIIDX_STAT_MASK                 ((uint32_t)0x0000000FU)         /* !< The NMI interrupt index (NMIIIDX)
                                                                                    register generates a value
                                                                                    corresponding to the highest priority
                                                                                    pending NMI source.  This value may
                                                                                    be used as an address offset for
                                                                                    fast, deterministic handling in the
                                                                                    NMI service routine.  A read of the
                                                                                    NMIIIDX register will clear the
                                                                                    corresponding interrupt status in the
                                                                                    NMIRIS register. */
#define SYSCTL_NMIIIDX_STAT_NO_INTR              ((uint32_t)0x00000000U)         /* !< No NMI pending */
#define SYSCTL_NMIIIDX_STAT_BORLVL               ((uint32_t)0x00000001U)         /* !< BOR Threshold NMI pending */
#define SYSCTL_NMIIIDX_STAT_WWDT0                ((uint32_t)0x00000002U)
#define SYSCTL_NMIIIDX_STAT_WWDT1                ((uint32_t)0x00000003U)
#define SYSCTL_NMIIIDX_STAT_LFCLKFAIL            ((uint32_t)0x00000004U)
#define SYSCTL_NMIIIDX_STAT_FLASHDED             ((uint32_t)0x00000005U)
#define SYSCTL_NMIIIDX_STAT_SRAMDED              ((uint32_t)0x00000006U)

/* SYSCTL_NMIRIS Bits */
/* SYSCTL_NMIRIS[WWDT1] Bits */
#define SYSCTL_NMIRIS_WWDT1_OFS                  (2)                             /* !< WWDT1 Offset */
#define SYSCTL_NMIRIS_WWDT1_MASK                 ((uint32_t)0x00000004U)         /* !< Watch Dog 0 Fault */
#define SYSCTL_NMIRIS_WWDT1_FALSE                ((uint32_t)0x00000000U)
#define SYSCTL_NMIRIS_WWDT1_TRUE                 ((uint32_t)0x00000004U)
/* SYSCTL_NMIRIS[SRAMDED] Bits */
#define SYSCTL_NMIRIS_SRAMDED_OFS                (5)                             /* !< SRAMDED Offset */
#define SYSCTL_NMIRIS_SRAMDED_MASK               ((uint32_t)0x00000020U)         /* !< SRAM Double Error Detect */
#define SYSCTL_NMIRIS_SRAMDED_FALSE              ((uint32_t)0x00000000U)
#define SYSCTL_NMIRIS_SRAMDED_TRUE               ((uint32_t)0x00000020U)
/* SYSCTL_NMIRIS[BORLVL] Bits */
#define SYSCTL_NMIRIS_BORLVL_OFS                 (0)                             /* !< BORLVL Offset */
#define SYSCTL_NMIRIS_BORLVL_MASK                ((uint32_t)0x00000001U)         /* !< Raw status of the BORLVL NMI */
#define SYSCTL_NMIRIS_BORLVL_FALSE               ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define SYSCTL_NMIRIS_BORLVL_TRUE                ((uint32_t)0x00000001U)         /* !< Interrupt pending */
/* SYSCTL_NMIRIS[FLASHDED] Bits */
#define SYSCTL_NMIRIS_FLASHDED_OFS               (4)                             /* !< FLASHDED Offset */
#define SYSCTL_NMIRIS_FLASHDED_MASK              ((uint32_t)0x00000010U)         /* !< Flash Double Error Detect */
#define SYSCTL_NMIRIS_FLASHDED_FALSE             ((uint32_t)0x00000000U)
#define SYSCTL_NMIRIS_FLASHDED_TRUE              ((uint32_t)0x00000010U)
/* SYSCTL_NMIRIS[WWDT0] Bits */
#define SYSCTL_NMIRIS_WWDT0_OFS                  (1)                             /* !< WWDT0 Offset */
#define SYSCTL_NMIRIS_WWDT0_MASK                 ((uint32_t)0x00000002U)         /* !< Watch Dog 0 Fault */
#define SYSCTL_NMIRIS_WWDT0_FALSE                ((uint32_t)0x00000000U)
#define SYSCTL_NMIRIS_WWDT0_TRUE                 ((uint32_t)0x00000002U)
/* SYSCTL_NMIRIS[LFCLKFAIL] Bits */
#define SYSCTL_NMIRIS_LFCLKFAIL_OFS              (3)                             /* !< LFCLKFAIL Offset */
#define SYSCTL_NMIRIS_LFCLKFAIL_MASK             ((uint32_t)0x00000008U)         /* !< LFXT-EXLF Monitor Fail */
#define SYSCTL_NMIRIS_LFCLKFAIL_FALSE            ((uint32_t)0x00000000U)
#define SYSCTL_NMIRIS_LFCLKFAIL_TRUE             ((uint32_t)0x00000008U)

/* SYSCTL_NMIISET Bits */
/* SYSCTL_NMIISET[WWDT1] Bits */
#define SYSCTL_NMIISET_WWDT1_OFS                 (2)                             /* !< WWDT1 Offset */
#define SYSCTL_NMIISET_WWDT1_MASK                ((uint32_t)0x00000004U)         /* !< Watch Dog 0 Fault */
#define SYSCTL_NMIISET_WWDT1_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_NMIISET_WWDT1_SET                 ((uint32_t)0x00000004U)
/* SYSCTL_NMIISET[SRAMDED] Bits */
#define SYSCTL_NMIISET_SRAMDED_OFS               (5)                             /* !< SRAMDED Offset */
#define SYSCTL_NMIISET_SRAMDED_MASK              ((uint32_t)0x00000020U)         /* !< SRAM Double Error Detect */
#define SYSCTL_NMIISET_SRAMDED_NO_EFFECT         ((uint32_t)0x00000000U)
#define SYSCTL_NMIISET_SRAMDED_SET               ((uint32_t)0x00000020U)
/* SYSCTL_NMIISET[BORLVL] Bits */
#define SYSCTL_NMIISET_BORLVL_OFS                (0)                             /* !< BORLVL Offset */
#define SYSCTL_NMIISET_BORLVL_MASK               ((uint32_t)0x00000001U)         /* !< Set the BORLVL NMI */
#define SYSCTL_NMIISET_BORLVL_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0h hs no effect */
#define SYSCTL_NMIISET_BORLVL_SET                ((uint32_t)0x00000001U)         /* !< Set interrupt */
/* SYSCTL_NMIISET[FLASHDED] Bits */
#define SYSCTL_NMIISET_FLASHDED_OFS              (4)                             /* !< FLASHDED Offset */
#define SYSCTL_NMIISET_FLASHDED_MASK             ((uint32_t)0x00000010U)         /* !< Flash Double Error Detect */
#define SYSCTL_NMIISET_FLASHDED_NO_EFFECT        ((uint32_t)0x00000000U)
#define SYSCTL_NMIISET_FLASHDED_SET              ((uint32_t)0x00000010U)
/* SYSCTL_NMIISET[WWDT0] Bits */
#define SYSCTL_NMIISET_WWDT0_OFS                 (1)                             /* !< WWDT0 Offset */
#define SYSCTL_NMIISET_WWDT0_MASK                ((uint32_t)0x00000002U)         /* !< Watch Dog 0 Fault */
#define SYSCTL_NMIISET_WWDT0_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_NMIISET_WWDT0_SET                 ((uint32_t)0x00000002U)
/* SYSCTL_NMIISET[LFCLKFAIL] Bits */
#define SYSCTL_NMIISET_LFCLKFAIL_OFS             (3)                             /* !< LFCLKFAIL Offset */
#define SYSCTL_NMIISET_LFCLKFAIL_MASK            ((uint32_t)0x00000008U)         /* !< LFXT-EXLF Monitor Fail */
#define SYSCTL_NMIISET_LFCLKFAIL_NO_EFFECT       ((uint32_t)0x00000000U)
#define SYSCTL_NMIISET_LFCLKFAIL_SET             ((uint32_t)0x00000008U)

/* SYSCTL_NMIICLR Bits */
/* SYSCTL_NMIICLR[WWDT1] Bits */
#define SYSCTL_NMIICLR_WWDT1_OFS                 (2)                             /* !< WWDT1 Offset */
#define SYSCTL_NMIICLR_WWDT1_MASK                ((uint32_t)0x00000004U)         /* !< Watch Dog 0 Fault */
#define SYSCTL_NMIICLR_WWDT1_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_NMIICLR_WWDT1_CLR                 ((uint32_t)0x00000004U)
/* SYSCTL_NMIICLR[SRAMDED] Bits */
#define SYSCTL_NMIICLR_SRAMDED_OFS               (5)                             /* !< SRAMDED Offset */
#define SYSCTL_NMIICLR_SRAMDED_MASK              ((uint32_t)0x00000020U)         /* !< SRAM Double Error Detect */
#define SYSCTL_NMIICLR_SRAMDED_NO_EFFECT         ((uint32_t)0x00000000U)
#define SYSCTL_NMIICLR_SRAMDED_CLR               ((uint32_t)0x00000020U)
/* SYSCTL_NMIICLR[BORLVL] Bits */
#define SYSCTL_NMIICLR_BORLVL_OFS                (0)                             /* !< BORLVL Offset */
#define SYSCTL_NMIICLR_BORLVL_MASK               ((uint32_t)0x00000001U)         /* !< Clr the BORLVL NMI */
#define SYSCTL_NMIICLR_BORLVL_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0h hs no effect */
#define SYSCTL_NMIICLR_BORLVL_CLR                ((uint32_t)0x00000001U)         /* !< Clear interrupt */
/* SYSCTL_NMIICLR[FLASHDED] Bits */
#define SYSCTL_NMIICLR_FLASHDED_OFS              (4)                             /* !< FLASHDED Offset */
#define SYSCTL_NMIICLR_FLASHDED_MASK             ((uint32_t)0x00000010U)         /* !< Flash Double Error Detect */
#define SYSCTL_NMIICLR_FLASHDED_NO_EFFECT        ((uint32_t)0x00000000U)
#define SYSCTL_NMIICLR_FLASHDED_CLR              ((uint32_t)0x00000010U)
/* SYSCTL_NMIICLR[WWDT0] Bits */
#define SYSCTL_NMIICLR_WWDT0_OFS                 (1)                             /* !< WWDT0 Offset */
#define SYSCTL_NMIICLR_WWDT0_MASK                ((uint32_t)0x00000002U)         /* !< Watch Dog 0 Fault */
#define SYSCTL_NMIICLR_WWDT0_NO_EFFECT           ((uint32_t)0x00000000U)
#define SYSCTL_NMIICLR_WWDT0_CLR                 ((uint32_t)0x00000002U)
/* SYSCTL_NMIICLR[LFCLKFAIL] Bits */
#define SYSCTL_NMIICLR_LFCLKFAIL_OFS             (3)                             /* !< LFCLKFAIL Offset */
#define SYSCTL_NMIICLR_LFCLKFAIL_MASK            ((uint32_t)0x00000008U)         /* !< LFXT-EXLF Monitor Fail */
#define SYSCTL_NMIICLR_LFCLKFAIL_NO_EFFECT       ((uint32_t)0x00000000U)
#define SYSCTL_NMIICLR_LFCLKFAIL_CLR             ((uint32_t)0x00000008U)

/*—————————————————————————————————————————————————————————————————————————————
 * SYSOSCCFG (系统振荡器配置寄存器)
 * 配置SYSOSC频率、STOP模式策略、异步时钟请求阻断
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSOSCCFG[USE4MHZSTOP] — STOP模式切换到4MHz [8] */
#define SYSCTL_SYSOSCCFG_USE4MHZSTOP_OFS         (8)                             /* !< USE4MHZSTOP Offset */
#define SYSCTL_SYSOSCCFG_USE4MHZSTOP_MASK        ((uint32_t)0x00000100U)         /* !< USE4MHZSTOP sets the SYSOSC stop
                                                                                    mode frequency policy.  When entering
                                                                                    STOP mode, the SYSOSC frequency may
                                                                                    be automatically switched to 4MHz to
                                                                                    reduce SYSOSC power consumption. */
#define SYSCTL_SYSOSCCFG_USE4MHZSTOP_DISABLE     ((uint32_t)0x00000000U)         /* !< Do not gear shift the SYSOSC to
                                                                                    4MHz in STOP mode */
#define SYSCTL_SYSOSCCFG_USE4MHZSTOP_ENABLE      ((uint32_t)0x00000100U)         /* !< Gear shift SYSOSC to 4MHz in STOP
                                                                                    mode */
/* SYSCTL_SYSOSCCFG[DISABLESTOP] Bits */
#define SYSCTL_SYSOSCCFG_DISABLESTOP_OFS         (9)                             /* !< DISABLESTOP Offset */
#define SYSCTL_SYSOSCCFG_DISABLESTOP_MASK        ((uint32_t)0x00000200U)         /* !< DISABLESTOP sets the SYSOSC stop
                                                                                    mode enable/disable policy.  When
                                                                                    operating in STOP mode, the SYSOSC
                                                                                    may be automatically disabled.  When
                                                                                    set, ULPCLK will run from LFCLK in
                                                                                    STOP mode and SYSOSC will be disabled
                                                                                    to reduce power consumption. */
#define SYSCTL_SYSOSCCFG_DISABLESTOP_DISABLE     ((uint32_t)0x00000000U)         /* !< Do not disable SYSOSC in STOP mode */
#define SYSCTL_SYSOSCCFG_DISABLESTOP_ENABLE      ((uint32_t)0x00000200U)         /* !< Disable SYSOSC in STOP mode and
                                                                                    source ULPCLK from LFCLK */
/* SYSCTL_SYSOSCCFG[BLOCKASYNCALL] Bits */
#define SYSCTL_SYSOSCCFG_BLOCKASYNCALL_OFS       (16)                            /* !< BLOCKASYNCALL Offset */
#define SYSCTL_SYSOSCCFG_BLOCKASYNCALL_MASK      ((uint32_t)0x00010000U)         /* !< BLOCKASYNCALL may be used to mask
                                                                                    block all asynchronous fast clock
                                                                                    requests, preventing hardware from
                                                                                    dynamically changing the active clock
                                                                                    configuration when operating in a
                                                                                    given mode. */
#define SYSCTL_SYSOSCCFG_BLOCKASYNCALL_DISABLE   ((uint32_t)0x00000000U)         /* !< Asynchronous fast clock requests
                                                                                    are controlled by the requesting
                                                                                    peripheral */
#define SYSCTL_SYSOSCCFG_BLOCKASYNCALL_ENABLE    ((uint32_t)0x00010000U)         /* !< All asynchronous fast clock
                                                                                    requests are blocked */
/* SYSCTL_SYSOSCCFG[DISABLE] Bits */
#define SYSCTL_SYSOSCCFG_DISABLE_OFS             (10)                            /* !< DISABLE Offset */
#define SYSCTL_SYSOSCCFG_DISABLE_MASK            ((uint32_t)0x00000400U)         /* !< DISABLE sets the SYSOSC
                                                                                    enable/disable policy.  SYSOSC may be
                                                                                    powered off in RUN, SLEEP, and STOP
                                                                                    modes to reduce power consumption.
                                                                                    When SYSOSC is disabled, MCLK and
                                                                                    ULPCLK are sourced from LFCLK. */
#define SYSCTL_SYSOSCCFG_DISABLE_DISABLE         ((uint32_t)0x00000000U)         /* !< Do not disable SYSOSC */
#define SYSCTL_SYSOSCCFG_DISABLE_ENABLE          ((uint32_t)0x00000400U)         /* !< Disable SYSOSC immediately and
                                                                                    source MCLK and ULPCLK from LFCLK */
/* SYSCTL_SYSOSCCFG[FASTCPUEVENT] Bits */
#define SYSCTL_SYSOSCCFG_FASTCPUEVENT_OFS        (17)                            /* !< FASTCPUEVENT Offset */
#define SYSCTL_SYSOSCCFG_FASTCPUEVENT_MASK       ((uint32_t)0x00020000U)         /* !< FASTCPUEVENT may be used to assert
                                                                                    a fast clock request when an
                                                                                    interrupt is asserted to the CPU,
                                                                                    reducing interrupt latency. */
#define SYSCTL_SYSOSCCFG_FASTCPUEVENT_DISABLE    ((uint32_t)0x00000000U)         /* !< An interrupt to the CPU will not
                                                                                    assert a fast clock request */
#define SYSCTL_SYSOSCCFG_FASTCPUEVENT_ENABLE     ((uint32_t)0x00020000U)         /* !< An interrupt to the CPU will assert
                                                                                    a fast clock request */
/* SYSCTL_SYSOSCCFG[FREQ] Bits */
#define SYSCTL_SYSOSCCFG_FREQ_OFS                (0)                             /* !< FREQ Offset */
#define SYSCTL_SYSOSCCFG_FREQ_MASK               ((uint32_t)0x00000003U)         /* !< Target operating frequency for the
                                                                                    system oscillator (SYSOSC) */
#define SYSCTL_SYSOSCCFG_FREQ_SYSOSCBASE         ((uint32_t)0x00000000U)         /* !< Base frequency (32MHz) */
#define SYSCTL_SYSOSCCFG_FREQ_SYSOSC4M           ((uint32_t)0x00000001U)         /* !< Low frequency (4MHz) */
#define SYSCTL_SYSOSCCFG_FREQ_SYSOSCUSER         ((uint32_t)0x00000002U)         /* !< User-trimmed frequency (16 or 24
                                                                                    MHz) */

/*—————————————————————————————————————————————————————————————————————————————
 * MCLKCFG (主时钟MCLK配置寄存器)
 * 配置MCLK分频、Flash等待状态、HSCLK/LFCLK源选择、ULPCLK分频
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_MCLKCFG[USEMFTICK] — MFCLK (4MHz) 使能 [12] */
#define SYSCTL_MCLKCFG_USEMFTICK_OFS             (12)                            /* !< USEMFTICK Offset */
#define SYSCTL_MCLKCFG_USEMFTICK_MASK            ((uint32_t)0x00001000U)         /* !< USEMFTICK specifies whether the
                                                                                    4MHz constant-rate clock (MFCLK) to
                                                                                    peripherals is enabled or disabled.
                                                                                    When enabled, MDIV must be disabled
                                                                                    (set to 0h=/1). */
#define SYSCTL_MCLKCFG_USEMFTICK_DISABLE         ((uint32_t)0x00000000U)         /* !< The 4MHz rate MFCLK to peripherals
                                                                                    is enabled */
#define SYSCTL_MCLKCFG_USEMFTICK_ENABLE          ((uint32_t)0x00001000U)         /* !< The 4MHz rate MFCLK to peripherals
                                                                                    is enabled. */
/* SYSCTL_MCLKCFG[MDIV] Bits */
#define SYSCTL_MCLKCFG_MDIV_OFS                  (0)                             /* !< MDIV Offset */
#define SYSCTL_MCLKCFG_MDIV_MASK                 ((uint32_t)0x0000000FU)         /* !< MDIV may be used to divide the MCLK
                                                                                    frequency when MCLK is sourced from
                                                                                    SYSOSC.  MDIV=0h corresponds to /1
                                                                                    (no divider).  MDIV=1h corresponds to
                                                                                    /2 (divide-by-2).  MDIV=Fh
                                                                                    corresponds to /16 (divide-by-16).
                                                                                    MDIV may be set between /1 and /16 on
                                                                                    an integer basis. */
/* SYSCTL_MCLKCFG[USEHSCLK] Bits */
#define SYSCTL_MCLKCFG_USEHSCLK_OFS              (16)                            /* !< USEHSCLK Offset */
#define SYSCTL_MCLKCFG_USEHSCLK_MASK             ((uint32_t)0x00010000U)         /* !< USEHSCLK, together with USELFCLK,
                                                                                    sets the MCLK source policy.  Set
                                                                                    USEHSCLK to use HSCLK (HFCLK or
                                                                                    SYSPLL) as the MCLK source in RUN and
                                                                                    SLEEP modes. */
#define SYSCTL_MCLKCFG_USEHSCLK_DISABLE          ((uint32_t)0x00000000U)         /* !< MCLK will not use the high speed
                                                                                    clock (HSCLK) */
#define SYSCTL_MCLKCFG_USEHSCLK_ENABLE           ((uint32_t)0x00010000U)         /* !< MCLK will use the high speed clock
                                                                                    (HSCLK) in RUN and SLEEP mode */
/* SYSCTL_MCLKCFG[USELFCLK] Bits */
#define SYSCTL_MCLKCFG_USELFCLK_OFS              (20)                            /* !< USELFCLK Offset */
#define SYSCTL_MCLKCFG_USELFCLK_MASK             ((uint32_t)0x00100000U)         /* !< USELFCLK sets the MCLK source
                                                                                    policy.  Set USELFCLK to use LFCLK as
                                                                                    the MCLK source.  Note that setting
                                                                                    USELFCLK does not disable SYSOSC, and
                                                                                    SYSOSC remains available for direct
                                                                                    use by analog modules. */
#define SYSCTL_MCLKCFG_USELFCLK_DISABLE          ((uint32_t)0x00000000U)         /* !< MCLK will not use the low frequency
                                                                                    clock (LFCLK) */
#define SYSCTL_MCLKCFG_USELFCLK_ENABLE           ((uint32_t)0x00100000U)         /* !< MCLK will use the low frequency
                                                                                    clock (LFCLK) */
/* SYSCTL_MCLKCFG[STOPCLKSTBY] Bits */
#define SYSCTL_MCLKCFG_STOPCLKSTBY_OFS           (21)                            /* !< STOPCLKSTBY Offset */
#define SYSCTL_MCLKCFG_STOPCLKSTBY_MASK          ((uint32_t)0x00200000U)         /* !< STOPCLKSTBY sets the STANDBY mode
                                                                                    policy (STANDBY0 or STANDBY1).  When
                                                                                    set, ULPCLK and LFCLK are disabled to
                                                                                    all peripherals in STANDBY mode, with
                                                                                    the exception of TIMG0 and TIMG1
                                                                                    which continue to run.  Wake-up is
                                                                                    only possible via an asynchronous
                                                                                    fast clock request. */
#define SYSCTL_MCLKCFG_STOPCLKSTBY_DISABLE       ((uint32_t)0x00000000U)         /* !< ULPCLK/LFCLK runs to all PD0
                                                                                    peripherals in STANDBY mode */
#define SYSCTL_MCLKCFG_STOPCLKSTBY_ENABLE        ((uint32_t)0x00200000U)         /* !< ULPCLK/LFCLK is disabled to all
                                                                                    peripherals in STANDBY mode except
                                                                                    TIMG0 and TIMG1 */
/* SYSCTL_MCLKCFG[FLASHWAIT] Bits */
#define SYSCTL_MCLKCFG_FLASHWAIT_OFS             (8)                             /* !< FLASHWAIT Offset */
#define SYSCTL_MCLKCFG_FLASHWAIT_MASK            ((uint32_t)0x00000F00U)         /* !< FLASHWAIT specifies the number of
                                                                                    flash wait states when MCLK is
                                                                                    sourced from HSCLK.  FLASHWAIT has no
                                                                                    effect when MCLK is sourced from
                                                                                    SYSOSC or LFCLK. */
#define SYSCTL_MCLKCFG_FLASHWAIT_WAIT0           ((uint32_t)0x00000000U)         /* !< No flash wait states are applied */
#define SYSCTL_MCLKCFG_FLASHWAIT_WAIT1           ((uint32_t)0x00000100U)         /* !< One flash wait state is applied */
#define SYSCTL_MCLKCFG_FLASHWAIT_WAIT2           ((uint32_t)0x00000200U)         /* !< 2 flash wait states are applied */
/* SYSCTL_MCLKCFG[MCLKDEADCHK] Bits */
#define SYSCTL_MCLKCFG_MCLKDEADCHK_OFS           (22)                            /* !< MCLKDEADCHK Offset */
#define SYSCTL_MCLKCFG_MCLKDEADCHK_MASK          ((uint32_t)0x00400000U)         /* !< MCLKDEADCHK enables or disables the
                                                                                    continuous MCLK dead check monitor.
                                                                                    LFCLK must be running before
                                                                                    MCLKDEADCHK is enabled. */
#define SYSCTL_MCLKCFG_MCLKDEADCHK_DISABLE       ((uint32_t)0x00000000U)         /* !< The MCLK dead check monitor is
                                                                                    disabled */
#define SYSCTL_MCLKCFG_MCLKDEADCHK_ENABLE        ((uint32_t)0x00400000U)         /* !< The MCLK dead check monitor is
                                                                                    enabled */
/* SYSCTL_MCLKCFG[UDIV] Bits */
#define SYSCTL_MCLKCFG_UDIV_OFS                  (4)                             /* !< UDIV Offset */
#define SYSCTL_MCLKCFG_UDIV_MASK                 ((uint32_t)0x00000030U)         /* !< UDIV specifies the ULPCLK divider
                                                                                    when MCLK is sourced from HSCLK.
                                                                                    UDIV has no effect when MCLK is
                                                                                    sourced from SYSOSC or LFCLK. */
#define SYSCTL_MCLKCFG_UDIV_NODIVIDE             ((uint32_t)0x00000000U)         /* !< ULPCLK is not divided and is equal
                                                                                    to MCLK */
#define SYSCTL_MCLKCFG_UDIV_DIVIDE2              ((uint32_t)0x00000010U)         /* !< ULPCLK is MCLK/2 (divided-by-2) */
#define SYSCTL_MCLKCFG_UDIV_DIVIDE3              ((uint32_t)0x00000020U)         /* !< ULPCLK is MCLK/3 (divided-by-3) */

/*—————————————————————————————————————————————————————————————————————————————
 * HSCLKEN (高速时钟源使能寄存器)
 * 使能/禁用HFXT、SYSPLL、HFCLK_IN
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_HSCLKEN[HFXTEN] — HFXT使能 [0] */
#define SYSCTL_HSCLKEN_HFXTEN_OFS                (0)                             /* !< HFXTEN Offset */
#define SYSCTL_HSCLKEN_HFXTEN_MASK               ((uint32_t)0x00000001U)         /* !< HFXTEN enables or disables the high
                                                                                    frequency crystal oscillator (HFXT). */
#define SYSCTL_HSCLKEN_HFXTEN_DISABLE            ((uint32_t)0x00000000U)         /* !< Disable the HFXT */
#define SYSCTL_HSCLKEN_HFXTEN_ENABLE             ((uint32_t)0x00000001U)         /* !< Enable the HFXT */
/* SYSCTL_HSCLKEN[USEEXTHFCLK] Bits */
#define SYSCTL_HSCLKEN_USEEXTHFCLK_OFS           (16)                            /* !< USEEXTHFCLK Offset */
#define SYSCTL_HSCLKEN_USEEXTHFCLK_MASK          ((uint32_t)0x00010000U)         /* !< USEEXTHFCLK selects the HFCLK_IN
                                                                                    digital clock input to be the source
                                                                                    for HFCLK.  When disabled, HFXT is
                                                                                    the HFCLK source and HFXTEN may be
                                                                                    set.  Do not set HFXTEN and
                                                                                    USEEXTHFCLK simultaneously. */
#define SYSCTL_HSCLKEN_USEEXTHFCLK_DISABLE       ((uint32_t)0x00000000U)         /* !< Use HFXT as the HFCLK source */
#define SYSCTL_HSCLKEN_USEEXTHFCLK_ENABLE        ((uint32_t)0x00010000U)         /* !< Use the HFCLK_IN digital clock
                                                                                    input as the HFCLK source */
/* SYSCTL_HSCLKEN[SYSPLLEN] Bits */
#define SYSCTL_HSCLKEN_SYSPLLEN_OFS              (8)                             /* !< SYSPLLEN Offset */
#define SYSCTL_HSCLKEN_SYSPLLEN_MASK             ((uint32_t)0x00000100U)         /* !< SYSPLLEN enables or disables the
                                                                                    system phase-lock loop (SYSPLL). */
#define SYSCTL_HSCLKEN_SYSPLLEN_DISABLE          ((uint32_t)0x00000000U)         /* !< Disable the SYSPLL */
#define SYSCTL_HSCLKEN_SYSPLLEN_ENABLE           ((uint32_t)0x00000100U)         /* !< Enable the SYSPLL */

/*—————————————————————————————————————————————————————————————————————————————
 * HSCLKCFG (高速时钟源选择寄存器)
 * 选择HSCLK来源: SYSPLL 或 HFCLK
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_HSCLKCFG[HSCLKSEL] — HSCLK源选择 [0] */
#define SYSCTL_HSCLKCFG_HSCLKSEL_OFS             (0)                             /* !< HSCLKSEL Offset */
#define SYSCTL_HSCLKCFG_HSCLKSEL_MASK            ((uint32_t)0x00000001U)         /* !< HSCLKSEL selects the HSCLK source
                                                                                    (SYSPLL or HFCLK). */
#define SYSCTL_HSCLKCFG_HSCLKSEL_SYSPLL          ((uint32_t)0x00000000U)         /* !< HSCLK is sourced from the SYSPLL */
#define SYSCTL_HSCLKCFG_HSCLKSEL_HFCLKCLK        ((uint32_t)0x00000001U)         /* !< HSCLK is sourced from the HFCLK */

/*—————————————————————————————————————————————————————————————————————————————
 * HFCLKCLKCFG (高频时钟HFCLK配置寄存器)
 * HFXT启动时间、频率范围选择、启动监控使能
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_HFCLKCLKCFG[HFXTTIME] — HFXT启动时间 [7:0]，分辨率64us */
#define SYSCTL_HFCLKCLKCFG_HFXTTIME_OFS          (0)                             /* !< HFXTTIME Offset */
#define SYSCTL_HFCLKCLKCFG_HFXTTIME_MASK         ((uint32_t)0x000000FFU)         /* !< HFXTTIME specifies the HFXT startup
                                                                                    time in 64us resolution.  If the
                                                                                    HFCLK startup monitor is enabled
                                                                                    (HFCLKFLTCHK), HFXT will be checked
                                                                                    after this time expires. */
#define SYSCTL_HFCLKCLKCFG_HFXTTIME_MINSTARTTIME ((uint32_t)0x00000000U)         /* !< Minimum startup time (approximatly
                                                                                    zero) */
#define SYSCTL_HFCLKCLKCFG_HFXTTIME_MAXSTARTTIME ((uint32_t)0x000000FFU)         /* !< Maximum startup time (approximatly
                                                                                    16.32ms) */
/* SYSCTL_HFCLKCLKCFG[HFCLKFLTCHK] Bits */
#define SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_OFS       (28)                            /* !< HFCLKFLTCHK Offset */
#define SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_MASK      ((uint32_t)0x10000000U)         /* !< HFCLKFLTCHK enables or disables the
                                                                                    HFCLK startup monitor. */
#define SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_DISABLE   ((uint32_t)0x00000000U)         /* !< HFCLK startup is not checked */
#define SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_ENABLE    ((uint32_t)0x10000000U)         /* !< HFCLK startup is checked */
/* SYSCTL_HFCLKCLKCFG[HFXTRSEL] Bits */
#define SYSCTL_HFCLKCLKCFG_HFXTRSEL_OFS          (12)                            /* !< HFXTRSEL Offset */
#define SYSCTL_HFCLKCLKCFG_HFXTRSEL_MASK         ((uint32_t)0x00003000U)         /* !< HFXT Range Select */
#define SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE4TO8    ((uint32_t)0x00000000U)         /* !< 4MHz &lt;= HFXT frequency &lt;=
                                                                                    8MHz */
#define SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE8TO16   ((uint32_t)0x00001000U)         /* !< 8MHz &lt; HFXT frequency &lt;=
                                                                                    16MHz */
#define SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE16TO32  ((uint32_t)0x00002000U)         /* !< 16MHz &lt; HFXT frequency &lt;=
                                                                                    32MHz */
#define SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE32TO48  ((uint32_t)0x00003000U)         /* !< 32MHz &lt; HFXT frequency &lt;=
                                                                                    48MHz */

/*—————————————————————————————————————————————————————————————————————————————
 * LFCLKCFG (低频时钟LFCLK配置寄存器)
 * LFXT驱动强度、时钟监控、低电容模式
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_LFCLKCFG[XT1DRIVE] — LFXT驱动强度 [1:0] */
#define SYSCTL_LFCLKCFG_XT1DRIVE_OFS             (0)                             /* !< XT1DRIVE Offset */
#define SYSCTL_LFCLKCFG_XT1DRIVE_MASK            ((uint32_t)0x00000003U)         /* !< XT1DRIVE selects the low frequency
                                                                                    crystal oscillator (LFXT) drive
                                                                                    strength. */
#define SYSCTL_LFCLKCFG_XT1DRIVE_LOWESTDRV       ((uint32_t)0x00000000U)         /* !< Lowest drive and current */
#define SYSCTL_LFCLKCFG_XT1DRIVE_LOWERDRV        ((uint32_t)0x00000001U)         /* !< Lower drive and current */
#define SYSCTL_LFCLKCFG_XT1DRIVE_HIGHERDRV       ((uint32_t)0x00000002U)         /* !< Higher drive and current */
#define SYSCTL_LFCLKCFG_XT1DRIVE_HIGHESTDRV      ((uint32_t)0x00000003U)         /* !< Highest drive and current */
/* SYSCTL_LFCLKCFG[MONITOR] Bits */
#define SYSCTL_LFCLKCFG_MONITOR_OFS              (4)                             /* !< MONITOR Offset */
#define SYSCTL_LFCLKCFG_MONITOR_MASK             ((uint32_t)0x00000010U)         /* !< MONITOR enables or disables the
                                                                                    LFCLK monitor, which continuously
                                                                                    checks LFXT or LFCLK_IN for a clock
                                                                                    stuck fault. */
#define SYSCTL_LFCLKCFG_MONITOR_DISABLE          ((uint32_t)0x00000000U)         /* !< Clock monitor is disabled */
#define SYSCTL_LFCLKCFG_MONITOR_ENABLE           ((uint32_t)0x00000010U)         /* !< Clock monitor is enabled */
/* SYSCTL_LFCLKCFG[LOWCAP] Bits */
#define SYSCTL_LFCLKCFG_LOWCAP_OFS               (8)                             /* !< LOWCAP Offset */
#define SYSCTL_LFCLKCFG_LOWCAP_MASK              ((uint32_t)0x00000100U)         /* !< LOWCAP controls the low-power LFXT
                                                                                    mode.  When the LFXT load capacitance
                                                                                    is less than 3pf, LOWCAP may be set
                                                                                    for reduced power consumption. */
#define SYSCTL_LFCLKCFG_LOWCAP_DISABLE           ((uint32_t)0x00000000U)         /* !< LFXT low capacitance mode is
                                                                                    disabled */
#define SYSCTL_LFCLKCFG_LOWCAP_ENABLE            ((uint32_t)0x00000100U)         /* !< LFXT low capacitance mode is
                                                                                    enabled */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSPLLCFG0 (系统PLL配置寄存器0)
 * PLL参考源选择、CLK0/CLK1/CLK2X输出使能和分频
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSPLLCFG0[ENABLECLK0] — SYSPLLCLK0输出使能 [4] */
#define SYSCTL_SYSPLLCFG0_ENABLECLK0_OFS         (4)                             /* !< ENABLECLK0 Offset */
#define SYSCTL_SYSPLLCFG0_ENABLECLK0_MASK        ((uint32_t)0x00000010U)         /* !< ENABLECLK0 enables or disables the
                                                                                    SYSPLLCLK0 output. */
#define SYSCTL_SYSPLLCFG0_ENABLECLK0_DISABLE     ((uint32_t)0x00000000U)         /* !< SYSPLLCLK0 is disabled */
#define SYSCTL_SYSPLLCFG0_ENABLECLK0_ENABLE      ((uint32_t)0x00000010U)         /* !< SYSPLLCLK0 is enabled */
/* SYSCTL_SYSPLLCFG0[ENABLECLK1] Bits */
#define SYSCTL_SYSPLLCFG0_ENABLECLK1_OFS         (5)                             /* !< ENABLECLK1 Offset */
#define SYSCTL_SYSPLLCFG0_ENABLECLK1_MASK        ((uint32_t)0x00000020U)         /* !< ENABLECLK1 enables or disables the
                                                                                    SYSPLLCLK1 output. */
#define SYSCTL_SYSPLLCFG0_ENABLECLK1_DISABLE     ((uint32_t)0x00000000U)         /* !< SYSPLLCLK1 is disabled */
#define SYSCTL_SYSPLLCFG0_ENABLECLK1_ENABLE      ((uint32_t)0x00000020U)         /* !< SYSPLLCLK1 is enabled */
/* SYSCTL_SYSPLLCFG0[RDIVCLK1] Bits */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_OFS           (12)                            /* !< RDIVCLK1 Offset */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_MASK          ((uint32_t)0x0000F000U)         /* !< RDIVCLK1 sets the final divider for
                                                                                    the SYSPLLCLK1 output. */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV2      ((uint32_t)0x00000000U)         /* !< SYSPLLCLK1 is divided by 2 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV4      ((uint32_t)0x00001000U)         /* !< SYSPLLCLK1 is divided by 4 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV6      ((uint32_t)0x00002000U)         /* !< SYSPLLCLK1 is divided by 6 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV8      ((uint32_t)0x00003000U)         /* !< SYSPLLCLK1 is divided by 8 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV10     ((uint32_t)0x00004000U)         /* !< SYSPLLCLK1 is divided by 10 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV12     ((uint32_t)0x00005000U)         /* !< SYSPLLCLK1 is divided by 12 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV14     ((uint32_t)0x00006000U)         /* !< SYSPLLCLK1 is divided by 14 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV16     ((uint32_t)0x00007000U)         /* !< SYSPLLCLK1 is divided by 16 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV18     ((uint32_t)0x00008000U)         /* !< SYSPLLCLK1 is divided by 18 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV20     ((uint32_t)0x00009000U)         /* !< SYSPLLCLK1 is divided by 20 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV22     ((uint32_t)0x0000A000U)         /* !< SYSPLLCLK1 is divided by 22 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV24     ((uint32_t)0x0000B000U)         /* !< SYSPLLCLK1 is divided by 24 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV26     ((uint32_t)0x0000C000U)         /* !< SYSPLLCLK1 is divided by 26 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV28     ((uint32_t)0x0000D000U)         /* !< SYSPLLCLK1 is divided by 28 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV30     ((uint32_t)0x0000E000U)         /* !< SYSPLLCLK1 is divided by 30 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK1_CLK1DIV32     ((uint32_t)0x0000F000U)         /* !< SYSPLLCLK1 is divided by 32 */
/* SYSCTL_SYSPLLCFG0[MCLK2XVCO] Bits */
#define SYSCTL_SYSPLLCFG0_MCLK2XVCO_OFS          (1)                             /* !< MCLK2XVCO Offset */
#define SYSCTL_SYSPLLCFG0_MCLK2XVCO_MASK         ((uint32_t)0x00000002U)         /* !< MCLK2XVCO selects the SYSPLL output
                                                                                    which is sent to the HSCLK mux for
                                                                                    use by MCLK. */
#define SYSCTL_SYSPLLCFG0_MCLK2XVCO_DISABLE      ((uint32_t)0x00000000U)         /* !< The SYSPLLCLK0 output is sent to
                                                                                    the HSCLK mux */
#define SYSCTL_SYSPLLCFG0_MCLK2XVCO_ENABLE       ((uint32_t)0x00000002U)         /* !< The SYSPLLCLK2X output is sent to
                                                                                    the HSCLK mux */
/* SYSCTL_SYSPLLCFG0[RDIVCLK2X] Bits */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_OFS          (16)                            /* !< RDIVCLK2X Offset */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_MASK         ((uint32_t)0x000F0000U)         /* !< RDIVCLK2X sets the final divider
                                                                                    for the SYSPLLCLK2X output. */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV1    ((uint32_t)0x00000000U)         /* !< SYSPLLCLK1 is divided by 1 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV2    ((uint32_t)0x00010000U)         /* !< SYSPLLCLK1 is divided by 2 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV3    ((uint32_t)0x00020000U)         /* !< SYSPLLCLK1 is divided by 3 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV4    ((uint32_t)0x00030000U)         /* !< SYSPLLCLK1 is divided by 4 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV5    ((uint32_t)0x00040000U)         /* !< SYSPLLCLK1 is divided by 5 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV6    ((uint32_t)0x00050000U)         /* !< SYSPLLCLK1 is divided by 6 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV7    ((uint32_t)0x00060000U)         /* !< SYSPLLCLK1 is divided by 7 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV8    ((uint32_t)0x00070000U)         /* !< SYSPLLCLK1 is divided by 8 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV9    ((uint32_t)0x00080000U)         /* !< SYSPLLCLK1 is divided by 9 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV10   ((uint32_t)0x00090000U)         /* !< SYSPLLCLK1 is divided by 10 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV11   ((uint32_t)0x000A0000U)         /* !< SYSPLLCLK1 is divided by 11 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV12   ((uint32_t)0x000B0000U)         /* !< SYSPLLCLK1 is divided by 12 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV13   ((uint32_t)0x000C0000U)         /* !< SYSPLLCLK1 is divided by 13 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV14   ((uint32_t)0x000D0000U)         /* !< SYSPLLCLK1 is divided by 14 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV15   ((uint32_t)0x000E0000U)         /* !< SYSPLLCLK1 is divided by 15 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK2X_CLK2XDIV16   ((uint32_t)0x000F0000U)         /* !< SYSPLLCLK1 is divided by 16 */
/* SYSCTL_SYSPLLCFG0[RDIVCLK0] Bits */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_OFS           (8)                             /* !< RDIVCLK0 Offset */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_MASK          ((uint32_t)0x00000F00U)         /* !< RDIVCLK0 sets the final divider for
                                                                                    the SYSPLLCLK0 output. */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV2      ((uint32_t)0x00000000U)         /* !< SYSPLLCLK0 is divided by 2 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV4      ((uint32_t)0x00000100U)         /* !< SYSPLLCLK0 is divided by 4 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV6      ((uint32_t)0x00000200U)         /* !< SYSPLLCLK0 is divided by 6 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV8      ((uint32_t)0x00000300U)         /* !< SYSPLLCLK0 is divided by 8 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV10     ((uint32_t)0x00000400U)         /* !< SYSPLLCLK0 is divided by 10 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV12     ((uint32_t)0x00000500U)         /* !< SYSPLLCLK0 is divided by 12 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV14     ((uint32_t)0x00000600U)         /* !< SYSPLLCLK0 is divided by 14 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV16     ((uint32_t)0x00000700U)         /* !< SYSPLLCLK0 is divided by 16 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV18     ((uint32_t)0x00000800U)         /* !< SYSPLLCLK0 is divided by 18 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV20     ((uint32_t)0x00000900U)         /* !< SYSPLLCLK0 is divided by 20 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV22     ((uint32_t)0x00000A00U)         /* !< SYSPLLCLK0 is divided by 22 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV24     ((uint32_t)0x00000B00U)         /* !< SYSPLLCLK0 is divided by 24 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV26     ((uint32_t)0x00000C00U)         /* !< SYSPLLCLK0 is divided by 26 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV28     ((uint32_t)0x00000D00U)         /* !< SYSPLLCLK0 is divided by 28 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV30     ((uint32_t)0x00000E00U)         /* !< SYSPLLCLK0 is divided by 30 */
#define SYSCTL_SYSPLLCFG0_RDIVCLK0_CLK0DIV32     ((uint32_t)0x00000F00U)         /* !< SYSPLLCLK0 is divided by 32 */
/* SYSCTL_SYSPLLCFG0[SYSPLLREF] Bits */
#define SYSCTL_SYSPLLCFG0_SYSPLLREF_OFS          (0)                             /* !< SYSPLLREF Offset */
#define SYSCTL_SYSPLLCFG0_SYSPLLREF_MASK         ((uint32_t)0x00000001U)         /* !< SYSPLLREF selects the system PLL
                                                                                    (SYSPLL) reference clock source. */
#define SYSCTL_SYSPLLCFG0_SYSPLLREF_SYSOSC       ((uint32_t)0x00000000U)         /* !< SYSPLL reference is SYSOSC */
#define SYSCTL_SYSPLLCFG0_SYSPLLREF_HFCLK        ((uint32_t)0x00000001U)         /* !< SYSPLL reference is HFCLK */
/* SYSCTL_SYSPLLCFG0[ENABLECLK2X] Bits */
#define SYSCTL_SYSPLLCFG0_ENABLECLK2X_OFS        (6)                             /* !< ENABLECLK2X Offset */
#define SYSCTL_SYSPLLCFG0_ENABLECLK2X_MASK       ((uint32_t)0x00000040U)         /* !< ENABLECLK2X enables or disables the
                                                                                    SYSPLLCLK2X output. */
#define SYSCTL_SYSPLLCFG0_ENABLECLK2X_DISABLE    ((uint32_t)0x00000000U)         /* !< SYSPLLCLK2X is disabled */
#define SYSCTL_SYSPLLCFG0_ENABLECLK2X_ENABLE     ((uint32_t)0x00000040U)         /* !< SYSPLLCLK2X is enabled */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSPLLCFG1 (系统PLL配置寄存器1)
 * 参考时钟预分频 (PDIV) 和反馈分频 (QDIV)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSPLLCFG1[PDIV] — 参考时钟预分频 [1:0] */
#define SYSCTL_SYSPLLCFG1_PDIV_OFS               (0)                             /* !< PDIV Offset */
#define SYSCTL_SYSPLLCFG1_PDIV_MASK              ((uint32_t)0x00000003U)         /* !< PDIV selects the SYSPLL reference
                                                                                    clock prescale divider. */
#define SYSCTL_SYSPLLCFG1_PDIV_REFDIV1           ((uint32_t)0x00000000U)         /* !< SYSPLLREF is not divided */
#define SYSCTL_SYSPLLCFG1_PDIV_REFDIV2           ((uint32_t)0x00000001U)         /* !< SYSPLLREF is divided by 2 */
#define SYSCTL_SYSPLLCFG1_PDIV_REFDIV4           ((uint32_t)0x00000002U)         /* !< SYSPLLREF is divided by 4 */
#define SYSCTL_SYSPLLCFG1_PDIV_REFDIV8           ((uint32_t)0x00000003U)         /* !< SYSPLLREF is divided by 8 */
/* SYSCTL_SYSPLLCFG1[QDIV] Bits */
#define SYSCTL_SYSPLLCFG1_QDIV_OFS               (8)                             /* !< QDIV Offset */
#define SYSCTL_SYSPLLCFG1_QDIV_MASK              ((uint32_t)0x00007F00U)         /* !< QDIV selects the SYSPLL feedback
                                                                                    path divider. */
#define SYSCTL_SYSPLLCFG1_QDIV_INVALID           ((uint32_t)0x00000000U)         /* !< Divide-by-one is not a valid QDIV
                                                                                    option */
#define SYSCTL_SYSPLLCFG1_QDIV_QDIVMIN           ((uint32_t)0x00000100U)         /* !< Feedback path is divided by 2 */
#define SYSCTL_SYSPLLCFG1_QDIV_QDIVMAX           ((uint32_t)0x00007E00U)         /* !< Feedback path is divided by 127
                                                                                    (0x7E) */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSPLLPARAM0 (系统PLL参数寄存器0，从FACTORYREGION加载)
 * 启动时间、电荷泵电流、电容B覆盖
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSPLLPARAM0[CPCURRENT] — 电荷泵电流 [21:16] */
#define SYSCTL_SYSPLLPARAM0_CPCURRENT_OFS        (16)                            /* !< CPCURRENT Offset */
#define SYSCTL_SYSPLLPARAM0_CPCURRENT_MASK       ((uint32_t)0x003F0000U)         /* !< Charge pump current */
/* SYSCTL_SYSPLLPARAM0[CAPBOVERRIDE] Bits */
#define SYSCTL_SYSPLLPARAM0_CAPBOVERRIDE_OFS     (31)                            /* !< CAPBOVERRIDE Offset */
#define SYSCTL_SYSPLLPARAM0_CAPBOVERRIDE_MASK    ((uint32_t)0x80000000U)         /* !< CAPBOVERRIDE controls the override
                                                                                    for Cap B */
#define SYSCTL_SYSPLLPARAM0_CAPBOVERRIDE_DISABLE ((uint32_t)0x00000000U)         /* !< Cap B override disabled */
#define SYSCTL_SYSPLLPARAM0_CAPBOVERRIDE_ENABLE  ((uint32_t)0x80000000U)         /* !< Cap B override enabled */
/* SYSCTL_SYSPLLPARAM0[STARTTIME] Bits */
#define SYSCTL_SYSPLLPARAM0_STARTTIME_OFS        (0)                             /* !< STARTTIME Offset */
#define SYSCTL_SYSPLLPARAM0_STARTTIME_MASK       ((uint32_t)0x0000003FU)         /* !< Startup time from enable to locked
                                                                                    clock, in 1us resolution */
/* SYSCTL_SYSPLLPARAM0[CAPBVAL] Bits */
#define SYSCTL_SYSPLLPARAM0_CAPBVAL_OFS          (24)                            /* !< CAPBVAL Offset */
#define SYSCTL_SYSPLLPARAM0_CAPBVAL_MASK         ((uint32_t)0x1F000000U)         /* !< Override value for Cap B */
/* SYSCTL_SYSPLLPARAM0[STARTTIMELP] Bits */
#define SYSCTL_SYSPLLPARAM0_STARTTIMELP_OFS      (8)                             /* !< STARTTIMELP Offset */
#define SYSCTL_SYSPLLPARAM0_STARTTIMELP_MASK     ((uint32_t)0x00003F00U)         /* !< Startup time from low power mode
                                                                                    exit to locked clock, in 1us
                                                                                    resolution */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSPLLPARAM1 (系统PLL参数寄存器1，从FACTORYREGION加载)
 * 环路滤波器电容A、电阻A、电阻C
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSPLLPARAM1[LPFCAPA] — 环路滤波器电容A [4:0] */
#define SYSCTL_SYSPLLPARAM1_LPFCAPA_OFS          (0)                             /* !< LPFCAPA Offset */
#define SYSCTL_SYSPLLPARAM1_LPFCAPA_MASK         ((uint32_t)0x0000001FU)         /* !< Loop filter Cap A */
/* SYSCTL_SYSPLLPARAM1[LPFRESC] Bits */
#define SYSCTL_SYSPLLPARAM1_LPFRESC_OFS          (24)                            /* !< LPFRESC Offset */
#define SYSCTL_SYSPLLPARAM1_LPFRESC_MASK         ((uint32_t)0xFF000000U)         /* !< Loop filter Res C */
/* SYSCTL_SYSPLLPARAM1[LPFRESA] Bits */
#define SYSCTL_SYSPLLPARAM1_LPFRESA_OFS          (8)                             /* !< LPFRESA Offset */
#define SYSCTL_SYSPLLPARAM1_LPFRESA_MASK         ((uint32_t)0x0003FF00U)         /* !< Loop filter Res A */

/*—————————————————————————————————————————————————————————————————————————————
 * GENCLKCFG (通用时钟配置寄存器)
 * CLK_OUT输出源/分频、MFPCLK源、CANCLK源、FCC配置、模拟电荷泵
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_GENCLKCFG[HFCLK4MFPCLKDIV] — HFCLK到MFPCLK的分频 [15:12] */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_OFS     (12)                            /* !< HFCLK4MFPCLKDIV Offset */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_MASK    ((uint32_t)0x0000F000U)         /* !< HFCLK4MFPCLKDIV selects the divider
                                                                                    applied to HFCLK when HFCLK is used
                                                                                    as the MFPCLK source.  Integer
                                                                                    dividers from /1 to /16 may be
                                                                                    selected. */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV1    ((uint32_t)0x00000000U)         /* !< HFCLK is not divided before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV2    ((uint32_t)0x00001000U)         /* !< HFCLK is divided by 2 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV3    ((uint32_t)0x00002000U)         /* !< HFCLK is divided by 3 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV4    ((uint32_t)0x00003000U)         /* !< HFCLK is divided by 4 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV5    ((uint32_t)0x00004000U)         /* !< HFCLK is divided by 5 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV6    ((uint32_t)0x00005000U)         /* !< HFCLK is divided by 6 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV7    ((uint32_t)0x00006000U)         /* !< HFCLK is divided by 7 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV8    ((uint32_t)0x00007000U)         /* !< HFCLK is divided by 8 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV9    ((uint32_t)0x00008000U)         /* !< HFCLK is divided by 9 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV10   ((uint32_t)0x00009000U)         /* !< HFCLK is divided by 10 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV11   ((uint32_t)0x0000A000U)         /* !< HFCLK is divided by 11 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV12   ((uint32_t)0x0000B000U)         /* !< HFCLK is divided by 12 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV13   ((uint32_t)0x0000C000U)         /* !< HFCLK is divided by 13 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV14   ((uint32_t)0x0000D000U)         /* !< HFCLK is divided by 14 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV15   ((uint32_t)0x0000E000U)         /* !< HFCLK is divided by 15 before being
                                                                                    used for MFPCLK */
#define SYSCTL_GENCLKCFG_HFCLK4MFPCLKDIV_DIV16   ((uint32_t)0x0000F000U)         /* !< HFCLK is divided by 16 before being
                                                                                    used for MFPCLK */
/* SYSCTL_GENCLKCFG[MFPCLKSRC] Bits */
#define SYSCTL_GENCLKCFG_MFPCLKSRC_OFS           (9)                             /* !< MFPCLKSRC Offset */
#define SYSCTL_GENCLKCFG_MFPCLKSRC_MASK          ((uint32_t)0x00000200U)         /* !< MFPCLKSRC selects the MFPCLK
                                                                                    (middle frequency precision clock)
                                                                                    source. */
#define SYSCTL_GENCLKCFG_MFPCLKSRC_SYSOSC        ((uint32_t)0x00000000U)         /* !< MFPCLK is sourced from SYSOSC */
#define SYSCTL_GENCLKCFG_MFPCLKSRC_HFCLK         ((uint32_t)0x00000200U)         /* !< MFPCLK is sourced from HFCLK */
/* SYSCTL_GENCLKCFG[CANCLKSRC] Bits */
#define SYSCTL_GENCLKCFG_CANCLKSRC_OFS           (8)                             /* !< CANCLKSRC Offset */
#define SYSCTL_GENCLKCFG_CANCLKSRC_MASK          ((uint32_t)0x00000100U)         /* !< CANCLKSRC selects the CANCLK
                                                                                    source. */
#define SYSCTL_GENCLKCFG_CANCLKSRC_HFCLK         ((uint32_t)0x00000000U)         /* !< CANCLK source is HFCLK */
#define SYSCTL_GENCLKCFG_CANCLKSRC_SYSPLLOUT1    ((uint32_t)0x00000100U)         /* !< CANCLK source is SYSPLLCLK1 */
/* SYSCTL_GENCLKCFG[FCCTRIGCNT] Bits */
#define SYSCTL_GENCLKCFG_FCCTRIGCNT_OFS          (24)                            /* !< FCCTRIGCNT Offset */
#define SYSCTL_GENCLKCFG_FCCTRIGCNT_MASK         ((uint32_t)0x1F000000U)         /* !< FCCTRIGCNT specifies the number of
                                                                                    trigger clock periods in the trigger
                                                                                    window. FCCTRIGCNT=0h (one trigger
                                                                                    clock period) up to 1Fh (32 trigger
                                                                                    clock periods) may be specified. */
/* SYSCTL_GENCLKCFG[ANACPUMPCFG] Bits */
#define SYSCTL_GENCLKCFG_ANACPUMPCFG_OFS         (22)                            /* !< ANACPUMPCFG Offset */
#define SYSCTL_GENCLKCFG_ANACPUMPCFG_MASK        ((uint32_t)0x00C00000U)         /* !< ANACPUMPCFG selects the analog mux
                                                                                    charge pump (VBOOST) enable method. */
#define SYSCTL_GENCLKCFG_ANACPUMPCFG_ONDEMAND    ((uint32_t)0x00000000U)         /* !< VBOOST is enabled on request from a
                                                                                    COMP, GPAMP, or OPA */
#define SYSCTL_GENCLKCFG_ANACPUMPCFG_ONACTIVE    ((uint32_t)0x00400000U)         /* !< VBOOST is enabled when the device
                                                                                    is in RUN or SLEEP mode, or when a
                                                                                    COMP/GPAMP/OPA is enabled */
#define SYSCTL_GENCLKCFG_ANACPUMPCFG_ONALWAYS    ((uint32_t)0x00800000U)         /* !< VBOOST is always enabled */
/* SYSCTL_GENCLKCFG[FCCSELCLK] Bits */
#define SYSCTL_GENCLKCFG_FCCSELCLK_OFS           (16)                            /* !< FCCSELCLK Offset */
#define SYSCTL_GENCLKCFG_FCCSELCLK_MASK          ((uint32_t)0x000F0000U)         /* !< FCCSELCLK selectes the frequency
                                                                                    clock counter (FCC) clock source. */
#define SYSCTL_GENCLKCFG_FCCSELCLK_MCLK          ((uint32_t)0x00000000U)         /* !< FCC clock is MCLK */
#define SYSCTL_GENCLKCFG_FCCSELCLK_SYSOSC        ((uint32_t)0x00010000U)         /* !< FCC clock is SYSOSC */
#define SYSCTL_GENCLKCFG_FCCSELCLK_HFCLK         ((uint32_t)0x00020000U)         /* !< FCC clock is HFCLK */
#define SYSCTL_GENCLKCFG_FCCSELCLK_EXTCLK        ((uint32_t)0x00030000U)         /* !< FCC clock is the CLK_OUT selection */
#define SYSCTL_GENCLKCFG_FCCSELCLK_SYSPLLCLK0    ((uint32_t)0x00040000U)         /* !< FCC clock is SYSPLLCLK0 */
#define SYSCTL_GENCLKCFG_FCCSELCLK_SYSPLLCLK1    ((uint32_t)0x00050000U)         /* !< FCC clock is SYSPLLCLK1 */
#define SYSCTL_GENCLKCFG_FCCSELCLK_SYSPLLCLK2X   ((uint32_t)0x00060000U)         /* !< FCC clock is SYSPLLCLK2X */
#define SYSCTL_GENCLKCFG_FCCSELCLK_FCCIN         ((uint32_t)0x00070000U)         /* !< FCC clock is the FCCIN external
                                                                                    input */
/* SYSCTL_GENCLKCFG[FCCTRIGSRC] Bits */
#define SYSCTL_GENCLKCFG_FCCTRIGSRC_OFS          (20)                            /* !< FCCTRIGSRC Offset */
#define SYSCTL_GENCLKCFG_FCCTRIGSRC_MASK         ((uint32_t)0x00100000U)         /* !< FCCTRIGSRC selects the frequency
                                                                                    clock counter (FCC) trigger source. */
#define SYSCTL_GENCLKCFG_FCCTRIGSRC_EXTPIN       ((uint32_t)0x00000000U)         /* !< FCC trigger is the external pin */
#define SYSCTL_GENCLKCFG_FCCTRIGSRC_LFCLK        ((uint32_t)0x00100000U)         /* !< FCC trigger is the LFCLK */
/* SYSCTL_GENCLKCFG[FCCLVLTRIG] Bits */
#define SYSCTL_GENCLKCFG_FCCLVLTRIG_OFS          (21)                            /* !< FCCLVLTRIG Offset */
#define SYSCTL_GENCLKCFG_FCCLVLTRIG_MASK         ((uint32_t)0x00200000U)         /* !< FCCLVLTRIG selects the frequency
                                                                                    clock counter (FCC) trigger mode. */
#define SYSCTL_GENCLKCFG_FCCLVLTRIG_RISE2RISE    ((uint32_t)0x00000000U)         /* !< Rising edge to rising edge
                                                                                    triggered */
#define SYSCTL_GENCLKCFG_FCCLVLTRIG_LEVEL        ((uint32_t)0x00200000U)         /* !< Level triggered */
/* SYSCTL_GENCLKCFG[EXCLKSRC] Bits */
#define SYSCTL_GENCLKCFG_EXCLKSRC_OFS            (0)                             /* !< EXCLKSRC Offset */
#define SYSCTL_GENCLKCFG_EXCLKSRC_MASK           ((uint32_t)0x00000007U)         /* !< EXCLKSRC selects the source for the
                                                                                    CLK_OUT external clock output block.
                                                                                    ULPCLK and MFPCLK require the CLK_OUT
                                                                                    divider (EXCLKDIVEN) to be enabled */
#define SYSCTL_GENCLKCFG_EXCLKSRC_SYSOSC         ((uint32_t)0x00000000U)         /* !< CLK_OUT is SYSOSC */
#define SYSCTL_GENCLKCFG_EXCLKSRC_ULPCLK         ((uint32_t)0x00000001U)         /* !< CLK_OUT is ULPCLK (EXCLKDIVEN must
                                                                                    be enabled) */
#define SYSCTL_GENCLKCFG_EXCLKSRC_LFCLK          ((uint32_t)0x00000002U)         /* !< CLK_OUT is LFCLK */
#define SYSCTL_GENCLKCFG_EXCLKSRC_MFPCLK         ((uint32_t)0x00000003U)         /* !< CLK_OUT is MFPCLK (EXCLKDIVEN must
                                                                                    be enabled) */
#define SYSCTL_GENCLKCFG_EXCLKSRC_HFCLK          ((uint32_t)0x00000004U)         /* !< CLK_OUT is HFCLK */
#define SYSCTL_GENCLKCFG_EXCLKSRC_SYSPLLOUT1     ((uint32_t)0x00000005U)         /* !< CLK_OUT is SYSPLLCLK1 (SYSPLLCLK1
                                                                                    must be &lt;=48MHz) */
/* SYSCTL_GENCLKCFG[EXCLKDIVVAL] Bits */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_OFS         (4)                             /* !< EXCLKDIVVAL Offset */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_MASK        ((uint32_t)0x00000070U)         /* !< EXCLKDIVVAL selects the divider
                                                                                    value for the divider in the CLK_OUT
                                                                                    external clock output block. */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV2        ((uint32_t)0x00000000U)         /* !< CLK_OUT source is divided by 2 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV4        ((uint32_t)0x00000010U)         /* !< CLK_OUT source is divided by 4 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV6        ((uint32_t)0x00000020U)         /* !< CLK_OUT source is divided by 6 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV8        ((uint32_t)0x00000030U)         /* !< CLK_OUT source is divided by 8 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV10       ((uint32_t)0x00000040U)         /* !< CLK_OUT source is divided by 10 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV12       ((uint32_t)0x00000050U)         /* !< CLK_OUT source is divided by 12 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV14       ((uint32_t)0x00000060U)         /* !< CLK_OUT source is divided by 14 */
#define SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV16       ((uint32_t)0x00000070U)         /* !< CLK_OUT source is divided by 16 */
/* SYSCTL_GENCLKCFG[EXCLKDIVEN] Bits */
#define SYSCTL_GENCLKCFG_EXCLKDIVEN_OFS          (7)                             /* !< EXCLKDIVEN Offset */
#define SYSCTL_GENCLKCFG_EXCLKDIVEN_MASK         ((uint32_t)0x00000080U)         /* !< EXCLKDIVEN enables or disables the
                                                                                    divider function of the CLK_OUT
                                                                                    external clock output block. */
#define SYSCTL_GENCLKCFG_EXCLKDIVEN_PASSTHRU     ((uint32_t)0x00000000U)         /* !< CLock divider is disabled
                                                                                    (passthrough, EXCLKDIVVAL is not
                                                                                    applied) */
#define SYSCTL_GENCLKCFG_EXCLKDIVEN_ENABLE       ((uint32_t)0x00000080U)         /* !< Clock divider is enabled
                                                                                    (EXCLKDIVVAL is applied) */

/*—————————————————————————————————————————————————————————————————————————————
 * GENCLKEN (通用时钟使能寄存器)
 * CLK_OUT和MFPCLK使能
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_GENCLKEN[EXCLKEN] — CLK_OUT外部时钟输出使能 [0] */
#define SYSCTL_GENCLKEN_EXCLKEN_OFS              (0)                             /* !< EXCLKEN Offset */
#define SYSCTL_GENCLKEN_EXCLKEN_MASK             ((uint32_t)0x00000001U)         /* !< EXCLKEN enables the CLK_OUT
                                                                                    external clock output block. */
#define SYSCTL_GENCLKEN_EXCLKEN_DISABLE          ((uint32_t)0x00000000U)         /* !< CLK_OUT block is disabled */
#define SYSCTL_GENCLKEN_EXCLKEN_ENABLE           ((uint32_t)0x00000001U)         /* !< CLK_OUT block is enabled */
/* SYSCTL_GENCLKEN[MFPCLKEN] Bits */
#define SYSCTL_GENCLKEN_MFPCLKEN_OFS             (4)                             /* !< MFPCLKEN Offset */
#define SYSCTL_GENCLKEN_MFPCLKEN_MASK            ((uint32_t)0x00000010U)         /* !< MFPCLKEN enables the middle
                                                                                    frequency precision clock (MFPCLK). */
#define SYSCTL_GENCLKEN_MFPCLKEN_DISABLE         ((uint32_t)0x00000000U)         /* !< MFPCLK is disabled */
#define SYSCTL_GENCLKEN_MFPCLKEN_ENABLE          ((uint32_t)0x00000010U)         /* !< MFPCLK is enabled */

/*—————————————————————————————————————————————————————————————————————————————
 * PMODECFG (电源模式配置寄存器)
 * DEEPSLEEP请求的目标模式选择
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_PMODECFG[DSLEEP] — 深睡眠目标模式 [1:0] */
#define SYSCTL_PMODECFG_DSLEEP_OFS               (0)                             /* !< DSLEEP Offset */
#define SYSCTL_PMODECFG_DSLEEP_MASK              ((uint32_t)0x00000003U)         /* !< DSLEEP selects the operating mode
                                                                                    to enter upon a DEEPSLEEP request
                                                                                    from the CPU. */
#define SYSCTL_PMODECFG_DSLEEP_STOP              ((uint32_t)0x00000000U)         /* !< STOP mode is entered */
#define SYSCTL_PMODECFG_DSLEEP_STANDBY           ((uint32_t)0x00000001U)         /* !< STANDBY mode is entered */
#define SYSCTL_PMODECFG_DSLEEP_SHUTDOWN          ((uint32_t)0x00000002U)         /* !< SHUTDOWN mode is entered */

/*—————————————————————————————————————————————————————————————————————————————
 * FCC (频率时钟计数器寄存器，只读)
 * 捕获的时钟计数值，用于频率测量
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_FCC[DATA] — FCC计数值 [21:0] */
#define SYSCTL_FCC_DATA_OFS                      (0)                             /* !< DATA Offset */
#define SYSCTL_FCC_DATA_MASK                     ((uint32_t)0x003FFFFFU)         /* !< 频率时钟计数器值，22位 */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSOSCTRIMUSER (SYSOSC用户校准参数寄存器)
 * 用户自定义的SYSOSC频率校准参数: 目标频率、电容、电阻粗/细调、FCL电阻
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSOSCTRIMUSER[RESCOARSE] — 电阻粗调 [13:8] */
#define SYSCTL_SYSOSCTRIMUSER_RESCOARSE_OFS      (8)                             /* !< RESCOARSE Offset */
#define SYSCTL_SYSOSCTRIMUSER_RESCOARSE_MASK     ((uint32_t)0x00003F00U)         /* !< RESCOARSE specifies the resister
                                                                                    coarse trim.  This value changes with
                                                                                    the target frequency. */
/* SYSCTL_SYSOSCTRIMUSER[RESFINE] Bits */
#define SYSCTL_SYSOSCTRIMUSER_RESFINE_OFS        (16)                            /* !< RESFINE Offset */
#define SYSCTL_SYSOSCTRIMUSER_RESFINE_MASK       ((uint32_t)0x000F0000U)         /* !< RESFINE specifies the resister fine
                                                                                    trim.  This value changes with the
                                                                                    target frequency. */
/* SYSCTL_SYSOSCTRIMUSER[RDIV] Bits */
#define SYSCTL_SYSOSCTRIMUSER_RDIV_OFS           (20)                            /* !< RDIV Offset */
#define SYSCTL_SYSOSCTRIMUSER_RDIV_MASK          ((uint32_t)0x1FF00000U)         /* !< RDIV specifies the frequency
                                                                                    correction loop (FCL) resistor trim.
                                                                                    This value changes with the target
                                                                                    frequency. */
/* SYSCTL_SYSOSCTRIMUSER[FREQ] Bits */
#define SYSCTL_SYSOSCTRIMUSER_FREQ_OFS           (0)                             /* !< FREQ Offset */
#define SYSCTL_SYSOSCTRIMUSER_FREQ_MASK          ((uint32_t)0x00000003U)         /* !< FREQ specifies the target
                                                                                    user-trimmed frequency for SYSOSC. */
#define SYSCTL_SYSOSCTRIMUSER_FREQ_SYSOSC16M     ((uint32_t)0x00000001U)         /* !< 16MHz user frequency */
#define SYSCTL_SYSOSCTRIMUSER_FREQ_SYSOSC24M     ((uint32_t)0x00000002U)         /* !< 24MHz user frequency */
/* SYSCTL_SYSOSCTRIMUSER[CAP] Bits */
#define SYSCTL_SYSOSCTRIMUSER_CAP_OFS            (4)                             /* !< CAP Offset */
#define SYSCTL_SYSOSCTRIMUSER_CAP_MASK           ((uint32_t)0x00000070U)         /* !< CAP specifies the SYSOSC capacitor
                                                                                    trim.  This value changes with the
                                                                                    target frequency. */

/*—————————————————————————————————————————————————————————————————————————————
 * SRAMBOUNDARY (SRAM边界配置寄存器)
 * 配置SRAM的读写/只执行边界地址，用于安全隔离
 * 边界地址及以下为RW，边界以上为RX；0值表示整个SRAM为RWX
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SRAMBOUNDARY[ADDR] — 边界地址 [20:5]，32字节对齐 */
#define SYSCTL_SRAMBOUNDARY_ADDR_OFS             (5)                             /* !< ADDR Offset */
#define SYSCTL_SRAMBOUNDARY_ADDR_MASK            ((uint32_t)0x000FFFE0U)         /* !< SRAM边界地址，RW区域<=此值，RX区域>此值 */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSTEMCFG (系统配置寄存器)
 * 配置Flash ECC DED和WWDT错误的复位/NMI行为
 * 写入时需配合KEY值 0x1B
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSTEMCFG[KEY] — 写入密钥 [31:24]，必须为 0x1B */
#define SYSCTL_SYSTEMCFG_KEY_OFS                 (24)                            /* !< KEY Offset */
#define SYSCTL_SYSTEMCFG_KEY_MASK                ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x1B (27) */
#define SYSCTL_SYSTEMCFG_KEY_VALUE               ((uint32_t)0x1B000000U)         /* !< 写入密钥值 */
/* SYSCTL_SYSTEMCFG[FLASHECCRSTDIS] Bits */
#define SYSCTL_SYSTEMCFG_FLASHECCRSTDIS_OFS      (2)                             /* !< FLASHECCRSTDIS Offset */
#define SYSCTL_SYSTEMCFG_FLASHECCRSTDIS_MASK     ((uint32_t)0x00000004U)         /* !< FLASHECCRSTDIS specifies whether a
                                                                                    flash ECC double error detect (DED)
                                                                                    will trigger a SYSRST or an NMI. */
#define SYSCTL_SYSTEMCFG_FLASHECCRSTDIS_FALSE    ((uint32_t)0x00000000U)         /* !< Flash ECC DED will trigger a SYSRST */
#define SYSCTL_SYSTEMCFG_FLASHECCRSTDIS_TRUE     ((uint32_t)0x00000004U)         /* !< Flash ECC DED will trigger a NMI */
/* SYSCTL_SYSTEMCFG[WWDTLP0RSTDIS] Bits */
#define SYSCTL_SYSTEMCFG_WWDTLP0RSTDIS_OFS       (0)                             /* !< WWDTLP0RSTDIS Offset */
#define SYSCTL_SYSTEMCFG_WWDTLP0RSTDIS_MASK      ((uint32_t)0x00000001U)         /* !< WWDTLP0RSTDIS specifies whether a
                                                                                    WWDT Error Event will trigger a
                                                                                    BOOTRST or an NMI. */
#define SYSCTL_SYSTEMCFG_WWDTLP0RSTDIS_FALSE     ((uint32_t)0x00000000U)         /* !< WWDTLP0 Error Event will trigger a
                                                                                    BOOTRST */
#define SYSCTL_SYSTEMCFG_WWDTLP0RSTDIS_TRUE      ((uint32_t)0x00000001U)         /* !< WWDTLP0 Error Event will trigger an
                                                                                    NMI */
/* SYSCTL_SYSTEMCFG[WWDTLP1RSTDIS] Bits */
#define SYSCTL_SYSTEMCFG_WWDTLP1RSTDIS_OFS       (1)                             /* !< WWDTLP1RSTDIS Offset */
#define SYSCTL_SYSTEMCFG_WWDTLP1RSTDIS_MASK      ((uint32_t)0x00000002U)         /* !< WWDTLP1RSTDIS specifies whether a
                                                                                    WWDT Error Event will trigger a
                                                                                    SYSRST or an NMI. */
#define SYSCTL_SYSTEMCFG_WWDTLP1RSTDIS_FALSE     ((uint32_t)0x00000000U)         /* !< WWDTLP1 Error Event will trigger a
                                                                                    SYSRST */
#define SYSCTL_SYSTEMCFG_WWDTLP1RSTDIS_TRUE      ((uint32_t)0x00000002U)         /* !< WWDTLP1 Error Event will trigger an
                                                                                    NMI */

/*—————————————————————————————————————————————————————————————————————————————
 * WRITELOCK (寄存器写保护锁)
 * 控制关键SYSCTL寄存器的写保护
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_WRITELOCK[ACTIVE] — 写保护使能 [0] */
#define SYSCTL_WRITELOCK_ACTIVE_OFS              (0)                             /* !< ACTIVE Offset */
#define SYSCTL_WRITELOCK_ACTIVE_MASK             ((uint32_t)0x00000001U)         /* !< ACTIVE controls whether critical
                                                                                    SYSCTL registers are write protected
                                                                                    or not. */
#define SYSCTL_WRITELOCK_ACTIVE_DISABLE          ((uint32_t)0x00000000U)         /* !< Allow writes to lockable registers */
#define SYSCTL_WRITELOCK_ACTIVE_ENABLE           ((uint32_t)0x00000001U)         /* !< Disallow writes to lockable
                                                                                    registers */

/*—————————————————————————————————————————————————————————————————————————————
 * CLKSTATUS (时钟状态寄存器，只读)
 * 指示各时钟源状态: 就绪/故障/死亡/选择/频率
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_CLKSTATUS[OPAMPCLKERR] — OPA时钟错误 [30] */
#define SYSCTL_CLKSTATUS_OPAMPCLKERR_OFS         (30)                            /* !< OPAMPCLKERR Offset */
#define SYSCTL_CLKSTATUS_OPAMPCLKERR_MASK        ((uint32_t)0x40000000U)         /* !< OPAMPCLKERR is set when the device
                                                                                    clock configuration does not support
                                                                                    an enabled OPA mode and the OPA may
                                                                                    not be functioning as expected. */
#define SYSCTL_CLKSTATUS_OPAMPCLKERR_FALSE       ((uint32_t)0x00000000U)         /* !< No OPA clock generation errors
                                                                                    detected */
#define SYSCTL_CLKSTATUS_OPAMPCLKERR_TRUE        ((uint32_t)0x40000000U)         /* !< OPA clock generation error detected */
/* SYSCTL_CLKSTATUS[LFOSCGOOD] Bits */
#define SYSCTL_CLKSTATUS_LFOSCGOOD_OFS           (11)                            /* !< LFOSCGOOD Offset */
#define SYSCTL_CLKSTATUS_LFOSCGOOD_MASK          ((uint32_t)0x00000800U)         /* !< LFOSCGOOD indicates when the LFOSC
                                                                                    startup has completed and the LFOSC
                                                                                    is ready for use. */
#define SYSCTL_CLKSTATUS_LFOSCGOOD_FALSE         ((uint32_t)0x00000000U)         /* !< LFOSC is not ready */
#define SYSCTL_CLKSTATUS_LFOSCGOOD_TRUE          ((uint32_t)0x00000800U)         /* !< LFOSC is ready */
/* SYSCTL_CLKSTATUS[HFCLKGOOD] Bits */
#define SYSCTL_CLKSTATUS_HFCLKGOOD_OFS           (8)                             /* !< HFCLKGOOD Offset */
#define SYSCTL_CLKSTATUS_HFCLKGOOD_MASK          ((uint32_t)0x00000100U)         /* !< HFCLKGOOD indicates that the HFCLK
                                                                                    started correctly.  When the HFXT is
                                                                                    started or HFCLK_IN is selected as
                                                                                    the HFCLK source,  this bit will be
                                                                                    set by hardware if a valid HFCLK is
                                                                                    detected, and cleared if HFCLK is not
                                                                                    operating within the expected range. */
#define SYSCTL_CLKSTATUS_HFCLKGOOD_FALSE         ((uint32_t)0x00000000U)         /* !< HFCLK did not start correctly */
#define SYSCTL_CLKSTATUS_HFCLKGOOD_TRUE          ((uint32_t)0x00000100U)         /* !< HFCLK started correctly */
/* SYSCTL_CLKSTATUS[HSCLKDEAD] Bits */
#define SYSCTL_CLKSTATUS_HSCLKDEAD_OFS           (20)                            /* !< HSCLKDEAD Offset */
#define SYSCTL_CLKSTATUS_HSCLKDEAD_MASK          ((uint32_t)0x00100000U)         /* !< HSCLKDEAD is set by hardware if the
                                                                                    selected source for HSCLK was started
                                                                                    but did not start successfully. */
#define SYSCTL_CLKSTATUS_HSCLKDEAD_FALSE         ((uint32_t)0x00000000U)         /* !< The HSCLK source was not started or
                                                                                    started correctly */
#define SYSCTL_CLKSTATUS_HSCLKDEAD_TRUE          ((uint32_t)0x00100000U)         /* !< The HSCLK source did not start
                                                                                    correctly */
/* SYSCTL_CLKSTATUS[SYSPLLBLKUPD] Bits */
#define SYSCTL_CLKSTATUS_SYSPLLBLKUPD_OFS        (29)                            /* !< SYSPLLBLKUPD Offset */
#define SYSCTL_CLKSTATUS_SYSPLLBLKUPD_MASK       ((uint32_t)0x20000000U)         /* !< SYSPLLBLKUPD indicates when writes
                                                                                    to SYSPLLCFG0/1 and SYSPLLPARAM0/1
                                                                                    are blocked. */
#define SYSCTL_CLKSTATUS_SYSPLLBLKUPD_FALSE      ((uint32_t)0x00000000U)         /* !< writes to SYSPLLCFG0/1 and
                                                                                    SYSPLLPARAM0/1 are allowed */
#define SYSCTL_CLKSTATUS_SYSPLLBLKUPD_TRUE       ((uint32_t)0x20000000U)         /* !< writes to SYSPLLCFG0/1 and
                                                                                    SYSPLLPARAM0/1 are blocked */
/* SYSCTL_CLKSTATUS[HFCLKOFF] Bits */
#define SYSCTL_CLKSTATUS_HFCLKOFF_OFS            (13)                            /* !< HFCLKOFF Offset */
#define SYSCTL_CLKSTATUS_HFCLKOFF_MASK           ((uint32_t)0x00002000U)         /* !< HFCLKOFF indicates if the HFCLK is
                                                                                    disabled or was dead at startup.
                                                                                    When the HFCLK is started, HFCLKOFF
                                                                                    is cleared by hardware.  Following
                                                                                    startup of the HFCLK, if the HFCLK
                                                                                    startup monitor determines that the
                                                                                    HFCLK was not started correctly,
                                                                                    HFCLKOFF is set. */
#define SYSCTL_CLKSTATUS_HFCLKOFF_FALSE          ((uint32_t)0x00000000U)         /* !< HFCLK started correctly and is
                                                                                    enabled */
#define SYSCTL_CLKSTATUS_HFCLKOFF_TRUE           ((uint32_t)0x00002000U)         /* !< HFCLK is disabled or was dead at
                                                                                    startup */
/* SYSCTL_CLKSTATUS[HFCLKBLKUPD] Bits */
#define SYSCTL_CLKSTATUS_HFCLKBLKUPD_OFS         (28)                            /* !< HFCLKBLKUPD Offset */
#define SYSCTL_CLKSTATUS_HFCLKBLKUPD_MASK        ((uint32_t)0x10000000U)         /* !< HFCLKBLKUPD indicates when writes
                                                                                    to the HFCLKCLKCFG register are
                                                                                    blocked. */
#define SYSCTL_CLKSTATUS_HFCLKBLKUPD_FALSE       ((uint32_t)0x00000000U)         /* !< Writes to HFCLKCLKCFG are allowed */
#define SYSCTL_CLKSTATUS_HFCLKBLKUPD_TRUE        ((uint32_t)0x10000000U)         /* !< Writes to HFCLKCLKCFG are blocked */
/* SYSCTL_CLKSTATUS[HSCLKGOOD] Bits */
#define SYSCTL_CLKSTATUS_HSCLKGOOD_OFS           (21)                            /* !< HSCLKGOOD Offset */
#define SYSCTL_CLKSTATUS_HSCLKGOOD_MASK          ((uint32_t)0x00200000U)         /* !< HSCLKGOOD is set by hardware if the
                                                                                    selected clock source for HSCLK
                                                                                    started successfully. */
#define SYSCTL_CLKSTATUS_HSCLKGOOD_FALSE         ((uint32_t)0x00000000U)         /* !< The HSCLK source did not start
                                                                                    correctly */
#define SYSCTL_CLKSTATUS_HSCLKGOOD_TRUE          ((uint32_t)0x00200000U)         /* !< The HSCLK source started correctly */
/* SYSCTL_CLKSTATUS[SYSPLLGOOD] Bits */
#define SYSCTL_CLKSTATUS_SYSPLLGOOD_OFS          (9)                             /* !< SYSPLLGOOD Offset */
#define SYSCTL_CLKSTATUS_SYSPLLGOOD_MASK         ((uint32_t)0x00000200U)         /* !< SYSPLLGOOD indicates if the SYSPLL
                                                                                    started correctly.  When the SYSPLL
                                                                                    is started, SYSPLLGOOD is cleared by
                                                                                    hardware.  After the startup settling
                                                                                    time has expired, the SYSPLL status
                                                                                    is tested.  If the SYSPLL started
                                                                                    successfully the SYSPLLGOOD bit is
                                                                                    set, else it is left cleared. */
#define SYSCTL_CLKSTATUS_SYSPLLGOOD_FALSE        ((uint32_t)0x00000000U)         /* !< SYSPLL did not start correctly */
#define SYSCTL_CLKSTATUS_SYSPLLGOOD_TRUE         ((uint32_t)0x00000200U)         /* !< SYSPLL started correctly */
/* SYSCTL_CLKSTATUS[ANACLKERR] Bits */
#define SYSCTL_CLKSTATUS_ANACLKERR_OFS           (31)                            /* !< ANACLKERR Offset */
#define SYSCTL_CLKSTATUS_ANACLKERR_MASK          ((uint32_t)0x80000000U)         /* !< ANACLKERR is set when the device
                                                                                    clock configuration does not support
                                                                                    an enabled analog peripheral mode and
                                                                                    the analog peripheral may not be
                                                                                    functioning as expected. */
#define SYSCTL_CLKSTATUS_ANACLKERR_FALSE         ((uint32_t)0x00000000U)         /* !< No analog clock errors detected */
#define SYSCTL_CLKSTATUS_ANACLKERR_TRUE          ((uint32_t)0x80000000U)         /* !< Analog clock error detected */
/* SYSCTL_CLKSTATUS[HSCLKMUX] Bits */
#define SYSCTL_CLKSTATUS_HSCLKMUX_OFS            (4)                             /* !< HSCLKMUX Offset */
#define SYSCTL_CLKSTATUS_HSCLKMUX_MASK           ((uint32_t)0x00000010U)         /* !< HSCLKMUX indicates if MCLK is
                                                                                    currently sourced from the high-speed
                                                                                    clock (HSCLK). */
#define SYSCTL_CLKSTATUS_HSCLKMUX_SYSOSC         ((uint32_t)0x00000000U)         /* !< MCLK is not sourced from HSCLK */
#define SYSCTL_CLKSTATUS_HSCLKMUX_HSCLK          ((uint32_t)0x00000010U)         /* !< MCLK is sourced from HSCLK */
/* SYSCTL_CLKSTATUS[LFCLKMUX] Bits */
#define SYSCTL_CLKSTATUS_LFCLKMUX_OFS            (6)                             /* !< LFCLKMUX Offset */
#define SYSCTL_CLKSTATUS_LFCLKMUX_MASK           ((uint32_t)0x000000C0U)         /* !< LFCLKMUX indicates if LFCLK is
                                                                                    sourced from the internal LFOSC, the
                                                                                    low frequency crystal (LFXT), or the
                                                                                    LFCLK_IN digital clock input. */
#define SYSCTL_CLKSTATUS_LFCLKMUX_LFOSC          ((uint32_t)0x00000000U)         /* !< LFCLK is sourced from the internal
                                                                                    LFOSC */
#define SYSCTL_CLKSTATUS_LFCLKMUX_LFXT           ((uint32_t)0x00000040U)         /* !< LFCLK is sourced from the LFXT
                                                                                    (crystal) */
#define SYSCTL_CLKSTATUS_LFCLKMUX_EXLF           ((uint32_t)0x00000080U)         /* !< LFCLK is sourced from LFCLK_IN
                                                                                    (external digital clock  input) */
/* SYSCTL_CLKSTATUS[SYSOSCFREQ] Bits */
#define SYSCTL_CLKSTATUS_SYSOSCFREQ_OFS          (0)                             /* !< SYSOSCFREQ Offset */
#define SYSCTL_CLKSTATUS_SYSOSCFREQ_MASK         ((uint32_t)0x00000003U)         /* !< SYSOSCFREQ indicates the current
                                                                                    SYSOSC operating frequency. */
#define SYSCTL_CLKSTATUS_SYSOSCFREQ_SYSOSC32M    ((uint32_t)0x00000000U)         /* !< SYSOSC is at base frequency (32MHz) */
#define SYSCTL_CLKSTATUS_SYSOSCFREQ_SYSOSC4M     ((uint32_t)0x00000001U)         /* !< SYSOSC is at low frequency (4MHz) */
#define SYSCTL_CLKSTATUS_SYSOSCFREQ_SYSOSCUSER   ((uint32_t)0x00000002U)         /* !< SYSOSC is at the user-trimmed
                                                                                    frequency (16 or 24MHz) */
#define SYSCTL_CLKSTATUS_SYSOSCFREQ_SYSOSCTURBO  ((uint32_t)0x00000003U)         /* !< Reserved */
/* SYSCTL_CLKSTATUS[LFXTGOOD] Bits */
#define SYSCTL_CLKSTATUS_LFXTGOOD_OFS            (10)                            /* !< LFXTGOOD Offset */
#define SYSCTL_CLKSTATUS_LFXTGOOD_MASK           ((uint32_t)0x00000400U)         /* !< LFXTGOOD indicates if the LFXT
                                                                                    started correctly.  When the LFXT is
                                                                                    started, LFXTGOOD is cleared by
                                                                                    hardware.  After the startup settling
                                                                                    time has expired, the LFXT status is
                                                                                    tested.  If the LFXT started
                                                                                    successfully the LFXTGOOD bit is set,
                                                                                    else it is left cleared. */
#define SYSCTL_CLKSTATUS_LFXTGOOD_FALSE          ((uint32_t)0x00000000U)         /* !< LFXT did not start correctly */
#define SYSCTL_CLKSTATUS_LFXTGOOD_TRUE           ((uint32_t)0x00000400U)         /* !< LFXT started correctly */
/* SYSCTL_CLKSTATUS[HSCLKSOFF] Bits */
#define SYSCTL_CLKSTATUS_HSCLKSOFF_OFS           (12)                            /* !< HSCLKSOFF Offset */
#define SYSCTL_CLKSTATUS_HSCLKSOFF_MASK          ((uint32_t)0x00001000U)         /* !< HSCLKSOFF is set when the high
                                                                                    speed clock sources (SYSPLL, HFCLK)
                                                                                    are disabled or dead.  It is the
                                                                                    logical AND of HFCLKOFF and
                                                                                    SYSPLLOFF. */
#define SYSCTL_CLKSTATUS_HSCLKSOFF_FALSE         ((uint32_t)0x00000000U)         /* !< SYSPLL, HFCLK, or both were started
                                                                                    correctly and remain enabled */
#define SYSCTL_CLKSTATUS_HSCLKSOFF_TRUE          ((uint32_t)0x00001000U)         /* !< SYSPLL and HFCLK are both either
                                                                                    off or dead */
/* SYSCTL_CLKSTATUS[FCLMODE] Bits */
#define SYSCTL_CLKSTATUS_FCLMODE_OFS             (24)                            /* !< FCLMODE Offset */
#define SYSCTL_CLKSTATUS_FCLMODE_MASK            ((uint32_t)0x01000000U)         /* !< FCLMODE indicates if the SYSOSC
                                                                                    frequency correction loop (FCL) is
                                                                                    enabled. */
#define SYSCTL_CLKSTATUS_FCLMODE_DISABLED        ((uint32_t)0x00000000U)         /* !< SYSOSC FCL is disabled */
#define SYSCTL_CLKSTATUS_FCLMODE_ENABLED         ((uint32_t)0x01000000U)         /* !< SYSOSC FCL is enabled */
/* SYSCTL_CLKSTATUS[CURHSCLKSEL] Bits */
#define SYSCTL_CLKSTATUS_CURHSCLKSEL_OFS         (16)                            /* !< CURHSCLKSEL Offset */
#define SYSCTL_CLKSTATUS_CURHSCLKSEL_MASK        ((uint32_t)0x00010000U)         /* !< CURHSCLKSEL indicates the current
                                                                                    clock source for HSCLK. */
#define SYSCTL_CLKSTATUS_CURHSCLKSEL_SYSPLL      ((uint32_t)0x00000000U)         /* !< HSCLK is currently sourced from the
                                                                                    SYSPLL */
#define SYSCTL_CLKSTATUS_CURHSCLKSEL_HFCLK       ((uint32_t)0x00010000U)         /* !< HSCLK is currently sourced from the
                                                                                    HFCLK */
/* SYSCTL_CLKSTATUS[CURMCLKSEL] Bits */
#define SYSCTL_CLKSTATUS_CURMCLKSEL_OFS          (17)                            /* !< CURMCLKSEL Offset */
#define SYSCTL_CLKSTATUS_CURMCLKSEL_MASK         ((uint32_t)0x00020000U)         /* !< CURMCLKSEL indicates if MCLK is
                                                                                    currently sourced from LFCLK. */
#define SYSCTL_CLKSTATUS_CURMCLKSEL_NOTLFCLK     ((uint32_t)0x00000000U)         /* !< MCLK is not sourced from LFCLK */
#define SYSCTL_CLKSTATUS_CURMCLKSEL_LFCLK        ((uint32_t)0x00020000U)         /* !< MCLK is sourced from LFCLK */
/* SYSCTL_CLKSTATUS[FCCDONE] Bits */
#define SYSCTL_CLKSTATUS_FCCDONE_OFS             (25)                            /* !< FCCDONE Offset */
#define SYSCTL_CLKSTATUS_FCCDONE_MASK            ((uint32_t)0x02000000U)         /* !< FCCDONE indicates when a frequency
                                                                                    clock counter capture is complete. */
#define SYSCTL_CLKSTATUS_FCCDONE_NOTDONE         ((uint32_t)0x00000000U)         /* !< FCC capture is not done */
#define SYSCTL_CLKSTATUS_FCCDONE_DONE            ((uint32_t)0x02000000U)         /* !< FCC capture is done */
/* SYSCTL_CLKSTATUS[SYSPLLOFF] Bits */
#define SYSCTL_CLKSTATUS_SYSPLLOFF_OFS           (14)                            /* !< SYSPLLOFF Offset */
#define SYSCTL_CLKSTATUS_SYSPLLOFF_MASK          ((uint32_t)0x00004000U)         /* !< SYSPLLOFF indicates if the SYSPLL
                                                                                    is disabled or was dead at startup.
                                                                                    When the SYSPLL is started, SYSPLLOFF
                                                                                    is cleared by hardware.  Following
                                                                                    startup of the SYSPLL, if the SYSPLL
                                                                                    startup monitor determines that the
                                                                                    SYSPLL was not started correctly,
                                                                                    SYSPLLOFF is set. */
#define SYSCTL_CLKSTATUS_SYSPLLOFF_FALSE         ((uint32_t)0x00000000U)         /* !< SYSPLL started correctly and is
                                                                                    enabled */
#define SYSCTL_CLKSTATUS_SYSPLLOFF_TRUE          ((uint32_t)0x00004000U)         /* !< SYSPLL is disabled or was dead
                                                                                    startup */
/* SYSCTL_CLKSTATUS[LFCLKFAIL] Bits */
#define SYSCTL_CLKSTATUS_LFCLKFAIL_OFS           (23)                            /* !< LFCLKFAIL Offset */
#define SYSCTL_CLKSTATUS_LFCLKFAIL_MASK          ((uint32_t)0x00800000U)         /* !< LFCLKFAIL indicates when the
                                                                                    continous LFCLK monitor detects a
                                                                                    LFXT or LFCLK_IN clock stuck failure. */
#define SYSCTL_CLKSTATUS_LFCLKFAIL_FALSE         ((uint32_t)0x00000000U)         /* !< No LFCLK fault detected */
#define SYSCTL_CLKSTATUS_LFCLKFAIL_TRUE          ((uint32_t)0x00800000U)         /* !< LFCLK stuck fault detected */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSSTATUS (系统状态寄存器，只读)
 * 指示Flash ECC错误、BOR状态、SWD/MCAN就绪、PMU/模拟泵状态等
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSSTATUS[SHDNIOLOCK] — SHUTDOWN IO锁定 [14] */
#define SYSCTL_SYSSTATUS_SHDNIOLOCK_OFS          (14)                            /* !< SHDNIOLOCK Offset */
#define SYSCTL_SYSSTATUS_SHDNIOLOCK_MASK         ((uint32_t)0x00004000U)         /* !< SHDNIOLOCK indicates when IO is
                                                                                    locked due to SHUTDOWN */
#define SYSCTL_SYSSTATUS_SHDNIOLOCK_FALSE        ((uint32_t)0x00000000U)         /* !< IO IS NOT Locked due to SHUTDOWN */
#define SYSCTL_SYSSTATUS_SHDNIOLOCK_TRUE         ((uint32_t)0x00004000U)         /* !< IO IS Locked due to SHUTDOWN */
/* SYSCTL_SYSSTATUS[EXTRSTPINDIS] Bits */
#define SYSCTL_SYSSTATUS_EXTRSTPINDIS_OFS        (12)                            /* !< EXTRSTPINDIS Offset */
#define SYSCTL_SYSSTATUS_EXTRSTPINDIS_MASK       ((uint32_t)0x00001000U)         /* !< EXTRSTPINDIS indicates when user
                                                                                    has disabled the use of external
                                                                                    reset pin */
#define SYSCTL_SYSSTATUS_EXTRSTPINDIS_FALSE      ((uint32_t)0x00000000U)         /* !< External Reset Pin Enabled */
#define SYSCTL_SYSSTATUS_EXTRSTPINDIS_TRUE       ((uint32_t)0x00001000U)         /* !< External Reset Pin Disabled */
/* SYSCTL_SYSSTATUS[PMUIREFGOOD] Bits */
#define SYSCTL_SYSSTATUS_PMUIREFGOOD_OFS         (6)                             /* !< PMUIREFGOOD Offset */
#define SYSCTL_SYSSTATUS_PMUIREFGOOD_MASK        ((uint32_t)0x00000040U)         /* !< PMUIREFGOOD is set by hardware when
                                                                                    the PMU current reference is ready. */
#define SYSCTL_SYSSTATUS_PMUIREFGOOD_FALSE       ((uint32_t)0x00000000U)         /* !< IREF is not ready */
#define SYSCTL_SYSSTATUS_PMUIREFGOOD_TRUE        ((uint32_t)0x00000040U)         /* !< IREF is ready */
/* SYSCTL_SYSSTATUS[SWDCFGDIS] Bits */
#define SYSCTL_SYSSTATUS_SWDCFGDIS_OFS           (13)                            /* !< SWDCFGDIS Offset */
#define SYSCTL_SYSSTATUS_SWDCFGDIS_MASK          ((uint32_t)0x00002000U)         /* !< SWDCFGDIS indicates when user has
                                                                                    disabled the use of SWD Port */
#define SYSCTL_SYSSTATUS_SWDCFGDIS_FALSE         ((uint32_t)0x00000000U)         /* !< SWD Port Enabled */
#define SYSCTL_SYSSTATUS_SWDCFGDIS_TRUE          ((uint32_t)0x00002000U)         /* !< SWD Port Disabled */
/* SYSCTL_SYSSTATUS[ANACPUMPGOOD] Bits */
#define SYSCTL_SYSSTATUS_ANACPUMPGOOD_OFS        (5)                             /* !< ANACPUMPGOOD Offset */
#define SYSCTL_SYSSTATUS_ANACPUMPGOOD_MASK       ((uint32_t)0x00000020U)         /* !< ANACPUMPGOOD is set by hardware
                                                                                    when the VBOOST analog mux charge
                                                                                    pump is ready. */
#define SYSCTL_SYSSTATUS_ANACPUMPGOOD_FALSE      ((uint32_t)0x00000000U)         /* !< VBOOST is not ready */
#define SYSCTL_SYSSTATUS_ANACPUMPGOOD_TRUE       ((uint32_t)0x00000020U)         /* !< VBOOST is ready */
/* SYSCTL_SYSSTATUS[REBOOTATTEMPTS] Bits */
#define SYSCTL_SYSSTATUS_REBOOTATTEMPTS_OFS      (30)                            /* !< REBOOTATTEMPTS Offset */
#define SYSCTL_SYSSTATUS_REBOOTATTEMPTS_MASK     ((uint32_t)0xC0000000U)         /* !< REBOOTATTEMPTS indicates the number
                                                                                    of boot attempts taken before the
                                                                                    user application starts. */
/* SYSCTL_SYSSTATUS[BORLVL] Bits */
#define SYSCTL_SYSSTATUS_BORLVL_OFS              (4)                             /* !< BORLVL Offset */
#define SYSCTL_SYSSTATUS_BORLVL_MASK             ((uint32_t)0x00000010U)         /* !< BORLVL indicates if a BOR event
                                                                                    occured and the BOR threshold was
                                                                                    switched to BOR0 by hardware. */
#define SYSCTL_SYSSTATUS_BORLVL_FALSE            ((uint32_t)0x00000000U)         /* !< No BOR violation occured */
#define SYSCTL_SYSSTATUS_BORLVL_TRUE             ((uint32_t)0x00000010U)         /* !< A BOR violation occured and the BOR
                                                                                    threshold was switched to BOR0 */
/* SYSCTL_SYSSTATUS[MCAN0READY] Bits */
#define SYSCTL_SYSSTATUS_MCAN0READY_OFS          (8)                             /* !< MCAN0READY Offset */
#define SYSCTL_SYSSTATUS_MCAN0READY_MASK         ((uint32_t)0x00000100U)         /* !< MCAN0READY indicates when the MCAN0
                                                                                    peripheral is ready. */
#define SYSCTL_SYSSTATUS_MCAN0READY_FALSE        ((uint32_t)0x00000000U)         /* !< MCAN0 is not ready */
#define SYSCTL_SYSSTATUS_MCAN0READY_TRUE         ((uint32_t)0x00000100U)         /* !< MCAN0 is ready */
/* SYSCTL_SYSSTATUS[FLASHDED] Bits */
#define SYSCTL_SYSSTATUS_FLASHDED_OFS            (0)                             /* !< FLASHDED Offset */
#define SYSCTL_SYSSTATUS_FLASHDED_MASK           ((uint32_t)0x00000001U)         /* !< FLASHDED indicates if a flash ECC
                                                                                    double bit error was detected (DED). */
#define SYSCTL_SYSSTATUS_FLASHDED_FALSE          ((uint32_t)0x00000000U)         /* !< No flash ECC double bit error
                                                                                    detected */
#define SYSCTL_SYSSTATUS_FLASHDED_TRUE           ((uint32_t)0x00000001U)         /* !< Flash ECC double bit error detected */
/* SYSCTL_SYSSTATUS[FLASHSEC] Bits */
#define SYSCTL_SYSSTATUS_FLASHSEC_OFS            (1)                             /* !< FLASHSEC Offset */
#define SYSCTL_SYSSTATUS_FLASHSEC_MASK           ((uint32_t)0x00000002U)         /* !< FLASHSEC indicates if a flash ECC
                                                                                    single bit error was detected and
                                                                                    corrected (SEC). */
#define SYSCTL_SYSSTATUS_FLASHSEC_FALSE          ((uint32_t)0x00000000U)         /* !< No flash ECC single bit error
                                                                                    detected */
#define SYSCTL_SYSSTATUS_FLASHSEC_TRUE           ((uint32_t)0x00000002U)         /* !< Flash ECC single bit error was
                                                                                    detected and corrected */
/* SYSCTL_SYSSTATUS[BORCURTHRESHOLD] Bits */
#define SYSCTL_SYSSTATUS_BORCURTHRESHOLD_OFS     (2)                             /* !< BORCURTHRESHOLD Offset */
#define SYSCTL_SYSSTATUS_BORCURTHRESHOLD_MASK    ((uint32_t)0x0000000CU)         /* !< BORCURTHRESHOLD indicates the
                                                                                    active brown-out reset supply monitor
                                                                                    configuration. */
#define SYSCTL_SYSSTATUS_BORCURTHRESHOLD_BORMIN  ((uint32_t)0x00000000U)         /* !< Default minimum threshold; a BOR0-
                                                                                    violation triggers a BOR */
#define SYSCTL_SYSSTATUS_BORCURTHRESHOLD_BORLEVEL1 ((uint32_t)0x00000004U)         /* !< A BOR1- violation generates a
                                                                                    BORLVL interrupt */
#define SYSCTL_SYSSTATUS_BORCURTHRESHOLD_BORLEVEL2 ((uint32_t)0x00000008U)         /* !< A BOR2- violation generates a
                                                                                    BORLVL interrupt */
#define SYSCTL_SYSSTATUS_BORCURTHRESHOLD_BORLEVEL3 ((uint32_t)0x0000000CU)         /* !< A BOR3- violation generates a
                                                                                    BORLVL interrupt */

/*—————————————————————————————————————————————————————————————————————————————
 * DEDERRADDR (存储器双位错误地址寄存器，只读)
 * 记录最后一次存储器DED (双位错误检测) 的地址
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_DEDERRADDR[ADDR] — DED错误地址 [31:0] */
#define SYSCTL_DEDERRADDR_ADDR_OFS               (0)                             /* !< ADDR Offset */
#define SYSCTL_DEDERRADDR_ADDR_MASK              ((uint32_t)0xFFFFFFFFU)         /* !< 存储器DED错误地址 */

/*—————————————————————————————————————————————————————————————————————————————
 * RSTCAUSE (复位原因寄存器，读清)
 * 记录自上次读取以来的最低级别复位原因
 * 复位级别: POR > BOR > BOOTRST > SYSRST > CPURST > BSL
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_RSTCAUSE[ID] — 复位原因ID [4:0]，读清 */
#define SYSCTL_RSTCAUSE_ID_OFS                   (0)                             /* !< ID Offset */
#define SYSCTL_RSTCAUSE_ID_MASK                  ((uint32_t)0x0000001FU)         /* !< 复位原因ID，范围 0x00-0x1D */
#define SYSCTL_RSTCAUSE_ID_NORST                 ((uint32_t)0x00000000U)         /* !< 自上次读取以来无复位 */
#define SYSCTL_RSTCAUSE_ID_PORHWFAIL             ((uint32_t)0x00000001U)         /* !< POR- violation, SHUTDNSTOREx or PMU
                                                                                    trim parity fault */
#define SYSCTL_RSTCAUSE_ID_POREXNRST             ((uint32_t)0x00000002U)         /* !< NRST triggered POR (>1s hold) */
#define SYSCTL_RSTCAUSE_ID_PORSW                 ((uint32_t)0x00000003U)         /* !< Software triggered POR */
#define SYSCTL_RSTCAUSE_ID_BORSUPPLY             ((uint32_t)0x00000004U)         /* !< BOR0- violation */
#define SYSCTL_RSTCAUSE_ID_BORWAKESHUTDN         ((uint32_t)0x00000005U)         /* !< SHUTDOWN mode exit */
#define SYSCTL_RSTCAUSE_ID_BOOTNONPMUPARITY      ((uint32_t)0x00000008U)         /* !< Non-PMU trim parity fault */
#define SYSCTL_RSTCAUSE_ID_BOOTCLKFAIL           ((uint32_t)0x00000009U)         /* !< Fatal clock failure */
#define SYSCTL_RSTCAUSE_ID_BOOTSW                ((uint32_t)0x0000000DU)         /* !< Software triggered BOOTRST */
#define SYSCTL_RSTCAUSE_ID_BOOTEXNRST            ((uint32_t)0x0000000CU)         /* !< NRST triggered BOOTRST (<1s
                                                                                    hold) */
#define SYSCTL_RSTCAUSE_ID_SYSBSLEXIT            ((uint32_t)0x00000010U)         /* !< BSL exit */
#define SYSCTL_RSTCAUSE_ID_SYSBSLENTRY           ((uint32_t)0x00000011U)         /* !< BSL entry */
#define SYSCTL_RSTCAUSE_ID_SYSWWDT0              ((uint32_t)0x0000000EU)         /* !< WWDT0 violation */
#define SYSCTL_RSTCAUSE_ID_SYSWWDT1              ((uint32_t)0x00000013U)         /* !< WWDT1 violation */
#define SYSCTL_RSTCAUSE_ID_SYSFLASHECC           ((uint32_t)0x00000014U)         /* !< Flash uncorrectable ECC error */
#define SYSCTL_RSTCAUSE_ID_SYSCPULOCK            ((uint32_t)0x00000015U)         /* !< CPULOCK violation */
#define SYSCTL_RSTCAUSE_ID_SYSDBG                ((uint32_t)0x0000001AU)         /* !< Debug triggered SYSRST */
#define SYSCTL_RSTCAUSE_ID_SYSSW                 ((uint32_t)0x0000001BU)         /* !< Software triggered SYSRST */
#define SYSCTL_RSTCAUSE_ID_CPUDBG                ((uint32_t)0x0000001CU)         /* !< Debug triggered CPURST */
#define SYSCTL_RSTCAUSE_ID_CPUSW                 ((uint32_t)0x0000001DU)         /* !< Software triggered CPURST */

/*—————————————————————————————————————————————————————————————————————————————
 * RESETLEVEL (软件复位级别寄存器)
 * 配合RESETCMD使用，指定软件触发复位的类型
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_RESETLEVEL[LEVEL] — 复位级别 [2:0] */
#define SYSCTL_RESETLEVEL_LEVEL_OFS              (0)                             /* !< LEVEL Offset */
#define SYSCTL_RESETLEVEL_LEVEL_MASK             ((uint32_t)0x00000007U)         /* !< 软件复位类型选择，范围 0-4 */
#define SYSCTL_RESETLEVEL_LEVEL_CPU              ((uint32_t)0x00000000U)         /* !< SYSRST: CPU和外设复位 */
#define SYSCTL_RESETLEVEL_LEVEL_BOOT             ((uint32_t)0x00000001U)         /* !< BOOTRST: CPU、外设和引导配置复位 */
#define SYSCTL_RESETLEVEL_LEVEL_BOOTLOADERENTRY  ((uint32_t)0x00000002U)         /* !< SYSRST并进入BSL (引导加载程序) */
#define SYSCTL_RESETLEVEL_LEVEL_POR              ((uint32_t)0x00000003U)         /* !< POR: 上电复位 */
#define SYSCTL_RESETLEVEL_LEVEL_BOOTLOADEREXIT   ((uint32_t)0x00000004U)         /* !< SYSRST并退出BSL */

/*—————————————————————————————————————————————————————————————————————————————
 * RESETCMD (软件复位命令寄存器，只写)
 * 写入KEY (0xE4) 和GO (1) 触发RESETLEVEL指定的复位
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_RESETCMD[KEY] — 写入密钥 [31:24]，必须为 0xE4 */
#define SYSCTL_RESETCMD_KEY_OFS                  (24)                            /* !< KEY Offset */
#define SYSCTL_RESETCMD_KEY_MASK                 ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0xE4 (228) */
#define SYSCTL_RESETCMD_KEY_VALUE                ((uint32_t)0xE4000000U)         /* !< 复位密钥值 */
/* SYSCTL_RESETCMD[GO] Bits */
#define SYSCTL_RESETCMD_GO_OFS                   (0)                             /* !< GO Offset */
#define SYSCTL_RESETCMD_GO_MASK                  ((uint32_t)0x00000001U)         /* !< Execute the reset specified in
                                                                                    RESETLEVEL.LEVEL.  Must be written
                                                                                    together with the KEY. */
#define SYSCTL_RESETCMD_GO_TRUE                  ((uint32_t)0x00000001U)         /* !< Issue reset */

/*—————————————————————————————————————————————————————————————————————————————
 * BORTHRESHOLD (BOR阈值选择寄存器)
 * 配置欠压复位 (BOR) 阈值级别
 * BOR0 = 默认最低阈值 (触发复位)；BOR1-BOR3 = 更高阈值 (触发NMI)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_BORTHRESHOLD[LEVEL] — BOR阈值级别 [1:0] */
#define SYSCTL_BORTHRESHOLD_LEVEL_OFS            (0)                             /* !< LEVEL Offset */
#define SYSCTL_BORTHRESHOLD_LEVEL_MASK           ((uint32_t)0x00000003U)         /* !< BOR阈值级别，范围 0-3 */
#define SYSCTL_BORTHRESHOLD_LEVEL_BORMIN         ((uint32_t)0x00000000U)         /* !< 默认最低阈值: BOR0-触发复位 */
#define SYSCTL_BORTHRESHOLD_LEVEL_BORLEVEL1      ((uint32_t)0x00000001U)         /* !< BOR1-: 触发BORLVL中断 */
#define SYSCTL_BORTHRESHOLD_LEVEL_BORLEVEL2      ((uint32_t)0x00000002U)         /* !< BOR2-: 触发BORLVL中断 */
#define SYSCTL_BORTHRESHOLD_LEVEL_BORLEVEL3      ((uint32_t)0x00000003U)         /* !< BOR3-: 触发BORLVL中断 */

/*—————————————————————————————————————————————————————————————————————————————
 * BORCLRCMD (BOR清除命令寄存器，只写)
 * 清除BOR违规状态并尝试更改BOR阈值
 * 写入KEY (0xC7) 和GO (1)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_BORCLRCMD[KEY] — 写入密钥 [31:24]，必须为 0xC7 */
#define SYSCTL_BORCLRCMD_KEY_OFS                 (24)                            /* !< KEY Offset */
#define SYSCTL_BORCLRCMD_KEY_MASK                ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0xC7 (199) */
#define SYSCTL_BORCLRCMD_KEY_VALUE               ((uint32_t)0xC7000000U)         /* !< 清除密钥值 */
/* SYSCTL_BORCLRCMD[GO] Bits */
#define SYSCTL_BORCLRCMD_GO_OFS                  (0)                             /* !< GO Offset */
#define SYSCTL_BORCLRCMD_GO_MASK                 ((uint32_t)0x00000001U)         /* !< GO clears any prior BOR violation
                                                                                    status indications and attempts to
                                                                                    change the active BOR mode to that
                                                                                    specified in the LEVEL field of the
                                                                                    BORTHRESHOLD register. */
#define SYSCTL_BORCLRCMD_GO_TRUE                 ((uint32_t)0x00000001U)         /* !< Issue clear */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSOSCFCLCTL (SYSOSC频率校准环控制寄存器，只写)
 * 使能SYSOSC频率校准环 (FCL)，可选择使用外部电阻 (ROSC引脚)
 * 写入KEY (0x2A)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSOSCFCLCTL[KEY] — 写入密钥 [31:24]，必须为 0x2A */
#define SYSCTL_SYSOSCFCLCTL_KEY_OFS              (24)                            /* !< KEY Offset */
#define SYSCTL_SYSOSCFCLCTL_KEY_MASK             ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x2A (42) */
#define SYSCTL_SYSOSCFCLCTL_KEY_VALUE            ((uint32_t)0x2A000000U)         /* !< 命令密钥值 */
/* SYSCTL_SYSOSCFCLCTL[SETUSEFCL] Bits */
#define SYSCTL_SYSOSCFCLCTL_SETUSEFCL_OFS        (0)                             /* !< SETUSEFCL Offset */
#define SYSCTL_SYSOSCFCLCTL_SETUSEFCL_MASK       ((uint32_t)0x00000001U)         /* !< Set SETUSEFCL to enable the
                                                                                    frequency correction loop in SYSOSC.
                                                                                    Once enabled, this state is locked
                                                                                    until the next BOOTRST. */
#define SYSCTL_SYSOSCFCLCTL_SETUSEFCL_TRUE       ((uint32_t)0x00000001U)         /* !< Enable the SYSOSC FCL */
/* SYSCTL_SYSOSCFCLCTL[SETUSEEXRES] Bits */
#define SYSCTL_SYSOSCFCLCTL_SETUSEEXRES_OFS      (1)                             /* !< SETUSEEXRES Offset */
#define SYSCTL_SYSOSCFCLCTL_SETUSEEXRES_MASK     ((uint32_t)0x00000002U)         /* !< Set SETUSEEXRES to specify that an
                                                                                    external resistor will be used for
                                                                                    the FCL.  An appropriate resistor
                                                                                    must be populated on the ROSC pin.
                                                                                    This state is locked until the next
                                                                                    BOOTRST. */
#define SYSCTL_SYSOSCFCLCTL_SETUSEEXRES_TRUE     ((uint32_t)0x00000002U)         /* !< Enable the SYSOSC external Resistor */

/*—————————————————————————————————————————————————————————————————————————————
 * LFXTCTL (LFXT控制寄存器，只写)
 * 启动LFXT低频晶振和切换LFCLK到LFXT
 * 写入KEY (0x91)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_LFXTCTL[KEY] — 写入密钥 [31:24]，必须为 0x91 */
#define SYSCTL_LFXTCTL_KEY_OFS                   (24)                            /* !< KEY Offset */
#define SYSCTL_LFXTCTL_KEY_MASK                  ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x91 (145) */
#define SYSCTL_LFXTCTL_KEY_VALUE                 ((uint32_t)0x91000000U)         /* !< 命令密钥值 */
/* SYSCTL_LFXTCTL[SETUSELFXT] Bits */
#define SYSCTL_LFXTCTL_SETUSELFXT_OFS            (1)                             /* !< SETUSELFXT Offset */
#define SYSCTL_LFXTCTL_SETUSELFXT_MASK           ((uint32_t)0x00000002U)         /* !< Set SETUSELFXT to switch LFCLK to
                                                                                    LFXT.  Once set, SETUSELFXT remains
                                                                                    set until the next BOOTRST. */
#define SYSCTL_LFXTCTL_SETUSELFXT_FALSE          ((uint32_t)0x00000000U)
#define SYSCTL_LFXTCTL_SETUSELFXT_TRUE           ((uint32_t)0x00000002U)         /* !< Use LFXT as the LFCLK source */
/* SYSCTL_LFXTCTL[STARTLFXT] Bits */
#define SYSCTL_LFXTCTL_STARTLFXT_OFS             (0)                             /* !< STARTLFXT Offset */
#define SYSCTL_LFXTCTL_STARTLFXT_MASK            ((uint32_t)0x00000001U)         /* !< Set STARTLFXT to start the low
                                                                                    frequency crystal oscillator (LFXT).
                                                                                    Once set, STARTLFXT remains set until
                                                                                    the next BOOTRST. */
#define SYSCTL_LFXTCTL_STARTLFXT_FALSE           ((uint32_t)0x00000000U)         /* !< LFXT not started */
#define SYSCTL_LFXTCTL_STARTLFXT_TRUE            ((uint32_t)0x00000001U)         /* !< Start LFXT */

/*—————————————————————————————————————————————————————————————————————————————
 * EXLFCTL (外部LFCLK控制寄存器，只写)
 * 切换LFCLK到LFCLK_IN外部数字时钟输入
 * 写入KEY (0x36)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_EXLFCTL[KEY] — 写入密钥 [31:24]，必须为 0x36 */
#define SYSCTL_EXLFCTL_KEY_OFS                   (24)                            /* !< KEY Offset */
#define SYSCTL_EXLFCTL_KEY_MASK                  ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x36 (54) */
#define SYSCTL_EXLFCTL_KEY_VALUE                 ((uint32_t)0x36000000U)         /* !< 命令密钥值 */
/* SYSCTL_EXLFCTL[SETUSEEXLF] Bits */
#define SYSCTL_EXLFCTL_SETUSEEXLF_OFS            (0)                             /* !< SETUSEEXLF Offset */
#define SYSCTL_EXLFCTL_SETUSEEXLF_MASK           ((uint32_t)0x00000001U)         /* !< Set SETUSEEXLF to switch LFCLK to
                                                                                    the LFCLK_IN digital clock input.
                                                                                    Once set, SETUSEEXLF remains set
                                                                                    until the next BOOTRST. */
#define SYSCTL_EXLFCTL_SETUSEEXLF_TRUE           ((uint32_t)0x00000001U)         /* !< Use LFCLK_IN as the LFCLK source */

/*—————————————————————————————————————————————————————————————————————————————
 * SHDNIOREL (SHUTDOWN IO释放控制寄存器，只写)
 * SHUTDOWN模式退出后释放IO
 * 写入KEY (0x91)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SHDNIOREL[KEY] — 写入密钥 [31:24]，必须为 0x91 */
#define SYSCTL_SHDNIOREL_KEY_OFS                 (24)                            /* !< KEY Offset */
#define SYSCTL_SHDNIOREL_KEY_MASK                ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x91 */
#define SYSCTL_SHDNIOREL_KEY_VALUE               ((uint32_t)0x91000000U)         /* !< 命令密钥值 */
/* SYSCTL_SHDNIOREL[RELEASE] Bits */
#define SYSCTL_SHDNIOREL_RELEASE_OFS             (0)                             /* !< RELEASE Offset */
#define SYSCTL_SHDNIOREL_RELEASE_MASK            ((uint32_t)0x00000001U)         /* !< Set RELEASE to release the IO after
                                                                                    a SHUTDOWN mode exit. */
#define SYSCTL_SHDNIOREL_RELEASE_TRUE            ((uint32_t)0x00000001U)         /* !< Release IO */

/*—————————————————————————————————————————————————————————————————————————————
 * EXRSTPIN (NRST引脚复位功能禁用寄存器，只写)
 * 禁用NRST引脚的复位功能，锁定至下次POR
 * 写入KEY (0x1E)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_EXRSTPIN[KEY] — 写入密钥 [31:24]，必须为 0x1E */
#define SYSCTL_EXRSTPIN_KEY_OFS                  (24)                            /* !< KEY Offset */
#define SYSCTL_EXRSTPIN_KEY_MASK                 ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x1E */
#define SYSCTL_EXRSTPIN_KEY_VALUE                ((uint32_t)0x1E000000U)         /* !< 命令密钥值 */
/* SYSCTL_EXRSTPIN[DISABLE] Bits */
#define SYSCTL_EXRSTPIN_DISABLE_OFS              (0)                             /* !< DISABLE Offset */
#define SYSCTL_EXRSTPIN_DISABLE_MASK             ((uint32_t)0x00000001U)         /* !< Set DISABLE to disable the reset
                                                                                    function of the NRST pin.  Once set,
                                                                                    this configuration is locked until
                                                                                    the next POR. */
#define SYSCTL_EXRSTPIN_DISABLE_FALSE            ((uint32_t)0x00000000U)         /* !< Reset function of NRST pin is
                                                                                    enabled */
#define SYSCTL_EXRSTPIN_DISABLE_TRUE             ((uint32_t)0x00000001U)         /* !< Reset function of NRST pin is
                                                                                    disabled */

/*—————————————————————————————————————————————————————————————————————————————
 * SYSSTATUSCLR (系统状态清除寄存器，只写)
 * 清除SYSSTATUS中的ECC粘滞位
 * 写入KEY (0xCE)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SYSSTATUSCLR[KEY] — 写入密钥 [31:24]，必须为 0xCE */
#define SYSCTL_SYSSTATUSCLR_KEY_OFS              (24)                            /* !< KEY Offset */
#define SYSCTL_SYSSTATUSCLR_KEY_MASK             ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0xCE (206) */
#define SYSCTL_SYSSTATUSCLR_KEY_VALUE            ((uint32_t)0xCE000000U)         /* !< 命令密钥值 */
/* SYSCTL_SYSSTATUSCLR[ALLECC] Bits */
#define SYSCTL_SYSSTATUSCLR_ALLECC_OFS           (0)                             /* !< ALLECC Offset */
#define SYSCTL_SYSSTATUSCLR_ALLECC_MASK          ((uint32_t)0x00000001U)         /* !< Set ALLECC to clear all ECC related
                                                                                    SYSSTATUS indicators. */
#define SYSCTL_SYSSTATUSCLR_ALLECC_CLEAR         ((uint32_t)0x00000001U)         /* !< Clear ECC error state */

/*—————————————————————————————————————————————————————————————————————————————
 * SWDCFG (SWD功能禁用寄存器，只写)
 * 禁用SWD引脚上的SWD功能，使其可用作GPIO
 * 写入KEY (0x62)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SWDCFG[KEY] — 写入密钥 [31:24]，必须为 0x62 */
#define SYSCTL_SWDCFG_KEY_OFS                    (24)                            /* !< KEY Offset */
#define SYSCTL_SWDCFG_KEY_MASK                   ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x62 (98) */
#define SYSCTL_SWDCFG_KEY_VALUE                  ((uint32_t)0x62000000U)         /* !< 命令密钥值 */
/* SYSCTL_SWDCFG[DISABLE] Bits */
#define SYSCTL_SWDCFG_DISABLE_OFS                (0)                             /* !< DISABLE Offset */
#define SYSCTL_SWDCFG_DISABLE_MASK               ((uint32_t)0x00000001U)         /* !< Set DISABLE to disable the SWD
                                                                                    function on SWD pins, allowing the
                                                                                    SWD pins to be used as GPIO. */
#define SYSCTL_SWDCFG_DISABLE_TRUE               ((uint32_t)0x00000001U)         /* !< Disable SWD function on SWD pins */

/*—————————————————————————————————————————————————————————————————————————————
 * FCCCMD (频率时钟计数器捕获命令寄存器，只写)
 * 启动FCC频率捕获
 * 写入KEY (0x0E) 和GO (1)
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_FCCCMD[KEY] — 写入密钥 [31:24]，必须为 0x0E */
#define SYSCTL_FCCCMD_KEY_OFS                    (24)                            /* !< KEY Offset */
#define SYSCTL_FCCCMD_KEY_MASK                   ((uint32_t)0xFF000000U)         /* !< 写入密钥，必须为 0x0E (14) */
#define SYSCTL_FCCCMD_KEY_VALUE                  ((uint32_t)0x0E000000U)         /* !< 命令密钥值 */
/* SYSCTL_FCCCMD[GO] Bits */
#define SYSCTL_FCCCMD_GO_OFS                     (0)                             /* !< GO Offset */
#define SYSCTL_FCCCMD_GO_MASK                    ((uint32_t)0x00000001U)         /* !< Set GO to start a capture with the
                                                                                    frequency clock counter (FCC). */
#define SYSCTL_FCCCMD_GO_TRUE                    ((uint32_t)0x00000001U)

/*—————————————————————————————————————————————————————————————————————————————
 * PMUOPAMP (GPAMP通用运放控制寄存器)
 * 配置GPAMP: 使能、输入通道选择、轨到轨模式、斩波模式、输出使能
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_PMUOPAMP[RRI] — 轨到轨输入模式 [5:4] */
#define SYSCTL_PMUOPAMP_RRI_OFS                  (4)                             /* !< RRI Offset */
#define SYSCTL_PMUOPAMP_RRI_MASK                 ((uint32_t)0x00000030U)         /* !< 轨到轨输入模式选择，范围 0-3 */
#define SYSCTL_PMUOPAMP_RRI_MODE0                ((uint32_t)0x00000000U)         /* !< PMOS input pairs */
#define SYSCTL_PMUOPAMP_RRI_MODE1                ((uint32_t)0x00000010U)         /* !< NMOS input pairs */
#define SYSCTL_PMUOPAMP_RRI_MODE2                ((uint32_t)0x00000020U)         /* !< Rail-to-rail mode */
#define SYSCTL_PMUOPAMP_RRI_MODE3                ((uint32_t)0x00000030U)         /* !< Rail-to-rail mode */
/* SYSCTL_PMUOPAMP[NSEL] Bits */
#define SYSCTL_PMUOPAMP_NSEL_OFS                 (2)                             /* !< NSEL Offset */
#define SYSCTL_PMUOPAMP_NSEL_MASK                ((uint32_t)0x0000000CU)         /* !< NSEL selects the GPAMP negative
                                                                                    channel input. */
#define SYSCTL_PMUOPAMP_NSEL_SEL0                ((uint32_t)0x00000000U)         /* !< GPAMP_OUT pin connected to negative
                                                                                    channel */
#define SYSCTL_PMUOPAMP_NSEL_SEL1                ((uint32_t)0x00000004U)         /* !< GPAMP_IN- pin connected to negative
                                                                                    channel */
#define SYSCTL_PMUOPAMP_NSEL_SEL2                ((uint32_t)0x00000008U)         /* !< GPAMP_OUT signal connected to
                                                                                    negative channel */
#define SYSCTL_PMUOPAMP_NSEL_SEL3                ((uint32_t)0x0000000CU)         /* !< No channel selected */
/* SYSCTL_PMUOPAMP[CHOPCLKMODE] Bits */
#define SYSCTL_PMUOPAMP_CHOPCLKMODE_OFS          (10)                            /* !< CHOPCLKMODE Offset */
#define SYSCTL_PMUOPAMP_CHOPCLKMODE_MASK         ((uint32_t)0x00000C00U)         /* !< CHOPCLKMODE selects the GPAMP
                                                                                    chopping mode. */
#define SYSCTL_PMUOPAMP_CHOPCLKMODE_CHOPDISABLED ((uint32_t)0x00000000U)         /* !< Chopping disabled */
#define SYSCTL_PMUOPAMP_CHOPCLKMODE_REGCHOP      ((uint32_t)0x00000400U)         /* !< Normal chopping */
#define SYSCTL_PMUOPAMP_CHOPCLKMODE_ADCASSIST    ((uint32_t)0x00000800U)         /* !< ADC Assisted chopping */
/* SYSCTL_PMUOPAMP[OUTENABLE] Bits */
#define SYSCTL_PMUOPAMP_OUTENABLE_OFS            (6)                             /* !< OUTENABLE Offset */
#define SYSCTL_PMUOPAMP_OUTENABLE_MASK           ((uint32_t)0x00000040U)         /* !< Set OUTENABLE to connect the GPAMP
                                                                                    output signal to the GPAMP_OUT pin */
#define SYSCTL_PMUOPAMP_OUTENABLE_FALSE          ((uint32_t)0x00000000U)         /* !< GPAMP_OUT signal is not connected
                                                                                    to the GPAMP_OUT pin */
#define SYSCTL_PMUOPAMP_OUTENABLE_TRUE           ((uint32_t)0x00000040U)         /* !< GPAMP_OUT signal is connected to
                                                                                    the GPAMP_OUT pin */
/* SYSCTL_PMUOPAMP[ENABLE] Bits */
#define SYSCTL_PMUOPAMP_ENABLE_OFS               (0)                             /* !< ENABLE Offset */
#define SYSCTL_PMUOPAMP_ENABLE_MASK              ((uint32_t)0x00000001U)         /* !< Set ENABLE to turn on the GPAMP. */
#define SYSCTL_PMUOPAMP_ENABLE_FALSE             ((uint32_t)0x00000000U)         /* !< GPAMP is disabled */
#define SYSCTL_PMUOPAMP_ENABLE_TRUE              ((uint32_t)0x00000001U)         /* !< GPAMP is enabled */
/* SYSCTL_PMUOPAMP[CHOPCLKFREQ] Bits */
#define SYSCTL_PMUOPAMP_CHOPCLKFREQ_OFS          (8)                             /* !< CHOPCLKFREQ Offset */
#define SYSCTL_PMUOPAMP_CHOPCLKFREQ_MASK         ((uint32_t)0x00000300U)         /* !< CHOPCLKFREQ selects the GPAMP
                                                                                    chopping clock frequency */
#define SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK16KHZ     ((uint32_t)0x00000000U)         /* !< 16kHz */
#define SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK8KHZ      ((uint32_t)0x00000100U)         /* !< 8kHz */
#define SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK4KHZ      ((uint32_t)0x00000200U)         /* !< 4kHz */
#define SYSCTL_PMUOPAMP_CHOPCLKFREQ_CLK2KHZ      ((uint32_t)0x00000300U)         /* !< 2kHz */
/* SYSCTL_PMUOPAMP[PCHENABLE] Bits */
#define SYSCTL_PMUOPAMP_PCHENABLE_OFS            (1)                             /* !< PCHENABLE Offset */
#define SYSCTL_PMUOPAMP_PCHENABLE_MASK           ((uint32_t)0x00000002U)         /* !< Set PCHENABLE to enable the
                                                                                    positive channel input. */
#define SYSCTL_PMUOPAMP_PCHENABLE_FALSE          ((uint32_t)0x00000000U)         /* !< Positive channel disabled */
#define SYSCTL_PMUOPAMP_PCHENABLE_TRUE           ((uint32_t)0x00000002U)         /* !< GPAMP_IN+ connected to positive
                                                                                    channel */

/*—————————————————————————————————————————————————————————————————————————————
 * SHUTDNSTORE0-3 (SHUTDOWN数据保持寄存器)
 * SHUTDOWN模式下保持的4字节数据，唤醒后可读取
 *—————————————————————————————————————————————————————————————————————————————*/
/* SYSCTL_SHUTDNSTORE0[DATA] — SHUTDOWN数据字节0 [7:0] */
#define SYSCTL_SHUTDNSTORE0_DATA_OFS             (0)                             /* !< DATA Offset */
#define SYSCTL_SHUTDNSTORE0_DATA_MASK            ((uint32_t)0x000000FFU)         /* !< SHUTDOWN数据保持字节0 */

/* SYSCTL_SHUTDNSTORE1 Bits */
/* SYSCTL_SHUTDNSTORE1[DATA] Bits */
#define SYSCTL_SHUTDNSTORE1_DATA_OFS             (0)                             /* !< DATA Offset */
#define SYSCTL_SHUTDNSTORE1_DATA_MASK            ((uint32_t)0x000000FFU)         /* !< Shutdown storage byte 1 */

/* SYSCTL_SHUTDNSTORE2 Bits */
/* SYSCTL_SHUTDNSTORE2[DATA] Bits */
#define SYSCTL_SHUTDNSTORE2_DATA_OFS             (0)                             /* !< DATA Offset */
#define SYSCTL_SHUTDNSTORE2_DATA_MASK            ((uint32_t)0x000000FFU)         /* !< Shutdown storage byte 2 */

/* SYSCTL_SHUTDNSTORE3 Bits */
/* SYSCTL_SHUTDNSTORE3[DATA] Bits */
#define SYSCTL_SHUTDNSTORE3_DATA_OFS             (0)                             /* !< DATA Offset */
#define SYSCTL_SHUTDNSTORE3_DATA_MASK            ((uint32_t)0x000000FFU)         /* !< Shutdown storage byte 3 */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_m0p_hw_sysctl_mspm0g1x0x_g3x0x__include */
