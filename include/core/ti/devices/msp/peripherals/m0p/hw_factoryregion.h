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
 * @file hw_factoryregion.h
 * @brief MSPM0 FACTORYREGION (工厂配置区域) 外设寄存器定义
 *
 * FACTORYREGION 模块存储芯片出厂时的配置和校准数据:
 *   - 设备标识: TRACEID (ATE追踪ID)、DEVICEID (JTAG ID)、USERID (用户标识/变体/版本)
 *   - BSL引脚配置: UART/I2C的RXD/TXD/SDA/SCL引脚分配及功能选择，BSL调用引脚配置
 *   - 存储器大小: MAINFLASH/SRAM/DATAFLASH容量 (单位KB)、MAIN FLASH Bank数量
 *   - PLL启动参数: 4组频率范围 (4-8/8-16/16-32/32-48MHz) 的启动时间、电荷泵电流、环路滤波器参数
 *   - 温度传感器: 室温校准码 (TEMP_SENSE0)
 *   - 引导CRC: BOOTCRC 记录OPEN区域的32位CRC校验值
 *
 * @note 所有寄存器均为只读，数据在芯片制造时由ATE (自动测试设备) 写入
 * @note PLL参数应由系统初始化代码加载到SYSPLLPARAM0/1寄存器
 */

#ifndef ti_devices_msp_peripherals_hw_factoryregion__include
#define ti_devices_msp_peripherals_hw_factoryregion__include

/* This preliminary header file does not have a version number */
/* MMR repo: https://bitbucket.itg.ti.com/projects/cmcu_msp65soc/repos/trim_platform_constants */
/* MMR revision: ff4b6bcad5d93ca548bdb94b818a30dc3f4acd3f */
/* Generator revision: c98c4ac511e2bd0d918c5d427bd46b7a359dacf1
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
* FACTORYREGION Registers — 工厂配置区域寄存器结构体定义
* 包含: 设备ID、BSL配置、PLL参数、存储器大小、温度校准、引导CRC
******************************************************************************/
#define FACTORYREGION_OPEN_OFS                   ((uint32_t)0x41C40000U)


/** @addtogroup FACTORYREGION_OPEN
  @{
*/

typedef struct {
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G352X)
  __I  uint32_t USERID;                            /* !< (@ 0x41C40000) 用户标识 (器件变体/版本号) */
#else
  __I  uint32_t TRACEID;                           /* !< (@ 0x41C40000) ATE追踪ID (基于晶圆的唯一标识) */
#endif
  __I  uint32_t DEVICEID;                          /* !< (@ 0x41C40004) 设备ID (JTAG ID Code，含制造商/部件号/版本) */
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G352X)
  __I  uint32_t TRACEID;                           /* !< (@ 0x41C40008) ATE追踪ID (基于晶圆的唯一标识) */
#else
  __I  uint32_t USERID;                            /* !< (@ 0x41C40008) 用户标识 (器件变体/版本号) */
