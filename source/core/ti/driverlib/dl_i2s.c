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

/**
 *  @file        dl_i2s.c
 *  @brief       I2S driver implementation
 *               I2S 驱动实现文件，提供时钟配置、模式切换、FIFO 管理及数据收发接口。
 */

#include <ti/driverlib/dl_i2s.h>

#ifdef __MSPM0_HAS_I2S__

/**
 *  @brief I2S APIs
 *  @details 中文说明：I2S 时钟、模式、FIFO 与数据收发辅助接口。
 */

/**
 * @brief Configure I2S clock tree.
 * @details 功能：配置 I2S 时钟源、WCLK/BCLK 分频与相位参数。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] config 参数：I2S 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_I2S_setClockConfig(I2S_Regs *i2s, const DL_I2S_ClockConfig *config)
{
    i2s->GPRCM.CLKCFG = I2S_CLKCFG_KEY_UNLOCK | config->clockSel; // 解锁并选择时钟源

    DL_Common_updateReg(&i2s->WCLKSRC, (uint32_t)config->wordBaudClockSource,
                        I2S_WCLKSRC_WBCLKSRC_MASK); // 设置音频时钟来源

    DL_Common_updateReg(
        &i2s->CLKCTL, (uint32_t)config->wclkPhase, I2S_CLKCTL_WCLKPHASE_MASK); // 设置 WCLK 分频模式

    i2s->WCLKDIV = config->wclkDivider; // 设置 WCLK 分频系数
    i2s->BCLKDIV = config->bclkDivider; // 设置 BCLK 分频系数
}

/**
 * @brief Read I2S clock tree configuration.
 * @details 功能：读取 I2S 时钟源、WCLK/BCLK 分频与相位配置。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] config 参数：用于回填的 I2S 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_I2S_getClockConfig(const I2S_Regs *i2s, DL_I2S_ClockConfig *config)
{
    uint32_t clockSel = i2s->GPRCM.CLKCFG & I2S_CLKCFG_DAICLK_MASK; // 读取时钟源
    config->clockSel = (DL_I2S_CLOCK_SOURCE)(clockSel);

    uint32_t wordBaudClockSource = i2s->WCLKSRC & I2S_WCLKSRC_WBCLKSRC_MASK; // 读取音频时钟来源
    config->wordBaudClockSource =
        (DL_I2S_WORD_BAUD_CLOCK_SOURCE)(wordBaudClockSource);

    uint32_t wclkPhase = i2s->CLKCTL & I2S_CLKCTL_WCLKPHASE_MASK; // 读取 WCLK 分频模式
    config->wclkPhase = (DL_I2S_WCLK_PHASE)(wclkPhase);

    config->wclkDivider = i2s->WCLKDIV; // 读取 WCLK 分频系数
    config->bclkDivider = i2s->BCLKDIV; // 读取 BCLK 分频系数
}

/**
 * @brief Initialize I2S format, direction and clock output behavior.
 * @details 功能：按配置初始化 I2S 数据格式、数据引脚方向和时钟输出模式。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] config 参数：I2S 初始化配置结构体。
 * @return 返回值：无。
 */
