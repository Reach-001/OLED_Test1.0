/*********************************************************************************************************************
* 文件名称          zf_device_bno085
* 适用平台          MSPM0G3507
* 备注              BNO085 IIC 驱动，数据协议为 SHTP，不是普通寄存器读写。
********************************************************************************************************************/

#include "board_config.h"
#include "zf_common_debug.h"
#include "zf_driver_delay.h"
#include "zf_driver_gpio.h"
#include "zf_driver_soft_iic.h"
#include "zf_device_bno085.h"

#if !BNO085_USE_SOFT_IIC
#include <ti/driverlib/dl_i2c.h>
#endif
#include <math.h>
#include <string.h>

#define BNO085_SHTP_HEADER_SIZE        (4U)
#define BNO085_MAX_PACKET_SIZE         (384U)
#define BNO085_I2C_BUFFER_LENGTH       (32U)
#define BNO085_I2C_TIMEOUT_COUNT       (20000UL)
#define BNO085_I2C_CLOCK_HZ            (4UL * 1000UL * 1000UL)
#define BNO085_STARTUP_READY_TIMEOUT   (300U)
#define BNO085_WRITE_RETRY_COUNT       (8U)
#define BNO085_WRITE_RETRY_DELAY_MS    (10U)
#define BNO085_CHANNEL_CONTROL         (2U)
#define BNO085_CHANNEL_REPORTS         (3U)
#define BNO085_CHANNEL_WAKE_REPORTS    (4U)

#define BNO085_REPORT_BASE_TIMESTAMP   (0xFBU)
#define BNO085_REPORT_SET_FEATURE      (0xFDU)

#define BNO085_SENSOR_ACCELEROMETER                  (0x01U)
#define BNO085_SENSOR_GYROSCOPE                      (0x02U)
#define BNO085_SENSOR_MAGNETIC_FIELD                 (0x03U)
#define BNO085_SENSOR_LINEAR_ACCELERATION            (0x04U)
#define BNO085_SENSOR_ROTATION_VECTOR                (0x05U)
#define BNO085_SENSOR_STEP_COUNTER                   (0x11U)
#define BNO085_SENSOR_STABILITY_CLASSIFIER           (0x13U)
#define BNO085_SENSOR_PERSONAL_ACTIVITY_CLASSIFIER   (0x1EU)

#define BNO085_ACCELEROMETER_Q1        (8U)
#define BNO085_LINEAR_ACCELEROMETER_Q1 (8U)
#define BNO085_MAGNETOMETER_Q1         (4U)
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

static uint8 s_bno085_addr = BNO085_I2C_ADDR_DEFAULT;
static uint8 s_bno085_ready = 0;
static uint8 s_last_error = BNO085_ERROR_NONE;
#if BNO085_USE_SOFT_IIC
static soft_iic_info_struct s_bno085_iic;
#endif

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

static int16 s_raw_accel_x;
static int16 s_raw_accel_y;
static int16 s_raw_accel_z;
static uint8 s_accel_accuracy;
static uint32 s_accel_update_count;

static int16 s_raw_lin_accel_x;
static int16 s_raw_lin_accel_y;
static int16 s_raw_lin_accel_z;
static uint8 s_lin_accel_accuracy;
static uint32 s_lin_accel_update_count;

static int16 s_raw_mag_x;
static int16 s_raw_mag_y;
static int16 s_raw_mag_z;
static uint8 s_mag_accuracy;
static uint32 s_mag_update_count;

static uint32 s_step_count;
static uint8 s_stability_classifier;
static uint8 s_activity_classifier;
static uint32 s_step_update_count;
static uint32 s_stability_update_count;
static uint32 s_activity_update_count;

static uint32 s_rx_count;
static bno085_imu_data_t s_imu_data;

