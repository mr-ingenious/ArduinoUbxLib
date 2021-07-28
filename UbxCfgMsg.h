#ifndef __AUBXLIB_CFGMSG_H_
#define __AUBXLIB_CFGMSG_H_

#include "Arduino.h"

/* ************************************************************************* */
struct UbxCfgMsgMTPayload {
	unsigned short cfgMsgClsID = 0;
	byte rate[6] = {0};
};

class UbxCfgMsgMT: public UbxPacket {
	private:
		void payloadToArray (byte* buffer, unsigned short bLen) {							
			buffer[0] = (pl.cfgMsgClsID >> 8) & 0xFF;
			buffer[1] = pl.cfgMsgClsID & 0xFF;

			buffer[2] = pl.rate[0];
			buffer[3] = pl.rate[1];
			buffer[4] = pl.rate[2];
			buffer[5] = pl.rate[3];
			buffer[6] = pl.rate[4];
			buffer[7] = pl.rate[5];
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
			buffer[0] = (pl.cfgMsgClsID >> 8) & 0xFF;
			buffer[1] = pl.cfgMsgClsID & 0xFF;
			
			buffer[2] = pl.rate;
		};
		
	public:
		UbxCfgMsgPayload pl;
				
		UbxCfgMsg () {
			h.msgClsID = UBX_CFG_MSG;
			h.length = 3;
		};
		
		unsigned short toPollingArray (byte* buffer, unsigned short bLen) {
			h.length = 2;
			
			if (bLen >= 10 && buffer != NULL) {
				headerToArray (buffer, bLen);

				buffer[0] = (pl.cfgMsgClsID >> 8) & 0xFF;
				buffer[1] = pl.cfgMsgClsID & 0xFF;
				
				checksumToArray (buffer, bLen);
				
				return 10;
			} else {
            	return 0;
			}
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
			buffer[0] = pl.filter;
			buffer[1] = pl.version;
			buffer[2] = pl.numSV;
			buffer[3] = pl.flags;
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
				pl.filter  = buffer[6];
				pl.version = buffer[7];
				pl.numSV   = buffer[8];
				pl.flags   = buffer[9];
				
				checksum  = buffer[10] << 8;
				checksum |= buffer[11];
				
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
			buffer[0] = (pl.measRate >> 8) & 0xFF;
			buffer[1] = pl.measRate & 0xFF;
			
			buffer[2] = (pl.navRate >> 8) & 0xFF;
			buffer[3] = pl.navRate & 0xFF;
			
			buffer[4] = (pl.timeRef >> 8) & 0xFF;
			buffer[5] = pl.timeRef & 0xFF;
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
				pl.measRate  = buffer[7] << 8;
				pl.measRate |= buffer[6];
				
				pl.navRate  = buffer[9] << 8;
				pl.navRate |= buffer[8];
				
				pl.timeRef  = buffer[11] << 8;
				pl.timeRef |= buffer[10];
				
				checksum  = buffer[12] << 8;
				checksum |= buffer[13];
				
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
				pl.reserved1 = buffer[6];
				pl.lpMode    = buffer[7];
				
				checksum  = buffer[8] << 8;
				checksum |= buffer[9];
				
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
				pl.mode       = buffer[6];
				pl.usage      = buffer[7];
				pl.maxSBAS    = buffer[8];
				pl.scanmode2  = buffer[9];
				
				pl.scanmode1[0] = buffer[10];
				pl.scanmode1[1] = buffer[11];
				pl.scanmode1[2] = buffer[12];
				pl.scanmode1[3] = buffer[13];
				
				checksum  = buffer[14] << 8;
				checksum |= buffer[15];
				
				valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
			}
		};
};
#endif
