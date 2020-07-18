#include "tusb.h"

// #include "msc_device.h"

TU_ATTR_WEAK int32_t tud_msc_read10_cb (uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) { return 0;}
TU_ATTR_WEAK int32_t tud_msc_write10_cb (uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) { return 0;}
TU_ATTR_WEAK void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {}
TU_ATTR_WEAK void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {}
TU_ATTR_WEAK int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize) { return 0;}
TU_ATTR_WEAK bool tud_msc_test_unit_ready_cb(uint8_t lun) {return false;}

