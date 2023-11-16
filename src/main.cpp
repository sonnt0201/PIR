#include <WiFi.h>

const char* ssid = "Sv 2022";
const char* password = "@sv22022";
const char *serverIp = "localhost";
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
    String postData = "device id:0\nvoltage:5\ntime:" + String(millis());

    Serial.println("Sending data to server:\n" + postData);

    // Send the data to the server
    client.print("POST / HTTP/1.1\r\n");
    client.print("Host: " + String(serverIp) + "\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: " + String(postData.length()) + "\r\n");
    client.print("\r\n");
    client.print(postData);

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
