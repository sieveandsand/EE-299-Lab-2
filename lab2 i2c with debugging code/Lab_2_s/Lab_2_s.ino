#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8); // bus 1

bool readIn = true;
bool readAns = false;
//bool readDigits = false;
bool done = false;

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  lcd.display();
  Wire.begin(4);         // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);       // start serial for output
}
void loop()
{
  //delay(100);
}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  //lcd.clear();
  //lcd.setCursor(0, 0);
  /*
  while(2 < Wire.available())  // loop through all but the last
  {
    char c = Wire.read();    // receive byte as a character
    lcd.print(c);      // print the character
    Serial.println(c);
  }*/
  
  //Serial.println(Wire.available());
  
  //lcd.setCursor(0, 1);
  //int x = Wire.read();      // receive byte as an integer
  //lcd.println(x);      // print the integer
  //Serial.println(x);
  int digits = 0;
  int currentDigit = 0;
  if (done) {
    readIn = true;
    readAns = false;
    //readDigits = false;
    done = false;
    lcd.clear();
  }
  if (readIn) {
    char dataChar = Wire.read();
    lcd.print(dataChar);
    Serial.println(dataChar);
    if (dataChar == 61) {
      readIn = false;
      readAns = true;
      //readDigits = true;
      //lcd.setCursor(0, 1);
    }
  } /*else if (readDigits) {
    digits = Wire.read();
    currentDigit = digits;
    readDigits = false;
    readInt = true;
  } */else if (readAns) {
//    if (currentDigit == digits) {
//      lcd.setCursor(0, 1);
//    }
    int ans = 0;
    lcd.setCursor(0, 1);
    byte a, b;
//    byte dataInt = Wire.read();

    Serial.print(Wire.available());
    a = Wire.read();
    b = Wire.read();

    ans = a;
    ans = (ans << 8) | b;
    lcd.print(ans);
    Serial.println(ans);



//    lcd.setCursor(0, 1);
//    int ans = Wire.read();
//    lcd.print(ans);
//    Serial.println(ans);


    
//    lcd.print(dataInt);
//    Serial.println(dataInt);
//    currentDigit--;
//    if (currentDigit == 1) {
//      done = true;
//    }
    
//    while(1 < Wire.available()) { // loop through all but the last
//      int dataInt = Wire.read();    // receive byte as a character
//      lcd.print(dataInt);      // print the character
//      Serial.println(dataInt);
//    }
    done = true;
  }
}
