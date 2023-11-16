#include <WiFi.h>

const char* ssid = "Sv 2022";
const char* password = "@sv22022";
const char *serverIp = "127.0.0.1";
const int serverPort = 8080;

const int pirPin = 13; // Replace with the actual pin your PIR sensor is connected to
bool checkForMotion();
void sendDataToServer();

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(pirPin, INPUT);

  // Your PIR sensor logic goes here
}

void loop() {
  // When motion is detected, send data to the server
  if (checkForMotion()) {
    sendDataToServer();
    delay(5000); // Wait for 5 seconds to avoid sending data too frequently
  }
}

bool checkForMotion() {
  // Replace this with your PIR sensor logic
  // Return true if motion is detected, false otherwise
  return digitalRead(pirPin) == HIGH;
}

void sendDataToServer() {
  WiFiClient client;

  // Connect to the server
  if (client.connect(serverIp, serverPort)) {
    Serial.println("Connected to server");

    // Construct the data payload
    String postReq = "POST / HTTP/1.1\n\nesp-id=0&vol=5&time=123456";

    Serial.println("Sending data to server:\n");

    // Send the data to the server
    client.print(postReq);
   
   

    // Wait for the response
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.println(line);
      }
    }

    // Close the connection
    client.stop();
  } else {
    Serial.println("Unable to connect to server");
  }
}
