/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  @file       UARTMSPM0.h
 *
 *  @brief      UART driver interface for the MSPM0 devices. MSPM0系列MCU的UART驱动接口
 *
 *  @defgroup   UARTMSPM0 UART driver APIs
 *
 *  # Overview
 *  Refer to @ref ti_drivers_UART_Overview for a complete description of APIs and examples
 *  of use.
 *
 ******************************************************************************
 */
/** @addtogroup UARTMSPM0
 *  @ingroup UART
* @{
*/
/* clang-format off */
#ifndef ti_drivers_uart_UARTMSPM0__include
#define ti_drivers_uart_UARTMSPM0__include

#include <ti/devices/msp/msp.h>
#include <ti/drivers/uart/UARTSupportMSPM0.h>

/*!
 *  @brief UART support function pointers. UART支持函数指针表
 */
extern const UARTSupport_Fxns UARTMSPSupportFxns;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief UART receive errors. UART接收错误中断掩码（溢出+中断+校验+帧错误）
 */
#define UART_RX_ERRS                                                         \
    (DL_UART_INTERRUPT_OVERRUN_ERROR | DL_UART_INTERRUPT_BREAK_ERROR | \
        DL_UART_INTERRUPT_PARITY_ERROR |                                  \
        DL_UART_INTERRUPT_FRAMING_ERROR)

/*!
 *  @brief      Private Metadata structure for a UART instance supporting
 *              hardware parameters.
 *              UART硬件属性结构体，包含寄存器地址、引脚、时钟等硬件配置
 */
typedef struct
{
    UART_Regs *regs;                        /*!< Pointer to UART register UART寄存器基地址 */
    IRQn_Type irq;                          /*!< UART Irq 中断号 */
    uint32_t rxPin;                         /*!< Rx port pin 接收引脚 */
    uint32_t rxPinFunction;                 /*!< Rx pin function 接收引脚复用功能 */
    uint32_t txPin;                         /*!< Tx port pin 发送引脚 */
    uint32_t txPinFunction;                 /*!< Tx pin function 发送引脚复用功能 */
    DL_UART_MODE mode;                      /*!< UART mode UART工作模式 */
    DL_UART_DIRECTION direction;            /*!< UART direction 数据方向 */
    DL_UART_FLOW_CONTROL flowControl;       /*!< Select UART flow control 流控制选择 */
    DL_UART_CLOCK clockSource;              /*!< Clock source 时钟源 */
    DL_UART_CLOCK_DIVIDE_RATIO clockDivider;/*!< Clock div 时钟分频系数 */
    DL_UART_RX_FIFO_LEVEL rxIntFifoThr;     /*!< Select threshold for Rx FIFO RX FIFO中断阈值 */
    DL_UART_TX_FIFO_LEVEL txIntFifoThr;     /*!< Select threshold for Tx FIFO TX FIFO中断阈值 */
} UARTMSP_HWAttrs;
/*!
 *  @brief  This function returns a pointer to UART object.
 *          返回UART对象指针
 *
 *  @pre    Config Parameters.
 *          Calling context: function
 *
 *  @param  handle:  A UART handle return by UART_open(). UART句柄
 *
 *  @return Return a UART object pointer. UART对象指针
 *
 */
__STATIC_INLINE UART_Object* UART_Obj_Ptr(UART_Handle handle)
{
    return (UART_Object *) (handle->object);
}

/*!
 *  @brief  This function returns a pointer to UART hardware Attribute.
 *          返回UART硬件属性指针
 *
 *  @pre    Config Parameters.
 *          Calling context: function
 *
 *  @param  handle:  A UART handle return by UART_open(). UART句柄
 *
 *  @return Return a UART HWAttrs pointer. 硬件属性指针
 *
 */
__STATIC_INLINE UARTMSP_HWAttrs* UART_HWAttrs_Ptr(UART_Handle handle)
{
    return (UARTMSP_HWAttrs *) (handle->hwAttrs);
}
/* The functions given below are based on @ref UART_Data_Object structure.
* These functions return a pointer to particular element in the structure
* which is based on the sequence in the structure. The sequence of elements
* in @ref UART_Data_Object must NOT be modified else below functions will be broken.
*/
/*!
 *  @brief  This function returns a pointer to UART buffers object.
 *          返回UART缓冲区对象指针
 *
 *  @pre    Config Parameters.
 *          Calling context: function
 *
 *  @param  *object:  A UART object pointer. UART对象指针
 *
 *  @return Return a UART buffers object pointer. 缓冲区对象指针
 *
 */
