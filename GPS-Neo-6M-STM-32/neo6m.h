#ifndef _NEO_6M
#define _NEO_6M

#define MESSAGE_ID      1
#define LATTITUDE       2
#define LONGITUDE       4
#define GPS_FIX_STATUS  7
#define GPS_CHECKSUM    9
#define GPGLL_LENGTH    5

#define GPS_MESSACE_CODE "GPGLL"

struct GPSInfo {
  char sentenceType[GPGLL_LENGTH];
  double latitude;
  double longitude;
  char hasFix;
  unsigned short checksumRec;
  unsigned short checksumCal;
};

class GPS {
  public:
    GPS();
    void fieldSeparator(char incomingByte);
    double getLatitude();
    double getLongitude();
    char getFix();
    unsigned short getChecksumRec();
    unsigned short getChecksumCal();

  private:
    char fieldData[16];
    unsigned short filedCounter;
    unsigned short charCounter;
    unsigned short shouldReadContinue;
    unsigned short isChecksum;
    unsigned short checksum;
    unsigned short isValidSentence;
    GPSInfo gpsInfo;

    void organizer(char field, char* data);
    double positionFormatter(char* coordinate);
    unsigned short hexToDecConverter(char* term);
};

#endif
