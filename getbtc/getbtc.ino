#include <M5Stack.h>

using namespace std;
#include "Free_Fonts.h"
#include <WiFi.h>
#include <string>
#include <math.h>
#include <vector>
#include <string>

int status = WL_IDLE_STATUS;
int lastPrice = 0;
int currentPrice;
int minPrice = 9999999; 
int maxPrice = 0;
char servername[] = "api.coindesk.com";
String answer;
WiFiClient client;

// the setup routine runs once when M5Stack starts up
void setup(){
  WiFi.begin("fns-net-2.4G", "13261632");
  Serial.begin(115200);
  M5.begin();

  // LCD display
  M5.Lcd.println("bitcoin");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("wifi connect ok");
  ConnectToClient();
}

void ConnectToClient(){
  if (client.connect(servername, 80)) {
    // Make a HTTP request:
    client.print(String("GET ") + "/v1/bpi/currentprice.json HTTP/1.1\r\n" +
                 "Host: api.coindesk.com\r\n" +
                 "Connection: close\r\n\r\n");
  }
}

void loop() {
  if (client.available()) {
    char c = client.read();
    answer += c;
  }

  if (!client.connected()) {
    m5.update();
    client.stop();

    String jsonAnswer;
    int jsonIndex;

    for (int i = 0; i < answer.length(); i++) {
      if (answer[i] == '{') {
        jsonIndex = i;
        break;
      }
    }

    jsonAnswer = answer.substring(jsonIndex);
    jsonAnswer.trim();

    int dateIndex = jsonAnswer.indexOf("updated");
    String dateString = jsonAnswer.substring(dateIndex + 10, dateIndex + 34);
    int rateIndex = jsonAnswer.indexOf("rate_float");
    String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 19);
    Serial.println(jsonAnswer);
    Serial.println("");
    Serial.println(dateString);
    Serial.println(priceString);
    dateString.trim();
    priceString.trim();
    int decimalplace = priceString.indexOf(".");
    String Dollars = priceString.substring(0, decimalplace);
    String Cents = priceString.substring(decimalplace+1);
    while (Cents.length() < 2) {
      Cents += "0";
    }
    String Amount = "$" + Dollars + "." + Cents;

    currentPrice = (Dollars + Cents).toInt();
    minPrice = min(minPrice,currentPrice);
    maxPrice = max(maxPrice,currentPrice);

    m5.Lcd.fillScreen(0x0000);
    m5.Lcd.setFont(&FreeSans9pt7b);

    m5.Lcd.setTextColor(RED);
    m5.Lcd.setCursor(20, 20);
    m5.Lcd.printf(("Min: " + String(minPrice).substring(0,Dollars.length()) + "." + String(minPrice).substring(Dollars.length())).c_str());

    m5.Lcd.setTextColor(GREEN);
    m5.Lcd.setCursor(205, 20);
    m5.Lcd.printf(("Max: " + String(maxPrice).substring(0,Dollars.length()) + "." + String(maxPrice).substring(Dollars.length())).c_str());

    m5.Lcd.setTextColor(WHITE);
    m5.Lcd.setCursor(50, 80);
    m5.Lcd.setFont(&FreeMonoBold18pt7b);
    m5.Lcd.printf("BTC Price");
    m5.Lcd.printf("\r\n");

    m5.Lcd.setCursor(70, 130);
    m5.Lcd.printf(Amount.c_str());

    m5.Lcd.setFont(&FreeMonoBold9pt7b);
    m5.Lcd.setCursor(25, 175);
    m5.Lcd.printf(String(dateString).c_str());
    
    if (currentPrice >= lastPrice) //UP
    {
      m5.Lcd.fillTriangle(140, 215, 180, 215, 160, 190, GREEN);
    }
    else if (currentPrice < lastPrice) //Down
    {
      m5.Lcd.fillTriangle(140, 215, 180, 215, 160, 240, RED);
    }

    lastPrice = currentPrice;

    // wait 20 seconds and key check
    for (int i = 0; i < 20; i++){
      if(M5.BtnA.wasPressed()) {
        m5.lcd.setBrightness(0);
      }
      if(M5.BtnB.wasPressed()) {
        m5.lcd.setBrightness(25);
      }
      if(M5.BtnC.wasPressed()) {
        m5.lcd.setBrightness(150);
      }
      m5.update();
      delay(1000);
    }
    answer = "";
    Amount = "";
    currentPrice = 0;
    ConnectToClient();
  }
}