#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h" 
#include "hardware/timer.h" //We need to time certain stuff
#include "hardware/irq.h" //Used for interrupt in order to control PWM
#include "hardware/pwm.h" //Used for driving audio via PWM
#include "hardware/sync.h" //For the "wait for interrupt" function

#define TRIG_PIN 4
#define ECHO_PIN 5
#define SPEAKER_PIN 16
#define BUZZER_PIN 13

//Wrapval determines how long a PWM cycle should last
#define WRAPVAL 12499
//The CLKDIV is for the clock divider in calibrating how
//many cycles can the given freq complete

//In simple terms, it's calibrating the speed of the frequency
#define CLKDIV 1.0f

#define AND &&
#define TIMEOUT 30000

//Map function constants
#define THRESHOLD 30
#define BUZZER_MAX 250
#define BUZZER_MIN 10 

void setup_pins(void);

uint64_t get_pulse(uint8_t trig_pin, uint8_t echo_pin);

uint64_t get_cm(uint8_t trig_pin, uint8_t echo_pin);

void pwm_interrupt_handler();

int pwm_speaker();

/* map: maps the values of the buzzer delay
* @param a: the distance taken by the ultrasonic sensor
* @param b: 
*/
int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high);