/*
  MCP23017Example_Int
   Len Koppl, KDØRC
   03/18/23
   Teensy 4.1

*/
// This example uses the MCP23017 interrupts to know which button was pressed.
// The Teensy or Arduino does not use interrupts - this is a polled process.
// There are no delay() statements in this code, so holding a button down won't
// block whatever else your code is doing.  This accommodates long or short presses
// to double the number of functions that can be activated with 16 button inputs.

// *******************************  Includes  *******************************
#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mux;

// *******************************  Constants and Defines  *******************************
const byte BTN_INT_PIN         = 32;   // Teensy or Arduino pin.  Connect to Int A or B of MCP23017
const unsigned long DEBOUNCE   = 50;   // Debounce time in ms
const unsigned long LONG_PRESS = 250;  // Greater than 250 ms defines a long press

// *******************************  Global Variables  *******************************
bool shortPress;  // Global so it can be set in GetButton() then used anywhere

// *******************************  setup()  *******************************
void setup()
{
  Serial.begin(9600);  // For compatibility with non-Teensy boards

  if (!mux.begin_I2C())
  {
    Serial.println("I2C MUX initialization failure");
  }

  pinMode(BTN_INT_PIN, INPUT_PULLUP);     // Teensy interrupt pin
  mux.setupInterrupts(true, false, LOW);  // mirroring (intA/B), not openDrain, polarity LOW

  for (int i = 0; i < 16; i++)  // Assume all pins are input on this MUX
  {
    mux.pinMode(i, INPUT_PULLUP);
    mux.setupInterruptPin(i, LOW);
  }

  mux.clearInterrupts();  // Get rid of any lingering crud
}  // end setuo()

// *******************************  loop()  *******************************
void loop()
{
  int button;

  button = GetButton();  // Poll for button presses.  -1 indicates no button pressed

  if (button >= 0)  // Valid button press
  {
    switch (button)
    {
      case 0:
        if (shortPress)
        {
          // Actions based on a short press
          Serial.println("Yep, it's a button 0 short press");
        }
        else  // Long Press
        {
          // Actions based on a long press
          Serial.println("Yep, it's a button 0 long press");
        }

        break;

      case 1:
        if (shortPress)
        {
          // Actions based on a short press
        }
        else  // Long Press
        {
          // Actions based on a long press
        }
        break;

        // cases 2 - 15 here

      default:
        Serial.print("Default action. Button: "); 
        Serial.println(button);
        break;
    }
  }
}  // end loop()

// *******************************  GetButton()  *******************************
int GetButton()
{
  static int btn                  = -1;
  static bool btnDn               = false;
  static unsigned long btnDnTimer = 0;

  if ((digitalRead(BTN_INT_PIN) == LOW) && !btnDn)  // Got a button press and first time in (i.e. no button release)
  {
    btn = mux.getLastInterruptPin();  // btn = button that was pressed
    mux.clearInterrupts();            // Don't keep reading the last button

    btnDnTimer = millis();  // Capture time button was pressed
    btnDn      = true;      // A button was pressed but not released, so don't come back through here

    return -1;  // Let caller know that we're not ready yet
  }
  else if ((mux.digitalRead(btn) == HIGH) && btnDn && (millis() - btnDnTimer > DEBOUNCE))  // Button was released
  {
    Serial.print("btn: ");
    Serial.print(btn);

    if (millis() - btnDnTimer > LONG_PRESS)  // Use time captured at button press to determine duration of press
    {
      shortPress = false;
      Serial.println("  Long Press");
    }
    else
    {
      shortPress = true;
      Serial.println("  Short Press");
    }

    btnDn = false;          // Reset for next press
    mux.clearInterrupts();  // Make sure nothing is lingering

    return btn;  // Serve up the button that was pressed
  }

  return -1;  // No button was pressed
}  // end GetButton()
