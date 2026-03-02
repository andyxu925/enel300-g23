#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_rom_gpio.h"
#include <stdio.h>


/*#define UART_1_TX 17
#define UART_1_RX 16*/
// 
#define IN1_MTR1 GPIO_NUM_26
#define IN2_MTR1 GPIO_NUM_27
#define I_MTR1 GPIO_NUM_14
#define IN1_MTR2 GPIO_NUM_17
#define IN2_MTR2 GPIO_NUM_5
#define I_MTR2 GPIO_NUM_19

void app_main(void)
{
    gpio_config_t gpio_conf = {};

    gpio_conf.pin_bit_mask = (1ULL<<IN1_MTR1) | (1ULL<<IN2_MTR1) | (1ULL<<I_MTR1) | (1ULL<<IN1_MTR2) | (1ULL<<IN2_MTR2) | (1ULL<<I_MTR2);

    gpio_conf.pull_up_en = 0;
    gpio_conf.pull_down_en = 0;

    gpio_conf.mode = GPIO_MODE_OUTPUT;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&gpio_conf);

    while(1)
    {
        // following code should spin mtr 1 forward
        gpio_set_level(IN1_MTR1, 1);
        gpio_set_level(IN2_MTR1, 0);
        gpio_set_level(I_MTR1, 1);
    }
}