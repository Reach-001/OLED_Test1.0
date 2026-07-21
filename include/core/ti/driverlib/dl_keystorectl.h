/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  @file       dl_keystorectl.h
 *  @brief      KEYSTORECTL (Keystore Controller) Driver Library / 密钥库控制器驱动库
 *  @defgroup   KEYSTORECTL Keystore Controller (KEYSTORECTL)
 *
 *  @anchor ti_dl_dl_keystorectl_Overview
 *  # Overview
 *
 *  The Keystore Controller (KEYSTORECTL) DriverLib allows full configuration
 *  of the MSPM0 KEYSTORECTL Module.
 *  密钥库控制器(KEYSTORECTL)驱动库，用于配置 MSPM0 KEYSTORECTL 模块。
 *  该模块提供密钥的写入、读取和传输功能，支持 128 位和 256 位密钥。
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup KEYSTORECTL
 * @{
 */
#ifndef ti_dl_dl_keystorectl__include
#define ti_dl_dl_keystorectl__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MSPM0_HAS_KEYSTORE_CTL__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_KEYSTORECTL_KEY_SLOT
 * @{
 */

/*! @brief Keystore Controller Key Slot 0 / 密钥槽 0 */
#define DL_KEYSTORECTL_KEY_SLOT_0           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT0)

/*! @brief Keystore Controller Key Slot 1 / 密钥槽 1 */
#define DL_KEYSTORECTL_KEY_SLOT_1           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT1)

/*! @brief Keystore Controller Key Slot 2 / 密钥槽 2 */
#define DL_KEYSTORECTL_KEY_SLOT_2           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT2)

/*! @brief Keystore Controller Key Slot 3 / 密钥槽 3 */
#define DL_KEYSTORECTL_KEY_SLOT_3           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT3)

/*! @brief Keystore Controller Key Slot 4 / 密钥槽 4 */
#define DL_KEYSTORECTL_KEY_SLOT_4           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT4)

/*! @brief Keystore Controller Key Slot 5 / 密钥槽 5 */
#define DL_KEYSTORECTL_KEY_SLOT_5           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT5)

/*! @brief Keystore Controller Key Slot 6 / 密钥槽 6 */
#define DL_KEYSTORECTL_KEY_SLOT_6           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT6)

/*! @brief Keystore Controller Key Slot 7 / 密钥槽 7 */
#define DL_KEYSTORECTL_KEY_SLOT_7           (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT7)


/*! @brief Keystore Controller Key Combined Slot 0 and 1 / 组合密钥槽 0 和 1 (用于 256 位密钥) */
#define DL_KEYSTORECTL_KEY_SLOT_0_AND_1    ((KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT0) | \
                                            (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT1))

/*! @brief Keystore Controller Key Combined Slot 2 and 3 / 组合密钥槽 2 和 3 (用于 256 位密钥) */
#define DL_KEYSTORECTL_KEY_SLOT_2_AND_3    ((KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT2) | \
                                            (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT3))

/*! @brief Keystore Controller Key Combined Slot 4 and 5 / 组合密钥槽 4 和 5 (用于 256 位密钥) */
#define DL_KEYSTORECTL_KEY_SLOT_4_AND_5    ((KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT4) | \
                                            (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT5))

/*! @brief Keystore Controller Key Combined Slot 6 and 7 / 组合密钥槽 6 和 7 (用于 256 位密钥) */
#define DL_KEYSTORECTL_KEY_SLOT_6_AND_7    ((KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT6) | \
                                            (KEYSTORECTL_KEYWR_KEYSLOTSEL_SLOT7))

/** @}*/

/* clang-format on */

/*! @enum DL_KEYSTORECTL_NUM_256_KEYS / 256 位密钥数量枚举 */
typedef enum {
    /*! No 256-bit keys / 无 256 位密钥 */
    DL_KEYSTORECTL_NUM_256_KEYS_ZERO = KEYSTORECTL_CFG_NK256_ZERO,
    /*! One 256-bit keys / 1 个 256 位密钥 */
    DL_KEYSTORECTL_NUM_256_KEYS_ONE = KEYSTORECTL_CFG_NK256_ONE,
    /*! Two 256-bit keys / 2 个 256 位密钥 */
    DL_KEYSTORECTL_NUM_256_KEYS_TWO = KEYSTORECTL_CFG_NK256_TWO,
    /*! Three 256-bit keys / 3 个 256 位密钥 */
    DL_KEYSTORECTL_NUM_256_KEYS_THREE = KEYSTORECTL_CFG_NK256_THREE,
    /*! Four 256-bit keys / 4 个 256 位密钥 */
    DL_KEYSTORECTL_NUM_256_KEYS_FOUR = KEYSTORECTL_CFG_NK256_FOUR,
} DL_KEYSTORECTL_NUM_256_KEYS;

