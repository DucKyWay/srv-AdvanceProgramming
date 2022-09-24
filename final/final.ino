#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define WIFI_SSID "DPle"
#define WIFI_PASSWORD "1234567890"
#define API_KEY "AIzaSyAfZXb0WgLrVN_Fs0ZnCR3NQlZK3DSIU-U"
#define DATABASE_URL "https://team-iot12-default-rtdb.firebaseio.com/"
#define USER_EMAIL "37278@srv.ac.th"
#define USER_PASSWORD "srv123456"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

int countWiFi = 0;

float t = 0.0;
float h = 0.0;

#define relaySW01 32
#define relaySW02 14
String stateRSW01 = "OFF";
String stateRSW02 = "OFF";

void setup() {
  Serial.begin(115200);
  pinMode(relaySW01, OUTPUT);
  pinMode(relaySW02, OUTPUT);
  digitalWrite(relaySW01, HIGH);
  digitalWrite(relaySW02, HIGH);
  
  Serial.println("Set[SHT31]");
  if (! sht31.begin(0x44)) {    // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
  Serial.println("Set [WIFI & Firebase]");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    if(countWiFi > 30) {
      Serial.println();
      countWiFi = 0;
    }
    Serial.print(".");
    delay(1000);
    countWiFi++;
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;

  Serial.println("Setup done!");
}

void loop() {
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // GET
    // Serial.printf("Get string[ctrSW1]... %s , ", Firebase.RTDB.getString(&fbdo, F("/name/ctrSW1")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
    // Serial.printf("Get string[ctrSW2]... %s\n", Firebase.RTDB.getString(&fbdo, F("/name/ctrSW2")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    if (Firebase.RTDB.getString(&fbdo, F("/name/ctrSW1"))) {
      if (fbdo.dataType() == "string") {
        String stringValue = fbdo.stringData();
        Serial.println("Get string[ctrSW1]... " + stringValue);

        if(stringValue == "ON") digitalWrite(relaySW01, LOW);
        else if(stringValue == "OFF") digitalWrite(relaySW01, HIGH);
      }
    }
    else {
      Serial.println("Get string[ctrSW1]... "  + fbdo.errorReason());
    }

    if (Firebase.RTDB.getString(&fbdo, F("/name/ctrSW2"))) {
      if (fbdo.dataType() == "string") {
        String stringValue = fbdo.stringData();
        Serial.println("Get string[ctrSW2]... " + stringValue);
        if(stringValue == "ON") digitalWrite(relaySW02, LOW);
        else if(stringValue == "OFF") digitalWrite(relaySW02, HIGH);
      }
    }
    else {
      Serial.println("Get string[ctrSW2]... "  + fbdo.errorReason());
    }
    
    t = sht31.readTemperature();
    h = sht31.readHumidity();
    if (isnan(t) || isnan(h)) {    // check if 'is not a number'
      Serial.println("Failed to read from SHT31 sensor!");
      return;
    } 

    
    if(digitalRead(relaySW01))  stateRSW01 = "OFF";
    else stateRSW01 = "ON";
    if(digitalRead(relaySW02))  stateRSW02 = "OFF";
    else stateRSW02 = "ON";
    
    Serial.print("Temp : "); Serial.print(t); Serial.print(" °C , ");
    Serial.print("Humd : "); Serial.print(h); Serial.print(" % , ");
    Serial.print("stateRSW01 : "); Serial.print(stateRSW01); Serial.print(" , ");
    Serial.print("stateRSW02 : "); Serial.println(stateRSW02);
    
    // SET
    Serial.printf("Set float[temp]... %s , ", Firebase.RTDB.setFloat(&fbdo, F("/name/temp"), t) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set float[humd]... %s\n", Firebase.RTDB.setFloat(&fbdo, F("/name/humd"), h) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set string[RealySW01]... %s , ", Firebase.RTDB.setString(&fbdo, F("/name/switch1"), stateRSW01) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set string[RealySW02]... %s\n", Firebase.RTDB.setString(&fbdo, F("/name/switch2"), stateRSW02) ? "ok" : fbdo.errorReason().c_str());
     
  }
}
