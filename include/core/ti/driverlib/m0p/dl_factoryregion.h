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
 */
/*!****************************************************************************
 *  @file       dl_factoryregion.h
 *  @brief      Factory Region Driver Library / 工厂区域驱动库
 *  @defgroup   FACTORYREGION Factory Region (FACTORYREGION)
 *
 *  @anchor ti_dl_dl_factoryregion_Overview
 *  # Overview
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup FACTORYREGION
 * @{
 */
#ifndef ti_dl_dl_factoryregion__include
#define ti_dl_dl_factoryregion__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __MSPM0GX51X_TRIM_CACHE__

/*!
 * @brief Macro indicating SRAM trim table has been
          initialized
 *        SRAM Trim 表已初始化标志宏
 *
 * This define is used to indicate that the trim
 * table in SRAM has been initialized with values
 * from TRIM.
 * 此宏用于标识 SRAM 中的 Trim 表已从 TRIM 区加载完毕。
 */
#define DL_FACTORYREGION_TRIM_FLAG_IS_SET (0xCAFECAFE)

    /*! @brief  Data struct for @ref DL_FactoryRegion_initTrimTable.
     *          SRAM Trim 缓存表数据结构
     */
    typedef struct
    {
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G352X)
        /*! Device variant identifier / 设备变体标识 */
        uint32_t USERID;
#else
        /*! Trace identifier / 追踪标识 */
        uint32_t TRACEID;
#endif
        /*! Device identifier / 设备标识 */
        uint32_t DEVICEID;
#if (DeviceFamily_PARENT == DeviceFamily_PARENT_MSPM0G352X)
        /*! Trace identifier / 追踪标识 */
        uint32_t TRACEID;
#else
        /*! Device variant identifier / 设备变体标识 */
        uint32_t USERID;
#endif
        /*! BSL UART pin configuration / BSL UART 引脚配置 */
        uint32_t BSLPIN_UART;
        /*! BSL I2C pin configuration / BSL I2C 引脚配置 */
        uint32_t BSLPIN_I2C;
        /*! BSL pin invocation configuration / BSL 引脚触发配置 */
        uint32_t BSLPIN_INVOKE;
        /*! Main, SRAM, and data bank memory size configuration / 主Flash/SRAM/数据Flash容量配置 */
        uint32_t SRAMFLASH;
        /*! SYSPLL0 4-8MHZ factory configuration / SYSPLL0 4-8MHz 工厂配置 */
        uint32_t PLLSTARTUP0_4_8MHZ;
        /*! SYSPLL1 4-8MHZ factory configuration / SYSPLL1 4-8MHz 工厂配置 */
        uint32_t PLLSTARTUP1_4_8MHZ;
        /*! SYSPLL0 8-16MHZ factory configuration / SYSPLL0 8-16MHz 工厂配置 */
        uint32_t PLLSTARTUP0_8_16MHZ;
        /*! SYSPLL1 8-16MHZ factory configuration / SYSPLL1 8-16MHz 工厂配置 */
        uint32_t PLLSTARTUP1_8_16MHZ;
        /*! SYSPLL0 16-32MHZ factory configuration / SYSPLL0 16-32MHz 工厂配置 */
        uint32_t PLLSTARTUP0_16_32MHZ;
        /*! SYSPLL1 16-32MHZ factory configuration / SYSPLL1 16-32MHz 工厂配置 */
        uint32_t PLLSTARTUP1_16_32MHZ;
        /*! SYSPLL0 32-48MHZ factory configuration / SYSPLL0 32-48MHz 工厂配置 */
        uint32_t PLLSTARTUP0_32_48MHZ;
        /*! SYSPLL1 32-48MHZ factory configuration / SYSPLL1 32-48MHz 工厂配置 */
        uint32_t PLLSTARTUP1_32_48MHZ;
        /*! Temperature sensor room temperature calibration code / 温度传感器室温校准码 */
        uint32_t TEMP_SENSE0;
        /*! SRAM trim table initialization flag / SRAM Trim 表初始化标志 */
        uint32_t DATA_SET;
    } DL_FACTORYREGION_TrimValues;

    /*!
     * @brief Pointer to trim table values in SRAM
     *        SRAM Trim 表指针
     */
    extern DL_FACTORYREGION_TrimValues *FACTORYVALUE;

    /**
     *  @brief  Check to see if TRIM content has been copied
     *          to SRAM.
     *
     *  @return      Status of trim table flag.
     *
     *  @retval      Returns '1' if flag has been set.
     *  @retval      Returns '0' if flag has not been set.
     *
     *  @par 中文说明
     *  检查工厂 Trim 参数是否已从只读工厂区复制到 SRAM 缓存表。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  true 表示 SRAM Trim 表已就绪；false 表示仍需直接读取工厂区。
     */
    __STATIC_INLINE bool DL_FactoryRegion_isTrimTableInSram(void)
    {
        return (FACTORYVALUE->DATA_SET == DL_FACTORYREGION_TRIM_FLAG_IS_SET);
    }
