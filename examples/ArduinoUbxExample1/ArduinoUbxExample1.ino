#include <Arduino.h>

#include <ArduinoUbxLib.h>

/* ****************************************************************************
  This example shows the usage of the Arduino UBX library. The code allows
  control via the serial console:
  - enter '1#' to stop all GPS NMEA output
  - enter '2#' to enable selected GPS NMEA output
  - enter '3#' to stop all UBX binary output
  - enter '4#' to enable selected UBX binary output
  - enter '5#' to '9#' to poll one out of these information:
          5: CFG-NMEA
          6: CFG-RATE
          7: RXM-SVSI
          8: CFG-SBAS
          9: MON-VER
  ****************************************************************************/


static const uint32_t GPSBaud = 9600;
const unsigned short gpsBufferLen = 200;

class MyGps: public UbxGps {
  private:
      unsigned long long lastUbxReceivedTs = 0;
      
      byte buffer[gpsBufferLen] = { 0 };
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
    UbxNavStatusPayload navStatusInfo;
    UbxNavPosLLHPayload navPosLLHInfo;
    UbxNavSolPayload navSolInfo;
    UbxNavTimeUTCPayload navTimeUTCInfo;
    UbxRxmSVSIPayload rxmSVSIInfo;

    MyGps() {
      lastUbxReceivedTs = millis();
    }
    
    void send (UbxPacket* p) {  
      unsigned short pLen = p->toArray (buffer, gpsBufferLen);
         
      Serial.print("send: [");

      for (unsigned int i = 0; i < pLen; i++) {
        if (i == 6) {
          Serial.print("| ");
        }
        
        if (buffer[i] < 0x10) {
          Serial.print ("0");
        }
        
        Serial.print (buffer[i] , HEX);
    
        Serial.print (" ");
      }
    
      Serial.println("]");
    
      Serial1.write (buffer, pLen);

      // clear the buffer
      memset (buffer, 0, gpsBufferLen);
    };

    void startSequence() {
      Serial.println (F("starting GPS sequence ..."));
      cfgCount  = 1;
      canSend = true;
    };

