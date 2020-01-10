/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "LCD1602.h"
#include "mpu6050.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void SystemClock_Config(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float pitch, roll, yaw;        //欧拉角

void writeMotor(unsigned char CMD) {
    HAL_GPIO_WritePin(i1_GPIO_Port, i1_Pin, GetBit(CMD, 0));
    HAL_GPIO_WritePin(i2_GPIO_Port, i2_Pin, GetBit(CMD, 1));
    HAL_GPIO_WritePin(i3_GPIO_Port, i3_Pin, GetBit(CMD, 2));
    HAL_GPIO_WritePin(i4_GPIO_Port, i4_Pin, GetBit(CMD, 3));
}

void motor_delay(uint32_t delay) {
    uint32_t i;
    for (i = 1; i < delay; i++) {
        asm("nop");
    }
}

/**
 * 逆时针
 * @param circle
 * @param delay
 */
void motor_cw(uint32_t circle, uint32_t delay)//circle循环数 delay控制转速
{
    uint32_t i, j;
    j = 6000;
    for (i = 0; i <= circle; i++) {
        writeMotor(8);
        motor_delay(j);
        writeMotor(12);
        motor_delay(j);
        writeMotor(4);
        motor_delay(j);
        writeMotor(6);
        motor_delay(j);
        writeMotor(2);
        motor_delay(j);
        writeMotor(3);
        motor_delay(j);
        writeMotor(1);
        motor_delay(j);
        writeMotor(9);
        motor_delay(j);
        motor_delay(delay);//这个控制转速
    }
}

/**
 * 顺时针
 * @param circle
 * @param delay
 */
void motor_ccw(uint32_t circle, uint32_t delay) {
    uint32_t i, j;
    j = 6000;
    for (i = 0; i <= circle; i++) {
        writeMotor(9);
        motor_delay(j);
        writeMotor(1);
        motor_delay(j);
        writeMotor(3);
        motor_delay(j);
        writeMotor(2);
        motor_delay(j);
        writeMotor(6);
        motor_delay(j);
        writeMotor(4);
        motor_delay(j);
        writeMotor(12);
        motor_delay(j);
        writeMotor(8);
        motor_delay(j);
        motor_delay(delay);//这个控制转速
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
    unsigned char rev_flag = 0;
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
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim3);
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    HAL_Delay(10);
    LCD1602_Init();

    MPU_6050_Init_Ex();

    motor_ccw(510, 1);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

//        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET) {
//            /* Key按键按下查询 */
//            HAL_Delay(3000);
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//            HAL_Delay(3000);
//        } else {
//            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//        }

        //uint8_t TxData[12] = "Hello World\n";
        //HAL_UART_Transmit(&huart1, TxData, 12, 20);

        HAL_Delay(10);
        if ((rev_flag = mpu_dmp_get_data(&pitch, &roll, &yaw)) == 0) {

            int pi = (int) (pitch * 10);
            int ro = (int) (roll * 10);
            int ya = (int) (yaw * 10);

            LCD1602_ClearScreen();
            LCD1602_Show_Str(0, 0, "Mpu6050");
            LCD1602_Show_Str_Printf(0, 1, "%d %d %d", pi, ro, ya);
        } else {
            printf("rev_flag %d\n", rev_flag);
            HAL_Delay(100);//读取频率不能太慢 防止FIFO溢出
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_5);
}

/* USER CODE BEGIN 4 */

// -----------------------

/**
  * @brief GPIO EXTI callback
  * @param None
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    /* Clear Wake Up Flag */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    // 1ms
    if (htim->Instance == htim3.Instance) {
        //编写回调逻辑，即定时器1定时1MS后的逻辑
        static int i = 0, state = 0, is = 0, k = 0;

        if (i < 100) i++;
        else {
            i = 0;
            if (k == 0) {
                if (is < 100) is++;
                else k = 1;
            } else {
                if (is > 0) is--;
                else k = 0;
            }
        }

        if (i <= is) {
            state = 0;
        } else {
            state = 1;
        }

        if (state == 0) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        }

    }
}
// -----------------------

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
