/**
 * @file    bsp_motor.c
 * @brief   电机驱动模块实现
 */

#include "bsp_motor.h"

#if MOTOR_ENABLE
/* Private helper: clamp motor command. */
static int16 motor_limit(int16 value, int16 max)
{
    if (value > max)  return max;
    if (value < -max) return -max;
    return value;
}
#endif

/*-----------------------------------------------------------
 * 电机初始化
 *-----------------------------------------------------------*/
void motor_init(void)
{
#if MOTOR_ENABLE
    /* 左电机 PWM 和方向 */
    pwm_init(MOTOR_L_PWM, MOTOR_PWM_FREQ, 0);
    gpio_init(MOTOR_L_DIR1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_L_DIR2, GPO, GPIO_LOW, GPO_PUSH_PULL);

    /* 右电机 PWM 和方向 */
    pwm_init(MOTOR_R_PWM, MOTOR_PWM_FREQ, 0);
    gpio_init(MOTOR_R_DIR1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_R_DIR2, GPO, GPIO_LOW, GPO_PUSH_PULL);
#endif
}

/*-----------------------------------------------------------
 * 设置单个电机速度
 *-----------------------------------------------------------*/
void motor_set_speed(motor_id_enum id, int16 speed)
{
#if !MOTOR_ENABLE
    (void)id;
    (void)speed;
    return;
#else
    /* 限幅 */
    speed = motor_limit(speed, MOTOR_PWM_MAX);

    /* 死区处理
     * BUG FIX: 原用 uint16 直接加 DEADZONE，若 MOTOR_PWM_MAX 接近 65535 时溢出截断后
     *          钳位判断失效。改用 uint32 中间变量计算后再钳位，防止溢出。 */
    uint16 pwm_duty;
    if (speed > 0)
    {
        uint32 temp = (uint32)speed + MOTOR_DEADZONE;
        pwm_duty = (uint16)(temp > MOTOR_PWM_MAX ? MOTOR_PWM_MAX : temp);
    }
    else if (speed < 0)
    {
        uint32 temp = (uint32)(-speed) + MOTOR_DEADZONE;
        pwm_duty = (uint16)(temp > MOTOR_PWM_MAX ? MOTOR_PWM_MAX : temp);
    }
    else
    {
        pwm_duty = 0;
    }

    /* 根据电机编号设置 */
    if (id == MOTOR_LEFT)
    {
        if (speed > 0)
        {
            /* 正转 */
            gpio_high(MOTOR_L_DIR1);
            gpio_low(MOTOR_L_DIR2);
        }
        else if (speed < 0)
        {
            /* 反转 */
            gpio_low(MOTOR_L_DIR1);
            gpio_high(MOTOR_L_DIR2);
        }
        else
        {
            /* 停止（刹车） */
            gpio_low(MOTOR_L_DIR1);
            gpio_low(MOTOR_L_DIR2);
        }
        pwm_set_duty(MOTOR_L_PWM, pwm_duty);
    }
    else if (id == MOTOR_RIGHT)
    {
        if (speed > 0)
        {
            /* 正转 */
            gpio_high(MOTOR_R_DIR1);
            gpio_low(MOTOR_R_DIR2);
        }
        else if (speed < 0)
        {
            /* 反转 */
            gpio_low(MOTOR_R_DIR1);
            gpio_high(MOTOR_R_DIR2);
        }
        else
        {
            /* 停止（刹车） */
            gpio_low(MOTOR_R_DIR1);
            gpio_low(MOTOR_R_DIR2);
        }
        pwm_set_duty(MOTOR_R_PWM, pwm_duty);
    }
#endif
}

/*-----------------------------------------------------------
 * 同时设置双电机速度
 *-----------------------------------------------------------*/
void motor_set_speed_both(int16 left, int16 right)
{
    motor_set_speed(MOTOR_LEFT, left);
    motor_set_speed(MOTOR_RIGHT, right);
}

/*-----------------------------------------------------------
 * 电机停止
 *-----------------------------------------------------------*/
void motor_stop(motor_id_enum id)
{
    motor_set_speed(id, 0);
}

/*-----------------------------------------------------------
 * 所有电机停止
 *-----------------------------------------------------------*/
void motor_stop_all(void)
{
    motor_set_speed(MOTOR_LEFT, 0);
    motor_set_speed(MOTOR_RIGHT, 0);
}

/*-----------------------------------------------------------
 * 电机使能/失能
 *-----------------------------------------------------------*/
void motor_enable(uint8 enable)
{
    /* 如果驱动芯片有使能引脚，在此控制 */
    /* 例如: gpio_set_level(MOTOR_EN_PIN, enable); */
    if (!enable)
    {
        motor_stop_all();
    }
}
