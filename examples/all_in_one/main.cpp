#include "Arduino.h"
#include "webusb.h"
#include "cdcusb.h"
#include "mscusb.h"
#include "dfuusb.h"

WebUSB USBSerial;
CDCusb USBSerial1;
MSCusb msc;
DFUusb dfu;

void conCB(bool isCon)
{
    Serial.printf("connection state changed, new state %s\n", isCon ? "connected" : "disconnected");
}
extern "C" void init_disk();

void setup()
{
    init_disk();
    Serial.begin(115200);

    msc.deviceID(0x303a, 0x0002);
    if (!USBSerial.begin())
        Serial.println("Failed to start webUSB stack");
    if (!USBSerial1.begin())
        Serial.println("Failed to start CDC USB stack");
    USBSerial.onConnect(conCB);
    USBSerial1.onConnect(conCB);
}

void echo_all(char c)
{
    USBSerial.write(c);
    USBSerial1.write(c);
    Serial.write(c);
}

void loop()
{
    while (USBSerial.available())
    {
        echo_all(USBSerial.read());
    }
    while (USBSerial1.available())
    {
        echo_all(USBSerial1.read());
    }
    while (Serial.available())
    {
        echo_all(Serial.read());
    }
}
