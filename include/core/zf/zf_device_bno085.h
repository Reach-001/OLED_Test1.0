/*********************************************************************************************************************
* 文件名称          zf_device_bno085
* 适用平台          MSPM0G3507
* 备注              BNO085 软件 IIC 驱动，数据协议为 SHTP，不是普通寄存器读写。
********************************************************************************************************************/

#ifndef _zf_device_bno085_h_
#define _zf_device_bno085_h_

#include "zf_common_typedef.h"

#define BNO085_I2C_ADDR_DEFAULT     (0x4B)
#define BNO085_I2C_ADDR_ALT         (0x4A)

#define BNO085_REPORT_RATE_HZ       (50U)
#define BNO085_REPORT_INTERVAL_US   (1000000UL / BNO085_REPORT_RATE_HZ)

typedef struct
{
    int16 raw_x;
    int16 raw_y;
    int16 raw_z;
    int32 mdps_x;
    int32 mdps_y;
    int32 mdps_z;
    uint8 accuracy;
    uint8 sequence;
    uint32 timestamp_us;
} bno085_gyro_data_t;

typedef struct
{
    bno085_gyro_data_t gyro;

    int16 quat_i;
    int16 quat_j;
    int16 quat_k;
    int16 quat_real;
    int16 quat_radian_accuracy;

    int32 roll_md;
    int32 pitch_md;
    int32 yaw_md;
    uint8 quat_accuracy;
} bno085_imu_data_t;

uint8 bno085_init(void);
uint8 bno085_update(void);
uint8 bno085_is_ready(void);
uint8 bno085_get_i2c_addr(void);
uint8 bno085_get_last_error(void);
uint32 bno085_get_rx_count(void);
uint32 bno085_get_gyro_update_count(void);
uint32 bno085_get_quat_update_count(void);
const bno085_imu_data_t *bno085_get_imu_data(void);

#endif
