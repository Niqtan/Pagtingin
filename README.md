# Pagtingin (Obstacle Detecting Glasses)

![Project Wearable](images/Glasses.jpg)

## Demonstration
https://www.youtube.com/watch?v=mGvRfI-a0dM

## Project Aims

The Pagtingin obstacle detecting glasses aims to help visually impaired people to easily detect obstacles in front of them through innovative glasses. While traditional walking sticks indeed help in detecting obstacles on the ground, they often fail to identify objects at chest or head level, such as low-hanging branches, open cabinets, or protruding signs. 

Hence, Pagtingin aims to aid this limitation and ensure a safe journey of navigation. Whether indoors or outdoors, the device will be able to identify potential hazards through an ultrasonic sensor and send real-time audio or tactile feedback, which helps users to be much more confident and safe in the user's day-to-day lives.

![Blind person walking happily](images/blind-man-people-with-disability-handicapped-person-everyday-life-visually-impaired-man-with-walking-stick-descending-steps-city-park.jpg)

Courtesy of Freepik

## Contents of the Project

In my proposed system, an ultrasonic sensor is placed in front of the user's glasses. The ultrasonic sensor will provide information to the glasses before audio or tactile feedback will be sent to the user.

![Ultrasonic sensor detection](images/download.jpeg)

## How does it work?

### Detection
Whenever the glasses detects an obstacle in front of the user within the 50 centimeter range, the ultrasonic sensor will send a signal to the glasses for readings.

## Feedback
After the detection phase, the glasses will use the ultrasonic readings in order to release an audio feedback relative to the distance in centimeters (cm) of the ultrasonic sensor to an obstacle.

If the obstacle is close enough (roughly 70cm) then the glasses will alert via a voice message. Once the user approaches within the 30cm mark,the buzzer will beep fasteer and faster as the user gets close.

## Flashing on the Raspberry Pi Pico
1. Hold the BOOTSEL button of the Raspberry Pi Pico
2. Simultaneously, whilst holding it, connect a USB cable to the Pico and let go of the BOOTSEL afterwards
3. Drag and drop the main.uf2 file on the drive of the Raspberry Pi Pico
Pico

After making the circuit, you may mount  this to other accessories as well.

## Learning Outcomes

By the end of the project, I wish to strenghthen my understanding of these topics:

- Understand the basics of programming programming a Raspberry Pi Pico, specifically in its C/C++ SDK.
- Apply the basics of circuit theory such as op amps, RC Low pass filters, and electrical circuit applications.
- Understand embedded systems concepts, such as communication protocols, interrupts, soldering, etc.
- Experience how to make a device with an embedded system, and the ergonomics behind them.
Love engineeeing more

## A Message
Oh.. engineering.... Initially, I thought this project would be easy, yet it seems that wouldn't be the case. Almost 4 months working on it oftenly, prototype's finished.

Some challenges? Definitely the speaker part haha. I got stuck with that problem for about 3 weeks. Turns out, my speaker was too weak to drive voice, so I just switched to a tune. Anyways, that was really the only problem which almost made me rip my hair apart. Thanks for tuning for this part of the readme.

## Project Status
The project is currently in its prototype form. An improved version will have more features and improvements to the design of the system coming soon. 

## Support

For questions, suggestions, or collaborations, feel free to contact the maintainer:

Niq Suguitan

- Github: @Niqtan

- Discord Username: snorelaxe14

- Email: niqban123@gmail.com

Thank you for checking out Pagtingin -- empowering with vision!
