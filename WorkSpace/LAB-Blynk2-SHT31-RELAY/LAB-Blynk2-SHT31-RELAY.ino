/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLfJBQzM10"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "n1WJmcj4i6AnhiIssoxGnWTnHGEZ1AKI"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include "Adafruit_SHT31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

#define relayPin01 32

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iPadNimit";
char pass[] = "1234567890";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  Serial.println("V0:[" + String(value) + "]");
  
  if(value == 1) { // if(value)
    digitalWrite(relayPin01, LOW);   // Turn ON, 1
  }
  else {
    digitalWrite(relayPin01, HIGH);  // Turn OFF, 0
  }

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  //Blynk.virtualWrite(V2, millis() / 1000);

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (isnan(t) || isnan(h)) {  // check if 'is not a number'
    Serial.println("Failed to read temperature");
  } 
  else { 
    String sht = "Temp : " + String(t) + " °C, Humd : "+ String(h) + " %";
    Serial.println(sht);
    Blynk.virtualWrite(V4, t);
    Blynk.virtualWrite(V5, h);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  
  pinMode(relayPin01, OUTPUT);
  digitalWrite(relayPin01, HIGH);

  if (! sht31.begin(0x44)) {    // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  // Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
