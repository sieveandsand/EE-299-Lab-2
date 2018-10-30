#include <Wire.h>

const int LINE_FEED = 10; // ascii code for line feed
const int SPACE = 32;

int incomingByte = 0;   // for incoming serial data

char data = 0;
char prevData = 0;
bool firstNegative = false;
bool secondNegative = false;
int firstNum = 0;
int secondNum = 0;
bool onFirst = true;  // initialized to be on the first number
char operation = 0;
bool clearScreen = false;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Wire.begin();
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte != LINE_FEED && incomingByte != SPACE) {
      prevData = data;
      data = (char)incomingByte;
      
      if (clearScreen) {
        clearScreen = false;
      }
      
      // lcd.print(data);
      sendData(data);
      
      if (onFirst) {
        if (prevData == 0 && data == '-') {
          firstNegative = true;
        } else if (!((prevData != '+' || prevData != '-' || prevData != '*' || prevData != '/') &&
            (data == '+' || data == '-' || data == '*' || data == '/'))) {
          firstNum *= 10;
          firstNum += (data - 48);
        } else {
          operation = data;
          onFirst = false;
        }
      } else {
        if ((prevData == '+' || prevData == '-' || prevData == '*' || prevData == '/') &&
            (data == '-')) {  // checks if the second number is negative
          secondNegative = true;     
        } else if ((data != '+' || data != '-' || data != '*' || data != '/') && data != '=') {  // checks if input is a number
          secondNum *= 10;
          secondNum += (data - 48);
        }
        
        if (data == '=') {
          calculate();
          reset();
          clearScreen = true;
        }
      }    
    }
  }
}

// prints the final result
void calculate() {
  if (firstNegative) {
    firstNum *= -1;
  }

  if (secondNegative) {
    secondNum *= -1;
  }

  if (operation == '+') {
    sendDataInt((firstNum + secondNum));
  } else if (operation == '-') {
    sendDataInt((firstNum - secondNum));
  } else if (operation == '/') {
    if (0 == secondNum) {
      sendDataInt(9802);  // add some comments
    } else {
      sendDataInt((firstNum / secondNum));
    }
  } else if (operation == '*') {
    sendDataInt((firstNum * secondNum));
  }
}

void reset() {
  data = 0;
  prevData = 0;
  firstNegative = false;
  secondNegative = false;
  firstNum = 0;
  secondNum = 0;
  onFirst = true;
  operation = 0;
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
