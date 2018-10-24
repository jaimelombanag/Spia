/*****************************************************************************
 *
 * Filename:
 * ---------
 *   SpiaV4.h
 *
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   archivo.h
 *
 * Usage:
 * ------
 *   Compile & Run:
 *
 *     Set "C_PREDEF=-D __V4__" in gcc_makefile file. And compile the 
 *     app using "make clean/new".
 *     Download image bin to module to run.
 *
 * Author: MarcelaD
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * Prueba GPIO   OK
 * Prueba GPRS   
 ****************************************************************************/
#ifndef __SpiaV4_H__
#define __SpiaV4_H__


#define LENGTH 300 // buffer lectura filesystem
   

// Define a timer and the handler
extern u32 timerId;
extern void* param;
//void Callback_OnTimerp(void);


int contadorTodoOk;
int flagLed;
void ProcessEstadoLed(void);

//funciones
void Init_variables(void);
void init_Reset(void);
void start_timerReset(void);
static void Timer_handler_Reset(u32 timerId, void* param);
static void Callback_Timer_ModuloC(u32 timerId, void* param);
void start_estado_cargador(void);//estado chg/pgd
void Callback_CHGPGD(void);
void init_gpio(void);
void Callback_OnTimer(void); //blink
void Callback_OnTimer_Jaime(void); //blink
void start_gps_timer(void);


// Estructura estados del cargador
typedef union{
	struct{
		unsigned PGD:1;
		unsigned CHG:1;
	};
	unsigned char All;	
}AppInputs_t;
AppInputs_t Inputs;

#endif  // _SpiaV4_H__