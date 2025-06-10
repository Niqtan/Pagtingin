#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//ESP32 General Libraries
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"


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

void setup_pins(void);

/* get_pulse: activates the HY-SRF05
* @param a: GPIO pin trig pin is connected 
* @param b: GPIO pin echo pin is connected
* @return: confirmation if successful operation
*/
uint64_t get_pulse(uint8_t trig_pin, uint8_t echo_pin);

/* get_cm: Converts the distance to cm
* @param a: GPIO pin trig pin is connected 
* @param b: GPIO pin echo pin is connected
* @return: the cm result after converting
*/

uint64_t get_cm(uint8_t trig_pin, uint8_t echo_pin);

//Initialize the interrupts of PWM and the buzzzer
void interrupt_initialize();

//Interrupt handler for the buzzer
void buzzer_cook_handler();

//Interrupt handler for the speaker
void pwm_interrupt_handler();

/* map: maps the values of the buzzer delay
* @param a: the distance taken by the ultrasonic sensor
* @param b: the starting range of the current value
* @param c: the ending range of the current value
* @param d: the new minimum target range 
* @param e: the new maximum target range
* @return: integer within the new range
*/
int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high);
