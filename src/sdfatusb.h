#pragma once
#include "mscusb.h"
#include "SPI.h"
#include "SdFat.h"

#if CFG_TUD_MSC

class SDFat2USB : public MSCusb
{
public:
    SDFat2USB();
    bool begin(char* str = nullptr);
    bool initSD(SdCsPin_t ssPin);
    bool initSD(SdCsPin_t ssPin, uint32_t maxSck);
    void setCallbacks(MSCCallbacks*);
    void setCapacity(uint32_t count, uint32_t size);
    void ready(bool ready);
    bool isReady();

    SdFat32 sdfat;
    MSCCallbacks* m_private;
    uint32_t block_count = 0;
    uint32_t block_size = 512;
    bool sdcardReady;

};

#endif
