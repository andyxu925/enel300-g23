/*
OLD CODE FOR L298N
#include "motor.h"
#include "driver/gpio.h"
#include "driver/ledc.h"


// gpios used for controlling L298N
#define IN1 GPIO_NUM_16
#define IN2 GPIO_NUM_17
#define IN3 GPIO_NUM_18
#define IN4 GPIO_NUM_19

#define ENA GPIO_NUM_27 //25
#define ENB GPIO_NUM_26 //26

#define LED_GPIO GPIO_NUM_12

#define PWM_FREQ 5000
#define PWM_RES LEDC_TIMER_8_BIT

static int speed = 200;
static int led_state = 0;

void motor_str(void)
{
    gpio_set_direction(IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN3, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN4, GPIO_MODE_OUTPUT);

    ledc_timer_config_t timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQ,
        .duty_resolution = PWM_RES
    };

    ledc_timer_config(&timer);

    ledc_channel_config_t ch0 = {
        .gpio_num = ENA,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = speed
    };

    ledc_channel_config(&ch0);

    ledc_channel_config_t ch1 = {
        .gpio_num = ENB,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = speed
    };

    ledc_channel_config(&ch1);
}

void motor_speed_slide(int duty)
{
    speed = duty;

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}

// LED code

void led_init()
{
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);
}

void led_toggle()
{
    led_state = !led_state;
    gpio_set_level(LED_GPIO, led_state);
}

// the following functions set the gpio's for each input from user

void motor_drive(void)
{
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 1);
    gpio_set_level(IN4, 0);
}

void motor_reverse(void)
{
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 1);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 1);
}

void motor_left(void)
{
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 1);
}

void motor_right(void)
{
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 1);
    gpio_set_level(IN3, 1);
    gpio_set_level(IN4, 0);
}

void motor_stop(void)
{
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 0);
}
*/


#include "motor.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

// left
#define IN1 GPIO_NUM_16  
#define IN2 GPIO_NUM_17  

// right
#define IN3 GPIO_NUM_19 //18  
#define IN4 GPIO_NUM_18  //19

// Current Sensing Pins (Set as INPUTS to avoid frying/conflicts)
#define I_MTR1 GPIO_NUM_27
#define I_MTR2 GPIO_NUM_26

#define LED_GPIO GPIO_NUM_12

#define PWM_FREQ 5000
#define PWM_RES LEDC_TIMER_8_BIT

static int speed = 200;

void motor_str(void)
{
    gpio_set_direction(I_MTR1, GPIO_MODE_INPUT);
    gpio_set_direction(I_MTR2, GPIO_MODE_INPUT);
    gpio_set_direction(IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN3, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN4, GPIO_MODE_OUTPUT);

    ledc_timer_config_t timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQ,
        .duty_resolution = PWM_RES
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t ch0 = {
        .gpio_num = IN1,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0
    };
    ledc_channel_config(&ch0);

    ledc_channel_config_t ch1 = {
        .gpio_num = IN3,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0
    };
    ledc_channel_config(&ch1);
}

void set_motor_speed(ledc_channel_t channel, int gpio_pwm, int gpio_gnd, int duty) {
    gpio_set_level(gpio_gnd, 0);
    ledc_set_pin(gpio_pwm, LEDC_HIGH_SPEED_MODE, channel);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

void motor_drive(void) {
    set_motor_speed(LEDC_CHANNEL_0, IN1, IN2, speed); 
    set_motor_speed(LEDC_CHANNEL_1, IN3, IN4, speed);
}

void motor_reverse(void) {
    set_motor_speed(LEDC_CHANNEL_0, IN2, IN1, speed); 
    set_motor_speed(LEDC_CHANNEL_1, IN4, IN3, speed);
}

void motor_stop(void) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}

void motor_left(void) {
    set_motor_speed(LEDC_CHANNEL_0, IN1, IN2, 0); 
    set_motor_speed(LEDC_CHANNEL_1, IN3, IN4, speed);
}

void motor_right(void) {
    set_motor_speed(LEDC_CHANNEL_0, IN1, IN2, speed);
    set_motor_speed(LEDC_CHANNEL_1, IN3, IN4, 0);
}

void motor_speed_slide(int duty) {
    speed = duty;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}

void led_init() {
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);
}

void led_toggle() {
    static int state = 0;
    state = !state;
    gpio_set_level(LED_GPIO, state);
}