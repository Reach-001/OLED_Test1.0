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

#include <stdbool.h>
#include <stdint.h>
#include <ti/devices/DeviceFamily.h>

#include <ti/driverlib/dl_flashctl.h>

static void DL_FlashCTL_programMemoryConfig(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t cmd);
static DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_executeCommandFromRAM(
    FLASHCTL_Regs *flashctl);
static void DL_FlashCTL_programMemory8Config(FLASHCTL_Regs *flashctl,
                                             uint32_t address, uint32_t cmd, const uint8_t *data);
static void DL_FlashCTL_programMemory16Config(FLASHCTL_Regs *flashctl,
                                              uint32_t address, uint32_t cmd, const uint16_t *data);
static void DL_FlashCTL_programMemory32Config(FLASHCTL_Regs *flashctl,
                                              uint32_t address, uint32_t cmd, const uint32_t *data);
static void DL_FlashCTL_programMemory64Config(FLASHCTL_Regs *flashctl,
                                              uint32_t address, uint32_t cmd, const uint32_t *data);

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define RAMFUNC \
    __attribute__((section(".TI.ramfunc"))) __attribute__((noinline))
#elif defined(__GNUC__)
#define RAMFUNC __attribute__((section(".ramfunc"))) __attribute__((noinline))
#elif defined(__IAR_SYSTEMS_ICC__)
#define RAMFUNC __ramfunc __attribute__((noinline))
#else
#error "Compiler not supported for this function"
#endif

/**
 * @brief 在 RAM 中执行已配置的 Flash 命令并阻塞等待结束。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 命令执行状态（通过/失败）。
 */
RAMFUNC static DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_executeCommandFromRAM(
    FLASHCTL_Regs *flashctl)
{
    volatile uint32_t status;
    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;

    /*
     * 轮询 STATCMD 寄存器直到命令完成或失败，
     * 确保在最高 CPU 频率下也能正确轮询。
     */
    do
    {
        status =
            flashctl->GEN.STATCMD &
            (FLASHCTL_STATCMD_CMDDONE_MASK | FLASHCTL_STATCMD_CMDPASS_MASK |
             FLASHCTL_STATCMD_CMDINPROGRESS_MASK |
             FLASHCTL_STATCMD_CMDPASS_STATFAIL);
    } while ((DL_FLASHCTL_COMMAND_STATUS)status !=
                 (DL_FLASHCTL_COMMAND_STATUS_PASSED) &&
             (DL_FLASHCTL_COMMAND_STATUS)status !=
                 (DL_FLASHCTL_COMMAND_STATUS_FAILED));

    return ((DL_FLASHCTL_COMMAND_STATUS)status);
}

/**
 * @brief 发起擦除命令（异步）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址（对应待擦除扇区或 Bank）。
 * @param[in] memorySize 擦除粒度（扇区或 Bank）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_eraseMemory(FLASHCTL_Regs *flashctl, uint32_t address,
                             DL_FLASHCTL_COMMAND_SIZE memorySize)
{
    /* 设置命令类型和擦除粒度 */
    flashctl->GEN.CMDTYPE =
        (uint32_t)memorySize | DL_FLASHCTL_COMMAND_TYPE_ERASE;

    /* 设置目标地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行擦除命令并阻塞等待结果。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址（对应待擦除扇区或 Bank）。
 * @param[in] memorySize 擦除粒度（扇区或 Bank）。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseMemoryFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address,
    DL_FLASHCTL_COMMAND_SIZE memorySize)
{
    /* 设置命令类型和擦除粒度 */
    flashctl->GEN.CMDTYPE =
        (uint32_t)memorySize | DL_FLASHCTL_COMMAND_TYPE_ERASE;

    /* 设置目标地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 主存储区整片擦除（Bank0 + 可选 Data Bank）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return true 表示全部擦除成功，false 表示中途失败。
 */
bool DL_FlashCTL_massErase(FLASHCTL_Regs *flashctl)
{
    bool status;

    DL_FlashCTL_unprotectMainMemory(flashctl);
    DL_FlashCTL_protectNonMainMemory(flashctl);

    DL_FlashCTL_eraseMemory(
        flashctl, FLASHCTL_BANK0_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_BANK);
    status = DL_FlashCTL_waitForCmdDone(flashctl);

    if (DL_FactoryRegion_getDATAFlashSize() && (status == true))
    {
        /* 设备存在 Data Flash 时，追加擦除 Data Bank */
        status = DL_FlashCTL_eraseDataBank(flashctl);
    }
    return (status);
}

/**
 * @brief 在 RAM 中执行主存储区整片擦除。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_massEraseFromRAM(
    FLASHCTL_Regs *flashctl)
{
    DL_FLASHCTL_COMMAND_STATUS status;

    DL_FlashCTL_unprotectMainMemory(flashctl);
    DL_FlashCTL_protectNonMainMemory(flashctl);

    status = DL_FlashCTL_eraseMemoryFromRAM(
        flashctl, FLASHCTL_BANK0_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_BANK);

    if (DL_FactoryRegion_getDATAFlashSize() &&
        (status != DL_FLASHCTL_COMMAND_STATUS_FAILED))
    {
        /* 主擦除成功后再擦除 Data Bank */
        status = DL_FlashCTL_eraseDataBankFromRAM(flashctl);
    }

    return (status);
}

