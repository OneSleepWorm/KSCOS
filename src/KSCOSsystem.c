#include "../inc/KSCOSsystem.h"
#include "../inc/dd.h"
#include "stdlib.h"

void* osmalloc(size_t size)
{
    printf("osmalloc:%d\n",size);
    return malloc(size);
}

void osfree(void* ptr)
{
    free(ptr);
}

void* oscalloc(size_t num, size_t size)
{
    printf("oscalloc:%d %d\n",num,size);
    return calloc(num, size);
}

void* osrealloc(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

void* osmemmove(void* dst, const void* src, size_t len)
{
    return memmove(dst, src, len);
}

#if __USE_STM32__
#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_rcc.h"

__volatile uint32_t KSCOSsystem_Clock = 0;
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

void KSCOS_Error_Handler(void);

void KSCOSSystem_Init(void)
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
    KSCOSsystem_Clock = 8000000;
    break;
  case KSCOS_NORMAL_CLOCK:
    RCC_OscInitStruct = NORMAL_RCC_OscInitStruct;
    RCC_ClkInitStruct = NORMAL_RCC_ClkInitStruct;
    KSCOSsystem_Clock = 72000000;
    break;
  case KSCOS_HIGH_CLOCK:
    RCC_OscInitStruct = HIGH_RCC_OscInitStruct;
    RCC_ClkInitStruct = HIGH_RCC_ClkInitStruct;
    KSCOSsystem_Clock = 168000000;
    break;
  default:
    RCC_OscInitStruct = LOW_RCC_OscInitStruct;
    RCC_ClkInitStruct = LOW_RCC_ClkInitStruct;
    KSCOSsystem_Clock = 8000000;
    break;
  }

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    KSCOS_Error_Handler();
  }

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_ACR_LATENCY_2) != HAL_OK)
  {
    KSCOS_Error_Handler();
  }
}

void KSCOS_Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    
  }
}
void sysdelay(uint32_t ms)
{
  HAL_Delay(ms);
}
#endif
#if __USE_PC__
#include <stdio.h>
void KSCOS_Error_Handler(void)
{
  printf("KSCOS_Error_Handler\n");
  while (1)
  {
    
  }
}
uint32_t sysgettime(void)
{
  return (uint32_t)GetTickCount();
}
void sysdelay(uint32_t ms)
{
  Sleep(ms);
}
void KSCOSSystem_Init(){return;}
void KSCOSSystemClock_Init(uint8_t clock_type){return;}
#endif
ki8 KSCOS_default_Error_Handler(void* data)
{
  KSCOS_Error_Handler();
  return -1;
}
