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
 *  @brief LCD 液晶显示模块驱动实现
 *         包含初始化、引脚功能配置、公共线/段线设置等功能
 */
#include <ti/driverlib/dl_lcd.h>

#ifdef __MSPM0_HAS_LCD__

/**
 *  @brief LCD APIs
 *  LCD 液晶显示模块驱动，提供初始化、引脚功能配置、公共线/段线设置等功能
 */

void DL_LCD_init(LCD_Regs *lcd, const DL_LCD_Config *config)
{
    /* 更新 LCDCTL0 寄存器：频率分频、MUX 速率、低功耗波形 */
    DL_Common_updateReg(&lcd->LCDCTL0,
        (uint32_t) config->frequencyDivider | (uint32_t) config->muxRate |
            (uint32_t) config->lowPowerWaveform,
        LCD_LCDCTL0_LCDDIVX_MASK | LCD_LCDCTL0_LCDMXX_MASK |
            LCD_LCDCTL0_LCDLP_MASK);
}

void DL_LCD_setPinAsLCDFunction(LCD_Regs *lcd, uint32_t pin)
{
    /* 先关闭 LCD 模块 */
    lcd->LCDCTL0 &= ~(LCD_LCDCTL0_LCDON_MASK);

    uint8_t idx  = pin >> 4;          /* 根据引脚号计算端口控制寄存器索引 */
    uint16_t val = 1 << (pin & 0x0F); /* 计算对应位的掩码 */

    /* 根据索引选择对应的 LCD 端口控制寄存器，设置引脚为 LCD 功能 */
    switch (idx) {
        case 0:
            lcd->LCDPCTL0 |= val;
            break;
        case 1:
            lcd->LCDPCTL1 |= val;
            break;
        case 2:
            lcd->LCDPCTL2 |= val;
            break;
        case 3:
            lcd->LCDPCTL3 |= val;
            break;
        default:
            break;
    }
}

void DL_LCD_setPinAsPortFunction(LCD_Regs *lcd, uint8_t pin)
{
    /* 先关闭 LCD 模块 */
    lcd->LCDCTL0 &= ~(LCD_LCDCTL0_LCDON_MASK);

    uint8_t idx  = pin >> 4;          /* 根据引脚号计算端口控制寄存器索引 */
    uint16_t val = 1 << (pin & 0x0F); /* 计算对应位的掩码 */

    /* 根据索引选择对应的 LCD 端口控制寄存器，清除引脚的 LCD 功能 */
    switch (idx) {
        case 0:
            lcd->LCDPCTL0 &= ~(val);
            break;
        case 1:
            lcd->LCDPCTL1 &= ~(val);
            break;
        case 2:
            lcd->LCDPCTL2 &= ~(val);
            break;
        case 3:
            lcd->LCDPCTL3 &= ~(val);
            break;
        default:
            break;
    }
}

void DL_LCD_setPinAsCommon(LCD_Regs *lcd, uint8_t pin, uint32_t com)
{
    /* 先关闭 LCD 模块 */
    lcd->LCDCTL0 &= ~(LCD_LCDCTL0_LCDON_MASK);

    /* 引脚值超过 64 则直接返回 */
    if (pin > 64) {
        return;
    }

    uint8_t idx  = pin >> 4;          /* 0-15 -> 0, 16-31 -> 1, 等 */
    uint16_t val = 1 << (pin & 0x0F); /* 根据引脚号计算位掩码 */
    uint8_t muxRate = lcd->LCDCTL0 & (LCD_LCDCTL0_LCDMXX_MASK); /* 获取当前 MUX 速率 */

    /* 在 LCDCSSEL 寄存器中将引脚设为公共线 */
    switch (idx) {
        case 0:
            lcd->LCDCSSEL0 |= val;
            break;
        case 1:
            lcd->LCDCSSEL1 |= val;
            break;
        case 2:
            lcd->LCDCSSEL2 |= val;
            break;
        case 3:
            lcd->LCDCSSEL3 |= val;
            break;
        default:
            break;
    }

    /* 在 LCD 内存中配置对应引脚的 COM 映射 */
    if ((muxRate == LCD_LCDCTL0_LCDMXX_MX_STATIC) ||
        (muxRate == LCD_LCDCTL0_LCDMXX_MX_2) ||
        (muxRate == LCD_LCDCTL0_LCDMXX_MX_3) ||
        (muxRate == LCD_LCDCTL0_LCDMXX_MX_4)) {
        /* 1-mux 到 4-mux 模式：每个字节包含两个引脚的 COM 信息 */
        if (pin & 1) {
            lcd->LCDM[pin / 2] |= (com & 0xF) << 4; /* 奇数引脚存高半字节 */
        } else {
            lcd->LCDM[pin / 2] |= (com & 0xF);       /* 偶数引脚存低半字节 */
        }
    } else {
        /* 5-mux 到 8-mux 模式：每个引脚占一个完整字节 */
        lcd->LCDM[pin] |= com;
    }
}

void DL_LCD_setPinAsSegment(LCD_Regs *lcd, uint8_t pin)
{
    /* 先关闭 LCD 模块 */
    lcd->LCDCTL0 &= ~(LCD_LCDCTL0_LCDON_MASK);

    uint8_t idx  = pin >> 4;          /* 0-15 -> 0, 16-31 -> 1, 等 */
    uint16_t val = 1 << (pin & 0x0F); /* 根据引脚号计算位掩码 */

    /* 在 LCDCSSEL 寄存器中清除引脚的公共线选择，恢复为段线 */
    switch (idx) {
        case 0:
            lcd->LCDCSSEL0 &= ~val;
            break;
        case 1:
            lcd->LCDCSSEL1 &= ~val;
            break;
        case 2:
            lcd->LCDCSSEL2 &= ~val;
            break;
        case 3:
            lcd->LCDCSSEL3 &= ~val;
            break;
        default:
            break;
    }
}

#endif /* __MSPM0_HAS_LCD__ */
