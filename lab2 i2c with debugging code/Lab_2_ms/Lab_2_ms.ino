#include <LiquidCrystal.h>
#include <Wire.h>

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
  Wire.begin();
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
        onFirst = true;
        lcd.clear();
      }
      char data = (char)incomingByte;

      //Serial.print(incomingByte);
      //Serial.print(data);
      
      lcd.print(data);
      sendData(data);
      /*
      Wire.beginTransmission(4);
      Wire.write(data);
      Serial.println(data);
      Wire.endTransmission();
      */
      
      if (data == '+' || data == '-' || data == '*' || data == '/' || data == '=') {
        if (data != '=') {
          operation = data;
          onFirst = false;
        } else { 
          isFinished = true;
          lcd.setCursor(0, 1);
          if (operation == '+') {
            lcd.print(firstNum + secondNum);
            sendDataInt((firstNum + secondNum));
          } else if (operation == '-') {
            lcd.print(firstNum - secondNum);
            sendDataInt((firstNum - secondNum));
          } else if (operation == '/') {
            lcd.print(firstNum / secondNum);
            sendDataInt((firstNum / secondNum));
          } else if (operation == '*') {
            lcd.print(firstNum * secondNum);
            sendDataInt((firstNum * secondNum));
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

void sendData(char data) {
  Wire.beginTransmission(4);
  Wire.write(data);
  Serial.println(data);
  Wire.endTransmission();
}

void sendDataInt(int dataInt) {
//  Wire.beginTransmission(4);
//  Wire.write(dataInt);
//  Wire.endTransmission();
//  Serial.println(dataInt);
  
//  int digits[10];
//  int data = dataInt;
//  int i = 0;
//  while (data != 0) {
//    digits[i] = data % 10;
//    data = data / 10;
//    i++;
//  }
//  int j = i;
//  while (j > 0) {
//    Serial.print(digits[j - 1]);
//    j--;
//  }
//  Serial.println();
//  Wire.beginTransmission(4);
//  Wire.write(i);
//  Wire.endTransmission();
//  Wire.beginTransmission(4);
//  Wire.write(digits, 10);
//  Wire.endTransmission();
//  while (i > 0) {
//    Wire.beginTransmission(4);
//    Wire.write(digits[i-1]);
//    Wire.endTransmission();
//    i--;
//  }

  byte data[2];
  data[0] = (dataInt >> 8) & 0xFF;
  data[1] = dataInt & 0xFF;
  Serial.println(dataInt);
  Wire.beginTransmission(4);
  Wire.write(data, 2);
  Wire.endTransmission();
  
}
