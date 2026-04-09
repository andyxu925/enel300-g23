
#include "web_server.h"
#include "esp_http_server.h"
#include "motor.h"


// below is just for the webpage design nothing really important
static const char *html_page =
"<html>"
"<body>"
"<style>"
"button { width:300px; height:150px; font-size:50px; margin:30px; color:purple; text-shadow: -3px -3px 0 white, -3px -3px 0 white, -3px -3px 0 white, -3px -3px 0 white; border-radius:30px; background-color: yellow; }"
"input[type=range] { width:600px; margin:20px; }"
"input[type=range]::-webkit-slider-thumb { height:50px; width:50px; -webkit-appearance: none; border-radius:50%; background: orange; border: 2px solid black; position: relative; background: radial-gradient(circle at center, orange 60%, #d37400 100%); }"
"</style>"
//"<body>"
"<h1>Group 23 Controller</h1>"
"<button onclick=\"fetch('/forward')\">Drive</button>"
"<button onclick=\"fetch('/backward')\">Reverse</button>"
"<button onclick=\"fetch('/left')\">Turn Left</button>"
"<button onclick=\"fetch('/right')\">Turn Right</button>"
"<button onclick=\"fetch('/stop')\">Stop</button>"
"<button onclick=\"fetch('/led')\">Toggle LED</button>"
//"<button onclick=\"fetch('/freq')\">Toggle Frequency</button>"
"<br><br>"
"<span style=\"font-size:50px;\">Frequency:</span>"
"<span id=\"fval\">20000</span>"
"<input type=\"range\" min=\"18000\" max=\"22000\" step=\"10\" "
"oninput=\"document.getElementById('fval').innerText = this.value;"
"fetch('/setFreq?val='+this.value)\">"
"<br><br>"
"<span style=\"font-size:50px;\">Speed:</span>"
"<input type=\"range\" min=\"0\" max=\"255\" "
"oninput=\"fetch('/speed?val='+this.value)\">"
"</body>"
"</html>";

// here is where the actual code is for the webserver

static esp_err_t root_handler(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// below functions take the motor functions and apply them when an input is read

static esp_err_t forward_handler(httpd_req_t *req)
{
    //motor_speed_slide(200);
    motor_drive();
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t stop_handler(httpd_req_t *req)
{
    motor_stop();
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t backward_handler(httpd_req_t *req)
{
    //motor_speed_slide(200);
    motor_reverse();
    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t left_handler(httpd_req_t *req)
{
    motor_left();
    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t right_handler(httpd_req_t *req)
{
    motor_right();
    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t speed_handler(httpd_req_t *req)
{
    char query[32];
    char param[8];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {

        if (httpd_query_key_value(query, "val", param, sizeof(param)) == ESP_OK) {

            int speed = atoi(param);
            motor_speed_slide(speed);
        }
    }

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// metal detector
int freq_value = 20000;

esp_err_t set_freq_handler(httpd_req_t *req)
{
    char buf[100];
    char param[32];

    if (httpd_req_get_url_query_len(req) > 0) {
        httpd_req_get_url_query_str(req, buf, sizeof(buf));

        if (httpd_query_key_value(buf, "val", param, sizeof(param)) == ESP_OK) {
            freq_value = atoi(param);
        }
    }

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// LED code
static esp_err_t led_handler(httpd_req_t *req)
{
    led_toggle();

    httpd_resp_send(req, "LED toggled", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// This is the webserver code specifically, basically run the specific function
// the webpage uses an http get request command that is sent to the esp which triggers the function we want to use
// html is sent back to the webpage 
void web_str(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    httpd_start(&server, &config);

    httpd_uri_t root = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_handler
    };

    httpd_uri_t forward = {
        .uri = "/forward",
        .method = HTTP_GET,
        .handler = forward_handler
    };

    httpd_uri_t stop = {
        .uri = "/stop",
        .method = HTTP_GET,
        .handler = stop_handler
    };

    httpd_uri_t backward_uri = {
        .uri = "/backward",
        .method = HTTP_GET,
        .handler = backward_handler
    };

    httpd_uri_t left_uri = {
        .uri = "/left",
        .method = HTTP_GET,
        .handler = left_handler
    };

    httpd_uri_t right_uri = {
        .uri = "/right",
        .method = HTTP_GET,
        .handler = right_handler
    };

    httpd_uri_t speed_uri = {
        .uri = "/speed",
        .method = HTTP_GET,
        .handler = speed_handler
    };

    httpd_uri_t led_uri = {
    .uri = "/led",
    .method = HTTP_GET,
    .handler = led_handler
    };

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &forward);
    httpd_register_uri_handler(server, &stop);
    httpd_register_uri_handler(server, &backward_uri);
    httpd_register_uri_handler(server, &left_uri);
    httpd_register_uri_handler(server, &right_uri);
    httpd_register_uri_handler(server, &speed_uri);
    httpd_register_uri_handler(server, &led_uri);
}