#include "KSCOSsystem.h"

#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_rcc.h"

const RCC_OscInitTypeDef LOW_RCC_OscInitStruct = {
    .OscillatorType = RCC_OSCILLATORTYPE_HSI,
    .HSIState = RCC_HSI_ON,
    .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
};
const RCC_ClkInitTypeDef LOW_RCC_ClkInitStruct = {
    .ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_HSE,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV1,
    .APB2CLKDivider = RCC_HCLK_DIV1,
};

const RCC_OscInitTypeDef NORMAL_RCC_OscInitStruct = {
    .OscillatorType = RCC_OSCILLATORTYPE_HSE,
    .HSEState = RCC_HSE_ON,
    .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
    .PLL.PLLState = RCC_PLL_ON,
    .PLL.PLLSource = RCC_PLLSOURCE_HSE,
    .PLL.PLLMUL = RCC_PLL_MUL9,
};
const RCC_ClkInitTypeDef NORMAL_RCC_ClkInitStruct = {
    .ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV2,
    .APB2CLKDivider = RCC_HCLK_DIV1,
};
const RCC_OscInitTypeDef HIGH_RCC_OscInitStruct = {
    .OscillatorType = RCC_OSCILLATORTYPE_HSE,
    .HSEState = RCC_HSE_ON,
    .HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
    .PLL.PLLState = RCC_PLL_ON,
    .PLL.PLLSource = RCC_PLLSOURCE_HSE,
    .PLL.PLLMUL = RCC_PLL_MUL16,
};
const RCC_ClkInitTypeDef HIGH_RCC_ClkInitStruct = {
    .ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV1,
    .APB2CLKDivider = RCC_HCLK_DIV1,
};

static void sys_Error_Handler(void);

void KSCOSsystem_Init(void)
{
  HAL_Init();
}

void KSCOSSystemClock_Init(unsigned char clock_type)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  switch (clock_type)
  {
  case KSCOS_LOW_CLOCK:
    RCC_OscInitStruct = LOW_RCC_OscInitStruct;
    RCC_ClkInitStruct = LOW_RCC_ClkInitStruct;
    break;
  case KSCOS_NORMAL_CLOCK:
    RCC_OscInitStruct = NORMAL_RCC_OscInitStruct;
    RCC_ClkInitStruct = NORMAL_RCC_ClkInitStruct;
    break;
  case KSCOS_HIGH_CLOCK:
    RCC_OscInitStruct = HIGH_RCC_OscInitStruct;
    RCC_ClkInitStruct = HIGH_RCC_ClkInitStruct;
    break;
  default:
    RCC_OscInitStruct = LOW_RCC_OscInitStruct;
    RCC_ClkInitStruct = LOW_RCC_ClkInitStruct;
    break;
  }

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    sys_Error_Handler();
  }

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_ACR_LATENCY_2) != HAL_OK)
  {
    sys_Error_Handler();
  }
}

static void sys_Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    
  }
}