#endif
  __I  uint32_t BSLPIN_UART;                       /* !< (@ 0x41C4000C) BSL UART引脚配置 (RXD/TXD引脚及功能选择) */
  __I  uint32_t BSLPIN_I2C;                        /* !< (@ 0x41C40010) BSL I2C引脚配置 (SDA/SCL引脚及功能选择) */
  __I  uint32_t BSLPIN_INVOKE;                     /* !< (@ 0x41C40014) BSL调用引脚配置 (引脚号/电平/GPIO选择) */
  __I  uint32_t SRAMFLASH;                         /* !< (@ 0x41C40018) 存储器大小配置 (Flash/SRAM容量及Bank数) */
  __I  uint32_t PLLSTARTUP0_4_8MHZ;                /* !< (@ 0x41C4001C) PLL参数0: 4-8MHz范围 (启动时间/电荷泵/电容) */
  __I  uint32_t PLLSTARTUP1_4_8MHZ;                /* !< (@ 0x41C40020) PLL参数1: 4-8MHz范围 (环路滤波器参数) */
  __I  uint32_t PLLSTARTUP0_8_16MHZ;               /* !< (@ 0x41C40024) PLL参数0: 8-16MHz范围 */
  __I  uint32_t PLLSTARTUP1_8_16MHZ;               /* !< (@ 0x41C40028) PLL参数1: 8-16MHz范围 */
  __I  uint32_t PLLSTARTUP0_16_32MHZ;              /* !< (@ 0x41C4002C) PLL参数0: 16-32MHz范围 */
  __I  uint32_t PLLSTARTUP1_16_32MHZ;              /* !< (@ 0x41C40030) PLL参数1: 16-32MHz范围 */
  __I  uint32_t PLLSTARTUP0_32_48MHZ;              /* !< (@ 0x41C40034) PLL参数0: 32-48MHz范围 */
  __I  uint32_t PLLSTARTUP1_32_48MHZ;              /* !< (@ 0x41C40038) PLL参数1: 32-48MHz范围 */
  __I  uint32_t TEMP_SENSE0;                       /* !< (@ 0x41C4003C) 温度传感器室温校准码 (ADC转换结果) */
  __I  uint32_t RESERVED0;                         /* !< (@ 0x41C40040) 保留 */
  __I  uint32_t RESERVED1;                         /* !< (@ 0x41C40044) 保留 */
  __I  uint32_t RESERVED2;                         /* !< (@ 0x41C40048) 保留 */
  __I  uint32_t RESERVED3;                         /* !< (@ 0x41C4004C) 保留 */
  __I  uint32_t RESERVED4;                         /* !< (@ 0x41C40050) 保留 */
  __I  uint32_t RESERVED5;                         /* !< (@ 0x41C40054) 保留 */
  __I  uint32_t RESERVED6;                         /* !< (@ 0x41C40058) 保留 */
  __I  uint32_t RESERVED7;                         /* !< (@ 0x41C4005C) 保留 */
  __I  uint32_t RESERVED8;                         /* !< (@ 0x41C40060) 保留 */
  __I  uint32_t RESERVED9;                         /* !< (@ 0x41C40064) 保留 */
  __I  uint32_t RESERVED10;                        /* !< (@ 0x41C40068) 保留 */
  __I  uint32_t RESERVED11;                        /* !< (@ 0x41C4006C) 保留 */
  __I  uint32_t RESERVED12;                        /* !< (@ 0x41C40070) 保留 */
  __I  uint32_t RESERVED13;                        /* !< (@ 0x41C40074) 保留 */
  __I  uint32_t RESERVED14;                        /* !< (@ 0x41C40078) 保留 */
  __I  uint32_t BOOTCRC;                           /* !< (@ 0x41C4007C) 引导CRC: OPEN区域全部地址的32位CRC校验值 */
} FACTORYREGION_OPEN_Regs;

/*@}*/ /* end of group FACTORYREGION_OPEN */

/** @addtogroup FACTORYREGION
  @{
*/

typedef struct {
       uint32_t RESERVED0[275841024];
  FACTORYREGION_OPEN_Regs  OPEN;                              /* !< (@ 0x41C40000) Customer-visible data (IDs, PLL trims,...) */
} FACTORYREGION_Regs;

/*@}*/ /* end of group FACTORYREGION */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* FACTORYREGION Register Control Bits — 工厂配置区域寄存器位域定义
******************************************************************************/

/*—————————————————————————————————————————————————————————————————————————————
 * TRACEID (ATE追踪ID寄存器，只读)
 * 由ATE在晶圆测试时写入的唯一追踪标识
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_TRACEID[DATA] — 追踪ID数据 [31:0] */
#define FACTORYREGION_TRACEID_DATA_OFS           (0)                             /* !< DATA Offset */
#define FACTORYREGION_TRACEID_DATA_MASK          ((uint32_t)0xFFFFFFFFU)         /* !< 32位追踪ID数据 */

