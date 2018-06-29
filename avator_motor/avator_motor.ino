#include <M5Stack.h>
#include "avator.h"
#include "AquesTalkTTS.h"

const int pwmChannel = 1;
const int stepPin = 21; 
const int dirPin = 22; 

const char* licencekey = ""; // AquesTalk-ESP licencekey

//const char* ssid       = "";        // WiFi SSID
//const char* password   = "";        // WiFi PW
//const char* ntpServer =  "ntp.jst.mfeed.ad.jp";
//const long  gmtOffset_sec = 9 * 3600;
//const int daylightOffset_sec = 0;

Avator *avator;
int count = 0;
float f = 0;
float last = 0;
const Expression expressions[] = {
  Angry,
  Sleepy,
  Happy,
  Sad
};
const int expressionsSize = sizeof(expressions) / sizeof(Expression);
int idx = 0;

void breath(void *args)
{
  int c = 0;
  for(;;)
  {
    c = c + 1 % 100;
    float f = sin(c * 2 * PI / 100.0);
    avator->setBreath(f);
    delay(33);
  }
}

void drawLoop(void *args)
{
  for(;;)
  {
//    int level = TTS.getLevel();
//    float f = level / 12000.0;
//    float open = min(1.0, last + f / 2.0);
    count += 3;
    float f0 = ((count % 360) / 180.0) * PI;
    float open = (sin(f0) + 1.0) / 2.0;
    last = f;
    avator->setMouthOpen(open);
    avator->draw();
    delay(33);
  }
}

void saccade(void *args)
{
  for(;;)
  {
    float vertical = (float)rand()/(float)(RAND_MAX / 2) - 1;
    float horizontal = (float)rand()/(float)(RAND_MAX / 2) - 1;
    avator->setGaze(vertical, horizontal);
    delay(500 + 100 * random(20));
  }
}

void blink(void *args)
{
  for(;;)
  {
    avator->setEyeOpen(1);
    delay(2500 + 100 * random(20));
    avator->setEyeOpen(0);
    delay(300 + 10 * random(20));
  }
}

void setup()
{
  int iret;
  iret = TTS.create(licencekey);
  M5.begin();
//ledcSetup(pwmChannel, 10000, 8);
//ledcAttachPin(stepPin, pwmChannel);
  M5.Lcd.setBrightness(30);
  avator = new Avator();
  xTaskCreatePinnedToCore(
                    drawLoop,     /* Function to implement the task */
                    "drawLoop",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
                    saccade,     /* Function to implement the task */
                    "saccade",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    3,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
                    breath,     /* Function to implement the task */
                    "breath",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
  xTaskCreatePinnedToCore(
                    blink,     /* Function to implement the task */
                    "blink",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    1);        /* Core where the task should run */
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  digitalWrite(stepPin,LOW);
  delay(500);
}

void loop()
{
  M5.update();
  if (M5.BtnA.wasPressed())
  {
     TTS.play("hidari-", 90);
    avator->setExpression(Neutral);
    digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
    // Makes 200 pulses for making one full cycle rotation
    for(int x = 0; x < 1800; x++) {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(900); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(100); 
    }
//    delay(1000); // One second delay    
//    digitalWrite(stepPin,HIGH);
    delay(800); // One second delay 
//    digitalWrite(stepPin,LOW) ;
  }
  if (M5.BtnB.wasPressed())
  {
     TTS.play("hai-ti-zu-", 90);
    avator->setExpression(expressions[idx]);
    idx = (idx + 1) % expressionsSize;
  }
  if (M5.BtnC.wasPressed())
  {
     TTS.play("migi- ", 90);
    avator->setExpression(Sad);
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
    // Makes 200 pulses for making one full cycle rotation
    for(int x = 0; x < 1800; x++) {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(900); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(100); 
    }
//    digitalWrite(stepPin,HIGH);
    delay(800); // One second delay 
//    digitalWrite(stepPin,LOW) ;
  }
  delay(250);
}
