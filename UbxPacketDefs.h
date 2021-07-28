#ifndef __AUBXLIB_UPD_H_
#define __AUBXLIB_UPD_H_

#include "Arduino.h"


/* ****************************************************************************
  Basic definitions
  ****************************************************************************/

const byte UBX_SYNCCHAR1 = 0xB5;
const byte UBX_SYNCCHAR2 = 0x62;
  
enum UBX_CLASS_ID {
    CLS_NAV = 0x01,
    CLS_RXM = 0x02,
    CLS_INF = 0x04,
    CLS_ACK = 0x05,
    CLS_CFG = 0x06,
    CLS_MON = 0x0A,
    CLS_AID = 0x0B,
    CLS_TIM = 0x0D,
    CLS_ESF = 0x10
};

enum UBX_NMEA_PACKET_TYPE {
	// NMEA PROPRIETARY MESSAGES
    UBX_NMEA_UBX00 = 0xF100, // Poll a PUBX,00 message and Lat/Long Position Data
    UBX_NMEA_UBX03 = 0xF103, // Poll a PUBX,03 message and Satellite Status
    UBX_NMEA_UBX04 = 0xF104, // Poll a PUBX,04 message and Time of Day and Clock Information
    UBX_NMEA_UBX05 = 0xF105, // Poll a PUBX,05 message and Lat/Long Position Data
    UBX_NMEA_UBX06 = 0xF106, // Poll a PUBX,06 message and Lat/Long Position Data
    UBX_NMEA_UBX40 = 0xF140, // Set NMEA message output rate
    UBX_NMEA_UBX41 = 0xF141, // Set Protocols and Baudrate
	
	// NMEA STANDARD MESSAGES
	UBX_NMEA_DTM   = 0xF00A, // Datum Reference
	UBX_NMEA_GBS   = 0xF009, // GNSS Satellite Fault Detection
	UBX_NMEA_GGA   = 0xF000, // Global positioning system fix data
	UBX_NMEA_GLL   = 0xF001, // Latitude and longitude, with time of position fix and status
	UBX_NMEA_GPQ   = 0xF040, // Poll message
	UBX_NMEA_GRS   = 0xF006, // GNSS Range Residuals
	UBX_NMEA_GSA   = 0xF002, // GNSS DOP and Active Satellites
	UBX_NMEA_GST   = 0xF007, // GNSS Pseudo Range Error Statistics
	UBX_NMEA_GSV   = 0xF003, // GNSS Satellites in View
	UBX_NMEA_RMC   = 0xF004, // Recommended Minimum data
	UBX_NMEA_THS   = 0xF00E, // True Heading and Status
	UBX_NMEA_TXT   = 0xF041, // Text Transmission
	UBX_NMEA_VTG   = 0xF005, // Course over ground and Ground speed
	UBX_NMEA_ZDA   = 0xF008  // Time and Date
};

enum UBX_PACKET_TYPE {
	UBX_NONE          = 0,
	
	UBX_ACK_ACK       = 0x0501, // 2 Answer Message Acknowledged
	UBX_ACK_NAK       = 0x0502, // 2 Answer Message Not_Acknowledged
				      
