#include "custom_feature_def.h"
#include "ql_stdlib.h"
#include "ql_common.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "ql_uart.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "ql_timer.h"
#include "ril_network.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"

#include "TcpIpOpen.h"
#include "DatosGps.h"

/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/


static ST_GprsConfig  m_gprsCfg;



ST_PDPContxt_Callback     callback_gprs_func = 
{
    Callback_GPRS_Actived,
    CallBack_GPRS_Deactived
};

ST_SOC_Callback      callback_soc_func=
{
    callback_socket_connect,
    callback_socket_close,
    callback_socket_accept,
    callback_socket_read,    
    callback_socket_write
};

/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/
void init_TcpIpOpenCPU(void){


	// Register & open UART port
    Ql_UART_Register(UART_PORT1, CallBack_UART_Hdlr, NULL);
    Ql_UART_Open(UART_PORT1, 115200, FC_NONE);

    //register & start timer 
    Ql_Timer_Register(TCP_TIMER_ID, Callback_Timer, NULL);
    Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);
    TcpTmr_status = TRUE;

    Ql_Timer_Register(TIMEOUT_90S_TIMER_ID, Callback_Timer, NULL);
    timeout_90S_monitor = FALSE;


    Ql_Timer_Register(TcpTimerId, TimerTcp_Callback_Timer, NULL);                        
    Ql_Timer_Start(TcpTimerId, 60000, TRUE);                                             

	
}




void TimerTcp_Callback_Timer(u32 timerId, void* param){
    if(TcpTimerId == timerId){

        if(flagConectado == 1){
            //Ql_Debug_Trace("Toma datos Actuales del GPS: \r\n");
            //Ql_Debug_Trace("Toma datos Actuales del GPS: \r\n");
            TomaDatosActuales();
            SendFrame();
        }
    }
}



/****************************************************************************************************************************/
/****************************************************************************************************************************/
void SendFrame(void){
    m_tcp_state = STATE_SOC_SEND;
}


char * LlenaBuffer (){

    int i = 0;
    while (i < 800) {
        m_send_buf[i] = 0x00; /*Se inicializa el Vector bufferTx1 como cadena vacía*/
        i++;
    }
    

    if(flagGpsValido = 1){

            Ql_sprintf(nmea,"\'latitud\':\'%s\',\'longitud\':\'%s\',\'velocidad\':\'%s\',"
                    "\'altitud\':\'%s\',\'curso\':\'%s\',\'fecha\':\'%s\',\'satelites\':\'%s\'", latitudgps, longitudgps, velocidad, altura, curso,fecha,numeroSatelites);

    }else{
            Ql_sprintf(nmea,"\'latitud\':\'0\',\'longitud\':\'0\',\'velocidad\':\'00\',"
                    "\'altitud\':\'0\',\'curso\':\'0\',\'fecha\':\'0\',\'satelites\':\'00\'");

    }



	Ql_sprintf(m_send_buf," {\'idMovil\':\'%s\',\'transaccion\':\'%s\',\'dirIpCliente\':\'%d.%d.%d.%d\',\'movImei\':\'%s\',\'version\':\'%s\',"
                "\'tipoCoordenada\':\'1\',\'kilometros\':\'%s\','origenSolicitud':'1', %s}\r\n","555555","02",10,10,10,10,"32132132132131","V4-char","20", nmea);


      // Convertir cada char a minúscula
    // usando tolower
    //for (int indice = 0; bufferTxModulo[indice] != '\0'; ++indice){
    //    bufferTxModulo[indice] = tolower(bufferTxModulo[indice]);
    //}
   

    return m_send_buf;
}
/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/
static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
    switch (msg)
    {
    case EVENT_UART_READY_TO_READ:
        {

           s32 totalBytes = ReadSerialPort(port, m_RxBuf_Uart, sizeof(m_RxBuf_Uart));
           if (totalBytes > 0)
           {
               proc_handle(m_RxBuf_Uart,sizeof(m_RxBuf_Uart));
           }
           break;
        }
    case EVENT_UART_READY_TO_WRITE:
        break;
    default:
        break;
    }
}

/******************************************************************************************************************/

