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
#include "TcpIp.h"


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

    Ql_Timer_Register(ATCmdtimerId, SendAtCmd_Callback_Timer, NULL);                        //Timer para la Tx de los comandos AT 
    Ql_Timer_Start(ATCmdtimerId, 1500, TRUE);                                               //Timer para la Tx de los comandos AT 1.5 segundos

}

void SendAtCmd_Callback_Timer(u32 timerId, void* param){
    if(ATCmdtimerId == timerId){
        SendAtCmd();
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
            Ril_Tcp_QISEND();
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
            Ql_Timer_Stop(ATCmdtimerId);
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


static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
     
}

char * CompletaBuffer (){
    char buffer[800];

    Ql_sprintf(nmea,"\'latitud\':\'0\',\'longitud\':\'0\',\'velocidad\':\'\',"
                    "\'altitud\':\'\',\'curso\':\'\',\'fecha\':\'\',\'satelites\':\'\'");

    //Ql_sprintf(bufferTxModulo," {\'idMovil\':\'%s\',\'transaccion\':\'%s\',\'dirIpCliente\':\'%d.%d.%d.%d\',\'movImei\':\'%s\',\'version\':\'%s\',"
    //            "\'tipoCoordenada\':\'0\',\'kilometros\':\'%s\','origenSolicitud':'1',%s}\x0A\x0D\x1A","555555","02",10,10,10,10,"32132132132131ei","3.0","20",nmea);

	Ql_sprintf(bufferTxModulo," {\'idMovil\':\'%s\',\'transaccion\':\'%s\',\'dirIpCliente\':\'%d.%d.%d.%d\',\'movImei\':\'%s\',\'version\':\'%s\',"
                "\'tipoCoordenada\':\'1\',\'kilometros\':\'%s\','origenSolicitud':'1', %s}|\x1A","555555","02",10,10,10,10,"32132132132131","3.0","20", nmea);


	//Ql_sprintf(bufferTxModulo,"test\n");



    return bufferTxModulo;
}


static s32 ATSend_Handler(char* line, u32 len, void* userData)
{
    Ql_UART_Write(m_myUartPort, (u8*)line, len);
 
    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        Ql_Debug_Trace("todo ok\r\n");
        return  RIL_ATRSP_SUCCESS;
    }
    else if (Ql_RIL_FindLine(line, len, "ERROR"))
    {  
        Ql_Debug_Trace("ERROR\r\n");
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

    else if (Ql_RIL_FindString(line, len, ">"))
    {



       
        Ql_Debug_Trace("\r\n<-- Se actiava el Desbloqueo -->\r\n");
    
        return  RIL_ATRSP_SUCCESS;
    }
   

    return RIL_ATRSP_CONTINUE; //continue wait
}

/*******************************************************************************************************************/
/********************************       Comandos AT para conexion del Modulo      **********************************/
/*******************************************************************************************************************/


s32 Ril_tcp_ATE0()
{
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
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
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
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
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
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
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
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
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
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_QICSGP()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+QICSGP=1,1,\"internet.comcel.com.co\",\"\",\"\",1");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_QIACT()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+QIACT=1");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_ATV1()  // +QFTPGET;  handler by URC
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "ATV1");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;
}

s32 Ril_tcp_QIACTPRE()
{
    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    //Ql_sprintf((char *)sendbuffer, "AT+QFTPPASS=\"%s\"\n", FTP_PASSWORD);
    Ql_sprintf((char *)sendbuffer, "AT+QIACT?");

    //Ql_Debug_Trace("%s\r\n",sendbuffer);
    
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_tcp_QIOPE()
{
    

	flagConectado = 1;

    s32 ret; 

    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    Ql_sprintf((char *)sendbuffer, "AT+QIOPEN=1,0,\"TCP\",\"200.91.204.38\",11253,0,1");
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer), NULL, NULL, 0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;

}

s32 Ril_Tcp_QISEND(){

    s32 ret; 
    Ql_memset(sendbuffer, 0, sizeof(sendbuffer));
    
    Ql_sprintf((char *)sendbuffer, "AT+QISEND=0");
    //ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer), NULL, NULL, 0);
    ret = Ql_RIL_SendATCmd(sendbuffer,Ql_strlen(sendbuffer),ATSend_Handler, NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffer, ret);
    if(ret < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }
    else
    {
        g_cmd_idx++;
    }
    return ret;

}


s32 Ril_Fin(){
     Ql_Debug_Trace("<-- Jaime2.-->\r\n");
    
     // do{

        //int i = 0;
        //while (i < 800) {
        //    bufferTxModulo[i] = 0x00; /*Se inicializa el Vector bufferTx1 como cadena vacía*/
        //    i++;
        //}
        
        bufferTxModulo[0]=*strcat(bufferTxModulo,"JAIME");             
        bufferTxModulo[0]=*strcat(bufferTxModulo,"ANDRES");          //Colaca la Version de Software
        bufferTxModulo[0]=*strcat(bufferTxModulo,"LOMBANA");             //Colacar separador ","
        bufferTxModulo[0]=*strcat(bufferTxModulo,"GONZALEZ");         //Imei
        bufferTxModulo[0]=*strcat(bufferTxModulo,"VALERIE");             //Colacar separador "|"
        bufferTxModulo[0]=*strcat(bufferTxModulo,"JAIME|");             //Colacar separador "|"
        
        //bufferTxModulo[0]=*strcat(bufferTxModulo," LOMBANA");             //Colacar separador "|"
        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0D");             
        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0A"); 

		//bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0D"); 
        //bufferTxModulo[0]=*strcat(bufferTxModulo,"\x0A");             
        bufferTxModulo[0]=*strcat(bufferTxModulo," \x1A");


        s32 ret;
        //char *sendbuffer = CompletaBuffer();
       
        Ql_Debug_Trace("<-- SE DEBE ENVIAR ESTO: %s -->\r\n",bufferTxModulo);

        ret = Ql_RIL_SendATCmd(bufferTxModulo,Ql_strlen(bufferTxModulo),NULL, NULL,0);
        if(ret < 0)
        {
            Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
        }
        else
        {



                /*Delay();
                Delay();
                Delay();
                Ql_Debug_Trace("<-- Pasa el Delay.-->\r\n");
                //ret = Ql_RIL_SendATCmd(bufferTxModulo,Ql_strlen(bufferTxModulo),NULL, NULL,0);
                ret = Ql_RIL_SendATCmd("\x1A",Ql_strlen("\x1A"),NULL, NULL,0);
                if(ret < 0)
                {
                    Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
                }
                else
                     Ql_Debug_Trace("\r\n<-- Deberia salir de enviar -->\r\n");
                    g_cmd_idx++;
                {



                    

               
            }
            */
                    Ql_Debug_Trace("\r\n<-- Deberia salir de enviar -->\r\n");
                    g_cmd_idx++;

            return ret;

         }

   // }while(1);


}
/*******************************************************************************************************************/
/*******************************************************************************************************************/
/****************************************************************************************************************************/
/****************************************************************************************************************************/
void Delay (void){
	int i;
	for (i = 0 ; i < 777777 ; i++){

	}
}