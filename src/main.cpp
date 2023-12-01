#include <WiFi.h>
#include <ctime>
#include "time.h"
const char *ssid = "Sv 2022";
const char *password = "@sv22022";
const char *serverIp = "192.168.1.83";
const int serverPort = 8080;
String volStr = "";
const char *ntpServer = "pool.ntp.org";
// Variable to save request epoch time
unsigned long epochTime;
int volt;

int delayTime = 10;

int countMax = 1000 / delayTime;
int count = 0;
// Function get time
unsigned long getTime();
const int pirPin = 34; // Replace with the actual pin your PIR sensor is connected to

bool checkForMotion();
void sendDataToServer();

void setup()
{
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  configTime(0, 0, ntpServer);

  pinMode(pirPin, INPUT);

  // Your PIR sensor logic goes here
}

void loop()
{
  // When motion is detected, send data to the server

  // Wait for 5 seconds to avoid sending data too frequently

  count++;

  volt = analogRead(pirPin);

  volStr += (count < countMax) ? String(volt) + '+' : String(volt);

  delay(delayTime);

  if (count == countMax)
  {
    sendDataToServer();
    epochTime = getTime();
    Serial.println("Detected!");
    // String dectected = "POST / HTTP/1.1 esp-id=0&vol=" + volStr + "&time=" + String(epochTime);
    // Serial.println(dectected);

    // reset count and volstr
    count = 0;
    volStr = "";
  }
}

unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}
bool checkForMotion()
{
  // Replace this with your PIR sensor logic
  // Return true if motion is detected, false otherwise
  return digitalRead(pirPin) == HIGH;
}

void sendDataToServer()
{
  WiFiClient client;

  // Connect to the server
  if (client.connect(serverIp, serverPort))
  {
    Serial.println("Connected to server");
    // Construct the data payload
    epochTime = getTime();
    volt = analogRead(pirPin);
    String postData = "POST / HTTP/1.1 esp-id=0&vol=" + volStr + "&time=" + String(epochTime);
    Serial.println(postData);
    // Serial.println("Sending data to server:\n" + postData);

    // Send the data to the server
    client.print(postData);

    // Wait for the response
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.println(line);
      }
    }

    // Close the connection
    client.stop();
  }
  else
  {
    Serial.println("Unable to connect to server");
  }
}
