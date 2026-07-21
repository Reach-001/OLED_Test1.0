/**
 * @file    bsp_encoder.c
 * @brief   编码器驱动模块实现
 */

#include "bsp_encoder.h"
#include <math.h>

/* 速度转换系数: (周长mm) / (编码器分辨率) / (采样周期s) */
#define SPEED_COEFFICIENT   ((3.14159f * WHEEL_DIAMETER) / ENCODER_RESOLUTION / (ENCODER_SAMPLE_MS / 1000.0f))

/*-----------------------------------------------------------
 * 编码器初始化
 *-----------------------------------------------------------*/
void encoder_init(void)
{
    /* 左轮编码器 - 正交模式 */
    encoder_quad_init(ENCODER_L_TIM, ENCODER_L_CH1, ENCODER_L_CH2);

    /* 右轮编码器 - 方向模式 (如果硬件只有一路脉冲+方向信号) */
    encoder_dir_init(ENCODER_R_TIM, ENCODER_R_CH1, ENCODER_R_DIR);
}

/*-----------------------------------------------------------
 * 获取编码器计数值
 *-----------------------------------------------------------*/
int16 encoder_get(encoder_id_enum id)
{
    if (id == ENCODER_LEFT)
    {
        return encoder_get_count(ENCODER_L_TIM);
    }
    else if (id == ENCODER_RIGHT)
    {
        /* 右轮编码器可能需要取反，根据安装方向调整 */
        return -encoder_get_count(ENCODER_R_TIM);
    }
    return 0;
}

/*-----------------------------------------------------------
 * 获取并清零编码器计数值
 * BUG FIX: 原 get 与 clear 两步之间未禁中断，期间硬件继续计数导致脉冲丢失。
 *          必须保存 interrupt_global_disable() 返回值并原样传回 enable()，
 *          否则 nest_count 永不归零 → 中断永久屏蔽 → 任务调度器冻结。
 *-----------------------------------------------------------*/
int16 encoder_get_and_clear(encoder_id_enum id)
{
    uint32 primask = interrupt_global_disable();
    int16 count = encoder_get(id);
    encoder_clear(id);
    interrupt_global_enable(primask);
    return count;
}

/*-----------------------------------------------------------
 * 清零编码器计数
 *-----------------------------------------------------------*/
void encoder_clear(encoder_id_enum id)
{
    if (id == ENCODER_LEFT)
    {
        encoder_clear_count(ENCODER_L_TIM);
    }
    else if (id == ENCODER_RIGHT)
    {
        encoder_clear_count(ENCODER_R_TIM);
    }
}

/*-----------------------------------------------------------
 * 清零所有编码器
 *-----------------------------------------------------------*/
void encoder_clear_all(void)
{
    encoder_clear(ENCODER_LEFT);
    encoder_clear(ENCODER_RIGHT);
}

/*-----------------------------------------------------------
 * 计数值转换为速度 (mm/s)
 *-----------------------------------------------------------*/
float encoder_count_to_speed(int16 count)
{
    return (float)count * SPEED_COEFFICIENT;
}
