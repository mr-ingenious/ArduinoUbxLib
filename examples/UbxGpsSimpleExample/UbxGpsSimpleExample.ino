#include <ArduinoUbxLib.h>
#include <SixBtnModuleLib.h>

const int PIN_BUTTONS = 9;
SBMReader inputReader (PIN_BUTTONS, INPUT);

const unsigned short bufferLen = 100;

class MyGps: public UbxGps {
  private:
      byte buffer[bufferLen] = { 0 };
      bool canSend = true;
      unsigned short cfgCount = 0;

      void sendCfgMsgMT(unsigned short msgClsID, bool enableMsg) {
        Serial.print ("sendCfgMsg: "); Serial.println (msgClsID);
        UbxCfgMsgMT p;

        if (enableMsg) {
            p.pl.rate[0] = 1;
            p.pl.rate[1] = 1;
            p.pl.rate[2] = 1;
            p.pl.rate[3] = 1;
            p.pl.rate[4] = 1;
            p.pl.rate[5] = 1;
        }
        
         p.pl.cfgMsgClsID = msgClsID;
         send (&p);
         // canSend = false;
       };

       
      void sendCfgMsg(unsigned short msgClsID, bool enableMsg) {
        Serial.print ("sendCfgMsg: "); Serial.println (msgClsID);
        UbxCfgMsg p;

        if (enableMsg) {
            p.pl.rate = 1;
        }
        
         p.pl.cfgMsgClsID = msgClsID;
         send (&p);
         // canSend = false;
       };

       void sendPoll (unsigned short msgClsID) {
          Serial.print ("sendPoll: "); Serial.println (msgClsID);
          UbxPoll p;
   
          p.h.msgClsID = msgClsID;
          send (&p);
          // canSend = false;
        };
       
  public:
    void send (UbxPacket* p) {
       unsigned short pLen = p->toArray (buffer, bufferLen);
         
      Serial.print("send: [");

      for (unsigned int i = 0; i < pLen; i++) {
        if (i == 6) {
          Serial.print(" | ");
        }
        
        if (buffer[i] < 0x10) {
          Serial.print ("0");
        }
        
        Serial.print (buffer[i] , HEX);
    
        Serial.print (" ");
        if (buffer[i]  == 0x0A) {
          Serial.println("|");
        }
      }
    
      Serial.println("]");
    
      Serial1.write (buffer, pLen);

      // clear the buffer
      memset (buffer, 0, bufferLen);
    };

    void startSequence() {
      // sendCfgMsg (UBX_NMEA_GGA, false);
    };

