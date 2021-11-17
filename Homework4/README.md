## Homework 4

Task: First state: you can use a joystick axis to cycle through the 4digits; using the other axis does nothing. A blinking decimal point shows the current digit position. When pressing the button (must use interrupt for maximum points!), you lock in on the selected digit and enter thesecond state. In this state, the decimal point stays always on, no longer blinking and you can no longer use the axis to cycle through the 4 digits. Instead, using the other axis, you can increment on decrement the number on the current digit. Pressing the button again returns you to the previous state. For bonus, save the last value in eeprom and load it when startingarduino. Also, keep in mind that when changing the number, you must increment it for each joystick movement - it should not work continuosly increment if you keep the joystick in one position.

Components:
 * 1 4-Digit 7-segment display
 * 1 joystick
 * 4 330 ohm resistors for each 7 segment display pin
 * 1 74hc595 shift register
 * wires

Setup:
![homework4](https://user-images.githubusercontent.com/39190235/142289010-226c4115-f7da-4775-bf23-d911f6b14b7e.jpg)


Video showcase: https://www.youtube.com/watch?v=SEu7q-twRSA
