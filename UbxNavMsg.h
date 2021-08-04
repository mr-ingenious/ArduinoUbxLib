#ifndef __AUBXLIB_NAVMSG_H_
#define __AUBXLIB_NAVMSG_H_

#include "Arduino.h"

/* ****************************************************************************
  UBX NAV class message definitions.
  ****************************************************************************/

/* ************************************************************************* */
struct UbxNavStatusPayload {
  unsigned int iTOW = 0;
  byte gpsFix = 0;
  byte flags = 0;
  byte fixStat = 0;
  byte flags2 = 0;
  unsigned int ttff = 0;
  unsigned int msss = 0;
};

class UbxNavStatus: public UbxPacket {
  public:
    UbxNavStatusPayload pl;
      
    UbxNavStatus () {
      h.msgClsID = UBX_NAV_STATUS;
      h.length = 16;
    };
    
    UbxNavStatus (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_STATUS;
      h.length = 16;
      
      if (len == 24) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.gpsFix);
        bufferToStruct (buffer, 11, &pl.flags);
        bufferToStruct (buffer, 12, &pl.fixStat);
        bufferToStruct (buffer, 13, &pl.flags2);
        bufferToStruct (buffer, 14, &pl.ttff);
        bufferToStruct (buffer, 18, &pl.msss);
        