#endif

    /**
     *  @brief   Get the size of the MAIN Flash region
     *
     *  @return  The size of the MAIN Flash region in number of kB.
     *
     *  @par 中文说明
     *  读取主 Flash 容量（单位：kB）。读取期间会临时关闭指令 Cache 以满足访问约束。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  返回主 Flash 容量（kB）。
     */
    __STATIC_INLINE uint16_t DL_FactoryRegion_getMAINFlashSize(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint16_t flashSize;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            flashSize = FACTORYVALUE->SRAMFLASH &
                        FACTORYREGION_SRAMFLASH_MAINFLASH_SZ_MASK;
        }
        else
        {
            flashSize = FACTORYREGION->SRAMFLASH &
                        FACTORYREGION_SRAMFLASH_MAINFLASH_SZ_MASK;
        }
#else
    uint16_t flashSize =
        FACTORYREGION->SRAMFLASH & FACTORYREGION_SRAMFLASH_MAINFLASH_SZ_MASK;
#endif
        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return flashSize;
    }

    /**
     *  @brief   Get the size of the SRAM region
     *
     *  @return  The size of the SRAM region in number of kB.
     *
     *  @par 中文说明
     *  读取 SRAM 容量（单位：kB）。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  返回 SRAM 容量（kB）。
     */
    __STATIC_INLINE uint16_t DL_FactoryRegion_getSRAMFlashSize(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint16_t flashSize;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            flashSize =
                (FACTORYVALUE->SRAMFLASH & FACTORYREGION_SRAMFLASH_SRAM_SZ_MASK) >>
                FACTORYREGION_SRAMFLASH_SRAM_SZ_OFS;
        }
        else
        {
            flashSize = (FACTORYREGION->SRAMFLASH &
                         FACTORYREGION_SRAMFLASH_SRAM_SZ_MASK) >>
                        FACTORYREGION_SRAMFLASH_SRAM_SZ_OFS;
        }
#else
    uint16_t flashSize =
        (FACTORYREGION->SRAMFLASH & FACTORYREGION_SRAMFLASH_SRAM_SZ_MASK) >>
        FACTORYREGION_SRAMFLASH_SRAM_SZ_OFS;
