#include "Arduino.h"
#include "tusb.h"

#include "hidkeyboard.h"
#define EPNUM_HID 0x02
#define REPORT_ID 3

HIDkeyboard::HIDkeyboard()
{
  enableHID = true;
  _EPNUM_HID = EPNUM_HID;
}

static uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID))};

bool HIDkeyboard::begin(char *str)
{
  // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
  uint8_t hid[] = {TUD_HID_DESCRIPTOR(ifIdx++, 6, HID_PROTOCOL_NONE, sizeof(desc_hid_report), (uint8_t)(_EPNUM_HID | 0x80), CFG_TUD_HID_BUFSIZE, 1)};
  memcpy(&desc_configuration[total], hid, sizeof(hid));
  total += sizeof(hid);
  count++;

  memcpy(&hid_report_desc[EspTinyUSB::hid_report_desc_len], (uint8_t *)desc_hid_report, sizeof(desc_hid_report));
  EspTinyUSB::hid_report_desc_len += TUD_HID_DESC_LEN;
  log_d("begin len: %d", EspTinyUSB::hid_report_desc_len);

  report_id = REPORT_ID;
  return EspTinyUSB::begin(str, 6);
}

void HIDkeyboard::sendKey(uint8_t _keycode, uint8_t modifier)
{
  /*------------- Keyboard -------------*/
  if (tud_hid_n_ready(0))
  {
    uint8_t keycode[6] = {0};
    keycode[0] = _keycode;

    tud_hid_keyboard_report(REPORT_ID, modifier, keycode);
    delay(2);
    // send empty key report if previously has key pressed
    tud_hid_keyboard_report(REPORT_ID, 0, NULL);
  }
}

void HIDkeyboard::sendChar(uint8_t _keycode)
{
  sendKey(keymap[_keycode].usage, keymap[_keycode].modifier);
}

void HIDkeyboard::sendString(char* _text)
{
  int t = strlen((char*)_text);
  uint8_t keycode;
  for(int i; i < t; i++) {
    delay(2);
    keycode = (uint8_t) _text[i];
    sendKey(keymap[keycode].usage, keymap[keycode].modifier);
  }
}
