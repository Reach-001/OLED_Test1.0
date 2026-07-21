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
 * @brief 文件用途：MSPM0G3507 I2C 驱动库实现。
 *        提供控制器/目标模式的时钟配置、FIFO 填充与刷新、
 *        阻塞式及非阻塞式数据收发等底层操作。
 */

#include <ti/driverlib/dl_i2c.h>

#ifdef __MSPM0_HAS_I2C__

/**
 *  @brief I2C Controller APIs
 *  @details 中文说明：控制器模式下的时钟与 FIFO 访问辅助接口。
 */

/**
 * @brief Configure I2C controller clock source and divider.
 * @details 功能：配置 I2C 控制器的时钟源与分频参数。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[in] config 参数：时钟配置结构体，包含时钟源与分频比。
 * @return 返回值：无。
 */
void DL_I2C_setClockConfig(I2C_Regs *i2c, const DL_I2C_ClockConfig *config)
{
    DL_Common_updateReg(&i2c->CLKSEL, (uint32_t)config->clockSel,
                        I2C_CLKSEL_BUSCLK_SEL_MASK | I2C_CLKSEL_MFCLK_SEL_MASK);

    DL_Common_updateReg(
        &i2c->CLKDIV, (uint32_t)config->divideRatio, I2C_CLKDIV_RATIO_MASK);
}

/**
 * @brief Get current I2C controller clock configuration.
 * @details 功能：读取 I2C 控制器当前的时钟源与分频配置。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[out] config 参数：用于回填时钟配置的结构体指针。
 * @return 返回值：无。
 */
void DL_I2C_getClockConfig(const I2C_Regs *i2c, DL_I2C_ClockConfig *config)
{
    uint32_t clockSel =
        i2c->CLKSEL & (I2C_CLKSEL_BUSCLK_SEL_MASK | I2C_CLKSEL_MFCLK_SEL_MASK);
    config->clockSel = (DL_I2C_CLOCK)(clockSel);

    uint32_t divideRatio = i2c->CLKDIV & I2C_CLKDIV_RATIO_MASK;
    config->divideRatio = (DL_I2C_CLOCK_DIVIDE)(divideRatio);
}

/**
 * @brief Fill controller TX FIFO from software buffer.
 * @details 功能：按顺序将缓存区数据写入控制器 TX FIFO，遇满即停止。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[in] buffer 参数：待发送数据缓冲区。
 * @param[in] count 参数：期望写入的字节数。
 * @return 返回值：实际成功写入 FIFO 的字节数。
 */
uint16_t DL_I2C_fillControllerTXFIFO(
    I2C_Regs *i2c, const uint8_t *buffer, uint16_t count)
{
    uint16_t i;
    for (i = (uint16_t)0; i < count; i++)
    {
        // 中文说明：逐字节尝试入队，确保不覆盖硬件 FIFO 内容。
        if (DL_I2C_isControllerTXFIFOFull(i2c) == false)
        {
            DL_I2C_transmitControllerData(i2c, buffer[i]);
        }
        else
        { // Controller TX FIFO is full
            // 中文说明：FIFO 已满，立即退出并返回当前已写入数量。
            break;
        }
    }
    return i;
}

/**
 * @brief Flush all entries in controller TX FIFO.
 * @details 功能：触发控制器 TX FIFO 清空流程，并等待硬件清空完成。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @return 返回值：无。
 */
void DL_I2C_flushControllerTXFIFO(I2C_Regs *i2c)
{
    DL_I2C_startFlushControllerTXFIFO(i2c);
    while (DL_I2C_isControllerTXFIFOEmpty(i2c) == false)
    {
        // 中文说明：轮询直到 FIFO 为空，保证后续发送从干净状态开始。
        ;
    }
    DL_I2C_stopFlushControllerTXFIFO(i2c);
}

/**
 * @brief Flush all entries in controller RX FIFO.
 * @details 功能：触发控制器 RX FIFO 清空流程，并等待硬件清空完成。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @return 返回值：无。
 */
void DL_I2C_flushControllerRXFIFO(I2C_Regs *i2c)
{
    DL_I2C_startFlushControllerRXFIFO(i2c);
    while (DL_I2C_isControllerRXFIFOEmpty(i2c) == false)
    {
        // 中文说明：等待 RX FIFO 清空，避免旧数据影响新事务。
        ;
    }
    DL_I2C_stopFlushControllerRXFIFO(i2c);
}

/**
 *  @brief I2C Target APIs
 *  @details 中文说明：目标模式下的 FIFO 读写与阻塞访问辅助接口。
 */
/**
 * @brief Fill target TX FIFO from software buffer.
 * @details 功能：向目标模式 TX FIFO 装载数据，满则停止。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[in] buffer 参数：待发送数据缓冲区。
 * @param[in] count 参数：期望写入的字节数。
 * @return 返回值：实际成功写入 FIFO 的字节数。
 */
