// constant variables for pins
#include "main.h"


int threshold = 30; // threshold distance to activate buzzer
// buzzer delay variables
int buzzerDelay;    
int buzzerDelayMin = 10;
int buzzerDelayMax = 250; 

long duration; // ultrasonic return pulse duration 
long cm;       // distance in centimeters

void setup_pins(void) {
  gpio_init(ECHO_PIN);
  gpio_set_dir(ECHO_PIN, GPIO_IN);

  gpio_init(SPEAKER_PIN);
  gpio_set_dir(SPEAKER_PIN, GPIO_OUT);
  
  gpio_init(TRIG_PIN);
  gpio_set_dir(TRIG_PIN, GPIO_OUT);
}

int main() {
  setup_pins();

  
}

void loop() {

  // send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  // read echo pulse
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  // control the buzzer
  if(cm<threshold){ // if an object is closer than the threshold
    // map the distance to the buzzer delay time
    buzzerDelay = map(cm,0,threshold,buzzerDelayMin,buzzerDelayMax);
    // beep the buzzer
    digitalWrite(buzzerPin,HIGH);
    delay(buzzerDelay);
    digitalWrite(buzzerPin,LOW);
    delay(buzzerDelay);
  }
  else{ // if an ob ject is farther away than the threshold
    digitalWrite(buzzerPin,LOW); // turn buzzer off
  }

  // uncomment the next line for debugging
  Serial.println(cm);

}


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
