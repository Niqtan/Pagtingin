#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//ESP32 General Libraries
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"


//ESP32 IoT 
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mqtt_client.h"


//ESP32 Bluetooth Libraries
#include <esp_bt_defs.h>

//Communication Protocols: I2C
    #include "driver/i2c_master.h"

//Communication Protocols: UART
    #include "driver/uart.h"

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


//Error debugging
esp_err_t ret;

/*
==============================================
==============================================
                  GPIO PINS
==============================================
==============================================
*/

//INPUT PINS

#define GPIO_INPUT_IO_12 CONFIG_GPIO_INPUT_12
#define GPIO_INPUT_IO_13 CONFIG_GPIO_INPUT_13
#define GPIO_INPUT_IO_14 CONFIG_GPIO_INPUT_14

#define GPIO_INPUT_PIN_SEL ((1ULL<<GPIO_INPUT_IO_12) | (1ULL<<GPIO_INPUT_IO_13) | (1ULL<<GPIO_INPUT_IO_14))

//OUTPUT PINS
#define GPIO_OUTPUT_IO_01 CONFIG_GPIO_OUTPUT_01 
#define GPIO_OUTPUT_IO_19 CONFIG_GPIO_OUTPUT_19
#define GPIO_OUTPUT_IO_23 CONFIG_GPIO_OUTPUT_23
#define GPIO_OUTPUT_IO_25 CONFIG_GPIO_OUTPUT_25
#define GPIO_OUTPUT_IO_26 CONFIG_GPIO_OUTPUT_26

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_01) | (1ULL<<GPIO_OUTPUT_IO_19) | (1ULL<<GPIO_OUTPUT_IO_23) | (1ULL<<GPIO_OUTPUT_IO_25) | (1ULL<<GPIO_OUTPUT_IO_26))

//UART Pins
const uart_port_t uart_port_num = UART_NUM_0;
    
    //UART Driver Installation
    #define UART_BUFFER_SIZE 2048
    QueueHandle_t uart_queue;

    #define EVENT_QUEUE_SIZE 10


//I2C Pins
    #define I2C_MASTER_SCL_IO 22 //SDA Pin
    #define I2C_MASTER_SDA_IO 21 //SCL Pin

    #define I2C_PORT_NUM I2C_NUM_0

//I2S Pins
    #include "i2s.h"
    #include "i2s_std.h"

//MQTT STUFF


//GLOBAL DEFINITIONS
#define WS_PIN 

#define TRIG_PIN 4
#define ECHO_PIN 5
#define SPEAKER_PIN 17
#define BUZZER_PIN 13

//Wrapval determines how long a PWM cycle should last
#define WRAPVAL 354
//The CLKDIV is for the clock divider in calibrating how
//many cycles can the given freq complete

//In simple terms, it's calibrating the speed of the frequency
#define CLKDIV 8.0f

#define AND &&
#define TIMEOUT 30000

//Map function constants
#define THRESHOLD 30
#define BUZZER_MAX 250
#define BUZZER_MIN 10 

/* STA Configuration */
#define ESP_WIFI_STA_SSID CONFIG_ESP_WIFI_REMOTE_AP_SSID
#define ESP_WIFI_STA_PASSWD CONFIG_ESP_WIFI_REMOTE_AP_SSID

//Event handler for the wifi communication protocol
static EventGroupHandle_t wifi_event_group;