void DL_I2S_init(I2S_Regs *i2s, const DL_I2S_Config *config)
{
    DL_Common_updateReg(&i2s->WCLKSRC, (uint32_t)config->wclkInvert,
                        I2S_WCLKSRC_WCLKINV_MASK); // 设置 WCLK 极性

    DL_Common_updateReg(&i2s->FMTCFG,
                        ((uint32_t)config->emptySlotOutput | (uint32_t)config->dataDelay |
                         (uint32_t)config->memoryAccessLength |
                         (uint32_t)config->samplingEdge | (uint32_t)config->phase),
                        (I2S_FMTCFG_EMPTYSLOTOUTPUT_MASK | I2S_FMTCFG_DATADLY_MASK |
                         I2S_FMTCFG_MEMLEN32_MASK | I2S_FMTCFG_SMPLEDGE_MASK |
                         I2S_FMTCFG_DUALPHASE_MASK)); // 配置数据格式参数

    DL_I2S_setSampleWordLength(i2s, config->sampleWordLength); // 设置样本字长

    DL_Common_updateReg(&i2s->DIRCFG,
                        ((uint32_t)config->dataPin0Direction) << I2S_DIRCFG_AD0_OFS,
                        I2S_DIRCFG_AD0_MASK); // 设置 AD0 方向
    i2s->WMASK0 = config->dataPin0ValidChannelMask; // 设置 AD0 通道掩码

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    DL_Common_updateReg(&i2s->DIRCFG,
                        ((uint32_t)config->dataPin1Direction) << I2S_DIRCFG_AD1_OFS,
                        I2S_DIRCFG_AD1_MASK);
    i2s->WMASK1 = config->dataPin1ValidChannelMask;
#endif

    if (config->mode == DL_I2S_MODE_CONTROLLER)
    {
        // 中文说明：控制器模式下由本模块驱动时钟线。
        if (config->enableMCLK)
        {
            /* If optional MCLK generation is required, configure MCLK */
            // 中文说明：同时输出 MCLK/WCLK/BCLK，用于外部音频器件主时钟输入。
            DL_Common_updateReg(&i2s->CLKCTL,
                                (I2S_CLKCTL_WBEN_EN | I2S_CLKCTL_MEN_EN),
                                (I2S_CLKCTL_WBEN_MASK | I2S_CLKCTL_MEN_MASK));
        }
        else
        {
            /* Otherwise generate WCLK and BCLK only */
            // 中文说明：仅输出 WCLK/BCLK，关闭 MCLK 以降低外设耦合。
            DL_Common_updateReg(&i2s->CLKCTL,
                                (I2S_CLKCTL_WBEN_EN | I2S_CLKCTL_MEN_DIS),
                                (I2S_CLKCTL_WBEN_MASK | I2S_CLKCTL_MEN_MASK));
        }
    }
    else
    {
        /* In target mode, WCLK and BCLK are input signals */
        // 中文说明：目标模式下时钟由外部提供，本模块仅消费时钟。
        DL_Common_updateReg(&i2s->CLKCTL,
                            I2S_CLKCTL_WBEN_DIS | I2S_CLKCTL_MEN_DIS,
                            I2S_CLKCTL_WBEN_MASK | I2S_CLKCTL_MEN_MASK);
    }

    /* Divider can be set outside of controller mode */
    i2s->MCLKDIV = config->mclkDivider; // 设置 MCLK 分频系数
}

/**
 * @brief Get current I2S operating mode.
 * @details 功能：根据 W/BCLK 生成使能状态判断当前为控制器或目标模式。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @return 返回值：当前 I2S 模式。
 */
DL_I2S_MODE DL_I2S_getMode(const I2S_Regs *i2s)
{
    DL_I2S_MODE mode = DL_I2S_MODE_CONTROLLER;

    if (DL_I2S_isWBCLKGenerationEnabled(i2s))
    {
        mode = DL_I2S_MODE_CONTROLLER; // W/BCLK 输出使能则为控制器模式
    }
    else
    {
        mode = DL_I2S_MODE_TARGET; // 否则为目标模式
    }

    return mode;
}

/**
 * @brief Set I2S operating mode.
 * @details 功能：在控制器模式下开启时钟输出，在目标模式下关闭本地时钟输出。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] mode 参数：目标设置模式。
 * @return 返回值：无。
 */
void DL_I2S_setMode(I2S_Regs *i2s, DL_I2S_MODE mode)
{
    if (mode == DL_I2S_MODE_CONTROLLER)
    {
        /* Controller mode means that I2S generates WCLK and BCLK */
        DL_I2S_enableWBCLKGeneration(i2s); // 控制器模式：开启时钟输出
    }
    else
    {
        /* Target mode means that I2S consumes WCLK and BCLK */
        DL_I2S_disableWBCLKGeneration(i2s); // 目标模式：关闭本地时钟输出
    }
}

