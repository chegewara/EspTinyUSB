/**
 * Simple CDC device connect with putty to use it
 * author: chegewara
 */
#include "Arduino.h"
#include "cdcusb.h"

CDCusb USBSerial;

void conCB(bool isCon)
{
    Serial.printf("connection state changed, new state %s\n", isCon ? "connected" : "disconnected");
}

void setup()
{
    Serial.begin(115200);

    if (!USBSerial.begin())
        Serial.println("Failed to start CDC USB stack");

    USBSerial.onConnect(conCB);
}

void echo_all(char c)
{
    USBSerial.write(c);
    Serial.write(c);
}

void loop()
{
    while (USBSerial.available())
    {
        echo_all(USBSerial.read());
    }
    while (Serial.available())
    {
        echo_all(Serial.read());
    }
}
