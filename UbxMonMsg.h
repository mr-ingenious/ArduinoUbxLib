#ifndef __AUBXLIB_MONMSG_H_
#define __AUBXLIB_MONMSG_H_

#include "Arduino.h"

/* ************************************************************************* */
struct UbxMonVerPayload {
	byte swVersion[30] = { 0 };
	byte hwVersion[10] = { 0 };
	// byte romVersion[30] = { 0 };
};

class UbxMonVer: public UbxPacket {
	public:
		UbxMonVerPayload pl;
			
		UbxMonVer () {
			h.msgClsID = UBX_MON_VER;
			h.length = 40; // MINIMUM!
		};
		
		UbxMonVer (byte* buffer, unsigned short len) {
			h.msgClsID = UBX_MON_VER;
			h.length = len - 8;
			
			if (len >= 48) {
				for (unsigned short bc = 6; bc < 36; bc++) {
					pl.swVersion[bc - 6]  = buffer[bc];
				};

				for (unsigned short bc = 36; bc < 46; bc++) {
					pl.hwVersion[bc - 36]  = buffer[bc];
				};
			}
			
			// if (len >= 78) {
			// 	for (unsigned short bc = 46; bc < 76; bc++) {
			// 		pl.romVersion[bc - 46]  = buffer[bc];
			// 	};
			// }
			
			checksum  = buffer[len - 2] << 8;
			checksum |= buffer[len - 1];
			
			valid = isChecksumValid (buffer, 2, h.length + 4, checksum);
		};
};

#endif
