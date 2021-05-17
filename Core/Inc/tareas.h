/*
*********************************************************************************************************
*                                            tareas.h
*			Este archivo contiene todas las tareas y todas las funciones que controlan
*			El elevador. El sistema operativo las administra y las ejecuta intercaladamente
*
*********************************************************************************************************
*/

#ifndef INC_TAREAS_H_
#define INC_TAREAS_H_



#endif /* INC_TAREAS_H_ */


#include "stm32f1xx_hal.h"
#include <os.h>
#include "main.h"
#include <app_cfg.h>
#include <cpu_core.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "LiquidCrystal.h"
#include "calibracion.h"			//En calibración se acomodan las constantes del sistema


/***************************************************************
 * 						VARIABLES
 **************************************************************/
extern TIM_HandleTypeDef htim4;		//Este timer es el que le envía PWM 50% al motor
bool start = false;					//La variable start es el pulsador de marcha o RUN
bool halt = true;					//Esta variable detiene inmediatamente el motor
bool giroHorario = false;
bool giroAntiHorario = false;
bool errorTimeoutSup = false;
bool errorTimeoutInf = false;
bool rejillasReady = false;
uint8_t rejillasActuales =0;
uint8_t nRejillas = 5;				//Cantidad de rejillas default para un ciclo de elevador
uint16_t distancia_mm = 3000;		//Distancia linear default que debe recorrer el elevador

uint16_t velMAX_RPM = 500;			//Estas variables todavía no se utilizan
uint16_t velMIN_RPM = 50;			//Estas variables todavía no se utilizan

typedef enum
{
	ModoAut,
	ModoMan,
	CantRejillas,
	Desplazamiento,
	VelMax,
	VelMin,
	count
}menu;				//Los estados que tiene el menú del LCD

menu menuState;		//Los estados que tiene el menú del LCD



/***************************************************************
 *							VARIABLES
 **************************************************************/

/***************************************************************
 *							Prototipo funciones
 **************************************************************/
void GiroHorario();
void GiroAntiHorario();
void CicloAutomatico();		//Esta funcion controla el modo automático del elevador
/***************************************************************
 *							Prototipo funciones
 **************************************************************/


