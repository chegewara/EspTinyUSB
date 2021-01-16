Simple and still WiP library for ESP32-S2.

Please don't expect too much from it, since it was meant to be excercise to learn USB and tinyusb.
Library allows to use few USB devices:
- CDC, communication device class,
- MSC, mass storage class,
- HID, human interface device class: keyboard, mouse, gamepad, generic IN/OUT,
- MIDI, musical instrument digital interface class,
- DFU, device firmware update class,
- WebUSB, its using vendor class to show webusb usage.

Included few very simple examples to show how easy it can be to start with it, including one example that combine 4 classes.

### Keyboard Layout support

EspTinyUSB library supports several layouts, the en_us layout is by default.

Set up a new layout:

    0.- Open EspTinyUSB/src/classes/hid/hidkeyboard.cpp with a text editor

    1.- Change #define kbd_en_us to another layout. Example: #define kbd_es_es

You can use:

    kbd_en_us
    kbd_es_es
    kbd_it_it

    2.- Save and close hidkeyboard.cpp

    3.- Upload code to ESP32-S2 board

**NOTE:** Some keys or modifiers have not been implemented. I don't have time or material to test all the keyboards.

You can change the keymaps:

* **ES_US:** EspTinyUSB/src/classes/hid/en_us.h
* **ES_ES:** EspTinyUSB/src/classes/hid/es_es.h
* **IT_IT:** EspTinyUSB/src/classes/hid/it_it.h

**Questions:** open issue or contact me twitter (@JoelSernaMoreno)

Issues and PRs are welcome.
