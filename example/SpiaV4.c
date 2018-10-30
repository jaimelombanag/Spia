/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Quectel Co., Ltd. 2013
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   example_gpio.c
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   This example demonstrates how to program a GPIO pin in OpenCPU.
 *   This example choose PINNAME_STATUS pin as GPIO.
 *
 *   All debug information will be output through DEBUG port.
 *
 *   The "Enum_PinName" enumeration defines all the GPIO pins.
 *
 * Usage:
 * ------
 *   Compile & Run:
 *
 *     Set "C_PREDEF=-D __EXAMPLE_GPIO__" in gcc_makefile file. And compile the 
 *     app using "make clean/new".
 *     Download image bin to module to run.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/
#ifdef __SpiaV4__
#include "ql_trace.h"
#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_timer.h"
#include <stdio.h>

#include "ql_error.h"
#include "ql_uart.h"
#include "ql_timer.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_fs.h"

//#include "TcpIp.h"
#include "DatosGps.h"
#include "SpiaV4.h"
#include "TcpIpOpen.h"

#include <string.h>
#include "ql_type.h"
#include "ql_memory.h"
#include "ql_stdlib.h"


// Define GPIO pin
Enum_PinName SLEEP = PINNAME_PIN_3;   // led gps


static s32 LedCmdtimerId=0x101;
static s32 UpgradeTimeOutTimerId = 0x130;

static void StateLedCmd_Callback_Timer(u32 timerId, void* param);
/************************************/
unsigned int contadorGlobal;

/************************************************************************/
/* The entrance for this example application                            */
/************************************************************************/
void proc_main_task(s32 taskId)
{

    s32 iRet, lvl;
    ST_MSG  msg; 

    
    Ql_Debug_Trace("\r\n<-- SPIA GPS 4 V 3.0.0 -->\r\n");   
    Ql_Debug_Trace("Fecha Programa: %s\r\n",__DATE__);
    Ql_Debug_Trace("Hora Programa: %s\r\n\r\n",__TIME__);
    // Initialize the GPIO pin (output high level, pull up)
    iRet = Ql_GPIO_Init(SLEEP, PINDIRECTION_OUT, lvl, PINPULLSEL_PULLDOWN);
    //Ql_Debug_Trace("<-- Initialize GPIO pin (%d): output, level=%d, pull up, iRet=%d -->\r\n", SLEEP, lvl, iRet);

    Ql_Timer_Register(LedCmdtimerId, StateLedCmd_Callback_Timer, NULL);                     //Timer para el estado del LED
    
    
    /*========   Inicializadores de Timers ==*/
    Ql_Timer_Start(LedCmdtimerId, 100, TRUE);                                                //Timer para el estado del LED
    //Delay Necesario Para la conexion del Modulo
    Ql_Sleep(7000);
    /*========   Inicializadores ==*/
    init_Gps();
    //init_TcpIp();
    init_TcpIpOpenCPU();




    while(TRUE){
        Ql_OS_GetMessage(&msg);

        switch(msg.message)
        {
        case MSG_ID_RIL_READY:
            Ql_Debug_Trace("<-- Inicio programa Spia GPS... -->\r\n");
            Ql_RIL_Initialize();
        default:
            break;
        }  
         ProcessEstadoLed();
         //SendAtCmd2(); 
    }
   
}




/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/



  

/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/


void UpgradeTimeOut_Callback_Timer(u32 timerId, void* param)
{
    if(UpgradeTimeOutTimerId == timerId);
    {
        Ql_Debug_Trace("\r\n<-- ERROR, too long time used upgrade !!!\r\n <-- Reboot 3 seconds later ...-->\r\n");
        Ql_Sleep(3000);
        Ql_Reset(0);
    }
}


void StateLedCmd_Callback_Timer(u32 timerId, void* param){

    /*contadorGlobal++;
    s32 lvl = Ql_GPIO_GetLevel(SLEEP);
    lvl = (~lvl) & 0x01; // cannot be 0x1
    Ql_GPIO_SetLevel(SLEEP, lvl);
    */

    if(flagConectado == 1 && flagGpsValido == 1){

        //Ql_Debug_Trace("\r\n<-- 1 ...\r\n <-- 1 ...-->\r\n");
        contadorTodoOk ++;
        s32 gpioLvl2 = Ql_GPIO_GetLevel(SLEEP);//led gp
        if (PINLEVEL_LOW == gpioLvl2 && contadorTodoOk > 25)
        {
            
            Ql_GPIO_SetLevel(SLEEP, PINLEVEL_HIGH);
            contadorTodoOk = 0;
        } else {
            Ql_GPIO_SetLevel(SLEEP, PINLEVEL_LOW);
            
        }



    }else {

    
        s32 gpioLvl2 = Ql_GPIO_GetLevel(SLEEP);//led gps
        if (PINLEVEL_LOW == gpioLvl2)
        {
            Ql_GPIO_SetLevel(SLEEP, PINLEVEL_HIGH);
        } else {
            Ql_GPIO_SetLevel(SLEEP, PINLEVEL_LOW);
        }

    }

    
    
}

void ProcessEstadoLed(void){

/*
    if(contadorGlobal == 30){
        Ql_Debug_Trace("\r\n<-- DEBE CAMBIAR DE TEMPO -->\r\n ");
        Ql_Timer_Stop(LedCmdtimerId);
        Ql_Timer_Start(LedCmdtimerId, 2000, TRUE); 
    }
    if(contadorGlobal == 45){
         Ql_Timer_Stop(LedCmdtimerId);
        Ql_Timer_Start(LedCmdtimerId, 500, TRUE); 
    }
    if(contadorGlobal == 60){
         Ql_Timer_Stop(LedCmdtimerId);
        Ql_Timer_Start(LedCmdtimerId, 100, TRUE); 
    }
*/


    if(flagConectado == 1 && flagGpsValido == 1 && flagLed != 1){
        flagLed = 1;
        Ql_Timer_Start(LedCmdtimerId, 100, TRUE); 
    }else if(flagConectado == 1 && flagGpsValido == 0 && flagLed != 2){
         flagLed = 2;
         Ql_Timer_Start(LedCmdtimerId, 500, TRUE); 
    }else if(flagConectado == 0 && flagGpsValido == 1 && flagLed != 3){
        flagLed = 3;
        Ql_Timer_Start(LedCmdtimerId, 1500, TRUE); 
    }else if (flagLed != 4){
        flagLed = 4;
        Ql_Timer_Start(LedCmdtimerId, 100, TRUE); 
    }
    

}


#endif //__SpiaV4__