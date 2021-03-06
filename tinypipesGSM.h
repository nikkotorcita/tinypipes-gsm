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
        /*
         * Attaches the Arduino's Serial object to the Tinypipe class
         * 
         * Parameters   : 
         *      baud     the desired baud rate
         *
         * Returns      : 
         *      none
         */
        void serialAttach(long baud);
        
        /*
         * Checks whether a GSM module is available on the serial port or not
         *
         * Parameters   :
         *      none
         *
         * Returns      :
         *     true if a GSM module is available
         */
        bool available();
        
        /*
         * Sends an SMS message to the specified mobile number
         * Parameters   :
         *      tag     string of characters that can be used to describe the payload data
         *      val     the payload data as an SMS message
         *      mobileNumber    the mobile number of the receiving device. Should be in
         *                      international format e.g. +639175878280
         * Returns      :
         *      none
         */
         void sendParameter(char *tag, int val, char *mobileNumber);

        /*
         * Checks whether a new SMS message has arrived from the specified mobile number
         * Parameters   :
         *      mobileNumber    the mobile number to check messages from
         * Returns      :
         *      none
         */
        String checkNewSMS(char *mobileNumber);
        
        /*
         * Returns the local date and time in yy/MM/dd,hh:mm:ss format
         * Parameters
         *      none
         * Returns
         *      The local timestamp
         */
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
