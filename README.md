# Hagrid's Handlebar

There's three branches so upload your work to each one. The code I've got on the master branch is just the bare minimum to get sensor response from the arduino.

I've included the sketch that gets the sensors working in arduino IDE in src/sensorFunctionalityTest.ino.

## TODO
- Get sensor input working in Visual Studio with OF
- Figure if i2C will work with OF/find work around
- Build 3D world
- Incorporate some type of game functionality

## Setup Instructions

- Set up the your arduino based on the schematic included
- Upload the StandardFirmata example that comes default with the arduino IDE to your board (I've also included it in src/StandardFirmata).
- Open the openFrameworks solution and change the PORT variable in ofApp.h to whatever port you are using.
- You're good to go!

## Sensors

- ADXL345 Accelerometer
- Rubber Stretch Resistor
- FSR Pressure Sensor
