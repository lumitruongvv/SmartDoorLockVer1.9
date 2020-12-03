/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GETPASSWORD_H
#define __GETPASSWORD_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stdbool.h"

/* GET Password */
uint8_t GET_Password(uint8_t* Password_Array);

uint8_t GET_Pin(uint8_t* Password_Array);

/* GET Touch */
// uint8_t GET_Touch(uint8_t* Password_Array);

/* GET Card */
uint8_t GET_Card(uint8_t* Card_Array);


/* GET Key */
uint8_t GET_Key(void);

uint8_t GET_Key_8(void);


/* GET Fingerprint */
bool GET_Finger(void);

bool SEARCH_Enroll_Finger(void);

bool SEARCH_Finger(void);

bool SEARCH_Get_Finger(void);

void cleanmemory(uint8_t* array, uint8_t value, uint8_t len);

#endif //__GETPASSWORD_H