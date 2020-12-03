#include "getpassword.h"
#include "define.h"
#include "tsm12.h"
#include "leddisplay.h"
#include "delay.h"
#include "timer.h"
#include "mfcv520.h"
#include "uart.h"
#include "fingerprint.h"
#include "managerpower.h"
#include "definepin.h"
#include "audio.h"
#include "mainfunctionlist.h"
#include "stm8l15x_usart.h"
#include "stm8l15x_gpio.h"
#include <stddef.h>
#include <string.h>

extern volatile uint8_t Sleep_Flag, Finger_Flag, Zb_Flag;
extern volatile uint8_t Button_Card_Flag, Bell_Flag, Finger_Timeout_Flag;
extern bool Check_Enroll;
extern uint8_t Finger_Value, fingerPrintStatem, Check_Language, Eeprom_Flag;
extern uint8_t Password_Temp_Array[50];
extern uint8_t Key;
extern uint16_t Finger_Timeout;
extern volatile uint8_t Count_Motor;

/* GET Password */
uint8_t GET_Password(uint8_t* Password_Array)
{
    cleanmemory(Password_Array, 0, 24);
    cleanmemory(Password_Temp_Array, 0, 50);
    uint8_t Key = 0, Count_Key = 0;
    do
    {
        while(Key == 0)
        {
            Key = TSM12_task_manager();
            delay_ms(40);
            if(Sleep_Flag == 1)
            {
                return BACK;
            }
            else if(Finger_Flag == 1)
            {
                Button_Card_Flag = 0;
                return 0;
            }
            // else if(Zb_Flag == 1)
            // {
            //     Zb_Flag = 0;
            //     Button_Card_Flag = 0;
            //     return 0;
            // }
            else if(Bell_Flag == 1)
            {
                Button_Card_Flag = 0;
                return 0;
            }
            
        }
        if(Key == '*')
        {
            return BACK;
        }
        else
        {
            if(Key == '#')
            {
                return REPEAT;
            }
            Password_Array[Count_Key] = Key;
            Password_Temp_Array[Count_Key] = Key;
            Key = 0;
            Count_Key++;
        }
    }while(Key != '#');
    return 0;
}

uint8_t GET_Pin(uint8_t* Password_Array)
{
    cleanmemory(Password_Array, 0, 24);
    uint8_t Key = 0, Count_Key = 0;
    do
    {
        while(Key == 0)
        {
            Key = TSM12_task_manager();
            delay_ms(40);
            if(Sleep_Flag == 1)
            {
                return 0;
            }
        }
        if(Key == '*')
        {
            return BACK;
        }else
        {
            if(Key == '#')
            {
                return REPEAT;
            }
            Password_Array[Count_Key] = Key;
            Key = 0;
            Count_Key++;
        }
    }while(Key != '#');
    return 0;
}

/* GET Key */
uint8_t GET_Key(void)
{
    uint8_t Keyc = 0;
    while(Keyc == 0)
    {
        Keyc = TSM12_key_pad_polling();
        // delay_ms(100);
        if(Sleep_Flag == 1)
            return BACK;
    }
    return Keyc;
}

uint8_t GET_Key_8(void)
{
    uint8_t Keyc = 0;
    while(Keyc == 0)
    {
        Keyc = TSM12_key_pad_polling();
        // delay_ms(100);
        if((Sleep_Flag == 1)||(Count_Motor == 0))
            return BACK;
    }
    return Keyc;
}

/* GET Card */
uint8_t GET_Card(uint8_t* Card_Array)
{
    uint8_t Key = 0;
    cleanmemory(Card_Array, 0x00, SIZEOFFRAME);
    AUDIO_Card_User_Manager_Send(SUGGEST_SCANING_CARD, 100, Check_Language);
    do
    {
        Key = TSM12_key_pad_polling();
        if(Key == '*') return Key;
        else
        {
            READ_Card(Card_Array);
        }
        

    } while ((Card_Array[0] == 0x00) && (Sleep_Flag == 0));
    if(Card_Array[0] != 0x00)
    {
        return 1;
    }
    else if(Sleep_Flag == 1)
    {
        return 2;
    }
    return 0;

}

