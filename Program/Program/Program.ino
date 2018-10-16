#include <LiquidCrystal.h>

const int rs = 2, rw = 3, enable = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, rw, enable, d4, d5, d6, d7);

int n1 = -1, n2 = -1;

void setup() {
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  // intializes the serial monitor with baud rate of 9600
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    n1 = Serial.read();
    Serial.println(n1, DEC);
  }
  

}
