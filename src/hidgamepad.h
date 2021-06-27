#include "hidusb.h"

#pragma once
#if CFG_TUD_HID

class HIDgamepad : public HIDusb
{
public:
    HIDgamepad(uint8_t id = 4);
    bool begin(char* str = nullptr);

    void buttons(uint16_t);
    void joystick1(int8_t, int8_t);
    void joystick2(int8_t, int8_t);
    void sendAll(uint16_t bt, int8_t x, int8_t y, int8_t z, int8_t rz);

private:
    void sendReport();
    uint8_t report[6];
};

#endif
