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
/**
 * @file dl_mcan.c
 * @brief MCAN (CAN-FD) 驱动库实现文件。
 *        包含 MCAN 初始化、消息收发、过滤器配置、中断管理、
 *        ECC 错误处理、低功耗备份恢复等全部接口实现。
 */

#include <ti/driverlib/dl_mcan.h>

#ifdef __MSPM0_HAS_MCAN__

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief  MCAN MSG RAM BANK number for ECC AGGR.
 *         ECC AGGR 使用的消息 RAM BANK 编号
 */
#define MCANSS_MSG_RAM_NUM (0U)

/**
 * \brief  Maximum Number of Rx Buffers.
 *         接收缓冲区最大数量
 */
#define MCANSS_RX_BUFFER_MAX (64U)

/**
 * \brief  Maximum Number of Tx Buffers.
 *         发送缓冲区最大数量
 */
#define MCANSS_TX_BUFFER_MAX (32U)

/**
 * \brief  Macro for standard Message ID filter.
 *         标准 ID 过滤器元素字大小
 */
#define MCANSS_STD_ID_FILTER_SIZE_WORDS (1U)

/**
 * \brief  Macro for extended Message ID filter.
 *         扩展 ID 过滤器元素字大小
 */
#define MCANSS_EXT_ID_FILTER_SIZE_WORDS (2U)

/**
 * \brief  Macro for Tx Event FIFO element size.
 *         发送事件 FIFO 元素字大小
 */
#define MCANSS_TX_EVENT_FIFO_SIZE_WORDS (2U)

/**
 * \brief  Macro for Interrupt Line enable mask.
 *         中断线使能掩码
 */
#define MCANSS_INTR_LINE_EN_MASK ((MCAN_ILE_EINT0_MASK | MCAN_ILE_EINT1_MASK))

/**
 * \brief  Mask and shift for Tx Buffers elements.
 *         发送缓冲区元素各字段掩码与移位
 */
#define MCANSS_TX_BUFFER_ELEM_ID_SHIFT (0U)
#define MCANSS_TX_BUFFER_ELEM_ID_MASK (0x1FFFFFFFU)
#define MCANSS_TX_BUFFER_ELEM_RTR_SHIFT (29U)
#define MCANSS_TX_BUFFER_ELEM_RTR_MASK (0x20000000U)
#define MCANSS_TX_BUFFER_ELEM_XTD_SHIFT (30U)
#define MCANSS_TX_BUFFER_ELEM_XTD_MASK (0x40000000U)
#define MCANSS_TX_BUFFER_ELEM_ESI_SHIFT (31U)
#define MCANSS_TX_BUFFER_ELEM_ESI_MASK (0x80000000U)
#define MCANSS_TX_BUFFER_ELEM_DLC_SHIFT (16U)
#define MCANSS_TX_BUFFER_ELEM_DLC_MASK (0x000F0000U)
#define MCANSS_TX_BUFFER_ELEM_BRS_SHIFT (20U)
#define MCANSS_TX_BUFFER_ELEM_BRS_MASK (0x00100000U)
#define MCANSS_TX_BUFFER_ELEM_FDF_SHIFT (21U)
#define MCANSS_TX_BUFFER_ELEM_FDF_MASK (0x00200000U)
#define MCANSS_TX_BUFFER_ELEM_EFC_SHIFT (23U)
#define MCANSS_TX_BUFFER_ELEM_EFC_MASK (0x00800000U)
#define MCANSS_TX_BUFFER_ELEM_MM_SHIFT (24U)
#define MCANSS_TX_BUFFER_ELEM_MM_MASK (0xFF000000U)

/**
 * \brief  Mask and shift for Rx Buffers elements.
 *         接收缓冲区元素各字段掩码与移位
 */
#define MCANSS_RX_BUFFER_ELEM_ID_SHIFT (0U)
#define MCANSS_RX_BUFFER_ELEM_ID_MASK (0x1FFFFFFFU)
#define MCANSS_RX_BUFFER_ELEM_RTR_SHIFT (29U)
#define MCANSS_RX_BUFFER_ELEM_RTR_MASK (0x20000000U)
#define MCANSS_RX_BUFFER_ELEM_XTD_SHIFT (30U)
#define MCANSS_RX_BUFFER_ELEM_XTD_MASK (0x40000000U)
#define MCANSS_RX_BUFFER_ELEM_ESI_SHIFT (31U)
#define MCANSS_RX_BUFFER_ELEM_ESI_MASK (0x80000000U)
#define MCANSS_RX_BUFFER_ELEM_RXTS_SHIFT (0U)
#define MCANSS_RX_BUFFER_ELEM_RXTS_MASK (0x0000FFFFU)
#define MCANSS_RX_BUFFER_ELEM_DLC_SHIFT (16U)
#define MCANSS_RX_BUFFER_ELEM_DLC_MASK (0x000F0000U)
#define MCANSS_RX_BUFFER_ELEM_BRS_SHIFT (20U)
#define MCANSS_RX_BUFFER_ELEM_BRS_MASK (0x00100000U)
#define MCANSS_RX_BUFFER_ELEM_FDF_SHIFT (21U)
#define MCANSS_RX_BUFFER_ELEM_FDF_MASK (0x00200000U)
#define MCANSS_RX_BUFFER_ELEM_FIDX_SHIFT (24U)
#define MCANSS_RX_BUFFER_ELEM_FIDX_MASK (0x7F000000U)
#define MCANSS_RX_BUFFER_ELEM_ANMF_SHIFT (31U)
#define MCANSS_RX_BUFFER_ELEM_ANMF_MASK (0x80000000U)

/**
 * \brief  Mask and shift for Standard Message ID Filter Elements.
 *         标准 ID 过滤器元素各字段掩码与移位
 */
#define MCANSS_STD_ID_FILTER_SFID2_SHIFT (0U)
#define MCANSS_STD_ID_FILTER_SFID2_MASK (0x000003FFU)
#define MCANSS_STD_ID_FILTER_SFID1_SHIFT (16U)
#define MCANSS_STD_ID_FILTER_SFID1_MASK (0x03FF0000U)
#define MCANSS_STD_ID_FILTER_SFEC_SHIFT (27U)
#define MCANSS_STD_ID_FILTER_SFEC_MASK (0x38000000U)
#define MCANSS_STD_ID_FILTER_SFT_SHIFT (30U)
#define MCANSS_STD_ID_FILTER_SFT_MASK (0xC0000000U)

/**
 * \brief  Extended Message ID Filter Element.
 *         扩展 ID 过滤器元素各字段掩码与移位
 */
#define MCANSS_EXT_ID_FILTER_EFID2_SHIFT (0U)
#define MCANSS_EXT_ID_FILTER_EFID2_MASK (0x1FFFFFFFU)
#define MCANSS_EXT_ID_FILTER_EFID1_SHIFT (0U)
#define MCANSS_EXT_ID_FILTER_EFID1_MASK (0x1FFFFFFFU)
#define MCANSS_EXT_ID_FILTER_EFEC_SHIFT (29U)
#define MCANSS_EXT_ID_FILTER_EFEC_MASK (0xE0000000U)
#define MCANSS_EXT_ID_FILTER_EFT_SHIFT (30U)
#define MCANSS_EXT_ID_FILTER_EFT_MASK (0xC0000000U)

/**
 * \brief  Mask and shift for Tx Event FIFO elements.
 *         发送事件 FIFO 元素各字段掩码与移位
 */
#define MCANSS_TX_EVENT_FIFO_ELEM_ID_SHIFT (0U)
#define MCANSS_TX_EVENT_FIFO_ELEM_ID_MASK (0x1FFFFFFFU)
#define MCANSS_TX_EVENT_FIFO_ELEM_RTR_SHIFT (29U)
#define MCANSS_TX_EVENT_FIFO_ELEM_RTR_MASK (0x20000000U)
#define MCANSS_TX_EVENT_FIFO_ELEM_XTD_SHIFT (30U)
#define MCANSS_TX_EVENT_FIFO_ELEM_XTD_MASK (0x40000000U)
#define MCANSS_TX_EVENT_FIFO_ELEM_ESI_SHIFT (31U)
#define MCANSS_TX_EVENT_FIFO_ELEM_ESI_MASK (0x80000000U)

#define MCANSS_TX_EVENT_FIFO_ELEM_TXTS_SHIFT (0U)
#define MCANSS_TX_EVENT_FIFO_ELEM_TXTS_MASK (0x0000FFFFU)
#define MCANSS_TX_EVENT_FIFO_ELEM_DLC_SHIFT (16U)
#define MCANSS_TX_EVENT_FIFO_ELEM_DLC_MASK (0x000F0000U)
#define MCANSS_TX_EVENT_FIFO_ELEM_BRS_SHIFT (20U)
#define MCANSS_TX_EVENT_FIFO_ELEM_BRS_MASK (0x00100000U)
#define MCANSS_TX_EVENT_FIFO_ELEM_FDF_SHIFT (21U)
#define MCANSS_TX_EVENT_FIFO_ELEM_FDF_MASK (0x00200000U)
#define MCANSS_TX_EVENT_FIFO_ELEM_ET_SHIFT (22U)
#define MCANSS_TX_EVENT_FIFO_ELEM_ET_MASK (0x00C00000U)
#define MCANSS_TX_EVENT_FIFO_ELEM_MM_SHIFT (24U)
#define MCANSS_TX_EVENT_FIFO_ELEM_MM_MASK (0xFF000000U)

#define HW_RD_REG32(addr) (uint32_t)(HW_RD_REG32_RAW((uint32_t)(addr)))

#define HW_WR_REG32(addr, value) \
    (HW_WR_REG32_RAW((uint32_t)(addr), (uint32_t)(value)))

