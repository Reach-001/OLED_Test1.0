#ifndef _APP_DEBUG_CONFIG_H_
#define _APP_DEBUG_CONFIG_H_

/* Debug UART 使用 UART0-A10/A11，与无线串口 UART1 分开。 */
#define APP_DEBUG_UART_ID           0
#define DEBUG_UART_INDEX            ( UART_0 )
#define DEBUG_UART_BAUDRATE         ( 115200 )
#define DEBUG_UART_TX_PIN           ( UART0_TX_A10 )
#define DEBUG_UART_RX_PIN           ( UART0_RX_A11 )
#define DEBUG_UART_USE_INTERRUPT    ( 1 )
#define DEBUG_UART_PRIORITY         ( UART0_INT_IRQn )
#define DEBUG_UART_PRIORITY0_7      ( 1 )

#endif