#endif
        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return flashSize;
    }

    /**
     *  @brief   Get the size of the DATA Flash region
     *
     *  @return  The size of the DATA Flash region in number of kB.
     *
     *  @par 中文说明
     *  读取 DATA Flash 容量（单位：kB）。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  返回 DATA Flash 容量（kB）。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getDATAFlashSize(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint16_t flashSize;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            flashSize = (FACTORYVALUE->SRAMFLASH &
                         FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_MASK) >>
                        FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_OFS;
        }
        else
        {
            flashSize = (FACTORYREGION->SRAMFLASH &
                         FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_MASK) >>
                        FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_OFS;
        }
#else
    uint16_t flashSize = (FACTORYREGION->SRAMFLASH &
                          FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_MASK) >>
                         FACTORYREGION_SRAMFLASH_DATAFLASH_SZ_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return flashSize;
    }

    /**
     *  @brief   Get the number of Flash banks on the device
     *
     *  @return  The number of flash banks. A value between [1, 4].
     *
     *  @par 中文说明
     *  获取设备主 Flash 的 Bank 数量。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  返回 Bank 数（范围通常为 1~4）。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getNumBanks(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t numBanks;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            numBanks = ((FACTORYVALUE->SRAMFLASH &
                         FACTORYREGION_SRAMFLASH_MAINNUMBANKS_MASK) >>
                        FACTORYREGION_SRAMFLASH_MAINNUMBANKS_OFS) +
                       (uint8_t)1;
        }
        else
        {
            numBanks = ((FACTORYREGION->SRAMFLASH &
                         FACTORYREGION_SRAMFLASH_MAINNUMBANKS_MASK) >>
                        FACTORYREGION_SRAMFLASH_MAINNUMBANKS_OFS) +
                       (uint8_t)1;
        }
#else
    uint8_t numBanks = ((FACTORYREGION->SRAMFLASH &
                         FACTORYREGION_SRAMFLASH_MAINNUMBANKS_MASK) >>
                        FACTORYREGION_SRAMFLASH_MAINNUMBANKS_OFS) +
                       (uint8_t)1;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return numBanks;
    }

    /**
     *  @brief   Get the trace ID of the device
     *           获取设备的追踪ID
     *
     *  @return  The trace ID of the device.
     *           设备追踪ID。
     */
    __STATIC_INLINE uint32_t DL_FactoryRegion_getTraceID(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint32_t traceID;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            traceID = FACTORYVALUE->TRACEID & FACTORYREGION_TRACEID_DATA_MASK;
        }
        else
        {
            traceID = FACTORYREGION->TRACEID & FACTORYREGION_TRACEID_DATA_MASK;
        }
#else
    uint32_t traceID =
        FACTORYREGION->TRACEID & FACTORYREGION_TRACEID_DATA_MASK;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return traceID;
    }

    /**
     *  @brief   Get JEDEC bank and company code
     *           获取 JEDEC 厂商代码
     *
     *  @return  TI's JEDEC bank and company code
     *           TI 的 JEDEC bank 及公司代码。
     */
    __STATIC_INLINE uint16_t DL_FactoryRegion_getManufacturerCode(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint16_t manufacturerCode;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            manufacturerCode = (FACTORYVALUE->DEVICEID &
                                FACTORYREGION_DEVICEID_MANUFACTURER_MASK) >>
                               FACTORYREGION_DEVICEID_MANUFACTURER_OFS;
        }
        else
        {
            manufacturerCode = (FACTORYREGION->DEVICEID &
                                FACTORYREGION_DEVICEID_MANUFACTURER_MASK) >>
                               FACTORYREGION_DEVICEID_MANUFACTURER_OFS;
        }
#else
    uint16_t manufacturerCode =
        (FACTORYREGION->DEVICEID & FACTORYREGION_DEVICEID_MANUFACTURER_MASK) >>
        FACTORYREGION_DEVICEID_MANUFACTURER_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return manufacturerCode;
    }

    /**
     *  @brief   Get the part number of the device
     *           获取设备部件编号
     *
     *  @return  The part number of the device
     *           设备部件编号。
     */
    __STATIC_INLINE uint16_t DL_FactoryRegion_getPartNumber(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint16_t partNumber;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            partNumber =
                (FACTORYVALUE->DEVICEID & FACTORYREGION_DEVICEID_PARTNUM_MASK) >>
                FACTORYREGION_DEVICEID_PARTNUM_OFS;
        }
        else
        {
            partNumber =
                (FACTORYREGION->DEVICEID & FACTORYREGION_DEVICEID_PARTNUM_MASK) >>
                FACTORYREGION_DEVICEID_PARTNUM_OFS;
        }
