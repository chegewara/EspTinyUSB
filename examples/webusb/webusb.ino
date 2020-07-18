/**
 * Simple webUSB device to echo with tinyusb webecho terminal
 * VID = 0xcafe to match filter on website
 * author: chegewara
 */
#include "Arduino.h"
#include "webusb.h"
WebUSB USBSerial;

void conCB(bool isCon)
{
    Serial.printf("connection state changed, new state %s\n", isCon? "connected" : "disconnected");
}


void setup() {
    Serial.begin(115200);
    USBSerial.deviceID(0xcafe, 0x0002);

    if(!USBSerial.begin())
        Serial.println("Failed to start webUSB stack");

    USBSerial.onConnect(conCB); // optional
}

void echo_all(char c)
{
    Serial.write(c);
    USBSerial.write(c);
}

void loop() {
    while (USBSerial.available())
    {
        echo_all(USBSerial.read());
    }

    while (Serial.available())
    {
        echo_all(Serial.read());
    }
}

