#include "custom_feature_def.h"
#include "ql_stdlib.h"
#include "ql_common.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "ql_timer.h"
#include "ril_network.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_audio.h"
#include "ql_stdlib.h"



#include "TcpIp.h"
#include "DatosGps.h"



/*
{'idMovil':'197026','transaccion':'02','dirIpCliente':'10.182.24.22','movImei':'867060031685312','version':'V4,2.3','tipoCoordenada':'0','kilometros':'280','origenSolicitud':'1','latitud':'0434.5840N','longitud':'07406.7863W','velocidad':'0.03.1','altitud':'2578.6','curso':'205.26','fecha':'191018233257','satelites':'10'}
2018-10-19 18:32:59:132 - 190.242.83.115:16331|SALE : {"transaccion":"02","eventoUnidad":"00"}
*/






void init_TcpIp(void){

	g_cmd_idx = 0;
	s32 ret; 
 	// Register & open UART port
    ret = Ql_UART_Register(UART_PORT1, CallBack_UART_Hdlr, NULL);
    if (ret < QL_RET_OK)
    {
        Ql_Debug_Trace("Fail to register serial port[%d], ret=%d\r\n", UART_PORT1, ret);
    }
    ret = Ql_UART_Open(UART_PORT1, 115200, FC_NONE);
    Ql_UART_Open(UART_PORT1, 115200, FC_NONE);

    flagwismo = 1;

    Ql_Timer_Register(ATCmdtimerId, SendAtCmd_Callback_Timer, NULL);                        //Timer para la Tx de los comandos AT 
    Ql_Timer_Start(ATCmdtimerId, 30000, TRUE);                                               //Timer para la Tx de los comandos AT 1.5 segundos

}

void SendAtCmd_Callback_Timer(u32 timerId, void* param){
    if(ATCmdtimerId == timerId){
        //SendAtCmd();
        poliConexion ++;
        if(flagConectado == 1){
            poliConexion = 0;
            //Ql_Debug_Trace("Toma datos Actuales del GPS: \r\n");
            //TomaDatosActuales();
            //EnviaTrama();
        }else{
            if(poliConexion > 2){
                poliConexion = 0;
                Rester_Modulo();
            }
        }
    }
}


void SendAtCmd(){
    static s32 cmdstates=255;
    static s32 istimerstart=0;
        
    if(cmdstates != g_cmd_idx)
    {
        if(istimerstart){ 
            istimerstart = 0;
           // Ql_Debug_Trace("<--STOP  timer!!!-->\r\n");
        }
        cmdstates = g_cmd_idx;
        switch (g_cmd_idx)
        {
        case AT_ATE0:// Echo mode off
            Ril_tcp_ATE0();
            break;
            
        case AT_CSQ:// 
            Ril_tcp_CSQ();
            break;
            
        case AT_QURCCFG:// 
            Ril_tcp_QURCCFG();
            break;
            
        case AT_QCFG:// 
            Ril_tcp_QCFG();
            break;
            
        case AT_CLCC:// 
            Ril_tcp_CLCC();
            break;
            
        case AT_CGSN:// 
            Ril_tcp_CGSN();
            break;
            
        case AT_QICSGP:// 

           
            Ril_tcp_QICSGP();
            break;
            
        case AT_QIACT:// 
           
            Ril_tcp_QIACT();
            break;
            
        case AT_ATV1:// 
            Ril_tcp_ATV1();
            break;
            
        case AT_QIACTPRE:// 
            Ril_tcp_QIACTPRE();
            break;
            
        case AT_QIOPEN:// 
            Ril_tcp_QIOPE();
           
            break;

        case AT_SEND:// 
            //EnviaTrama();
            //Ril_Tcp_QISEND();
            //g_cmd_idx++;
            break;    

        case FIN_CADENA:// 
           
            Ql_Debug_Trace("<-- Jaime1.-->\r\n");
             //g_cmd_idx++;
            Ril_Fin();
            break;        

        default:
            Ql_Debug_Trace("<-- at commands finished.-->\r\n");
            
            // Start to fota upgrade
            //DoFOTAUpgrade();
            cmdstates=255;
            g_cmd_idx = 0;
            //Ql_Timer_Stop(ATCmdtimerId);
            break;
        }
    }
    else
    {
        if(!istimerstart){
            istimerstart = 1;
            //Ql_Debug_Trace("<--start timer!!!-->\r\n");
        }
     //   Ql_Debug_Trace("<--g_cmd_idx=%d-->\r\n",g_cmd_idx);
    }
}

