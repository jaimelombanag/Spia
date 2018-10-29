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
void CallBack_GPRS_Deactived(u8 contextId, s32 errCode, void* customParam);
void Callback_GPRS_Actived(u8 contexId, s32 errCode, void* customParam);
void callback_socket_close(s32 socketId, s32 errCode, void* customParam );
void callback_socket_read(s32 socketId, s32 errCode, void* customParam );
void callback_socket_write(s32 socketId, s32 errCode, void* customParam );
void callback_socket_connect(s32 socketId, s32 errCode, void* customParam );
void callback_socket_accept(s32 listenSocketId, s32 errCode, void* customParam );




#define PDP_CONTEXT_ID 1
#define SEND_BUFFER_LEN     10*1024
#define RECV_BUFFER_LEN     2048


static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);
static s32 ATSend_Handler(char* line, u32 len, void* userData);
void SendAtCmd_Callback_Timer(u32 timerId, void* param);
void SendAtCmd();
void SendAtCmd2();
char * CompletaBuffer ();
void init_TcpIp(void);
void EnviaTrama(void);
void Delay (void);

//Definicion de varibles
static Enum_SerialPort m_myUartPort  = UART_PORT1;
static s32 ATCmdtimerId=0x100;
unsigned char bufferTxModulo[800];
static char sendbuffer[100];
unsigned char nmea[310];
unsigned int flagConectado;
unsigned int flagwismo;
u16  g_cmd_idx;
unsigned int poliConexion;
u8 ip_addr[5];
static u64 m_nSentLen  = 0;      // Bytes of number sent data through current socket    
static u8 m_recv_buf[RECV_BUFFER_LEN];
static s32 m_socketid= -1;  // Store Socket Id that returned by Ql_SOC_Create()
static char *m_pCurrentPos = NULL; 
static s32 m_remain_len = 0;     // record the remaining number of bytes in send buffer.



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
