/*
 * Copyright (c) 2016-2023, Texas Instruments Incorporated
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
 *  @file       ADC.h
 *
 *  @brief      Analog to Digital Conversion (ADC) Input Driver
 *
 *  @defgroup   ADC Analog to Digital Conversion (ADC)
 *
 *  @anchor ti_drivers_ADC_Overview
 *  # Overview
 *
 *  The ADC driver allows you to manage an Analog to Digital peripheral via
 *  simple and portable APIs. This driver supports sampling and converting
 *  raw values into microvolts.
 *
 *  <hr>
 *  @anchor ti_drivers_ADC_Usage
 *  # Usage
 *
 *  This documentation provides a basic @ref ti_drivers_ADC_Synopsis
 *  "usage summary" and a set of @ref ti_drivers_ADC_Examples "examples"
 *  in the form of commented code fragments. Detailed descriptions of the
 *  APIs are provided in subsequent sections.
 *
 *  @anchor ti_drivers_ADC_Synopsis
 *  ## Synopsis
 *  @anchor ti_drivers_ADC_Synopsis_Code
 *  @code
 *  // Import ADC Driver definitions
 *  #include <ti/drivers/ADC.h>
 *
 *  // Define name for ADC channel index
 *  #define THERMOCOUPLE_OUT  0
 *
 *  // One-time init of ADC driver
 *  ADC_init();
 *
 *  // initialize optional ADC parameters
 *  ADC_Params params;
 *  ADC_Params_init(&params);
 *  params.isProtected = true;
 *
 *  // Open ADC channels for usage
 *  ADC_Handle adcHandle = ADC_open(THERMOCOUPLE_OUT, &params);
 *
 *  // Sample the analog output from the Thermocouple
 *  ADC_convert(adcHandle, &result);
 *
 *  // Convert the sample to microvolts
 *  resultUv = ADC_convertToMicroVolts(adcHandle, result);
 *
 *  ADC_close(adcHandle);
 *  @endcode
 *
 *  <hr>
 *  @anchor ti_drivers_ADC_Examples
 *  # Examples
 *
 *  @li @ref ti_drivers_ADC_Examples_open "Opening an ADC instance"
 *  @li @ref ti_drivers_ADC_Examples_convert "Taking an ADC sample"
 *  @li @ref ti_drivers_ADC_Examples_convert_microvolts "Converting a sample to microvolts"
 *  @li @ref ti_drivers_ADC_Examples_convert_chain "Executing multi-channel sampling"
 *
 *  @anchor ti_drivers_ADC_Examples_open
 *  ## Opening an ADC instance
 *
 *  @code
 *  ADC_Handle adc;
 *  ADC_Params params;
 *
 *  ADC_Params_init(&params);
 *
 *  adc = ADC_open(0, &params);
 *  if (adc == NULL) {
 *      // ADC_open() failed
 *      while (1) {}
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_ADC_Examples_convert
 *  ## Taking an ADC sample
 *
 *  An ADC conversion with an ADC peripheral is started by calling
 *  ADC_convert(). The result value is returned by ADC_convert()
 *  once the conversion is finished.
 *
 *  @code
 *  int_fast16_t res;
 *  uint_fast16_t adcValue;
 *
 *  res = ADC_convert(adc, &adcValue);
 *  if (res == ADC_STATUS_SUCCESS)
 *  {
 *      print(adcValue);
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_ADC_Examples_convert_microvolts
 *  ## Converting a sample to microvolts
 *
 *  The result value returned by ADC_convert() is a raw value. The
 *  following uses ADC_convertToMicroVolts() to convert the raw value
 *  into microvolts.
 *  @code
 *  int_fast16_t res;
 *  uint_fast16_t adcValue;
 *  uint32_t adcValueUv;
 *
 *  res = ADC_convert(adc, &adcValue);
 *  if (res == ADC_STATUS_SUCCESS)
 *  {
 *      adcValueUv = ADC_convertToMicroVolts(adc, adcValue);
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_ADC_Examples_convert_chain
 *  ## Executing multi-channel sampling
 *
 *  ADC_convertChain() provides an optimized way to perform
 *  ADC conversions for several ADC instances.
 *  It takes a list of identically configured ADC instances and returns
 *  a buffer with the corresponding result values once the conversion
 *  is finished. One typical use-case would be reading a group of sensors
 *  that share the sampling duration.
 *
 *  Should the configuration of the ADC instances differ, the configuration
 *  of the first instance in the list is used to set the parameters of the
 *  entire conversion chain.
 *  @code
 *  ADC_Handle   adc[ADC_COUNT];
 *  int_fast16_t res;
 *  uint16_t     retValues[ADC_COUNT];
 *  uint8_t      i;
 *
 *  res = ADC_convertChain(adc, retValues, ADC_COUNT);
 *  if (res == ADC_STATUS_SUCCESS)
 *  {
 *      for (i = 0; i < ADC_COUNT; i++) {
 *          print(retValues[i]);
 *      }
 *  }
 *  @endcode
 *
 *  <hr>
 ******************************************************************************
 */
