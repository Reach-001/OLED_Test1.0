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
/*!*****************************************************************************
 *  @file       SPI.h
 *
 *  @brief      SPI Driver Interface
 *  SPI 驱动接口头文件，定义了 SPI 通用抽象层的数据类型、枚举、宏及函数声明。
 *
 *  @defgroup   SPI Serial Peripheral Interface (SPI)
 *
 *  The SPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/SPI.h>
 *  @endcode
 *
 *  @anchor ti_drivers_SPI_Overview
 *  # Overview
 *  The Serial Peripheral Interface (SPI) driver is a generic, full-duplex
 *  driver that transmits and receives data on a SPI bus.  SPI is sometimes
 *  called SSI (Synchronous Serial Interface).
 *  The SPI protocol defines the format of a data transfer over the SPI bus,
 *  but it leaves flow control, data formatting, and handshaking mechanisms
 *  to higher-level software layers.
 *
 *  The SPI driver operates on some key definitions and assumptions:
 *  - The driver operates transparently from the chip select. Some SPI
 *    controllers feature a hardware chip select to assert SPI peripheral
 *    peripherals. See the specific device implementations on chip
 *    select requirements.
 *
 *  - The SPI protocol does not account for a built-in handshaking mechanism
 *    and neither does this SPI driver. Therefore, when operating in
 *    #SPI_PERIPHERAL mode, the application must provide such a mechanism to
 *    ensure that the SPI peripheral is ready for the SPI controller. The SPI
 *    peripheral must call #SPI_transfer() *before* the SPI controller starts
 *    transmitting.
 *    Some example application mechanisms could include:
 *    - Timed delays on the SPI controller to guarantee the SPI peripheral is
 *      ready for a SPI transaction.
 *    - A form of GPIO flow control from the peripheral to the SPI controller to
 *      notify the controller when ready.
 *
 *  <hr>
 *  @anchor ti_drivers_SPI_Usage
 *  # Usage
 *
 *  To use the SPI driver to send data over the SPI bus, the application
 *  calls the following APIs:
 *    - SPI_init(): Initialize the SPI driver.
 *    - SPI_Params_init():  Initialize a #SPI_Params structure with default
 *      values.  Then change the parameters from non-default values as
 *      needed.
 *    - SPI_open():  Open an instance of the SPI driver, passing the
 *      initialized parameters, or NULL, and an index to the configuration to
 *      open (detailed later).
 *    - SPI_transfer():  Transmit/receive data.  This function takes a
 *      #SPI_Transaction argument that describes the transfer that is requested.
 *    - SPI_close():  De-initialize the SPI instance.
 *
 *  @anchor ti_drivers_SPI_Synopsis
 *  ## Synopsis
 *  The following code example opens a SPI instance as a SPI controller,
 *  and issues a transaction.
 *
 *  @code
 *  SPI_Handle      spi;
 *  SPI_Params      spiParams;
 *  SPI_Transaction spiTransaction;
 *  uint8_t         transmitBuffer[MSGSIZE];
 *  uint8_t         receiveBuffer[MSGSIZE];
 *  bool            transferOK;
 *
 *  SPI_init();  // Initialize the SPI driver
 *
 *  SPI_Params_init(&spiParams);  // Initialize SPI parameters
 *  spiParams.dataSize = 8;       // 8-bit data size
 *
 *  spi = SPI_open(CONFIG_SPI0, &spiParams);
 *  if (spi == NULL) {
 *      while (1);  // SPI_open() failed
 *  }
 *
 *  // Fill in transmitBuffer
 *  spiTransaction.count = MSGSIZE;
 *  spiTransaction.txBuf = (void *)transmitBuffer;
 *  spiTransaction.rxBuf = (void *)receiveBuffer;
 *
 *  transferOK = SPI_transfer(spi, &spiTransaction);
 *  if (!transferOK) {
 *      // Error in SPI or transfer already in progress.
 *      while (1);
 *  }
 *  @endcode
 *
 *  More details on usage are provided in the following subsections.
 *
 *  @anchor ti_drivers_SPI_Examples
 *  ## Examples #
 *  * @ref ti_drivers_SPI_Synopsis "Usage Synopsis"
 *  * @ref ti_drivers_SPI_Example_openblocking "Open in blocking mode"
 *  * @ref ti_drivers_SPI_Example_opencallback "Open in callback mode"
 *  * @ref ti_drivers_SPI_Example_6bitframes "Sending 6 bit frames"
 *  * @ref ti_drivers_SPI_Example_12bitframes "Sending 12 bit frames"
 *  * @ref ti_drivers_SPI_Example_callbackarg "Callback function using arg"
 *
 *  ## Initializing the SPI Driver
 *
 *  SPI_init() must be called before any other SPI APIs.  This function
 *  iterates through the elements of the @p SPI_config[] array, calling
 *  the element's device implementation SPI initialization function.
 *
 *  ## Opening the SPI Driver
 *  After initializing the SPI driver by calling SPI_init(), the application
 *  can open a SPI instance by calling SPI_open().  This function
 *  takes an index into the @p SPI_config[] array, and a SPI parameters data
 *  structure. The SPI instance is specified by the index of the SPI in
 *  @p SPI_config[]. Calling SPI_open() a second time with the same index
 *  previously passed to SPI_open() will result in an error.  You can,
 *  though, re-use the index if the instance is closed via SPI_close().
 *
 *  If no #SPI_Params structure is passed to SPI_open(), default values are
 *  used. If the open call is successful, it returns a non-NULL value.
 *
 *  @anchor ti_drivers_SPI_Example_openblocking
 *  Example opening a SPI driver instance in blocking mode:
 *  @code
 *  SPI_Handle  spi;
 *  SPI_Params  spiParams;
 *
 *  SPI_Params_init(&spiParams);
 *  spiParams.transferMode = SPI_MODE_BLOCKING;
 *  spi = SPI_open(CONFIG_SPI0, &spiParams);
 *
 *  if (spi == NULL) {
 *      // Error opening SPI
 *      while(1);
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_SPI_Example_opencallback
 *  Example opening a SPI driver instance in callback mode:
 *  @code
 *  SPI_Handle spi;
 *  SPI_Params spiParams;
 *
 *  SPI_Params_init(&spiParams);
 *  spiParams.transferMode = SPI_MODE_CALLBACK;
 *  spiParams.transferCallbackFxn = UserCallbackFxn;
 *
 *  spi = SPI_open(CONFIG_SPI0, &spiParams);
 *  if (spi == NULL) {
 *      // Error opening SPI
 *      while (1);
 *  }
 *  @endcode
 *
 *  ## SPI Parameters
 *
 *  The #SPI_Params structure is passed to the SPI_open() call.  If NULL
 *  is passed for the parameters, SPI_open() uses default parameters.
 *  A #SPI_Params structure is initialized with default values by passing
 *  it to SPI_Params_init().
 *  Some of the SPI parameters are described below.  To see brief descriptions
 *  of all the parameters, see #SPI_Params.
 *
 *  ### SPI Mode
 *  The SPI driver operates in both SPI controller and SPI peripheral modes.
 *  Logically, the implementation is identical, however the difference
 *  between these two modes is driven by hardware.  The default mode is
 *  #SPI_CONTROLLER, but can be set to peripheral mode by setting
 *  #SPI_Params.mode to #SPI_PERIPHERAL in the parameters passed to SPI_open().
 *  See @ref ti_drivers_SPI_ControllerPeripheralModes "Controller/Peripheral Modes"
 *  for further details.
 *
 *  ### SPI Transfer Mode
 *  The SPI driver supports two transfer modes of operation: blocking and
 *  callback. The transfer mode is determined by the transferMode parameter
 *  in the #SPI_Params data structure. The SPI driver
 *  defaults to blocking mode, if the application does not set it.
 *  Once a SPI driver is opened, the only way to change the operation mode
 *  is to close and re-open the SPI instance with the new transfer mode.
 *
 *  In blocking mode, a task's code execution is blocked until a SPI
 *  transaction has completed or a timeout has occurred. This ensures
 *  that only one SPI transfer operates at a given time. Other tasks requesting
 *  SPI transfers while a transfer is currently taking place will receive
 *  a FALSE return value.  If a timeout occurs the transfer is canceled, the
 *  task is unblocked & will receive a FALSE return value. The transaction
 *  count field will have the amount of frames which were transferred
 *  successfully before the timeout.  In blocking mode, transfers cannot be
 *  performed in software or hardware ISR context.
 *
 *  In callback mode, a SPI transaction functions asynchronously, which
 *  means that it does not block code execution. After a SPI transaction
 *  has been completed, the SPI driver calls a user-provided hook function.
 *  Callback mode is supported in the execution context of tasks and
 *  hardware interrupt routines.
 *
 *  ### SPI Frame Formats and Data Size
 *  The SPI driver can configure the device's SPI peripheral to transfer
 *  data in several SPI format options: SPI (with various polarity and phase
 *  settings), TI, and Micro-wire. The frame format is set with
 *  #SPI_Params.frameFormat. Some SPI implementations may not support all frame
 *  formats & the SPI driver will fail to opened.  Refer to the device specific
 *  implementation documentation for details on which frame formats are
 *  supported.
 *
 *  The smallest single unit of data transmitted onto the SPI bus is called
 *  a SPI frame and is of size #SPI_Params.dataSize.  A series of SPI frames
 *  transmitted/received on a SPI bus is referred to as a SPI transaction.
 *
 *  ## SPI Transactions
 *
 *  A SPI transaction consists of a series of SPI frames
 *  transmitted/received on a SPI bus.  A SPI transaction is performed
 *  using SPI_transfer(). SPI_transfer() accepts a pointer to a
 *  #SPI_Transaction structure that dictates the quantity of data to be
 *  sent and received.
 *  The #SPI_Transaction.txBuf and #SPI_Transaction.rxBuf are both pointers
 *  to data buffers.  If txBuf is NULL, the driver sends SPI frames with all
 *  data set to the default value specified in the hardware attributes. If
 *  rxBuf is NULL, the driver discards all SPI frames received. SPI_transfer()
 *  of a SPI transaction is performed atomically.
 *
 *  @warning The use of NULL as a sentinel txBuf or rxBuf value to determine
 *  whether the SPI transaction includes a tx or rx component implies
 *  that it is not possible to perform a transmit or receive transfer
 *  directly from/to a buffer with a base address of 0x00000000. To support
 *  this rare use-case, the application will have to manually copy the
 *  contents of location 0x00000000 to/from a temporary buffer before/after
 *  the tx/rx SPI transaction.
 *
 *  When the SPI is opened, the dataSize value determines the element types
 *  of txBuf and rxBuf. If the dataSize is from 4 to 8 bits, the driver
 *  assumes the data buffers are of type uint8_t (unsigned char). If the
 *  dataSize is from 8 to 16 bits, the driver assumes the data buffers are
 *  of type uint16_t (unsigned short).  If the dataSize is greater than
 *  16 bits, the driver assumes the data buffers are uint32_t (unsigned long).
 *  Some SPI driver implementations may not support all data sizes; refer
 *  to device specific SPI implementation documentation for details on
 *  what data sizes are supported.
 *
 *  The optional #SPI_Transaction.arg variable can only be used when the
 *  SPI driver has been opened in callback mode. This variable is used to
 *  pass a user-defined value into the user-defined callback function.
 *
 *  SPI_transfer() always performs full-duplex SPI transactions. This means
 *  the SPI simultaneously receives data as it transmits data. The application
 *  is responsible for formatting the data to be transmitted as well as
 *  determining whether the data received is meaningful.
 *  Specifics about SPI frame formatting and data sizes are provided in
 *  device-specific data sheets and technical reference manuals.
 *
 *  The following code snippets perform SPI transactions.
 *
 *  @anchor ti_drivers_SPI_Example_6bitframes
 *  Example transferring 6-bit SPI frames.  The transmit and receive
 *  buffers are of type uint8_t.
 *  @code
 *  SPI_Transaction spiTransaction;
 *  uint8_t         transmitBuffer[BUFSIZE];
 *  uint8_t         receiveBuffer[BUFSIZE];
 *  bool            transferOK;
 *
 *  SPI_Params_init(&spiParams);
 *  spiParams.dataSize = 6;
 *  spi = SPI_open(CONFIG_SPI0, &spiParams);
 *  ...
 *  spiTransaction.count = someIntegerValue;
 *  spiTransaction.txBuf = transmitBuffer;
 *  spiTransaction.rxBuf = receiveBuffer;
 *
 *  transferOK = SPI_transfer(spi, &spiTransaction);
 *  if (!transferOK) {
 *      // Error in SPI or transfer already in progress.
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_SPI_Example_12bitframes
 *  Example transferring 12-bit SPI frames.  The transmit and receive
 *  buffers are of type uint16_t.
 *  @code
 *  SPI_Transaction spiTransaction;
 *  uint16_t        transmitBuffer[BUFSIZE];
 *  uint16_t        receiveBuffer[BUFSIZE];
 *  bool            transferOK;
 *
 *  SPI_Params_init(&spiParams);
 *  spiParams.dataSize = 12;
 *  spi = SPI_open(CONFIG_SPI0, &spiParams);
 *  ...
 *  spiTransaction.count = someIntegerValue;
 *  spiTransaction.txBuf = transmitBuffer;
 *  spiTransaction.rxBuf = receiveBuffer;
 *
 *  transferOK = SPI_transfer(spi, &spiTransaction);
 *  if (!transferOK) {
 *      // Error in SPI or transfer already in progress.
 *  }
 *  @endcode
 *
 * The following example shows an example of a callback function that
 * utilizes the arg parameter.
 *  @anchor ti_drivers_SPI_Example_callbackarg
 *  @code
 *  // SPI is opened with callback function as seen in other examples
 *  // Our goal is to post a semaphore when transfer with sufficient size
 *  // completes.
 *  ...
 *  // Pass pointer to an initialized semaphore to callback via arg
 *  spiTransaction.count = someIntegerValue;
 *  spiTransaction.txBuf = transmitBuffer;
 *  spiTransaction.rxBuf = receiveBuffer;
 *  spiTransaction.arg   = &mySemaphore;
 *
 *  ...
 *
 *  // Our callback function is defined here
 *  void spiCallbackFxn(SPI_Handle spi, SPI_Transaction *tran)
 *  {
 *      sem_t *semPtr = (sem_t *)(tran->arg);
 *
 *      // Post the semaphore if our transaction was more than LIMIT
 *      if (tran->status == SPI_STATUS_SUCCESS &&
 *          tran->count > LIMIT) {
 *          sem_post(semPtr);
 *      }
 *  }
 *  @endcode
 *
 *  ## Canceling a transaction
 *  SPI_transferCancel() is used to cancel a SPI transaction when the driver is
 *  used in #SPI_MODE_CALLBACK mode.
 *
 *  Calling this API while no transfer is in progress has no effect. If a
 *  transfer is in progress, it is canceled and the callback functions is
 *  called.
 *  The #SPI_Status status field in the #SPI_Transaction structure
 *  can be examined within the callback to determine if the transaction
 *  succeeded.
 *
 *  Example:
 *  @code
 *  SPI_transferCancel(spi);
 *  @endcode
 *
 *  @anchor ti_drivers_SPI_ControllerPeripheralModes
 *  ## Controller/Peripheral Modes
 *  This SPI driver functions in both SPI controller and SPI peripheral modes.
 *  Logically, the implementation is identical, however the difference between
 *  these two modes is driven by hardware. As a SPI controller, the peripheral is
 *  in control of the clock signal and therefore will commence communications
 *  to the SPI peripheral immediately. As a SPI peripheral, the SPI driver
 *  prepares the peripheral to transmit and receive data in a way such that the
 *  peripheral is ready to transfer data when the SPI controller initiates a
 *  transaction.
 *
 *  ## Asserting on Chip Select
 *  The SPI protocol requires that the SPI controller asserts a SPI peripheral's
 *  chip select pin prior to starting a SPI transaction. While this protocol is
 *  generally followed, various types of SPI peripherals have different
 *  timing requirements as to when and for how long the chip select pin must
 *  remain asserted for a SPI transaction.
 *
 *  Commonly, the SPI controller uses a hardware chip select to assert and
 *  de-assert the SPI peripheral for every data frame. In other cases, a SPI
 *  peripheral imposes the requirement of asserting the chip select over several SPI
 *  data frames. This is generally accomplished by using a regular,
 *  general-purpose output pin. Due to the complexity of such SPI peripheral
 *  implementations, this SPI driver has been designed to operate
 *  transparently to the SPI chip select. When the hardware chip
 *  select is used, the peripheral automatically selects/enables the
 *  peripheral. When using a software chip select, the application needs to
 *  handle the proper chip select and pin configuration.  Chip select support
 *  will vary per SPI peripheral, refer to the device specific implementation
 *  documentation for details on chip select support.
 *
 *  - _Hardware chip select_  No additional action by the application is
 *    required.
 *  - _Software chip select_  The application needs to handle the chip select
 *    assertion and de-assertion for the proper SPI peripheral.
 *
 *  <hr>
 *  @anchor ti_drivers_SPI_Configuration
 *  # Configuration
 *
 *  In order to use the SPI APIs, the application is required to provide
 *  device-specific SPI configuration in the ti_drivers_config.c file.
 *  The SPI driver interface defines a configuration data structure:
 *
 *  @code
 *  typedef struct  {
 *      SPI_FxnTable  const    *fxnTablePtr;
 *      void                   *object;
 *      void          const    *hwAttrs;
 *  } SPI_Config;
 *  @endcode
 *
 *  The application must declare an array of #SPI_Config elements, named
 *  @p SPI_config[].  Each element of @p SPI_config[] must be populated with
 *  pointers to a device specific SPI driver implementation's function
 *  table, driver object, and hardware attributes.  The hardware attributes
 *  define properties such as the SPI peripheral's base address, and
 *  the PICO and POCI pins.  Each element in @p SPI_config[] corresponds to
 *  a SPI instance, and none of the elements should have NULL pointers.
 *  There is no correlation between the index and the
 *  peripheral designation (such as SPI0 or SPI1).  For example, it is
 *  possible to use SPI_config[0] for SPI1.
 *
 *  Because the SPI configuration is very device dependent, you will need to
 *  check the doxygen for the device specific SPI implementation.  There you
 *  will find a description of the SPI hardware attributes.  Please also
 *  refer to the ti_drivers_config.c file of any of your examples to see the
 *  SPI configuration.
 *
 *******************************************************************************
 */