static s32 ReadSerialPort(Enum_SerialPort port, /*[out]*/u8* pBuffer, /*[in]*/u32 bufLen)
{
    s32 rdLen = 0;
    s32 rdTotalLen = 0;
    if (NULL == pBuffer || 0 == bufLen)
    {
        return -1;
    }
    Ql_memset(pBuffer, 0x0, bufLen);
    while (1)
    {
        rdLen = Ql_UART_Read(port, pBuffer + rdTotalLen, bufLen - rdTotalLen);
        if (rdLen <= 0)  // All data is read out, or Serial Port Error!
        {
            break;
        }
        rdTotalLen += rdLen;
        // Continue to read...
    }
    if (rdLen < 0) // Serial Port Error!
    {
        Ql_Debug_Trace("<--Fail to read from port[%d]-->\r\n", port);
        return -99;
    }
    return rdTotalLen;
}

/******************************************************************************************************************/

static void proc_handle(char *pData,s32 len)
{
    char *p = NULL;
    s32 iret;
    u8 srvport[10];

    //command: Set_APN_Param=<APN>,<username>,<password>
    p = Ql_strstr(pData,"Set_APN_Param=");
    if (p)
    {
        Ql_memset(m_apn, 0, 10);
        if (Analyse_Command(pData, 1, '>', m_apn))
        {
            Ql_Debug_Trace("<--APN Parameter Error.-->\r\n");
            return;
        }
        Ql_memset(m_userid, 0, 10);
        if (Analyse_Command(pData, 2, '>', m_userid))
        {
            Ql_Debug_Trace("<--APN Username Parameter Error.-->\r\n");
            return;
        }
        Ql_memset(m_passwd, 0, 10);
        if (Analyse_Command(pData, 3, '>', m_passwd))
        {
            Ql_Debug_Trace("<--APN Password Parameter Error.-->\r\n");
            return;
        }
        
        Ql_Debug_Trace("<--Set APN Parameter Successfully<%s>,<%s>,<%s>.-->\r\n",m_apn,m_userid,m_passwd);

        return;
    }
    
    //command: Set_Srv_Param=<srv ip>,<srv port>
    p = Ql_strstr(pData,"Set_Srv_Param=");
    if (p)
    {
        Ql_memset(m_SrvADDR, 0, sizeof(m_SrvADDR));
        if (Analyse_Command(pData, 1, '>', m_SrvADDR))
        {
            Ql_Debug_Trace("<-- Server Address Parameter Error. -->\r\n");
            return;
        }
        Ql_memset(srvport, 0, 10);
        if (Analyse_Command(pData, 2, '>', srvport))
        {
            Ql_Debug_Trace("<-- Server Port Parameter Error. -->\r\n");
            return;
        }
        m_SrvPort = Ql_atoi(srvport);
        Ql_Debug_Trace("<-- Set TCP Server Parameter Successfully<%s>,<%d>. -->\r\n",m_SrvADDR,m_SrvPort);

        m_tcp_state = STATE_NW_GET_SIMSTATE;
        Ql_Debug_Trace("<-- Restart the TCP connection process. -->\r\n");
        Ql_SOC_Close(m_socketid);
        if (!TcpTmr_status)
        {
            Ql_Timer_Start(TCP_TIMER_ID, TCP_TIMER_PERIOD, TRUE);
            TcpTmr_status = TRUE;
        }
        return;
    }

    //if not command,send it to server
    m_pCurrentPos = m_send_buf;
    Ql_strcpy(m_pCurrentPos + m_remain_len, pData);
    
    m_remain_len = Ql_strlen(m_pCurrentPos);
    
}

/******************************************************************************************************************/

static void checkErr_AckNumber(s32 err_code)
{
    if(SOC_INVALID_SOCKET == err_code)
    {
        Ql_Debug_Trace("<-- Invalid socket ID -->\r\n");
    }
    else if(SOC_INVAL == err_code)
    {
        Ql_Debug_Trace("<-- Invalid parameters for ACK number -->\r\n");
    }
    else if(SOC_ERROR == err_code)
    {
        Ql_Debug_Trace("<-- Unspecified error for ACK number -->\r\n");
    }
    else
    {
        // get the socket option successfully
    }
}


