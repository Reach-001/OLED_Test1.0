/**
 * @file    app_uart.h
 * @brief   UART 应用层控制接口
 */

#ifndef _APP_UART_H_
#define _APP_UART_H_

#include "board_config.h"
#include "zf_driver_uart.h"
#include <stdbool.h>

#define APP_UART_CHANNEL_NUM 4

typedef struct
{
    bool enabled;
    bool inited;
    uart_index_enum index;
    uart_tx_pin_enum tx_pin;
    uart_rx_pin_enum rx_pin;
    uint32 baud;
    uint32 rx_count;
    uint8 last_rx;
} app_uart_state_t;

void app_uart_init(void);
void app_uart_task(void);
bool app_uart_set_enable(uint8 ch, bool enable);
const app_uart_state_t *app_uart_get_state(uint8 ch);
const char *app_uart_pin_name(uint32 pin);

#endif /* _APP_UART_H_ */