/** @addtogroup SPI
 * @{
 */
/* clang-format off */
#ifndef ti_drivers_SPI__include
#define ti_drivers_SPI__include

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup SPI_CONTROL SPI_control command and status codes
 *  These SPI macros are reservations for SPI.h
 *  @{
 */

/*!
 * Common SPI_control() command code reservation offset.
 * SPI driver implementations should offset command codes with #SPI_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define SPIXYZ_CMD_COMMAND0     SPI_CMD_RESERVED + 0
 * #define SPIXYZ_CMD_COMMAND1     SPI_CMD_RESERVED + 1
 * @endcode
 *
 * SPI_control() 命令码保留偏移量。驱动实现应从此值开始递增定义私有命令码。
 */
#define SPI_CMD_RESERVED (32)

/*!
 * Common SPI_control status code reservation offset.
 * SPI driver implementations should offset status codes with
 * #SPI_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define SPIXYZ_STATUS_ERROR0    SPI_STATUS_RESERVED - 0
 * #define SPIXYZ_STATUS_ERROR1    SPI_STATUS_RESERVED - 1
 * #define SPIXYZ_STATUS_ERROR2    SPI_STATUS_RESERVED - 2
 * @endcode
 *
 * SPI_control() 状态码保留偏移量。驱动实现应从此值开始递减定义私有错误码。
 */
#define SPI_STATUS_RESERVED (-32)

/**
 *  @defgroup SPI_STATUS Status Codes
 *  SPI_STATUS_* macros are general status codes returned by SPI_control()
 *  @{
 *  @ingroup SPI_CONTROL
 */

/*!
 * @brief   Successful status code returned by SPI_control().
 * SPI_control() 执行成功时返回的状态码。
 *
 * This value is returned from SPI_control() if the control code was executed
 * successfully.
 */
#define SPI_STATUS_SUCCESS (0)

/*!
 * @brief   Generic error status code returned by SPI_control().
 * SPI_control() 执行失败时返回的通用错误码。
 *
 * This value is returned from SPI_control() if the control code was not
 * executed successfully.
 */
#define SPI_STATUS_ERROR (-1)

/*!
 * @brief   An error status code returned by SPI_control() for undefined
 * command codes.
 * SPI_control() 遇到未定义命令码时返回的错误码。
 *
 * This value is returned from SPI_control() if the control code is not
 * recognized by the driver implementation.
 */
#define SPI_STATUS_UNDEFINEDCMD (-2)
/** @}*/

/**
 *  @defgroup SPI_CMD Command Codes
 *  SPI_CMD_* macros are general command codes for SPI_control(). Not all SPI
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup SPI_CONTROL
 */

/* Add SPI_CMD_<commands> here */

/** @}*/

/** @}*/

/*!
 *  @brief    Wait forever define used to specify timeouts.
 *  永久等待，用于指定超时时间为无限等待。
 */
#define SPI_WAIT_FOREVER (~(0U))

/*!
 *  @brief      A handle that is returned from a SPI_open() call.
 *  SPI_open() 返回的句柄类型，指向 SPI_Config_ 结构体指针。
 */
typedef struct SPI_Config_ *SPI_Handle;

/*!
 *  @brief      Status codes that are set by the SPI driver.
 *  SPI 驱动设置的传输状态码。
 */
typedef enum
{
    /*! SPI transfer completed */
    SPI_TRANSFER_COMPLETED = 0,         /* SPI 传输完成 */
    /*! SPI transfer started and in progress */
    SPI_TRANSFER_STARTED,               /* SPI 传输已启动并正在进行 */
    /*! SPI transfer was canceled */
    SPI_TRANSFER_CANCELED,              /* SPI 传输已取消 */
    /*! SPI transfer failed */
    SPI_TRANSFER_FAILED,                /* SPI 传输失败 */
    /*! SPI chip select was de-asserted (only
     * applicable in return partial mode)
     */
    SPI_TRANSFER_CSN_DEASSERT,          /* SPI 片选已释放（仅适用于部分返回模式） */
    /*! SPI transfer is pending until the chip
     * select is asserted
     */
    SPI_TRANSFER_PEND_CSN_ASSERT,       /* SPI 传输等待片选信号有效 */
    /*! SPI transfer added to transaction queue */
    SPI_TRANSFER_QUEUED                  /* SPI 传输已加入事务队列 */
} SPI_Status;

/*!
 *  @brief
 *  A #SPI_Transaction data structure is used with SPI_transfer(). It indicates
 *  how many #SPI_FrameFormat frames are sent and received from the buffers
 *  pointed to txBuf and rxBuf.
 *  The arg variable is an user-definable argument which gets passed to the
 *  #SPI_CallbackFxn when the SPI driver is in #SPI_MODE_CALLBACK.
 *
 *  SPI 事务结构体，配合 SPI_transfer() 使用。指示需要发送/接收的帧数
 *  以及 txBuf/rxBuf 缓冲区。arg 可在回调模式下传递用户自定义参数。
 */
typedef struct
{
    /* User input (write-only) fields */
    /*! Number of frames for this transaction */
    size_t count;               /* 本次事务的帧数 */
    /*! void * to a buffer with data to be transmitted */
    void *txBuf;                /* 发送数据缓冲区指针 */
    /*! void * to a buffer to receive data */
    void *rxBuf;                /* 接收数据缓冲区指针 */
    /*! Argument to be passed to the callback function */
    void *arg;                  /* 传递给回调函数的用户参数 */
    /* User output (read-only) fields */
    /*! #SPI_Status code set by SPI_transfer */
    SPI_Status status;          /* 由 SPI_transfer 设置的传输状态码 */
    /*! Field used internally by the driver and must
     * never be accessed by the application.
     */
    void *nextPtr;              /* 驱动内部使用，应用程序禁止访问 */
} SPI_Transaction;

/*!
 *  @brief      The definition of a callback function used by the SPI driver
 *              when used in #SPI_MODE_CALLBACK
 *  SPI 回调函数类型定义，用于回调模式下的传输完成通知。
 *
 *  @param      SPI_Handle          A #SPI_Handle          SPI 句柄
 *  @param      SPI_Transaction*    Pointer to a #SPI_Transaction  指向已完成的事务结构体
 */
typedef void (*SPI_CallbackFxn)(SPI_Handle handle, SPI_Transaction *transaction);
/*!
 *  @brief
 *  Definitions for various SPI modes of operation.
 *  SPI 工作模式定义。
 */
typedef enum
{
    /*! SPI in controller mode */
    SPI_CONTROLLER = 0,     /* SPI 主机模式 */
    /*! SPI in peripheral mode */
    SPI_PERIPHERAL = 1      /* SPI 从机模式 */
} SPI_Mode;

/*!
 *  @brief
 *  Definitions for parity.
 *  SPI 奇偶校验定义。
 */
typedef enum
{
    /*! SPI parity even */
    SPI_PARITY_EVEN = 0,    /* 偶校验 */
    /*! SPI parity odd */
    SPI_PARITY_ODD = 1,     /* 奇校验 */
    /*! SPI parity none */
    SPI_PARITY_NONE = 2,    /* 无校验 */
} SPI_Parity;
/*!
 *  @brief
 *  Definitions for bit order.
 *  SPI 位序定义。
 */
typedef enum
{
    /*! MSB first */
    SPI_BIT_ORDER_MSB_FIRST = 0,    /* 高位先发 */
    /*! LSB first */
    SPI_BIT_ORDER_LSB_FIRST = 1,    /* 低位先发 */
} SPI_BitOrder;
/*!
 *  @brief
 *  Definitions for chip select.
 *  SPI 片选引脚定义。
 */
typedef enum
{
    /*! Chip select 0 */
    SPI_CHIP_SELECT_0 = 0,      /* 片选 0 */
    /*! Chip select 1 */
    SPI_CHIP_SELECT_1 = 1,      /* 片选 1 */
    /*! Chip select 2 */
    SPI_CHIP_SELECT_2 = 2,      /* 片选 2 */
    /*! Chip select 3 */
    SPI_CHIP_SELECT_3 = 3,      /* 片选 3 */
    /*! No chip select */
    SPI_CHIP_SELECT_NONE = 4    /* 无片选 */
} SPI_Chip_Select;

/*!
 *  @brief
 *  Definitions for various SPI data frame formats.
 *  SPI 数据帧格式定义，包含极性(Polarity)和相位(Phase)组合。
 */
typedef enum
{
    /*! SPI Motorola 3 mode Polarity 0 Phase 0 */
    SPI_MOTO3_POL0_PHA0 = 0,   /* Motorola 3线模式，极性0 相位0 */
    /*! SPI Motorola 3 mode Polarity 0 Phase 1 */
    SPI_MOTO3_POL0_PHA1 = 1,   /* Motorola 3线模式，极性0 相位1 */
    /*! SPI Motorola 3 mode Polarity 1 Phase 0 */
    SPI_MOTO3_POL1_PHA0 = 2,   /* Motorola 3线模式，极性1 相位0 */
    /*! SPI Motorola 3 mode Polarity 1 Phase 1 */
    SPI_MOTO3_POL1_PHA1 = 3,   /* Motorola 3线模式，极性1 相位1 */
    /*! SPI Motorola 4 mode Polarity 0 Phase 0 */
    SPI_MOTO4_POL0_PHA0 = 4,   /* Motorola 4线模式，极性0 相位0 */
    /*! SPI Motorola 4 mode Polarity 0 Phase 1 */
    SPI_MOTO4_POL0_PHA1 = 5,   /* Motorola 4线模式，极性0 相位1 */
    /*! SPI Motorola 4 mode Polarity 1 Phase 0 */
    SPI_MOTO4_POL1_PHA0 = 6,   /* Motorola 4线模式，极性1 相位0 */
    /*! SPI Motorola 4 mode Polarity 1 Phase 1 */
    SPI_MOTO4_POL1_PHA1 = 7,   /* Motorola 4线模式，极性1 相位1 */
    /*! TI mode (not supported on all
     *implementations)
     */
    SPI_TI = 8                  /* TI 同步模式（部分实现不支持） */
} SPI_FrameFormat;

/*!
 *  @brief
 *
 *  SPI transfer mode determines the whether the SPI controller operates
 *  synchronously or asynchronously. In #SPI_MODE_BLOCKING mode SPI_transfer()
 *  blocks code execution until the SPI transaction has completed. In
 *  #SPI_MODE_CALLBACK SPI_transfer() does not block code execution and instead
 *  calls a #SPI_CallbackFxn callback function when the transaction has
 *  completed (successfully or not).
 *
 *  SPI 传输模式，决定同步（阻塞）或异步（回调）操作。
 *  阻塞模式下 SPI_transfer() 会阻塞直到传输完成；
 *  回调模式下传输完成后调用回调函数通知应用程序。
 */
typedef enum
{
    /*!
     * SPI_transfer() blocks execution. This mode can only be used when called
     * within a Task context
     */
    SPI_MODE_BLOCKING,      /* 阻塞模式，仅可在任务上下文中调用 */
    /*!
     * SPI_transfer() does not block code execution and will call a
     * #SPI_CallbackFxn. This mode can be used in a Task, software or hardware
     * interrupt context.
     */
    SPI_MODE_CALLBACK       /* 回调模式，可在任务、软件中断或硬件中断上下文中使用 */
} SPI_TransferMode;

/*!
 *  @brief SPI Parameters
 *
 *  SPI Parameters are used to with the SPI_open() call. Default values for
 *  these parameters are set using SPI_Params_init().
 *
 *  SPI 参数结构体，配合 SPI_open() 使用。默认值通过 SPI_Params_init() 设置。
 *
 *  @sa         SPI_Params_init()
 */
typedef struct
{
    /*! Blocking or Callback mode */
    SPI_TransferMode transferMode;          /* 传输模式：阻塞或回调 */
    /*! Transfer timeout in system ticks */
    uint32_t transferTimeout;               /* 传输超时时间（系统 tick） */
    /*! Callback function pointer */
    SPI_CallbackFxn transferCallbackFxn;    /* 回调模式下的回调函数指针 */
    /*! Controller or Peripheral mode */
    SPI_Mode mode;                          /* 工作模式：主机或从机 */
    /*! @brief SPI bit rate in Hz */
    uint32_t bitRate;                       /* SPI 时钟速率（Hz） */
    /*! SPI data frame size in bits */
    uint32_t dataSize;                      /* 数据帧大小（位），范围 4~16 */
    /*! SPI frame format */
    SPI_FrameFormat frameFormat;            /* 数据帧格式 */
    /*! Parity can be set as even, odd or none */
    SPI_Parity parity;                      /* 奇偶校验设置 */
    /*! false: MSB first, true: LSB first */
    SPI_BitOrder bitOrder;                  /* 位序：MSB优先 或 LSB优先 */
    /*! Custom argument used by driver implementation */
    void *custom;                           /* 驱动实现自定义参数 */
} SPI_Params;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_close().
 *  SPI_close() 的驱动实现函数指针类型。
 */
typedef void (*SPI_CloseFxn)(SPI_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_control().
 *  SPI_control() 的驱动实现函数指针类型。
 */
typedef int_fast16_t (*SPI_ControlFxn)(SPI_Handle handle, uint_fast16_t cmd, void *arg);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_init().
 *  SPI_init() 的驱动实现函数指针类型。
 */
typedef void (*SPI_InitFxn)(SPI_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_open().
 *  SPI_open() 的驱动实现函数指针类型。
 */
typedef SPI_Handle (*SPI_OpenFxn)(SPI_Handle handle, SPI_Params *params);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_transfer().
 *  SPI_transfer() 的驱动实现函数指针类型。
 */
typedef bool (*SPI_TransferFxn)(SPI_Handle handle, SPI_Transaction *transaction);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_transferCancel().
 *  SPI_transferCancel() 的驱动实现函数指针类型。
 */
typedef void (*SPI_TransferCancelFxn)(SPI_Handle handle);

/*!
 *  @brief      The definition of a SPI function table that contains the
 *              required set of functions to control a specific SPI driver
 *              implementation.
 *  SPI 函数表，包含特定驱动实现所需的全部操作函数指针。
 */
typedef struct
{
    /*! Function to close the specified peripheral */
    SPI_CloseFxn closeFxn;              /* 关闭外设函数 */

    /*! Function to implementation specific control function */
    SPI_ControlFxn controlFxn;          /* 控制命令函数 */

    /*! Function to initialize the given data object */
    SPI_InitFxn initFxn;                /* 初始化函数 */

    /*! Function to open the specified peripheral */
    SPI_OpenFxn openFxn;                /* 打开外设函数 */

    /*! Function to initiate a SPI data transfer */
    SPI_TransferFxn transferFxn;        /* 发起数据传输函数 */

    /*! Function to cancel SPI data transfer */
    SPI_TransferCancelFxn transferCancelFxn;  /* 取消数据传输函数 */
} SPI_FxnTable;

/*!
 *  @brief SPI Global configuration
 *
 *  The #SPI_Config structure contains a set of pointers used to characterize
 *  the SPI driver implementation.
 *
 *  This structure needs to be defined before calling SPI_init() and it must
 *  not be changed thereafter.
 *
 *  SPI 全局配置结构体，包含驱动实现所需的函数表、对象和硬件属性指针。
 *  必须在调用 SPI_init() 前定义，之后不可修改。
 *
 *  @sa     SPI_init()
 */
typedef struct SPI_Config_
{
    /*! Pointer to a table of driver-specific implementations of SPI APIs */
    SPI_FxnTable const *fxnTablePtr;    /* 指向驱动特定 API 函数表的指针 */

    /*! Pointer to a driver specific data object */
    void *object;                       /* 指向驱动特定数据对象的指针 */

    /*! Pointer to a driver specific hardware attributes structure */
    void const *hwAttrs;                /* 指向驱动特定硬件属性结构体的指针 */
} SPI_Config;

/*!
 *  @brief  Function to close a SPI peripheral specified by the SPI handle
 *  关闭由 SPI 句柄指定的 SPI 外设，释放相关资源。
 *
 *  @pre    SPI_open() has to be called first. 需先调用 SPI_open()。
 *
 *  @param  handle A #SPI_Handle returned from SPI_open()  由 SPI_open() 返回的句柄
 *
 *  @sa     SPI_open()
 */
extern void SPI_close(SPI_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a given
 *          #SPI_Handle.
 *  对已打开的 SPI 句柄执行实现特定的控制命令。
 *
 *  Commands for SPI_control can originate from SPI.h or from implementation
 *  specific SPI*.h (SPIMSPM0.h) files.
 *  While commands from SPI.h are API portable across driver implementations,
 *  not all implementations may support all these commands.
 *  Conversely, commands from driver implementation specific SPI*.h files add
 *  unique driver capabilities but are not API portable across all SPI driver
 *  implementations.
 *
 *  Commands supported by SPI.h follow a SPI_CMD_\<cmd\> naming
 *  convention.<br>
 *  Commands supported by SPI*.h follow a SPI*_CMD_\<cmd\> naming
 *  convention.<br>
 *  Each control command defines @b arg differently. The types of @b arg are
 *  documented with each command.
 *
 *  See @ref SPI_CMD "SPI_control command codes" for command codes.
 *
 *  See @ref SPI_STATUS "SPI_control return status codes" for status codes.
 *
 *  @pre    SPI_open() has to be called first. 需先调用 SPI_open()。
 *
 *  @param  handle      A #SPI_Handle returned from SPI_open()  由 SPI_open() 返回的句柄
 *
 *  @param  cmd         SPI.h or SPI*.h commands.  控制命令码
 *
 *  @param  controlArg  An optional R/W (read/write) command argument
 *                      accompanied with cmd  命令参数指针
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *  @return 实现特定的返回码，负值表示操作失败。
 *
 *  @sa     SPI_open()
 */
extern int_fast16_t SPI_control(SPI_Handle handle, uint_fast16_t cmd, void *controlArg);

/*!
 *  @brief  This function initializes the SPI module.
 *  初始化 SPI 模块，必须在其他 SPI API 之前调用。不会修改外设寄存器。
 *
 *  @pre    The SPI_config[] array must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other SPI driver APIs. This function call does not modify any
 *          peripheral registers.
 */
extern void SPI_init(void);

/*!
 *  @brief  This function opens a given SPI peripheral.
 *  打开指定的 SPI 外设，配置硬件并初始化驱动对象。
 *
 *  @pre    SPI controller has been initialized using SPI_init() 需先调用 SPI_init()。
 *
 *  @param  index         Index of config to use in the *SPI_config* array
 *                        SPI_config 数组中的配置索引
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values. All the fields in this structure are
 *                        RO (read-only).
 *                        参数块指针，为 NULL 时使用默认值。
 *
 *  @return A #SPI_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *  @return 成功返回 SPI 句柄，失败或已打开则返回 NULL。
 *
 *  @sa     SPI_init()
 *  @sa     SPI_close()
 */
extern SPI_Handle SPI_open(uint_least8_t index, SPI_Params *params);

/*!
 *  @brief  Function to initialize the #SPI_Params struct to its defaults
 *  将 SPI_Params 结构体初始化为默认值。
 *
 *  @param  params      An pointer to #SPI_Params structure for
 *                      initialization  指向待初始化的 SPI_Params 结构体指针
 *
 *  Defaults values are:
 *  * SPI_Params.transferMode        = #SPI_MODE_BLOCKING
 *  * SPI_Params.transferTimeout     = #SPI_WAIT_FOREVER
 *  * SPI_Params.transferCallbackFxn = NULL
 *  * SPI_Params.mode                = #SPI_CONTROLLER
 *  * SPI_Params.bitRate             = 500000 (Hz)
 *  * SPI_Params.dataSize            = 8 (bits)
 *  * SPI_Params.frameFormat         = #SPI_MOTO4_POL0_PHA0
 *  * SPI_Params.parity              = false
 *  * SPI_Params.bitOrder            = SPI_BIT_ORDER_MSB_FIRST
 */
extern void SPI_Params_init(SPI_Params *params);

/*!
 *  @brief  Function to perform SPI transactions
 *  执行 SPI 数据传输事务。主机模式下立即开始传输，从机模式下准备接收。
 *
 *  If the SPI is in #SPI_CONTROLLER mode, it will immediately start the
 *  transaction. If the SPI is in #SPI_PERIPHERAL mode, it prepares the driver for
 *  a transaction with a SPI controller device. The device will then wait until
 *  the controller begins the transfer.
 *
 *  In #SPI_MODE_BLOCKING, #SPI_transfer() will block task execution until the
 *  transaction has completed or a timeout has occurred.
 *
 *  In #SPI_MODE_CALLBACK, %SPI_transfer() does not block task execution, but
 *  calls a #SPI_CallbackFxn once the transfer has finished. This makes
 *  %SPI_tranfer() safe to be used within a Task, software or hardware
 *  interrupt context. If queued transactions are supported SPI_Transfer may
 *  be called multiple times to queue multiple transactions. If the driver does
 *  not support this functionality additional calls will return false. Refer to
 *  device specific SPI driver documentation for support information.
 *
 *  From calling #SPI_transfer() until transfer completion, the #SPI_Transaction
 *  structure must stay persistent and must not be altered by application code.
 *  It is also forbidden to modify the content of the #SPI_Transaction.txBuf
 *  during a transaction, even though the physical transfer might not have
 *  started yet. Doing this can result in data corruption. This is especially
 *  important for peripheral operations where SPI_transfer() might be called a long
 *  time before the actual data transfer begins.
 *
 *  @param  handle      A #SPI_Handle  SPI 句柄
 *
 *  @param  transaction A pointer to a #SPI_Transaction. All of the fields within
 *                      transaction except #SPI_Transaction.count and
 *                      #SPI_Transaction.status are WO (write-only) unless
 *                      otherwise noted in the driver implementations. If a
 *                      transaction timeout has occurred, #SPI_Transaction.count
 *                      will contain the number of frames that were transferred.
 *                      Neither is it allowed to modify the transaction object nor
 *                      the content of #SPI_Transaction.txBuf until the transfer
 *                      has completed.
 *                      事务结构体指针。传输完成前禁止修改事务对象和 txBuf 内容。
 *
 *  @return @p true if started successfully; else @p false
 *  @return 成功启动返回 true，否则返回 false。
 *
 *  @sa     #SPI_open
 *  @sa     #SPI_transferCancel
 */
extern bool SPI_transfer(SPI_Handle handle, SPI_Transaction *transaction);

/*!
 *  @brief  Function to cancel SPI transactions
 *  取消正在进行的 SPI 传输事务。阻塞模式下无效果。
 *
 *  In #SPI_MODE_BLOCKING, SPI_transferCancel has no effect.
 *
 *  In #SPI_MODE_CALLBACK, SPI_transferCancel() will stop an SPI transfer if
 *  if one is in progress.
 *  If a transaction was in progress, its callback function will be called
 *  in context from which this API is called from. The #SPI_CallbackFxn
 *  function can determine if the transaction was successful or not by reading
 *  the #SPI_Status status value in the #SPI_Transaction structure.
 *
 *  @pre    SPI_init(), SPI_open(), SPI_transfer() are called
 *
 *  @param  handle      A #SPI_Handle  SPI 句柄
 *
 *  @sa     #SPI_open
 *  @sa     #SPI_transfer
 */
extern void SPI_transferCancel(SPI_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_SPI__include */
/* clang-format on */
/** @}*/
