/**
 * Simple CDC device connect with putty to use it
 * author: chegewara
 * Serial - used only for logging
 * Serial1 - can be used to control GPS or any other device, may be replaced with Serial
 */
#include "Arduino.h"
#include "cdcusb.h"

CDCusb USBSerial;

class MyUSBCallbacks : public CDCCallbacks {
    void onCodingChange(cdc_line_coding_t const* p_line_coding)
    {
        int bitrate = USBSerial.getBitrate();
        Serial.printf("new bitrate: %d\n", bitrate);
        Serial1.updateBaudRate(bitrate);
    }

    bool onConnect(bool dtr, bool rts)
    {
        Serial.printf("connection state changed, dtr: %d, rts: %d\n", dtr, rts);
        return true;  // allow to persist reset, when Arduino IDE is trying to enter bootloader mode
    }

    void onData()
    {
        int len = USBSerial.available();
        Serial.printf("\nnew data, len %d\n", len);
        uint8_t buf[len] = {};
        USBSerial.read(buf, len);
        Serial1.write(buf, len);
    }

    void onWantedChar(char c)
    {
        Serial.printf("wanted char: %c\n", c);
    }
};


void setup()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, 2, 3);
    USBSerial.setCallbacks(new MyUSBCallbacks());
    USBSerial.setWantedChar('x');

    if (!USBSerial.begin())
        Serial.println("Failed to start CDC USB stack");

}

void loop()
{
    while (Serial1.available())
    {
        int len = Serial1.available();
        char buf1[len];
        Serial1.read(buf1, len);
        int a = USBSerial.write((uint8_t*)buf1, len);
    }
}