/*! @enum DL_KEYSTORECTL_KEY_SIZE / 密钥大小枚举 */
typedef enum {
    /*! Key size set to 128 / 128 位密钥 */
    DL_KEYSTORECTL_KEY_SIZE_128_BITS = KEYSTORECTL_KEYWR_KEYSZSEL_K128,
    /*! Key size set to 256 / 256 位密钥 */
    DL_KEYSTORECTL_KEY_SIZE_256_BITS = KEYSTORECTL_KEYWR_KEYSZSEL_K256,
} DL_KEYSTORECTL_KEY_SIZE;

/*! @enum DL_KEYSTORECTL_NUM_SLOTS / 设备密钥槽数量枚举 */
typedef enum {
    /*! Two 128-bit key slots present on device / 设备有 2 个 128 位密钥槽 */
    DL_KEYSTORECTL_NUM_SLOTS_TWO = KEYSTORECTL_STATUS_NKEYSLOTS_TWO,
    /*! Three 128-bit key slots present on device / 设备有 3 个 128 位密钥槽 */
    DL_KEYSTORECTL_NUM_SLOTS_THREE = KEYSTORECTL_STATUS_NKEYSLOTS_THREE,
    /*! Four 128-bit key slots present on device / 设备有 4 个 128 位密钥槽 */
    DL_KEYSTORECTL_NUM_SLOTS_FOUR = KEYSTORECTL_STATUS_NKEYSLOTS_FOUR,
} DL_KEYSTORECTL_NUM_SLOTS;

/*! @enum DL_KEYSTORECTL_CRYPTO_SEL / 加密引擎选择枚举 */
typedef enum {
    /*! Transfer the key from keystore KEYSTORECTL to the AES engine / 将密钥从密钥库传输到 AES 引擎 */
    DL_KEYSTORECTL_CRYPTO_SEL_AES = KEYSTORECTL_KEYRD_CRYPTOSEL_AES,
} DL_KEYSTORECTL_CRYPTO_SEL;

/*! @enum DL_KEYSTORECTL_STATUS / 密钥库控制器状态枚举 */
typedef enum {
    /*! Valid status bit. Configuration is valid or operation completed.
     *  有效状态：配置有效或操作已完成 */
    DL_KEYSTORECTL_STATUS_VALID = KEYSTORECTL_STATUS_STAT_VALID,
    /*! No Configuration Status bit, no number of 256 keys has been identified.
     *  The user must call @ref DL_KEYSTORECTL_setNumberOf256Keys.
     *  未配置状态：尚未设置 256 位密钥数量，需调用 setNumberOf256Keys */
    DL_KEYSTORECTL_STATUS_NO_CONFIG = KEYSTORECTL_STATUS_STAT_NO_CFG,
    /*! Invalid number set during @ref DL_KEYSTORECTL_setNumberOf256Keys
     *  无效的 256 位密钥数量设置 */
    DL_KEYSTORECTL_STATUS_INVALID_NUM_KEYS =
        KEYSTORECTL_STATUS_STAT_INVALID_NK256,
    /*! Busy RX status bit. The engine is busy receiving a key deposit.
     *  忙于接收密钥（写入中） */
    DL_KEYSTORECTL_STATUS_BUSY_RX = KEYSTORECTL_STATUS_STAT_BUSY_RECEIVE,
    /*! Busy TX status bit. The engine is busy transmitting a key to a crypto engine
     *  忙于发送密钥到加密引擎（传输中） */
    DL_KEYSTORECTL_STATUS_BUSY_TX = KEYSTORECTL_STATUS_STAT_BUSY_TRANSMIT,
    /*! Invalid key write configuration. This means one or more of the
     *  following: the selected slot exceeds available slots, the selected slot
     *  already contains a valid programmed key, the selected slot has a
     *  mismatched key size compared to the KEYSTORECTL configuration.
     *  无效的密钥写入配置：槽超出范围、槽已有密钥、或密钥大小不匹配 */
    DL_KEYSTORECTL_STATUS_INVALID_SLOT_WR =
        KEYSTORECTL_STATUS_STAT_INVALID_KEYSLOTSELW,
    /*! Invalid key read configuration (transmitting a key). This means one or
     *  more of the following: the selected slot exceeds available slots, the
     *  selected slot does not contain a valid programmed key, the selected slot
     *  has a mismatched key size
     *  无效的密钥读取配置：槽超出范围、槽无有效密钥、或密钥大小不匹配 */
    DL_KEYSTORECTL_STATUS_INVALID_SLOT_RD =
        KEYSTORECTL_STATUS_STAT_INVALID_KEYSLOTSELR,
    /*! Key slots are not valid after a write. Write failed during the
        @ref DL_KEYSTORECTL_writeKey (driver library specific).
        写入失败：密钥槽在写入后无效 */
    DL_KEYSTORECTL_STATUS_WRITE_FAILED = 0x0BAD,
} DL_KEYSTORECTL_STATUS;