/**
 * @brief 多 Bank 设备逐 Bank 擦除主存储区。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return true 表示擦除成功，false 表示失败。
 */
bool DL_FlashCTL_massEraseMultiBank(FLASHCTL_Regs *flashctl)
{
    bool status = true;
    uint32_t bankStartAddr = 0x0;
    uint8_t numBanks = DL_FactoryRegion_getNumBanks();
    uint32_t flashSize = DL_FactoryRegion_getMAINFlashSize();
    /* Assuming a sector size of 1KB */
    uint32_t bankSize = (((uint32_t)flashSize / (uint32_t)numBanks) * 1024U);

    DL_FLASHCTL_BANK_SELECT bankSelect = DL_FLASHCTL_BANK_SELECT_0;

    bool eraseFlag = true;
    uint8_t bankIndex = 0;
    while (bankIndex < numBanks && status != false)
    {
        /* If flash bank swap policy is enabled, the primary bank will be write
         * and erase protected. Thus, we will not attempt an erase of the
         * primary bank.
         */
        if (DL_SYSCTL_isFlashBankSwapEnabled())
        {
            eraseFlag = (bankIndex < (numBanks / (uint8_t)2)) ? false : true;
        }
        else
        {
            eraseFlag = true;
        }
        switch (bankIndex)
        {
        case 0:
            bankSelect = DL_FLASHCTL_BANK_SELECT_0;
            break;
        case 1:
            bankSelect = DL_FLASHCTL_BANK_SELECT_1;
            break;
        case 2:
            bankSelect = DL_FLASHCTL_BANK_SELECT_2;
            break;
        case 3:
            bankSelect = DL_FLASHCTL_BANK_SELECT_3;
            break;
        default:
            break;
        }

        bankStartAddr = (bankSize * bankIndex);

        if (eraseFlag == true)
        {
            /* 切换到地址覆盖模式，按 bank/region 直接定位目标 */
            DL_FlashCTL_enableAddressOverrideMode(flashctl);
            DL_FlashCTL_setBankSelect(flashctl, bankSelect);
            DL_FlashCTL_setRegionSelect(
                flashctl, DL_FLASHCTL_REGION_SELECT_MAIN);

            DL_FlashCTL_unprotectMainMemory(flashctl);
            DL_FlashCTL_protectNonMainMemory(flashctl);
            DL_FlashCTL_eraseMemory(
                flashctl, bankStartAddr, DL_FLASHCTL_COMMAND_SIZE_BANK);
            status = DL_FlashCTL_waitForCmdDone(flashctl);
        }
        bankIndex++;
    }
    /* 退出覆盖模式，恢复系统地址翻译 */
    DL_FlashCTL_disableAddressOverrideMode(flashctl);

    if (DL_FactoryRegion_getDATAFlashSize() && status == true)
    {
        status = DL_FlashCTL_eraseDataBank(flashctl);
    }

    return (status);
}

/**
 * @brief 工厂复位：擦除主存储区并擦除 Non-Main 区。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return true 表示复位擦除成功，false 表示失败。
 */
