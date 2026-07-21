/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       HwiP.h
 *
 *  @brief      Hardware Interrupt module for the RTOS Porting Interface
 *              RTOS移植接口的硬件中断模块
 *
 *  The ::HwiP_disable/::HwiP_restore APIs can be called recursively. The order
 *  of the HwiP_restore calls, must be in reversed order. For example:
 *  @code
 *  uintptr_t key1, key2;
 *  key1 = HwiP_disable();
 *  key2 = HwiP_disable();
 *  HwiP_restore(key2);
 *  HwiP_restore(key1);
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef ti_dpl_HwiP__include
#define ti_dpl_HwiP__include

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief    Number of bytes greater than or equal to the size of any RTOS
 *            HwiP object. / HwiP对象的最大字节数
 *
 *  NoRTOS:   12
 *  FreeRTOS: 12
 *  BIOS 6.x: 28
 *  BIOS 7.x: 20
 */
#define HwiP_STRUCT_SIZE   (28)

/*!
 *  @brief    HwiP structure. / HwiP结构体
 *
 *  Opaque structure that should be large enough to hold any of the RTOS
 *  specific HwiP objects. / 不透明结构体，需足以容纳任意RTOS的HwiP对象
 */
typedef union HwiP_Struct {
    uint32_t dummy;  /*!< Align object */
    uint8_t data[HwiP_STRUCT_SIZE];
} HwiP_Struct;

/*!
 *  @brief    Opaque client reference to an instance of a HwiP
 *            HwiP实例的不透明句柄
 *
 *  A HwiP_Handle returned from the ::HwiP_create represents that instance.
 */
typedef  void *HwiP_Handle;

/*!
 *  @brief    Status codes for HwiP APIs / HwiP API返回状态码
 */
typedef enum {
    HwiP_OK = 0,
    HwiP_FAILURE = -1
} HwiP_Status;

/*!
 *  @brief  Prototype for the entry function for a hardware interrupt
 *          / 硬件中断入口函数原型
 */
typedef void (*HwiP_Fxn)(uintptr_t arg);

/*!
 *  @brief    Basic HwiP Parameters / HwiP基本参数
 *
 *  Structure that contains the parameters passed into ::HwiP_create
 *  when creating a HwiP instance. The ::HwiP_Params_init function should
 *  be used to initialize the fields to default values before the application sets
 *  the fields manually. The HwiP default parameters are noted in
 *  HwiP_Params_init.
 *
 *  Parameter enableInt specifies if the interrupt should be enabled
 *  upon creation of the HwiP object.  The default is true.
 */
typedef struct {
    uintptr_t  arg;       /*!< Argument passed into the Hwi function. / 传入中断函数的参数 */
    uint32_t   priority;  /*!< Device specific priority. / 设备特定优先级 */
    bool       enableInt; /*!< Enable interrupt on creation. / 创建时是否使能中断 */
} HwiP_Params;

/*!
 *  @brief    Interrupt number posted by SwiP / SwiP使用的中断号
 *
 *  The SwiP module needs its scheduler to run at key points in SwiP
 *  processing.  This is accomplished via an interrupt that is configured
 *  at the lowest possible interrupt priority level and is plugged with
 *  the SwiP scheduler.  This interrupt must be the *only* interrupt at
 *  that lowest priority.  SwiP will post this interrupt whenever its
 *  scheduler needs to run.
 *
 *  The default value for your device should suffice, but if a different
 *  interrupt is needed to be used for SwiP scheduling then HwiP_swiPIntNum
 *  can be assigned with this interrupt (early on, before HwiPs are created
 *  and before any SwiP gets posted).
 */
extern int HwiP_swiPIntNum;

/*!
 *  @brief  Function to construct a hardware interrupt object. / 构造硬件中断对象（静态分配）
 *
 *  @param  hwiP   Pointer to HwiP_Struct object. / 指向HwiP_Struct对象的指针
 *  @param  interruptNum Interrupt Vector Id / 中断向量号
 *  @param  hwiFxn entry function of the hardware interrupt / 硬件中断入口函数
 *
 *  @param  params    Pointer to the instance configuration parameters. NULL
 *                    denotes to use the default parameters. The HwiP default
 *                    parameters are noted in ::HwiP_Params_init. / 实例配置参数指针，NULL表示使用默认参数
 *
 *  @return A HwiP_Handle on success or a NULL on an error / 成功返回HwiP_Handle句柄，失败返回NULL
 */
extern HwiP_Handle HwiP_construct(HwiP_Struct *hwiP, int interruptNum,
                                  HwiP_Fxn hwiFxn, HwiP_Params *params);

/*!
 *  @brief  Function to destruct a hardware interrupt object / 析构硬件中断对象（静态分配）
 *
 *  @param  hwiP  Pointer to a HwiP_Struct object that was passed to
 *                HwiP_construct(). / 传入HwiP_construct()的HwiP_Struct指针
 *
 *  @return
 */
extern void HwiP_destruct(HwiP_Struct *hwiP);

/*!
 *  @brief  Function to clear a single interrupt / 清除单个中断
 *
 *  @param  interruptNum interrupt number to clear / 要清除的中断号
 */
extern void HwiP_clearInterrupt(int interruptNum);

