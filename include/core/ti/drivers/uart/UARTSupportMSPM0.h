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
 *  @file       UARTSupportMSPM0.h
 *
 *  @brief      UART driver support APIs for the MSPM0 devices. MSPM0 UART驱动支持层API，定义数据结构和底层接口
 *
 *  @defgroup   UARTSupportMSPM0 UART driver supporting APIs
 *
 *  # Overview
 *  Refer to @ref ti_drivers_UART_Overview for a complete description of APIs and examples
 *  of use.
 *
 ******************************************************************************
 */
/** @addtogroup UARTSupportMSPM0
 *  @ingroup UART
* @{
*/
/* clang-format off */
#ifndef ti_drivers_uart_UARTSupportMSPM0__include
#define ti_drivers_uart_UARTSupportMSPM0__include

#include <ti/drivers/dpl/ClockP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#include <ti/drivers/dma/DMAMSPM0.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/drivers/UART.h>

/*!
 *  @brief      Function pointers to implementation/hardware specific logic.
 *              硬件相关实现函数指针表，各平台需实现这些接口
 */
typedef struct {
   /*!
    *  @brief      Enable the hardware, typically called during UART_open().
    *              使能硬件，通常在UART_open()中调用
    *
    *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
    *  @param[in]  params    Reference to the UART_Params struct passed in by
    *                        the user. UART参数
    *  @return Returns true on success, else false. 成功返回true
    */
    bool (*enable)(UART_Handle handle, UART_Params *params);

   /*!
    *  @brief      Disable the hardware, typically called during UART_close().
    *              禁用硬件，通常在UART_close()中调用
    *
    *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
    */
    void (*disable)(UART_Handle handle);

   /*!
    *  @brief      Kick the transmitter in the hardware.
    *              触发硬件发送器启动传输
    *
    *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
    */
    void (*txChar)(UART_Handle handle);

   /*!
    *  @brief      Read method to use.
    *              底层读取方法
    *
    *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
    *  @param[out] buf       location to read data into. 接收缓冲区
    *  @param[in]  size      amount of data to read in bytes. 要读取的字节数
    *  @param[out] bytesRead If non-NULL, the location to store the number
    *                        of bytes actually read into the buffer. If NULL,
    *                        this parameter will be ignored. In callback mode,
    *                        NULL could be passed in for this parameter, since
    *                        the callback function will be passed the number of
    *                        bytes read. 实际读取字节数
    *  @return Returns a status indicating success or failure of the read.
    *          返回读操作状态码
    *
    *  @retval #UART_STATUS_SUCCESS    The call was successful.
    *  @retval #UART_STATUS_EINUSE     Another read from the UART is currently
    *                                   ongoing.
    *  @retval #UART_STATUS_ECANCELLED In #UART_Mode_BLOCKING, the read was
    *                                   canceled by a call to UART_readCancel()
    *                                   before any data could be received.
    */
    int_fast16_t (*read)(
        UART_Handle, void *buf, size_t size, size_t *bytesRead);

   /*!
    *  @brief      Write method to use.
    *              底层写入方法
    *
    *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
    *  @param[in]  buf       location to write data from. 待发送缓冲区
    *  @param[in]  size      amount of data to write in bytes. 要发送的字节数
    *  @param[out] bytesWritten If non-NULL, the location to store the number
    *                        of bytes actually written into the buffer. If
    *                        NULL, this parameter will be ignored. In callback
    *                        mode, NULL could be passed in for this parameter,
    *                        since the callback function will be passed the
    *                        number of bytes written. 实际发送字节数
    *  @return Returns a status indicating success or failure of the write.
    *          返回写操作状态码
    *
    *  @retval #UART_STATUS_SUCCESS The call was successful.
    *  @retval #UART_STATUS_EINUSE  Another write to the UART is currently
    *                                ongoing.
    */
    int_fast16_t (*write)(
        UART_Handle, const void *buf, size_t size, size_t *bytesWritten);
} UARTSupport_Fxns;

/*!
 *  @brief      Private Metadata structure for a UART instance.
 *              UART实例私有元数据结构体，存储驱动运行时状态
 */