/*—————————————————————————————————————————————————————————————————————————————
 * DEVICEID (设备ID寄存器，只读)
 * JTAG ID Code: 含设备版本、部件号、制造商代码
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_DEVICEID[VERSION] — 设备版本号 [31:28] */
#define FACTORYREGION_DEVICEID_VERSION_OFS       (28)                            /* !< VERSION Offset */
#define FACTORYREGION_DEVICEID_VERSION_MASK      ((uint32_t)0xF0000000U)         /* !< 设备版本号，每次逻辑或掩膜修订时更新，范围 0-15 */
/* FACTORYREGION_DEVICEID[PARTNUM] — 部件号 [27:12] */
#define FACTORYREGION_DEVICEID_PARTNUM_OFS       (12)                            /* !< PARTNUM Offset */
#define FACTORYREGION_DEVICEID_PARTNUM_MASK      ((uint32_t)0x0FFFF000U)         /* !< 设备部件号，范围 0x0000-0xFFFF */
/* FACTORYREGION_DEVICEID[MANUFACTURER] — 制造商代码 [11:1] */
#define FACTORYREGION_DEVICEID_MANUFACTURER_OFS  (1)                             /* !< MANUFACTURER Offset */
#define FACTORYREGION_DEVICEID_MANUFACTURER_MASK ((uint32_t)0x00000FFEU)         /* !< TI的JEDEC银行和公司代码 (0x00B) */
/* FACTORYREGION_DEVICEID[ALWAYS_1] — 固定位 [0]，始终为1 */
#define FACTORYREGION_DEVICEID_ALWAYS_1_OFS      (0)                             /* !< ALWAYS_1 Offset */
#define FACTORYREGION_DEVICEID_ALWAYS_1_MASK     ((uint32_t)0x00000001U)         /* !< 固定为1 */

/*—————————————————————————————————————————————————————————————————————————————
 * USERID (用户标识寄存器，只读)
 * 包含部件标识、变体编号、主/次版本号
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_USERID[PART] — 部件标识 [15:0] */
#define FACTORYREGION_USERID_PART_OFS            (0)                             /* !< PART Offset */
#define FACTORYREGION_USERID_PART_MASK           ((uint32_t)0x0000FFFFU)         /* !< 部件唯一标识，基于DEVICEID中的die信息 */
/* FACTORYREGION_USERID[VARIANT] — 变体编号 [23:16] */
#define FACTORYREGION_USERID_VARIANT_OFS         (16)                            /* !< VARIANT Offset */
#define FACTORYREGION_USERID_VARIANT_MASK        ((uint32_t)0x00FF0000U)         /* !< 变体编号，指示同部件号的存储/封装变体 */
/* FACTORYREGION_USERID[MINORREV] — 次版本号 [27:24] */
#define FACTORYREGION_USERID_MINORREV_OFS        (24)                            /* !< MINORREV Offset */
#define FACTORYREGION_USERID_MINORREV_MASK       ((uint32_t)0x0F000000U)         /* !< 次版本号，单调递增，保持向后兼容 */
/* FACTORYREGION_USERID[MAJORREV] — 主版本号 [30:28] */
#define FACTORYREGION_USERID_MAJORREV_OFS        (28)                            /* !< MAJORREV Offset */
#define FACTORYREGION_USERID_MAJORREV_MASK       ((uint32_t)0x70000000U)         /* !< 主版本号，单调递增，重大变更可能需要修改PCB或软件 */
/* FACTORYREGION_USERID[START] — 起始位 [31] */
#define FACTORYREGION_USERID_START_OFS           (31)                            /* !< START Offset */
#define FACTORYREGION_USERID_START_MASK          ((uint32_t)0x80000000U)         /* !< 起始标志位 */

/*—————————————————————————————————————————————————————————————————————————————
 * BSLPIN_UART (BSL UART引脚配置寄存器，只读)
 * 配置BSL模式下UART的RXD/TXD引脚分配和功能选择
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_BSLPIN_UART[UART_RXD_PAD] — UART RXD引脚编号 [7:0] */
#define FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_OFS (0)                             /* !< UART_RXD_PAD Offset */
#define FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_MASK ((uint32_t)0x000000FFU)         /* !< BSL使用的UART RXD引脚编号 */
/* FACTORYREGION_BSLPIN_UART[UART_RXD_PF] — UART RXD引脚功能选择 [15:8] */
#define FACTORYREGION_BSLPIN_UART_UART_RXD_PF_OFS (8)                             /* !< UART_RXD_PF Offset */
#define FACTORYREGION_BSLPIN_UART_UART_RXD_PF_MASK ((uint32_t)0x0000FF00U)         /* !< UART RXD引脚功能选择值 */
/* FACTORYREGION_BSLPIN_UART[UART_TXD_PAD] — UART TXD引脚编号 [23:16] */
#define FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_OFS (16)                            /* !< UART_TXD_PAD Offset */
#define FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_MASK ((uint32_t)0x00FF0000U)         /* !< BSL使用的UART TXD引脚编号 */
/* FACTORYREGION_BSLPIN_UART[UART_TXD_PF] — UART TXD引脚功能选择 [31:24] */
#define FACTORYREGION_BSLPIN_UART_UART_TXD_PF_OFS (24)                            /* !< UART_TXD_PF Offset */
#define FACTORYREGION_BSLPIN_UART_UART_TXD_PF_MASK ((uint32_t)0xFF000000U)         /* !< UART TXD引脚功能选择值 */

