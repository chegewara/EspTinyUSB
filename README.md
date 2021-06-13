# Description

Library allows to build USB class devices and to make it usable with minimal effort:
- CDC, communication device class,
- MSC, mass storage class,
- HID, human interface device class: keyboard, mouse, gamepad, generic IN/OUT,
- MIDI, musical instrument digital interface class,
- DFU, device firmware update class,
- WebUSB, its using vendor class to show webusb usage.

# Hardware
To use native USB we need to connect pins 19 and 20 to usb cable or with similar connectors:
![](https://ae01.alicdn.com/kf/HTB1MFvqNgHqK1RjSZJnq6zNLpXaR/10-sztuk-Mini-Micro-USB-do-DIP-2-54mm-Adapter-z-cze-modu-u-Panel-kobiet.jpg)
![](https://ae01.alicdn.com/kf/HTB1cfmCgcnI8KJjSspeq6AwIpXa6/AMS1117-3-3V-AMS1117-3-3V-Mini-USB-5V-3-3V-DC-Perfect-Power-Supply-Module.jpg)

# How to
Library allows to set all values in standard USB device like:
- manufacturer
- product name
- serial number
- revision
- VID and PID

```
ANYusb device;  // any USB class like HID, MSC, CDC
device.manufacturer(char*);
device.product(char*); // product name
device.serial(char*);  // serial number SN
device.revision(uint16_t); // product revison
device.deviceID(uint16_t VID, uint16_t PID);
device.deviceID(uint16_t* VID, uint16_t* PID);
```

# Additional configuration necessary

For arduino, create `platform.local.txt` in `.../Users/XXXX/Documents/Arduino/hardware/espressif/esp32/` with contents found in the `platform.local.txt.example`.
For platformio, make sure to add the build flags found in `platformio.ini.example`.

# Contributions
Issues and PRs are welcome.
