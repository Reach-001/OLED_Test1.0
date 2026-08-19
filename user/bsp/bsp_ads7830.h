#ifndef _BSP_ADS7830_H_
#define _BSP_ADS7830_H_

#include "zf_driver_soft_iic.h"

/* ADS7830 8 位 I2C ADC 驱动。
 * 当前实现使用逐飞软件 IIC，适合在 MSPM0 工程中直接复用普通 GPIO。
 * 读数流程固定为：写控制字 -> 重复 START -> 读 1 字节转换结果。
 */

/* TI ADS7830 固定 7 位 I2C 地址为 1001000 (0x48)。
 * 注意：0x4B 是 ADS7828 通过 A0/A1 引脚可选地址之一，ADS7830 无地址引脚，不能使用。 */
#define ADS7830_I2C_ADDR                 (0x48U)
#define ADS7830_CHANNEL_COUNT            (8U)

#define ADS7830_STATUS_OK                (0U)
#define ADS7830_STATUS_ERROR             (1U)

typedef enum
{
    ADS7830_POWER_DOWN_BETWEEN_CONVERSIONS = 0x00U,
    ADS7830_INTERNAL_REF_OFF_ADC_ON        = 0x01U,
    ADS7830_INTERNAL_REF_ON_ADC_OFF        = 0x02U,
    ADS7830_INTERNAL_REF_ON_ADC_ON         = 0x03U,
} ads7830_power_mode_enum;

/* ADS7830 的输入选择码会放入控制字 bit7:4。
 * 控制字 bit7(SD)=1 表示单端输入，bit6:4 为通道号，因此单端 CHn 编码 = 0x08 + n，
 * 即 0x08~0x0F 顺序排列；bit7(SD)=0 为差分输入，编码 0x00~0x07。
 * 调用侧应优先使用 ads7830_read_single() 传入 0~7 的通道号。
 */
typedef enum
{
    ADS7830_DIFF_CH0_CH1 = 0x00U,   /* 差分: CH0(+), CH1(-) */
    ADS7830_DIFF_CH2_CH3 = 0x01U,   /* 差分: CH2(+), CH3(-) */
    ADS7830_DIFF_CH4_CH5 = 0x02U,   /* 差分: CH4(+), CH5(-) */
    ADS7830_DIFF_CH6_CH7 = 0x03U,   /* 差分: CH6(+), CH7(-) */
    ADS7830_DIFF_CH1_CH0 = 0x04U,   /* 差分: CH1(+), CH0(-) */
    ADS7830_DIFF_CH3_CH2 = 0x05U,   /* 差分: CH3(+), CH2(-) */
    ADS7830_DIFF_CH5_CH4 = 0x06U,   /* 差分: CH5(+), CH4(-) */
    ADS7830_DIFF_CH7_CH6 = 0x07U,   /* 差分: CH7(+), CH6(-) */
    ADS7830_SINGLE_CH0   = 0x08U,   /* 单端: CH0 */
    ADS7830_SINGLE_CH1   = 0x09U,   /* 单端: CH1 */
    ADS7830_SINGLE_CH2   = 0x0AU,   /* 单端: CH2 */
    ADS7830_SINGLE_CH3   = 0x0BU,   /* 单端: CH3 */
    ADS7830_SINGLE_CH4   = 0x0CU,   /* 单端: CH4 */
    ADS7830_SINGLE_CH5   = 0x0DU,   /* 单端: CH5 */
    ADS7830_SINGLE_CH6   = 0x0EU,   /* 单端: CH6 */
    ADS7830_SINGLE_CH7   = 0x0FU,   /* 单端: CH7 */
} ads7830_channel_command_enum;

void  ads7830_init(soft_iic_info_struct *bus,
                   uint8 address,
                   uint32 delay,
                   gpio_pin_enum scl_pin,
                   gpio_pin_enum sda_pin);
uint8 ads7830_is_ready(soft_iic_info_struct *bus);
uint8 ads7830_read_command(soft_iic_info_struct *bus,
                           ads7830_channel_command_enum channel_command,
                           ads7830_power_mode_enum power_mode,
                           uint8 *value);
uint8 ads7830_read_single(soft_iic_info_struct *bus,
                          uint8 channel,
                          ads7830_power_mode_enum power_mode,
                          uint8 *value);
uint8 ads7830_read_single_default(soft_iic_info_struct *bus,
                                  uint8 channel,
                                  uint8 *value);
uint8 ads7830_read_differential(soft_iic_info_struct *bus,
                                uint8 positive_channel,
                                ads7830_power_mode_enum power_mode,
                                uint8 *value);

#endif
