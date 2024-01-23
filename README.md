# Motor Control System with HTML Interface over Ethernet

This repository contains the code and instructions for creating a system to control three motors via an HTML control panel over Ethernet. The system utilizes Arduino boards to interface with the motors and an HTML control panel to send commands to the Arduino boards.

## Prerequisites
- Arduino IDE
- An Ethernet-enabled Arduino board (e.g., Arduino Uno with Ethernet Shield)
- Three DC motors and corrosponding L298N motor drivers (others can be used but pin out may be different)
- Ethernet connection for the Arduino board
- Web browser for accessing the HTML control panel

## Arduino Code

### Dual Motor Control (ArduinoDualMotorHTTPControl_Granular.ino)
- This Arduino sketch provides control for two motors.
- The default IP for the Arduino running this code is `192.168.1.177`
  - This can be changed by changing the `IPAddress ip(192, 168, 1, 177);` line in the code
- The supported HTTP GET requests are:
  - `/forwardL?speed=x` - Move the left motor forward at speed `x`.
  - `/backwardL?speed=x` - Move the left motor backward at speed `x`.
  - `/stopL` - Stop the left motor.
  - `/forwardR?speed=x` - Move the right motor forward at speed `x`.
  - `/backwardR?speed=x` - Move the right motor backward at speed `x`.
  - `/stopR` - Stop the right motor.
- You can add more requests by changing adding/removing items from the `commands[]` array
  - each item has 2 items
  - A string item which will contain the text the `executeMotorCommand()` function with search for in the GET request
  - And a function pointer which will contain just the name of the function you want to execute when it finds the string in the request (eg. `GET /forwardL?speed=255` will execute the `forwardL()` function and pass 255 as the speed variable
  - example: `struct Command { {"newMotorCommand", newMotorFunction}}`
  - You dont have to pass a speed variable when creating a new command however it is defined in the struct that there is one so VSCode with throw a warning at compilation that you can ignore
  
### Single Motor Control (ArduinoAltitudeMotorHTTPControl_Granular.ino)
- This Arduino sketch provides control for a single motor.
- The default IP for the Arduino running this code is `192.168.1.178`
  - This can be changed by changing the `IPAddress ip(192, 168, 1, 178);` line in the code
- The supported HTTP GET requests are:
  - `/up?speed=x` - Move the motor forward at speed `x`.
  - `/down?speed=x` - Move the motor backward at speed `x`.
  - `/stopV` - Stop the motor.
- You can add more requests by changing adding/removing items from the `commands[]` array
  - each item has 2 items
  - A string item which will contain the text the `executeMotorCommand()` function with search for in the GET request
  - And a function pointer which will contain just the name of the function you want to execute when it finds the string in the request (eg. `GET /up?speed=255` will execute the `up()` function and pass 255 as the speed variable
  - example: `struct Command { {"newMotorCommand", newMotorFunction}}`
  - You dont have to pass a speed variable when creating a new command however it is defined in the struct that there is one so VSCode with throw a warning at compilation that you can ignore


## HTML Control Panel
- The HTML control panel (W2ControlPanelV0_18.html) allows users to send HTTP requests to the Arduino board to control the motors.
- To use the control panel, open the HTML file in a web browser and interact with the provided controls to send HTTP requests to the Arduino board.

## Getting Started
1. Upload the appropriate Arduino code to your Ethernet-enabled Arduino board using the Arduino IDE or VSCode using the `PlatformIO` extension.
2. Connect the motors and motor drivers to the Arduino board as per the code and your motor specifications.
3. Ensure that the Arduino board is connected to the Ethernet network.
4. Access the HTML control panel in a web browser to start controlling the motors.

## Controls
- As of right now (Jan 23, 2024) only keyboard controls are implemented, controller may come at a later date
- W: fires both side thrusters forward at the set speed
- S: fires both side thrusters backward at the set speed
- A: fires the left thruster backward at the set speed and the right one forward at the set speed
- D: fires the right thruster backward at the set speed and the left one forward at the set speed

---
