#ifndef _NEO_6M
#define _NEO_6M

#define MESSAGE_ID    1
#define LATTITUDE   2
#define LONGITUDE   4
#define GPS_FIX_STATUS  7
#define GPS_CHECKSUM  9
#define GPGLL_LENGTH  5

#define GPS_MESSACE_CODE "GPGLL"

struct GPSInfo {
  char sentenceType[GPGLL_LENGTH];
  double latitude;
  double longitude;
  char hasFix;
  char checkSumA[2];
};

class GPS {
  public:
    GPS();
    void fieldSeparator(char incomingByte);
    double getLatitude();
    double getlLongitude();
    unsigned short hexToDecConverter(char* term);


  private:
    char fieldData[16];
    char filedCounter;
    char charCounter;
    char shouldReadContinue;
    char isChecksum;
    unsigned short checksum;
    GPSInfo gps;
    unsigned short isValidSentence;

    void organizer(char field, char* data);
    double positionFormatter(char* coordinate);
    
};

#endif