/*!
 *  @brief Configuration struct for @ref DL_KEYSTORECTL_writeKey
 *  密钥写入配置结构体，用于 @ref DL_KEYSTORECTL_writeKey
 */
typedef struct {
    /*! The key slot (or slots in the case of 256-bit keys) to be written.
     *  One of @ref DL_KEYSTORECTL_KEY_SLOT
     *  要写入的密钥槽（256 位密钥时为组合槽） */
    uint32_t keySlot;
    /*! The size of the key (128-bit or 256-bit).
     *  One of @ref DL_KEYSTORECTL_KEY_SIZE
     *  密钥大小（128 位或 256 位） */
    DL_KEYSTORECTL_KEY_SIZE keySize;
    /*! uint32_t pointer to the current key in memory
     *  指向内存中密钥数据的 uint32_t 指针 */
    uint32_t *key;
} DL_KEYSTORECTL_KeyWrConfig;

/*!
 *  @brief Configuration struct for @ref DL_KEYSTORECTL_transferKey
 *  密钥传输配置结构体，用于 @ref DL_KEYSTORECTL_transferKey
 */
typedef struct {
    /*! The key slot (or slots in the case of 256-bit keys) to be transferred.
     *  One of @ref DL_KEYSTORECTL_KEY_SLOT
     *  要传输的密钥槽（256 位密钥时为组合槽） */
    uint32_t keySlot;
    /*! The size of the key (128-bit or 256-bit).
     *  One of @ref DL_KEYSTORECTL_KEY_SIZE
     *  密钥大小（128 位或 256 位） */
    DL_KEYSTORECTL_KEY_SIZE keySize;
    /*! The intended crypto recipient. One of @ref DL_KEYSTORECTL_CRYPTO_SEL
     *  目标加密引擎，目前仅支持 AES */
    DL_KEYSTORECTL_CRYPTO_SEL cryptoSel;
} DL_KEYSTORECTL_Config;

/**
 *  @brief Sets the number of combined 256 key slots
 *  设置组合 256 位密钥槽的数量
 *
 *  For a given number of 128-bit key slots n, there can be up to n/2 slots
 *  combined to work as one 256-bit key slot. These larger slots are added to
 *  earlier slots first. Thus, a 4-slot device with one 256-key slot will have
 *  slots 0 and 1 combined, and slot 2 and 3 will remain 128-bit key slots.
 *      Refer to the device datasheet for the specific number of key slots.
 *  对于 n 个 128 位密钥槽，最多可组合出 n/2 个 256 位密钥槽。
 *  组合优先从低位槽开始，例如 4 槽设备配置 1 个 256 位密钥时，
 *  槽 0 和槽 1 被组合，槽 2 和槽 3 保持 128 位。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] numKeys      Number of 256-bit keys, one of
 *                          @ref DL_KEYSTORECTL_NUM_256_KEYS. Must be less than
 *                          the total key slots on the device divided by two
 *                          256 位密钥数量，必须小于等于设备总槽位数的一半
 *
 *  @note this function can only be written in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 仅可在 INITDONE 信号置位前的安全运行状态下写入
 */