    void checkState() {
      if (canSend) {
        switch (cfgCount) {
          case 1:
            sendCfgMsg (UBX_NMEA_GLL, false); canSend = true;
            sendCfgMsg (UBX_NMEA_GSA, false); canSend = true;
            sendCfgMsg (UBX_NMEA_GSV, false); canSend = true;
            sendCfgMsg (UBX_NMEA_RMC, false); canSend = true;
            sendCfgMsg (UBX_NMEA_VTG, false); canSend = true;
            sendCfgMsg (UBX_NMEA_GGA, false); canSend = false;
            break;
          
          case 2:
            sendCfgMsg (UBX_NMEA_GLL, true); canSend = true;
            sendCfgMsg (UBX_NMEA_GSA, true); canSend = true;
            sendCfgMsg (UBX_NMEA_GSV, true); canSend = true;
            sendCfgMsg (UBX_NMEA_RMC, true); canSend = true;
            sendCfgMsg (UBX_NMEA_VTG, true); canSend = true;
            sendCfgMsg (UBX_NMEA_GGA, true); canSend = false;
            break;

          case 3:
            sendCfgMsg (UBX_NAV_POSLLH,  false); canSend = true;
            sendCfgMsg (UBX_NAV_SOL,     false); canSend = true;
            sendCfgMsg (UBX_NAV_STATUS,  false); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEUTC, false); canSend = false;
            break;

          case 4:
            sendCfgMsg (UBX_NAV_POSLLH,  true); canSend = true;
            sendCfgMsg (UBX_NAV_SOL,     true); canSend = true;
            sendCfgMsg (UBX_NAV_STATUS,  true); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEUTC, true); canSend = false;
            break;
          
          case 5:
            sendPoll (UBX_CFG_NMEA); canSend = false;
            break;
                
          case 6:
            sendPoll (UBX_CFG_RATE); canSend = false;
            break;
                
          case 7:
            sendPoll (UBX_CFG_RXM); canSend = false;
            break;
                
          case 8:
            sendPoll (UBX_CFG_SBAS); canSend = false;
            break;

          // funktioniert nicht:
          // case 10: sendCfgMsg (UBX_CFG_NMEA, false); canSend = false; break;
          // case 11: sendCfgMsg (UBX_CFG_RATE, false); canSend = false; break;
          // case 12: sendCfgMsg (UBX_CFG_RXM, false); canSend = false; break;
          // case 13: sendCfgMsg (UBX_CFG_SBAS, false); canSend = false; break;
          // case 10: sendCfgMsg (UBX_CFG_NMEA, true); canSend = false; break;
          // case 11: sendCfgMsg (UBX_CFG_RATE, true); canSend = false; break;
          // case 12: sendCfgMsg (UBX_CFG_RXM, true); canSend = false; break;
          // case 13: sendCfgMsg (UBX_CFG_SBAS, true); canSend = false; break;
        }
      }
    };

    void onReceive (UbxPacket* packet) {
       Serial.print ("onReceive: UBX msg, packet type:  ");
       Serial.print (packet->h.msgClsID, HEX);
       Serial.print (", payload length: ");
       Serial.println (packet->h.length);

       switch (packet->h.msgClsID) {
          case UBX_ACK_ACK: {
            UbxAckAck* p = (UbxAckAck*) packet;
            Serial.println ("[ACK]");
            Serial.print   ("   clsID: 0x"); Serial.println (p->pl.pClsID, HEX);
            Serial.print   ("   msgID: 0x"); Serial.println (p->pl.pMsgID, HEX);
            // cfgCount++;
            // canSend = true;
            }
            break;
            
          case UBX_ACK_NAK: {
            UbxAckNak* p = (UbxAckNak*) packet;
            Serial.println ("[NAK]");
            Serial.print   ("   clsID: 0x"); Serial.println (p->pl.pClsID, HEX);
            Serial.print   ("   msgID: 0x"); Serial.println (p->pl.pMsgID, HEX);
            // cfgCount++;
            // canSend = true;
            }
            break;
            
          case UBX_NAV_STATUS: {
            UbxNavStatus* p = (UbxNavStatus*) packet;
            Serial.println ("[NAV_STATUS]");
            Serial.print   ("   iTOW:    "); Serial.println (p->pl.iTOW);
            Serial.print   ("   gpsfix:  "); Serial.println (p->pl.gpsFix);
            Serial.print   ("   flags:   0x"); Serial.println (p->pl.flags, HEX);
            Serial.print   ("   fixStat: 0x"); Serial.println (p->pl.fixStat, HEX);
            Serial.print   ("   flags2:  0x"); Serial.println (p->pl.flags2, HEX);
            Serial.print   ("   ttff:    "); Serial.println (p->pl.ttff);
            Serial.print   ("   msss:    "); Serial.println (p->pl.msss);
            // canSend = true;
            }
            break;
          
          case UBX_NAV_SOL: {
            UbxNavSol* p = (UbxNavSol*) packet;
            Serial.println ("[NAV_SOL]");
            Serial.print   ("   iTOW:   "); Serial.println (p->pl.iTOW);
            Serial.print   ("   fTOW:   "); Serial.println (p->pl.fTOW);
            Serial.print   ("   week:   "); Serial.println (p->pl.week);
            Serial.print   ("   gpsFix: "); Serial.println (p->pl.gpsFix);
            Serial.print   ("   flags:  "); Serial.println (p->pl.flags);
            Serial.print   ("   ecefX:  "); Serial.println (p->pl.ecefX);
            Serial.print   ("   ecefY:  "); Serial.println (p->pl.ecefY);
            Serial.print   ("   ecefZ:  "); Serial.println (p->pl.ecefZ);
            Serial.print   ("   pAcc:   "); Serial.println (p->pl.pAcc);
            Serial.print   ("   ecefVX: "); Serial.println (p->pl.ecefVX);
            Serial.print   ("   ecefVY: "); Serial.println (p->pl.ecefVY);
            Serial.print   ("   ecefVZ: "); Serial.println (p->pl.ecefVZ);
            Serial.print   ("   sAcc:   "); Serial.println (p->pl.sAcc);
            Serial.print   ("   pDOP:   "); Serial.println (p->pl.pDOP);
            Serial.print   ("   numSV:  "); Serial.println (p->pl.numSV);
            // canSend = true;
            }
            break;
          
          case UBX_NAV_POSLLH: {
            UbxNavPosLLH* p = (UbxNavPosLLH*) packet;
            Serial.println ("[NAV_POSLLH]");
            Serial.print   ("   iTOW:   "); Serial.println (p->pl.iTOW);
            Serial.print   ("   lon:    "); Serial.println (p->pl.lon);
            Serial.print   ("   lat:    "); Serial.println (p->pl.lat);
            Serial.print   ("   height: "); Serial.println (p->pl.height);
            Serial.print   ("   hMSL:   "); Serial.println (p->pl.hMSL);
            Serial.print   ("   hAcc:   "); Serial.println (p->pl.hAcc);
            Serial.print   ("   vAcc:   "); Serial.println (p->pl.vAcc);
            // canSend = true;
            }
            break;
          
          case UBX_NAV_TIMEUTC: {
            UbxNavTimeUTC* p = (UbxNavTimeUTC*) packet;
            Serial.println ("[NAV_TIMEUTC]");
            Serial.print   ("   iTOW:  "); Serial.println (p->pl.iTOW);
            Serial.print   ("   tAcc:  "); Serial.println (p->pl.tAcc);
            Serial.print   ("   nano:  "); Serial.println (p->pl.nano);
            Serial.print   ("   year:  "); Serial.println (p->pl.year);
            Serial.print   ("   month: "); Serial.println (p->pl.month);
            Serial.print   ("   day:   "); Serial.println (p->pl.day);
            Serial.print   ("   hour:  "); Serial.println (p->pl.hour);
            Serial.print   ("   min:   "); Serial.println (p->pl.min);
            Serial.print   ("   sec:   "); Serial.println (p->pl.sec);
            Serial.print   ("   valid: "); Serial.println (p->pl.valid);       
            //canSend = true;
            }
            break;
            
          case UBX_CFG_NMEA: {
            UbxCfgNMEA* p = (UbxCfgNMEA*) packet;
            Serial.println ("[CFG_NMEA]");
            Serial.print   ("   filter:  0x"); Serial.println (p->pl.filter, HEX);
            Serial.print   ("   version: "); Serial.println (p->pl.version);
            Serial.print   ("   numSV:   "); Serial.println (p->pl.numSV);
            Serial.print   ("   flags:   0x"); Serial.println (p->pl.flags, HEX);
            //canSend = true;
            }
            break;
            
          case UBX_CFG_RATE: {
            UbxCfgRate* p = (UbxCfgRate*) packet;
            Serial.println ("[CFG_RATE]");
            Serial.print   ("   measRate: "); Serial.println (p->pl.measRate);
            Serial.print   ("   navRate:  "); Serial.println (p->pl.navRate);
            Serial.print   ("   timeRef:  "); Serial.println (p->pl.timeRef);
            // canSend = true;
            }
            break;
            
          case UBX_CFG_RXM: {
            UbxCfgRXM* p = (UbxCfgRXM*) packet;
            Serial.println ("[CFG_RXM]");
            Serial.print   ("   reserved1: "); Serial.println (p->pl.reserved1);
            Serial.print   ("   lpMode:    0x"); Serial.println (p->pl.lpMode, HEX);
            // canSend = true;
            }
            break;
            
          case UBX_CFG_SBAS: {
            UbxCfgSBAS* p = (UbxCfgSBAS*) packet;
            Serial.println ("[CFG_SBAS]");
            Serial.print ("   mode:      0x"); Serial.println (p->pl.mode, HEX);
            Serial.print ("   usage:     0x"); Serial.println (p->pl.usage, HEX);
            Serial.print ("   maxSBAS:   "); Serial.println (p->pl.maxSBAS);
            Serial.print ("   scanmode2: 0x"); Serial.println (p->pl.scanmode2, HEX);
            Serial.print ("   scanmode1: 0x"); Serial.print (p->pl.scanmode1[0], HEX);
            Serial.print (p->pl.scanmode1[1], HEX); Serial.print (p->pl.scanmode1[2], HEX);
            Serial.println (p->pl.scanmode1[3], HEX);
            // canSend = true;
            } 
            break;
       }
    };

    unsigned short incCC () {
      if(cfgCount < 30) {
        cfgCount++;
      }
      return cfgCount;
    };

    unsigned short decCC () {
      if (cfgCount >0) {
        cfgCount--;
      }
      return cfgCount;
    };

    void resetCC () {
      cfgCount = 0;
    };

    void setCanSend (bool newCS) {
      canSend = newCS;
    };

    void onReceive (char* nmeaMsg, unsigned short mLen) {
      Serial.print ("onReceive: NMEA [");
      for (unsigned short i = 0; i < mLen; i++) {
        if (nmeaMsg [i] == 0x0D) {
          Serial.print ("<CR>");
        } else if (nmeaMsg [i] == 0x0A) {
          Serial.println ("<LF>]");
        } else {
          Serial.print ((char) nmeaMsg [i]);
        }
      }
    };
};

