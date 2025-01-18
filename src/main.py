"""
--------------------- PAGTINGIN ---------------------
Here are some instructions in order to setup the project
1. Setup the raspberry pi pico W a
    - To do this, we'll need to confirm the circuit and how it will
    - interact with other components
2. Next, setup the HC-SR04 sensor and try to get readings of the ultrasonic sensor
    - In order to do this we'll need to calculate the distance and time
3. Next, after setting up the HC-SR04 sensor, we'll use the piezzo buzzer
as output.
    - In order to do this, we'll use control flows like if conditions
    - in order to check the distance of an obstacle.
    - For example, during the 30cm mark, we'll use weak beeping and 
    - try to warn the human
    - However, when approaching the 20cm mark, it will be much more
    - intense, and the same goes for the 10cm mark.
4. After so, we'll confirm the aftershocks and try to gather results and
test it out before attaching it to the hat.
"""

from machine import Pin
import utime

trig = Pin(14, Pin.OUT)
echo = Pin(12, Pin.IN)
buzzer = Pin(25, Pin.OUT)

buzzer_delay_min = 10
buzzer_delay_max = 250
threshold = 30

# Will return a integer
def convert(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def play_and_sound(distance):
    #Plays and vibrates depending on the distance
    if distance < threshold:
        buzzer_delay = convert(distance, 0, threshold, buzzer_delay_min, buzzer_delay_max)
        buzzer.value(1)
        utime.sleep_us(buzzer_delay)
        buzzer.value(0)
        utime.sleep_us(buzzer_delay)
    else:
        # If not within specified distance, then it becomes inactive
        buzzer.value(0)

try:
    while True:
        #Timing for the TRIG pin to send pulse
        trig.value(0)
        utime.sleep_us(2)
        trig.value(1)
        utime.sleep_us(10)
        trig.value(0)
        
        echo_stop_time = 0
        echo_start_time = 0

        #Wait for the ECHO PIN to go HIGH
        while echo.value() == 0:
            echo_start_time = utime.ticks_us()
        
        #If ECHO pin does go high, record start time
        while echo.value() == 1:
            echo_stop_time = utime.ticks_us()

        #Ignore the ECHO pin going HIGH and record the stop_time when
        #it goes low
        if echo_start_time > 0 and echo_stop_time > 0:
            #Calculate for the time for the sound
            ping_travel_time = echo_stop_time - echo_start_time

            #Convert it to distance cm by time * speed of sound / 2
            #With the average temperature being 30Â°C
            dist_cm = int((ping_travel_time * 0.0343) / 2)

            #Sound off the piezzo buzzer
            play_and_sound(dist_cm)

            print("Distance = ", dist_cm)

except KeyboardInterrupt:
    #Reset all the pins 
    trig.init(Pin.IN)
    echo.init(Pin.IN)
    buzzer.deinit()
    print("Cleaned up")