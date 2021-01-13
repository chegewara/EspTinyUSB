/**
 * Simple HID keyboard
 * author: chegewara
 */

#include "Arduino.h"
#include "hidkeyboard.h"

HIDkeyboard dev;

void dataCB(uint8_t report_id, uint8_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    for (size_t i = 0; i < bufsize; i++)
    {
        Serial.printf("%d\n", buffer[i]);
        Serial.printf("%c\n", buffer[i]);
    }
}


void setup()
{
    Serial.begin(115200);
    dev.begin();
    dev.onData(dataCB);
}

void loop()
{
    delay(1000);
    dev.sendKey(HID_KEY_A);
    delay(1000);
    Serial.println(dev.sendString(String("123456789\n"))?"OK":"FAIL");
    delay(1000);
    Serial.println(dev.sendString(String("abcdefghijklmnopqrst Uvwxyz\n"))?"OK":"FAIL");
}
