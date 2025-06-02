#include "mpu6050.h"

uint8_t MPU_Data[] = {0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0x00, 0x00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00};
uint8_t preg1_Data = 0x3B;
double TEMP_OUT = 0.0;
// float roll, pitch, yaw;

void MPU6050_INIT(void)
{
    // 延时
    osDelay(100);
    uint8_t SendAddress = 0x6b;
    uint8_t SendData = 0x00; // 解除休眠状态
    HAL_I2C_Mem_Write(&hi2c3, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x19; // 采样率分频器
    SendData = 0x07;
    HAL_I2C_Mem_Write(&hi2c3, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x1A; // 低通滤波器
    SendData = 0x06;
    HAL_I2C_Mem_Write(&hi2c3, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x1B; // 陀螺仪
    SendData = 0x08;    // ± 500 °/s
    HAL_I2C_Mem_Write(&hi2c3, 0xD1, SendAddress, 1, &SendData, 1, 0xff);

    SendAddress = 0x1C; // 加速度计
    SendData = 0x00;    // ± 2g
    HAL_I2C_Mem_Write(&hi2c3, 0xD1, SendAddress, 1, &SendData, 1, 0xff);
}

void MPU6050_Read(void)
{
    HAL_I2C_Mem_Read(&hi2c3, 0xD1, preg1_Data, I2C_MEMADD_SIZE_8BIT, MPU_Data, 14, HAL_MAX_DELAY);

    double Temp = (MPU_Data[6] << 8) | MPU_Data[7];
    if (Temp > 32768)
        Temp -= 65536;
    Temp = (36.53 + Temp / 340);
    TEMP_OUT = Temp;

    int16_t ACCEL_XOUT = ((MPU_Data[0] << 8) | MPU_Data[1]) / 16384.0f;
    int16_t ACCEL_YOUT = ((MPU_Data[2] << 8) | MPU_Data[3]) / 16384.0f;
    int16_t ACCEL_ZOUT = ((MPU_Data[4] << 8) | MPU_Data[5]) / 16384.0f;
    int16_t GYRO_XOUT = ((MPU_Data[8] << 8) | MPU_Data[9]) / 65.5f;
    int16_t GYRO_YOUT = ((MPU_Data[10] << 8) | MPU_Data[11]) / 65.5f;
    int16_t GYRO_ZOUT = ((MPU_Data[12] << 8) | MPU_Data[13]) / 65.5f;

    osDelay(5);

    // float roll_a = atan2(ACCEL_YOUT, sqrt(ACCEL_XOUT * ACCEL_XOUT + ACCEL_ZOUT * ACCEL_ZOUT)) / 3.141593 * 180.0f;
    // float pitch_a = atan2(-ACCEL_XOUT, sqrt(ACCEL_YOUT * ACCEL_YOUT + ACCEL_ZOUT * ACCEL_ZOUT)) / 3.141593 * 180.0f;
    // float yaw_g = yaw + GYRO_ZOUT * 0.005;
    // float pitch_g = pitch + GYRO_XOUT * 0.005;
    // float roll_g = roll + GYRO_YOUT * 0.005;
    // const float alpha = 0.95238;
    // yaw = yaw_g;
    // if (yaw < 0)
    // {
    //     yaw += 360.0f;
    // }
    // else if (yaw >= 360.0f)
    // {
    //     yaw -= 360.0f;
    // }
    // roll = alpha * roll_g + (1 - alpha) * roll_a;
    // pitch = alpha * pitch_g + (1 - alpha) * pitch_a;
}