#else
    uint16_t partNumber =
        (FACTORYREGION->DEVICEID & FACTORYREGION_DEVICEID_PARTNUM_MASK) >>
        FACTORYREGION_DEVICEID_PARTNUM_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return partNumber;
    }

    /**
     *  @brief   Get the version of the device
     *           获取设备版本号
     *
     *  @return  The version of the device
     *           设备版本号。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getVersion(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t version;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            version =
                (FACTORYVALUE->DEVICEID & FACTORYREGION_DEVICEID_VERSION_MASK) >>
                FACTORYREGION_DEVICEID_VERSION_OFS;
        }
        else
        {
            version =
                (FACTORYREGION->DEVICEID & FACTORYREGION_DEVICEID_VERSION_MASK) >>
                FACTORYREGION_DEVICEID_VERSION_OFS;
        }
#else
    uint8_t version =
        (FACTORYREGION->DEVICEID & FACTORYREGION_DEVICEID_VERSION_MASK) >>
        FACTORYREGION_DEVICEID_VERSION_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return version;
    }

    /**
     *  @brief   Get the bit pattern identifying the part
     *           获取 USERID 中的部件标识位域
     *
     *  @return  The bit pattern identifying the part
     *           部件标识位域值。
     */
    __STATIC_INLINE uint16_t DL_FactoryRegion_getUserIDPart(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint16_t userIDPart;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            userIDPart = (FACTORYVALUE->USERID & FACTORYREGION_USERID_PART_MASK) >>
                         FACTORYREGION_USERID_PART_OFS;
        }
        else
        {
            userIDPart =
                (FACTORYREGION->USERID & FACTORYREGION_USERID_PART_MASK) >>
                FACTORYREGION_USERID_PART_OFS;
        }
