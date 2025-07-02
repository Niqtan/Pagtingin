#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//ESP32 General Libraries
#include "driver/gpio.h"
#include "sdkconfig.h"

//Communication Protocols: UART
#include "driver/uart.h"


//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


//Error debugging
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

esp_err_t ret;

//ESPLOG Debugging
const char *TAG;

/*
==============================================
==============================================
                 PINS
==============================================
==============================================
*/

#define UART_RX_PIN 1 
#define UART_TX_PIN 3

//UART Driver Installation
#define UART_TX_SIZE 1024
QueueHandle_t uart_queue;



/* I2S PINS 
where DIN is the Serial data,
LRCK is the word select (Select channel),
SCK is the CLK or master clock
BCK is the bit lcock for timing each data bit
*/

//I2S libraries
#include "driver/i2s_std.h"

#define LRCK_IO 32
#define DOUT_IO 33
#define BCK_IO 27
#define SCK_IO 26

//I2S Constants
#define SAMPLE_RATE 44100

// Beep beep beep   
#define BEEP_FREQ 3000
#define AMPLITUDE 30000
#define DURATION 0.5

//Mapping the beeping sounds
#define THRESHOLD 30


i2s_chan_handle_t tx_handler;


