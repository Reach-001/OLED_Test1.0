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
* 文件名称          zf_driver_timer
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
#include "zf_driver_gpio.h"

#include "zf_driver_timer.h"

timer_function_enum timer_function_state[TIM_NUM] = 
{
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT
};

static timer_mode_enum timer_mode_state[TIM_NUM] = 
{
    TIMER_SYSTEM_CLOCK, TIMER_SYSTEM_CLOCK,
    TIMER_SYSTEM_CLOCK, TIMER_SYSTEM_CLOCK,
    TIMER_SYSTEM_CLOCK, TIMER_SYSTEM_CLOCK,
    TIMER_SYSTEM_CLOCK
};

GPTIMER_Regs * const timer_reg[TIM_MAX] = {TIMA0, TIMA1, TIMG0, TIMG6, TIMG7, TIMG8, TIMG12};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 外设确认功能状态 库内部调用
// 参数说明     index           TIMER 外设模块号
// 参数说明     mode            需要确的功能模块
// 返回参数     uint8           1-可以使用 0-不可以使用
// 使用示例     zf_assert(timer_funciton_check(index, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 timer_funciton_check (timer_index_enum index,timer_function_enum mode)
{
    uint8 return_state = 1;
    if(TIMER_FUNCTION_INIT == timer_function_state[index])
    {
        timer_function_state[index] = mode;
    }
    else if(timer_function_state[index] == mode)
    {
        return_state = 1;
    }
    else
    {
        return_state = 0;
    }
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 时钟使能 库内部调用
// 参数说明     index           TIMER 外设模块号
// 返回参数     void
// 使用示例     timer_clock_enable(index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timer_clock_enable (timer_index_enum index)
{
    timer_reg[index]->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 开始计时
// 参数说明     index           TIMER 外设模块号
// 返回参数     void
// 使用示例     timer_start(index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timer_start (timer_index_enum index)
{
    timer_reg[index]->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 停止计时
// 参数说明     index           TIMER 外设模块号
// 返回参数     void
// 使用示例     timer_stop(index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timer_stop (timer_index_enum index)
{
    timer_reg[index]->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 获取计时器计数
// 参数说明     index           TIMER 外设模块号
// 返回参数     uint16          计数值
// 使用示例     timer_get(index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint16 timer_get (timer_index_enum index)
{
    uint32 return_value = 0;
    return_value = timer_reg[index]->COUNTERREGS.CTR;

    if(TIMER_US == timer_mode_state[index])
    {
        if(TIM_G12 == index)
        {
            return_value = return_value / 80;
        }
    }
    else if(TIMER_MS == timer_mode_state[index])
    {
        return_value = return_value / 50;
        if(TIM_G12 == index)
        {
            return_value = return_value / (50 * 200);
        }
    }
    return return_value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 清除计时器计数
// 参数说明     index           TIMER 外设模块号
// 返回参数     void
// 使用示例     timer_clear(index);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timer_clear (timer_index_enum index)
{
    timer_reg[index]->COUNTERREGS.CTR = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIMER 计时器初始化
// 参数说明     index           TIMER 外设模块号
// 参数说明     mode            计时方式
// 返回参数     void
// 使用示例     timer_init(index, mode);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timer_init (timer_index_enum index, timer_mode_enum mode)
{
    // 如果程序在输出了断言信息 并且提示出错位置在这里
    // 就去查看你在什么地方调用这个函数 检查你的传入参数
    // 这里是检查是否有重复使用定时器
    // 比如初始化了 TIM1_PWM 然后又初始化成 TIM_1 这种用法是不允许的
    zf_assert(timer_funciton_check(index, TIMER_FUNCTION_TIMER));

    GPTIMER_Regs *timer_obj;
    timer_obj = timer_reg[index];
    
    timer_clock_enable(index);
    
    timer_obj->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_UP;
    timer_obj->COUNTERREGS.LOAD = 0xFFFF;
    
    uint32 clock_select = SYSTEM_CLOCK_80M;
    switch(mode)
    {
        case TIMER_US:
        {
            if((TIM_G0 == index) || (TIM_G8 == index))
            {
                clock_select /= 2;
            }
            
            timer_obj->CLKDIV = 0;
            timer_obj->COMMONREGS.CPS = clock_select / 1000000 - 1;
            timer_mode_state[index] = TIMER_US;
        }break;
        case TIMER_MS:
        {
            if(TIM_G0 == index || TIM_G8 == index)
            {
                clock_select /= 2;
            }

            timer_obj->CLKDIV = 7;
            timer_obj->COMMONREGS.CPS = clock_select / 8 / 50000 - 1;
            // 定时器设置为50KHz G12为10MHz
            
            timer_mode_state[index] = TIMER_MS;
        }break;
        default:
        {
            if((TIM_G0 == index) || (TIM_G8 == index))
            {
                clock_select /= 2;
            }

            timer_obj->CLKDIV = clock_select / 40000000 - 1;
            timer_obj->COMMONREGS.CPS = 0;
            // 40 * 1000 * 1000 == 40 MHz
            timer_mode_state[index] = TIMER_SYSTEM_CLOCK;
        }break;
    }
    timer_obj->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

    timer_stop(index);
    timer_clear(index);
}