/**
 * @brief Blocking transmit 8-bit sample data.
 * @details 功能：等待 TX FIFO 有空间后发送 8 位数据。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] data 参数：待发送 8 位数据。
 * @return 返回值：无。
 */
void DL_I2S_transmitDataBlocking8(I2S_Regs *i2s, uint8_t data)
{
    while (DL_I2S_isTXFIFOFull(i2s))
    {
        // 中文说明：轮询等待 FIFO 可写，避免覆盖未发送数据。
        ;
    }

    DL_I2S_transmitData8(i2s, data);
}

/**
 * @brief Blocking transmit 16-bit sample data.
 * @details 功能：等待 TX FIFO 有空间后发送 16 位数据。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] data 参数：待发送 16 位数据。
 * @return 返回值：无。
 */
void DL_I2S_transmitDataBlocking16(I2S_Regs *i2s, uint16_t data)
{
    while (DL_I2S_isTXFIFOFull(i2s))
    {
        // 中文说明：硬件满时阻塞，保证发送顺序与完整性。
        ;
    }

    DL_I2S_transmitData16(i2s, data);
}

/**
 * @brief Blocking transmit 32-bit sample data.
 * @details 功能：等待 TX FIFO 有空间后发送 32 位数据。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] data 参数：待发送 32 位数据。
 * @return 返回值：无。
 */
void DL_I2S_transmitDataBlocking32(I2S_Regs *i2s, uint32_t data)
{
    while (DL_I2S_isTXFIFOFull(i2s))
    {
        // 中文说明：当发送队列已满时持续等待。
        ;
    }

    DL_I2S_transmitData32(i2s, data);
}

/**
 * @brief Blocking receive 8-bit sample data.
 * @details 功能：等待 RX FIFO 非空后读取 8 位数据。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @return 返回值：读取到的 8 位数据。
 */
uint8_t DL_I2S_receiveDataBlocking8(const I2S_Regs *i2s)
{
    while (DL_I2S_isRXFIFOEmpty(i2s))
    {
        // 中文说明：无数据时阻塞等待，直到接收有效数据。
        ;
    }

    return DL_I2S_receiveData8(i2s);
}

/**
 * @brief Blocking receive 16-bit sample data.
 * @details 功能：等待 RX FIFO 非空后读取 16 位数据。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @return 返回值：读取到的 16 位数据。
 */
uint16_t DL_I2S_receiveDataBlocking16(const I2S_Regs *i2s)
{
    while (DL_I2S_isRXFIFOEmpty(i2s))
    {
        // 中文说明：轮询等待数据到达。
        ;
    }

    return DL_I2S_receiveData16(i2s);
}

/**
 * @brief Blocking receive 32-bit sample data.
 * @details 功能：等待 RX FIFO 非空后读取 32 位数据。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @return 返回值：读取到的 32 位数据。
 */
uint32_t DL_I2S_receiveDataBlocking32(const I2S_Regs *i2s)
{
    while (DL_I2S_isRXFIFOEmpty(i2s))
    {
        // 中文说明：确保返回值来自硬件最新接收样本。
        ;
    }

    return DL_I2S_receiveData32(i2s);
}

/**
 * @brief Try to transmit one 8-bit sample.
 * @details 功能：检查 TX FIFO 空间后发送 8 位数据并返回状态。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] data 参数：待发送 8 位数据。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 已满。
 */
bool DL_I2S_transmitDataCheck8(I2S_Regs *i2s, uint8_t data)
{
    bool status;
    if (DL_I2S_isTXFIFOFull(i2s))
    {
        status = false; // FIFO 满，发送失败
    }
    else
    {
        DL_I2S_transmitData8(i2s, data);
        status = true; // 发送成功
    }
    return status;
}

