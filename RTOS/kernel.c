#include "kernel.h"

#include "main.h"
#include "kernel_tick.h"

static uint32_t last_toggle_tick;

void Kernel_Init(void)
{
  last_toggle_tick = KernelTick_Get();
}

void Kernel_RunOnce(void)
{
  uint32_t current_tick = KernelTick_Get();

  if ((uint32_t)(current_tick - last_toggle_tick) >= (KERNEL_TICK_HZ / 2U))
  {
    last_toggle_tick = current_tick;
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }
}

