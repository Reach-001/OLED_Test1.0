/**
 * @file    bsp_motor.h
 * @brief   电机驱动模块 - 支持双路直流电机
 *
 * @details 采用方向+PWM控制方式，兼容 TB6612/DRV8833 等驱动芯片
 *          正数前进，负数后退，0 停止
 */

#ifndef _BSP_MOTOR_H_
#define _BSP_MOTOR_H_

#include "board_config.h"

/* 电机编号枚举 */
typedef enum
{
    MOTOR_LEFT = 0,     /* 左电机 */
    MOTOR_RIGHT,        /* 右电机 */
    MOTOR_NUM           /* 电机数量 */
} motor_id_enum;

/**
 * @brief   电机初始化
 */
void motor_init(void);

/**
 * @brief   设置单个电机速度
 * @param   id      电机编号
 * @param   speed   速度值 [-MOTOR_PWM_MAX, +MOTOR_PWM_MAX]
 *                  正数前进，负数后退
 */
void motor_set_speed(motor_id_enum id, int16 speed);

/**
 * @brief   同时设置双电机速度
 * @param   left    左电机速度
 * @param   right   右电机速度
 */
void motor_set_speed_both(int16 left, int16 right);

/**
 * @brief   电机停止
 * @param   id      电机编号
 */
void motor_stop(motor_id_enum id);

/**
 * @brief   所有电机停止
 */
void motor_stop_all(void);

/**
 * @brief   电机使能/失能 (可选，需硬件支持)
 * @param   enable  1=使能, 0=失能
 */
void motor_enable(uint8 enable);

#endif /* _BSP_MOTOR_H_ */
