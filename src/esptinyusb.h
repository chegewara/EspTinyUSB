
#include "Arduino.h"
#include "usb_descriptors.h"

#pragma once
//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

typedef struct 
{
    char langId[2];
    const char *manufacturer;
    const char *product;
    const char *serial;

    const char *cdc;
    const char *dfu;
    const char *hid;
    const char *midi;
    const char *msc;
    const char *vendor;
} descriptor_strings_t;

static const char *descriptor_str_config[11];

class USBCallbacks { 
public:
    virtual ~USBCallbacks() { }
    virtual void onMount() { }
    virtual void onUnmount() { }
    virtual void onSuspend(bool remote_wakeup_en) { }
    virtual void onResume() { }
};

class EspTinyUSB : Stream
{
public:
    EspTinyUSB(bool extPhy = false);
    bool begin(char* str, uint8_t n);
    static void registerDeviceCallbacks(USBCallbacks* cb);

    static size_t hid_report_desc_len;

    tusb_desc_device_t getDeviceDescriptor();
    void setDeviceDescriptorStrings();
    uint8_t *getConfigurationDescriptor();
    void deviceID(uint16_t, uint16_t);
    void deviceID(uint16_t *, uint16_t *);
    void useDFU(bool);
    void useMSC(bool);
    static void manufacturer(char*);
    static void product(char*);
    static void serial(char*);
    static void revision(uint16_t);
    virtual void setBaseEP(uint8_t) = 0;

    virtual int available(void) = 0;
    virtual int peek(void) = 0;
    virtual int read(void) = 0;
    virtual size_t read(uint8_t *buffer, size_t size) = 0;
    virtual void flush(void) = 0;
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;

    friend uint8_t const *tud_descriptor_device_cb(void);
    friend uint8_t const *tud_descriptor_configuration_cb(uint8_t index);
    friend uint8_t const *tud_descriptor_device_cb(void);
    friend uint8_t const *tud_descriptor_configuration_cb(uint8_t index);
    friend uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid);
    friend tusb_desc_device_t *tusb_get_active_desc(void);
    friend char **tusb_get_active_str_desc(void);
    friend void tusb_clear_descriptor(void);
    friend void tud_mount_cb(void);
    friend void tud_umount_cb(void);
    friend void tud_suspend_cb(bool remote_wakeup_en);
    friend void tud_resume_cb(void);

protected:
    static uint8_t *descriptor_config;
    static uint8_t *descriptor_config_if;
    uint8_t _itf;
    static USBCallbacks* m_callbacks;

    xTaskHandle usbTaskHandle;

    static bool enableCDC;
    static bool enableMSC;
    static bool enableMIDI;
    static bool enableHID;
    static bool enableVENDOR;
    static bool enableDFU;

    static descriptor_strings_t strings;
    static uint8_t desc_configuration[500];

    char langId[2];
    static uint16_t _VID;
    static uint16_t _PID;
    bool isEnabled = false;
    static uint8_t ifIdx;
    static int total;
    static uint8_t count;
    static uint16_t _revision;
    static uint16_t _bcdUSB;
};
