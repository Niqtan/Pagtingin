// constant variables for pins
const int trigPin = 3;
const int echoPin = 2;
const int buzzerPin = 22;

int threshold = 30; // threshold distance to activate buzzer
// buzzer delay variables
int buzzerDelay;    
int buzzerDelayMin = 10;
int buzzerDelayMax = 250; 

long duration; // ultrasonic return pulse duration 
long cm;       // distance in centimeters

void setup() { // setup code that only runs once
  // set pin modes
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(buzzerPin,OUTPUT);
  // uncomment the next line to user serial print for debugging
  // Serial.begin(9600);
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
