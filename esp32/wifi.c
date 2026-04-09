/*
#include "wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

// esp hosts wifi server through this function
void wifi_str(void) //void wifi_init_softap(void)
{

}
*/

#include "wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

// below is code to allow any device to connect to esp to get webserver

void wifi_str(void)
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "Group23_controller",
            .ssid_len = 0,
            .password = "Lebron23",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();
}