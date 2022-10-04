#pragma once

/********************************************************/
/*          MPU-6050/MPU-6000 Register Map              */
/*          Check datasheet for details.                */
/*  https://datasheetspdf.com/datasheet/MPU6050.html    */

#define MPU_REG_SMPRT_DIV    0x19
#define MPU_REG_CONFIG       0x1A
#define MPU_REG_GYRO_CONFIG  0x1B
#define MPU_REG_ACCEL_CONFIG 0x1C
#define MPU_REG_FIFO_ENABLE  0x23
#define MPU_REG_INT_PIN_CFG  0x37
#define MPU_REG_INT_ENABLE   0x38
#define MPU_REG_ACCEL_XOUT_H 0x3B
#define MPU_REG_ACCEL_XOUT_L 0x3C
#define MPU_REG_ACCEL_YOUT_H 0x3D
#define MPU_REG_ACCEL_YOUT_L 0x3E
#define MPU_REG_ACCEL_ZOUT_H 0x3F
#define MPU_REG_ACCEL_ZOUT_L 0x40
#define MPU_REG_TEMP_OUT_H   0x41
#define MPU_REG_TEMP_OUT_L   0x42
#define MPU_REG_GYRO_XOUT_H  0x43
#define MPU_REG_GYRO_XOUT_L  0x44
#define MPU_REG_GYRO_YOUT_H  0x45
#define MPU_REG_GYRO_YOUT_L  0x46
#define MPU_REG_GYRO_ZOUT_H  0x47
#define MPU_REG_GYRO_ZOUT_L  0x48
#define MPU_REG_USER_CTRL    0x6A
#define MPU_REG_PWR_MGMT_1   0x6B
#define MPU_REG_PWR_MGMT_2   0x6C
#define MPU_REG_WHO_AM_I     0x75