bool DL_FlashCTL_factoryReset(FLASHCTL_Regs *flashctl)
{
    bool status;

    /* 擦除主存储区 */
    status = DL_FlashCTL_massErase(flashctl);

    if (status == true)
    {
        DL_FlashCTL_unprotectNonMainMemory(flashctl);
        DL_FlashCTL_eraseMemory(flashctl, FLASHCTL_NONMAIN_ADDRESS,
                                DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }
    return (status);
}

/**
 * @brief 在 RAM 中执行工厂复位流程。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_factoryResetFromRAM(
    FLASHCTL_Regs *flashctl)
{
    DL_FLASHCTL_COMMAND_STATUS status;

    /* 擦除主存储区 */
    status = DL_FlashCTL_massEraseFromRAM(flashctl);

    if (status == DL_FLASHCTL_COMMAND_STATUS_PASSED)
    {
        DL_FlashCTL_unprotectNonMainMemory(flashctl);
        status = DL_FlashCTL_eraseMemoryFromRAM(flashctl,
                                                FLASHCTL_NONMAIN_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    }
    return (status);
}

/**
 * @brief 多 Bank 工厂复位：逐 Bank 擦主存储区并擦 Non-Main。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return true 表示成功，false 表示失败。
 */
bool DL_FlashCTL_factoryResetMultiBank(FLASHCTL_Regs *flashctl)
{
    bool status;

    /* 擦除主存储区 */
    status = DL_FlashCTL_massEraseMultiBank(flashctl);

    if (status == true)
    {
        DL_FlashCTL_unprotectNonMainMemory(flashctl);
        DL_FlashCTL_eraseMemory(flashctl, FLASHCTL_NONMAIN_ADDRESS,
                                DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }
    return (status);
}

/**
 * @brief 配置基础编程命令参数（类型/字节掩码/地址）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码（CMDBYTEN）。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemoryConfig(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t cmd)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            DL_FLASHCTL_COMMAND_TYPE_PROGRAM;

    flashctl->GEN.CMDBYTEN = cmd;

    /* 设置目标地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);
}

/**
 * @brief 配置 8 位编程命令与数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 8 位数据指针。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemory8Config(FLASHCTL_Regs *flashctl,
                                             uint32_t address, uint32_t cmd, const uint8_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* 写入数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 配置 16 位编程命令与数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 16 位数据指针。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemory16Config(FLASHCTL_Regs *flashctl,
                                              uint32_t address, uint32_t cmd, const uint16_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* 写入数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 配置 32 位编程命令与数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 32 位数据指针。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemory32Config(FLASHCTL_Regs *flashctl,
                                              uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* 写入数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 配置 64 位编程命令与数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 64 位数据指针（以两个 32 位字表示）。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemory64Config(FLASHCTL_Regs *flashctl,
                                              uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_programMemoryConfig(flashctl, address, cmd);

    /* 写入数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
}

/**
 * @brief 发起 8 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 8 位源数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低 8 位进行编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITHOUT_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 8 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 8 位源数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低 8 位进行编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITHOUT_ECC, data);
    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 16 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 16 位源数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* 使能 16 位编程 */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITHOUT_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 16 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 16 位源数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* 使能 16 位编程 */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 32 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 32 位源数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 32 位编程 */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITHOUT_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 32 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 32 位源数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 32 位编程 */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 64 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 64 位数据指针（两个 32 位字）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 64 位编程 */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITHOUT_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 64 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 64 位数据指针（两个 32 位字）。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 64 位编程 */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 8 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 8 位源数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低 8 位进行编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 8 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 8 位源数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    /* 仅使能低 8 位进行编程 */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 16 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 16 位源数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* 使能 16 位编程 */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 16 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 16 位源数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    /* 使能 16 位编程 */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 32 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 32 位源数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 32 位编程 */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);
    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 32 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 32 位源数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 32 位编程 */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);
    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 64 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 64 位数据指针（两个 32 位字）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 64 位编程，带 ECC */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 64 位编程命令（硬件自动生成 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 64 位数据指针（两个 32 位字）。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 64 位编程，带 ECC */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 8 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 8 位源数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory8WithECCManual(FLASHCTL_Regs *flashctl,
                                             uint32_t address, const uint8_t *data, const uint8_t *eccCode)
{
    /* 使能 8 位编程，带 ECC */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 8 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 8 位源数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM8WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
    const uint8_t *eccCode)
{
    /* 使能 8 位编程，带 ECC */
    DL_FlashCTL_programMemory8Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_8_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 16 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 16 位源数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory16WithECCManual(FLASHCTL_Regs *flashctl,
                                              uint32_t address, const uint16_t *data, const uint8_t *eccCode)
{
    /* 使能 16 位编程，带 ECC */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 16 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 16 位源数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM16WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
    const uint8_t *eccCode)
{
    /* 使能 16 位编程，带 ECC */
    DL_FlashCTL_programMemory16Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_16_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 32 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 32 位源数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory32WithECCManual(FLASHCTL_Regs *flashctl,
                                              uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    /* 使能 32 位编程，带 ECC */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 32 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 32 位源数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM32WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    /* 使能 32 位编程，带 ECC */
    DL_FlashCTL_programMemory32Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_32_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 64 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 64 位数据指针（两个 32 位字）。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory64WithECCManual(FLASHCTL_Regs *flashctl,
                                              uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    /* 使能 64 位编程，带 ECC */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 64 位编程命令（手动提供 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 64 位数据指针（两个 32 位字）。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    /* 使能 64 位编程，带 ECC */
    DL_FlashCTL_programMemory64Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_64_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 阻塞写入 64 位数据并由硬件生成 ECC。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针（按 32 位字组织）。
 * @param[in] dataSize 32 位字数量（需为偶数）。
 * @param[in] regionSelect 目标区域。
 *
 * @return true 表示成功，false 表示失败。
 */
bool DL_FlashCTL_programMemoryBlocking64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* 检查数据大小有效性 */
    if (dataSize == (uint32_t)0 ||
        ((dataSize & (uint32_t)1) == (uint32_t)1))
    {
        status = false;
    }

    while ((dataSize != (uint32_t)0) && status)
    {
        /* 执行 Flash 操作前清除状态寄存器 */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        DL_FlashCTL_programMemory64WithECCGenerated(flashctl, address, data);
        dataSize = dataSize - (uint32_t)2;
        data = data + 2;
        address = address + (uint32_t)8;

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

/**
 * @brief 在 RAM 中执行阻塞 64 位编程（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针（按 32 位字组织）。
 * @param[in] dataSize 32 位字数量（需为偶数）。
 * @param[in] regionSelect 目标区域。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS
DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS;

    /* 检查数据大小有效性 */
    if (dataSize == (uint32_t)0 ||
        ((dataSize & (uint32_t)1) == (uint32_t)1))
    {
        status = DL_FLASHCTL_COMMAND_STATUS_FAILED;
    }

    while ((dataSize != (uint32_t)0) &&
           (status != DL_FLASHCTL_COMMAND_STATUS_FAILED))
    {
        /* 执行 Flash 操作前清除状态寄存器 */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        status = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
            flashctl, address, data);
        dataSize = dataSize - (uint32_t)2;
        data = data + 2;
        address = address + (uint32_t)8;
    }

    return (status);
}

/**
 * @brief 阻塞写入 64 位数据并手动提供 ECC。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针（按 32 位字组织）。
 * @param[in] eccCode ECC 数据指针。
 * @param[in] dataSize 32 位字数量（需为偶数）。
 * @param[in] regionSelect 目标区域。
 *
 * @return true 表示成功，false 表示失败。
 */
bool DL_FlashCTL_programMemoryBlocking64WithECCManual(FLASHCTL_Regs *flashctl,
                                                      uint32_t address, uint32_t *data, uint8_t *eccCode, uint32_t dataSize,
                                                      DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* 检查数据大小有效性 */
    if (dataSize == (uint32_t)0 ||
        ((dataSize & (uint32_t)1) == (uint32_t)1))
    {
        status = false;
    }

    while ((dataSize != (uint32_t)0) && status)
    {
        /* 执行 Flash 操作前清除状态寄存器 */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        DL_FlashCTL_programMemory64WithECCManual(
            flashctl, address, data, eccCode);
        dataSize = dataSize - (uint32_t)2;
        data = data + 2;
        eccCode = eccCode + 1;
        address = address + (uint32_t)8;

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

/**
 * @brief 在 RAM 中执行阻塞 64 位编程（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针（按 32 位字组织）。
 * @param[in] eccCode ECC 数据指针。
 * @param[in] dataSize 32 位字数量（需为偶数）。
 * @param[in] regionSelect 目标区域。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS
DL_FlashCTL_programMemoryBlockingFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint8_t *eccCode, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS;

    /* 检查数据大小有效性 */
    if (dataSize == (uint32_t)0 ||
        ((dataSize & (uint32_t)1) == (uint32_t)1))
    {
        status = DL_FLASHCTL_COMMAND_STATUS_FAILED;
    }

    while ((dataSize != (uint32_t)0) &&
           (status != DL_FLASHCTL_COMMAND_STATUS_FAILED))
    {
        /* 执行 Flash 操作前清除状态寄存器 */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        status = DL_FlashCTL_programMemoryFromRAM64WithECCManual(
            flashctl, address, data, eccCode);
        dataSize = dataSize - (uint32_t)2;
        data = data + 2;
        eccCode = eccCode + 1;
        address = address + (uint32_t)8;
    }

    return (status);
}

/**
 * @brief 阻塞写入数据（按 32/64 位自动分段）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针。
 * @param[in] dataSize 32 位字数量。
 * @param[in] regionSelect 目标区域。
 *
 * @return true 表示成功，false 表示失败。
 */
bool DL_FlashCTL_programMemoryBlocking(FLASHCTL_Regs *flashctl,
                                       uint32_t address, uint32_t *data, uint32_t dataSize,
                                       DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* 检查数据大小有效性 */
    if (dataSize == (uint32_t)0)
    {
        status = false;
    }

    while ((dataSize != (uint32_t)0) && status)
    {
        /* 执行 Flash 操作前清除状态寄存器 */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32 位情况 */
        if (dataSize == (uint32_t)1)
        {
            DL_FlashCTL_programMemory32(flashctl, address, data);

            dataSize = dataSize - (uint32_t)1;
            data = data + 1;
            address = address + (uint32_t)4;
        }
        else
        {
            /* 64 位情况 */
            DL_FlashCTL_programMemory64(flashctl, address, data);
            dataSize = dataSize - (uint32_t)2;
            data = data + 2;
            address = address + (uint32_t)8;
        }

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

/**
 * @brief 在 RAM 中执行阻塞写入（按 32/64 位自动分段）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针。
 * @param[in] dataSize 32 位字数量。
 * @param[in] regionSelect 目标区域。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_programMemoryFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_IN_PROGRESS;

    /* 检查数据大小有效性 */
    if (dataSize == (uint32_t)0)
    {
        status = DL_FLASHCTL_COMMAND_STATUS_FAILED;
    }

    while ((dataSize != (uint32_t)0) &&
           (status != DL_FLASHCTL_COMMAND_STATUS_FAILED))
    {
        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32 位情况 */
        if (dataSize == (uint32_t)1)
        {
            status =
                DL_FlashCTL_programMemoryFromRAM32(flashctl, address, data);

            dataSize = dataSize - (uint32_t)1;
            data = data + 1;
            address = address + (uint32_t)4;
        }
        else
        {
            /* 64 位情况 */
            status =
                DL_FlashCTL_programMemoryFromRAM64(flashctl, address, data);
            dataSize = dataSize - (uint32_t)2;
            data = data + 2;
            address = address + (uint32_t)8;
        }
    }

    return (status);
}

/**
 * @brief 取消主存储区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_unprotectMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = 0;
    flashctl->GEN.CMDWEPROTB = 0;
    flashctl->GEN.CMDWEPROTC = 0;
}

/**
 * @brief 取消 Data 存储区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_unprotectDataMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = 0;
    flashctl->GEN.CMDWEPROTB = 0;
    flashctl->GEN.CMDWEPROTC = 0;
}

/**
 * @brief 开启主存储区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_protectMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = FLASHCTL_CMDWEPROTA_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTB = FLASHCTL_CMDWEPROTB_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTC = FLASHCTL_CMDWEPROTC_VAL_MAXIMUM;
}

/**
 * @brief 取消 Non-Main 存储区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_unprotectNonMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTNM = 0;
}

/**
 * @brief 开启 Non-Main 存储区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_protectNonMainMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTNM = FLASHCTL_CMDWEPROTNM_VAL_MAXIMUM;
}

/**
 * @brief 取消全部用户区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_unprotectAllMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = 0;
    flashctl->GEN.CMDWEPROTB = 0;
    flashctl->GEN.CMDWEPROTC = 0;
    flashctl->GEN.CMDWEPROTNM = 0;
    flashctl->GEN.CMDWEPROTTR = 0;
    flashctl->GEN.CMDWEPROTEN = 0;
}

/**
 * @brief 开启全部用户区写擦保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_protectAllMemory(FLASHCTL_Regs *flashctl)
{
    flashctl->GEN.CMDWEPROTA = FLASHCTL_CMDWEPROTA_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTB = FLASHCTL_CMDWEPROTB_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTC = FLASHCTL_CMDWEPROTC_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTNM = FLASHCTL_CMDWEPROTNM_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTTR = FLASHCTL_CMDWEPROTTR_VAL_MAXIMUM;
    flashctl->GEN.CMDWEPROTEN = FLASHCTL_CMDWEPROTEN_VAL_MAXIMUM;
}

#ifdef DEVICE_HAS_NO_CMDWEPROTA
/**
 * @brief 在无 CMDWEPROTA 器件上取消指定扇区保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] addr 目标扇区地址。
 * @param[in] regionSelect 目标区域。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_unprotectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
                                 DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint32_t sectorMask;

    /*
     * Devices without CMDWEPROTA will use CMDWEPROTB to unprotect all sectors of MAIN memory
     */

    if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN)
    {
        sectorMask = (uint32_t)1
                     << ((sectorInBank / (uint32_t)8) % (uint32_t)32);
        flashctl->GEN.CMDWEPROTB &= ~sectorMask;
    }
    else if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN)
    {
        sectorMask = (uint32_t)1 << (sectorNumber % (uint32_t)32);
        flashctl->GEN.CMDWEPROTNM &= ~sectorMask;
    }
    else
    {
        ; /* Not expected to reach this else statement */
    }
}
#else
/**
 * @brief 取消指定扇区保护（支持 Bank Swap 物理映射处理）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] addr 目标扇区地址。
 * @param[in] regionSelect 目标区域。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_unprotectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
                                 DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint8_t numBanks = DL_FactoryRegion_getNumBanks();
    uint32_t mainFlashSize = DL_FactoryRegion_getMAINFlashSize();
    uint32_t physicalSectorNumber = 0;
    uint32_t sectorMask;

    if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN)
    {
        /* If the banks have been swapped, CMDWEPROTA only protects physical
         * bank 0 (logical bank 1 in a swap), so if the address points to this
         * region we must protect it using CMDWEPROTA instead of CMDWEPROTB
         */
        if (DL_SYSCTL_isExecuteFromUpperFlashBank() && numBanks > 1)
        {
            /* physical sectors are swapped. Calculate physical sector to
             * determine use of CMDWEPROTA */
            if (sectorNumber >= (mainFlashSize / 2))
            {
                physicalSectorNumber = sectorNumber - (mainFlashSize / 2);
            }
            else
            {
                physicalSectorNumber = sectorNumber + (mainFlashSize / 2);
            }
        }
        else
        {
            physicalSectorNumber = sectorNumber;
        }

        if (physicalSectorNumber < (uint32_t)32)
        {
            /* Use CMDWEPROTA */
            sectorMask = (uint32_t)1 << physicalSectorNumber;
            flashctl->GEN.CMDWEPROTA &= ~sectorMask;
        }
        else
        {
            /* Use CMDWEPROTB */
            if (sectorInBank < (uint32_t)256)
            {
                /* Single bank system */
                if (DL_FactoryRegion_getNumBanks() == (uint32_t)1)
                {
                    sectorMask =
                        (uint32_t)1
                        << ((sectorInBank - (uint32_t)32) / (uint32_t)8);
                    flashctl->GEN.CMDWEPROTB &= ~sectorMask;
                }
                else
                { /* Multi bank system */
                    sectorMask = (uint32_t)1 << (sectorInBank / (uint32_t)8);
                    flashctl->GEN.CMDWEPROTB &= ~sectorMask;
                }
            }
            /* Use CMDWEPROTC */
            else if (sectorInBank < (uint32_t)511)
            {
                sectorMask =
                    ((uint32_t)1
                     << ((sectorInBank - (uint32_t)256) / (uint32_t)8));
                flashctl->GEN.CMDWEPROTC &= ~sectorMask;
            }
            else
            {
                ; /* Not expected to reach this else statement */
            }
        }
    }
    else if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN)
    {
        sectorMask = (uint32_t)1 << (sectorNumber % (uint32_t)32);
        flashctl->GEN.CMDWEPROTNM &= ~sectorMask;
    }
    else
    {
        ; /* Not expected to reach this else statement */
    }
}
#endif

