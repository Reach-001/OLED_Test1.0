# MSPM0G3507 逐飞库快速入门模板

基于 TI MSPM0G3507 + 逐飞科技开源库的 GCC 工程模板，适用于智能车、机器人等嵌入式项目开发。

## 芯片规格

| 参数 | 数值 |
|------|------|
| 内核 | ARM Cortex-M0+ |
| 主频 | 80 MHz (SYSPLL) |
| Flash | 128 KB |
| SRAM | 32 KB |
| GPIO | GPIOA (A0-A31) + GPIOB (B0-B27) |

## 工程结构

```
├── user/main/           # 用户代码（在此开发）
│   ├── main.c           # 主程序入口
│   └── isr.c            # 中断服务函数
├── source/core/         # 逐飞库源码（勿修改）
│   ├── zf_common/       # 公共模块
│   ├── zf_driver/       # 外设驱动
│   └── zf_device/       # 设备驱动
├── include/core/zf/     # 逐飞库头文件
├── linker/              # 链接脚本 + SVD
├── ti_msp_dl_config.*   # SysConfig 生成的时钟配置
└── empty.syscfg         # SysConfig 工程文件
```

## 快速开始

### 环境准备

1. **编译器**: ARM GCC (arm-none-eabi-gcc)
2. **IDE**: VS Code + EIDE 插件
3. **调试器**: CMSIS-DAP + OpenOCD
4. **配置工具**: TI SysConfig (可选，用于修改时钟)

### 编译 & 烧录

```bash
# VS Code 中
Ctrl+Shift+B → build        # 编译
Ctrl+Shift+B → flash        # 烧录
F5                          # 调试
```

### 代码模板位置

用户代码写在 `user/main/` 目录下：

```
user/main/
├── main.c    ← 主程序（在此写初始化和主循环）
└── isr.c     ← 中断服务（一般不需要修改）
```

### 框架使用说明

新手建议先阅读：

```text
docs/FRAMEWORK_USAGE.md
```

该文档说明了如何修改参数、添加任务、编写业务逻辑，以及任务调度的完整流程。

### main.c 完整模板

```c
/*
 * 文件名称: main.c
 * 文件作用: 主程序入口
 * 硬件连接: 根据实际项目填写
 */

#include "zf_common_headfile.h"

/*-----------------------------------------------------------
 * 用户全局变量定义
 *-----------------------------------------------------------*/
// 在此定义全局变量


/*-----------------------------------------------------------
 * 定时器回调函数（如需要）
 *-----------------------------------------------------------*/
void pit_10ms_callback(uint32 flag, void *param)
{
    // 10ms 定时任务
}

/*-----------------------------------------------------------
 * 主函数
 *-----------------------------------------------------------*/
int main(void)
{
    /*==================== 系统初始化 ====================*/
    clock_init(SYSTEM_CLOCK_80M);   // 时钟初始化（必须第一个调用）
    debug_init();                    // 调试串口（UART0, 115200）

    /*==================== 外设初始化 ====================*/
    // GPIO
    gpio_init(B22, GPO, GPIO_LOW, GPO_PUSH_PULL);   // LED

    // UART
    // uart_init(UART_1, 115200, UART1_TX_A8, UART1_RX_A9);

    // PWM
    // pwm_init(PWM_TIM_A0_CH0_A8, 10000, 0);

    // ADC
    // adc_init(ADC0_CH0_A27, ADC_12BIT);

    // 编码器
    // encoder_quad_init(TIM_G8, TIMG8_ENCODER1_CH1_B10, TIMG8_ENCODER1_CH2_B11);

    // 定时中断
    // pit_ms_init(PIT_TIM_A0, 10, pit_10ms_callback, NULL);

    /*==================== 用户初始化代码 ====================*/
    // 在此添加其他初始化


    /*==================== 主循环 ====================*/
    while (1)
    {
        // 在此编写主循环代码

        gpio_toggle_level(B22);
        system_delay_ms(500);
    }
}
```

### 最小示例（LED 闪烁）

```c
#include "zf_common_headfile.h"

int main(void)
{
    clock_init(SYSTEM_CLOCK_80M);
    debug_init();
    
    gpio_init(B22, GPO, GPIO_LOW, GPO_PUSH_PULL);
    
    while (1)
    {
        gpio_toggle_level(B22);
        system_delay_ms(500);
    }
}
```

---

