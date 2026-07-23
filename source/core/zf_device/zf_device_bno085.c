/*********************************************************************************************************************
* 文件名称          zf_device_bno085
* 适用平台          MSPM0G3507
* 备注              BNO085 软件 IIC 驱动，数据协议为 SHTP，不是普通寄存器读写。
********************************************************************************************************************/

#include "board_config.h"
#include "zf_common_debug.h"
#include "zf_driver_delay.h"
#include "zf_driver_soft_iic.h"
#include "zf_device_bno085.h"

#include <math.h>
#include <string.h>

#define BNO085_SHTP_HEADER_SIZE        (4U)
#define BNO085_MAX_PACKET_SIZE         (384U)
#define BNO085_I2C_BUFFER_LENGTH       (32U)
#define BNO085_STARTUP_DELAY_MS        (300U)
#define BNO085_STARTUP_READY_TIMEOUT   (100U)
#define BNO085_STARTUP_FLUSH_MS        (80U)

#define BNO085_CHANNEL_CONTROL         (2U)
#define BNO085_CHANNEL_REPORTS         (3U)
#define BNO085_CHANNEL_WAKE_REPORTS    (4U)

#define BNO085_REPORT_BASE_TIMESTAMP   (0xFBU)
#define BNO085_REPORT_SET_FEATURE      (0xFDU)

#define BNO085_SENSOR_GYROSCOPE        (0x02U)
#define BNO085_SENSOR_ROTATION_VECTOR  (0x05U)

#define BNO085_ROTATION_VECTOR_Q1      (14U)
#define BNO085_GYRO_Q1                 (9U)
#define BNO085_PI_F                    (3.14159265358979323846f)

enum
{
    BNO085_ERROR_NONE = 0,
    BNO085_ERROR_NO_ACK,
    BNO085_ERROR_READ,
    BNO085_ERROR_PACKET,
    BNO085_ERROR_ENABLE,
};

static soft_iic_info_struct s_bno085_iic;
static uint8 s_bno085_addr = BNO085_I2C_ADDR_DEFAULT;
static uint8 s_bno085_ready = 0;
static uint8 s_last_error = BNO085_ERROR_NONE;

static uint8 s_shtp_header[BNO085_SHTP_HEADER_SIZE];
static uint8 s_shtp_data[BNO085_MAX_PACKET_SIZE];
static uint8 s_tx_packet[BNO085_SHTP_HEADER_SIZE + BNO085_MAX_PACKET_SIZE];
static uint8 s_sequence_number[6];
static uint16 s_packet_length;
static uint16 s_data_spot;
static uint8 s_last_channel;

static int16 s_raw_quat_i;
static int16 s_raw_quat_j;
static int16 s_raw_quat_k;
static int16 s_raw_quat_real;
static int16 s_raw_quat_radian_accuracy;
static uint8 s_quat_accuracy;
static uint32 s_quat_update_count;

static int16 s_raw_gyro_x;
static int16 s_raw_gyro_y;
static int16 s_raw_gyro_z;
static uint8 s_gyro_accuracy;
static uint8 s_gyro_sequence;
static uint32 s_gyro_timestamp_us;
static uint32 s_gyro_update_count;

static uint32 s_rx_count;
static bno085_imu_data_t s_imu_data;

static uint8 bno085_probe(uint8 addr);
static uint8 bno085_receive_packet(void);
static uint8 bno085_get_data(uint16 chars_remaining);
static uint8 bno085_send_packet(uint8 channel, uint8 data_length);
static uint8 bno085_enable_report(uint8 report_id, uint32 interval_us);
static void bno085_parse_input_report(void);
static uint16 bno085_parse_one_report(uint16 offset, uint32 timestamp_us);
static void bno085_fill_imu_data(void);
static int16 bno085_read_i16_le(const uint8 *data);
static uint32 bno085_read_u32_le(const uint8 *data);
static void bno085_write_u32_le(uint8 *data, uint32 value);
static float bno085_q_to_float(int16 fixed_point, uint8 q_point);
static int32 bno085_gyro_raw_to_mdps(int16 raw);
static int32 bno085_radians_to_mdeg(float radians);