#else
    uint16_t userIDPart =
        (FACTORYREGION->USERID & FACTORYREGION_USERID_PART_MASK) >>
        FACTORYREGION_USERID_PART_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return userIDPart;
    }

    /**
     *  @brief   Get the bit pattern identifying a variant of a part
     *           获取 USERID 中的变体标识位域
     *
     *  @return  The bit pattern identifying a variant of a part
     *           变体标识位域值。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getUserIDVariant(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t userIDVariant;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            userIDVariant =
                (FACTORYVALUE->USERID & FACTORYREGION_USERID_VARIANT_MASK) >>
                FACTORYREGION_USERID_VARIANT_OFS;
        }
        else
        {
            userIDVariant =
                (FACTORYREGION->USERID & FACTORYREGION_USERID_VARIANT_MASK) >>
                FACTORYREGION_USERID_VARIANT_OFS;
        }
#else
    uint8_t userIDVariant =
        (FACTORYREGION->USERID & FACTORYREGION_USERID_VARIANT_MASK) >>
        FACTORYREGION_USERID_VARIANT_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return userIDVariant;
    }

    /**
     *  @brief   Get the value that preserves compatibility with lesser minorrev values
     *           获取 USERID 中的次版本号
     *
     *  Monotonic increasing value indicating a new revision that preserves
     *  compatibility with lesser minorrev values. New capability may
     *  be introduced such that lesser minorrev numbers may not be
     *  compatible with greater if the new capability is used
     *
     *  @return  The value that preserves compatibility with lesser minorrev values
     *           次版本号，单调递增，向后兼容。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getUserIDMinorRev(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t minorRev;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            minorRev =
                (FACTORYVALUE->USERID & FACTORYREGION_USERID_MINORREV_MASK) >>
                FACTORYREGION_USERID_MINORREV_OFS;
        }
        else
        {
            minorRev =
                (FACTORYREGION->USERID & FACTORYREGION_USERID_MINORREV_MASK) >>
                FACTORYREGION_USERID_MINORREV_OFS;
        }
#else
    uint8_t minorRev =
        (FACTORYREGION->USERID & FACTORYREGION_USERID_MINORREV_MASK) >>
        FACTORYREGION_USERID_MINORREV_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return minorRev;
    }

    /**
     *  @brief   Get the value that preserves compatibility with lesser majorrev values
     *           获取 USERID 中的主版本号
     *
     *  Monotonic increasing value indicating a new revision significant
     *  enough that users of the device may have to revise PCB or or
     *  software design
     *
     *  @return  The value that tells users they may have to revise PCB or majorrev design
     *           主版本号，变更较大时用户可能需要调整 PCB 或软件设计。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getUserIDMajorRev(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t majorRev;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            majorRev =
                (FACTORYVALUE->USERID & FACTORYREGION_USERID_MAJORREV_MASK) >>
                FACTORYREGION_USERID_MAJORREV_OFS;
        }
        else
        {
            majorRev =
                (FACTORYREGION->USERID & FACTORYREGION_USERID_MAJORREV_MASK) >>
                FACTORYREGION_USERID_MAJORREV_OFS;
        }
#else
    uint8_t majorRev =
        (FACTORYREGION->USERID & FACTORYREGION_USERID_MAJORREV_MASK) >>
        FACTORYREGION_USERID_MAJORREV_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return majorRev;
    }

    /**
     *  @brief   Get the UART RXD PIN used by BSL
     *           获取 BSL 使用的 UART RXD 引脚 Pad 编号
     *
     *  @return  The UART RXD PIN used by BSL
     *           BSL UART RXD 引脚 Pad 编号。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinUARTRXDPad(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslUARTRXDPad;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslUARTRXDPad = (FACTORYVALUE->BSLPIN_UART &
                             FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_MASK) >>
                            FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_OFS;
        }
        else
        {
            bslUARTRXDPad = (FACTORYREGION->BSLPIN_UART &
                             FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_MASK) >>
                            FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_OFS;
        }
#else
    uint8_t bslUARTRXDPad = (FACTORYREGION->BSLPIN_UART &
                             FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_MASK) >>
                            FACTORYREGION_BSLPIN_UART_UART_RXD_PAD_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslUARTRXDPad;
    }

    /**
     *  @brief   Get the UART RXD Pin function selection value used by BSL
     *           获取 BSL UART RXD 引脚功能选择值
     *
     *  @return  The UART RXD Pin function selection value used by BSL
     *           BSL UART RXD 引脚功能选择值。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinUARTRXDFunction(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslUARTRXDFunction;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslUARTRXDFunction = (FACTORYVALUE->BSLPIN_UART &
                                  FACTORYREGION_BSLPIN_UART_UART_RXD_PF_MASK) >>
                                 FACTORYREGION_BSLPIN_UART_UART_RXD_PF_OFS;
        }
        else
        {
            bslUARTRXDFunction = (FACTORYREGION->BSLPIN_UART &
                                  FACTORYREGION_BSLPIN_UART_UART_RXD_PF_MASK) >>
                                 FACTORYREGION_BSLPIN_UART_UART_RXD_PF_OFS;
        }
#else
    uint8_t bslUARTRXDFunction =
        (FACTORYREGION->BSLPIN_UART &
         FACTORYREGION_BSLPIN_UART_UART_RXD_PF_MASK) >>
        FACTORYREGION_BSLPIN_UART_UART_RXD_PF_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslUARTRXDFunction;
    }

    /**
     *  @brief   Get the UART TXD PIN used by BSL
     *           获取 BSL 使用的 UART TXD 引脚 Pad 编号
     *
     *  @return  The UART TXD PIN used by BSL
     *           BSL UART TXD 引脚 Pad 编号。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinUARTTXDPad(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslUARTTXDPad;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslUARTTXDPad = (FACTORYVALUE->BSLPIN_UART &
                             FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_MASK) >>
                            FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_OFS;
        }
        else
        {
            bslUARTTXDPad = (FACTORYREGION->BSLPIN_UART &
                             FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_MASK) >>
                            FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_OFS;
        }
#else
    uint8_t bslUARTTXDPad = (FACTORYREGION->BSLPIN_UART &
                             FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_MASK) >>
                            FACTORYREGION_BSLPIN_UART_UART_TXD_PAD_OFS;
#endif
        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslUARTTXDPad;
    }

    /**
     *  @brief   Get the UART TXD Pin function selection value used by BSL
     *           获取 BSL UART TXD 引脚功能选择值
     *
     *  @return  The UART TXD Pin function selection value used by BSL
     *           BSL UART TXD 引脚功能选择值。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinUARTTXDFunction(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslUARTTXDFunction;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslUARTTXDFunction = (FACTORYVALUE->BSLPIN_UART &
                                  FACTORYREGION_BSLPIN_UART_UART_TXD_PF_MASK) >>
                                 FACTORYREGION_BSLPIN_UART_UART_TXD_PF_OFS;
        }
        else
        {
            bslUARTTXDFunction = (FACTORYREGION->BSLPIN_UART &
                                  FACTORYREGION_BSLPIN_UART_UART_TXD_PF_MASK) >>
                                 FACTORYREGION_BSLPIN_UART_UART_TXD_PF_OFS;
        }
#else
    uint8_t bslUARTTXDFunction =
        (FACTORYREGION->BSLPIN_UART &
         FACTORYREGION_BSLPIN_UART_UART_TXD_PF_MASK) >>
        FACTORYREGION_BSLPIN_UART_UART_TXD_PF_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslUARTTXDFunction;
    }

    /**
     *  @brief   Get the I2C SDA Pin used by BSL
     *           获取 BSL 使用的 I2C SDA 引脚 Pad 编号
     *
     *  @return  The I2C SDA Pin used by BSL
     *           BSL I2C SDA 引脚 Pad 编号。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinI2CSDAPad(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslI2CSDAPad;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslI2CSDAPad = (FACTORYVALUE->BSLPIN_I2C &
                            FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_MASK) >>
                           FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_OFS;
        }
        else
        {
            bslI2CSDAPad = (FACTORYREGION->BSLPIN_I2C &
                            FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_MASK) >>
                           FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_OFS;
        }
#else
    uint8_t bslI2CSDAPad = (FACTORYREGION->BSLPIN_I2C &
                            FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_MASK) >>
                           FACTORYREGION_BSLPIN_I2C_I2C_SDA_PAD_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslI2CSDAPad;
    }

    /**
     *  @brief   Get the I2C SDA Pin function selection value used by BSL
     *           获取 BSL I2C SDA 引脚功能选择值
     *
     *  @return  The I2C SDA Pin function selection value used by BSL
     *           BSL I2C SDA 引脚功能选择值。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinI2CSDAFunction(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslI2CSDAFunction;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslI2CSDAFunction = (FACTORYVALUE->BSLPIN_I2C &
                                 FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_MASK) >>
                                FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_OFS;
        }
        else
        {
            bslI2CSDAFunction = (FACTORYREGION->BSLPIN_I2C &
                                 FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_MASK) >>
                                FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_OFS;
        }
#else
    uint8_t bslI2CSDAFunction =
        (FACTORYREGION->BSLPIN_I2C &
         FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_MASK) >>
        FACTORYREGION_BSLPIN_I2C_I2C_SDA_PF_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslI2CSDAFunction;
    }

    /**
     *  @brief   Get the I2C SCL Pin used by BSL
     *           获取 BSL 使用的 I2C SCL 引脚 Pad 编号
     *
     *  @return  The I2C SCL Pin used by BSL
     *           BSL I2C SCL 引脚 Pad 编号。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinI2CSCLPad(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslI2CSCLPad;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslI2CSCLPad = (FACTORYVALUE->BSLPIN_I2C &
                            FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_MASK) >>
                           FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_OFS;
        }
        else
        {
            bslI2CSCLPad = (FACTORYREGION->BSLPIN_I2C &
                            FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_MASK) >>
                           FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_OFS;
        }
#else
    uint8_t bslI2CSCLPad = (FACTORYREGION->BSLPIN_I2C &
                            FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_MASK) >>
                           FACTORYREGION_BSLPIN_I2C_I2C_SCL_PAD_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslI2CSCLPad;
    }

    /**
     *  @brief   Get the I2C SCL Pin function selection value used by BSL
     *           获取 BSL I2C SCL 引脚功能选择值
     *
     *  @return  The I2C SCL Pin function selection value used by BSL
     *           BSL I2C SCL 引脚功能选择值。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinI2CSCLFunction(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslI2CSCLFunction;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslI2CSCLFunction = (FACTORYVALUE->BSLPIN_I2C &
                                 FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_MASK) >>
                                FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_OFS;
        }
        else
        {
            bslI2CSCLFunction = (FACTORYREGION->BSLPIN_I2C &
                                 FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_MASK) >>
                                FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_OFS;
        }
#else
    uint8_t bslI2CSCLFunction =
        (FACTORYREGION->BSLPIN_I2C &
         FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_MASK) >>
        FACTORYREGION_BSLPIN_I2C_I2C_SCL_PF_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslI2CSCLFunction;
    }

    /**
     *  @brief   Get the GPIO level used by BSL pin invocation
     *           获取 BSL 引脚触发时的 GPIO 电平
     *
     *  @return  The GPIO level used by BSL pin invocation
     *           BSL 引脚触发电平（0 或 1）。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinInvokeGPIOLevel(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslInvokeGPIOLevel;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslInvokeGPIOLevel =
                (FACTORYVALUE->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_OFS;
        }
        else
        {
            bslInvokeGPIOLevel =
                (FACTORYREGION->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_OFS;
        }
#else
    uint8_t bslInvokeGPIOLevel =
        (FACTORYREGION->BSLPIN_INVOKE &
         FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_MASK) >>
        FACTORYREGION_BSLPIN_INVOKE_GPIO_LEVEL_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslInvokeGPIOLevel;
    }

    /**
     *  @brief   Get the GPIO Pin selection value used by BSL pin invocation
     *           获取 BSL 引脚触发的 GPIO 引脚选择值
     *
     *  @return  The GPIO Pin selection value used by BSL pin invocation
     *           BSL 引脚触发的 GPIO 引脚选择值。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinInvokeGPIOPin(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslInvokeGPIOPin;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslInvokeGPIOPin =
                (FACTORYVALUE->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_OFS;
        }
        else
        {
            bslInvokeGPIOPin =
                (FACTORYREGION->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_OFS;
        }
#else
    uint8_t bslInvokeGPIOPin =
        (FACTORYREGION->BSLPIN_INVOKE &
         FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_MASK) >>
        FACTORYREGION_BSLPIN_INVOKE_GPIO_PIN_SEL_OFS;
#endif
        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslInvokeGPIOPin;
    }

    /**
     *  @brief   Get the GPIO module selection used by BSL pin invocation
     *           获取 BSL 引脚触发的 GPIO 模块选择
     *
     *  @return  The GPIO module selection used by BSL pin invocation
     *           BSL 引脚触发的 GPIO 模块选择。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinInvokeGPIOModule(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslInvokeGPIOModule;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslInvokeGPIOModule =
                (FACTORYVALUE->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_OFS;
        }
        else
        {
            bslInvokeGPIOModule =
                (FACTORYREGION->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_OFS;
        }
#else
    uint8_t bslInvokeGPIOModule =
        (FACTORYREGION->BSLPIN_INVOKE &
         FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_MASK) >>
        FACTORYREGION_BSLPIN_INVOKE_GPIO_REG_SEL_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslInvokeGPIOModule;
    }

    /**
     *  @brief   Get the BSL invocation pin number
     *           获取 BSL 引脚触发的 Pad 编号
     *
     *  @return  The BSL invocation pin number
     *           BSL 引脚触发的 Pad 编号。
     */
    __STATIC_INLINE uint8_t DL_FactoryRegion_getBSLPinInvokeGPIOModulePad(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint8_t bslInvokeGPIOModulePad;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            bslInvokeGPIOModulePad =
                (FACTORYVALUE->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_OFS;
        }
        else
        {
            bslInvokeGPIOModulePad =
                (FACTORYREGION->BSLPIN_INVOKE &
                 FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_MASK) >>
                FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_OFS;
        }
#else
    uint8_t bslInvokeGPIOModulePad =
        (FACTORYREGION->BSLPIN_INVOKE &
         FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_MASK) >>
        FACTORYREGION_BSLPIN_INVOKE_BSL_PAD_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return bslInvokeGPIOModulePad;
    }

    /**
     *  @brief   Get the ADC conversion results of temperature sensor output voltage
     *           获取温度传感器输出电压的 ADC 转换结果
     *
     * Returns the temperature sensor output voltage at the factory trim temperature
     * in ADC result code format. This ADC result code is based upon 12-bit
     * sampling mode together with the 1.4-V internal voltage reference.
     *
     *  @return  The ADC conversion results of temperature sensor output voltage
     *           温度传感器在工厂校准温度下的 ADC 码值（12位，1.4V 内部参考）。
     */
    __STATIC_INLINE uint32_t DL_FactoryRegion_getTemperatureVoltage(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

#ifdef __MSPM0GX51X_TRIM_CACHE__
        uint32_t tempVoltage;
        if (DL_FactoryRegion_isTrimTableInSram())
        {
            tempVoltage = (FACTORYVALUE->TEMP_SENSE0 &
                           FACTORYREGION_TEMP_SENSE0_DATA_MASK) >>
                          FACTORYREGION_TEMP_SENSE0_DATA_OFS;
        }
        else
        {
            tempVoltage = (FACTORYREGION->TEMP_SENSE0 &
                           FACTORYREGION_TEMP_SENSE0_DATA_MASK) >>
                          FACTORYREGION_TEMP_SENSE0_DATA_OFS;
        }
#else
    uint32_t tempVoltage =
        (FACTORYREGION->TEMP_SENSE0 & FACTORYREGION_TEMP_SENSE0_DATA_MASK) >>
        FACTORYREGION_TEMP_SENSE0_DATA_OFS;
#endif

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return tempVoltage;
    }

    /**
     *  @brief   Get the BOOTCRC value
     *           获取 BOOT CRC 值
     *
     *  The 32-bit CRC of all locations in OPEN including reserved locations.
     *
     *  @return  The BOOTCRC value
     *           OPEN 区全部地址（含保留）的 32 位 CRC 值。
     */
    __STATIC_INLINE uint32_t DL_FactoryRegion_getBOOTCRCData(void)
    {
        /* Save CPUSS state and then disable the cache before TRIM access */
        uint32_t ctlTemp = DL_CORE_getInstructionConfig();
        DL_CORE_configInstruction(DL_CORE_PREFETCH_ENABLED, DL_CORE_CACHE_DISABLED,
                                  DL_CORE_LITERAL_CACHE_ENABLED);

        uint32_t BOOTCRCData =
            (FACTORYREGION->BOOTCRC & FACTORYREGION_BOOTCRC_DATA_MASK) >>
            FACTORYREGION_BOOTCRC_DATA_OFS;

        /* Restore CPUSS state */
        CPUSS->CTL = ctlTemp;

        return BOOTCRCData;
    }

