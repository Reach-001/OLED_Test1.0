/**
 * @file    bsp_key.c
 * @brief   按键驱动模块实现
 */

#include "bsp_key.h"

/* 按键引脚数组 */
static const gpio_pin_enum key_pins[BSP_KEY_NUM] = {
    KEY1_PIN,
    KEY2_PIN,
};

/* 按键状态结构 */
typedef struct
{
    uint8 state;            /* 当前状态 */
    uint8 last_state;       /* 上次状态 */
    uint8 long_reported;    /* 长按事件已上报，松开后清零 */
    uint16 press_count;     /* 按下计数 */
    bsp_key_event_enum event;   /* 事件 */
} key_state_t;

static key_state_t s_key_state[BSP_KEY_NUM] = {0};

/*-----------------------------------------------------------
 * 按键初始化
 *-----------------------------------------------------------*/
void bsp_key_init(void)
{
    for (uint8 i = 0; i < BSP_KEY_NUM; i++)
    {
#if KEY_ACTIVE_LEVEL == 0
        gpio_init(key_pins[i], GPI, GPIO_HIGH, GPI_PULL_UP);
#else
        gpio_init(key_pins[i], GPI, GPIO_LOW, GPI_PULL_DOWN);
#endif
        s_key_state[i].state = 0;
        s_key_state[i].last_state = 0;
        s_key_state[i].long_reported = 0;
        s_key_state[i].press_count = 0;
        s_key_state[i].event = KEY_EVENT_NONE;
    }
}

/*-----------------------------------------------------------
 * 按键扫描
 *-----------------------------------------------------------*/
void bsp_key_scan(void)
{
    for (uint8 i = 0; i < BSP_KEY_NUM; i++)
    {
        /* 读取按键电平 */
        uint8 level = gpio_get_level(key_pins[i]);
        uint8 pressed = (level == KEY_ACTIVE_LEVEL) ? 1 : 0;

        if (pressed)
        {
            if (s_key_state[i].press_count < 0xffff)
            {
                s_key_state[i].press_count++;
            }

            if (s_key_state[i].press_count == KEY_DEBOUNCE_COUNT)
            {
                /* 消抖后确认按下 */
                s_key_state[i].state = 1;
            }
            else if (s_key_state[i].press_count >= KEY_LONG_PRESS_COUNT)
            {
                if (!s_key_state[i].long_reported)
                {
                    s_key_state[i].event = KEY_EVENT_LONG_PRESS;
                    s_key_state[i].long_reported = 1;
                }
            }
        }
        else
        {
            /* 按键松开 */
            if (s_key_state[i].state)
            {
                /* 之前是按下状态 */
                if (s_key_state[i].press_count >= KEY_DEBOUNCE_COUNT &&
                    s_key_state[i].press_count < KEY_LONG_PRESS_COUNT)
                {
                    s_key_state[i].event = KEY_EVENT_PRESS;
                }
                else if (s_key_state[i].long_reported)
                {
                    s_key_state[i].event = KEY_EVENT_RELEASE;
                }
                s_key_state[i].state = 0;
            }
            s_key_state[i].press_count = 0;
            s_key_state[i].long_reported = 0;
        }

        s_key_state[i].last_state = s_key_state[i].state;
    }
}

/*-----------------------------------------------------------
 * 获取按键事件
 *-----------------------------------------------------------*/
bsp_key_event_enum bsp_key_get_event(bsp_key_id_enum id)
{
    if (id >= BSP_KEY_NUM) return KEY_EVENT_NONE;
    return s_key_state[id].event;
}

/*-----------------------------------------------------------
 * 获取按键当前状态
 *-----------------------------------------------------------*/
uint8 bsp_key_get_state(bsp_key_id_enum id)
{
    if (id >= BSP_KEY_NUM) return 0;
    return s_key_state[id].state;
}

/*-----------------------------------------------------------
 * 清除按键事件
 *-----------------------------------------------------------*/
void bsp_key_clear_event(bsp_key_id_enum id)
{
    if (id >= BSP_KEY_NUM) return;
    s_key_state[id].event = KEY_EVENT_NONE;
}