	UBX_CFG_ANT       = 0x0613, // 0 Poll Request Poll Antenna Control Settings
	// UBX_CFG_ANT       = 0x0613, // 4 Get/Set Get/Set Antenna Control Settings
	UBX_CFG_CFG       = 0x0609, // (12) or (13) Command Clear, Save and Load configurations
	UBX_CFG_DAT       = 0x0606, // 0 Poll Request Poll Datum Setting
	// UBX_CFG_DAT       = 0x0606, // 2 Set Set Standard Datum
	// UBX_CFG_DAT       = 0x0606, // 44 Set Set User_defined Datum
	// UBX_CFG_DAT       = 0x0606, // 52 Get Get currently selected Datum
	UBX_CFG_EKF       = 0x0612, // 0 Poll Request Poll EKF Module Settings
	// UBX_CFG_EKF       = 0x0612, // 16 Get/Set Get/Set EKF Module Settings _ LEA_6R
	UBX_CFG_ESFGWT    = 0x0629, // 44 Get/Set message Get/Set settings of gyro+wheel tick sol (GWT) _ LEA_6R
	UBX_CFG_FXN       = 0x060E, // 0 Poll Request Poll FXN configuration
	// UBX_CFG_FXN       = 0x060E, // 36 Command RXM FixNOW configuration.
	UBX_CFG_INF       = 0x0602, // 1 Poll Request Poll INF message configuration for one protocol
	// UBX_CFG_INF       = 0x0602, // 0 + 10*N Set/Get Information message configuration
	UBX_CFG_ITFM      = 0x0639, // 8 Command Jamming/Interference Monitor configuration.
	UBX_CFG_MSG       = 0x0601, // 2 Poll Request Poll a message configuration
	// UBX_CFG_MSG       = 0x0601, // 8 Set/Get Set Message Rate(s)
	// UBX_CFG_MSG       = 0x0601, // 3 Set/Get Set Message Rate
	UBX_CFG_NAV5      = 0x0624, // 0 Poll Request Poll Navigation Engine Settings
	// UBX_CFG_NAV5      = 0x0624, // 36 Get/Set Get/Set Navigation Engine Settings
	UBX_CFG_NAVX5     = 0x0623, // 0 Poll Request Poll Navigation Engine Expert Settings
	// UBX_CFG_NAVX5     = 0x0623, // 40 Get/Set Get/Set Navigation Engine Expert Settings
	UBX_CFG_NMEA      = 0x0617, // 0 Poll Request Poll the NMEA protocol configuration
	// UBX_CFG_NMEA      = 0x0617, // 4 Set/Get Set/Get the NMEA protocol configuration
	UBX_CFG_NVS       = 0x0622, // 13 Command Clear, Save and Load non_volatile storage data
	UBX_CFG_PM2       = 0x063B, // 0 Poll Request Poll extended Power Management configuration
	// UBX_CFG_PM2       = 0x063B, // 44 Set/Get Extended Power Management configuration
	UBX_CFG_PM        = 0x0632, // 0 Poll Request Poll Power Management configuration
	// UBX_CFG_PM        = 0x0632, // 24 Set/Get Power Management configuration
	UBX_CFG_PRT       = 0x0600, // 0 Poll Request Polls the configuration of the used I/O Port
	// UBX_CFG_PRT       = 0x0600, // 1 Poll Request Polls the configuration for one I/O Port
	// UBX_CFG_PRT       = 0x0600, // 20 Get/Set Get/Set Port Configuration for UART
	// UBX_CFG_PRT       = 0x0600, // 20 Get/Set Get/Set Port Configuration for USB Port
	// UBX_CFG_PRT       = 0x0600, // 20 Get/Set Get/Set Port Configuration for SPI Port
	// UBX_CFG_PRT       = 0x0600, // 20 Get/Set Get/Set Port Configuration for DDC Port
	UBX_CFG_RATE      = 0x0608, // 0 Poll Request Poll Navigation/Measurement Rate Settings
	// UBX_CFG_RATE      = 0x0608, // 6 Get/Set Navigation/Measurement Rate Settings
	UBX_CFG_RINV      = 0x0634, // 0 Poll Request Poll contents of Remote Inventory
	// UBX_CFG_RINV      = 0x0634, // 1 + 1*N Set/Get Set/Get contents of Remote Inventory
	UBX_CFG_RST       = 0x0604, // 4 Command Reset Receiver / Clear Backup Data Structures
	UBX_CFG_RXM       = 0x0611, // 0 Poll Request Poll RXM configuration
	// UBX_CFG_RXM       = 0x0611, // 2 Set/Get RXM configuration
	UBX_CFG_SBAS      = 0x0616, // 0 Poll Request Poll contents of SBAS Configuration
	// UBX_CFG_SBAS      = 0x0616, // 8 Command SBAS Configuration
	UBX_CFG_TMODE2    = 0x063D, // 0 Poll Request Poll Time Mode Settings
	// UBX_CFG_TMODE2    = 0x063D, // 28 Get/Set Time Mode Settings 2
	UBX_CFG_TMODE     = 0x061D, // 0 Poll Request Poll Time Mode Settings

	UBX_MON_HW2       = 0x0A0B, // 28 Periodic/Polled Extended Hardware Status
	UBX_MON_HW        = 0x0A09, // 68 Periodic/Polled Hardware Status
	// UBX_MON_HW        = 0x0A09, // 68 Periodic/Polled Hardware Status
	UBX_MON_IO        = 0x0A02, // 0 + 20*N Periodic/Polled I/O Subsystem Status
	UBX_MON_MSGPP     = 0x0A06, // 120 Periodic/Polled Message Parse and Process Status
	UBX_MON_RXBUF     = 0x0A07, // 24 Periodic/Polled Receiver Buffer Status
	UBX_MON_RXR       = 0x0A21, // 1 Get Receiver Status Information
	UBX_MON_TXBUF     = 0x0A08, // 28 Periodic/Polled Transmitter Buffer Status
	UBX_MON_VER       = 0x0A04, // 70 + 30*N Answer to Poll Receiver/Software/ROM Version

