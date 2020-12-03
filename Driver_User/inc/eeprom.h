/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

#define I2C_SLAVE_ADDRESS 0xA0
#define I2C_ADDRESS_24E25 0xA0
#define I2C_ADDRESS_FINGER 0x30
#define I2C_ADDRESS_FINGER_WRITE 0x60
#define I2C_ADDRESS_FINGER_READ 0x61
#define ADDRESS_KEY_FIRST 0
#define ADDRESS_CARD_FIRST 600
#define ADDRESS_FINGERPRINT_FIRST 3000
#define ADDRESS_END 5400
#define ADDRESS_LEFT_LOCK_STATE 5440
#define ADDRESS_LANGUAGE 5441
#define ADDRESS_VOLUME 5442
#define ADDRESS_SAVE_FINGER 5443
#define ADDRESS_SAVE_PIN 5444
#define ADDRESS_SAVE_CARD 5445
#define ADDRESS_SAVE_STATE_EEPROM 5450
#define ADDRESS_DEVICE_CODE_FIRST 5500
#define ADDRESS_DEVICE_CODE_END 5508
#define ALL_FRAME_USE_OF_KEY 25
#define ALL_FRAME_USE_OF_CARD 100
#define ALL_FRAME_USE_OF_FINGERPRINT 100
#define ALL_FRAME_USE_COMMON 225
#define ALL_BYTE_USE_OF_KEY 600
#define ALL_BYTE_USE_OF_CARD 2400
#define ALL_BYTE_USE_OF_FINGERPRINT 2400
#define ALL_PAGE_USE_COMMON 85
#define CHECK_N_BYTE 240



//uint8_t ADDRESS_FIRST = 0x00;


void I2C_Config(void);

void WRITER_Byte_To_Eeprom(uint16_t ReadAddr, uint8_t Byte);

void WRITER_Page_To_Eeprom(uint16_t ReadAddr, uint8_t Byte);

uint8_t READ_Byte_From_Eeprom(uint16_t ReadAddr);

void WRITER_Frame_To_Eeprom(uint16_t ReadAddr, uint8_t* Frame_Data, uint8_t Size_Of_Frame);

uint8_t READ_Frame_From_Eeprom(uint8_t *Buffer, uint16_t ReadAddr, uint8_t Size_Of_Frame);

void WRITER_Byte_To_Finger(uint8_t Byte);
void WRITER_Buffer_To_Finger(uint8_t* Buffer,uint8_t length);
void WRITER_Frame_To_Finger(uint8_t* Frame_Data, uint8_t Size_Of_Frame);
#endif //__EEPROM_H