void SendAtCmd2(){
   

        switch (flagwismo){

        case (1):
            Ril_tcp_ATE0();
            break;
            
        case (2):
            Ril_tcp_CSQ();
            break;
            
        case (3):
            Ril_tcp_QURCCFG();
            break;
            
        case (4):
            Ril_tcp_QCFG();
            break;
            
        case (5):
            Ril_tcp_CLCC();
            break;
            
        case (6):
            Ril_tcp_CGSN();
            break;
            
        case (7):
            Ril_tcp_QICSGP();
            break;
            
        case (8):
            Ril_tcp_QIACT();
            break;
            
        case (9):
            Ril_tcp_ATV1();
            break;
            
        case (10):
            Ril_tcp_QIACTPRE();
            break;
            
        case (11):
            Ril_tcp_QIOPE();
            break;

        case (12):
            Ril_Tcp_QISEND();
            //g_cmd_idx++;
            break;    

        case (13):
            Ql_Debug_Trace("<-- LA BANDERA ES: %d -->\r\n",flagwismo);
            Ql_Debug_Trace("<-- Jaime1.-->\r\n");
            flagwismo = 20;
             //g_cmd_idx++;
            Ril_Fin();
            break;      

        case (14):
          
            break; 
        case (20):
          
            break;                

       // default:
       //     Ql_Debug_Trace("<-- at commands finished.-->\r\n");
            
            // Start to fota upgrade
      
            //cmdstates=255;
            //g_cmd_idx = 0;
            //Ql_Timer_Stop(ATCmdtimerId);
       //     break;
        }
    
}


static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
     
}

char * CompletaBuffer (){

    int i = 0;
    while (i < 800) {
        bufferTxModulo[i] = 0x00; /*Se inicializa el Vector bufferTx1 como cadena vacía*/
        i++;
    }
    

    if(flagGpsValido = 1){

            Ql_sprintf(nmea,"\'latitud\':\'%s\',\'longitud\':\'%s\',\'velocidad\':\'%s\',"
                    "\'altitud\':\'%s\',\'curso\':\'%s\',\'fecha\':\'%s\',\'satelites\':\'%s\'", latitudgps, longitudgps, velocidad, altura, curso,fecha,numeroSatelites);

    }else{
            Ql_sprintf(nmea,"\'latitud\':\'0\',\'longitud\':\'0\',\'velocidad\':\'00\',"
                    "\'altitud\':\'0\',\'curso\':\'0\',\'fecha\':\'0\',\'satelites\':\'00\'");

    }



	Ql_sprintf(bufferTxModulo," {\'idMovil\':\'%s\',\'transaccion\':\'%s\',\'dirIpCliente\':\'%d.%d.%d.%d\',\'movImei\':\'%s\',\'version\':\'%s\',"
                "\'tipoCoordenada\':\'1\',\'kilometros\':\'%s\','origenSolicitud':'1', %s}|\x1A","555555","02",10,10,10,10,"32132132132131","V4-char","20", nmea);


      // Convertir cada char a minúscula
    // usando tolower
    //for (int indice = 0; bufferTxModulo[indice] != '\0'; ++indice){
    //    bufferTxModulo[indice] = tolower(bufferTxModulo[indice]);
    //}
   

    return bufferTxModulo;
}

 
/*******************************************************************************************************************/
/********************************       Comandos AT para conexion del Modulo      **********************************/
/*******************************************************************************************************************/


s32 Ril_tcp_ATE0()
{

    flagConectado = 1;
    s32 ret; 
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    Ql_sprintf((char *)sendbuffer, "ATE0");  
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 2;
        g_cmd_idx++;
    }
    return ret;
}

s32 Ril_tcp_CSQ()
{
    s32 ret; 
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    Ql_sprintf((char *)sendbuffer, "AT+CSQ");
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 3;
        g_cmd_idx++;
    }
    return ret;   
}

s32 Ril_tcp_QURCCFG()
{
    s32 ret; 
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    Ql_sprintf((char *)sendbuffer, "AT+QURCCFG=\"urcport\",\"uart1\"");
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 4;
        g_cmd_idx++;
    }
    return ret;
}

