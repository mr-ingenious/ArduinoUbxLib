#ifndef __AUBXLIB_CFGMSG_H_
#define __AUBXLIB_CFGMSG_H_

#include "Arduino.h"

/* ****************************************************************************
  UBX CFG class message definitions.
  ****************************************************************************/

/* ************************************************************************* */
struct UbxCfgMsgMTPayload {
  unsigned short cfgMsgClsID = 0;
  byte rate[6] = { 0 };
};

class UbxCfgMsgMT: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
        
      structToBuffer (buffer, 0, &pl.cfgMsgClsID);
      structToBuffer (buffer, 2, &pl.rate[0]);
      structToBuffer (buffer, 3, &pl.rate[1]);
      structToBuffer (buffer, 4, &pl.rate[2]);
      structToBuffer (buffer, 5, &pl.rate[3]);
      structToBuffer (buffer, 6, &pl.rate[4]);
      structToBuffer (buffer, 7, &pl.rate[5]);
    };
    
  public:
    UbxCfgMsgMTPayload pl;
            
    UbxCfgMsgMT () {
      h.msgClsID = UBX_CFG_MSG;
      h.length = 8;
    };
};

/* ************************************************************************* */
struct UbxCfgMsgPayload {
  unsigned short cfgMsgClsID = 0;
  byte rate = 0;
};

class UbxCfgMsg: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      structToBuffer (buffer, 0, &pl.cfgMsgClsID);
      structToBuffer (buffer, 2, &pl.rate);
    };
    
  public:
    UbxCfgMsgPayload pl;
        
    UbxCfgMsg () {
      h.msgClsID = UBX_CFG_MSG;
      h.length = 3;
    };
};

/* ************************************************************************* */
struct UbxCfgMsgPollReqPayload {
  unsigned short cfgMsgClsID = 0;
};

class UbxCfgMsgPollReq: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      structToBuffer (buffer, 0, &pl.cfgMsgClsID);
    };
    
  public:
    UbxCfgMsgPollReqPayload pl;
        
    UbxCfgMsgPollReq () {
      h.msgClsID = UBX_CFG_MSG;
      h.length = 2;
    };
};

/* ************************************************************************* */
struct UbxCfgCfgPayload {
  byte clearMask [4] = { 0 };
  byte saveMask [4] = { 0 };
  byte loadMask [4] = { 0 };
  byte deviceMask = 0; // optional field
};

class UbxCfgCfg: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      buffer[0] = pl.clearMask[0];
      buffer[1] = pl.clearMask[1];
      buffer[2] = pl.clearMask[2];
      buffer[3] = pl.clearMask[3];
      
      buffer[4] = pl.saveMask[0];
      buffer[5] = pl.saveMask[1];
      buffer[6] = pl.saveMask[2];
      buffer[7] = pl.saveMask[3];

      buffer[8] = pl.loadMask[0];
      buffer[9] = pl.loadMask[1];
      buffer[10] = pl.loadMask[2];
      buffer[11] = pl.loadMask[3];
    };
    
  public:
    UbxCfgCfgPayload pl;
        
    UbxCfgCfg () {
      h.msgClsID = UBX_CFG_CFG;
      h.length = 12;
    };
    
    unsigned short toArrayWithDeviceMask (byte* buffer, unsigned short bLen) {
      h.length = 13;
      if ((buffer != NULL) && (bLen >= 8 + h.length)) {
        headerToArray (buffer, bLen);
        
        payloadToArray (buffer + 6, bLen - 6);
        buffer[12] = pl.deviceMask;
        
        checksumToArray (buffer, bLen);
        
        h.length = 12; // reset length
        return h.length + 1 + 8;
      } else {
        h.length = 12; // reset length
        return 0;
      }
    };
};

/* ************************************************************************* */
struct UbxCfgNMEAPayload {
  byte filter = 0;
  byte version = 0;
  byte numSV = 0;
  byte flags = 0;
};

