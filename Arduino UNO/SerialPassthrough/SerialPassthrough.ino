#include <SoftwareSerial.h>

SoftwareSerial passthroughSerial(10, 11);  // RX, TX

#define SOFT_BAUD_RATE 9600
#define HARD_BAUD_RATE 9600

void setup(){
  Serial.begin(9600);
  
  Serial.println("[Arduino Init]");
  Serial.print("[Passthrough at ");
  Serial.print(SOFT_BAUD_RATE);
  Serial.println(" Baud]");
  
  passthroughSerial.begin(9600);  //Communication
}

void loop(){
    if(Serial.available()) {
      passthroughSerial.print((char) Serial.read());
    }
    if(passthroughSerial.available()){
      Serial.prwint((char) passthroughSerial.read());
    }
}