/******************************************************************************************************************/
/******************************************************************************************************************/
static void Callback_Timer(u32 timerId, void* param)
{
    s32 iRet = 0;
    if (TIMEOUT_90S_TIMER_ID == timerId)
    {
        Ql_Debug_Trace("<--90s time out!!!-->\r\n");
        Ql_Debug_Trace("<-- Close socket.-->\r\n");
        
        Ql_SOC_Close(m_socketid);

        m_tcp_state = STATE_GPRS_DEACTIVATE;

        timeout_90S_monitor = FALSE;
    }
    else if (TCP_TIMER_ID == timerId)
    {
        //Ql_Debug_Trace("<--...........m_tcp_state=%d..................-->\r\n",m_tcp_state);
        switch (m_tcp_state)
        {
            case STATE_NW_GET_SIMSTATE:
            {
            	flagConectado = 0;
                s32 simStat = 0;
                //if (Ql_NW_GetSIMCardState()== Ql_SIM_NORMAL)
                RIL_SIM_GetSimState(&simStat);
                if (simStat == SIM_STAT_READY)
                {
                    m_tcp_state = STATE_NW_QUERY_STATE;
                    Ql_Debug_Trace("<--SIM card status is READY -->\r\n");
                }else
                {
                //    Ql_Timer_Stop(TCP_TIMER_ID);
                    Ql_Debug_Trace("<--SIM card status is NOT READY -->\r\n");
                }
                break;
            }        
            case STATE_NW_QUERY_STATE:
            {
                s32 creg = 0;
                s32 cgreg = 0;
                //Ql_NW_GetNetworkState(&creg, &cgreg);
                ret = RIL_NW_GetGSMState(&creg);
                ret = RIL_NW_GetGPRSState(&cgreg);
                Ql_Debug_Trace("<--Network State:creg=%d,cgreg=%d-->\r\n",creg,cgreg);
                if((cgreg == NW_STAT_REGISTERED)||(cgreg == NW_STAT_REGISTERED_ROAMING))
                {
                    m_tcp_state = STATE_GPRS_REGISTER;
                }
                break;
            }
            case STATE_GPRS_REGISTER:
            {
                /*initial the PDP  SOCKET context*/
                ret = Ql_GPRS_Register(PDP_CONTEXT_ID, &callback_gprs_func, NULL);
                ret = Ql_SOC_Register(&callback_soc_func, NULL);
                if(m_socketid < 0)
                {
                    /*only need create one time*/
                    m_socketid = Ql_SOC_Create(PDP_CONTEXT_ID, SOCKET_TYPE);
                }
                m_tcp_state = STATE_GPRS_CONFIG;
                Ql_Debug_Trace("<--initial the PDP SOCKET context.-->\r\n");
                break;
            }
            case STATE_GPRS_CONFIG:
            {
                Ql_strcpy(m_gprsCfg.apnName, m_apn);
                Ql_strcpy(m_gprsCfg.apnUserId, m_userid);
                Ql_strcpy(m_gprsCfg.apnPasswd, m_passwd);
                m_gprsCfg.authtype = 0;
                ret = Ql_GPRS_Config(PDP_CONTEXT_ID, &m_gprsCfg);
                if (GPRS_PDP_SUCCESS == ret)
                {
                    Ql_Debug_Trace("<--configure GPRS param successfully.-->\r\n");
                }else
                {
                    Ql_Debug_Trace("<--configure GPRS param failure,ret=%d.-->\r\n",ret);
                }
                
                m_tcp_state = STATE_SOC_CONNECT;
                break;
            }
#if 0               
            case STATE_GPRS_GET_DNSADDRESS:
            {            
                u8 primaryAddr[16] = {0};
                u8 bkAddr[16] = {0};
                ret =Ql_GPRS_GetDNSAddress(0, (u32*)primaryAddr,  (u32*)bkAddr);
                if (ret == GPRS_PDP_SUCCESS)
                {
                    Ql_Debug_Trace("<--Get DNS address successfully,primaryAddr=%d.%d.%d.%d,bkAddr=%d.%d.%d.%d-->\r\n",primaryAddr[0],primaryAddr[1],primaryAddr[2],primaryAddr[3],bkAddr[0],bkAddr[1],bkAddr[2],bkAddr[3]);            
                    m_tcp_state = STATE_GPRS_GET_LOCALIP;
                }else
                {
                     Ql_Debug_Trace("<--Get DNS address failure,ret=%d.-->\r\n",ret);
                    m_tcp_state = STATE_GPRS_DEACTIVATE;
                }
                break;
            }
         
            case STATE_GPRS_GET_LOCALIP:
            {
                ST_GprsIPAddress ip_addr;
                Ql_memset(ip_addr, 0, sizeof(ip_addr));
                ip_addr.contextId = PDP_CONTEXT_ID;
                ret = Ql_GPRS_GetLocalIPAddress(&ip_addr);
                if (ret == GPRS_PDP_SUCCESS)
                {
                    Ql_Debug_Trace("<--Get Local Ip successfully,Local Ip=%d.%d.%d.%d-->\r\n",ip_addr.IPAddress);
                    m_tcp_state = STATE_SOC_REGISTER;
                }else
                {
                    Ql_Debug_Trace("<--Get Local Ip failure,ret=%d.-->\r\n",ret);
                }
                break;
            }
#endif            
            case STATE_SOC_CONNECT:
            {
                m_tcp_state = STATE_SOC_CONNECTING;
                ret = Ql_SOC_Connect(m_socketid,(u8 *) m_SrvADDR, m_SrvPort);
                if(ret != SOC_SUCCESS)
                {
                    Ql_Debug_Trace("<--Socket Connect failure,ret=%d.-->\r\n",ret);
                    Ql_Debug_Trace("<-- Close socket.-->\r\n");
                    Ql_SOC_Close(m_socketid);

                    m_tcp_state = STATE_GPRS_DEACTIVATE;
                }
                break;
            }
            case STATE_SOC_SEND:
            {
               // if (!Ql_strlen(m_send_buf))//no data need to send
                 //   break;
               //Ql_memset(m_send_buf,0x55,sizeof(m_send_buf));
               //m_send_buf[sizeof(m_send_buf)-1]=0;



               //m_send_buf[0]=*strcat(m_send_buf,"JAIME");             
        	   //m_send_buf[0]=*strcat(m_send_buf,"ANDRES");          //Colaca la Version de Software
        	   //m_send_buf[0]=*strcat(m_send_buf,"LOMBANA");             //Colacar separador ","
        	   //m_send_buf[0]=*strcat(m_send_buf,"GONZALEZ");         //Imei
        	   //m_send_buf[0]=*strcat(m_send_buf,"VALERIE");             //Colacar separador "|"
        	   //m_send_buf[0]=*strcat(m_send_buf,"JAIME\r\n");   


        	   LlenaBuffer(); 

               m_remain_len = sizeof(m_send_buf);
               m_pCurrentPos = m_send_buf;
               
                m_tcp_state = STATE_SOC_SENDING;
                
                do
                {
                    ret = Ql_SOC_Send(m_socketid, m_pCurrentPos, m_remain_len);
                    Ql_Debug_Trace("<--Send data,socketid=%d,number of bytes sent=%d-->\r\n",m_socketid,ret);
                    if(ret == m_remain_len)//send compelete
                    {
                        m_remain_len = 0;
                        m_pCurrentPos = NULL;
                        m_nSentLen += ret;
                        m_tcp_state = STATE_SOC_ACK;
                        break;
                    }
                    else if(ret < 0)
                    {
                        Ql_Debug_Trace("<--Send data failure,ret=%d.-->\r\n",ret);
                        Ql_Debug_Trace("<-- Close socket.-->\r\n");
                        Ql_SOC_Close(m_socketid);//error , Ql_SOC_Close
                        m_remain_len = 0;
                        m_pCurrentPos = NULL; 
                        m_tcp_state = STATE_GPRS_DEACTIVATE;
                        break;
                    }
                    else if(ret < m_remain_len)//continue send, do not send all data
                    {
                        m_remain_len -= ret;
                        m_pCurrentPos += ret; 
                        m_nSentLen += ret;
                        break;
                    }
                }while(1);
                break;
            }
            case STATE_SOC_ACK:
            {
                u64 ackedNumCurr;
                ret = Ql_SOC_GetAckNumber(m_socketid, &ackedNumCurr);
                if (ret < 0)
                {
                    checkErr_AckNumber(ret);
                }
                if (m_nSentLen == ackedNumCurr)
                {
                    //Ql_Debug_Trace("<-- ACK Number:%llu/%llu. Server has received all data. -->\r\n\r\n", m_nSentLen, ackedNumCurr);
                    if (timeout_90S_monitor) //stop timeout monitor
                    {
                        Ql_Debug_Trace("<-- Stop 90s timer -->\r\n");
                        //Ql_Timer_Stop(TIMEOUT_90S_TIMER_ID);
                        timeout_90S_monitor = FALSE;
                    }
                    
                    Ql_memset(m_send_buf,0,SEND_BUFFER_LEN);
                    //m_tcp_state = STATE_SOC_SEND;

                    // stop the timer logic, let program idle
                    //Ql_Timer_Stop(TCP_TIMER_ID);
                    //TcpTmr_status = FALSE;
                }
                else
                {
                    if (!timeout_90S_monitor)//start timeout monitor
                    {
                        Ql_Timer_Start(TIMEOUT_90S_TIMER_ID, TIMEOUT_90S_PERIOD, FALSE);
                        timeout_90S_monitor = TRUE;
                    }
                    
                    Ql_Debug_Trace("<-- ACK Number:%llu/%llu from socket[%d] -->\r\n", ackedNumCurr, m_nSentLen, m_socketid);
                }
                break;
            }
            case STATE_GPRS_DEACTIVATE:
            {
                Ql_Debug_Trace("<--Deactivate GPRS.-->\r\n");
                Ql_GPRS_Deactivate(PDP_CONTEXT_ID);
                m_tcp_state = STATE_NW_QUERY_STATE;
                break;
            }
            default:
                break;
        }    
    }
}

