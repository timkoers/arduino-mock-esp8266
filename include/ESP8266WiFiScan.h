#ifndef ESP8266WIFISCAN_H_
#define ESP8266WIFISCAN_H_

#include <gmock/gmock.h>
#include <stdint.h>

#include "ESP8266WiFiType.h"
#include "ESP8266WiFiGeneric.h"
#include "user_interface.h"

class ESP8266WiFiScanClass_ {
	public:
	  
		ESP8266WiFiScanClass_();
		  
		int8_t scanNetworks(bool async = false, bool show_hidden = false, uint8 channel = 0, uint8* ssid = NULL);
        void scanNetworksAsync(std::function<void(int)> onComplete, bool show_hidden = false);

        int8_t scanComplete();
        void scanDelete();

        // scan result
        bool getNetworkInfo(uint8_t networkItem, String &ssid, uint8_t &encryptionType, int32_t &RSSI, uint8_t* &BSSID, int32_t &channel, bool &isHidden);

        String SSID(uint8_t networkItem);
        uint8_t encryptionType(uint8_t networkItem);
        int32_t RSSI(uint8_t networkItem);
        uint8_t * BSSID(uint8_t networkItem);
        String BSSIDstr(uint8_t networkItem);
        int32_t channel(uint8_t networkItem);
        bool isHidden(uint8_t networkItem);

	protected:
        static bool _scanAsync;
        static bool _scanStarted;
        static bool _scanComplete;

        static size_t _scanCount;
        static void* _scanResult;

        static std::function<void(int)> _onComplete;

        static void _scanDone(void* result, int status);
        static void * _getScanInfoByIndex(int i);
};
extern ESP8266WiFiScanClass_ ESP8266WiFiScanClass;

class ESP8266WiFiScanClassMock {
  public:
  
	MOCK_METHOD4(scanNetworks, int8_t(bool, bool, uint8, uint8));
	MOCK_METHOD2(scanNetworksAsync, void(std::function<void(int)>, bool));

	MOCK_METHOD0(scanComplete, void());
	MOCK_METHOD0(scanDelete, void());

	// scan result
	MOCK_METHOD7(getNetworkInfo, bool(uint8_t, String&, uint8_t&, int32_t&, uint8_t* &, int32_t &, bool &));

	MOCK_METHOD1(SSID, String(uint8_t));
	MOCK_METHOD1(encryptionType, uint8_t(uint8_t));
	MOCK_METHOD1(RSSI, int32_t(uint8_t));
	MOCK_METHOD1(BSSID, uint8_t *(uint8_t));
	MOCK_METHOD1(BSSIDstr, String (uint8_t));
	MOCK_METHOD1(channel, int32_t (uint8_t));
	MOCK_METHOD1(isHidden, bool(uint8_t));
	
	// ....
};

ESP8266WiFiScanClassMock* ESP8266WiFiScanMockMockInstance();
void releaseESP8266WiFiScanClassMock();

#endif /* ESP8266WIFISCAN_H_ */