typedef struct {
    const UARTSupport_Fxns *supportFxns; /*!< implementation specific 平台相关实现函数表 */
    void *userArg;                  /*!< User supplied argument for callback 回调用户参数 */
    uint8_t buffersSupported : 1;   /*!< RX/TX buffers supported 是否支持缓冲区 */
    uint8_t eventsSupported : 1;    /*!< True if events are supported 是否支持事件 */
    uint8_t callbacksSupported : 1; /*!< True if callbacks are supported 是否支持回调 */
    uint8_t dmaSupported : 1;       /*!< True if dma is supported 是否支持DMA */
    uint8_t returnMode : 1;         /*!< @ref UART_ReadReturnMode(partial/full) 读返回模式 */
    uint8_t inUse : 1;              /*!< True if driver already open 驱动已打开标志 */
    uint8_t readCancel : 1;         /*!< True if a read has been canceled 读取消标志 */
    uint8_t writeCancel : 1;        /*!< True if a write has been canceled 写取消标志 */
    uint8_t overrunActive : 1;      /*!< Is a RX overrun active RX溢出活跃标志 */
    uint8_t readTimedOut : 1;       /*!< Has read timed out 读超时标志 */
    uint8_t writeTimedOut : 1;      /*!< Has write timed out 写超时标志 */
    uint8_t readMode : 2;           /*!< Read @ref UART_Mode 读模式 */
    uint8_t writeMode : 2;          /*!< Write @ref UART_Mode 写模式 */
    uint8_t rxDmaChannel;           /*!< DMA rx channel number DMA接收通道号 */
    uint8_t txDmaChannel;           /*!< DMA tx channel number DMA发送通道号 */
    uint8_t readInUse : 1;          /*!< Flag to show ongoing read 读操作进行中标志 */
    uint8_t writeInUse : 1;         /*!< Flag to show ongoing write 写操作进行中标志 */
    uint32_t overrunCount;          /*!< Total counts of overruns 累计溢出次数 */
#ifndef UART_NO_DMA
    uint8_t noOfDMAChannels;        /*!< Total DMA channels used DMA通道数量 */
    DMAMSPM0_Handle DMA_Handle;     /*!< DMA handle DMA句柄 */
#endif
} UART_Object;


/*!
 *  @brief      Private Metadata structure for a UART instance supporting
 *              buffers.
 *              UART缓冲区对象，管理收发环形缓冲区和信号量
 */
typedef struct {
    uint8_t *rxBufPtr;          /*!< RX buffer memory address RX缓冲区内存地址 */
    uint8_t *txBufPtr;          /*!< TX buffer memory address TX缓冲区内存地址 */
    uint16_t rxBufSize;         /*!< RX buffer size in bytes, length of an array RX缓冲区大小 */
    uint16_t txBufSize;         /*!< TX buffer size in bytes TX缓冲区大小 */
    RingBuf_Object rxBuf;       /*!< Receive data buffer 接收环形缓冲区 */
    RingBuf_Object txBuf;       /*!< Transmit data buffer 发送环形缓冲区 */
    SemaphoreP_Handle rxSem;    /*!< Semaphore for receive 接收信号量 */
    SemaphoreP_Handle txSem;    /*!< Semaphore for transmit 发送信号量 */
} UART_Buffers_Object;

/*!
 *  @brief      Private Metadata structure for a UART instance supporting
 *              events.
 *              UART事件回调对象
 */
typedef struct {
    UART_EventCallback eventCallback; /*!< Pointer to event callback function 事件回调函数指针 */
    uint32_t eventMask; /*!< Mask of events that application is interacted in 应用关注的事件掩码 */
} UART_EventCallback_Object;

/*!
 *  @brief      Private Metadata structure for a UART instance supporting
 *              callbacks.
 *              UART回调对象，管理回调模式下的读写状态和缓冲区
 */
typedef struct {
    UART_Callback readCallback;  /*!< Pointer to read callback 读回调函数指针 */
    UART_Callback writeCallback; /*!< Pointer to write callback 写回调函数指针 */
    uint8_t *readBuf;            /*!< Buffer data pointer 接收数据缓冲区指针 */
    const uint8_t *writeBuf;     /*!< Buffer data pointer 发送数据缓冲区指针 */
    uint16_t readSize;           /*!< Number of bytes to read 请求读取字节数 */
    uint16_t writeSize;          /*!< Number of bytes to write 请求发送字节数 */
    uint16_t writeCount;         /*!< Number of bytes left to write 剩余待写字节数 */
    uint16_t readCount;          /*!< Number of bytes left to read 剩余待读字节数 */
    uint16_t bytesRead;          /*!< Number of bytes read 已读取字节数 */
    uint16_t bytesWritten;       /*!< Number of bytes written 已写字节数 */
    int32_t rxStatus;            /*!< Rx Status 接收状态 */
    int32_t txStatus;            /*!< Tx Status 发送状态 */
    size_t rxSize;               /*!< Number of bytes to read in DMA xfer DMA接收字节数 */
    size_t txSize;               /*!< Number of bytes to write with DMA DMA发送字节数 */
    void *userArg;               /*!< User supplied arg for callbacks 回调用户参数 */
} UART_Callback_Object;

