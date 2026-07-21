/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  @file       dl_unicommspi.c
 *  @brief      Unified Communication Module (UNICOMM) - SPI Driver Library
 *              统一通信模块 SPI 驱动库，提供 SPI 初始化、收发、FIFO 管理、
 *              时钟配置及低功耗备份/恢复等功能的实现。
 ******************************************************************************
 */

#include <ti/driverlib/dl_unicommspi.h>

#ifdef __MCU_HAS_UNICOMMSPI__

/**
 * @brief Initialize UNICOMM SPI protocol format and mode.
 * @details 功能：配置 UNICOMM SPI 帧格式、位宽、校验、位序与主从模式。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] config 参数：SPI 初始化配置结构体。
 * @return 返回值：无。
 */
void DL_SPI_init(UNICOMM_Inst_Regs *unicomm, DL_SPI_Config *config)
{
    DL_Common_updateReg(&unicomm->spi->CTL0,
                        (uint32_t)config->chipSelectPin | (uint32_t)config->frameFormat |
                            (uint32_t)config->dataSize,
                        UNICOMMSPI_CTL0_CSSEL_MASK | UNICOMMSPI_CTL0_FRF_MASK |
                            UNICOMMSPI_CTL0_SPO_MASK | UNICOMMSPI_CTL0_SPH_MASK |
                            UNICOMMSPI_CTL0_DSS_MASK);

    DL_Common_updateReg(&unicomm->spi->CTL1,
                        (uint32_t)config->parity | (uint32_t)config->bitOrder |
                            (uint32_t)config->mode,
                        UNICOMMSPI_CTL1_PES_MASK | UNICOMMSPI_CTL1_PREN_MASK |
                            UNICOMMSPI_CTL1_PTEN_MASK | UNICOMMSPI_CTL1_MSB_MASK |
                            UNICOMMSPI_CTL1_CP_MASK);
}

/**
 * @brief Configure UNICOMM SPI clock source and divider.
 * @details 功能：设置 UNICOMM SPI 时钟源与分频值。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] config 参数：SPI 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_SPI_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_ClockConfig *config)
{
    unicomm->spi->CLKSEL = (uint32_t)config->clockSel;

    unicomm->spi->CLKDIV = (uint32_t)config->divideRatio;
}

/**
 * @brief Read UNICOMM SPI clock source and divider.
 * @details 功能：读取 UNICOMM SPI 当前时钟源与分频配置。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] config 参数：用于回填的 SPI 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_SPI_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_ClockConfig *config)
{
    config->clockSel = (DL_SPI_CLOCK)unicomm->spi->CLKSEL;

    config->divideRatio = (DL_SPI_CLOCK_DIVIDE_RATIO)unicomm->spi->CLKDIV;
}

/**
 * @brief Blocking receive 8-bit data in UNICOMM SPI mode.
 * @details 功能：等待 RX FIFO 非空后读取 8 位数据。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：读取到的 8 位数据。
 */
uint8_t DL_SPI_receiveDataBlocking8(UNICOMM_Inst_Regs *unicomm)
{
    while (DL_SPI_isRXFIFOEmpty(unicomm))
    {
        // 中文说明：无数据时持续等待。
    };

    return DL_SPI_receiveData8(unicomm);
}

/**
 * @brief Blocking receive 16-bit data in UNICOMM SPI mode.
 * @details 功能：等待 RX FIFO 非空后读取 16 位数据。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：读取到的 16 位数据。
 */
uint16_t DL_SPI_receiveDataBlocking16(UNICOMM_Inst_Regs *unicomm)
{
    while (DL_SPI_isRXFIFOEmpty(unicomm))
    {
        // 中文说明：FIFO 空时阻塞。
    };
    return DL_SPI_receiveData16(unicomm);
}

/**
 * @brief Blocking transmit 8-bit data in UNICOMM SPI mode.
 * @details 功能：等待 TX FIFO 可写后发送 8 位数据并等待总线空闲。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送 8 位数据。
 * @return 返回值：无。
 */