/*—————————————————————————————————————————————————————————————————————————————
 * BSLPIN_I2C (BSL I2C引脚配置寄存器，只读)
 * 配置BSL模式下I2C的SDA/SCL引脚分配和功能选择
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_BSLPIN_I2C[I2C_SDA_PAD] — I2C SDA引脚编号 [7:0] */
#define FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_OFS (0)                             /* !< I2C_SDA_PAD Offset */
#define FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_MASK ((uint32_t)0x000000FFU)         /* !< BSL使用的I2C SDA引脚编号 */
/* FACTORYREGION_BSLPIN_I2C[I2C_SDA_PF] — I2C SDA引脚功能选择 [15:8] */
#define FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_OFS  (8)                             /* !< I2C_SDA_PF Offset */
#define FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_MASK ((uint32_t)0x0000FF00U)         /* !< I2C SDA引脚功能选择值 */
/* FACTORYREGION_BSLPIN_I2C[I2C_SCL_PAD] — I2C SCL引脚编号 [23:16] */
#define FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_OFS (16)                            /* !< I2C_SCL_PAD Offset */
#define FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_MASK ((uint32_t)0x00FF0000U)         /* !< BSL使用的I2C SCL引脚编号 */
/* FACTORYREGION_BSLPIN_I2C[I2C_SCL_PF] — I2C SCL引脚功能选择 [31:24] */
#define FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_OFS  (24)                            /* !< I2C_SCL_PF Offset */
#define FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_MASK ((uint32_t)0xFF000000U)         /* !< I2C SCL引脚功能选择值 */

/*—————————————————————————————————————————————————————————————————————————————
 * BSLPIN_INVOKE (BSL调用引脚配置寄存器，只读)
 * 配置BSL调用所需的GPIO引脚、电平和模块选择
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_BSLPIN_INVOKE[BSL_PAD] — BSL调用引脚编号 [6:0] */
#define FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_OFS  (0)                             /* !< BSL_PAD Offset */
#define FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_MASK ((uint32_t)0x0000007FU)         /* !< BSL调用引脚编号，范围 0-127 */
/* FACTORYREGION_BSLPIN_INVOKE[GPIO_LEVEL] — GPIO电平配置 [7] */
#define FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_OFS (7)                             /* !< GPIO_LEVEL Offset */
#define FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_MASK ((uint32_t)0x00000080U)         /* !< BSL调用时的GPIO有效电平 (0=低/1=高) */
/* FACTORYREGION_BSLPIN_INVOKE[GPIO_PIN_SEL] — GPIO模块内引脚号 [12:8] */
#define FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_OFS (8)                             /* !< GPIO_PIN_SEL Offset */
#define FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_MASK ((uint32_t)0x00001F00U)         /* !< GPIO模块内的引脚编号，范围 0-31 */
/* FACTORYREGION_BSLPIN_INVOKE[GPIO_REG_SEL] — GPIO模块选择 [14:13] */
#define FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_OFS (13)                            /* !< GPIO_REG_SEL Offset */
#define FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_MASK ((uint32_t)0x00006000U)         /* !< GPIO模块选择，范围 0-3 */

