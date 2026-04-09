/*
#include <stdio.h>
#include "motor.h"
#include "wifi.h"
#include "web_server.h"

// starts all functions defined in each file
void app_main(void)
{
    motor_str();
    wifi_str();
    web_str(); 
}
*/ 
// done

#include "motor.h"
#include "wifi.h"
#include "web_server.h"
#include "driver/dac.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern int freq_value;

void app_main(void)
{
    motor_str();
    wifi_str();
    web_str();
    led_init();

    //dac_output_enable(DAC_CHANNEL_1);
/*
    while (1)
    {
        uint8_t dac_val;

        // Clamp
        if (freq_value < 18000) freq_value = 18000;
        if (freq_value > 22000) freq_value = 22000;

        dac_val = (uint8_t)(((freq_value - 18000) * 255) / 4000);

        dac_output_voltage(DAC_CHANNEL_1, dac_val);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
        */
}