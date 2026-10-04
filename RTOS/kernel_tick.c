#include "kernel_tick.h"

static TIM_HandleTypeDef htim2;
static volatile uint32_t kernel_tick_count;

HAL_StatusTypeDef KernelTick_Init(void)
{
  RCC_ClkInitTypeDef clock_config = {0};
  uint32_t flash_latency = 0U;
  uint32_t timer_clock_hz = HAL_RCC_GetPCLK1Freq();
  const uint32_t timer_counter_hz = 1000000U;

  HAL_RCC_GetClockConfig(&clock_config, &flash_latency);
  if (clock_config.APB1CLKDivider != RCC_HCLK_DIV1)
  {
    timer_clock_hz *= 2U;
  }

  if ((timer_clock_hz % timer_counter_hz) != 0U ||
      (timer_counter_hz % KERNEL_TICK_HZ) != 0U ||
      (timer_clock_hz / timer_counter_hz) > 65536U)
  {
    return HAL_ERROR;
  }

  __HAL_RCC_TIM2_CLK_ENABLE();
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = (timer_clock_hz / timer_counter_hz) - 1U;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = (timer_counter_hz / KERNEL_TICK_HZ) - 1U;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    return HAL_ERROR;
  }

  HAL_NVIC_SetPriority(TIM2_IRQn, 1U, 0U);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  return HAL_TIM_Base_Start_IT(&htim2);
}

uint32_t KernelTick_Get(void)
{
  return kernel_tick_count;
}

void KernelTick_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    kernel_tick_count++;
  }
}
