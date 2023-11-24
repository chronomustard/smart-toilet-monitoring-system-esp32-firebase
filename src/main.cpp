#include "MQ135.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "time.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Ciki"
#define WIFI_PASSWORD "kiky123321"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCRcHHsibK-mdwIet-L-MNpXncMLykb2SA"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "rickypapudi@gmail.com"
#define USER_PASSWORD "monitoringudaraftui"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://monitoring-udara-ftui-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define MQ_PIN 4  //gas sensor
#define AO 34     //sound sensor

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String flushPath = "/flush";
String rangePath = "/range";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

unsigned int output;
int Decibels;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 1000*50;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void setup(){
  Serial.begin(115200);
  pinMode (MQ_PIN, INPUT);
  pinMode (AO, INPUT);
  delay(5000);

  initWiFi();
  configTime(0, 0, ntpServer);

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
}

void loop() {
  MQ135 gasSensor = MQ135(MQ_PIN);
  float air_quality = gasSensor.getPPM();
  Serial.print("Air Quality ");
  Serial.print(air_quality);
  Serial.println("  PPM");

  unsigned long start_time = millis(); 
  float PeakToPeak = 0;  
  unsigned int maximum_signal = 0;  //minimum value
  unsigned int minimum_signal = 4095;  //maximum value
  while (millis() - start_time < 50)
  {
    output = analogRead(AO);  
    if (output < 4095)  
    {
      if (output > maximum_signal)
      {
        maximum_signal = output;  
      }
      else if (output < minimum_signal)
      {
        minimum_signal = output; 
      }
    }
  }

  delay(100);

  PeakToPeak = maximum_signal - minimum_signal; 
  //Serial.println(PeakToPeak);
  Decibels = map(PeakToPeak, 50, 500, 49.5, 90);  
  Serial.println(Decibels);

  timestamp = getTime();
  Serial.print ("time: ");
  Serial.println (timestamp);

  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0 && humanpresence == 1)){
    sendDataPrevMillis = millis();

    // Prints the distance in the Serial Monitor
    parentPath= databasePath + "/" + String(timestamp);

    json.set(rangePath.c_str(), String(air_quality));
    json.set(flushPath.c_str(), String(Decibels));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}
