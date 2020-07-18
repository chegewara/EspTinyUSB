/**
 * Simple MSC device, use as ram disk with 1.5MB of size (requires wrover module)
 * author: chegewara
 */
#include "Arduino.h"
#include "mscusb.h"
MSCusb dev;

extern "C" void init_disk();

void setup()
{
    init_disk();
    Serial.begin(115200);
    dev.begin();
}

void loop()
{

}
