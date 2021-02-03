#ifndef ESP8266WIFIGENERIC_H_
#define ESP8266WIFIGENERIC_H_

#include <gmock/gmock.h>
#include <stdint.h>

#include "ESP8266WiFiType.h"
#include <functional>
#include <memory>

#ifdef DEBUG_ESP_WIFI
#ifdef DEBUG_ESP_PORT
#define DEBUG_WIFI_GENERIC(fmt, ...) DEBUG_ESP_PORT.printf_P( (PGM_P)PSTR(fmt), ##__VA_ARGS__ )
#endif
#endif

#ifndef DEBUG_WIFI_GENERIC
#define DEBUG_WIFI_GENERIC(...) do { (void)0; } while (0)
#endif

struct WiFiEventHandlerOpaque;
typedef std::shared_ptr<WiFiEventHandlerOpaque> WiFiEventHandler;

typedef void (*WiFiEventCb)(WiFiEvent_t);

enum class DNSResolveType: uint8_t
{
    DNS_AddrType_IPv4 = 0,	// LWIP_DNS_ADDRTYPE_IPV4 = 0
    DNS_AddrType_IPv6,		// LWIP_DNS_ADDRTYPE_IPV6 = 1
    DNS_AddrType_IPv4_IPv6,	// LWIP_DNS_ADDRTYPE_IPV4_IPV6 = 2
    DNS_AddrType_IPv6_IPv4	// LWIP_DNS_ADDRTYPE_IPV6_IPV4 = 3
};

struct WiFiState;

class ESP8266WiFiGenericClass_ {
	public:
	  
		ESP8266WiFiGenericClass_();
		  
		// Note: this function is deprecated. Use one of the functions below instead.
		void onEvent(WiFiEventCb cb, WiFiEvent_t event = WIFI_EVENT_ANY) __attribute__((deprecated));

		// Subscribe to specific event and get event information as an argument to the callback
		WiFiEventHandler onStationModeConnected(std::function<void(const WiFiEventStationModeConnected&)>);
		WiFiEventHandler onStationModeDisconnected(std::function<void(const WiFiEventStationModeDisconnected&)>);
		WiFiEventHandler onStationModeAuthModeChanged(std::function<void(const WiFiEventStationModeAuthModeChanged&)>);
		WiFiEventHandler onStationModeGotIP(std::function<void(const WiFiEventStationModeGotIP&)>);
		WiFiEventHandler onStationModeDHCPTimeout(std::function<void(void)>);
		WiFiEventHandler onSoftAPModeStationConnected(std::function<void(const WiFiEventSoftAPModeStationConnected&)>);
		WiFiEventHandler onSoftAPModeStationDisconnected(std::function<void(const WiFiEventSoftAPModeStationDisconnected&)>);
		WiFiEventHandler onSoftAPModeProbeRequestReceived(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived&)>);
		WiFiEventHandler onWiFiModeChange(std::function<void(const WiFiEventModeChange&)>);

		int32_t channel(void);

		bool setSleepMode(WiFiSleepType_t type, uint8_t listenInterval = 0);

		WiFiSleepType_t getSleepMode();
		uint8_t getListenInterval ();
		bool isSleepLevelMax ();

		bool setPhyMode(WiFiPhyMode_t mode);
		WiFiPhyMode_t getPhyMode();

		void setOutputPower(float dBm);

		void persistent(bool persistent);

		bool mode(WiFiMode_t, WiFiState* state = nullptr);
		WiFiMode_t getMode();

		bool enableSTA(bool enable);
		bool enableAP(bool enable);

		bool forceSleepBegin(uint32 sleepUs = 0);
		bool forceSleepWake();

		static uint32_t shutdownCRC (const WiFiState* state);
		static bool shutdownValidCRC (const WiFiState* state);
		static void preinitWiFiOff (); //meant to be called in user-defined preinit()

	protected:
        static bool _persistent;
        static WiFiMode_t _forceSleepLastMode;

        static void _eventCallback(void *event);

        // called by WiFi.mode(SHUTDOWN/RESTORE, state)
        // - sleepUs is WiFi.forceSleepBegin() parameter, 0 = forever
        // - saveState is the user's state to hold configuration on restore
        bool shutdown (uint32 sleepUs = 0, WiFiState* stateSave = nullptr);
        bool resumeFromShutdown (WiFiState* savedState = nullptr);

        // ----------------------------------------------------------------------------------------------
        // ------------------------------------ Generic Network function --------------------------------
        // ----------------------------------------------------------------------------------------------

	public:
        int hostByName(const char* aHostname, IPAddress& aResult);
        int hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms);
