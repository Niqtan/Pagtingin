# Pagtingin (Obstacle Detecting Glasses)

![Project Wearable](PicoPrototype/images/Glasses.jpg)
Image 1. Pagtingin Prototype V1

## Prototype Demonstration
https://www.youtube.com/watch?v=mGvRfI-a0dM

## Project Aims

The Pagtingin obstacle detecting glasses aims to help visually impaired people to easily detect obstacles in front of them through innovative glasses. While traditional walking sticks indeed help in detecting obstacles on the ground, they often fail to identify objects at chest or head level, such as low-hanging branches, open cabinets, or protruding signs. 

Hence, Pagtingin aims to aid this limitation and ensure a safe journey of navigation. Whether indoors or outdoors, the device will be able to identify potential hazards through a Time-of-Flight sensor and send real-time audio feedback, which helps users to be much more confident and safe in the user's day-to-day lives.

(+ I wanna try walking blindly sometimes)

![Blind person walking happily](PicoPrototype/images/blind-man-people-with-disability-handicapped-person-everyday-life-visually-impaired-man-with-walking-stick-descending-steps-city-park.jpg)

Courtesy of Freepik

## PCB Version
![PCB front view](PCB/image.png)
Image 2. PCB Front View

![PCB Back View](PCB/image-1.png)
Image 3. PCB Back View

## Panelized Version

![alt text](PCB/Pagtingin%20Gerber%20Panelized/Untitled_Combined_Top.png)
Image 3. Panelized PCB Front View

![alt text](PCB/Pagtingin%20Gerber%20Panelized/Untitled_Combined_Bottom.png)
Image 4. Panelized PCB Back View

## CAD

### Glasses Frame

![alt text](CADDesign/framefront.png)
Image 5. Glasses Frame Front View

![alt text](CADDesign/frametop.png)
Image 6. Glasses Frame Top View

![alt text](CADDesign/frameback.png)
Image 7. Glasses Frame Back View

### Temples

![alt text](CADDesign/templeside.png)
Image 8. Glasses Temple Side View

![alt text](CADDesign/templetop.png)
Image 9. Glasses Temple Top View

![alt text](CADDesign/templebottom.png)
Image 10. Glasses Temple Bottom View

## 3D Printing
In terms of 3D printing, it might be better to print this in resin since the glasses frame will require precise dimensions. You may be able to print this in FDM, but it is recommended for the parameters to be as precise as possible.

## BOM
https://docs.google.com/spreadsheets/d/17i14ZackUdraa6Qyv3OiZuTYL3An4RdyssWMnPNHM5A/edit?usp=sharing

## Firmware
Still in progress. You may see previous prototypes in the folder for previous version's firmware. But for the firmware, I am planning to use the ESP-IDF in programming it. I chose the ESP-IDF because it allows full control over the ESP32-PICO-D4's peripherals and such. (also because I wanna challenge myself)