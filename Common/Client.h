#pragma once
#include <functional>
#include "Global.h"
#include "PacketType.h"
#include "Network.h"
#include "ClientPlayersManager.h"
#define CONNECTION_TIMEOUT 5000 // in milisec


class Client;
using PacketHandler = std::function<void(const PacketID &id, sf::Packet &packet, Client *)>; /* std::function store pointer to function, more info https://stackoverflow.com/questions/9054774/difference-between-stdfunction-and-a-standard-function-pointer   */


class ClientPlayersManager;
class Client
{
public:
	Client(ClientPlayersManager &clientPlayersManager);
	~Client();

	bool connect();
	bool disconnect();
	void listen();
	bool sendPacket(sf::Packet &packet );
	const sf::Time getTime() const;
	const sf::Time& getLastTimeHeartBeat() const;
	void setTime(const sf::Time &time);
	void setServer(const sf::IpAddress &ip, const PortNumber &portNumber);
	bool isConnected() const;

	template<class T>
	void setup(void(T::*handler)
		(const PacketID&, sf::Packet&, Client*), T* instance)
	{
		this->packetHandler = std::bind(handler, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	void setup(void(*l_handler)(const PacketID&, sf::Packet&, Client*));
	void unregisterPacketHandler();
	void update(const sf::Time& time);
	sf::Mutex* getMutex;

	void setPlayerName(std::string &playerName);
	
   bool sendCreatePlayerPacket();
   bool sendMovePlayerPacket(MoveDirection dir);

   ClientID getClientID();


   ClientPlayersManager &playersManager;
private:

	std::string playerName;

	bool connected;
	sf::UdpSocket udpSocket;
	sf::IpAddress serverIP;
	PortNumber portNumber;
	PacketHandler packetHandler;

	sf::Time serverTime;
	sf::Time lastHeartBeat;

	sf::Thread listenThread;
	sf::Mutex mutex;
   ClientID clientID;

};

