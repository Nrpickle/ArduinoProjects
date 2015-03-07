/*
 SD card datalogger (with advanced features)

 Nick McComb [nick@nickmccomb.net]
 March 2015
 
 */

#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>

#define DEBUG

const int ledPin = 13;
const int trialResetPin = 9;
const int trialNumAddr = 0;
const int chipSelect = 4;
const int maxTrialNum = 250;

String filename;  //Global filename

short currentTrial;

void setup()
{
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("[Arduino Init]");
    Serial.print("[SD Card Init]");
  #endif 
  pinMode(10, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  //Init card 
  if (!SD.begin(chipSelect)) {
    #ifdef DEBUG
      Serial.println("[### Card Init Failed! ###]");
    #endif
    while(1){
      digitalWrite(ledPin, HIGH);
      delay(150);
      digitalWrite(ledPin, LOW);
      delay(150);
    }
  }
  #ifdef DEBUG
    Serial.println("[SD Card Init Successful]");
  #endif
  
  pinMode(trialResetPin, INPUT_PULLUP);  //"Pull down to reset"
  
  if( !digitalRead(trialResetPin) ){  //If trials need to be reset
    #ifdef DEBUG
      Serial.println("[Resetting Trial Number]");
    #endif 
    currentTrial = 1;
    EEPROM.write(trialNumAddr, currentTrial);
  }
  else {
    currentTrial = EEPROM.read(trialNumAddr);
    EEPROM.write(trialNumAddr, currentTrial + 1);    //Increment EEPROM for next time
  }
  
  //Check for too large of trial number (only up to 250 supported currently), overflows after that
  if(currentTrial >= maxTrialNum) {
    #ifdef DEBUG
      Serial.println("[#Trial too high, resetting... #]");
    #endif
    currentTrial = 1;
    EEPROM.write(trialNumAddr, currentTrial);
  }
  
  //Setup filename
  filename = "LOG";
  filename += String(currentTrial);
  filename += ".txt";
  
  #ifdef DEBUG
    Serial.println("[End Arduino Init]");
    Serial.print("[Current Trial: ");
    Serial.print(currentTrial);
    Serial.println("]");
  #endif
}

/*

Final output string:
iter, A0, A1, time, potato
*/
void loop()
{
  while(1);
  String dataString = "";
  
  static int iter = 0;
  
  dataString += String(iter);
  dataString += ",";
  
  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int data = analogRead(analogPin);
    dataString += String(data);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  dataString += ",";
  dataString += "potato";
  

  
  File dataFile = SD.open(filename.c_str(), FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

/*

#include <SD.h>

const int chipSelect = 4;

Sd2Card card;

#define DEBUG

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("[System Init]");
  
  Serial.println("[SD Card Init]");
  #endif
  
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }
  
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  
  
}

void loop(){

}

*/