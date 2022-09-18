#include "neo6m.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

GPS::GPS() {
}

void GPS::fieldSeparator(char byteValue) {
  switch (byteValue) {
    case '$': {
        filedCounter = 0;
        charCounter = 0;
        shouldReadContinue = 1;
        isChecksum = 0;
        checksum = 0;
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

double GPS::getLatitude() {
  double tempResult;
  if (1) {
    tempResult = gpsInfo.latitude;
  } else {
    tempResult = 9999.0;
  }
  return tempResult;
}

double GPS::getLongitude() {
  double tempResult;
  if (1) {
    tempResult = gpsInfo.longitude;
  } else {
    tempResult = 9999.0;
  }
  return tempResult;
}

char GPS::getFix() {
  return gpsInfo.hasFix;
}
unsigned short GPS::getChecksumRec() {
  return gpsInfo.checksumRec;
}
unsigned short GPS::getChecksumCal() {
  return gpsInfo.checksumCal;
}

void GPS::organizer(char field, char* data) {
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
          if (gpsInfo.checksumRec != gpsInfo.checksumCal) {
            isValidSentence = 0;
          } else {
            isValidSentence = 1;
          }
        }
      }
      break;
  }
  charCounter = 0;
  memset(fieldData, '\0', sizeof(fieldData));
}

double GPS::positionFormatter(char* coordinate) {
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

unsigned short GPS::hexToDecConverter(char* term) {
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
