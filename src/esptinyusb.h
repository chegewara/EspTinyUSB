
#include "Arduino.h"
#include "usb_descriptors.h"

#pragma once
//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

typedef void (*esp_tud_mount_cb)(void);
typedef void (*esp_tud_umount_cb)(void);
typedef void (*esp_tud_suspend_cb)(bool remote_wakeup_en);
typedef void (*esp_tud_resume_cb)(void);
typedef void (*usb_connected_cb)(bool);
typedef void (*usb_data_cb_t)(void);
typedef void (*hid_on_data_t)(uint8_t report_id, uint8_t report_type, uint8_t const* buffer, uint16_t bufsize);

typedef struct 
{
    char langId[2];
    char *manufacturer;
    char *product;
    char *serial;

    char *cdc;
    char *dfu;
    char *hid;
    char *midi;
    char *msc;
    char *vendor;
} descriptor_strings_t;

static char *descriptor_str_config[11];

class EspTinyUSB : Stream
{
public:
    EspTinyUSB(bool extPhy = false);
    bool begin(char* str, uint8_t n);
    static void registerDeviceCallbacks(esp_tud_mount_cb _mount_cb = nullptr, esp_tud_umount_cb _umount_cb = nullptr,
                                        esp_tud_suspend_cb _suspend_cb = nullptr, esp_tud_resume_cb _resume_cb = nullptr);

    tusb_desc_device_t getDeviceDescriptor();
    void setDeviceDescriptorStrings();
    uint8_t *getConfigurationDescriptor();
    void deviceID(uint16_t, uint16_t);
    void deviceID(uint16_t *, uint16_t *);
    void useDFU(bool);
    void useMSC(bool);
    void manufacturer(char*);
    void product(char*);
    void serial(char*);
    void revision(uint16_t);

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

protected:
    static uint8_t *descriptor_config;
    static uint8_t *descriptor_config_if;
    uint8_t _itf;

    xTaskHandle usbTaskHandle;

    static bool enableCDC;
    static bool enableMSC;
    static bool enableMIDI;
    static bool enableHID;
    static bool enableVENDOR;
    static bool enableDFU;

    descriptor_strings_t strings;
    static uint8_t desc_configuration[500];

    char langId[2];
    static uint16_t _VID;
    static uint16_t _PID;
    bool isEnabled = false;
    static uint8_t ifIdx;
    static int total;
    static uint8_t count;
    uint16_t _revision;
};
