#include "Arduino.h"
#include "tusb.h"

#include "mscusb.h"
#define EPNUM_MSC 0x04

MSCusb::MSCusb()
{
    enableMSC = true;
}

bool MSCusb::begin()
{
    // Interface number, string index, EP Out & EP In address, EP size
    uint8_t msc[] = {TUD_MSC_DESCRIPTOR(ifIdx++, 5, EPNUM_MSC, 0x80 | EPNUM_MSC, 64)}; // highspeed 512
    memcpy(&desc_configuration[total], msc, sizeof(msc));
    total += sizeof(msc);
    count++;
    if (!EspTinyUSB::begin()) return false;
    return true;
}