__STATIC_INLINE void DL_KEYSTORECTL_setNumberOf256Keys(
    KEYSTORECTL_Regs *keystorectl, DL_KEYSTORECTL_NUM_256_KEYS numKeys)
{
    DL_Common_updateReg(
        &keystorectl->CFG, (uint32_t) numKeys, KEYSTORECTL_CFG_NK256_MASK);
}

/**
 *  @brief Returns the current number of combined 256 key slots
 *  获取当前组合 256 位密钥槽的数量
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @return number of 256-bit keys. One of @ref DL_KEYSTORECTL_NUM_256_KEYS
 *          256 位密钥数量
 *
 *  @sa DL_KEYSTORECTL_setNumberOf256Keys
 */
__STATIC_INLINE DL_KEYSTORECTL_NUM_256_KEYS DL_KEYSTORECTL_getNumberOf256Keys(
    const KEYSTORECTL_Regs *keystorectl)
{
    uint32_t numKeys = (keystorectl->CFG & KEYSTORECTL_CFG_NK256_MASK);

    return (DL_KEYSTORECTL_NUM_256_KEYS)(numKeys);
}

/**
 *  @brief Sets the intended destination key slot for the next write
 *  设置下一次写入的目标密钥槽
 *
 *  Sets the destination key slot for a data write to that region. In the case
 *  of 128-bit key slot, only one slot should be selected. In the case of a 256-
 *  bit key slot, both registers should be selected and the key size should also
 *  reflect this change. If 256- it keys are desired, they should be configured
 *  using @ref DL_KEYSTORECTL_NUM_256_KEYS.
 *      Refer to the device datasheet for the specific number of key slots.
 *  128 位密钥仅选一个槽；256 位密钥需选组合槽，且密钥大小也需相应设置。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] keySlot      The intended key slot. Can be 128-bit or a 256-bit.
 *                          One of @ref DL_KEYSTORECTL_KEY_SLOT
 *                          目标密钥槽，可为单槽或组合槽
 *
 *  @note this function can only be written in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 仅可在 INITDONE 信号置位前的安全运行状态下写入
 *
 *  @sa DL_KEYSTORECTL_setNumberOf256Keys
 *  @sa DL_KEYSTORECTL_setDestinationKeySize
 */
__STATIC_INLINE void DL_KEYSTORECTL_setDestinationKeySlot(
    KEYSTORECTL_Regs *keystorectl, uint32_t keySlot)
{
    DL_Common_updateReg(
        &keystorectl->KEYWR, keySlot, KEYSTORECTL_KEYWR_KEYSLOTSEL_MASK);
}

/**
 *  @brief Returns the intended destination key slot for the next write
 *  获取下一次写入的目标密钥槽
 *
 *  Refer to the device datasheet for the specific number of key slots.
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @returns   The intended key slot. Can be 128-bit or a 256-bit. One of
 *             @ref DL_KEYSTORECTL_KEY_SLOT
 *             当前配置的目标密钥槽
 *
 *  @note this function can only be read in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 仅可在 INITDONE 信号置位前的安全运行状态下读取
 *
 *  @sa DL_KEYSTORECTL_setDestinationKeySlot
 */
__STATIC_INLINE uint32_t DL_KEYSTORECTL_getDestinationKeySlot(
    const KEYSTORECTL_Regs *keystorectl)
{
    return (keystorectl->KEYWR & KEYSTORECTL_KEYWR_KEYSLOTSEL_MASK);
}

/**
 *  @brief Sets the intended destination key slot size for next write
 *  设置下一次写入的目标密钥大小
 *
 *  Sets the destination key slot size for a data write to that region. If 256-
 *  bit keys are desired, they should be  configured using
 *  @ref DL_KEYSTORECTL_NUM_256_KEYS.
 *      Refer to the device datasheet for the specific number of key slots.
 *  若需使用 256 位密钥，需先通过 @ref DL_KEYSTORECTL_NUM_256_KEYS 配置。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] keySize      The intended key size. One of
 *                          @ref DL_KEYSTORECTL_KEY_SIZE
 *                          目标密钥大小（128 位或 256 位）
 *
 *  @note this function can only be written in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 仅可在 INITDONE 信号置位前的安全运行状态下写入
 *
 *  @sa DL_KEYSTORECTL_setNumberOf256Keys
 *  @sa DL_KEYSTORECTL_setDestinationKeySlot
 */