/*!
 *  @brief  Function to create an interrupt on CortexM devices
 *          / 在CortexM设备上创建中断（动态分配）
 *
 *  @param  interruptNum Interrupt Vector Id / 中断向量号
 *
 *  @param  hwiFxn entry function of the hardware interrupt / 硬件中断入口函数
 *
 *  @param  params    Pointer to the instance configuration parameters. NULL
 *                    denotes to use the default parameters. The HwiP default
 *                    parameters are noted in ::HwiP_Params_init. / 实例配置参数指针，NULL表示使用默认参数
 *
 *  @return A HwiP_Handle on success or a NULL on an error / 成功返回HwiP_Handle句柄，失败返回NULL
 */
extern HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn,
                               HwiP_Params *params);

/*!
 *  @brief  Function to delete an interrupt on CortexM devices
 *          / 在CortexM设备上删除中断（动态分配）
 *
 *  @param  handle returned from the HwiP_create call / HwiP_create返回的句柄
 *
 *  @return
 */
extern void HwiP_delete(HwiP_Handle handle);

/*!
 *  @brief  Function to disable interrupts to enter a critical region
 *          / 禁用中断以进入临界区
 *
 *  This function can be called multiple times, but must unwound in the reverse
 *  order. For example
 *  @code
 *  uintptr_t key1, key2;
 *  key1 = HwiP_disable();
 *  key2 = HwiP_disable();
 *  HwiP_restore(key2);
 *  HwiP_restore(key1);
 *  @endcode
 *
 *  @return A key that must be passed to HwiP_restore to re-enable interrupts.
 *          / 返回的key必须传给HwiP_restore以重新使能中断
 */
extern uintptr_t HwiP_disable(void);

/*!
 *  @brief  Function to enable interrupts / 使能中断
 */
extern void HwiP_enable(void);

/*!
 *  @brief  Function to disable a single interrupt / 禁用单个中断
 *
 *  @param  interruptNum interrupt number to disable / 要禁用的中断号
 */
extern void HwiP_disableInterrupt(int interruptNum);

/*!
 *  @brief  Function to enable a single interrupt / 使能单个中断
 *
 *  @param  interruptNum interrupt number to enable / 要使能的中断号
 */
extern void HwiP_enableInterrupt(int interruptNum);

/*!
 *  @brief  Function  to return a status based on whether it is in an interrupt
 *      context. / 判断当前是否处于中断上下文
 *
 *  @return A status: indicating whether the function was called in an
 *      ISR (true) or at thread level (false). / 在ISR中调用返回true，线程级返回false
 */
extern bool HwiP_inISR(void);

/*!
 *  @brief  Function to determine whether interrupts are currently enabled.
 *          / 判断中断是否已使能
 *
 *  @return Current state of interrupts. / 当前中断状态
 *    - true  Interrupts are currently enabled. / 中断已使能
 *    - false Interrupts are currently disabled. / 中断已禁用
 */
extern bool HwiP_interruptsEnabled(void);

/*!
 *  @brief  Initialize params structure to default values. / 将参数结构体初始化为默认值
 *
 *  The default parameters are:
 *   - arg: 0
 *   - priority: ~0
 *   - enableInt: true
 *
 *  @param params  Pointer to the instance configuration parameters. / 指向参数结构体的指针
 */
extern void HwiP_Params_init(HwiP_Params *params);

/*!
 *  @brief  Function to plug an interrupt vector / 注册中断向量
 *
 *  @param  interruptNum ID of interrupt to plug / 要注册的中断号
 *  @param  fxn ISR that services plugged interrupt / 处理该中断的ISR函数
 */
extern void HwiP_plug(int interruptNum, void *fxn);

/*!
 *  @brief  Function to generate an interrupt / 软件触发中断
 *
 *  @param  interruptNum ID of interrupt to generate / 要触发的中断号
 */
extern void HwiP_post(int interruptNum);

/*!
 *  @brief  Function to restore interrupts to exit a critical region
 *          / 恢复中断以退出临界区
 *
 *  @param  key return from HwiP_disable / HwiP_disable返回的key
 */
extern void HwiP_restore(uintptr_t key);

/*!
 *  @brief  Function to overwrite HwiP function and arg / 覆盖HwiP的中断函数和参数
 *
 *  @param  hwiP handle returned from the HwiP_create or construct call / HwiP_create或construct返回的句柄
 *  @param  fxn  pointer to ISR function / ISR函数指针
 *  @param  arg  argument to ISR function / ISR函数参数
 */
extern void HwiP_setFunc(HwiP_Handle hwiP, HwiP_Fxn fxn, uintptr_t arg);

/*!
 *  @brief  Function to set the priority of a hardware interrupt / 设置硬件中断优先级
 *
 *  @param  interruptNum id of the interrupt to change / 要修改的中断号
 *  @param  priority new priority / 新优先级
 */
extern void HwiP_setPriority(int interruptNum, uint32_t priority);

/*!
 *  @brief  Function to call the HW ISR function registered by ::HwiP_construct
 *          / 调用由HwiP_construct注册的硬件ISR函数
 *
 *  @note   This function may not be available on all implementations / 并非所有实现都支持此函数
 *
 *  @param  interruptNum Interrupt Vector Id / 中断向量号
 */
void HwiP_dispatchInterrupt(int interruptNum);

#ifdef __cplusplus
}
#endif

#endif /* ti_dpl_HwiP__include */
