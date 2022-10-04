#include "hal_mpu6050.h"

/* Private function */
HAL_StatusTypeDef HAL_MPU6050_ReadRegs(MPU_HandleTypeDef *hmpu, uint8_t *RegAddress, uint16_t RegCount, uint8_t *pData);
HAL_StatusTypeDef HAL_MPU6050_WriteRegs(MPU_HandleTypeDef *hmpu, uint8_t *RegAddress, uint16_t RegCount, uint8_t *pData);


/**
  * @brief MPU6050 Initialization Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MPU6050_Init(MPU_HandleTypeDef *hmpu)
{
  // set AD0 pin state
  HAL_GPIO_WritePin(hmpu->AD0_GPIO, hmpu->AD0_Pin, hmpu->AD0_State);

  hmpu->address = (MPU_DEFAULT_7BIT_ADDR | hmpu->AD0_State) << 1;

  uint8_t check;
  HAL_ValidOperation(HAL_MPU6050_ReadReg, hmpu, MPU_REG_WHO_AM_I, &check);
  if(check != MPU_DEFAULT_7BIT_ADDR)
  {
    return HAL_ERROR;
  }
  HAL_ValidOperation(HAL_MPU6050_Reset, hmpu);
  HAL_ValidOperation(HAL_MPU6050_SetGyro, hmpu, hmpu->GyroFullScale);
  HAL_ValidOperation(HAL_MPU6050_SetAcce, hmpu, hmpu->AcceFullScale);
  HAL_ValidOperation(HAL_MPU6050_SetSamplingRate, hmpu, hmpu->SamplingRate);

  uint8_t addr[] = {
    MPU_REG_INT_ENABLE,
    MPU_REG_USER_CTRL,
    MPU_REG_FIFO_ENABLE,
    MPU_REG_INT_PIN_CFG,
  },
  temp[] = {
    0x01,
    0x00,
    0X00,
    0x80,
  };
  HAL_ValidOperation(HAL_MPU6050_WriteRegs, hmpu, addr, sizeof(addr), temp);
  return HAL_OK;
}

/**
  * @brief MPU6050 Reset Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_Reset(MPU_HandleTypeDef *hmpu)
{
  uint8_t 
  temp = 0x80;
  HAL_ValidOperation(HAL_MPU6050_WriteReg, hmpu, MPU_REG_PWR_MGMT_1, &temp);
  HAL_Delay(100);
  temp = 0x01;
  HAL_ValidOperation(HAL_MPU6050_WriteReg, hmpu, MPU_REG_PWR_MGMT_1, &temp);
  temp = 0x00;
  HAL_ValidOperation(HAL_MPU6050_WriteReg, hmpu, MPU_REG_PWR_MGMT_2, &temp);

  return HAL_OK;
}

/**
  * @brief MPU6050 GYRO_CONFIG Register Set function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param val Value of the GYRO_CONFIG register to be set
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_SetGyro(MPU_HandleTypeDef *hmpu, uint8_t val)
{
  hmpu->GyroFullScale = val;
  val = val << 3;
  return HAL_MPU6050_WriteReg(hmpu, MPU_REG_GYRO_CONFIG, &val);
}

/**
  * @brief MPU6050 ACCE_CONFIG Register Set function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param val Value of the ACCE_CONFIG register to be set
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_SetAcce(MPU_HandleTypeDef *hmpu, uint8_t val)
{
  hmpu->AcceFullScale = val;
  val = val << 3;
  return HAL_MPU6050_WriteReg(hmpu, MPU_REG_ACCEL_CONFIG, &val);
}

/**
  * @brief Set LPF Frequency
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param lpf LPF freqency
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MPU6050_SetLPF(MPU_HandleTypeDef *hmpu, uint16_t lpf)
{
  uint8_t temp = 0;
  if(lpf >= 188) temp = 1;
  else if(lpf >= 98) temp = 2;
  else if(lpf >= 42) temp = 3;
  else if(lpf >= 20) temp = 4;
  else if(lpf >= 10) temp = 5;
  else temp = 6;
  return HAL_MPU6050_WriteReg(hmpu, MPU_REG_CONFIG, &temp);
}

/**
  * @brief Set Sampling Rate
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param rate Sampling rate(4~1000 Hz)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MPU6050_SetSamplingRate(MPU_HandleTypeDef *hmpu, uint16_t rate)
{
  uint8_t temp;
  if(rate > 1000) rate = 1000;
  if(rate < 4) rate = 4;
  hmpu->SamplingRate = rate;
  temp = 1000 / rate - 1;
  HAL_ValidOperation(HAL_MPU6050_WriteReg, hmpu, MPU_REG_SMPRT_DIV, &temp);
  HAL_ValidOperation(HAL_MPU6050_SetLPF, hmpu, rate / 2);
  return HAL_OK;
}

/**
  * @brief MPU6050 Register Writing Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param RegAddress Address of the register
  * @param pData Pointer to data buffer
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_WriteReg(MPU_HandleTypeDef *hmpu, uint8_t RegAddress, uint8_t *pData)
{
  return HAL_I2C_Mem_Write(hmpu->hi2c, hmpu->address, RegAddress, I2C_MEMADD_SIZE_8BIT, pData, 1, 1000);
}

/**
  * @brief MPU6050 Registers Writing Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param RegAddress Address array of the registers
  * @param RegCount Count of the registers
  * @param pData Pointer to data buffer
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_WriteRegs(MPU_HandleTypeDef *hmpu, uint8_t *RegAddress, uint16_t RegCount, uint8_t *pData)
{
  for(int i = 0; i < RegCount; i++)
  {
    HAL_ValidOperation(HAL_MPU6050_WriteReg, hmpu, RegAddress[i], pData + i);
  }
  return HAL_OK;
}

/**
  * @brief MPU6050 Register Reading Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param RegAddress Address of the register
  * @param pData Pointer to data buffer
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_ReadReg(MPU_HandleTypeDef *hmpu, uint8_t RegAddress, uint8_t *pData)
{
  return HAL_I2C_Mem_Read(hmpu->hi2c, hmpu->address, RegAddress, I2C_MEMADD_SIZE_8BIT, pData, 1, 1000);
}

/**
  * @brief MPU6050 Registers Reading Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param RegAddress Address array of the registers
  * @param RegCount Count of the registers
  * @param pData Pointer to data buffer
  * @retval HAL status
  */
