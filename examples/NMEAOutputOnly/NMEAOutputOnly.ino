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
       
      void sendCfgMsg(unsigned short msgClsID, bool enableMsg) {
        Serial.print ("sendCfgMsg: 0x"); Serial.println (msgClsID, HEX);
        UbxCfgMsg p;

        if (enableMsg) {
            p.pl.rate = 1;
        }
        
         p.pl.cfgMsgClsID = msgClsID;
         send (&p);
         delay (100);
       };

       void printBuffer (byte* buffer, unsigned short pLen) {
      Serial.print("[");
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
      Serial.println ("]");
    };

    void send (UbxPacket* p) {  
      unsigned short pLen = p->toArray (buffer, gpsBufferLen);
         
      Serial.println("send: ");
      printBuffer(buffer, pLen);
    
      Serial1.write (buffer, pLen);

      // clear the buffer
      memset (buffer, 0, gpsBufferLen);
    };
       
  public:
    MyGps() {
    }

    void configMsgOutput() {
      Serial.println (F("configuring message output ..."));
      Serial.println (F("  1. disabling all UBX output ..."));
      sendCfgMsg (UBX_NAV_POSLLH,  false); 
      sendCfgMsg (UBX_NAV_SOL,     false); 
      sendCfgMsg (UBX_NAV_STATUS,  false); 
      sendCfgMsg (UBX_NAV_TIMEUTC, false); 
      sendCfgMsg (UBX_NAV_TIMEGPS, false); 
      
      Serial.println (F("  2. disabling all NMEA GPS output ..."));
      sendCfgMsg (UBX_NMEA_GLL, false); 
      sendCfgMsg (UBX_NMEA_GSA, false); 
      sendCfgMsg (UBX_NMEA_GSV, false); 
      sendCfgMsg (UBX_NMEA_RMC, false); 
      sendCfgMsg (UBX_NMEA_VTG, false); 
      sendCfgMsg (UBX_NMEA_GGA, false); 
      
      Serial.println (F("  3. enabling all NMEA GPS output ..."));
      sendCfgMsg (UBX_NMEA_GLL, true); 
      sendCfgMsg (UBX_NMEA_GSA, true); 
      sendCfgMsg (UBX_NMEA_GSV, true); 
      sendCfgMsg (UBX_NMEA_RMC, true); 
      sendCfgMsg (UBX_NMEA_VTG, true); 
      sendCfgMsg (UBX_NMEA_GGA, true);
      Serial.println (F("done sending config messages. "));
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

// ------------------------------------------------------------------
void communicateWithGps() {
  while (Serial1.available()) {
    gps.parse (Serial1.read());
  }
};

// ------------------------------------------------------------------
void setup() {
  Serial.begin (115200);
  
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
