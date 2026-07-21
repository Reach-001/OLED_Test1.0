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
 *  @file       dl_uart.c
 *  @brief      UART Driver Library Implementation
 *              UART 驱动库实现文件，包含初始化、波特率配置、IrDA 配置、
 *              阻塞/非阻塞收发、FIFO 批量操作以及 Main/Extend 配置备份恢复。
 ******************************************************************************
 */

#include <ti/driverlib/dl_uart_extend.h>
#include <ti/driverlib/dl_uart_main.h>

#if defined(__MSPM0_HAS_UART_MAIN__) || defined(__MSPM0_HAS_UART_EXTD__)

/**
 * @brief Initialize UART format and direction.
 * @details 功能：配置 UART 模式、收发方向、流控、校验、数据位和停止位。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] config 参数：UART 初始化配置结构体。
 * @return 返回值：无。
 */
void DL_UART_init(UART_Regs *uart, const DL_UART_Config *config)
{
    DL_UART_disable(uart);

    DL_Common_updateReg(&uart->CTL0,
                        (uint32_t)config->mode | (uint32_t)config->direction |
                            (uint32_t)config->flowControl,
                        UART_CTL0_RXE_MASK | UART_CTL0_TXE_MASK | UART_CTL0_MODE_MASK |
                            UART_CTL0_RTSEN_MASK | UART_CTL0_CTSEN_MASK | UART_CTL0_FEN_MASK);

    DL_Common_updateReg(&uart->LCRH,
                        (uint32_t)config->parity | (uint32_t)config->wordLength |
                            (uint32_t)config->stopBits,
                        UART_LCRH_PEN_ENABLE | UART_LCRH_EPS_MASK | UART_LCRH_SPS_MASK |
                            UART_LCRH_WLEN_MASK | UART_LCRH_STP2_MASK);
}

/**
 * @brief Configure UART clock source and divider.
 * @details 功能：设置 UART 的时钟源与分频值。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] config 参数：UART 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_UART_setClockConfig(UART_Regs *uart, const DL_UART_ClockConfig *config)
{
    uart->CLKSEL = (uint32_t)config->clockSel;

    uart->CLKDIV = (uint32_t)config->divideRatio;
}

/**
 * @brief Read UART clock source and divider.
 * @details 功能：读取 UART 当前时钟源与分频配置。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[out] config 参数：用于回填的 UART 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_UART_getClockConfig(const UART_Regs *uart, DL_UART_ClockConfig *config)
{
    config->clockSel = (DL_UART_CLOCK)uart->CLKSEL;

    config->divideRatio = (DL_UART_CLOCK_DIVIDE_RATIO)uart->CLKDIV;
}

/**
 * @brief Configure UART baud rate and oversampling.
 * @details 功能：根据输入时钟与目标波特率计算分频值并配置过采样倍率。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] clockFreq 参数：UART 时钟频率（Hz）。
 * @param[in] baudRate 参数：目标波特率（bps）。
 * @return 返回值：无。
 */
void DL_UART_configBaudRate(
    UART_Regs *uart, uint32_t clockFreq, uint32_t baudRate)
{
    uint32_t divisor;

    /*
     *  The baud rate divisor, brd, is calculated with the following formula:
     *  brd = ibrd.fbrd = clockOutput / (OVS * baudRate)
     *  where ibrd is the integer part, fbrd is the fractional part.
     *  Since fbrd is 6 bits, multiply brd by 64:
     *       64 * brd = (clockOutput * 64) / (16 * baudRate)
     *                = (clockOutput * 4) / baudRate
     *    add 1/2 to round the least significant bit of fbrd:
     *        64 * brd + 1/2 = (clockOutput * 8 / (2 * baudRate) + 1/2
     *        divisor = 64*brd+1/2  = [(clockOutput * 8)/ baudRate + 1] / 2
     *
     *  The lower 6 bits is fbrd, upper part is ibrd
     *  Note: If ibrd is 0, FBRD will be ignored and no data will be
     *  transferred.
     */

    /*  Calculate baud rate divisor based on OVS: */
    if ((baudRate * (uint32_t)8) > clockFreq)
    {
        // 中文说明：高波特率场景优先选 3x 过采样以扩大可配置范围。
        DL_UART_setOversampling(uart, DL_UART_OVERSAMPLING_RATE_3X);
        divisor = ((clockFreq * (uint32_t)64) / (baudRate * (uint32_t)3)) +
                  ((uint32_t)1 / (uint32_t)2);
    }
    else if ((baudRate * (uint32_t)16) > clockFreq)
    {
        // 中文说明：中间区域采用 8x 过采样，按等效关系调整分母。
        DL_UART_setOversampling(uart, DL_UART_OVERSAMPLING_RATE_8X);
        baudRate = baudRate / (uint32_t)2;
        divisor = (((clockFreq * (uint32_t)8) / baudRate) + (uint32_t)1) /
                  (uint32_t)2;
    }
    else
    {
        // 中文说明：常规低波特率采用 16x 过采样以提升采样鲁棒性。
        DL_UART_setOversampling(uart, DL_UART_OVERSAMPLING_RATE_16X);
        divisor = (((clockFreq * (uint32_t)8) / baudRate) + (uint32_t)1) /
                  (uint32_t)2;
    }

    /* Set the integer and fractional parts of the baud rate divisor */
    DL_UART_setBaudRateDivisor(
        uart, divisor >> (uint32_t)6, divisor & (uint32_t)0x3F);
}

