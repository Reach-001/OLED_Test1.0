# MSPM0G3507 快速上手框架使用说明

本工程基于逐飞库，目标是让新手先快速验证硬件，再逐步添加自己的业务逻辑。底层 GPIO、PWM、ADC、编码器、PIT、串口等接口以逐飞库为准，用户代码只做板级封装和应用逻辑。

## 1. 工程分层

```text
user/
├── config/
│   └── board_config.h      # 唯一参数入口，改引脚、周期、PID 都在这里
├── bsp/
│   ├── bsp_led.*           # LED 板级封装
│   ├── bsp_key.*           # 按键板级封装
│   ├── bsp_motor.*         # 电机板级封装
│   ├── bsp_encoder.*       # 编码器板级封装
│   └── bsp_track.*         # 循迹传感器板级封装
├── app/
│   ├── app_task.*          # 任务调度、系统状态、任务入口
│   └── app_control.*       # PID 和控制算法
└── main/
    └── main.c              # 初始化和主循环
```

推荐约定：

- 改参数只改 `user/config/board_config.h`
- 读写硬件放在 `user/bsp/`
- 业务流程放在 `user/app/`
- `main.c` 只做初始化和调用任务，不堆业务细节

## 2. 程序启动流程

```text
main()
├── system_init()    # 时钟、调试串口
├── bsp_init()       # LED、按键、电机、编码器、循迹传感器
├── app_init()       # 控制模块、任务定时器
├── self_test()      # 上电自检
└── while (1)        # 主循环执行任务
```

`task_init()` 会启动一个 PIT 定时器。定时器中断只设置任务标志，真正的任务函数在 `main.c` 的主循环里执行。

这样做的好处是：中断里不跑复杂逻辑，任务里可以安全调用普通函数和打印。

## 3. 修改参数

所有常用参数集中在：

```text
user/config/board_config.h
```

常见修改项：

```c
#define LED_PIN                 B22
#define KEY1_PIN                A0
#define MOTOR_L_PWM             PWM_TIM_A0_CH0_B8
#define TRACK_THRESHOLD         2000
#define TASK_PERIOD_MS          5
#define TASK_CONTROL_PERIOD     2
#define PID_STEER_KP            50.0f
```

任务周期的计算方式：

```text
任务实际周期 = TASK_PERIOD_MS * TASK_xxx_PERIOD
```

例如：

```c
#define TASK_PERIOD_MS          5
#define TASK_CONTROL_PERIOD     2
```

控制任务周期就是 `5ms * 2 = 10ms`。

## 4. 如何添加一个新任务

示例：添加一个 `uart` 串口处理任务，每 50ms 执行一次。

### 4.1 在 board_config.h 添加周期

```c
#define TASK_UART_PERIOD        10      /* 10 * 5ms = 50ms */
```

### 4.2 在 app_task.h 添加任务标志和函数声明

```c
typedef struct
{
    uint8 track;
    uint8 control;
    uint8 display;
    uint8 key;
    uint8 uart;        /* 新增 */
} task_flag_t;

void task_uart(void);
```

### 4.3 在 app_task.c 的 task_timer_callback() 设置标志

```c
if (s_task_tick % TASK_UART_PERIOD == 0)
{
    g_task_flag.uart = 1;
}
```

### 4.4 在 app_task.c 实现任务逻辑

```c
void task_uart(void)
{
    /* 这里写串口接收解析、状态上传等逻辑 */
}
```

### 4.5 在 main.c 主循环执行任务

```c
if (g_task_flag.uart)
{
    g_task_flag.uart = 0;
    task_uart();
}
```

## 5. 任务里应该写什么

适合写在任务里的内容：

- 读取传感器
- 更新状态机
- PID 计算
- 电机输出
- 按键事件处理
- 串口解析
- 周期性打印

不建议写在任务里的内容：

- 长时间 `while` 等待
- 很长的 `system_delay_ms()`
- 大量阻塞式打印
- 会卡住主循环的测试代码

如果必须等待外设响应，建议拆成“状态机”：本次任务发命令，下次任务检查结果。

## 6. 如何写一个新业务逻辑

推荐新建一个应用模块，例如：

```text
user/app/app_user.c
user/app/app_user.h
```

典型接口：

```c
void app_user_init(void);
void app_user_task(void);
```

然后：

1. 在 `app_init()` 中调用 `app_user_init()`
2. 新增一个任务标志，例如 `user`
3. 在 `task_user()` 中调用 `app_user_task()`

业务模块可以调用 BSP 接口，例如：

```c
track_read(&data);
motor_set_speed_both(left, right);
bsp_key_get_event(BSP_KEY_1);
```

不要在业务模块里直接写一堆 GPIO/PWM 初始化。硬件细节尽量放在 BSP 层。

## 7. 当前内置任务说明

| 任务 | 函数 | 默认周期 | 作用 |
|------|------|----------|------|
| 循迹采样 | `task_track()` | 5ms | 读取循迹传感器 |
| 控制计算 | `task_control()` | 10ms | 编码器读取、PID、输出电机 |
| 显示打印 | `task_display()` | 100ms | 串口打印状态 |
| 按键处理 | `task_key()` | 20ms | 扫描按键、启动停止、调速 |

默认上电后不会直接运行电机。按 `KEY1` 启动，再按 `KEY1` 停止。

## 8. 常见问题

### 编译找不到函数或变量

检查对应 `.c` 文件是否在 `.eide/eide.yml` 的 `srcDirs` 覆盖目录下。用户代码建议放在：

```text
user/main
user/bsp
user/app
```

### 新增任务不执行

依次检查：

- `board_config.h` 有没有定义 `TASK_xxx_PERIOD`
- `task_flag_t` 有没有新增标志
- `task_timer_callback()` 有没有置位
- `main.c` 主循环有没有清标志并调用任务函数

### 电机上电就动

检查 `motor_init()` 和 `task_control()`。框架默认初始化后停止电机，只有系统状态为 `SYS_STATE_RUNNING` 才输出控制量。

### 串口打印太多导致卡顿

降低 `TASK_DISPLAY_PERIOD` 的执行频率，或减少 `printf` 内容。串口打印属于慢操作，不适合高频执行。

## 9. 推荐开发顺序

1. 先改 `board_config.h` 的 LED、按键、传感器、电机、编码器引脚
2. 编译并下载
3. 看 LED 自检和串口输出
4. 检查按键是否能启动/停止
5. 检查循迹 ADC 原始值
6. 检查编码器方向
7. 低速测试电机方向
8. 再调 PID 参数
