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

/**
 * 文件用途：MSPM0G3507 SPI 外设驱动库实现，包含 SPI 初始化、时钟配置、
 * 阻塞/非阻塞收发、FIFO 批量读写及低功耗配置备份与恢复等函数。
 */

#include <ti/driverlib/dl_spi.h>

#ifdef __MSPM0_HAS_SPI__

/**
 * @brief Initialize SPI protocol format and mode.
 * @details 功能：配置 SPI 帧格式、数据位宽、奇偶校验、位序与主从模式。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] config 参数：SPI 初始化配置结构体。
 * @return 返回值：无。
 */
void DL_SPI_init(SPI_Regs *spi, const DL_SPI_Config *config)
{
    DL_Common_updateReg(&spi->CTL0,
                        (uint32_t)config->chipSelectPin | (uint32_t)config->frameFormat |
                            (uint32_t)config->dataSize,
                        SPI_CTL0_CSSEL_MASK | SPI_CTL0_FRF_MASK | SPI_CTL0_SPO_MASK |
                            SPI_CTL0_SPH_MASK | SPI_CTL0_DSS_MASK);

    DL_Common_updateReg(&spi->CTL1,
                        (uint32_t)config->parity | (uint32_t)config->bitOrder |
                            (uint32_t)config->mode,
                        SPI_CTL1_PES_MASK | SPI_CTL1_PREN_MASK | SPI_CTL1_PTEN_MASK |
                            SPI_CTL1_MSB_MASK | SPI_CTL1_CP_MASK);
}

/**
 * @brief Configure SPI clock source and divider.
 * @details 功能：设置 SPI 外设时钟源与分频值。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] config 参数：SPI 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_SPI_setClockConfig(SPI_Regs *spi, const DL_SPI_ClockConfig *config)
{
    spi->CLKSEL = (uint32_t)config->clockSel;

    spi->CLKDIV = (uint32_t)config->divideRatio;
}

/**
 * @brief Read SPI clock source and divider.
 * @details 功能：读取 SPI 当前时钟源与分频配置。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] config 参数：用于回填的 SPI 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_SPI_getClockConfig(const SPI_Regs *spi, DL_SPI_ClockConfig *config)
{
    config->clockSel = (DL_SPI_CLOCK)spi->CLKSEL;

    config->divideRatio = (DL_SPI_CLOCK_DIVIDE_RATIO)spi->CLKDIV;
}

/**
 * @brief Blocking receive 8-bit data.
 * @details 功能：等待 RX FIFO 非空后读取 8 位数据。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @return 返回值：读取到的 8 位数据。
 */
uint8_t DL_SPI_receiveDataBlocking8(const SPI_Regs *spi)
{
    while (DL_SPI_isRXFIFOEmpty(spi))
    {
        // 中文说明：轮询等待接收 FIFO 出现有效数据。
    };

    return DL_SPI_receiveData8(spi);
}

/**
 * @brief Blocking receive 16-bit data.
 * @details 功能：等待 RX FIFO 非空后读取 16 位数据。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @return 返回值：读取到的 16 位数据。
 */
uint16_t DL_SPI_receiveDataBlocking16(const SPI_Regs *spi)
{
    while (DL_SPI_isRXFIFOEmpty(spi))
    {
        // 中文说明：无数据时阻塞等待。
    };
    return DL_SPI_receiveData16(spi);
}

/**
 * @brief Blocking receive 32-bit data.
 * @details 功能：等待 RX FIFO 非空后读取 32 位数据。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @return 返回值：读取到的 32 位数据。
 */
uint32_t DL_SPI_receiveDataBlocking32(const SPI_Regs *spi)
{
    while (DL_SPI_isRXFIFOEmpty(spi))
    {
        // 中文说明：确保返回值来自本次有效接收。
    };
    return DL_SPI_receiveData32(spi);
}

/**
 * @brief Blocking transmit 8-bit data.
 * @details 功能：等待 TX FIFO 可写后发送 8 位数据，并等待总线空闲。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] data 参数：待发送 8 位数据。
 * @return 返回值：无。
 */
void DL_SPI_transmitDataBlocking8(SPI_Regs *spi, uint8_t data)
{
    while (DL_SPI_isTXFIFOFull(spi))
    {
        // 中文说明：FIFO 满时等待，防止覆盖待发送数据。
    };
    DL_SPI_transmitData8(spi, data);
    while (DL_SPI_isBusy(spi))
    {
        // 中文说明：等待移位寄存器发送完成。
    };
}

