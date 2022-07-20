/**
 * Simple MSC device with SdFatUSB file system on SD card
 * author: sergsk
 */

#include "sdfatusb.h"

#if CFG_TUD_MSC

#define CS 15

SDFat2USB SdFatUSB;
File32 root;
File32 file;


bool fs_changed = false;

class MyMSCCallbacks : public MSCCallbacks {

  void onInquiry(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    memcpy(vendor_id,   "Test",           4);
    memcpy(product_id,  "Test Device",   11);
    memcpy(product_rev, "1.0",            3);
  }

  void onFlush(uint8_t lun) {
    getDefaultCallbacks()->onFlush(lun);
    fs_changed = true;
  }
};


void setup() {

  SdFatUSB.setCallbacks( new MyMSCCallbacks() );

  if( SdFatUSB.initSD( CS ) ) {
    if( SdFatUSB.begin() ) {
      log_d("MSC lun 1 begin");

    } else log_e("LUN 1 failed");

  } else log_d("Failed to init SD");

}

void loop()
{
  delay(1000);

  if( SdFatUSB.isReady() && fs_changed) {

      log_d("==============================================================================================");

      uint16_t mdate, mtime;
      char nm[64];
      uint32_t sz;

      root.open("/");
      while( file.openNext(&root, O_RDONLY) ) {

        if( file.isFile() && !file.isHidden() && file.getModifyDateTime(&mdate, &mtime) ) {
          file.getName(nm, 64);

          byte yy =   mdate >> 9;
          byte mm = ( mdate & 0b0000000111100000 ) >> 5;
          byte dd =   mdate & 0b11111;

          byte hr =    mtime >> 11;
          byte mn = (  mtime & 0b0000011111100000 ) >> 5;
          byte sc = (( mtime & 0b11111 ) << 1); // + dir.creationTimeTenths / 100.0;

          log_d("%-64s: %8u %4d-%02d-%02d %02d:%02d:%02d", nm, file.fileSize(), 1980 + yy, mm, dd, hr, mn, sc);

        }
        file.close();
      }
      root.close();

     fs_changed = false;
  }

}

#endif