s32 Ril_tcp_QCFG()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPUSER=\"%s\"\n", FTP_USER_NAME);
    Ql_sprintf((char *)sendbuffer, "AT+QCFG=\"vbatt\",-1,3500,0");

    
    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 5;
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_CLCC()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+CLCC");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 6;
        g_cmd_idx++;
    }
    return ret;
}


s32 Ril_tcp_CGSN()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+CGSN");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 7;
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_QICSGP()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+QICSGP=1,1,\"internet.comcel.com.co\",\"\",\"\",1\n");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 8;
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_QIACT()
{


    /*
    s32 ret; 
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    Ql_sprintf((char *)sendbuffer, "AT+QIACT=1\n");
    //ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),ATSend_Handler, NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 9;
        g_cmd_idx++;
    }
    */


        s32 ret;
        ret = Ql_GPRS_Activate(PDP_CONTEXT_ID);
        if (ret == GPRS_PDP_SUCCESS)
        {
            flagwismo = 9;
            Ql_Debug_Trace("GPRS LISTO!\r\n\r\n");
        }else{
            Ql_Debug_Trace("<-- Fail to activate GPRS, ret=%d. \r\n\r\n", ret);
            flagwismo = 7;
            return;
        }


    return ret;

}

s32 Ril_tcp_ATV1()  // +QFTPGET;  handler by URC
{
    s32 ret; 

/*
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "ATV1\n");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 10;
        g_cmd_idx++;
    }

*/

        //ret = Ql_SOC_Register(&callback_soc_func, NULL);
        if (SOC_SUCCESS == ret)
        {
           flagwismo = 10;
        }else if (SOC_ALREADY == ret)
        {
            Ql_Debug_Trace("<--Socket callback function has already been registered,ret=%d.\r\n",ret);
           flagwismo = 10;
        }else
        {
            Ql_Debug_Trace("<--Register Socket callback function failure,ret=%d.\r\n",ret);
        }    
    return ret;
}

s32 Ril_tcp_QIACTPRE()
{
   

    /*
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+QIACT?\n");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        flagwismo = 11;
        g_cmd_idx++;
    }

    */


    m_socketid = Ql_SOC_Create(PDP_CONTEXT_ID, SOC_TYPE_TCP);
        if (m_socketid >= 0)
        {
            //flagwismo = 11;
        }else
        {
            Ql_Debug_Trace("<--Create socket id failure,error=%d.\r\n",m_socketid);
        }

    return m_socketid;

}

s32 Ril_tcp_QIOPE()
{
    
    s32 ret; 

    /*
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QIOPEN=1,0,\"TCP\",\"200.91.204.38\",11260,0,1");
    Ql_sprintf((char *)sendbuffer, "AT+QIOPEN=1,0,\"TCP\",\"%s\",\"%d\",0,1\n", "200.91.204.38", 11254);




    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),ATSend_Handler, NULL,0);
    //ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer), NULL, NULL, 0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    
    if(ret < 0)
    {
        Rester_Modulo();
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        //flagwismo = 12;
        g_cmd_idx++;
    }

    */



       
        ret = Ql_SOC_Connect(m_socketid,(u8 *) "200.91.204.38", 11260);
        if(ret == SOC_SUCCESS)
        {
            Ql_Debug_Trace("Socket OK\r\n");
            //flagwismo = 11;
        }else if(ret == SOC_WOULDBLOCK)
        {
            Ql_Debug_Trace("<--Waiting for the result of socket connection,ret=%d.\r\n",ret);
            //waiting CallBack_getipbyname

        }else //error
        {
            Ql_Debug_Trace("<--Socket Connect failure,ret=%d.\r\n",ret);
            Ql_Debug_Trace("<-- Close socket.\r\n");
            Ql_SOC_Close(m_socketid);
            m_socketid = -1;

            if(ret == SOC_BEARER_FAIL)
            {
                DesactiveGprs();
            }
            else
            {
                DesactiveGprs();
            }
        }


    return ret;

}


s32 GetLocalIp(){

        s32 ret; 
        Ql_memset(ip_addr, 0, 5);
        ret = Ql_GPRS_GetLocalIPAddress(PDP_CONTEXT_ID, (u32 *)ip_addr);
        if (ret == GPRS_PDP_SUCCESS)
        {
            Ql_Debug_Trace("Ip=%d.%d.%d.%d\r\n\r\n",ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3]);
            flagwismo = 12;
        }else
        {
            Ql_Debug_Trace("<--Get Local Ip failure,ret=%d.\r\n",ret);
        }

        return ret;
}


