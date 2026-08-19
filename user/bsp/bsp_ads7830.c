#include "bsp_ads7830.h"

#define ADS7830_CONTROL_BYTE(channel_command, power_mode) \
    ((uint8)((((uint8)(channel_command)) << 4U) | (((uint8)(power_mode)) << 2U)))

static uint8 ads7830_validate_channel(uint8 channel)
{
    return (channel < ADS7830_CHANNEL_COUNT) ? ADS7830_STATUS_OK : ADS7830_STATUS_ERROR;
}

static ads7830_channel_command_enum ads7830_single_command(uint8 channel)
{
    /* ADS7830 单端通道编码为 0x08~0x0F 顺序排列（bit7=SD=1，bit6:4=通道号），
     * 因此直接由通道号偏移得到，无需做奇偶重排。 */
    return (ads7830_channel_command_enum)(ADS7830_SINGLE_CH0 + channel);
}

static ads7830_channel_command_enum ads7830_differential_command(uint8 positive_channel)
{
    if ((positive_channel & 0x01U) == 0U)
    {
        return (ads7830_channel_command_enum)(ADS7830_DIFF_CH0_CH1 + (positive_channel >> 1U));
    }

    return (ads7830_channel_command_enum)(ADS7830_DIFF_CH1_CH0 + ((positive_channel - 1U) >> 1U));
}

void ads7830_init(soft_iic_info_struct *bus,
                  uint8 address,
                  uint32 delay,
                  gpio_pin_enum scl_pin,
                  gpio_pin_enum sda_pin)
{
    if (bus == NULL)
    {
        return;
    }

    soft_iic_init(bus, address, delay, scl_pin, sda_pin);
}

uint8 ads7830_is_ready(soft_iic_info_struct *bus)
{
    uint8 ack;

    if (bus == NULL)
    {
        return ADS7830_STATUS_ERROR;
    }

    soft_iic_start(bus);
    ack = soft_iic_send_data(bus, (uint8)(bus->addr << 1U));
    soft_iic_stop(bus);

    return (ack != 0U) ? ADS7830_STATUS_OK : ADS7830_STATUS_ERROR;
}

uint8 ads7830_read_command(soft_iic_info_struct *bus,
                           ads7830_channel_command_enum channel_command,
                           ads7830_power_mode_enum power_mode,
                           uint8 *value)
{
    uint8 command;

    if ((bus == NULL) || (value == NULL))
    {
        return ADS7830_STATUS_ERROR;
    }

    command = ADS7830_CONTROL_BYTE(channel_command, power_mode);

    soft_iic_start(bus);
    if (soft_iic_send_data(bus, (uint8)(bus->addr << 1U)) == 0U)
    {
        soft_iic_stop(bus);
        return ADS7830_STATUS_ERROR;
    }

    if (soft_iic_send_data(bus, command) == 0U)
    {
        soft_iic_stop(bus);
        return ADS7830_STATUS_ERROR;
    }

    /* ADS7830 的转换结果通过同一地址读回，控制字后使用重复 START 保持事务连续。 */
    soft_iic_start(bus);
    if (soft_iic_send_data(bus, (uint8)((bus->addr << 1U) | 0x01U)) == 0U)
    {
        soft_iic_stop(bus);
        return ADS7830_STATUS_ERROR;
    }

    *value = soft_iic_read_data(bus, 1U);
    soft_iic_stop(bus);

    return ADS7830_STATUS_OK;
}

uint8 ads7830_read_single(soft_iic_info_struct *bus,
                          uint8 channel,
                          ads7830_power_mode_enum power_mode,
                          uint8 *value)
{
    if (ads7830_validate_channel(channel) != ADS7830_STATUS_OK)
    {
        return ADS7830_STATUS_ERROR;
    }

    return ads7830_read_command(bus,
                                ads7830_single_command(channel),
                                power_mode,
                                value);
}

uint8 ads7830_read_single_default(soft_iic_info_struct *bus,
                                  uint8 channel,
                                  uint8 *value)
{
    return ads7830_read_single(bus,
                               channel,
                               ADS7830_INTERNAL_REF_ON_ADC_ON,
                               value);
}

uint8 ads7830_read_differential(soft_iic_info_struct *bus,
                                uint8 positive_channel,
                                ads7830_power_mode_enum power_mode,
                                uint8 *value)
{
    if (ads7830_validate_channel(positive_channel) != ADS7830_STATUS_OK)
    {
        return ADS7830_STATUS_ERROR;
    }

    return ads7830_read_command(bus,
                                ads7830_differential_command(positive_channel),
                                power_mode,
                                value);
}