uint8 bno085_init(void)
{
    memset(s_sequence_number, 0, sizeof(s_sequence_number));
    memset(&s_imu_data, 0, sizeof(s_imu_data));
    s_bno085_ready = 0;
    s_last_error = BNO085_ERROR_NONE;
    s_rx_count = 0;
    s_gyro_update_count = 0;
    s_quat_update_count = 0;

    soft_iic_init(&s_bno085_iic,
                  BNO085_I2C_ADDR_DEFAULT,
                  BNO085_SOFT_IIC_DELAY,
                  BNO085_SCL_PIN,
                  BNO085_SDA_PIN);

    system_delay_ms(BNO085_STARTUP_DELAY_MS);

    if (bno085_probe(BNO085_I2C_ADDR_DEFAULT))
    {
        s_bno085_addr = BNO085_I2C_ADDR_DEFAULT;
    }
    else if (bno085_probe(BNO085_I2C_ADDR_ALT))
    {
        s_bno085_addr = BNO085_I2C_ADDR_ALT;
    }
    else
    {
        s_last_error = BNO085_ERROR_NO_ACK;
        return 1;
    }

    s_bno085_iic.addr = s_bno085_addr;

    // BNO085 上电后会先吐启动包；这里清掉旧包，不主动 soft reset。
    for (uint16 i = 0; i < BNO085_STARTUP_FLUSH_MS; i += 2U)
    {
        (void)bno085_receive_packet();
        system_delay_ms(2);
    }

    if (!bno085_enable_report(BNO085_SENSOR_GYROSCOPE, BNO085_REPORT_INTERVAL_US) ||
        !bno085_enable_report(BNO085_SENSOR_ROTATION_VECTOR, BNO085_REPORT_INTERVAL_US))
    {
        s_last_error = BNO085_ERROR_ENABLE;
        return 1;
    }

    /*
     * 使能命令发出后，BNO085 会先回复 GetFeature Response（控制通道包），
     * 再开始输出传感器数据包。这里等待约 200ms 并冲洗掉控制通道包，
     * 确保 bno085_update() 第一次调用就能取到有效数据。
     */
    for (uint16 i = 0; i < 200U; i += 5U)
    {
        (void)bno085_receive_packet();
        system_delay_ms(5);
    }

    s_bno085_ready = 1;
    return 0;
}

uint8 bno085_update(void)
{
    if (!s_bno085_ready)
    {
        return 0;
    }

    if (!bno085_receive_packet())
    {
        return 0;
    }

    if ((s_last_channel == BNO085_CHANNEL_REPORTS) ||
        (s_last_channel == BNO085_CHANNEL_WAKE_REPORTS))
    {
        bno085_parse_input_report();
        bno085_fill_imu_data();
        return 1;
    }

    return 0;
}

uint8 bno085_is_ready(void)
{
    return s_bno085_ready;
}

uint8 bno085_get_i2c_addr(void)
{
    return s_bno085_addr;
}

uint8 bno085_get_last_error(void)
{
    return s_last_error;
}

uint32 bno085_get_rx_count(void)
{
    return s_rx_count;
}

uint32 bno085_get_gyro_update_count(void)
{
    return s_gyro_update_count;
}

uint32 bno085_get_quat_update_count(void)
{
    return s_quat_update_count;
}

const bno085_imu_data_t *bno085_get_imu_data(void)
{
    return &s_imu_data;
}

static uint8 bno085_probe(uint8 addr)
{
    uint8 ack = 0;

    s_bno085_iic.addr = addr;
    for (uint16 i = 0; i < BNO085_STARTUP_READY_TIMEOUT; i += 10U)
    {
        soft_iic_start(&s_bno085_iic);
        ack = soft_iic_send_data(&s_bno085_iic, (uint8)(addr << 1));
        soft_iic_stop(&s_bno085_iic);

        if (ack)
        {
            return 1;
        }
        system_delay_ms(10);
    }

    return 0;
}

static uint8 bno085_receive_packet(void)
{
    soft_iic_read_8bit_array(&s_bno085_iic, s_shtp_header, BNO085_SHTP_HEADER_SIZE);

    /* 0xFF 0xFF 0xFF 0xFF：设备无响应或总线浮高 */
    if ((s_shtp_header[0] == 0xFFU) && (s_shtp_header[1] == 0xFFU) &&
        (s_shtp_header[2] == 0xFFU) && (s_shtp_header[3] == 0xFFU))
    {
        return 0;
    }

    /* 0x00 0x00 0x00 0x00：BNO085 当前无新数据，不是错误，直接返回 0 */
    if ((s_shtp_header[0] == 0x00U) && (s_shtp_header[1] == 0x00U) &&
        (s_shtp_header[2] == 0x00U) && (s_shtp_header[3] == 0x00U))
    {
        return 0;
    }

    s_packet_length = (uint16)((((uint16)s_shtp_header[1] << 8) | s_shtp_header[0]) & 0x7FFFU);
    if (s_packet_length < BNO085_SHTP_HEADER_SIZE)
    {
        /* 包长度非法才记录错误 */
        s_last_error = BNO085_ERROR_PACKET;
        return 0;
    }

    s_packet_length -= BNO085_SHTP_HEADER_SIZE;
    if (s_packet_length > BNO085_MAX_PACKET_SIZE)
    {
        s_last_error = BNO085_ERROR_PACKET;
        return 0;
    }

    s_data_spot = 0;
    if (s_packet_length != 0U)
    {
        if (!bno085_get_data(s_packet_length))
        {
            return 0;
        }
    }

    s_rx_count++;
    s_last_channel = s_shtp_header[2];
    s_last_error = BNO085_ERROR_NONE;
    return 1;
}

