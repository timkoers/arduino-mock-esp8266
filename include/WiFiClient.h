/*
  WiFiClient.h - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified by Ivan Grokhotkov, December 2014 - esp8266 support
*/

#ifndef wificlient_h
#define wificlient_h
#include <memory>
#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"
#include "include/slist.h"

#ifndef TCP_MSS
#define TCP_MSS 1460 // lwip1.4
#endif

#define WIFICLIENT_MAX_PACKET_SIZE TCP_MSS
#define WIFICLIENT_MAX_FLUSH_WAIT_MS 300

#define TCP_DEFAULT_KEEPALIVE_IDLE_SEC          7200 // 2 hours
#define TCP_DEFAULT_KEEPALIVE_INTERVAL_SEC      75   // 75 sec
#define TCP_DEFAULT_KEEPALIVE_COUNT             9    // fault after 9 failures

class ClientContext;
class WiFiServer;

class WiFiClient_ : public Client, public SList<WiFiClient> {
protected:
  WiFiClient_(ClientContext* client);

public:
  WiFiClient_();
  virtual ~WiFiClient_();
  WiFiClient_(const WiFiClient_&);
  WiFiClient_& operator=(const WiFiClient_&);

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port) override;
  virtual int connect(const char *host, uint16_t port) override;
  virtual int connect(const String& host, uint16_t port);
  virtual size_t write(uint8_t) override;
  virtual size_t write(const uint8_t *buf, size_t size) override;
  virtual size_t write_P(PGM_P buf, size_t size);
  size_t write(Stream& stream);

  // This one is deprecated, use write(Stream& instead)
  size_t write(Stream& stream, size_t unitSize) __attribute__ ((deprecated));

  virtual int available() override;
  virtual int read() override;
  virtual int read(uint8_t *buf, size_t size) override;
  virtual int peek() override;
  virtual size_t peekBytes(uint8_t *buffer, size_t length);
  size_t peekBytes(char *buffer, size_t length) {
    return peekBytes((uint8_t *) buffer, length);
  }
  virtual void flush() override { (void)flush(0); }
  virtual void stop() override { (void)stop(0); }
  bool flush(unsigned int maxWaitMs);
  bool stop(unsigned int maxWaitMs);
  virtual uint8_t connected() override;
  virtual operator bool() override;

  IPAddress remoteIP();
  uint16_t  remotePort();
  IPAddress localIP();
  uint16_t  localPort();

  static void setLocalPortStart(uint16_t port) { _localPort = port; }

  size_t availableForWrite();

  friend class WiFiServer;

  using Print::write;

  static void stopAll();
  static void stopAllExcept(WiFiClient * c);

  void     keepAlive (uint16_t idle_sec = TCP_DEFAULT_KEEPALIVE_IDLE_SEC, uint16_t intv_sec = TCP_DEFAULT_KEEPALIVE_INTERVAL_SEC, uint8_t count = TCP_DEFAULT_KEEPALIVE_COUNT);
  bool     isKeepAliveEnabled () const;
  uint16_t getKeepAliveIdle () const;
  uint16_t getKeepAliveInterval () const;
  uint8_t  getKeepAliveCount () const;
  void     disableKeepAlive () { keepAlive(0, 0, 0); }

  // default NoDelay=False (Nagle=True=!NoDelay)
  // Nagle is for shortly delaying outgoing data, to send less/bigger packets
  // Nagle should be disabled for telnet-like/interactive streams
  // Nagle is meaningless/ignored when Sync=true
  static void setDefaultNoDelay (bool noDelay);
  static bool getDefaultNoDelay ();
  bool getNoDelay() const;
  void setNoDelay(bool nodelay);

  // default Sync=false
  // When sync is true, all writes are automatically flushed.
  // This is slower but also does not allocate
  // temporary memory for sending data
  static void setDefaultSync (bool sync);
  static bool getDefaultSync ();
  bool getSync() const;
  void setSync(bool sync);

protected:

  static int8_t _s_connected(void* arg, void* tpcb, int8_t err);
  static void _s_err(void* arg, int8_t err);

  int8_t _connected(void* tpcb, int8_t err);
  void _err(int8_t err);

  ClientContext* _client;
  static uint16_t _localPort;
};

extern WiFiClient_ WiFiClient;

class WiFiClientMock{
	public:
	