/*—————————————————————————————————————————————————————————————————————————————
 * SRAMFLASH (存储器大小配置寄存器，只读)
 * 主Flash、SRAM、数据Flash的容量 (单位KB) 及Bank数量
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_SRAMFLASH[MAINFLASH_SZ] — 主Flash容量 [11:0]，单位KB */
#define FACTORYREGION_SRAMFLASH_MAINFLASH_SZ_OFS (0)                             /* !< MAINFLASH_SZ Offset */
#define FACTORYREGION_SRAMFLASH_MAINFLASH_SZ_MASK ((uint32_t)0x00000FFFU)         /* !< 主Flash容量，值即为KB数，范围 0-4095 */
/* FACTORYREGION_SRAMFLASH[MAINNUMBANKS] — 主Flash Bank数 [13:12] */
#define FACTORYREGION_SRAMFLASH_MAINNUMBANKS_OFS (12)                            /* !< MAINNUMBANKS Offset */
#define FACTORYREGION_SRAMFLASH_MAINNUMBANKS_MASK ((uint32_t)0x00003000U)         /* !< 主Flash Bank数量 */
#define FACTORYREGION_SRAMFLASH_MAINNUMBANKS_ONEBANK ((uint32_t)0x00000000U)      /* !< 1个Bank */
#define FACTORYREGION_SRAMFLASH_MAINNUMBANKS_TWOBANKS ((uint32_t)0x00001000U)     /* !< 2个Bank */
#define FACTORYREGION_SRAMFLASH_MAINNUMBANKS_THREEBANKS ((uint32_t)0x00002000U)   /* !< 3个Bank */
#define FACTORYREGION_SRAMFLASH_MAINNUMBANKS_FOURBANKS ((uint32_t)0x00003000U)    /* !< 4个Bank */
/* FACTORYREGION_SRAMFLASH[SRAM_SZ] — SRAM容量 [25:16]，单位KB */
#define FACTORYREGION_SRAMFLASH_SRAM_SZ_OFS      (16)                            /* !< SRAM_SZ Offset */
#define FACTORYREGION_SRAMFLASH_SRAM_SZ_MASK     ((uint32_t)0x03FF0000U)         /* !< SRAM容量，值即为KB数，范围 0-1023 */
/* FACTORYREGION_SRAMFLASH[DATAFLASH_SZ] — 数据Flash容量 [31:26]，单位KB */
#define FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_OFS (26)                            /* !< DATAFLASH_SZ Offset */
#define FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_MASK ((uint32_t)0xFC000000U)        /* !< 数据Flash容量，值即为KB数，范围 0-63 */

/*—————————————————————————————————————————————————————————————————————————————
 * PLL启动参数 — 4-8MHz范围
 * 包含PLL0 (启动时间/电荷泵/电容) 和PLL1 (环路滤波器)
 * 参数格式相同，按频率范围分为4组 (4-8/8-16/16-32/32-48MHz)
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_PLLSTARTUP0_4_8MHZ[STARTTIME] — 启用到锁定的启动时间 [5:0]，分辨率1us */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_STARTTIME_OFS (0)                             /* !< STARTTIME Offset */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_STARTTIME_MASK ((uint32_t)0x0000003FU)         /* !< 启动时间 (启用到锁定)，分辨率1us */
/* FACTORYREGION_PLLSTARTUP0_4_8MHZ[STARTTIMELP] Bits */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_STARTTIMELP_OFS (8)                             /* !< STARTTIMELP Offset */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_STARTTIMELP_MASK ((uint32_t)0x00003F00U)         /* !< Startup time from Low Power Exit to
                                                                                    Locked Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_4_8MHZ[CPCURRENT] Bits */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CPCURRENT_OFS (16)                            /* !< CPCURRENT Offset */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CPCURRENT_MASK ((uint32_t)0x003F0000U)         /* !< Charge Pump Current */
/* FACTORYREGION_PLLSTARTUP0_4_8MHZ[CAPBVAL] Bits */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CAPBVAL_OFS (24)                            /* !< CAPBVAL Offset */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CAPBVAL_MASK ((uint32_t)0x1F000000U)         /* !< Override Value for Cap B */
/* FACTORYREGION_PLLSTARTUP0_4_8MHZ[CAPBOVERRIDE] Bits */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CAPBOVERRIDE_OFS (31)                            /* !< CAPBOVERRIDE Offset */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CAPBOVERRIDE_MASK ((uint32_t)0x80000000U)         /* !< Override Enable For Cap B */
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CAPBOVERRIDE_DISABLE ((uint32_t)0x00000000U)
#define FACTORYREGION_PLLSTARTUP0_4_8MHZ_CAPBOVERRIDE_ENABLE ((uint32_t)0x80000000U)