	UBX_NAV_AOPSTATUS = 0x0160, // 20 Periodic/Polled AssistNow Autonomous Status
	UBX_NAV_CLOCK     = 0x0122, // 20 Periodic/Polled Clock Solution
	UBX_NAV_DGPS      = 0x0131, // 16 + 12*numCh Periodic/Polled DGPS Data Used for NAV
	UBX_NAV_DOP       = 0x0104, // 18 Periodic/Polled Dilution of precision
	UBX_NAV_EKFSTATUS = 0x0140, // 36 Periodic/Polled Dead Reckoning Software Status
	UBX_NAV_POSECEF   = 0x0101, // 20 Periodic/Polled Position Solution in ECEF
	UBX_NAV_POSLLH    = 0x0102, // 28 Periodic/Polled Geodetic Position Solution
	UBX_NAV_SBAS      = 0x0132, // 12 + 12*cnt Periodic/Polled SBAS Status Data
	UBX_NAV_SOL       = 0x0106, // 52 Periodic/Polled Navigation Solution Information

	UBX_NAV_STATUS    = 0x0103, // 16 Periodic/Polled Receiver Navigation Status
	UBX_NAV_SVINFO    = 0x0130, // 8 + 12*numCh Periodic/Polled Space Vehicle Information
	UBX_NAV_TIMEGPS   = 0x0120, // 16 Periodic/Polled GPS Time Solution
	UBX_NAV_TIMEUTC   = 0x0121, // 20 Periodic/Polled UTC Time Solution
	UBX_NAV_VELECEF   = 0x0111, // 20 Periodic/Polled Velocity Solution in ECEF
	UBX_NAV_VELNED    = 0x0112 // 36 Periodic/Polled Velocity Solution in NED
};

enum UbxfieldType { UBX_FTYPE_UNDEF,
                 UBX_FTYPE_U1, UBX_FTYPE_I1, UBX_FTYPE_X1,
                 UBX_FTYPE_U2, UBX_FTYPE_I2, UBX_FTYPE_X2,
                 UBX_FTYPE_U4, UBX_FTYPE_I4, UBX_FTYPE_X4, UBX_FTYPE_R4,
                 UBX_FTYPE_R8,
                 UBX_FTYPE_CH };

union UbxValueDef {
    byte b;
    unsigned short us;
    unsigned int ui;
    float f;
    double d;
    long double ld;
};
    
struct UbxMsgField {
    UbxfieldType type = UBX_FTYPE_UNDEF;
    UbxValueDef value;
};

struct pInfo {
	byte* ptr = 0;
	unsigned short len = 0;
};

class UbxPacket {
	protected: 
        byte data[120] = { 0 };
		
		static unsigned short calcChecksum (byte* data, unsigned short offset, unsigned short dlen) {
			byte CK_A = 0, CK_B = 0;

			for (unsigned int i = offset; i < offset + dlen; i++) {
				CK_A = CK_A + data [i];
				CK_B = CK_B + CK_A;
			}

			unsigned short checksum = CK_A << 8;
			checksum |= CK_B;
			
			return checksum;
		};
		
	public:
        UbxPacket () {
            /*
            header[0].type = UBX_FTYPE_U1;
            header[0].value.b = UBX_SYNCCHAR1;
            
            header[1].type = UBX_FTYPE_U1;
            header[1].value.b = UBX_SYNCCHAR2;
            
            header[2].type = UBX_FTYPE_U1;
            header[2].value.b = msgClsID & 0xFF;
            
            header[3].type = UBX_FTYPE_U1;
            header[3].value.b = (msgClsID >> 8) & 0xFF;
            
            header[4].type = UBX_FTYPE_U2;
            header[4].value.us = length;
            */            
        };
        
		unsigned short msgClsID;
		unsigned short length = 0;
		unsigned short checksum = 0;
		
        /* returns an array of the bytes with sync chars, class, id, length, payload and checksum */
        virtual pInfo toArray () = 0;
            
