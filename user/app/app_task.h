/**
 * @file    app_task.h
 * @brief   任务调度模块 - 基于定时器的简易任务管理
 *
 * @details 采用时间片轮询方式，无需 RTOS
 *          各任务在定时中断中设置标志，主循环中执行
 */

#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#include "board_config.h"
#include "bsp_track.h"

/* 任务标志位。
 * 新增任务时，在结构体里加一个 uint8 标志，例如 uint8 uart;
 * 标志只表示“该任务该执行了”，不要在中断里直接执行任务逻辑。
 */
typedef struct
{
    uint8 track;        /* 循迹采样任务 */
    uint8 control;      /* 控制计算任务 */
    uint8 display;      /* 显示刷新任务 */
    uint8 ui;           /* UI 动画刷新任务 */
    uint8 key;          /* 按键扫描任务 */
    uint8 uart;         /* UART 轮询任务 */
} task_flag_t;

/* 全局任务标志 */
extern volatile task_flag_t g_task_flag;

/* 系统状态枚举 */
typedef enum
{
    SYS_STATE_IDLE = 0,     /* 待机状态 */
    SYS_STATE_READY,        /* 就绪状态 (等待启动) */
    SYS_STATE_RUNNING,      /* 运行状态 */
    SYS_STATE_STOP,         /* 停止状态 */
    SYS_STATE_ERROR,        /* 错误状态 */
} sys_state_enum;

/**
 * @brief   任务调度初始化
 */
void task_init(void);

/**
 * @brief   定时器回调 (在中断中调用)
 *
 * 这里只设置任务标志，不能执行耗时逻辑、printf 或长延时。
 */
void task_timer_callback(void);

/**
 * @brief   循迹采样任务
 */
void task_track(void);

/**
 * @brief   控制计算任务
 */
void task_control(void);

/**
 * @brief   显示刷新任务
 */
void task_display(void);

/**
 * @brief   UI 动画刷新任务
 */
void task_ui(void);

/**
 * @brief   按键处理任务
 */
void task_key(void);

/**
 * @brief   UART 轮询任务
 */
void task_uart(void);

/**
 * @brief   获取循迹传感器数据指针（供 UI 等模块读取）
 * @return  指向最新 track_data 的只读指针
 */
const track_data_t* task_get_track_data(void);

/**
 * @brief   获取系统毫秒计时
 */
uint32 task_get_ms(void);

/**
 * @brief   设置系统状态
 * @param   state   状态
 */
void task_set_state(sys_state_enum state);

/**
 * @brief   获取系统状态
 * @return  sys_state_enum
 */
sys_state_enum task_get_state(void);

/**
 * @brief   系统启动
 */
void task_start(void);

/**
 * @brief   系统停止
 */
void task_stop(void);

#endif /* _APP_TASK_H_ */