#define HW_GET_FIELD(regVal, REG_FIELD) \
    (((regVal) & (uint32_t)REG_FIELD##_MASK) >> (uint32_t)REG_FIELD##_OFS)

#define HW_SET_FIELD32(regVal, REG_FIELD, fieldVal)                      \
    ((regVal) = ((regVal) & (uint32_t)(~(uint32_t)REG_FIELD##_MASK)) |   \
                ((((uint32_t)(fieldVal)) << (uint32_t)REG_FIELD##_OFS) & \
                 (uint32_t)REG_FIELD##_MASK))

#define HW_WR_FIELD32(regAddr, REG_FIELD, fieldVal)                     \
    (HW_WR_FIELD32_RAW((uint32_t)(regAddr), (uint32_t)REG_FIELD##_MASK, \
                       (uint32_t)REG_FIELD##_OFS, (uint32_t)(fieldVal)))

#define HW_RD_FIELD32(regAddr, REG_FIELD)                               \
    (HW_RD_FIELD32_RAW((uint32_t)(regAddr), (uint32_t)REG_FIELD##_MASK, \
                       (uint32_t)REG_FIELD##_OFS))

// 成功
#define STW_SOK ((int32_t)0)
// 操作失败
#define STW_EFAIL (-((int32_t)1))
// 参数错误
#define STW_EBADARGS (-((int32_t)2))
// 参数无效
#define STW_EINVALID_PARAMS (-((int32_t)3))
// 超时
#define STW_ETIMEOUT (-((int32_t)4))
// 越界
#define STW_EOUT_OF_RANGE (-((int32_t)5))
// 不支持的命令
#define STW_EUNSUPPORTED_CMD (-((int32_t)6))
// 不支持的操作
#define STW_EUNSUPPORTED_OPS (-((int32_t)7))

// 消息 RAM 基地址偏移（用于绝对地址计算）
#define MCAN_MCAN_MSG_MEM (0x0U)

/* To fix Misra-C errors */
#ifndef TRUE
#define TRUE ((Bool)1)
#endif
#ifndef FALSE
#define FALSE ((Bool)0)
#endif

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   This API will unblock write access to write protected registers.
 *          解锁写保护寄存器访问（设置 CCE 位）。
 * \param   mcan        Base Address of the MCAN Registers.
 *                      MCAN 寄存器基地址。
 * \return  None.
 */
static void DL_MCAN_writeProtectedRegAccessUnlock(MCAN_Regs *mcan);

/**
 * \brief   This API will block write access to write protected registers.
 *          锁定写保护寄存器访问（清除 CCE 位）。
 * \param   mcan        Base Address of the MCAN Registers.
 *                      MCAN 寄存器基地址。
 * \return  None.
 */
static void DL_MCAN_writeProtectedRegAccessLock(MCAN_Regs *mcan);

/**
 * \brief   This API will load the register from ECC memory bank.
 *          从 ECC 存储区加载指定寄存器（通过 SVBUS 读取）。
 * \param   mcan        Base Address of the MCAN Registers.
 *                      MCAN 寄存器基地址。
 * \param   regOffset   Offset of the register to read.
 *                      待读取寄存器偏移。
 * \return  None.
 */
static void DL_MCAN_eccLoadRegister(MCAN_Regs *mcan, uint32_t regOffset);

/**
 * \brief   This API will read the message object from Message RAM.
 *          从消息 RAM 读取一个接收报文对象。
 * \param   mcan        Base Address of the MCAN Registers.
 *                      MCAN 寄存器基地址。
 * \param   elemAddr    Address of the message object.
 *                      消息对象地址。
 * \param   elem        Message Object. Refer struct #DL_MCAN_RxBufElement.
 *                      接收报文输出结构体。
 * \return  None.
 */
static void DL_MCAN_readMsg(
    uint32_t mcan, uint32_t elemAddr, DL_MCAN_RxBufElement *elem);

/**
 * \brief   This API will write the message object to Message RAM.
 *          向消息 RAM 写入一个发送报文对象。
 * \param   mcan        Base Address of the MCAN Registers.
 *                      MCAN 寄存器基地址。
 * \param   elemAddr    Address of the message object.
 *                      消息对象地址。
 * \param   elem        Message Object. Refer struct #DL_MCAN_TxBufElement.
 *                      发送报文输入结构体。
 * \return  None.
 */
static void DL_MCAN_writeMsg(
    uint32_t mcan, uint32_t elemAddr, const DL_MCAN_TxBufElement *elem);

/**
 * \brief   This API will return payload depending on 'dlc'  field.
 *          根据 DLC 字段返回实际有效载荷字节数。
 * \param   dlc             Data Length Code. 数据长度码。
 * \return  data size       Size of the payload. 有效载荷大小（字节）。
 */
static uint32_t DL_MCAN_getDataSize(uint32_t dlc);

/**
 * \brief   This API will return message object size.
 *          根据元素尺寸编码返回消息 RAM 中对象的字大小。
 * \param   elemSize        Element Size. 元素尺寸编码。
 * \return  message object size 消息对象字大小。
 */
static uint32_t DL_MCAN_getMsgObjSize(uint32_t elemSize);

/**
 *  \brief   This function reads a 32-bit value from a hardware register
 *           and returns the value.
 *           从硬件寄存器读取 32 位值。
 *  \param   addr    Address of the memory mapped hardware register.
 *                   内存映射硬件寄存器地址。
 *  \return  Unsigned 32-bit value read from a register. 读取到的 32 位值。
 */
__STATIC_INLINE uint32_t HW_RD_REG32_RAW(uint32_t addr);

/**
 *  \brief   This function writes a 32-bit value to a hardware register.
 *           向硬件寄存器写入 32 位值。
 *  \param   addr    Address of the memory mapped hardware register.
 *                   内存映射硬件寄存器地址。
 *  \param   value   unsigned 32-bit value which has to be written to the register.
 *                   待写入的 32 位值。
 */
__STATIC_INLINE void HW_WR_REG32_RAW(uint32_t addr, uint32_t value);

/**
 *  \brief   This function reads a 32 bit register, modifies specific set of
 *           bits and writes back to the register.
 *           读取 32 位寄存器，修改指定位域后写回。
 *  \param   addr    Address of the memory mapped hardware register.
 *                   内存映射硬件寄存器地址。
 *  \param   mask    Mask for the bit field. 位域掩码。
 *  \param   shift   Bit field shift from LSB. 位域左移量。
 *  \param   value   Value to be written to bit-field. 待写入位域值。
 */
__STATIC_INLINE void HW_WR_FIELD32_RAW(
    uint32_t addr, uint32_t mask, uint32_t shift, uint32_t value);

/**
 *  \brief   This function reads a 32 bit register, masks specific set of bits
 *           and the left shifted value.
 *           读取 32 位寄存器并提取指定位域值。
 *  \param   addr    Address of the memory mapped hardware register.
 *                   内存映射硬件寄存器地址。
 *  \param   mask    Mask for the bit field. 位域掩码。
 *  \param   shift   Bit field shift from LSB. 位域左移量。
 *  \return  Bit-field value (absolute value - shifted to LSB position)
 *           位域值（已右移到 LSB 位置）。
 */
__STATIC_INLINE uint32_t HW_RD_FIELD32_RAW(
    uint32_t addr, uint32_t mask, uint32_t shift);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 * @brief Check whether MCAN instance clock domain is ready.
 * @details 功能：检查指定 MCAN 实例时钟同步是否完成。
 * @param[in] instance 参数：MCAN 实例选择。
 * @return 返回值：`true` 就绪，`false` 仍在同步。
 */
bool DL_MCAN_isReady(DL_MCAN_INSTANCE instance)
{
    return ((SYSCTL->SOCLOCK.SYSSTATUS & (uint32_t)instance) ==
            (uint32_t)instance);
}

/**
 * @brief Configure MCAN functional clock source and divider.
 * @details 功能：配置 MCAN 时钟源与功能时钟分频。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] config 参数：MCAN 时钟配置结构体。
 * @return 返回值：无。
 */
void DL_MCAN_setClockConfig(MCAN_Regs *mcan, const DL_MCAN_ClockConfig *config)
{
    DL_Common_updateReg(&SYSCTL->SOCLOCK.GENCLKCFG,
                        (uint32_t)config->clockSel, SYSCTL_GENCLKCFG_CANCLKSRC_MASK);

    mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKDIV = (uint32_t)config->divider;
}

/**
 * @brief Read MCAN functional clock source and divider configuration.
 * @details 功能：读取当前 MCAN 时钟源配置并写回结构体。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] config 参数：用于回填的时钟配置结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getClockConfig(MCAN_Regs *mcan, DL_MCAN_ClockConfig *config)
{
    uint32_t clockSource =
        (SYSCTL->SOCLOCK.GENCLKCFG & SYSCTL_GENCLKCFG_CANCLKSRC_MASK);

    config->clockSel = (DL_MCAN_FCLK)(clockSource);
}

/**
 * @brief Check whether MCAN wrapper reset is active.
 * @details 功能：读取复位状态位，判断 MCAN 是否处于复位中。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：`true` 复位中，`false` 非复位中。
 */
bool DL_MCAN_isInReset(const MCAN_Regs *mcan)
{
    uint32_t reset;
    bool state;

    reset = HW_RD_FIELD32(
        &mcan->MCANSS.STAT, MCAN_TI_WRAPPER_PROCESSORS_REGS_STAT_RESET);
    if (1U == reset)
    {
        state = true;
    }
    else
    {
        state = false;
    }
    return state;
}

/**
 * @brief Check whether CAN-FD operation is enabled.
 * @details 功能：读取 FDOE 状态位判断是否启用 CAN-FD。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：`true` 已启用，`false` 未启用。
 */
bool DL_MCAN_isFDOpEnable(const MCAN_Regs *mcan)
{
    uint32_t fdoe;
    bool state;

    fdoe = HW_RD_FIELD32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_STAT,
        MCAN_TI_WRAPPER_PROCESSORS_REGS_STAT_ENABLE_FDOE);
    if (1U == fdoe)
    {
        state = true;
    }
    else
    {
        state = false;
    }
    return state;
}

/**
 * @brief Check whether MCAN message RAM initialization is done.
 * @details 功能：读取 MEM_INIT_DONE 状态位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：`true` 已完成，`false` 未完成。
 */
bool DL_MCAN_isMemInitDone(const MCAN_Regs *mcan)
{
    uint32_t memInit;
    bool state;

    memInit = HW_RD_FIELD32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_STAT,
        MCAN_TI_WRAPPER_PROCESSORS_REGS_STAT_MEM_INIT_DONE);
    if (1U == memInit)
    {
        state = true;
    }
    else
    {
        state = false;
    }
    return state;
}

/**
 * @brief Set MCAN operation mode.
 * @details 功能：写 INIT 位切换正常模式/软件初始化模式。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] mode 参数：模式值。
 * @return 返回值：无。
 */
void DL_MCAN_setOpMode(MCAN_Regs *mcan, uint32_t mode)
{
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_INIT, mode);
}

/**
 * @brief Get MCAN operation mode.
 * @details 功能：读取 INIT 位返回当前工作模式。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：模式值。
 */
uint32_t DL_MCAN_getOpMode(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_INIT));
}

/**
 * @brief Initialize MCAN controller core options.
 * @details 功能：配置 MCAN 模式、TDC 参数与消息 RAM 看门狗预装值。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] initParams 参数：初始化参数结构体。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数超限。
 */
int32_t DL_MCAN_init(MCAN_Regs *mcan, const DL_MCAN_InitParams *initParams)
{
    int32_t status;
    uint32_t regVal;

    /* Configure MCAN wakeup and clock stop controls */
    regVal = HW_RD_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_CTRL);
    HW_SET_FIELD32(regVal, MCAN_TI_WRAPPER_REGS_CTRL_WAKEUPREQEN,
                   initParams->wkupReqEnable);
    HW_SET_FIELD32(regVal, MCAN_TI_WRAPPER_REGS_CTRL_AUTOWAKEUP,
                   initParams->autoWkupEnable);
    HW_SET_FIELD32(regVal, MCAN_TI_WRAPPER_REGS_CTRL_DBGSUSP_FREE,
                   initParams->emulationEnable);

    HW_WR_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_CTRL, regVal);

    // 中文说明：打开写保护寄存器访问窗口，允许改写受保护配置。
    DL_MCAN_writeProtectedRegAccessUnlock(mcan);

    /* Configure MCAN mode(FD vs Classic CAN operation) and controls */
    regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_CCCR);
    HW_SET_FIELD32(regVal, MCAN_CCCR_FDOE, initParams->fdMode);
    HW_SET_FIELD32(regVal, MCAN_CCCR_BRSE, initParams->brsEnable);
    HW_SET_FIELD32(regVal, MCAN_CCCR_TXP, initParams->txpEnable);
    HW_SET_FIELD32(regVal, MCAN_CCCR_EFBI, initParams->efbi);
    HW_SET_FIELD32(regVal, MCAN_CCCR_PXHD, initParams->pxhddisable);
    HW_SET_FIELD32(regVal, MCAN_CCCR_DAR, initParams->darEnable);
    HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_CCCR, regVal);

    // 中文说明：先做范围校验，避免写入越界导致协议异常。
    if ((DL_MCAN_TDCR_TDCF_MAX >= initParams->tdcConfig.tdcf) &&
        (DL_MCAN_TDCR_TDCO_MAX >= initParams->tdcConfig.tdco) &&
        (DL_MCAN_RWD_WDC_MAX >= initParams->wdcPreload))
    {
        /* Configure Transceiver Delay Compensation */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TDCR, MCAN_TDCR_TDCF,
                      initParams->tdcConfig.tdcf);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TDCR, MCAN_TDCR_TDCO,
                      initParams->tdcConfig.tdco);
        /* Configure MSG RAM watchdog counter preload value */
        HW_WR_FIELD32(
            &mcan->MCANSS.MCAN.MCAN_RWD, MCAN_RWD_WDC, initParams->wdcPreload);
        /* Enable/Disable Transceiver Delay Compensation */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_TDC,
                      initParams->tdcEnable);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }

    // 中文说明：配置完成后关闭写保护寄存器访问窗口。
    DL_MCAN_writeProtectedRegAccessLock(mcan);

    return status;
}

/**
 * @brief Configure runtime MCAN global behavior.
 * @details 功能：配置监视模式、全局过滤器、时间戳与超时计数器。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] configParams 参数：运行配置参数结构体。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数超限。
 */
int32_t DL_MCAN_config(
    MCAN_Regs *mcan, const DL_MCAN_ConfigParams *configParams)
{
    int32_t status;

    DL_MCAN_writeProtectedRegAccessUnlock(mcan);

    /* Configure MCAN control registers */
    HW_WR_FIELD32(
        &mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_MON, configParams->monEnable);
    HW_WR_FIELD32(
        &mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_ASM, configParams->asmEnable);
    /* Configure Global Filter */
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_GFC, MCAN_GFC_RRFE,
                  configParams->filterConfig.rrfe);
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_GFC, MCAN_GFC_RRFS,
                  configParams->filterConfig.rrfs);
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_GFC, MCAN_GFC_ANFE,
                  configParams->filterConfig.anfe);
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_GFC, MCAN_GFC_ANFS,
                  configParams->filterConfig.anfs);

    // 中文说明：仅在时间戳/超时预装值有效时写入相关计数寄存器。
    if ((DL_MCAN_TSCC_TCP_MAX >= configParams->tsPrescalar) &&
        (DL_MCAN_TOCC_TOP_MAX >= configParams->timeoutPreload))
    {
        /* Configure Time-stamp counter */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TSCC, MCAN_TSCC_TSS,
                      configParams->tsSelect);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TSCC, MCAN_TSCC_TCP,
                      (configParams->tsPrescalar - 1U));
        /* Configure Time-out counter */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TOCC, MCAN_TOCC_TOS,
                      configParams->timeoutSelect);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TOCC, MCAN_TOCC_TOP,
                      configParams->timeoutPreload);
        /* Enable Time-out counter */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TOCC, MCAN_TOCC_ETOC,
                      configParams->timeoutCntEnable);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }

    // 中文说明：参数写完后锁回配置窗口。
    DL_MCAN_writeProtectedRegAccessLock(mcan);

    return status;
}

/**
 * @brief Configure MCAN ECC behavior.
 * @details 功能：配置 ECC 检查、总使能与读改写选项。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] configParams 参数：ECC 配置结构体。
 * @return 返回值：无。
 */
