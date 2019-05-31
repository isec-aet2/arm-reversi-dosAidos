
#ifndef _CONFIGS_
#define _CONFIGS_

#include "main.h"
#include "stdio.h"
#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_ts.h"


void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA2D_Init(void);
void MX_DSIHOST_DSI_Init(void);
void MX_FMC_Init(void);
void MX_LTDC_Init(void);
void MX_TIM6_Init(void);
void MX_ADC1_Init(void);
void MX_SDMMC2_SD_Init(void);
void MX_TIM7_Init(void);
void Error_Handler(void);
void assert_failed(uint8_t *file, uint32_t line);


#endif
