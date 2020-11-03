#include <SFE_BMP180.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define TriggerPin A0
#define EchoPin A1

SFE_BMP180 Pressure;

SoftwareSerial ESPSerial (4, 5);

double SonarDuration;
double SonarDistance;
double SonarInstallHeight;

double PressureNormal;

String StartLine = "_";
String EndLine = ";";


void setup() 
{
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  
  Serial.begin(9600);
  ESPSerial.begin(9600);

  Pressure.begin();

  GetPressureNormal();
  GetSonarInstallHeight();

  delay(1000);
}

void loop() 
{
  SendDistanceData(SonarCheck());
  SendPressureData(PressureCheck());
}

void SendDistanceData(double func)
{
  Serial.println(StartLine + "D" + func + EndLine);
  ESPSerial.println(StartLine + "D" + func + EndLine);
}

void SendPressureData(double func)
{
  Serial.println(StartLine + "P" + func + EndLine);
  ESPSerial.println(StartLine + "P" + func + EndLine);
}

double SonarCheck()
{
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  SonarDuration = pulseIn(EchoPin, HIGH);
  SonarDistance = SonarInstallHeight - (SonarDuration / 2) / 2.91;

  if (SonarDistance < 0)
  {
    SonarDistance = 0;
  }

  return (SonarDistance);
}

double PressureCheck()
{
  double CurrentPressure;
  double DeltaPressure;
  
  CurrentPressure = getPressure();
  DeltaPressure = CurrentPressure - PressureNormal;
  
  if (DeltaPressure < 0)
  {
    DeltaPressure = 0;
  }

  return (DeltaPressure);
}

double getPressure()
{
  char status;
  double T, P, p0, a;

  status = Pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = Pressure.getTemperature(T);
    if (status != 0)
    {
      status = Pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = Pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
      }
    }
  }
}

void GetPressureNormal()
{
  PressureNormal = getPressure();
}

void GetSonarInstallHeight()
{
  digitalWrite(TriggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  SonarDuration = pulseIn(EchoPin, HIGH);
  SonarInstallHeight = (SonarDuration / 2) / 2.91;
}
