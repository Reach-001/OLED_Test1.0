/**
 * @file    bsp_track.h
 * @brief   循迹传感器驱动模块 - 支持多路红外/灰度传感器
 *
 * @details 采用加权算法计算偏差，支持 1-8 路传感器扩展
 *          偏差范围: [-100, +100]，负数偏左，正数偏右，0 居中
 */

#ifndef _BSP_TRACK_H_
#define _BSP_TRACK_H_

#include "board_config.h"

/* 传感器原始数据结构 */
typedef struct
{
    uint16 raw[TRACK_SENSOR_NUM];   /* ADC 原始值 */
    uint8  digital[TRACK_SENSOR_NUM]; /* 数字化结果 (0=白, 1=黑) */
} track_data_t;

/**
 * @brief   循迹传感器初始化
 */
void track_init(void);

/**
 * @brief   获取 ADS7830 通信状态
 * @return  uint8   1=初始化时 IIC 应答正常, 0=未应答
 */
uint8 track_is_ready(void);

/**
 * @brief   读取所有传感器原始值
 * @param   data    数据结构指针
 */
void track_read_raw(track_data_t *data);

/**
 * @brief   读取并数字化传感器数据
 * @param   data    数据结构指针
 */
void track_read(track_data_t *data);

/**
 * @brief   分时采样循迹传感器
 * @param   data    数据结构指针
 *
 * 每次调用只访问一个 ADS7830 通道一次，避免一次性读完 8 路导致主循环长时间阻塞。
 */
void track_update_step(track_data_t *data);

/**
 * @brief   计算循迹偏差 (加权算法)
 * @param   data    传感器数据
 * @return  int16   偏差值 [-100, +100]
 *                  负数=偏左, 正数=偏右, 0=居中
 * BUG FIX: 返回类型由 int8 改为 int16，防止权值均值超出 ±127 时截断
 */
int16 track_get_error(track_data_t *data);

/**
 * @brief   检测是否全白 (丢线)
 * @param   data    传感器数据
 * @return  uint8   1=全白(丢线), 0=正常
 */
uint8 track_is_lost(track_data_t *data);

/**
 * @brief   检测是否全黑 (十字/起点线)
 * @param   data    传感器数据
 * @return  uint8   1=全黑, 0=正常
 */
uint8 track_is_cross(track_data_t *data);

/**
 * @brief   设置黑白阈值 (用于标定)
 * @param   threshold   阈值
 */
void track_set_threshold(uint16 threshold);

/**
 * @brief   获取当前阈值
 * @return  uint16  阈值
 */
uint16 track_get_threshold(void);

#endif /* _BSP_TRACK_H_ */
