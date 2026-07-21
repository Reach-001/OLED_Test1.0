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

/**
 *  @file dl_keystorectl.c
 *  @brief 密钥库控制器驱动实现 / KEYSTORECTL Driver Implementation
 *  提供密钥写入(loadKey/writeKey)和密钥传输(transferKey)功能的底层实现。
 */

#include <ti/driverlib/dl_keystorectl.h>

#ifdef __MSPM0_HAS_KEYSTORE_CTL__

/** @brief 将数据从源地址复制到目标寄存器 / Copy data from source to destination register */
static void DL_KEYSTORECTL_loadData(
    volatile uint32_t *pDest, uint32_t *pSrc, uint8_t len);
/** @brief 根据密钥大小返回对应的 uint32_t 字数 / Get word count from key size */
static uint8_t DL_KEYSTORECTL_getWordsFromSize(
    DL_KEYSTORECTL_KEY_SIZE keySize);

/** @brief 将密钥加载到密钥库控制器输入缓冲区 */
void DL_KEYSTORECTL_loadKey(KEYSTORECTL_Regs *keystorectl, uint32_t *key)
{
    DL_KEYSTORECTL_KEY_SIZE keySize;
    uint8_t numWords;
    volatile uint32_t *destPtr = (volatile uint32_t *) &keystorectl->KEYIN;
    uint32_t *srcPtr           = (uint32_t *) key;

    keySize  = DL_KEYSTORECTL_getDestinationKeySize(keystorectl); // 获取已配置的密钥大小
    numWords = DL_KEYSTORECTL_getWordsFromSize(keySize);          // 转换为字数（128位=4字, 256位=8字）

    DL_KEYSTORECTL_loadData(destPtr, srcPtr, numWords); // 写入密钥数据
}

/** @brief 将密钥写入密钥库（完整写入流程） */
DL_KEYSTORECTL_STATUS DL_KEYSTORECTL_writeKey(
    KEYSTORECTL_Regs *keystorectl, DL_KEYSTORECTL_KeyWrConfig *keyWrConfig)
{
    DL_KEYSTORECTL_STATUS status = DL_KEYSTORECTL_STATUS_VALID;
    uint32_t validSlot;
    uint32_t slotNum;
    uint8_t numWords;
    volatile uint32_t *destPtr = (volatile uint32_t *) &keystorectl->KEYIN;

    /* 1. Configure the key write by writing size and slot / 配置密钥大小和目标槽 */
    DL_Common_updateReg(&keystorectl->KEYWR,
        (((uint32_t) keyWrConfig->keySize) |
            ((uint32_t) keyWrConfig->keySlot)),
        (KEYSTORECTL_KEYWR_KEYSZSEL_MASK | KEYSTORECTL_KEYWR_KEYSLOTSEL_MASK));

    /* 2. Confirm a valid write configuration / 确认写入配置有效 */
    status = DL_KEYSTORECTL_getStatus(keystorectl);

    /* 3. Write key to the input buffer / 将密钥写入输入缓冲区 */
    if (status == DL_KEYSTORECTL_STATUS_VALID) {
        numWords = DL_KEYSTORECTL_getWordsFromSize(keyWrConfig->keySize);

        DL_KEYSTORECTL_loadData(destPtr, keyWrConfig->key, numWords);

        /* 4. Loop until status is back to valid (no longer busy) / 等待接收完成 */
        status = DL_KEYSTORECTL_getStatus(keystorectl);
        while (status == DL_KEYSTORECTL_STATUS_BUSY_RX) {
            status = DL_KEYSTORECTL_getStatus(keystorectl);
        }
    }

    /* 5. Confirm key slots were successfully written / 确认密钥槽写入成功 */
    slotNum   = (keyWrConfig->keySlot >> KEYSTORECTL_KEYWR_KEYSLOTSEL_OFS);
    validSlot = (1 << (slotNum + KEYSTORECTL_STATUS_VALID_OFS));
    if (status == DL_KEYSTORECTL_STATUS_VALID) {
        validSlot = (validSlot & DL_KEYSTORECTL_getValidKeySlots(keystorectl));
        if (validSlot == 0) {
            status = DL_KEYSTORECTL_STATUS_WRITE_FAILED; // 写入后槽无效，标记失败
        }
    }

    return status;
}

/** @brief 将密钥库中的密钥传输到目标加密引擎 */
DL_KEYSTORECTL_STATUS DL_KEYSTORECTL_transferKey(
    KEYSTORECTL_Regs *keystorectl, const DL_KEYSTORECTL_Config *config)
{
    DL_KEYSTORECTL_STATUS status = DL_KEYSTORECTL_STATUS_VALID;

    /* 1. Confirm valid status on the KEYSTORECTL / 确认控制器状态有效 */
    status = DL_KEYSTORECTL_getStatus(keystorectl);

    /* 2. Configure the read transfer by writing size, slot, and recipient / 配置传输参数 */
    if (status == DL_KEYSTORECTL_STATUS_VALID) {
        DL_Common_updateReg(&keystorectl->KEYRD,
            (((uint32_t) config->keySize) | ((uint32_t) config->cryptoSel) |
                ((uint32_t) config->keySlot)),
            (KEYSTORECTL_KEYRD_KEYSZSEL_MASK |
                KEYSTORECTL_KEYRD_CRYPTOSEL_MASK |
                KEYSTORECTL_KEYRD_KEYSLOTSEL_MASK));

        /* 3. Confirm a valid status and transfer configuration / 确认配置有效 */
        status = DL_KEYSTORECTL_getStatus(keystorectl);

        /* 4. Loop until status is back to valid / 等待传输完成 */
        while (status == DL_KEYSTORECTL_STATUS_BUSY_TX) {
            status = DL_KEYSTORECTL_getStatus(keystorectl);
        }
    }

    return status;
}

/** @brief 将数据逐字从源地址复制到目标寄存器 */
static void DL_KEYSTORECTL_loadData(
    volatile uint32_t *pDest, uint32_t *pSrc, uint8_t len)
{
    uint8_t i;
    uint32_t *pTemp;

    pTemp = pSrc;

    for (i = 0; i < len; i++) {
        *pDest = *pTemp++; // 逐字写入硬件寄存器
    }
}

/** @brief 根据密钥大小返回对应的 uint32_t 字数（128位=4, 256位=8） */
static uint8_t DL_KEYSTORECTL_getWordsFromSize(DL_KEYSTORECTL_KEY_SIZE keySize)
{
    uint8_t numWords;

    switch (keySize) {
        case DL_KEYSTORECTL_KEY_SIZE_128_BITS:
            numWords = 4; // 128 位 = 4 个 uint32_t
            break;
        case DL_KEYSTORECTL_KEY_SIZE_256_BITS:
            numWords = 8; // 256 位 = 8 个 uint32_t
            break;
        default:
            numWords = 0; // 无效大小
            break;
    }

    return numWords;
}

#endif /* __MSPM0_HAS_KEYSTORE_CTL__ */