/*
*********************************************************************************************************
*                                        TAREA 1:  LecturaPulsadores()
*
* Description : Esta tarea lee los pulsadores. Los pines se activan con estado LOW
* 				Cada vez que se presione un pulsador que cambie alguna variable, se
* 				resume la tarea del display la cual actualiza lo que muestra
*
* Argument(s) : p_arg       Argument passed to ' LecturaPulsadores()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void LecturaPulsadores (void *p_arg)
{
	menuState=ModoAut;								//Con este estado arranca el programa
	while(DEF_TRUE)
	{
		OSTimeDlyHMSM(0, 0, 0, 25u); 				//Tiempo de lectura: 100ms

		/***************************PULSADOR MENU **************************************/
		if(HAL_GPIO_ReadPin(GPIOB, PULSADOR1_Pin) == GPIO_PIN_RESET && !start)
		{
			menuState++;
			if (menuState == count)
			{
				menuState = ModoAut;				//El menú es circular
			}
			OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display
			OSTimeDlyHMSM(0, 0, 0, 400u);			//Delay luego de pulsarlo

		}
		/***************************PULSADOR MENU **************************************/

		/***************************PULSADOR ARRIBA ************************************/
		if(HAL_GPIO_ReadPin(GPIOB, PULSADOR2_Pin) == GPIO_PIN_RESET)
		{
			switch(menuState)
					  	{
					  	case ModoAut:				//Si display muestra modo automatico:
					  		break;

					  	case ModoMan:
					  		if (!HAL_GPIO_ReadPin(GPIOB, Fc_Sup_Pin)) // subo solo si no toco FC
					  		{
					  			giroHorario = true;
					  			giroAntiHorario = false;
					  		}
					  		else
					  		{
					  			giroHorario = false;
					  			giroAntiHorario = false;
					  		}
					  		break;


					  	case CantRejillas:			//Si display muestra cant de rejillas:

					  		if (nRejillas < 255)
					  		{
					  			nRejillas ++;
					  		}
					  		break;

					  	case Desplazamiento:		//Si display muestra desplazamiento:
					  		if (distancia_mm < 65535)
					  		{
					  			distancia_mm ++;
					  		}
					  		break;

					  	case VelMax:				//Si display muestra velocidad maxima:
					  		if (velMAX_RPM < 65535)
					  		{
					  			velMAX_RPM++;
					  		}
					  		break;

					  	case VelMin:				//Si display muestra velocidad minima:
					  		if (velMIN_RPM < 65535)
					  		{
					  			velMIN_RPM++;
					  		}
					  		break;

					  	case count:					//Este estado no debería darse
					  		break;

					  	}
			if( menuState != ModoMan && menuState != ModoAut )
				{
				OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display
				}

			OSTimeDlyHMSM(0, 0, 0, 100u);				//Delay luego de pulsarlo

		}	//End if pulsador arriba
		/***************************PULSADOR ARRIBA ************************************/

		/***************************PULSADOR ABAJO ************************************/
		else
		{

			if(HAL_GPIO_ReadPin(GPIOB, PULSADOR3_Pin) == GPIO_PIN_RESET)
			{
				switch(menuState)
							{
							case ModoAut:			//Si display muestra modo automatico:
								break;

							case ModoMan:			//Si display muestra modo manual:
								if (!HAL_GPIO_ReadPin(GPIOB, Fc_Inf_Pin))
								{						//Bajo solo si no toco el FC
									giroHorario = false;
									giroAntiHorario = true;
								}
								else
								{
									giroHorario = false;
									giroAntiHorario = false;
								}
								break;


							case CantRejillas:		//Si display muestra cantidad de rejillas:
								if (nRejillas > 0)
								{
									nRejillas --;
								}
								break;

							case Desplazamiento:	//Si display muestra desplazameinto:
								if (distancia_mm > 0)
								{
									distancia_mm --;
								}
								break;

							case VelMax:			//Si display muestra velocidad maxima:
								if (velMAX_RPM > 0)
								{
									velMAX_RPM--;
								}
								break;

							case VelMin:			//Si display muestra velocidad minima:
								if (velMIN_RPM > 0)
								{
									velMIN_RPM--;
								}
								break;

							case count:			//Este estado no deberia darse:
								break;

							}

				if( menuState != ModoMan && menuState != ModoAut )
				{
				OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display
				}
				OSTimeDlyHMSM(0, 0, 0, 100u);			//Delay luego de pulsarlo

			}
			else //Si modo manual y no se está pulsando ni arriba ni abajo
			{
				if(menuState == ModoMan)
				{
					giroHorario = false;
					giroAntiHorario = false;
				}
			}

		}//End else if pulsador abajo


		/***************************PULSADOR ABAJO ************************************/

		/***************************PULSADOR START/STOP *******************************/
		if(HAL_GPIO_ReadPin(GPIOB, PULSADOR4_Pin) == GPIO_PIN_RESET)
		{
			if (menuState == ModoAut || menuState == ModoMan)	//solo start en manual o automatico
			{
				start = !start;
				if (!start)
				{
					halt = true;
					OSTimeDlyResume(APP_CFG_TASK3_PRIO); //Le quita los delay a la tarea del motor
				}
				OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display
				OSTimeDlyHMSM(0, 0, 0, 400u);			//Delay luego de pulsarlo
			}
		}
		/***************************PULSADOR START/STOP *******************************/


	}


}


