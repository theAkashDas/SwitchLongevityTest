# Switch Longevity Test

_*Use Case*_ - To check how long a push button switch last by automating the switch pressing action by a servo.

### Components used 
  - STM32F103C8
  - SG90 Servo
  - DS3231 EEPROM
  - ST Link
  - FTDI 
  - Push Button to test.
  - Connecting wires and breadboard.

<img src = "Extras/IMG_20210924_195256.jpg" width = "400"> 

### Working Flow

##### Setup 
  - First the code will do the required setup. 
  - Initialization of servo motor.
  - Initialization of pins.
  - Read the memory at specific address to get previous stored value.

##### Loop
  - Read memory for the stored value after 25 secs to reduce eeprom wear.
  - digital Read PB4 state.
  - if stateChange variable is not updated in last 20 sec, stopFunction set to 1.
  - if stopFucntion is not set to 1, the servo is used to press the switch.
  - count is the total no. of times switch is pressed.
  - cycle is the number of ON-OFF cycle done.
  - updating eeprom data after every 25 secs.
  - updating stateChange with a specific condition of pb4 status.
  - repeating all over again.


Changed the code flow a little. Working Fine. Now need to optimise the code and architect the code structure.


