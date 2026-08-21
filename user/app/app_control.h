/**
 * @file    app_control.h
 * @brief   控制算法模块 - PID 控制器实现
 *
 * @details 包含转向PD控制和速度PI控制
 *          支持参数在线调整
 */

#ifndef _APP_CONTROL_H_
#define _APP_CONTROL_H_

#include "board_config.h"

/* PID 参数结构体 */
typedef struct
{
    float kp;           /* 比例系数 */
    float ki;           /* 积分系数 */
    float kd;           /* 微分系数 */
    float integral;     /* 积分累计 */
    float last_error;   /* 上次误差 */
    float integral_max; /* 积分限幅 */
    float output_max;   /* 输出限幅 */
} pid_t;

/* 控制参数结构体 (便于保存/加载) */
typedef struct
{
    pid_t steer_pid;    /* 转向 PID */
    pid_t speed_pid;    /* 速度 PID */
    int16 target_speed; /* 目标速度 */
    uint8 enable;       /* 控制使能 */
} control_param_t;

/**
 * @brief   控制模块初始化
 */
void control_init(void);

/**
 * @brief   转向控制计算
 * @param   track_error     循迹偏差 [-100, +100]
 * @return  int16           转向量 (用于差速)
 * BUG FIX: 参数由 int8 改为 int16，与 track_get_error 返回类型对齐
 */
int16 control_steer(int16 track_error);

/**
 * @brief   速度控制计算
 * @param   current_speed   当前速度
 * @return  int16           速度输出
 */
int16 control_speed(int16 current_speed);

/**
 * @brief   综合控制输出 (电机差速)
 * @param   track_error     循迹偏差
 * @param   speed_l         左轮速度反馈
 * @param   speed_r         右轮速度反馈
 * @param   out_l           [out] 左电机输出
 * @param   out_r           [out] 右电机输出
 */
/* BUG FIX: track_error 由 int8 改为 int16，与 track_get_error 返回类型对齐 */
void control_run(int16 track_error, int16 speed_l, int16 speed_r,
                 int16 *out_l, int16 *out_r);

/**
 * @brief   设置目标速度
 * @param   speed   目标速度
 */
void control_set_target_speed(int16 speed);

/**
 * @brief   获取目标速度
 * @return  int16   目标速度
 */
int16 control_get_target_speed(void);

/**
 * @brief   使能/禁用控制
 * @param   enable  1=使能, 0=禁用
 */
void control_enable(uint8 enable);

/**
 * @brief   获取控制使能状态
 * @return  uint8   1=使能, 0=禁用
 */
uint8 control_is_enabled(void);

/**
 * @brief   设置转向 PID 参数
 */
void control_set_steer_pid(float kp, float ki, float kd);

/**
 * @brief   设置速度 PID 参数
 */
void control_set_speed_pid(float kp, float ki, float kd);

/**
 * @brief   获取控制参数指针 (用于调参/保存)
 * @return  control_param_t*
 */
control_param_t* control_get_param(void);

/**
 * @brief   保存滑条调参值到 Flash
 * @return  uint8   0=成功, 1=失败
 */
uint8 control_save_tune_params(void);

/**
 * @brief   重置 PID 积分和状态
 */
void control_reset(void);

#endif /* _APP_CONTROL_H_ */