/*!
 *  @brief      Private Metadata structure for a UART instance supporting
 *              data structures needed for UART TX/RX execution.
 *              UART数据对象，组合所有子对象，字段顺序不可修改
 */
/* 警告！此结构体中字段顺序不可修改，内联访问函数依赖此布局 */
typedef struct
{
    UART_Object object;                /*!< Struct of UART object parameters UART对象参数 */
    UART_Buffers_Object buffersObject; /*!< Struct of UART buffer object to support buffers 缓冲区对象 */
    UART_EventCallback_Object eventCallbackObject; /*!< Struct of UART event object 事件回调对象 */
    UART_Callback_Object callbackObject; /*!< Struct of UART callback object 回调对象 */

}UART_Data_Object;

/*!
 *  @brief  UART Global configuration.
 *
 *  The @ref UART_Config structure contains a set of pointers used to characterize
 *  the UART driver implementation.
 *  UART全局配置结构体，包含对象和硬件属性指针
 */
typedef struct UART_Config_
{
    /*! Pointer to a driver specific data object. 驱动数据对象指针 */
    void *object;

    /*! Pointer to a driver specific hardware attributes structure. 硬件属性结构体指针 */
    void const *hwAttrs;
} UART_Config;

/*!
 *  @brief  Array of @ref UART_Config structure.
 *          UART配置数组
 */
extern  const UART_Config UART_config[];
/*!
 *  @brief  Count of UART instances.
 *          UART实例数量
 */
extern const uint_least8_t UART_count;



/*!
 *  @brief      UART read that only supports buffered mode.
 *              仅支持缓冲模式的UART读取
 *
 *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
 *  @param[out] buf       location to read data into. 接收缓冲区
 *  @param[in]  size      amount of data to read in bytes. 要读取的字节数
 *  @param[out] bytesRead If non-NULL, the location to store the number
 *                        of bytes actually read into the buffer. If NULL,
 *                        this parameter will be ignored. 实际读取字节数
 *  @return Returns a status indicating success or failure of the read.
 *          返回读操作状态码
 *
 *  @retval #UART_STATUS_SUCCESS    The call was successful.
 *  @retval #UART_STATUS_EINUSE     Another read from the UART is currently
 *                                   ongoing.
 *  @retval #UART_STATUS_ECANCELLED In #UART_Mode_BLOCKING, the read was
 *                                   canceled by a call to UART_readCancel()
 *                                   before any data could be received.
 */
int_fast16_t UART_readBuffered(UART_Handle handle, void *buf, size_t size, size_t *bytesRead);

/*!
 *  @brief      UART write that only supports buffered mode.
 *              仅支持缓冲模式的UART写入
 *
 *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
 *  @param[in]  buf       location to write data from. 待发送缓冲区
 *  @param[in]  size      amount of data to write in bytes. 要发送的字节数
 *  @param[out] bytesWritten If non-NULL, the location to store the number
 *                        of bytes actually written into the buffer. If
 *                        NULL, this parameter will be ignored. 实际发送字节数
 *  @return Returns a status indicating success or failure of the write.
 *          返回写操作状态码
 *
 *  @retval #UART_STATUS_SUCCESS The call was successful.
 *  @retval #UART_STATUS_EINUSE  Another write to the UART is currently
 *                                ongoing.
 */
int_fast16_t UART_writeBuffered(UART_Handle handle, const void *buf, size_t size, size_t *bytesWritten);

