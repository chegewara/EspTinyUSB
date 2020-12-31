#include "Arduino.h"
#include "tusb.h"
#include "soc/rtc_cntl_reg.h"

#include "hidusb.h"
#define EPNUM_HID   0x03

static HIDusb *_hidUSB = NULL;

HIDusb::HIDusb()
{
    enableHID = true;
    _hidUSB = this;
    _EPNUM_HID = EPNUM_HID;
}

void HIDusb::setBaseEP(uint8_t ep)
{
  _EPNUM_HID = ep;
}

void HIDusb::onData(hid_on_data_t cb)
{
    _data_cb = cb;
}

size_t HIDusb::write(uint8_t _r) {
  uint8_t report = _r;
  if(tud_hid_report(report_id, &report, 1)) return 1;

  return 0;
}

size_t HIDusb::write(const uint8_t *buffer, size_t len) {
  if(tud_hid_report(report_id, buffer, len)) {
    log_d("write hid: %s", (char*)buffer);
    log_d("len: %d", len);
  } else return -1;
  return len;
}

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(void)
{
    return _hidUSB->hid_report_desc;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  log_w("%s", __func__);
  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  // This example doesn't use multiple report and report ID
  (void) report_id;
  (void) report_type;
    log_d("tud_hid_set_report_cb: %d", report_id);

  if(_hidUSB->_data_cb) {
    _hidUSB->_data_cb(report_id, (uint8_t)report_type, buffer, bufsize);
  }
}
