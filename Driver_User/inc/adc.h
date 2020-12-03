/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

void ADC_Config_Init(void);

void ADC_Config_Low_Power_Mode(void);

void ADC_Config_DeInit(void);

uint16_t ADC_Read(void);

uint16_t ADC17_Read(void);

#endif //__ADC_H