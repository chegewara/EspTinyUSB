#include "hidgamepad.h"
#define EPNUM_HID   0x03
#if CFG_TUD_HID

HIDgamepad::HIDgamepad(uint8_t id)
{
  report_id = id;
  enableHID = true;
  _EPNUM_HID = EPNUM_HID;
}

bool HIDgamepad::begin(char* str)
{
    uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(report_id))};
    // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
    uint8_t hid[] = {TUD_HID_DESCRIPTOR(ifIdx++, 6, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), (uint8_t)(_EPNUM_HID | 0x80), CFG_TUD_HID_BUFSIZE, 10)};
    memcpy(&desc_configuration[total], hid, sizeof(hid));
    total += sizeof(hid);
    count++;

    memcpy(&hid_report_desc[EspTinyUSB::hid_report_desc_len], (uint8_t *)desc_hid_report, sizeof(desc_hid_report));
    EspTinyUSB::hid_report_desc_len += TUD_HID_DESC_LEN;
    log_d("begin len: %d", EspTinyUSB::hid_report_desc_len);

    return EspTinyUSB::begin(str, 6);
}

void HIDgamepad::sendReport()
{
    // Button Map (2 bytes) |  X | Y | Z | Rz
    write(report, 6);
}

void HIDgamepad::buttons(uint16_t bt)
{
    report[0] = bt;
    report[1] = bt >> 8;
    sendReport();
}

void HIDgamepad::joystick1(int8_t x, int8_t y)
{
    report[2] = x;
    report[3] = y;
    sendReport();
}

void HIDgamepad::joystick2(int8_t z, int8_t rz)
{
    report[4] = z;
    report[5] = rz;
    sendReport();
}

void HIDgamepad::sendAll(uint16_t bt, int8_t x, int8_t y, int8_t z, int8_t rz)
{
    report[0] = bt;
    report[1] = bt >> 8;
    report[2] = x;
    report[3] = y;
    report[4] = z;
    report[5] = rz;
    sendReport();
}

#endif
