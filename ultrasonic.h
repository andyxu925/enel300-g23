/*
 * ultrasonic.h
 *
 *  Created on: Mar 6, 2026
 *      Author: diana
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

// Configure your pins here or in CubeMX main.h
#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOA
#define ECHO_PIN GPIO_PIN_8
#define ECHO_PORT GPIOA

// Functions
void Ultrasonic_Init(void);
float Ultrasonic_Read(void);


#endif /* INC_ULTRASONIC_H_ */
