#include "main.h"

//Include the arrays consisting of the decoded audio files
#include "alert.h"

void setup_pins(void) {
 gpio_init(ECHO_PIN);
 gpio_set_dir(ECHO_PIN, GPIO_IN);

 gpio_init(TRIG_PIN);
 gpio_set_dir(TRIG_PIN, GPIO_OUT);

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
       printf("You're Cooked!\n");
       return 0;
     }
   }

   uint64_t echo_start_time = time_us_64();
   
   while (gpio_get(echo_pin) == 1) {
     if (time_us_64() - start_time > TIMEOUT) {
       printf("You're Cooked!\n");
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

//Slice num for getting the slice num
uint8_t pwm_slice_num;

//Flag for calling PWM
volatile bool pwm_flag = false;

void interrupt_initialize() {
  gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);
  
  //Converts our GPIO pin to a slice num
  pwm_slice_num = pwm_gpio_to_slice_num(SPEAKER_PIN);
 
  /* Explanation of PWM interrupt:
  Trigger: the RP2040 will send out a PWM signal (i.e. the interrupt handler) once the PWM 
  wraps around or reaches the wrap limit 
  */

  pwm_clear_irq(pwm_slice_num);
  pwm_set_irq_enabled(pwm_slice_num, true);

  irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler);
  irq_set_priority(PWM_IRQ_WRAP, 0);
  irq_set_enabled(PWM_IRQ_WRAP, true);

  pwm_config le_conf = pwm_get_default_config();
  pwm_config_set_clkdiv(&le_conf, CLKDIV);
  /*PWM freq = clock freq/(wrap + 1) * clock divider val
  So, if we have a 11k sample rate:
    *Since we have a 11k sample rate, we want 4x of that for the PWM freq*
    44kHz = 125,000,000 Hz / (wrap + 1) * 8.0
    To find the wrap val, you may just rearrange the equation via algebra:
    wrap = (125,000,000 / 44000 * 8) - 1
    wrap = 354
  */

  pwm_config_set_wrap(&le_conf, WRAPVAL);   

  pwm_init(pwm_slice_num, &le_conf, false);

  pwm_set_gpio_level(SPEAKER_PIN, 0);

  /* Explanation of buzzer interrupt:
  Since the buzzer interrupt has a higher priority than the PWM interrupt,
  once the processor detects 
  */
  gpio_set_irq_enabled(BUZZER_PIN, GPIO_IRQ_EDGE_RISE, true);
  irq_set_exclusive_handler(IO_IRQ_BANK0, buzzer_cook_handler);
  irq_set_priority(IO_IRQ_BANK0, 1);
  irq_set_enabled(IO_IRQ_BANK0, true);

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
    //This decodes each element in the array of obstacle.h, and that will repeat for 8 times
    // for each PWM cycle. As a result, it will release the audio.
     pwm_set_gpio_level(SPEAKER_PIN, (WAV_DATA[wav_position>>3])); //Divide by 8
     wav_position++;
   }
   else {
     //Reset if it reaches the very end of array
     wav_position = 0;
     pwm_flag = false;
   }
}

 uint64_t distance;

volatile bool buzz_flag =  false;
int buzzer_delay;

void buzzer_cook_handler() {

gpio_acknowledge_irq(BUZZER_PIN, GPIO_IRQ_EDGE_RISE);

buzzer_delay = map(distance, 0, THRESHOLD, BUZZER_MIN, BUZZER_MAX);

//Use a flag to signal that it should be working
buzz_flag = true;
}


int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high) {
  return (distance - starting_low) * (ending_high - ending_low) / (starting_high - starting_low) + ending_low;
}

int main() {
 //Initialize I/O
 stdio_init_all();
 
 setup_pins();

 interrupt_initialize();

  while (true) {
    distance = get_cm(TRIG_PIN, ECHO_PIN);
    if (distance < THRESHOLD) {

      pwm_flag = false;
      
      gpio_put(BUZZER_PIN, 1);
      pwm_set_enabled(pwm_slice_num, pwm_flag);   
      printf("PWM is set to false\n");
      if (buzz_flag == true) {
        gpio_put(BUZZER_PIN, 1);
        sleep_ms(buzzer_delay);
        gpio_put(BUZZER_PIN, 0);
        sleep_ms(buzzer_delay); 
        buzz_flag = false;
      }
    }
    else if (distance >= THRESHOLD AND distance <= 70) {
      //Call PWM signal for activating the speaker
      gpio_put(BUZZER_PIN, 0);
      if (!pwm_flag) {
        pwm_flag = true;
        pwm_set_enabled(pwm_slice_num, pwm_flag);
        printf("PWM is set to true\n");
      }
    }
   else {
    pwm_flag = false;
     gpio_put(BUZZER_PIN, 0);
     pwm_set_enabled(pwm_slice_num, pwm_flag);
     printf("PWM is set to false w/ buzzer \n");
   }


  sleep_ms(10);
  printf("Distance: %lld cm\n", distance);
  }

} 