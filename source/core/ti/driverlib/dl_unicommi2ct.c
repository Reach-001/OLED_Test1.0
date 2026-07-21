/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#include <ti/driverlib/dl_unicommi2ct.h>

#ifdef __MCU_HAS_UNICOMMI2CT__

/**
 * @brief Configure I2C-target mode clock for UNICOMM.
 * @details 功能：配置 UNICOMM I2C 目标模式的时钟源与分频。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] config 参数：I2C 目标模式时钟配置结构体。
 * @return 返回值：无。
 */
void DL_I2CT_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ClockConfig *config)
{
    DL_Common_updateReg(&unicomm->i2ct->CLKSEL, (uint32_t)config->clockSel,
                        UNICOMMI2CT_CLKSEL_BUSCLK_SEL_MASK |
                            UNICOMMI2CT_CLKSEL_MFCLK_SEL_MASK);

    DL_Common_updateReg(&unicomm->i2ct->CLKDIV, (uint32_t)config->divideRatio,
                        UNICOMMI2CT_CLKDIV_RATIO_MASK);
}

/**
 * @brief Read I2C-target mode clock configuration from UNICOMM.
 * @details 功能：读取 UNICOMM I2C 目标模式时钟源与分频配置。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] config 参数：用于回填的时钟配置结构体。
 * @return 返回值：无。
 */
void DL_I2CT_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ClockConfig *config)
{
    uint32_t clockSel =
        unicomm->i2ct->CLKSEL & (UNICOMMI2CT_CLKSEL_BUSCLK_SEL_MASK |
                                 UNICOMMI2CT_CLKSEL_MFCLK_SEL_MASK);
    config->clockSel = (DL_I2CT_CLOCK)(clockSel);

    uint32_t divideRatio =
        unicomm->i2ct->CLKDIV & UNICOMMI2CT_CLKDIV_RATIO_MASK;
    config->divideRatio = (DL_I2CT_CLOCK_DIVIDE)(divideRatio);
}

/**
 * @brief Fill UNICOMM I2CT TX FIFO from buffer.
 * @details 功能：批量写入 TX FIFO，直到 FIFO 满或达到目标数量。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入字节数。
 * @return 返回值：实际写入字节数。
 */
uint8_t DL_I2CT_fillTXFIFO(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint8_t count)
{
    uint8_t i;
    for (i = (uint8_t)0; i < count; i++)
    {
        // 中文说明：FIFO 未满时写入，FIFO 满时提前结束。
        if (DL_I2CT_isTXFIFOFull(unicomm) == false)
        {
            DL_I2CT_transmitData(unicomm, buffer[i]);
        }
        else
        { //  TX FIFO is full
            break;
        }
    }
    return i;
}

/**
 * @brief Blocking transmit one byte in I2CT mode.
 * @details 功能：等待发送请求有效后发送 1 字节，并等待总线空闲。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送字节。
 * @return 返回值：无。
 */
void DL_I2CT_transmitDataBlocking(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    while ((DL_I2CT_getStatus(unicomm) & DL_I2CT_STATUS_TRANSMIT_REQUEST) !=
           DL_I2CT_STATUS_TRANSMIT_REQUEST)
    {
        // 中文说明：等待控制器发起读请求。
        ;
    }
    DL_I2CT_transmitData(unicomm, data);
    while ((DL_I2CT_getStatus(unicomm) & DL_I2CT_STATUS_BUS_BUSY) ==
           DL_I2CT_STATUS_BUS_BUSY)
    {
        // 中文说明：等待本次事务结束。
        ;
    }
}

/**
 * @brief Try to transmit one byte in I2CT mode.
 * @details 功能：TX FIFO 未满时发送 1 字节，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] data 参数：待发送字节。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 满。
 */
bool DL_I2CT_transmitDataCheck(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    bool status;
    if (DL_I2CT_isTXFIFOFull(unicomm))
    {
        status = false;
    }
    else
    {
        DL_I2CT_transmitData(unicomm, data);
        status = true;
    }
    return status;
}

/**
 * @brief Blocking receive one byte in I2CT mode.
 * @details 功能：等待接收请求有效后读取 1 字节数据。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：接收到的字节。
 */
uint8_t DL_I2CT_receiveDataBlocking(UNICOMM_Inst_Regs *unicomm)
{
    while ((DL_I2CT_getStatus(unicomm) & DL_I2CT_STATUS_RECEIVE_REQUEST) !=
           DL_I2CT_STATUS_RECEIVE_REQUEST)
    {
        // 中文说明：等待控制器写入数据请求。
        ;
    }
    return DL_I2CT_receiveData(unicomm);
}

/**
 * @brief Try to receive one byte in I2CT mode.
 * @details 功能：RX FIFO 非空时读取 1 字节，否则返回失败。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] buffer 参数：接收输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 空。
 */
bool DL_I2CT_receiveDataCheck(UNICOMM_Inst_Regs *unicomm, uint8_t *buffer)
{
    bool status;
    if (DL_I2CT_isRXFIFOEmpty(unicomm))
    {
        status = false;
    }
    else
    {
        *buffer = DL_I2CT_receiveData(unicomm);
        status = true;
    }
    return status;
}

/**
 * @brief Flush UNICOMM I2CT TX FIFO.
 * @details 功能：清空 I2CT TX FIFO 并等待硬件完成。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：无。
 */
void DL_I2CT_flushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_I2CT_startFlushTXFIFO(unicomm);
    while (DL_I2CT_isTXFIFOEmpty(unicomm) == false)
    {
        // 中文说明：轮询直到 FIFO 为空。
        ;
    }
    DL_I2CT_stopFlushTXFIFO(unicomm);
}

/**
 * @brief Flush UNICOMM I2CT RX FIFO.
 * @details 功能：清空 I2CT RX FIFO 并等待硬件完成。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：无。
 */
void DL_I2CT_flushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_I2CT_startFlushRXFIFO(unicomm);
    while (DL_I2CT_isRXFIFOEmpty(unicomm) == false)
    {
        // 中文说明：轮询直到 RX FIFO 无残留数据。
        ;
    }
    DL_I2CT_stopFlushRXFIFO(unicomm);
}

#endif /* __MCU_HAS_UNICOMMI2CT__ */
