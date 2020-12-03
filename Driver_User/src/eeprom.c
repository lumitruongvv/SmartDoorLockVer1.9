#include "eeprom.h"
#include "delay.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_gpio.h"

uint8_t Byte_Recieve;
uint8_t Fingerledsuccessstep1[] = {0x04, 0x00};
uint8_t Fingerledsuccessstep2[] = {0x07, 0xBF};
uint8_t Fingerledsuccessstep3[] = {0x00, 0x00};
uint8_t Fingerledsuccessstep4[] = {0x04, 0x04};

uint8_t Fingerledoffstep1[] = {0x04, 0x00};
uint8_t Fingerledoffstep2[] = {0x06, 0xBE};
uint8_t Fingerledoffstep3[] = {0x02, 0x00};
uint8_t Fingerledoffstep4[] = {0x40, 0x10};

uint8_t Fingerledfailstep1[] = {0x04, 0x00};
uint8_t Fingerledfailstep2[] = {0x06, 0xBF};
uint8_t Fingerledfailstep3[] = {0x00, 0x00};
uint8_t Fingerledfailstep4[] = {0x04, 0x01};


uint8_t Fingerledcheckstep1[] = {0x04, 0x00};
uint8_t Fingerledcheckstep2[] = {0x08, 0xBF};
uint8_t Fingerledcheckstep3[] = {0x00, 0x00};
uint8_t Fingerledcheckstep4[] = {0x04, 0x10};

uint8_t Fingersleep1[] = {0x04, 0x00};
uint8_t Fingersleep2[] = {0x00, 0x08};
uint8_t Fingersleep3[] = {0x04, 0x00};
uint8_t Fingersleep4[] = {0x00, 0x08};

extern uint8_t Check_Eeprom_Manager_Or_User;



void I2C_Config(void){

  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Mode_Out_OD_HiZ_Fast);
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_OD_HiZ_Fast);
    
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,ENABLE); 

  I2C_Init(I2C1,I2C_MAX_STANDARD_FREQ, I2C_SLAVE_ADDRESS, I2C_Mode_I2C, I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  I2C_Cmd(I2C1, ENABLE);
}

void WRITER_Byte_To_Eeprom(uint16_t ReadAddr, uint8_t Byte){
  /* While the bus is busy */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
  
  /* Send Start */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  
  /* Send Address */
  I2C_Send7bitAddress(I2C1, I2C_ADDRESS_24E25, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  
  
  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(I2C1, (uint8_t)((ReadAddr & 0xFF00) >> 8));
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  
  /* Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(I2C1, (uint8_t)(ReadAddr & 0x00FF));
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /* Send Byte */
  I2C_SendData(I2C1, Byte);
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  
  /*Send Stop*/
  I2C_GenerateSTOP(I2C1, ENABLE);    
  /*Test on EV7 and clear it */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
}

void WRITER_Page_To_Eeprom(uint16_t ReadAddr, uint8_t Byte)
{
  ReadAddr = 64*ReadAddr;
  /* While the bus is busy */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
  
  /* Send Start */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  
  /* Send Address */
  I2C_Send7bitAddress(I2C1, I2C_ADDRESS_24E25, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  
  
  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(I2C1, (uint8_t)((ReadAddr & 0xFF00) >> 8));
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  
  /* Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(I2C1, (uint8_t)(ReadAddr & 0x00FF));
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  for(uint8_t i = 0; i < 64; i++)
  {
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
    {}
    /* Send Byte */
    I2C_SendData(I2C1, Byte);
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {}
  }
  
  /*Send Stop*/
  I2C_GenerateSTOP(I2C1, ENABLE);    
  /*Test on EV7 and clear it */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
}

uint8_t READ_Byte_From_Eeprom(uint16_t ReadAddr){
  
    /* While the bus is busy */
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {}     
    
    /* Send Start */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {}
    
    /* Send Address */
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS_24E25, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {}
    
    /* Send the EEPROM's internal address to read from: MSB of the address first */
    I2C_SendData(I2C1, (uint8_t)((ReadAddr & 0xFF00) >> 8));
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {}
    
    /* Send the EEPROM's internal address to read from: LSB of the address */
    I2C_SendData(I2C1, (uint8_t)(ReadAddr & 0x00FF));
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {}
   
    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {}
        
    /* Send Address  and read data */
    I2C_Send7bitAddress(I2C1, I2C_ADDRESS_24E25, I2C_Direction_Receiver);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {}
  
    /* Disable Acknowledgement */
    I2C_AcknowledgeConfig(I2C1, DISABLE);
  
    /*Send Stop*/
    I2C_GenerateSTOP(I2C1, ENABLE);
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {}
    
    /* Read a byte from the EEPROM */
    Byte_Recieve = I2C_ReceiveData(I2C1);
    return Byte_Recieve;
}


void WRITER_Frame_To_Eeprom(uint16_t ReadAddr, uint8_t* Frame_Data, uint8_t Size_Of_Frame){
  for(uint8_t i = 0; i < Size_Of_Frame; i++){
    WRITER_Byte_To_Eeprom(ReadAddr, Frame_Data[i]);
    ReadAddr++;
    delay_ms(5);
  }
}


uint8_t READ_Frame_From_Eeprom(uint8_t *Buffer, uint16_t ReadAddr, uint8_t Size_Of_Frame){
  uint8_t Number_Of_Password = 0;
  for(uint8_t i = 0; i < Size_Of_Frame; i++){
    Buffer[i] = READ_Byte_From_Eeprom(ReadAddr);
    if(Buffer[i] == 0xFF)
    {
        Number_Of_Password++;
        if(i == 0||i== 1||i==2)
        return Size_Of_Frame;
    }
    ReadAddr++;
    delay_us(10);
  }
  return (Size_Of_Frame-Number_Of_Password-3);
}

void WRITER_Byte_To_Finger(uint8_t Byte){
  /* While the bus is busy */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
  
  /* Send Start */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  
  /* Send Address */
  I2C_Send7bitAddress(I2C1, I2C_ADDRESS_FINGER_WRITE, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  
  /* Send Byte */
  I2C_SendData(I2C1, Byte);
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
  
  /*Send Stop*/
  I2C_GenerateSTOP(I2C1, ENABLE);    
  /*Test on EV7 and clear it */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
}

void WRITER_Buffer_To_Finger(uint8_t *Buffer,uint8_t length)
{
  uint8_t i= length;
  /* While the bus is busy */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
  
  /* Send Start */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  
  /* Send Address */
  I2C_Send7bitAddress(I2C1, I2C_ADDRESS_FINGER_WRITE, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  while(i--)
  {
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
    {}
    /* Send Byte */
    I2C_SendData(I2C1,*Buffer);
    Buffer++;
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {}
  }
    /*Send Stop*/
  I2C_GenerateSTOP(I2C1, ENABLE);    
  /*Test on EV7 and clear it */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {}
}

void WRITER_Frame_To_Finger(uint8_t* Frame_Data, uint8_t Size_Of_Frame){
  for(uint8_t i = 0; i < Size_Of_Frame; i++){
    WRITER_Byte_To_Finger(Frame_Data[i]);
    delay_ms(5);
  }
}