void DL_MCAN_eccConfig(
    MCAN_Regs *mcan, const DL_MCAN_ECCConfigParams *configParams)
{
    DL_MCAN_eccLoadRegister(mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER
                                      .PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL);
    HW_WR_FIELD32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL,
        MCAN_TI_WRAPPER_ECC_REGS_CTRL_ECC_CHECK, configParams->enableChk);
    DL_MCAN_eccLoadRegister(mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER
                                      .PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL);
    HW_WR_FIELD32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL,
        MCAN_TI_WRAPPER_ECC_REGS_CTRL_ECC_ENABLE, configParams->enable);
    DL_MCAN_eccLoadRegister(mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER
                                      .PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL);
    HW_WR_FIELD32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL,
        MCAN_TI_WRAPPER_ECC_REGS_CTRL_ENABLE_RMW, configParams->enableRdModWr);
    DL_MCAN_eccLoadRegister(mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER
                                      .PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL);
}

/**
 * @brief Configure nominal and data phase bit timing.
 * @details 功能：设置 MCAN 仲裁相位与数据相位位时序参数。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] configParams 参数：位时序参数结构体。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数超限。
 */
int32_t DL_MCAN_setBitTime(
    MCAN_Regs *mcan, const DL_MCAN_BitTimingParams *configParams)
{
    int32_t status;

    DL_MCAN_writeProtectedRegAccessUnlock(mcan);

    // 中文说明：先验证并配置仲裁相位位时序。
    if ((DL_MCAN_NBTP_NSJW_MAX >= configParams->nomSynchJumpWidth) &&
        (DL_MCAN_NBTP_NTSEG2_MAX >= configParams->nomTimeSeg2) &&
        (DL_MCAN_NBTP_NTSEG1_MAX >= configParams->nomTimeSeg1) &&
        (DL_MCAN_NBTP_NBRP_MAX >= configParams->nomRatePrescalar))
    {
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NSJW,
                      configParams->nomSynchJumpWidth);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NTSEG2,
                      configParams->nomTimeSeg2);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NTSEG1,
                      configParams->nomTimeSeg1);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NBRP,
                      configParams->nomRatePrescalar);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    if (STW_SOK == status)
    {
        // 中文说明：仅在仲裁相位成功后再配置数据相位位时序。
        if ((DL_MCAN_DBTP_DSJW_MAX >= configParams->dataSynchJumpWidth) &&
            (DL_MCAN_DBTP_DTSEG2_MAX >= configParams->dataTimeSeg2) &&
            (DL_MCAN_DBTP_DTSEG1_MAX >= configParams->dataTimeSeg1) &&
            (DL_MCAN_DBTP_DBRP_MAX >= configParams->dataRatePrescalar))
        {
            HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DSJW,
                          configParams->dataSynchJumpWidth);
            HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DTSEG2,
                          configParams->dataTimeSeg2);
            HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DTSEG1,
                          configParams->dataTimeSeg1);
            HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DBRP,
                          configParams->dataRatePrescalar);
            status = STW_SOK;
        }
        else
        {
            status = STW_EFAIL;
        }
    }

    DL_MCAN_writeProtectedRegAccessLock(mcan);
    return status;
}

/**
 * @brief Configure MCAN Message RAM layout.
 * @details 功能：配置过滤器、RX/TX FIFO、Buffer 与 Tx Event FIFO 布局。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] msgRAMConfigParams 参数：消息 RAM 布局参数。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 表示配置非法。
 */
int32_t DL_MCAN_msgRAMConfig(
    MCAN_Regs *mcan, const DL_MCAN_MsgRAMConfigParams *msgRAMConfigParams)
{
    int32_t status;
    uint32_t elemNum = 0U;

    DL_MCAN_writeProtectedRegAccessUnlock(mcan);

    /* Configure Message Filters section */
    if (0U != msgRAMConfigParams->lss)
    {
        // 中文说明：标准 ID 过滤器区域配置。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_SIDFC, MCAN_SIDFC_FLSSA,
                      (msgRAMConfigParams->flssa >> 2U));
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_SIDFC, MCAN_SIDFC_LSS,
                      msgRAMConfigParams->lss);
    }
    if (0U != msgRAMConfigParams->lse)
    {
        // 中文说明：扩展 ID 过滤器区域配置。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_XIDFC, MCAN_XIDFC_FLESA,
                      (msgRAMConfigParams->flesa >> 2U));
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_XIDFC, MCAN_XIDFC_LSE,
                      msgRAMConfigParams->lse);
    }
    /* Configure Rx FIFO 0 section */
    if (0U != msgRAMConfigParams->rxFIFO0size)
    {
        // 中文说明：RX FIFO0 起始地址、深度与水位配置。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF0C, MCAN_RXF0C_F0SA,
                      (msgRAMConfigParams->rxFIFO0startAddr >> 2U));
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF0C, MCAN_RXF0C_F0S,
                      msgRAMConfigParams->rxFIFO0size);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF0C, MCAN_RXF0C_F0WM,
                      msgRAMConfigParams->rxFIFO0waterMark);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF0C, MCAN_RXF0C_F0OM,
                      msgRAMConfigParams->rxFIFO0OpMode);
        /* Configure Rx FIFO0 elements size */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXESC, MCAN_RXESC_F0DS,
                      msgRAMConfigParams->rxFIFO0ElemSize);
    }
    /* Configure Rx FIFO 1 section */
    if (0U != msgRAMConfigParams->rxFIFO1size)
    {
        // 中文说明：RX FIFO1 起始地址、深度与水位配置。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF1C, MCAN_RXF1C_F1SA,
                      (msgRAMConfigParams->rxFIFO1startAddr >> 2U));
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF1C, MCAN_RXF1C_F1S,
                      msgRAMConfigParams->rxFIFO1size);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF1C, MCAN_RXF1C_F1WM,
                      msgRAMConfigParams->rxFIFO1waterMark);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF1C, MCAN_RXF1C_F1OM,
                      msgRAMConfigParams->rxFIFO1OpMode);
        /* Configure Rx FIFO1 elements size */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXESC, MCAN_RXESC_F1DS,
                      msgRAMConfigParams->rxFIFO1ElemSize);
    }
    /* Configure Rx Buffer Start Address */
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXBC, MCAN_RXBC_RBSA,
                  (msgRAMConfigParams->rxBufStartAddr >> 2U));
    /* Configure Rx Buffer elements size */
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXESC, MCAN_RXESC_RBDS,
                  msgRAMConfigParams->rxBufElemSize);
    /* Configure Tx Event FIFO section */
    if (0U != msgRAMConfigParams->txEventFIFOSize)
    {
        // 中文说明：TX Event FIFO 区域配置。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFC, MCAN_TXEFC_EFSA,
                      (msgRAMConfigParams->txEventFIFOStartAddr >> 2U));
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFC, MCAN_TXEFC_EFS,
                      msgRAMConfigParams->txEventFIFOSize);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFC, MCAN_TXEFC_EFWM,
                      msgRAMConfigParams->txEventFIFOWaterMark);
    }
    /* Configure Tx Buffer and FIFO/Q section */
    elemNum = msgRAMConfigParams->txBufNum + msgRAMConfigParams->txFIFOSize;
    // 中文说明：总元素数必须满足硬件最大 Tx 槽位限制。
    if ((MCANSS_TX_BUFFER_MAX >= elemNum) &&
        ((0U != msgRAMConfigParams->txBufNum) ||
         (0U != msgRAMConfigParams->txFIFOSize)))
    {
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXBC, MCAN_TXBC_TBSA,
                      (msgRAMConfigParams->txStartAddr >> 2U));
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXBC, MCAN_TXBC_NDTB,
                      msgRAMConfigParams->txBufNum);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXBC, MCAN_TXBC_TFQS,
                      msgRAMConfigParams->txFIFOSize);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXBC, MCAN_TXBC_TFQM,
                      msgRAMConfigParams->txBufMode);
        /* Configure Tx Buffer/FIFO0/FIFO1 elements size */
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXESC, MCAN_TXESC_TBDS,
                      msgRAMConfigParams->txBufElemSize);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }

    // 中文说明：消息 RAM 布局配置完成后关闭写保护访问。
    DL_MCAN_writeProtectedRegAccessLock(mcan);

    return status;
}

/**
 * @brief Set global extended ID mask.
 * @details 功能：校验后写入 XIDAM 扩展 ID 掩码。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] idMask 参数：扩展 ID 掩码值。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数越界。
 */
int32_t DL_MCAN_setExtIDAndMask(MCAN_Regs *mcan, uint32_t idMask)
{
    int32_t status;

    if (DL_MCAN_XIDAM_EIDM_MAX >= idMask)
    {
        DL_MCAN_writeProtectedRegAccessUnlock(mcan);

        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_XIDAM, MCAN_XIDAM_EIDM, idMask);

        DL_MCAN_writeProtectedRegAccessLock(mcan);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    return status;
}

/**
 * @brief Write one MCAN TX message element into message RAM.
 * @details 功能：根据 Buffer/FIFO 选择计算目标地址并写入 TX 报文对象。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] memType 参数：写入内存类型（Buffer 或 FIFO）。
 * @param[in] bufNum 参数：目标缓冲区号（仅 Buffer 模式有效）。
 * @param[in] elem 参数：待写入的 TX 报文元素。
 * @return 返回值：无。
 */
void DL_MCAN_writeMsgRam(MCAN_Regs *mcan, uint32_t memType, uint32_t bufNum,
                         const DL_MCAN_TxBufElement *elem)
{
    uint32_t startAddr = 0U, elemSize = 0U, elemAddr = 0U;
    uint32_t idx = 0U, enableMod = 0U;

    if (((uint32_t)DL_MCAN_MEM_TYPE_BUF) == memType)
    {
        idx = bufNum;
        enableMod = 1U;
    }
    if (((uint32_t)DL_MCAN_MEM_TYPE_FIFO) == memType)
    {
        // 中文说明：FIFO 模式由硬件 TFQP 给出当前可写入索引。
        idx = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXFQS, MCAN_TXFQS_TFQP);
        enableMod = 1U;
    }
    if (1U == enableMod)
    {
        startAddr =
            HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXBC, MCAN_TXBC_TBSA);
        elemSize =
            HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXESC, MCAN_TXESC_TBDS);
        startAddr = (uint32_t)(startAddr << 2U);
        elemSize = DL_MCAN_getMsgObjSize(elemSize);
        elemSize *= 4U;
        elemAddr = startAddr + (elemSize * idx);
        elemAddr += MCAN_MCAN_MSG_MEM;
        DL_MCAN_writeMsg((uint32_t)mcan, elemAddr, elem);
    }
}

/**
 * @brief Request transmission for a TX buffer.
 * @details 功能：设置 TXBAR 对应 bit 触发指定缓冲区发送。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] bufNum 参数：发送缓冲区编号。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数越界。
 */
int32_t DL_MCAN_TXBufAddReq(MCAN_Regs *mcan, uint32_t bufNum)
{
    int32_t status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > bufNum)
    {
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBAR);
        regVal |= ((uint32_t)1U << bufNum);

        /*
         * For writing to TXBAR CCE bit should be '0'. This need not be
         * reverted because for other qualified writes this is locked state
         * and can't be written.
         */
        DL_MCAN_writeProtectedRegAccessLock(mcan);
        HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_TXBAR, regVal);

        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    return status;
}

/**
 * @brief Get RX new-data status bitmaps.
 * @details 功能：读取 NDAT1/NDAT2 新数据标志。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] newDataStatus 参数：新数据状态输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getNewDataStatus(
    const MCAN_Regs *mcan, DL_MCAN_RxNewDataStatus *newDataStatus)
{
    newDataStatus->statusLow = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_NDAT1);
    newDataStatus->statusHigh = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_NDAT2);
}

/**
 * @brief Clear RX new-data status bitmaps.
 * @details 功能：向 NDAT1/NDAT2 写回掩码清除新数据标志。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] newDataStatus 参数：待清除的新数据位图。
 * @return 返回值：无。
 */
void DL_MCAN_clearNewDataStatus(
    MCAN_Regs *mcan, const DL_MCAN_RxNewDataStatus *newDataStatus)
{
    HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_NDAT1, newDataStatus->statusLow);
    HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_NDAT2, newDataStatus->statusHigh);
}

