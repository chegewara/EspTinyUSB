#include "Arduino.h"
// #include "cdc_device.h"
#include "esptinyusb.h"
#include "cdcusb.h"

#define EPNUM_CDC   0x02

static CDCusb *_CDCusb = NULL;

CDCusb::CDCusb(uint8_t itf)
{
    _itf = itf;
    _CDCusb = this;
    enableCDC = true;
    _EPNUM_CDC = EPNUM_CDC;
}

void CDCusb::setBaseEP(uint8_t ep)
{
  _EPNUM_CDC = ep;
}

bool CDCusb::begin(char* str)
{
    // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
    uint8_t cdc[TUD_CDC_DESC_LEN] = {TUD_CDC_DESCRIPTOR(ifIdx, 4, (0x80 | (_EPNUM_CDC - 1)), 8, _EPNUM_CDC, 0x80 | _EPNUM_CDC, 64)};
    memcpy(&desc_configuration[total], cdc, sizeof(cdc));
    total += sizeof(cdc);
    ifIdx += 2;
    count += 2;

    if(!EspTinyUSB::begin(str, 4)) return false;
    return true;
}

int CDCusb::available()
{
    return tud_cdc_n_available(_itf);
}

int CDCusb::peek()
{
    if (tud_cdc_n_connected(_itf))
    {
        uint8_t buffer;
        tud_cdc_n_peek(_itf, 0, &buffer);
        return buffer;
    }
    else
    {
        return -1;
    }
}

int CDCusb::read()
{
    if (1)
    {
        if (tud_cdc_n_available(_itf))
        {
            char c;
            uint32_t count = tud_cdc_n_read(_itf, &c, 1);
            return c;
        }
    }

    return -1;
}

size_t CDCusb::read(uint8_t *buffer, size_t size)
{
    if (1)
    {
        if (tud_cdc_n_available(_itf))
        {
            uint32_t count = tud_cdc_n_read(_itf, buffer, size);
            return count;
        }
    }

    return 0;
}

size_t CDCusb::write(uint8_t buffer)
{
    uint8_t c = buffer;
    if (tud_cdc_n_connected(_itf))
    {
        uint32_t d = tud_cdc_n_write(_itf, &c, 1);
        tud_cdc_n_write_flush(_itf);
        return d;
    }
    else
    {
        return 0;
    }
}

size_t CDCusb::write(const uint8_t *buffer, size_t size)
{
    if (tud_cdc_n_connected(_itf))
    {
        uint32_t d = tud_cdc_n_write(_itf, buffer, size);
        tud_cdc_n_write_flush(_itf);
        return d;
    }
    else
    {
        return 0;
    }
}

void CDCusb::flush()
{
    tud_cdc_n_read_flush(_itf);
    tud_cdc_n_write_flush(_itf);
}

CDCusb::operator bool() const
{
    return tud_cdc_n_connected(_itf);
}

void CDCusb::onConnect(usb_connected_cb cb)
{
    _connected_cb = cb;
}

void CDCusb::onData(usb_data_cb_t cb)
{
    _data_cb = cb;
}

// Invoked when received new data
void tud_cdc_rx_cb(uint8_t itf)
{
    if (_CDCusb->_data_cb)
    {
        _CDCusb->_data_cb();
    }
}

// void tud_cdc_n_set_wanted_char (uint8_t itf, char wanted);
// Invoked when received `wanted_char`
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char) {}

// Invoked when line state DTR & RTS are changed via SET_CONTROL_LINE_STATE
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    bool serial_connected = false;
    if (dtr && rts)
    {
        serial_connected = true;
    }

    if (_CDCusb->_connected_cb)
    {
        _CDCusb->_connected_cb(serial_connected);
    }
}