/**
 * @brief ADC 模数转换驱动头文件
 *        提供 ADC 外设的打开、关闭、单通道/多通道采样及原始值转微伏等公共 API。
 */
/** @addtogroup ADC
 * @{
 */
/* clang-format off */

#ifndef ti_drivers_ADC__include
#define ti_drivers_ADC__include

#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/dpl/SemaphoreP.h>

#ifdef __cplusplus
extern "C" {
#endif

 /*!
  *  @deprecated ADC_convertRawToMicroVolts() is succeeded by
  *  ADC_convertToMicroVolts().
  *  已废弃：请使用 ADC_convertToMicroVolts() 替代。
  */
 #define ADC_convertRawToMicroVolts ADC_convertToMicroVolts

/*!
 *  @defgroup ADC_CONTROL ADC_control command and status codes
 *  These ADC macros are reservations for ADC.h
 *  @{
 */

/*!
 * @hideinitializer
 * Common ADC_control command code reservation offset.
 * ADC driver implementations should offset command codes with ADC_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define ADCXYZ_CMD_COMMAND0      ADC_CMD_RESERVED + 0
 * #define ADCXYZ_CMD_COMMAND1      ADC_CMD_RESERVED + 1
 * @endcode
 * ADC_control() 命令码预留偏移量，具体驱动的命令码从此值开始递增。
 */
#define ADC_CMD_RESERVED (32)

/*!
 * @hideinitializer
 * Common ADC_control status code reservation offset.
 * ADC driver implementations should offset status codes with
 * ADC_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define ADCXYZ_STATUS_ERROR0     ADC_STATUS_RESERVED - 0
 * #define ADCXYZ_STATUS_ERROR1     ADC_STATUS_RESERVED - 1
 * #define ADCXYZ_STATUS_ERROR2     ADC_STATUS_RESERVED - 2
 * @endcode
 * ADC_control() 状态码预留偏移量，具体驱动的状态码从此值开始递减。
 */
#define ADC_STATUS_RESERVED (-32)

/*!
 * @brief   Successful status code returned by ADC_control().
 *          ADC_control() 操作成功时返回此状态码。
 *
 * ADC_control() returns ADC_STATUS_SUCCESS if the control code was executed
 * successfully.
 *  @{
 *  @ingroup ADC_CONTROL
 */
#define ADC_STATUS_SUCCESS (0)

/*!
 * @brief   Generic error status code returned by ADC_control().
 *          ADC_control() 操作失败时返回此通用错误状态码。
 *
 * ADC_control() returns ADC_STATUS_ERROR if the control code was not executed
 * successfully.
 */
#define ADC_STATUS_ERROR (-1)

/*!
 * @brief   An error status code returned by ADC_control() for undefined
 * command codes.
 *          当命令码未被驱动识别时，ADC_control() 返回此状态码。
 *
 * ADC_control() returns ADC_STATUS_UNDEFINEDCMD if the control code is not
 * recognized by the driver implementation.
 */
#define ADC_STATUS_UNDEFINEDCMD (-2)
/** @}*/

/**
 *  @defgroup ADC_CMD Command Codes
 *  ADC_CMD_* macros are general command codes for ADC_control(). Not all ADC
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup ADC_CONTROL
 */

/* Add ADC_CMD_<commands> here */

/** @}*/

/** @}*/

/*!
 *  @brief      A handle that is returned from an ADC_open() call.
 *              ADC_open() 返回的 ADC 句柄类型。
 */
typedef struct ADC_Config_ *ADC_Handle;

/*!
 *  @brief  ADC Parameters used with ADC_open().
 *          ADC_open() 使用的参数结构体。
 *
 *  ADC_Params_init() must be called prior to setting fields in
 *  this structure.  设置字段前必须先调用 ADC_Params_init() 初始化。
 *
 *  @sa     ADC_Params_init()
 */
typedef struct
{
    void *custom;     /*!< Custom argument used by driver
                          implementation
                          驱动实现自定义参数 */
    bool isProtected; /*!< By default ADC uses a semaphore
                          to guarantee thread safety. Setting
                          this parameter to 'false' will eliminate
                          the usage of a semaphore for thread
                          safety. The user is then responsible
                          for ensuring that parallel invocations
                          of ADC_convert() are thread safe.
                          默认使用信号量保证线程安全，设为 false
                          可禁用信号量，此时用户需自行保证
                          ADC_convert() 并发调用的线程安全。 */
} ADC_Params;


/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_close(). ADC_close() 的驱动实现函数指针。
 */
typedef void (*ADC_CloseFxn)(ADC_Handle handle);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_control(). ADC_control() 的驱动实现函数指针。
 */
typedef int_fast16_t (*ADC_ControlFxn)(ADC_Handle handle, uint_fast16_t cmd, void *arg);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_ConvertFxn(). ADC_convert() 的驱动实现函数指针。
 */
typedef int_fast16_t (*ADC_ConvertFxn)(ADC_Handle handle, uint16_t *value);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_ConvertChainFxn(). ADC_convertChain() 的驱动实现函数指针。
 */
typedef int_fast16_t (*ADC_ConvertChainFxn)(ADC_Handle *handleList, uint16_t *dataBuffer, uint8_t channelCount);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_convertToMicroVolts(). ADC_convertToMicroVolts() 的驱动实现函数指针。
 */
typedef uint32_t (*ADC_ConvertToMicroVoltsFxn)(ADC_Handle handle, uint16_t adcValue);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_init(). ADC_init() 的驱动实现函数指针。
 */
typedef void (*ADC_InitFxn)(ADC_Handle handle);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              ADC_open(). ADC_open() 的驱动实现函数指针。
 */
typedef ADC_Handle (*ADC_OpenFxn)(ADC_Handle handle, ADC_Params *params);

/*!
 *  @brief      The definition of an ADC function table that contains the
 *              required set of functions to control a specific ADC driver
 *              implementation.
 *              ADC 函数表，包含控制特定 ADC 驱动实现所需的全部函数指针。
 */
typedef struct
{
    /*! Function to close the specified peripheral
     *  关闭指定外设的函数 */
    ADC_CloseFxn closeFxn;

    /*! Function to perform implementation specific features
     *  执行驱动特定控制命令的函数 */
    ADC_ControlFxn controlFxn;

    /*! Function to initiate an ADC single channel conversion
     *  启动 ADC 单通道转换的函数 */
    ADC_ConvertFxn convertFxn;

    /*! Function to initiate an ADC multi-channel conversion
     *  启动 ADC 多通道转换的函数 */
    ADC_ConvertChainFxn convertChainFxn;

    /*! Function to convert ADC result to microvolts
     *  将 ADC 结果转换为微伏的函数 */
    ADC_ConvertToMicroVoltsFxn convertToMicroVolts;

    /*! Function to initialize the given data object
     *  初始化驱动数据对象的函数 */
    ADC_InitFxn initFxn;

    /*! Function to open the specified peripheral
     *  打开指定外设的函数 */
    ADC_OpenFxn openFxn;
} ADC_FxnTable;

/*! ADC function table pointer ADC 函数表实例 */
extern const ADC_FxnTable ADCMSPM0_fxnTable;

/*!
 *  @brief ADC driver's custom @ref ADC_Config "configuration"
 *  structure.
 *         ADC 驱动配置结构体，包含函数表、数据对象和硬件属性。
 *
 *  @sa     ADC_init()
 *  @sa     ADC_open()
 */
typedef struct ADC_Config_
{
    /*! Pointer to a @ref ADCMSPM0_fxnTable "function pointer table"
     *  with driver-specific implementations of ADC APIs
     *  指向 ADC API 驱动实现函数表的指针 */
    ADC_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific @p ADCMSPM0_Object "data object".
     *  指向驱动数据对象的指针 */
    void *object;

    /*! Pointer to a driver specific @p ADCMSPM0_HWAttrs
     *  "hardware attributes structure".
     *  指向硬件属性结构体的指针 */
    void const *hwAttrs;
} ADC_Config;

/*!
 *  @brief  Function to close an ADC driver instance
 *          关闭 ADC 驱动实例，释放相关资源。
 *
 *  @pre        ADC_open() has to be called first.
 *              必须先调用 ADC_open()。
 *
 *  @param[in]  handle An #ADC_Handle returned from ADC_open()
 *              ADC_open() 返回的句柄。
 */
extern void ADC_close(ADC_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a
 *          driver instance.
 *          对 ADC 驱动实例执行特定控制命令。
 *
 *  @pre    ADC_open() has to be called first.
 *          必须先调用 ADC_open()。
 *
 *  @param[in]  handle   An #ADC_Handle returned from ADC_open()
 *                       ADC_open() 返回的句柄。
 *
 *  @param[in]  cmd     A command value defined by the device specific
 *                      implementation
 *                      设备特定实现定义的命令值。
 *
 *  @param[in]  arg     An optional R/W (read/write) argument that is
 *                      accompanied with @p cmd
 *                      与 cmd 配合使用的可选读写参数。
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *          实现特定的返回码，负值表示操作失败。
 *
 *  @retval #ADC_STATUS_SUCCESS The call was successful. 调用成功。
 *  @retval #ADC_STATUS_UNDEFINEDCMD The @p cmd value is not supported by
 *                                   the device specific implementation.
 *                                   命令码不被驱动识别。
 */
extern int_fast16_t ADC_control(ADC_Handle handle, uint_fast16_t cmd, void *arg);

/*!
 *  @brief  Function to perform an ADC conversion
 *          执行单通道 ADC 采样转换。
 *
 *  Function to perform a single channel sample conversion.
 *
 *  @pre    ADC_open() has been called
 *          必须先调用 ADC_open()。
 *
 *  @param[in]      handle    An #ADC_Handle returned from ADC_open()
 *                            ADC_open() 返回的句柄。
 *  @param[in,out]  value     A pointer to a uint16_t to store the conversion
 *                            result
 *                            存放转换结果的指针。
 *
 *  @retval #ADC_STATUS_SUCCESS  The conversion was successful. 转换成功。
 *  @retval #ADC_STATUS_ERROR    The conversion failed and @p value is
 *                               invalid. 转换失败，结果无效。
 *
 *  @sa     ADC_convertToMicroVolts()
 */
extern int_fast16_t ADC_convert(ADC_Handle handle, uint16_t *value);

/*!
 *  @brief  Function to perform a multi-channel ADC conversion
 *          执行多通道 ADC 采样转换。
 *
 *  Function to perform a multi-channel sample conversion.
 *
 *  @pre    ADC_open() has been called
 *          必须先调用 ADC_open()。
 *
 *  @param[in]      handleList    A list of #ADC_Handle which have returned
 *                                from ADC_open()
 *                                ADC_open() 返回的句柄列表。
 *  @param[in,out]  dataBuffer    A pointer to a uint16_t data buffer to store
 *                                the conversion result
 *                                存放转换结果的数据缓冲区指针。
 *  @param[in]      channelCount  The number of channels that make up the list
 *                                of #ADC_Handle
 *                                通道数量。
 *
 *  @retval #ADC_STATUS_SUCCESS  The conversion was successful. 转换成功。
 *  @retval #ADC_STATUS_ERROR    The conversion failed and @p value is
 *                               invalid. 转换失败，结果无效。
 *
 *  @sa     ADC_convert()
 */
extern int_fast16_t ADC_convertChain(ADC_Handle *handleList, uint16_t *dataBuffer, uint8_t channelCount);

/*!
 *  @brief  Function to convert a raw ADC sample into microvolts.
 *          将 ADC 原始采样值转换为微伏值。
 *
 *  @pre    ADC_convert() has to be called first.
 *          必须先调用 ADC_convert()。
 *
 *  @param[in]  handle      An #ADC_Handle returned from ADC_open()
 *                          ADC_open() 返回的句柄。
 *
 *  @param[in]  adcValue    A sampling result return from ADC_convert()
 *                          ADC_convert() 返回的原始采样值。
 *
 *  @return @p adcValue converted into microvolts
 *          转换后的微伏值。
 *
 *  @sa     ADC_convert()
 */
extern uint32_t ADC_convertToMicroVolts(ADC_Handle handle, uint16_t adcValue);

/*!
 *  @brief  Function to initialize the ADC driver.
 *          初始化 ADC 驱动，必须在其他 ADC API 之前调用。
 *
 *  This function must also be called before any other ADC driver APIs.
 */
extern void ADC_init(void);

/*!
 *  @brief  Function to initialize the ADC peripheral
 *          打开并初始化指定索引的 ADC 外设。
 *
 *  Function to initialize the ADC peripheral specified by the
 *  particular index value.
 *
 *  @pre    ADC_init() has been called
 *          必须先调用 ADC_init()。
 *
 *  @param[in]  index     Index in the @p ADC_Config[] array.
 *                        ADC_Config[] 数组中的索引。
 *  @param[in]  params    Pointer to an initialized #ADC_Params structure.
 *                        If NULL, the default #ADC_Params values are used.
 *                        已初始化的 ADC_Params 结构体指针，NULL 则使用默认值。
 *
 *  @return An #ADC_Handle on success or NULL on error.
 *          成功返回句柄，失败返回 NULL。
 *
 *  @sa     ADC_init()
 *  @sa     ADC_close()
 */
extern ADC_Handle ADC_open(uint_least8_t index, ADC_Params *params);

/*!
 *  @brief  Initialize an #ADC_Params structure to its default values.
 *          将 ADC_Params 结构体初始化为默认值。
 *
 *  @param[in]  params  A pointer to an #ADC_Params structure.
 *                      ADC_Params 结构体指针。
 *
 *  Default values are:
 *  @arg #ADC_Params.custom = NULL
 *  @arg #ADC_Params.isProtected = true
 */
extern void ADC_Params_init(ADC_Params *params);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ADC__include */
/* clang-format on */
/** @}*/