# 逐飞库 API 详解

## 1. 系统初始化

每个程序必须首先调用：

```c
clock_init(SYSTEM_CLOCK_80M);  // 时钟初始化，必须第一个调用
debug_init();                   // 调试串口初始化（UART0, 115200）
```

**注意**：逐飞库在运行时动态初始化外设，无需在 SysConfig 中预配置（时钟除外）。

---

## 2. GPIO 通用输入输出

### 引脚命名

```c
A0, A1, A2 ... A31   // GPIOA 端口
B0, B1, B2 ... B27   // GPIOB 端口
```

### 初始化

```c
// gpio_init(引脚, 方向, 初始电平, 模式)

// 输出模式
gpio_init(B22, GPO, GPIO_LOW,  GPO_PUSH_PULL);   // 推挽输出，默认低
gpio_init(B22, GPO, GPIO_HIGH, GPO_OPEN_DRAIN);  // 开漏输出，默认高

// 输入模式
gpio_init(A0, GPI, GPIO_HIGH, GPI_PULL_UP);      // 上拉输入
gpio_init(A1, GPI, GPIO_LOW,  GPI_PULL_DOWN);    // 下拉输入
gpio_init(A2, GPI, GPIO_LOW,  GPI_FLOATING_IN);  // 浮空输入
gpio_init(A3, GPI, GPIO_LOW,  GPI_ANAOG_IN);     // 模拟输入（ADC用）
```

### 输出操作

```c
gpio_high(B22);              // 输出高电平
gpio_low(B22);               // 输出低电平
gpio_toggle_level(B22);      // 电平翻转
gpio_set_level(B22, 1);      // 设置电平（0 或 1）
```

### 输入操作

```c
uint8 level = gpio_get_level(A0);  // 读取引脚电平
```

### 完整引脚列表

| 端口 | 引脚范围 | 说明 |
|------|----------|------|
| GPIOA | A0 - A31 | 32 个引脚 |
| GPIOB | B0 - B27 | 28 个引脚 |

---

## 3. 延时函数

```c
system_delay_ms(100);   // 毫秒延时
system_delay_us(50);    // 微秒延时
```

---

## 4. UART 串口通信

### 资源概览

| 串口 | TX 引脚 | RX 引脚 |
|------|---------|---------|
| UART0 | A0, A10, A28, B0 | A1, A11, A31, B1 |
| UART1 | A8, A17, B4, B6 | A9, A18, B5, B7 |
| UART2 | A21, A23, B15, B17 | A22, A24, B16, B18 |
| UART3 | A14, A26, B2, B12 | A13, A25, B3, B13 |

### 初始化

```c
// uart_init(串口号, 波特率, TX引脚, RX引脚)
uart_init(UART_1, 115200, UART1_TX_A8, UART1_RX_A9);
uart_init(UART_2, 9600,   UART2_TX_B17, UART2_RX_B18);
```

### 发送数据

```c
uart_write_byte(UART_1, 0x55);                      // 发送单字节
uart_write_string(UART_1, "Hello World!\r\n");      // 发送字符串
uart_write_buffer(UART_1, data_buf, sizeof(buf));   // 发送数组
```

### 接收数据

```c
uint8 dat;

// 非阻塞查询（推荐）
if (uart_query_byte(UART_1, &dat))
{
    // 收到数据 dat
}

// 阻塞等待（有超时）
uart_read_byte(UART_1, &dat);
```

### 中断接收

```c
// 设置回调函数
void uart1_callback(uint32 state, void *param)
{
    if (state == UART_INTERRUPT_STATE_RX)
    {
        uint8 dat;
        uart_query_byte(UART_1, &dat);
        // 处理接收数据
    }
}

// 初始化时设置
uart_init(UART_1, 115200, UART1_TX_A8, UART1_RX_A9);
uart_set_callback(UART_1, uart1_callback, NULL);
uart_set_interrupt_config(UART_1, UART_INTERRUPT_CONFIG_RX_ENABLE);
```

---

## 5. PWM 脉冲宽度调制

### 资源概览

