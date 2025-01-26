
# Pagtingin (Obstacle Detecting Glasses)

## Project Aims

Pagtingin aims to help visually impaired people to easily detect obstacles in front of them through innovative technology. By using an ultrasonic sensor and real-time feedback, Pagtingin ensures a safe journey of navigation. Whether indoors or outdoors, the device will be able to identify potential hazards and send real-time audio or tactile feedback, which helps users to be much more confident and safe in their day-to-day lives.

## Contents of the Project

In my proposed system, an ultrasonic sensor is placed in front of the user's glasses. The ultrasonic sensor will provide information to the glasses before audio or tactile feedback will be sent to the user.

## How does it work?

### Detection
Whenever the glasses detects an obstacle in front of the user within the 30 centimeter range, the ultrasonic sensor will send a signal to the glasses for readings.

### Feedback
After the detection phase, the glasses will use the signal in order to release an audio or tactile feedback relative to the distance in cm of the ultrasonic sensor to an obstacle.

## Circuit Diagram
image here

## Parts Needed
- 1 * ESP32 Development Board WROOM
- 1 * 9V Alkaline Battery
- 1 * HR-SR04 Ultrasonic distance sensor
- 1 * Short Solderless Breadboard
- 1 * Active Buzzer
- 1 * LM358 Operational Amplifier
- 1 * 2N2222 NPN Transistor
- 1 * USB to Dual 9V Battery
- 1 * 1MΩ Resistor, 2 * 10KΩ Resistor  
- A bundle of jumper wires


## Flashing on the ESP32
- 1. Connect the ESP32 WROOM to the computer using a cable
- 2. Install the development board for the ESP32 on the Arduino IDE.
- 3. Select ESP32 DEV BOARD 
- 4. Select the COM Port on the Arduino IDE
- 5. Click compile and upload

## Learning Outcomes

By the end of the project, I wish to strenghthen my understanding of these topics:

- Understand the basics of programming in embedded systems, specifically in micropython and alternative ways of programming a microcontroller
- Apply the basics of circuit theory such as op amps, transistors, and electrical circuit applications.
- Experience how to make a device with an embedded system, and the ergonomics behind them.

## Project Status
The project is currently in its prototype form. An improved version will have more features and improvements to the design of the system coming soon. 

## Support

For questions, suggestions, or collaborations, feel free to contact the maintainer:

Niq Suguitan

- Github: @Niqtan

- Discord Username: snorelaxe14

- Email: niqban123@gmail.com

Thank you for checking out Pagtingin -- empowering with vision!