__STATIC_INLINE UART_Buffers_Object *UART_buffersObject(UART_Object *object)
{
    return (UART_Buffers_Object *) (object + 1);
}

/*!
 *  @brief  This function returns a pointer to UART event callback object.
 *          返回UART事件回调对象指针
 *
 *  @pre    Config Parameters.
 *          Calling context: function
 *
 *  @param  *object:  A UART object pointer. UART对象指针
 *
 *  @return Return a UART event callback object pointer. 事件回调对象指针
 *
 */
__STATIC_INLINE UART_EventCallback_Object *UART_eventCallbackObject(UART_Object *object)
{
    return (UART_EventCallback_Object *) (UART_buffersObject(object) + 1);
}

/*!
 *  @brief  This function returns a pointer to UART callback object.
 *          返回UART回调对象指针
 *
 *  @pre    Config Parameters.
 *          Calling context: function
 *
 *  @param  *object:  A UART object pointer. UART对象指针
 *
 *  @return Return a UART callback object pointer. 回调对象指针
 *
 */
__STATIC_INLINE UART_Callback_Object *UART_callbackObject(UART_Object *object)
{
    return (UART_Callback_Object *) (UART_eventCallbackObject(object) + 1);
}

/*!
 *  @brief  This function handles the UART interrupts.
 *          UART中断处理函数，处理接收、发送、DMA完成及错误中断
 *
 *  @pre    Config Parameters.
 *
 *  @param  handle: A UART handle return by UART_open(). UART句柄
 *
 *  @return void
 *
 */
void UARTMSP_interruptHandler(UART_Handle handle);
#ifndef UART_NO_DMA
/*!
 *  @brief  This function configures and starts DMA transfer.
 *          配置并启动DMA发送传输
 *
 *  @pre    Config Parameters.
 *          Calling context: Task
 *
 *  @param  handle: A UART handle return by UART_open(). UART句柄
 *
 *  @return void
 *
 *  @sa     UART_open()
 */
void UARTMSP_dmaTx(UART_Handle handle);

/*!
 *  @brief  This function configures and starts DMA receive.
 *          配置并启动DMA接收
 *
 *  @pre    Config Parameters.
 *          Calling context: Task
 *
 *  @param  handle: A UART handle return by UART_open(). UART句柄
 *  @param  copyfifo: true: copy fifo. true: 先复制FIFO中已有数据
 *                    false: don't copy fifo. false: 不复制
 *  @return void
 *
 *  @sa     UART_open()
 */
void UARTMSP_dmaRx(UART_Handle handle, bool copyfifo);

/*!
 *  @brief  This function stops DMA rx and disables the channel.
 *          停止DMA接收并禁用通道
 *
 *  @pre    Config Parameters.
 *          Calling context: Task
 *
 *  @param  handle: A UART handle return by UART_open(). UART句柄
 *
 *  @return void
 *
 *  @sa     UART_open()
 */
void UARTMSP_dmaStopRx(UART_Handle handle);

/*!
 *  @brief  This function stops DMA tx and disables the channel.
 *          停止DMA发送并禁用通道
 *
 *  @pre    Config Parameters.
 *          Calling context: Task
 *
 *  @param  handle: A UART handle return by UART_open(). UART句柄
 *
 *  @return Returns the remaining bytes to transmit. 返回剩余待发送字节数
 *
 *  @sa     UART_open()
 */
uint32_t UARTMSP_dmaStopTx(UART_Handle handle);
#endif
/*!
 *  @brief  This function enables receive, receive timeout, and error interrupts.
 *          使能接收、接收超时和错误中断
 *
 *  @pre    Config Parameters.
 *          Calling context: Task
 *
 *  @param  handle: A UART handle return by UART_open(). UART句柄
 *
 *  @return void
 *
 *  @sa     UART_open()
 */
void UARTMSP_enableInts(UART_Handle handle);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ti_drivers_uart_UARTMSPM0__include */
/* clang-format on */
/** @}*/
