#pragma once

#include <stdint.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h"
#include "hal_mpu6050_reg_addr.h"

#define MPU_DEFAULT_7BIT_ADDR (0x68)
#define MPU_AD0_STATE (0x01)
#define MPU_ADDR ((MPU_DEFAULT_7BIT_ADDR | MPU_AD0_STATE) << 1)

#define MPU_AD0_GPIO GPIOB
#define MPU_AD0_PIN GPIO_PIN_0

// Gyroscope full scale selection
#define MPU_GYRO_FS_250 (0)
#define MPU_GYRO_FS_500 (1)
#define MPU_GYRO_FS_1000 (2)
#define MPU_GYRO_FS_2000 (3)

// Acdelerometer full scale selection
#define MPU_ACCE_FS_2G (0)
#define MPU_ACCE_FS_4G (1)
#define MPU_ACCE_FS_8G (2)
#define MPU_ACCE_FS_16G (3)

// Return HAL_ERROR if the function returns a non HAL_OK value
#define HAL_ValidOperation(func, ...) if (  \
  func(__VA_ARGS__) != HAL_OK               \
) return HAL_ERROR

typedef struct
{
  I2C_HandleTypeDef *hi2c;  // I2C handler pointer
  
  GPIO_TypeDef *AD0_GPIO;   // GPIO port to pin AD0
  uint16_t AD0_Pin;         // GPIO pin to pin AD0
  uint8_t AD0_State;        // State of AD0

  GPIO_TypeDef *INT_GPIO;   // GPIO port to pin INT
  uint16_t INT_Pin;         // GPIO pin to pin INT
  
  uint8_t GyroFullScale;    // Gyroscope full scale select
  uint8_t AcceFullScale;    // Accelerometer full scale select

  uint16_t SamplingRate;    // Sampling Rate 4~1000 Hz

  uint8_t address;          // I2C address for mpu
} MPU_HandleTypeDef;

typedef struct
{
  union {                   // Accelerometer raw data
    struct {
      int16_t AcceXRaw;
      int16_t AcceYRaw;
      int16_t AcceZRaw;
    };
    uint8_t AcceRaw[6];
  };
  double Ax;
  double Ay;
  double Az;

  union {
    struct {
      int16_t GyroXRaw;
      int16_t GyroYRaw;
      int16_t GyroZRaw;
    };
    uint8_t GyroRaw[6];
  };
  double Gx;
  double Gy;
  double Gz;

  int16_t TempRaw;
  float Temp;
} MPU_DataTypeDef;

HAL_StatusTypeDef HAL_MPU6050_WriteReg(MPU_HandleTypeDef *hmpu, uint8_t RegAddress, uint8_t *pData);
HAL_StatusTypeDef HAL_MPU6050_ReadReg(MPU_HandleTypeDef *hmpu, uint8_t RegAddress, uint8_t *pData);
HAL_StatusTypeDef HAL_MPU6050_Init(MPU_HandleTypeDef *hmpu);
HAL_StatusTypeDef HAL_MPU6050_Reset(MPU_HandleTypeDef *hmpu);

HAL_StatusTypeDef HAL_MPU6050_SetGyro(MPU_HandleTypeDef *hmpu, uint8_t val);
HAL_StatusTypeDef HAL_MPU6050_SetAcce(MPU_HandleTypeDef *hmpu, uint8_t val);
HAL_StatusTypeDef HAL_MPU6050_SetSamplingRate(MPU_HandleTypeDef *hmpu, uint16_t rate);
HAL_StatusTypeDef HAL_MPU6050_SetLPF(MPU_HandleTypeDef *hmpu, uint16_t lpf);

HAL_StatusTypeDef HAL_MPU6050_ReadGyro_Raw(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData);
HAL_StatusTypeDef HAL_MPU6050_ReadAcce_Raw(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData);

void HAL_MPU6050_Convert_Gyro(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData);
void HAL_MPU6050_Convert_Acce(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData);

HAL_StatusTypeDef HAL_MPU6050_INT_Handler(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData);
