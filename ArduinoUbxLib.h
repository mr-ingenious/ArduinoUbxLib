#ifndef __AUBXLIB_H_
#define __AUBXLIB_H_

#include "UbxBaseMsg.h"
#include "UbxCfgMsg.h"
#include "UbxNavMsg.h"
#include "UbxMonMsg.h"
#include "UbxRXMMsg.h"

#include "UbxNMEA.h"

#include "Arduino.h"

#define UBX_LOG(lbr, ...) if (logOutput) { if (lbr) { Serial.println ( __VA_ARGS__); } else { Serial.print ( __VA_ARGS__); }};
#define UBX_LOGF(lbr, msg) if (logOutput) { if (lbr) { Serial.println (F(msg)); } else { Serial.print (F(msg)); }};

/* ****************************************************************************
  Basic definitions
  ****************************************************************************/
enum UbxParserStates {
    PSTATE_INIT         = 0,
    
    PSTATE_UBX_SYNC1    = 1, // first sync char found
    PSTATE_UBX_SYNC2    = 2, // second sync char found
    PSTATE_UBX_MSGCLS   = 3,
    PSTATE_UBX_MSGID    = 4,
    PSTATE_UBX_LEN1     = 5,
    PSTATE_UBX_LEN2     = 6,
    PSTATE_UBX_PAYLOAD  = 7,
    PSTATE_UBX_CHK_A    = 8,
                        
    PSTATE_NMEA_SYNC    = 20, // $ char
                        
    PSTATE_NMEA_IDGP1   = 21, // NMEA GP ...
                        
    PSTATE_NMEA_IDP     = 22, // UBX proprietary PUBX
    PSTATE_NMEA_IDU     = 23,
    PSTATE_NMEA_IDB     = 24,
    PSTATE_NMEA_IDX     = 25,
    
    PSTATE_NMEA_PAYLOAD = 26,
    
    PSTATE_NMEA_CR      = 27, // Carriage Return
    PSTATE_NMEA_LF      = 28  // Linefeed --> NMEA message complete
};
  
enum UBX_PARSER_PACKETTYPE {UBX_NONE = 0, UBX_BINMSG = 1, UBX_GPMSG = 2, UBX_PUBXMSG = 3, UBX_UNSUPPORTED_MSG = 10};
  
struct UbxParserInfo {
    UbxParserStates state = PSTATE_INIT;
    unsigned short pbCount = 0; // payload bytes count
    unsigned short payloadLen = 0;
    unsigned short msgClsID = UBX_UNDEFINED_MSGTYPE;
    byte msgType = UBX_NONE;
};

const unsigned short __inBufLen = 500;

/* ****************************************************************************
  UbxGps: the main class of the UBX message parser.
  ****************************************************************************/
class UbxGps {
  private:
    UbxParserInfo parseInfo;
    byte inBuf[__inBufLen] = { 0 };
    
  protected:
    bool logOutput = false;
    
    void resetParser ();

    byte handleUbxPacket ();
    virtual byte handleUnsupportedBinMsg (byte* buffer, unsigned short len) { return UBX_UNSUPPORTED_MSG; };
    void handleNMEA_GPMsg ();
    void handleNMEA_PUBXMsg ();
    
    void setLogOutput (bool newLogOutput) {
        logOutput = newLogOutput;
    };
    
  public:
    UbxGps ();
    
    byte parse (byte data);
    virtual void onReceive (UbxPacket* p) {};
    virtual void onReceive (char* nmeaMsg, unsigned short mLen) {};
};

#endif
