#include <MFRC522.h> // for the RFID
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card

// define pins for RFID
#define CS_RFID 10
#define RST_RFID 9
// define select pin for SD card module
#define CS_SD 4 

// Create a file to store the data
File myFile;

// Instance of the class for RFID
MFRC522 rfid(CS_RFID, RST_RFID); 

// Variable to hold the tag's UID
String uidString;


// Define check in time
const int checkInHour = 9;
const int checkInMinute = 5;

//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;

// Pins for LEDs and buzzer
const int redLED = 6;

void setup() {
  
  // Set LEDs and buzzer as outputs
  pinMode(redLED, OUTPUT);  
  
  // Init Serial port
  Serial.begin(9600);
  while(!Serial); // for Leonardo/Micro/Zero
  
  // Init SPI bus
  SPI.begin(); 
  // Init MFRC522 
  rfid.PCD_Init(); 

  // Setup for the SD card
  Serial.print("Initializing SD card...");

  delay(3000);

  if(!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");

    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  //look for new cards
  digitalWrite(CS_RFID,LOW);
  if(rfid.PICC_IsNewCardPresent()) {
    readRFID();
    logCard();
  }
  delay(10);
}

void readRFID() {
  rfid.PICC_ReadCardSerial();

  Serial.print("Tag UID: ");

  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + 
    String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);
  Serial.println(uidString);

  delay(2000);
  digitalWrite(CS_RFID,HIGH);
}

void logCard() {
  // Enables SD card chip select pin
  digitalWrite(CS_SD,LOW);
  
  // Open file
  myFile=SD.open("DATA.txt", FILE_WRITE);

  // If the file opened ok, write to it
  if (myFile) {
    Serial.println("File opened ok");

    delay(2000);
    myFile.print(uidString);
    myFile.print(", ");   
    
    delay(2000);
    
    myFile.close();

  }
  else {
    
    Serial.println("error opening data.txt");  

  }
  // Disables SD card chip select pin  
  digitalWrite(CS_SD,HIGH);
}
