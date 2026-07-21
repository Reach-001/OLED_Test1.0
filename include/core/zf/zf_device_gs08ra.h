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
* 文件名称          zf_device_gs08ra
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2025-06-1        SeekFree            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* 接线定义：
*                   ------------------------------------
*                   模块管脚        单片机管脚
*                   S0              查看 zf_device_gs08ra.h 中 GS08RA_S0_PIN
*                   S1              查看 zf_device_gs08ra.h 中 GS08RA_S1_PIN
*                   S2              查看 zf_device_gs08ra.h 中 GS08RA_S2_PIN
*                   S3              不需要连接，悬空即可
*                   OUT             查看 zf_device_gs08ra.h 中 GS08RA_OUT_PIN
*                   3V3             3.3V电源
*                   GND             电源地
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _zf_device_gs08ra_h_
#define _zf_device_gs08ra_h_

#include "zf_common_debug.h"

#include "zf_driver_gpio.h"

//=================================================定义 GS08RA 基本配置================================================
#define GS08RA_S0_PIN           ( A16           )	    // 选择器引脚A
#define GS08RA_S1_PIN           ( A17           )	    // 选择器引脚B
#define GS08RA_S2_PIN           ( B17           )	    // 选择器引脚C

#define GS08RA_OUT_PIN          ( ADC0_CH4_B25  )       // 灰度传感器数据采集引脚
#define GS08RA_ADC_RESLUTION    ( ADC_8BIT      )       // 灰度传感器ADC精度

#define GS08A_CHANNEL_NUM       ( 8 )                   // 模块通道数量
//=================================================定义 GS08RA 基本配置================================================


//=================================================声明 GS08RA 全局变量=================================================
extern uint8 gs08ra_threshold  ;                    // 用于二值化的阈值
extern uint8 gs08ra_max_val [GS08A_CHANNEL_NUM] ;   // 最大值	
extern uint8 gs08ra_min_val [GS08A_CHANNEL_NUM] ;   // 最小值
extern uint8 gs08ra_raw_val [GS08A_CHANNEL_NUM] ;   // 原始灰度数据
extern uint8 gs08ra_deal_val[GS08A_CHANNEL_NUM];    // 归一化处理后的数据
extern uint8 gs08ra_bin_val [GS08A_CHANNEL_NUM] ;   // 使用归一化之后的数据进行二值化
//=================================================声明 GS08RA 全局变量=================================================


//=================================================声明 GS08RA 基础函数=================================================
void gs08ra_set_max(void);
void gs08ra_set_min(void);
void gs08ra_set_threshold(uint8 threshold);
void gs08ra_scan_read(void);
void gs08ra_init(void);
//=================================================声明 GS08RA 基础函数=================================================


#endif