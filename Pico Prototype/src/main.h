#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h" 
#include "hardware/timer.h" //We need to time certain stuff
#include "hardware/gpio.h" //For the buzzer interrupt
#include "hardware/irq.h" //Used for interrupt in order to control PWM
#include "hardware/pwm.h" //Used for driving audio via PWM

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