__STATIC_INLINE void DL_KEYSTORECTL_setDestinationKeySize(
    KEYSTORECTL_Regs *keystorectl, DL_KEYSTORECTL_KEY_SIZE keySize)
{
    DL_Common_updateReg(&keystorectl->KEYWR, (uint32_t) keySize,
        KEYSTORECTL_KEYWR_KEYSZSEL_MASK);
}

/**
 *  @brief Returns the intended destination key size for the next write
 *  获取下一次写入的目标密钥大小
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @returns   The intended key size. One of @ref DL_KEYSTORECTL_KEY_SIZE
 *             当前配置的目标密钥大小
 *
 *  @note this function can only be read in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 仅可在 INITDONE 信号置位前的安全运行状态下读取
 *
 *  @sa DL_KEYSTORECTL_setDestinationKeySize
 */
__STATIC_INLINE DL_KEYSTORECTL_KEY_SIZE DL_KEYSTORECTL_getDestinationKeySize(
    const KEYSTORECTL_Regs *keystorectl)
{
    uint32_t keySize = (keystorectl->KEYWR & KEYSTORECTL_KEYWR_KEYSZSEL_MASK);

    return (DL_KEYSTORECTL_KEY_SIZE)(keySize);
}

/**
 *  @brief Loads a key into the KEYSTORECTL
 *  将密钥加载到密钥库控制器
 *
 *  Loads a key of the provided size into the KEYSTORECTL provided there is a
 *  valid bit set. The specific intended slot and size should already be set by
 *  the user before using this.
 *      Refer to the device datasheet for the specific number of key slots.
 *  将指定大小的密钥加载到密钥库控制器。调用前需先设置好目标槽和密钥大小。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] key          Pointer to an uint32_t array containing the key.
 *                          指向密钥数据的 uint32_t 数组指针
 *
 *  @note this function can only be written in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 仅可在 INITDONE 信号置位前的安全运行状态下写入
 *
 *  @sa DL_KEYSTORECTL_setDestinationKeySlot
 *  @sa DL_KEYSTORECTL_setDestinationKeySize
 */
void DL_KEYSTORECTL_loadKey(KEYSTORECTL_Regs *keystorectl, uint32_t *key);

/**
 *  @brief Sets the intended source key slot for the next transfer
 *  设置下一次传输的源密钥槽
 *
 *  Sets the source key slot for a data transfer to another crypto peripheral.
 *  In the case of 128-bit key slot, only one slot should be selected. In the
 *  case of a 256-bit key slot, both registers should be selected and the key
 *  size should also reflect this change. If 256- it keys are desired, they
 *  can be accessed using @ref DL_KEYSTORECTL_getNumberOf256Keys.
 *      Refer to the device datasheet for the specific number of key slots.
 *  设置用于向加密外设传输密钥的源密钥槽。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] keySlot      The intended key slot. Can be 128-bit or a 256-bit.
 *                          One of @ref DL_KEYSTORECTL_KEY_SLOT
 *                          源密钥槽，可为单槽或组合槽
 *
 *  @sa DL_KEYSTORECTL_getNumOf256Keys
 *  @sa DL_KEYSTORECTL_setSourceKeySize
 */
__STATIC_INLINE void DL_KEYSTORECTL_setSourceKeySlot(
    KEYSTORECTL_Regs *keystorectl, uint32_t keySlot)
{
    DL_Common_updateReg(
        &keystorectl->KEYRD, keySlot, KEYSTORECTL_KEYRD_KEYSLOTSEL_MASK);
}

/**
 *  @brief Gets the intended source key slot for the next transfer
 *  获取下一次传输的源密钥槽
 *
 *  Refer to the device datasheet for the specific number of key slots.
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @returns   The intended key slot. Can be 128-bit or a 256-bit. One of
 *             @ref DL_KEYSTORECTL_KEY_SLOT
 *             当前配置的源密钥槽
 *
 *  @sa DL_KEYSTORECTL_setSourceKeySlot
 */
