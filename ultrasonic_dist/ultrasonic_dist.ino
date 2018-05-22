#include "Ultrasonic.h"
#include <M5Stack.h>

#define DISTANCE 30

Ultrasonic ultrasonic(22);

void setup()
{
    Serial.begin(115200);
    M5.begin();
    delay(500);
}
void loop()
{
    long RangeInInches;
    long RangeInCentimeters;

    RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval

//    Serial.print("Distance: " + RangeInCentimeters);
//    Serial.print(RangeInCentimeters);//0~400cm
//    Serial.println(" cm");

    M5.Lcd.clear();

    m5.Lcd.setTextColor(WHITE);
    m5.Lcd.setCursor(40, 80);
    M5.Lcd.setTextFont(4);
    M5.Lcd.printf("Dist = %d cm", RangeInCentimeters);
    delay(500);
}
