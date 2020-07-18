/**
 * Simple example that enables DFU class, it will restart esp32 to bootloader mode when issued idf.py dfu-flash
 * also CDC class is active, but does nothing
 * author: chegewara
 */
#include "Arduino.h"
#include "cdcusb.h"
#include "dfuusb.h"

// CDCusb USBSerial;
DFUusb dev;

void conCB(bool isCon)
{
    Serial.printf("connection state changed, new state %s\n", isCon? "connected" : "disconnected");
}

void setup() {
    Serial.begin(115200);

    dev.begin();
    // if(!USBSerial.begin())
    //     Serial.println("Failed to start CDC USB stack");

    // USBSerial.onConnect(conCB);
}


void loop() {

}

