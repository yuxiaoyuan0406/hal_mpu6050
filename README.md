# HAL MPU6050

MPU6050 driver based on STM32 HAL.

## Usage

- Define a global handler variable.

```c
MPU_HandleTypeDef hmpu;
MPU_DataTypeDef mpu_data;
```

- Define a init function in which set the parameters.

*assume you have already init a i2c instance elsewhere, and all the GPIO pins used are init elsewhere*

```c
void MPU_Init()
{
  hmpu.hi2c = &hi2c2;

  hmpu.AD0_GPIO = GPIOB;
  hmpu.AD0_Pin = GPIO_PIN_0;
  hmpu.AD0_State = 0;

  hmpu.INT_GPIO = GPIOB;
  hmpu.INT_Pin = GPIO_PIN_1;

  hmpu.GyroFullScale = MPU_GYRO_FS_1000;
  hmpu.AcceFullScale = MPU_ACCE_FS_16G;

  hmpu.SamplingRate = 1000;

  HAL_MPU6050_Init(&hmpu);
}
```

- Call init function when you need it.

```c
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  MPU_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
```

- To read gyroscope and accelermeter(is this word spelled correctly?)

```c
HAL_MPU6050_ReadGyro_Raw(&hmpu, &mpu_data);
HAL_MPU6050_ReadAcce_Raw(&hmpu, &mpu_data);
```

- To convert raw data to actual physical quantity

```c
HAL_MPU6050_Convert_Gyro(&hmpu, &mpu_data);
HAL_MPU6050_Convert_Acce(&hmpu, &mpu_data);
```

Note: some MCUs don't have a FPU(float process unit), the convertion may take **way too much** time.
