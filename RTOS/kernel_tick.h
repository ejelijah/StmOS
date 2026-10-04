#ifndef KERNEL_TICK_H
#define KERNEL_TICK_H

#include "stm32f4xx_hal.h"

#define KERNEL_TICK_HZ 1000U

HAL_StatusTypeDef KernelTick_Init(void);
uint32_t KernelTick_Get(void);
void KernelTick_IRQHandler(void);

#endif
