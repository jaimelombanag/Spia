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
 *   utility.c 
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   The APIs are used to parse string.
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

#include "ql_stdlib.h"
#include "ql_trace.h"
#include "ql_uart.h"

//check Symbol completeness
static bool Check_Separator(char* pCfgStr)
{
    u32 i=0,j=0,k=0;
    char* pChar1;
    char* pChar2;
    char* pConnCfg = pCfgStr;
    
    while (pChar1 = Ql_strstr(pConnCfg, "<"))
    {
        i++;
        pConnCfg = pChar1 + 1;
    }

    pConnCfg = pCfgStr;
    while (pChar1 = Ql_strstr(pConnCfg, ">"))
    {
        j++;
        pConnCfg = pChar1 + 1;
    }
    
    pConnCfg = pCfgStr;
    while (pChar1 = Ql_strstr(pConnCfg, ">,<"))
    {
        k++;
        pConnCfg = pChar1 + 1;
    }
    
    if (!((i == j)&&(i == k + 1)))
    {
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
* Function:     Analyse_Command
*
* Description:
*               Analyse command string
*
* Parameters:
*               src_str:
*                    [in]point to string which need to analyse.
*               symbol_num:
*                    [in]symbol number, the data which want to get in the front
*                        of the symbol.
*               symbol:
*                    [in]symbol ">"
*               dest_buf:
*                    [out]Point to the buffer that save the analysed data .
* Return:
*               None
*****************************************************************************/
s32 Analyse_Command(u8* src_str,s32 symbol_num,u8 symbol, u8* dest_buf)
{
    s32 i = 0;
    u8 *p[30];
    u8 *q;
    s32 result = -1;

    if (!Check_Separator(src_str))
    {
        return result;
    }
    
    if (q = Ql_strstr(src_str,"\r\n"))//remove\r\n
    {
        *q = '\0';
    }

    if (!(q = Ql_strstr(src_str,"<")))//find first'<'
    {
        return result;
    }
    p[0] = q + 1;//remove first'<'
    
    switch(symbol)
    {
        case '>':
            
            for(i=0;i<symbol_num;i++)
            {
                if (p[i+1] = Ql_strstr(p[i],">"))
                {
                    p[i+1] += 3;
                    if (i == symbol_num - 1)
                    {
                        result = 0;
                    }
                }else
                {
                    break;
                }      
            }

            if (!result)
            {
                Ql_strncpy(dest_buf,p[i-1],p[i]-p[i-1]-3);
            }
            break;

        default:
            result = -1;
            break;
    }

    return result;
}

void _PrintSDKVer(void)
{
    char strVer[30];

    Ql_memset(strVer, 0x0, sizeof(strVer));
    Ql_GetSDKVer(strVer, sizeof(strVer));
    Ql_Debug_Trace("< %s >\r\n", strVer);
}

#define HEX_PRINT_EN 0
#if HEX_PRINT_EN
static char ptr_str[2049];
#endif
void DBG_HexPrint(u8* pData, u32 nSize)
{
#if HEX_PRINT_EN
    u32 i;
    Ql_memset(ptr_str, 0x0, sizeof(ptr_str));
    for (i = 0; (i < (sizeof(ptr_str) / 3) && i < nSize); i++)
    {
        Ql_sprintf(ptr_str + i * 3, "%02X ", *(pData + i));
    }
    Ql_UART_Write(UART_PORT1, (u8*)ptr_str, i*3);
    Ql_UART_Write(UART_PORT1, (u8*)"\r\n\r\n", 4);
#endif
}

