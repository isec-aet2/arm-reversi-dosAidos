
#include "main.h"

#ifndef _I_
#define _I_

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void checkMenuTS();
_Bool checkGameTS();
void checkEndTS();
void checkTIM6();
void checkTIM7();
_Bool checkPB();


#endif
