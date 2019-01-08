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

#ifndef MICROBIT_IBEACON_H
#define MICROBIT_IBEACON_H

#include "MicroBitConfig.h"

/*
 * Return to our predefined compiler settings.
 */
#if !defined(__arm)
#pragma GCC diagnostic pop
#endif

#include "MicroBitBLEManager.h"

/**
  * Class definition for the MicroBitIBeacon.
  *
  */
class MicroBitIBeacon
{
  public:

    static MicroBitIBeacon* getInstance();
    

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
    int setParams(BLEDevice* ble, const UUID &proximityUUID, int16_t major, int16_t minor, int8_t calibratedPower = 0xF0);

  private:

    /**
      * Private constructor.
      */
    MicroBitIBeacon();

    static MicroBitIBeacon *_instance;    
};

#endif