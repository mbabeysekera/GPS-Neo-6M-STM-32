#include "neo6m.h"

unsigned long timer = 0;
GPS gpsService;

void setup() {
  disableDebugPorts();
  afio_cfg_debug_ports(AFIO_DEBUG_NONE);
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  while (Serial2.available() > 0 ) {
    gpsService.fieldSeparator(Serial2.read());
  }
  if ((millis() - timer) >= 1000) {
//    Serial.print("MessageID: "); Serial.println(gpsInfo.sentenceType);
    Serial.print("Position : "); Serial.print(gpsService.getLatitude(), 6); Serial.print(" ,"); Serial.println(gpsService.getLongitude(), 6);
    Serial.print("Fix Data : "); Serial.println(gpsService.getFix());
    Serial.print("CheckRec : "); Serial.println(gpsService.getChecksumRec());
    Serial.print("CheckCal : "); Serial.println(gpsService.getChecksumCal());
    Serial.print("\r\n");
    timer = millis();
  }
}
