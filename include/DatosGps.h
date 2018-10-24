/*****************************************************************************
 *
 * Filename:
 * ---------
 *   DatosGps.h 
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
#ifndef __DATOSGPS_H__
#define __DATOSGPS_H__

//Declara callback , funciones que seran invocadas
void init_Gps(void);
void Gps_Callback_Timer(u32 timerId, void* param);
static s32 ATResponse_GPS_Handler(char* line, u32 len, void* param);
void SeparaTramaGps(void);
void changueSpeedKm (void);
void TomaDatosActuales(void);
static s32 DtsActuales_Handler(char* line, u32 len, void* userData);

//Definicion de varibles

static char sendbuffergps[100];
static s32 GpstimerId=0x102;
static char sg_aDbgBuf[100];
char loc[256];
char datosGps[150];

char horaGreenwich[];
char latitud[];
char longitud[];
char altura[];
char indaltura[];
char curso[];
char velocidad[];
char fecha[];
char numeroSatelites[];
char fixQuality[];
unsigned char velocidadkm[6];
unsigned int  veloKmh;
unsigned int flagGpsValido;




    



#endif  // __DATOSGPS_H__