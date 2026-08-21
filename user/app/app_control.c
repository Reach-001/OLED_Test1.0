/**
 * @file    app_control.c
 * @brief   控制算法模块实现
 */

#include "app_control.h"
#include "zf_driver_flash.h"

#define CONTROL_FLASH_SECTION        (FLASH_MAX_SECTION_INDEX - 1)
#define CONTROL_FLASH_PAGE           (FLASH_MAX_PAGE_INDEX - 1)
#define CONTROL_FLASH_MAGIC          0x4354524CU  /* "CTRL" */
#define CONTROL_FLASH_VERSION        1U
#define CONTROL_FLASH_WORD_COUNT     8U
#define CONTROL_FLASH_CHECK_XOR      0xA5A55A5AU

/* 控制参数实例 */
static control_param_t s_control = {
    /* 转向 PD */
    .steer_pid = {
        .kp = PID_STEER_KP,
        .ki = PID_STEER_KI,
        .kd = PID_STEER_KD,
        .integral = 0,
        .last_error = 0,
        .integral_max = PID_STEER_INTEGRAL_MAX,
        .output_max = PID_STEER_OUTPUT_MAX,
    },
    /* 速度 PI */
    .speed_pid = {
        .kp = PID_SPEED_KP,
        .ki = PID_SPEED_KI,
        .kd = PID_SPEED_KD,
        .integral = 0,
        .last_error = 0,
        .integral_max = PID_SPEED_INTEGRAL_MAX,
        .output_max = PID_SPEED_OUTPUT_MAX,
    },
    .target_speed = TARGET_SPEED_DEFAULT,
    .enable = 0,
};

/* 限幅函数 */
static float limit_f(float value, float max)
{
    if (value > max)  return max;
    if (value < -max) return -max;
    return value;
}

static int16 limit_i(int16 value, int16 max)
{
    if (value > max)  return max;
    if (value < -max) return -max;
    return value;
}

static uint32 control_flash_checksum(const uint32 *data, uint16 len)
{
    uint32 checksum = CONTROL_FLASH_CHECK_XOR;

    for (uint16 i = 0; i < len; i++)
    {
        checksum ^= data[i] + 0x9E3779B9U + (checksum << 6) + (checksum >> 2);
    }

    return checksum;
}

static uint8 control_param_in_range(uint32 target_speed,
                                    uint32 steer_kp,
                                    uint32 steer_kd,
                                    uint32 speed_kp,
                                    uint32 speed_ki)
{
    return (target_speed <= TARGET_SPEED_MAX
            && steer_kp <= 120U
            && steer_kd <= 80U
            && speed_kp <= 100U
            && speed_ki <= 60U);
}

static uint8 control_load_tune_params(void)
{
    uint32 data[CONTROL_FLASH_WORD_COUNT] = {};

    if (!flash_check(CONTROL_FLASH_SECTION, CONTROL_FLASH_PAGE))
        return 1;

    flash_read_page(CONTROL_FLASH_SECTION, CONTROL_FLASH_PAGE,
                    data, CONTROL_FLASH_WORD_COUNT);

    if (data[0] != CONTROL_FLASH_MAGIC || data[1] != CONTROL_FLASH_VERSION)
        return 1;

    if (data[7] != control_flash_checksum(data, CONTROL_FLASH_WORD_COUNT - 1))
        return 1;

    if (!control_param_in_range(data[2], data[3], data[4], data[5], data[6]))
        return 1;

    s_control.target_speed = (int16)data[2];
    control_set_steer_pid((float)data[3], PID_STEER_KI, (float)data[4]);
    control_set_speed_pid((float)data[5], (float)data[6], PID_SPEED_KD);
    return 0;
}

/*-----------------------------------------------------------
 * 控制模块初始化
 *-----------------------------------------------------------*/
void control_init(void)
{
    control_load_tune_params();
    control_reset();
}

/*-----------------------------------------------------------
 * PID 计算通用函数
 *-----------------------------------------------------------*/
static float pid_calculate(pid_t *pid, float error)
{
    float output;

    /* 积分 */
    pid->integral += error;
    pid->integral = limit_f(pid->integral, pid->integral_max);

    /* PID 计算 */
    output = pid->kp * error +
             pid->ki * pid->integral +
             pid->kd * (error - pid->last_error);

    /* 保存误差 */
    pid->last_error = error;

    /* 输出限幅 */
    output = limit_f(output, pid->output_max);

    return output;
}

/*-----------------------------------------------------------
 * 转向控制计算
 *-----------------------------------------------------------*/
