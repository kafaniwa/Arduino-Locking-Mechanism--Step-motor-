#include <Adafruit_PN532.h>
#include "Wire.h"
#include "LiquidCrystal.h" // Optioanl

#include<Uni_polar_Stepper.h>

# define motor_steps 200
 
#define SCK  (13)
#define MOSI (11)
#define SS   (10)
#define MISO (12)
 
Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);




// initialize the stepper library on pins 4 through 7:
Uni_polar_Stepper my_step_motor(4, 5, 6, 7);

int rpm = 150;

int count = 20; // number of steps the motor has taken
bool locked = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("14CORE | NFC - SPI Test Code TAG Reader");
  Serial.println("Initializing please wait.......");
  delay(3000);
  nfc.begin();
 
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Device Found PN5 Chip"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware version > "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.println((versiondata>>8) & 0xFF, DEC);
  nfc.SAMConfig(); //Set to read RFID tags
  Serial.println("Waiting for an ISO14443A Card ...");

  my_step_motor.set_step_per_rev(motor_steps);

my_step_motor.set_RPM(rpm);
}


void loop()
{
   uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  // Look for new cards
if (!success && locked == true)
  {
    for(int i = 0; i < count; i++)
    {
  my_step_motor.rotate_x_deg(90);
    }
    locked = false;
    return;
  }
  
   if (!success)
  {
    locked = false;
    return;
  }
  

  if (success && locked == true)
  {
    return;
  }

 if (success)
  {
  
   for(int i = 0; i < count; i++)
    {
  my_step_motor.rotate_x_deg(270);
    }
  
  locked = true;
  }

 
  
}
