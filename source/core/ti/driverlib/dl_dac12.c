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
 *
 * @par 中文文件说明
 * MSPM0G3507 12位 DAC 驱动库源文件。
 * 实现 DAC12 模块的初始化、阻塞输出、FIFO 批量填充和自校准等非内联函数。
 */

#include <ti/driverlib/dl_dac12.h>

#ifdef __MSPM0_HAS_DAC12__

/**
 * @brief 初始化 DAC12 外设配置。
 *
 * @param[in] dac12 DAC12 外设寄存器基址。
 * @param[in] config DAC12 初始化配置。
 *
 * @return 无返回值。
 */
void DL_DAC12_init(DAC12_Regs *dac12, const DL_DAC12_Config *config)
{
    /* CTL0 - 配置输入数据格式（表示方式和分辨率） */
    DL_DAC12_configDataFormat(
        dac12, config->representation, config->resolution);

    /* CTL1 - 配置输出使能、参考电压源和放大器 */
    DL_Common_updateReg(&dac12->CTL1,
                        (uint32_t)config->outputEnable |
                            (uint32_t)config->voltageReferenceSource |
                            (uint32_t)config->amplifierSetting,
                        DAC12_CTL1_OPS_MASK | DAC12_CTL1_REFSP_MASK | DAC12_CTL1_REFSN_MASK |
                            DAC12_CTL1_AMPEN_MASK | DAC12_CTL1_AMPHIZ_MASK);

    /* CTL2 - 配置FIFO、触发源和DMA触发阈值 */
    DL_Common_updateReg(&dac12->CTL2,
                        ((uint32_t)config->fifoEnable | (uint32_t)config->fifoTriggerSource |
                         (uint32_t)config->dmaTriggerEnable |
                         (uint32_t)config->dmaTriggerThreshold),
                        DAC12_CTL2_DMATRIGEN_MASK | DAC12_CTL2_FIFOTH_MASK |
                            DAC12_CTL2_FIFOEN_MASK | DAC12_CTL2_FIFOTRIGSEL_MASK);

    /* CTL3 - 配置采样定时器使能和采样率 */
    DL_Common_updateReg(&dac12->CTL3,
                        ((uint32_t)config->sampleTimeGeneratorEnable |
                         (uint32_t)config->sampleRate),
                        DAC12_CTL3_STIMCONFIG_MASK | DAC12_CTL3_STIMEN_MASK);
}

/**
 * @brief 阻塞写入一个 8 位样本到 DAC FIFO。
 *
 * @param[in] dac12 DAC12 外设寄存器基址。
 * @param[in] data 待写入的 8 位数据。
 *
 * @return 无返回值。
 */
void DL_DAC12_outputBlocking8(DAC12_Regs *dac12, uint8_t data)
{
    /* FIFO 满时等待，避免覆盖未消耗样本 */
    while (DL_DAC12_isFIFOFull(dac12))
    {
        ;
    }

    DL_DAC12_output8(dac12, data);
}

/**
 * @brief 阻塞写入一个 12 位样本到 DAC FIFO。
 *
 * @param[in] dac12 DAC12 外设寄存器基址。
 * @param[in] data 待写入的 12 位数据。
 *
 * @return 无返回值。
 */
void DL_DAC12_outputBlocking12(DAC12_Regs *dac12, uint16_t data)
{
    /* FIFO 满时等待，保证写入时序正确 */
    while (DL_DAC12_isFIFOFull(dac12))
    {
        ;
    }

    DL_DAC12_output12(dac12, data);
}

/**
 * @brief 批量写入 8 位样本到 DAC FIFO。
 *
 * @param[in] dac12 DAC12 外设寄存器基址。
 * @param[in] buffer 源数据缓冲区。
 * @param[in] count 最大尝试写入数量。
 *
 * @return 实际成功写入样本数。
 */
uint32_t DL_DAC12_fillFIFO8(
    DAC12_Regs *dac12, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_DAC12_isFIFOFull(dac12))  /* FIFO 未满时继续写入 */
        {
            DL_DAC12_output8(dac12, buffer[i]);
        }
        else
        {
            break;  /* FIFO 满则提前退出 */
        }
    }

    return i;  /* 返回实际写入样本数 */
}

/**
 * @brief 批量写入 12 位样本到 DAC FIFO。
 *
 * @param[in] dac12 DAC12 外设寄存器基址。
 * @param[in] buffer 源数据缓冲区。
 * @param[in] count 最大尝试写入数量。
 *
 * @return 实际成功写入样本数。
 */
uint32_t DL_DAC12_fillFIFO12(
    DAC12_Regs *dac12, const uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        if (!DL_DAC12_isFIFOFull(dac12))  /* FIFO 未满时继续写入 */
        {
            DL_DAC12_output12(dac12, buffer[i]);
        }
        else
        {
            break;  /* FIFO 满则提前退出 */
        }
    }

    return i;
}

/**
 * @brief 启动并阻塞等待 DAC 自校准完成。
 *
 * @param[in] dac12 DAC12 外设寄存器基址。
 *
 * @return 无返回值。
 */
void DL_DAC12_performSelfCalibrationBlocking(DAC12_Regs *dac12)
{
    DL_DAC12_startCalibration(dac12);
    /*
     * Tri-stated during calibration, and CALON will continue to be set
     * Prevents the function from exiting until the calibration is finished.
     */
    /* CALON 清零前持续等待，确保读取到稳定校准结果 */
    while (DL_DAC12_isCalibrationRunning(dac12))
    {
        ;
    }
}

#endif /* __MSPM0_HAS_DAC12__ */
