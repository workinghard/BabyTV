# BabyTV
BabyTV running on Atmega328p and using WS2812B display

![alt text][logo]

## Index
 1. [Overview](#overview)
 2. [Hardware](#hardware)
 3. [Software](#software)
 4. [3D Parts](#3dparts)
 5. [Full Partlist](#partlist)

## Overview
Display a nice looking pixel grafic on a 16x16 matrix. Both static images and animations are possible and implemented. Let your child switch between images/animations with a remote. Control brightness and switch between "channels" with two knobs.  

You can [watch a demo on youtube](https://youtu.be/bw_D5chaiGM)

This project is inspired by others which are using Neopixel matrix and developed already stable and fast working frameworks for arduino. 

## Hardware
A 16x16 neopixel matrix with a WS2812B controller is being used as a display. It is driven by an arduino nano board with a atmega328p chip. One rotary encoder knob allows to switch between "channels" back and forth. One potentiometer is being used for brightness control. The infrared sensor is picking up any signals and switching a channel based on it.

![alt text][ElectronicsOverview]

![alt text][ElectronicsMainboard]

[Full schematics can be found in the project](https://github.com/workinghard/BabyTV/raw/master/Schematics/BabyTV.pdf)

## Software
FastLed framework was picked for fast animations. Each channel (Image/Animation) is in a separate c-file. A rotary encoder framework is based on a software scan. A hardware interrupt for RE was not being used to guarantee flawless communication with the display. No additional dependencies are required. Full source code can be found in the project.  
 
## 3Dparts
TODO

----

##Partlist



[logo]: https://github.com/workinghard/BabyTV/raw/master/images/BabyTV_Icon.png "Logo"
[demo]: http://img.youtube.com/vi/bw_D5chaiGM/0.jpg "Demo"
[ElectronicsOverview]: https://github.com/workinghard/BabyTV/raw/master/images/BabyTV_Electronics_Overview.JPG
[ElectronicsMainboard]: https://github.com/workinghard/BabyTV/raw/master/images/BabyTV_Electronics_Mainboard.JPG
