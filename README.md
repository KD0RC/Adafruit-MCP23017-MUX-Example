# Adafruit MCP23017 MUX Example
A quick example of how to read button presses with an Adafruit MCP23017 MUX board.

This example uses the MCP23017 interrupts to know which button was pressed.
The Teensy or Arduino does not use interrupts - this is a polled process.
There are no delay() statements in this code, so holding a button down won't
block whatever else your code is doing.  This accommodates long or short presses
to double the number of functions that can be activated with 16 button inputs.
