#include "ArduinoUbxLib.h"

#include "Arduino.h"

/* ****************************************************************************
  Constructor:
  ****************************************************************************/
UbxGps::UbxGps () {
};

/* ****************************************************************************
  handleUbxPacket: analyzes the information the parser collected and creates
                   objects of known messages to be handed over to the
                   onReceive() method. If the message is unknown, it is handed
                   over to the handleUnsupportedBinMsg () method for further
                   processing.
  ****************************************************************************/
byte UbxGps::handleUbxPacket () {
    if (logOutput)  {
      UBX_LOGF (false, "handleUbxPacket: UBX binary [");
      for (unsigned short i = 0; i < parseInfo.payloadLen + 8; i++) {
        if (inBuf [i] < 0x10) {
          UBX_LOGF (false, "0");
        }
      
        UBX_LOG (false, inBuf [i], HEX);
        UBX_LOGF (false, " ");

        if (i == 5) {
          UBX_LOGF (false, " | ");
        }
      }
      
      UBX_LOGF (true, "]");
    }
    
    UbxPacket* packet = NULL;
    byte res = UBX_NONE;
  
  switch (parseInfo.msgClsID) {
    case UBX_ACK_ACK:    
      packet = new UbxAckAck (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_ACK_NAK:
      packet = new UbxAckNak (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_STATUS:
      packet = new UbxNavStatus (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_SOL:
      packet = new UbxNavSol (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_POSLLH:
      packet = new UbxNavPosLLH (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_TIMEUTC:
      packet = new UbxNavTimeUTC (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_TIMEGPS:
      packet = new UbxNavTimeGPS (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_CFG_NMEA:
      packet = new UbxCfgNMEA (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_CFG_RATE:
      packet = new UbxCfgRate (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_CFG_RXM:
      packet = new UbxCfgRXM (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_MON_VER:
      packet = new UbxMonVer (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_RXM_SVSI:
      packet = new UbxRxmSVSI (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_CLOCK:
      packet = new UbxNavClock (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_DGPS:
      packet = new UbxNavDGPS (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_NAV_DOP:
      packet = new UbxNavDOP (inBuf, parseInfo.payloadLen + 8);
      break;
      
    case UBX_CFG_PM:
      packet = new UbxCfgPM (inBuf, parseInfo.payloadLen + 8);
      break;
      
    default:
      UBX_LOGF (false, "UBX msg with unknown type detected, cls/msgid: ");
      UBX_LOG (true, parseInfo.msgClsID, HEX);
      res = handleUnsupportedBinMsg (inBuf, parseInfo.payloadLen + 8);
  }
  
  if (NULL != packet && packet->valid) {
      onReceive (packet);
      delete packet;
      res = UBX_BINMSG;
  } else {
      if (NULL == packet) {
          UBX_LOGF (true, "packet is NULL! ");
      } else if (!packet->valid) {
          UBX_LOGF (true, "packet invalid! ");
      }
  }
  
  return res;
};

/* ****************************************************************************
  handleNMEA_GPMsg: calls the onReceive() method for NMEA strings.
  ****************************************************************************/
void UbxGps::handleNMEA_GPMsg () {
  onReceive ((char*) inBuf, parseInfo.pbCount);
  
  if (false) {
    UBX_LOGF (false, "handleNMEA_GPMsg: NMEA GP [");
    for (unsigned short i = 0; i < parseInfo.pbCount; i++) {
      if (inBuf [i] == 0x0D) {
        UBX_LOGF (false, "<CR>");
      } else if (inBuf [i] == 0x0D) {
        UBX_LOGF (true, "<LF>]");
      }else {
        UBX_LOG (false,(char) inBuf [i]);
      }
    }
    UBX_LOGF (true, "");
  }
};

/* ****************************************************************************
  handleNMEA_PUBXMsg: calls the onReceive() method for PUBX strings.
  ****************************************************************************/
void UbxGps::handleNMEA_PUBXMsg () {
  onReceive ((char*) inBuf, parseInfo.pbCount);
  
  if (false) {
    UBX_LOGF (false, "handleNMEA_PUBXMsg: NMEA PUBX [");
    for (unsigned short i = 0; i < parseInfo.pbCount; i++) {
      if (inBuf [i] == 0x0D) {
        UBX_LOGF (false, "<CR>");
      } else if (inBuf [i] == 0x0D) {
        UBX_LOGF (true, "<LF>]");
      } else {
        UBX_LOGF (false, (char) inBuf [i]);
      }
    }
    UBX_LOGF (true, "");
  }
};

/* ****************************************************************************
  resetParser: resets the inner parser info structure.
  ****************************************************************************/
void UbxGps::resetParser () {
  parseInfo.state = PSTATE_INIT;
  parseInfo.msgClsID = UBX_UNDEFINED_MSGTYPE;
  parseInfo.pbCount = 0;
  parseInfo.payloadLen = 0;
  parseInfo.msgType = UBX_NONE;
  
  memset (inBuf, 0, __inBufLen);
}

/* ****************************************************************************
  parse: analyzes the input byte and updates the inner parser state. If a
         complete ubx message, a NMEA or PUBX string has been identified, the
         corresponding handle...() methods are called for further processing.
  ****************************************************************************/
byte UbxGps::parse (byte data) {
  const UbxParserStates lastState = parseInfo.state;
  byte res = UBX_NONE;
  
  switch (parseInfo.state) {
    case PSTATE_INIT:
      if (data == UBX_SYNCCHAR1) {
        inBuf [0] = data;
        parseInfo.state = PSTATE_UBX_SYNC1;
      } else if (data == '$') {
        inBuf [parseInfo.pbCount++] = data;
        parseInfo.state = PSTATE_NMEA_SYNC;
      }
      break;
      
    case PSTATE_UBX_SYNC1:
      if (data == UBX_SYNCCHAR2) {
        inBuf [1] = data;
        parseInfo.state = PSTATE_UBX_SYNC2;
      } else {
        UBX_LOGF (true, "parse: PSTATE_UBX_SYNC1: unexpected character, reset.");
        resetParser ();
      }
      break;
      
    case PSTATE_UBX_SYNC2:
      if (data == CLS_NAV ||
          data == CLS_RXM ||
          data == CLS_INF ||
          data == CLS_ACK ||
          data == CLS_CFG ||
          data == CLS_MON ||
          data == CLS_AID ||
          data == CLS_TIM ||
          data == CLS_ESF) {
        parseInfo.state = PSTATE_UBX_MSGCLS;
        parseInfo.msgClsID = data << 8;
        inBuf [2] = data;
      } else {
        UBX_LOGF (true, "parse: PSTATE_UBX_SYNC2: unexpected character, reset.");
        resetParser ();
      }
      break;
      
    case PSTATE_UBX_MSGCLS:
      inBuf [3] = data;
      parseInfo.msgClsID |= data;
      parseInfo.state = PSTATE_UBX_MSGID;
      break;
      
    case PSTATE_UBX_MSGID: // first length byte
      inBuf [4] = data;
      parseInfo.payloadLen = data;
      parseInfo.state = PSTATE_UBX_LEN1;
      break;
      
    case PSTATE_UBX_LEN1: // second length byte
      inBuf [5] = data;
      parseInfo.payloadLen |= data << 8;
      // UBX_LOGF (false, "PSTATE_LEN2: payloadLen: ");
      // UBX_LOG (true, parseInfo.payloadLen);
      parseInfo.state = PSTATE_UBX_LEN2;
      break;
      
    case PSTATE_UBX_LEN2: // payload ...
      // UBX_LOGF (false, "PSTATE_LEN2: payload, pbCount: ");
      // UBX_LOG (true, parseInfo.pbCount);
      
      if (6 + parseInfo.pbCount < __inBufLen) {
        inBuf [6 + parseInfo.pbCount] = data;
      
        if (parseInfo.pbCount == parseInfo.payloadLen - 1) {
            parseInfo.state = PSTATE_UBX_PAYLOAD;
            // UBX_LOGF (true, "PSTATE_LEN2: payload done.");
        }
        parseInfo.pbCount++;
      } else {
          UBX_LOGF (true, "parse: PSTATE_UBX_LEN2: msg length exceeds buffer length, reset.");
          resetParser ();
      }
      break;
      
    case PSTATE_UBX_PAYLOAD:
      // UBX_LOGF (true, "PSTATE_PAYLOAD: CK_A found.");
      if (6 + parseInfo.payloadLen < __inBufLen) {
        inBuf [6 + parseInfo.payloadLen] = data;
        parseInfo.state = PSTATE_UBX_CHK_A;
      } else {
          UBX_LOGF (true, "parse: PSTATE_UBX_PAYLOAD: msg length exceeds buffer length, reset.");
          resetParser ();
      }
      break;
      
    case PSTATE_UBX_CHK_A:
      // UBX_LOGF (true, "PSTATE_PAYLOAD: CK_B found.");
      if (7 + parseInfo.payloadLen < __inBufLen) {
        inBuf [7 + parseInfo.payloadLen] = data;        
        res = handleUbxPacket ();
      }
      // UBX_LOGF (true, "parse: PSTATE_UBX_CHK_A: msg complete, reset");
      resetParser ();
      break;
      
    case PSTATE_NMEA_SYNC:
      inBuf[parseInfo.pbCount++] = data;
      if  (data == 'G') {
        parseInfo.state = PSTATE_NMEA_IDGP1;
      } else if (data == 'P') {
         parseInfo.state = PSTATE_NMEA_IDP;
      } else {
          UBX_LOGF (true, "parse: PSTATE_NMEA_SYNC: unexpected character, reset.");
          resetParser ();
      }
      break;
      
    case PSTATE_NMEA_IDGP1:
      inBuf[parseInfo.pbCount++] = data;
      if  (data == 'P') {
        parseInfo.state = PSTATE_NMEA_PAYLOAD;
        parseInfo.msgType = UBX_GPMSG;
      } else {
        UBX_LOGF (true, "parse: PSTATE_NMEA_IDGP1: unexpected character, reset.");
        resetParser ();
      }
      break;
      
    case PSTATE_NMEA_IDP:
      inBuf[parseInfo.pbCount++] = data;
      if  (data == 'U') {
        parseInfo.state = PSTATE_NMEA_IDU;
      } else {
        UBX_LOGF (true, "parse: PSTATE_NMEA_IDP: unexpected character, reset.");
        resetParser ();
      }
      break;
      
    case PSTATE_NMEA_IDU:
      inBuf[parseInfo.pbCount++] = data;
      if  (data == 'B') {
        parseInfo.state = PSTATE_NMEA_IDB;
      } else {
        UBX_LOGF (true, "parse: PSTATE_NMEA_IDU: unexpected character, reset.");
        resetParser ();
      }
      break;
      
    case PSTATE_NMEA_IDB:
      inBuf[parseInfo.pbCount++] = data;
      if  (data == 'X') {
        parseInfo.state = PSTATE_NMEA_PAYLOAD;
        parseInfo.msgType = UBX_PUBXMSG;
      } else {
        UBX_LOGF (true, "parse: PSTATE_NMEA_IDB: unexpected character, reset.");
        resetParser ();
      }
      break;
      
    case PSTATE_NMEA_PAYLOAD:
      if (parseInfo.pbCount < __inBufLen) {
        inBuf[parseInfo.pbCount++] = data;
        if (data == 0x0D) {
          parseInfo.state = PSTATE_NMEA_CR;
        }
      } else {
          UBX_LOGF (true, "parse: PSTATE_NMEA_PAYLOAD: msg length exceeds buffer length, reset.");
          resetParser ();
      }
      break;
      
    case PSTATE_NMEA_CR:
      if (parseInfo.pbCount < __inBufLen) {
        inBuf[parseInfo.pbCount++] = data;
        if  (data == 0x0A) {  // NMEA message complete
            // reset parseInfo
            if (parseInfo.msgType == UBX_PUBXMSG) {
                handleNMEA_PUBXMsg();
            } else if (parseInfo.msgType == UBX_GPMSG) {
                handleNMEA_GPMsg();
            }
            
            res = parseInfo.msgType;
            
            // UBX_LOGF (true, "parse: PSTATE_NMEA_CR: msg complete, reset");
            resetParser ();
        }
      } else {
          UBX_LOGF (true, "parse: PSTATE_NMEA_PAYLOAD: msg length exceeds buffer length, reset.");
          resetParser ();
      }
      break;
      
    default:
      UBX_LOGF (true, "parse: unknown parser state, reset.");
      resetParser ();
  }
  
  if (false) {
    UBX_LOGF (false, "   PSTATE: ");
    UBX_LOG (false, lastState);
    UBX_LOGF (false, " --> ");
    UBX_LOG (true, parseInfo.state);
  }
  
  return res;
};