/*
*********************************************************************************************************
*                                        TAREA 2:  ActualizarDisplay()
*
* Description : Esta tarea actualiza lo que se muestra en el display. La misma se autosuspende.
* 				Borra, escribe y se suspende hasta que la vuelvan a resumir
*
* Argument(s) : p_arg       Argument passed to 'Task2()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void ActualizarDisplay (void *p_arg)
{
												//Configuracion inicial
	menuState=ModoAut;
	LiquidCrystal(GPIOA, LCD_RS_Pin, LCD_RW_Pin, LCD_EN_Pin, LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin);
	OSTimeDlyHMSM(0u, 0u, 1u, 0u);



	  while (1)
	  {
		  	clear();
		  	setCursor(0,0);
		  	char buffer[5];			//Este buffer contiene los números de los parametros

		  	switch(menuState)
		  	{
		  	case ModoAut:

		  		print("Modo Automatico");
		  		if (errorTimeoutSup)
		  		{
		  			setCursor(0,1);
		  			print("Err Fc Sup");
		  		}
		  		else
		  		{
		  			if (errorTimeoutInf)
		  			{
		  				setCursor(0,1);
		  				print("Err Fc Inf");
		  			}

		  		}
		  		break;
		  	case ModoMan:

		  		print("Modo Manual");

		  		break;
		  	case CantRejillas:

		  		print("Cant de Rejillas");
		  		setCursor(0,1);
		  		sprintf(buffer,"%d",nRejillas);
		  		print(buffer);
		  		break;

		  	case Desplazamiento:
		  		print("Distancia en mm");
		  		setCursor(0,1);
		  		sprintf(buffer,"%d",distancia_mm);
		  		print(buffer);
		  		break;

		  	case VelMax:
		  		print("RPM motor Max");
		  		setCursor(0,1);
		  		sprintf(buffer,"%d",velMAX_RPM);
		  		print(buffer);
		  		break;

		  	case VelMin:
		  		print("RPM motor Min");
		  		setCursor(0,1);
		  		sprintf(buffer,"%d",velMIN_RPM);
		  		print(buffer);
		  		break;

		  	case count:
		  		break;

		  	}
		  	if(start)
		  	{
		  		setCursor (7,1);
		  		print("RUN");
		  		blink();
		  	}
		  	else
		  	{
		  		noBlink();
		  	}

		  	OSTaskSuspend(APP_CFG_TASK2_PRIO); //Se autosuspende
	  }

}

/*
*********************************************************************************************************
*                              TAREA 3: ControlMotor (void *p_arg)
*
* Description : Esta tarea controla el motor según las variables seteadas por las
* 				otras funciones
*
* Argument(s) : p_arg       Argument passed to 'ControlMotor ' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void ControlMotor (void *p_arg)
{

	while(DEF_TRUE)
	{
		OSTimeDlyHMSM(0u, 0u, 0u, 40);

		if(start)
		{
			if (menuState == ModoMan)	//if modo manual
			{
				if (giroHorario && !giroAntiHorario )
				{
					GiroHorario();
				}
				else
					if (!giroHorario && giroAntiHorario )
					{
						GiroAntiHorario();
					}
					else // Si estoy en modo manual but no presiono los pulsadores
					{

						halt = true;
					}
			} //End if modo manual
			else
			{
				if(menuState == ModoAut)	//if modo automatico)
				{
					CicloAutomatico();
				}
			}

		}
		else   //if not start
		{
			halt = true;
		}

	}

}



/*
*********************************************************************************************************
*                               TAREA 4: Detener (void *p_arg)
*
* Description : Esta tarea detiene inmediatamente al motor segun el estado de la variable halt
* 				Tiene alta prioridad y bajo delay
*
* Argument(s) : p_arg       Argument passed to 'Detener' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void Detener (void *p_arg)
{
	while(DEF_TRUE)
		{
			if(halt)
			{
				HAL_GPIO_WritePin(GPIOB, ENA_Pin, GPIO_PIN_SET); 		//INHABILITO DRIVER
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
				halt = false;
			}
				OSTimeDly(1);							//Tiene alta prioridad y bajo delay

		}
}

/*
*********************************************************************************************************
*                                    FinalesDeCarrera (void *p_arg)
*
* Description :Esta tarea se encarga de leer los finales de carrera y detener el motor
*
* Argument(s) : p_arg       Argument passed to 'Rampa' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void FinalesDeCarrera (void *p_arg)
{

	while(DEF_TRUE)
		{
			if(HAL_GPIO_ReadPin(GPIOB, Fc_Inf_Pin))
			{
				halt=true;
				OSTaskSuspend(APP_CFG_TASK5_PRIO); //Se autosuspende
			}
			else
			{
				if(HAL_GPIO_ReadPin(GPIOB, Fc_Sup_Pin))
						{
							halt=true;
							OSTaskSuspend(APP_CFG_TASK5_PRIO); //Se autosuspende
						}
			}
			OSTimeDly(2);
		}
}

/*
*********************************************************************************************************
*                                    Piezas (void *p_arg)
*
* Description : Esta tarea cuenta piezas para comenzar el ciclo. Lee solo flancos
* 				Ascendentes
*
* Argument(s) : p_arg       Argument passed to 'Piezas' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void Piezas (void *p_arg)
{
	bool mismaPieza = false; // Para leer solo flanco ascendente

	while(DEF_TRUE)
		{
		OSTimeDly(50);
		if(!mismaPieza && HAL_GPIO_ReadPin(GPIOB, Sensor_rejillas_Pin) && !rejillasReady)
		{
			rejillasActuales ++;
			mismaPieza = true;
		}
		else
		{
			if (mismaPieza && ! HAL_GPIO_ReadPin(GPIOB, Sensor_rejillas_Pin))
			{
				mismaPieza = false; //Flanco descendente
			}
		}

		if (rejillasActuales >= nRejillas)
		{
			rejillasReady = true;
			rejillasActuales =0;
		}



		}
}

/****************************************************************************************
 * 							DEFINICIÓN DE FUNCIONES
*****************************************************************************************/

