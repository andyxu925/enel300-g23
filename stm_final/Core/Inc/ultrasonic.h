#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"

void ultrasonic_init(void);
void ultrasonic_trig(void);
float ultrasonic_echo(void);

#endif /* INC_ULTRASONIC_H_ */