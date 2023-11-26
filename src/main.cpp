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
#define WIFI_SSID "Redmi 10"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDGkMzKa2MkGnN-UmyIoXhowNT9OD0HE6s"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "rickypapudi@gmail.com"
#define USER_PASSWORD "etikakamarmandi"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://etika-kamar-mandi-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int trigPin = 19;
const int echoPin = 18;

const int sigPiezo = 34;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;

int flagFlush;

int countPeople;
int countFlush;

int countPeople_data;
int countFlush_data;

int peopleDetect;
int resetSend;
int piezoval;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String flushPath = "/flush";
String peoplePath = "/people";
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
unsigned long timerDelay = 1000*30;

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

  //Ultrasonic Settings
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(sigPiezo, INPUT); // Sets piezo as an Input
  delay(500);

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
  //Ultrasonic reading

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  piezoval = analogRead(sigPiezo);
  delay(500);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  Serial.print("Piezo (value): ");
  Serial.println(piezoval);

  if (distanceCm < 60){
    resetSend = 0;
    peopleDetect = 1;

    Serial.print("\n--- PERSON DETECTED ---");
    
    if (piezoval > 30){
      flagFlush = 1;
      Serial.print("\n--- FLUSH DETECTED ---");
    }
  }

  else {
    resetSend = 1;
    Serial.print("\n--- NO PERSON ---");
    if(peopleDetect == 1){
      countPeople = countPeople_data + 1;
      if(flagFlush == 1){
        countFlush = countFlush_data + 1;
      }
    }
    peopleDetect = 0;
    flagFlush = 0;
  }

  timestamp = getTime();
  Serial.print ("time: ");
  Serial.println (timestamp);

  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0) && resetSend == 1){
    sendDataPrevMillis = millis();
    
    resetSend = 0;

    // Prints the distance in the Serial Monitor
    parentPath= databasePath + "/" + String(timestamp);

    json.set(peoplePath.c_str(), String(countPeople));
    json.set(flushPath.c_str(), String(countFlush));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}
