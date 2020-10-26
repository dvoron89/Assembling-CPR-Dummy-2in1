#include <ESP8266WiFi.h>

char* SSID = "CPR_Dummy_01";
char* PASS = "CPR_Pass_01";
int Port = 8888;

IPAddress IP(192, 168, 88, 88);
IPAddress Gateway(192, 168, 4, 9);
IPAddress Subnet(255, 255, 255, 0);

WiFiServer WIFIServer(Port);
WiFiClient WIFIClient;

void setup() 
{
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASS);
  WiFi.softAPConfig(IP, Gateway, Subnet);

  WIFIServer.begin();
}

void loop() 
{
  ProcessData();
}

void ProcessData()
{
  while (Serial.available() > 0)
  {
    String Line = Serial.readStringUntil('\n');
    ProcessLine(Line);
  }
}

void ProcessLine(String line)
{
  WIFISend(line);
}

void WIFISend(String line)
{
  if (!WIFIClient.connected())
  {
    WIFIClient = WIFIServer.available();
  }
  WIFIClient.println(line);
}