        pInfo toPollingArray () {
			data[0] =  UBX_SYNCCHAR1;
			data[1] =  UBX_SYNCCHAR2;
			
            data[2] = (msgClsID >> 8) & 0xFF;
			data[3] = msgClsID & 0xFF;
			
			// length is always 0 for polling requests.
			data[4] = 0;
			data[5] = 0;
			
			checksum = calcChecksum (data, 2, 6);
			
			data[6] =(checksum >> 8) & 0xFF;
			data[7] =(checksum) & 0xFF;
            
			pInfo res;
			
			res.ptr = data;
			res.len = 8;
			
            return res;
        };
        
        /* sets class and id, length payload and checksum */
		bool fromArray (byte* data, unsigned short dlen) {
            if (dlen >= 4) {
				msgClsID = data[0];
				msgClsID |= data[1] << 8;
				
				length = data[3];
				length |= data[2] << 8;
            } else {
                return false;
            }
            
            if (dlen >= length + 4) {
                memcpy (&checksum, data + dlen - 2, 2);
                return true;
            } else {
                return false;
            }
        };
		
		// virtual unsigned short getPayloadSize();
};

/* ************************************************************************* */
class UbxAckAck: public UbxPacket {
	public:
		UbxAckAck() {
            Serial.println ("UbxAckAck() called");
			msgClsID = UBX_ACK_ACK;
            length = 2;
		};
		
		byte clsID = 0;
		byte msgID = 0;
        
        pInfo toArray () {
			data[0] =  UBX_SYNCCHAR1;
			data[1] =  UBX_SYNCCHAR2;
			
            data[2] = (msgClsID >> 8) & 0xFF;
			data[3] = msgClsID & 0xFF;
			
			data[4] = length & 0xFF;
			data[5] = (length >> 8) & 0xFF;
			
            data[6] = clsID;
            data[7] = msgID;
            
			checksum = calcChecksum (data, 2, 6);
			
			data[8] =(checksum >> 8) & 0xFF;
			data[9] =(checksum) & 0xFF;
            
			pInfo res;
			
			res.ptr = data;
			res.len = 10;
			
            return res;
        }
        
        /* sets class and id, length payload and checksum */
        bool fromArray (byte* data, unsigned short msgLen) {
            if (msgLen == 10) {
                memcpy (&msgClsID, data + 2, 2);
                memcpy (&length, data + 4, 2);
                memcpy (&clsID, data + 6, 1);
                memcpy (&msgID, data + 7, 1);
                memcpy (&checksum, data + 8, 2);
                return true;
            } else {
                return false;
            }
        };
};

/* ************************************************************************* */
struct UbxAckPayload  {
	byte pclsID = 0;
	byte pmsgID = 0;
};

class UbxAckNak: public UbxPacket {
	public:
		UbxAckNak() {
			msgClsID = UBX_ACK_NAK;
		};
		
		UbxAckPayload pl;

		byte clsID = 0;
		byte msgID = 0;
};

/* ************************************************************************* */
class UbxCfgMsgNmea: public UbxPacket {
	public:
		UbxCfgMsgNmea () {
			msgClsID = UBX_CFG_MSG;
			length = 8;
		};
		
        unsigned short cfgMsgClsID = 0;
        byte rate[6] =  { 0 };
		
		pInfo toArray () {
			data[0] =  UBX_SYNCCHAR1;
			data[1] =  UBX_SYNCCHAR2;
			
            data[2] = (msgClsID >> 8) & 0xFF;
			data[3] = msgClsID & 0xFF;
			
			data[4] = length & 0xFF;
			data[5] = (length >> 8) & 0xFF;
            
            data[6] = (cfgMsgClsID >> 8) & 0xFF;
            data[7] = cfgMsgClsID & 0xFF;
			
			data[8] = rate[0];
			data[9] = rate[1];
			data[10] = rate[2];
			data[11] = rate[3];
			data[12] = rate[4];
			data[13] = rate[5];
            
			checksum = calcChecksum (data, 2, 12);
			
			data[14] =(checksum >> 8) & 0xFF;
			data[15] =(checksum) & 0xFF;
			
            pInfo res;
			res.ptr = data;
			res.len = 16;
			
            return res;
		};
};

/* ************************************************************************* */
class UbxCfgMsg: public UbxPacket {
	public:
		UbxCfgMsg () {
			msgClsID = UBX_CFG_MSG;
			length = 3;
		};
		
