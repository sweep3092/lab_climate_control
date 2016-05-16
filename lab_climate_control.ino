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

  int serial_input = Serial.available();
  if (serial_input > 0) {
    String command;
    for(int i = 0; i < serial_input; i++) {
      command.concat((char)Serial.read());
    }
    
    if (command.compareTo("up") == 0) {
      preset_temp++;
    }
    else if (command.compareTo("down") == 0) {
      preset_temp--;
    }
    else if (command.compareTo("cur") == 0) {
      Serial.println(round(t));
    }
    else if (command.compareTo("set") == 0) {
      Serial.println(preset_temp);
    }
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

  
  if (relay_delay > 300) {
    if (round(t) > preset_temp) {
      // スイッチオン！
      digitalWrite(RELAY, HIGH);
    }
    else {
      // スイッチオフ！
      digitalWrite(RELAY, LOW);
    }
    relay_delay = 0;
  }
  
}
