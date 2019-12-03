# Hagrid's Handlebar

A Arduino/openFrameworks project to create a controller that emulates a motorcycle handle.

The 3D environment is created in openFrameworks, using Firmata to transmit signal from the controller.

## Setup Instructions

- Set up the your arduino based on the schematic included
- Upload the ModifiedFirmata example to the board
- Open the openFrameworks solution and change the PORT variable in ofApp.h to whatever port you are using.
- You're good to go!

## Sensors

- ADXL345 Accelerometer
- ~~Rubber Stretch Resistor~~
- FSR Pressure Sensor

## Demo

[![Demo of the project](https://img.youtube.com/vi/VID/0.jpg)](https://www.youtube.com/watch?v=PPxQZJnwMJ8&feature=youtu.be)
