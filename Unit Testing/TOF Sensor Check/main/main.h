#include "sdkconfig.h"
#include "driver/gpio.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//ESP32 General Libraries
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"


//Communication Protocols: I2C
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"

//Communication Protocols: UART
#include "driver/uart.h"


//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


//Error debugging
esp_err_t ret;

//Distance
volatile uint16_t distance_cm;

/*
==============================================
==============================================
                 PINS
==============================================
==============================================
*/

/* OUTPUT PINS */
#define GPIO_OUTPUT_IO_22 22
#define GPIO_OUTPUT_IO_23 23

/* UART Pins */
const uart_port_t uart_port_num = UART_NUM_0;

#define UART_RX_PIN 1 
#define UART_TX_PIN 3

//UART Driver Installation
#define UART_BUFFER_SIZE 2048
QueueHandle_t uart_queue;

#define EVENT_QUEUE_SIZE 10


/* I2C Pins */

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