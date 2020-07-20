#include "Arduino.h"
#include "webusb.h"
#include "cdcusb.h"
#include "mscusb.h"
#include "dfuusb.h"

extern "C" void init_disk();

WebUSB USBSerial;
CDCusb USBSerial1;
MSCusb msc;
DFUusb dfu;

void conCB(bool isCon)
{
    Serial.printf("connection state changed, new state %s\n", isCon ? "connected" : "disconnected");
}

void mount_cb()
{
    Serial.println("Mount");
}

void umount_cb()
{
    Serial.println("Unmount");
}

void resume_cb()
{
    Serial.println("Resume");
}

void suspend_cb(bool remote_wakeup_en)
{
    Serial.println("Suspend");
}


void setup()
{
    init_disk();
    Serial.begin(115200);

    msc.deviceID(0x303a, 0x0002);
    if (!USBSerial.begin())
        Serial.println("Failed to start webUSB stack");
    if (!USBSerial1.begin())
        Serial.println("Failed to start CDC USB stack");

    dfu.begin();
    msc.begin();
    USBSerial.onConnect(conCB);
    USBSerial1.onConnect(conCB);
    EspTinyUSB::registerDeviceCallbacks(mount_cb, umount_cb, suspend_cb, resume_cb);
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