MyGps gps;

static const uint32_t GPSBaud = 9600;

void setup() {
  Serial.begin (115200);
  
  // wait for serial to be opened before starting the program
  while (!Serial) {delay (100);};
  
  Serial.println (F("Starting ..."));
  Serial1.begin (GPSBaud);
  gps.startSequence();
}

unsigned long lastReceivedTs = 0;
bool sepWritten = true;

void communicateWithGps() {
  while (Serial1.available()) {
    char in = Serial1.read();
    if (false) {
      if (in == 0xB5) {
        Serial.print("\nreceived (UBX): ");
      }
      
      if (in < 0x10) {
        Serial.print ("0");
      }

      if ((in >= 0x20) && (in <= 0x7E)) {
        Serial.print(in);
      } else {
        Serial.print (in, HEX);
        Serial.print (" ");
      }
      
      if (in == 0x0A) {
        Serial.print("\nreceived (NMEA): ");
      }
    }

    gps.parse (in);
    
    lastReceivedTs = millis();
  }

  gps.checkState();
};

// --------------------------------------------------------------------------------
void handleButtonEvent (byte btn) {
  switch (btn) {
    case BUTTON_LEFT:
      Serial.println (F("Buttons: [LEFT] triggered"));
      break;
    case BUTTON_RIGHT:
      Serial.println (F("Buttons: [RIGHT] triggered"));
      break;
    case BUTTON_UP:
      Serial.print (F("Buttons: [UP] triggered --> increase cfgCount: "));
      gps.setCanSend(false); 
      Serial.println (gps.incCC());
      break;
    case BUTTON_DOWN:
      Serial.print (F("Buttons: [DOWN] triggered --> decrease cfgCount: "));
      gps.setCanSend(false);
      Serial.println (gps.decCC());
      break;
    case BUTTON_CENTER:
      Serial.println (F("Buttons: [CENTER] triggered --> canSend = true"));
      gps.setCanSend (true);
      break;
    case BUTTON_BACK:
      Serial.println (F("Buttons: [BACK] triggered --> reset cfgCount"));
      gps.setCanSend(false);
      gps.resetCC();
      break;
    case BUTTON_NONE:
      // Serial.println (F("Buttons: nothing ..."));
      break;
  }
}

void loop() {
  communicateWithGps ();
  handleButtonEvent (inputReader.readButtons());
};
