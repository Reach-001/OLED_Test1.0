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
 *  @brief 工厂区域驱动实现
 *         提供 TRIM 参数到 SRAM 缓存表的初始化功能。
 */

#include <stdint.h>

#include <ti/driverlib/m0p/dl_factoryregion.h>

#if ((DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0GX51X) || \
     (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G352X))

/* 在 SRAM 中分配 Trim 缓存表空间（链接脚本 .TrimTable 段） */
__attribute__((section(".TrimTable")))
DL_FACTORYREGION_TrimValues DL_FACTORYREGION_TrimTable;
/* 指向 SRAM Trim 缓存表的全局指针 */
DL_FACTORYREGION_TrimValues *FACTORYVALUE = &DL_FACTORYREGION_TrimTable;

/**
 * @brief 将工厂 TRIM 区字段复制到 SRAM 缓存表。
 *
 * @param[in] 无。
 *
 * @return 无返回值。
 */
void DL_FactoryRegion_initTrimTable(void)
{
    /* 按字段逐项拷贝，确保后续读取不再依赖 TRIM 区实时访问 */
    DL_FACTORYREGION_TrimTable.TRACEID = FACTORYREGION->TRACEID;
    DL_FACTORYREGION_TrimTable.DEVICEID = FACTORYREGION->DEVICEID;
    DL_FACTORYREGION_TrimTable.USERID = FACTORYREGION->USERID;
    DL_FACTORYREGION_TrimTable.BSLPIN_UART = FACTORYREGION->BSLPIN_UART;
    DL_FACTORYREGION_TrimTable.BSLPIN_I2C = FACTORYREGION->BSLPIN_I2C;
    DL_FACTORYREGION_TrimTable.BSLPIN_INVOKE = FACTORYREGION->BSLPIN_INVOKE;
    DL_FACTORYREGION_TrimTable.SRAMFLASH = FACTORYREGION->SRAMFLASH;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP0_4_8MHZ =
        FACTORYREGION->PLLSTARTUP0_4_8MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP1_4_8MHZ =
        FACTORYREGION->PLLSTARTUP1_4_8MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP0_8_16MHZ =
        FACTORYREGION->PLLSTARTUP0_8_16MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP1_8_16MHZ =
        FACTORYREGION->PLLSTARTUP1_8_16MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP0_16_32MHZ =
        FACTORYREGION->PLLSTARTUP0_16_32MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP1_16_32MHZ =
        FACTORYREGION->PLLSTARTUP1_16_32MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP0_32_48MHZ =
        FACTORYREGION->PLLSTARTUP0_32_48MHZ;
    DL_FACTORYREGION_TrimTable.PLLSTARTUP1_32_48MHZ =
        FACTORYREGION->PLLSTARTUP1_32_48MHZ;
    DL_FACTORYREGION_TrimTable.TEMP_SENSE0 = FACTORYREGION->TEMP_SENSE0;
    /* 置位完成标志：上层据此判断是否改读 SRAM Trim 表 */
    DL_FACTORYREGION_TrimTable.DATA_SET = DL_FACTORYREGION_TRIM_FLAG_IS_SET;
    return;
}
#endif
