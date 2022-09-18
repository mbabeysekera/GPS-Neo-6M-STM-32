#ifndef _NEO_6M
#define _NEO_6M

#define MESSAGE_ID      1
#define LATTITUDE       2
#define LONGITUDE       4
#define GPS_FIX_STATUS  7
#define GPS_CHECKSUM    9
#define GPGLL_LENGTH    5

#define GPS_MESSACE_CODE "GPGLL" //GPGLL message was selected for the project

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
    void fieldSeparator(char incomingByte); //this function will process each character
    double getLatitude();
    double getLongitude();
    char getFix();
    unsigned short getChecksumRec();
    unsigned short getChecksumCal();

  private:
    char fieldData[16]; // to hold the incoming data temporary, array of 16 eliments is used
    unsigned short filedCounter;
    unsigned short charCounter;
    unsigned short shouldReadContinue;
    unsigned short isChecksum;
    unsigned short checksum;
    unsigned short isValidSentence;
    GPSInfo gpsInfo;

    void organizer(char field, char* data); // each filed is identified by this function and put them in relevent field
    double positionFormatter(char* coordinate); // GPS module LAT LONG will be converted into standard GPS coordinates
    unsigned short hexToDecConverter(char* term); // convert chechsum data from GPS module to decimal format
};

#endif
