#include "Arduino.h"
#include "tinypipesGSM.h"

Tinypipe::Tinypipe()
{
    isConnected = false;
    answer = 0;
}

void Tinypipe::serialAttach(int baud)
{
    Serial.begin(baud);
}

bool Tinypipe::available()
{
    if(isConnected)
        return true;
    else {
        isConnected  = sendATcommand(AT, RESPONSE_OK, 2000);
        if(isConnected) {
            sendATcommand(AT_SMS_TEXT_MODE, RESPONSE_OK, 1000);
            sendATcommand(AT_NEW_SMS_INDICATION, RESPONSE_OK, 1000);
            sendATcommand(AT_ENABLE_NETWORK_TIME_UPDATE, RESPONSE_OK, 1000);
        }
    }
    return isConnected;
}

void Tinypipe::sendParameter(char[] tag, int val, char[] mobileNumber)
{
    char charVal[50];
    char auxString[50];
    String str(val);
    bool ans = false;

    str.toCharArray(charVal, 50);

    sprintf(auxString, AT_SEND_SMS, mobileNumber);
    ans = sendATcommand(auxString, ">", 2000);
    if(ans) {
        Serial.println(charVal);
        Serial.write(0x1a);

        ans = sendATcommand("", RESPONSE_OK, 20000);
        if(ans) {
            Serial.println("sent");
        }
        else {
            Serial.println("error 2");
        }
    }
    else {
        Serial.println("error 1");
    }
}

String Tinypipe::checkNewSMS()
{
    int i = 0;
    bool ans = false;
    String sms;

    if(Serial.available() == 0)
        return sms;

    clearString(SMS, SMS_SIZE);

    do {
        if(Serial.available() > 0) {
            SMS[i] = Serial.read();
            i++;

            if(strstr(SMS, "enable") != NNULL) {
                sms = "enable";
                ans = true;
            }
            else if(strstr(SMS, "disable") != NULL) {
                sms = "disable";
                ans = true;
            }
            else if(strstr(SMS, "read") != NULL) {
                sms = "read";
                ans = true;
            }
        }
    }while(!ans);
    return sms;
}

String Tinypipe::getLocalTimestamp()
{
    int i = 0;
    int timeout = 2000;
    bool ans = false;
    unsigned long previous;
    String timestamp;

    clearString(LOCAL_TIME, TS_SIZE);
    
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    Serial.println(AT_GET_LOCAL_TIME);
    previous = millis();

    do{
        if(Serial.available() != 0) {
            LOCAL_TIME[i] = Serial.read();
            i++;
        }
    }while((millis() - previous) < timeout);
    
    if(strstr(LOCAL_TIME, RESPONSE_LOCAL_TIME) != NULL) {
        timestamp = String(LOCAL_TIME);
        Serial.println(timestamp.substring(5, 25));
        return timestamp.substring(5, 25);
    }
    else {
        return NULL;
    }
}

bool Tinypipe::sendATcommand(char *cmd, char *result, unsigned int timeout)
{
    int i = 0;
    bool ans = false;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);
    delay(100);

    while(Serial.available() > 0) {
        Serial.read();
    }

    Serial.println(cmd);

    do{
        if(Serial.available() != 0) {
            response[i] = Serial.read();
            i++;

            if(strstr(response, result) != NULL)
                ans = true;
        }
    }while(!ans && ((millis() - previous) < timeout));
    
    return ans;
}

void Tinypipe::clearString(char[] str, int size)
{
    memset(str, '\0', size); } 