/******************************************************************************************************************/
/******************************          PARA LA CONEXION DEL SOCKET              *********************************/
/******************************************************************************************************************/
void Callback_GPRS_Actived(u8 contexId, s32 errCode, void* customParam)
{
    if(errCode == SOC_SUCCESS)
    {
        Ql_Debug_Trace("<--CallBack: active GPRS successfully.-->\r\n");
        m_tcp_state = STATE_SOC_CONNECT;
    }else
    {
        Ql_Debug_Trace("<--CallBack: active GPRS successfully,errCode=%d-->\r\n",errCode);
        m_tcp_state = STATE_GPRS_ACTIVATE;
    }      
}

void Callback_GetIpByName(u8 contexId, u8 requestId, s32 errCode,  u32 ipAddrCnt, u32* ipAddr)
{
    u8 i=0;
    if (errCode == SOC_SUCCESS)
    {
        Ql_Debug_Trace("<--CallBack: get ip by name successfully.-->\r\n");
        for(i=0;i<ipAddrCnt;i++)
        {
            ipAddr += (i*4);
            Ql_Debug_Trace("<--Entry=%d, ip=%d.%d.%d.%d-->\r\n",i,ipAddr[0],ipAddr[1],ipAddr[2],ipAddr[3]);
        }
        Ql_memcpy(m_ipaddress, ipAddr, 4);
        m_tcp_state = STATE_SOC_REGISTER;
    }else
    {
        Ql_Debug_Trace("<--CallBack: get ip by name failure,(contexid=%d, requestId=%d,error=%d,num_entry=%d)-->\r\n",contexId, requestId,errCode,ipAddrCnt);
    }
}