/**
 * @brief Blocking transmit 16-bit data.
 * @details 功能：等待 TX FIFO 可写后发送 16 位数据，并等待总线空闲。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] data 参数：待发送 16 位数据。
 * @return 返回值：无。
 */
void DL_SPI_transmitDataBlocking16(SPI_Regs *spi, uint16_t data)
{
    while (DL_SPI_isTXFIFOFull(spi))
    {
        // 中文说明：FIFO 满时等待。
    };
    DL_SPI_transmitData16(spi, data);
    while (DL_SPI_isBusy(spi))
    {
        // 中文说明：等待硬件完成本帧传输。
    };
}

/**
 * @brief Blocking transmit 32-bit data.
 * @details 功能：等待 TX FIFO 可写后发送 32 位数据，并等待总线空闲。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] data 参数：待发送 32 位数据。
 * @return 返回值：无。
 */
void DL_SPI_transmitDataBlocking32(SPI_Regs *spi, uint32_t data)
{
    while (DL_SPI_isTXFIFOFull(spi))
    {
        // 中文说明：发送队列满时阻塞。
    };
    DL_SPI_transmitData32(spi, data);
    while (DL_SPI_isBusy(spi))
    {
        // 中文说明：等待总线空闲后再返回。
    };
}

/**
 * @brief Try to receive one 8-bit data.
 * @details 功能：RX FIFO 非空时读取 8 位数据，否则返回失败。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 表示读取成功，`false` 表示 FIFO 为空。
 */
bool DL_SPI_receiveDataCheck8(const SPI_Regs *spi, uint8_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(spi))
    {
        status = false;
    }
    else
    {
        *buffer = DL_SPI_receiveData8(spi);
        status = true;
    }

    return status;
}

/**
 * @brief Try to receive one 16-bit data.
 * @details 功能：RX FIFO 非空时读取 16 位数据，否则返回失败。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 表示读取成功，`false` 表示 FIFO 为空。
 */
bool DL_SPI_receiveDataCheck16(const SPI_Regs *spi, uint16_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(spi))
    {
        status = false;
    }
    else
    {
        *buffer = DL_SPI_receiveData16(spi);
        status = true;
    }

    return status;
}

/**
 * @brief Try to receive one 32-bit data.
 * @details 功能：RX FIFO 非空时读取 32 位数据，否则返回失败。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 表示读取成功，`false` 表示 FIFO 为空。
 */
bool DL_SPI_receiveDataCheck32(const SPI_Regs *spi, uint32_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(spi))
    {
        status = false;
    }
    else
    {
        *buffer = DL_SPI_receiveData32(spi);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one 8-bit data.
 * @details 功能：TX FIFO 未满时发送 8 位数据，否则返回失败。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] data 参数：待发送 8 位数据。
 * @return 返回值：`true` 表示发送成功，`false` 表示 FIFO 已满。
 */
bool DL_SPI_transmitDataCheck8(SPI_Regs *spi, uint8_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(spi))
    {
        status = false;
    }
    else
    {
        DL_SPI_transmitData8(spi, data);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one 16-bit data.
 * @details 功能：TX FIFO 未满时发送 16 位数据，否则返回失败。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] data 参数：待发送 16 位数据。
 * @return 返回值：`true` 表示发送成功，`false` 表示 FIFO 已满。
 */
bool DL_SPI_transmitDataCheck16(SPI_Regs *spi, uint16_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(spi))
    {
        status = false;
    }
    else
    {
        DL_SPI_transmitData16(spi, data);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one 32-bit data.
 * @details 功能：TX FIFO 未满时发送 32 位数据，否则返回失败。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] data 参数：待发送 32 位数据。
 * @return 返回值：`true` 表示发送成功，`false` 表示 FIFO 已满。
 */
bool DL_SPI_transmitDataCheck32(SPI_Regs *spi, uint32_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(spi))
    {
        status = false;
    }
    else
    {
        DL_SPI_transmitData32(spi, data);
        status = true;
    }

    return status;
}