/**
 * @brief Read one RX message element from MCAN message RAM.
 * @details 功能：按 Buffer/FIFO 类型定位元素地址并解析为接收报文。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] memType 参数：读取内存类型（Buffer 或 FIFO）。
 * @param[in] bufNum 参数：目标缓冲区号（仅 Buffer 模式有效）。
 * @param[in] fifoNum 参数：目标 FIFO 号（仅 FIFO 模式有效）。
 * @param[out] elem 参数：接收报文输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_readMsgRam(const MCAN_Regs *mcan, uint32_t memType,
                        uint32_t bufNum, uint32_t fifoNum, DL_MCAN_RxBufElement *elem)
{
    uint32_t startAddr = 0U, elemSize = 0U, elemAddr = 0U;
    uint32_t enableMod = 0U, idx = 0U;

    if (((uint32_t)DL_MCAN_MEM_TYPE_BUF) == memType)
    {
        startAddr =
            HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXBC, MCAN_RXBC_RBSA);
        elemSize =
            HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXESC, MCAN_RXESC_RBDS);
        idx = bufNum;
        enableMod = 1U;
    }
    if (((uint32_t)DL_MCAN_MEM_TYPE_FIFO) == memType)
    {
        // 中文说明：FIFO 模式下根据 FIFO 号选择对应队列读指针。
        switch (fifoNum)
        {
        case ((uint32_t)DL_MCAN_RX_FIFO_NUM_0):
            startAddr = HW_RD_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXF0C, MCAN_RXF0C_F0SA);
            elemSize = HW_RD_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXESC, MCAN_RXESC_F0DS);
            idx = HW_RD_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXF0S, MCAN_RXF0S_F0GI);
            enableMod = 1U;
            break;
        case ((uint32_t)DL_MCAN_RX_FIFO_NUM_1):
            startAddr = HW_RD_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXF1C, MCAN_RXF1C_F1SA);
            elemSize = HW_RD_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXESC, MCAN_RXESC_F1DS);
            idx = HW_RD_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXF1S, MCAN_RXF1S_F1GI);
            enableMod = 1U;
            break;
        default:
            /* Invalid option */
            break;
        }
    }
    if (1U == enableMod)
    {
        // 中文说明：按照元素大小与索引换算消息 RAM 绝对地址。
        startAddr = (uint32_t)(startAddr << 2U);
        elemSize = DL_MCAN_getMsgObjSize(elemSize);
        elemSize *= 4U;
        elemAddr = startAddr + (elemSize * idx);
        elemAddr += MCAN_MCAN_MSG_MEM;
        DL_MCAN_readMsg((uint32_t)mcan, elemAddr, elem);
    }
}

/**
 * @brief Read one TX event element.
 * @details 功能：按当前 TX Event FIFO 读指针解析事件元素。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] txEventElem 参数：事件输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_readTxEventFIFO(
    const MCAN_Regs *mcan, DL_MCAN_TxEventFIFOElement *txEventElem)
{
    uint32_t startAddr = 0U, elemSize = 0U, elemAddr = 0U;
    uint32_t idx = 0U, regVal;

    startAddr = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFC, MCAN_TXEFC_EFSA);
    elemSize = MCANSS_TX_EVENT_FIFO_SIZE_WORDS;
    idx = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFS, MCAN_TXEFS_EFGI);

    startAddr = (uint32_t)(startAddr << 2U);
    elemSize *= 4U;
    elemAddr = startAddr + (elemSize * idx);
    elemAddr += MCAN_MCAN_MSG_MEM;

    regVal = HW_RD_REG32(((uint32_t)mcan + (uint32_t)elemAddr));
    txEventElem->id =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_ID_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_ID_SHIFT);
    txEventElem->rtr =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_RTR_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_RTR_SHIFT);
    txEventElem->xtd =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_XTD_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_XTD_SHIFT);
    txEventElem->esi =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_ESI_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_ESI_SHIFT);
    elemAddr += 4U;

    regVal = HW_RD_REG32(((uint32_t)mcan + (uint32_t)elemAddr));

    txEventElem->txts =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_TXTS_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_TXTS_SHIFT);
    txEventElem->dlc =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_DLC_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_DLC_SHIFT);
    txEventElem->brs =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_BRS_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_BRS_SHIFT);
    txEventElem->fdf =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_FDF_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_FDF_SHIFT);
    txEventElem->et =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_ET_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_ET_SHIFT);
    txEventElem->mm =
        (uint32_t)((regVal & MCANSS_TX_EVENT_FIFO_ELEM_MM_MASK) >>
                   MCANSS_TX_EVENT_FIFO_ELEM_MM_SHIFT);
}

/**
 * @brief Add one standard ID filter element.
 * @details 功能：将标准 ID 过滤器元素写入消息 RAM 对应位置。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] filtNum 参数：过滤器索引。
 * @param[in] elem 参数：标准 ID 过滤器元素。
 * @return 返回值：无。
 */
void DL_MCAN_addStdMsgIDFilter(MCAN_Regs *mcan, uint32_t filtNum,
                               const DL_MCAN_StdMsgIDFilterElement *elem)
{
    uint32_t startAddr, elemAddr, regVal;

    startAddr = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_SIDFC, MCAN_SIDFC_FLSSA);
    startAddr = (uint32_t)(startAddr << 2U);
    elemAddr = startAddr + (filtNum * MCANSS_STD_ID_FILTER_SIZE_WORDS * 4U);
    elemAddr += MCAN_MCAN_MSG_MEM;

    regVal = 0U;
    regVal |= (uint32_t)(elem->sfid2 << MCANSS_STD_ID_FILTER_SFID2_SHIFT);
    regVal |= (uint32_t)(elem->sfid1 << MCANSS_STD_ID_FILTER_SFID1_SHIFT);
    regVal |= (uint32_t)(elem->sfec << MCANSS_STD_ID_FILTER_SFEC_SHIFT);
    regVal |= (uint32_t)(elem->sft << MCANSS_STD_ID_FILTER_SFT_SHIFT);
    HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);
}

/**
 * @brief Add one extended ID filter element.
 * @details 功能：将扩展 ID 过滤器两字元素写入消息 RAM。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] filtNum 参数：过滤器索引。
 * @param[in] elem 参数：扩展 ID 过滤器元素。
 * @return 返回值：无。
 */
void DL_MCAN_addExtMsgIDFilter(MCAN_Regs *mcan, uint32_t filtNum,
                               const DL_MCAN_ExtMsgIDFilterElement *elem)
{
    uint32_t startAddr, elemAddr, regVal;

    startAddr = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_XIDFC, MCAN_XIDFC_FLESA);
    startAddr = (uint32_t)(startAddr << 2U);
    elemAddr = startAddr + (filtNum * MCANSS_EXT_ID_FILTER_SIZE_WORDS * 4U);
    elemAddr += MCAN_MCAN_MSG_MEM;

    regVal = 0U;
    regVal |= (uint32_t)(elem->efid1 << MCANSS_EXT_ID_FILTER_EFID1_SHIFT);
    regVal |= (uint32_t)(elem->efec << MCANSS_EXT_ID_FILTER_EFEC_SHIFT);
    HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);

    elemAddr += 4U;
    regVal = 0U;
    regVal |= (uint32_t)(elem->efid2 << MCANSS_EXT_ID_FILTER_EFID2_SHIFT);
    regVal |= (uint32_t)(elem->eft << MCANSS_EXT_ID_FILTER_EFT_SHIFT);
    HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);
}

/**
 * @brief Enable or disable MCAN loopback mode.
 * @details 功能：配置 MCAN 内环/外环回环模式及相关测试位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] lpbkMode 参数：回环模式选择。
 * @param[in] enable 参数：`true` 使能，`false` 关闭。
 * @return 返回值：无。
 */
void DL_MCAN_lpbkModeEnable(MCAN_Regs *mcan, uint32_t lpbkMode, bool enable)
{
    DL_MCAN_writeProtectedRegAccessUnlock(mcan);

    if (true == enable)
    {
        // 中文说明：进入测试模式并打开回环位。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_TEST, 0x1U);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TEST, MCAN_TEST_LBCK, enable);
        if (((uint32_t)DL_MCAN_LPBK_MODE_INTERNAL) == lpbkMode)
        {
            // 中文说明：内环模式需使能总线监视位，避免驱动总线。
            HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_MON, 0x1U);
        }
    }
    else
    {
        // 中文说明：关闭回环后同步恢复 TEST/MON 控制位。
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TEST, MCAN_TEST_LBCK, enable);
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_TEST, 0x0U);
        if (((uint32_t)DL_MCAN_LPBK_MODE_INTERNAL) == lpbkMode)
        {
            HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_MON, 0x0U);
        }
    }
    DL_MCAN_writeProtectedRegAccessLock(mcan);
}

/**
 * @brief Get MCAN error counters.
 * @details 功能：读取发送/接收错误计数与错误日志计数。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] errCounter 参数：错误计数输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getErrCounters(
    const MCAN_Regs *mcan, DL_MCAN_ErrCntStatus *errCounter)
{
    errCounter->canErrLogCnt =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_ECR, MCAN_ECR_CEL);
    errCounter->transErrLogCnt =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_ECR, MCAN_ECR_TEC);
    errCounter->recErrCnt =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_ECR, MCAN_ECR_REC);
    errCounter->rpStatus =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_ECR, MCAN_ECR_RP);
}

/**
 * @brief Get MCAN protocol status snapshot.
 * @details 功能：读取 PSR 并解析协议状态字段。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] protStatus 参数：协议状态输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getProtocolStatus(
    const MCAN_Regs *mcan, DL_MCAN_ProtocolStatus *protStatus)
{
    uint32_t regVal;

    // 一次读取 PSR 寄存器，逐字段解析
    regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_PSR);
    protStatus->lastErrCode = HW_GET_FIELD(regVal, MCAN_PSR_LEC);
    protStatus->act = HW_GET_FIELD(regVal, MCAN_PSR_ACT);
    protStatus->errPassive = HW_GET_FIELD(regVal, MCAN_PSR_EP);
    protStatus->warningStatus = HW_GET_FIELD(regVal, MCAN_PSR_EW);
    protStatus->busOffStatus = HW_GET_FIELD(regVal, MCAN_PSR_BO);
    protStatus->dlec = HW_GET_FIELD(regVal, MCAN_PSR_DLEC);
    protStatus->resi = HW_GET_FIELD(regVal, MCAN_PSR_RESI);
    protStatus->rbrs = HW_GET_FIELD(regVal, MCAN_PSR_RBRS);
    protStatus->rfdf = HW_GET_FIELD(regVal, MCAN_PSR_RFDF);
    protStatus->pxe = HW_GET_FIELD(regVal, MCAN_PSR_PXE);
    protStatus->tdcv = HW_GET_FIELD(regVal, MCAN_PSR_TDCV);
}

/**
 * @brief Enable or disable MCAN interrupt sources.
 * @details 功能：按掩码设置 MCAN_IE 中断使能位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] intrMask 参数：中断源掩码。
 * @param[in] enable 参数：`true` 使能，`false` 关闭。
 * @return 返回值：无。
 */
void DL_MCAN_enableIntr(MCAN_Regs *mcan, uint32_t intrMask, bool enable)
{
    uint32_t regVal;

    if (true == enable)
    {
        // 读-改-写 IE 寄存器，置位对应中断使能
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_IE);
        regVal |= intrMask;
        HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_IE, regVal);
    }
    else
    {
        // 读-改-写 IE 寄存器，清除对应中断使能
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_IE);
        regVal &= ~intrMask;
        HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_IE, regVal);
    }
}

/**
 * @brief Route interrupt sources to line0/line1.
 * @details 功能：配置 MCAN_ILS，将指定中断映射到目标中断线。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] intrMask 参数：待映射中断源掩码。
 * @param[in] lineNum 参数：目标中断线编号。
 * @return 返回值：无。
 */
void DL_MCAN_selectIntrLine(
    MCAN_Regs *mcan, uint32_t intrMask, uint32_t lineNum)
{
    uint32_t regVal;

    if (((uint32_t)DL_MCAN_INTR_LINE_NUM_0) == lineNum)
    {
        // 清除 ILS 对应位 → 映射到 Line0
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_ILS);
        regVal &= ~intrMask;
        HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_ILS, regVal);
    }
    else
    {
        // 置位 ILS 对应位 → 映射到 Line1
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_ILS);
        regVal |= intrMask;
        HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_ILS, regVal);
    }
}

/**
 * @brief Get interrupt line selection mapping.
 * @details 功能：读取 ILS 映射位图。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：中断线选择位图。
 */
uint32_t DL_MCAN_getIntrLineSelectStatus(const MCAN_Regs *mcan)
{
    return (HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_ILS));
}

/**
 * @brief Enable or disable one interrupt output line.
 * @details 功能：配置 ILE 中对应中断线使能位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] lineNum 参数：中断线号。
 * @param[in] enable 参数：true 使能，false 关闭。
 * @return 返回值：无。
 */
void DL_MCAN_enableIntrLine(MCAN_Regs *mcan, uint32_t lineNum, bool enable)
{
    uint32_t regVal;

    // 掩码限制有效中断线号
    lineNum &= MCANSS_INTR_LINE_EN_MASK;
    // 读-改-写 ILE 寄存器，设置/清除对应中断线使能
    regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_ILE);
    regVal &= ~((uint32_t)0x1U << lineNum);
    regVal |= ((uint32_t)enable << lineNum);
    HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_ILE, regVal);
}

/**
 * @brief Get MCAN interrupt status register.
 * @details 功能：读取 IR 中断状态位图。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：中断状态位图。
 */
uint32_t DL_MCAN_getIntrStatus(const MCAN_Regs *mcan)
{
    return (HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_IR));
}

/**
 * @brief Clear selected interrupt status and write EOI.
 * @details 功能：清除 IR 指定位并写入子系统 EOI。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] intrMask 参数：待清除中断掩码。
 * @param[in] eoi 参数：EOI 路由源编号。
 * @return 返回值：无。
 */