void DL_SPI_transmitDataBlocking8(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    while (DL_SPI_isTXFIFOFull(unicomm))
    {
        // 中文说明：FIFO 满时等待。
    };
    DL_SPI_transmitData8(unicomm, data);
    while (DL_SPI_isBusy(unicomm))
    {
        // 中文说明：等待移位发送完成。
    };
}

/**
 * @brief Blocking transmit 16-bit data in UNICOMM SPI mode.
 * @details 功能：等待 TX FIFO 可写后发送 16 位数据并等待总线空闲。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送 16 位数据。
 * @return 返回值：无。
 */
void DL_SPI_transmitDataBlocking16(UNICOMM_Inst_Regs *unicomm, uint16_t data)
{
    while (DL_SPI_isTXFIFOFull(unicomm))
    {
        // 中文说明：FIFO 满时等待。
    };
    DL_SPI_transmitData16(unicomm, data);
    while (DL_SPI_isBusy(unicomm))
    {
        // 中文说明：等待总线空闲。
    };
}

/**
 * @brief Blocking transmit 32-bit data in UNICOMM SPI mode.
 * @details 功能：等待 TX FIFO 可写后发送 32 位数据并等待总线空闲。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送 32 位数据。
 * @return 返回值：无。
 */
void DL_SPI_transmitDataBlocking32(UNICOMM_Inst_Regs *unicomm, uint32_t data)
{
    while (DL_SPI_isTXFIFOFull(unicomm))
    {
        // 中文说明：FIFO 满时阻塞。
    };
    DL_SPI_transmitData32(unicomm, data);
    while (DL_SPI_isBusy(unicomm))
    {
        // 中文说明：等待本次传输彻底结束。
    };
}

/**
 * @brief Try to receive one 8-bit data in UNICOMM SPI mode.
 * @details 功能：RX FIFO 非空时读取 8 位数据，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 空。
 */
bool DL_SPI_receiveDataCheck8(UNICOMM_Inst_Regs *unicomm, uint8_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(unicomm))
    {
        status = false;
    }
    else
    {
        *buffer = DL_SPI_receiveData8(unicomm);
        status = true;
    }

    return status;
}

/**
 * @brief Try to receive one 16-bit data in UNICOMM SPI mode.
 * @details 功能：RX FIFO 非空时读取 16 位数据，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 空。
 */
