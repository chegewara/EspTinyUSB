/**
 * Simple MSC device with sd card
 * author: chegewara
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "mscusb.h"

#define MISO 36
#define MOSI 35
#define SCK 34
#define CS 33

// >>> required to run msc with sd card
#define DISK_BLOCK_SIZE 512
uint32_t blockCount = 0;
bool sdcardReady;
MSCusb dev;
// <<<

SPIClass* spi = NULL;

void setup(){
    Serial.begin(115200);
    dev.begin(); // init msc device, its not required to call it that early

    spi = new SPIClass(FSPI);
    spi->begin(SCK, MISO, MOSI, CS);
    if(!SD.begin(CS, *spi, 20000000)){
        Serial.println("Card Mount Failed");
        return;
    }
    
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    test();

    blockCount = SD.cardSize() / DISK_BLOCK_SIZE;
    sdcardReady = true;

}

void loop(){

}
