#ifndef ESP8266WIFIAP_H_
#define ESP8266WIFIAP_H_

#include <gmock/gmock.h>
#include <stdint.h>

#include "ESP8266WiFiType.h"
#include "ESP8266WiFiGeneric.h"
#include "user_interface.h"

class ESP8266WiFiAPClass_ {
	public:
	  
		ESP8266WiFiAPClass_();
		  
		bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4);
        bool softAP(const String& ssid,const String& passphrase = emptyString,int channel = 1,int ssid_hidden = 0,int max_connection = 4);
        bool softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
        bool softAPdisconnect(bool wifioff = false);

        uint8_t softAPgetStationNum();

        IPAddress softAPIP();

        uint8_t* softAPmacAddress(uint8_t* mac);
        String softAPmacAddress(void);

		String softAPSSID() const;
		String softAPPSK() const;

};
extern ESP8266WiFiAPClass_ ESP8266WiFiAPClass;

class ESP8266WiFiAPClassMock {
  public:
  
	MOCK_METHOD5(softAP, bool(const char*, const char*, int, int, int));
	MOCK_METHOD5(softAP, bool(const String&,const String&, int, int, int));
	MOCK_METHOD3(softAPConfig, bool(IPAddress, IPAddress, IPAddress));
	MOCK_METHOD1(softAPdisconnect, bool(bool));

	MOCK_METHOD0(softAPgetStationNum, uint8_t());

	MOCK_METHOD0(softAPIP, IPAddress());

	MOCK_METHOD1(softAPmacAddress, uint8_t*(uint8_t*));
	MOCK_METHOD0(softAPmacAddress, String(void));

	MOCK_CONST_METHOD0(softAPSSID, String());
	MOCK_CONST_METHOD0(softAPPSK, String());
};

ESP8266WiFiAPClassMock* ESP8266WiFiAPMockMockInstance();
void releaseESP8266WiFiAPClassMock();

#endif /* ESP8266WIFIAP_H_ */