#include <Wire.h>

const int LINE_FEED = 10; // ascii code for line feed


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
      
      lcd.print(data);
      sendData(data);
      
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

// send inputs as characters
void sendData(char data) {
  Wire.beginTransmission(4);
  Wire.write(data);
  Serial.println(data);
  Wire.endTransmission();
}

// send answer as two bytes
void sendDataInt(int dataInt) {
  byte data[2];
  data[0] = (dataInt >> 8) & 0xFF;
  data[1] = dataInt & 0xFF;
  Serial.println(dataInt);
  Wire.beginTransmission(4);
  Wire.write(data, 2);
  Wire.endTransmission();
}
