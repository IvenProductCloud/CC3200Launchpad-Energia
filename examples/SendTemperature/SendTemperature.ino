#include <IvenCloud.h>
#include <Wire.h>
#include "Adafruit_TMP006.h"


IvenCloudWiFi device;
Adafruit_TMP006 tmp006(0x41);
bool isActive = false;
#define frequency 2000 // interval of sending data in milliseconds

/****************   FILL THIS SPECS    ************************/
/**************************************************************/

char ssid[] = "<your wifi ssid>";
char pass[] = "<your wifi password>";

const char* secretKey = "<secret key>";
const char* deviceUId = "<device id>";



void setup() {

  Serial.begin(9600);
  Serial.println("start");
  Serial.println("Connecting WiFi");
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED) {
    delay(300);
  }
  Serial.println("Connected");

  Serial.println("Loading temperature sensor");
  if (! tmp006.begin(TMP006_CFG_8SAMPLE)) {
    Serial.println("No sensor found");
    while (1);
  }
  Serial.println("Sensor loaded");
  Serial.println();
}

void loop() {

  if (!isActive) {
    Serial.println("Activating device");
    IvenResponse i = device.activateDevice(secretKey, deviceUId);
    if (i.httpStatus == 200) {
      isActive = true;
      Serial.println("Activated");
      Serial.println();
    }
    else {
      Serial.println("Couldn't activate");
      Serial.print("HTTP Status: ");
      Serial.println(i.httpStatus);
      Serial.print("Error code: ");
      Serial.println(i.error);
      Serial.println();
    }
  } else {
    Serial.println("Reading temperature");
    float temp = tmp006.readDieTempC();
    Serial.print("Temperature is: ");
    Serial.print(temp);
    Serial.println(" C");
    Serial.println();

    Serial.println("Sending data to Iven Cloud");
    IvenData data;
    data.add("num", temp);

    IvenResponse p = device.sendData(data);
    if (p.ivenCode == 1000) {
      Serial.println("Successfully sent to cloud");
      Serial.println();
    } else {
      Serial.println("Something wrong with the data check iven code to identify the problem");
      Serial.print("iven code: ");
      Serial.println(p.ivenCode);
      Serial.println();
    }

    delay(frequency);
  }
}