        bufferToChecksum (buffer, 22, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxNavPosLLHPayload {
  unsigned int iTOW = 0;
  int lon = 0;
  int lat = 0;
  int height = 0;
  int hMSL = 0;
  unsigned int hAcc = 0;
  unsigned int vAcc = 0;
};

class UbxNavPosLLH: public UbxPacket {
  public:
    UbxNavPosLLHPayload pl;
    
    UbxNavPosLLH () {
      h.msgClsID = UBX_NAV_POSLLH;
      h.length = 28;
    };
    
    UbxNavPosLLH (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_POSLLH;
      h.length = 28;
      
      if (len == 36) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.lon);
        bufferToStruct (buffer, 14, &pl.lat);
        bufferToStruct (buffer, 18, &pl.height);
        bufferToStruct (buffer, 22, &pl.hMSL);
        bufferToStruct (buffer, 26, &pl.hAcc);
        bufferToStruct (buffer, 30, &pl.vAcc);
        
        bufferToChecksum (buffer, 34, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxNavSolPayload {
  unsigned int iTOW = 0;
  int fTOW = 0;
  short week = 0;
  byte gpsFix = 0;
  byte flags = 0;
  int ecefX = 0;
  int ecefY = 0;
  int ecefZ = 0;
  unsigned int pAcc = 0;
  int ecefVX = 0;
  int ecefVY = 0;
  int ecefVZ = 0;
  unsigned int sAcc = 0;
  unsigned short pDOP = 0;
  byte reserved1 = 0;
  byte numSV = 0;
  unsigned int reserved2 = 0;
};

class UbxNavSol: public UbxPacket {
  public: 
    UbxNavSolPayload pl;
    
    UbxNavSol () {
      h.msgClsID = UBX_NAV_SOL;
      h.length = 52;
    };
    
    UbxNavSol (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_SOL;
      h.length = 52;
      
      if (len == 60) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.fTOW);
        bufferToStruct (buffer, 14, &pl.week);
        bufferToStruct (buffer, 16, &pl.gpsFix);
        bufferToStruct (buffer, 17, &pl.flags);
        bufferToStruct (buffer, 18, &pl.ecefX);
        bufferToStruct (buffer, 22, &pl.ecefY);
        bufferToStruct (buffer, 26, &pl.ecefZ);
        bufferToStruct (buffer, 30, &pl.pAcc);
        bufferToStruct (buffer, 34, &pl.ecefVX);
        bufferToStruct (buffer, 38, &pl.ecefVY);
        bufferToStruct (buffer, 42, &pl.ecefVZ);
        bufferToStruct (buffer, 46, &pl.sAcc);
        bufferToStruct (buffer, 50, &pl.pDOP);
        bufferToStruct (buffer, 52, &pl.reserved1);
        bufferToStruct (buffer, 53, &pl.numSV);
        bufferToStruct (buffer, 54, &pl.reserved2);
        
        bufferToChecksum (buffer, 58, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxNavTimeGPSPayload {
  unsigned int iTOW = 0;
  int fTOW = 0;
  short week = 0;
  byte leapS = 0;
  byte valid = 0;
  unsigned short tAcc = 0;
};

class UbxNavTimeGPS: public UbxPacket {
  public:
    UbxNavTimeGPSPayload pl;
    
    UbxNavTimeGPS () {
      h.msgClsID = UBX_NAV_TIMEGPS;
      h.length = 16;
    }
    
    UbxNavTimeGPS (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_TIMEGPS;
      h.length = 16;

      if (len == 24) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.fTOW);
        bufferToStruct (buffer, 14, &pl.week);
        bufferToStruct (buffer, 16, &pl.leapS);
        bufferToStruct (buffer, 17, &pl.valid);
        bufferToStruct (buffer, 18, &pl.tAcc);
        
        bufferToChecksum (buffer, 22, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    }  
};

/* ************************************************************************* */
struct UbxNavTimeUTCPayload {
  unsigned int iTOW = 0;
  unsigned int tAcc = 0;
  int nano = 0;
  unsigned short year = 0;
  byte month = 0;
  byte day = 0;
  byte hour = 0;
  byte min = 0;
  byte sec = 0;
  byte valid = 0;
};

class UbxNavTimeUTC: public UbxPacket {
  public:
    UbxNavTimeUTCPayload pl;
    
    UbxNavTimeUTC () {
      h.msgClsID = UBX_NAV_TIMEUTC;
      h.length = 20;
    }
    
    UbxNavTimeUTC (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_TIMEUTC;
      h.length = 20;

      if (len == 28) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.tAcc);
        bufferToStruct (buffer, 14, &pl.nano);
        bufferToStruct (buffer, 18, &pl.year);
        bufferToStruct (buffer, 20, &pl.month);
        bufferToStruct (buffer, 21, &pl.day);
        bufferToStruct (buffer, 22, &pl.hour);
        bufferToStruct (buffer, 23, &pl.min);
        bufferToStruct (buffer, 24, &pl.sec);
        bufferToStruct (buffer, 25, &pl.valid);
        
        bufferToChecksum (buffer, 26, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    }  
};

/* ************************************************************************* */
struct UbxNavClockPayload {
  unsigned int iTOW = 0;
  int clkB = 0;
  int clkD = 0;
  unsigned int tAcc = 0;
  unsigned int fAcc = 0;
};

class UbxNavClock: public UbxPacket {
  public:
    UbxNavClockPayload pl;
    
    UbxNavClock () {
      h.msgClsID = UBX_NAV_CLOCK;
      h.length = 20;
    }
    
    UbxNavClock (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_CLOCK;
      h.length = 20;

      if (len == 28) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.clkB);
        bufferToStruct (buffer, 14, &pl.clkD);
        bufferToStruct (buffer, 18, &pl.tAcc);
        bufferToStruct (buffer, 22, &pl.fAcc);
        
        bufferToChecksum (buffer, 26, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    }  
};

/* ************************************************************************* */
struct UbxNavDGPSDataset {
    byte svid = 0;
    byte flags = 0;
    unsigned short ageC = 0;
    float prc = 0.0;
    float prrc = 0.0;
};

const byte __MAX_DGPS_CHANNELS = 10;

struct UbxNavDGPSPayload {
  unsigned int iTOW = 0;
  int age = 0;
  short baseId = 0;
  short baseHealth  = 0;
  byte numCh = 0;
  byte status = 0;
  unsigned short reserved1 = 0;
  UbxNavDGPSDataset channels [__MAX_DGPS_CHANNELS]; // TODO: we support only 10 channels
                                                    //       how long can this possibly be?
};

class UbxNavDGPS: public UbxPacket {
  public:
    UbxNavDGPSPayload pl;
    
    UbxNavDGPS () {
      h.msgClsID = UBX_NAV_DGPS;
      h.length = 16; // minimum!
    }
    
    UbxNavDGPS (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_DGPS;
      h.length = 16; // minimum!

      if (len >= 24) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.age);
        bufferToStruct (buffer, 14, &pl.baseId);
        bufferToStruct (buffer, 16, &pl.baseHealth);
        bufferToStruct (buffer, 18, &pl.numCh);
        bufferToStruct (buffer, 19, &pl.status);
        bufferToStruct (buffer, 20, &pl.reserved1);
        
        for (byte ch = 0; ch < __MAX_DGPS_CHANNELS; ch++) {
            bufferToStruct (buffer, 22 + (12 * ch), &pl.channels[ch].svid);
            bufferToStruct (buffer, 23 + (12 * ch), &pl.channels[ch].flags);
            bufferToStruct (buffer, 24 + (12 * ch), &pl.channels[ch].ageC);
            bufferToStruct (buffer, 26 + (12 * ch), &pl.channels[ch].prc);
            bufferToStruct (buffer, 30 + (12 * ch), &pl.channels[ch].prrc);
        }        
        
        bufferToChecksum (buffer, 22 + (12 * pl.numCh), &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    }  
};

/* ************************************************************************* */
struct UbxNavDOPPayload {
  unsigned int iTOW = 0;
  unsigned short gDOP = 0;
  unsigned short pDOP = 0;
  unsigned short tDOP = 0;
  unsigned short vDOP = 0;
  unsigned short hDOP = 0;
  unsigned short nDOP = 0;
  unsigned short eDOP = 0;
};

class UbxNavDOP: public UbxPacket {
  public:
    UbxNavDOPPayload pl;
    
    UbxNavDOP () {
      h.msgClsID = UBX_NAV_DOP;
      h.length = 18;
    }
    
    UbxNavDOP (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_NAV_DOP;
      h.length = 18;

      if (len == 26) {
        bufferToStruct (buffer,  6, &pl.iTOW);
        bufferToStruct (buffer, 10, &pl.gDOP);
        bufferToStruct (buffer, 12, &pl.pDOP);
        bufferToStruct (buffer, 14, &pl.tDOP);
        bufferToStruct (buffer, 16, &pl.vDOP);
        bufferToStruct (buffer, 18, &pl.hDOP);
        bufferToStruct (buffer, 20, &pl.nDOP);
        bufferToStruct (buffer, 22, &pl.eDOP);
        
        bufferToChecksum (buffer, 24, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    }  
};

#endif
