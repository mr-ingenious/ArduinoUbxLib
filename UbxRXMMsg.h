#ifndef __AUBXLIB_RXMMSG_H_
#define __AUBXLIB_RXMMSG_H_

#include "Arduino.h"

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
				pl.iTOW  = buffer[9] << 24;
				pl.iTOW |= buffer[8] << 16;
				pl.iTOW |= buffer[7] << 8;
				pl.iTOW |= buffer[6];
				
				pl.week  = buffer[11] << 8;
				pl.week |= buffer[10];
				
				pl.numVis = buffer[12];
				
				pl.numSV = buffer[13];
				
				for (unsigned short svc = 0; svc < pl.numSV; svc++) {
					if (svc == 80) {
						break;
					}
					
					pl.svInfo[svc].svid   = buffer[14 + 6 * svc];
					
					pl.svInfo[svc].svFlag = buffer[15 + 6 * svc];
					
					pl.svInfo[svc].azim   = buffer[17 + 6 * svc] << 8;
					pl.svInfo[svc].azim  |= buffer[16 + 6 * svc];
					
					pl.svInfo[svc].elev   = buffer[18 + 6 * svc];
					pl.svInfo[svc].age    = buffer[19 + 6 * svc];
				}
				
				checksum  = buffer[len - 2] << 8;
				checksum |= buffer[len - 1];
				
				valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
			}
		};
};

#endif