| 定时器 | 通道数 | 可用引脚 |
|--------|--------|----------|
| TIMA0 | CH0-CH3 | A0/A8/A21/B8/B14, A1/A3/A7/A9/A22/B9/B12/B20, A3/A7/A10/A15/B0/B4/B12/B17/B20, A4/A12/A17/A23/A25/A28/B2/B13/B24/B26 |
| TIMA1 | CH0-CH1 | A10/A15/A17/A28/B0/B2/B4/B17/B26, A11/A16/A18/A24/A31/B1/B3/B5/B18/B27 |
| TIMG0 | CH0-CH1 | A5/A12/A23/B10, A6/A13/A24/B11 |
| TIMG6 | CH0-CH1 | A5/A21/A29/B2/B6/B10/B26, A6/A22/A30/B3/B7/B11/B27 |
| TIMG7 | CH0-CH1 | A3/A17/A23/A26/A28/B15, A2/A4/A7/A18/A24/A27/A31/B16/B19 |
| TIMG8 | CH0-CH1 | A1/A3/A5/A7/A21/A23/A26/A29/B6/B10/B15/B21, A0/A2/A4/A6/A22/A27/A30/B7/B11/B16/B19/B22 |
| TIMG12 | CH0-CH1 | A10/A14/B13/B20, A25/A31/B14/B24 |

### 初始化与控制

```c
// pwm_init(通道, 频率Hz, 占空比‰)
pwm_init(PWM_TIM_A0_CH0_A8, 10000, 5000);   // 10kHz, 50%占空比
pwm_init(PWM_TIM_G8_CH0_B6, 20000, 2500);   // 20kHz, 25%占空比

// 动态调整占空比（0-10000 对应 0%-100%）
pwm_set_duty(PWM_TIM_A0_CH0_A8, 7500);      // 改为 75%
pwm_set_duty(PWM_TIM_A0_CH0_A8, 0);         // 停止输出
```

### 电机控制示例

```c
// 初始化 4 路 PWM（双 H 桥电机驱动）
pwm_init(PWM_TIM_A0_CH0_A8, 10000, 0);   // 电机1 正转
pwm_init(PWM_TIM_A0_CH1_A9, 10000, 0);   // 电机1 反转
pwm_init(PWM_TIM_A0_CH2_A10, 10000, 0);  // 电机2 正转
pwm_init(PWM_TIM_A0_CH3_A12, 10000, 0);  // 电机2 反转

// 电机1 正转 50%
pwm_set_duty(PWM_TIM_A0_CH0_A8, 5000);
pwm_set_duty(PWM_TIM_A0_CH1_A9, 0);
```

---

## 6. ADC 模数转换

### 资源概览

| ADC | 通道 | 引脚 |
|-----|------|------|
| ADC0 | CH0-CH7 | A27, A26, A25, A24, B25, B24, B20, A22 |
| ADC1 | CH0-CH7 | A15, A16, A17, A18, B17, B18, B19, A21 |

### 初始化与采集

```c
// adc_init(ADC引脚, 分辨率)
adc_init(ADC0_CH0_A27, ADC_12BIT);   // 12位分辨率
adc_init(ADC1_CH0_A15, ADC_10BIT);   // 10位分辨率

// 单次采集
uint16 value = adc_convert(ADC0_CH0_A27);

// 均值滤波采集（采 10 次取平均）
uint16 value = adc_mean_filter_convert(ADC0_CH0_A27, 10);
```

### 电压换算

```c
// 12位 ADC，参考电压 3.3V
float voltage = adc_convert(ADC0_CH0_A27) * 3.3f / 4096.0f;
```

---

## 7. PIT 定时中断

### 资源概览

| PIT 编号 | 对应定时器 |
|----------|-----------|
| PIT_TIM_A0 | TIMA0 |
| PIT_TIM_A1 | TIMA1 |
| PIT_TIM_G0 | TIMG0 |
| PIT_TIM_G6 | TIMG6 |
| PIT_TIM_G7 | TIMG7 |
| PIT_TIM_G8 | TIMG8 |
| PIT_TIM_G12 | TIMG12 |

### 初始化与使用

```c
// 回调函数
void pit_callback(uint32 flag, void *param)
{
    // 定时执行的代码
    gpio_toggle_level(B22);
}

// 毫秒级定时中断
pit_ms_init(PIT_TIM_A0, 10, pit_callback, NULL);   // 10ms 周期

// 微秒级定时中断
pit_us_init(PIT_TIM_G0, 500, pit_callback, NULL);  // 500us 周期

// 控制
pit_disable(PIT_TIM_A0);   // 禁止中断
pit_enable(PIT_TIM_A0);    // 使能中断
```

---