static uint8 bno085_get_data(uint16 chars_remaining)
{
    uint8 rx[BNO085_I2C_BUFFER_LENGTH];

    while (chars_remaining > 0U)
    {
        uint16 bytes_to_read = chars_remaining;
        if (bytes_to_read > (BNO085_I2C_BUFFER_LENGTH - BNO085_SHTP_HEADER_SIZE))
        {
            bytes_to_read = BNO085_I2C_BUFFER_LENGTH - BNO085_SHTP_HEADER_SIZE;
        }

        if ((s_data_spot + bytes_to_read) > BNO085_MAX_PACKET_SIZE)
        {
            s_last_error = BNO085_ERROR_PACKET;
            return 0;
        }

        // BNO08x 长包分段读取时，每个 chunk 前 4 字节都会重复 SHTP header。
        soft_iic_read_8bit_array(&s_bno085_iic,
                                 rx,
                                 (uint32)(bytes_to_read + BNO085_SHTP_HEADER_SIZE));
        memcpy(&s_shtp_data[s_data_spot], &rx[BNO085_SHTP_HEADER_SIZE], bytes_to_read);

        s_data_spot += bytes_to_read;
        chars_remaining -= bytes_to_read;
    }

    return 1;
}

static uint8 bno085_send_packet(uint8 channel, uint8 data_length)
{
    uint16 total_length;

    if ((channel >= sizeof(s_sequence_number)) || (data_length > BNO085_MAX_PACKET_SIZE))
    {
        s_last_error = BNO085_ERROR_PACKET;
        return 0;
    }

    total_length = (uint16)data_length + BNO085_SHTP_HEADER_SIZE;
    s_tx_packet[0] = (uint8)(total_length & 0xFFU);
    s_tx_packet[1] = (uint8)((total_length >> 8) & 0xFFU);
    s_tx_packet[2] = channel;
    s_tx_packet[3] = s_sequence_number[channel]++;
    memcpy(&s_tx_packet[BNO085_SHTP_HEADER_SIZE], s_shtp_data, data_length);

    soft_iic_write_8bit_array(&s_bno085_iic, s_tx_packet, total_length);
    return 1;
}

static uint8 bno085_enable_report(uint8 report_id, uint32 interval_us)
{
    memset(s_shtp_data, 0, 17U);
    s_shtp_data[0] = BNO085_REPORT_SET_FEATURE;
    s_shtp_data[1] = report_id;
    bno085_write_u32_le(&s_shtp_data[5], interval_us);
    bno085_write_u32_le(&s_shtp_data[9], 0UL);
    bno085_write_u32_le(&s_shtp_data[13], 0UL);
    return bno085_send_packet(BNO085_CHANNEL_CONTROL, 17U);
}

static void bno085_parse_input_report(void)
{
    uint16 offset = 0U;
    uint32 timestamp_us = 0U;

    if ((s_packet_length >= 5U) && (s_shtp_data[0] == BNO085_REPORT_BASE_TIMESTAMP))
    {
        timestamp_us = bno085_read_u32_le(&s_shtp_data[1]);
        offset = 5U;
    }

    while (offset < s_packet_length)
    {
        uint16 consumed = bno085_parse_one_report(offset, timestamp_us);
        if (consumed == 0U)
        {
            break;
        }
        offset += consumed;
    }
}

static uint16 bno085_parse_one_report(uint16 offset, uint32 timestamp_us)
{
    uint8 report_id;
    uint8 status;

    if ((s_packet_length - offset) < 4U)
    {
        return 0U;
    }

    report_id = s_shtp_data[offset];
    status = s_shtp_data[offset + 2U] & 0x03U;

    switch (report_id)
    {
        case BNO085_SENSOR_GYROSCOPE:
            if ((s_packet_length - offset) < 10U)
            {
                return 0U;
            }
            s_gyro_accuracy = status;
            s_gyro_sequence = s_shtp_data[offset + 1U];
            s_gyro_timestamp_us = timestamp_us;
            s_raw_gyro_x = bno085_read_i16_le(&s_shtp_data[offset + 4U]);
            s_raw_gyro_y = bno085_read_i16_le(&s_shtp_data[offset + 6U]);
            s_raw_gyro_z = bno085_read_i16_le(&s_shtp_data[offset + 8U]);
            s_gyro_update_count++;
            return 10U;

        case BNO085_SENSOR_ROTATION_VECTOR:
            if ((s_packet_length - offset) < 14U)
            {
                return 0U;
            }
            s_quat_accuracy = status;
            s_raw_quat_i = bno085_read_i16_le(&s_shtp_data[offset + 4U]);
            s_raw_quat_j = bno085_read_i16_le(&s_shtp_data[offset + 6U]);
            s_raw_quat_k = bno085_read_i16_le(&s_shtp_data[offset + 8U]);
            s_raw_quat_real = bno085_read_i16_le(&s_shtp_data[offset + 10U]);
            s_raw_quat_radian_accuracy = bno085_read_i16_le(&s_shtp_data[offset + 12U]);
            s_quat_update_count++;
            return 14U;

        default:
            return 0U;
    }
}