inline HAL_StatusTypeDef HAL_MPU6050_ReadRegs(MPU_HandleTypeDef *hmpu, uint8_t *RegAddress, uint16_t RegCount, uint8_t *pData)
{
  for(int i = 0; i < RegCount; i++)
  {
    HAL_ValidOperation(HAL_MPU6050_ReadReg, hmpu, RegAddress[i], pData + i);
  }
  return HAL_OK;
}

/**
  * @brief MPU6050 Acceleration Raw Data Reading Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param pData Pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MPU6050_ReadAcce_Raw(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData)
{
  uint8_t temp[6];
  HAL_ValidOperation(HAL_I2C_Mem_Read, 
    hmpu->hi2c, hmpu->address, MPU_REG_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, temp, sizeof(temp), 1000);
  // uint8_t *raw = (uint8_t *)&pData->AcceXRaw;
  for(int i = 0; i < 3; i++)
  {
    pData->AcceRaw[2 * i] = temp[2 * i + 1];
    pData->AcceRaw[2 * i + 1] = temp[2 * i];
  }
  return HAL_OK;
}

/**
  * @brief MPU6050 Gyroscope Raw Data Reading Function
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param pData Pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MPU6050_ReadGyro_Raw(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData)
{
  uint8_t temp[6];
  HAL_ValidOperation(HAL_I2C_Mem_Read, 
    hmpu->hi2c, hmpu->address, MPU_REG_GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, temp, sizeof(temp), 1000);
  uint8_t *raw = (uint8_t *)&pData->GyroXRaw;
  for(int i = 0; i < 3; i++)
  {
    pData->GyroRaw[2 * i] = temp[2 * i + 1];
    pData->GyroRaw[2 * i + 1] = temp[2 * i];
  }
  return HAL_OK;
}

/**
  * @brief Convert MPU6050 Gyroscope Raw Data to real value (unit: degrees/s)
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param pData Pointer to data buffer
  * @retval none
  */
void HAL_MPU6050_Convert_Gyro(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData)
{
  float factor = 1;
  switch (hmpu->GyroFullScale)
  {
  case MPU_GYRO_FS_250:
    factor = 131.0;
    break;
  case MPU_GYRO_FS_500:
    factor = 65.5;
    break;
  case MPU_GYRO_FS_1000:
    factor = 32.8;
    break;
  case MPU_GYRO_FS_2000:
    factor = 16.4;
    break;
  default:
    factor = 16384.0;
    break;
  }
  for(int i = 0; i < 3; i++)
  {
    *(&pData->Gx + i) = *(&pData->GyroXRaw + i) / factor;
  }
}

/**
  * @brief Convert MPU6050 Acceleration Raw Data to real value (unit: g)
  * @param hmpu Pointer to a MPU_HandleTypeDef structure that contains
  *                the configuration information
  * @param pData Pointer to data buffer
  * @retval none
  */
void HAL_MPU6050_Convert_Acce(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData)
{
  float factor = 1;
  switch (hmpu->AcceFullScale)
  {
  case MPU_ACCE_FS_2G:
    factor = 16384.0;
    break;
  case MPU_ACCE_FS_4G:
    factor = 8192.0;
    break;
  case MPU_ACCE_FS_8G:
    factor = 4096.0;
    break;
  case MPU_ACCE_FS_16G:
    factor = 2048;
    break;
  default:
    factor = 16384.0;
    break;
  }
  for(int i = 0; i < 3; i++)
  {
    *(&pData->Ax + i) = *(&pData->AcceXRaw + i) / factor;
  }
}

__weak HAL_StatusTypeDef HAL_MPU6050_INT_Callback(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData)
{
  HAL_ValidOperation(HAL_MPU6050_ReadAcce_Raw, hmpu, pData);
  HAL_ValidOperation(HAL_MPU6050_ReadGyro_Raw, hmpu, pData);
  HAL_MPU6050_Convert_Acce(hmpu, pData);
  HAL_MPU6050_Convert_Gyro(hmpu, pData);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_MPU6050_INT_Handler(MPU_HandleTypeDef *hmpu, MPU_DataTypeDef *pData)
{
  return HAL_MPU6050_INT_Callback(hmpu, pData);
}
