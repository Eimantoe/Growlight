
#include <DS3231.h>

DS3231  rtc(SDA, SCL);

const short YEAR   = 2018,
            MONTH  = 9,
            DAY    = 24,
            HOUR   = 9,
            MINUTE = 0,
            SECOND = 0;

Time Tmain, Tadd;

const short LEDPIN        = 9,
            NEEDSWATERPIN = 10,
            NOWATERPIN    = 12,
            INTERRUPTPIN  = 2;
            
const short MAXBYTEVALUE = 255;
const long int WATERINGPERIOD = 172800;


short int daysToAdd;

long int  currentTimeInSeconds,
          timeToWaterInSeconds;

void addWateringHours()
{
  timeToWaterInSeconds = currentTimeInSeconds + WATERINGPERIOD;
}

void setup() {
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);
  pinMode(NEEDSWATERPIN, OUTPUT);
  pinMode(NOWATERPIN, OUTPUT);
  
  pinMode(INTERRUPTPIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), blink, FALLING);
  
  rtc.begin();
  rtc.setTime(HOUR, MINUTE, SECOND); 
  rtc.setDate(DAY, MONTH, YEAR);
  
  Tmain                = rtc.getTime();
  currentTimeInSeconds = rtc.getUnixTime(Tmain);
  
  timeToWaterInSeconds = 0;
}

void checkIfNeedsWater()
{
  if (currentTimeInSeconds >= timeToWaterInSeconds)
  {
    analogWrite(NEEDSWATERPIN, MAXBYTEVALUE);
    analogWrite(NOWATERPIN, 0);
  }
  else
  {
    analogWrite(NEEDSWATERPIN, 0);
    analogWrite(NOWATERPIN, MAXBYTEVALUE);
  }
  
  Tmain                = rtc.getTime();
  currentTimeInSeconds = rtc.getUnixTime(Tmain);
}

void loop() {
  
  Tmain                = rtc.getTime();  
  currentTimeInSeconds = rtc.getUnixTime(Tmain);
  
  checkIfNeedsWater(); 
    
  int hours = Tmain.hour;
  int mins  = Tmain.min;
  
  //Aušra
  if(hours == 7)
  {
    analogWrite(LEDPIN, mins * 4);
  }
  //Dienos periodas
  else if(hours >= 8 && hours < 20)
  {
    analogWrite(LEDPIN, 255);
  }
  //Saulėlydis
  else if(hours == 20)
  {
    analogWrite(LEDPIN, (60-mins) * 4);
  }
  
  Tmain                = rtc.getTime(); 
  currentTimeInSeconds = rtc.getUnixTime(Tmain);
}

void blink()
{
    addWateringHours();
}

