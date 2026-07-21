/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
 *  @file       I2CTargetMSPM0.h
 *
 *  @brief      I2CTarget driver implementation for a MSPM0 device I2C
 *              target.
 *              MSPM0 器件 I2C Target 的驱动实现
 *
 *  @defgroup   I2CTargetMSPM0 I2C driver APIs
 *
 *  @note       All calls should go through the generic I2CTarget API.
 *
 *  # Driver Include #
 *  The I2CTarget header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/I2CTarget.h>
 *  #include <ti/drivers/i2c/I2CTargetMSPM0.h>
 *  @endcode
 *
 *  Refer to @ref I2CTarget.h for a complete description of APIs.
 *
 *  # Overview
 *  The generic I2CTarget API is normally used in application code,
 *  for example, @ref I2CTarget_open(). Do not use APIs in this header file
 *  directly. The board file will define the device specific config, and casting
 *  in the general API will ensure that the correct device specific functions
 *  are called.
 *
 *  ## General Behavior
 *  Before using the I2C in MSPM0:
 *   - The I2CTarget driver is initialized by calling I2CTarget_init().
 *   - The I2C HW is configured and system dependencies are declared (e.g. IOs,
 *     power, etc.) by calling I2CTarget_open().
 *   - The I2CTarget driver starts acting on I2C bus activities after calling
 *     @ref I2CTarget_start().
 *
 *  ## Power Management
 *  The Power drivers are yet to implement but the behavior should follow like the information given here.
 *  The I2CTargetMSPM0 driver sets a power constraint when @ref I2CTarget_start()
 *  is called to keep the device out of standby; so when all tasks are blocked, the device will
 *  enter idle mode instead of standby. When the application calls @ref I2CTarget_stop(), the
 *  the power constraint to prohibit standby is released.
 *  The following statements are valid:
 *    - After I2CTarget_open() call: I2C peripheral is enabled in target mode, but the device can
 *      enter standby. The device will need to be woken up (for example on GPIO or timer events)
 *      before I2C bus activity can be processed.
 *    - After I2CTarget_start() call: I2C events will trigger the user callback function. The device
 *      might enter idle, but not standby.
 *    - After I2C_stop() call: Same as after I2CTarget_open() call.
 *    - After I2C_close() call: I2C peripheral is disabled.
 *
 * ## Supported Functions
 *  @note All calls should go through the generic API.
 *
 *  @anchor ti_drivers_I2CTARGETMSPM0_USE_CASES
 *  # Use Cases
 *  See generic API for examples.
 *
 ******************************************************************************
 */
/** @addtogroup I2CTargetMSPM0
 *  @ingroup I2CTarget
* @{
*/
/* clang-format off */

#ifndef ti_drivers_I2CTargetMSPM0__include
#define ti_drivers_I2CTargetMSPM0__include

#include <stdint.h>
#include <stdbool.h>

#include <ti/driverlib/dl_i2c.h>
#include <ti/drivers/I2CTarget.h>

#ifdef POWER_MANAGEMENT_MSPM0
#include <ti/drivers/Power.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  I2CTargetMSPM0 Hardware attributes
 *          I2CTargetMSPM0 硬件属性结构体，定义 I2C 外设的基地址、中断、引脚及配置参数
 *
 *  The baseAddr and intNum fields define the base address and the interrupt
 *  number of the I2C peripheral.  These values are passed to driverlib APIs
 *  and therefore must be populated by driverlib macro definitions.  These
 *  macros are found in the header files:
 *      - ti\devices\msp\peripherals\hw_i2c.h
 *
 *  The powerMngrId is the Power driver resource ID for the I2C peripheral.
 *  These macros are defined in the device specific Power include file.
 *
 *  intPriority is the I2C peripheral's interrupt priority, This value is passed
 *  unmodified to Hwi_create().
 *
 *  sdaPinMux and sclPinMux are values drawn from the GPIO driver. They indicate
 *  the function for that specific IO pin that select I2C mode.
 *
 *  sdaPinMux and sclPinMux are values drawn from the GPIO driver. They indicate
 *  the function for that specific DIO pin that select I2C mode.
 *
 *  A sample structure is shown below:
 *  @code
 *  const I2CMSPM0_HWAttrs I2CHWAttrs[CONFIG_I2C_COUNT] = {
 *      {
 *          .i2c                            = I2C_INST,
 *          .intNum                         = I2C_INST_INT_IRQN,
 *          .intPriority                    = (~0),
 *          .sdaPin                         = GPIO_I2C_IOMUX_SDA,
 *          .sdaPinIndex                    = GPIO_I2C_SDA_PIN,
 *          .sdaPinMux                      = GPIO_I2C_IOMUX_SDA_FUNC,
 *          .sclPin                         = GPIO_I2C_IOMUX_SCL,
 *          .sclPinIndex                    = GPIO_I2C_SCL_PIN,
 *          .sclPinMux                      = GPIO_I2C_IOMUX_SCL_FUNC,
 *          .clockSource                    = DL_I2C_CLOCK_BUSCLK,
 *          .clockDivider                   = DL_I2C_CLOCK_DIVIDE_1,
 *          .txIntFifoThr                   = DL_I2C_TX_FIFO_LEVEL_BYTES_1,
 *          .rxIntFifoThr                   = DL_I2C_RX_FIFO_LEVEL_BYTES_1,
 *          .isClockStretchingEnabled       = true,
 *          .isAnalogGlitchFilterEnabled    = false,
 *          .i2cClk                         = I2C_CLOCK_MHZ,
 *      },
 *  };
 *  @endcode
 */
