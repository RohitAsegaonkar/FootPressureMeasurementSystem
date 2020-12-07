/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <FS.h>
#endif
#include <Wire.h>


// Replace with your network credentials
 char* ssid = "Rohan B4U";
const char* password = "Malhar@123";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
int forcePin_right = A0;
int forceReading_right;

String readRightPressureData() {
  forceReading_right = analogRead(forcePin_right);
  if (isnan(forceReading_right)) {    
    Serial.println("Failed to read from Pressure sensor!");
    return "";
  }
  else {
    Serial.println(forceReading);
    return String(forceReading);
  }
}

String readLeftPressureData() {
  forceReading = analogRead(forcePin);
  if (isnan(forceReading)) {    
    Serial.println("Failed to read from Pressure sensor!");
    return "";
  }
  else {
    Serial.println(forceReading);
    return String(forceReading);
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    Serial.println(readPressureData());
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/rightPressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readRightPressureData().c_str());
  });
  server.on("/leftPressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readLeftPressureData().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){}
  