static void bno085_fill_imu_data(void)
{
    float qi;
    float qj;
    float qk;
    float qr;
    float sinr_cosp;
    float cosr_cosp;
    float sinp;
    float pitch;
    float siny_cosp;
    float cosy_cosp;

    s_imu_data.gyro.raw_x = s_raw_gyro_x;
    s_imu_data.gyro.raw_y = s_raw_gyro_y;
    s_imu_data.gyro.raw_z = s_raw_gyro_z;
    s_imu_data.gyro.mdps_x = bno085_gyro_raw_to_mdps(s_raw_gyro_x);
    s_imu_data.gyro.mdps_y = bno085_gyro_raw_to_mdps(s_raw_gyro_y);
    s_imu_data.gyro.mdps_z = bno085_gyro_raw_to_mdps(s_raw_gyro_z);
    s_imu_data.gyro.accuracy = s_gyro_accuracy;
    s_imu_data.gyro.sequence = s_gyro_sequence;
    s_imu_data.gyro.timestamp_us = s_gyro_timestamp_us;

    s_imu_data.quat_i = s_raw_quat_i;
    s_imu_data.quat_j = s_raw_quat_j;
    s_imu_data.quat_k = s_raw_quat_k;
    s_imu_data.quat_real = s_raw_quat_real;
    s_imu_data.quat_radian_accuracy = s_raw_quat_radian_accuracy;
    s_imu_data.quat_accuracy = s_quat_accuracy;

    qi = bno085_q_to_float(s_raw_quat_i, BNO085_ROTATION_VECTOR_Q1);
    qj = bno085_q_to_float(s_raw_quat_j, BNO085_ROTATION_VECTOR_Q1);
    qk = bno085_q_to_float(s_raw_quat_k, BNO085_ROTATION_VECTOR_Q1);
    qr = bno085_q_to_float(s_raw_quat_real, BNO085_ROTATION_VECTOR_Q1);

    sinr_cosp = 2.0f * ((qr * qi) + (qj * qk));
    cosr_cosp = 1.0f - (2.0f * ((qi * qi) + (qj * qj)));
    s_imu_data.roll_md = bno085_radians_to_mdeg(atan2f(sinr_cosp, cosr_cosp));

    sinp = 2.0f * ((qr * qj) - (qk * qi));
    if (sinp >= 1.0f)
    {
        pitch = BNO085_PI_F / 2.0f;
    }
    else if (sinp <= -1.0f)
    {
        pitch = -BNO085_PI_F / 2.0f;
    }
    else
    {
        pitch = asinf(sinp);
    }
    s_imu_data.pitch_md = bno085_radians_to_mdeg(pitch);

    siny_cosp = 2.0f * ((qr * qk) + (qi * qj));
    cosy_cosp = 1.0f - (2.0f * ((qj * qj) + (qk * qk)));
    s_imu_data.yaw_md = bno085_radians_to_mdeg(atan2f(siny_cosp, cosy_cosp));
}

static int16 bno085_read_i16_le(const uint8 *data)
{
    return (int16)((uint16)data[0] | ((uint16)data[1] << 8));
}

static uint32 bno085_read_u32_le(const uint8 *data)
{
    return ((uint32)data[0]) |
           ((uint32)data[1] << 8) |
           ((uint32)data[2] << 16) |
           ((uint32)data[3] << 24);
}

static void bno085_write_u32_le(uint8 *data, uint32 value)
{
    data[0] = (uint8)(value & 0xFFU);
    data[1] = (uint8)((value >> 8) & 0xFFU);
    data[2] = (uint8)((value >> 16) & 0xFFU);
    data[3] = (uint8)((value >> 24) & 0xFFU);
}

static float bno085_q_to_float(int16 fixed_point, uint8 q_point)
{
    return (float)fixed_point / (float)(1UL << q_point);
}

static int32 bno085_gyro_raw_to_mdps(int16 raw)
{
    float dps = bno085_q_to_float(raw, BNO085_GYRO_Q1) * (180.0f / BNO085_PI_F);
    return (int32)(dps * 1000.0f);
}

static int32 bno085_radians_to_mdeg(float radians)
{
    return (int32)(radians * (180000.0f / BNO085_PI_F));
}
