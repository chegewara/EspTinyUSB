#pragma once
#include "mscusb.h"
#include "SPI.h"
#include "SdFat.h"

#if CFG_TUD_MSC

class SDFat2USB : public MSCusb, public MSCCallbacks
{
public:

    SDFat2USB();

    // MSCusb
    bool    begin(char* str = nullptr);
    bool    initSD(SdCsPin_t ssPin);
    bool    initSD(SdCsPin_t ssPin, uint32_t maxSck);
    void    setCallbacks(MSCCallbacks*);
    void    setCapacity(uint32_t count, uint32_t size);
    void    ready(bool ready);
    bool    isReady();

    // MSCCallbacks
    void    onInquiry(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]);
    bool    onReady(uint8_t lun);
    void    onCapacity(uint8_t lun, uint32_t* block_count, uint16_t* block_size);
    bool    onStop(uint8_t lun, uint8_t power_condition, bool start, bool load_eject);
    int32_t onRead(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    int32_t onWrite(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    void    onFlush(uint8_t lun);

private:

    // SdFat
    SdFat32     sdFat;
    SdCsPin_t   sdSSPin      = 0;
    uint32_t    sdSck        = SPI_FULL_SPEED;
    uint32_t    sdBlockCount = 0;
    uint32_t    sdBlockSize  = 512; // default
    bool        sdCardReady  = false;
};

#endif