s32 DesactiveGprs(){

        Ql_Debug_Trace("<--Deactivate GPRS.\r\n");
        Ql_RIL_SendATCmd("AT+QIDEACT=1",Ql_strlen("AT+QIDEACT=1"),NULL,NULL,0);
        Ql_GPRS_Deactivate(PDP_CONTEXT_ID);
        flagwismo = 1;
}





s32 Ril_Tcp_QISEND(){


        

    
         //concat_trama();



        bufferTxModulo[0]=*strcat(bufferTxModulo,"JAIME");             
        bufferTxModulo[0]=*strcat(bufferTxModulo,"ANDRES");          //Colaca la Version de Software
        bufferTxModulo[0]=*strcat(bufferTxModulo,"LOMBANA");             //Colacar separador ","
        bufferTxModulo[0]=*strcat(bufferTxModulo,"GONZALEZ");         //Imei
        bufferTxModulo[0]=*strcat(bufferTxModulo,"VALERIE");             //Colacar separador "|"
        bufferTxModulo[0]=*strcat(bufferTxModulo,"JAIME\r\n");             //Colacar separador "|"
        
        //bufferTxModulo[0]=*strcat(bufferTxModulo," LOMBANA");             //Colacar separador "|"
        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0D");             
        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0A"); 

        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0D"); 
        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0A");             
        //bufferTxModulo[0]=*strcat(bufferTxModulo," \x1A");



         m_remain_len = Ql_strlen(bufferTxModulo);
         flagwismo = 14;

        do
        {
            s32 ret; 
            ret = Ql_SOC_Send(m_socketid,bufferTxModulo,m_remain_len);
            Ql_Debug_Trace("\r\nBytes enviados=%d\r\n",ret);
            //Ql_Debug_Trace("Socketid=%d,Bytes enviados=%d\r\n",m_socketid,ret);
            Ql_Debug_Trace("\r\nTrama:%s\r\n\r\n",bufferTxModulo);
            if(ret == m_remain_len)//send compelete
            {
                //m_tcp_state = STATE_SOC_ACK;
                Ql_Debug_Trace("\r\n<-- Debe enviar la servidor -->\r\n");
                break;
            }
            else if(ret < 0)
            {
                Ql_Debug_Trace("<--NO SE ENVIO AL SERVIDOR,ret=%d.\r\n",ret);
                Ql_Debug_Trace("<-- Close socket.\r\n");
                Ql_SOC_Close(m_socketid);//error , Ql_SOC_Close
                m_remain_len = 0; 
                DesactiveGprs();
                break;
            }
            else if(ret < m_remain_len)//continue send, do not send all data
            {
                Ql_Debug_Trace("\r\n<-- Continua -->\r\n");
                m_remain_len -= ret;
                break;
            }
        }while(1);
    

    /*

    s32 ret; 
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    
    Ql_sprintf((char *)sendbuffer, "AT+QISEND=0\n");

   
    

    //ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer), NULL, NULL, 5000);
    //ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),ATSend_Handler, NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {

        if(ret == -2){
            Ril_Fin();
        }else{
            Rester_Modulo();
            Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
        }
    }
    else
    {
        //flagwismo = 13;
        g_cmd_idx++;
    }
    return ret;

    */

}