static uint8 bno085_probe(uint8 addr);
static uint8 bno085_receive_packet(void);
static uint8 bno085_get_data(uint16 chars_remaining);
static uint8 bno085_send_packet(uint8 channel, uint8 data_length);
static uint8 bno085_enable_report(uint8 report_id, uint32 interval_us);
static void bno085_iic_bus_recover(void);
static void bno085_iic_init(void);
#if !BNO085_USE_SOFT_IIC
static void bno085_iic_enable_pullup(gpio_pin_enum pin);
static void bno085_iic_clear_status(void);
static uint8 bno085_iic_wait_complete(void);
static uint8 bno085_iic_wait_idle(void);
#endif
static void bno085_iic_abort_transfer(void);
static uint8 bno085_iic_probe_address(uint8 addr);
static uint8 bno085_iic_read_advanced(uint8 *data, uint16 len, uint8 wait_idle, uint8 send_stop);
static uint8 bno085_iic_write(const uint8 *data, uint16 len);
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

    /* 总线恢复：上次会话若被中途打断（MCU 复位/调试暂停），
     * 从机可能拉低 SDA 卡死总线，先打 9 个时钟把它冲出来 */
    bno085_iic_bus_recover();
    bno085_iic_init();

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

    // BNO085 上电后会先吐启动包；这里清掉旧包，不主动 soft reset。
    // 参考 STM32 HAL 版本的 demo：soft reset 虽被 ACK，但会让某些模块
    // 只回启动/命令包而不再输出传感器 report，因此保持上电后简单冲洗最稳定。
    for (uint16 i = 0; i < BNO085_STARTUP_FLUSH_MS; i += 2U)
    {
        (void)bno085_receive_packet();
        system_delay_ms(2);
    }

    if (!bno085_enable_report(BNO085_SENSOR_GYROSCOPE, 1000000UL / BNO085_GYRO_RATE_HZ))
    {
        if (s_last_error == BNO085_ERROR_NONE)
        {
            s_last_error = BNO085_ERROR_ENABLE;
        }
        return 1;
    }

    /* 除陀螺仪外都不作为启动门控，避免融合报告异常导致整颗 IMU 被跳过。 */
#if BNO085_ROTATION_VECTOR_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_ROTATION_VECTOR, 1000000UL / BNO085_ROTATION_VECTOR_RATE_HZ);
#endif
#if BNO085_ACCEL_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_ACCELEROMETER, 1000000UL / BNO085_ACCEL_RATE_HZ);
#endif
#if BNO085_LINEAR_ACCEL_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_LINEAR_ACCELERATION, 1000000UL / BNO085_LINEAR_ACCEL_RATE_HZ);
#endif
#if BNO085_MAG_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_MAGNETIC_FIELD, 1000000UL / BNO085_MAG_RATE_HZ);
#endif
#if BNO085_STEP_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_STEP_COUNTER, 1000000UL / BNO085_STEP_RATE_HZ);
#endif
#if BNO085_STABILITY_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_STABILITY_CLASSIFIER, 1000000UL / BNO085_STABILITY_RATE_HZ);
#endif
#if BNO085_ACTIVITY_RATE_HZ > 0
    (void)bno085_enable_report(BNO085_SENSOR_PERSONAL_ACTIVITY_CLASSIFIER, 1000000UL / BNO085_ACTIVITY_RATE_HZ);
#endif

    /*
     * 使能命令发出后，BNO085 会先回复 GetFeature Response（控制通道包），
     * 再开始输出传感器数据包。这里等待约 200ms 并冲洗掉控制通道包，
     * 确保 bno085_update() 第一次调用就能取到有效数据。
     */
    for (uint16 i = 0; i < BNO085_FEATURE_FLUSH_MS; i += 5U)
    {
        (void)bno085_receive_packet();
        system_delay_ms(5);
    }

    s_bno085_ready = 1;
    s_last_error = BNO085_ERROR_NONE;
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

uint32 bno085_get_accel_update_count(void)
{
    return s_accel_update_count;
}

