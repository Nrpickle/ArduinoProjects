// Connections:
// LCD pin 1 to Arduino GND
// LCD pin 2 to Arduino 5v
// LCD pin 3 (Contrast) to GND
// rs (LCD pin 4) to Arduino pin 12
// rw (LCD pin 5) to Arduino pin 11
// enable (LCD pin 6) to Arduino pin 10
// LCD pin 15 to Arduino pin 13
// LCD pin 16 to Arduino GND
// LCD pins d4, d5, d6, d7 to Arduino pins 5, 4, 3, 2

//Tested with DS1307 Breakout from Sparkfun
//pin SDA to Arduino Analog pin 4
//pin SCL to Arduino Analog pin 5
//pin GND to Arduino GND
//pin VCC to Arduino 5v


#include <Wire.h>
#define DS1307_I2C_ADDRESS 0x68
//#include <LiquidCrystal.h> // we need this library for the LCD commands
//LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

int backLight = 13; // pin 13 will control the backlight

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}


void setup()
{
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  Wire.begin();
  Serial.begin(9600);
  
  //For reprogramming only
  second = 0;
  minute = 55;
  hour = 23;
  dayOfWeek = 6;
  dayOfMonth = 6;
  month = 3;
  year = 15;
  
  //setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  
  //lcd.begin(16, 2); // tells Arduino the LCD dimensions

}
void loop()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  Serial.print(" ");
  Serial.print(hour, DEC);
  Serial.print(":");
  if (minute<10) {
    Serial.print("0");
  }
    Serial.print(minute, DEC);
    Serial.print(":");
  if (second<10) {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  switch(dayOfWeek){
    case 1:
      Serial.print("Sun");
      break;
    case 2:
      Serial.print("Mon");
      break;
    case 3:
      Serial.print("Tue");
      break;
    case 4:
      Serial.print("Wed");
      break;
    case 5:
      Serial.print("Thu");
      break;
    case 6:
      Serial.print("Fri");
      break;
    case 7:
      Serial.print("Sat");
      break;
  }
  Serial.print(" ");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/20");
  Serial.print(year, DEC);
  Serial.println("");
  delay(1000);
}

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
void setDateDs1307 (byte second, // 0-59
  byte minute, // 0-59
  byte hour, // 1-23
  byte dayOfWeek, // 1-7
  byte dayOfMonth, // 1-28/29/30/31
  byte month, // 1-12
  byte year) // 0-99
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second)); // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.write(0x10); // sends 0x10 (hex) 00010000 (binary) to control register - turns on square wave
  Wire.endTransmission();
}

// Gets the date and time from the ds1307
void getDateDs1307(byte *second,
  byte *minute,
  byte *hour,
  byte *dayOfWeek,
  byte *dayOfMonth,
  byte *month,
  byte *year)
{
// Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // A few of these need masks because certain bits are control bits
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f); // Need to change this if 12 hour am/pm
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}