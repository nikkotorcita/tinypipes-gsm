#ifndef _TINYPIPE_H_
#define _TINYPIPE_H_

#define SMS_SIZE                        500 
#define TS_SIZE                         60

#define AT                              "AT"
#define AT_SMS_TEXT_MODE                "AT+CMGF=1"
#define AT_NEW_SMS_INDICATION           "AT+CNMI=3,2,0,0,0"
#define AT_SEND_SMS                     "AT+CMGS=\"%s\""
#define AT_ENABLE_NETWORK_TIME_UPDATE   "AT+CLTS=1"
#define AT_GET_LOCAL_TIME               "AT+CCLK?"

#define RESPONSE_OK                     "OK"
#define RESPONSE_LOCAL_TIME             "+CCLK:"

#define PANEL_ENABLE                    1
#define PANEL_DISABLE                   2
#define PARAM_READ                      3

#include "Arduino.h"

class TinyPipe {
    public:
        TinyPipe();
        ~TinyPipe();
        void serialAttach(long baud);
        void serialAttach(int rx, int tx, int baud);
        bool available();
        void sendParameter(char *tag, int val, char *mobileNumber);
        char *checkNewSMS(char *mobileNumber);
        String getLocalTimestamp();
    
    private: 
        bool isConnected;
        char SMS[SMS_SIZE];
        char LOCAL_TIME[TS_SIZE];

        void clearString(char *str, int size);
        bool sendATcommand(char *cmd, char *result, unsigned int timeout);
        void clearSerialBuffer();
};

extern TinyPipe Tinypipe;
#endif
