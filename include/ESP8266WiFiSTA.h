#ifndef ESP8266WIFISTA_H_
#define ESP8266WIFISTA_H_

#include <gmock/gmock.h>
#include <stdint.h>

#include "ESP8266WiFiType.h"
#include "ESP8266WiFiGeneric.h"
#include "user_interface.h"

class ESP8266WiFiSTAClass_ {
	public:
	  
		ESP8266WiFiSTAClass_();
		  
		wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
        wl_status_t begin(char* ssid, char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
        wl_status_t begin(const String& ssid, const String& passphrase = emptyString, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
        wl_status_t begin();

        //The argument order for ESP is not the same as for Arduino. However, there is compatibility code under the hood
        //to detect Arduino arg order, and handle it correctly. Be aware that the Arduino default value handling doesn't
        //work here (see Arduino docs for gway/subnet defaults). In other words: at least 3 args must always be given.
        bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000);

        bool reconnect();
        bool disconnect(bool wifioff = false);

        bool isConnected();

        bool setAutoConnect(bool autoConnect);
        bool getAutoConnect();

        bool setAutoReconnect(bool autoReconnect);
        bool getAutoReconnect();

        int8_t waitForConnectResult(unsigned long timeoutLength = 60000);

        // STA network info
        IPAddress localIP();

        uint8_t * macAddress(uint8_t* mac);
        String macAddress();

        IPAddress subnetMask();
        IPAddress gatewayIP();
        IPAddress dnsIP(uint8_t dns_no = 0);

        String hostname();
        bool hostname(const String& aHostname) { return hostname(aHostname.c_str()); }
        bool hostname(const char* aHostname);

        // STA WiFi info
        wl_status_t status();
        String SSID() const;
        String psk() const;

        uint8_t * BSSID();
        String BSSIDstr();

        int32_t RSSI();

        static void enableInsecureWEP (bool enable = true) { _useInsecureWEP = enable; }

		bool beginWPSConfig(void);
        bool beginSmartConfig();
        bool stopSmartConfig();
        bool smartConfigDone();

	protected:
        static void _smartConfigCallback(uint32_t status, void* result);
};
extern ESP8266WiFiSTAClass_ ESP8266WiFiSTAClass;

class ESP8266WiFiSTAClassMock {
  public:
	// Public methods	
	MOCK_METHOD2(onEvent, void(WiFiEventCb, WiFiEvent_t)) __attribute__((deprecated));
	
	// WiFi Events
	MOCK_METHOD5(begin, wl_status_t(const char*, const char *, int32_t, const uint8_t*, bool);
	MOCK_METHOD5(begin, wl_status_t(char*, char *, int32_t, const uint8_t*, bool);
	MOCK_METHOD5(begin, wl_status_t(const String&, const String&, int32_t, const uint8_t*, bool);
	
	MOCK_METHOD5(config, bool(IPAddress, IPAddress, IPAddress, IPAddress, IPAddress));
	MOCK_METHOD0(reconnect, bool());
	MOCK_METHOD1(disconnect, bool(bool));
	
	MOCK_METHOD0(isConnected, bool());
	
	MOCK_METHOD1(setAutoConnect, bool(bool));
	MOCK_METHOD0(getAutoConnect, bool());
	
	MOCK_METHOD1(setAutoReconnect, bool(bool));
	MOCK_METHOD0(getAutoReconnect, bool());
	
	MOCK_METHOD1(waitForConnectResult, int8_t(unsigned long));
	
	MOCK_METHOD1(localIP, IPAddress());
	
	MOCK_METHOD1(macAddress, uint8_t*(uint8_t*));
    MOCK_METHOD0(macAddress, String());
	
	MOCK_METHOD0(subnetMask, IPAddress());
	MOCK_METHOD0(gatewayIP, IPAddress());
	MOCK_METHOD1(dnsIP, IPAddress(uint8_t));
	
	MOCK_METHOD0(hostname, String());
    MOCK_METHOD1(hostname, bool(const char*));
	
	// STA WiFi info
	MOCK_METHOD0(status, wl_status_t());
	MOCK_CONST_METHOD0(SSID, String());
	MOCK_CONST_METHOD0(psk, String());
	
	MOCK_METHOD0(BSSID, uint8_t*());
	MOCK_METHOD0(BSSIDstr, String());
	
	MOCK_METHOD0(RSSI, int32_t());
	
	MOCK_METHOD1(enableInsecureWEP, void(bool enable));
	
	MOCK_METHOD0(beginWPSConfig, bool(void));
	MOCK_METHOD0(beginSmartConfig, bool());
	MOCK_METHOD0(stopSmartConfig, bool());
	MOCK_METHOD0(smartConfigDone, bool());
	
	// ....
};

ESP8266WiFiSTAClassMock* ESP8266WiFiSTAMockMockInstance();
void releaseESP8266WiFiSTAClassMock();

#endif /* ESP8266WIFISTA_H_ */