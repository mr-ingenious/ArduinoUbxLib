#include <Arduino.h>

// -- GPS Module ------------------------------------------------
#include <ArduinoUbxLib.h>
static const uint32_t GPSBaud = 9600;
const unsigned short gpsBufferLen = 100;

// -- Battery ---------------------------------------------------
#define VBATPIN A7

// -- OLED Display ----------------------------------------------
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

const int PIN_DISPLAY_CS  = 11;
const int PIN_DISPLAY_DC  = 17;
const int PIN_DISPLAY_RES = 6;
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI display (U8G2_R0, PIN_DISPLAY_CS, PIN_DISPLAY_DC, PIN_DISPLAY_RES);

const unsigned int DEFAULT_PAGE_CYCLE_TIME = 5000; // ms
const unsigned int DEFAULT_PAGE_UPDATE_INTERVAL = 250; // ms
const bool START_PAGE_CYCLING = true;
const unsigned short DEFAULT_DISPLAY_INACTIVITY_TIMEOUT = 60; // seconds


// -- 6 Button Module  -------------------------------------------
#include <SixBtnModuleLib.h>
const int PIN_BUTTONS = 19;
SBMReader inputReader (PIN_BUTTONS, INPUT);

// -- Activity LED -----------------------------------------------
const int PIN_ACT_LED = 5;
int actLedState = LOW;

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

      void alterLed() {
        if (actLedState == LOW) {
        actLedState = HIGH;
        } else {
        actLedState = LOW;
        }
        
        digitalWrite (PIN_ACT_LED, actLedState);
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
      digitalWrite (PIN_ACT_LED, HIGH);
      
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
      digitalWrite (PIN_ACT_LED, LOW);
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
            cfgCount = 3; canSend = true;
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
            sendCfgMsg (UBX_NAV_TIMEUTC, false); canSend = false;
            // sendCfgMsg (UBX_NAV_TIMEGPS, false); canSend = false;
            cfgCount = 4; canSend = true;
            break;

          case 4:
            Serial.println (F("Enabling all UBX output ..."));
            sendCfgMsg (UBX_NAV_POSLLH,  true); canSend = true;
            sendCfgMsg (UBX_NAV_SOL,     true); canSend = true;
            sendCfgMsg (UBX_NAV_STATUS,  true); canSend = true;
            sendCfgMsg (UBX_NAV_TIMEUTC, true); canSend = true;
            // sendCfgMsg (UBX_NAV_TIMEGPS, true);
            canSend = false;
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

      if (millis() - lastUbxReceivedTs > 10000) {
        Serial.print ("checkState: resetting messages configuration ...");
        startSequence();
      }
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
/**
   Controls the display paging, i.e. the sequence which pages are currently displayed.
*/


enum page {PAGE_TITLE, PAGE_DATETIME, PAGE_POSITION, PAGE_GPSINFO, PAGE_BATTERY, PAGE_SETTINGS};

page DEFAULT_FIRST_PAGE = PAGE_TITLE;

class PagingControl {
  private:
    bool pagingActive = START_PAGE_CYCLING;
    bool displayActive = true;
    
    unsigned long lastPageCycleTs = 0;
    unsigned long lastPageUpdateTs = 0;
    unsigned long lastPagingToggleTs = 0;
    unsigned long lastUserActivityTs = 0;
    unsigned long lastBrightnessChange = 0;
    
    unsigned long pageCycleInterval = DEFAULT_PAGE_CYCLE_TIME;
    unsigned long pageUpdateInterval = DEFAULT_PAGE_UPDATE_INTERVAL;
    unsigned long pageToggleIntervalMin = 500;
    unsigned int brightness = 255;
    unsigned short displayInactivityTimeout = DEFAULT_DISPLAY_INACTIVITY_TIMEOUT * 1000;
    
    page currentPage = DEFAULT_FIRST_PAGE;

    bool isPageCycleTimeout() {
      if ((millis() - lastPageCycleTs) > pageCycleInterval)
        return true;
      else
        return false;
    }

    bool isPageUpdateTimeout() {
      if ((millis() - lastPageUpdateTs) > pageUpdateInterval)
        return true;
      else
        return false;
    }

    bool isUserInactiveTimeout() {
      if ((millis() - lastUserActivityTs) > displayInactivityTimeout)
        return true;
      else
        return false;
    }

    void setNextPage(bool forwardPaging) {
      lastPageCycleTs = millis();

      switch  (currentPage) {
        case PAGE_TITLE:
          (forwardPaging) ? currentPage = PAGE_DATETIME : currentPage = PAGE_BATTERY;
          break;
        case PAGE_DATETIME:
          (forwardPaging) ? currentPage = PAGE_POSITION : currentPage = PAGE_BATTERY;
          break;
        case PAGE_POSITION:
          (forwardPaging) ? currentPage = PAGE_GPSINFO : currentPage = PAGE_DATETIME;
          break;
        case PAGE_GPSINFO:
          (forwardPaging) ? currentPage = PAGE_BATTERY : currentPage = PAGE_POSITION;
          break;
        case PAGE_BATTERY:
          (forwardPaging) ? currentPage = PAGE_DATETIME : currentPage = PAGE_GPSINFO;
          break;
      }
    }

    void drawTitleInfo () {
      display.clearBuffer();
      
      display.setFont (u8g2_font_8x13_mf);
      unsigned int strLength = display.getUTF8Width ("Weather Logger");
      display.drawStr ((display.getDisplayWidth() - strLength) / 2, 35, "Weather Logger");
      
      display.sendBuffer();
    }

    void drawPositionInfo() {      
      display.clearBuffer();    
      display.setFont (u8g2_font_6x10_mf);
      display.drawStr (0, 10, "[POSITION]");
      display.drawStr (0, 30, "Lon: ");
      display.setCursor (50, 30);

      double coord = 0.0;

      display.drawStr (0, 30, "Lat: ");
      display.setCursor (50, 30);

      if (gps.navPosLLHInfo.lon == 0 && gps.navPosLLHInfo.lat == 0) {
        display.print ("-");
      } else {
        coord = (double) gps.navPosLLHInfo.lat / 10000000.0;

        display.print (coord, 7);
        if (coord >= 0.0) {
          display.print (" |N");
        } else {
          display.print (" |S");
        }
      }

      display.drawStr (0, 42, "Lon: ");
      display.setCursor (50, 42);
      if (gps.navPosLLHInfo.lon == 0 && gps.navPosLLHInfo.lat == 0) {
        display.print ("-");
      } else {
        coord = (double) gps.navPosLLHInfo.lon / 10000000.0;

        display.print (coord, 7);
        if (coord >= 0.0) {
          display.print (" |E");
        } else {
          display.print (" |W");
        }
      }

      float height = gps.navPosLLHInfo.height / 1000;
      display.drawStr (0, 54, "Height: ");
      display.setCursor (50, 54);
      display.print (height, 2);
      display.print (" m");

      display.sendBuffer();
    }
 
    void drawGPSInfo() {
      display.clearBuffer();

      display.setFont (u8g2_font_6x10_mf);
      display.drawStr (0, 10, "[GPS INFO]");

      display.drawStr (0, 20, "fix: ");
      display.setCursor (50, 20);
      display.print ("0x");
      display.print (gps.navStatusInfo.gpsFix, HEX);
      display.print (" / 0x");
      display.print (gps.navStatusInfo.flags, HEX);

      
      display.drawStr (0, 30, "pAcc: ");
      display.setCursor (50, 30);
      display.print ((float)gps.navSolInfo.pAcc / 100.0, 2);
      display.print (" m");
      
      display.drawStr (0, 40, "pDOP: ");
      display.setCursor (50, 40);
      display.print (gps.navSolInfo.pDOP);

      display.drawStr (0, 50, "numSV: ");
      display.setCursor (50, 50);
      display.print (gps.navSolInfo.numSV);
      
      display.sendBuffer();
    }

    void drawDateTimeInfo() {
      display.clearBuffer();
      
      display.setFont (u8g2_font_6x10_mf);
      display.setCursor (0, 10);
      display.print ("[DATE/TIME] ");
      if (gps.navTimeUTCInfo.valid) {
        display.print ("[OK]");
      } else {
        display.print ("[NOK]");
      }

      display.drawStr (0, 30, "Date: ");
      display.setCursor (40, 30);
      display.print (gps.navTimeUTCInfo.year);
      display.print ("-");
      if (gps.navTimeUTCInfo.month < 10) { display.print ("0"); };
      display.print (gps.navTimeUTCInfo.month);
      display.print ("-");
      if (gps.navTimeUTCInfo.day < 10) { display.print ("0"); };
      display.print (gps.navTimeUTCInfo.day);
      
      display.drawStr (0, 42, "Time: ");
      display.setCursor (40, 42);
      if (gps.navTimeUTCInfo.hour < 10) { display.print ("0"); };
      display.print (gps.navTimeUTCInfo.hour);
      display.print (":");
      if (gps.navTimeUTCInfo.min < 10) { display.print ("0"); };
      display.print (gps.navTimeUTCInfo.min);
      display.print (":");
      if (gps.navTimeUTCInfo.sec < 10) { display.print ("0"); };
      display.print (gps.navTimeUTCInfo.sec);

      display.drawStr (0, 54, "tAcc: ");
      display.setCursor (40, 54);
      display.print (gps.navTimeUTCInfo.tAcc);
      display.print (" ns");
      
      display.sendBuffer();
    }

    void drawBatteryInfo() {
      display.clearBuffer();

      // Code from https://learn.adafruit.com/adafruit-feather-m0-adalogger/power-management
      
      float batteryVoltage = analogRead(VBATPIN);
      batteryVoltage *= 2;    // we divided by 2, so multiply back
      batteryVoltage *= 3.3;  // Multiply by 3.3V, our reference voltage
      batteryVoltage /= 1024; // convert to voltage
      
      display.setFont (u8g2_font_6x10_mf);
      display.setCursor (0, 10);
      display.print ("[BATTERY]");
      display.drawStr (0, 30, "Voltage: ");
      display.setCursor (60, 30);
      display.print (batteryVoltage, 2);
      display.print (" V");
      
      display.sendBuffer();
    }
    
    void drawSettings() {
      display.clearBuffer();
      
      display.setFont (u8g2_font_6x10_mf);
      display.setCursor (0, 10);
      display.print ("[SETTINGS]");
      
      display.sendBuffer();
    }
    
    void drawPagingStoppedIndication() {
      // display.drawFrame(0, 0, 128, 64);
      display.setFont(u8g2_font_unifont_t_symbols);
      display.drawGlyph(100, 20, 9208);
      display.sendBuffer();
    }

  public:
    PagingControl() {
      lastPageCycleTs = millis() + pageCycleInterval;
      lastPageUpdateTs = millis() + pageUpdateInterval;
      lastPagingToggleTs = millis() + pageToggleIntervalMin;
      lastUserActivityTs = millis();
    }

    void updateDisplay() {
      if (isUserInactiveTimeout()) {
        if (displayActive) {
          Serial.println (F("updateDisplay: user inactive, switching off display ..."));
          display.setPowerSave (1);
          displayActive = false;
        }
      } else {
        if (!displayActive) {
          Serial.println (F("updateDisplay: user inactive, switching on display ..."));
          display.setPowerSave (0);
        }
        
        displayActive = true;
      }

      if (displayActive) {
        if (isPageCycleTimeout() && pagingActive) {
          setNextPage(true);
        }
  
        if (isPageUpdateTimeout()) {
          lastPageUpdateTs = millis();
          switch (currentPage) {
            case PAGE_TITLE:
              drawTitleInfo();
              break;
              
            case PAGE_DATETIME:
              drawDateTimeInfo();
              break;
              
            case PAGE_POSITION:
              drawPositionInfo();
              break;
              
            case PAGE_GPSINFO:
              drawGPSInfo();
              break;
  
            case PAGE_BATTERY:
              drawBatteryInfo();
              break;
          }
  
          if (!pagingActive) {
            // drawPagingStoppedIndication();
          }
  
          unsigned int brightness_new  = 255;
          if ((millis() - lastBrightnessChange > 5000) &&
              (brightness_new >= 0) && (brightness_new <= 255) &&
              ((brightness_new >= brightness + 25) || (brightness_new <= brightness - 25))) {
            brightness = brightness_new;
            lastBrightnessChange = millis();
            display.setContrast(brightness);
            Serial.print (F("Setting brightness to: "));
            Serial.println (brightness);
          }
        }
      }
    }

    void btnCenter() {
      if (displayActive && (millis() - lastPagingToggleTs) > pageToggleIntervalMin) {
        lastPagingToggleTs = millis();
        pagingActive = !pagingActive;
        Serial.print (F("Set paging to "));
        Serial.println (pagingActive);
      }
      lastUserActivityTs = millis();
    }

    void btnBack() {
      lastUserActivityTs = millis();
    }

    void btnUp() {
      lastUserActivityTs = millis();
    }

    void btnDown() {
      lastUserActivityTs = millis();
    }

    void btnRight() {
      if (displayActive) {
        setNextPage (true);
      }
      lastUserActivityTs = millis();
    }

    void btnLeft() {
      if (displayActive) {
        setNextPage (false);
      }
      lastUserActivityTs = millis();
    }
};

PagingControl pageControl;

// --------------------------------------------------------------------------------
void setupDisplay() {
  display.begin();
  display.enableUTF8Print();
  unsigned int displayWidth = display.getDisplayWidth();
  unsigned int displayHeight = display.getDisplayHeight();
  Serial.print (F("Display dimensions: "));
  Serial.print (displayWidth);
  Serial.print ("x" );
  Serial.println (displayHeight);
}

void setup() {
  Serial.begin (115200);
  
  // wait for serial to be opened before starting the program
  // while (!Serial) {delay (100);};
  
  Serial.println (F("Starting ..."));
  Serial1.begin (GPSBaud);
  gps.startSequence();

  setupDisplay ();
  
  pinMode (PIN_ACT_LED, OUTPUT);
  digitalWrite (PIN_ACT_LED, actLedState);
}

unsigned long lastReceivedTs = 0;
bool sepWritten = true;

void communicateWithGps() {
  while (Serial1.available()) {
    digitalWrite (PIN_ACT_LED, HIGH);
    
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
  digitalWrite (PIN_ACT_LED, LOW);

  gps.checkState();
};

// --------------------------------------------------------------------------------
void handleButtonEvent2 (byte btn) {
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

// --------------------------------------------------------------------------------
void handleButtonEvent (byte btn) {
  switch (btn) {
    case BUTTON_LEFT:
      Serial.println (F("Buttons: [LEFT] triggered --> paging left"));
      pageControl.btnLeft();
      break;
    case BUTTON_RIGHT:
      Serial.println (F("Buttons: [RIGHT] triggered --> paging right"));
      pageControl.btnRight();
      break;
    case BUTTON_UP:
      Serial.println (F("Buttons: [UP] triggered --> paging up"));
      pageControl.btnUp();
      break;
    case BUTTON_DOWN:
      Serial.println(F("Buttons: [DOWN] triggered --> paging down"));
      pageControl.btnDown();
      break;
    case BUTTON_CENTER:
      Serial.println (F("Buttons: [CENTER] triggered --> option OK"));
      pageControl.btnCenter();
      break;
    case BUTTON_BACK:
      Serial.println (F("Buttons: [BACK] triggered --> option EXIT"));
      pageControl.btnBack();
      break;
  }
}


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

void loop() {
  communicateWithGps ();
  handleButtonEvent (inputReader.readButtons());
  handleSerialInEvent ();
  pageControl.updateDisplay();
};
