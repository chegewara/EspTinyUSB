#include "esptinyusb.h"

#pragma once

// typedef void (*cdc_data_cb_t)(void);

class CDCusb : public EspTinyUSB
{
private:
    // uint8_t _itf;

public:
    CDCusb(uint8_t itf = 0);
    bool begin();
    int available(void);
    int peek(void);
    int read(void);
    size_t read(uint8_t *buffer, size_t size);
    void flush(void);
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);

    operator bool() const;


    usb_connected_cb _connected_cb;
    usb_data_cb_t _data_cb;

    void onConnect(usb_connected_cb cb);
    void onData(usb_data_cb_t);
};