/* FACTORYREGION_PLLSTARTUP1_4_8MHZ Bits */
/* FACTORYREGION_PLLSTARTUP1_4_8MHZ[LPFCAPA] Bits */
#define FACTORYREGION_PLLSTARTUP1_4_8MHZ_LPFCAPA_OFS (0)                             /* !< LPFCAPA Offset */
#define FACTORYREGION_PLLSTARTUP1_4_8MHZ_LPFCAPA_MASK ((uint32_t)0x0000001FU)         /* !< Loop Filter Cap A */
/* FACTORYREGION_PLLSTARTUP1_4_8MHZ[LPFRESA] Bits */
#define FACTORYREGION_PLLSTARTUP1_4_8MHZ_LPFRESA_OFS (8)                             /* !< LPFRESA Offset */
#define FACTORYREGION_PLLSTARTUP1_4_8MHZ_LPFRESA_MASK ((uint32_t)0x0003FF00U)         /* !< Loop Filter Res A */
/* FACTORYREGION_PLLSTARTUP1_4_8MHZ[LPFRESC] Bits */
#define FACTORYREGION_PLLSTARTUP1_4_8MHZ_LPFRESC_OFS (24)                            /* !< LPFRESC Offset */
#define FACTORYREGION_PLLSTARTUP1_4_8MHZ_LPFRESC_MASK ((uint32_t)0xFF000000U)         /* !< Loop Filter Res C */

/*—————————————————————————————————————————————————————————————————————————————
 * PLL启动参数 — 8-16MHz范围
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_PLLSTARTUP0_8_16MHZ[STARTTIME] — 启用到锁定的启动时间 [5:0]，分辨率1us */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_STARTTIME_OFS (0)                             /* !< STARTTIME Offset */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_STARTTIME_MASK ((uint32_t)0x0000003FU)         /* !< Startup time from Enable to Locked
                                                                                    Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_8_16MHZ[STARTTIMELP] Bits */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_STARTTIMELP_OFS (8)                             /* !< STARTTIMELP Offset */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_STARTTIMELP_MASK ((uint32_t)0x00003F00U)         /* !< Startup time from Low Power Exit to
                                                                                    Locked Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_8_16MHZ[CPCURRENT] Bits */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CPCURRENT_OFS (16)                            /* !< CPCURRENT Offset */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CPCURRENT_MASK ((uint32_t)0x003F0000U)         /* !< Charge Pump Current */
/* FACTORYREGION_PLLSTARTUP0_8_16MHZ[CAPBVAL] Bits */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CAPBVAL_OFS (24)                            /* !< CAPBVAL Offset */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CAPBVAL_MASK ((uint32_t)0x1F000000U)         /* !< Override Value for Cap B */
/* FACTORYREGION_PLLSTARTUP0_8_16MHZ[CAPBOVERRIDE] Bits */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CAPBOVERRIDE_OFS (31)                            /* !< CAPBOVERRIDE Offset */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CAPBOVERRIDE_MASK ((uint32_t)0x80000000U)         /* !< Override Enable For Cap B */
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CAPBOVERRIDE_DISABLE ((uint32_t)0x00000000U)
#define FACTORYREGION_PLLSTARTUP0_8_16MHZ_CAPBOVERRIDE_ENABLE ((uint32_t)0x80000000U)

/* FACTORYREGION_PLLSTARTUP1_8_16MHZ Bits */
/* FACTORYREGION_PLLSTARTUP1_8_16MHZ[LPFCAPA] Bits */
#define FACTORYREGION_PLLSTARTUP1_8_16MHZ_LPFCAPA_OFS (0)                             /* !< LPFCAPA Offset */
#define FACTORYREGION_PLLSTARTUP1_8_16MHZ_LPFCAPA_MASK ((uint32_t)0x0000001FU)         /* !< Loop Filter Cap A */
/* FACTORYREGION_PLLSTARTUP1_8_16MHZ[LPFRESA] Bits */
#define FACTORYREGION_PLLSTARTUP1_8_16MHZ_LPFRESA_OFS (8)                             /* !< LPFRESA Offset */
#define FACTORYREGION_PLLSTARTUP1_8_16MHZ_LPFRESA_MASK ((uint32_t)0x0003FF00U)         /* !< Loop Filter Res A */
/* FACTORYREGION_PLLSTARTUP1_8_16MHZ[LPFRESC] Bits */
#define FACTORYREGION_PLLSTARTUP1_8_16MHZ_LPFRESC_OFS (24)                            /* !< LPFRESC Offset */
#define FACTORYREGION_PLLSTARTUP1_8_16MHZ_LPFRESC_MASK ((uint32_t)0xFF000000U)         /* !< Loop Filter Res C */

