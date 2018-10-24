/*****************************************************************************
 *
 * Filename:
 * ---------
 *   TcpIp.h 
 *
 * Project:
 * --------
 *   Spia GPS 
 *
 * Description:
 * ------------
 *  
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


#ifndef __TCPIP_H__
#define __TCPIP_H__

//Declara callback , funciones que seran invocadas
void init_TcpIp(void);
void SendAtCmd_Callback_Timer(u32 timerId, void* param);
void SendAtCmd();
static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);
char * CompletaBuffer ();
void Delay (void);

//Definicion de varibles
static s32 ATCmdtimerId=0x100;
u16  g_cmd_idx;
static Enum_SerialPort m_myUartPort  = UART_PORT1;
unsigned char bufferTxModulo[800];
unsigned char nmea[310];
static char sendbuffer[100];
unsigned int flagConectado;

//static char sg_aDbgBuf[DBG_BUF_MAX_LEN];  //buffer comandos AT



/*********    Pasos para la conexion del modulo *********/
typedef enum tagATCmdType {
    AT_ATE0,
    AT_CSQ,
    AT_QURCCFG,
    AT_QCFG,
    AT_CLCC,
    AT_CGSN,
    AT_QICSGP,
    AT_QIACT,
    AT_ATV1,
    AT_QIACTPRE,
    AT_QIOPEN,
    AT_SEND,
    FIN_CADENA
}ATCmdType;



#endif  // __TCPIP_H__