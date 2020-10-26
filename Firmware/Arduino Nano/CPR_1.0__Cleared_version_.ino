#include <SFE_BMP180.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define TriggerPin A0
#define EchoPin A1

SFE_BMP180 Pressure;

SoftwareSerial ESPSerial (4, 5);

double SonarDuration;
double SonarDistance;
double SonarDistanceTraveled;
double SonarSuccessMax = 60;
int SonarDistancePercent;
double SonarInstallHeight;

double PressureMin;
double PressureMax;
double PressureNormal;
double MaxPressure = 0;
double PressureSuccesMin = 12.0;
double PressureSuccessMax = 15.0;
int PressurePercent;

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
  SonarDistance = (SonarDuration / 2) / 2.91;
  SonarDistanceTraveled = SonarInstallHeight - SonarDistance;

  if (SonarDistanceTraveled < 0)
  {
    SonarDistanceTraveled = 0;
  }

  SonarDistancePercent = SonarDistanceTraveled / SonarSuccessMax * 100;

  if (SonarDistancePercent > 100)
  {
    SonarDistancePercent = 100;
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

  PressurePercent = DeltaPressure / PressureSuccessMax * 100;

  if (PressurePercent > 100)
  {
    PressurePercent = 100;
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
    // ожидание замера температуры
    delay(status);
    status = Pressure.getTemperature(T);
    if (status != 0)
    {
      status = Pressure.startPressure(3);
      if (status != 0)
      {
        // ожидание замера давления
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
