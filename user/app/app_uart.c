/**
 * @file    app_uart.c
 * @brief   UART 应用层控制实现
 */

#include "app_uart.h"

#ifndef APP_UART_BAUD
#define APP_UART_BAUD WIRELESS_UART_BAUD
#endif

#ifndef APP_UART_RX_LIMIT_PER_TASK
#define APP_UART_RX_LIMIT_PER_TASK 16
#endif

static app_uart_state_t s_uart[APP_UART_CHANNEL_NUM] = {
    { true,  true,  UART_0, UART0_TX_A10,       UART0_RX_A11,       APP_UART_BAUD, 0, 0 },  /* UART0 系统已启 (debug) */
    { false, false, UART_1, WIRELESS_UART_TX,   WIRELESS_UART_RX,   APP_UART_BAUD, 0, 0 },
    { false, false, UART_2, UART2_TX_A21,       UART2_RX_A22,       APP_UART_BAUD, 0, 0 },
    { false, false, UART_3, UART3_TX_B12,       UART3_RX_B13,       APP_UART_BAUD, 0, 0 },
};

static bool app_uart_valid_ch(uint8 ch)
{
    return ch < APP_UART_CHANNEL_NUM;
}

static void app_uart_hw_init(uint8 ch)
{
    app_uart_state_t *state = &s_uart[ch];

    uart_init(state->index, state->baud, state->tx_pin, state->rx_pin);
    state->inited = true;
}

void app_uart_init(void)
{
    /* 仅清零统计计数，不覆盖静态初始值 (enabled/inited 已在声明时设好) */
    for (uint8 ch = 0; ch < APP_UART_CHANNEL_NUM; ch++)
    {
        s_uart[ch].rx_count = 0;
        s_uart[ch].last_rx  = 0;
    }
}

bool app_uart_set_enable(uint8 ch, bool enable)
{
    if (!app_uart_valid_ch(ch)) return false;

    static UART_Regs *const uart_list[APP_UART_CHANNEL_NUM] = {
        UART0, UART1, UART2, UART3
    };

    if (enable && !s_uart[ch].inited)
        app_uart_hw_init(ch);

    s_uart[ch].enabled = enable;

    /* 硬件层开关 UART 外设。UART0 是系统 debug 串口，只设标志不关硬件。 */
    if (ch == 0) return true;

    if (enable)
        DL_UART_Main_enable(uart_list[ch]);
    else if (s_uart[ch].inited)
        DL_UART_Main_disable(uart_list[ch]);

    return true;
}

const app_uart_state_t *app_uart_get_state(uint8 ch)
{
    if (!app_uart_valid_ch(ch)) return NULL;
    return &s_uart[ch];
}

void app_uart_task(void)
{
    uint8 data;

    for (uint8 ch = 0; ch < APP_UART_CHANNEL_NUM; ch++)
    {
        if (!s_uart[ch].enabled || !s_uart[ch].inited)
            continue;

        // 每次任务最多读取固定字节数，避免串口突发数据拖慢主循环。
        for (uint8 i = 0; i < APP_UART_RX_LIMIT_PER_TASK; i++)
        {
            if (!uart_query_byte(s_uart[ch].index, &data))
                break;

            s_uart[ch].last_rx = data;
            s_uart[ch].rx_count++;
        }
    }
}

const char *app_uart_pin_name(uint32 pin)
{
    switch (pin & UART_PIN_INDEX_MASK)
    {
        case A10: return "A10";
        case A11: return "A11";
        case A21: return "A21";
        case A22: return "A22";
        case B4:  return "B4";
        case B5:  return "B5";
        case B12: return "B12";
        case B13: return "B13";
        default:  return "?";
    }
}
