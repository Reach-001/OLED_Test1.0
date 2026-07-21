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
 * @file hw_unicomm.h
 * @brief UNICOMM 通用通信接口外设寄存器定义
 *
 * UNICOMM 是 MSPM0G3507 中 UART/SPI/I2C 共用的底层通信控制器。
 * 通过 IPMODE 寄存器选择具体协议模式。
 * 包含 GPRCM（电源/复位/时钟）和安全 DMA 地址范围配置。
 */

#ifndef ti_devices_msp_peripherals_hw_unicomm__include
#define ti_devices_msp_peripherals_hw_unicomm__include

/* Filename: hw_unicomm.h */
/* Revised: 2024-07-09 09:20:22 */
/* Revision: 0a8f209063d706f1d2624ae41af7d5bd8aec6cce */

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
* UNICOMM Registers
******************************************************************************/
/* GPRCM 寄存器组基地址偏移 */
#define UNICOMM_GPRCM_OFS                        ((uint32_t)0x00000800U)


/** @addtogroup UNICOMM_GPRCM
  @{
*/

/* GPRCM：电源控制、复位控制、时钟配置、状态寄存器 */
typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) 电源使能寄存器 */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) 复位控制寄存器 */
  __IO uint32_t CLKCFG;                            /* !< (@ 0x00000808) 外设时钟配置寄存器 */
       uint32_t RESERVED0[2];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) 状态寄存器 */
} UNICOMM_GPRCM_Regs;

/*@}*/ /* end of group UNICOMM_GPRCM */

/** @addtogroup UNICOMM
  @{
*/

/* UNICOMM 主寄存器结构体 */
typedef struct {
       uint32_t RESERVED0[512];
  UNICOMM_GPRCM_Regs  GPRCM;                             /* !< (@ 0x00000800) GPRCM 寄存器组 */
       uint32_t RESERVED1[570];
  __IO uint32_t IPMODE;                            /* !< (@ 0x00001100) 协议模式选择寄存器 (UART/SPI/I2C) */
  __IO uint32_t BASEADDR;                          /* !< (@ 0x00001104) DMA 安全访问基地址配置 */
  __IO uint32_t RANGE;                             /* !< (@ 0x00001108) DMA 安全访问地址范围 */
} UNICOMM_Regs;

/*@}*/ /* end of group UNICOMM */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* UNICOMM Register Control Bits
* UNICOMM 寄存器位域定义
******************************************************************************/

/*--- 电源使能寄存器 (PWREN) 位域 ---*/
/* UNICOMM_PWREN Bits */
/* UNICOMM_PWREN[ENABLE] Bits */
#define UNICOMM_PWREN_ENABLE_OFS                 (0)                             /* !< ENABLE Offset */
#define UNICOMM_PWREN_ENABLE_MASK                ((uint32_t)0x00000001U)         /* !< Enable the power */
#define UNICOMM_PWREN_ENABLE_DISABLE             ((uint32_t)0x00000000U)         /* !< Disable Power */
#define UNICOMM_PWREN_ENABLE_ENABLE              ((uint32_t)0x00000001U)         /* !< Enable Power */
/* UNICOMM_PWREN[KEY] Bits */
#define UNICOMM_PWREN_KEY_OFS                    (24)                            /* !< KEY Offset */
#define UNICOMM_PWREN_KEY_MASK                   ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define UNICOMM_PWREN_KEY_UNLOCK_W               ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/*--- 复位控制寄存器 (RSTCTL) 位域 ---*/
/* UNICOMM_RSTCTL Bits */
/* UNICOMM_RSTCTL[RESETSTKYCLR] Bits */
#define UNICOMM_RSTCTL_RESETSTKYCLR_OFS          (1)                             /* !< RESETSTKYCLR Offset */
#define UNICOMM_RSTCTL_RESETSTKYCLR_MASK         ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define UNICOMM_RSTCTL_RESETSTKYCLR_NOP          ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMM_RSTCTL_RESETSTKYCLR_CLR          ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* UNICOMM_RSTCTL[RESETASSERT] Bits */
#define UNICOMM_RSTCTL_RESETASSERT_OFS           (0)                             /* !< RESETASSERT Offset */
#define UNICOMM_RSTCTL_RESETASSERT_MASK          ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define UNICOMM_RSTCTL_RESETASSERT_NOP           ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMM_RSTCTL_RESETASSERT_ASSERT        ((uint32_t)0x00000001U)         /* !< Assert reset */
/* UNICOMM_RSTCTL[KEY] Bits */
#define UNICOMM_RSTCTL_KEY_OFS                   (24)                            /* !< KEY Offset */
#define UNICOMM_RSTCTL_KEY_MASK                  ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define UNICOMM_RSTCTL_KEY_UNLOCK_W              ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/*--- 时钟配置寄存器 (CLKCFG) 位域 ---*/
/* UNICOMM_CLKCFG Bits */
/* UNICOMM_CLKCFG[KEY] Bits */
#define UNICOMM_CLKCFG_KEY_OFS                   (24)                            /* !< KEY Offset */
#define UNICOMM_CLKCFG_KEY_MASK                  ((uint32_t)0xFF000000U)         /* !< KEY to Allow State Change -- 0xA9 */
#define UNICOMM_CLKCFG_KEY_UNLOCK                ((uint32_t)0xA9000000U)         /* !< key value to allow change field of
                                                                                    GPRCM */
