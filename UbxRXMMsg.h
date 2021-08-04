#ifndef __AUBXLIB_RXMMSG_H_
#define __AUBXLIB_RXMMSG_H_

#include "Arduino.h"

/* ****************************************************************************
  UBX RXM class message definitions.
  ****************************************************************************/

/* ************************************************************************* */
struct UbxSVSInumSVInfo {
  byte svid = 0;
  byte svFlag = 0;
  unsigned short azim = 0;
  byte elev = 0;
  byte age = 0;
};

struct UbxRxmSVSIPayload {
  unsigned int iTOW = 0;
  short week = 0;
  byte numVis = 0;
  byte numSV = 0;
  UbxSVSInumSVInfo svInfo[80];
};

class UbxRxmSVSI: public UbxPacket {
  public:
    UbxRxmSVSIPayload pl;

    UbxRxmSVSI () {
      h.msgClsID = UBX_RXM_SVSI;
      h.length = 8; // MINIMUM!
    };

    UbxRxmSVSI (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_RXM_SVSI; 
      h.length = len - 8;
      
      if (len >= 16) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.week);
        bufferToStruct (buffer, 12, &pl.numVis);
        bufferToStruct (buffer, 13, &pl.numSV);
        
        for (unsigned short svc = 0; svc < pl.numSV; svc++) {
          if (svc == 80) {
            break;
          }
          
          bufferToStruct (buffer, 14 + 6 * svc, &pl.svInfo[svc].svid);
          bufferToStruct (buffer, 15 + 6 * svc, &pl.svInfo[svc].svFlag);
          bufferToStruct (buffer, 17 + 6 * svc, &pl.svInfo[svc].azim);          
          bufferToStruct (buffer, 18 + 6 * svc, &pl.svInfo[svc].elev);
          bufferToStruct (buffer, 19 + 6 * svc, &pl.svInfo[svc].age);
        }
        
        bufferToChecksum (buffer, len - 2, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

#endif
