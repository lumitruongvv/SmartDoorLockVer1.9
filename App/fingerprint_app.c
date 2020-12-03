#include "fingerprint_app.h"
#include "ncp.h"
#include "stdio.h"
#include "definepin.h"
#include "stm8l15x_gpio.h"
#include "delay.h"
#include "ncp.h"
#include "define.h"
#include "drivermotor.h"
#include "stdbool.h"
#include "../Mid/fingerprint/fingerprint.h"

extern bool Check_Finger, Check_Enroll;
extern uint8_t Password_Finger;

void FingerPrint_AppHandle(u8 event, u8* par, u8 par_len)
{
    u8 errCode=ERROR_SUCCESS;

    errCode=par[0];

    switch (event)
    {
        case FINGERPRINT_HANDSHAKE_END:        
            if(errCode==ERROR_SUCCESS) 
            break;
        case FINGERPRINT_ENROLL_END:
            {
                Check_Enroll = TRUE;
                break;
            }
        case FINGERPRINT_SEARCH_END:
            if(errCode==ERROR_SUCCESS)
            {   
                u16 userID=0;
                userID=(u16)(par[1]<<8)+ (u16)par[2];
                for(uint8_t i = 0; i < NUMBERFINGER; i++)
                {
                    if(userID == i)
                    {
                        Check_Finger = TRUE;
                        Password_Finger = i;
                        break;
                    }
                }              
            }
            break;
        default:
            break;
    }
}
