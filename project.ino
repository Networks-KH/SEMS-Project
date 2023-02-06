//#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal lcd(13, 12, 14, 27, 26, 25);
#define BLYNK_PRINT Serial
#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
EnergyMonitor emon;
#define vCalibration 83.3   //
#define currCalibration 0.50  //
BlynkTimer timer;
char auth[] = "tXVq_VqvrHFbHZEHm5OtX6Q3nLj8c0FR"; //Your auth token from email
char ssid[] = "omarIP";    //Type your WiFi name
char pass[] = "12345678";    //Type your WiFi password
float kWh = 0;
float cost = 0;
unsigned long lastmillis = millis();//
void myTimerEvent()
{
  emon.calcVI(20, 2000);
  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;  // 
  cost = kWh*0.18;
  yield();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  Serial.print("Vrms: ");
  Serial.println(emon.Vrms, 2);
  lcd.print(emon.Vrms, 2);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Irms:");
  lcd.print(emon.Irms, 4);
  lcd.print("A");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power:");
  lcd.print(emon.apparentPower, 4);
  lcd.print("W");
  lcd.setCursor(0, 1);
  lcd.print("kWh:");
  lcd.print(kWh, 4);
  lcd.print("W");
   delay(2500);
   lcd.clear();
   lcd.setCursor(0, 0);
  lcd.print("cost:");
  lcd.print(cost, 4);
  lcd.print("R");
  delay(2500);
  lastmillis = millis();
  Blynk.virtualWrite(V0, emon.Vrms);
  Blynk.virtualWrite(V1, emon.Irms);
  Blynk.virtualWrite(V2, emon.apparentPower);
  Blynk.virtualWrite(V3, kWh);
  Blynk.virtualWrite(V4, cost);
}
void setup()
{
//  lcd.begin(16, 2);
  //lcd.init(); 
  Serial.begin(112500);
  lcd.begin();                     
  lcd.backlight();
   Blynk.begin(auth, ssid, pass,"Blynk.cloud",80);
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration);   // Current: input pin, calibration.
  timer.setInterval(5000L, myTimerEvent);
  lcd.setCursor(3, 0);
  lcd.print("Smart Energy");
  
  lcd.setCursor(5, 1);
  lcd.print("Monitor");
  delay(3000);
  lcd.clear();
}
void loop()
{
  Blynk.run();
  timer.run();
}
