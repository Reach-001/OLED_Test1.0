/*********************************************************************************************************************
* MSPM0G3507 Opensource Library 即（MSPM0G3507 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3507 开源库的一部分
* 
* MSPM0G3507 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          zf_driver_pit
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2025-06-1        SeekFree            first version
********************************************************************************************************************/

#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_common_interrupt.h"
#include "zf_driver_timer.h"

#include "zf_driver_pit.h"

const IRQn_Type irq_index[PIT_NUM] = {
    TIMA0_INT_IRQn, TIMA1_INT_IRQn, TIMG0_INT_IRQn, TIMG6_INT_IRQn,
    TIMG7_INT_IRQn, TIMG8_INT_IRQn, TIMG12_INT_IRQn};

static  void pit_callbakc_defalut (uint32 event, void *ptr);
void_callback_uint32_ptr pit_callback_list[PIT_NUM] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
void *pit_callback_ptr_list[PIT_NUM] = 
{
    pit_callbakc_defalut, pit_callbakc_defalut, pit_callbakc_defalut, pit_callbakc_defalut,
    pit_callbakc_defalut, pit_callbakc_defalut, pit_callbakc_defalut
};


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断默认回调函数
// 参数说明     event               触发中断的事件 此处默认为 0
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void pit_callbakc_defalut (uint32 event, void *ptr)
{
    (void)event;
    (void)ptr;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 定时器分频设置
// 参数说明     pit_n           PIT 外设模块号
// 参数说明     period_us       需要定时器的时间 单位微妙
// 参数说明     clock_src       定时器输入的时钟频率
// 返回参数     void
// 使用示例     pit_set_div(pit_n, 1000, 80000000);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void pit_set_div (pit_index_enum pit_n, uint32 period_us, uint32 clock_src)
{
    uint32 count;
    uint32 temp_div;
    uint16 load_reg;
    uint8  clkdiv_reg;
    uint8  cps_reg;
    
    GPTIMER_Regs *timer_obj;
    timer_obj = timer_reg[pit_n];
    
    // 计算分频系数
    count       = period_us * (clock_src / 1000000);
    temp_div    = count >> 16;
    clkdiv_reg  = temp_div >> 8;
    if((0 == (count % 65536)) && (0 == (temp_div % 256)))
    {
        clkdiv_reg--;
        cps_reg = temp_div / (clkdiv_reg + 1) - 1;
    }
    else
    {
        cps_reg = temp_div / (clkdiv_reg + 1);
    }

    load_reg    = count / (clkdiv_reg + 1) / (cps_reg + 1) - 1;
    
    // 如果这里报错，则说明设定的时间太大，定时器无法实现
    zf_assert(7 >= clkdiv_reg);
    
    timer_obj->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
    timer_obj->CLKDIV = clkdiv_reg;
    timer_obj->COMMONREGS.CPS = cps_reg;
    timer_obj->COUNTERREGS.LOAD = load_reg;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断使能
// 参数说明     pit_n           PIT 外设模块号
// 返回参数     void
// 使用示例     pit_enable(pit_n);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pit_enable (pit_index_enum pit_n)
{
    timer_reg[pit_n]->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断禁止
// 参数说明     pit_n           PIT 外设模块号
// 返回参数     void
// 使用示例     pit_disable(pit_n);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pit_disable (pit_index_enum pit_n)
{
    timer_reg[pit_n]->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 初始化 一般调用 pit_ms_init 或 pit_us_init
// 参数说明     pit_n               PIT 外设模块号
// 参数说明     period              PIT 周期 一般是芯片或者模块时钟频率计算
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     pit_init(pit_n, period, callback, NULL);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pit_init (pit_index_enum pit_n, uint32 period, void_callback_uint32_ptr callback, void *ptr)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 就去查看你在什么地方调用这个函数 检查你的传入参数
    // 这里是检查是否有重复使用定时器
    // 比如初始化了 TIM1_PWM 然后又初始化成 TIM1_PIT 这种用法是不允许的
    zf_assert(timer_funciton_check((timer_index_enum)pit_n, TIMER_FUNCTION_TIMER));
    // 如果是这一行报错 那我就得问问你为什么周期写的是 0
    zf_assert(0 != period);
    zf_assert(0x7FF0 >= period);

    timer_clock_enable((timer_index_enum)pit_n);                                // 使能时钟

    pit_callback_ptr_list[pit_n]    = ptr;
    pit_callback_list[pit_n]        = (NULL == callback) ? (pit_callbakc_defalut) : (callback);

    GPTIMER_Regs *timer_obj;
    timer_obj = timer_reg[pit_n];

    timer_obj->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_UP | GPTIMER_CTRCTL_REPEAT_REPEAT_1;
    timer_obj->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
    if((TIM_G0 == pit_n) || (TIM_G8 == pit_n))
    {
        timer_obj->CLKDIV = 0;
        timer_obj->COMMONREGS.CPS = 0;
    }
    else
    {
        timer_obj->CLKDIV = 1;
        timer_obj->COMMONREGS.CPS = 0;
    }
    timer_obj->COUNTERREGS.LOAD = period;
    timer_obj->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_L_SET;

    interrupt_enable(irq_index[(pit_n)]);

    pit_enable(pit_n);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM PIT 中断初始化 us 周期
// 参数说明     pit_n               使用的 PIT 编号
// 参数说明     us                  PIT 周期 us 级别
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     pit_us_init(pit_n, period, callback, NULL);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pit_us_init(pit_index_enum pit_n, uint32 period, void_callback_uint32_ptr callback, void *ptr)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 就去查看你在什么地方调用这个函数 检查你的传入参数
    // 这里是检查是否有重复使用定时器
    // 比如初始化了 TIM1_PWM 然后又初始化成 TIM1_PIT 这种用法是不允许的
    zf_assert(timer_funciton_check((timer_index_enum)pit_n, TIMER_FUNCTION_TIMER));
    // 如果是这一行报错 那我就得问问你为什么周期写的是 0
    zf_assert(0 != period);
    zf_assert(0x7FFFFF >= period);

    timer_clock_enable((timer_index_enum)pit_n);                                // 使能时钟

    pit_callback_ptr_list[pit_n]    = ptr;
    pit_callback_list[pit_n]        = (NULL == callback) ? (pit_callbakc_defalut) : (callback);

    GPTIMER_Regs *timer_obj;
    timer_obj = timer_reg[pit_n];
    
    if(PIT_TIM_G12 == pit_n)
    {
        timer_obj->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
        timer_obj->CLKDIV = 7;
        timer_obj->COUNTERREGS.LOAD = period * 10;
    }
    else
    {
        if((PIT_TIM_G0 == pit_n) || (PIT_TIM_G8 == pit_n))
        {
            pit_set_div(pit_n, period, SYSTEM_CLOCK_80M / 2);
        }
        else
        {
            pit_set_div(pit_n, period, SYSTEM_CLOCK_80M);
        }
    }
    
    timer_obj->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_UP | GPTIMER_CTRCTL_REPEAT_REPEAT_1;;
    timer_obj->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_L_SET;

    interrupt_enable(irq_index[(pit_n)]);

    pit_enable(pit_n);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM PIT 中断初始化 ms 周期
// 参数说明     pit_n               使用的 PIT 编号
// 参数说明     ms                  PIT 周期 ms 级别
// 参数说明     callback            回调函数 为无返回值 uint32 加 void * 参数的函数
// 参数说明     *ptr                回调参数 用户自拟定的参数指针 不需要的话就传入 NULL
// 返回参数     void
// 使用示例     pit_ms_init(pit_n, period, callback, NULL);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pit_ms_init(pit_index_enum pit_n, uint32 period, void_callback_uint32_ptr callback, void *ptr)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 就去查看你在什么地方调用这个函数 检查你的传入参数
    // 这里是检查是否有重复使用定时器
    // 比如初始化了 TIM1_PWM 然后又初始化成 TIM1_PIT 这种用法是不允许的
    zf_assert(timer_funciton_check((timer_index_enum)pit_n, TIMER_FUNCTION_TIMER));
    // 如果是这一行报错 那我就得问问你为什么周期写的是 0
    zf_assert(0 != period);
    zf_assert(0x7FF0 >= period);

    pit_us_init(pit_n, period * 1000 , callback, ptr);
}
