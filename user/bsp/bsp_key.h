/**
 * @file    bsp_key.h
 * @brief   按键驱动模块 - 支持短按、长按检测
 */

#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

#include "board_config.h"

/* 按键编号枚举 */
typedef enum
{
    BSP_KEY_1 = 0,
    BSP_KEY_2,
    BSP_KEY_3,
    BSP_KEY_NUM
} bsp_key_id_enum;

/* 按键事件枚举 */
typedef enum
{
    KEY_EVENT_NONE = 0,     /* 无事件 */
    KEY_EVENT_PRESS,        /* 短按 */
    KEY_EVENT_LONG_PRESS,   /* 长按 */
    KEY_EVENT_RELEASE,      /* 释放 */
} bsp_key_event_enum;

/**
 * @brief   按键初始化
 */
void bsp_key_init(void);

/**
 * @brief   按键扫描，由 TASK_KEY_PERIOD 决定调用周期
 */
void bsp_key_scan(void);

/**
 * @brief   获取按键事件
 * @param   id      按键编号
 * @return  按键事件
 */
bsp_key_event_enum bsp_key_get_event(bsp_key_id_enum id);

/**
 * @brief   获取按键当前状态
 * @param   id      按键编号
 * @return  1=按下, 0=松开
 */
uint8 bsp_key_get_state(bsp_key_id_enum id);

/**
 * @brief   清除按键事件
 * @param   id      按键编号
 */
void bsp_key_clear_event(bsp_key_id_enum id);

#endif /* _BSP_KEY_H_ */