// TODO: Verify parameters and units
/**
 *  @brief      Set the IrDA configurations
 *
 *  Calculates the baud rate divisor given the clock output of the UART clock
 *  source and the target baud rate. This API also enables IrDA mode.
 *
 *  @param[in]  uart         Pointer to the register overlay for the peripheral
 *  @param[in]  polarity     The receive input polarity.
 *                           One of @DL_UART_IRDA_POLARITY.
 *  @param[in]  pulseLength  The length of the IrDA transmit pulse.
 *  @param[in]  irdaClk      The clock used for the transmit pulse.
 *                           One of @DL_UART_IRDA_CLOCK.
 *  @details    中文说明：功能是配置 IrDA 极性与脉宽，并自动使能 IrDA。
 *  @return     返回值：无。
 */
void DL_UART_configIrDAMode(UART_Regs *uart, DL_UART_IRDA_POLARITY polarity,
                            uint32_t pulseLength, DL_UART_IRDA_CLOCK irdaClk)
{
    DL_Common_updateReg(&uart->IRCTL,
                        (uint32_t)polarity | UART_IRCTL_IREN_ENABLE,
                        UART_IRCTL_IRRXPL_MASK | UART_IRCTL_IREN_MASK);

    DL_UART_setIrDAPulseLength(uart, pulseLength, irdaClk);
}

/**
 * @brief Configure IrDA pulse length register fields.
 * @details 功能：根据脉宽与 IrDA 时钟源计算并设置 IRTXPL 字段。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] pulseLength 参数：发射脉宽配置值。
 * @param[in] irdaClk 参数：IrDA 脉冲时钟源。
 * @return 返回值：无。
 */
void DL_UART_setIrDAPulseLength(
    UART_Regs *uart, uint32_t pulseLength, DL_UART_IRDA_CLOCK irdaClk)
{
    uint32_t IRTXPL;

    if (pulseLength == DL_UART_PULSE_WIDTH_3_16_BIT_PERIOD)
    {
        // Set IRTXCLK = 0 = BITCLK16 and IRTXPL = 5
        DL_Common_updateReg(&uart->IRCTL, 0x14,
                            UART_IRCTL_IRTXCLK_MASK | UART_IRCTL_IRTXPL_MASK);
    }
    else
    {
        IRTXPL =
            pulseLength * (uint32_t)2 * (uint32_t)irdaClk - (uint32_t)1;

        DL_Common_updateReg(&uart->IRCTL, IRTXPL,
                            UART_IRCTL_IRTXCLK_MASK | UART_IRCTL_IRTXPL_MASK);
    }
}

/**
 * @brief Blocking receive one UART byte.
 * @details 功能：等待 RX FIFO 非空后读取 1 字节数据。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @return 返回值：接收到的 1 字节数据。
 */
uint8_t DL_UART_receiveDataBlocking(const UART_Regs *uart)
{
    while (DL_UART_isRXFIFOEmpty(uart))
    {
    };

    return DL_UART_receiveData(uart);
}

/**
 * @brief Blocking transmit one UART byte.
 * @details 功能：等待 TX FIFO 可写后发送 1 字节并等待发送完成。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] data 参数：待发送字节。
 * @return 返回值：无。
 */
