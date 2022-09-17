#include "neo6m.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

GPS::GPS() {
  filedCounter = 0;
  charCounter = 0;
  shouldReadContinue = 0;
  isChecksum = 0;
  checksum = 0x00;
  isValidSentence = 0;
}

void GPS::fieldSeparator(char incomingByte) {
  if (incomingByte == '$') {
    isChecksum = 0;
    checksum = 0x00;
    shouldReadContinue = 1;
    filedCounter = 0;
    charCounter = 0;
    memset(fieldData, '\0', sizeof(fieldData));
  }
  else {
    if ((incomingByte == ',' || incomingByte == '*' || incomingByte == '\r' || incomingByte == '\n') && shouldReadContinue) {
      filedCounter++;
      charCounter = 0;
      organizer(filedCounter, fieldData);
      memset(fieldData, '\0', sizeof(fieldData));
      if (incomingByte == '\r' || incomingByte == '\n') {
        shouldReadContinue = 0;
      }
      if (incomingByte == '*')
      {
        isChecksum = 1;
      }
    }
    else if (shouldReadContinue) {
      fieldData[charCounter] = incomingByte;
      charCounter++;
    }
    if (incomingByte != '*' && incomingByte != '\r' && incomingByte != '\n' && !isChecksum) {
      checksum ^= (unsigned short)incomingByte;
    }
  }

}

double GPS::getLatitude() {
  return gps.latitude;
}

double GPS::getlLongitude() {
  return gps.longitude;
}

void GPS::organizer(char field, char* data) {
  switch (field) {
    case MESSAGE_ID:
      strcpy(gps.sentenceType, data);
      if (strcmp(GPS_MESSACE_CODE, gps.sentenceType) != 0) {
        shouldReadContinue = 0;
      }
      break;
    case LATTITUDE:
      if (isValidSentence) {
        gps.latitude = positionFormatter(data);
      }
      break;
    case LONGITUDE:
      if (isValidSentence) {
        gps.longitude = positionFormatter(data);
      }
      break;
    case GPS_FIX_STATUS: gps.hasFix = *data; break;
    case GPS_CHECKSUM:
      strcpy(gps.checkSumA, data);
      //      hexToDecConverter(gps.checkSumA);
      if (1) {
        isValidSentence = 1;
      } else {
        isValidSentence = 0;
      }
      break;
  }
}

double GPS::positionFormatter(char* coordinate) {
  long intPart = 0;
  long decimalPart = 0L;
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

  long tempDec = (intPart % 100L) * 100000L;
  decimalPart = tempDec + decimalPart;
  intPart = intPart / 100L;

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
