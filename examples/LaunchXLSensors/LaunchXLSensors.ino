#include <IvenCloud.h>
#include <Wire.h>
#include "Adafruit_TMP006.h"
#include <BMA222.h>


IvenCloudWiFi device;
Adafruit_TMP006 tmp006(0x41);
BMA222 acm;
bool isActive = false;
/* put your setup code here, to run once:
   SSID and PASS to connect WiFi */
char ssid[] = "<your wifi ssid>";
char pass[] = "<your wifi password>";

const char* secretKey = "<secret key>";
const char* deviceUId = "<device id>";

#define frequency 2000 // interval of sending data in milliseconds

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
  Serial.println("Temperature sensor loaded");

  Serial.print("Loading acceloremeter sensor: ");
  acm.begin();
  uint8_t chipID = acm.chipID();
  Serial.print("Acceloremeter sensor loaded: ");
  Serial.println(chipID);
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

    Serial.println("Reading accelerometer");
    int acmValue = acm.readXData();
    Serial.print("Acm X-axis: ");
    Serial.print(acmValue);
    Serial.println();

    Serial.println("Sending data to Iven Cloud");
    IvenData data;
    data.add("< HW Profile Key for temperature >", temp);
    data.add("< HW Profile Key for accelerometer >", acmValue);
    
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