void DL_UART_transmitDataBlocking(UART_Regs *uart, uint8_t data)
{
    while (DL_UART_isTXFIFOFull(uart))
    {
    };
    DL_UART_transmitData(uart, data);
    while (DL_UART_isBusy(uart))
    {
        // 中文说明：等待移位寄存器发送完成，避免连续发送时序冲突。
    };
}

/**
 * @brief Try to receive one UART byte.
 * @details 功能：RX FIFO 非空时读取 1 字节，否则返回失败。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 空。
 */
bool DL_UART_receiveDataCheck(const UART_Regs *uart, uint8_t *buffer)
{
    bool status;
    if (DL_UART_isRXFIFOEmpty(uart))
    {
        status = false;
    }
    else
    {
        *buffer = DL_UART_receiveData(uart);
        status = true;
    }

    return status;
}

/**
 * @brief Try to transmit one UART byte.
 * @details 功能：TX FIFO 未满时发送 1 字节，否则返回失败。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] data 参数：待发送字节。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 满。
 */
bool DL_UART_transmitDataCheck(UART_Regs *uart, uint8_t data)
{
    bool status;
    if (DL_UART_isTXFIFOFull(uart))
    {
        status = false;
    }
    else
    {
        DL_UART_transmitData(uart, data);
        status = true;
    }

    return status;
}

/**
 * @brief Drain UART RX FIFO into buffer.
 * @details 功能：批量读取 RX FIFO，直到 FIFO 空或达到最大数量。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取数量。
 * @return 返回值：实际读取字节数。
 */
uint32_t DL_UART_drainRXFIFO(
    const UART_Regs *uart, uint8_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_UART_isRXFIFOEmpty(uart))
        {
            buffer[i] = DL_UART_receiveData(uart);
        }
        else
        {
            break;
        }
    }

    return i;
}

/**
 * @brief Fill UART TX FIFO from buffer.
 * @details 功能：批量写入 TX FIFO，直到 FIFO 满或达到最大数量。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入数量。
 * @return 返回值：实际写入字节数。
 */
uint32_t DL_UART_fillTXFIFO(
    UART_Regs *uart, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_UART_isTXFIFOFull(uart))
        {
            DL_UART_transmitData(uart, buffer[i]);
        }
        else
        {
            break;
        }
    }

    return i;
}
#ifdef __MSPM0_HAS_UART_MAIN__

/**
 * @brief Save UART Main configuration.
 * @details 功能：保存 UART Main 关键寄存器，供后续恢复。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 保存成功，`false` 表示已有有效备份。
 */
bool DL_UART_Main_saveConfiguration(
    const UART_Regs *uart, DL_UART_Main_backupConfig *ptr)
{
    bool stateSaved = !ptr->backupRdy;
    if (stateSaved)
    {
        ptr->controlWord = uart->CTL0;
        ptr->clockSel = uart->CLKSEL;
        ptr->divideRatio = uart->CLKDIV;
        ptr->interruptFifoLevelSelectWord = uart->IFLS;
        ptr->ibrd = uart->IBRD;
        ptr->fbrd = uart->FBRD;
        ptr->lineControlRegisterWord = uart->LCRH;
        ptr->glitchFilterControlWord = uart->GFCTL;
        ptr->interruptMask0 = uart->CPU_INT.IMASK;
        ptr->interruptMask1 = uart->DMA_TRIG_RX.IMASK;
        ptr->interruptMask2 = uart->DMA_TRIG_TX.IMASK;
        ptr->backupRdy = true;
    }
    return stateSaved;
}

/**
 * @brief Restore UART Main configuration.
 * @details 功能：从备份结构恢复 UART Main 配置并按原状态重新使能。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 恢复成功，`false` 表示无有效备份。
 */
bool DL_UART_Main_restoreConfiguration(
    UART_Regs *uart, DL_UART_Main_backupConfig *ptr)
{
    bool stateRestored = ptr->backupRdy;
    if (stateRestored)
    {
        /* Set CTL0.ENABLE=0 during initialization */
        // 中文说明：恢复期间先关闭外设，防止寄存器处于半配置状态。
        uart->CTL0 = ptr->controlWord & ~(UART_CTL0_ENABLE_MASK);
        uart->CLKSEL = ptr->clockSel;
        uart->CLKDIV = ptr->divideRatio;
        uart->IFLS = ptr->interruptFifoLevelSelectWord;
        uart->IBRD = ptr->ibrd;
        uart->FBRD = ptr->fbrd;
        uart->LCRH = ptr->lineControlRegisterWord;
        uart->GFCTL = ptr->glitchFilterControlWord;
        uart->CPU_INT.IMASK = ptr->interruptMask0;
        uart->DMA_TRIG_RX.IMASK = ptr->interruptMask1;
        uart->DMA_TRIG_TX.IMASK = ptr->interruptMask2;

        /* Re-enable UART if it was originally enabled */
        if ((ptr->controlWord & UART_CTL0_ENABLE_MASK) ==
            UART_CTL0_ENABLE_MASK)
        {
            // 中文说明：仅当原状态为使能时才恢复使能位。
            DL_UART_enable(uart);
        }
        ptr->backupRdy = false;
    }
    return stateRestored;
}