s32 Ril_Fin(){
     Ql_Debug_Trace("<-- Jaime2.-->\r\n");

     Delay();
     Delay();
     Delay();
    
      do{

        Ql_Debug_Trace("<-- LA BANDERA ES: %d -->\r\n",flagwismo);
        s32 ret;
        CompletaBuffer();
       
        //Ql_Debug_Trace("<-- SE DEBE ENVIAR ESTO: %s -->\r\n",bufferTxModulo);
        //ret = Ql_RIL_SendATCmd(bufferTxModulo,Ql_strlen(bufferTxModulo),NULL, NULL,5000);
        //ret = Ql_RIL_SendATCmd(bufferTxModulo,Ql_strlen(bufferTxModulo),NULL, NULL,5000);


        Ql_Debug_Trace("<-- SE DEBE ENVIAR ESTO: %s -->\r\n","\x61\x62\x63\x64\x65\x66\x67\x68\x69\x70\x7C");
        ret = Ql_RIL_SendATCmd("\x61\x62\x63\x64\x65\x66\x67\x68\x69\x70\x7C",Ql_strlen("\x61\x62\x63\x64\x65\x66\x67\x68\x69\x70\x7C"),NULL, NULL,1000);

        if(ret < 0)
        {
            Ql_Debug_Trace("\r\n<-- send AT command failure jAIME-->\r\n");
            s32 ret2;
             
            Ql_Debug_Trace("<-- FINALIZA CADENA:  -->\r\n");
            ret2 = Ql_RIL_SendATCmd("\x1A",Ql_strlen("\x1A"),NULL, NULL,5000);

                if(ret < 0)
                {
                    Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
                }
                else
                {

                     Ql_Debug_Trace("\r\n<-- Debio Enviar -->\r\n");
                    g_cmd_idx++;
                    flagwismo = 14;
                    return ret;
                }
            
        }
        else
        {


            Delay();
            Delay();
            Delay();
             Delay();
            Delay();
            Delay();
             Delay();
            Delay();
            Delay();
             Delay();
            Delay();
            Delay();
            
            s32 ret2;
             
            Ql_Debug_Trace("<-- FINALIZA CADENA:  -->\r\n");
            ret2 = Ql_RIL_SendATCmd("\x1A",Ql_strlen("\x1A"),NULL, NULL,5000);

                if(ret < 0)
                {
                    Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
                }
                else
                {

                     Ql_Debug_Trace("\r\n<-- Debio Enviar -->\r\n");
                    g_cmd_idx++;
                    flagwismo = 14;
                    return ret;
                }
            
           

         }

    }while(1);

}


/********************************************************************************************************************/
/*****************************************************  Callbacks****************************************************/
/********************************************************************************************************************/


/********************************************************************************************************************/
/********************************************************************************************************************/

s32 Rester_Modulo(){
    flagConectado = 0;
    flagwismo = 1;
    Ql_Reset(0);
}
/*******************************************************************************************************************/
/*******************************************************************************************************************/
static s32 ATSend_Handler(char* line, u32 len, void* userData)
{
    Ql_UART_Write(m_myUartPort, (u8*)line, len);
 
    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        if(flagwismo != 11 ){
            Ql_Debug_Trace("todo ok\r\n");
            return  RIL_ATRSP_SUCCESS;
        }
    }
    else if (Ql_RIL_FindLine(line, len, "ERROR"))
    {  
        if(flagwismo == 8){
            Ql_Debug_Trace("ERROR, Revise SIM o datos...\r\n");
            Rester_Modulo();
        }else{
            Ql_Debug_Trace("ERROR\r\n");
        }
        
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CMS ERROR:"))
    {
        return  RIL_ATRSP_FAILED;
    }

    else if (Ql_RIL_FindString(line, len, "+QIOPEN: 0,0")|| Ql_RIL_FindString(line, len, "+QIOPEN: 0,562"))
    
    {
        flagConectado = 1;
        flagwismo = 12;
        return  RIL_ATRSP_SUCCESS;
    }

    else if (Ql_RIL_FindString(line, len, ">"))
    {
        //flagwismo = 13;
        Ril_Fin();
        //Ql_Debug_Trace("\r\n<-- Se abre > -->\r\n");
        return  RIL_ATRSP_SUCCESS;
    }

    else if (Ql_RIL_FindString(line, len, "SEND OK"))
    {
        Ql_Debug_Trace("\r\n<-- Deberia salir de enviar -->\r\n");
        flagwismo = 14;
        return  RIL_ATRSP_SUCCESS;
    }

    else if (Ql_RIL_FindString(line, len, "Y"))
    {
        Ql_Debug_Trace("\r\n<-- LLEGA KEEP ALIVE -->\r\n");
       
        return  RIL_ATRSP_SUCCESS;
    }
   

    return RIL_ATRSP_CONTINUE; //continue wait
}
/****************************************************************************************************************************/
/****************************************************************************************************************************/
void EnviaTrama(void){
    Ril_Tcp_QISEND();
}

/****************************************************************************************************************************/
/****************************************************************************************************************************/
void Delay (void){
	int i;
	for (i = 0 ; i < 777777 ; i++){

	}
}