uint32 bno085_get_lin_accel_update_count(void)
{
    return s_lin_accel_update_count;
}

uint32 bno085_get_mag_update_count(void)
{
    return s_mag_update_count;
}

uint32 bno085_get_step_update_count(void)
{
    return s_step_update_count;
}

uint32 bno085_get_stability_update_count(void)
{
    return s_stability_update_count;
}

uint32 bno085_get_activity_update_count(void)
{
    return s_activity_update_count;
}

const bno085_imu_data_t *bno085_get_imu_data(void)
{
    return &s_imu_data;
}

/*-----------------------------------------------------------
 * IIC 总线恢复
 * 从机可能因上次会话被中途打断（MCU 复位/调试暂停）而拉低 SDA
 * 卡死总线；先打 9 个 SCL 脉冲让从机吐完残余数据，再补一个 STOP。
 *-----------------------------------------------------------*/
static void bno085_iic_bus_recover(void)
{
    gpio_init(BNO085_SCL_PIN, GPO, GPIO_HIGH, GPO_OPEN_DRAIN);
    gpio_init(BNO085_SDA_PIN, GPO, GPIO_HIGH, GPO_OPEN_DRAIN);
    gpio_high(BNO085_SDA_PIN);                          /* 释放 SDA */
    gpio_high(BNO085_SCL_PIN);
    for (uint8 i = 0; i < 9U; i++)
    {
        gpio_low(BNO085_SCL_PIN);
        system_delay_us(10);
        gpio_high(BNO085_SCL_PIN);
        system_delay_us(10);
    }
    /* 补 STOP：SCL 高电平期间 SDA 由低跳高 */
    gpio_low(BNO085_SDA_PIN);
    system_delay_us(10);
    gpio_high(BNO085_SDA_PIN);
    system_delay_us(10);
}

#if BNO085_USE_SOFT_IIC

static void bno085_iic_init(void)
{
    soft_iic_init(&s_bno085_iic,
                  s_bno085_addr,
                  BNO085_SOFT_IIC_DELAY,
                  BNO085_SCL_PIN,
                  BNO085_SDA_PIN);
}

static void bno085_iic_abort_transfer(void)
{
    soft_iic_stop(&s_bno085_iic);
}

static uint8 bno085_iic_probe_address(uint8 addr)
{
    uint8 ack;

    s_bno085_iic.addr = addr;
    soft_iic_start(&s_bno085_iic);
    ack = soft_iic_send_data(&s_bno085_iic, (uint8)(addr << 1));
    soft_iic_stop(&s_bno085_iic);

    return ack;
}

static uint8 bno085_iic_read_advanced(uint8 *data, uint16 len, uint8 wait_idle, uint8 send_stop)
{
    (void)wait_idle;

    if ((NULL == data) || (0U == len))
    {
        return 0;
    }

    s_bno085_iic.addr = s_bno085_addr;
    soft_iic_start(&s_bno085_iic);
    if (!soft_iic_send_data(&s_bno085_iic, (uint8)((s_bno085_addr << 1) | 0x01U)))
    {
        soft_iic_stop(&s_bno085_iic);
        s_last_error = BNO085_ERROR_NO_ACK;
        return 0;
    }

    for (uint16 i = 0; i < len; i++)
    {
        uint8 nack_last_byte = ((i + 1U) == len) && send_stop;
        data[i] = soft_iic_read_data(&s_bno085_iic, nack_last_byte);
    }

    if (send_stop)
    {
        soft_iic_stop(&s_bno085_iic);
    }

    return 1;
}