    void checkState() {
      if (canSend) {
        switch (cfgCount) {
          case 1:
            Serial.println (F("Disabling all NMEA GPS output ..."));
            sendCfgMsg (UBX_NMEA_GLL, false); canSend = true;
            sendCfgMsg (UBX_NMEA_GSA, false); canSend = true;
            sendCfgMsg (UBX_NMEA_GSV, false); canSend = true;
            sendCfgMsg (UBX_NMEA_RMC, false); canSend = true;
            sendCfgMsg (UBX_NMEA_VTG, false); canSend = true;
            sendCfgMsg (UBX_NMEA_GGA, false); canSend = false;
            break;
          
          case 2:
            Serial.println (F("Enabling all NMEA GPS output ..."));
            sendCfgMsg (UBX_NMEA_GLL, true); canSend = true;
            sendCfgMsg (UBX_NMEA_GSA, true); canSend = true;
            sendCfgMsg (UBX_NMEA_GSV, true); canSend = true;
            sendCfgMsg (UBX_NMEA_RMC, true); canSend = true;
            sendCfgMsg (UBX_NMEA_VTG, true); canSend = true;
            sendCfgMsg (UBX_NMEA_GGA, true); canSend = false;
            break;

          case 3:
            Serial.println (F("Disabling all UBX output ..."));
            sendCfgMsg (UBX_NAV_POSLLH,  false); canSend = true;
            sendCfgMsg (UBX_NAV_SOL,     false); canSend = true;
            sendCfgMsg (UBX_NAV_STATUS,  false); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEUTC, false); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEGPS, false); canSend = false;
            break;

          case 4:
            Serial.println (F("Enabling all UBX output ..."));
            sendCfgMsg (UBX_NAV_POSLLH,  true); canSend = true;
            sendCfgMsg (UBX_NAV_SOL,     true); canSend = true;
            sendCfgMsg (UBX_NAV_STATUS,  true); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEUTC, true); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEGPS, true); canSend = false;
            break;
          
          case 5:
            Serial.println (F("Polling CFG_NMEA ..."));
            sendPoll (UBX_CFG_NMEA); canSend = false;
            break;
                
          case 6:
            Serial.println (F("Polling CFG_RATE ..."));
            sendPoll (UBX_CFG_RATE); canSend = false;
            break;
                
          case 7:
            Serial.println (F("Polling RXM_SVSI ..."));
            sendPoll (UBX_RXM_SVSI); canSend = false;
            break;
                
          case 8:
            Serial.println (F("Polling CFG_SBAS ..."));
            sendPoll (UBX_CFG_SBAS); canSend = false;
            break;

          case 9:
            Serial.println (F("Polling MON_VER ..."));
            sendPoll (UBX_MON_VER); canSend = false;
            break;
        }
      }

      /*
      if (millis() - lastUbxReceivedTs > 10000) {
        Serial.print ("checkState: resetting messages configuration ...");
        startSequence();
      }
      */
    };

    void onReceive (UbxPacket* packet) {
       Serial.print ("onReceive: UBX msg, packet type:  0x");
       Serial.print (packet->h.msgClsID, HEX);
       Serial.print (", payload length: ");
       Serial.print (packet->h.length);
       Serial.print (", valid: ");
       Serial.println (packet->valid);

       lastUbxReceivedTs = millis();

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
            navStatusInfo = p->pl;
            
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
            navSolInfo = p->pl;
            
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
            navPosLLHInfo = p->pl;
            
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
            navTimeUTCInfo = p->pl;
            
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

          case UBX_NAV_TIMEGPS: {
            UbxNavTimeGPS* p = (UbxNavTimeGPS*) packet;
            
            Serial.println ("[NAV_TIMEGPS]");
            Serial.print   ("   iTOW:  "); Serial.println (p->pl.iTOW);
            Serial.print   ("   fTOW:  "); Serial.println (p->pl.fTOW);
            Serial.print   ("   week:  "); Serial.println (p->pl.week);
            Serial.print   ("   leapS: "); Serial.println (p->pl.leapS);
            Serial.print   ("   valid: "); Serial.println (p->pl.valid);
            Serial.print   ("   tAcc:  "); Serial.println (p->pl.tAcc);       
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

          case UBX_MON_VER: {
            UbxMonVer* p = (UbxMonVer*) packet;
            
            Serial.println ("[MON_VER]");
            Serial.print ("   swVersion:  "); Serial.println ((char*) (p->pl.swVersion));
            Serial.print ("   hwVersion:  "); Serial.println ((char*) (p->pl.hwVersion));
            // Serial.print ("   romVersion: "); Serial.println ((char*) (p->pl.romVersion));
            // canSend = true;
            } 
            break;

          case UBX_RXM_SVSI: {
            UbxRxmSVSI* p = (UbxRxmSVSI*) packet;
            rxmSVSIInfo = p->pl;
            
            Serial.println ("[RXM_SVSI]");
            Serial.print ("   iTOW:   "); Serial.println (p->pl.iTOW);
            Serial.print ("   week:   "); Serial.println (p->pl.week);
            Serial.print ("   numVis: "); Serial.println (p->pl.numVis);
            Serial.print ("   numSV:  "); Serial.println (p->pl.numSV);

            for (unsigned short svc = 0; svc < p->pl.numSV; svc++) {
              Serial.print ("     [#"); Serial.print (svc); Serial.print (" - SVInfo id ");
              Serial.print (p->pl.svInfo[svc].svid);Serial.print (": ");
              Serial.print (" svFlag 0x"); Serial.print (p->pl.svInfo[svc].svFlag, HEX);
              Serial.print (", azim  "); Serial.print (p->pl.svInfo[svc].azim);
              Serial.print (", elev "); Serial.print(p->pl.svInfo[svc].elev);
              Serial.print (", age "); Serial.print (p->pl.svInfo[svc].age);
              Serial.println ("]");
            }
            // canSend = true;
            } 
            break;
       }
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

    void setCC (unsigned short newCfgCt) {
      cfgCount = newCfgCt;
    };

    void setCanSend (bool newCS) {
      canSend = newCS;
    };

    void toggleLogOutput () {
      logOutput = !logOutput;
    };
};

MyGps gps;

// ------------------------------------------------------------------
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
  }

  gps.checkState();
};

// ------------------------------------------------------------------
void handleSerialInEvent () {
 while (Serial.available()) {
    char in = Serial.read();

    switch (in) {
      case '+':
        Serial.print (F("Serial in: increase cfgCount: "));
        gps.setCanSend(false); 
        Serial.println (gps.incCC());
        break;
        
      case '-':
        Serial.print (F("Serial in: decrease cfgCount: "));
        gps.setCanSend(false);
        Serial.println (gps.decCC());
        break;
        
      case '#':
        Serial.println (F("Serial in: canSend = true"));
        gps.setCanSend (true);
        break;
        
      case 'r':
        Serial.println (F("Serial in: reset cfgCount"));
        gps.setCanSend(false);
        gps.resetCC();
        break;
        
      case 'l':
        Serial.println (F("Serial in: toggling logging output"));
        gps.toggleLogOutput();
        break;
        
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        Serial.print (F("Serial in: number entered: ")); Serial.println (in);
        gps.setCanSend(false);
        gps.setCC(atoi(&in));
        break;       
    }
  }
}

// ------------------------------------------------------------------
void setup() {
  Serial.begin (115200);
  
  // wait for serial to be opened before starting the program
  while (!Serial) {delay (100);};
  
  Serial.println (F("Starting ..."));
  Serial1.begin (GPSBaud);

  // gps.startSequence();
}

// ------------------------------------------------------------------
void loop() {
  communicateWithGps ();
  handleSerialInEvent ();
};
