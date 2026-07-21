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
 *  @file       dl_flashctl.h
 *  @brief      Flash Controller Driver Library (Flash 控制器驱动库头文件)
 *  @defgroup   FLASHCTL Flash Controller (FLASHCTL)
 *
 *  @anchor ti_dl_dl_flashctl_Overview
 *  # Overview
 *
 *  The Flash Controller Driver Library allows full configuration of
 *  the MSPM0 Non-Volatile memory system.
 *  The non-volatile memory (NVM) system provides nonvolatile flash memory for
 *  storing executable code and data.
 *
 *  本文件提供 MSPM0 Flash 控制器的完整驱动接口，包括擦除、编程、
 *  读校验、空白校验、写保护管理以及 ECC 处理等功能。
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup FLASHCTL
 * @{
 */
#ifndef ti_dl_dl_flashctl__include
#define ti_dl_dl_flashctl__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/m0p/dl_factoryregion.h>
#include <ti/driverlib/m0p/dl_sysctl.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if (FLASHCTL_SYS_DATAWIDTH == 128)
/*!
 * @brief Device has 128 bit flash word width
 *        (器件支持 128 位 Flash 字宽)
 */
#define DEVICE_HAS_FLASH_128_BIT_WORD
#endif

#if (FLASHCTL_SYS_WEPROTAWIDTH == 0)
/*!
 *  @brief Device does not have CMDWEPROTA
 *         (器件不含 CMDWEPROTA 寄存器)
 */
#define DEVICE_HAS_NO_CMDWEPROTA
#endif

    /* clang-format off */

/*!
 * @brief Address for Bank 0 (Bank 0 起始地址)
 */
#define FLASHCTL_BANK0_ADDRESS                                      (0x00000000)

/*!
 * @brief Address for MAIN memory region (主存储区起始地址)
 */
#define FLASHCTL_MAIN_ADDRESS                                       (0x00000000)

/*!
 * @brief Address for NONMAIN memory region (Non-Main 存储区起始地址)
 */
#define FLASHCTL_NONMAIN_ADDRESS                                    (0x41c00000)

/*!
 * @brief Number of NONMAIN sectors (Non-Main 区扇区数)
 */
#define NUMBER_OF_NONMAIN_SECTORS                                            (1)

/*!
 * @brief Address for DATA memory region (Data 存储区起始地址)
 */
#define FLASHCTL_DATA_ADDRESS                                       (0x41d00000)

/*!
 * @brief Enable programming 8 bits without ECC enabled
 *        (无 ECC 的 8 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_8_WITHOUT_ECC                           (0x00000001)

/*!
 * @brief Enable programming 16 bits without ECC enabled
 *        (无 ECC 的 16 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_16_WITHOUT_ECC                          (0x00000003)

/*!
 * @brief Enable programming 32 bits without ECC enabled
 *        (无 ECC 的 32 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_32_WITHOUT_ECC                          (0x0000000F)

/*!
 * @brief Enable programming 64 bits without ECC enabled
 *        (无 ECC 的 64 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_64_WITHOUT_ECC                          (0x000000FF)

/*!
 * @brief Enable programming 8 bits with ECC enabled
 *        (带 ECC 的 8 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_8_WITH_ECC                              (0x00000101)

/*!
 * @brief Enable programming 16 bits with ECC enabled
 *        (带 ECC 的 16 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_16_WITH_ECC                             (0x00000103)

/*!
 * @brief Enable programming 32 bits with ECC enabled
 *        (带 ECC 的 32 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_32_WITH_ECC                             (0x0000010F)

/*!
 * @brief Enable programming 64 bits with ECC enabled
 *        (带 ECC 的 64 位编程字节使能掩码)
 */
#define DL_FLASHCTL_PROGRAM_64_WITH_ECC                             (0x000001FF)

/*!
 * @brief Enable read verify 8 bits without ECC enabled
 *        (无 ECC 的 8 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_8_WITHOUT_ECC                       (0x00000001)

/*!
 * @brief Enable read verify 16 bits without ECC enabled
 *        (无 ECC 的 16 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_16_WITHOUT_ECC                      (0x00000003)

/*!
 * @brief Enable read verify 32 bits without ECC enabled
 *        (无 ECC 的 32 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_32_WITHOUT_ECC                      (0x0000000F)

/*!
 * @brief Enable read verify 64 bits without ECC enabled
 *        (无 ECC 的 64 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_64_WITHOUT_ECC                      (0x000000FF)

/*!
 * @brief Enable read verify 8 bits with ECC enabled
 *        (带 ECC 的 8 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_8_WITH_ECC                          (0x00000101)

/*!
 * @brief Enable read verify 16 bits with ECC enabled
 *        (带 ECC 的 16 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_16_WITH_ECC                         (0x00000103)

/*!
 * @brief Enable read verify 32 bits with ECC enabled
 *        (带 ECC 的 32 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_32_WITH_ECC                         (0x0000010F)

/*!
 * @brief Enable read verify 64 bits with ECC enabled
 *        (带 ECC 的 64 位读校验字节使能掩码)
 */
#define DL_FLASHCTL_READ_VERIFY_64_WITH_ECC                         (0x000001FF)

/** @addtogroup DL_FLASHCTL_INTERRUPT
 *  @{
 */
/*!
 * @brief Flash Command execution has completed (Flash 命令执行完成中断)
 */
#define DL_FLASHCTL_INTERRUPT_DONE                       (FLASHCTL_MIS_DONE_SET)
/** @}*/

/** @addtogroup DL_FLASHCTL_COMMAND_TYPE
 *  @{
 */
/*!
 * @brief No Operation Flash Command (空操作命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_NO_OPERATION    (FLASHCTL_CMDTYPE_COMMAND_NOOP)

/*!
 * @brief Program Command Type (编程命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_PROGRAM      (FLASHCTL_CMDTYPE_COMMAND_PROGRAM)

/*!
 * @brief Erase Command Type (擦除命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_ERASE          (FLASHCTL_CMDTYPE_COMMAND_ERASE)

/*!
 * @brief Read-Verify Command Type (读校验命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY                                   \
                                           (FLASHCTL_CMDTYPE_COMMAND_READVERIFY)

/*!
 * @brief Blank Verify Command Type (空白校验命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_BLANK_VERIFY                                  \
                                          (FLASHCTL_CMDTYPE_COMMAND_BLANKVERIFY)

/*!
 * @brief Mode Change Command Type (模式切换命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_MODE_CHANGE                                   \
                                           (FLASHCTL_CMDTYPE_COMMAND_MODECHANGE)

/*!
 * @brief Clear Status Command Type (清除状态命令)
 */
#define DL_FLASHCTL_COMMAND_TYPE_CLEAR_STATUS                                  \
                                          (FLASHCTL_CMDTYPE_COMMAND_CLEARSTATUS)
/** @}*/

/*!
 * @brief Size of one flash sector (单个 Flash 扇区大小，1KB)
 */
#define DL_FLASHCTL_SECTOR_SIZE                                           (1024)

    /* clang-format on */

    /*! @enum DL_FLASHCTL_COMMAND_SIZE (Flash 命令操作粒度) */
    typedef enum
    {
        /*! Flash Command size is one word (单字) */
        DL_FLASHCTL_COMMAND_SIZE_ONE_WORD = FLASHCTL_CMDTYPE_SIZE_ONEWORD,
        /*! Flash Command size is two words (双字) */
        DL_FLASHCTL_COMMAND_SIZE_TWO_WORDS = FLASHCTL_CMDTYPE_SIZE_TWOWORD,
        /*! Flash Command size is four words (四字) */
        DL_FLASHCTL_COMMAND_SIZE_FOUR_WORDS = FLASHCTL_CMDTYPE_SIZE_FOURWORD,
        /*! Flash Command size is eight words (八字) */
        DL_FLASHCTL_COMMAND_SIZE_EIGHT_WORDS = FLASHCTL_CMDTYPE_SIZE_EIGHTWORD,
        /*! Flash Command size is a sector (扇区) */
        DL_FLASHCTL_COMMAND_SIZE_SECTOR = FLASHCTL_CMDTYPE_SIZE_SECTOR,
        /*! Flash Command size is a bank (整个 Bank) */
        DL_FLASHCTL_COMMAND_SIZE_BANK = FLASHCTL_CMDTYPE_SIZE_BANK

    } DL_FLASHCTL_COMMAND_SIZE;

    /*! @enum DL_FLASHCTL_REGION_SELECT (Flash 区域选择) */
    typedef enum
    {
        /*! Flash Region Select Main (主存储区) */
        DL_FLASHCTL_REGION_SELECT_MAIN = FLASHCTL_CMDCTL_REGIONSEL_MAIN,
        /*! Flash Region Select Non-Main (Non-Main 存储区) */
        DL_FLASHCTL_REGION_SELECT_NONMAIN = FLASHCTL_CMDCTL_REGIONSEL_NONMAIN

    } DL_FLASHCTL_REGION_SELECT;

    /*! @enum DL_FLASHCTL_BANK_SELECT (Flash Bank 选择) */
    typedef enum
    {
        /*! Bank 0 */
        DL_FLASHCTL_BANK_SELECT_0 = FLASHCTL_CMDCTL_BANKSEL_BANK0,
        /*! Bank 1 */
        DL_FLASHCTL_BANK_SELECT_1 = FLASHCTL_CMDCTL_BANKSEL_BANK1,
        /*! Bank 2 */
        DL_FLASHCTL_BANK_SELECT_2 = FLASHCTL_CMDCTL_BANKSEL_BANK2,
        /*! Bank 3 */
        DL_FLASHCTL_BANK_SELECT_3 = FLASHCTL_CMDCTL_BANKSEL_BANK3,
        /*! Bank 4 */
        DL_FLASHCTL_BANK_SELECT_4 = FLASHCTL_CMDCTL_BANKSEL_BANK4
    } DL_FLASHCTL_BANK_SELECT;

    /*! @enum DL_FLASHCTL_FAIL_TYPE (Flash 命令失败原因) */
    typedef enum
    {
        /*! Flash Command did not fail during execution (未失败) */
        DL_FLASHCTL_FAIL_TYPE_NO_FAILURE = 0x0,
        /*! Flash Command failed due to write/erase protect sector violation (写擦保护冲突) */
        DL_FLASHCTL_FAIL_TYPE_WRITE_ERASE_PROTECT =
            (FLASHCTL_STATCMD_FAILWEPROT_STATFAIL),
        /*! Flash Command failed due to verify error (校验错误) */
        DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR = FLASHCTL_STATCMD_FAILVERIFY_STATFAIL,
        /*! Flash Command failed due to the use of an illegal address (非法地址) */
        DL_FLASHCTL_FAIL_TYPE_ILLEGAL_ADDRESS =
            FLASHCTL_STATCMD_FAILILLADDR_STATFAIL,
        /*! Flash Command failed because a bank has been set to a mode other than READ (Bank 模式错误) */
        DL_FLASHCTL_FAIL_TYPE_WRONG_BANK_MODE = FLASHCTL_STATCMD_FAILMODE_STATFAIL,
        /*! Flash Command failed due to an undefined error (其他未定义错误) */
        DL_FLASHCTL_FAIL_TYPE_MISCELLANEOUS = FLASHCTL_STATCMD_FAILMISC_STATFAIL,
    } DL_FLASHCTL_FAIL_TYPE;

    /*! @enum DL_FLASHCTL_COMMAND_STATUS (Flash 命令执行状态) */
    typedef enum
    {
        /*! Flash Command passed (命令通过) */
        DL_FLASHCTL_COMMAND_STATUS_PASSED = (FLASHCTL_STATCMD_CMDDONE_STATDONE |
                                             FLASHCTL_STATCMD_CMDPASS_STATPASS),
        /*! Flash Command failed (命令失败) */
        DL_FLASHCTL_COMMAND_STATUS_FAILED = (FLASHCTL_STATCMD_CMDDONE_STATDONE |
                                             FLASHCTL_STATCMD_CMDPASS_STATFAIL),
        /*! Flash Command is still in progress (命令执行中) */
        DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS =
            (FLASHCTL_STATCMD_CMDINPROGRESS_STATINPROGRESS)

    } DL_FLASHCTL_COMMAND_STATUS;

    /*! @enum DL_FLASHCTL_IIDX (Flash 中断索引) */
    typedef enum
    {
        /*! DONE Interrupt Pending (DONE 中断待处理) */
        DL_FLASHCTL_IIDX_DONE = FLASHCTL_IIDX_STAT_DONE
    } DL_FLASHCTL_IIDX;

    /**
     *  @brief      Enable flash interrupt
     *              (启用 Flash 命令完成中断)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     */
    __STATIC_INLINE void DL_FlashCTL_enableInterrupt(FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.IMASK = FLASHCTL_IMASK_DONE_ENABLED;
    }

    /**
     *  @brief      Disable flash interrupt
     *              (禁用 Flash 命令完成中断)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     */
    __STATIC_INLINE void DL_FlashCTL_disableInterrupt(FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.IMASK = FLASHCTL_IMASK_DONE_DISABLED;
    }

    /**
     *  @brief      Check if the flash interrupt is enabled
     *              (检查 Flash 中断是否已启用)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If the flash interrupt is enabled
     *              (中断是否启用)
     *
     *  @retval     false if interrupt is disabled
     *  @retval     true if interrupt is enabled
     */
    __STATIC_INLINE bool DL_FlashCTL_isInterruptEnabled(
        const FLASHCTL_Regs *flashctl)
    {
        return (flashctl->GEN.IMASK == FLASHCTL_IMASK_DONE_ENABLED);
    }

    /**
     *  @brief      Check interrupt flag of enabled flash interrupt
     *              (读取已启用中断的挂起状态)
     *
     *  Checks if the flash interrupt that was previously enabled is pending.
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If the flash interrupt is pending
     *              (中断挂起标志)
     *
     *  @retval     0 if no interrupt is enabled
     *  @retval     The value of @ref DL_FLASHCTL_INTERRUPT
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getEnabledInterruptStatus(
        const FLASHCTL_Regs *flashctl)
    {
        return (flashctl->GEN.MIS);
    }

    /**
     *  @brief      Check interrupt flag of the flash interrupt
     *              (读取原始中断挂起状态，无需预先启用)
     *
     *  Checks if the flash interrupt is pending. Interrupts do not have to be
     *  previously enabled.
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If the flash interrupt is pending
     *              (原始中断挂起标志)
     *
     *  @retval     0 if no interrupt is enabled
     *  @retval     The value of @ref DL_FLASHCTL_INTERRUPT
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getRawInterruptStatus(
        const FLASHCTL_Regs *flashctl)
    {
        return (flashctl->GEN.RIS);
    }

    /**
     *  @brief      Get highest priority pending flash interrupt
     *              (获取最高优先级的待处理中断索引)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     The highest priority pending FLASH interrupt
     *              (中断索引)
     *
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getPendingInterrupt(
        const FLASHCTL_Regs *flashctl)
    {
        return (flashctl->GEN.IIDX);
    }

    /**
     *  @brief      Clear pending flash interrupt
     *              (清除 Flash 中断挂起标志)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_clearInterruptStatus(FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.ICLR = FLASHCTL_ICLR_DONE_CLR;
    }

    /**
     *  @brief      Checks if a command execution has been initiated
     *              (检查命令是否已触发执行)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If a command has been set to execute
     *              (是否已触发)
     */
    __STATIC_INLINE bool DL_FlashCTL_isCommandExecuted(
        const FLASHCTL_Regs *flashctl)
    {
        return (flashctl->GEN.CMDEXEC == FLASHCTL_CMDEXEC_VAL_EXECUTE);
    }

    /**
     *  @brief      Initiates a command execution
     *              (触发 Flash 命令执行)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_setCommandExecute(FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
    }

    /**
     *  @brief      Disable Stair-Step Erase
     *              (禁用阶梯擦除，使用默认 VHV 电压)
     *
     * The default VHV trim voltage setting will be used for all erase pulses.
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_disbleStairStepErase(FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDCTL |= FLASHCTL_CMDCTL_SSERASEDIS_DISABLE;
    }

    /**
     *  @brief      Checks if stair-step erase is disabled
     *              (检查阶梯擦除是否已禁用)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If stair-step erase is disabled
     *              (是否已禁用)
     */
    __STATIC_INLINE bool DL_FlashCTL_isStairStepEraseDisabled(
        const FLASHCTL_Regs *flashctl)
    {
        return ((flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_SSERASEDIS_MASK) ==
                FLASHCTL_CMDCTL_SSERASEDIS_DISABLE);
    }

    /**
     *  @brief      Enable Stair-Step Erase
     *              (启用阶梯擦除，逐步升高 VHV 电压)
     *
     * The VHV voltage will be stepped during successive erase pulses. The step
     * count, step voltage, begin and end voltages are all hard-wired. This is
     * enabled by default.
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_enableStairStepErase(FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDCTL &= ~(FLASHCTL_CMDCTL_SSERASEDIS_MASK);
    }

    /**
     *  @brief      Enable address override mode
     *              (启用地址覆盖模式，绕过硬件地址翻译)
     *
     * This overrides hardware address translation in CMDADDR from a system address
     * to a bank address and bank ID.
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     * @sa DL_FlashCTL_setCommandAddress
     * @sa DL_FlashCTL_setBankSelect
     * @sa DL_FlashCTL_setRegionSelect
     */
    __STATIC_INLINE void DL_FlashCTL_enableAddressOverrideMode(
        FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDCTL |= FLASHCTL_CMDCTL_ADDRXLATEOVR_OVERRIDE;
    }

    /**
     *  @brief      Checks if address override mode is enabled
     *              (检查地址覆盖模式是否已启用)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If address override mode is enabled
     *              (是否已启用)
     */
    __STATIC_INLINE bool DL_FlashCTL_isAddressOverrideModeEnabled(
        const FLASHCTL_Regs *flashctl)
    {
        return ((flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_ADDRXLATEOVR_MASK) ==
                FLASHCTL_CMDCTL_ADDRXLATEOVR_OVERRIDE);
    }

    /**
     *  @brief      Disable address override mode
     *              (禁用地址覆盖模式，恢复系统地址翻译)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_disableAddressOverrideMode(
        FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDCTL &= ~(FLASHCTL_CMDCTL_ADDRXLATEOVR_MASK);
    }

    /**
     *  @brief      Enable overriding hardware generation of ECC code
     *              (启用 ECC 硬件生成覆盖，允许手动提供 ECC)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     * @sa DL_FlashCTL_setCommandDataECC
     */
    __STATIC_INLINE void DL_FlashCTL_enableOverrideHardwareGeneratedECC(
        FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDCTL |= FLASHCTL_CMDCTL_ECCGENOVR_OVERRIDE;
    }

    /**
     *  @brief      Checks if overriding hardware generation of ECC code is enabled
     *              (检查 ECC 硬件生成覆盖是否已启用)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     If overriding hardware generation of ECC code is enabled
     *              (是否已启用)
     */
    __STATIC_INLINE bool DL_FlashCTL_isOverrideHardwareGeneratedECCEnabled(
        const FLASHCTL_Regs *flashctl)
    {
        return ((flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_ECCGENOVR_MASK) ==
                FLASHCTL_CMDCTL_ECCGENOVR_OVERRIDE);
    }

    /**
     *  @brief      Disable overriding hardware generation of ECC code
     *              (禁用 ECC 硬件生成覆盖，恢复自动 ECC)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_disableOverrideHardwareGeneratedECC(
        FLASHCTL_Regs *flashctl)
    {
        flashctl->GEN.CMDCTL &= ~(FLASHCTL_CMDCTL_ECCGENOVR_MASK);
    }

    /**
     *  @brief      Set the region select
     *              (设置目标区域，地址覆盖模式下使用)
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *                          (FLASHCTL 外设寄存器基址)
     *  @param[in]  region      Region to select. One of @ref DL_FLASHCTL_REGION_SELECT
     *                          (目标区域)
     *
     *  @sa DL_FlashCTL_enableAddressOverrideMode
     */
    __STATIC_INLINE void DL_FlashCTL_setRegionSelect(
        FLASHCTL_Regs *flashctl, DL_FLASHCTL_REGION_SELECT region)
    {
        DL_Common_updateReg(&flashctl->GEN.CMDCTL, (uint32_t)region,
                            (uint32_t)FLASHCTL_CMDCTL_REGIONSEL_MASK);
    }

    /**
     *  @brief      Get the region select
     *              (获取当前目标区域设置)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *
     *  @return     The region selected
     *              (当前区域)
     *
     *  @retval     One of @ref DL_FLASHCTL_REGION_SELECT
     */
    __STATIC_INLINE DL_FLASHCTL_REGION_SELECT DL_FlashCTL_getRegionSelect(
        const FLASHCTL_Regs *flashctl)
    {
        uint32_t region = flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_REGIONSEL_MASK;

        return (DL_FLASHCTL_REGION_SELECT)(region);
    }

    /**
     *  @brief      Set the bank select
     *              (设置目标 Bank，地址覆盖模式下使用)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *  @param[in]  bank      Bank to select. One of @ref DL_FLASHCTL_BANK_SELECT
     *                        (目标 Bank)
     *
     *  @sa DL_FlashCTL_enableAddressOverrideMode
     */
    __STATIC_INLINE void DL_FlashCTL_setBankSelect(
        FLASHCTL_Regs *flashctl, DL_FLASHCTL_BANK_SELECT bank)
    {
        DL_Common_updateReg(&flashctl->GEN.CMDCTL, (uint32_t)bank,
                            (uint32_t)FLASHCTL_CMDCTL_BANKSEL_MASK);
    }

    /**
     *  @brief      Get the bank select
     *              (获取当前目标 Bank 设置)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *
     *  @return     The bank selected
     *              (当前 Bank)
     *
     *  @retval     One of @ref DL_FLASHCTL_BANK_SELECT
     */
    __STATIC_INLINE DL_FLASHCTL_BANK_SELECT DL_FlashCTL_getBankSelect(
        const FLASHCTL_Regs *flashctl)
    {
        uint32_t bank = flashctl->GEN.CMDCTL & FLASHCTL_CMDCTL_BANKSEL_MASK;

        return (DL_FLASHCTL_BANK_SELECT)(bank);
    }

    /**
     *  @brief      Set the bytes to enable for programming data
     *              (设置编程字节使能掩码，支持子字编程)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  byteMask   Mask of bytes to enable. Between [0,0x1FF].
     *                         (字节使能掩码)
     */
    __STATIC_INLINE void DL_FlashCTL_setCommandByteEnable(
        FLASHCTL_Regs *flashctl, uint32_t byteMask)
    {
        flashctl->GEN.CMDBYTEN = byteMask;
    }

    /**
     *  @brief      Get the bytes that are enabled for programming data
     *              (获取当前编程字节使能掩码)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *
     *  @return     The enabled bytes
     *              (字节使能掩码)
     *
     *  @retval     Value between [0,0x1FF].
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getCommandByteEnable(
        const FLASHCTL_Regs *flashctl)
    {
        return ((uint32_t)flashctl->GEN.CMDCTL &
                (uint32_t)(FLASHCTL_CMDBYTEN_VAL_MASK));
    }

    /**
     *  @brief      Set the data for a command data register
     *              (向命令数据寄存器写入编程数据)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  index      Select the command data register to write to.
     *                         (CMDDATA 寄存器索引)
     *  @param[in]  data       Data to write to the command data register.
     *                         (待写入数据)
     */
    __STATIC_INLINE void DL_FlashCTL_setCommandData(
        FLASHCTL_Regs *flashctl, uint8_t index, const uint32_t *data)
    {
        volatile uint32_t *pReg = &(flashctl->GEN.CMDDATA0) + index;

        *pReg = *data;
    }

    /**
     *  @brief      Get the data from a command data register
     *              (从命令数据寄存器读取数据)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  index      Select the command data register to read from.
     *                         (CMDDATA 寄存器索引)
     *
     *  @return     The data in the command data register
     *              (寄存器数据)
     *
     *  @retval     Value between [0,0xFFFFFFFF].
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getCommandData(
        FLASHCTL_Regs *flashctl, uint8_t index)
    {
        volatile uint32_t *pReg = &(flashctl->GEN.CMDDATA0);

        return (
            (uint32_t)(*(pReg + index) & (uint32_t)FLASHCTL_CMDDATA0_VAL_MASK));
    }

    /**
     *  @brief      Set the ECC code in the command data ECC register
     *              (向 ECC 数据寄存器手动写入 ECC 码)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  index      Select the command data ECC register to write to.
     *                         (CMDDATAECC 寄存器索引)
     *  @param[in]  data       Data to write to the command data ECC register.
     *                         Value between [0, 0xFF]
     *                         (ECC 码)
     */
    __STATIC_INLINE void DL_FlashCTL_setCommandDataECC(
        FLASHCTL_Regs *flashctl, uint8_t index, const uint8_t *data)
    {
        volatile uint32_t *pReg = &(flashctl->GEN.CMDDATAECC0) + index;

        *pReg = *data;
    }

    /**
     *  @brief      Get the ECC code in the command data ECC register
     *              (从 ECC 数据寄存器读取 ECC 码)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  index      Select the command data ECC register to read from.
     *                         (CMDDATAECC 寄存器索引)
     *
     *  @return     The data in the command data ECC register
     *              (ECC 码)
     *
     *  @retval     Value between [0,0xFF].
     */
    __STATIC_INLINE uint8_t DL_FlashCTL_getCommandDataECC(
        FLASHCTL_Regs *flashctl, uint8_t index)
    {
        volatile uint32_t *pReg = &(flashctl->GEN.CMDDATAECC0);

        return ((uint8_t)(*(pReg + index) & (uint32_t)FLASHCTL_CMDDATAECC0_VAL0_MASK));
    }

    /**
     *  @brief      Sets the target address for a command
     *              (设置 Flash 命令的目标地址，需 64 位对齐)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *  @param[in]  address   Destination address to program/erase/read
     *                        (目标地址)
     */
    __STATIC_INLINE void DL_FlashCTL_setCommandAddress(
        FLASHCTL_Regs *flashctl, uint32_t address)
    {
        flashctl->GEN.CMDADDR = address;
    }

    /**
     *  @brief      Returns the status of the current command
     *              (获取当前命令执行状态)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     The status of the current command
     *              (命令状态)
     *
     *  @retval     One of @ref DL_FLASHCTL_COMMAND_STATUS values
     */
    __STATIC_INLINE DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_getCommandStatus(
        const FLASHCTL_Regs *flashctl)
    {
        uint32_t commandStatus =
            flashctl->GEN.STATCMD &
            (FLASHCTL_STATCMD_CMDDONE_MASK | FLASHCTL_STATCMD_CMDPASS_MASK |
             FLASHCTL_STATCMD_CMDINPROGRESS_MASK |
             FLASHCTL_STATCMD_CMDPASS_STATFAIL);

        return (DL_FLASHCTL_COMMAND_STATUS)(commandStatus);
    }

    /**
     *  @brief      Returns the reason a command failed
     *              (获取命令失败原因)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     The type of command failure
     *              (失败类型)
     *
     *  @retval     One of @ref DL_FLASHCTL_FAIL_TYPE values
     */
    __STATIC_INLINE DL_FLASHCTL_FAIL_TYPE DL_FlashCTL_getFailureStatus(
        const FLASHCTL_Regs *flashctl)
    {
        uint32_t commandFailureType =
            flashctl->GEN.STATCMD &
            (FLASHCTL_STATCMD_FAILWEPROT_MASK | FLASHCTL_STATCMD_FAILVERIFY_MASK |
             FLASHCTL_STATCMD_FAILILLADDR_MASK |
             FLASHCTL_STATCMD_FAILMODE_MASK | FLASHCTL_STATCMD_FAILMISC_MASK);

        return (DL_FLASHCTL_FAIL_TYPE)(commandFailureType);
    }

    /**
     *  @brief      Blocking function that waits for a command execution to finish
     *              (阻塞等待命令执行完成)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the command was successful
     *              (命令是否成功)
     *
     *  @retval     false if command didn't succeed
     *  @retval     true if command was successful
     */
    __STATIC_INLINE bool DL_FlashCTL_waitForCmdDone(const FLASHCTL_Regs *flashctl)
    {
        /* 等待命令完成 */
        while ((flashctl->GEN.STATCMD & FLASHCTL_STATCMD_CMDDONE_MASK) !=
               FLASHCTL_STATCMD_CMDDONE_STATDONE)
        {
            ;
        }

        return ((flashctl->GEN.STATCMD & FLASHCTL_STATCMD_CMDPASS_MASK) ==
                FLASHCTL_STATCMD_CMDPASS_STATPASS);
    }

    /**
     *  @brief      Sets clear status bit and executes command
     *              (清除状态寄存器并恢复最大保护)
     *
     *  @pre        This command should be called before @ref DL_FlashCTL_unprotectSector
     *              to ensure that memory is not re-protected after unprotecting it.
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    __STATIC_INLINE void DL_FlashCTL_executeClearStatus(FLASHCTL_Regs *flashctl)
    {
        /* 设置清除状态命令 */
        flashctl->GEN.CMDTYPE = DL_FLASHCTL_COMMAND_TYPE_CLEAR_STATUS;
        /* 触发命令执行 */
        flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;

        /* 等待清除状态命令完成 */
        while ((FLASHCTL->GEN.STATCMD & FLASHCTL_STATCMD_CMDINPROGRESS_MASK) ==
               FLASHCTL_STATCMD_CMDINPROGRESS_STATINPROGRESS)
        {
            ;
        }
    }

    /**
     *  @brief      Gets the sector number of the input address over the whole
     *              memory map
     *              (获取地址在全局存储映射中的扇区编号)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *  @param[in]  addr      Memory address within the desired sector
     *                        (目标地址)
     *
     *  @return     The sector number that contains the input address
     *              (全局扇区编号)
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getFlashSectorNumber(
        FLASHCTL_Regs *flashctl, uint32_t addr)
    {
        (void)flashctl; /* Suppress unused parameter warning */
        return (addr >> (uint32_t)10);
    }

    /**
     *  @brief      Gets the sector number of the input address relative to the
     *              bank of the address
     *              (获取地址在所属 Bank 内的扇区编号)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *  @param[in]  addr      Memory address within the desired sector
     *                        (目标地址)
     *
     *  @return     The sector number in the current bank that contains the address
     *              (Bank 内扇区编号)
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getFlashSectorNumberInBank(
        FLASHCTL_Regs *flashctl, uint32_t addr)
    {
        uint8_t numBanks = DL_FactoryRegion_getNumBanks();
        uint32_t mainFlashSize = DL_FactoryRegion_getMAINFlashSize();
        uint32_t sector, bankSectors, sectorInBank;

        /* Current sector over the whole memory map */
        sector = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
        if (numBanks > 1)
        {
            /* Number of sectors per bank, considered max sector
             * count for a bank-adjusted sector. Assume banks are
             * evenly distributed */
            bankSectors = (mainFlashSize / numBanks);
            /* We will not assume that the maximum number of bank sectors is a
             * multiple of 2. Which does mean incurring a modulo operation. */
            sectorInBank = sector % bankSectors;
        }
        else
        {
            sectorInBank = sector;
        }

        return sectorInBank;
    }

    /**
     *  @brief      Set the number of wait states used by the Flash
     *              (设置 Flash 等待周期数)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  waitState  Number of wait states to set.
     *                         (等待周期数：0-24MHz 为 0，24-48MHz 为 1，
     *                         48-72MHz 为 2，72-80MHz 为 3)
     */
    __STATIC_INLINE void DL_FlashCTL_setWaitState(
        FLASHCTL_Regs *flashctl, uint32_t waitState)
    {
        flashctl->GEN.CFGCMD = waitState;
    }

    /**
     *  @brief      Get the number of wait states used by the Flash
     *              (获取当前 Flash 等待周期数)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *
     *  @return     The number of wait states used by the Flash
     *              (等待周期数)
     */
    __STATIC_INLINE uint32_t DL_FlashCTL_getWaitState(
        const FLASHCTL_Regs *flashctl)
    {
        return (flashctl->GEN.CFGCMD & FLASHCTL_CFGCMD_WAITSTATE_MASK);
    }

    /**
     *  @brief      Performs an erase on unprotected memory
     *              (对指定地址所在扇区或 Bank 发起擦除命令，非阻塞)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Memory address within desired sector or bank to erase
     *                         (目标地址)
     *  @param[in]  memorySize The size of the memory block to erase.
     *                         (擦除粒度：BANK 或 SECTOR)
     *
     * @post        This API just starts the erase process. Check the result of it
     *              using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     */
    void DL_FlashCTL_eraseMemory(FLASHCTL_Regs *flashctl, uint32_t address,
                                 DL_FLASHCTL_COMMAND_SIZE memorySize);

    /**
     *  @brief      Performs an erase on unprotected memory, and executes command
     *              from RAM
     *              (在 RAM 中执行擦除命令并阻塞等待结果)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Memory address within desired sector or bank to erase
     *                         (目标地址)
     *  @param[in]  memorySize The size of the memory block to erase.
     *                         (擦除粒度：BANK 或 SECTOR)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL_Regs *flashctl, uint32_t address,
        DL_FLASHCTL_COMMAND_SIZE memorySize);

    /**
     *  @brief      Performs a mass erase on main memory on one bank
     *              (对主存储区执行整片擦除，单 Bank 场景)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the erase succeeded
     *              (true 成功，false 失败)
     *
     *  @retval     false if erase didn't succeed
     *  @retval     true  if erase was successful
     */
    bool DL_FlashCTL_massErase(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Performs a mass erase on main memory, and executes command
     *              from RAM
     *              (在 RAM 中执行主存储区整片擦除)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_massEraseFromRAM(
        FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Performs a mass erase on main memory, and erases all flash banks
     *              (逐 Bank 擦除主存储区)
     *
     *  Performs a mass erase on main memory within each flash bank.
     *
     *  NOTE: This API sets all main memory to unprotected from erase/program
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *
     *  @return     Whether or not the erase succeeded
     *
     *  @retval     false if erase didn't succeed
     *  @retval     true  if erase was successful
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     */
    bool DL_FlashCTL_massEraseMultiBank(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Performs a factory reset erase on main and non-main memory
     *              (工厂复位：擦除主存储区和 Non-Main 区，单 Bank)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the erase succeeded
     *              (true 成功，false 失败)
     */
    bool DL_FlashCTL_factoryReset(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Performs a factory reset erase on main and non-main memory, and
     *              executes command from RAM
     *              (在 RAM 中执行工厂复位)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_factoryResetFromRAM(
        FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Performs a factory reset erase on main and non-main memory
     *              (工厂复位：逐 Bank 擦除主存储区和 Non-Main 区)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the erase succeeded
     *              (true 成功，false 失败)
     */
    bool DL_FlashCTL_factoryResetMultiBank(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Programs 8 bit data to unprotected memory at the given address
     *              (发起 8 位编程，不含 ECC，非阻塞)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 8-bit source data (8 位数据指针)
     */
    void DL_FlashCTL_programMemory8(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Programs 8 bit data to unprotected memory at the given address,
     *              and executes command from RAM
     *              (在 RAM 中执行 8 位编程，不含 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 8-bit source data (8 位数据指针)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Programs 16 bit data to unprotected memory at the given address
     *              (发起 16 位编程，不含 ECC，非阻塞)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 16-bit source data (16 位数据指针)
     */
    void DL_FlashCTL_programMemory16(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Programs 16 bit data to unprotected memory at the given
     *              address, and executes command from RAM
     *              (在 RAM 中执行 16 位编程，不含 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 16-bit source data (16 位数据指针)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Programs 32 bit data to unprotected memory at the given address
     *              (发起 32 位编程，不含 ECC，非阻塞)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 32-bit source data (32 位数据指针)
     */
    void DL_FlashCTL_programMemory32(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 32 bit data to unprotected memory at the given
     *              address, and executes command from RAM
     *              (在 RAM 中执行 32 位编程，不含 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 32-bit source data (32 位数据指针)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 64 bit data to unprotected memory at the given address
     *              (发起 64 位编程，不含 ECC，非阻塞)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 32-bit source data (两个 32 位字)
     */
    void DL_FlashCTL_programMemory64(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 64 bit data to unprotected memory at the given
     *              address, and executes command from RAM
     *              (在 RAM 中执行 64 位编程，不含 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 32-bit source data (两个 32 位字)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 8 bit data with hardware generated ECC code
     *              (发起 8 位编程，硬件自动生成 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 8-bit source data (8 位数据指针)
     */
    void DL_FlashCTL_programMemory8WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Programs 8 bit data with hardware generated ECC code, and
     *              executes command from RAM
     *              (在 RAM 中执行 8 位编程，硬件自动 ECC)
     *
     *  Programs 8 bit data, along with the 8 ECC bits which correspond to the
     *  8-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 8-bit source data
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Programs 16 bit data with hardware generated ECC code
     *              (发起 16 位编程，硬件自动生成 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 16-bit source data (16 位数据指针)
     */
    void DL_FlashCTL_programMemory16WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Programs 16 bit data with hardware generated ECC code, and
     *              executes command from RAM
     *              (在 RAM 中执行 16 位编程，硬件自动 ECC)
     *
     *  Programs 16 bit data, along with the 8 ECC bits which correspond to the
     *  16-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *   The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 16-bit source data
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Programs 32 bit data with hardware generated ECC code
     *              (发起 32 位编程，硬件自动生成 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 32-bit source data (32 位数据指针)
     */
    void DL_FlashCTL_programMemory32WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 32 bit data with hardware generated ECC code, and
     *              executes command from RAM
     *              (在 RAM 中执行 32 位编程，硬件自动 ECC)
     *
     *  Programs 32 bit data, along with the 8 ECC bits which correspond to the
     *  32-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *   The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 64 bit data with hardware generated ECC code
     *              (发起 64 位编程，硬件自动生成 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (目标地址)
     *  @param[in]  data       Pointer to the 32-bit source data (两个 32 位字)
     */
    void DL_FlashCTL_programMemory64WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 64 bit data with hardware generated ECC code, and
     *              executes command from RAM
     *              (在 RAM 中执行 64 位编程，硬件自动 ECC)
     *
     *  Programs 16 bit data, along with the 8 ECC bits which correspond to the
     *  16-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *   The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 8 bit data with user provided ECC code
     *              (发起 8 位编程，手动提供 ECC)
     *
     *  Programs 8 bit data, along with the 8 ECC bits which correspond to the
     *  8-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 8-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_programMemory8WithECCManual(FLASHCTL_Regs *flashctl,
                                                 uint32_t address, const uint8_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Programs 8 bit data with user provided ECC code, and executes
     *              command from RAM
     *              (在 RAM 中执行 8 位编程，手动 ECC)
     *
     *  Programs 8 bit data, along with the 8 ECC bits which correspond to the
     *  8-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 8-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Programs 16 bit data with user provided ECC code
     *              (发起 16 位编程，手动提供 ECC)
     *
     *  Programs 16 bit data, along with the 8 ECC bits which correspond to the
     *  16-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 16-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_programMemory16WithECCManual(FLASHCTL_Regs *flashctl,
                                                  uint32_t address, const uint16_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Programs 16 bit data with user provided ECC code, and executes
     *              command from RAM
     *              (在 RAM 中执行 16 位编程，手动 ECC)
     *
     *  Programs 16 bit data, along with the 8 ECC bits which correspond to the
     *  16-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 16-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Programs 32 bit data with user provided ECC code
     *              (发起 32 位编程，手动提供 ECC)
     *
     *  Programs 32 bit data, along with the 8 ECC bits which correspond to the
     *  32-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_programMemory32WithECCManual(FLASHCTL_Regs *flashctl,
                                                  uint32_t address, const uint32_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Programs 32 bit data with user provided ECC code, and executes
     *              command from RAM
     *              (在 RAM 中执行 32 位编程，手动 ECC)
     *
     *  Programs 32 bit data, along with the 8 ECC bits which correspond to the
     *  32-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Programs 64 bit data with user provided ECC code
     *              (发起 64 位编程，手动提供 ECC)
     *
     *  Programs 64 bit data, along with the 8 ECC bits which correspond to the
     *  64-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_programMemory64WithECCManual(FLASHCTL_Regs *flashctl,
                                                  uint32_t address, const uint32_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Programs 64 bit data with user provided ECC code, and executes
     *              command from RAM
     *              (在 RAM 中执行 64 位编程，手动 ECC)
     *
     *  Programs 64 bit data, along with the 8 ECC bits which correspond to the
     *  64-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Programs provided data with hardware generated ECC code
     *              (阻塞式编程，硬件自动 ECC)
     *
     *  Blocking function that programs a set of data, along with the 8 ECC bits
     *  which correspond to the data at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  dataSize      The number of 32-bit words to program. Value
     *                            should be an even number, to ensure data is
     *                            programmed 64-bits at a time.
     *  @param[in]  data          Pointer to the data source to program into flash
     *  @param[in]  regionSelect  The region of memory to erase. One of
     *                            @ref DL_FLASHCTL_REGION_SELECT
     *
     *  @return     Whether or not the program succeeded
     *
     *  @retval     false   Program didn't succeed, or invalid dataSize
     *  @retval     true    Program was successful
     *
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    bool DL_FlashCTL_programMemoryBlocking64WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
        uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Programs provided data with hardware generated ECC code, and
     *              executes command from RAM
     *              (在 RAM 中执行阻塞式编程，硬件自动 ECC)
     *
     *  Blocking function that programs a set of data, along with the 8 ECC bits
     *  which correspond to the data at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  dataSize      The number of 32-bit words to program. Value
     *                            should be an even number, to ensure data is
     *                            programmed 64-bits at a time.
     *  @param[in]  data          Pointer to the data source to program into flash
     *  @param[in]  regionSelect  The region of memory to erase. One of
     *                            @ref DL_FLASHCTL_REGION_SELECT
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
        uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Programs provided data with user provided ECC code
     *              (阻塞式编程，手动提供 ECC)
     *
     *  Blocking function that programs a set of data, along with the 8 ECC bits
     *  which correspond to the data at the given address. This API assumes that
     *  hardware generation of the ECC code HAS been disabled, and so the user
     *  must provide the ECC code to program.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  dataSize      The number of 32-bit words to program. Value
     *                            should be an even number, to ensure data is
     *                            programmed 64-bits at a time.
     *  @param[in]  data          Pointer to the data source to program into flash
     *  @param[in]  eccCode       Pointer to ECC code to program corresponding to data
     *  @param[in]  regionSelect  The region of memory to erase. One of
     *                            @ref DL_FLASHCTL_REGION_SELECT
     *
     *  @return     Whether or not the program succeeded
     *
     *  @retval     false   Program didn't succeed, or invalid dataSize
     *  @retval     true    Program was successful
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    bool DL_FlashCTL_programMemoryBlocking64WithECCManual(FLASHCTL_Regs *flashctl,
                                                          uint32_t address, uint32_t *data, uint8_t *eccCode, uint32_t dataSize,
                                                          DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Programs provided data with user provided ECC code, and
     *              executes command from RAM
     *              (在 RAM 中执行阻塞式编程，手动 ECC)
     *
     *  Blocking function that programs a set of data, along with the 8 ECC bits
     *  which correspond to the data at the given address. This API assumes that
     *  hardware generation of the ECC code HAS been disabled, and so the user
     *  must provide the ECC code to program.
     *
     *  The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  dataSize      The number of 32-bit words to program. Value
     *                            should be an even number, to ensure data is
     *                            programmed 64-bits at a time.
     *  @param[in]  data          Pointer to the data source to program into flash
     *  @param[in]  eccCode       Pointer to ECC code to program corresponding to data
     *  @param[in]  regionSelect  The region of memory to erase. One of
     *                            @ref DL_FLASHCTL_REGION_SELECT
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    DL_FLASHCTL_COMMAND_STATUS
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
        uint8_t *eccCode, uint32_t dataSize,
        DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Programs provided data to unprotected memory at a given address
     *              (阻塞式编程，自动按 64/32 位分段)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (起始地址)
     *  @param[in]  data       Pointer to the data source (源数据指针)
     *  @param[in]  dataSize   The number of 32-bit words to program (32 位字数量)
     *  @param[in]  regionSelect  The region of memory (目标区域)
     *
     *  @return     Whether or not the program succeeded
     *              (true 成功，false 失败)
     */
    bool DL_FlashCTL_programMemoryBlocking(FLASHCTL_Regs *flashctl,
                                           uint32_t address, uint32_t *data, uint32_t dataSize,
                                           DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Programs provided data to unprotected memory at a given
     *              address, and executes command from RAM
     *              (在 RAM 中执行阻塞式分段编程)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  address    Destination memory address (64-bit aligned)
     *                         (起始地址)
     *  @param[in]  data       Pointer to the data source (源数据指针)
     *  @param[in]  dataSize   The number of 32-bit words to program (32 位字数量)
     *  @param[in]  regionSelect  The region of memory (目标区域)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM(
        FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
        uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Unprotects all main memory from erase/program
     *              (取消主存储区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_unprotectMainMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Unprotects all data memory from erase/program
     *              (取消 Data 存储区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_unprotectDataMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Protects all main memory from erase/program
     *              (开启主存储区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_protectMainMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Unprotects all non-main memory from erase/program
     *              (取消 Non-Main 存储区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_unprotectNonMainMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Protects all non-main memory from erase/program
     *              (开启 Non-Main 存储区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_protectNonMainMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Unprotects all user memory from erase/program
     *              (取消全部用户区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_unprotectAllMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Protects all user memory from erase/program
     *              (开启全部用户区写擦保护)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     */
    void DL_FlashCTL_protectAllMemory(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Sets a given sector to unprotected from erase/program
     *              (取消指定地址所在扇区的擦写保护)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  addr       Address of sector to unprotect (扇区内任意地址)
     *  @param[in]  regionSelect  The region of memory (目标区域)
     */
    void DL_FlashCTL_unprotectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
                                     DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Sets a given sector to protected from erase/program
     *              (开启指定地址所在扇区的擦写保护)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *                         (FLASHCTL 外设寄存器基址)
     *  @param[in]  addr       Address of sector to protect (扇区内任意地址)
     *  @param[in]  regionSelect  The region of memory (目标区域)
     */
    void DL_FlashCTL_protectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
                                   DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Verifies 8-bit data in specified address
     *              (对指定地址执行 8 位数据读校验，不含 ECC)
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *                          (FLASHCTL 外设寄存器基址)
     *  @param[in]  address     Memory address of flash to verify (待校验地址)
     *  @param[in]  data        Pointer to the data source to verify (期望数据)
     */
    void DL_FlashCTL_readVerify8(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Verifies 8-bit data in specified address, and executes command
     *              from RAM
     *              (在 RAM 中执行 8 位读校验)
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *                          (FLASHCTL 外设寄存器基址)
     *  @param[in]  address     Memory address of flash to verify (待校验地址)
     *  @param[in]  data        Pointer to the data source to verify (期望数据)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Verifies 16-bit data in specified address
     *              (16 位数据读校验)
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *                          (FLASHCTL 外设寄存器基址)
     *  @param[in]  address     Memory address of flash to verify (待校验地址)
     *  @param[in]  data        Pointer to the data source to verify (期望数据)
     */
    void DL_FlashCTL_readVerify16(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Verifies 16-bit data in specified address, and executes command
     *              from RAM
     *              (在 RAM 中执行 16 位读校验)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Verifies 32-bit data in specified address
     *              (32 位数据读校验)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     */
    void DL_FlashCTL_readVerify32(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 32-bit data in specified address, and executes command
     *              from RAM
     *              (在 RAM 中执行 32 位读校验)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 64-bit data in specified address
     *              (64 位数据读校验)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify.
     *  @param[in]  data        Pointer to the data source to verify
     */
    void DL_FlashCTL_readVerify64(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 64-bit data in specified address, and executes command
     *              from RAM
     *              (在 RAM 中执行 64 位读校验)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify.
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     *
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 8-bit data in specified address with hardware
     *              generated ECC code
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify8WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Verifies 8-bit data in specified address with hardware generated
     *              ECC code, and executes command from RAM
     *              (在 RAM 中执行 8 位读校验，硬件自动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data);

    /**
     *  @brief      Verifies 16-bit data in specified address with hardware
     *              generated ECC code
     *              (16 位数据读校验，硬件自动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify16WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Verifies 16-bit data in specified address with hardware
     *              generated ECC code, and executes command from RAM
     *              (在 RAM 中执行 16 位读校验，硬件自动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data);

    /**
     *  @brief      Verifies 32-bit data in specified address with hardware
     *              generated ECC code
     *              (32 位数据读校验，硬件自动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify32WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 32-bit data in specified address with hardware
     *              generated ECC code, and executes command from RAM
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 64-bit data in specified address with hardware
     *              generated ECC code
     *              (64 位数据读校验，硬件自动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify64WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 32-bit data in specified address with hardware
     *              generated ECC code, and executes command from RAM
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code has NOT
     * been disabled, and so the flash controller will generate the ECC bits.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Verifies 8-bit data in specified address with user provided ECC
     *              code
     *              (8 位数据读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify8WithECCManual(FLASHCTL_Regs *flashctl,
                                              uint32_t address, const uint8_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Verifies 8-bit data in specified address with user provided ECC
     *              code, and executes command from RAM
     *              (在 RAM 中执行 8 位读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Verifies 16-bit data in specified address with user provided ECC
     *              code
     *              (16 位数据读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify16WithECCManual(FLASHCTL_Regs *flashctl,
                                               uint32_t address, const uint16_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Verifies 16-bit data in specified address with user provided ECC
     *              code, and executes command from RAM
     *              (在 RAM 中执行 16 位读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Verifies 32-bit data in specified address with user provided ECC
     *              code
     *              (32 位数据读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify32WithECCManual(FLASHCTL_Regs *flashctl,
                                               uint32_t address, const uint32_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Verifies 32-bit data in specified address with user provided ECC
     *              code, and executes command from RAM
     *              (在 RAM 中执行 32 位读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Verifies 64-bit data in specified address with user provided ECC
     *              code
     *              (64 位数据读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_readVerify64WithECCManual(FLASHCTL_Regs *flashctl,
                                               uint32_t address, const uint32_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Verifies 64-bit data in specified address with user provided ECC
     *              code, and executes command from RAM
     *              (在 RAM 中执行 64 位读校验，手动 ECC)
     *
     * The READVERIFY command may be used to read a flash location and compare it
     * to data to be verified along with the 8 ECC bits which correspond to the
     * data. This API assumes that hardware generation of the ECC code HAS been
     * disabled, so the user must provide the ECC code to program.
     *
     * The command is executed from RAM, and blocks until the command is finished.
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *  @param[in]  address     Memory address of flash to verify
     *  @param[in]  data        Pointer to the data source to verify
     *  @param[in]  eccCode     Pointer to ECC code to program corresponding to data
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     *
     *  @return     Whether or not the command was successful.
     *              One of @ref DL_FLASHCTL_COMMAND_STATUS
     *
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_FAILED   if command didn't succeed
     *  @retval     DL_FLASHCTL_COMMAND_STATUS_PASSED   if command was successful
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCManual(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
        const uint8_t *eccCode);

    /**
     *  @brief      Verifies that a flash word is blank
     *              (发起空白校验命令，检查 Flash 字是否仍为擦除态)
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *                          (FLASHCTL 外设寄存器基址)
     *  @param[in]  address     Memory address of flash to verify (待校验地址)
     */
    void DL_FlashCTL_blankVerify(FLASHCTL_Regs *flashctl, uint32_t address);

    /**
     *  @brief      Verifies that a flash word is blank, and executes command from
     *              RAM
     *              (在 RAM 中执行空白校验)
     *
     *  @param[in]  flashctl    Pointer to the register overlay for the peripheral
     *                          (FLASHCTL 外设寄存器基址)
     *  @param[in]  address     Memory address of flash to verify (待校验地址)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_blankVerifyFromRAM(
        FLASHCTL_Regs *flashctl, uint32_t address);

    /**
     *  @brief      Programs 128 bit data to unprotected memory at the given address
     *              (发起 128 位编程，不含 ECC)
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: This API does not enable programming the ECC code.
     */
    void DL_FlashCTL_programMemory128(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    /**
     *  @brief      Programs 128 bit data with hardware generated ECC code
     *              (发起 128 位编程，硬件自动 ECC)
     *
     *  Programs 128 bit data, along with the 16 ECC bits which correspond to the
     *  128-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code has NOT been disabled,
     *  and so the flash controller will generate the ECC bits.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_programMemory128WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data);

    // /**
    //  *  @brief      Programs 128 bit data with hardware generated ECC code
    //  *
    //  *  Programs 128 bit data, along with the 16 ECC bits which correspond to the
    //  *  128-bit data word, to unprotected memory at the given address. This API
    //  *  assumes that hardware generation of the ECC code has NOT been disabled,
    //  *  and so the flash controller will generate the ECC bits.
    //  *
    //  *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
    //  *  @param[in]  address    Destination memory address to program data. The
    //  *                         address must be flash word (64-bit) aligned i.e.
    //  *                         aligned to a 0b000 boundary.
    //  *  @param[in]  data       Pointer to the 32-bit source data
    //  *
    //  *  @pre         Before programming memory, the user must unprotect the region
    //  *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
    //  *  @post        This API just starts the program process. Check the result of it
    //  *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
    //  *
    //  *  NOTE: After completion of a program operation, the flash controller will
    //  *  configure all memory to a protected state.
    //  *  NOTE: After completion of a program operation, the flash controller will
    //  *  disable programming of the ECC code (regardless of whether ECC code is
    //  *  hardware generated or manually provided).
    //  */
    // void DL_FlashCTL_programMemory128WithECCGenerated(
    //     FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data);

    /**
     *  @brief      Programs 128 bit data with user provided ECC code
     *
     *  Programs 128 bit data, along with the 16 ECC bits which correspond to the
     *  128-bit data word, to unprotected memory at the given address. This API
     *  assumes that hardware generation of the ECC code HAS been disabled,
     *  and so the user must provide the ECC code to program.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  data       Pointer to the 32-bit source data
     *  @param[in]  eccCode    Pointer to the ECC code to program
     *
     *  @pre         User must call @ref DL_FlashCTL_enableOverrideHardwareGeneratedECC
     *               to disable hardware generation of the ECC code, so the ECC code
     *               can be manually provided for programming. This override setting
     *               will persist through multiple programs, until
     *               @ref DL_FlashCTL_disableOverrideHardwareGeneratedECC is called
     *  @pre         Before programming memory, the user must unprotect the region
     *               of memory to program. Refer to @ref DL_FlashCTL_unprotectSector
     *  @post        This API just starts the program process. Check the result of it
     *               using an interrupt or the @ref DL_FlashCTL_waitForCmdDone API
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: After completion of a program operation, the flash controller will
     *  disable programming of the ECC code (regardless of whether ECC code is
     *  hardware generated or manually provided).
     */
    void DL_FlashCTL_programMemory128WithECCManual(FLASHCTL_Regs *flashctl,
                                                   uint32_t address, const uint32_t *data, const uint8_t *eccCode);

    /**
     *  @brief      Programs provided data up to 128-bits with ECC generated
     *              while blocking between writes
     *              (阻塞式编程，优先 128 位，硬件自动 ECC)
     *
     *  Blocking function that programs a set of data using multi-word programming
     *  for up to 2 flash words. Refer to the device datasheet if the device
     *  supports multi-word programming.
     *  When possible, the data will be programmed as either 64-bit data or as
     *  32-bit data.
     *
     *  @param[in]  flashctl   Pointer to the register overlay for the peripheral
     *  @param[in]  address    Destination memory address to program data. The
     *                         address must be flash word (64-bit) aligned i.e.
     *                         aligned to a 0b000 boundary.
     *  @param[in]  dataSize      The number of 32-bit words to program
     *  @param[in]  data          Pointer to the data source to program into flash
     *  @param[in]  regionSelect  The region of memory to erase. One of
     *                            @ref DL_FLASHCTL_REGION_SELECT
     *
     *  @return     Whether or not the program succeeded
     *
     *  @retval     false   Program didn't succeed
     *  @retval     true    Program was successful
     *
     *  NOTE: After completion of a program operation, the flash controller will
     *  configure all memory to a protected state.
     *  NOTE: This API does not enable programming the ECC code.
     */
    bool DL_FlashCTL_programMemoryBlocking128WithECCGenerated(
        FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
        uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect);

    /**
     *  @brief      Performs an erase on DATA bank
     *              (逐扇区擦除 DATA Bank)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the erase succeeded
     *              (true 成功，false 失败)
     */
    bool DL_FlashCTL_eraseDataBank(FLASHCTL_Regs *flashctl);

    /**
     *  @brief      Performs an erase on DATA bank, and executes command
     *              from RAM
     *              (在 RAM 中逐扇区擦除 DATA Bank)
     *
     *  @param[in]  flashctl  Pointer to the register overlay for the peripheral
     *                        (FLASHCTL 外设寄存器基址)
     *
     *  @return     Whether or not the command was successful.
     *              (命令执行状态)
     */
    DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseDataBankFromRAM(
        FLASHCTL_Regs *flashctl);

#ifdef __cplusplus
}
#endif

#endif /* ti_dl_dl_flashctl__include */
/** @}*/
