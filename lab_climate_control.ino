#include "DHT.h"
#include <Wire.h>
#include "I2C_LIB.h"

#define RELAY  2
#define DHTPIN 3

#define TEMPUP   12
#define TEMPDOWN 13
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

int preset_temp = 26;

int relay_delay = 0;

boolean chtsw(byte dx) {
  boolean tsw = digitalRead(dx);
  while (tsw == digitalRead(dx));
  delay(300);
  
  return !tsw;
}



void setup() {
  Serial.begin(9600);
  
  pinMode(RELAY, OUTPUT);
  pinMode(TEMPUP, INPUT_PULLUP);
  pinMode(TEMPDOWN, INPUT_PULLUP);
  
  // LCD
  pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, OUTPUT);
  
  delay(100);
  lcd_init();
  lcd_setCursor(0, 0);
  lcd_printStr("Climate");
  lcd_setCursor(0, 1);
  lcd_printStr("Control");
  
  
  delay(1000);
  
  dht.begin();
}

void loop() {
  char lcd_upper[8];
  char lcd_lower[8];
  
  delay(200);


  if(Serial.avaible() > 0){
    string command = Serial.read();

    if(command == 'up'){
      preset_temp++;
    }
    else if(command == 'down'){
      preset_temp--;
    }
    else if(command == 'cur'){
      Serial.println("Cur: %dC", round(t));
    }
    else if(command == 'set'){
      Serial.println("Set: %dC", preset_temp);
    }
    else {
      
    }
  }

  
  if (digitalRead(TEMPUP) == LOW) {
    preset_temp++;
  }
  if (digitalRead(TEMPDOWN) == LOW) {
    preset_temp--;
  }
  
  float t = dht.readTemperature();
  if(isnan(t)){
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd_clear();
  sprintf(lcd_upper, "Set: %dC", preset_temp);
  lcd_setCursor(0, 0);
  lcd_printStr(lcd_upper);
  sprintf(lcd_lower, "Cur: %dC", round(t));
  lcd_setCursor(0, 1);
  lcd_printStr(lcd_lower);
  
  relay_delay++;
  if (relay_delay > 65534) {
   relay_delay = 300;
  }
  
//  Serial.println(relay_delay);
  
  if (relay_delay > 300) {
    if (round(t) > preset_temp) {
      // スイッチオン！
      digitalWrite(RELAY, HIGH);
//      Serial.println("HIGH");
    }
    else {
      // スイッチオフ！
      digitalWrite(RELAY, LOW);
//      Serial.println("LOW");
    }
    relay_delay = 0;
  }
  
}
