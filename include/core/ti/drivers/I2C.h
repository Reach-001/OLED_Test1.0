/*
 * Copyright (c) 2015-2023, Texas Instruments Incorporated
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
 *  @file       I2C.h
 *
 *  @brief      I2C Driver / I2C 驱动程序
 *
 *  本文件定义了 TI MSPM0 平台 I2C 驱动的通用接口，包括传输模式、
 *  事务结构体、参数配置及 API 声明。应用层通过此头文件访问 I2C 功能。
 *
 *  @defgroup   I2C Inter-Integrated Circuit (I2C)
 *
 *  The I2C header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/I2C.h>
 *  @endcode
 *
 *  @anchor ti_drivers_I2C_Overview
 *  # Overview
 *
 *  The I2C driver is designed to operate as an I2C controller and will not
 *  function as an I2C target. Multi-controller arbitration is not supported;
 *  therefore, this driver assumes it is the only I2C controller on the bus.
 *  This I2C driver's API set provides the ability to transmit and receive
 *  data over an I2C bus between the I2C controller and I2C target(s). The
 *  application is responsible for manipulating and interpreting the data.
 *
 *
 *  <hr>
 *  @anchor ti_drivers_I2C_Usage
 *  # Usage
 *
 *  This section provides a basic @ref ti_drivers_I2C_Synopsis
 *  "usage summary" and a set of @ref ti_drivers_I2C_Examples "examples"
 *  in the form of commented code fragments. Detailed descriptions of the
 *  I2C APIs and their effect are provided in subsequent sections.
 *
 *  @anchor ti_drivers_I2C_Synopsis
 *  ## Synopsis #
 *  @anchor ti_drivers_I2C_Synopsis_Code
 *  @code
 *  // Import I2C Driver definitions.
 *  #include <ti/drivers/I2C.h>
 *
 *  // Define name for an index of an I2C bus.
 *  #define SENSORS 0
 *
 *  // Define the target address of device on the SENSORS bus.
 *  #define OPT_ADDR 0x48
 *
 *  // One-time init of I2C driver.
 *  I2C_init();
 *
 *  // initialize optional I2C bus parameters.
 *  I2C_Params params;
 *  I2C_Params_init(&params);
 *  params.bitRate = I2C_100kHz;
 *
 *  // Open I2C bus for usage.
 *  I2C_Handle i2cHandle = I2C_open(SENSORS, &params);
 *
 *  // Initialize target address of transaction.
 *  I2C_Transaction transaction = {0};
 *  transaction.targetAddress = OPT_ADDR;
 *
 *  // Read from I2C target device.
 *  transaction.readBuf = data;
 *  transaction.readCount = sizeof(data);
 *  transaction.writeCount = 0;
 *  I2C_transfer(i2cHandle, &transaction);
 *
 *  // Write to I2C target device.
 *  transaction.writeBuf = command;
 *  transaction.writeCount = sizeof(command);
 *  transaction.readCount = 0;
 *  I2C_transferTimeout(i2cHandle, &transaction, 5000);
 *
 *  // Close I2C.
 *  I2C_close(i2cHandle);
 *  @endcode
 *
 *  @anchor ti_drivers_I2C_Examples
 *  ## Examples
 *
 *  @li @ref ti_drivers_I2C_Example_open "Getting an I2C bus handle"
 *  @li @ref ti_drivers_I2C_Example_write3bytes "Sending 3 bytes"
 *  @li @ref ti_drivers_I2C_Example_read5bytes "Reading 5 bytes"
 *  @li @ref ti_drivers_I2C_Example_writeread "Writing then reading in a single transaction"
 *  @li @ref ti_drivers_I2C_Example_callback "Using Callback mode"
 *
 *  @anchor ti_drivers_I2C_Example_open
 *  ## Opening the I2C Driver
 *
 *  After calling I2C_init(), the application can open an I2C instance by
 *  calling I2C_open().The following code example opens an I2C instance with
 *  default parameters by passing @p NULL for the #I2C_Params argument.
 *
 *  @code
 *  I2C_Handle i2cHandle;
 *
 *  i2cHandle = I2C_open(0, NULL);
 *
 *  if (i2cHandle == NULL) {
 *      // Error opening I2C
 *      while (1) {}
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_I2C_Example_write3bytes
 *  ## Sending three bytes of data.
 *
 *  @code
 *  I2C_Transaction i2cTransaction = {0};
 *  uint8_t writeBuffer[3];
 *
 *  writeBuffer[0] = 0xAB;
 *  writeBuffer[1] = 0xCD;
 *  writeBuffer[2] = 0xEF;
 *
 *  i2cTransaction.targetAddress = 0x50;
 *  i2cTransaction.writeBuf = writeBuffer;
 *  i2cTransaction.writeCount = 3;
 *  i2cTransaction.readBuf = NULL;
 *  i2cTransaction.readCount = 0;
 *
 *  status = I2C_transfer(i2cHandle, &i2cTransaction);
 *
 *  if (status == false) {
 *      // Unsuccessful I2C transfer
 *      if (i2cTransaction.status == I2C_STATUS_ADDR_NACK) {
 *          // I2C target address not acknowledged
 *      }
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_I2C_Example_read5bytes
 *  ## Reading five bytes of data.
 *
 *  @code
 *  I2C_Transaction i2cTransaction = {0};
 *  uint8_t readBuffer[5];
 *
 *  i2cTransaction.targetAddress = 0x50;
 *  i2cTransaction.writeBuf = NULL;
 *  i2cTransaction.writeCount = 0;
 *  i2cTransaction.readBuf = readBuffer;
 *  i2cTransaction.readCount = 5;
 *
 *  status = I2C_transfer(i2cHandle, &i2cTransaction);
 *
 *  if (status == false) {
 *      if (i2cTransaction.status == I2C_STATUS_ADDR_NACK) {
 *          // I2C target address not acknowledged
 *      }
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_I2C_Example_writeread
 *  ## Writing two bytes and reading four bytes in a single transaction.
 *
 *  @code
 *  I2C_Transaction i2cTransaction = {0};
 *  uint8_t readBuffer[4];
 *  uint8_t writeBuffer[2];
 *
 *  writeBuffer[0] = 0xAB;
 *  writeBuffer[1] = 0xCD;
 *
 *  i2cTransaction.targetAddress = 0x50;
 *  i2cTransaction.writeBuf = writeBuffer;
 *  i2cTransaction.writeCount = 2;
 *  i2cTransaction.readBuf = readBuffer;
 *  i2cTransaction.readCount = 4;
 *
 *  status = I2C_transfer(i2cHandle, &i2cTransaction);
 *
 *  if (status == false) {
 *       if (i2cTransaction->status == I2C_STATUS_ADDR_NACK) {
 *           // target address not acknowledged
 *       }
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_I2C_Example_callback
 *  ## Using callback mode
 *  This final example shows usage of #I2C_MODE_CALLBACK, with queuing
 *  of multiple transactions. Because multiple transactions are simultaneously
 *  queued, separate #I2C_Transaction structures must be used. Each
 *  #I2C_Transaction will contain a custom application argument of a
 *  semaphore handle. The #I2C_Transaction.arg will point to the semaphore
 *  handle. When the callback function is called, the #I2C_Transaction.arg is
 *  checked for @p NULL. If this value is not @p NULL, then it can be assumed
 *  the @p arg is pointing to a valid semaphore handle. The semaphore handle
 *  is then used to call @p sem_post(). Hypothetically, this can be used to
 *  signal transaction completion to the task(s) that queued the
 *  transaction(s).
 *
 *  @code
 *  void callbackFxn(I2C_Handle handle, I2C_Transaction *msg, bool status)
 *  {
 *      // if transaction failed.
 *      if (status == false) {
 *          if (msg->status == I2C_STATUS_ADDR_NACK) {
 *              // target address not acknowledged
 *          }
 *          else if (msg->status == I2C_STATUS_CANCEL) {
 *              // transaction canceled by I2C_cancel()
 *          }
 *      }
 *
 *      // Check for a custom argument.
 *      if (msg->arg != NULL) {
 *
 *          // In this example, the custom argument is a semaphore handle.
 *          // Perform a semaphore post.
 *          sem_post((sem_t *) (msg->arg));
 *      }
 *  }
 *  @endcode
 *
 *  Snippets of the thread code that initiates the transactions are shown below.
 *  Note the use of multiple #I2C_Transaction structures. The handle of the
 *  semaphore to be posted is specified via @p i2cTransaction2.arg.
 *  I2C_transfer() is called three times to initiate each transaction.
 *  Since callback mode is used, these functions return immediately. After
 *  the transactions have been queued, other work can be done. Eventually,
 *  @p sem_wait() is called causing the thread to block until the transaction
 *  completes. When the transaction completes, the application's callback
 *  function, @p callbackFxn will be called. Once #I2C_CallbackFxn posts the
 *  semaphore, the thread will be unblocked and can resume execution.
 *
 *  @code
 *  void thread(arg0, arg1)
 *  {
 *
 *      I2C_Transaction i2cTransaction0 = {0};
 *      I2C_Transaction i2cTransaction1 = {0};
 *      I2C_Transaction i2cTransaction2 = {0};
 *
 *      // ...
 *
 *      i2cTransaction0.arg = NULL;
 *      i2cTransaction1.arg = NULL;
 *      i2cTransaction2.arg = semaphoreHandle;
 *
 *      // ...
 *
 *      I2C_transfer(i2c, &i2cTransaction0);
 *      I2C_transfer(i2c, &i2cTransaction1);
 *      I2C_transfer(i2c, &i2cTransaction2);
 *
 *      // ...
 *
 *      sem_wait(semaphoreHandle);
 *  }
 *  @endcode
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup I2C
* @{
*/
#ifndef ti_drivers_I2C__include
#define ti_drivers_I2C__include

