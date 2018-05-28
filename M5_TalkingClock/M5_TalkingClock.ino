#include <M5Stack.h>
#include <WiFi.h>
#include <time.h>
#include "AquesTalkTTS.h"

const char* licencekey = "XXXX-XXXX-XXXX-XXXX";	// AquesTalk-ESP licencekey

const char* ssid       = "";		// WiFi	SSID
const char* password   = "";		// WiFi PW
const char* ntpServer =  "ntp.jst.mfeed.ad.jp";
const long  gmtOffset_sec = 9 * 3600;
const int daylightOffset_sec = 0;

void setup()
{
  int iret;
  Serial.begin(115200);
  M5.begin();
  
 	// Init Text-to-Speech (AquesTalk-ESP + I2S + Internal-DAC)
  iret = TTS.create(licencekey);
  if(iret){
       Serial.print("ERR: TTS_create():");
       Serial.println(iret);
  }
  
  // run clock task
  xTaskCreate(taskClock, "taskClock", 4096, NULL, 2, NULL);
  
}

void loop()
{
  int iret;
  struct tm timeinfo;
  char koe[100];

	if(M5.BtnA.wasPressed()){
	  if(getLocalTime(&timeinfo)){
			// 年月日の読み上げ
	    sprintf(koe,"<NUMK VAL=%d COUNTER=nenn>/<NUMK VAL=%d COUNTER=gatu>/<NUMK VAL=%d COUNTER=nichi>.",
	      timeinfo.tm_year+1900,timeinfo.tm_mon+1, timeinfo.tm_mday);
	    iret = TTS.play(koe, 100);
	    if(iret){
	      Serial.println("ERR:TTS.play()");
	    }
		}
	}
	else if(M5.BtnB.wasPressed()){
	  if(getLocalTime(&timeinfo)){
			// 時分秒の読み上げ
	    sprintf(koe,"<NUMK VAL=%d COUNTER=ji>/<NUMK VAL=%d COUNTER=funn>/<NUMK VAL=%d COUNTER=byo->.",
	      timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
	    iret = TTS.play(koe, 100);
	    if(iret){
	      Serial.println("ERR:TTS.play()");
	    }
		}
	}
	else if(M5.BtnC.wasPressed()){
		TTS.stop();
	}

  M5.update();
}

void taskClock(void *arg)
{

  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);


  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
   struct tm timeinfo;
  getLocalTime(&timeinfo);  // すぐにWiFiのdisconnectするとNG

 //printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  
  for(;;){
	  struct tm timeinfo;
	  if(!getLocalTime(&timeinfo)){
	    Serial.println("Failed to obtain time");
	  }
	  else {
			Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
			DrawTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
		}
		delay(1000);
	}
	
}

// from
//  M5Stack-IDF/components/M5Stack/examples/Advanced/Display/TFT_Clock_Digital/TFT_Clock_Digital.ino
void DrawTime(int hh, int mm, int ss)
{
	static byte omm = 99, oss = 99;
	static byte xcolon = 0, xsecs = 0;
	static unsigned int colour = 0;
	
    int xpos = 0;
    int ypos = 85; // Top left corner ot clock text, about half way down
    int ysecs = ypos + 24;

    if (omm != mm) { // Redraw hours and minutes time every minute
      omm = mm;
      // Draw hours and minutes
      if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
      xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 8);             // Draw hours
      xcolon = xpos; // Save colon coord for later to flash on/off later
      xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
      if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
      xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
      xsecs = xpos; // Sae seconds 'x' position for later display updates
    }
    if (oss != ss) { // Redraw seconds time every second
      oss = ss;
      xpos = xsecs;

      if (ss % 2) { // Flash the colons on/off
        M5.Lcd.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
        M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
      }
      else {
        M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
      }

      //Draw seconds
      if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
      M5.Lcd.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
    }
	
}
