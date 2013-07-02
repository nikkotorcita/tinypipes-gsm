#ifndef _TINYPIPE_H_
#define _TINYPIPE_H_

#define SMS_SIZE 500 
#define TS_SIZE 40

#define AT "AT"
#define AT_SMS_TEXT_MODE "AT+CMGF=1"
#define AT_NEW_SMS_INDICATION "AT+CNMI=3,2,0,0,0"
#define AT_SEND_SMS "AT+CMGS=\"%s\""
#define AT_ENABLE_NETWORK_TIME_UPDATE "AT+CLTS=1"
#define AT_GET_LOCAL_TIME "AT+CCLK?"

#define RESPONSE_OK "OK"
#define RESPONSE_LOCAL_TIME "+CCLK:"

class Tinypipe {
    public:
        Tinypipe();
        ~Tinypipe();
        void serialAttach(int baud);
        void serialAttach(int rx, int tx, int baud);
        bool available();
        void sendParameter(String tag, int val);
        String checkNewSMS();
        String getLocalTimestamp();
    
    private: 
        bool isConnected;
        char SMS[SMS_SIZE];
        char LOCAL_TIME[TS_SIZE];

        void clearString(char str[]);
        bool sendATcommand(char *cmd, char *result, unsigned int timeout);
}
#endif
