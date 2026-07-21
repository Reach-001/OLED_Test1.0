/**
 * @file    bsp_led.c
 * @brief   LED 驱动模块实现
 */

#include "bsp_led.h"

/*-----------------------------------------------------------
 * LED 初始化
 *-----------------------------------------------------------*/
void led_init(void)
{
    gpio_init(LED_PIN, GPO, !LED_ON_LEVEL, GPO_PUSH_PULL);
}

/*-----------------------------------------------------------
 * LED 点亮
 *-----------------------------------------------------------*/
void led_on(void)
{
#if LED_ON_LEVEL
    gpio_high(LED_PIN);
#else
    gpio_low(LED_PIN);
#endif
}

/*-----------------------------------------------------------
 * LED 熄灭
 *-----------------------------------------------------------*/
void led_off(void)
{
#if LED_ON_LEVEL
    gpio_low(LED_PIN);
#else
    gpio_high(LED_PIN);
#endif
}

/*-----------------------------------------------------------
 * LED 翻转
 *-----------------------------------------------------------*/
void led_toggle(void)
{
    gpio_toggle_level(LED_PIN);
}

/*-----------------------------------------------------------
 * LED 闪烁指定次数
 *-----------------------------------------------------------*/
void led_blink(uint8 times, uint16 ms)
{
    for (uint8 i = 0; i < times; i++)
    {
        led_on();
        system_delay_ms(ms);
        led_off();
        system_delay_ms(ms);
    }
}
