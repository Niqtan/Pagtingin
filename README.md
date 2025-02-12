
# Pagtingin (Obstacle Detecting Glasses)

## Project Aims

The Pagtingin obstacle detecting glasses aims to help visually impaired people to easily detect obstacles in front of them through innovative glasses. While traditional walking sticks indeed help in detecting obstacles on the ground, they often fail to identify objects at chest or head level, such as low-hanging branches, open cabinets, or protruding signs. 

Hence, Pagtingin aims to aid this limitation and ensure a safe journey of navigation. Whether indoors or outdoors, the device will be able to identify potential hazards through an ultrasonic sensor and send real-time audio or tactile feedback, which helps users to be much more confident and safe in the user's day-to-day lives.

![Blind person walking happily](https://github.com/Niqtan/Pagtingin/blob/main/blind-man-people-with-disability-handicapped-person-everyday-life-visually-impaired-man-with-walking-stick-descending-steps-city-park.jpg)

## Contents of the Project

In my proposed system, an ultrasonic sensor is placed in front of the user's glasses. The ultrasonic sensor will provide information to the glasses before audio or tactile feedback will be sent to the user.

![Ultrasonic sensor detection](https://github.com/Niqtan/Pagtingin/blob/main/download.jpeg)

## How does it work?

### Detection
Whenever the glasses detects an obstacle in front of the user within the 30 centimeter range, the ultrasonic sensor will send a signal to the glasses for readings.

### Feedback
After the detection phase, the glasses will use the signal in order to release an audio or tactile feedback relative to the distance in cm of the ultrasonic sensor to an obstacle.

## Circuit Diagram
![KiCad circuit diagram](https://github.com/Niqtan/Pagtingin/blob/main/Screenshot%202025-01-28%20172207.png)

## Parts Needed
- 1 * Raspberry Pi Pico 
- 1 * 9V Alkaline Battery
- 1 * Short Solderless Breadboard
 - 1 * HR-SR04 Ultrasonic distance sensor
- 1 * 0.5W, 8Ω Portable Speaker
- 1 * TDA2822M Integrated Circuit (IC)
- 2 * 1µF, one ceramic and one electrolytic
- 1 * 32Resistor, 4 * 10KΩ Resistor  
- A bundle of jumper wires


## Flashing on the ESP32
1.  Connect the Raspberry Pi Pico to the computer using a cable
2.  Install the development board for the Raspberry Pi Pico on the Arduino IDE.
3.  Select Raspberry Pi PIco Development Board 
4. Select the COM Port on the Arduino IDE
5. Click compile and upload

## Learning Outcomes

By the end of the project, I wish to strenghthen my understanding of these topics:

- Understand the basics of programming in embedded systems, specifically in the Arduino IDE and alternative ways of programming a microcontroller
- Apply the basics of circuit theory such as op amps, transistors, and electrical circuit applications.
- Experience how to make a device with an embedded system, and the ergonomics behind them.

## To do list - Valid 02-02-2025
- Try to implement the code in the Pico SDK
    - Implement the PWM code first to see which period would the best
    - Try to find a library or some sort of code for the HR-SR04
- How to use a NPN transistor to amplify audio signal:
    - https://www.instructables.com/Simple-Audio-Amplifier-Using-Single-Transistor/
- How to use the TDA2822M for amplifying audio:
    - https://www.utmel.com/components/tda2822m-low-voltage-power-amplifier-datasheet-pinout-and-circuit?id=702
- Calculations for the 8 Ohm 0.5W speaker:
    - https://electronics.stackexchange.com/questions/129277/how-many-volts-can-a-1-watt-8-ohm-speaker-take



## Project Status
The project is currently in its prototype form. An improved version will have more features and improvements to the design of the system coming soon. 

## Support

For questions, suggestions, or collaborations, feel free to contact the maintainer:

Niq Suguitan

- Github: @Niqtan

- Discord Username: snorelaxe14

- Email: niqban123@gmail.com

Thank you for checking out Pagtingin -- empowering with vision!