/*!
 *  @brief      UART read that only supports callback mode.
 *              仅支持回调模式的UART读取
 *
 *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
 *  @param[out] buf       location to read data into. 接收缓冲区
 *  @param[in]  size      amount of data to read in bytes. 要读取的字节数
 *  @param[out] bytesRead If non-NULL, the location to store the number
 *                        of bytes actually read into the buffer. If NULL,
 *                        this parameter will be ignored. In callback mode,
 *                        NULL could be passed in for this parameter, since
 *                        the callback function will be passed the number of
 *                        bytes read. 实际读取字节数
 *  @return Returns a status indicating success or failure of the read.
 *          返回读操作状态码
 *
 *  @retval #UART_STATUS_SUCCESS    The call was successful.
 *  @retval #UART_STATUS_EINUSE     Another read from the UART is currently
 *                                   ongoing.
 *  @retval #UART_STATUS_ECANCELLED In #UART_Mode_BLOCKING, the read was
 *                                   canceled by a call to UART_readCancel()
 *                                   before any data could be received.
 */
int_fast16_t UART_readCallback(UART_Handle handle, void *buf, size_t size, size_t *bytesRead);

/*!
 *  @brief      UART write that only supports callback mode.
 *              仅支持回调模式的UART写入
 *
 *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
 *  @param[in]  buf       location to write data from. 待发送缓冲区
 *  @param[in]  size      amount of data to write in bytes. 要发送的字节数
 *  @param[out] bytesWritten If non-NULL, the location to store the number
 *                        of bytes actually written into the buffer. If
 *                        NULL, this parameter will be ignored. In callback
 *                        mode, NULL could be passed in for this parameter,
 *                        since the callback function will be passed the
 *                        number of bytes written. 实际发送字节数
 *  @return Returns a status indicating success or failure of the write.
 *          返回写操作状态码
 *
 *  @retval #UART_STATUS_SUCCESS The call was successful.
 *  @retval #UART_STATUS_EINUSE  Another write to the UART is currently
 *                                ongoing.
 */
int_fast16_t UART_writeCallback(UART_Handle handle, const void *buf, size_t size, size_t *bytesWritten);

/*!
 *  @brief      UART read that only supports buffered and callback mode.
 *              支持缓冲和回调模式的全功能UART读取
 *
 *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
 *  @param[out] buf       location to read data into. 接收缓冲区
 *  @param[in]  size      amount of data to read in bytes. 要读取的字节数
 *  @param[out] bytesRead If non-NULL, the location to store the number
 *                        of bytes actually read into the buffer. If NULL,
 *                        this parameter will be ignored. In callback mode,
 *                        NULL could be passed in for this parameter, since
 *                        the callback function will be passed the number of
 *                        bytes read. 实际读取字节数
 *  @return Returns a status indicating success or failure of the read.
 *          返回读操作状态码
 *
 *  @retval #UART_STATUS_SUCCESS    The call was successful.
 *  @retval #UART_STATUS_EINUSE     Another read from the UART is currently
 *                                   ongoing.
 *  @retval #UART_STATUS_ECANCELLED In #UART_Mode_BLOCKING, the read was
 *                                   canceled by a call to UART_readCancel()
 *                                   before any data could be received.
 */
int_fast16_t UART_readFullFeatured(UART_Handle handle, void *buf, size_t size, size_t *bytesRead);

/*!
 *  @brief      UART write that only supports buffered and callback mode.
 *              支持缓冲和回调模式的全功能UART写入
 *
 *  @param[in]  handle    UART_Handle instance from UART_open(). UART句柄
 *  @param[in]  buf       location to write data from. 待发送缓冲区
 *  @param[in]  size      amount of data to write in bytes. 要发送的字节数
 *  @param[out] bytesWritten If non-NULL, the location to store the number
 *                        of bytes actually written into the buffer. If
 *                        NULL, this parameter will be ignored. In callback
 *                        mode, NULL could be passed in for this parameter,
 *                        since the callback function will be passed the
 *                        number of bytes written. 实际发送字节数
 *  @return Returns a status indicating success or failure of the write.
 *          返回写操作状态码
 *
 *  @retval #UART_STATUS_SUCCESS The call was successful.
 *  @retval #UART_STATUS_EINUSE  Another write to the UART is currently
 *                                ongoing.
 */
int_fast16_t UART_writeFullFeatured(UART_Handle handle, const void *buf, size_t size, size_t *bytesWritten);


#endif /* ti_drivers_uart_UARTSupportMSPM0__include */
/* clang-format on */
/** @}*/