/**
 * @brief Try to transmit one 16-bit sample.
 * @details 功能：检查 TX FIFO 空间后发送 16 位数据并返回状态。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] data 参数：待发送 16 位数据。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 已满。
 */
bool DL_I2S_transmitDataCheck16(I2S_Regs *i2s, uint16_t data)
{
    bool status;
    if (DL_I2S_isTXFIFOFull(i2s))
    {
        status = false; // FIFO 满，发送失败
    }
    else
    {
        DL_I2S_transmitData16(i2s, data);
        status = true; // 发送成功
    }
    return status;
}

/**
 * @brief Try to transmit one 32-bit sample.
 * @details 功能：检查 TX FIFO 空间后发送 32 位数据并返回状态。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] data 参数：待发送 32 位数据。
 * @return 返回值：`true` 发送成功，`false` 表示 FIFO 已满。
 */
bool DL_I2S_transmitDataCheck32(I2S_Regs *i2s, uint32_t data)
{
    bool status;
    if (DL_I2S_isTXFIFOFull(i2s))
    {
        status = false; // FIFO 满，发送失败
    }
    else
    {
        DL_I2S_transmitData32(i2s, data);
        status = true; // 发送成功
    }
    return status;
}

/**
 * @brief Try to receive one 8-bit sample.
 * @details 功能：当 RX FIFO 非空时读取 8 位数据并返回状态。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] buffer 参数：接收数据输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 为空。
 */
bool DL_I2S_receiveDataCheck8(const I2S_Regs *i2s, uint8_t *buffer)
{
    bool status;
    if (DL_I2S_isRXFIFOEmpty(i2s))
    {
        status = false; // FIFO 空，读取失败
    }
    else
    {
        *buffer = DL_I2S_receiveData8(i2s);
        status = true; // 读取成功
    }
    return status;
}

/**
 * @brief Try to receive one 16-bit sample.
 * @details 功能：当 RX FIFO 非空时读取 16 位数据并返回状态。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] buffer 参数：接收数据输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 为空。
 */
bool DL_I2S_receiveDataCheck16(const I2S_Regs *i2s, uint16_t *buffer)
{
    bool status;
    if (DL_I2S_isRXFIFOEmpty(i2s))
    {
        status = false; // FIFO 空，读取失败
    }
    else
    {
        *buffer = DL_I2S_receiveData16(i2s);
        status = true; // 读取成功
    }
    return status;
}

/**
 * @brief Try to receive one 32-bit sample.
 * @details 功能：当 RX FIFO 非空时读取 32 位数据并返回状态。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] buffer 参数：接收数据输出指针。
 * @return 返回值：`true` 读取成功，`false` 表示 FIFO 为空。
 */
bool DL_I2S_receiveDataCheck32(const I2S_Regs *i2s, uint32_t *buffer)
{
    bool status;
    if (DL_I2S_isRXFIFOEmpty(i2s))
    {
        status = false; // FIFO 空，读取失败
    }
    else
    {
        *buffer = DL_I2S_receiveData32(i2s);
        status = true; // 读取成功
    }
    return status;
}

/**
 * @brief Drain RX FIFO into 8-bit buffer.
 * @details 功能：批量读取 RX FIFO 到 8 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取元素数。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_I2S_drainRXFIFO8(
    const I2S_Regs *i2s, uint8_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_I2S_isRXFIFOEmpty(i2s))
        {
            // 中文说明：FIFO 非空时立即搬运一个样本到软件缓冲区。
            buffer[i] = DL_I2S_receiveData8(i2s);
        }
        else
        {
            // 中文说明：FIFO 已空则提前结束，避免无效读取。
            break;
        }
    }

    return i;
}

/**
 * @brief Drain RX FIFO into 16-bit buffer.
 * @details 功能：批量读取 RX FIFO 到 16 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取元素数。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_I2S_drainRXFIFO16(
    const I2S_Regs *i2s, uint16_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_I2S_isRXFIFOEmpty(i2s))
        {
            buffer[i] = DL_I2S_receiveData16(i2s); // FIFO 非空时搬运样本
        }
        else
        {
            break; // FIFO 已空，提前结束
        }
    }

    return i;
}

/**
 * @brief Drain RX FIFO into 32-bit buffer.
 * @details 功能：批量读取 RX FIFO 到 32 位缓冲区，直到 FIFO 空或达到上限。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[out] buffer 参数：接收缓冲区。
 * @param[in] maxCount 参数：最大读取元素数。
 * @return 返回值：实际读取元素数。
 */
