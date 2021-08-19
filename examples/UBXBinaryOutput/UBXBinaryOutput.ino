#include <Arduino.h>

#include <ArduinoUbxLib.h>

/* ****************************************************************************
  This example shows the usage of the Arduino UBX library. The code first
  configures the message output for NMEA output by sending UBX binaray messages.
  Afterwards, the received NMEA strings are printed via the Serial console.
  ****************************************************************************/

static const uint32_t GPSBaud = 9600;
const unsigned short gpsBufferLen = 200;

class MyGps: public UbxGps {
  private:
      byte buffer[gpsBufferLen] = { 0 };
      
      UbxNavStatusPayload navStatusInfo;
      UbxNavPosLLHPayload navPosLLHInfo;
      UbxNavSolPayload navSolInfo;
      UbxNavTimeUTCPayload navTimeUTCInfo;
      UbxRxmSVSIPayload rxmSVSIInfo;
    
      void sendCfgMsg(unsigned short msgClsID, bool enableMsg) {
        Serial.print (F("sendCfgMsg: 0x")); Serial.println (msgClsID, HEX);
        UbxCfgMsg p;

        if (enableMsg) {
            p.pl.rate = 1;
        }
        
         p.pl.cfgMsgClsID = msgClsID;
         send (&p);
         delay (100);
       };

      void printBuffer (byte* buffer, unsigned short pLen) {
        Serial.print(F("["));
        for (unsigned int i = 0; i < pLen; i++) {
          if (i == 6) {
            Serial.print(F("| "));
          }
          
          if (buffer[i] < 0x10) {
            Serial.print (F("0"));
          }
          Serial.print (buffer[i] , HEX);
          Serial.print (F(" "));
        }
        Serial.println (F("]"));
      };
  
      void send (UbxPacket* p) {  
        unsigned short pLen = p->toArray (buffer, gpsBufferLen);
           
        Serial.println(F("send: "));
        printBuffer(buffer, pLen);
      
        Serial1.write (buffer, pLen);
  
        // clear the buffer
        memset (buffer, 0, gpsBufferLen);
      };

      void setMsgRate () {
        UbxCfgRate cfgRateMsg;
        cfgRateMsg.pl.measRate = 100;
        cfgRateMsg.pl.navRate = 1;
        cfgRateMsg.pl.timeRef = 0;
        
        send (&cfgRateMsg);
      }
       
  public:
    MyGps() {
      logOutput = true;
    }

    void configMsgOutput() {
      Serial.println (F("configuring message output ..."));
      Serial.println (F("  1. disabling all NMEA GPS output ..."));
      sendCfgMsg (UBX_NMEA_GLL, false); 
      sendCfgMsg (UBX_NMEA_GSA, false); 
      sendCfgMsg (UBX_NMEA_GSV, false); 
      sendCfgMsg (UBX_NMEA_RMC, false); 
      sendCfgMsg (UBX_NMEA_VTG, false); 
      sendCfgMsg (UBX_NMEA_GGA, false); 
      
      Serial.println (F("  2. disabling all UBX output ..."));
      sendCfgMsg (UBX_NAV_POSLLH,  false); 
      sendCfgMsg (UBX_NAV_SOL,     false); 
      sendCfgMsg (UBX_NAV_STATUS,  false); 
      sendCfgMsg (UBX_NAV_TIMEUTC, false); 
      sendCfgMsg (UBX_NAV_TIMEGPS, false);

      Serial.println (F("  3. setting measurement rate etc.  ..."));
      // setMsgRate ();
      
      Serial.println (F("  4. Enabling UBX output ..."));
      sendCfgMsg (UBX_NAV_POSLLH,  true);
      Serial.println (F("done sending config messages. "));
    };