#ifdef __MSPM0GX51X_TRIM_CACHE__
    /**
     *  @brief Initializes the struct with values from TRIM
     *
     *  Workaround for TRIM region becoming unreadable upon
     *  setting flash wait states equal to 2 on MSPM0GX51X
     *  devices.
     *
     *  @par 中文说明
     *  将 TRIM 区关键校准值复制到 SRAM 结构体，规避特定器件在高等待周期下 TRIM 不可读问题。
     *
     *  @par 中文参数
     *  无。
     *
     *  @par 中文返回值
     *  无返回值。
     *
     */
    void DL_FactoryRegion_initTrimTable(void);
#endif

#ifdef __MSPM0C110X_ADC_ERR_06__
    /**
     *  @brief Get the ADC offset value
     *         获取 ADC 偏移值
     *
     *  Workaround for errata on MSPM0C110x devices: ADC_ERR_06
     *  0x41C40040 is expected to point to factory region reserved0 bit
     *
     *  @return  ADC offset value
     *           ADC 偏移校准值（有符号浮点）。
     */
    __STATIC_INLINE float DL_FactoryRegion_getADCOffset(void)
    {
        return ((float)(*(int16_t *)(0x41C40040)));
    }
#endif

#ifdef __cplusplus
}
#endif

#endif /* ti_dl_dl_factoryregion__include */
/** @}*/
