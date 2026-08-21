/**
 * @file    user_headfile.h
 * @brief   用户头文件汇总 - 统一包含所有模块
 */

#ifndef _USER_HEADFILE_H_
#define _USER_HEADFILE_H_

/*==================== 配置层 ====================*/
#include "board_config.h"

/*==================== BSP 层 ====================*/
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "bsp_ads7830.h"
#include "bsp_track.h"

/*==================== APP 层 ====================*/
#include "app_control.h"
#include "app_task.h"
#include "app_ui.h"

#endif /* _USER_HEADFILE_H_ */
