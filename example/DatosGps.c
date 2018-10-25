#include <stdio.h>
#include <string.h>
#include "ql_trace.h"     //envio de informacion de forma serial 
#include "ql_stdlib.h"    //envio de informacion de forma serial 
#include "ql_type.h"
#include "ql_uart.h"
#include "ril_util.h"
#include "ril.h"  
#include "ql_timer.h"     // control timer
#include "ql_fs.h"// file system
#include "custom_feature_def.h" //OPen CPU RIL

#include "DatosGps.h"
#include "TcpIp.h"



void init_Gps(void) {   // inicializacion del gps

	s32 gpsResult1;
	Ql_memset(sendbuffergps, 0, sizeof(sendbuffergps));
    Ql_sprintf((char *)sendbuffergps, "AT+QGPS=1");  
	gpsResult1 = Ql_RIL_SendATCmd(sendbuffergps,Ql_strlen(sendbuffergps),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffergps, gpsResult1);
    if(gpsResult1 < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }else {
		Ql_Debug_Trace("GPS Activado\r\n");
	}
   


	/*+++++++++++++++++++++++++Comandos para la señal del GPS++++++++++++++++++++++++++++++++++++*/

	Ql_memset(sendbuffergps, 0, sizeof(sendbuffergps));
    Ql_sprintf((char *)sendbuffergps, "AT+QGPSCFG=\"glonassenable\",1");  
	gpsResult1 = Ql_RIL_SendATCmd(sendbuffergps,Ql_strlen(sendbuffergps),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffergps, gpsResult1);
    if(gpsResult1 < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }else {
		Ql_Debug_Trace("GPS Activado\r\n");
	}


	Ql_memset(sendbuffergps, 0, sizeof(sendbuffergps));
    Ql_sprintf((char *)sendbuffergps, "AT+QGPSCFG=\"autogps\",1");  
	gpsResult1 = Ql_RIL_SendATCmd(sendbuffergps,Ql_strlen(sendbuffergps),NULL,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffergps, gpsResult1);
    if(gpsResult1 < 0)
    {
        Ql_Debug_Trace("\r\n<-- send AT command failure -->\r\n");
    }else {
		Ql_Debug_Trace("GPS Activado\r\n");
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	//register a timer (GPS) save
	Ql_Timer_Register(GpstimerId, Gps_Callback_Timer, NULL);                        //Timer para la Rx DEL gps 
    Ql_Timer_Start(GpstimerId, 20000, TRUE);                                         //Timer para la Rx DEL gps  1.5 segundos

     //init_TcpIp();
	
}

void Gps_Callback_Timer(u32 timerId, void* param){
    if(GpstimerId == timerId){
    	
        s32 ret;
		ret = GetGPSLOC(loc);
		if (ret == RIL_AT_SUCCESS ) {
			flagGpsValido = 1;
			Ql_strcpy(datosGps, loc + 12);
			//Ql_Debug_Trace("Datos del GPS: %s\r\n", datosGps);
			//Ql_Debug_Trace("\r\n<-- flagConectado ...%d\r\n <-- flagGpsValido ...-->%d\r\n", flagConectado, flagGpsValido);
			SeparaTramaGps();
			
		}else{
			flagGpsValido = 0;
		}
    }
}

s32 GetGPSLOC(char* gpsLOC) {

	//return Ql_RIL_SendATCmd("AT+QGPSLOC=2", Ql_strlen("AT+QGPSLOC=2"),ATResponse_GPS_Handler, gpsLOC, 0);
	return Ql_RIL_SendATCmd("AT+QGPSLOC?", Ql_strlen("AT+QGPSLOC?"),ATResponse_GPS_Handler, gpsLOC, 0);
}

static s32 ATResponse_GPS_Handler(char* line, u32 len, void* param) {
	char* p1 = NULL;
	//Ql_Debug_Trace("entra al handler\r\n");
	p1 = Ql_RIL_FindString(line, len, "OK");
	if (p1) {
		//Ql_Debug_Trace("encontro ok\r\n");
		return RIL_ATRSP_SUCCESS;
	}
	p1 = Ql_RIL_FindLine(line, len, "ERROR");// find <CR><LF>ERROR<CR><LF>, <CR>ERROR<CR>Â£Â¬<LF>ERROR<LF>
	if (p1) {
		return RIL_ATRSP_FAILED;
	}
	/********************************************************/
	p1 = Ql_RIL_FindLine(line, len, "+CME ERROR: 516");
	if (p1) {
		//Ql_Debug_Trace("GPS no fijado\r\n");
		return RIL_ATRSP_FAILED;
	}
	p1 = Ql_RIL_FindLine(line, len, "+QIURC: \"recv\",0");
	if (p1) {
		return RIL_ATRSP_FAILED;
	}
	Ql_memcpy((char*) param, line, len - 2);
	//return RIL_ATRSP_CONTINUE; //continue wait	
}


void SeparaTramaGps(void){
	//SendStringConfigurador((unsigned char*) "GGA: \r\n",&deviceDatac);
	//SendStringConfigurador((unsigned char*) TramaSaveGGA,&deviceDatac);
	//SendStringConfigurador((unsigned char*) "\r\n",&deviceDatac);

	unsigned int v;
	char * pch;
	// printf ("Splitting string \"%s\" into tokens:\n",str);
	//SendStringConfig((unsigned char*) "Esta es la Trama RMC:\n", &deviceDatac);
	//SendStringConfig((unsigned char*) RMC, &deviceDatac);
	pch = strtok(datosGps, ",");   // Aqui deja solo la coma
	v = 0;
	while (pch != NULL) {
		switch (v) {
				case 0:
					Ql_sprintf(horaGreenwich, pch);
					break;
				case 1:
					Ql_sprintf(latitudgps, pch);
					break;
				case 2:
					Ql_sprintf(longitudgps, pch);
					break;
				case 3:
					//Ql_sprintf(longitud, pch);
					break;
				case 4:
					Ql_sprintf(altura, pch);
					break;
				case 5:
					//Ql_sprintf(indaltura, pch);
					break;
				case 6:
					Ql_sprintf(curso, pch);
					break;
				case 7:
					Ql_sprintf(velocidad, pch);
					break;
				case 8:
					//strcpy(indaltura, pch);
					break;
				case 9:
					Ql_sprintf(fecha, pch);
					break;
				case 10:
					Ql_sprintf(numeroSatelites, pch);
					break;	
				}
		
		//Ql_Debug_Trace("%s\n",pch);// Aqui deberias guardar tu dato en el array!
		pch = strtok(NULL, ",");  // Aca tambien iria solo la coma.!!
		v++;
	}
	for (int r = 0; r < 150; r++) {
		datosGps[r] = 0x00;
	}
}

void SeparaTramaGps2(void){
	
	for (int r = 0; r < 10; r++) {
		
		 curso[r] = 0x00;
		 latitudgps[r] = 0x00;
		 longitudgps[r] = 0x00;
		 altura[r] = 0x00;
		 velocidad[r] = 0x00;
		 fecha[r] = 0x00;
		 numeroSatelites[r] = 0x00;
		
	}


	unsigned int v;
	char * pch;
	// printf ("Splitting string \"%s\" into tokens:\n",str);
	//SendStringConfig((unsigned char*) "Esta es la Trama RMC:\n", &deviceDatac);
	//SendStringConfig((unsigned char*) RMC, &deviceDatac);
	pch = strtok(datosGpsActual, ",");   // Aqui deja solo la coma
	v = 0;
	while (pch != NULL) {
		switch (v) {
				case 0:
					//strcpy(horaGreenwich, pch);
					Ql_sprintf(horaGreenwich,pch);
					break;
				case 1:
					//strcpy(latitud, pch);
					Ql_sprintf(latitudgps,pch);
					break;
				case 2:
					//strcpy(longitud, pch);
					Ql_sprintf(longitudgps,pch);
					break;
				case 3:
					//strcpy(longitud, pch);
					break;
				case 4:
					//strcpy(altura, pch);
					Ql_sprintf(altura,pch);
					break;
				case 5:
					//strcpy(indaltura, pch);
					break;
				case 6:
					//strcpy(curso, pch);
					Ql_sprintf(curso,pch);
					break;
				case 7:
					Ql_sprintf(velocidad, pch);
					break;
				case 8:
					//strcpy(indaltura, pch);
					break;
				case 9:
					Ql_sprintf(fecha, pch);
					break;
				case 10:
					Ql_sprintf(numeroSatelites, pch);
					break;	
				}
		
		//Ql_Debug_Trace("%s\n",pch);// Aqui deberias guardar tu dato en el array!
		pch = strtok(NULL, ",");  // Aca tambien iria solo la coma.!!
		v++;
	}
	for (int r = 0; r < 150; r++) {
		datosGpsActual[r] = 0x00;
	}
	
	/*
	Ql_Debug_Trace("============================================%s\n",latitudgps);
	Ql_Debug_Trace("============================================%s\n",longitudgps);
	Ql_Debug_Trace("============================================%s\n",velocidad);
	Ql_Debug_Trace("============================================%s\n",altura);
	Ql_Debug_Trace("============================================%s\n",curso);
	Ql_Debug_Trace("============================================%s\n",fecha);
	Ql_Debug_Trace("============================================%s\n",numeroSatelites);
	*/

}


void TomaDatosActuales(void){
	Datos_Actuales_Gps();
}


s32 Datos_Actuales_Gps()
{
    s32 ret; 
    Ql_memset(sendbuffergps, 0, sizeof(sendbuffergps));
    Ql_sprintf((char *)sendbuffergps, "AT+QGPSLOC=2");  
    
    ret = Ql_RIL_SendATCmd(sendbuffergps,Ql_strlen(sendbuffergps),DtsActuales_Handler,NULL,0);
    Ql_Debug_Trace("<-- Send AT:%s, ret = %d -->\r\n",sendbuffergps, ret);
    if(ret < 0)
    {
    	//Se debe tomar los datos guardados
    }
    
    return ret;
}


static s32 DtsActuales_Handler(char* line, u32 len, void* userData)
{
    Ql_UART_Write(m_myUartPort, (u8*)line, len);
 

	size_t Size = strlen(line);
    if(Size > 20){
    	//Ql_Debug_Trace("Lo que responde %s\r\n", line);
    	Ql_strcpy(datosGpsActual, line + 12);
		SeparaTramaGps2();
	}

    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        //Ql_Debug_Trace("todo ok \r\n");
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

    return RIL_ATRSP_CONTINUE; //continue wait
}