## 8. EXTI 外部中断

支持所有 GPIO 引脚作为外部中断源。

### 初始化与使用

```c
// 回调函数
void key_callback(uint32 event, void *param)
{
    // event: EXTI_TRIGGER_RISING / EXTI_TRIGGER_FALLING / EXTI_TRIGGER_BOTH
    if (event == EXTI_TRIGGER_FALLING)
    {
        // 下降沿触发
    }
}

// exti_init(引脚, 触发方式, 回调函数, 用户参数)
exti_init(A0, EXTI_TRIGGER_FALLING, key_callback, NULL);  // 下降沿
exti_init(A1, EXTI_TRIGGER_RISING, key_callback, NULL);   // 上升沿
exti_init(A2, EXTI_TRIGGER_BOTH, key_callback, NULL);     // 双边沿

// 控制
exti_disable(A0);   // 禁止中断
exti_enable(A0);    // 使能中断
```

---

## 9. 编码器接口

### 资源概览（正交编码器仅 TIMG8 支持）

| 定时器 | CH1 引脚 | CH2 引脚 |
|--------|----------|----------|
| TIMG8 | A1/A3/A5/A7/A21/A23/A26/A29/B6/B10/B15/B21 | A0/A2/A4/A6/A22/A27/A30/B7/B11/B16/B19/B22 |

其他定时器(TIMA0/TIMA1/TIMG0/TIMG6/TIMG7/TIMG12)支持方向编码器模式。

### 正交编码器（AB 相）

```c
// encoder_quad_init(定时器, CH1引脚, CH2引脚)
encoder_quad_init(TIM_G8, TIMG8_ENCODER1_CH1_B10, TIMG8_ENCODER1_CH2_B11);

// 读取计数值（有符号，正负表示方向）
int16 count = encoder_get_count(TIM_G8);

// 清零
encoder_clear_count(TIM_G8);
```

### 方向编码器（脉冲 + 方向）

```c
// encoder_dir_init(定时器, 脉冲引脚, 方向引脚)
encoder_dir_init(TIM_A0, TIMA0_ENCODER1_CH1_A8, A9);

int16 count = encoder_get_count(TIM_A0);
encoder_clear_count(TIM_A0);
```

---

## 10. SPI 通信

### 资源概览

| SPI | SCK 引脚 | MOSI 引脚 | MISO 引脚 | CS 引脚 |
|-----|----------|-----------|-----------|---------|
| SPI0 | A6/A11/A12/B18 | A5/A9/A14/B17 | A4/A10/A13/B19 | A2/A8/B25 |
| SPI1 | A17/B9/B16/B23 | A18/B8/B15/B22 | A16/B7/B14/B21 | A2/A26/B6/B20 |

### 初始化

```c
// spi_init(SPI号, 模式, 波特率, SCK, MOSI, MISO, CS)
spi_init(SPI_0, SPI_MODE0, 1000000, 
         SPI0_SCK_A6, SPI0_MOSI_A5, SPI0_MISO_A4, SPI0_CS_A2);

// 不使用硬件 MISO 或 CS
spi_init(SPI_0, SPI_MODE0, 1000000, 
         SPI0_SCK_A6, SPI0_MOSI_A5, SPI_MISO_NULL, SPI_CS_NULL);
```

### SPI 模式

| 模式 | CPOL | CPHA | 说明 |
|------|------|------|------|
| SPI_MODE0 | 0 | 0 | 空闲低电平，第一边沿采样 |
| SPI_MODE1 | 0 | 1 | 空闲低电平，第二边沿采样 |
| SPI_MODE2 | 1 | 0 | 空闲高电平，第一边沿采样 |
| SPI_MODE3 | 1 | 1 | 空闲高电平，第二边沿采样 |

### 数据传输

```c
// 写
spi_write_8bit(SPI_0, 0xAA);
spi_write_8bit_array(SPI_0, data, len);
spi_write_16bit(SPI_0, 0x1234);

// 读
uint8 dat = spi_read_8bit(SPI_0);
spi_read_8bit_array(SPI_0, buffer, len);

// 寄存器操作
spi_write_8bit_register(SPI_0, 0x20, 0x01);
uint8 val = spi_read_8bit_register(SPI_0, 0x20);

// 全双工传输
spi_transfer_8bit(SPI_0, tx_buf, rx_buf, len);
```

---

## 11. 软件 IIC