        unsigned short cfgMsgClsID = 0;
        byte rate = 0;
		
		pInfo toPollingArray () {
			data[0] =  UBX_SYNCCHAR1;
			data[1] =  UBX_SYNCCHAR2;
			
            data[2] = (msgClsID >> 8) & 0xFF;
			data[3] = msgClsID & 0xFF;
			
			data[4] = 0x02;
			data[5] = 0;
			
			data[6] = (cfgMsgClsID >> 8) & 0xFF;
            data[7] = cfgMsgClsID & 0xFF;
			
			checksum = calcChecksum (data, 2, 6);
			
			data[8] =(checksum >> 8) & 0xFF;
			data[9] =(checksum) & 0xFF;
            
			pInfo res;
			
			res.ptr = data;
			res.len = 10;
			
            return res;
        };
		
		pInfo toArray () {
			data[0] =  UBX_SYNCCHAR1;
			data[1] =  UBX_SYNCCHAR2;
			
            data[2] = (msgClsID >> 8) & 0xFF;
			data[3] = msgClsID & 0xFF;
			
			data[4] = (length) & 0xFF;
			data[5] = (length >> 8) & 0xFF;
            
            data[6] = (cfgMsgClsID >> 8) & 0xFF;
            data[7] = cfgMsgClsID & 0xFF;
			data[8] = rate;
            
			checksum = calcChecksum (data, 2, 7);
			
			data[9] =(checksum >> 8) & 0xFF;
			data[10] =(checksum) & 0xFF;
			
            pInfo res;
			res.ptr = data;
			res.len = 11;
			
            return res;
		};
};

/* ************************************************************************* */
struct UbxNavStatusPayload {
	unsigned int iTOW = 0;
	byte gpsFix = 0;
	byte flags = 0;
	byte fixStat = 0;
	byte flags2 = 0;
	unsigned int ttff;
	unsigned int msss;
};

class UbxNavStatus: public UbxPacket {
	public:
		UbxNavStatus () {
			msgClsID = UBX_NAV_STATUS;
			length = 24;
		};
		
		unsigned int iTOW = 0;
		byte gpsFix = 0;
		byte flags = 0;
		byte fixStat = 0;
		byte flags2 = 0;
		unsigned int ttff;
		unsigned int msss;
        
        pInfo toArray () {   
			data[0] =  UBX_SYNCCHAR1;
			data[1] =  UBX_SYNCCHAR2;
			
            data[2] = (msgClsID >> 8) & 0xFF;
			data[3] = msgClsID & 0xFF;
			
			data[4] = (length) & 0xFF;
			data[5] = (length >> 8) & 0xFF;
            
            data[6] = (iTOW) & 0xFF;
            data[7] = (iTOW >> 8) & 0xFF;
            data[8] = (iTOW >> 16) & 0xFF;
            data[9] = (iTOW >> 24) & 0xFF;
            
            data[10] = gpsFix;
            data[11] = flags;
            data[12] = fixStat;
            data[13] = flags2;
            
            data[14] = (ttff) & 0xFF;
            data[15] = (ttff >> 8) & 0xFF;
            data[16] = (ttff >> 16) & 0xFF;
            data[17] = (ttff >> 24) & 0xFF;
            
            data[18] = (msss) & 0xFF;
            data[19] = (msss >> 8) & 0xFF;
            data[20] = (msss >> 16) & 0xFF;
            data[21] = (msss >> 24) & 0xFF;
            
			checksum = calcChecksum (data, 2, 20);
			
			data[22] =(checksum >> 8) & 0xFF;
			data[23] =(checksum) & 0xFF;
			
            pInfo res;
			res.ptr = data;
			res.len = 24;
			
            return res;
        };
};

struct UbxNavPosLLHPayload {
	unsigned int iTOW = 0;
	int lon = 0;
	int lat = 0;
	int height = 0;
	int hMSL = 0;
	unsigned int hAcc = 0;
	unsigned int vAcc = 0;
};

/* ************************************************************************* */
class UbxNavPosLLH: public UbxPacket {
	public:
		UbxNavPosLLH () {
			msgClsID = UBX_NAV_POSLLH;
		};
		
		unsigned int iTOW = 0;
		int lon = 0;
        int lat = 0;
        int height = 0;
        int hMSL = 0;
        unsigned int hAcc = 0;
        unsigned int vAcc = 0;
};


