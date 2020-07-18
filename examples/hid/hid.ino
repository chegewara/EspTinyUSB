/**
 * Simple vendor In/Out HID device
 * author: chegewara
 */
#include "Arduino.h"
#include "hidusb.h"

HIDusb dev;

void dataCB(uint8_t report_id, uint8_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    Serial.println((char*)buffer);
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
}
