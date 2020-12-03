/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RESET_H
#define __RESET_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#define WINDOW_VALUE 97
#define COUNTER_INIT 104

#define RELOAD_VALUE   254

void RESET_CPU(void);

void RESET_CPU_BY_IWDG(void);

#endif //__AUDIO_H