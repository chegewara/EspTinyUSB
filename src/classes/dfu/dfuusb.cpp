#include "Arduino.h"
#include "tusb.h"
#include "soc/rtc_cntl_reg.h"

#include "dfuusb.h"

DFUusb::DFUusb()
{
    enableDFU = true;
}

bool DFUusb::begin()
{
    // Interface number, string index, attributes, detach timeout, transfer size
    uint8_t dfu[] = {TUD_DFU_RT_DESCRIPTOR(ifIdx++, 9, 0x0f, 1000, 1024)};
    memcpy(&desc_configuration[total], dfu, sizeof(dfu));
    ESP_LOG_BUFFER_HEX("", dfu, sizeof(dfu));
    total += sizeof(dfu);
    count++;
    if (!EspTinyUSB::begin()) return false;
    return true;
}

/**
 * enter dfu bootload mode
 */
void tud_dfu_rt_reboot_to_dfu(void)
{
    REG_WRITE(RTC_CNTL_OPTION1_REG, RTC_CNTL_FORCE_DOWNLOAD_BOOT);
    SET_PERI_REG_MASK(RTC_CNTL_OPTIONS0_REG, RTC_CNTL_SW_PROCPU_RST);
}
