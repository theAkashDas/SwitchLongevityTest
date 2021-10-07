# Switch Longevity Test

*Use Case* - To check how long a push button switch last by automating the switch pressing action by a servo.

### Components used 
  - STM32F103C8
  - SG90 Servo
  - DS3231 EEPROM
  - ST Link
  - FTDI 
  - Push Button to test.
  - Connecting wires and breadboard.


### Working Flow

###### Setup 
  - First the code will do the required setup. 
  - Initialization of servo motor.
  - Initialization of pins.
  - Read the memory at specific address to get previous stored value.

###### Loop
  - Read memory for the stored value.
  - Check whether state of PB4 has changed in the last 20 sec.


