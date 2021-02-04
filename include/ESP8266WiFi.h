// Header for Wifi Mock

#ifndef WiFi_h
#define WiFi_h

#include <gmock/gmock.h>

extern "C" {
#include "include/wl_definitions.h"
}

#include "IPAddress.h"

#include "ESP8266WiFiType.h"
#include "ESP8266WiFiSTA.h"
#include "ESP8266WiFiAP.h"
#include "ESP8266WiFiScan.h"
#include "ESP8266WiFiGeneric.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "WiFiServerSecure.h"
#include "WiFiClientSecure.h"
#include "BearSSLHelpers.h"
#include "CertStoreBearSSL.h"

class ESP8266WiFiClass_ : public ESP8266WiFiGenericClass, public ESP8266WiFiSTAClass, public ESP8266WiFiScanClass, public ESP8266WiFiAPClass {
	public:
		// workaround same function name with different signature
		using ESP8266WiFiGenericClass::channel;

		using ESP8266WiFiSTAClass::SSID;
		using ESP8266WiFiSTAClass::RSSI;
		using ESP8266WiFiSTAClass::BSSID;
		using ESP8266WiFiSTAClass::BSSIDstr;

		using ESP8266WiFiScanClass::SSID;
		using ESP8266WiFiScanClass::encryptionType;
		using ESP8266WiFiScanClass::RSSI;
		using ESP8266WiFiScanClass::BSSID;
		using ESP8266WiFiScanClass::BSSIDstr;
		using ESP8266WiFiScanClass::channel;
		using ESP8266WiFiScanClass::isHidden;
	
	public:
		void printDiag(Print& dest);
		
		friend class WiFiClient;
        friend class WiFiServer;
};
extern ESP8266WiFiClass_ WiFi;

class ESP8266WiFiClassMock {
  public:
    MOCK_METHOD1(printDiag, void(Print&));
};

ESP8266WiFiClassMock* WiFiMockInstance();
void releaseWiFiMock();

#endif
