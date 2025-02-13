#include "main.h"

//Include the arrays consisting of the decoded audio files
#include "obstacle.h"

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
       printf("Your shit is doomed!\n");
       return 0;
     }
   }

   uint64_t echo_start_time = time_us_64();
   
   while (gpio_get(echo_pin) == 1) {
     if (time_us_64() - start_time > TIMEOUT) {
       printf("Your shit is doomed!\n");
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

int wav_position = 0;

void pwm_interrupt_handler() {
   //ISR Function for managing the interrupt
   
   pwm_clear_irq(pwm_gpio_to_slice_num(SPEAKER_PIN));

   //JUST TO BE CLEAR:
   /* This portion of the code functions like a loop
   By using the interrupt, it can keep calling the hardware 
   over and over until certain conditions have been met
   */                              //Multiply by 8
   if (wav_position < (WAV_DATA_LENGTH<<3) - 1) {
     
     //How does this work?

     /* IN PWM, we have cycles. A cycle is basically something wherein 
     it is a measurement of an on and off time cycle

     In 1 PWM cycle, they decode a sound
     */
     pwm_set_gpio_level(SPEAKER_PIN, WAV_DATA[wav_position>>3]); //Divide by 8
     wav_position++;
   }
   else {
     //Reset if it reaches the very end of array
     wav_position = 0;
   }

}

int pwm_speaker() {
 gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);

 //Converts our GPIO pin to a slice num
 uint8_t slice_num = pwm_gpio_to_slice_num(SPEAKER_PIN);

 //Set the PWM interrupt
 pwm_clear_irq(slice_num);
 pwm_set_irq_enabled(slice_num, true);

 //Setting up the interrupt handler
 irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler);
 irq_set_enabled(PWM_IRQ_WRAP, true);

 //If we set the clock divider to 1, then it has a maximum freq
 // of 125mhz
  
 pwm_config le_conf = pwm_get_default_config();
 pwm_config_set_clkdiv(&le_conf, CLKDIV);
 //PWM freq = clock freq/(wrap + 1) * clock divider val
 //So, if we want 10kHz output from the rp2040:
   //10kHz = 125,000,000 Hz / (wrap + 1) * 1.0
   //To find the wrap val, you may just rearrange the equation via algebra:
   //wrap = (125,000,000 / 10000) - 1
 pwm_config_set_wrap(&le_conf, WRAPVAL);
 
 pwm_init(slice_num, &le_conf, true);

 pwm_set_gpio_level(SPEAKER_PIN, 0);

 while (1) {
  __wfi();
 }

 return 0;
}

int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high) {
  return (distance - starting_low) * (ending_high - ending_low) / (starting_high - starting_low) + ending_low;
}

int main() {
 //Initialize I/O
 stdio_init_all();
 
 setup_pins();

  int buzzer_delay;

  //Flag for calling PWM
  bool pwm_is_active = false;

  while (true) {
   uint64_t distance = get_cm(TRIG_PIN, ECHO_PIN);
   
    if (distance >= THRESHOLD AND distance <= 50) {
      //Call PWM signal for activating the speaker
      if (!pwm_is_active) {
        pwm_speaker();
        pwm_is_active = true;
      }
    }
    else if (distance < THRESHOLD) {
      buzzer_delay = map(distance, 0, THRESHOLD, BUZZER_MIN, BUZZER_MAX);
    
      gpio_put(BUZZER_PIN, 1);
      sleep_ms(buzzer_delay);
      gpio_put(BUZZER_PIN, 0);
      sleep_ms(buzzer_delay);
      
      pwm_is_active = true;
   
    }
   else {
     pwm_is_active = true;
     gpio_put(BUZZER_PIN, 0);
   }

   sleep_ms(1);
   printf("Distance: %lld cm\n", distance);
  }

} 