void callback_socket_connect(s32 socketId, s32 errCode, void* customParam )
{
    if (errCode == SOC_SUCCESS)
    {
        if (timeout_90S_monitor) //stop timeout monitor
        {
           //Ql_Timer_Stop(TIMEOUT_90S_TIMER_ID);
           timeout_90S_monitor = FALSE;
        }

        Ql_Debug_Trace("<--Callback: socket connect successfully.-->\r\n");

        flagConectado = 1;

        m_tcp_state = STATE_SOC_SEND;
    }else
    {
    	flagConectado = 0;
        Ql_Debug_Trace("<--Callback: socket connect failure,(socketId=%d),errCode=%d-->\r\n",socketId,errCode);
        Ql_SOC_Close(socketId);
        m_tcp_state = STATE_SOC_CREATE;
    }
}

void callback_socket_close(s32 socketId, s32 errCode, void* customParam )
{
    m_nSentLen  = 0;
    Ql_SOC_Close(socketId); /*we close this again so next open can run normal*/
    m_tcp_state = STATE_GPRS_DEACTIVATE;
    Ql_Debug_Trace("<--CallBack: socket closed sockid %d  error %d begin re-connect.-->\r\n",socketId,errCode); 
}

void callback_socket_accept(s32 listenSocketId, s32 errCode, void* customParam )
{  
}

