# ArduinoUbxLib
# Purpose
A C++ Arduino library that helps communicating with uBlox GNSS receivers via the proprietary ubx protocol. 

# Software library
## Design Goals
1. Easy to use and useful
2. Extendable


## How to use the library
The main part of the library is the class *UbxGps* that provides the functionality to parse data that is input via the *parse()* method. Any correctly identified (and known) UBX messages, NMEA GPS or proprietary PUBX messages are reported with the *onReceive()* methods. When overriding these methods the identified messages can be further processed. Messages generally have the payload *pl* included which is a struct containing all specific data. For example, the structure of the NAV-STATUS ubx message is as follows:

```
struct UbxNavStatusPayload {
	unsigned int iTOW = 0;
	byte gpsFix = 0;
	byte flags = 0;
	byte fixStat = 0;
	byte flags2 = 0;
	unsigned int ttff = 0;
	unsigned int msss = 0;
};
```

# Hardware
I tested the software with my GY-GPS6MV2 module that has a uBlox NEO-6M chip. Compatibility with other uBlox chips is not guaranteed, but feedback is highly appreciated!