#if LWIP_IPV4 && LWIP_IPV6
        int hostByName(const char* aHostname, IPAddress& aResult, uint32_t timeout_ms, DNSResolveType resolveType);
#endif
        bool getPersistent();
			
    protected:
        friend class ESP8266WiFiSTAClass;
        friend class ESP8266WiFiScanClass;
        friend class ESP8266WiFiAPClass;
};
extern ESP8266WiFiGenericClass_ ESP8266WiFiGenericClass;

class ESP8266WiFiGenericClassMock {
  public:
	// Public methods	
	MOCK_METHOD2(onEvent, void(WiFiEventCb, WiFiEvent_t)) __attribute__((deprecated));
	
	// WiFi Events
	MOCK_METHOD1(onStationModeConnected, WiFiEventHandler(std::function<void(const WiFiEventStationModeConnected&)>));
	MOCK_METHOD1(onStationModeDisconnected, WiFiEventHandler(std::function<void(const WiFiEventStationModeDisconnected&)>));
	MOCK_METHOD1(onStationModeAuthModeChanged, WiFiEventHandler(std::function<void(const WiFiEventStationModeAuthModeChanged&)>));
	MOCK_METHOD1(onStationModeGotIP, WiFiEventHandler(std::function<void(const WiFiEventStationModeGotIP&)>));
	MOCK_METHOD1(onStationModeDHCPTimeout, WiFiEventHandler(std::function<void(void)>));
	MOCK_METHOD1(onSoftAPModeStationConnected, WiFiEventHandler(std::function<void(const WiFiEventSoftAPModeStationConnected&)>));
	MOCK_METHOD1(onStationModeConnected, WiFiEventHandler(std::function<void(const WiFiEventStationModeConnected&)>));
	MOCK_METHOD1(onSoftAPModeStationDisconnected, WiFiEventHandler(std::function<void(const WiFiEventSoftAPModeStationDisconnected&)>));
	MOCK_METHOD1(onSoftAPModeProbeRequestReceived, WiFiEventHandler(std::function<void(const WiFiEventSoftAPModeProbeRequestReceived&)>));
	MOCK_METHOD1(onWiFiModeChange, WiFiEventHandler(std::function<void(const WiFiEventModeChange&)>));
	
    MOCK_METHOD0(channel, int());
    MOCK_METHOD2(setSleepMode, bool(WiFiSleepType_t, uint8_t));
    MOCK_METHOD0(getSleepMode, WiFiSleepType_t());
    MOCK_METHOD0(getListenInterval, uint8_t());
    MOCK_METHOD0(isSleepLevelMax, bool());
    MOCK_METHOD1(setPhyMode, bool(WiFiPhyMode_t));
    MOCK_METHOD0(getPhyMode, WiFiPhyMode_t());
    MOCK_METHOD1(setOutputPower, void(float));
    MOCK_METHOD0(persistent, void(bool));
    MOCK_METHOD2(mode, bool(WiFiMode_t, WiFiState));
    MOCK_METHOD0(getMode, WiFiMode_t());
    MOCK_METHOD1(enableSTA, bool(bool));
    MOCK_METHOD1(enableAP, bool(bool));
    MOCK_METHOD1(forceSleepBegin, bool(uint32));
    MOCK_METHOD1(forceSleepWake, bool());
    MOCK_METHOD1(shutdownCRC, uint32_t(const WiFiState*));
    MOCK_METHOD1(shutdownValidCRC, bool(const WiFiState*));
    MOCK_METHOD0(preinitWiFiOff, void());
	
	// Protected method's
	MOCK_METHOD2(shutdown, bool(uint32, WiFiState*));
	MOCK_METHOD1(resumeFromShutdown, bool(WiFiState*));
	
	// Public methods
	MOCK_METHOD2(hostByName, int(const char*, IPAddress&));
	MOCK_METHOD3(hostByName, int(const char*, IPAddress&, uint32_t));    
#if LWIP_IPV4 && LWIP_IPV6
	MOCK_METHOD4(hostByName, int(const char*, IPAddress&, uint32_t, DNSResolveType));
#endif
	MOCK_METHOD0(getPersistent, bool());	
};

ESP8266WiFiGenericClassMock* ESP8266WiFiGenericClassMockMockInstance();
void releaseESP8266WiFiGenericClassMock();

#endif /* ESP8266WIFIGENERIC_H_ */