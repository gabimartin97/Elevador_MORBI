/*
 * calibracion.h
 *
 *  Created on: May 14, 2021
 *      Author: gabri
 */

#ifndef INC_CALIBRACION_H_
#define INC_CALIBRACION_H_



#endif /* INC_CALIBRACION_H_ */

#define desplazamientoXrev 0.1	//cuantos mm se desplaza linealmente por cada vuelta del mot
#define pulsosXrev  400			//seteado en el driver

#define timeout_Fc_Sup 4000		//Tiempo en el que busca el final de carrera superior
#define timeout_Fc_Inf 4000		//Tiempo en el que busca el final de carrera inferior
#define tiempoDescarga 2000		//Tiempo de reposo de la descarga superior

#define delayGralPulsadores 50
#define delayPulsadorUpDwn 50

static const uint32_t rampa[10]= {10,1,1,1,1,1,1,1,1,0};