uint32_t DL_I2S_drainRXFIFO32(
    const I2S_Regs *i2s, uint32_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++)
    {
        if (!DL_I2S_isRXFIFOEmpty(i2s))
        {
            buffer[i] = DL_I2S_receiveData32(i2s); // FIFO 非空时搬运样本
        }
        else
        {
            break; // FIFO 已空，提前结束
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 8-bit buffer.
 * @details 功能：将 8 位缓冲区数据批量写入 TX FIFO，遇满停止。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：期望写入元素数。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_I2S_fillTXFIFO8(
    I2S_Regs *i2s, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_I2S_isTXFIFOFull(i2s))
        {
            DL_I2S_transmitData8(i2s, buffer[i]); // FIFO 未满时写入样本
        }
        else
        {
            break; // FIFO 已满，提前结束
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 16-bit buffer.
 * @details 功能：将 16 位缓冲区数据批量写入 TX FIFO，遇满停止。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：期望写入元素数。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_I2S_fillTXFIFO16(
    I2S_Regs *i2s, const uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_I2S_isTXFIFOFull(i2s))
        {
            DL_I2S_transmitData16(i2s, buffer[i]); // FIFO 未满时写入样本
        }
        else
        {
            break; // FIFO 已满，提前结束
        }
    }

    return i;
}

/**
 * @brief Fill TX FIFO from 32-bit buffer.
 * @details 功能：将 32 位缓冲区数据批量写入 TX FIFO，遇满停止。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @param[in] buffer 参数：发送缓冲区。
 * @param[in] count 参数：期望写入元素数。
 * @return 返回值：实际写入元素数。
 */
uint32_t DL_I2S_fillTXFIFO32(
    I2S_Regs *i2s, const uint32_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_I2S_isTXFIFOFull(i2s))
        {
            DL_I2S_transmitData32(i2s, buffer[i]); // FIFO 未满时写入样本
        }
        else
        {
            break; // FIFO 已满，提前结束
        }
    }

    return i;
}

/**
 * @brief Clear TX FIFO content.
 * @details 功能：启动 TX FIFO 清空流程，并等待硬件确认完成后关闭清空位。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @return 返回值：无。
 */
void DL_I2S_clearTXFIFO(I2S_Regs *i2s)
{
    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_TXCLR_ENABLE, I2S_IFLS_TXCLR_MASK);

    while (!DL_I2S_isTXFIFOClearComplete(i2s))
    {
        // 中文说明：等待硬件完成 TX FIFO 清除操作。
        ;
    }

    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_TXCLR_DISABLE, I2S_IFLS_TXCLR_MASK);
}

/**
 * @brief Clear RX FIFO content.
 * @details 功能：启动 RX FIFO 清空流程，并等待硬件确认完成后关闭清空位。
 * @param[in] i2s 参数：I2S 外设寄存器基址。
 * @return 返回值：无。
 */
void DL_I2S_clearRXFIFO(I2S_Regs *i2s)
{
    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_RXCLR_ENABLE, I2S_IFLS_RXCLR_MASK);

    while (!DL_I2S_isRXFIFOClearComplete(i2s))
    {
        // 中文说明：等待硬件完成 RX FIFO 清除操作。
        ;
    }

    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_RXCLR_DISABLE, I2S_IFLS_RXCLR_MASK);
}

#endif /* __MSPM0_HAS_I2S__ */
