#include "main.h"

//Include the arrays consisting of the decoded audio files
#include "obstacle.h"

typedef struct {
  pwm_config le_conf;
  uint8_t slice_num;
  uint64_t distance;
} Pulse;

void setup_pins(void) {
 gpio_init(ECHO_PIN);
 gpio_set_dir(ECHO_PIN, GPIO_IN);

 gpio_init(TRIG_PIN);
 gpio_set_dir(TRIG_PIN, GPIO_OUT);

 gpio_init(SPEAKER_PIN);
 gpio_set_dir(SPEAKER_PIN, GPIO_OUT);

 gpio_init(BUZZER_PIN);
 gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

uint64_t get_pulse(uint8_t trig_pin, uint8_t echo_pin) {
   gpio_put(trig_pin, 1);
   sleep_us(10);
   gpio_put(trig_pin, 0);


   uint64_t start_time = time_us_64();;

   while (gpio_get(echo_pin) == 0) {
     if (time_us_64() - start_time > TIMEOUT) {
       printf("You're Cooked\n");
       return 0;
     }
   }

   uint64_t echo_start_time = time_us_64();
   
   while (gpio_get(echo_pin) == 1) {
     if (time_us_64() - start_time > TIMEOUT) {
       printf("You're Cooked\n");
       return 0;
     }
   }

   uint64_t echo_stop_time = time_us_64();

   return echo_stop_time - echo_start_time;

}


uint64_t get_cm(uint8_t trig_pin, uint8_t echo_pin) {
   uint64_t travel_time = get_pulse(trig_pin, echo_pin);
   
   return (travel_time / 29) / 2;
}

int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high) {
  return (distance - starting_low) * (ending_high - ending_low) / (starting_high - starting_low) + ending_low;
}

int wav_position = 0;
//Flag for calling PWM
bool pwm_is_active = false;

void pwm_interrupt_handler() {
   //ISR Function for managing the interrupt
   
   pwm_clear_irq(pwm_gpio_to_slice_num(SPEAKER_PIN));

   //JUST TO BE CLEAR:
   /* This portion of the code functions like a loop
   By using the interrupt, it can keep calling the hardware 
   over and over until certain conditions have been met
   */                              //Multiply by 8
   if (wav_position < WAV_DATA_LENGTH - 1) {
     
     //How does this work?

     /* IN PWM, we have cycles. A cycle is basically something wherein 
     it is a measurement of an on and off time cycle

     Bit shifting by 8 means that we repeat the same value to the pwm pin 8 times 
     in a row, which gets us at the right sampling rate for the audio
     */
     pwm_set_gpio_level(SPEAKER_PIN, WAV_DATA[wav_position]); //Divide by 8
     wav_position++;
   }
   else {
     //Reset if it reaches the very end of array
     wav_position = 0;
     pwm_is_active = false;
   }

}

Pulse* pwm_initialize() {
 gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);

 Pulse *pwm_settings;

 //Converts our GPIO pin to a slice num
 pwm_settings->slice_num = pwm_gpio_to_slice_num(SPEAKER_PIN);

 //Set the PWM interrupt
 pwm_clear_irq(pwm_settings->slice_num);
 pwm_set_irq_enabled(pwm_settings->slice_num, true);

 //Setting up the interrupt handler
 irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler);
 irq_set_enabled(PWM_IRQ_WRAP, true);
  
 pwm_settings->le_conf = pwm_get_default_config();
 pwm_config_set_clkdiv(&pwm_settings->le_conf, CLKDIV * 8);
 //PWM freq = clock freq/(wrap + 1) * clock divider val
 //So, if we have a 11k sample rate:
   //11kHz = 125,000,000 Hz / (wrap + 1) * 8.0
   //To find the wrap val, you may just rearrange the equation via algebra:
   //wrap = (125,000,000 / 11000 * 8) - 1
   //wrap = 1419
 pwm_config_set_wrap(&pwm_settings->le_conf, WRAPVAL);

 return pwm_settings;
}

void pwm_speaker(void* pvParameters) {
  Pulse* pwm_settings;

  pwm_init(&pwm_settings->slice_num, &pwm_settings->le_conf, true);

  pwm_set_gpio_level(SPEAKER_PIN, 0);

  while (pwm_is_active) {
  __wfi();
  }

}

void buzzer_cook(void* pvParameters) {
  int buzzer_delay; 
  Pulse* pwm_settings;

  buzzer_delay = map(pwm_settings->distance, 0, THRESHOLD, BUZZER_MIN, BUZZER_MAX);
      
  gpio_put(BUZZER_PIN, 1);
  sleep_ms(buzzer_delay);
  gpio_put(BUZZER_PIN, 0);
  sleep_ms(buzzer_delay);
      
}

void activate_tasks(void* pvParameters)
{
  Pulse *pwm_settings = pwm_initialize();

  //Task handlers
  
  TaskHandle_t PWM_task = NULL;
  
  TaskHandle_t Buzzer_task = NULL;

  while (true) {
    pwm_settings->distance = get_cm(TRIG_PIN, ECHO_PIN);
    
    if (pwm_settings->distance >= THRESHOLD AND pwm_settings->distance <= 50) {
      //Call PWM signal for activating the speaker
      if (pwm_is_active == false) {
        pwm_is_active = true;
        //Task for firing up the PWM speaker
        if (!PWM_task) {
          xTaskCreate(pwm_speaker, "PWM_Interrupt", 1024, NULL, tskIDLE_PRIORITY, &PWM_task);
        }
      }
    }
    else if (pwm_settings->distance < THRESHOLD) {
      //Task for firing up the buzzer
      if (!Buzzer_task) {
        xTaskCreate(buzzer_cook, "buzzer_cooks", 1024, NULL, 1, &Buzzer_task);
        pwm_is_active = false;
      }
    }
   else {
     pwm_is_active = false;
     gpio_put(BUZZER_PIN, 0);
   }

   sleep_ms(10);
   printf("Distance: %lld cm\n", pwm_settings->distance);
  }
}


int main() {
 //Initialize I/O
 stdio_init_all();
 
 setup_pins();

 xTaskCreate(activate_tasks, "Master_tasker", NULL, 2, NULL);

 //Start the freeRTOS scheduler
 vTaskStartScheduler();
} 