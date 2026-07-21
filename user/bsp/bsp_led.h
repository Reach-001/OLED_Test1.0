/**
 * @file    bsp_led.h
 * @brief   LED 驱动模块
 */

#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "board_config.h"

/**
 * @brief   LED 初始化
 */
void led_init(void);

/**
 * @brief   LED 点亮
 */
void led_on(void);

/**
 * @brief   LED 熄灭
 */
void led_off(void);

/**
 * @brief   LED 翻转
 */
void led_toggle(void);

/**
 * @brief   LED 闪烁指定次数
 * @param   times   闪烁次数
 * @param   ms      单次亮灭时间(ms)
 */
void led_blink(uint8 times, uint16 ms);

#endif /* _BSP_LED_H_ */
