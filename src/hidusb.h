
#include "esptinyusb.h"

#pragma once


class HIDusb : public EspTinyUSB
{
public:
    HIDusb();
    bool begin(char* str);
    int available(void) { return -1; }
    int peek(void) { return -1; }
    int read(void) { return -1; }
    size_t read(uint8_t *buffer, size_t size) { return 0; }
    void flush(void) { return; }
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size) { return 0; }

    void onData(hid_on_data_t cb);

    hid_on_data_t _data_cb;
};
