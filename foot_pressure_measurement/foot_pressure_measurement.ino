/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>


// Replace with your network credentials
 char* ssid = "Curiousity";
const char* password = "Physics121";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
int forcePin_right = 34;
int forceReading_right;
int forcePin_left = 35;
int forceReading_left;

String readRightPressureData() {
  forceReading_right = analogRead(forcePin_right);
  if (isnan(forceReading_right)) {    
    Serial.println("Failed to read from Pressure sensor!");
    return "";
  }
  else {
    Serial.println(forceReading_right);
    return String(forceReading_right);
  }
}

String readLeftPressureData() {
  forceReading_left = analogRead(forcePin_left);
  if (isnan(forceReading_left)) {    
    Serial.println("Failed to read from Pressure sensor!");
    return "";
  }
  else {
    Serial.println(forceReading_left);
    return String(forceReading_left);
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
    Serial.println(readRightPressureData());
    Serial.println(readLeftPressureData());
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
 
void loop(){
      File tempLog = SPIFFS.open("/data.csv", "a"); // Write the time and the temperature to the csv file
      tempLog.print(readRightPressureData());
      tempLog.print(',');
      tempLog.println(readLeftPressureData());
      tempLog.close();}
  
