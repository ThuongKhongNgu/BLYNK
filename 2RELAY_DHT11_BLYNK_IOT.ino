#define BLYNK_TEMPLATE_ID "TMPL6Wg_LoLnr"
#define BLYNK_TEMPLATE_NAME "Giam sat nhiet do do am tren man lcd"
#define BLYNK_AUTH_TOKEN "e_PYXgdWXlEQDSeRn5J0RwNzzLrIiQow"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include "DHTesp.h"
DHTesp dht;
#define dhtPin D0
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Trieu Ninh";
char pass[] = "12344321";
#define button1 D3
#define button2 D4
#define relay1 D5
#define relay2 D6
unsigned long timeBlink=millis();
unsigned long timeDelay=millis();
boolean updateRelay=0;
WidgetLED LEDCONNECT(V0);
#define RELAY1 V1
#define RELAY2 V2
#define TEMP   V3
#define HUMI   V4

// Every time we connect to the cloud...
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(RELAY1,RELAY2);
}

// When App button is pushed - switch the state
BLYNK_WRITE(RELAY1) {
  int p = param.asInt();
  digitalWrite(relay1, p);
}
BLYNK_WRITE(RELAY2) {
  int p = param.asInt();
  digitalWrite(relay2, p);
}

ICACHE_RAM_ATTR void handleButton(){
  if(millis()-timeDelay>200){
    if(digitalRead(button1)==LOW){
      digitalWrite(relay1,!digitalRead(relay1));
    }
    if(digitalRead(button2)==LOW){
      digitalWrite(relay2,!digitalRead(relay2));
    }
    updateRelay=1;
    timeDelay=millis();
  }
}

void setup(){
  // Debug console
  Serial.begin(115200);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  attachInterrupt(button1,handleButton,FALLING);
  attachInterrupt(button2,handleButton,FALLING);

  dht.setup(dhtPin, DHTesp::DHT11);

  Wire.begin();
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Dang thiet lap");
  lcd.setCursor(0,1);
  lcd.print("ket noi Blynk!");
  delay(1000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0,0);        
  lcd.print("Nhiet do:");    
  lcd.setCursor(0,1);        
  lcd.print("Do am   :");
}

void loop(){
  Blynk.run();
  if(millis()-timeBlink>1000){
    if(LEDCONNECT.getValue()){
      LEDCONNECT.off();
    }else{
      LEDCONNECT.on();
    }

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    Serial.print(dht.getStatusString());
    Serial.print("\t");
    Serial.print(humidity, 1);
    Serial.print("\t\t");
    Serial.println(temperature, 1);
    if(dht.getStatusString()=="OK"){
      Blynk.virtualWrite(TEMP,temperature);
      Blynk.virtualWrite(HUMI,humidity);

      lcd.setCursor(9,0);        
      lcd.print(String(temperature,1)); 
      lcd.print((char)223);
      lcd.print("C  ");   
      lcd.setCursor(9,1);
      lcd.print(String(humidity,1));        
      lcd.print("%  ");
    }
    timeBlink=millis();
  }
  if(updateRelay==1){
    Blynk.virtualWrite(RELAY1,digitalRead(relay1));
    Blynk.virtualWrite(RELAY2,digitalRead(relay2));
    updateRelay=0;
  }
}