bool DL_SPI_receiveDataCheck16(UNICOMM_Inst_Regs *unicomm, uint16_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(unicomm))
    {
        status = false;
    }
    else
    {
        *buffer = DL_SPI_receiveData16(unicomm);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one 8-bit data in UNICOMM SPI mode.
 * @details 功能：TX FIFO 未满时发送 8 位数据，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送 8 位数据。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 满。
 */
bool DL_SPI_transmitDataCheck8(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(unicomm))
    {
        status = false;
    }
    else
    {
        DL_SPI_transmitData8(unicomm, data);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one 16-bit data in UNICOMM SPI mode.
 * @details 功能：TX FIFO 未满时发送 16 位数据，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送 16 位数据。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 满。
 */
bool DL_SPI_transmitDataCheck16(UNICOMM_Inst_Regs *unicomm, uint16_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(unicomm))
    {
        status = false;
    }
    else
    {
        DL_SPI_transmitData16(unicomm, data);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one 32-bit data in UNICOMM SPI mode.
 * @details 功能：TX FIFO 未满时发送 32 位数据，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送 32 位数据。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 满。
 */
bool DL_SPI_transmitDataCheck32(UNICOMM_Inst_Regs *unicomm, uint32_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(unicomm))
    {
        status = false;
    }
    else
    {
        DL_SPI_transmitData32(unicomm, data);
        status = true;
    }

    return status;
}

/**
 * @brief Drain RX FIFO into 8-bit buffer in UNICOMM SPI mode.
 * @details 功能：批量读取 RX FIFO 到 8 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取数量。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_SPI_drainRXFIFO8(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_SPI_isRXFIFOEmpty(unicomm))
        {
            buffer[i] = DL_SPI_receiveData8(unicomm);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Drain RX FIFO into 16-bit buffer in UNICOMM SPI mode.
 * @details 功能：批量读取 RX FIFO 到 16 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取数量。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_SPI_drainRXFIFO16(
    UNICOMM_Inst_Regs *unicomm, uint16_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_SPI_isRXFIFOEmpty(unicomm))
        {
            buffer[i] = DL_SPI_receiveData16(unicomm);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 8-bit buffer in UNICOMM SPI mode.
 * @details 功能：批量写入 8 位数据到 TX FIFO，直到 FIFO 满或达到上限。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_SPI_fillTXFIFO8(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_SPI_isTXFIFOFull(unicomm))
        {
            DL_SPI_transmitData8(unicomm, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 16-bit buffer in UNICOMM SPI mode.
 * @details 功能：批量写入 16 位数据到 TX FIFO，直到 FIFO 满或达到上限。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_SPI_fillTXFIFO16(
    UNICOMM_Inst_Regs *unicomm, uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_SPI_isTXFIFOFull(unicomm))
        {
            DL_SPI_transmitData16(unicomm, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Save UNICOMM SPI runtime configuration.
 * @details 功能：保存 UNICOMM SPI 关键寄存器配置，便于低功耗后恢复。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 保存成功，`false` 表示已有有效备份。
 */
bool DL_SPI_saveConfiguration(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_backupConfig *ptr)
{
    bool stateSaved = !ptr->backupRdy;
    if (stateSaved)
    {
        ptr->controlWord0 = unicomm->spi->CTL0;
        ptr->controlWord1 = unicomm->spi->CTL1;
        ptr->clockControl = unicomm->spi->CLKCTL;
        ptr->clockSel = unicomm->spi->CLKSEL;
        ptr->divideRatio = unicomm->spi->CLKDIV;
        ptr->interruptFifoLevelSelectWord = unicomm->spi->IFLS;
        ptr->interruptMask0 = unicomm->spi->CPU_INT.IMASK;
        ptr->interruptMask1 = unicomm->spi->DMA_TRIG_RX.IMASK;
        ptr->interruptMask2 = unicomm->spi->DMA_TRIG_TX.IMASK;
        ptr->backupRdy = true;
    }
    return stateSaved;
}

/**
 * @brief Restore UNICOMM SPI runtime configuration.
 * @details 功能：恢复 UNICOMM SPI 关键寄存器并按原状态重新使能。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 恢复成功，`false` 表示无有效备份。
 */
bool DL_SPI_restoreConfiguration(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_backupConfig *ptr)
{
    bool stateRestored = ptr->backupRdy;
    if (stateRestored)
    {
        /* Set CTL1.ENABLE=0 during initialization */
        // 中文说明：恢复期间先关闭外设，避免半配置状态工作。
        unicomm->spi->CTL1 =
            ptr->controlWord1 & ~(UNICOMMSPI_CTL1_ENABLE_MASK);
        unicomm->spi->CTL0 = ptr->controlWord0;
        unicomm->spi->CLKCTL = ptr->clockControl;
        unicomm->spi->CLKSEL = ptr->clockSel;
        unicomm->spi->CLKDIV = ptr->divideRatio;
        unicomm->spi->IFLS = ptr->interruptFifoLevelSelectWord;
        unicomm->spi->CPU_INT.IMASK = ptr->interruptMask0;
        unicomm->spi->DMA_TRIG_RX.IMASK = ptr->interruptMask1;
        unicomm->spi->DMA_TRIG_TX.IMASK = ptr->interruptMask2;

        /* Re-enable SPI if it was originally enabled */
        if ((ptr->controlWord1 & UNICOMMSPI_CTL1_ENABLE_MASK) ==
            UNICOMMSPI_CTL1_ENABLE_MASK)
        {
            // 中文说明：仅在原状态使能时恢复使能位。
            DL_SPI_enable(unicomm);
        }
        ptr->backupRdy = false;
    }
    return stateRestored;
}

/**
 * @brief Fill TX FIFO from 32-bit buffer in UNICOMM SPI mode.
 * @details 功能：批量写入 32 位数据到 TX FIFO，直到 FIFO 满或达到上限。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_SPI_fillTXFIFO32(
    UNICOMM_Inst_Regs *unicomm, uint32_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_SPI_isTXFIFOFull(unicomm))
        {
            DL_SPI_transmitData32(unicomm, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}

#endif /* __MCU_HAS_UNICOMMSPI__ */
