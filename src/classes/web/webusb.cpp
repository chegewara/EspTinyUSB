#include "Arduino.h"
// #include "vendor_device.h"
#include "esptinyusb.h"
#include "webusb.h"

static bool web_serial_connected = false;
static WebUSB *_webUSB = NULL;
#define MS_OS_20_DESC_LEN 0xB2
#define EPNUM_VENDOR 0x03
#define _vendor  "Vendor class (webUSB)"

// https://developers.google.com/web/fundamentals/native-hardware/build-for-webusb#microsoft_os_compatibility_descriptors
uint8_t desc_ms_os_20[] = {
    // Set header: length, type, windows version, total length
    U16_TO_U8S_LE(0x000A), U16_TO_U8S_LE(MS_OS_20_SET_HEADER_DESCRIPTOR),
    U32_TO_U8S_LE(0x06030000), U16_TO_U8S_LE(MS_OS_20_DESC_LEN),

    // Configuration subset header: length, type, configuration index, reserved,
    // configuration total length
    U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_CONFIGURATION),
    0, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A),

    // Function Subset header: length, type, first interface, reserved, subset
    // length
    U16_TO_U8S_LE(0x0008), U16_TO_U8S_LE(MS_OS_20_SUBSET_HEADER_FUNCTION),
    0 /*itf num*/, 0, U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08),

    // MS OS 2.0 Compatible ID descriptor: length, type, compatible ID, sub
    // compatible ID
    U16_TO_U8S_LE(0x0014), U16_TO_U8S_LE(MS_OS_20_FEATURE_COMPATBLE_ID), 'W',
    'I', 'N', 'U', 'S', 'B', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, // sub-compatible

    // MS OS 2.0 Registry property descriptor: length, type
    U16_TO_U8S_LE(MS_OS_20_DESC_LEN - 0x0A - 0x08 - 0x08 - 0x14),
    U16_TO_U8S_LE(MS_OS_20_FEATURE_REG_PROPERTY), U16_TO_U8S_LE(0x0007),
    U16_TO_U8S_LE(0x002A), // wPropertyDataType, wPropertyNameLength and
                           // PropertyName "DeviceInterfaceGUIDs\0" in UTF-16
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00, 'I', 0x00,
    'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00, 'a', 0x00, 'c', 0x00,
    'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00,
    0x00,
    U16_TO_U8S_LE(0x0050), // wPropertyDataLength
    // bPropertyData: “{975F44D9-0D08-43FD-8B3E-127CA8AFFF9D}”.
    '{', 0x00, '9', 0x00, '7', 0x00, '5', 0x00, 'F', 0x00, '4', 0x00, '4', 0x00,
    'D', 0x00, '9', 0x00, '-', 0x00, '0', 0x00, 'D', 0x00, '0', 0x00, '8', 0x00,
    '-', 0x00, '4', 0x00, '3', 0x00, 'F', 0x00, 'D', 0x00, '-', 0x00, '8', 0x00,
    'B', 0x00, '3', 0x00, 'E', 0x00, '-', 0x00, '1', 0x00, '2', 0x00, '7', 0x00,
    'C', 0x00, 'A', 0x00, '8', 0x00, 'A', 0x00, 'F', 0x00, 'F', 0x00, 'F', 0x00,
    '9', 0x00, 'D', 0x00, '}', 0x00, 0x00, 0x00, 0x00, 0x00};

WebUSB::WebUSB(uint8_t itf)
{
    _itf = itf;
    _webUSB = this;
    enableVENDOR = true;
    String url = String("www.tinyusb.org/examples/webusb-serial");
    landingPageURI(url, true);
    _EPNUM_VENDOR = EPNUM_VENDOR;
}

void WebUSB::setBaseEP(uint8_t ep)
{
  _EPNUM_VENDOR = ep;
}

bool WebUSB::begin(char* str, const char *url, bool ssl)
{
    if (url != nullptr)
        landingPageURI(url, ssl);

    // Interface number, string index, EP Out & IN address, EP size
    uint8_t vendor[] = {TUD_VENDOR_DESCRIPTOR(ifIdx++, 7, _EPNUM_VENDOR, 0x80 | _EPNUM_VENDOR, 64)};
    memcpy(&desc_configuration[total], vendor, sizeof(vendor));
    total += sizeof(vendor);
    count++;
    _bcdUSB = 0x210;
    return EspTinyUSB::begin(str, 7);
}

int WebUSB::available()
{
    return tud_vendor_n_available(_itf);
}

int WebUSB::peek()
{
    ESP_LOGD(__func__, "");

    int pos;
    uint8_t buffer[1];
    if (web_serial_connected)
    {
        tud_vendor_n_peek(_itf, 0, buffer);
        return buffer[0];
    }
    else
    {
        return -1;
    }
}

