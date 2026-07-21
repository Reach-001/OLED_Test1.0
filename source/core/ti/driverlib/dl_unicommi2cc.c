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

#include <ti/driverlib/dl_unicommi2cc.h>

#ifdef __MCU_HAS_UNICOMMI2CC__

/**
 * @brief Configure I2C-controller mode clock for UNICOMM.
 * @details 功能：配置 UNICOMM I2C 控制器模式的时钟源与分频。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] config 参数：I2C 控制器时钟配置结构体。
 * @return 返回值：无。
 */
void DL_I2CC_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_ClockConfig *config)
{
    DL_Common_updateReg(&unicomm->i2cc->CLKSEL, (uint32_t)config->clockSel,
                        UNICOMMI2CC_CLKSEL_BUSCLK_SEL_MASK |
                            UNICOMMI2CC_CLKSEL_MFCLK_SEL_MASK);

    DL_Common_updateReg(&unicomm->i2cc->CLKDIV, (uint32_t)config->divideRatio,
                        UNICOMMI2CC_CLKDIV_RATIO_MASK);
}

/**
 * @brief Read I2C-controller mode clock configuration from UNICOMM.
 * @details 功能：读取 UNICOMM I2C 控制器模式时钟源与分频。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[out] config 参数：用于回填的时钟配置结构体。
 * @return 返回值：无。
 */
void DL_I2CC_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_ClockConfig *config)
{
    uint32_t clockSel =
        unicomm->i2cc->CLKSEL & (UNICOMMI2CC_CLKSEL_BUSCLK_SEL_MASK |
                                 UNICOMMI2CC_CLKSEL_MFCLK_SEL_MASK);
    config->clockSel = (DL_I2CC_CLOCK)(clockSel);

    uint32_t divideRatio =
        unicomm->i2cc->CLKDIV & UNICOMMI2CC_CLKDIV_RATIO_MASK;
    config->divideRatio = (DL_I2CC_CLOCK_DIVIDE)(divideRatio);
}

/**
 * @brief Fill UNICOMM I2CC TX FIFO from buffer.
 * @details 功能：批量写入 TX FIFO，直到 FIFO 满或达到目标数量。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：最大写入字节数。
 * @return 返回值：实际写入字节数。
 */
uint16_t DL_I2CC_fillTXFIFO(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint16_t count)
{
    uint16_t i;
    for (i = (uint16_t)0; i < count; i++)
    {
        // 中文说明：仅在 FIFO 非满时写入，避免覆盖待发送数据。
        if (DL_I2CC_isTXFIFOFull(unicomm) == false)
        {
            DL_I2CC_transmitData(unicomm, buffer[i]);
        }
        else
        { //  TX FIFO is full
            // 中文说明：FIFO 满则提前结束。
            break;
        }
    }
    return i;
}

/**
 * @brief Flush UNICOMM I2CC TX FIFO.
 * @details 功能：清空 I2CC TX FIFO 并等待硬件完成。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：无。
 */
void DL_I2CC_flushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_I2CC_startFlushTXFIFO(unicomm);
    while (DL_I2CC_isTXFIFOEmpty(unicomm) == false)
    {
        // 中文说明：轮询直到 FIFO 清空。
        ;
    }
    DL_I2CC_stopFlushTXFIFO(unicomm);
}

/**
 * @brief Flush UNICOMM I2CC RX FIFO.
 * @details 功能：清空 I2CC RX FIFO 并等待硬件完成。
 * @param[in] unicomm 参数：UNICOMM 外设寄存器实例。
 * @return 返回值：无。
 */
void DL_I2CC_flushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_I2CC_startFlushRXFIFO(unicomm);
    while (DL_I2CC_isRXFIFOEmpty(unicomm) == false)
    {
        // 中文说明：轮询直到 RX FIFO 无残留数据。
        ;
    }
    DL_I2CC_stopFlushRXFIFO(unicomm);
}

#endif /* __MCU_HAS_UNICOMMI2CC__ */
