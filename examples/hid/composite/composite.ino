/**
 * Simple HID mouse and keyboard composite
 * author: chegewara
 */
#if CFG_TUD_HID

#include "hidcomposite.h"

HIDcomposite device;

void setup()
{
    Serial.begin(115200);
    device.begin();
}

void loop()
{
    delay(1000);
    device.doublePressLeft();
    delay(1000);
    device.pressRight();
    delay(1000);
    device.move(-150, -150);
    delay(1000);
    device.move(150, 150);
    delay(1000);
    device.pressLeft();
    delay(1000);
    device.scrollUp(1);
    delay(1000);
    device.scrollDown(1);
    delay(1000);
    device.sendKey(HID_KEY_A);
    delay(1000);
    Serial.println(device.sendString(String("123456789\n"))?"OK":"FAIL");
    delay(1000);
    Serial.println(device.sendString(String("abcdefghijklmnopqrst Uvwxyz\n"))?"OK":"FAIL");    
}

#endif
