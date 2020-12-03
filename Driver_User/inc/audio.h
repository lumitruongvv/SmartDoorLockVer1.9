/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_H
#define __AUDIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#define AUDIO_PIN_CLK GPIO_Pin_3
#define AUDIO_PIN_CS GPIO_Pin_4
#define AUDIO_PIN_DATA GPIO_Pin_5
#define AUDIO_PIN_COMMON GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5

void AUDIO_GPIO_Init(void);

void AUDIO_Send(uint8_t Address);

uint8_t AUDIO_Handle_Send_2(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_3(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_4(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_5(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_6(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_0(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_0_6(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_0_8(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_0_9(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_0_9_0(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send_Not_Press(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Handle_Send(uint8_t Type_Command1, uint8_t Type_Command1_1, uint8_t Type_Command1_2, uint16_t Time_Sound);

uint8_t AUDIO_Press_Send(uint8_t Type_Command1, uint16_t Time_Sound);

uint8_t AUDIO_Finger_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Finger_First_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Get_Finger_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Common_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

uint8_t AUDIO_Card_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);

void AUDIO_Card_User_Manager_Send(uint8_t Type_Command1, uint16_t Time_Sound, uint8_t Language);
#endif //__AUDIO_H