/**
 * @brief Drain RX FIFO into 8-bit buffer.
 * @details 功能：批量读取 RX FIFO 到 8 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取数量。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_SPI_drainRXFIFO8(
    const SPI_Regs *spi, uint8_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_SPI_isRXFIFOEmpty(spi))
        {
            // 中文说明：FIFO 中有数据则立即搬运到软件缓冲区。
            buffer[i] = DL_SPI_receiveData8(spi);
        }
        else
        {
            // 中文说明：FIFO 为空时提前结束。
            break;
        }
    }

    return i;
}

/**
 * @brief Drain RX FIFO into 16-bit buffer.
 * @details 功能：批量读取 RX FIFO 到 16 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取数量。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_SPI_drainRXFIFO16(
    const SPI_Regs *spi, uint16_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_SPI_isRXFIFOEmpty(spi))
        {
            buffer[i] = DL_SPI_receiveData16(spi);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Drain RX FIFO into 32-bit buffer.
 * @details 功能：批量读取 RX FIFO 到 32 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取数量。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_SPI_drainRXFIFO32(
    const SPI_Regs *spi, uint32_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_SPI_isRXFIFOEmpty(spi))
        {
            buffer[i] = DL_SPI_receiveData32(spi);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 8-bit buffer.
 * @details 功能：批量写入 8 位数据到 TX FIFO，直到 FIFO 满或达到上限。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_SPI_fillTXFIFO8(
    SPI_Regs *spi, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_SPI_isTXFIFOFull(spi))
        {
            DL_SPI_transmitData8(spi, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 16-bit buffer.
 * @details 功能：批量写入 16 位数据到 TX FIFO，直到 FIFO 满或达到上限。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_SPI_fillTXFIFO16(
    SPI_Regs *spi, const uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_SPI_isTXFIFOFull(spi))
        {
            DL_SPI_transmitData16(spi, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Save SPI runtime configuration.
 * @details 功能：保存 SPI 关键寄存器配置，便于低功耗后恢复。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 表示本次成功保存，`false` 表示已有有效备份。
 */
bool DL_SPI_saveConfiguration(const SPI_Regs *spi, DL_SPI_backupConfig *ptr)
{
    bool stateSaved = !ptr->backupRdy;
    if (stateSaved)
    {
        ptr->controlWord0 = spi->CTL0;
        ptr->controlWord1 = spi->CTL1;
        ptr->clockControl = spi->CLKCTL;
        ptr->clockSel = spi->CLKSEL;
        ptr->divideRatio = spi->CLKDIV;
        ptr->interruptFifoLevelSelectWord = spi->IFLS;
        ptr->interruptMask0 = spi->CPU_INT.IMASK;
        ptr->interruptMask1 = spi->DMA_TRIG_RX.IMASK;
        ptr->interruptMask2 = spi->DMA_TRIG_TX.IMASK;
        ptr->backupRdy = true;
    }
    return stateSaved;
}

/**
 * @brief Restore SPI runtime configuration.
 * @details 功能：从备份结构恢复 SPI 关键寄存器并按原状态重新使能。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 表示恢复成功，`false` 表示当前无有效备份。
 */
bool DL_SPI_restoreConfiguration(SPI_Regs *spi, DL_SPI_backupConfig *ptr)
{
    bool stateRestored = ptr->backupRdy;
    if (stateRestored)
    {
        /* Set CTL1.ENABLE=0 during initialization */
        // 中文说明：恢复期间先禁止外设，避免半配置状态下工作。
        spi->CTL1 = ptr->controlWord1 & ~(SPI_CTL1_ENABLE_MASK);
        spi->CTL0 = ptr->controlWord0;
        spi->CLKCTL = ptr->clockControl;
        spi->CLKSEL = ptr->clockSel;
        spi->CLKDIV = ptr->divideRatio;
        spi->IFLS = ptr->interruptFifoLevelSelectWord;
        spi->CPU_INT.IMASK = ptr->interruptMask0;
        spi->DMA_TRIG_RX.IMASK = ptr->interruptMask1;
        spi->DMA_TRIG_TX.IMASK = ptr->interruptMask2;

        /* Re-enable SPI if it was originally enabled */
        if ((ptr->controlWord1 & SPI_CTL1_ENABLE_MASK) ==
            SPI_CTL1_ENABLE_MASK)
        {
            // 中文说明：仅在原先使能时恢复使能位，保持行为一致。
            DL_SPI_enable(spi);
        }
        ptr->backupRdy = false;
    }
    return stateRestored;
}

/**
 * @brief Fill TX FIFO from 32-bit buffer.
 * @details 功能：批量写入 32 位数据到 TX FIFO，直到 FIFO 满或达到上限。
 * @param[in] spi 参数：SPI 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_SPI_fillTXFIFO32(
    SPI_Regs *spi, const uint32_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_SPI_isTXFIFOFull(spi))
        {
            DL_SPI_transmitData32(spi, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}

#endif /* __MSPM0_HAS_SPI__ */