class UbxCfgNMEA: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      structToBuffer (buffer, 0, &pl.filter);
      structToBuffer (buffer, 1, &pl.version);
      structToBuffer (buffer, 2, &pl.numSV);
      structToBuffer (buffer, 3, &pl.flags);
    };
    
  public:
    UbxCfgNMEAPayload pl;
        
    UbxCfgNMEA () {
      h.msgClsID = UBX_CFG_NMEA;
      h.length = 4;
    };
    
    UbxCfgNMEA (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_CFG_NMEA;
      h.length = 4;
      
      if (len == 12) {
        bufferToStruct (buffer, 6, &pl.filter);
        bufferToStruct (buffer, 7, &pl.version);
        bufferToStruct (buffer, 8, &pl.numSV);
        bufferToStruct (buffer, 9, &pl.flags);
        
        bufferToChecksum (buffer, 10, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxCfgRatePayload {
  unsigned short measRate = 0;
  unsigned short navRate = 0;
  unsigned short timeRef = 0;
};

class UbxCfgRate: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      structToBuffer (buffer, 0, &pl.measRate);
      structToBuffer (buffer, 2, &pl.navRate);
      structToBuffer (buffer, 4, &pl.timeRef);
    };
    
  public:
    UbxCfgRatePayload pl;
        
    UbxCfgRate () {
      h.msgClsID = UBX_CFG_RATE;
      h.length = 6;
    };
    
    UbxCfgRate (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_CFG_RATE;
      h.length = 6;
      
      if (len == 14) {
        bufferToStruct (buffer,  6, &pl.measRate);
        bufferToStruct (buffer,  8, &pl.navRate);
        bufferToStruct (buffer, 10, &pl.timeRef);
        
        bufferToChecksum (buffer, 12, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxCfgRXMPayload {
  byte reserved1 = 8; // always set to 8!
  byte lpMode = 0;
};

class UbxCfgRXM: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      buffer[0] = pl.reserved1;
      buffer[1] = pl.lpMode;
    };
    
  public:
    UbxCfgRXMPayload pl;
        
    UbxCfgRXM () {
      h.msgClsID = UBX_CFG_RXM;
      h.length = 2;
    };
    
    UbxCfgRXM (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_CFG_RXM;
      h.length = 2;
      
      if (len == 10) {
        bufferToStruct (buffer, 6, &pl.reserved1);
        bufferToStruct (buffer, 7, &pl.lpMode);
        
        bufferToChecksum (buffer, 8, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxCfgSBASPayload {
  byte mode = 0;
  byte usage = 0;
  byte maxSBAS = 0;
  byte scanmode2 = 0;
  byte scanmode1 [4] = { 0 };
};

class UbxCfgSBAS: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      buffer[0] = pl.mode;
      buffer[1] = pl.usage;
      buffer[2] = pl.maxSBAS;
      buffer[3] = pl.scanmode2;
      
      buffer[4] = pl.scanmode1[0];
      buffer[5] = pl.scanmode1[1];
      buffer[6] = pl.scanmode1[2];
      buffer[7] = pl.scanmode1[3];
    };
    
  public:
    UbxCfgSBASPayload pl;
        
    UbxCfgSBAS () {
      h.msgClsID = UBX_CFG_SBAS;
      h.length = 8;
    };
    
    UbxCfgSBAS (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_CFG_SBAS;
      h.length = 8;
      
      if (len == 16) {
        bufferToStruct (buffer, 6, &pl.mode);
        bufferToStruct (buffer, 7, &pl.usage);
        bufferToStruct (buffer, 8, &pl.maxSBAS);
        bufferToStruct (buffer, 9, &pl.scanmode2);
        
        bufferToStruct (buffer, 10, &pl.scanmode1[0]);
        bufferToStruct (buffer, 11, &pl.scanmode1[1]);
        bufferToStruct (buffer, 12, &pl.scanmode1[2]);
        bufferToStruct (buffer, 13, &pl.scanmode1[3]);
        
        bufferToChecksum (buffer, 14, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

/* ************************************************************************* */
struct UbxCfgPMPayload {
  byte version = 0;
  byte reserved1 = 0;
  byte reserved2 = 0;
  byte reserved3 = 0;
  byte flags [4] = { 0 };
  unsigned int updatePeriod = 0;
  unsigned int searchPeriod = 0;
  unsigned int gridOffset = 0;
  unsigned short onTime = 0;
  unsigned short minAcqTime = 0;
};

class UbxCfgPM: public UbxPacket {
  private:
    void payloadToArray (byte* buffer, unsigned short bLen) {
      structToBuffer (buffer,  0, &pl.version);
      structToBuffer (buffer,  1, &pl.reserved1);
      structToBuffer (buffer,  2, &pl.reserved2);
      structToBuffer (buffer,  3, &pl.reserved3);
      structToBuffer (buffer,  4, &pl.flags[0]);
      structToBuffer (buffer,  5, &pl.flags[1]);
      structToBuffer (buffer,  6, &pl.flags[2]);
      structToBuffer (buffer,  7, &pl.flags[3]);
      structToBuffer (buffer,  8, &pl.updatePeriod);
      structToBuffer (buffer, 12, &pl.searchPeriod);
      structToBuffer (buffer, 16, &pl.gridOffset);
      structToBuffer (buffer, 20, &pl.onTime);
      structToBuffer (buffer, 22, &pl.minAcqTime);     
    };
    
  public:
    UbxCfgPMPayload pl;
        
    UbxCfgPM () {
      h.msgClsID = UBX_CFG_PM;
      h.length = 24;
    };
    
    UbxCfgPM (byte* buffer, unsigned short len) {
      h.msgClsID = UBX_CFG_PM;
      h.length = 24;
      
      if (len == 32) {
        bufferToStruct (buffer,  6, &pl.version);
        bufferToStruct (buffer,  7, &pl.reserved1);
        bufferToStruct (buffer,  8, &pl.reserved2);
        bufferToStruct (buffer,  9, &pl.reserved3);
        bufferToStruct (buffer, 10, &pl.flags[0]);
        bufferToStruct (buffer, 11, &pl.flags[1]);
        bufferToStruct (buffer, 12, &pl.flags[2]);
        bufferToStruct (buffer, 13, &pl.flags[3]);
        bufferToStruct (buffer, 14, &pl.updatePeriod);
        bufferToStruct (buffer, 18, &pl.searchPeriod);
        bufferToStruct (buffer, 22, &pl.gridOffset);
        bufferToStruct (buffer, 26, &pl.onTime);
        bufferToStruct (buffer, 28, &pl.minAcqTime);
        
        bufferToChecksum (buffer, 30, &checksum);
        
        valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
      }
    };
};

#endif
