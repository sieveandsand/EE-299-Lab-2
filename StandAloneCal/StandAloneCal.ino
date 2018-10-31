/*
 * EE 299 Lab 2 Stand alone calculator
 * 
 * Feifan Qiao, Mitchell Szeto, Bert Zhao
 * 
 * This is a stand alone calculator that takes user input
 * and prints calculation result on a lcd screen
 * 
 * Last modified: 10/30/2018
 */
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8); // bus 1

const int LINE_FEED = 10;     // ascii code for line feed
const int SPACE = 32;         // ascii code for space

int incomingByte = 0;         // for incoming serial data

char data = 0;
char prevData = 0;
bool firstNegative = false;   // checks if the first number is negative
bool secondNegative = false;  // checks if the second number is negative
int firstNum = 0;             // stores the value of the first number
int secondNum = 0;            // stores the value of the second number
bool onFirst = true;          // checks if current step is on the first number
char operation = 0;           // stores the operator
bool clearScreen = false;     // checks if it's time to clear screen

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);        // opens serial port, sets data rate to 9600 bps
  lcd.display();
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte != LINE_FEED && incomingByte != SPACE) {
      prevData = data;
      data = (char)incomingByte;
      
      if (clearScreen) {
        lcd.clear();
        lcd.setCursor(0, 0);
        clearScreen = false;
      }
      
      lcd.print(data);

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

// calculates and prints the final result
void calculate() {
  lcd.setCursor(0, 1);  // sets the cursor on the second row
  
  if (firstNegative) {
    firstNum *= -1;
  }

  if (secondNegative) {
    secondNum *= -1;
  }

  if (operation == '+') {
    lcd.print(firstNum + secondNum);
  } else if (operation == '-') {
    lcd.print(firstNum - secondNum);
  } else if (operation == '/') {
    if (0 == secondNum) {
      lcd.print("Div by 0 error");
    } else {
      lcd.print(firstNum / secondNum);
    }
  } else if (operation == '*') {
    lcd.print(firstNum * secondNum);
  }
}

// resets all variable values
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
