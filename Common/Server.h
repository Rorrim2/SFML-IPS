#pragma once
#include <SFML/Network.hpp>
#include <functional>
#include <unordered_map>

#include "Global.h"
#include "Network.h"
#include "PacketType.h"

#define HEARTBEAT_INVERVAL 1000
#define HEARTBEAT_TRIES 5

struct ClientInfo
{
	sf::IpAddress clientIP;
	PortNumber clientPort;
	sf::Time lastHeartbeat;
	sf::Time lastHeartbeatSend;
	bool heartbeatWaiting;
	uint16_t heartbeatTry;
	uint32_t latency;

	ClientInfo(const sf::IpAddress &ip, const PortNumber &port, const sf::Time &hearbeat)
		: clientIP(ip), clientPort(port), lastHeartbeat(hearbeat), heartbeatWaiting(false), heartbeatTry(0), latency(0) {}

};


class Server;
using Clients = std::unordered_map<ClientID, ClientInfo>;
using PacketHandler = std::function<void(sf::IpAddress &, const PortNumber &, const PacketID&, sf::Packet &packet, Server *)>;
using TimeoutHandler = std::function<void(const ClientID &)>;


class Server
{
public:
	template<class T >
	Server(void(T::*handler)(sf::IpAddress &, const PortNumber &, const PacketID &, sf::Packet &, Server *), T *instance)
		: listenThread(listen, this)
	{
		this->packetHandler = std::bind(handler, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	}

	Server(void(*handler)(sf::IpAddress &, const PortNumber &, const PacketID &, sf::Packet &, Server *));
	~Server();

	template<class T>
	void bindTimeoutHandler(void(T::*handler)(const ClientID &), T *instance)
	{
		this->timeoutHandler = std::bind(handler, instance, std::placeholders::_1);
	}
	void bindTimeoutHandler(void(*handler)(const ClientID &));

	bool send(const ClientID &id, sf::Packet &packet);
	bool send(const sf::IpAddress &ip, const PortNumber &port, sf::Packet &packet);
	void broadcast(sf::Packet &packet, const ClientID &ignore = static_cast<ClientID>(Network::NullID));


	void listen();
	void update(const sf::Time &time);

	ClientID addClient(const sf::IpAddress &ip, const PortNumber &port);
	ClientID getClientID(const sf::IpAddress &ip, const PortNumber &port);
	bool hasClient(ClientID &id);
	bool hasClient(const sf::IpAddress &ip, const PortNumber &port);
	bool getClientInfo(const ClientID &id, ClientInfo &info);
	bool removeClient(const ClientID &id);
	bool removeClient(const sf::IpAddress &id, const PortNumber &port);

	void disconnectAll();
	bool start();
	bool stop();
	bool isRunning();

	uint32_t getClientCount();
	std::string getClientList();

	sf::Mutex &getMutex();

private:
	void setup();

	ClientID lastID;

	sf::UdpSocket inUDPSocket;
	sf::UdpSocket outUDPSocket;

	PacketHandler packetHandler;
	TimeoutHandler timeoutHandler;

	Clients clients;

	sf::Time serverTime;

	bool running;

	sf::Thread listenThread;
	sf::Mutex mutex;

	size_t totalSend;
	size_t totalReceive;

};