软件模拟 IIC，可使用任意 GPIO 引脚。

### 初始化

```c
soft_iic_info_struct iic_dev;

// soft_iic_init(结构体, 7位设备地址, 延时, SCL引脚, SDA引脚)
soft_iic_init(&iic_dev, 0x68, 100, A6, A7);   // MPU6050 地址 0x68
soft_iic_init(&iic_dev, 0x50, 100, B10, B11); // EEPROM 地址 0x50
```

### 数据传输

```c
// 写寄存器
soft_iic_write_8bit_register(&iic_dev, 0x6B, 0x00);        // 单字节
soft_iic_write_8bit_registers(&iic_dev, 0x00, data, len);  // 多字节

// 读寄存器
uint8 val = soft_iic_read_8bit_register(&iic_dev, 0x75);   // 单字节
soft_iic_read_8bit_registers(&iic_dev, 0x3B, buffer, 14);  // 多字节

// 16位寄存器地址
soft_iic_write_16bit_register(&iic_dev, 0x0000, 0x1234);
uint16 val = soft_iic_read_16bit_register(&iic_dev, 0x0000);

// SCCB 协议（摄像头）
soft_iic_sccb_write_register(&iic_dev, 0x12, 0x80);
uint8 val = soft_iic_sccb_read_register(&iic_dev, 0x0A);
```

---

## 12. Flash 存储

用于保存参数、配置等掉电不丢失的数据。

### 存储规格

| 参数 | 数值 |
|------|------|
| 基地址 | 0x00016000 |
| 扇区数 | 6 个 |
| 每扇区页数 | 2 页 |
| 页大小 | 1024 字节 |
| 擦除单位 | 页 |

### 直接读写

```c
uint32 write_data[4] = {100, 200, 300, 400};
uint32 read_data[4];

// 擦除（写入前必须擦除）
flash_erase_page(0, 0);   // 扇区0，页0

// 写入
flash_write_page(0, 0, write_data, 4);

// 读取
flash_read_page(0, 0, read_data, 4);
```

### 使用缓冲区

```c
// 读取到缓冲区
flash_read_page_to_buffer(0, 0);

// 修改缓冲区数据
flash_union_buffer[0].float_type = 3.14f;
flash_union_buffer[1].int32_type = -100;
flash_union_buffer[2].uint16_type = 1000;

// 写回 Flash
flash_erase_page(0, 0);
flash_write_page_from_buffer(0, 0);

// 清空缓冲区
flash_buffer_clear();
```

---

## 13. 调试打印

`debug_init()` 初始化后，可使用标准 `printf`：

```c
printf("System Clock: %d Hz\r\n", system_clock);
printf("ADC Value: %d\r\n", adc_value);
printf("Float: %.2f\r\n", 3.14159f);
```

---

# 中断配置说明

所有中断服务函数已在 `isr.c` 中实现，采用回调机制：

| 中断源 | 中断向量 | 回调设置方式 |
|--------|----------|--------------|
| 定时器 | TIMx_IRQHandler | `pit_xx_init()` 时传入回调 |
| 串口 | UARTx_IRQHandler | `uart_set_callback()` |
| 外部中断 | GROUP1_IRQHandler | `exti_init()` 时传入回调 |

---

# SysConfig 与逐飞库的关系

| 配置项 | 配置方式 | 说明 |
|--------|----------|------|
| 时钟树 | SysConfig | HFXT/LFXT/SYSPLL 配置 |
| GPIO | 逐飞库 | `gpio_init()` 运行时配置 |
| UART | 逐飞库 | `uart_init()` 运行时配置 |
| PWM | 逐飞库 | `pwm_init()` 运行时配置 |
| ADC | 逐飞库 | `adc_init()` 运行时配置 |
| SPI | 逐飞库 | `spi_init()` 运行时配置 |
| 定时器 | 逐飞库 | `pit_xx_init()` 运行时配置 |

**核心原则**：SysConfig 只管时钟，逐飞库管所有外设。

---

# 许可证

- 逐飞库：GPL-3.0
- TI DriverLib：BSD-3-Clause

---

# 参考资料

- [MSPM0G3507 数据手册](https://www.ti.com/product/MSPM0G3507)
- [逐飞科技官方淘宝店](https://seekfree.taobao.com/)
- [TI MSPM0 SDK](https://www.ti.com/tool/MSPM0-SDK)