    void onReceive (UbxPacket* packet) {
       Serial.print (F("onReceive: UBX msg, packet type:  0x"));
       Serial.print (packet->h.msgClsID, HEX);
       Serial.print (F(", payload length: "));
       Serial.print (packet->h.length);
       Serial.print (F(", valid: "));
       Serial.println (packet->valid);

       switch (packet->h.msgClsID) {
          case UBX_ACK_ACK: {
            UbxAckAck* p = (UbxAckAck*) packet;
            Serial.println (F("[ACK]"));
            Serial.print   (F("   clsID: 0x")); Serial.println (p->pl.pClsID, HEX);
            Serial.print   (F("   msgID: 0x")); Serial.println (p->pl.pMsgID, HEX);
            }
            break;
            
          case UBX_ACK_NAK: {
            UbxAckNak* p = (UbxAckNak*) packet;
            Serial.println (F("[NAK]"));
            Serial.print   (F("   clsID: 0x")); Serial.println (p->pl.pClsID, HEX);
            Serial.print   (F("   msgID: 0x")); Serial.println (p->pl.pMsgID, HEX);
            }
            break;
            
          case UBX_NAV_STATUS: {
            UbxNavStatus* p = (UbxNavStatus*) packet;
            navStatusInfo = p->pl;
            
            Serial.println (F("[NAV_STATUS]"));
            Serial.print   (F("   iTOW:    ")); Serial.println (p->pl.iTOW);
            Serial.print   (F("   gpsfix:  ")); Serial.println (p->pl.gpsFix);
            Serial.print   (F("   flags:   0x")); Serial.println (p->pl.flags, HEX);
            Serial.print   (F("   fixStat: 0x")); Serial.println (p->pl.fixStat, HEX);
            Serial.print   (F("   flags2:  0x")); Serial.println (p->pl.flags2, HEX);
            Serial.print   (F("   ttff:    ")); Serial.println (p->pl.ttff);
            Serial.print   (F("   msss:    ")); Serial.println (p->pl.msss);
            }
            break;
          
          case UBX_NAV_SOL: {
            UbxNavSol* p = (UbxNavSol*) packet;
            navSolInfo = p->pl;
            
            Serial.println (F("[NAV_SOL]"));
            Serial.print   (F("   iTOW:   ")); Serial.println (p->pl.iTOW);
            Serial.print   (F("   fTOW:   ")); Serial.println (p->pl.fTOW);
            Serial.print   (F("   week:   ")); Serial.println (p->pl.week);
            Serial.print   (F("   gpsFix: ")); Serial.println (p->pl.gpsFix);
            Serial.print   (F("   flags:  ")); Serial.println (p->pl.flags);
            Serial.print   (F("   ecefX:  ")); Serial.println (p->pl.ecefX);
            Serial.print   (F("   ecefY:  ")); Serial.println (p->pl.ecefY);
            Serial.print   (F("   ecefZ:  ")); Serial.println (p->pl.ecefZ);
            Serial.print   (F("   pAcc:   ")); Serial.println (p->pl.pAcc);
            Serial.print   (F("   ecefVX: ")); Serial.println (p->pl.ecefVX);
            Serial.print   (F("   ecefVY: ")); Serial.println (p->pl.ecefVY);
            Serial.print   (F("   ecefVZ: ")); Serial.println (p->pl.ecefVZ);
            Serial.print   (F("   sAcc:   ")); Serial.println (p->pl.sAcc);
            Serial.print   (F("   pDOP:   ")); Serial.println (p->pl.pDOP);
            Serial.print   (F("   numSV:  ")); Serial.println (p->pl.numSV);
            }
            break;
          
          case UBX_NAV_POSLLH: {
            UbxNavPosLLH* p = (UbxNavPosLLH*) packet;
            navPosLLHInfo = p->pl;
            
            Serial.println (F("[NAV_POSLLH]"));
            Serial.print   (F("   iTOW:   ")); Serial.println (p->pl.iTOW);
            Serial.print   (F("   lon:    ")); Serial.println (p->pl.lon);
            Serial.print   (F("   lat:    ")); Serial.println (p->pl.lat);
            Serial.print   (F("   height: ")); Serial.println (p->pl.height);
            Serial.print   (F("   hMSL:   ")); Serial.println (p->pl.hMSL);
            Serial.print   (F("   hAcc:   ")); Serial.println (p->pl.hAcc);
            Serial.print   (F("   vAcc:   ")); Serial.println (p->pl.vAcc);
            }
            break;
       }
    };
};

MyGps gps;

// ------------------------------------------------------------------
void communicateWithGps() {
  while (Serial1.available()) {
    gps.parse (Serial1.read());
  }
};

  
  // wait for serial to be opened before starting the program
  while (!Serial) {delay (100);};
  
  Serial.println (F("Starting ..."));
  Serial1.begin (GPSBaud);

  delay (200);
  gps.configMsgOutput();
}

// ------------------------------------------------------------------
void loop() {
  communicateWithGps ();
};
