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
FastLed framework was picked for fast animations. Each channel (Image/Animation) is in a separate c-file. A rotary encoder framework is based on a software scan. A hardware interrupt for RE was not being used to guarantee flawless communication with the display. No additional dependencies are required. Full source code can be found in the Arduino folder.  
 
## 3Dparts
All needed 3D parts can be found in the STL folder or on [Thingiverse](https://www.thingiverse.com/thing:2790910). Using PLA filament there should be no dimensional issues. If you are using ABS or any material which shrinks, you will most likely need to scale the parts up.

I printed body and Back with PLA and everything else with ABS on my [Original Prusa i3 MK2S](https://shop.prusa3d.com/en/) 

----

## Partlist
 * 3D printer with at least 200x200mm build plate [like Original Prusa i3 MK3](https://shop.prusa3d.com/en/)
 * 16x16 pixel board with WS2812B chipset [like this on Amazon](https://www.amazon.com/gp/product/B01DC0IOCK)
 * Arduino nano with atmega328p chipset [like this on Aliexpress](https://www.aliexpress.com/item/Nano-CH340-ATmega328P-MicroUSB-Compatible-for-Arduino-Nano-V3-0/32740641316.html)
 * Rotary Encoder [like this on Aliexpress](https://www.aliexpress.com/item/Free-Shipping-10pcs-lot-Rotary-encoder-switch-five-feet-short-handle-Best-quality/32543561073.html)
 * 5V/10A Power supply [like this on Amazon](https://www.amazon.com/ALITOVE-Transformer-Converter-5-5x2-1mm-100V-240V/dp/B01M0KLECZ)
 * 2x 100nF Capacitor
 * 4x 10k Ohm Resistor
 * Any linear potentiometer
 * Any infrared receiver
 

[logo]: https://github.com/workinghard/BabyTV/raw/master/images/BabyTV_Icon.png "Logo"
[demo]: http://img.youtube.com/vi/bw_D5chaiGM/0.jpg "Demo"
[ElectronicsOverview]: https://github.com/workinghard/BabyTV/raw/master/images/BabyTV_Electronics_Overview.JPG
[ElectronicsMainboard]: https://github.com/workinghard/BabyTV/raw/master/images/BabyTV_Electronics_Mainboard.JPG
