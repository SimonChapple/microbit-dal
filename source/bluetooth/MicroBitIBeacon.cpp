/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBitConfig.h"
#include "MicroBitIBeacon.h"

MicroBitIBeacon *MicroBitIBeacon::_instance = NULL;

/* The underlying Nordic libraries that support BLE do not compile cleanly with the stringent GCC settings we employ.
 * If we're compiling under GCC, then we suppress any warnings generated from this code (but not the rest of the DAL)
 * The ARM cc compiler is more tolerant. We don't test __GNUC__ here to detect GCC as ARMCC also typically sets this
 * as a compatability option, but does not support the options used...
 */
#if !defined(__arm)
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/*
 * Return to our predefined compiler settings.
 */
#if !defined(__arm)
#pragma GCC diagnostic pop
#endif

const int IBEACON_PAYLOAD_LENGTH = 26;
const uint8_t IBEACON_MANUFACTURER_DATA = 0xff;
const uint8_t IBEACON_COMPANY_ID[] = {0x4c, 0x00};
const uint8_t IBEACON_BEACON_TYPE[] = {0x02, 0x15};

MicroBitIBeacon::MicroBitIBeacon() 
{
}

MicroBitIBeacon * MicroBitIBeacon::getInstance()
{
    if (_instance == 0)
    _instance = new MicroBitIBeacon;

    return _instance;
}

/**
  * Set the content of iBeacon frames
  *
  * @param proximityUUID 16-byte proximity UUID
  *
  * @param major 2-byte major value
  *
  * @param minor 2-byte minor value
  *
  * @param calibratedPower the transmission range of the beacon (Defaults to: 0xF0 ~10m).
  *
  * @note The calibratedPower value ranges from -100 to +20 to a resolution of 1. The calibrated power should be binary encoded.
  * More information can be found at https://github.com/google/eddystone/tree/master/eddystone-uid#tx-power
  */
int MicroBitIBeacon::setParams(BLEDevice* ble, const UUID &proximityUUID, int16_t major, int16_t minor, int8_t calibratedPower)
{
    uint8_t rawData[IBEACON_PAYLOAD_LENGTH + 1];
    
    // populate iBeacon payload
    int index = 0;
    rawData[index++] = IBEACON_COMPANY_ID[0];
    rawData[index++] = IBEACON_COMPANY_ID[1];
    rawData[index++] = IBEACON_BEACON_TYPE[0];
    rawData[index++] = IBEACON_BEACON_TYPE[1];
    // copy UUID reverse byte order since UUID class is little endian and packet 
    uint8_t * proxUUID = const_cast<uint8_t*>(proximityUUID.getBaseUUID());
    for(int ii=proximityUUID.getLen()-1;ii>=0;ii--) {
        rawData[index++] = *(proxUUID + ii);
    }
    rawData[index++] = (major >> 8) & 0xff;
    rawData[index++] = major & 0xff;
    rawData[index++] = (minor >> 8) & 0xff;
    rawData[index++] = minor & 0xff;
    rawData[index++] = calibratedPower & 0xff;
    
    ble_error_t err = ble->gap().accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, rawData, index);
    
    return MICROBIT_OK;
}