/* UNICOMM_CLKCFG[BLOCKASYNC] Bits */
#define UNICOMM_CLKCFG_BLOCKASYNC_OFS            (8)                             /* !< BLOCKASYNC Offset */
#define UNICOMM_CLKCFG_BLOCKASYNC_MASK           ((uint32_t)0x00000100U)         /* !< Async Clock Request is blocked from
                                                                                    starting SYSOSC or forcing bus clock
                                                                                    to 32MHz */
#define UNICOMM_CLKCFG_BLOCKASYNC_DISABLE        ((uint32_t)0x00000000U)         /* !< Not block async clock request */
#define UNICOMM_CLKCFG_BLOCKASYNC_ENABLE         ((uint32_t)0x00000100U)         /* !< Block async clock request */

/*--- 状态寄存器 (STAT) 位域 ---*/
/* UNICOMM_STAT Bits */
/* UNICOMM_STAT[RESETSTKY] Bits */
#define UNICOMM_STAT_RESETSTKY_OFS               (16)                            /* !< RESETSTKY Offset */
#define UNICOMM_STAT_RESETSTKY_MASK              ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define UNICOMM_STAT_RESETSTKY_NORES             ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define UNICOMM_STAT_RESETSTKY_RESET             ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */

/*--- 协议模式选择寄存器 (IPMODE) 位域 ---*/
/* SELECT: 选择 UNICOMM 工作协议，0=UART, 1=SPI, 2=I2C控制器, 3=I2C外设 */
/* UNICOMM_IPMODE Bits */
/* UNICOMM_IPMODE[SELECT] Bits */
#define UNICOMM_IPMODE_SELECT_OFS                (0)                             /* !< SELECT Offset */
#define UNICOMM_IPMODE_SELECT_MASK               ((uint32_t)0x00000003U)         /* !< Free run control */
#define UNICOMM_IPMODE_SELECT_UART               ((uint32_t)0x00000000U)         /* !< Default functionality is UART */
#define UNICOMM_IPMODE_SELECT_SPI                ((uint32_t)0x00000001U)         /* !< SPG in SPI Mode */
#define UNICOMM_IPMODE_SELECT_I2C_CONTROLLER     ((uint32_t)0x00000002U)         /* !< SPG in I2C Controller Mode */
#define UNICOMM_IPMODE_SELECT_I2C_PERIPHERAL     ((uint32_t)0x00000003U)         /* !< SPG in I2C Peripheral Mode */

/*--- DMA 安全访问基地址 (BASEADDR) 位域 ---*/
/* UNICOMM_BASEADDR Bits */
/* UNICOMM_BASEADDR[ADDR] Bits */
#define UNICOMM_BASEADDR_ADDR_OFS                (6)                             /* !< ADDR Offset */
#define UNICOMM_BASEADDR_ADDR_MASK               ((uint32_t)0x7FFFFFC0U)         /* !< Base Address of SRAM range that DMA
                                                                                    can read/write. Configure a power of
                                                                                    2 base that is aligned to the range
                                                                                    specified. */

/*--- DMA 安全访问地址范围 (RANGE) 位域 ---*/
/* UNICOMM_RANGE Bits */
/* UNICOMM_RANGE[RANGE] Bits */
#define UNICOMM_RANGE_RANGE_OFS                  (0)                             /* !< RANGE Offset */
#define UNICOMM_RANGE_RANGE_MASK                 ((uint32_t)0x000FFFFFU)         /* !< Range (in bytes) of address range
                                                                                    that DMA can access */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_unicomm__include */
