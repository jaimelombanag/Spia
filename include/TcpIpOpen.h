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


#ifndef __TCPIPOPEN_H__
#define __TCPIPOPEN_H__



/*****************************************************************
* Configure APN
******************************************************************/
static u8 m_apn[100] = "internet.comcel.com.co";
static u8 m_userid[10] = "";
static u8 m_passwd[10] = "";


/*****************************************************************
* Configure TCP/UDP Server (ip, port)
******************************************************************/
static u8  m_SrvADDR[100] = "200.91.204.38";
#define  TCP_PORT           (11250)
#define  UDP_PORT           (11250)
#define  SOCKET_TYPE        (SOC_TYPE_TCP)  // SOC_TYPE_TCP or SOC_TYPE_UDP


/*****************************************************************
* define process state
******************************************************************/
typedef enum{
    STATE_NW_GET_SIMSTATE,
    STATE_NW_QUERY_STATE,
    STATE_GPRS_REGISTER,
    STATE_GPRS_CONFIG,
    STATE_GPRS_ACTIVATE,
    STATE_GPRS_ACTIVATING,
    STATE_GPRS_GET_DNSADDRESS,
    STATE_GPRS_GET_LOCALIP,
    STATE_CHACK_SRVADDR,
    STATE_SOC_REGISTER,
    STATE_SOC_CREATE,
    STATE_SOC_CONNECT,
    STATE_SOC_CONNECTING,
    STATE_SOC_SEND,
    STATE_SOC_SENDING,
    STATE_SOC_ACK,
    STATE_SOC_CLOSE,
    STATE_GPRS_DEACTIVATE,
    STATE_TOTAL_NUM
}Enum_TCPSTATE;
static u8 m_tcp_state = STATE_NW_GET_SIMSTATE;
#define PDP_CONTEXT_ID 1
/*****************************************************************
* UART Param
******************************************************************/
#define SERIAL_RX_BUFFER_LEN  2048
static u8 m_RxBuf_Uart[SERIAL_RX_BUFFER_LEN];

/*****************************************************************
* timer param
******************************************************************/
#define TCP_TIMER_ID         TIMER_ID_USER_START
#define TIMEOUT_90S_TIMER_ID TIMER_ID_USER_START + 1   //timeout

static s32 TcpTimerId=0x107;

#define TCP_TIMER_PERIOD     800
#define TIMEOUT_90S_PERIOD   90000

static s32 TcpTmr_status = FALSE;
static s32 timeout_90S_monitor = FALSE;

#define SEND_BUFFER_LEN     1000
#define RECV_BUFFER_LEN     2048
#define DATOS_BUFFER_GPS    2048

#if (SOCKET_TYPE==SOC_TYPE_TCP)
static u32 m_SrvPort = TCP_PORT;
#else
static u32 m_SrvPort = UDP_PORT;
#endif



static u8 Imei[20];
static u8 ip_addr[5];
static u8 m_ipaddress[5];  //only save the number of server ip, remove the comma
static u8 m_send_buf[SEND_BUFFER_LEN];
static u8 m_recv_buf[RECV_BUFFER_LEN];
static u8 datosNmea[DATOS_BUFFER_GPS];
static u64 m_nSentLen  = 0;      // Bytes of number sent data through current socket    
static s32 m_socketid = -1; 
static s32 m_remain_len = 0;     // record the remaining number of bytes in send buffer.
static char *m_pCurrentPos = NULL; 

/*****************************************************************
* GPRS and socket callback function
******************************************************************/
void callback_socket_connect(s32 socketId, s32 errCode, void* customParam );
void callback_socket_close(s32 socketId, s32 errCode, void* customParam );
void callback_socket_accept(s32 listenSocketId, s32 errCode, void* customParam );
void callback_socket_read(s32 socketId, s32 errCode, void* customParam );
void callback_socket_write(s32 socketId, s32 errCode, void* customParam );

void Callback_GPRS_Actived(u8 contexId, s32 errCode, void* customParam);
void CallBack_GPRS_Deactived(u8 contextId, s32 errCode, void* customParam );
void Callback_GetIpByName(u8 contexId, u8 requestId, s32 errCode,  u32 ipAddrCnt, u32* ipAddr);

/*****************************************************************
* uart callback function
******************************************************************/
static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);

/*****************************************************************
* timer callback function
******************************************************************/
static void Callback_Timer(u32 timerId, void* param);


void TimerTcp_Callback_Timer(u32 timerId, void* param);
/*****************************************************************
* other subroutines
******************************************************************/
extern s32 Analyse_Command(u8* src_str,s32 symbol_num,u8 symbol, u8* dest_buf);
static s32 ReadSerialPort(Enum_SerialPort port, /*[out]*/u8* pBuffer, /*[in]*/u32 bufLen);
static void proc_handle(char *pData,s32 len);
static s32 Imei_Handler(char* line, u32 len, void* userData);
void init_TcpIpOpenCPU(void);
void SendFrame(void);
char *zStrrep(char *str, char x, char y);

static s32 ret;


unsigned int flagConectado;



#endif  // __TCPIPOPEN_H__