typedef struct
{
    I2CTARGET_BASE_HWATTRS /*!< I2C base hardware attributes I2C 基础硬件属性 */

    /* SDA pin PINCM index and mux SDA 引脚 PINCM 索引和复用配置 */
    uint8_t sdaPincm; /*!< SDA Pincm value SDA 引脚 PINCM 值 */
    uint8_t sdaPinIndex;  /*!< SDA pin index SDA 引脚索引 */
    uint32_t sdaPinMux;   /*!< SDA pin function SDA 引脚复用功能 */
    /* SCL pin PINCM index and mux SCL 引脚 PINCM 索引和复用配置 */
    uint8_t sclPincm;    /*!< SCL Pincm value SCL 引脚 PINCM 值 */
    uint8_t sclPinIndex; /*!< SCL pin index SCL 引脚索引 */
    uint32_t sclPinMux;  /*!< SCL pin function SCL 引脚复用功能 */
#ifdef POWER_MANAGEMENT_MSPM0
    /* I2C peripheral's Power driver ID I2C 外设电源驱动 ID */
    PowerMSPM0_Resource powerMngrId;
#endif
    DL_I2C_CLOCK clockSource; /*!< I2C clock source I2C 时钟源 */
    DL_I2C_CLOCK_DIVIDE clockDivider; /*!< I2C clock divider to get the required bps I2C 时钟分频器 */
    DL_I2C_RX_FIFO_LEVEL rxIntFifoThr; /*!< I2C receive FIFO interrupt threshold I2C 接收 FIFO 中断阈值 */
    DL_I2C_TX_FIFO_LEVEL txIntFifoThr; /*!< I2C transmit FIFO interrupt threshold I2C 发送 FIFO 中断阈值 */
    bool isClockStretchingEnabled; /*!< Clock stretching enable/disable flag 时钟延展使能标志 */
    bool isAnalogGlitchFilterEnabled; /*!< Analog Glitch Filter enable/disable flag 模拟毛刺滤波器使能标志 */
    uint16_t i2cClk;               /*!< The clock must be in MHz I2C 时钟频率，单位 MHz */
} I2CTargetMSPM0_HWAttrs;

/*!
 *  @brief      I2CTargetMSPM0 Object.
 *              I2CTargetMSPM0 运行时对象，应用不得直接访问其成员
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    I2CTARGET_BASE_OBJECT
#ifdef POWER_MANAGEMENT_MSPM0
    /* I2C power notification */
    void *i2cPostFxn;
    Power_NotifyObj i2cPostObj;
#endif
} I2CTargetMSPM0_Object;

/*!
*
* @brief Checks an I2C base address.
*        This function determines if a I2C port base address is valid.
*        检查 I2C 基地址是否有效
*
* @param i2c is the base address of the I2C port.
*         I2C 端口基地址
*
* @return Returns \c true if the base address is valid and \c false
*         otherwise
*         基地址有效返回 true，否则返回 false
*/
static bool I2CBaseValid(I2C_Regs *i2c)
{
    #if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G1X0X_G3X0X)
        return ((i2c == (I2C_Regs *) I2C0_BASE)||(i2c == (I2C_Regs *) I2C1_BASE));
    #elif ((DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0L122X_L222X) || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0GX51X)  || \
       (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G352X))
       return ((i2c == (I2C_Regs *) I2C0_BASE)||(i2c == (I2C_Regs *) I2C1_BASE)||(i2c == (I2C_Regs *) I2C2_BASE));
    #else
        return (i2c == (I2C_Regs *) I2C0_BASE);
    #endif
}

/*!
 *  @cond NODOC
 *  I2C Target MSPM0 interrupt flags used to trigger Hwi callback.
 *  I2C Target MSPM0 中断标志位，用于触发 Hwi 回调
 */
#define I2CTARGET_INT_FLAGS (   DL_I2C_INTERRUPT_TARGET_TXFIFO_EMPTY | \
                                DL_I2C_INTERRUPT_TARGET_RX_DONE | \
                                DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER | \
                                DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER | \
                                DL_I2C_INTERRUPT_TARGET_START | \
                                DL_I2C_INTERRUPT_TARGET_STOP)
/*! @endcond */

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_I2CTargetMSPM0__include */
/* clang-format on */
/** @}*/
