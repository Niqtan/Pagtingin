#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//ESP32 General Libraries
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"

//Communication Protocols: UART
#include "driver/uart.h"

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


//Error debugging
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
esp_err_t ret;
const char *TAG;


/* GPIO PINS */

//INPUT PINS
#define GPIO_INPUT_IO_0 0
#define GPIO_INPUT_IO_15 15
#define GPIO_INPUT_IO_19 19

#define GPIO_INPUT_PIN_SEL  ( (1ULL << GPIO_INPUT_IO_0)  | \
                              (1ULL << GPIO_INPUT_IO_15) | \
                              (1ULL << GPIO_INPUT_IO_19) )

//OUTPUT PINS
#define GPIO_OUTPUT_IO_2 2
#define GPIO_OUTPUT_IO_4 4
#define GPIO_OUTPUT_IO_5 5
#define GPIO_OUTPUT_IO_18 18
#define GPIO_OUTPUT_IO_12 12
#define GPIO_OUTPUT_IO_13 13
#define GPIO_OUTPUT_IO_14 14
#define GPIO_OUTPUT_IO_23 23
#define GPIO_OUTPUT_IO_25 25

#define GPIO_OUTPUT_PIN_SEL  ( (1ULL << GPIO_OUTPUT_IO_2)  | \
                               (1ULL << GPIO_OUTPUT_IO_4)  | \
                               (1ULL << GPIO_OUTPUT_IO_5)  | \
                               (1ULL << GPIO_OUTPUT_IO_12) | \
                               (1ULL << GPIO_OUTPUT_IO_13) | \
                               (1ULL << GPIO_OUTPUT_IO_14) | \
                               (1ULL << GPIO_OUTPUT_IO_18) | \
                               (1ULL << GPIO_OUTPUT_IO_23) | \
                               (1ULL << GPIO_OUTPUT_IO_25) )
/* UART Pins */
const uart_port_t uart_port_num = UART_NUM_0;
    
//UART Driver Installation
#define UART_BUFFER_SIZE 2048
QueueHandle_t uart_queue;

#define EVENT_QUEUE_SIZE 10

#define UART_RX_PIN 1 
#define UART_TX_PIN 3

/* I2C Pins */
#include "driver/i2c_master.h"

//The bus handle is like a USB hub maanger,
//all configurations are plugged into this hub
i2c_master_bus_handle_t tof_bus_handle;

//Handler for the specific I2C device (usually a slave) on the I2C bus
i2c_master_dev_handle_t tof_dev_handle;

//Handler for read and write operations
i2c_master_dev_handle_t i2c_dev;

#define I2C_MASTER_SCL_IO 22 //SDA Pin
#define I2C_MASTER_SDA_IO 21 //SCL Pin

#define I2C_PORT_NUM I2C_NUM_0
#define I2C_TOF_ADDRESS 0x29


//I2S libraries
#include "driver/i2s_std.h"

#define LRCK_IO 32
#define DOUT_IO 33
#define BCK_IO 27
#define SCK_IO 26

//I2S Constants
#define SAMPLE_RATE 44100

// Beep beep beep   
#define BEEP_FREQ 2000
#define AMPLITUDE 30000
#define DURATION 0.2

//Mapping the beeping sounds
#define THRESHOLD 30

i2s_chan_handle_t tx_handler;

//Distance
volatile uint16_t distance_cm;

//Map function constants
#define THRESHOLD 30
#define BUZZER_MAX 250
#define BUZZER_MIN 10 
