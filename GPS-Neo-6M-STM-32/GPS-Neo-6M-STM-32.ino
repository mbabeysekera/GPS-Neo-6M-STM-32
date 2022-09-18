/*
  This code was specially developed for those who are interested in creating their own library for arduino projects.
  In this project, NEO-6M GPS module is used to demonstrate how to read serial data and process them to
  get GPS coordinates that can be used with GOOGLE APIs to pin the location on GOOGLE maps. 

  For this project no external library has been used besides standard C++ libraries.

  For more details on NEO-6M GPS Module and NMEA (National Marine Electronics Association) please visit 
  following websites:
      GPS Sentence details -> http://aprs.gids.nl/nmea/#gll
                           -> https://www.rfwireless-world.com/Terminology/GPS-sentences-or-NMEA-sentences.html
*/

/*
  Auther  : Buddhika Abeysekera
  Date    : 18/9/2022
  LinkedIn: https://www.linkedin.com/in/babey/ 
  GitHub  : https://github.com/mbabeysekera/
*/

#include "neo6m.h"

unsigned long timer = 0;
GPS gpsService;

void setup() {
  //  disableDebugPorts();
  afio_cfg_debug_ports(AFIO_DEBUG_NONE); // This function will disable all DEBUG pins and use them as GPIOs
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  while (Serial2.available() > 0 ) {
    gpsService.fieldSeparator(Serial2.read());
  }
  /* This section will print processed data to serial monitor of the IDE for every 1second */
  if ((millis() - timer) >= 1000) {
    Serial.print("Position : "); Serial.print(gpsService.getLatitude(), 6); Serial.print(" ,"); Serial.println(gpsService.getLongitude(), 6);
    Serial.print("Fix Data : "); Serial.println(gpsService.getFix());
    Serial.print("CheckRec : "); Serial.println(gpsService.getChecksumRec());
    Serial.print("CheckCal : "); Serial.println(gpsService.getChecksumCal());
    Serial.print("\r\n");
    timer = millis();
  }
}
