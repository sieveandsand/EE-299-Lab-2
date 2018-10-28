#include <LiquidCrystal.h>

const int LINE_FEED = 10; // ascii code for line feed

LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8); // bus 1

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
  lcd.begin(16, 2);
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  lcd.display();
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte != LINE_FEED) {
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
          Serial.print(firstNum);
          Serial.print(" ");
          Serial.println(secondNum);
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
    lcd.print(firstNum / secondNum);
  } else if (operation == '*') {
    lcd.print(firstNum * secondNum);
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