int WebUSB::read()
{
    ESP_LOGD(__func__, "");
    if (web_serial_connected)
    {
        char c;
        if (tud_vendor_n_available(_itf))
        {
            uint32_t count = tud_vendor_n_read(_itf, &c, 1);
            return c;
        }
    }

    return -1;
}

size_t WebUSB::read(uint8_t *buffer, size_t size)
{
    ESP_LOGV(__func__, "");
    if (web_serial_connected)
    {
        if (tud_vendor_n_available(_itf))
        {
            uint32_t count = tud_vendor_n_read(_itf, buffer, size);
            return count;
        }
    }

    return -1;
}

size_t WebUSB::write(uint8_t c)
{
    ESP_LOGV(__func__, "");
    return write(&c, 1);
}

size_t WebUSB::write(const uint8_t *buffer, size_t size)
{
    uint32_t sent = tud_vendor_n_write(_itf, buffer, size);
    return sent;
}

void WebUSB::flush() {}

WebUSB::operator bool() const
{
    return web_serial_connected;
}

void WebUSB::onConnect(usb_connected_cb cb)
{
    _connected_cb = cb;
}

void WebUSB::landingPageURI(String url, bool ssl)
{
    landingPageURI((const char *)url.c_str(), ssl);
}

void WebUSB::landingPageURI(const char *url, bool ssl)
{
    if (_url != NULL)
    {
        free(_url);
    }
    uint8_t size = strlen(url);
    _url = (uint8_t *)calloc(1, size + 3);
    memcpy(&_url[3], url, size);
    _url[0] = size + 3;
    _url[1] = 3;
    _url[2] = ssl;
}

void WebUSB::onData(usb_data_cb_t cb)
{
    _data_cb = cb;
}

void tud_vendor_rx_cb(uint8_t itf)
{
    if (_webUSB->_data_cb)
    {
    ESP_LOGD(__func__, "");
        _webUSB->_data_cb();
    }
}

//--------------------------------------------------------------------+
// WebUSB use vendor class
//--------------------------------------------------------------------+
extern "C"
{
#define BOS_TOTAL_LEN \
    (TUD_BOS_DESC_LEN + TUD_BOS_WEBUSB_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN)

    uint8_t const desc_bos[] = {
        // total length, number of device caps
        TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, 2),

        // Vendor Code, iLandingPage
        TUD_BOS_WEBUSB_DESCRIPTOR(VENDOR_REQUEST_WEBUSB, 1),

        // Microsoft OS 2.0 descriptor
        TUD_BOS_MS_OS_20_DESCRIPTOR(MS_OS_20_DESC_LEN, VENDOR_REQUEST_MICROSOFT)};

    uint8_t const *tud_descriptor_bos_cb(void)
    {
        if(_webUSB == NULL) return nullptr;

        return desc_bos;
    }

    // Invoked when received VENDOR control request
    bool tud_vendor_control_request_cb(uint8_t rhport, tusb_control_request_t const *request)
    {
        if(_webUSB == NULL) return false;
        // TODO add check if rhport != itf => ignore
        switch (request->bRequest)
        {
        case VENDOR_REQUEST_WEBUSB:
            if (!_webUSB->_url)
                return false;
            return tud_control_xfer(rhport, request, (void *)_webUSB->_url, _webUSB->_url[0]);

        case VENDOR_REQUEST_MICROSOFT:
            if (request->wIndex == 7)
            {
                // Get Microsoft OS 2.0 compatible descriptor
                uint16_t total_len;
                memcpy(&total_len, desc_ms_os_20 + 8, 2);

                return tud_control_xfer(rhport, request, (void *)desc_ms_os_20, total_len);
            }
            else
            {
                return false;
            }
            break;
        case 0x22:
            // Webserial simulate the CDC_REQUEST_SET_CONTROL_LINE_STATE (0x22) to
            // connect and disconnect.
            web_serial_connected = (request->wValue != 0);

            if (_webUSB->_connected_cb != nullptr)
            {
                _webUSB->_connected_cb(web_serial_connected);
            }

            // response with status OK
            return tud_control_status(rhport, request);

        default:
            // stall unknown request
            return false;
        }

        return true;
    }

    // Invoked when DATA Stage of VENDOR's request is complete
    bool tud_vendor_control_complete_cb(uint8_t rhport, tusb_control_request_t const *request)
    {
        if(_webUSB == NULL) return false;
        (void)rhport;
        (void)request;
        Serial.println("DATA stage");

        // nothing to do
        return true;
    }
}