void DL_MCAN_clearIntrStatus(
    MCAN_Regs *mcan, uint32_t intrMask, DL_MCAN_INTR_SRC_MCAN eoi)
{
    HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_IR, intrMask);
    HW_WR_REG32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_EOI,
                (uint32_t)eoi);
}

/**
 * @brief Get high priority message info.
 * @details 功能：读取 HPMS 的缓冲区索引、过滤器索引和来源。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] hpm 参数：高优先级消息状态输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getHighPriorityMsgStatus(
    const MCAN_Regs *mcan, DL_MCAN_HighPriorityMsgInfo *hpm)
{
    hpm->bufIdx = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_HPMS, MCAN_HPMS_BIDX);
    hpm->msi = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_HPMS, MCAN_HPMS_MSI);
    hpm->filterIdx =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_HPMS, MCAN_HPMS_FIDX);
    hpm->filterList =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_HPMS, MCAN_HPMS_FLST);
}

/**
 * @brief Get RX FIFO runtime status.
 * @details 功能：按 FIFO 号读取填充深度、读写索引与溢出状态。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in,out] fifoStatus 参数：输入 FIFO 编号，输出状态信息。
 * @return 返回值：无。
 */
void DL_MCAN_getRxFIFOStatus(
    const MCAN_Regs *mcan, DL_MCAN_RxFIFOStatus *fifoStatus)
{
    uint32_t regVal;

    // 按 FIFO 编号读取对应状态寄存器并解析各字段
    switch (fifoStatus->num)
    {
    case ((uint32_t)DL_MCAN_RX_FIFO_NUM_0):
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_RXF0S);
        fifoStatus->fillLvl = HW_GET_FIELD(regVal, MCAN_RXF0S_F0FL);
        fifoStatus->getIdx = HW_GET_FIELD(regVal, MCAN_RXF0S_F0GI);
        fifoStatus->putIdx = HW_GET_FIELD(regVal, MCAN_RXF0S_F0PI);
        fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_RXF0S_F0F);
        fifoStatus->msgLost = HW_GET_FIELD(regVal, MCAN_RXF0S_RF0L);
        break;
    case ((uint32_t)DL_MCAN_RX_FIFO_NUM_1):
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_RXF1S);
        fifoStatus->fillLvl = HW_GET_FIELD(regVal, MCAN_RXF1S_F1FL);
        fifoStatus->getIdx = HW_GET_FIELD(regVal, MCAN_RXF1S_F1GI);
        fifoStatus->putIdx = HW_GET_FIELD(regVal, MCAN_RXF1S_F1PI);
        fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_RXF1S_F1F);
        fifoStatus->msgLost = HW_GET_FIELD(regVal, MCAN_RXF1S_RF1L);
        break;
    default:
        /* Invalid option */
        break;
    }
}

/**
 * @brief Acknowledge RX FIFO entry to release it.
 * @details 功能：写入 Rx FIFO 确认索引，释放已读取元素。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] fifoNum 参数：FIFO 编号（0 或 1）。
 * @param[in] idx 参数：待确认元素索引。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 索引越界。
 */
int32_t DL_MCAN_writeRxFIFOAck(MCAN_Regs *mcan, uint32_t fifoNum, uint32_t idx)
{
    int32_t status;
    uint32_t size;

    switch (fifoNum)
    {
    case ((uint32_t)DL_MCAN_RX_FIFO_NUM_0):
        size =
            HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF0C, MCAN_RXF0C_F0S);
        if (size >= idx)
        {
            // 中文说明：索引合法时写入 FIFO0 读确认，释放对应槽位。
            HW_WR_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXF0A, MCAN_RXF0A_F0AI, idx);
            status = STW_SOK;
        }
        else
        {
            status = STW_EFAIL;
        }
        break;
    case ((uint32_t)DL_MCAN_RX_FIFO_NUM_1):
        size =
            HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_RXF1C, MCAN_RXF1C_F1S);
        if (size >= idx)
        {
            // 中文说明：索引合法时写入 FIFO1 读确认，释放对应槽位。
            HW_WR_FIELD32(
                &mcan->MCANSS.MCAN.MCAN_RXF1A, MCAN_RXF1A_F1AI, idx);
            status = STW_SOK;
        }
        else
        {
            status = STW_EFAIL;
        }
        break;
    default:
        status = STW_EFAIL;
        break;
    }

    return status;
}

/**
 * @brief Get TX FIFO/Queue status.
 * @details 功能：读取空闲级别、读写索引与满标志。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] fifoStatus 参数：TX FIFO/Queue 状态输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getTxFIFOQueStatus(
    const MCAN_Regs *mcan, DL_MCAN_TxFIFOStatus *fifoStatus)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXFQS);
    fifoStatus->freeLvl = HW_GET_FIELD(regVal, MCAN_TXFQS_TFFL);
    fifoStatus->getIdx = HW_GET_FIELD(regVal, MCAN_TXFQS_TFGI);
    fifoStatus->putIdx = HW_GET_FIELD(regVal, MCAN_TXFQS_TFQP);
    fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_TXFQS_TFQF);
}

/**
 * @brief Get TX buffer request pending bitmap.
 * @details 功能：读取 TXBRP 挂起请求位图。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：请求挂起位图。
 */
uint32_t DL_MCAN_getTxBufReqPend(const MCAN_Regs *mcan)
{
    return (HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBRP));
}

/**
 * @brief Request TX buffer cancellation.
 * @details 功能：设置 TXBCR 对应 bit 发起发送取消请求。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] buffNum 参数：待取消缓冲区号。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数越界。
 */
int32_t DL_MCAN_txBufCancellationReq(MCAN_Regs *mcan, uint32_t buffNum)
{
    int32_t status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > buffNum)
    {
        // 读取 TXBCR 并置位对应缓冲区取消请求
        regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCR);
        regVal |= ((uint32_t)1U << buffNum);

        /*
         * For writing to TXBCR CCE bit should be '0'. This need not be
         * reverted because for other qualified writes this is locked state
         * and can't be written.
         */
        // 写 TXBCR 前需先锁定写保护（CCE=0）
        DL_MCAN_writeProtectedRegAccessLock(mcan);
        HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCR, regVal);

        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    return status;
}

/**
 * @brief Get TX buffer transmission occurred bitmap.
 * @details 功能：读取 TXBTO 发送完成位图。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：发送完成位图。
 */
uint32_t DL_MCAN_getTxBufTransmissionStatus(const MCAN_Regs *mcan)
{
    return (HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBTO));
}

/**
 * @brief Get TX buffer cancellation finished bitmap.
 * @details 功能：读取 TXBCF 取消完成位图。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：取消完成位图。
 */
uint32_t DL_MCAN_txBufCancellationStatus(const MCAN_Regs *mcan)
{
    return (HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCF));
}

/**
 * @brief Enable or disable TX buffer transmission interrupt.
 * @details 功能：配置 TXBTIE 指定位中断开关。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] bufNum 参数：缓冲区号。
 * @param[in] enable 参数：true 使能，false 关闭。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数越界。
 */
int32_t DL_MCAN_TXBufTransIntrEnable(
    MCAN_Regs *mcan, uint32_t bufNum, bool enable)
{
    int32_t status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > bufNum)
    {
        if (true == enable)
        {
            regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBTIE);
            regVal |= ((uint32_t)1U << bufNum);
            HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_TXBTIE, regVal);
        }
        else
        {
            regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBTIE);
            regVal &= ~((uint32_t)0x1U << bufNum);
            HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_TXBTIE, regVal);
        }
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    return status;
}

/**
 * @brief Enable or disable TX buffer cancellation interrupt.
 * @details 功能：配置 TXBCIE 指定位中断开关。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] bufNum 参数：缓冲区号。
 * @param[in] enable 参数：true 使能，false 关闭。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 参数越界。
 */
int32_t DL_MCAN_getTxBufCancellationIntrEnable(
    const MCAN_Regs *mcan, uint32_t bufNum, bool enable)
{
    int32_t status;
    uint32_t regVal;

    if (MCANSS_TX_BUFFER_MAX > bufNum)
    {
        if (true == enable)
        {
            regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCIE);
            regVal |= ((uint32_t)0x1U << bufNum);
            HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCIE, regVal);
        }
        else
        {
            regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCIE);
            regVal &= ~((uint32_t)0x1U << bufNum);
            HW_WR_REG32(&mcan->MCANSS.MCAN.MCAN_TXBCIE, regVal);
        }
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }
    return status;
}

/**
 * @brief Get TX event FIFO status.
 * @details 功能：读取 Tx Event FIFO 深度、索引与丢失标志。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] fifoStatus 参数：Tx Event FIFO 状态输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getTxEventFIFOStatus(
    const MCAN_Regs *mcan, DL_MCAN_TxEventFIFOStatus *fifoStatus)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_TXEFS);
    fifoStatus->fillLvl = HW_GET_FIELD(regVal, MCAN_TXEFS_EFFL);
    fifoStatus->getIdx = HW_GET_FIELD(regVal, MCAN_TXEFS_EFGI);
    fifoStatus->putIdx = HW_GET_FIELD(regVal, MCAN_TXEFS_EFPI);
    fifoStatus->fifoFull = HW_GET_FIELD(regVal, MCAN_TXEFS_EFF);
    fifoStatus->eleLost = HW_GET_FIELD(regVal, MCAN_TXEFS_TEFL);
}

/**
 * @brief Add or remove clock stop request.
 * @details 功能：设置/清除 CCCR.CSR 请求 MCAN 时钟停止。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] enable 参数：true 请求停止，false 取消请求。
 * @return 返回值：无。
 */
void DL_MCAN_addClockStopRequest(MCAN_Regs *mcan, bool enable)
{
    if (true == enable)
    {
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_CSR, 0x1U);
    }
    else
    {
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_CSR, 0x0U);
    }
}

/**
 * @brief Acknowledge one entry in TX Event FIFO.
 * @details 功能：写入 TXEFA 确认索引，释放对应 Tx Event FIFO 槽位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] idx 参数：待确认元素索引。
 * @return 返回值：`STW_SOK` 成功，`STW_EFAIL` 索引越界。
 */
int32_t DL_MCAN_writeTxEventFIFOAck(MCAN_Regs *mcan, uint32_t idx)
{
    int32_t status;
    uint32_t size;

    size = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFC, MCAN_TXEFC_EFS);

    if (size >= idx)
    {
        HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TXEFA, MCAN_TXEFA_EFAI, idx);
        status = STW_SOK;
    }
    else
    {
        status = STW_EFAIL;
    }

    return status;
}

/**
 * @brief Force ECC error injection.
 * @details 功能：按配置注入 ECC 单比特/双比特错误。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] eccErr 参数：ECC 错误注入配置结构体。
 * @return 返回值：无。
 */
void DL_MCAN_eccForceError(
    MCAN_Regs *mcan, const DL_MCAN_ECCErrForceParams *eccErr)
{
    uint32_t regVal;

    if ((eccErr->errType == ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC)) ||
        (eccErr->errType == ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED)))
    {
        // 中文说明：分三步写入行号、bit 位与注错控制字段，触发 ECC 注错。
        DL_MCAN_eccLoadRegister(
            mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                      .MCANERR_ERR_CTRL1);
        regVal = HW_RD_REG32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                                  .MCANERR_ERR_CTRL1);
        HW_SET_FIELD32(regVal, MCAN_ERR_CTRL1_ECC_ROW, eccErr->rowNum);
        HW_WR_REG32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                         .MCANERR_ERR_CTRL1,
                    regVal);
        DL_MCAN_eccLoadRegister(
            mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                      .MCANERR_ERR_CTRL2);
        regVal = HW_RD_REG32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                                  .MCANERR_ERR_CTRL2);
        HW_SET_FIELD32(regVal, MCAN_ERR_CTRL2_ECC_BIT1, eccErr->bit1);
        HW_SET_FIELD32(regVal, MCAN_ERR_CTRL2_ECC_BIT2, eccErr->bit2);
        HW_WR_REG32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                         .MCANERR_ERR_CTRL2,
                    regVal);
        DL_MCAN_eccLoadRegister(
            mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                      .MCANERR_CTRL);
        regVal = HW_RD_REG32(
            &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL);
        HW_SET_FIELD32(regVal, MCAN_TI_WRAPPER_ECC_REGS_CTRL_FORCE_N_ROW,
                       eccErr->errForce);
        HW_SET_FIELD32(
            regVal, MCAN_TI_WRAPPER_ECC_REGS_CTRL_ERROR_ONCE, eccErr->errOnce);
        if (eccErr->errType == ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC))
        {
            // 中文说明：单比特纠错注入。
            HW_SET_FIELD32(
                regVal, MCAN_TI_WRAPPER_ECC_REGS_CTRL_FORCE_SEC, 0x1U);
        }
        else
        {
            /* eccErr->errType == ((uint32_t) DL_MCAN_ECC_ERR_TYPE_DED) */
            // 中文说明：双比特检错注入。
            HW_SET_FIELD32(
                regVal, MCAN_TI_WRAPPER_ECC_REGS_CTRL_FORCE_DED, 0x1U);
        }
        HW_WR_REG32(
            &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL,
            regVal);
        DL_MCAN_eccLoadRegister(
            mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                      .MCANERR_CTRL);
    }
}