static uint8 bno085_iic_write(const uint8 *data, uint16 len)
{
    if ((NULL == data) || (0U == len))
    {
        return 0;
    }

    s_bno085_iic.addr = s_bno085_addr;
    soft_iic_start(&s_bno085_iic);
    if (!soft_iic_send_data(&s_bno085_iic, (uint8)(s_bno085_addr << 1)))
    {
        soft_iic_stop(&s_bno085_iic);
        s_last_error = BNO085_ERROR_NO_ACK;
        return 0;
    }

    for (uint16 i = 0; i < len; i++)
    {
        if (!soft_iic_send_data(&s_bno085_iic, data[i]))
        {
            soft_iic_stop(&s_bno085_iic);
            s_last_error = BNO085_ERROR_NO_ACK;
            return 0;
        }
    }

    soft_iic_stop(&s_bno085_iic);
    return 1;
}

#else

/*-----------------------------------------------------------
 * 使能 SCL/SDA 内部上拉
 * 硬件 IIC 使用开漏输出，必须依赖上拉电阻才能产生高电平。
 * 若 BNO085 模块未焊接外部上拉（典型 4.7kΩ~10kΩ），仅靠内部弱上拉
 * 兜底也能让 100kHz 通信基本工作；有外部上拉时并联无害。
 *-----------------------------------------------------------*/
extern const uint8 gpio_iomux_index[60];                    /* 引脚 PINCM 索引表（zf_driver_gpio.c） */

static void bno085_iic_enable_pullup(gpio_pin_enum pin)
{
    uint8 io_group = ((pin >> GPIO_GROUP_INDEX_OFFSET) & GPIO_GROUP_INDEX_MASK);
    uint8 io_pin   = ((pin >> GPIO_PIN_INDEX_OFFSET) & GPIO_PIN_INDEX_MASK);

    IOMUX->SECCFG.PINCM[gpio_iomux_index[io_group * 32 + io_pin]] |= IOMUX_PINCM_PIPU_ENABLE;
}

