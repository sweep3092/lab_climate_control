#define I2Cadr 0x3e  // 固定 I2C用アドレス
byte contrast = 30;   // コントラスト(0～63)

void lcd_cmd(byte x);


//***** I2C_LCD 初期化関数 （必須）
void lcd_init(void) {   // I2C_LCDの初期化
   Wire.begin();
  lcd_cmd(0x38); lcd_cmd(0x39); lcd_cmd(0x4); lcd_cmd(0x14);
  lcd_cmd(0x70 | (contrast & 0xF)); lcd_cmd(0x5C | ((contrast>>4) &0x3));
  lcd_cmd(0x6C); delay(200); lcd_cmd(0x38); lcd_cmd(0x0C); lcd_cmd(0x01);
  delay(2);
}
//***** I2C_LCDへの書込み
void lcd_cmd(byte x) {// I2C_LCDへの書き込み
  Wire.beginTransmission(I2Cadr);
  Wire.write(0x00); // CO = 0,RS = 0
  Wire.write(x);
  Wire.endTransmission();
}
//***** 画面クリア 関数
void lcd_clear(void) {  lcd_cmd(0x01); } 
//***** 画面表示Off 関数
void lcd_DisplayOff() {  lcd_cmd(0x08); }
//***** 画面表示On 関数
void lcd_DisplayOn() { lcd_cmd(0x0C);  }
//===== サブ関数1 
void lcd_contdata(byte x) {
Wire.write(0xC0); // CO = 1, RS = 1
  Wire.write(x);
}
//===== サブ関数２ 
void lcd_lastdata(byte x) {
  Wire.write(0x40); // CO = 0, RS = 1
  Wire.write(x);
}
//***** 文字の表示関数
void lcd_printStr(const char *s) {
  Wire.beginTransmission(I2Cadr);
  while (*s) {
    if (*(s + 1)) {
      lcd_contdata(*s);
    } else {
      lcd_lastdata(*s);
    }
    s++;
  }
  Wire.endTransmission();
}
 
//***** 表示位置の指定関数 ｘ:桁(0～7)、y：行(0,1）
void lcd_setCursor(byte x, byte y) {
  lcd_cmd(0x80 | (y * 0x40 + x));
}