/**
 * @brief Read MCAN ECC error status.
 * @details 功能：读取 ECC 单/双比特错误标志、错误位与错误行地址。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] eccErr 参数：ECC 错误状态输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_eccGetErrorStatus(MCAN_Regs *mcan, DL_MCAN_ECCErrStatus *eccErr)
{
    uint32_t regVal;

    DL_MCAN_eccLoadRegister(
        mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                  .MCANERR_ERR_STAT1);
    regVal = HW_RD_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_ERR_STAT1);
    eccErr->secErr = HW_GET_FIELD(regVal, MCAN_ERR_STAT1_ECC_SEC);
    eccErr->dedErr = HW_GET_FIELD(regVal, MCAN_ERR_STAT1_ECC_DED);
    eccErr->bit1 = HW_GET_FIELD(regVal, MCAN_ERR_STAT1_ECC_BIT1);
    DL_MCAN_eccLoadRegister(
        mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                  .MCANERR_ERR_STAT2);
    regVal = HW_RD_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_ERR_STAT2);
    eccErr->row = HW_GET_FIELD(regVal, MCAN_ERR_STAT2_ECC_ROW);
}

/**
 * @brief Clear MCAN ECC error status.
 * @details 功能：按错误类型清除 ECC 状态位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] errType 参数：错误类型（SEC/DED）。
 * @return 返回值：无。
 */
void DL_MCAN_eccClearErrorStatus(MCAN_Regs *mcan, uint32_t errType)
{
    DL_MCAN_eccLoadRegister(
        mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                  .MCANERR_ERR_STAT1);
    switch (errType)
    {
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC):
        HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                           .MCANERR_ERR_STAT1,
                      MCAN_ERR_STAT1_ECC_SEC, 0x1U);
        break;
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED):
        HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                           .MCANERR_ERR_STAT1,
                      MCAN_ERR_STAT1_ECC_DED, 0x1U);
        break;
    default:
        /* Invalid option */
        break;
    }
    DL_MCAN_eccLoadRegister(
        mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                  .MCANERR_ERR_STAT1);
}

/**
 * @brief Write ECC interrupt EOI.
 * @details 功能：向 SEC/DED EOI 寄存器写 1，结束当前 ECC 中断服务。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] errType 参数：ECC 错误类型（SEC/DED）。
 * @return 返回值：无。
 */
void DL_MCAN_eccWriteEOI(MCAN_Regs *mcan, uint32_t errType)
{
    switch (errType)
    {
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC):
        HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                           .MCANERR_SEC_EOI,
                      MCAN_SEC_EOI_EOI_WR, 0x1U);
        break;
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED):
        HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                           .MCANERR_DED_EOI,
                      MCAN_DED_EOI_EOI_WR, 0x1U);
        break;
    default:
        /* Invalid option */
        break;
    }
}

/**
 * @brief Enable or disable ECC interrupt.
 * @details 功能：按错误类型通过 SET/CLR 寄存器控制 ECC 中断开关。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] errType 参数：ECC 错误类型（SEC/DED）。
 * @param[in] enable 参数：true 使能，false 关闭。
 * @return 返回值：无。
 */
void DL_MCAN_eccEnableIntr(MCAN_Regs *mcan, uint32_t errType, bool enable)
{
    if (true == enable)
    {
        // 中文说明：按错误类型写使能 SET 寄存器。
        switch (errType)
        {
        case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC):
            HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                               .MCANERR_SEC_ENABLE_SET,
                          MCAN_SEC_ENABLE_SET_MSGMEM_ENABLE_SET, 0x1U);
            break;
        case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED):
            HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                               .MCANERR_DED_ENABLE_SET,
                          MCAN_DED_ENABLE_SET_MSGMEM_ENABLE_SET, 0x1U);
            break;
        default:
            /* Invalid option */
            break;
        }
    }
    else
    {
        // 中文说明：按错误类型写使能 CLR 寄存器。
        switch (errType)
        {
        case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC):
            HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                               .MCANERR_SEC_ENABLE_CLR,
                          MCAN_SEC_ENABLE_CLR_MSGMEM_ENABLE_CLR, 0x1U);
            break;
        case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED):
            HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                               .MCANERR_DED_ENABLE_CLR,
                          MCAN_DED_ENABLE_CLR_MSGMEM_ENABLE_CLR, 0x1U);
            break;
        default:
            /* Invalid option */
            break;
        }
    }
}

/**
 * @brief Get ECC interrupt pending status.
 * @details 功能：读取 SEC/DED 对应挂起标志位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] errType 参数：ECC 错误类型（SEC/DED）。
 * @return 返回值：挂起状态位（0/1）。
 */
uint32_t DL_MCAN_eccGetIntrStatus(const MCAN_Regs *mcan, uint32_t errType)
{
    uint32_t retVal = 0U;

    switch (errType)
    {
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC):
        retVal = HW_RD_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS
                                    .MCAN_ECC_REGS.MCANERR_SEC_STATUS,
                               MCAN_SEC_STATUS_MSGMEM_PEND);
        break;
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED):
        retVal = HW_RD_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS
                                    .MCAN_ECC_REGS.MCANERR_DED_STATUS,
                               MCAN_DED_STATUS_MSGMEM_PEND);
        break;
    default:
        retVal = 0U;
        break;
    }
    return retVal;
}

/**
 * @brief Clear ECC interrupt pending status.
 * @details 功能：向对应状态位写 1 清除 SEC/DED 中断挂起。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] errType 参数：ECC 错误类型（SEC/DED）。
 * @return 返回值：无。
 */
void DL_MCAN_eccClearIntrStatus(MCAN_Regs *mcan, uint32_t errType)
{
    switch (errType)
    {
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_SEC):
        HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                           .MCANERR_SEC_STATUS,
                      MCAN_SEC_STATUS_MSGMEM_PEND, 0x1U);
        break;
    case ((uint32_t)DL_MCAN_ECC_ERR_TYPE_DED):
        HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                           .MCANERR_DED_STATUS,
                      MCAN_DED_STATUS_MSGMEM_PEND, 0x1U);
        break;
    default:
        break;
    }
}

/**
 * @brief Configure external timestamp counter prescaler.
 * @details 功能：设置外部时间戳计数器预分频值。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] prescalar 参数：预分频值。
 * @return 返回值：无。
 */
void DL_MCAN_extTSCounterConfig(MCAN_Regs *mcan, uint32_t prescalar)
{
    HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS
                       .MCANSS_EXT_TS_PRESCALER,
                  MCAN_EXT_TS_PRESCALER_PRESCALER, prescalar);
}

/**
 * @brief Enable or disable external timestamp counter.
 * @details 功能：控制外部时间戳计数器运行开关。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] enable 参数：true 使能，false 关闭。
 * @return 返回值：无。
 */
void DL_MCAN_extTSCounterEnable(MCAN_Regs *mcan, bool enable)
{
    HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_CTRL,
                  MCAN_TI_WRAPPER_REGS_CTRL_EXT_TS_CNTR_EN, enable);
}

/**
 * @brief Enable or disable external timestamp overflow interrupt.
 * @details 功能：控制外部时间戳溢出中断源使能。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] enable 参数：true 使能，false 关闭。
 * @return 返回值：无。
 */
void DL_MCAN_extTSEnableIntr(MCAN_Regs *mcan, bool enable)
{
    if (true == enable)
    {
        // 中文说明：使能外部时间戳溢出中断源。
        HW_WR_FIELD32(
            &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_IE,
            MCAN_TI_WRAPPER_PROCESSORS_REGS_IE_EXT_TS_CNTR_OVFL, 1U);
    }
    else
    {
        // 中文说明：通过 IECS 清除位关闭外部时间戳溢出中断源。
        HW_WR_FIELD32(
            &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_IECS,
            MCAN_IECS_EXT_TS_CNTR_OVFL, 1U);
    }
}

/**
 * @brief Write EOI for external timestamp overflow interrupt.
 * @details 功能：通知中断控制逻辑该中断已处理完成。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：无。
 */
void DL_MCAN_extTSWriteEOI(MCAN_Regs *mcan)
{
    HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_EOI,
                  MCAN_EOI_EOI, 0x1U);
}

/**
 * @brief Get unserviced external timestamp overflow interrupt count.
 * @details 功能：读取外部时间戳溢出未服务计数。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：未服务中断计数。
 */
uint32_t DL_MCAN_extTSGetUnservicedIntrCount(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS
                               .MCANSS_EXT_TS_UNSERVICED_INTR_CNTR,
                          MCAN_EXT_TS_UNSERVICED_INTR_CNTR_EXT_TS_INTR_CNTR));
}

/* ========================================================================== */
/*                          Advance Functions                                 */
/* ========================================================================== */

/**
 * @brief Get MCAN and wrapper revision information.
 * @details 功能：读取 MCANSS 与 MCAN 核心版本字段并填充输出结构体。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] revId 参数：版本信息输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getRevisionId(const MCAN_Regs *mcan, DL_MCAN_RevisionId *revId)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_PID);
    revId->minor = HW_GET_FIELD(regVal, MCAN_PID_MINOR);
    revId->major = HW_GET_FIELD(regVal, MCAN_PID_MAJOR);
    revId->modId = HW_GET_FIELD(regVal, MCAN_PID_MODULE_ID);
    revId->scheme = HW_GET_FIELD(regVal, MCAN_PID_SCHEME);

    regVal = HW_RD_REG32(&mcan->MCANSS.MCAN.MCAN_CREL);
    revId->day = HW_GET_FIELD(regVal, MCAN_CREL_DAY);
    revId->mon = HW_GET_FIELD(regVal, MCAN_CREL_MON);
    revId->year = HW_GET_FIELD(regVal, MCAN_CREL_YEAR);
    revId->subStep = HW_GET_FIELD(regVal, MCAN_CREL_SUBSTEP);
    revId->step = HW_GET_FIELD(regVal, MCAN_CREL_STEP);
    revId->rel = HW_GET_FIELD(regVal, MCAN_CREL_REL);
}

/**
 * @brief Get MCAN clock-stop request acknowledge status.
 * @details 功能：读取 CSR 位判断时钟停止请求确认状态。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：CSR 位值。
 */
uint32_t DL_MCAN_getClockStopAck(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_CSR));
}

/**
 * @brief Set external timestamp overflow raw status.
 * @details 功能：软件置位外部时间戳溢出原始状态位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：无。
 */
void DL_MCAN_extTSSetRawStatus(MCAN_Regs *mcan)
{
    HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_IRS,
                  MCAN_IRS_EXT_TS_CNTR_OVFL, 1U);
}

/**
 * @brief Clear external timestamp overflow raw status.
 * @details 功能：软件清除外部时间戳溢出原始状态位。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：无。
 */
void DL_MCAN_extTSClearRawStatus(MCAN_Regs *mcan)
{
    HW_WR_FIELD32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_ICS,
                  MCAN_ICS_EXT_TS_CNTR_OVFL, 1U);
}

/**
 * @brief Get MCAN RX pin sampled state.
 * @details 功能：读取 TEST.RX 监测值。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：RX 引脚状态。
 */
uint32_t DL_MCAN_getRxPinState(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TEST, MCAN_TEST_RX));
}

/**
 * @brief Set MCAN TX pin forced state.
 * @details 功能：在测试模式下设置 TX 引脚输出状态。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] state 参数：TX 引脚状态编码。
 * @return 返回值：无。
 */
void DL_MCAN_setTxPinState(MCAN_Regs *mcan, uint32_t state)
{
    DL_MCAN_writeProtectedRegAccessUnlock(mcan);

    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_TEST, 0x1U);
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TEST, MCAN_TEST_TX, state);

    DL_MCAN_writeProtectedRegAccessLock(mcan);
}

/**
 * @brief Get MCAN TX pin forced state.
 * @details 功能：读取 TEST.TX 字段。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：TX 引脚状态编码。
 */
uint32_t DL_MCAN_getTxPinState(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TEST, MCAN_TEST_TX));
}

/**
 * @brief Get timestamp counter value.
 * @details 功能：读取 TSCV 计数值。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：时间戳计数值。
 */
uint32_t DL_MCAN_getTSCounterVal(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TSCV, MCAN_TSCV_TSC));
}

/**
 * @brief Get clock-stop acknowledge state.
 * @details 功能：读取 CSA 位判断时钟停止确认状态。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：CSA 位值。
 */
uint32_t DL_MCAN_getClkStopAck(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_CSA));
}

/**
 * @brief Read configured nominal/data bit timing parameters.
 * @details 功能：从 NBTP/DBTP 寄存器提取位时序参数。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] configParams 参数：位时序输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_getBitTime(
    const MCAN_Regs *mcan, DL_MCAN_BitTimingParams *configParams)
{
    configParams->nomSynchJumpWidth =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NSJW);
    configParams->nomTimeSeg2 =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NTSEG2);
    configParams->nomTimeSeg1 =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NTSEG1);
    configParams->nomRatePrescalar =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_NBTP, MCAN_NBTP_NBRP);

    configParams->dataSynchJumpWidth =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DSJW);
    configParams->dataTimeSeg2 =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DTSEG2);
    configParams->dataTimeSeg1 =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DTSEG1);
    configParams->dataRatePrescalar =
        HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_DBTP, MCAN_DBTP_DBRP);
}

/**
 * @brief Reset timestamp counter value.
 * @details 功能：将 TSCV 清零。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：无。
 */