static void bno085_iic_init(void)
{
    DL_I2C_ClockConfig clock_config;
    uint32 timer_period;

    afio_init(BNO085_SCL_PIN, GPO, BNO085_SCL_AF, GPO_AF_OPEN_DTAIN);
    afio_init(BNO085_SDA_PIN, GPO, BNO085_SDA_AF, GPO_AF_OPEN_DTAIN);

    /* 启用内部上拉：防止模块没有外部上拉时总线浮高，导致 NACK/E1 */
    bno085_iic_enable_pullup(BNO085_SCL_PIN);
    bno085_iic_enable_pullup(BNO085_SDA_PIN);

    /* 复位→上电→配置：与本项目 clock_reset() 中对其他外设的顺序保持一致 */
    DL_I2C_reset(I2C1);
    DL_I2C_enablePower(I2C1);
    system_delay_us(10);                                    /* 等待 PWREN 稳定 */

    /*
     * 使用固定 4 MHz MFCLK 作为 I2C 输入时钟，避免 BUSCLK/ULPCLK 分频变化
     * 导致实际 SCL 偏离配置值。100 kHz 时 MTPR = 4MHz / 100k / 10 - 1 = 3。
     */
    DL_SYSCTL_enableMFCLK();
    clock_config.clockSel = DL_I2C_CLOCK_MFCLK;
    clock_config.divideRatio = DL_I2C_CLOCK_DIVIDE_1;
    DL_I2C_setClockConfig(I2C1, &clock_config);

    timer_period = BNO085_I2C_CLOCK_HZ / BNO085_IIC_SPEED / 10UL;
    if (timer_period > 0UL)
    {
        timer_period--;
    }
    if (timer_period > 0x7FUL)
    {
        timer_period = 0x7FUL;
    }
    DL_I2C_setTimerPeriod(I2C1, (uint8)timer_period);

    DL_I2C_setControllerAddressingMode(I2C1, DL_I2C_CONTROLLER_ADDRESSING_MODE_7_BIT);
    DL_I2C_setControllerTXFIFOThreshold(I2C1, DL_I2C_TX_FIFO_LEVEL_BYTES_1);
    DL_I2C_setControllerRXFIFOThreshold(I2C1, DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_enableControllerACK(I2C1);
    DL_I2C_enableControllerClockStretching(I2C1);
    DL_I2C_enableController(I2C1);
    bno085_iic_clear_status();
}

static void bno085_iic_clear_status(void)
{
    DL_I2C_clearInterruptStatus(I2C1,
                                DL_I2C_INTERRUPT_CONTROLLER_RX_DONE |
                                DL_I2C_INTERRUPT_CONTROLLER_TX_DONE |
                                DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER |
                                DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER |
                                DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_FULL |
                                DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_EMPTY |
                                DL_I2C_INTERRUPT_CONTROLLER_NACK |
                                DL_I2C_INTERRUPT_CONTROLLER_START |
                                DL_I2C_INTERRUPT_CONTROLLER_STOP |
                                DL_I2C_INTERRUPT_CONTROLLER_ARBITRATION_LOST |
                                DL_I2C_INTERRUPT_TIMEOUT_A |
                                DL_I2C_INTERRUPT_TIMEOUT_B);
}

static uint8 bno085_iic_wait_complete(void)
{
    uint32 timeout = BNO085_I2C_TIMEOUT_COUNT;

    while (timeout --)
    {
        uint32 status = DL_I2C_getControllerStatus(I2C1);
        if ((status & DL_I2C_CONTROLLER_STATUS_ERROR) != 0U)
        {
            return 0;
        }
        if ((status & DL_I2C_CONTROLLER_STATUS_BUSY) == 0U)
        {
            return 1;
        }
    }

    return 0;
}

static uint8 bno085_iic_wait_idle(void)
{
    uint32 timeout = BNO085_I2C_TIMEOUT_COUNT;

    while (timeout--)
    {
        if ((DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_BUSY) == 0U)
        {
            return 1;
        }
    }

    return 0;
}

static void bno085_iic_abort_transfer(void)
{
    uint32 timeout;

    bno085_iic_clear_status();
    DL_I2C_resetControllerTransfer(I2C1);

    DL_I2C_startFlushControllerTXFIFO(I2C1);
    timeout = BNO085_I2C_TIMEOUT_COUNT;
    while (!DL_I2C_isControllerTXFIFOEmpty(I2C1) && (timeout-- > 0U))
    {
    }
    DL_I2C_stopFlushControllerTXFIFO(I2C1);

    DL_I2C_startFlushControllerRXFIFO(I2C1);
    timeout = BNO085_I2C_TIMEOUT_COUNT;
    while (!DL_I2C_isControllerRXFIFOEmpty(I2C1) && (timeout-- > 0U))
    {
    }
    DL_I2C_stopFlushControllerRXFIFO(I2C1);
    bno085_iic_clear_status();
}

static uint8 bno085_iic_probe_address(uint8 addr)
{
    uint32 timeout;

    if (!bno085_iic_wait_idle())
    {
        bno085_iic_abort_transfer();
    }

    bno085_iic_clear_status();

    /*
     * 地址探测只验证从机 ACK，不读取 SHTP 数据包。BNO085 无数据时可能
     * NACK 读地址，但仍会 ACK 写地址；这和 Arduino Wire.endTransmission()
     * 的设备探测方式一致。
     */
    DL_I2C_startControllerTransfer(I2C1, addr, DL_I2C_CONTROLLER_DIRECTION_TX, 0U);

    timeout = BNO085_I2C_TIMEOUT_COUNT;
    while ((DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_BUSY) != 0U)
    {
        if ((DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_ERROR) != 0U)
        {
            bno085_iic_abort_transfer();
            return 0;
        }
        if (timeout-- == 0U)
        {
            bno085_iic_abort_transfer();
            return 0;
        }
    }

    if ((DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_ERROR) != 0U)
    {
        bno085_iic_abort_transfer();
        return 0;
    }

    bno085_iic_clear_status();
    return 1;
}

/*-----------------------------------------------------------
 * IIC 读取 len 字节，可选择最后是否发 STOP。
 * BNO085 SHTP 包需要先读 header 再按长度读 payload，某些模块在
 * header 和 payload 之间插入 STOP 后会拒绝第二次读，因此这里支持
 * repeated START：header 不 STOP，payload 最后一段再 STOP。
 *-----------------------------------------------------------*/
static uint8 bno085_iic_read_advanced(uint8 *data, uint16 len, uint8 wait_idle, uint8 send_stop)
{
    uint32 timeout;

    if ((NULL == data) || (0U == len))
    {
        return 0;
    }

    if (wait_idle && !bno085_iic_wait_idle())
    {
        bno085_iic_abort_transfer();
    }
    if (wait_idle)
    {
        bno085_iic_clear_status();
        DL_I2C_startFlushControllerRXFIFO(I2C1);
        timeout = BNO085_I2C_TIMEOUT_COUNT;
        while (!DL_I2C_isControllerRXFIFOEmpty(I2C1) && (timeout-- > 0U))
        {
        }
        DL_I2C_stopFlushControllerRXFIFO(I2C1);
    }

    DL_I2C_startControllerTransferAdvanced(I2C1,
                                           s_bno085_addr,
                                           DL_I2C_CONTROLLER_DIRECTION_RX,
                                           len,
                                           DL_I2C_CONTROLLER_START_ENABLE,
                                           send_stop ? DL_I2C_CONTROLLER_STOP_ENABLE : DL_I2C_CONTROLLER_STOP_DISABLE,
                                           DL_I2C_CONTROLLER_ACK_ENABLE);
    for (uint16 i = 0; i < len; i++)
    {
        uint32 timeout = BNO085_I2C_TIMEOUT_COUNT;
        while (DL_I2C_isControllerRXFIFOEmpty(I2C1))
        {
            if (DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_ERROR)
            {
                bno085_iic_abort_transfer();
                return 0;
            }
            if (timeout-- == 0U)
            {
                bno085_iic_abort_transfer();
                return 0;
            }
        }
        data[i] = DL_I2C_receiveControllerData(I2C1);
    }

    if (send_stop && !bno085_iic_wait_idle())
    {
        bno085_iic_abort_transfer();
        return 0;
    }

    bno085_iic_clear_status();
    return 1;
}

static uint8 bno085_iic_write(const uint8 *data, uint16 len)
{
    uint16 sent = 0;
    uint32 timeout;

    if ((NULL == data) || (0U == len))
    {
        return 0;
    }

    if (!bno085_iic_wait_complete())
    {
        bno085_iic_abort_transfer();
    }
    bno085_iic_clear_status();
    DL_I2C_startFlushControllerTXFIFO(I2C1);
    timeout = BNO085_I2C_TIMEOUT_COUNT;
    while (!DL_I2C_isControllerTXFIFOEmpty(I2C1))
    {
        if (timeout-- == 0U)
        {
            bno085_iic_abort_transfer();
            return 0;
        }
    }
    DL_I2C_stopFlushControllerTXFIFO(I2C1);

    sent = DL_I2C_fillControllerTXFIFO(I2C1, data, len);
    DL_I2C_startControllerTransfer(I2C1, s_bno085_addr, DL_I2C_CONTROLLER_DIRECTION_TX, len);

    /* 发送阶段持续检查 ERROR：从机地址未应答或数据未应答时立即中止 */
    while (sent < len)
    {
        uint32 timeout = BNO085_I2C_TIMEOUT_COUNT;
        while (DL_I2C_isControllerTXFIFOFull(I2C1))
        {
            if (DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_ERROR)
            {
                bno085_iic_abort_transfer();
                s_last_error = BNO085_ERROR_NO_ACK;
                return 0;
            }
            if (timeout-- == 0U)
            {
                bno085_iic_abort_transfer();
                return 0;
            }
        }
        DL_I2C_transmitControllerData(I2C1, data[sent]);
        sent++;
    }

    /* 等待总线空闲，期间若出现 ERROR（如最后一个字节未 ACK）也视为失败 */
    timeout = BNO085_I2C_TIMEOUT_COUNT;
    while ((DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_BUSY) != 0U)
    {
        if (DL_I2C_getControllerStatus(I2C1) & DL_I2C_CONTROLLER_STATUS_ERROR)
        {
            bno085_iic_abort_transfer();
            s_last_error = BNO085_ERROR_NO_ACK;
            return 0;
        }
        if (timeout-- == 0U)
        {
            bno085_iic_abort_transfer();
            return 0;
        }
    }

    bno085_iic_clear_status();
    return 1;
}

#endif

static uint8 bno085_probe(uint8 addr)
{
    uint8 old_addr = s_bno085_addr;

    s_bno085_addr = addr;
    for (uint16 i = 0; i < BNO085_STARTUP_READY_TIMEOUT; i += 10U)
    {
        if (bno085_iic_probe_address(addr))
        {
            /* 若启动包已经可读，顺手读完整包；无数据不是探测失败。 */
            (void)bno085_receive_packet();
            s_last_error = BNO085_ERROR_NONE;
            return 1;
        }
        system_delay_ms(10);
    }

    s_bno085_addr = old_addr;
    return 0;
}

static uint8 bno085_receive_packet(void)
{
    if (!bno085_iic_read_advanced(s_shtp_header, BNO085_SHTP_HEADER_SIZE, 1, 1))
    {
        return 0;
    }

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
            bno085_iic_abort_transfer();
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

        /*
         * BNO08x 的长包分段读取会在每个 I2C read transaction 前重复
         * 4 字节 SHTP header，payload 需要从第 5 字节开始拷贝。
         */
        if (!bno085_iic_read_advanced(rx,
                                      bytes_to_read + BNO085_SHTP_HEADER_SIZE,
                                      1,
                                      1))
        {
            /* 包体读取中途被 NACK：传输异常，丢弃该包 */
            s_last_error = BNO085_ERROR_READ;
            return 0;
        }
        memcpy(&s_shtp_data[s_data_spot],
               &rx[BNO085_SHTP_HEADER_SIZE],
               bytes_to_read);

        s_data_spot += bytes_to_read;
        chars_remaining -= bytes_to_read;
    }

    return 1;
}

