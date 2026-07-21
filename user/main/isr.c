/*
 * 文件名称: isr.c
 * 文件作用: 中断服务函数实现 - 将硬件中断向量路由到逐飞库回调机制
 *
 * 修改记录:
 * 日期              备注
 * 2026-07-17        初始版本
 */

#include "isr.h"

/* ==================== 定时器中断（PIT 周期中断） ==================== */

/** @brief TIMA0 中断 - 对应 pit_index 0 */
void TIMA0_IRQHandler(void)
{
    pit_callback_list[0](0, pit_callback_ptr_list[0]);
}

/** @brief TIMA1 中断 - 对应 pit_index 1 */
void TIMA1_IRQHandler(void)
{
    pit_callback_list[1](0, pit_callback_ptr_list[1]);
}

/** @brief TIMG0 中断 - 对应 pit_index 2 */
void TIMG0_IRQHandler(void)
{
    pit_callback_list[2](0, pit_callback_ptr_list[2]);
}

/** @brief TIMG6 中断 - 对应 pit_index 3 */
void TIMG6_IRQHandler(void)
{
    pit_callback_list[3](0, pit_callback_ptr_list[3]);
}

/** @brief TIMG7 中断 - 对应 pit_index 4 */
void TIMG7_IRQHandler(void)
{
    pit_callback_list[4](0, pit_callback_ptr_list[4]);
}

/** @brief TIMG8 中断 - 对应 pit_index 5 */
void TIMG8_IRQHandler(void)
{
    pit_callback_list[5](0, pit_callback_ptr_list[5]);
}

/** @brief TIMG12 中断 - 对应 pit_index 6 */
void TIMG12_IRQHandler(void)
{
    pit_callback_list[6](0, pit_callback_ptr_list[6]);
}

/* ==================== 串口中断（UART） ==================== */

/** @brief UART0 中断 - 发送/接收事件分发 */
void UART0_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART0))
    {
        case DL_UART_IIDX_TX:
            uart_callback_list[0](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[0]);
            break;
        case DL_UART_IIDX_RX:
            uart_callback_list[0](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[0]);
#if DEBUG_UART_USE_INTERRUPT && (APP_DEBUG_UART_ID == 0)
            debug_interrupr_handler();
#endif
            break;
        default: break;
    }
    DL_UART_clearInterruptStatus(UART0, UART0->CPU_INT.RIS);
}

/** @brief UART1 中断 - 发送/接收事件分发 */
void UART1_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART1))
    {
        case DL_UART_IIDX_TX:
            uart_callback_list[1](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[1]);
            break;
        case DL_UART_IIDX_RX:
            uart_callback_list[1](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[1]);
#if DEBUG_UART_USE_INTERRUPT && (APP_DEBUG_UART_ID == 1)
            debug_interrupr_handler();
#else
            wireless_module_uart_handler(); /* 无线串口模块回调 */
#endif
            break;
        default: break;
    }
    DL_UART_clearInterruptStatus(UART1, UART1->CPU_INT.RIS);
}

/** @brief UART2 中断 - 发送/接收事件分发 */
void UART2_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART2))
    {
        case DL_UART_IIDX_TX:
            uart_callback_list[2](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[2]);
            break;
        case DL_UART_IIDX_RX:
            uart_callback_list[2](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[2]);
#if DEBUG_UART_USE_INTERRUPT && (APP_DEBUG_UART_ID == 2)
            debug_interrupr_handler();
#endif
            break;
        default: break;
    }
    DL_UART_clearInterruptStatus(UART2, UART2->CPU_INT.RIS);
}

/** @brief UART3 中断 - 发送/接收事件分发 */
void UART3_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART3))
    {
        case DL_UART_IIDX_TX:
            uart_callback_list[3](UART_INTERRUPT_STATE_TX, uart_callback_ptr_list[3]);
            break;
        case DL_UART_IIDX_RX:
            uart_callback_list[3](UART_INTERRUPT_STATE_RX, uart_callback_ptr_list[3]);
#if DEBUG_UART_USE_INTERRUPT && (APP_DEBUG_UART_ID == 3)
            debug_interrupr_handler();
#endif
            break;
        default: break;
    }
    DL_UART_clearInterruptStatus(UART3, UART3->CPU_INT.RIS);
}

/* ==================== GPIO 外部中断 ==================== */

/**
 * @brief GROUP1 中断 - GPIO EXTI 事件分发
 *
 * MSPM0G3507 的所有 GPIO 外部中断共用 GROUP1 向量，
 * 驱动层通过 CPU_INT.IIDX 寄存器区分具体引脚，
 * 再查询 POLARITY 寄存器获取边沿方向后调用用户回调。
 */
void GROUP1_IRQHandler(void)
{
    uint8  exti_index = 0;
    uint8  exti_event = 0;
    uint32 register_temp;

    /* 检查 GPIOA */
    register_temp = gpio_group[0]->CPU_INT.IIDX;
    if (register_temp)
    {
        exti_index = register_temp - 1;
        exti_event = (exti_index <= 15)
            ? (gpio_group[0]->POLARITY15_0  >> ((exti_index % 16) * 2)) & 0x03
            : (gpio_group[0]->POLARITY31_16 >> ((exti_index % 16) * 2)) & 0x03;
        exti_callback_list[exti_index](exti_event, exti_callback_ptr_list[exti_index]);
    }
    else
    {
        /* 检查 GPIOB */
        register_temp = gpio_group[1]->CPU_INT.IIDX;
        if (register_temp)
        {
            exti_index = register_temp - 1;
            exti_event = (exti_index <= 15)
                ? (gpio_group[1]->POLARITY15_0  >> ((exti_index % 16) * 2)) & 0x03
                : (gpio_group[1]->POLARITY31_16 >> ((exti_index % 16) * 2)) & 0x03;
            exti_callback_list[exti_index](exti_event, exti_callback_ptr_list[exti_index]);
        }
    }
}
