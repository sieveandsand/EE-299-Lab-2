#include <LiquidCrystal.h>
#include <Wire.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8); // bus 1

bool readIn = true;
bool readAns = false;
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
  delay(100);
}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  if (done) { // finished one operation 
    readIn = true;
    readAns = false;
    done = false;
    lcd.clear();
  }
  if (readIn) { // read inputs as characters
    char dataChar = Wire.read();
    lcd.print(dataChar);
    Serial.println(dataChar);
    if (dataChar == 61) {
      readIn = false;
      readAns = true;
    }
  } else if (readAns) { // read answer as integer
    int ans = 0;
    lcd.setCursor(0, 1);

    // read answer as 2 bytes
    byte a, b;
    a = Wire.read();
    b = Wire.read();
    
    ans = a;
    ans = (ans << 8) | b;
    lcd.print(ans);
    Serial.println(ans);
    done = true;
  }
}
