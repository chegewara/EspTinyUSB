/**
 * Simple HID gamepad
 * author: chegewara
 */


#include "hidgamepad.h"
#if CFG_TUD_HID
HIDgamepad gamepad;

void setup()
{
    Serial.begin(115200);
    gamepad.begin();
}

void loop()
{
    delay(1000);
    for (size_t i = 0; i < 16; i++)
    {
        // buttons send map of buttons represented by bits
        gamepad.buttons(1<<i);
        delay(1000);
    }
    gamepad.joystick1(100, -100);
    delay(1000);
    gamepad.joystick2(100, -100);
    delay(1000);
    // Button Map |  X | Y | Z | Rz
    gamepad.sendAll(0xffff, 0, 0, 0, 0);
    delay(1000);
}

#endif
