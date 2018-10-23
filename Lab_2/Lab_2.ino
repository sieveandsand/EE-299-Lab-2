

#include <LiquidCrystal.h>

const int LINE_FEED = 10; // ascii code for line feed


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8); // bus 1

int incomingByte = 0;   // for incoming serial data
int firstNum = 0;
bool firstNegative = false;
int secondNum = 0;
bool secondNegative = false;

bool onFirst = true;
bool isFinished = false;
char operation = '+';

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  lcd.display();
}

void loop() {
  // send data only when you receive data
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (incomingByte != LINE_FEED) {

      if (isFinished) {
        firstNum = 0;
        secondNum = 0;
        isFinished = false;
        lcd.clear();
      }
      char data = (char)incomingByte;
      
      lcd.print(data);
      if (data == '+' || data == '-' || data == '*' || data == '/' || data == '=') {
        if (data != '=') {
          operation = data;
          onFirst = false;
        } else { 
          isFinished = true;
          lcd.setCursor(0, 1);
          if (operation == '+') {
            lcd.print(firstNum + secondNum);
          } else if (operation == '-') {
            lcd.print(firstNum - secondNum);
          } else if (operation == '/') {
            lcd.print(firstNum / secondNum);
          } else if (operation == '*') {
            lcd.print(firstNum * secondNum);
          }
        }
      } else if (onFirst) {
        firstNum *= 10;
        firstNum += (data - 48);
      } else if (!onFirst) {
        secondNum *= 10;
        secondNum += (data - 48);
      }
    }
  }
}
