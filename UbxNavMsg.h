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
        pl.iTOW  = buffer[9] << 24;
        pl.iTOW |= buffer[8] << 16;
        pl.iTOW |= buffer[7] << 8;
        pl.iTOW |= buffer[6];
        
        pl.gpsFix  = buffer[10];
        pl.flags   = buffer[11];
        pl.fixStat = buffer[12];
        pl.flags2  = buffer[13];
        
        pl.ttff  = buffer[17] << 24;
        pl.ttff |= buffer[16] << 16;
        pl.ttff |= buffer[15] << 8;
        pl.ttff |= buffer[14];
        
        pl.msss  = buffer[21] << 24;
        pl.msss |= buffer[20] << 16;
        pl.msss |= buffer[19] << 8;
        pl.msss |= buffer[18];
        
        checksum  = buffer[22] << 8;
        checksum |= buffer[23];
        
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
        pl.iTOW  = buffer[9] << 24;
        pl.iTOW |= buffer[8] << 16;
        pl.iTOW |= buffer[7] << 8;
        pl.iTOW |= buffer[6];

        pl.lon  = buffer[13] << 24;
        pl.lon |= buffer[12] << 16;
        pl.lon |= buffer[11] << 8;
        pl.lon |= buffer[10];
        
        pl.lat  = buffer[17] << 24;
        pl.lat |= buffer[16] << 16;
        pl.lat |= buffer[15] << 8;
        pl.lat |= buffer[14];
        
        pl.height  = buffer[21] << 24;
        pl.height |= buffer[20] << 16;
        pl.height |= buffer[19] << 8;
        pl.height |= buffer[18];
        
        pl.hMSL  = buffer[25] << 24;
        pl.hMSL |= buffer[24] << 16;
        pl.hMSL |= buffer[23] << 8;
        pl.hMSL |= buffer[22];
        
        pl.hAcc  = buffer[29] << 24;
        pl.hAcc |= buffer[28] << 16;
        pl.hAcc |= buffer[27] << 8;
        pl.hAcc |= buffer[26];
        
        pl.vAcc  = buffer[33] << 24;
        pl.vAcc |= buffer[32] << 16;
        pl.vAcc |= buffer[31] << 8;
        pl.vAcc |= buffer[30];
        
        checksum  = buffer[34] << 8;
        checksum |= buffer[35];
        
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
        pl.iTOW  = buffer[9] << 24;
        pl.iTOW |= buffer[8] << 16;
        pl.iTOW |= buffer[7] << 8;
        pl.iTOW |= buffer[6];

        pl.fTOW  = buffer[13] << 24;
        pl.fTOW |= buffer[12] << 16;
        pl.fTOW |= buffer[11] << 8;
        pl.fTOW |= buffer[10];
        
        pl.week  = buffer[15] << 8;
        pl.week |= buffer[14];
        
        pl.gpsFix = buffer[16];
        pl.flags  = buffer[17];
        
        pl.ecefX  = buffer[21] << 24;
        pl.ecefX |= buffer[20] << 16;
        pl.ecefX |= buffer[19] << 8;
        pl.ecefX |= buffer[18];
        
        pl.ecefY  = buffer[25] << 24;
        pl.ecefY |= buffer[24] << 16;
        pl.ecefY |= buffer[23] << 8;
        pl.ecefY |= buffer[22];
        
        pl.ecefZ  = buffer[29] << 24;
        pl.ecefZ |= buffer[28] << 16;
        pl.ecefZ |= buffer[27] << 8;
        pl.ecefZ |= buffer[26];
        
        pl.pAcc  = buffer[33] << 24;
        pl.pAcc |= buffer[32] << 16;
        pl.pAcc |= buffer[31] << 8;
        pl.pAcc |= buffer[30];

        pl.ecefVX  = buffer[37] << 24;
        pl.ecefVX |= buffer[36] << 16;
        pl.ecefVX |= buffer[35] << 8;
        pl.ecefVX |= buffer[34];
             
        pl.ecefVY  = buffer[41] << 24;
        pl.ecefVY |= buffer[40] << 16;
        pl.ecefVY |= buffer[39] << 8;
        pl.ecefVY |= buffer[38];
             
        pl.ecefVZ  = buffer[45] << 24;
        pl.ecefVZ |= buffer[44] << 16;
        pl.ecefVZ |= buffer[43] << 8;
        pl.ecefVZ |= buffer[42];

        pl.sAcc  = buffer[49] << 24;
        pl.sAcc |= buffer[48] << 16;
        pl.sAcc |= buffer[47] << 8;
        pl.sAcc |= buffer[46];
        
        pl.pDOP  = buffer[51] << 8;
        pl.pDOP |= buffer[50];
        
        pl.reserved1 = buffer[52];
        pl.numSV     = buffer[53];
        
        pl.reserved2  = buffer[57] << 24;
        pl.reserved2 |= buffer[56] << 16;
        pl.reserved2 |= buffer[55] << 8;
        pl.reserved2 |= buffer[54];
        
        checksum  = buffer[58] << 8;
        checksum |= buffer[59];
        
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
        pl.iTOW  = buffer[9] << 24;
        pl.iTOW |= buffer[8] << 16;
        pl.iTOW |= buffer[7] << 8;
        pl.iTOW |= buffer[6];

        pl.fTOW  = buffer[13] << 24;
        pl.fTOW |= buffer[12] << 16;
        pl.fTOW |= buffer[11] << 8;
        pl.fTOW |= buffer[10];
        
        pl.week  = buffer[15] << 8;
        pl.week |= buffer[14];
        
        pl.leapS = buffer[16];
        
        pl.valid = buffer[17];
        
        pl.tAcc  = buffer[21] << 24;
        pl.tAcc |= buffer[20] << 16;
        pl.tAcc |= buffer[19] << 8;
        pl.tAcc |= buffer[18];
        
        checksum  = buffer[22] << 8;
        checksum |= buffer[23];
        
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
        pl.iTOW  = buffer[9] << 24;
        pl.iTOW |= buffer[8] << 16;
        pl.iTOW |= buffer[7] << 8;
        pl.iTOW |= buffer[6];

        pl.tAcc  = buffer[13] << 24;
        pl.tAcc |= buffer[12] << 16;
        pl.tAcc |= buffer[11] << 8;
        pl.tAcc |= buffer[10];
        
        pl.nano  = buffer[17] << 24;
        pl.nano |= buffer[16] << 16;
        pl.nano |= buffer[15] << 8;
        pl.nano |= buffer[14];
        
        pl.year  = buffer[19] << 8;
        pl.year |= buffer[18];
        
        pl.month = buffer[20];
        pl.day   = buffer[21];
        pl.hour  = buffer[22];
        pl.min   = buffer[23];
        pl.sec   = buffer[24];
        pl.valid = buffer[25];
        
        checksum  = buffer[26] << 8;
        checksum |= buffer[27];
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    }  
};

#endif
