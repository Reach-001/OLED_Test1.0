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
    { false, false, UART_0, UART0_TX_A10,       UART0_RX_A11,       APP_UART_BAUD, 0, 0, 0 },
    { false, false, UART_1, WIRELESS_UART_TX,   WIRELESS_UART_RX,   APP_UART_BAUD, 0, 0, 0 },
    { false, false, UART_2, UART2_TX_A21,       UART2_RX_A22,       APP_UART_BAUD, 0, 0, 0 },
    { false, false, UART_3, UART3_TX_B12,       UART3_RX_B13,       APP_UART_BAUD, 0, 0, 0 },
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
#if !APP_UART_ENABLE
    for (uint8 ch = 0; ch < APP_UART_CHANNEL_NUM; ch++)
    {
        s_uart[ch].enabled = false;
        s_uart[ch].inited = false;
        s_uart[ch].tx_count = 0;
        s_uart[ch].rx_count = 0;
        s_uart[ch].last_rx  = 0;
    }
#else
    /* 仅清零统计计数，不覆盖静态初始值 (enabled/inited 已在声明时设好) */
    for (uint8 ch = 0; ch < APP_UART_CHANNEL_NUM; ch++)
    {
        s_uart[ch].tx_count = 0;
        s_uart[ch].rx_count = 0;
        s_uart[ch].last_rx  = 0;
    }

    /* 上电默认开启 UART0（与 debug 串口共用，debug_init 已完成硬件初始化）
     * 和 UART1（无线/蓝牙模块），避免每次上电都要进 UI 手动打开。 */
    s_uart[0].enabled = true;
    s_uart[0].inited  = true;           /* 硬件由 debug_init() 初始化，这里不重复操作 */
    (void)app_uart_set_enable(1, true); /* 初始化 UART1 硬件并开启 */
#endif
}

bool app_uart_set_enable(uint8 ch, bool enable)
{
#if !APP_UART_ENABLE
    (void)ch;
    (void)enable;
    return false;
#else
    if (!app_uart_valid_ch(ch)) return false;

    static UART_Regs *const uart_list[APP_UART_CHANNEL_NUM] = {
        UART0, UART1, UART2, UART3
    };

    /* 首次开启时初始化硬件 */
    if (enable && !s_uart[ch].inited)
        app_uart_hw_init(ch);

    s_uart[ch].enabled = enable;

    /* 硬件开关。disable → SWRST 复位配置 → inited=false */
    if (enable)
        DL_UART_Main_enable(uart_list[ch]);
    else {
        DL_UART_Main_disable(uart_list[ch]);
        s_uart[ch].inited = false;
    }

    return true;
#endif
}

void app_uart_send_test(uint8 ch)
{
    if (!app_uart_valid_ch(ch)) return;
    if (!s_uart[ch].enabled || !s_uart[ch].inited) return;

    const char *msg = "Hello World!\r\n";
    uart_write_string(s_uart[ch].index, msg);
    s_uart[ch].tx_count += 13;  /* 13 字节: "Hello World!\r\n" 不含 '\0' */
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