__STATIC_INLINE uint32_t DL_KEYSTORECTL_getSourceKeySlot(
    const KEYSTORECTL_Regs *keystorectl)
{
    return (keystorectl->KEYRD & KEYSTORECTL_KEYRD_KEYSLOTSEL_MASK);
}

/**
 *  @brief Sets the intended source key slot size for next transfer
 *  设置下一次传输的源密钥大小
 *
 *  Sets the source key slot size for a data transfer to that region. If 256-
 *  bit keys are desired, they should be  configured using
 *  @ref DL_KEYSTORECTL_NUM_256_KEYS.
 *      Refer to the device datasheet for the specific number of key slots.
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] keySize      The intended key size. One of
 *                          @ref DL_KEYSTORECTL_KEY_SIZE
 *                          源密钥大小（128 位或 256 位）
 *
 *  @sa DL_KEYSTORECTL_setNumberOf256Keys
 *  @sa DL_KEYSTORECTL_setSourceKeySlot
 */
__STATIC_INLINE void DL_KEYSTORECTL_setSourceKeySize(
    KEYSTORECTL_Regs *keystorectl, DL_KEYSTORECTL_KEY_SIZE keySize)
{
    DL_Common_updateReg(&keystorectl->KEYRD, (uint32_t) keySize,
        KEYSTORECTL_KEYRD_KEYSZSEL_MASK);
}

/**
 *  @brief Gets the intended source key size for the next transfer
 *  获取下一次传输的源密钥大小
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @returns   The intended key size. One of @ref DL_KEYSTORECTL_KEY_SIZE
 *             当前配置的源密钥大小
 *
 *  @sa DL_KEYSTORECTL_setSourceKeySize
 */
__STATIC_INLINE DL_KEYSTORECTL_KEY_SIZE DL_KEYSTORECTL_getSourceKeySize(
    const KEYSTORECTL_Regs *keystorectl)
{
    uint32_t keySize = (keystorectl->KEYRD & KEYSTORECTL_KEYRD_KEYSZSEL_MASK);

    return (DL_KEYSTORECTL_KEY_SIZE)(keySize);
}

/**
 *  @brief Gets the current status of the KEYSTORECTL module
 *  获取密钥库控制器模块的当前状态
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @return    The status of the keystorectl. One of @ref DL_KEYSTORECTL_STATUS
 *             当前状态值
 */
__STATIC_INLINE DL_KEYSTORECTL_STATUS DL_KEYSTORECTL_getStatus(
    const KEYSTORECTL_Regs *keystorectl)
{
    uint32_t status = (keystorectl->STATUS & KEYSTORECTL_STATUS_STAT_MASK);

    return (DL_KEYSTORECTL_STATUS)(status);
}

/**
 *  @brief Gets currently written key slots
 *  获取当前已写入的有效密钥槽
 *
 *  To understand the key slot configuration, consider the configuration of the
 *  keys including the number of 256-bit keys, and the number of available key
 *  slots
 *  需结合 256 位密钥数量和可用槽数来理解返回值的含义。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @return    Bitwise OR of all valid slots, corresponding to
 *             @ref DL_KEYSTORECTL_KEY_SLOT values.
 *             所有有效槽的按位或值
 */
__STATIC_INLINE uint32_t DL_KEYSTORECTL_getValidKeySlots(
    const KEYSTORECTL_Regs *keystorectl)
{
    return (keystorectl->STATUS & KEYSTORECTL_STATUS_VALID_MASK);
}

/**
 *  @brief Gets the total number of 128-bit key slots in HW on the device
 *  获取设备硬件上的 128 位密钥槽总数
 *
 *  This is the number of key slots that the KEYSTORECTL module has in HW on the
 *  device. It does not reflect the configuration or the number of 256-bit keys,
 *  only the total number of slots. This also governs the valid configurations,
 *  and the number of 256-bit keys must be less than or equal to half the value.
 *  返回设备硬件实际拥有的 128 位密钥槽数，256 位密钥数量不得超过该值的一半。
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @return    The number of 128-bit slots on the device. One of
 *             @ref DL_KEYSTORECTL_NUM_SLOTS
 *             设备的 128 位密钥槽数量
 */
