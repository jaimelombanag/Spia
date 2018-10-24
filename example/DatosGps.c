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
    Ql_Timer_Start(GpstimerId, 10000, TRUE);                                         //Timer para la Rx DEL gps  1.5 segundos

     init_TcpIp();
	
}

void Gps_Callback_Timer(u32 timerId, void* param){
    if(GpstimerId == timerId){
    	
        s32 ret;
		ret = GetGPSLOC(loc);
		if (ret == RIL_AT_SUCCESS ) {


			Ql_strcpy(datosGps, loc + 12);
			//Ql_Debug_Trace("Datos del GPS: %s\r\n", datosGps);


			Ql_Debug_Trace("\r\n<-- flagConectado ...%d\r\n <-- flagGpsValido ...-->%d\r\n", flagConectado, flagGpsValido);

			flagGpsValido = 1;
			
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
				case 1:
					strcpy(horaGreenwich, pch);
					break;
				case 2:
					strcpy(latitud, pch);
					break;
				case 3:
					strcpy(longitud, pch);
					break;
				case 4:
					//strcpy(longitud, pch);
					break;
				case 5:
					strcpy(altura, pch);
					break;
				case 6:
					//strcpy(indaltura, pch);
					break;
				case 7:
					strcpy(curso, pch);
					break;
				case 8:
					strcpy(velocidad, pch);
					break;
				case 9:
					//strcpy(indaltura, pch);
					break;
				case 10:
					strcpy(fecha, pch);
					break;
				case 11:
					strcpy(numeroSatelites, pch);
					break;	
				}
		
		//Ql_Debug_Trace("%s\n",pch);// Aqui deberias guardar tu dato en el array!
		pch = strtok(NULL, ",");  // Aca tambien iria solo la coma.!!
		v++;
	}
	for (int r = 0; r < 150; r++) {
		datosGps[r] = 0x00;
	}
	changueSpeedKm();

}



void changueSpeedKm (void){
	  unsigned char j;
	  int copyVel;
	  velocidadkm[0]=0x30;    //Inicializa Velocidad en 000.00
	  velocidadkm[1]=0x30;
	  velocidadkm[2]=0x30;
	  velocidadkm[3]='.';
	  velocidadkm[4]=0x30;
	  velocidadkm[5]=0x30;
	  velocidadkm[6]=0x00;

	  veloKmh=0;          //Velocidad en Km/h

	  j=0;
	  while((j<6)&&(velocidad[j]!='.')){  //Contar bytes antes del (.)
	    j++;
	  }

	  velocidad[0]-=0x30; //Pasar de ASCII a decimal
	  velocidad[1]-=0x30;
	  velocidad[2]-=0x30;
	  velocidad[3]-=0x30;
	  velocidad[4]-=0x30;
	  velocidad[5]-=0x30;
	  switch(j){   //Ver cuantas cifras tiene la velocidad....
	    case(1):  //La velocidad esta entre 0.00 y 9.99 Knots -> VelMax:1850*9.99 = 18481 -> 18.48 Km/h
	        if(velocidad[0]>0){ //La velocidad es Mayor de cero
	          veloKmh+=(1852*velocidad[0]);     //1ra Cifra antes de la coma
	          veloKmh+=(185*velocidad[2]);      //1ra Cifra despues de la coma(se deberia * por 185.2)
	          veloKmh+=(18*velocidad[3]);       //2da Cifra despues de la coma(se deberia * por 18.52)
	          copyVel=veloKmh/1000;             //Valor de Velocidad

	          if(veloKmh > 9999){          //Velocidad Mayor a 10Km/h
	        	  velocidadkm[1]=0x31;
	            veloKmh-=10000;
	            while(veloKmh > 999){
	              veloKmh-=1000;
	              velocidadkm[2]+=1;
	            }
	            while(veloKmh > 99){
	              veloKmh-=100;
	              velocidadkm[4]+=1;
	            }
	            while(veloKmh > 9){
	              veloKmh-=10;
	              velocidadkm[5]+=1;
	            }
	          }

	          else{                         //Velocidad Menor a 10Km/h
	            while(veloKmh > 999){
	              veloKmh-=1000;
	              velocidadkm[2]+=1;
	            }
	            while(veloKmh > 99){
	              veloKmh-=100;
	              velocidadkm[4]+=1;
	            }
	            while(veloKmh > 9){
	              veloKmh-=10;
	              velocidadkm[5]+=1;
	            }
	          }

	          veloKmh= copyVel;   //Recuperar Valor de la Velocidad...
	        }
	    break;

	    case(2):  //La velocidad esta entre 10.00 y 99.99 Knots
	      veloKmh+=(1852*velocidad[0]);     //1ra Cifra antes de la coma
	      veloKmh+=(185*velocidad[1]);      //2da Cifra antes de la coma(se deberia * por 185.2)
	      veloKmh+=(18*velocidad[3]);       //1ra Cifra despues de la coma(se deberia * por 18.52)
	      veloKmh+=(2*velocidad[4]);        //2da Cifra despues de la coma(se deberia * por 1.8 se aproxima a 2)
	      copyVel=veloKmh/100;              //Valor de Velocidad
	          if(veloKmh > 10000){          //Velocidad Mayor a 100Km/h
	        	  velocidadkm[0]=0x31;
	            veloKmh-=10000;
	            while(veloKmh > 999){
	              veloKmh-=1000;
	              velocidadkm[1]+=1;
	            }
	            while(veloKmh > 99){
	              veloKmh-=100;
	              velocidadkm[2]+=1;
	            }
	            while(veloKmh > 9){
	              veloKmh-=10;
	              velocidadkm[4]+=1;
	            }
	            while(veloKmh > 0){
	              veloKmh-=1;
	              velocidadkm[5]+=1;
	            }
	          }

	          else{                         //Velocidad Menor a 100Km/h
	            while(veloKmh > 999){
	              veloKmh-=1000;
	              velocidadkm[1]+=1;
	            }
	            while(veloKmh > 99){
	              veloKmh-=100;
	              velocidadkm[2]+=1;
	            }
	            while(veloKmh > 9){
	              veloKmh-=10;
	              velocidadkm[4]+=1;
	            }
	            while(veloKmh > 0){
	              veloKmh-=1;
	              velocidadkm[5]+=1;
	            }
	          }
	          veloKmh= copyVel;   //Recuperar Valor de la Velocidad...
	    break;
	    default:  //La velocidad es muy alta es mayor a 180 Km/h
	    break;
	  }

	  //Ql_Debug_Trace("La velocidad es: %s\n",velocidadkm);
	 
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
    	Ql_Debug_Trace("Lo que responde %s\r\n", line);
	}

    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        Ql_Debug_Trace("todo ok \r\n");
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
