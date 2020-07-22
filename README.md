Simple and still WiP library for ESP32-S2.

Please don't expect too much from it, since it was meant to be excercise to learn USB and tinyusb.
Library allows to use few USB class:
- CDC, communication device class,
- MSC, mass storage class,
- HID, human interface device class, this class is temporary hardcoded to use IN/OUT vendor HID,
- MIDI, musical instrument digital interface class,
- DFU, device firmware update class,
- WebUSB, its using vendor class to show webusb usage.

Included few very simple examples to show how easy it can be to start with it, including one example that combine 4 classes.



Issues and PRs are welcome.

This is last know arduino-esp32 commit that this library is working with:

https://github.com/espressif/arduino-esp32/commit/38057d66adcca85be522170180da1220cabadceb
