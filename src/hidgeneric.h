
#include "hidusb.h"

#pragma once

class HIDgeneric : public HIDusb
{
public:
    HIDgeneric();
    bool begin(char* str = nullptr);
};