struct UbxNavSolPayload {
	unsigned int iTOW = 0;
	int fTOW;
	short week;
	byte gpsFix;
	byte flags;
	int ecefX;
	int ecefY;
	int ecefZ;
	unsigned int pAcc;
	int ecefVX;
	int ecefVY;
	int ecefVZ;
	unsigned int sAcc;
	unsigned short pDOP;
	byte reserved1;
	byte numSV;
	unsigned int reserved2;
};

/* ****************************************************************************
  UBX Messages
  ****************************************************************************/
// ACK_ACK 0x05 0x01 2 Answer Message Acknowledged
// ACK_NAK 0x05 0x00 2 Answer Message Not_Acknowledged
// 
// AID_ALM 0x0B 0x30 0 Poll Request Poll GPS Aiding Almanac Data
// AID_ALM 0x0B 0x30 1 Poll Request Poll GPS Aiding Almanac Data for a SV
// AID_ALM 0x0B 0x30 (8) or (40) Input/Output Message GPS Aiding Almanac Input/Output Message
// AID_ALPSRV 0x0B 0x32 16 Output Message ALP client requests AlmanacPlus data from server
// AID_ALPSRV 0x0B 0x32 16 + 1*dataSize Input Message ALP server sends AlmanacPlus data to client
// AID_ALPSRV 0x0B 0x32 8 + 2*size Output Message ALP client sends AlmanacPlus data to server.
// AID_ALP 0x0B 0x50 0 + 2*N Input message ALP file data transfer to the receiver
// AID_ALP 0x0B 0x50 1 Input message Mark end of data transfer
// AID_ALP 0x0B 0x50 1 Output message Acknowledges a data transfer
// AID_ALP 0x0B 0x50 1 Output message Indicate problems with a data transfer
// AID_ALP 0x0B 0x50 24 Periodic/Polled Poll the AlmanacPlus status
// AID_AOP 0x0B 0x33 0 Poll request Poll AssistNow Autonomous data
// AID_AOP 0x0B 0x33 1 Poll request Poll AssistNow Autonomous data for one satellite
// AID_AOP 0x0B 0x33 (48) or (192) Input/Output Message AssistNow Autonomous data
// AID_DATA 0x0B 0x10 0 Poll Polls all GPS Initial Aiding Data
// AID_EPH 0x0B 0x31 0 Poll Request Poll GPS Aiding Ephemeris Data
// AID_EPH 0x0B 0x31 1 Poll Request Poll GPS Aiding Ephemeris Data for a SV
// AID_EPH 0x0B 0x31 (8) or (104) Input/Output Message GPS Aiding Ephemeris Input/Output Message
// AID_HUI 0x0B 0x02 0 Poll Request Poll GPS Health, UTC and ionosphere parameters
// AID_HUI 0x0B 0x02 72 Input/Output Message GPS Health, UTC and ionosphere parameters
// AID_INI 0x0B 0x01 0 Poll Request Poll GPS Initial Aiding Data
// AID_INI 0x0B 0x01 48 Polled Aiding position, time, frequency, clock drift
// AID_REQ 0x0B 0x00 0 Virtual Sends a poll (AID_DATA) for all GPS Aiding Data
// 
// CFG_ANT 0x06 0x13 0 Poll Request Poll Antenna Control Settings
// CFG_ANT 0x06 0x13 4 Get/Set Get/Set Antenna Control Settings
// CFG_CFG 0x06 0x09 (12) or (13) Command Clear, Save and Load configurations
// CFG_DAT 0x06 0x06 0 Poll Request Poll Datum Setting
// CFG_DAT 0x06 0x06 2 Set Set Standard Datum
// CFG_DAT 0x06 0x06 44 Set Set User_defined Datum
// CFG_DAT 0x06 0x06 52 Get Get currently selected Datum
// CFG_EKF 0x06 0x12 0 Poll Request Poll EKF Module Settings
// CFG_EKF 0x06 0x12 16 Get/Set Get/Set EKF Module Settings _ LEA_6R
// CFG_ESFGWT 0x06 0x29 44 Get/Set message Get/Set settings of gyro+wheel tick sol (GWT) _ LEA_6R
// CFG_FXN 0x06 0x0E 0 Poll Request Poll FXN configuration
// CFG_FXN 0x06 0x0E 36 Command RXM FixNOW configuration.
// CFG_INF 0x06 0x02 1 Poll Request Poll INF message configuration for one protocol
// CFG_INF 0x06 0x02 0 + 10*N Set/Get Information message configuration
// CFG_ITFM 0x06 0x39 8 Command Jamming/Interference Monitor configuration.
// CFG_MSG 0x06 0x01 2 Poll Request Poll a message configuration
// CFG_MSG 0x06 0x01 8 Set/Get Set Message Rate(s)
// CFG_MSG 0x06 0x01 3 Set/Get Set Message Rate
// CFG_NAV5 0x06 0x24 0 Poll Request Poll Navigation Engine Settings
// CFG_NAV5 0x06 0x24 36 Get/Set Get/Set Navigation Engine Settings
// CFG_NAVX5 0x06 0x23 0 Poll Request Poll Navigation Engine Expert Settings
// CFG_NAVX5 0x06 0x23 40 Get/Set Get/Set Navigation Engine Expert Settings
// CFG_NMEA 0x06 0x17 0 Poll Request Poll the NMEA protocol configuration
// CFG_NMEA 0x06 0x17 4 Set/Get Set/Get the NMEA protocol configuration
// CFG_NVS 0x06 0x22 13 Command Clear, Save and Load non_volatile storage data
// CFG_PM2 0x06 0x3B 0 Poll Request Poll extended Power Management configuration
// CFG_PM2 0x06 0x3B 44 Set/Get Extended Power Management configuration
// CFG_PM 0x06 0x32 0 Poll Request Poll Power Management configuration
// CFG_PM 0x06 0x32 24 Set/Get Power Management configuration
// CFG_PRT 0x06 0x00 0 Poll Request Polls the configuration of the used I/O Port
// CFG_PRT 0x06 0x00 1 Poll Request Polls the configuration for one I/O Port
// CFG_PRT 0x06 0x00 20 Get/Set Get/Set Port Configuration for UART
// CFG_PRT 0x06 0x00 20 Get/Set Get/Set Port Configuration for USB Port
// CFG_PRT 0x06 0x00 20 Get/Set Get/Set Port Configuration for SPI Port
// CFG_PRT 0x06 0x00 20 Get/Set Get/Set Port Configuration for DDC Port
// CFG_RATE 0x06 0x08 0 Poll Request Poll Navigation/Measurement Rate Settings
// CFG_RATE 0x06 0x08 6 Get/Set Navigation/Measurement Rate Settings
// CFG_RINV 0x06 0x34 0 Poll Request Poll contents of Remote Inventory
// CFG_RINV 0x06 0x34 1 + 1*N Set/Get Set/Get contents of Remote Inventory
// CFG_RST 0x06 0x04 4 Command Reset Receiver / Clear Backup Data Structures
// CFG_RXM 0x06 0x11 0 Poll Request Poll RXM configuration
// CFG_RXM 0x06 0x11 2 Set/Get RXM configuration
// CFG_SBAS 0x06 0x16 0 Poll Request Poll contents of SBAS Configuration
// CFG_SBAS 0x06 0x16 8 Command SBAS Configuration
// CFG_TMODE2 0x06 0x3D 0 Poll Request Poll Time Mode Settings
// CFG_TMODE2 0x06 0x3D 28 Get/Set Time Mode Settings 2
// CFG_TMODE 0x06 0x1D 0 Poll Request Poll Time Mode Settings
// 
// CFG_TMODE 0x06 0x1D 28 Get/Set Time Mode Settings
// CFG_TP5 0x06 0x31 0 Poll Request Poll Timepulse Parameters
// CFG_TP5 0x06 0x31 1 Poll Request Poll TimePulse Parameters
// CFG_TP5 0x06 0x31 32 Get/Set Get/Set TimePulse Parameters
// CFG_TP 0x06 0x07 0 Poll Request Poll TimePulse Parameters
// CFG_TP 0x06 0x07 20 Get/Set Get/Set TimePulse Parameters
// CFG_USB 0x06 0x1B 0 Poll Request Poll a USB configuration
// CFG_USB 0x06 0x1B 108 Get/Set Get/Set USB Configuration
// 
// ESF_MEAS 0x10 0x02 (8 + 4*N) or (12 + 4*N)
// 
// ESF_STATUS 0x10 0x10 16 + 4*numSens Periodic/Polled Sensor Fusion Status Information (LEA_6R)
// ESF_STATUS 0x10 0x10 16 + 4*numSens Periodic/Polled Sensor Fusion Status Information (LEA_6R)
// 
// INF_DEBUG 0x04 0x04 0 + 1*N Output ASCII String output, indicating debug output
// INF_ERROR 0x04 0x00 0 + 1*N Output ASCII String output, indicating an error
// INF_NOTICE 0x04 0x02 0 + 1*N Output ASCII String output, with informational contents
// INF_TEST 0x04 0x03 0 + 1*N Output ASCII String output, indicating test output
// INF_WARNING 0x04 0x01 0 + 1*N Output ASCII String output, indicating a warning
// 
// 
// MON_HW2 0x0A 0x0B 28 Periodic/Polled Extended Hardware Status
// MON_HW 0x0A 0x09 68 Periodic/Polled Hardware Status
// MON_HW 0x0A 0x09 68 Periodic/Polled Hardware Status
// MON_IO 0x0A 0x02 0 + 20*N Periodic/Polled I/O Subsystem Status
// MON_MSGPP 0x0A 0x06 120 Periodic/Polled Message Parse and Process Status
// MON_RXBUF 0x0A 0x07 24 Periodic/Polled Receiver Buffer Status
// MON_RXR 0x0A 0x21 1 Get Receiver Status Information
// MON_TXBUF 0x0A 0x08 28 Periodic/Polled Transmitter Buffer Status
// MON_VER 0x0A 0x04 70 + 30*N Answer to Poll Receiver/Software/ROM Version
// 
// 
// NAV_AOPSTATUS 0x01 0x60 20 Periodic/Polled AssistNow Autonomous Status
// NAV_CLOCK 0x01 0x22 20 Periodic/Polled Clock Solution
// NAV_DGPS 0x01 0x31 16 + 12*numCh Periodic/Polled DGPS Data Used for NAV
// NAV_DOP 0x01 0x04 18 Periodic/Polled Dilution of precision
// NAV_EKFSTATUS 0x01 0x40 36 Periodic/Polled Dead Reckoning Software Status
// NAV_POSECEF 0x01 0x01 20 Periodic/Polled Position Solution in ECEF
// NAV_POSLLH 0x01 0x02 28 Periodic/Polled Geodetic Position Solution
// NAV_SBAS 0x01 0x32 12 + 12*cnt Periodic/Polled SBAS Status Data
// NAV_SOL 0x01 0x06 52 Periodic/Polled Navigation Solution Information
// 
// NAV_STATUS 0x01 0x03 16 Periodic/Polled Receiver Navigation Status
// NAV_SVINFO 0x01 0x30 8 + 12*numCh Periodic/Polled Space Vehicle Information
// NAV_TIMEGPS 0x01 0x20 16 Periodic/Polled GPS Time Solution
// NAV_TIMEUTC 0x01 0x21 20 Periodic/Polled UTC Time Solution
// NAV_VELECEF 0x01 0x11 20 Periodic/Polled Velocity Solution in ECEF
// NAV_VELNED 0x01 0x12 36 Periodic/Polled Velocity Solution in NED
// 
// RXM_ALM 0x02 0x30 0 Poll Request Poll GPS Constellation Almanach Data
// RXM_ALM 0x02 0x30 1 Poll Request Poll GPS Constellation Almanach Data for a SV
// RXM_ALM 0x02 0x30 (8) or (40) Poll Answer / Periodic GPS Aiding Almanach Input/Output Message
// RXM_EPH 0x02 0x31 0 Poll Request Poll GPS Constellation Ephemeris Data
// RXM_EPH 0x02 0x31 1 Poll Request Poll GPS Constellation Ephemeris Data for a SV
// RXM_EPH 0x02 0x31 (8) or (104) Poll Answer / Periodic GPS Aiding Ephemeris Input/Output Message
// RXM_PMREQ 0x02 0x41 8 Input Requests a Power Management task
// RXM_RAW 0x02 0x10 8 + 24*numSV Periodic/Polled Raw Measurement Data
// RXM_SFRB 0x02 0x11 42 Periodic Subframe Buffer
// RXM_SVSI 0x02 0x20 8 + 6*numSV Periodic/Polled SV Status Info
// 
// TIM_SVIN 0x0D 0x04 28 Periodic/Polled Survey_in data
// TIM_TM2 0x0D 0x03 28 Periodic/Polled Time mark data
// TIM_TP 0x0D 0x01 16 Periodic/Polled Timepulse Timedata
// TIM_VRFY 0x0D 0x06 20 Polled/Once Sourced Time Verification

#endif