void DL_MCAN_resetTSCounter(MCAN_Regs *mcan)
{
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_TSCV, MCAN_TSCV_TSC, 0x0U);
}

/**
 * @brief Get timeout counter value.
 * @details 功能：读取 TOCV 当前值。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：超时计数值。
 */
uint32_t DL_MCAN_getTOCounterVal(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_TOCV, MCAN_TOCV_TOC));
}

/**
 * @brief Get ECC aggregator revision information.
 * @details 功能：读取 ECC AGGR 版本字段并写入输出结构体。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] revId 参数：ECC AGGR 版本输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_eccAggrGetRevisionId(
    const MCAN_Regs *mcan, DL_MCAN_ECCAggrRevisionId *revId)
{
    uint32_t regVal;

    regVal = HW_RD_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_REV);
    revId->minor = HW_GET_FIELD(regVal, MCAN_REV_REVMIN);
    revId->major = HW_GET_FIELD(regVal, MCAN_REV_REVMAJ);
    revId->modId = HW_GET_FIELD(regVal, MCAN_REV_MODULE_ID);
    revId->scheme = HW_GET_FIELD(regVal, MCAN_REV_SCHEME);
}

/**
 * @brief Get ECC wrapper revision information.
 * @details 功能：通过 ECC 载入流程读取 wrapper 版本字段。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] revId 参数：ECC wrapper 版本输出结构体。
 * @return 返回值：无。
 */
void DL_MCAN_eccWrapGetRevisionId(
    MCAN_Regs *mcan, DL_MCAN_ECCWrapRevisionId *revId)
{
    uint32_t regVal;

    DL_MCAN_eccLoadRegister(
        mcan, (uint32_t)&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                  .MCANERR_WRAP_REV);
    regVal = HW_RD_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_WRAP_REV);
    revId->minor = HW_GET_FIELD(regVal, MCAN_WRAP_REV_REVMIN);
    revId->major = HW_GET_FIELD(regVal, MCAN_WRAP_REV_REVMAJ);
    revId->modId = HW_GET_FIELD(regVal, MCAN_WRAP_REV_MODULE_ID);
    revId->scheme = HW_GET_FIELD(regVal, MCAN_WRAP_REV_SCHEME);
}

/**
 * @brief Check whether external timestamp overflow interrupt is enabled.
 * @details 功能：读取 IES 中溢出中断使能状态。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：true 已使能，false 未使能。
 */
bool DL_MCAN_extTSIsIntrEnable(const MCAN_Regs *mcan)
{
    bool status;

    if (1U == HW_RD_FIELD32(
                  &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_IES,
                  MCAN_IES_EXT_TS_CNTR_OVFL))
    {
        status = true;
    }
    else
    {
        status = false;
    }

    return status;
}

/**
 * @brief Get MCAN endian test value.
 * @details 功能：读取 ENDN 寄存器常量用于端序校验。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：端序标识值。
 */
uint32_t DL_MCAN_getEndianVal(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_ENDN, MCAN_ENDN_ETV));
}

/**
 * @brief Get extended ID AND mask value.
 * @details 功能：读取 XIDAM 扩展 ID 掩码配置。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：扩展 ID 掩码。
 */
uint32_t DL_MCAN_getExtIDANDMask(const MCAN_Regs *mcan)
{
    return (HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_XIDAM, MCAN_XIDAM_EIDM));
}

/* ========================================================================== */
/*                          Internal Functions                                */
/* ========================================================================== */

/**
 * @brief Unlock write-protected registers.
 * @details 功能：设置 CCE 位解锁写保护寄存器访问。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：无。
 */
static void DL_MCAN_writeProtectedRegAccessUnlock(MCAN_Regs *mcan)
{
    // 设置 CCE=1 允许写入受保护配置寄存器
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_CCE, 0x1U);
}

/**
 * @brief Lock write-protected registers.
 * @details 功能：清除 CCE 位锁定写保护寄存器访问。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @return 返回值：无。
 */
static void DL_MCAN_writeProtectedRegAccessLock(MCAN_Regs *mcan)
{
    // 清除 CCE=0 禁止写入受保护配置寄存器
    HW_WR_FIELD32(&mcan->MCANSS.MCAN.MCAN_CCCR, MCAN_CCCR_CCE, 0x0U);
}

/**
 * @brief Load ECC register via SVBUS read.
 * @details 功能：通过 SVBUS 从 ECC 存储区加载目标寄存器。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in] regOffset 参数：目标寄存器偏移地址。
 * @return 返回值：无。
 */
static void DL_MCAN_eccLoadRegister(MCAN_Regs *mcan, uint32_t regOffset)
{
    uint32_t regVal = 0U, offset;

    offset = regOffset & 0xFFU;
    regVal |= ((uint32_t)MCANSS_MSG_RAM_NUM << MCAN_VECTOR_ECC_VECTOR_OFS);
    regVal |= (offset << MCAN_VECTOR_RD_SVBUS_ADDRESS_OFS);
    regVal |= ((uint32_t)1U << MCAN_VECTOR_RD_SVBUS_OFS);
    HW_WR_REG32(
        &mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_VECTOR,
        regVal);
    // 中文说明：等待 SVBUS 读回完成，确保后续访问目标 ECC 寄存器有效。
    while (MCAN_VECTOR_RD_SVBUS_DONE_MASK !=
           (HW_RD_REG32(&mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS
                             .MCANERR_VECTOR) &
            MCAN_VECTOR_RD_SVBUS_DONE_MASK))
    {
    }
}

/**
 * @brief Read RX message from message RAM.
 * @details 功能：从消息 RAM 指定地址读取接收报文对象。
 * @param[in] mcan 参数：MCAN 寄存器基地址。
 * @param[in] elemAddr 参数：消息对象地址偏移。
 * @param[out] elem 参数：接收报文输出结构体。
 * @return 返回值：无。
 */
static void DL_MCAN_readMsg(
    uint32_t mcan, uint32_t elemAddr, DL_MCAN_RxBufElement *elem)
{
    uint32_t regVal = 0U, loopCnt = 0U;

    regVal = HW_RD_REG32(((uint32_t)mcan + (uint32_t)elemAddr));
    elem->id = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_ID_MASK) >>
                          MCANSS_RX_BUFFER_ELEM_ID_SHIFT);
    elem->rtr = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_RTR_MASK) >>
                           MCANSS_RX_BUFFER_ELEM_RTR_SHIFT);
    elem->xtd = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_XTD_MASK) >>
                           MCANSS_RX_BUFFER_ELEM_XTD_SHIFT);
    elem->esi = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_ESI_MASK) >>
                           MCANSS_RX_BUFFER_ELEM_ESI_SHIFT);

    elemAddr += 4U;
    regVal = HW_RD_REG32(((uint32_t)mcan + (uint32_t)elemAddr));
    elem->rxts = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_RXTS_MASK) >>
                            MCANSS_RX_BUFFER_ELEM_RXTS_SHIFT);
    elem->dlc = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_DLC_MASK) >>
                           MCANSS_RX_BUFFER_ELEM_DLC_SHIFT);
    elem->brs = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_BRS_MASK) >>
                           MCANSS_RX_BUFFER_ELEM_BRS_SHIFT);
    elem->fdf = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_FDF_MASK) >>
                           MCANSS_RX_BUFFER_ELEM_FDF_SHIFT);
    elem->fidx = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_FIDX_MASK) >>
                            MCANSS_RX_BUFFER_ELEM_FIDX_SHIFT);
    elem->anmf = (uint32_t)((regVal & MCANSS_RX_BUFFER_ELEM_ANMF_MASK) >>
                            MCANSS_RX_BUFFER_ELEM_ANMF_SHIFT);
    elemAddr += 4U;

    loopCnt = 0U;
    /* Reading words from message RAM and forming payload bytes out of it */
    while ((4U <= (DL_MCAN_getDataSize(elem->dlc) - loopCnt)) &&
           (0U != (DL_MCAN_getDataSize(elem->dlc) - loopCnt)))
    {
        // 中文说明：按 32bit 读取后拆分为 4 个字节写入 data[]。
        regVal = HW_RD_REG32(((uint32_t)mcan + (uint32_t)elemAddr));
        elem->data[loopCnt] = (uint16_t)(regVal & 0x000000FFU);
        elem->data[(loopCnt + 1U)] = (uint16_t)((regVal & 0x0000FF00U) >> 8U);
        elem->data[(loopCnt + 2U)] = (uint16_t)((regVal & 0x00FF0000U) >> 16U);
        elem->data[(loopCnt + 3U)] = (uint16_t)((regVal & 0xFF000000U) >> 24U);
        elemAddr += 4U;
        loopCnt += 4U;
    }
    /* Reading remaining bytes from message RAM */
    if (0U < (DL_MCAN_getDataSize(elem->dlc) - loopCnt))
    {
        // 中文说明：处理不足 4 字节的尾数据。
        regVal = HW_RD_REG32(((uint32_t)mcan + (uint32_t)elemAddr));
        elem->data[loopCnt] = (uint16_t)(regVal & 0x000000FFU);
        elem->data[(loopCnt + 1U)] = (uint16_t)((regVal & 0x0000FF00U) >> 8U);
        elem->data[(loopCnt + 2U)] = (uint16_t)((regVal & 0x00FF0000U) >> 16U);
    }
}

/**
 * @brief Write TX message to message RAM.
 * @details 功能：将发送报文对象写入消息 RAM 指定地址。
 * @param[in] mcan 参数：MCAN 寄存器基地址。
 * @param[in] elemAddr 参数：消息对象地址偏移。
 * @param[in] elem 参数：发送报文输入结构体。
 * @return 返回值：无。
 */
static void DL_MCAN_writeMsg(
    uint32_t mcan, uint32_t elemAddr, const DL_MCAN_TxBufElement *elem)
{
    uint32_t regVal = 0, loopCnt = 0U;

    // 第一个字：ID、RTR、XTD、ESI 字段
    regVal = 0U;
    regVal |= (((uint32_t)(elem->id << MCANSS_TX_BUFFER_ELEM_ID_SHIFT)) |
               ((uint32_t)(elem->rtr << MCANSS_TX_BUFFER_ELEM_RTR_SHIFT)) |
               ((uint32_t)(elem->xtd << MCANSS_TX_BUFFER_ELEM_XTD_SHIFT)) |
               ((uint32_t)(elem->esi << MCANSS_TX_BUFFER_ELEM_ESI_SHIFT)));
    HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);
    elemAddr += 4U;

    regVal = 0U;
    regVal |= ((uint32_t)(elem->dlc << MCANSS_TX_BUFFER_ELEM_DLC_SHIFT)) |
              ((uint32_t)(elem->brs << MCANSS_TX_BUFFER_ELEM_BRS_SHIFT)) |
              ((uint32_t)(elem->fdf << MCANSS_TX_BUFFER_ELEM_FDF_SHIFT)) |
              ((uint32_t)(elem->efc << MCANSS_TX_BUFFER_ELEM_EFC_SHIFT)) |
              ((uint32_t)(elem->mm << MCANSS_TX_BUFFER_ELEM_MM_SHIFT));
    HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);
    elemAddr += 4U;

    loopCnt = 0U;
    /* Framing words out of the payload bytes and writing it to message RAM */
    while ((4U <= (DL_MCAN_getDataSize(elem->dlc) - loopCnt)) &&
           (0U != (DL_MCAN_getDataSize(elem->dlc) - loopCnt)))
    {
        // 中文说明：每 4 字节打包为 1 个 32bit 字写入消息 RAM。
        regVal = 0U;
        regVal |= ((uint32_t)elem->data[loopCnt] |
                   ((uint32_t)elem->data[(loopCnt + 1U)] << 8U) |
                   ((uint32_t)elem->data[(loopCnt + 2U)] << 16U) |
                   ((uint32_t)elem->data[(loopCnt + 3U)] << 24U));
        HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);
        elemAddr += 4U;
        loopCnt += 4U;
    }
    /* Framing a word out of remaining payload bytes and writing it to
     * message RAM */
    if (0U < (DL_MCAN_getDataSize(elem->dlc) - loopCnt))
    {
        // 中文说明：处理最后不足 4 字节的数据打包。
        regVal = 0U;
        regVal |= ((uint32_t)elem->data[loopCnt] |
                   ((uint32_t)elem->data[(loopCnt + 1U)] << 8U) |
                   ((uint32_t)elem->data[(loopCnt + 2U)] << 16U) |
                   ((uint32_t)elem->data[(loopCnt + 3U)] << 24U));
        HW_WR_REG32(((uint32_t)mcan + (uint32_t)elemAddr), regVal);
    }
}

/**
 * @brief Get payload size from DLC.
 * @details 功能：根据 DLC 编码查表返回实际有效载荷字节数。
 * @param[in] dlc 参数：数据长度码（0-15）。
 * @return 返回值：有效载荷字节数。
 */
