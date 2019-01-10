#pragma once
#include <functional>
#include "Global.h"
#include "PacketType.h"
#include "Network.h"
#include "ClientPlayersManager.h"
#define CONNECTION_TIMEOUT 5000 // in milisec
#define SYNCTIME_COUNT 5

class Client;
using PacketHandler = std::function<void(const PacketID &id, sf::Packet &packet, Client *)>; /* std::function store pointer to function, more info https://stackoverflow.com/questions/9054774/difference-between-stdfunction-and-a-standard-function-pointer   */


class Client
{
public:
	Client();
	~Client();

	bool connect();
	bool disconnect();
	void listen();
   void sendSyncTimeServer();
	bool sendPacket(sf::Packet &packet );
   const sf::Time getTime() const;
   const sf::Time getServerTime() const;
	const sf::Time& getLastTimeHeartBeat() const;
	void setTime(const sf::Time &time);
	void setServer(const sf::IpAddress &ip, const PortNumber &portNumber);
	bool isConnected() const;
   bool isSynced();

	template<class T>
	void setup(void(T::*handler)
		(const PacketID&, sf::Packet&, Client*), T* instance)
	{
		this->packetHandler = std::bind(handler, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	void setup(void(*l_handler)(const PacketID&, sf::Packet&, Client*));
	void unregisterPacketHandler();
	void update(const sf::Time& time);
	sf::Mutex& getMutex();

	void setPlayerName(std::string &playerName);
	
   bool sendCreatePlayerPacket();

   ClientID getClientID();


private:

	std::string playerName;

	bool connected;
	sf::UdpSocket udpSocket;
	sf::IpAddress serverIP;
	PortNumber portNumber;
	PacketHandler packetHandler;

   sf::Time localTime;
	sf::Time serverTime;
	sf::Time lastHeartBeat;

	sf::Thread listenThread;
	sf::Mutex mutex;
   ClientID clientID;

   std::vector<std::pair<sf::Time, sf::Time>> timeSyncPair;
   int syncTimeCount;
   bool isSyncCompleted;
};

