
#include "esptinyusb.h"

#pragma once

class HIDusb : public EspTinyUSB
{
public:
    HIDusb();
    virtual bool begin(char* str = nullptr) = 0;
    int available(void) { return -1; }
    int peek(void) { return -1; }
    int read(void) { return -1; }
    size_t read(uint8_t *buffer, size_t size) { return 0; }
    void flush(void) { return; }
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    void setBaseEP(uint8_t);

    void onData(hid_on_data_t cb);

    uint8_t _EPNUM_HID;
    uint8_t report_id;
    hid_on_data_t _data_cb;
    uint8_t hid_report_desc[500];
    size_t hid_report_desc_len;
};