#ifdef DEVICE_HAS_NO_CMDWEPROTA
/**
 * @brief 在无 CMDWEPROTA 器件上开启指定扇区保护。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] addr 目标扇区地址。
 * @param[in] regionSelect 目标区域。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_protectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
                               DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint32_t sectorMask;

    /*
     * Devices without CMDWEPROTA will use CMDWEPROTB to protect all sectors of MAIN memory
     */

    if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN)
    {
        sectorMask = ((uint32_t)1 << (sectorInBank / (uint32_t)8));
        flashctl->GEN.CMDWEPROTB |= sectorMask;
    }
    else if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN)
    {
        sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
        sectorMask = (uint32_t)1 << (sectorNumber % (uint32_t)32);
        flashctl->GEN.CMDWEPROTNM |= sectorMask;
    }
    else
    {
        ; /* Not expected to reach this else statement */
    }
}
#else
/**
 * @brief 开启指定扇区保护（支持 Bank Swap 物理映射处理）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] addr 目标扇区地址。
 * @param[in] regionSelect 目标区域。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_protectSector(FLASHCTL_Regs *flashctl, uint32_t addr,
                               DL_FLASHCTL_REGION_SELECT regionSelect)
{
    uint32_t sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
    uint32_t sectorInBank =
        DL_FlashCTL_getFlashSectorNumberInBank(flashctl, addr);
    uint8_t numBanks = DL_FactoryRegion_getNumBanks();
    uint32_t mainFlashSize = DL_FactoryRegion_getMAINFlashSize();
    uint32_t physicalSectorNumber = 0;
    uint32_t sectorMask;

    if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_MAIN)
    {
        /* If the banks have been swapped, CMDWEPROTA only protects physical
         * bank 0 (logical bank 1 in a swap), so if the address points to this
         * region we must protect it using CMDWEPROTA instead of CMDWEPROTB
         */
        if (DL_SYSCTL_isExecuteFromUpperFlashBank() && numBanks > 1)
        {
            /* physical sectors are swapped. Calculate physical sector to
             * determine use of CMDWEPROTA */
            if (sectorNumber >= (mainFlashSize / 2))
            {
                physicalSectorNumber = sectorNumber - (mainFlashSize / 2);
            }
            else
            {
                physicalSectorNumber = sectorNumber + (mainFlashSize / 2);
            }
        }
        else
        {
            physicalSectorNumber = sectorNumber;
        }

        if (physicalSectorNumber < (uint32_t)32)
        {
            /* Use CMDWEPROTA */
            sectorMask = (uint32_t)1 << physicalSectorNumber;
            flashctl->GEN.CMDWEPROTA |= sectorMask;
        }
        else
        {
            /* Use CMDWEPROTB */
            if (sectorNumber < (uint32_t)256)
            {
                /* Single bank system */
                if (DL_FactoryRegion_getNumBanks() == (uint32_t)1)
                {
                    sectorMask =
                        (uint32_t)1
                        << ((sectorInBank - (uint32_t)32) / (uint32_t)8);
                    flashctl->GEN.CMDWEPROTB |= sectorMask;
                }
                else
                { /* Multi bank system */
                    sectorMask =
                        ((uint32_t)1 << (sectorInBank / (uint32_t)8));
                    flashctl->GEN.CMDWEPROTB |= sectorMask;
                }
            }
            /* Use CMDWEPROTC */
            else if (sectorNumber < (uint32_t)511)
            {
                sectorMask =
                    ((uint32_t)1
                     << ((sectorInBank - (uint32_t)256) / (uint32_t)8));
                flashctl->GEN.CMDWEPROTC |= sectorMask;
            }
            else
            {
                ; /* Not expected to reach this else statement */
            }
        }
    }
    else if ((uint32_t)regionSelect == FLASHCTL_CMDCTL_REGIONSEL_NONMAIN)
    {
        sectorNumber = DL_FlashCTL_getFlashSectorNumber(flashctl, addr);
        sectorMask = (uint32_t)1 << (sectorNumber % (uint32_t)32);
        flashctl->GEN.CMDWEPROTNM |= sectorMask;
    }
    else
    {
        ; /* Not expected to reach this else statement */
    }
}
#endif