/******************************** CicloAutomatico() *************************************
 * Esta función ejectua 1 solo ciclo de subida y bajada  del elevador.
 * Primero calculula un delay proporcional a la distancia que se estima recorrer.
 *
 * PRIMER MEDIO CICLO: CICLO DE SUBIDA
 * Arranca si el sistema se encuentra en start y el final de carrera inferior está activo
 * 1/4 del recorrido lo realiza a baja velocidad. 2/4 a alta velocidad y el ultimo tramo
 * a baja velocidad nuevamente. Superada la distancia calculada sigue avanzando a baja
 * velocidad con el objetivo de encontrar el final de carrera. si no lo encuentra lanza
 *  un error
 *
 * SEGUNDO MEDIO CICLO: CICLO DE BAJADA
* Arranca si el sistema se encuentra en start y el final de carrera superior está activo
 * 1/4 del recorrido lo realiza a baja velocidad. 2/4 a alta velocidad y el ultimo tramo
 * a baja velocidad nuevamente. Superada la distancia calculada sigue avanzando a baja
 * velocidad con el objetivo de encontrar el final de carrera. si no lo encuentra lanza
 *  un error
 *
 ****************************************************************************************/

void CicloAutomatico()
{
	/****************** CALCULOS ********************
	*Según la distancia que se desea recorrer, se necesitan x cantidad de pulsos enviados
	*Se envia 2 khz para velocidad max y 1 khz para velocidad min
	*
	**/
	const float delay_ms = ((distancia_mm / desplazamientoXrev ) * pulsosXrev ) / 2000; // Calculo delay
	const float tiempo2khz = delay_ms / 2;
	const float tiempo1khz = (delay_ms / 4) * 2;
	/****************** CALCULOS ********************/
	/****************** Reset Errores ********************/
	if (HAL_GPIO_ReadPin(GPIOB, Fc_Inf_Pin) && (errorTimeoutInf || errorTimeoutSup)) //Reset errores FC
	{
		errorTimeoutInf = false;
		errorTimeoutSup = false;
		OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display

	}
	/****************** Reset Errores ********************/

	/********************** CICLO DE SUBIDA ***********************************/
	if(start && HAL_GPIO_ReadPin(GPIOB, Fc_Inf_Pin))//if start y Fc inferior
	{

		htim4.Instance->PSC = 1;					//Preescaler motor = 1.El motor recibe 1khz
		GiroHorario();
		OSTimeDly((uint32_t)tiempo1khz);			//Recorre 1/4 tramo a baja velocidad
		OSTaskResume(APP_CFG_TASK5_PRIO); 			//Resume la tarea Finales de Carrera
		if (!start) return;							//Si se interrumpe la marcha
		htim4.Instance->PSC = 0;					//Preescaler motor = 0.El motor recibe 2khz
		OSTimeDly((uint32_t)tiempo2khz);			//Recorre 2/4 tramo a alta velocidad
		if (!start) return;
		htim4.Instance->PSC = 1;
		OSTimeDly((uint32_t)tiempo1khz);			//Recorre 1/4 tramo a baja velocidad
		if (!start) return;


		OSTimeDly(timeout_Fc_Sup);					//Timeout FC superior
		halt = true;
		if (!HAL_GPIO_ReadPin(GPIOB, Fc_Sup_Pin))   //Si no se alcanzó el final de carrera sup
		{
			errorTimeoutSup = true;
			start = false;
			OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display

		}
		if (!start || errorTimeoutSup ) return;
		OSTimeDly(tiempoDescarga);
		if (!start || errorTimeoutSup) return;
	}
	/********************** CICLO DE SUBIDA ***********************************/

	/********************** CICLO DE BAJADA ***********************************/
	if(start && HAL_GPIO_ReadPin(GPIOB, Fc_Sup_Pin))//if start y Fc superior
	{
		htim4.Instance->PSC = 1;					//Preescaler motor = 1.El motor recibe 1khz
		GiroAntiHorario();
		OSTimeDly((uint32_t)tiempo1khz);			//Recorre 1/4 tramo a baja velocidad
		OSTaskResume(APP_CFG_TASK5_PRIO); 			//Resume la tarea Finales de Carrera
		if (!start) return;							//Si se interrumpe la marcha
		htim4.Instance->PSC = 0;					//Preescaler motor = 0.El motor recibe 2khz
		OSTimeDly((uint32_t)tiempo2khz);			//Recorre 2/4 tramo a alta velocidad
		if (!start) return;
		htim4.Instance->PSC = 1;
		OSTimeDly((uint32_t)tiempo1khz);			//Recorre 1/4 tramo a baja velocidad
		if (!start) return;

		OSTimeDly(timeout_Fc_Inf);					//Timeout FC inferior
		halt = true;
		if (!HAL_GPIO_ReadPin(GPIOB, Fc_Inf_Pin))   //Si no se alcanzó el final de carrera inf
		{
			errorTimeoutInf = true;
			start = false;
			OSTaskResume(APP_CFG_TASK2_PRIO); 		//Resume la tarea display
		}
		OSTimeDly(tiempoDescarga);					//Delay tiempo descarga
	}
	/********************** CICLO DE BAJADA ***********************************/

}

void GiroHorario()
{
	HAL_GPIO_WritePin(GPIOB, ENA_Pin, GPIO_PIN_RESET); 		//HABILITO DRIVER
	OSTimeDlyHMSM(0, 0, 0, 1u);
	HAL_GPIO_WritePin(GPIOB, DIR_Pin, GPIO_PIN_RESET); 		//GIRO PA UN LADO

	OSTimeDlyHMSM(0, 0, 0, 1u);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void GiroAntiHorario()
{
	HAL_GPIO_WritePin(GPIOB, ENA_Pin, GPIO_PIN_RESET); 		//HABILITO DRIVER
	OSTimeDlyHMSM(0, 0, 0, 1u);
	HAL_GPIO_WritePin(GPIOB, DIR_Pin, GPIO_PIN_SET); 		//GIRO PA OTRO LADO
	OSTimeDlyHMSM(0, 0, 0, 1u);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