/* BUG FIX: track_error 由 int8 改为 int16，与 track_get_error 返回类型对齐 */
int16 control_steer(int16 track_error)
{
    float error = (float)track_error;
    float output = pid_calculate(&s_control.steer_pid, error);
    return (int16)output;
}

/*-----------------------------------------------------------
 * 速度控制计算
 *-----------------------------------------------------------*/
int16 control_speed(int16 current_speed)
{
    float error = (float)(s_control.target_speed - current_speed);
    float output = pid_calculate(&s_control.speed_pid, error);
    return (int16)output;
}

/*-----------------------------------------------------------
 * 综合控制输出
 *-----------------------------------------------------------*/
/* BUG FIX: track_error 由 int8 改为 int16，防止隐式截断 */
void control_run(int16 track_error, int16 speed_l, int16 speed_r,
                 int16 *out_l, int16 *out_r)
{
    if (!s_control.enable)
    {
        *out_l = 0;
        *out_r = 0;
        return;
    }

    /* 转向控制 */
    int16 steer = control_steer(track_error);

    /* 速度控制 (取平均速度) */
    int16 avg_speed = (speed_l + speed_r) / 2;
    int16 speed_out = control_speed(avg_speed);

    /* 差速输出 */
    *out_l = limit_i(speed_out - steer, MOTOR_PWM_MAX);
    *out_r = limit_i(speed_out + steer, MOTOR_PWM_MAX);
}

/*-----------------------------------------------------------
 * 设置目标速度
 *-----------------------------------------------------------*/
void control_set_target_speed(int16 speed)
{
    s_control.target_speed = limit_i(speed, TARGET_SPEED_MAX);
}

/*-----------------------------------------------------------
 * 获取目标速度
 *-----------------------------------------------------------*/
int16 control_get_target_speed(void)
{
    return s_control.target_speed;
}

/*-----------------------------------------------------------
 * 使能/禁用控制
 *-----------------------------------------------------------*/
void control_enable(uint8 enable)
{
    if (enable && !s_control.enable)
    {
        /* 从禁用到使能，重置状态 */
        control_reset();
    }
    s_control.enable = enable;
}

/*-----------------------------------------------------------
 * 获取控制使能状态
 *-----------------------------------------------------------*/
uint8 control_is_enabled(void)
{
    return s_control.enable;
}

/*-----------------------------------------------------------
 * 设置转向 PID 参数
 *-----------------------------------------------------------*/
void control_set_steer_pid(float kp, float ki, float kd)
{
    s_control.steer_pid.kp = kp;
    s_control.steer_pid.ki = ki;
    s_control.steer_pid.kd = kd;
}

/*-----------------------------------------------------------
 * 设置速度 PID 参数
 *-----------------------------------------------------------*/
void control_set_speed_pid(float kp, float ki, float kd)
{
    s_control.speed_pid.kp = kp;
    s_control.speed_pid.ki = ki;
    s_control.speed_pid.kd = kd;
}

/*-----------------------------------------------------------
 * 获取控制参数指针
 *-----------------------------------------------------------*/
control_param_t* control_get_param(void)
{
    return &s_control;
}

/*-----------------------------------------------------------
 * 保存滑条调参值
 *-----------------------------------------------------------*/
uint8 control_save_tune_params(void)
{
    uint32 data[CONTROL_FLASH_WORD_COUNT] = {};

    data[0] = CONTROL_FLASH_MAGIC;
    data[1] = CONTROL_FLASH_VERSION;
    data[2] = (uint32)s_control.target_speed;
    data[3] = (uint32)((int16)s_control.steer_pid.kp);
    data[4] = (uint32)((int16)s_control.steer_pid.kd);
    data[5] = (uint32)((int16)s_control.speed_pid.kp);
    data[6] = (uint32)((int16)s_control.speed_pid.ki);
    data[7] = control_flash_checksum(data, CONTROL_FLASH_WORD_COUNT - 1);

    /* Flash 擦写寿命有限，只在用户确认滑条或恢复默认值时调用。 */
    return flash_write_page(CONTROL_FLASH_SECTION, CONTROL_FLASH_PAGE,
                            data, CONTROL_FLASH_WORD_COUNT);
}

/*-----------------------------------------------------------
 * 重置 PID 积分和状态
 *-----------------------------------------------------------*/
void control_reset(void)
{
    s_control.steer_pid.integral = 0;
    s_control.steer_pid.last_error = 0;
    s_control.speed_pid.integral = 0;
    s_control.speed_pid.last_error = 0;
}