/*—————————————————————————————————————————————————————————————————————————————
 * PLL启动参数 — 16-32MHz范围
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_PLLSTARTUP0_16_32MHZ[STARTTIME] — 启用到锁定的启动时间 [5:0]，分辨率1us */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_STARTTIME_OFS (0)                             /* !< STARTTIME Offset */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_STARTTIME_MASK ((uint32_t)0x0000003FU)         /* !< Startup time from Enable to Locked
                                                                                    Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_16_32MHZ[STARTTIMELP] Bits */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_STARTTIMELP_OFS (8)                             /* !< STARTTIMELP Offset */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_STARTTIMELP_MASK ((uint32_t)0x00003F00U)         /* !< Startup time from Low Power Exit to
                                                                                    Locked Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_16_32MHZ[CPCURRENT] Bits */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CPCURRENT_OFS (16)                            /* !< CPCURRENT Offset */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CPCURRENT_MASK ((uint32_t)0x003F0000U)         /* !< Charge Pump Current */
/* FACTORYREGION_PLLSTARTUP0_16_32MHZ[CAPBVAL] Bits */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CAPBVAL_OFS (24)                            /* !< CAPBVAL Offset */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CAPBVAL_MASK ((uint32_t)0x1F000000U)         /* !< Override Value for Cap B */
/* FACTORYREGION_PLLSTARTUP0_16_32MHZ[CAPBOVERRIDE] Bits */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CAPBOVERRIDE_OFS (31)                            /* !< CAPBOVERRIDE Offset */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CAPBOVERRIDE_MASK ((uint32_t)0x80000000U)         /* !< Override Enable For Cap B */
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CAPBOVERRIDE_DISABLE ((uint32_t)0x00000000U)
#define FACTORYREGION_PLLSTARTUP0_16_32MHZ_CAPBOVERRIDE_ENABLE ((uint32_t)0x80000000U)

/* FACTORYREGION_PLLSTARTUP1_16_32MHZ Bits */
/* FACTORYREGION_PLLSTARTUP1_16_32MHZ[LPFCAPA] Bits */
#define FACTORYREGION_PLLSTARTUP1_16_32MHZ_LPFCAPA_OFS (0)                             /* !< LPFCAPA Offset */
#define FACTORYREGION_PLLSTARTUP1_16_32MHZ_LPFCAPA_MASK ((uint32_t)0x0000001FU)         /* !< Loop Filter Cap A */
/* FACTORYREGION_PLLSTARTUP1_16_32MHZ[LPFRESA] Bits */
#define FACTORYREGION_PLLSTARTUP1_16_32MHZ_LPFRESA_OFS (8)                             /* !< LPFRESA Offset */
#define FACTORYREGION_PLLSTARTUP1_16_32MHZ_LPFRESA_MASK ((uint32_t)0x0003FF00U)         /* !< Loop Filter Res A */
/* FACTORYREGION_PLLSTARTUP1_16_32MHZ[LPFRESC] Bits */
#define FACTORYREGION_PLLSTARTUP1_16_32MHZ_LPFRESC_OFS (24)                            /* !< LPFRESC Offset */
#define FACTORYREGION_PLLSTARTUP1_16_32MHZ_LPFRESC_MASK ((uint32_t)0xFF000000U)         /* !< Loop Filter Res C */

