/**
 * @file    bsp_track.c
 * @brief   循迹传感器驱动模块实现
 */

#include "bsp_track.h"

#if TRACK_USE_ADS7830
#include "bsp_ads7830.h"

static soft_iic_info_struct s_ads7830_bus;
static uint8 s_ads7830_ready = 0;
#else
/* 传感器引脚数组 */
static const adc_pin_enum track_pins[TRACK_SENSOR_NUM] = {
#if TRACK_SENSOR_NUM >= 1
    TRACK_SENSOR_1,
#endif
#if TRACK_SENSOR_NUM >= 2
    TRACK_SENSOR_2,
#endif
#if TRACK_SENSOR_NUM >= 3
    TRACK_SENSOR_3,
#endif
#if TRACK_SENSOR_NUM >= 4
    TRACK_SENSOR_4,
#endif
#if TRACK_SENSOR_NUM >= 5
    TRACK_SENSOR_5,
#endif
#if TRACK_SENSOR_NUM >= 6
    TRACK_SENSOR_6,
#endif
#if TRACK_SENSOR_NUM >= 7
    TRACK_SENSOR_7,
#endif
#if TRACK_SENSOR_NUM >= 8
    TRACK_SENSOR_8,
#endif
};
#endif

/* 权重数组改为 int16，防止 TRACK_WEIGHT_LIST 超出 ±127 时静默截断
 * BUG FIX: 原为 int8，若权值超出 ±127 会在数组初始化时无声截断 */
static const int16 track_weights[TRACK_SENSOR_NUM] = TRACK_WEIGHT_LIST;

/* 当前阈值 */
#if TRACK_USE_ADS7830
static uint16 s_threshold = TRACK_ADS7830_THRESHOLD;
#else
static uint16 s_threshold = TRACK_THRESHOLD;
#endif

/*-----------------------------------------------------------
 * 循迹传感器初始化
 *-----------------------------------------------------------*/
void track_init(void)
{
#if TRACK_USE_ADS7830
    ads7830_init(&s_ads7830_bus,
                 ADS7830_I2C_ADDR,
                 ADS7830_SOFT_IIC_DELAY,
                 ADS7830_SCL_PIN,
                 ADS7830_SDA_PIN);
    s_ads7830_ready = (ads7830_is_ready(&s_ads7830_bus) == ADS7830_STATUS_OK) ? 1 : 0;
#else
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        adc_init(track_pins[i], TRACK_ADC_RESOLUTION);
    }
#endif
}

/*-----------------------------------------------------------
 * 读取所有传感器原始值
 *-----------------------------------------------------------*/
void track_read_raw(track_data_t *data)
{
#if TRACK_USE_ADS7830
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        uint32 sum = 0;
        uint8 valid_count = 0;

        for (uint8 sample = 0; sample < TRACK_ADC_FILTER_COUNT; sample++)
        {
            uint8 value = 0;

            if ((s_ads7830_ready != 0U) &&
                (ads7830_read_single_default(&s_ads7830_bus, i, &value) == ADS7830_STATUS_OK))
            {
                sum += value;
                valid_count++;
            }
        }

        /* ADS7830 只有 8 位输出。通信失败时置 0，避免沿用上一帧误导控制层。 */
        data->raw[i] = (valid_count > 0U) ? (uint16)(sum / valid_count) : 0U;
    }
#else
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        data->raw[i] = adc_mean_filter_convert(track_pins[i], TRACK_ADC_FILTER_COUNT);
    }
#endif
}

/*-----------------------------------------------------------
 * 读取并数字化传感器数据
 *-----------------------------------------------------------*/
void track_read(track_data_t *data)
{
    track_read_raw(data);

    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        /* 大于阈值为黑线 (具体逻辑根据传感器类型调整) */
        data->digital[i] = (data->raw[i] > s_threshold) ? 1 : 0;
    }
}

/*-----------------------------------------------------------
 * 计算循迹偏差 (加权算法)
 * BUG FIX: 返回类型由 int8 改为 int16，防止权值均值超出 ±127 时截断
 *-----------------------------------------------------------*/
int16 track_get_error(track_data_t *data)
{
    int16 sum_weight = 0;
    int16 sum_count = 0;

    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        if (data->digital[i])
        {
            sum_weight += track_weights[i];
            sum_count++;
        }
    }

    /* 避免除零 */
    if (sum_count == 0)
    {
        return 0;  /* 丢线时返回0，由上层处理 */
    }

    return (int16)(sum_weight / sum_count);
}

/*-----------------------------------------------------------
 * 检测是否全白 (丢线)
 *-----------------------------------------------------------*/
uint8 track_is_lost(track_data_t *data)
{
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        if (data->digital[i])
        {
            return 0;  /* 只要有一个检测到黑线就不是丢线 */
        }
    }
    return 1;
}

/*-----------------------------------------------------------
 * 检测是否全黑 (十字/起点线)
 *-----------------------------------------------------------*/
uint8 track_is_cross(track_data_t *data)
{
    uint8 count = 0;
    for (uint8 i = 0; i < TRACK_SENSOR_NUM; i++)
    {
        if (data->digital[i])
        {
            count++;
        }
    }
    /* 超过一半的传感器检测到黑线认为是十字 */
    return (count >= (TRACK_SENSOR_NUM * 2 / 3)) ? 1 : 0;
}

/*-----------------------------------------------------------
 * 设置黑白阈值
 *-----------------------------------------------------------*/
void track_set_threshold(uint16 threshold)
{
    s_threshold = threshold;
}

/*-----------------------------------------------------------
 * 获取当前阈值
 *-----------------------------------------------------------*/
uint16 track_get_threshold(void)
{
    return s_threshold;
}