__STATIC_INLINE DL_KEYSTORECTL_NUM_SLOTS DL_KEYSTORECTL_getNumSlots(
    const KEYSTORECTL_Regs *keystorectl)
{
    uint32_t numSlots =
        (keystorectl->STATUS & KEYSTORECTL_STATUS_NKEYSLOTS_MASK);

    return (DL_KEYSTORECTL_NUM_SLOTS)(numSlots);
}

/**
 *  @brief Writes a key into the KEYSTORE
 *  将密钥写入密钥库
 *
 *  The sequence this function will follow:
 *  执行序列：
 *
 *  1. Confirm valid status on the KEYSTORECTL / 确认控制器状态有效
 *  2. Configure the key write by writing size and slot to the key that is to be
 *          written. Passed in via @ref DL_KEYSTORECTL_KeyWrConfig
 *     配置写入参数（密钥大小和目标槽）
 *  3. Confirm a valid write configuration / 确认写入配置有效
 *  4. Write key to the input buffer / 将密钥写入输入缓冲区
 *  5. Loop until status is back to valid / 等待状态恢复为有效
 *  6. Confirm key slots are successfully written / 确认密钥槽写入成功
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *
 *  @param[in] keyWrConfig  Pointer to the write key configuration struct
 *                          @ref DL_KEYSTORECTL_KeyWrConfig
 *                          密钥写入配置结构体指针
 *
 *  @return    Status of the operation of type @ref DL_KEYSTORECTL_STATUS. Type
 *             will be DL_KEYSTORECTL_STATUS_VALID if successful, or return the
 *             error status.
 *             操作状态，成功返回 DL_KEYSTORECTL_STATUS_VALID
 *
 *  @note The function @ref DL_KEYSTORECTL_setNumberOf256Keys must be run
 *        before this can be executed.
 *  @note It is not possible to overwrite a previously configured key. A full
 *        BOOTRST must be executed before the KEYSTORECTL is erased and can be
 *        re-initialized and written.
 *  @note this function can only be written in a secure operating state before
 *        the INITDONE signal is asserted.
 *  @note 调用前必须先执行 setNumberOf256Keys；无法覆盖已有密钥，
 *        需执行完整 BOOTRST 后才可重新初始化和写入。
 *        仅可在 INITDONE 信号置位前的安全运行状态下写入。
 */
DL_KEYSTORECTL_STATUS DL_KEYSTORECTL_writeKey(
    KEYSTORECTL_Regs *keystorectl, DL_KEYSTORECTL_KeyWrConfig *keyWrConfig);

/**
 *  @brief Transfers key loaded in KEYSTORE into intended crypto
 *  将密钥库中的密钥传输到目标加密引擎
 *
 *  Combination of previous functions in order to give the entire operation.
 *  Expected runtime sequence:
 *  执行序列：
 *
 *  1. Confirm valid status on the KEYSTORECTL / 确认控制器状态有效
 *  2. Configure the read transfer by writing size, slot, and recipient
 *        This is passed in via the struct @ref DL_KEYSTORECTL_Config
 *     配置传输参数（密钥大小、源槽和目标加密引擎）
 *  3. Confirm a valid status and transfer configuration of the KEYSTORECTL
 *     确认状态和传输配置有效
 *  4. Loop until status is back to valid / 等待状态恢复为有效
 *
 *  @param[in] keystorectl  Pointer to the register overlay for the peripheral
 *                          外设寄存器基址指针
 *  @param[in] config       Pointer to a filled out config struct of type
 *                          @ref DL_KEYSTORECTL_Config
 *                          密钥传输配置结构体指针
 *
 *  @return    Status of the operation of type @ref DL_KEYSTORECTL_STATUS. Type
 *             will be DL_KEYSTORECTL_STATUS_VALID if successful, or return the
 *             error status.
 *             操作状态，成功返回 DL_KEYSTORECTL_STATUS_VALID
 */
DL_KEYSTORECTL_STATUS DL_KEYSTORECTL_transferKey(
    KEYSTORECTL_Regs *keystorectl, const DL_KEYSTORECTL_Config *config);

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_KEYSTORE_CTL__ */

#endif /* ti_dl_dl_KEYSTORECTL__include */
/** @}*/