/*GET Finger */
bool GET_Finger(void)
{
    bool Check_Finger_local = FALSE;
    uint8_t Correct_Number_Of_Times = 0, Check_Key = 0;
    uint32_t Timeoutfinger = 0x3FFF;
    for(uint8_t i = 0; i < 7; i++)
    {
        do
        {
            TIME_Proc();
            Check_Finger_local = Function_HandEnroll(Finger_Value);
            FingerPrint_Proc();
            Timeoutfinger--;
            if(Timeoutfinger%32 == 0)
                Check_Key = TSM12_key_pad_polling();
            
        } while ((Check_Finger_local == FALSE) && (Timeoutfinger != 0) && (Check_Key != '*'));
        if(Check_Key == '*')
        {
            Check_Key = 0;
            LED_Dispaly(0x00, 0x00);
            AUDIO_Press_Send(SOUND_WHEN_PRESS_NUMBER, 300);
            FingerPrint_Send_Off_Led();
            Function_Remove_One_Fingerprint(Finger_Value);
            if(Eeprom_Flag == 0)
            {
                Function_Handle_Setting_Fingerprint();
            }else
            {
                Function_Handle_Setting_Resgister();   
            }
            Key = 0xFF;
            return TRUE;
        }else if(Check_Finger_local == TRUE)
        {
            Timeoutfinger = 0x3FFF;
            Check_Finger_local = FALSE;
            Correct_Number_Of_Times++;
            if(Correct_Number_Of_Times == 7)
            {
                Correct_Number_Of_Times = 0;
                Check_Finger_local = TRUE;
                LED_Dispaly(0x00, 0x00);
                FingerPrint_Send_Success_Led();
                AUDIO_Get_Finger_Send(SOUND_WHEN_PRESS_NUMBER, 200, Check_Language);
                return TRUE;
            }
            FingerPrint_Send_Off_Led();
            // FingerPrint_Enable_Int();
            AUDIO_Get_Finger_Send(SOUND_WHEN_PRESS_NUMBER, 200, Check_Language);
            FingerPrint_Send_Check_Led();
            AUDIO_Get_Finger_Send(CHANGE_ANGLE_FINGERPRINT, 10, Check_Language);
            Finger_Flag = 0;
            
        }else
        {
            return FALSE;
        }
        
    }
    return FALSE;
}

/* Search Fingger*/
bool SEARCH_Finger(void)
{   
    uint32_t Timeoutfinger = 200;
    bool Check_Finger_local = FALSE;
	do
    {	
	  	TIME_Proc();
        Check_Finger_local = Function_HandSearch();
        FingerPrint_Proc();
        delay_ms(5);
        Timeoutfinger--;
    } while ((Check_Finger_local == FALSE) && (Finger_Timeout_Flag == 0) && (Timeoutfinger > 1));
    if(Check_Finger_local == TRUE)
    {
        return TRUE;
    }else
    {
        return FALSE;
    }
    
}

bool SEARCH_Enroll_Finger(void)
{   
    uint32_t Timeoutfinger = 100;
    bool Check_Finger_local = FALSE;
    do
    {
        TIME_Proc();
        Check_Finger_local = Function_HandSearch();
        FingerPrint_Proc();
        delay_ms(5);
        Timeoutfinger--;
    } while ((Check_Finger_local == FALSE) && (Timeoutfinger != 0));
    if(Check_Finger_local == TRUE)
    {
        return TRUE;
    }else
    {
        return FALSE;
    }
    
}

bool SEARCH_Get_Finger(void)
{
    uint32_t Timeoutfinger = 200;
    bool Check_Finger_local = FALSE;
    do
    {
        TIME_Proc();
        Check_Finger_local = Function_HandSearch();
        FingerPrint_Proc();
        delay_ms(5);
        Timeoutfinger--;
    } while ((Check_Finger_local == FALSE) && (Timeoutfinger > 1));
    if(Check_Finger_local == TRUE)
    {
        return TRUE;
    }else
    {
        return FALSE;
    }
}

void cleanmemory(uint8_t* array, uint8_t value, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        array[i] = value;
    }
}