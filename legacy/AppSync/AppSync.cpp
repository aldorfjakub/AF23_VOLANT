#include "AppSync.h"
#include <SerialBT.h>

void AppSync::init()
{
  Serial.begin(9600); // PC serial
  SerialBT.begin();   // Start Bluetooth Serial
  SerialBT.setName("PicoW_BT");
  delay(1000);
  Serial.println("Bluetooth initialized. Waiting for connection...");
}

void AppSync::update()
{
  if (SerialBT)
  {
    if (SerialBT.available())
    {
      char c = SerialBT.read(); // Read data from Bluetooth
      Serial.print("Received: ");
      Serial.println(c);

      // Control the onboard LED based on received commands

      if (c >= 48 && c <= 57)
      {

        activeSpoilerLed = c - '0'; // Works for 0-9
      }

      if (c == 'o')
      {
        digitalWrite(LED_BUILTIN, HIGH); // Turn on LED
        Serial.println("LED ON");
      }
      else if (c == 'f')
      {
        digitalWrite(LED_BUILTIN, LOW); // Turn off LED
        Serial.println("LED OFF");
      }
      else
      {
        Serial.println("Unknown command");
      }
    }
  }
  else
  {
    Serial.println("Bluetooth not connected");
  }
}
