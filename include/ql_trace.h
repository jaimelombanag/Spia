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
 *   ql_trace.h 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *  debug trace API
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
 

#ifndef __QL_TRACE_H__
#define __QL_TRACE_H__
#include "ql_type.h"

typedef enum {
    BASIC_MODE, //use the USB DM port print the debug information
    ADVANCE_MODE,
    DEBUG_PORT_UART1,
    DEBUG_PORT_USBAT /*use the USB AT port  print the debug information*/                    
} Enum_DebugMode;

/*****************************************************************
* Function:     Ql_Debug_Trace 
* 
* Description:
*               This function prints formatted output to 
*               debug serial port. Its function is same to 'sprintf'.
*
* Parameters:
*               fmt:
*                   Pointer to a null-terminated multibyte string 
*                   specifying how to interpret the data.
*                   The maximum string length is 512 bytes.
* Return:        
*               Number of characters printed
*****************************************************************/
extern s32  (*Ql_Debug_Trace)(char* fmt, ...);

/*****************************************************************
* Function:     Ql_Debug_SetMode 
* 
* Description:
*               This function  Set debug port 
*
* Parameters:
*               mode :
*                          BASIC_MODE: Set USB DM port as debug port
                            DEBUG_PORT_USBAT: Set USB AT port as debug port

* Return:        
*               Number of characters printed
*****************************************************************/
extern void Ql_Debug_SetMode(Enum_DebugMode mode);

/*****************************************************************
* Function:     Ql_Debug_GetMode 
* 
* Description:
*               This function get debug port 
*
* Return Value:
*              
*                          BASIC_MODE:USB DM port is  debug port
                            DEBUG_PORT_USBAT: USB AT port is debug port

* Return:        
*               Number of characters printed
*****************************************************************/
extern Enum_DebugMode Ql_Debug_GetMode(void);
#endif  // #end-of __QL_TRACE_H__
