#ifndef _BSP_ADS7830_H_
#define _BSP_ADS7830_H_

#include "zf_driver_soft_iic.h"

/* ADS7830 8 位 I2C ADC 驱动。
 * 当前实现使用逐飞软件 IIC，适合在 MSPM0 工程中直接复用普通 GPIO。
 * 读数流程固定为：写控制字 -> 重复 START -> 读 1 字节转换结果。
 */

#define ADS7830_I2C_ADDR                 (0x4BU)
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
 * 单端通道编码不是 0~7 顺序排列，调用侧应优先使用 ads7830_read_single()。
 */
typedef enum
{
    ADS7830_DIFF_CH0_CH1 = 0x00U,
    ADS7830_DIFF_CH2_CH3 = 0x01U,
    ADS7830_DIFF_CH4_CH5 = 0x02U,
    ADS7830_DIFF_CH6_CH7 = 0x03U,
    ADS7830_DIFF_CH1_CH0 = 0x04U,
    ADS7830_DIFF_CH3_CH2 = 0x05U,
    ADS7830_DIFF_CH5_CH4 = 0x06U,
    ADS7830_DIFF_CH7_CH6 = 0x07U,
    ADS7830_SINGLE_CH0   = 0x08U,
    ADS7830_SINGLE_CH2   = 0x09U,
    ADS7830_SINGLE_CH4   = 0x0AU,
    ADS7830_SINGLE_CH6   = 0x0BU,
    ADS7830_SINGLE_CH1   = 0x0CU,
    ADS7830_SINGLE_CH3   = 0x0DU,
    ADS7830_SINGLE_CH5   = 0x0EU,
    ADS7830_SINGLE_CH7   = 0x0FU,
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
