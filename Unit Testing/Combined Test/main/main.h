#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//ESP32 General Libraries
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_sleep.h"
#include "esp_timer.h"

//Communication Protocols: UART
#include "driver/uart.h"

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//WIFI
#include "esp_wifi.h"



//Error debugging
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
esp_err_t ret;
const char *TAG;


/* Finite State Machine */
typedef enum {
    STATE_IDLE,
    STATE_ALERT,
    STATE_MUTE
} system_state_t;

//IDLE WHEN: Chilling and doing nothing
//ALERT WHEN: Include here the playing the buzzer and stuff
//MUTE WHEN: Interrupt and turn everything off

system_state_t current_state = STATE_IDLE;

//OUTPUT PINS
#define GPIO_OUTPUT_IO_5 5
#define GPIO_OUTPUT_IO_18 18
#define GPIO_OUTPUT_IO_19 19
#define GPIO_OUTPUT_IO_23 23


#define GPIO_OUTPUT_PIN_SEL  ( (1ULL << GPIO_OUTPUT_IO_5) | \
                                (1ULL < GPIO_OUTPUT_IO_18) | \
                                (1ULL << GPIO_OUTPUT_IO_19) | \
                               (1ULL << GPIO_OUTPUT_IO_23) )

                               /* UART Pins */
const uart_port_t uart_port_num = UART_NUM_0;
    
//UART Driver Installation
#define UART_TX_SIZE 1024
#define UART_RX_PIN 1 
#define UART_TX_PIN 3

/* I2C Pins */
#include "driver/i2c_master.h"

//Task Handler 
TaskHandle_t write_handler = NULL;
TaskHandle_t read_handler = NULL;

//Handler for read and write operations
i2c_master_dev_handle_t i2c_dev;

#define I2C_MASTER_SCL_IO 22 //SDA Pin
#define I2C_MASTER_SDA_IO 21 //SCL Pin

#define I2C_PORT_NUM_0 I2C_NUM_0
#define I2C_PORT_NUM_1 I2C_NUM_1
#define I2C_TOF_DEFAULT_ADDRESS 0X29
#define I2C_TOF1_ADDRESS 0x30
#define I2C_TOF2_ADDRESS 0x31 

//I2S libraries
#include "driver/i2s_std.h"

//Task handler for playing beeps
TaskHandle_t music_handler = NULL;

#define LRCK_IO 32
#define DOUT_IO 33
#define BCK_IO 27
#define SCK_IO 26

//I2S Constants
#define SAMPLE_RATE 44100

// Beep beep beep   
#define BEEP_FREQ 2000
#define AMPLITUDE 30000
#define DURATION 0.5

//Mapping the beeping sounds
#define THRESHOLD 30

i2s_chan_handle_t tx_handler;

//Distance
uint16_t distance_cm;
int beep_delay;

//Map function constants
#define THRESHOLD 30
#define BUZZER_MAX 250
#define BUZZER_MIN 10 
