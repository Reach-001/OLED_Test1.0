/**
 * @file    bsp_encoder.h
 * @brief   编码器驱动模块 - 支持双路正交/方向编码器
 */

#ifndef _BSP_ENCODER_H_
#define _BSP_ENCODER_H_

#include "board_config.h"

/* 编码器编号枚举 */
typedef enum
{
    ENCODER_LEFT = 0,   /* 左轮编码器 */
    ENCODER_RIGHT,      /* 右轮编码器 */
    ENCODER_NUM         /* 编码器数量 */
} encoder_id_enum;

/**
 * @brief   编码器初始化
 */
void encoder_init(void);

/**
 * @brief   获取编码器计数值（带方向）
 * @param   id      编码器编号
 * @return  int16   计数值，正负表示方向
 */
int16 encoder_get(encoder_id_enum id);

/**
 * @brief   获取并清零编码器计数值
 * @param   id      编码器编号
 * @return  int16   计数值
 */
int16 encoder_get_and_clear(encoder_id_enum id);

/**
 * @brief   清零编码器计数
 * @param   id      编码器编号
 */
void encoder_clear(encoder_id_enum id);

/**
 * @brief   清零所有编码器
 */
void encoder_clear_all(void);

/**
 * @brief   计数值转换为速度 (脉冲/采样周期 → mm/s)
 * @param   count   计数值
 * @return  float   速度 (mm/s)
 */
float encoder_count_to_speed(int16 count);

#endif /* _BSP_ENCODER_H_ */