		ON_CALL(*this, status())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::status));
		ON_CALL(*this, connect(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::connect));
		ON_CALL(*this, connect(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::connect));
		ON_CALL(*this, connect(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::connect));
		ON_CALL(*this, write(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::write));
		ON_CALL(*this, write(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::write));
		ON_CALL(*this, write_P(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::write_P));
		ON_CALL(*this, write(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::write));
		
		ON_CALL(*this, available())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::available));
		ON_CALL(*this, read())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::read));
		ON_CALL(*this, read(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::read));
		ON_CALL(*this, peek())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::peek));
		ON_CALL(*this, peekBytes(_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::peekBytes));
		ON_CALL(*this, flush())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::flush));
		ON_CALL(*this, stop())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::stop));
		ON_CALL(*this, flush(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::flush));
		ON_CALL(*this, stop(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::stop));
		ON_CALL(*this, connected())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::connected));
		
		ON_CALL(*this, remoteIP())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::remoteIP));
		ON_CALL(*this, remotePort())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::remotePort));
		ON_CALL(*this, localIP())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::localIP));
		ON_CALL(*this, localPort())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::localPort));
			
		ON_CALL(*this, setLocalPortStart(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::setLocalPortStart));
		
		ON_CALL(*this, availableForWrite())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::availableForWrite));
			
		ON_CALL(*this, stopAll())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::stopAll));
		ON_CALL(*this, stopAllExcept(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::stopAllExcept));
		
		ON_CALL(*this, keepAlive(_,_,_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::keepAlive));
		ON_CALL(*this, isKeepAliveEnabled())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::isKeepAliveEnabled));
		ON_CALL(*this, getKeepAliveIdle())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::getKeepAliveIdle));
		ON_CALL(*this, getKeepAliveInterval())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::getKeepAliveInterval));
		ON_CALL(*this, getKeepAliveCount())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::getKeepAliveCount));
		ON_CALL(*this, disableKeepAlive())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::disableKeepAlive));
			
		ON_CALL(*this, setDefaultNoDelay(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::setDefaultNoDelay));
		ON_CALL(*this, setDefaultNoDelay())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::setDefaultNoDelay));
		ON_CALL(*this, getNoDelay())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::getNoDelay));
		ON_CALL(*this, setNoDelay())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::setNoDelay));
		
		ON_CALL(*this, setDefaultSync(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::setDefaultSync));
		ON_CALL(*this, getDefaultSync())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::getDefaultSync));
		ON_CALL(*this, getSync())
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::getSync));
		ON_CALL(*this, setSync(_))
			.WillByDefault(Invoke(&realWiFiClient, &WiFiClient_::setSync));
		
		MOCK_METHOD0(status, uint8_t());
		MOCK_METHOD2(connect, int(IPAddress, uint16_t));
		MOCK_METHOD2(connect, int(const char *, uint16_t));
		MOCK_METHOD2(connect, int(const String&, uint16_t));
		MOCK_METHOD1(write, size_t(uint8_t));
		MOCK_METHOD2(write,size_t(const uint8_t *, size_t));
		MOCK_METHOD2(write_P, size_t(PGM_P, size_t));
		MOCK_METHOD1(write, size_t(Stream&));

		// This one is deprecated, use write(Stream& instead)
		MOCK_METHOD2(write, size_t(Stream&, size_t)) __attribute__ ((deprecated));

		MOCK_METHOD0(available, int());
		MOCK_METHOD0(read, int());
		MOCK_METHOD2(read, int(uint8_t *, size_t));
		MOCK_METHOD0(peek,int());
		MOCK_METHOD2(peekBytes, size_t(uint8_t *, size_t));
		MOCK_METHOD2(peekBytes, size_t(char *, size_t));
		MOCK_METHOD0(flush, void());
		MOCK_METHOD0(stop, void());
		MOCK_METHOD1(flush, bool(unsigned int));
		MOCK_METHOD1(stop, bool(unsigned int));
		MOCK_METHOD0(connected, uint8_t());		

		MOCK_METHOD0(remoteIP, IPAddress());
		MOCK_METHOD0(remotePort, uint16_t());
		MOCK_METHOD0(localIP, IPAddress());
		MOCK_METHOD0(localPort, uint16_t());
		
		MOCK_METHOD1(setLocalPortStart, void(uint16_t));

		MOCK_METHOD0(availableForWrite, size_t());

		MOCK_METHOD0(stopAll, void());
		MOCK_METHOD1(stopAllExcept, void(WiFiClient*));

		MOCK_CONST_METHOD3(keepAlive, void(uint16_t, uint16_t, uint8_t));
		MOCK_CONST_METHOD0(isKeepAliveEnabled ,bool());
		MOCK_CONST_METHOD0(getKeepAliveIdle, uint16_t());
		MOCK_CONST_METHOD0(getKeepAliveInterval, uint16_t ());
		MOCK_CONST_METHOD0(getKeepAliveCount ,uint8_t());
		MOCK_CONST_METHOD0(disableKeepAlive, void());

		// default NoDelay=False (Nagle=True=!NoDelay)
		// Nagle is for shortly delaying outgoing data, to send less/bigger packets
		// Nagle should be disabled for telnet-like/interactive streams
		// Nagle is meaningless/ignored when Sync=true
		MOCK_METHOD1(setDefaultNoDelay, void(bool));
		MOCK_METHOD0(getDefaultNoDelay, bool());
		MOCK_CONST_METHOD0(getNoDelay, bool());
		MOCK_METHOD1(setNoDelay, void(bool));

		// default Sync=false
		// When sync is true, all writes are automatically flushed.
		// This is slower but also does not allocate
		// temporary memory for sending data
		MOCK_METHOD1(setDefaultSync, void(bool));
		MOCK_METHOD0(getDefaultSync, bool());
		MOCK_CONST_METHOD0(getSync, bool());
		MOCK_METHOD1(setSync, void(bool));
		
	private:
		WiFiClient_ realWiFiClient;
#endif
