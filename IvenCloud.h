// Created by Rıza Arda Kırmızıoğlu and Berk Özdilek

/*
IvenCloudWiFi is an Arduino library to connect Arduino boards to Iven Product Cloud by using
Arduino WiFi shield. Checkout the examples folder to see examples.
Further examples and guides can be found in iven blog : http://blog.iven.io
*/

#ifndef PROJECT_IVENCLOUDWiFi_H
#define PROJECT_IVENCLOUDWiFi_H

#include <Arduino.h>
#include <WiFi.h>
#include "ShaClass.h"
#include "IvenData.h"
#include "IvenResponse.h"

#define server "demo.iven.io"
#define port 80

//#define IVEN_DEBUG

class IvenCloudWiFi { 
public:
		// Creates IvenCloudWiFi object
		IvenCloudWiFi();

		// Activates device, saves API-KEY into object and returns IvenResponse object for observation.
		IvenResponse activateDevice(const char* secretKey, const char* deviceId);

		// Posts data to Iven Product Cloud by using API-KEY and returns IvenResponse object for observation.
		IvenResponse sendData(IvenData& sensorData);

		// Send do task event
		IvenResponse doTask(int taskCode);

private:
		IvenResponse response;
		String _apiKey;
		WiFiClient _client;
		char buffer[128];
		bool _check;

private:
		bool handleResponseHeader();
		bool parseApiKey();
		bool handleResponseBody();
		void createActivationCode(const char* secretKey, const char* deviceId, char* activationCode);
		void sendDataRequest(IvenData* data);
		void activationRequest(char* activationCode);
		bool ConnectClient();
};

#endif //PROJECT_IVENCLOUDWiFi_H
