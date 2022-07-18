#include "sdfatusb.h"

#ifdef CFG_TUD_MSC

class SDFatCallbacks : public MSCCallbacks {
    SDFat2USB* m_parent;
public:
    SDFatCallbacks(SDFat2USB* sdfat) { m_parent = sdfat; }
    ~SDFatCallbacks() { }
    void onInquiry(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
    {
        if (m_parent->m_private)
        {
            m_parent->m_private->onInquiry(lun, vendor_id, product_id, product_rev);
        } else {
            const char vid[] = "ESP32-S2";
            const char pid[] = "SDFAT32";
            const char rev[] = "1.0";

            memcpy(vendor_id  , vid, strlen(vid));
            memcpy(product_id , pid, strlen(pid));
            memcpy(product_rev, rev, strlen(rev));
            log_v("default onInquiry");
        }
    }
    bool onReady(uint8_t lun) {
        if (m_parent->m_private)
        {
            log_v("custom SDFAT disk onready");
            return m_parent->m_private->onReady(lun);
        } else {
            log_v("SDFAT disk always ready");
            return m_parent->sdcardReady;
        }
    }
    void onCapacity(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
    {
        (void) lun;
        *block_count = m_parent->block_count;
        *block_size = m_parent->block_size;
        log_v("sdfat disk block count: %d, block size: %d", *block_count, *block_size);
    }
    bool onStop(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
    {
        (void) lun;
        (void) power_condition;

        if ( load_eject )
        {
            if (start)
            {
                // load disk storage
                log_v("default start/stop load");
            }else
            {
                // unload disk storage
                log_v("default start/stop unload");
            }
        }

        return true;
    }
    int32_t onRead(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
    {
        log_v("default onread");
        (void) lun;
        (void) offset;
        bool rd = m_parent->sdfat.card()->readSectors(lba, (uint8_t*) buffer, bufsize/512);
        return rd ? bufsize : -1;
    }
    int32_t onWrite(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
    {
        log_v("default onwrite");
        (void) lun;
        (void) offset;
        return m_parent->sdfat.card()->writeSectors(lba, (uint8_t*) buffer, bufsize/512) ? bufsize : -1;
    }
    void onFlush(uint8_t lun)
    {
        log_v("default onflush");
        (void) lun;
        m_parent->sdfat.card()->syncDevice();
        m_parent->sdfat.cacheClear();
    }
};

SDFat2USB::SDFat2USB( )
{
    MSCusb::setCallbacks(new SDFatCallbacks(this));
}

bool SDFat2USB::begin(char* str)
{
    assert(block_count);
    assert(block_size);

    return MSCusb::begin(str);
}

bool SDFat2USB::initSD(SdCsPin_t ssPin, uint32_t 	maxSck)
{
    if(!sdfat.begin(ssPin, maxSck)){
        log_e("Card Mount Failed");
        return false;
    }
    block_count = sdfat.card()->sectorCount();
    sdcardReady = true;
    return true;
}

bool SDFat2USB::initSD(SdCsPin_t ssPin)
{
    return SDFat2USB::initSD(ssPin, SPI_FULL_SPEED);
}

void SDFat2USB::setCapacity(uint32_t count, uint32_t size)
{
    block_count = count;
    block_size = size;
}

void SDFat2USB::setCallbacks(MSCCallbacks* cb)
{
    m_private = cb;
}

void SDFat2USB::ready(bool ready)
{

}

bool SDFat2USB::isReady()
{
    return sdcardReady;
}

#endif