static uint8 bno085_send_packet(uint8 channel, uint8 data_length)
{
    uint16 total_length;

    if (channel >= sizeof(s_sequence_number))
    {
        s_last_error = BNO085_ERROR_PACKET;
        return 0;
    }

    total_length = (uint16)data_length + BNO085_SHTP_HEADER_SIZE;
    s_tx_packet[0] = (uint8)(total_length & 0xFFU);
    s_tx_packet[1] = (uint8)((total_length >> 8) & 0xFFU);
    s_tx_packet[2] = channel;
    s_tx_packet[3] = s_sequence_number[channel];
    memcpy(&s_tx_packet[BNO085_SHTP_HEADER_SIZE], s_shtp_data, data_length);

    /* BNO085 启动阶段可能正在输出启动包或处理上一个 SetFeature，短暂 NACK 后重试。 */
    for (uint8 retry = 0; retry < BNO085_WRITE_RETRY_COUNT; retry++)
    {
        if (bno085_iic_write(s_tx_packet, total_length))
        {
            s_sequence_number[channel]++;           /* 整包发送成功后才递增通道序号 */
            return 1;
        }
        system_delay_ms(BNO085_WRITE_RETRY_DELAY_MS);
    }

    if (s_last_error == BNO085_ERROR_NONE)
    {
        s_last_error = BNO085_ERROR_NO_ACK;
    }
    return 0;
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
    uint16 bytes_available;
    uint8 report_id;
    uint8 status;

    bytes_available = (uint16)(s_packet_length - offset);
    if (bytes_available < 4U)
    {
        return 0U;
    }

    report_id = s_shtp_data[offset];
    status = s_shtp_data[offset + 2U] & 0x03U;

    switch (report_id)
    {
        case BNO085_SENSOR_GYROSCOPE:
            if (bytes_available < 10U)
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
            if (bytes_available < 14U)
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

        case BNO085_SENSOR_ACCELEROMETER:
            if (bytes_available < 10U)
            {
                return 0U;
            }
            s_accel_accuracy = status;
            s_raw_accel_x = bno085_read_i16_le(&s_shtp_data[offset + 4U]);
            s_raw_accel_y = bno085_read_i16_le(&s_shtp_data[offset + 6U]);
            s_raw_accel_z = bno085_read_i16_le(&s_shtp_data[offset + 8U]);
            s_accel_update_count++;
            return 10U;

        case BNO085_SENSOR_LINEAR_ACCELERATION:
            if (bytes_available < 10U)
            {
                return 0U;
            }
            s_lin_accel_accuracy = status;
            s_raw_lin_accel_x = bno085_read_i16_le(&s_shtp_data[offset + 4U]);
            s_raw_lin_accel_y = bno085_read_i16_le(&s_shtp_data[offset + 6U]);
            s_raw_lin_accel_z = bno085_read_i16_le(&s_shtp_data[offset + 8U]);
            s_lin_accel_update_count++;
            return 10U;

        case BNO085_SENSOR_MAGNETIC_FIELD:
            if (bytes_available < 10U)
            {
                return 0U;
            }
            s_mag_accuracy = status;
            s_raw_mag_x = bno085_read_i16_le(&s_shtp_data[offset + 4U]);
            s_raw_mag_y = bno085_read_i16_le(&s_shtp_data[offset + 6U]);
            s_raw_mag_z = bno085_read_i16_le(&s_shtp_data[offset + 8U]);
            s_mag_update_count++;
            return 10U;

        case BNO085_SENSOR_STEP_COUNTER:
            if (bytes_available < 12U)
            {
                return 0U;
            }
            s_step_count = bno085_read_u32_le(&s_shtp_data[offset + 8U]);
            s_step_update_count++;
            return 12U;

        case BNO085_SENSOR_STABILITY_CLASSIFIER:
            if (bytes_available < 6U)
            {
                return 0U;
            }
            s_stability_classifier = s_shtp_data[offset + 4U];
            s_stability_update_count++;
            return 6U;

        case BNO085_SENSOR_PERSONAL_ACTIVITY_CLASSIFIER:
            if (bytes_available < 6U)
            {
                return 0U;
            }
            s_activity_classifier = s_shtp_data[offset + 5U];
            s_activity_update_count++;
            return 6U;

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

    /* 加速度计、线性加速度计、磁力计原始值与精度 */
    s_imu_data.accel_raw_x = s_raw_accel_x;
    s_imu_data.accel_raw_y = s_raw_accel_y;
    s_imu_data.accel_raw_z = s_raw_accel_z;
    s_imu_data.accel_accuracy = s_accel_accuracy;

    s_imu_data.lin_accel_raw_x = s_raw_lin_accel_x;
    s_imu_data.lin_accel_raw_y = s_raw_lin_accel_y;
    s_imu_data.lin_accel_raw_z = s_raw_lin_accel_z;
    s_imu_data.lin_accel_accuracy = s_lin_accel_accuracy;

    s_imu_data.mag_raw_x = s_raw_mag_x;
    s_imu_data.mag_raw_y = s_raw_mag_y;
    s_imu_data.mag_raw_z = s_raw_mag_z;
    s_imu_data.mag_accuracy = s_mag_accuracy;

    /* 计步与姿态分类 */
    s_imu_data.step_count = s_step_count;
    s_imu_data.stability_classifier = s_stability_classifier;
    s_imu_data.activity_classifier = s_activity_classifier;
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
