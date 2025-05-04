#include "LEDController.h"


#define SIPO_DATA 6
#define SIPO_CLOCK 3
#define SIPO_LATCH 2
#define LED_COUNT 15


/// @brief Initializes the LED module
void LEDController::init() {
pinMode(SIPO_DATA, OUTPUT);
        pinMode(SIPO_CLOCK, OUTPUT);
        pinMode(SIPO_LATCH, OUTPUT);

        digitalWrite(SIPO_DATA, LOW);
        digitalWrite(SIPO_CLOCK, LOW);
        digitalWrite(SIPO_LATCH, LOW);
}


/// @brief Turns the LEDs on/off
    /// @param ledMask 15-bit mask (eg. 0b100010010100011)
    void LEDController::setLeds(uint16_t ledMask)
    {
        ledMask = ledMask << 1;

        digitalWrite(SIPO_LATCH, LOW);

        shiftOut(SIPO_DATA, SIPO_CLOCK, LSBFIRST, ledMask);
        shiftOut(SIPO_DATA, SIPO_CLOCK, LSBFIRST, (ledMask >> 8));

        digitalWrite(SIPO_LATCH, HIGH);
    }


 void LEDController::setLedsByCount(uint8_t count)
    {
        // Clamp the LED count
        if (count > LED_COUNT) count = LED_COUNT;

        uint16_t ledMask = 0;

        // The bits need to be set from the end
        for (int i = LED_COUNT - 1; i >= LED_COUNT - count; i--)
        {
            bitSet(ledMask, i);
        }

        LEDController::setLeds(ledMask);
    }