/*! @cond */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>

/*! @endcond */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup I2C_STATUS I2C status codes
 *  These macros are reservations for I2C.h.
 *  I2C 状态码定义，用于表示传输结果或错误类型。
 *  @{
 */

/*! @cond */
/*!
 * @private
 * Common I2C_control status code reservation offset.
 * I2C driver implementations should offset status codes with
 * #I2C_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define I2CXYZ_STATUS_ERROR0     I2C_STATUS_RESERVED - 0
 * #define I2CXYZ_STATUS_ERROR1     I2C_STATUS_RESERVED - 1
 * #define I2CXYZ_STATUS_ERROR2     I2C_STATUS_RESERVED - 2
 * @endcode
 */
#define I2C_STATUS_RESERVED (-32)
/*! @endcond */

/*!
 * @brief  I2C transaction is queued but has not started.
 *         I2C 事务已排队但尚未开始。
 */
#define I2C_STATUS_QUEUED (1)

/*!
 * @brief  Successful status code returned by I2C API.
 *         I2C API 返回的成功状态码。
 */
#define I2C_STATUS_SUCCESS (0)

/*!
 * @brief  Generic error status code returned by I2C API.
 *         I2C API 返回的通用错误状态码。
 */
#define I2C_STATUS_ERROR (-1)

/*!
 * @brief  An error status code returned by I2C_control() for undefined
 * command codes.
 *         I2C_control() 遇到未定义命令码时返回的错误状态码。
 */
#define I2C_STATUS_UNDEFINEDCMD (-2)

/*!
 * @brief  I2C operation timed-out.
 *         I2C 操作超时。
 */
#define I2C_STATUS_TIMEOUT (-3)

/*!
 * @brief  I2C serial clock line timeout.
 *         I2C 串行时钟线超时。
 */
#define I2C_STATUS_CLOCK_TIMEOUT (-4)

/*!
 * @brief  I2C target address not acknowledged.
 *         I2C 目标地址未应答（NACK）。
 */
#define I2C_STATUS_ADDR_NACK (-5)

/*!
 * @brief  I2C data byte not acknowledged.
 *         I2C 数据字节未应答（NACK）。
 */
#define I2C_STATUS_DATA_NACK (-6)

/*!
 * @brief  I2C multi-controller arbitration lost.
 *         I2C 多主机仲裁失败。
 */
#define I2C_STATUS_ARB_LOST (-7)

/*!
 * @brief  I2C transaction is in progress or returned without completing.
 *         I2C 事务正在进行中或未完成即返回。
 */
#define I2C_STATUS_INCOMPLETE (-8)

/*!
 * @brief  I2C bus already in use by another controller. The I2C transaction.
 * was therefore unable to start.
 *         I2C 总线已被其他主机占用，事务无法启动。
 */
#define I2C_STATUS_BUS_BUSY (-9)

/*!
 * @brief  I2C transaction canceled by I2C_cancel().
 *         I2C 事务已被 I2C_cancel() 取消。
 */
#define I2C_STATUS_CANCEL (-10)

/*!
 * @brief  I2C transaction is invalid.
 *         I2C 事务无效。
 *
 * This may occur if:
 *   1. The #I2C_Transaction.readCount and #I2C_Transaction.writeCount are
 *      both set to 0.
 *   2. A call to I2C_transfer() is made from a #I2C_CallbackFxn while queued
 *      transactions are being canceled. See also: I2C_cancel().
 *
 * 可能的原因：
 *   1. readCount 和 writeCount 同时为 0。
 *   2. 在回调函数中调用 I2C_transfer() 时队列事务正在被取消。
 */
#define I2C_STATUS_INVALID_TRANS (-11)
/** @} */

/*!
 *  @brief  Wait forever define used to specify timeouts.
 *          永久等待，用于指定无限超时。
 */
#define I2C_WAIT_FOREVER (~(0U))

/*!
 *  @brief  A handle that is returned from an I2C_open() call.
 *          I2C_open() 返回的句柄，用于后续 I2C 操作。
 */
typedef struct I2C_Config_ *I2C_Handle;

/*!
 *  @brief  Defines a transaction to be used with I2C_transfer() or
 *          I2C_transferTimeout().
 *          定义 I2C 传输事务结构体，用于 I2C_transfer() 或 I2C_transferTimeout()。
 *
 *  After a call to I2C_transfer(), the #I2C_Transaction.status reflects
 *  the current transfer status.
 *  调用 I2C_transfer() 后，status 字段反映当前传输状态。
 *
 *  @sa I2C_transfer(), I2C_transferTimeout()
 */
typedef struct {
    /*!
     *  Pointer to a buffer of at least #I2C_Transaction.writeCount bytes.
     *  If #I2C_Transaction.writeCount is 0, this pointer is not used.
     *  指向写缓冲区的指针，大小至少为 writeCount 字节。
     *  若 writeCount 为 0 则不使用。
     */
    void *writeBuf;

    /*!
     *  Number of bytes to write to the I2C target device. A value of 0
     *  indicates no data will be written to the target device and only a read
     *  will occur. If this value
     *  is not 0, the driver will always perform the write transfer first.
     *  The data written to the I2C bus is preceded by the
     *  #I2C_Transaction.targetAddress with the write bit set. If
     *  @p writeCount bytes are successfully sent and
     *  acknowledged, the transfer will complete or perform a read--depending
     *  on #I2C_Transaction.readCount.
     *
     *  写入 I2C 目标设备的字节数。为 0 时仅执行读操作。
     *  非 0 时驱动先执行写操作，写完成后根据 readCount 决定是否继续读。
     *
     *  @note Both #I2C_Transaction.writeCount and #I2C_Transaction.readCount
     *  can not be 0.
     *  注意：writeCount 和 readCount 不能同时为 0。
     */
    size_t writeCount;

    /*!
     *  Pointer to a buffer of at least #I2C_Transaction.readCount bytes.
     *  If #I2C_Transaction.readCount is 0, this pointer is not used.
     *  指向读缓冲区的指针，大小至少为 readCount 字节。
     *  若 readCount 为 0 则不使用。
     */
    void *readBuf;

    /*!
     *  Number of bytes to read from the I2C target device. A value of 0
     *  indicates no data will be read and only a write will occur. If
     *  #I2C_Transaction.writeCount is not 0, this driver will perform the
     *  write first, followed by the read. The data read from the bus is
     *  preceded by the #I2C_Transaction.targetAddress with the read bit set.
     *  After @p readCount bytes are successfully read, the transfer will
     *  complete.
     *
     *  从 I2C 目标设备读取的字节数。为 0 时仅执行写操作。
     *  若 writeCount 非 0，则先写后读。读取完成后传输结束。
     *
     *  @note Both #I2C_Transaction.writeCount and #I2C_Transaction.readCount
     *  can not be 0.
     *  注意：writeCount 和 readCount 不能同时为 0。
     */
    size_t readCount;

    /*!
     * Pointer to a custom argument to be passed to the #I2C_CallbackFxn
     * function via the #I2C_Transaction structure.
     * 自定义参数指针，通过 I2C_Transaction 传递给回调函数。
     *
     * @note The #I2C_CallbackFxn function is only called when operating in
     * #I2C_MODE_CALLBACK.
     *
     * @sa  #I2C_MODE_CALLBACK
     * @sa  #I2C_CallbackFxn
     */
    void *arg;

    /*!
     *  I2C status of the current transaction. The status may be used to
     *  determine why a transaction failed. Potential codes are:
     *  当前事务的 I2C 状态，可用于判断传输失败原因。可能的状态码：
     *      @li #I2C_STATUS_SUCCESS
     *      @li #I2C_STATUS_ERROR
     *      @li #I2C_STATUS_TIMEOUT
     *      @li #I2C_STATUS_CLOCK_TIMEOUT
     *      @li #I2C_STATUS_ADDR_NACK
     *      @li #I2C_STATUS_DATA_NACK
     *      @li #I2C_STATUS_ARB_LOST
     *      @li #I2C_STATUS_INCOMPLETE
     *      @li #I2C_STATUS_BUS_BUSY
     *      @li #I2C_STATUS_CANCEL
     *      @li #I2C_STATUS_INVALID_TRANS
     *
     *  This status may also be used to determine if a transaction is queued
     *  (#I2C_STATUS_QUEUED) or in progress (#I2C_STATUS_INCOMPLETE).
     */
    volatile int_fast16_t status;

    /*!
     *  I2C target address used for the transaction. The target address is
     *  the first byte transmitted during an I2C transfer. The read/write bit
     *  is automatically set based upon the #I2C_Transaction.writeCount and
     *  #I2C_Transaction.readCount.
     *  I2C 事务使用的目标地址。传输时第一个发送的字节，
     *  读/写位由驱动根据 writeCount 和 readCount 自动设置。
     */
    uint_least8_t targetAddress;

    /*!
     *  @private This is reserved for use by the driver and must never be
     *  modified by the application.
     */
    void *nextPtr;
} I2C_Transaction;

/*!
 *  @brief    Return behavior of I2C_Transfer() specified in the #I2C_Params.
 *            I2C_transfer() 的返回行为，在 I2C_Params 中指定。
 *
 *  This enumeration defines the return behaviors for a call to I2C_transfer().
 *  此枚举定义了 I2C_transfer() 调用的返回行为。
 *
 *  @sa  #I2C_Params
 */
typedef enum {
    /*!
     *  In #I2C_MODE_BLOCKING, calls to I2C_transfer() block until the
     *  #I2C_Transaction completes. Other threads calling I2C_transfer()
     *  while a transaction is in progress are also placed into a blocked
     *  state. If multiple threads are blocked, the thread with the highest
     *  priority will be unblocked first. This implies that arbitration
     *  will not be executed in chronological order.
     *
     *  阻塞模式：I2C_transfer() 调用会阻塞直到事务完成。
     *  高优先级线程优先解除阻塞（非时间顺序）。
     *
     *  @note When using #I2C_MODE_BLOCKING, I2C_transfer() must be called
     *  from a thread context.
     *  注意：阻塞模式下必须从线程上下文调用 I2C_transfer()。
     */
    I2C_MODE_BLOCKING,

    /*!
     *  In #I2C_MODE_CALLBACK, calls to I2C_transfer() return immediately. The
     *  application's callback function, #I2C_Params.transferCallbackFxn, is
     *  called when the transaction is complete. Sequential calls to
     *  I2C_transfer() will place #I2C_Transaction structures into an
     *  internal queue. Queued transactions are automatically started after the
     *  previous transaction has completed. This queuing occurs regardless of
     *  any error state from previous transactions. The transactions are
     *  always executed in chronological order. The
     *  #I2C_Params.transferCallbackFxn function will be called asynchronously
     *  as each transaction is completed.
     *
     *  回调模式：I2C_transfer() 立即返回。事务完成后调用回调函数。
     *  多次调用会将事务排入队列，按时间顺序依次执行。
     */
    I2C_MODE_CALLBACK
} I2C_TransferMode;

/*!
 *  @brief  The definition of a callback function.
 *          回调函数类型定义。
 *
 *  When operating in #I2C_MODE_CALLBACK, the callback function is called
 *  when an I2C_transfer() completes. The application is responsible for
 *  declaring an #I2C_CallbackFxn function and providing a pointer
 *  in #I2C_Params.transferCallbackFxn.
 *  回调模式下，I2C_transfer() 完成后调用此函数。
 *
 *  @warning  The callback function is called from an interrupt context.
 *            回调函数在中断上下文中调用，不要执行阻塞操作。
 *
 *  @param[out]  handle    #I2C_Handle used with the initial call to
 *  I2C_transfer().
 *  @param[out]  handle    I2C_transfer() 使用的 I2C 句柄。
 *
 *  @param[out]  transaction    Pointer to the #I2C_Transaction structure used
 *  with the initial call to I2C_transfer(). This structure contains the
 *  custom argument specified by @p transaction.arg and the transaction status.
 *  @param[out]  transaction    指向 I2C_Transaction 结构体的指针，包含自定义参数和事务状态。
 *
 *  @param[out]  transferStatus    Boolean indicating if the I2C transaction
 *  was successful. If @p true, the transaction was successful. If @p false,
 *  the transaction failed.
 *  @param[out]  transferStatus    传输是否成功。true 表示成功，false 表示失败。
 */
typedef void (*I2C_CallbackFxn)(
    I2C_Handle handle, I2C_Transaction *transaction, bool transferStatus);

/*!
 *  @brief  Bit rate for an I2C driver instance specified in the #I2C_Params.
 *          I2C 驱动实例的比特率，在 I2C_Params 中指定。
 *
 *  This enumeration defines the bit rates used with an I2C_transfer().
 *  此枚举定义了 I2C 传输支持的比特率。
 *
 *  @note You must check that the device specific implementation supports the
 *  desired #I2C_BitRate.
 *  注意：需确认具体设备实现是否支持所选比特率。
 */
typedef enum {
    I2C_100kHz  = 0, /*!< I2C Standard-mode. Up to 100 kbit/s.  标准模式，最高 100 kbit/s. */
    I2C_400kHz  = 1, /*!< I2C Fast-mode. Up to 400 kbit/s.     快速模式，最高 400 kbit/s. */
    I2C_1000kHz = 2, /*!< I2C Fast-mode Plus. Up to 1Mbit/s.   快速模式增强，最高 1 Mbit/s. */
} I2C_BitRate;

/*!
 *  @brief I2C parameters used with I2C_open().
 *         I2C_open() 使用的 I2C 参数结构体。
 *
 *  I2C_Params_init() must be called prior to setting fields in
 *  this structure.
 *  设置字段前必须先调用 I2C_Params_init() 初始化。
 *
 *  @sa  I2C_Params_init()
 */
typedef struct {
    /*! #I2C_TransferMode for all I2C transfers. / 所有 I2C 传输的传输模式。 */
    I2C_TransferMode transferMode;

    /*!
     *  Pointer to a #I2C_CallbackFxn to be invoked after a
     *  I2C_transfer() completes when operating in #I2C_MODE_CALLBACK.
     *  回调模式下 I2C_transfer() 完成后调用的回调函数指针。
     */
    I2C_CallbackFxn transferCallbackFxn;

    /*!
     * A #I2C_BitRate specifying the frequency at which the I2C peripheral
     * will transmit data during a I2C_transfer().
     * I2C 传输的比特率设置。
     */
    I2C_BitRate bitRate;

    /*! Pointer to a device specific extension of the #I2C_Params
     *  指向设备特定扩展参数的指针。 */
    void *custom;
} I2C_Params;

/*! @cond NODOC */
#define I2C_BASE_OBJECT                                                       \
    /* I2C control variables */                                               \
    I2C_TransferMode transferMode;       /* Blocking or Callback mode */      \
    I2C_CallbackFxn transferCallbackFxn; /* Callback function pointer */      \
    I2C_Transaction *currentTransaction; /* Ptr to current I2C transaction */ \
                                                                              \
    /* I2C transaction pointers for I2C_MODE_CALLBACK */                      \
    I2C_Transaction *volatile headPtr; /* Head ptr for queued transactions */ \
    I2C_Transaction *tailPtr;          /* Tail ptr for queued transactions */ \
                                                                              \
    /* I2C RTOS objects */                                                    \
    HwiP_Struct hwi;                    /* Hwi object handle */               \
    SemaphoreP_Struct mutex;            /* Grants exclusive access to I2C */  \
    SemaphoreP_Struct transferComplete; /* Signal I2C transfer complete */    \
                                                                              \
    /* Read and write variables */                                            \
    const uint8_t *writeBuf; /* Internal inc. writeBuf index */               \
    size_t writeCount;       /* Internal dec. writeCounter */                 \
    uint8_t *readBuf;        /* Internal inc. readBuf index */                \
    size_t readCount;        /* Internal dec. readCounter */                  \
                                                                              \
    bool isOpen; /* Flag to show module is open */                            \
/*! @endcond */

/*!
 *  @cond NODOC
 *  I2C Object. Applications must not access any member variables of
 *  this structure!
 */
typedef struct {
    I2C_BASE_OBJECT
} I2C_Object;
/*! @endcond */

/*! @cond NODOC */
#define I2C_BASE_HWATTRS                                                          \
    /*! I2C Peripheral's instance */                                              \
    I2C_Regs *i2c;                                                                \
    /*! I2C Peripheral's interrupt vector */                                      \
    uint32_t intNum;                                                              \
    /*! I2C Peripheral's interrupt priority.                                    \
     *                                                                          \
     *  Note for MSPM0: The Arm Cortex-M0+ uses two of the priority bits,       \
     *  meaning ~0 has the same effect as (3 << 6).                             \
     *                                                                          \
     *  (3 << 6) will apply the lowest priority.                                \
     *  (1 << 6) will apply the highest priority.                               \
     *                                                                          \
     *  Setting the priority to 0 is not supported by the I2C driver.           \
     *                                                                          \
     *  Hwi's with priority 0 ignore the Hwi dispatcher to support zero-latency \
     *  interrupts, thus invalidating the critical sections in this driver.     \
     */ \
    uint32_t intPriority;
/*! @endcond */

/*!
 *  @cond NODOC
 *  I2C HWAttrs.
 */
typedef struct {
    I2C_BASE_HWATTRS
} I2C_HWAttrs;
/*! @endcond */

/*!
 *  @brief I2C driver's configuration structure.
 *         I2C 驱动配置结构体。
 *
 *  @sa     I2C_init()
 *  @sa     I2C_open()
 */
typedef struct I2C_Config_ {
    /*! Pointer to a driver specific "data object".
     *  指向驱动特定数据对象的指针。 */
    void *object;

    /*! Pointer to a driver specific hardware attributes
     *  "hardware attributes structure".
     *  指向驱动特定硬件属性结构体的指针。 */
    void const *hwAttrs;
} I2C_Config;

/*!
 *  @brief Instance of I2C driver's configuration structure.
 *         I2C 驱动配置结构体实例数组。
 */
extern const I2C_Config I2C_config[];
/*!
 *  @brief Count of I2C instance
 *         I2C 实例数量。
 */
extern const uint_least8_t I2C_count;

/*!
 *  @brief  Cancels all I2C transfers / 取消所有 I2C 传输。
 *
 *  This function will cancel asynchronous I2C_transfer() operations by
 *  generating a STOP condition on the I2C bus.
 *  通过在 I2C 总线上产生 STOP 条件来取消异步传输操作。
 *
 *  Calls to I2C_cancel() return immediately; however, the transaction
 *  may not yet be canceled.
 *
 *  For #I2C_MODE_BLOCKING, the current transaction is canceled.
 *
 *  For #I2C_MODE_CALLBACK mode, the in progress transfer, as
 *  well as any queued transfers, will be canceled. The individual callback
 *  functions for each transfer will be called in chronological order. The
 *  callback functions are called in an interrupt context. Additional calls
 *  to I2C_transfer() invoked from the callback function of a canceled
 *  transaction will always fail. In such cases, the #I2C_Transaction.status
 *  will indicate #I2C_STATUS_INVALID_TRANS.
 *
 *  A canceled transaction may be identified when the #I2C_Transaction.status
 *  is set to #I2C_STATUS_CANCEL.
 *
 *  @note This API may not handle cases where the I2C target holds the clock
 *  line indefinitely.
 *
 *  @pre    I2C_Transfer() has been called.
 *
 *  @param[in]  handle  An #I2C_Handle returned from I2C_open()
 *  @param[in]  handle  I2C_open() 返回的 I2C 句柄。
 *
 *  @note   Different I2C target devices will behave differently when an
 *          in-progress transfer fails and needs to be canceled. The target
 *          may need to be reset, or there may be other target-specific
 *          steps that can be used to successfully resume communication.
 *  注意：不同 I2C 目标设备在传输取消时行为不同，可能需要复位目标设备。
 *
 *  @sa  I2C_transfer()
 *  @sa  #I2C_MODE_CALLBACK
 */
extern void I2C_cancel(I2C_Handle handle);

/*!
 *  @brief  Function to close an I2C driver instance.
 *          关闭 I2C 驱动实例。
 *
 *  @pre  I2C_open() has been called.
 *
 *  @param[in]  handle  An #I2C_Handle returned from I2C_open().
 *  @param[in]  handle  I2C_open() 返回的 I2C 句柄。
 */
extern void I2C_close(I2C_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a
 *          driver instance.
 *          对驱动实例执行特定功能控制命令。
 *
 *  @pre    I2C_open() has to be called first.
 *
 *  @param[in]  handle   An #I2C_Handle returned from I2C_open().
 *  @param[in]  handle   I2C_open() 返回的 I2C 句柄。
 *
 *  @param[in]  cmd     A command value defined by the device specific
 *                      implementation.
 *  @param[in]  cmd     设备特定实现定义的命令值。
 *
 *  @param[in]  controlArg    An optional R/W (read/write) argument that is
 *                            accompanied with @p cmd
 *  @param[in]  controlArg    配合 cmd 使用的可选读写参数。
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *  @return 实现特定的返回码，负值表示操作失败。
 *
 *  @retval #I2C_STATUS_SUCCESS The call was successful.
 *  @retval #I2C_STATUS_UNDEFINEDCMD The @p cmd value is not supported by
 *                                   the device specific implementation.
 */
extern int_fast16_t I2C_control(
    I2C_Handle handle, uint_fast16_t cmd, void *controlArg);

/*!
 *  @brief  Function to initialize the I2C driver.
 *          初始化 I2C 驱动。必须在其他 I2C API 之前调用。
 *
 *  This function must also be called before any other I2C driver APIs.
 */
extern void I2C_init(void);

/*!
 *  @brief  Open an I2C driver instance.
 *          打开一个 I2C 驱动实例。
 *
 *  @pre    I2C_init() has been called.
 *
 *  @param[in]  index    Index in the @p I2C_Config[] array.
 *  @param[in]  index    I2C_Config[] 数组中的索引。
 *
 *  @param[in]  params    Pointer to an initialized #I2C_Params structure.
 *                        If NULL, the default #I2C_Params values are used.
 *  @param[in]  params    指向 I2C_Params 结构体的指针。为 NULL 时使用默认值。
 *
 *  @return An #I2C_Handle on success, or @p NULL on an error.
 *  @return 成功返回 I2C 句柄，失败返回 NULL。
 *
 *  @sa     I2C_init()
 *  @sa     I2C_close()
 */
extern I2C_Handle I2C_open(uint_least8_t index, I2C_Params *params);

/*!
 *  @brief  Initialize an #I2C_Params structure to its default values.
 *          将 I2C_Params 结构体初始化为默认值。
 *
 *  @param[in]  params    A pointer to #I2C_Params structure for
 *                        initialization.
 *  @param[in]  params    指向待初始化的 I2C_Params 结构体的指针。
 *
 *  Defaults values are:
 *  @arg #I2C_Params.transferMode = #I2C_MODE_BLOCKING
 *  @arg #I2C_Params.transferCallbackFxn = @p NULL
 *  @arg #I2C_Params.bitRate = #I2C_100kHz
 *  @arg #I2C_Params.custom = @p NULL
 */
extern void I2C_Params_init(I2C_Params *params);

/*!
 *  @brief  Set the I2C SCL clock timeout.
 *          设置 I2C SCL 时钟超时。
 *
 *  An I2C target can extend a I2C transaction by periodically pulling the
 *  clock low to create a slow bit transfer rate. The application can use this
 *  API to program a counter in the I2C module. The count is used to force a
 *  timeout if an I2C target holds the clock line low for longer than the
 *  @p timeout duration. An #I2C_STATUS_CLOCK_TIMEOUT status indicates a
 *  timeout event occured.
 *  I2C 目标设备可通过拉低时钟线来延长传输。此 API 设置超时计数器，
 *  当目标设备持有时钟线时间超过 timeout 时触发超时。
 *
 *  @param[in]  handle      An #I2C_Handle returned from I2C_open()
 *  @param[in]  handle      I2C_open() 返回的 I2C 句柄。
 *
 *  @param[in]  timeout     Timeout in units of I2C clock cycles. Refer to
 *                          the device specifc reference manual to determine
 *                          how to calculate the timeout value.
 *  @param[in]  timeout     超时值，单位为 I2C 时钟周期。
 *
 *  @return  Possible return values include:
 *            @li #I2C_STATUS_SUCCESS
 *            @li #I2C_STATUS_ERROR
 *  @return  可能的返回值：I2C_STATUS_SUCCESS 或 I2C_STATUS_ERROR。
 *
 *  @sa I2C_transfer()
 */
extern int_fast16_t I2C_setClockTimeout(I2C_Handle handle, uint32_t timeout);

/*!
 *  @brief  Perform an I2C transaction with an I2C target peripheral.
 *          执行一次 I2C 事务。
 *
 *  This function will perform an I2C transfer, as specified by an
 *  #I2C_Transaction structure.
 *  按 I2C_Transaction 结构体描述执行 I2C 传输。
 *
 *  @note When using #I2C_MODE_BLOCKING, this must be called from a thread
 *  context.
 *  注意：阻塞模式下必须从线程上下文调用。
 *
 *  @param[in]  handle      An #I2C_Handle returned from I2C_open()
 *  @param[in]  handle      I2C_open() 返回的 I2C 句柄。
 *
 *  @param[in]  transaction  A pointer to an #I2C_Transaction. The application
 *  is responsible for allocating and initializing an #I2C_Transaction
 *  structure prior to passing it to I2C_Transfer(). This
 *  structure must persist in memory unmodified until the transfer is complete.
 *  @param[in]  transaction  指向 I2C_Transaction 的指针。传输完成前不得修改。
 *
 *  @note #I2C_Transaction structures cannot be re-used until the previous
 *  transaction has completed. Upon the completion of a transaction, the
 *  #I2C_Transaction.status may be used for error handling.
 *  注意：事务完成后才能复用。status 字段可用于错误处理。
 *
 *  @return In #I2C_MODE_BLOCKING: @p true for a successful transfer; @p false
 *          for an error (for example, an I2C bus fault (NACK)).
 *  @return 阻塞模式：成功返回 true，失败返回 false。
 *
 *  @return In #I2C_MODE_CALLBACK: always @p true. The #I2C_CallbackFxn @p bool
 *          argument will be @p true to indicate success, and @p false to
 *          indicate an error.
 *  @return 回调模式：始终返回 true，实际结果通过回调参数传递。
 *
 *  @pre I2C_open() has been called.
 *
 *  @sa  I2C_open(), I2C_transferTimeout()
 *  @sa  I2C_Transaction
 */
extern bool I2C_transfer(I2C_Handle handle, I2C_Transaction *transaction);

/*!
 *  @brief  Perform an I2C transaction with an I2C target peripheral.
 *          执行带超时功能的 I2C 事务。
 *
 *  This function will perform an I2C transfer, as specified by an
 *  #I2C_Transaction structure. If the timeout is exceeded, then the
 *  I2C transaction is canceled.
 *  超时后将取消事务。
 *
 *  @note When using #I2C_MODE_BLOCKING, this must be called from a thread
 *  context.
 *  注意：阻塞模式下必须从线程上下文调用。
 *
 *  @note  The timeout restriction is only applied when using
 *  #I2C_MODE_BLOCKING. If using #I2C_MODE_CALLBACK, the application should
 *  manage timeouts using I2C_cancel(). Additionally, this timeout may not
 *  handle cases where the I2C target holds the clock line indefinitely.
 *  注意：超时仅在阻塞模式下生效。回调模式需自行调用 I2C_cancel() 管理超时。
 *
 *  @param[in]  handle    An #I2C_Handle returned from I2C_open()
 *  @param[in]  handle    I2C_open() 返回的 I2C 句柄。
 *
 *  @param[in]  transaction  A pointer to an #I2C_Transaction. The application
 *  is responsible for allocating and initializing an #I2C_Transaction
 *  structure prior to passing it to I2C_TransferTimeout(). This
 *  structure must persist in memory unmodified until the transfer is complete.
 *  @param[in]  transaction  指向 I2C_Transaction 的指针。传输完成前不得修改。
 *
 *  @param[in]  timeout    The time in system ticks to wait for the transaction
 *  to complete. Passing I2C_WAIT_FOREVER into this parameter will cause
 *  I2C_transferTimeout() to behave the same as I2C_transfer() but with a
 *  more detailed return status.
 *  @param[in]  timeout    等待事务完成的系统节拍数。传入 I2C_WAIT_FOREVER 则永久等待。
 *
 *  @note #I2C_Transaction structures cannot be re-used until the previous
 *  transaction has completed. Upon the completion of a transaction, the
 *  #I2C_Transaction.status may be used for error handling.
 *
 *  @return  In #I2C_MODE_CALLBACK: always @p I2C_STATUS_SUCCESS.
 *           The #I2C_CallbackFxn @p transferStatus argument will be @p true
 *           to indicate success, and @p false to indicate an error.
 *  @return  回调模式：始终返回 I2C_STATUS_SUCCESS，实际结果通过回调传递。
 *
 *  @return  In #I2C_MODE_BLOCKING: Possible return values include:
 *  @return  阻塞模式可能的返回值：
 *            @li #I2C_STATUS_SUCCESS
 *            @li #I2C_STATUS_ERROR
 *            @li #I2C_STATUS_TIMEOUT
 *            @li #I2C_STATUS_CLOCK_TIMEOUT
 *            @li #I2C_STATUS_ADDR_NACK
 *            @li #I2C_STATUS_DATA_NACK
 *            @li #I2C_STATUS_ARB_LOST
 *            @li #I2C_STATUS_INCOMPLETE
 *            @li #I2C_STATUS_BUS_BUSY
 *            @li #I2C_STATUS_CANCEL
 *            @li #I2C_STATUS_INVALID_TRANS
 *
 *  @pre I2C_open() has been called.
 *
 *  @sa  I2C_open(), I2C_transfer()
 *  @sa  I2C_Transaction
 */
extern int_fast16_t I2C_transferTimeout(
    I2C_Handle handle, I2C_Transaction *transaction, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_I2C__include */
/** @}*/
