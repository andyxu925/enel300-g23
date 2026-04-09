/*
#ifndef Motor_h
#define Motor_h

// just listing the functions so other files can use
void motor_str(void);
void motor_spd(void);
void motor_fwd(void);
void motor_bck(void);
void motor_lft(void);
void motor_rht(void);
void motor_stp(void);

#endif

// done
*/

#ifndef MOTOR_H
#define MOTOR_H

#include "driver/ledc.h"

void motor_str(void);
void motor_drive(void);
void motor_reverse(void);
void motor_left(void);
void motor_right(void);
void motor_stop(void);
void motor_speed_slide(int duty);
void led_init(void);
void led_toggle(void);
void set_motor_speed(ledc_channel_t channel, int gpio_pwm, int gpio_gnd, int duty);
#endif