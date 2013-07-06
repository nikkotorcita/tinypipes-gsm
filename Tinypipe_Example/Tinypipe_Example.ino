#define MOBILE_NUMBER   "+639175878287"
#define GO

#include <tinypipesGSM.h>

int led = 13;
int res = 0;
int val = 0;
String ts;
String msg;

void setup()
{
    pinMode(led, OUTPUT);
    Tinypipe.serialAttach(115200);
    
    //blocks until gsm module becomes available on serial port
    while(!Tinypipe.available());
}

void loop()
{
  msg = Tinypipe.checkNewSMS(MOBILE_NUMBER);
  if(msg == "enable")
    digitalWrite(led, HIGH);
  else if(msg == "disable")
    digitalWrite(led, LOW);
  else if(msg == "read") {
    val = analogRead(A0);
    Tinypipe.sendParameter("voltage", val, MOBILE_NUMBER);  
  }
  else if(msg == "time") {
    ts = Tinypipe.getLocalTimestamp();
    Serial.println(ts); 
  }
}
