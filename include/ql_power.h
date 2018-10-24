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
 *   ql_power.h 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   Power APIs defines.
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
 

#ifndef __POWER_MGMT_H__
#define __POWER_MGMT_H__
#include "ql_type.h"

typedef enum {
    KEY_UP,
    KEY_DOWN
}Enum_KeyState;

typedef enum {
    POWER_ON,
    POWER_OFF
}Enum_PowerKeyOpType;

typedef enum {
    PWRKEYPWRON = 0,
    CHRPWRON	= 1,
    RTCPWRON = 2,
    CHRPWROFF = 3,
    WDTRESET = 4, /*NORMAL*/
    ABNRESET = 5,  /*ABNORMAL RESET*/
    USBPWRON = 6,  
    USBPWRON_WDT = 7,
    PRECHRPWRON = 8,
    HWSYSRST = 9,
    UNKNOWN_PWRON = 0xF9	
}Enum_PowerOnReason;

typedef enum{
    VBATT_UNDER_WRN = 0,
    VBATT_UNDER_PDN,
    VBATT_OVER_WRN,
    VBATT_OVER_PDN
}Enum_VoltageIndType;

/*****************************************************************
* Function:     Ql_PowerDown
*
* Description:
*               This function powers down the module. Before powering  
*               down, this function unregisters the GSM/GPRS network.
*               So there's some delay for powering down.
* Parameters:
*               pwrDwnType:
*                   Must be 1 = Normally power off
* Return:
*               None
*****************************************************************/
void Ql_PowerDown(u8 pwrDwnType);


/*****************************************************************
* Function:     Ql_SleepEnable 
* 
* Description:
*               Set the module into sleep mode at once
*
* Return:        
*               QL_RET_OK indicates this function successes.
*****************************************************************/
s32 Ql_SleepEnable(void);

/*****************************************************************
* Function:     Ql_SleepDisable 
* 
* Description:
*               Exit  the sleep mode 
*
* Return:        
*               QL_RET_OK indicates this function successes.
*****************************************************************/
 s32 Ql_SleepDisable(void);

#endif
