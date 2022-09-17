#include <string>
#include <stdlib.h>
#include <ctype.h>

#define MESSAGE_ID    1
#define LATTITUDE   2
#define LONGITUDE   4
#define GPS_FIX_STATUS  7
#define GPS_CHECKSUM  9
#define GPGLL_LENGTH  5

#define GPS_MESSACE_CODE "GPGLL"

using namespace std;

struct GPSInfo {
  char sentenceType[GPGLL_LENGTH];
  double latitude;
  double longitude;
  char hasFix;
  unsigned short checksumRec;
  unsigned short checksumCal;
};

char fieldData[16];

unsigned short filedCounter = 0;
unsigned short charCounter = 0;
unsigned short shouldReadContinue = 0;
unsigned short isChecksum = 0;
unsigned short checksum = 0x00;
unsigned short isValidSentence = 0;
GPSInfo gpsInfo;

unsigned long timer = 0;

char readSimulator(char* string);
void fieldSeparator(char byte);
void organizer(char field, char* data);
double positionFormatter(char* coordinate);
unsigned short hexToDecConverter(char* term);

void setup() {
  disableDebugPorts();
  afio_cfg_debug_ports(AFIO_DEBUG_NONE);
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  while (Serial2.available() > 0 ) {
    fieldSeparator(Serial2.read());
  }
  if ((millis() - timer) >= 1000) {
    if(gpsInfo.checksumRec != gpsInfo.checksumCal) {
      gpsInfo.latitude = 9999.0;
      gpsInfo.longitude = 9999.0;
    }
    //    Serial.print("MessageID: "); Serial.println(gpsInfo.sentenceType);
    Serial.print("Position : "); Serial.print(gpsInfo.latitude, 6); Serial.print(" ,"); Serial.println(gpsInfo.longitude, 6);
    //    Serial.print("Fix Data : "); Serial.println(gpsInfo.hasFix);
    //    Serial.print("CheckRec : "); Serial.println(gpsInfo.checksumRec);
    //    Serial.print("CheckCal : "); Serial.println(gpsInfo.checksumCal);
    //    Serial.print("\r\n");
    timer = millis();
  }

}

void fieldSeparator(char byteValue) {
  switch (byteValue) {
    case '$': {
        filedCounter = 0;
        charCounter = 0;
        shouldReadContinue = 1;
        isChecksum = 0;
        checksum = 0x00;
        isValidSentence = 0;
      } break;
    case '\r':
    case ',':
      {
        if (byteValue == ',') {
          checksum ^= (short)byteValue;
        }
        charCounter = 0;
        filedCounter++;
        organizer(filedCounter, fieldData);
      } break;
    case '*': {
        isChecksum = 1;
        charCounter = 0;
        filedCounter++;
      } break;
    case '\n': {
        shouldReadContinue = 0;
        filedCounter = 0;
      }
      break;
    default: {
        if (shouldReadContinue) {
          fieldData[charCounter] = byteValue;
          charCounter++;
          if (!isChecksum) {
            checksum ^= (short)byteValue;
          }
        }
      };
  }
}

void organizer(char field, char* data) {
  switch (field) {
    case MESSAGE_ID: {
        if (strcmp(GPS_MESSACE_CODE, data) == 0) {
          strcpy(gpsInfo.sentenceType, data);
          shouldReadContinue = 1;
        } else {
          shouldReadContinue = 0;
        }
      } break;
    case LATTITUDE: {
        if (shouldReadContinue) {
          gpsInfo.latitude = positionFormatter(data);
        }
      }
      break;
    case LONGITUDE: {
        if (shouldReadContinue) {
          gpsInfo.longitude = positionFormatter(data);
        }
      }
      break;
    case GPS_FIX_STATUS: {
        if (shouldReadContinue) {
          gpsInfo.hasFix = *data;
        }
      }
      break;
    case GPS_CHECKSUM: {
        if (shouldReadContinue) {
          gpsInfo.checksumRec = hexToDecConverter(data);
          gpsInfo.checksumCal = checksum;
        }
      }
      break;
  }
  charCounter = 0;
  memset(fieldData, '\0', sizeof(fieldData));
}

double positionFormatter(char* coordinate) {
  long intPart = 0;
  long decimalPart = 0;
  char isDecimal = 0;
  char tempArray[8] = { 0 };
  int counter = 0;
  char isNegative = 0;
  while (*coordinate) {
    if (*coordinate == '-') {
      isNegative = 1;
    }
    else if (*coordinate == '.') {
      isDecimal = 1;
      counter = 0;
      intPart = atol(tempArray);
    }
    else {
      tempArray[counter] = *coordinate;
      counter++;
    }
    coordinate++;
  }
  decimalPart = atol(tempArray);
  long tempDec = (intPart % 100) * 100000L;
  decimalPart = tempDec + decimalPart;
  intPart = intPart / 100;
  double r = ((double)intPart + ((double)decimalPart / 10000000.0) * 1.6666667);
  if (isNegative) {
    r *= -1;
  }
  return r;
}

unsigned short hexToDecConverter(char* term) {
  short val[2] = { 0 };
  int i = 0;
  while (*term) {
    if (*term >= 'A' && *term <= 'F') {
      val[i] = (short)(*term - 55);
    }
    else {
      val[i] = (short)(*term - 48);
    }
    term++;
    i++;
  }
  return val[0] * 16 + val[1];
}