uint8_t DL_I2C_fillTargetTXFIFO(
    I2C_Regs *i2c, const uint8_t *buffer, uint8_t count)
{
    uint8_t i;
    for (i = (uint8_t)0; i < count; i++)
    {
        // 中文说明：仅在 FIFO 可写时送入一个字节。
        if (DL_I2C_isTargetTXFIFOFull(i2c) == false)
        {
            DL_I2C_transmitTargetData(i2c, buffer[i]);
        }
        else
        { // Target TX FIFO is full
            // 中文说明：硬件队列已满，退出循环并返回已写入长度。
            break;
        }
    }
    return i;
}

/**
 * @brief Flush all entries in target TX FIFO.
 * @details 功能：清空目标模式 TX FIFO，并阻塞等待清空完成。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @return 返回值：无。
 */
void DL_I2C_flushTargetTXFIFO(I2C_Regs *i2c)
{
    DL_I2C_startFlushTargetTXFIFO(i2c);
    while (DL_I2C_isTargetTXFIFOEmpty(i2c) == false)
    {
        // 中文说明：等待 FIFO 空标志，确保缓存区被完全回收。
        ;
    }
    DL_I2C_stopFlushTargetTXFIFO(i2c);
}

/**
 * @brief Flush all entries in target RX FIFO.
 * @details 功能：清空目标模式 RX FIFO，并阻塞等待清空完成。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @return 返回值：无。
 */
void DL_I2C_flushTargetRXFIFO(I2C_Regs *i2c)
{
    DL_I2C_startFlushTargetRXFIFO(i2c);
    while (DL_I2C_isTargetRXFIFOEmpty(i2c) == false)
    {
        // 中文说明：等待所有接收残留数据被硬件丢弃。
        ;
    }
    DL_I2C_stopFlushTargetRXFIFO(i2c);
}

/**
 * @brief Transmit one byte in target mode with blocking wait.
 * @details 功能：等待目标发送请求有效后发送 1 字节，并等待总线空闲。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[in] data 参数：待发送的 1 字节数据。
 * @return 返回值：无。
 */
void DL_I2C_transmitTargetDataBlocking(I2C_Regs *i2c, uint8_t data)
{
    while ((DL_I2C_getTargetStatus(i2c) &
            DL_I2C_TARGET_STATUS_TRANSMIT_REQUEST) !=
           DL_I2C_TARGET_STATUS_TRANSMIT_REQUEST)
    {
        // 中文说明：等待控制器发起读请求，目标端才可安全回传数据。
        ;
    }
    DL_I2C_transmitTargetData(i2c, data);
    while ((DL_I2C_getTargetStatus(i2c) & DL_I2C_TARGET_STATUS_BUS_BUSY) ==
           DL_I2C_TARGET_STATUS_BUS_BUSY)
    {
        // 中文说明：等待本次传输真正结束，避免紧接着访问造成冲突。
        ;
    }
}

/**
 * @brief Try to transmit one byte in target mode.
 * @details 功能：在 TX FIFO 未满时发送 1 字节，并返回是否发送成功。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[in] data 参数：待发送的 1 字节数据。
 * @return 返回值：`true` 表示已写入，`false` 表示 FIFO 已满。
 */
bool DL_I2C_transmitTargetDataCheck(I2C_Regs *i2c, uint8_t data)
{
    bool status;
    if (DL_I2C_isTargetTXFIFOFull(i2c))
    {
        status = false;
    }
    else
    {
        DL_I2C_transmitTargetData(i2c, data);
        status = true;
    }
    return status;
}

/**
 * @brief Receive one byte in target mode with blocking wait.
 * @details 功能：等待目标接收请求有效后，从 RX 数据寄存器读取 1 字节。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @return 返回值：接收到的 1 字节数据。
 */
uint8_t DL_I2C_receiveTargetDataBlocking(const I2C_Regs *i2c)
{
    while (
        (DL_I2C_getTargetStatus(i2c) & DL_I2C_TARGET_STATUS_RECEIVE_REQUEST) !=
        DL_I2C_TARGET_STATUS_RECEIVE_REQUEST)
    {
        // 中文说明：等待控制器发起写请求，确保读取到有效新数据。
        ;
    }
    return DL_I2C_receiveTargetData(i2c);
}

/**
 * @brief Try to receive one byte in target mode.
 * @details 功能：在 RX FIFO 非空时读取 1 字节，并返回读取是否成功。
 * @param[in] i2c 参数：I2C 外设寄存器基址。
 * @param[out] buffer 参数：用于保存读取字节的输出指针。
 * @return 返回值：`true` 表示读取成功，`false` 表示 FIFO 为空。
 */
bool DL_I2C_receiveTargetDataCheck(const I2C_Regs *i2c, uint8_t *buffer)
{
    bool status;
    if (DL_I2C_isTargetRXFIFOEmpty(i2c))
    {
        status = false;
    }
    else
    {
        *buffer = DL_I2C_receiveTargetData(i2c);
        status = true;
    }
    return status;
}

#endif /* __MSPM0_HAS_I2C__ */