static uint32_t DL_MCAN_getDataSize(uint32_t dlc)
{
    // DLC 到实际字节数映射表
    uint32_t dataSize[16] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
    return (dataSize[dlc]);
}

/**
 * @brief Get message object size from element size code.
 * @details 功能：根据元素尺寸编码查表返回消息 RAM 中对象的 32 位字数。
 * @param[in] elemSize 参数：元素尺寸编码（0-7）。
 * @return 返回值：消息对象字大小。
 */
static uint32_t DL_MCAN_getMsgObjSize(uint32_t elemSize)
{
    // 元素尺寸编码到字大小映射表
    uint32_t objSize[8] = {4, 5, 6, 7, 8, 10, 14, 18};
    return (objSize[elemSize]);
}

/**
 * @brief Read 32-bit register.
 * @details 功能：从指定地址读取 32 位寄存器值。
 * @param[in] addr 参数：寄存器地址。
 * @return 返回值：寄存器值。
 */
__STATIC_INLINE uint32_t HW_RD_REG32_RAW(uint32_t addr)
{
    uint32_t regVal = *(volatile uint32_t *)addr;
    return (regVal);
}

/**
 * @brief Write 32-bit register.
 * @details 功能：向指定地址写入 32 位值。
 * @param[in] addr 参数：寄存器地址。
 * @param[in] value 参数：待写入值。
 * @return 返回值：无。
 */
__STATIC_INLINE void HW_WR_REG32_RAW(uint32_t addr, uint32_t value)
{
    *(volatile uint32_t *)addr = value;
    return;
}

/**
 * @brief Read-modify-write register bit field.
 * @details 功能：读取寄存器、修改指定位域后写回。
 * @param[in] addr 参数：寄存器地址。
 * @param[in] mask 参数：位域掩码。
 * @param[in] shift 参数：位域移位量。
 * @param[in] value 参数：待写入位域值。
 * @return 返回值：无。
 */
__STATIC_INLINE void HW_WR_FIELD32_RAW(
    uint32_t addr, uint32_t mask, uint32_t shift, uint32_t value)
{
    uint32_t regVal = *(volatile uint32_t *)addr;
    // 清除目标位域，再写入新值
    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    *(volatile uint32_t *)addr = regVal;
    return;
}
/**
 * @brief Read register bit field.
 * @details 功能：读取寄存器并提取指定位域值。
 * @param[in] addr 参数：寄存器地址。
 * @param[in] mask 参数：位域掩码。
 * @param[in] shift 参数：位域移位量。
 * @return 返回值：位域值（已右移到 LSB）。
 */
__STATIC_INLINE uint32_t HW_RD_FIELD32_RAW(
    uint32_t addr, uint32_t mask, uint32_t shift)
{
    uint32_t regVal = *(volatile uint32_t *)addr;
    regVal = (regVal & mask) >> shift;
    return (regVal);
}

/**
 * @brief Save MCAN configuration snapshot.
 * @details 功能：将时钟、中断与 MCAN 关键寄存器备份到结构体，供低功耗恢复使用。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[out] ptr 参数：备份配置结构体。
 * @return 返回值：true 表示本次成功保存，false 表示已有有效备份未覆盖。
 */
bool DL_MCAN_saveConfiguration(
    const MCAN_Regs *mcan, DL_MCAN_backupConfig *ptr)
{
    // 仅在尚无有效备份时执行保存
    bool saveState = !ptr->backupRdy;
    if (saveState)
    {
        /* Clock configuration 时钟配置 */
        ptr->clkDivConf = mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKDIV;
        ptr->clkDivConf = mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKDIV;
        ptr->clkConf = mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKCTL;
        ptr->clkEnConf = mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKEN;

        /* Interrupt Configuration */
        ptr->txBuffTxIntConf = mcan->MCANSS.MCAN.MCAN_TXBTIE;
        ptr->txBuffCancIntConf = mcan->MCANSS.MCAN.MCAN_TXBCIE;
        ptr->intLnSelConf = mcan->MCANSS.MCAN.MCAN_ILS;
        ptr->intLnEnableConf = mcan->MCANSS.MCAN.MCAN_ILE;
        ptr->ssIntEnConf =
            mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_IE;
        ptr->intEnConf = mcan->MCANSS.MCAN.MCAN_IE;
        ptr->intEvnt0Conf = mcan->MCANSS.TI_WRAPPER.MSP.CPU_INT.IMASK;

        /* MCAN configuration */
        ptr->ctrlConf = mcan->MCANSS.MCAN.MCAN_CCCR;
        ptr->nomBitTimeConf = mcan->MCANSS.MCAN.MCAN_NBTP;
        ptr->dataBitTimeConf = mcan->MCANSS.MCAN.MCAN_DBTP;
        ptr->timeCntConf = mcan->MCANSS.MCAN.MCAN_TSCC;
        ptr->timeCntVal = mcan->MCANSS.MCAN.MCAN_TSCV;
        ptr->timeOutConf = mcan->MCANSS.MCAN.MCAN_TOCC;
        ptr->timeOutCntVal = mcan->MCANSS.MCAN.MCAN_TOCV;
        ptr->txDelCompConf = mcan->MCANSS.MCAN.MCAN_TDCR;
        ptr->globFiltIDConf = mcan->MCANSS.MCAN.MCAN_GFC;
        ptr->stdFiltIDConf = mcan->MCANSS.MCAN.MCAN_SIDFC;
        ptr->exFiltIDConf = mcan->MCANSS.MCAN.MCAN_XIDFC;
        ptr->exFiltIDMsk = mcan->MCANSS.MCAN.MCAN_XIDAM;
        ptr->rxFIFO0Conf = mcan->MCANSS.MCAN.MCAN_RXF0C;
        ptr->rxBuffConf = mcan->MCANSS.MCAN.MCAN_RXBC;
        ptr->rxFIFO1Conf = mcan->MCANSS.MCAN.MCAN_RXF1C;
        ptr->rxDataSize = mcan->MCANSS.MCAN.MCAN_RXESC;
        ptr->txBuffConf = mcan->MCANSS.MCAN.MCAN_TXBC;
        ptr->txDataSize = mcan->MCANSS.MCAN.MCAN_TXESC;
        ptr->txEvntFIFOConf = mcan->MCANSS.MCAN.MCAN_TXEFC;
        ptr->ssCtrlConf =
            mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_CTRL;
        ptr->preSclConf = mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS
                              .MCANSS_EXT_TS_PRESCALER;
        ptr->edcVecConf =
            mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_VECTOR;
        ptr->edcConf2 =
            mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL;
        ptr->edcConf1 =
            mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_ERR_CTRL1;
        ptr->edcConf0 =
            mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_ERR_CTRL2;
        ptr->ramWDConf = mcan->MCANSS.MCAN.MCAN_RWD;
        ptr->testConf = mcan->MCANSS.MCAN.MCAN_TEST;
        ptr->backupRdy = true;
    }
    return saveState;
}

/**
 * @brief Restore MCAN configuration snapshot.
 * @details 功能：从备份结构体恢复时钟、中断和 MCAN 配置，并还原 CCE/INIT 状态。
 * @param[in] mcan 参数：MCAN 寄存器实例。
 * @param[in,out] ptr 参数：备份配置结构体。
 * @return 返回值：true 表示本次成功恢复，false 表示不存在可用备份。
 */
bool DL_MCAN_restoreConfiguration(MCAN_Regs *mcan, DL_MCAN_backupConfig *ptr)
{
    // 仅在存在有效备份时执行恢复
    bool stateRestored = ptr->backupRdy;
    bool cceState;
    bool initState;
    if (stateRestored)
    {
        // 恢复后清除备份有效标志，防止重复恢复
        ptr->backupRdy = false;

        /**
         * Stores actual state of Configuration Change Enable. CCE will be set
         * during restore operation and will configure CCE actual state at the
         * end.
         * 记录 CCE 与 INIT 原始状态，恢复结束后还原。
         */
        cceState =
            ((ptr->ctrlConf & MCAN_CCCR_CCE_MASK) == MCAN_CCCR_CCE_MASK);
        initState =
            ((ptr->ctrlConf & MCAN_CCCR_INIT_MASK) == MCAN_CCCR_INIT_MASK);

        /**
         * MCAN configuration only overwrittring CCE configuration since INIT
         * reset value is 0x1. CCE and INIT must be set to 0x1 to allow
         * reconfiguration of MCAN.
         */
        // 中文说明：恢复期间强制打开 CCE，允许写回全部配置寄存器。
        mcan->MCANSS.MCAN.MCAN_CCCR = (ptr->ctrlConf |= MCAN_CCCR_CCE_MASK);
        mcan->MCANSS.MCAN.MCAN_NBTP = ptr->nomBitTimeConf;
        mcan->MCANSS.MCAN.MCAN_DBTP = ptr->dataBitTimeConf;
        mcan->MCANSS.MCAN.MCAN_TSCC = ptr->timeCntConf;
        mcan->MCANSS.MCAN.MCAN_TSCV = ptr->timeCntVal;
        mcan->MCANSS.MCAN.MCAN_TOCC = ptr->timeOutConf;
        mcan->MCANSS.MCAN.MCAN_TOCV = ptr->timeOutCntVal;
        mcan->MCANSS.MCAN.MCAN_TDCR = ptr->txDelCompConf;
        mcan->MCANSS.MCAN.MCAN_GFC = ptr->globFiltIDConf;
        mcan->MCANSS.MCAN.MCAN_SIDFC = ptr->stdFiltIDConf;
        mcan->MCANSS.MCAN.MCAN_XIDFC = ptr->exFiltIDConf;
        mcan->MCANSS.MCAN.MCAN_XIDAM = ptr->exFiltIDMsk;
        mcan->MCANSS.MCAN.MCAN_RXF0C = ptr->rxFIFO0Conf;
        mcan->MCANSS.MCAN.MCAN_RXBC = ptr->rxBuffConf;
        mcan->MCANSS.MCAN.MCAN_RXF1C = ptr->rxFIFO1Conf;
        mcan->MCANSS.MCAN.MCAN_RXESC = ptr->rxDataSize;
        mcan->MCANSS.MCAN.MCAN_TXBC = ptr->txBuffConf;
        mcan->MCANSS.MCAN.MCAN_TXESC = ptr->txDataSize;
        mcan->MCANSS.MCAN.MCAN_TXEFC = ptr->txEvntFIFOConf;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_CTRL =
            ptr->ssCtrlConf;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS
            .MCANSS_EXT_TS_PRESCALER = ptr->preSclConf;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_VECTOR =
            ptr->edcVecConf;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_CTRL =
            ptr->edcConf2;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_ERR_CTRL1 =
            ptr->edcConf1;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCAN_ECC_REGS.MCANERR_ERR_CTRL2 =
            ptr->edcConf0;
        mcan->MCANSS.MCAN.MCAN_RWD = ptr->ramWDConf;
        mcan->MCANSS.MCAN.MCAN_TEST = ptr->testConf;

        /* Clock configuration */
        mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKDIV = ptr->clkDivConf;
        mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKCTL = ptr->clkConf;
        mcan->MCANSS.TI_WRAPPER.MSP.MCANSS_CLKEN = ptr->clkEnConf;

        /* Interrupt Configuration */
        mcan->MCANSS.MCAN.MCAN_TXBTIE = ptr->txBuffTxIntConf;
        mcan->MCANSS.MCAN.MCAN_TXBCIE = ptr->txBuffCancIntConf;
        mcan->MCANSS.MCAN.MCAN_ILS = ptr->intLnSelConf;
        mcan->MCANSS.MCAN.MCAN_ILE = ptr->intLnEnableConf;
        mcan->MCANSS.TI_WRAPPER.PROCESSORS.MCANSS_REGS.MCANSS_IE =
            ptr->ssIntEnConf;
        mcan->MCANSS.MCAN.MCAN_IE = ptr->intEnConf;
        mcan->MCANSS.TI_WRAPPER.MSP.CPU_INT.IMASK = ptr->intEvnt0Conf;

        /* Restores MCAN configuration change state */
        if (false == cceState)
        {
            // 中文说明：还原 CCE 原始状态。
            DL_Common_updateReg(
                &mcan->MCANSS.MCAN.MCAN_CCCR, 0, MCAN_CCCR_CCE_MASK);
        }
        else
        {
            DL_Common_updateReg(&mcan->MCANSS.MCAN.MCAN_CCCR,
                                MCAN_CCCR_CCE_MASK, MCAN_CCCR_CCE_MASK);
        }

        /* Restores MCAN intialization status */
        if (false == initState)
        {
            // 中文说明：还原 INIT 原始状态，恢复到备份前运行模式。
            DL_Common_updateReg(
                &mcan->MCANSS.MCAN.MCAN_CCCR, 0, MCAN_CCCR_INIT_MASK);
        }
        else
        {
            DL_Common_updateReg(&mcan->MCANSS.MCAN.MCAN_CCCR,
                                MCAN_CCCR_INIT_MASK, MCAN_CCCR_INIT_MASK);
        }
    }

    return stateRestored;
}

#endif /* __MSPM0_HAS_MCAN__ */