#endif /* __MSPM0_HAS_UART_MAIN__ */

#ifdef __MSPM0_HAS_UART_EXTD__

/**
 * @brief Save UART Extend configuration.
 * @details 功能：保存 UART Extend 关键寄存器，供后续恢复。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 保存成功，`false` 表示已有有效备份。
 */
bool DL_UART_Extend_saveConfiguration(
    const UART_Regs *uart, DL_UART_Extend_backupConfig *ptr)
{
    bool stateSaved = !ptr->backupRdy;
    if (stateSaved)
    {
        ptr->controlWord = uart->CTL0;
        ptr->clockSel = uart->CLKSEL;
        ptr->divideRatio = uart->CLKDIV;
        ptr->interruptFifoLevelSelectWord = uart->IFLS;
        ptr->ibrd = uart->IBRD;
        ptr->fbrd = uart->FBRD;
        ptr->lineControlRegisterWord = uart->LCRH;
        ptr->glitchFilterControlWord = uart->GFCTL;
        ptr->linControlWord = uart->LINCTL;
        ptr->irdaControlWord = uart->IRCTL;
        ptr->addressMask = uart->AMASK;
        ptr->address = uart->ADDR;
        ptr->interruptMask0 = uart->CPU_INT.IMASK;
        ptr->interruptMask1 = uart->DMA_TRIG_RX.IMASK;
        ptr->interruptMask2 = uart->DMA_TRIG_TX.IMASK;
        ptr->backupRdy = true;
    }
    return stateSaved;
}

/**
 * @brief Restore UART Extend configuration.
 * @details 功能：从备份结构恢复 UART Extend 配置并按原状态重新使能。
 * @param[in] uart 参数：UART 外设寄存器基址。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：`true` 恢复成功，`false` 表示无有效备份。
 */
bool DL_UART_Extend_restoreConfiguration(
    UART_Regs *uart, DL_UART_Extend_backupConfig *ptr)
{
    bool stateRestored = ptr->backupRdy;
    if (stateRestored)
    {
        /* Set CTL0.ENABLE=0 during initialization */
        // 中文说明：恢复期间先关闭外设，保证寄存器原子恢复。
        uart->CTL0 = ptr->controlWord & ~(UART_CTL0_ENABLE_MASK);
        uart->CLKSEL = ptr->clockSel;
        uart->CLKDIV = ptr->divideRatio;
        uart->IFLS = ptr->interruptFifoLevelSelectWord;
        uart->IBRD = ptr->ibrd;
        uart->FBRD = ptr->fbrd;
        uart->LCRH = ptr->lineControlRegisterWord;
        uart->GFCTL = ptr->glitchFilterControlWord;
        uart->LCRH = ptr->lineControlRegisterWord;
        uart->IRCTL = ptr->irdaControlWord;
        uart->AMASK = ptr->addressMask;
        uart->ADDR = ptr->address;
        uart->CPU_INT.IMASK = ptr->interruptMask0;
        uart->DMA_TRIG_RX.IMASK = ptr->interruptMask1;
        uart->DMA_TRIG_TX.IMASK = ptr->interruptMask2;

        /* Re-enable UART if it was originally enabled */
        if ((ptr->controlWord & UART_CTL0_ENABLE_MASK) ==
            UART_CTL0_ENABLE_MASK)
        {
            // 中文说明：按备份状态恢复使能，避免改变原始运行态。
            DL_UART_enable(uart);
        }
        ptr->backupRdy = false;
    }
    return stateRestored;
}

#endif /* __MSPM0_HAS_UART_EXTD__ */

#endif /* __MSPM0_HAS_UART_MAIN__ || __MSPM0_HAS_UART_EXTD__ */
