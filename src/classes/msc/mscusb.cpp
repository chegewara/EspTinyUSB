#include "Arduino.h"
#include "tusb.h"

#include "mscusb.h"
#define EPNUM_MSC 0x04

MSCusb::MSCusb()
{
    enableMSC = true;
    _EPNUM_MSC = EPNUM_MSC;
}

bool MSCusb::begin(char* str)
{
    // Interface number, string index, EP Out & EP In address, EP size
    uint8_t msc[] = {TUD_MSC_DESCRIPTOR(ifIdx++, 5, _EPNUM_MSC, (uint8_t)(0x80 | _EPNUM_MSC), 64)}; // highspeed 512
    memcpy(&desc_configuration[total], msc, sizeof(msc));
    total += sizeof(msc);
    count++;
    if (!EspTinyUSB::begin(str, 5)) return false;
    return true;
}

void MSCusb::setBaseEP(uint8_t ep)
{
  _EPNUM_MSC = ep;
}