void callback_socket_read(s32 socketId, s32 errCode, void* customParam )
{
    s32 ret;
    if(errCode)
    {
        Ql_Debug_Trace("<--CallBack: socket read failure,(sock=%d,error=%d)-->\r\n",socketId,errCode);
        Ql_Debug_Trace("<-- Close socket.-->\r\n");
        Ql_SOC_Close(socketId);
        
        m_tcp_state = STATE_GPRS_DEACTIVATE; 
        return;
    }

    Ql_memset(m_recv_buf, 0, RECV_BUFFER_LEN);
    do
    {
        ret = Ql_SOC_Recv(socketId, m_recv_buf, RECV_BUFFER_LEN);

        if((ret < 0) && (ret != -2))
        {
            Ql_Debug_Trace("<-- Receive data failure,ret=%d.-->\r\n",ret);
            Ql_Debug_Trace("<-- Close socket.-->\r\n");
            Ql_SOC_Close(socketId); //you can close this socket  
            m_tcp_state = STATE_SOC_CREATE;
            break;
        }
        else if(ret == -2)
        {
            //wait next CallBack_socket_read
            break;
        }
        else if(ret < RECV_BUFFER_LEN)
        {
            Ql_Debug_Trace("<--Receive data from sock(%d),len(%d):%s\r\n",socketId,ret,m_recv_buf);
            break;
        }else if(ret == RECV_BUFFER_LEN)
        {
            Ql_Debug_Trace("<--Receive data from sock(%d),len(%d):%s\r\n",socketId,ret,m_recv_buf);
        }
    }while(1);
}


void callback_socket_write(s32 socketId, s32 errCode, void* customParam )
{
    s32 ret;

    if(errCode)
    {
        Ql_Debug_Trace("<--CallBack: socket write failure,(sock=%d,error=%d)-->\r\n",socketId,errCode);
        Ql_Debug_Trace("<-- Close socket.-->\r\n");
        Ql_SOC_Close(socketId);
        m_tcp_state = STATE_GPRS_DEACTIVATE;
        return;
    }


    m_tcp_state = STATE_SOC_SENDING;

    do
    {
        ret = Ql_SOC_Send(m_socketid, m_pCurrentPos, m_remain_len);
        Ql_Debug_Trace("<--CallBack: Send data,socketid=%d,number of bytes sent=%d-->\r\n",m_socketid,ret);

        if(ret == m_remain_len)//send compelete
        {
            m_remain_len = 0;
            m_pCurrentPos = NULL;
            m_nSentLen += ret;
            m_tcp_state = STATE_SOC_ACK;
            break;
         }
         else if(ret < 0)
         {
              Ql_Debug_Trace("<--Send data failure,ret=%d.-->\r\n",ret);
              Ql_Debug_Trace("<-- Close socket.-->\r\n");
              Ql_SOC_Close(socketId);//error , Ql_SOC_Close

              m_remain_len = 0;
              m_pCurrentPos = NULL; 
              m_tcp_state = STATE_GPRS_DEACTIVATE;     
              break;
        }
        else if(ret < m_remain_len)//continue send, do not send all data
        {
            m_remain_len -= ret;
            m_pCurrentPos += ret; 
            m_nSentLen += ret;
            break;
        }
     }while(1);
}


void CallBack_GPRS_Deactived(u8 contextId, s32 errCode, void* customParam )
{
    if (errCode == SOC_SUCCESS)
    {
        Ql_Debug_Trace("<--CallBack: deactived GPRS successfully.-->\r\n"); 
        m_tcp_state = STATE_NW_QUERY_STATE;
    }else
    {
        Ql_Debug_Trace("<--CallBack: deactived GPRS failure,(contexid=%d,error_cause=%d)-->\r\n",contextId,errCode); 
    }
}