/**
 * @brief 配置基础 READ_VERIFY 命令参数。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_readVerifyConfig(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t cmd)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    flashctl->GEN.CMDBYTEN = cmd;

    /* 设置目标地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);
}

/**
 * @brief 配置 8 位 READ_VERIFY 数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_readVerify8Config(FLASHCTL_Regs *flashctl,
                                          uint32_t address, uint32_t cmd, const uint8_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 配置 16 位 READ_VERIFY 数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_readVerify16Config(FLASHCTL_Regs *flashctl,
                                           uint32_t address, uint32_t cmd, const uint16_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 配置 32 位 READ_VERIFY 数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_readVerify32Config(FLASHCTL_Regs *flashctl,
                                           uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    flashctl->GEN.CMDDATA0 = *data;
}

/**
 * @brief 配置 64 位 READ_VERIFY 数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 期望数据指针（两个 32 位字）。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_readVerify64Config(FLASHCTL_Regs *flashctl,
                                           uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_readVerifyConfig(flashctl, address, cmd);

    /* 写入数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
}

/**
 * @brief 发起 8 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码 */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_8_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 16 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码 */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_16_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 32 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码 */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_32_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 64 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针（两个 32 位字）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码 */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_64_WITHOUT_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行 8 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    DL_FlashCTL_readVerify8Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_8_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 16 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    DL_FlashCTL_readVerify16Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_16_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 32 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify32Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_32_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 64 位读校验（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针（两个 32 位字）。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify64Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_64_WITHOUT_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 8 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    DL_FlashCTL_readVerify8Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_8_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 16 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    DL_FlashCTL_readVerify16Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_16_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 32 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify32Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_32_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 64 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针（两个 32 位字）。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    DL_FlashCTL_readVerify64Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_64_WITH_ECC, data);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 8 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM8WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify8Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_8_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 16 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM16WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify16Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_16_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 32 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM32WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify32Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_32_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 在 RAM 中执行 64 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针（两个 32 位字）。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_readVerifyFromRAM64WithECCManual(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data,
    const uint8_t *eccCode)
{
    DL_FlashCTL_readVerify64Config(
        flashctl, address, DL_FLASHCTL_READ_VERIFY_64_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 发起 8 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify8WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint8_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_8_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 16 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify16WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint16_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_16_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 32 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify32WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_32_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 64 位读校验（硬件 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针（两个 32 位字）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_64_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 8 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify8WithECCManual(FLASHCTL_Regs *flashctl,
                                          uint32_t address, const uint8_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_8_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 16 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify16WithECCManual(FLASHCTL_Regs *flashctl,
                                           uint32_t address, const uint16_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_16_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 32 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify32WithECCManual(FLASHCTL_Regs *flashctl,
                                           uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_32_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 64 位读校验（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 * @param[in] data 期望数据指针（两个 32 位字）。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_readVerify64WithECCManual(FLASHCTL_Regs *flashctl,
                                           uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_READ_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 配置用于比较的字节掩码，带 ECC */
    flashctl->GEN.CMDBYTEN = DL_FLASHCTL_READ_VERIFY_64_WITH_ECC;

    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
    flashctl->GEN.CMDDATAECC0 = *eccCode;

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起空白校验命令。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_blankVerify(FLASHCTL_Regs *flashctl, uint32_t address)
{
    /* Set command and word size. BLANKVERIFY can only be applied to a single
     * flash word at a time */
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_BLANK_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 在 RAM 中执行空白校验命令。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 待校验地址。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_blankVerifyFromRAM(
    FLASHCTL_Regs *flashctl, uint32_t address)
{
    /* Set command and word size. BLANKVERIFY can only be applied to a single
     * flash word at a time */
    flashctl->GEN.CMDTYPE = (uint32_t)DL_FLASHCTL_COMMAND_SIZE_ONE_WORD |
                            (uint32_t)DL_FLASHCTL_COMMAND_TYPE_BLANK_VERIFY;

    /* 设置待校验地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);

    /* 跳转到 RAM 执行命令并等待完成 */
    return DL_FlashCTL_executeCommandFromRAM(flashctl);
}

/**
 * @brief 逐扇区擦除 Data Bank。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return true 表示擦除成功，false 表示失败。
 */
bool DL_FlashCTL_eraseDataBank(FLASHCTL_Regs *flashctl)
{
    bool status = true;
    uint32_t address = FLASHCTL_DATA_ADDRESS;
    uint8_t dataFlashSectors = DL_FactoryRegion_getDATAFlashSize();

    while ((dataFlashSectors != (uint8_t)0) && (status == true))
    {
        DL_FlashCTL_executeClearStatus(flashctl);
        DL_FlashCTL_unprotectSector(
            flashctl, address, DL_FLASHCTL_REGION_SELECT_MAIN);
        DL_FlashCTL_eraseMemory(
            flashctl, address, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
        status = DL_FlashCTL_waitForCmdDone(flashctl);

        address = address + (uint32_t)DL_FLASHCTL_SECTOR_SIZE;
        dataFlashSectors = dataFlashSectors - (uint8_t)1;
    }

    return (status);
}

/**
 * @brief 在 RAM 中逐扇区擦除 Data Bank。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 *
 * @return Flash 命令执行状态。
 */
DL_FLASHCTL_COMMAND_STATUS DL_FlashCTL_eraseDataBankFromRAM(
    FLASHCTL_Regs *flashctl)
{
    DL_FLASHCTL_COMMAND_STATUS status = DL_FLASHCTL_COMMAND_STATUS_PASSED;
    uint32_t address = FLASHCTL_DATA_ADDRESS;
    uint8_t dataFlashSectors = DL_FactoryRegion_getDATAFlashSize();

    while ((dataFlashSectors != (uint8_t)0) &&
           (status == DL_FLASHCTL_COMMAND_STATUS_PASSED))
    {
        DL_FlashCTL_executeClearStatus(flashctl);
        DL_FlashCTL_unprotectSector(
            flashctl, address, DL_FLASHCTL_REGION_SELECT_MAIN);
        status = DL_FlashCTL_eraseMemoryFromRAM(
            flashctl, address, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

        address = address + (uint32_t)DL_FLASHCTL_SECTOR_SIZE;
        dataFlashSectors = dataFlashSectors - (uint8_t)1;
    }

    return (status);
}

#ifdef DEVICE_HAS_FLASH_128_BIT_WORD

static void DL_FlashCTL_programMemory128Config(FLASHCTL_Regs *flashctl,
                                               uint32_t address, uint32_t cmd, const uint32_t *data);
static void DL_FlashCTL_programMemoryConfigMultiWord(FLASHCTL_Regs *flashctl,
                                                     uint32_t address, uint32_t cmd, DL_FLASHCTL_COMMAND_SIZE cmdSize);

/*!
 * @brief Enable programming 128 bits without ECC enabled
 */
#define DL_FLASHCTL_PROGRAM_128_WITHOUT_ECC (0x0000FFFF)

/*!
 * @brief Enable programming 128 bits with ECC enabled
 */
#define DL_FLASHCTL_PROGRAM_128_WITH_ECC (0x0001FFFF)

/**
 * @brief 配置多字宽编程命令基础参数。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 命令类型与字节使能掩码。
 * @param[in] cmdSize 命令字宽。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemoryConfigMultiWord(FLASHCTL_Regs *flashctl,
                                                     uint32_t address, uint32_t cmd, DL_FLASHCTL_COMMAND_SIZE cmdSize)
{
    flashctl->GEN.CMDTYPE =
        (uint32_t)cmdSize | DL_FLASHCTL_COMMAND_TYPE_PROGRAM;

    flashctl->GEN.CMDBYTEN = cmd;

    /* 设置目标地址 */
    DL_FlashCTL_setCommandAddress(flashctl, address);
}

/**
 * @brief 配置 128 位编程命令与数据。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] cmd 字节使能掩码。
 * @param[in] data 128 位源数据指针（四个 32 位字）。
 *
 * @return 无返回值。
 */
static void DL_FlashCTL_programMemory128Config(FLASHCTL_Regs *flashctl,
                                               uint32_t address, uint32_t cmd, const uint32_t *data)
{
    DL_FlashCTL_programMemoryConfigMultiWord(
        flashctl, address, cmd, DL_FLASHCTL_COMMAND_SIZE_TWO_WORDS);

    /* 写入数据寄存器 */
    flashctl->GEN.CMDDATA0 = *data;
    flashctl->GEN.CMDDATA1 = *(data + 1);
    flashctl->GEN.CMDDATA2 = *(data + 2);
    flashctl->GEN.CMDDATA3 = *(data + 3);
}

/**
 * @brief 发起 128 位编程命令（不含 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 128 位数据指针（四个 32 位字）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory128(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 128 位编程 */
    DL_FlashCTL_programMemory128Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_128_WITHOUT_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 128 位编程命令（硬件自动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 128 位数据指针（四个 32 位字）。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory128WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, const uint32_t *data)
{
    /* 使能 128 位编程，带 ECC */
    DL_FlashCTL_programMemory128Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_128_WITH_ECC, data);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 发起 128 位编程命令（手动 ECC）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 目标地址。
 * @param[in] data 128 位数据指针（四个 32 位字）。
 * @param[in] eccCode ECC 数据指针。
 *
 * @return 无返回值。
 */
void DL_FlashCTL_programMemory128WithECCManual(FLASHCTL_Regs *flashctl,
                                               uint32_t address, const uint32_t *data, const uint8_t *eccCode)
{
    /* 使能 128 位编程，带 ECC */
    DL_FlashCTL_programMemory128Config(
        flashctl, address, DL_FLASHCTL_PROGRAM_128_WITH_ECC, data);

    flashctl->GEN.CMDDATAECC0 = *eccCode;
    flashctl->GEN.CMDDATAECC1 = *(eccCode + 1);

    /* 触发命令执行 */
    flashctl->GEN.CMDEXEC = FLASHCTL_CMDEXEC_VAL_EXECUTE;
}

/**
 * @brief 阻塞写入数据（优先使用 128 位模式，必要时退化为 64/32 位）。
 *
 * @param[in] flashctl FLASHCTL 外设寄存器基址。
 * @param[in] address 起始地址。
 * @param[in] data 源数据指针。
 * @param[in] dataSize 32 位字数量。
 * @param[in] regionSelect 目标区域。
 *
 * @return true 表示成功，false 表示失败。
 */
bool DL_FlashCTL_programMemoryBlocking128WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* 检查数据大小有效性 */
    if ((dataSize == (uint32_t)0) || dataSize < (uint32_t)4)
    {
        status = false;
    }

    while ((dataSize != (uint32_t)0) && status)
    {
        /* 执行 Flash 操作前清除状态寄存器 */
        DL_FlashCTL_executeClearStatus(flashctl);

        /* 每次写入前取消扇区保护 */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32 位情况 */
        if (dataSize == (uint32_t)1)
        {
            DL_FlashCTL_programMemory32WithECCGenerated(
                flashctl, address, data);

            dataSize = dataSize - (uint32_t)1;
            data = data + 1;
            address = address + (uint32_t)4;
        }
        else if (dataSize < (uint32_t)4)
        {
            /* 64 位情况 */
            DL_FlashCTL_programMemory64WithECCGenerated(
                flashctl, address, data);
            dataSize = dataSize - (uint32_t)2;
            data = data + 2;
            address = address + (uint32_t)8;
        }
        else
        {
            /* 128 位情况 */
            DL_FlashCTL_programMemory128WithECCGenerated(
                flashctl, address, data);
            dataSize = dataSize - (uint32_t)4;
            data = data + 4;
            address = address + (uint32_t)16;
        }

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}

#endif /* DEVICE_HAS_FLASH_128_BIT_WORD */
