#include "ArduinoUbxLib.h"

#include "Arduino.h"

/* ****************************************************************************
  Constructor:
  ****************************************************************************/
UbxGps::UbxGps () {
  
};

/* ****************************************************************************
  checkChecksum: calculates the checksum according to 8-Bit Fletcher Algorithm.
  ****************************************************************************/
// unsigned short UbxGps::calcChecksum (UbxPacket* packet) {
//   byte CK_A = 0, CK_B = 0;
//   
//   pInfo pi = packet->toArray ();
//   
//   for (unsigned int i = 2; i < pi.len; i++) {
//      CK_A = CK_A + pi.ptr[i];
//      CK_B = CK_B + CK_A;
//   }
//  
//   unsigned short checksum = CK_A;
//   checksum = checksum << 8;
//   checksum |= CK_B;
//   
//   return checksum;
// };

/* ****************************************************************************
  checkChecksum:
  ****************************************************************************/
// bool UbxGps::checkChecksum (UbxPacket* packet) {
//   return (calcChecksum (packet) == packet->checksum);
// };

/* ****************************************************************************
  handleUbxPacket:
  ****************************************************************************/
void UbxGps::handleUbxPacket () {
    if (false) {
      Serial.print ("handleUbxPacket: UBX binary [");
      for (unsigned short i = 0; i < parseInfo.payloadLen + 8; i++) {
        if (inBuf [i] < 0x10) {
          Serial.print ("0");
        }
      
        Serial.print (inBuf [i], HEX);
        Serial.print (" ");

        if (i == 5) {
          Serial.print (" | ");
        }
      }
      
      Serial.println ("]");
    }
    
    switch (parseInfo.msgClsID) {
        case UBX_ACK_ACK: {
          // Serial.println ("handleUbxPacket: UBX_ACK_ACK found.");        
          UbxAckAck p = UbxAckAck (inBuf, parseInfo.payloadLen + 8);
          
          if (p.valid) {
            onReceive (&p);
          }}
          break;
          
        case UBX_ACK_NAK: {
          // Serial.println ("handleUbxPacket: UBX_ACK_NAK found.");
          UbxAckNak p = UbxAckNak (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
          
        case UBX_NAV_STATUS: {
          // Serial.println ("handleUbxPacket: UBX_NAV_STATUS found.");
          UbxNavStatus p = UbxNavStatus (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
        
        case UBX_NAV_SOL: {
          // Serial.println ("handleUbxPacket: UBX_NAV_SOL found.");
          UbxNavSol p = UbxNavSol (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
        
        case UBX_NAV_POSLLH: {
          // Serial.println ("handleUbxPacket: UBX_NAV_POSLLH found.");
          UbxNavPosLLH p = UbxNavPosLLH (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;

        case UBX_NAV_TIMEUTC: {
          // Serial.println ("handleUbxPacket: UBX_NAV_TIMEUTC found.");
          UbxNavTimeUTC p = UbxNavTimeUTC (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
          
        case UBX_CFG_NMEA: {
          // Serial.println ("handleUbxPacket: UBX_CFG_NMEA found.");
          UbxCfgNMEA p = UbxCfgNMEA (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
          
        case UBX_CFG_RATE: {
          // Serial.println ("handleUbxPacket: UBX_CFG_RATE found.");
          UbxCfgRate p = UbxCfgRate (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
          
        case UBX_CFG_RXM: {
          // Serial.println ("handleUbxPacket: UBX_CFG_RXM found.");
          UbxCfgRXM p = UbxCfgRXM (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
          
        case UBX_CFG_SBAS: {
          // Serial.println ("handleUbxPacket: UBX_CFG_SBAS found.");
          UbxCfgSBAS p = UbxCfgSBAS (inBuf, parseInfo.payloadLen + 8);
          if (p.valid) {
            onReceive (&p);
          }}
          break;
    }
};

/* ****************************************************************************
  handleNMEA_GPMsg:
  ****************************************************************************/
void UbxGps::handleNMEA_GPMsg () {
  onReceive ((char*) inBuf, parseInfo.pbCount);
  
  if (false) {
    Serial.print ("handleNMEA_GPMsg: NMEA GP [");
    for (unsigned short i = 0; i < parseInfo.pbCount; i++) {
      if (inBuf [i] == 0x0D) {
        Serial.print ("<CR>");
      } else if (inBuf [i] == 0x0D) {
        Serial.println ("<LF>]");
      }else {
        Serial.print ((char) inBuf [i]);
      }
    }
    Serial.println();
  }
};

/* ****************************************************************************
  handleNMEA_PUBXMsg:
  ****************************************************************************/
void UbxGps::handleNMEA_PUBXMsg () {
  onReceive ((char*) inBuf, parseInfo.pbCount);
  
  if (false) {
    Serial.print ("handleNMEA_PUBXMsg: NMEA PUBX [");
    for (unsigned short i = 0; i < parseInfo.pbCount; i++) {
      if (inBuf [i] == 0x0D) {
        Serial.print ("<CR>");
      } else if (inBuf [i] == 0x0D) {
        Serial.println ("<LF>]");
      } else {
        Serial.print ((char) inBuf [i]);
      }
    }
    Serial.println();
  }
};

/* ****************************************************************************
  resetParser:
  ****************************************************************************/
void UbxGps::resetParser () {
  parseInfo.state = PSTATE_INIT;
  parseInfo.msgClsID = UBX_NONE;
  parseInfo.pbCount = 0;
  parseInfo.payloadLen = 0;
  parseInfo.isNMEA_PUBX = false;
  parseInfo.isNMEA_PUBX = false;
  
  memset (inBuf, 0, __inBufLen);
}

/* ****************************************************************************
  parse:
  ****************************************************************************/
void UbxGps::parse (byte data) {
    const UbxParserStates lastState = parseInfo.state;
    
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
            Serial.println ("parse: PSTATE_UBX_SYNC1: unexpected character, reset.");
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
            Serial.println ("parse: PSTATE_UBX_SYNC2: unexpected character, reset.");
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
          // Serial.print ("PSTATE_LEN2: payloadLen: ");
          // Serial.println (parseInfo.payloadLen);
          parseInfo.state = PSTATE_UBX_LEN2;
          break;
          
        case PSTATE_UBX_LEN2: // payload ...
          // Serial.print ("PSTATE_LEN2: payload, pbCount: ");
          // Serial.println (parseInfo.pbCount);
          
          if (6 + parseInfo.pbCount < __inBufLen) {
            inBuf [6 + parseInfo.pbCount] = data;
          
            if (parseInfo.pbCount == parseInfo.payloadLen - 1) {
                parseInfo.state = PSTATE_UBX_PAYLOAD;
                // Serial.println ("PSTATE_LEN2: payload done.");
            }
            parseInfo.pbCount++;
          } else {
              Serial.println ("parse: PSTATE_UBX_LEN2: msg length exceeds buffer length, reset.");
              resetParser ();
          }
          break;
          
        case PSTATE_UBX_PAYLOAD:
          // Serial.println ("PSTATE_PAYLOAD: CK_A found.");
          inBuf [6 + parseInfo.payloadLen] = data;
          parseInfo.state = PSTATE_UBX_CHK_A;
          break;
          
        case PSTATE_UBX_CHK_A:
          // Serial.println ("PSTATE_PAYLOAD: CK_B found.");
          inBuf [7 + parseInfo.payloadLen] = data;        
          handleUbxPacket ();
          // Serial.println ("parse: PSTATE_UBX_CHK_A: msg complete, reset");
          resetParser ();
          break;
          
        case PSTATE_NMEA_SYNC:
          inBuf[parseInfo.pbCount++] = data;
          if  (data == 'G') {
            parseInfo.state = PSTATE_NMEA_IDGP1;
          } else if (data == 'P') {
             parseInfo.state = PSTATE_NMEA_IDP;
          } else {
              Serial.println ("parse: PSTATE_NMEA_SYNC: unexpected character, reset.");
              resetParser ();
          }
          break;
          
        case PSTATE_NMEA_IDGP1:
          inBuf[parseInfo.pbCount++] = data;
          if  (data == 'P') {
            parseInfo.state = PSTATE_NMEA_PAYLOAD;
            parseInfo.isNMEA_GP = true;
          } else {
            Serial.println ("parse: PSTATE_NMEA_IDGP1: unexpected character, reset.");
            resetParser ();
          }
          break;
          
        case PSTATE_NMEA_IDP:
          inBuf[parseInfo.pbCount++] = data;
          if  (data == 'U') {
            parseInfo.state = PSTATE_NMEA_IDU;
          } else {
            Serial.println ("parse: PSTATE_NMEA_IDP: unexpected character, reset.");
            resetParser ();
          }
          break;
          
        case PSTATE_NMEA_IDU:
          inBuf[parseInfo.pbCount++] = data;
          if  (data == 'B') {
            parseInfo.state = PSTATE_NMEA_IDB;
          } else {
            Serial.println ("parse: PSTATE_NMEA_IDU: unexpected character, reset.");
            resetParser ();
          }
          break;
          
        case PSTATE_NMEA_IDB:
          inBuf[parseInfo.pbCount++] = data;
          if  (data == 'X') {
            parseInfo.state = PSTATE_NMEA_PAYLOAD;
            parseInfo.isNMEA_PUBX = true;
          } else {
            Serial.println ("parse: PSTATE_NMEA_IDB: unexpected character, reset.");
            resetParser ();
          }
          break;
          
        case PSTATE_NMEA_PAYLOAD:
          if (parseInfo.pbCount == __inBufLen - 1) {
              Serial.println ("parse: PSTATE_NMEA_PAYLOAD: msg length exceeds buffer length, reset.");
              resetParser ();
          } else {
            inBuf[parseInfo.pbCount++] = data;
            if (data == 0x0D) {
              parseInfo.state = PSTATE_NMEA_CR;
            }
          }
          break;
          
        case PSTATE_NMEA_CR:
          inBuf[parseInfo.pbCount++] = data;
          if  (data == 0x0A) {  // NMEA message complete
              // reset parseInfo
                          
              if (parseInfo.isNMEA_PUBX) {
                  handleNMEA_PUBXMsg();
              } else if (parseInfo.isNMEA_GP) {
                  handleNMEA_GPMsg();
              }
              
              // Serial.println ("parse: PSTATE_NMEA_CR: msg complete, reset");
              resetParser ();
          }
          break;
    }
    
    if (false) {
      Serial.print ("   PSTATE: ");
      Serial.print (lastState);
      Serial.print (" --> ");
      Serial.println (parseInfo.state);
    }
};