/*—————————————————————————————————————————————————————————————————————————————
 * PLL启动参数 — 32-48MHz范围
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_PLLSTARTUP0_32_48MHZ[STARTTIME] — 启用到锁定的启动时间 [5:0]，分辨率1us */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_STARTTIME_OFS (0)                             /* !< STARTTIME Offset */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_STARTTIME_MASK ((uint32_t)0x0000003FU)         /* !< Startup time from Enable to Locked
                                                                                    Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_32_48MHZ[STARTTIMELP] Bits */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_STARTTIMELP_OFS (8)                             /* !< STARTTIMELP Offset */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_STARTTIMELP_MASK ((uint32_t)0x00003F00U)         /* !< Startup time from Low Power Exit to
                                                                                    Locked Clock in resolution of 1usec */
/* FACTORYREGION_PLLSTARTUP0_32_48MHZ[CPCURRENT] Bits */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CPCURRENT_OFS (16)                            /* !< CPCURRENT Offset */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CPCURRENT_MASK ((uint32_t)0x003F0000U)         /* !< Charge Pump Current */
/* FACTORYREGION_PLLSTARTUP0_32_48MHZ[CAPBVAL] Bits */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CAPBVAL_OFS (24)                            /* !< CAPBVAL Offset */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CAPBVAL_MASK ((uint32_t)0x1F000000U)         /* !< Override Value for Cap B */
/* FACTORYREGION_PLLSTARTUP0_32_48MHZ[CAPBOVERRIDE] Bits */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CAPBOVERRIDE_OFS (31)                            /* !< CAPBOVERRIDE Offset */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CAPBOVERRIDE_MASK ((uint32_t)0x80000000U)         /* !< Override Enable For Cap B */
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CAPBOVERRIDE_DISABLE ((uint32_t)0x00000000U)
#define FACTORYREGION_PLLSTARTUP0_32_48MHZ_CAPBOVERRIDE_ENABLE ((uint32_t)0x80000000U)

/* FACTORYREGION_PLLSTARTUP1_32_48MHZ Bits */
/* FACTORYREGION_PLLSTARTUP1_32_48MHZ[LPFCAPA] Bits */
#define FACTORYREGION_PLLSTARTUP1_32_48MHZ_LPFCAPA_OFS (0)                             /* !< LPFCAPA Offset */
#define FACTORYREGION_PLLSTARTUP1_32_48MHZ_LPFCAPA_MASK ((uint32_t)0x0000001FU)         /* !< Loop Filter Cap A */
/* FACTORYREGION_PLLSTARTUP1_32_48MHZ[LPFRESA] Bits */
#define FACTORYREGION_PLLSTARTUP1_32_48MHZ_LPFRESA_OFS (8)                             /* !< LPFRESA Offset */
#define FACTORYREGION_PLLSTARTUP1_32_48MHZ_LPFRESA_MASK ((uint32_t)0x0003FF00U)         /* !< Loop Filter Res A */
/* FACTORYREGION_PLLSTARTUP1_32_48MHZ[LPFRESC] Bits */
#define FACTORYREGION_PLLSTARTUP1_32_48MHZ_LPFRESC_OFS (24)                            /* !< LPFRESC Offset */
#define FACTORYREGION_PLLSTARTUP1_32_48MHZ_LPFRESC_MASK ((uint32_t)0xFF000000U)         /* !< Loop Filter Res C */

/*—————————————————————————————————————————————————————————————————————————————
 * TEMP_SENSE0 (温度传感器校准寄存器，只读)
 * 室温下的ADC转换结果，用于温度计算的基准值
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_TEMP_SENSE0[DATA] — 温度传感器校准数据 [31:0] */
#define FACTORYREGION_TEMP_SENSE0_DATA_OFS       (0)                             /* !< DATA Offset */
#define FACTORYREGION_TEMP_SENSE0_DATA_MASK      ((uint32_t)0xFFFFFFFFU)         /* !< 室温ADC校准值 */

/*—————————————————————————————————————————————————————————————————————————————
 * BOOTCRC (引导CRC校验寄存器，只读)
 * OPEN区域全部地址 (含保留区域) 的32位CRC校验值
 *—————————————————————————————————————————————————————————————————————————————*/
/* FACTORYREGION_BOOTCRC[DATA] — CRC校验值 [31:0] */
#define FACTORYREGION_BOOTCRC_DATA_OFS           (0)                             /* !< DATA Offset */
#define FACTORYREGION_BOOTCRC_DATA_MASK          ((uint32_t)0xFFFFFFFFU)         /* !< 32位CRC校验值 */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_factoryregion__include */
