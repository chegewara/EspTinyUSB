/**
 * Simple MSC device with SdFat32USB file system on SD card
 * author: sergsk
 */

#include "sdfatusb.h"

#if CFG_TUD_MSC

#define CS 15

SDFat2USB SdFat32USB;

void setup() {
  SdFat32USB.manufacturer("espressif");
  SdFat32USB.serial("1234-567890");
  SdFat32USB.product("Test device");
  SdFat32USB.revision(100);
  SdFat32USB.deviceID(0xdead, 0xbeef);

  if( SdFat32USB.initSD( CS ) ) {
    if( SdFat32USB.begin() ) {
      log_d("MSC lun 1 begin");

    } else log_e("LUN 1 failed");

  } else log_d("Failed to init SD");

}

void loop()
{
  delay(1000);
}

#endif
