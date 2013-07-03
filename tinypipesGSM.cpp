#include "tinypipesGSM.h"

TinyPipe Tinypipe;

TinyPipe::TinyPipe()
{
    isConnected = false;
}

TinyPipe::~TinyPipe()
{

}

void TinyPipe::serialAttach(long baud)
{
    Serial.begin(baud);
    Serial.println("attached!");
}

bool TinyPipe::available()
{
    if(isConnected)
        return true;
    else {
        Serial.println("waiting for gsm module...");
        isConnected  = sendATcommand(AT, RESPONSE_OK, 2000);
        if(isConnected) {
            sendATcommand(AT_SMS_TEXT_MODE, RESPONSE_OK, 1000);
            sendATcommand(AT_NEW_SMS_INDICATION, RESPONSE_OK, 1000);
            sendATcommand(AT_ENABLE_NETWORK_TIME_UPDATE, RESPONSE_OK, 1000);

            Serial.println("connected to gsm network");
        }
    }
    clearSerialBuffer();

    return isConnected;
}
#if 1
void TinyPipe::sendParameter(char *tag, int val, char *mobileNumber)
{
    char charVal[20];
    char auxString[50];
    String str;
    bool ans = false;

    if(tag != NULL) {
        str = String(tag) + " " + String(val);
    }
    else {
        str = String(val);
    }

    str.toCharArray(charVal, 20);

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

    delay(1000);
    clearSerialBuffer();
}
#endif
int TinyPipe::checkNewSMS()
{
    int i = 0;
    int ret = 0;

    if(Serial.available() == 0) {
        return ret;
    }

    clearString(SMS, SMS_SIZE);

    while(Serial.available() > 0 && ret == 0) {
        SMS[i] = Serial.read();
        i++;

        if(strstr(SMS, "enable") != NULL) {
            ret = PANEL_ENABLE;        
        }
        else if(strstr(SMS, "disable") != NULL) {
            ret = PANEL_DISABLE;
        }
        else if(strstr(SMS, "read") != NULL) {
            ret = PARAM_READ;
        }
    }

    if(ret > 0) {
        Serial.println(SMS);
        delay(1000);
        clearSerialBuffer();
    }

    delay(1000);

    return ret;
}

String TinyPipe::getLocalTimestamp()
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

bool TinyPipe::sendATcommand(char *cmd, char *result, unsigned int timeout)
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
    previous = millis();

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

void TinyPipe::clearString(char *str, int size)
{
    memset(str, '\0', size); 
} 

void TinyPipe::clearSerialBuffer()
{
    while(Serial.available() > 0) {
        Serial.read();
    }
}
