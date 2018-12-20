#include "Server.h"

Server::Server(void(*handler)(sf::IpAddress &, const PortNumber &, const PacketID &, sf::Packet &, Server *))
	: listenThread(&Server::listen, this)
{
	this->packetHandler = std::bind(handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

Server::~Server()
{
	stop();
}

void Server::bindTimeoutHandler(void(*handler)(const ClientID &))
{
	this->timeoutHandler = std::bind(handler, std::placeholders::_1);
}

bool Server::send(const ClientID & id, sf::Packet & packet)
{
	sf::Lock lock(this->mutex);
	auto iter = this->clients.find(id);
	if (iter != this->clients.end())
	{
		return send(iter->second.clientIP, iter->second.clientPort, packet);
	}
	return false;
}

bool Server::send(const sf::IpAddress & ip, const PortNumber & port, sf::Packet & packet)
{
	if (this->outUDPSocket.send(packet, ip, port) == sf::Socket::Status::Done)
	{
		this->totalSend += packet.getDataSize();
		return true;
	}
	else
	{
		DEBUG_COUT("Failed to sending packet...");
		return false;
	}
}

void Server::broadcast(sf::Packet & packet, const ClientID & ignore)
{
	sf::Lock lock(this->mutex);

	for (auto & iter : this->clients)
	{
		if (iter.first != ignore)
		{
			if (this->outUDPSocket.send(packet, iter.second.clientIP, iter.second.clientPort) != sf::Socket::Status::Done)
			{
				DEBUG_COUT("Failed sending broadcast packet to client: " << iter.first);
			}
			else
			{
				this->totalSend += packet.getDataSize();
			}
		}
	}
}

void Server::listen()
{
	sf::IpAddress ip;
	PortNumber port;
	sf::Packet packet;

	while (isRunning())
	{
		packet.clear();


		sf::Socket::Status status = this->inUDPSocket.receive(packet, ip, port);
		if (status != sf::Socket::Status::Done)
		{
			if (isRunning())
			{
				DEBUG_COUT("Error receiving a packet from " << ip << ":" << port);
				continue;
			}
         else
         {
            DEBUG_COUT("Socket unbound");
            break;
         }
		}

		this->totalReceive += packet.getDataSize();

		PacketID id;
		//propert packet?
#ifndef UNIX
      packet >> id;
#else
		if ((packet >> id) == false) continue;
#endif // UNIX


		PacketType packetType = static_cast<PacketType>(id);
		if (packetType < PacketType::Disconnect || packetType > PacketType::OutOfBounds) continue;

		if (packetType == PacketType::Heartbeat)
		{
			sf::Lock lock(this->mutex);
			for (auto & iter : this->clients)
			{
				if (iter.second.clientIP != ip || iter.second.clientPort != port) continue;

				if (iter.second.heartbeatWaiting == false)
				{
               DEBUG_COUT("Wrong heartbeat packet receive!");
					break;
				}

				iter.second.latency = this->serverTime.asMilliseconds() - iter.second.lastHeartbeatSend.asMilliseconds();
				iter.second.lastHeartbeat = this->serverTime;
				iter.second.heartbeatWaiting = false;
				iter.second.heartbeatTry = 0;

				break;
			}
		}
      else if (packetType == PacketType::SyncTime)
      {
         sf::Int32 clientTime;
         packet >> clientTime;
         sf::Packet p;
         StampPacket(PacketType::SyncTime, p);
         p << clientTime << this->serverTime.asMilliseconds();
         send(ip, port, p);
      }
		else if(packetHandler)
		{
			this->packetHandler(ip, port, id, packet, this);
		}
	}
}

void Server::update(const sf::Time & time)
{
	this->serverTime += time;
	//this statement is only for keep variable above 0 if it go signed
	if (this->serverTime.asMilliseconds() < 0)
	{
		this->serverTime -= sf::milliseconds(static_cast<sf::Int32>(Network::HighestTimestamp));
		
		sf::Lock lock(this->mutex);
		for (auto & iter : this->clients)
		{
			iter.second.lastHeartbeat = sf::milliseconds(std::abs(iter.second.lastHeartbeat.asMilliseconds() - static_cast<sf::Int32>(Network::HighestTimestamp)));
		}
	}

	sf::Lock lock(this->mutex);
	for (auto iter = this->clients.begin(); iter != this->clients.end(); )
	{
		uint32_t elapsed = this->serverTime.asMilliseconds() - iter->second.lastHeartbeat.asMilliseconds();
		if (elapsed >= HEARTBEAT_INVERVAL)
		{
			//check if client is alive
			if (elapsed >= static_cast<int>(Network::ClientTimeout) || iter->second.heartbeatTry >= HEARTBEAT_TRIES)
			{
				DEBUG_COUT("Client has timed out");

				if (this->timeoutHandler != nullptr) this->timeoutHandler(iter->first);

				iter = this->clients.erase(iter);
				continue;
			}
		}


		if (iter->second.heartbeatWaiting == false || (elapsed >= HEARTBEAT_INVERVAL * (iter->second.heartbeatTry + 1)))
		{
			//hearbeat
			//its show for only two last heartbeats
			if (iter->second.heartbeatTry >= 3)
			{
				DEBUG_COUT("Re-try(" << iter->second.heartbeatTry << ") heartbeat for client " << iter->first);
			}
			sf::Packet heartbeatPack;
			StampPacket(PacketType::Heartbeat, heartbeatPack);
			heartbeatPack << this->serverTime.asMilliseconds();
			send(iter->first, heartbeatPack);
			if (iter->second.heartbeatTry == 0)
			{
				iter->second.lastHeartbeatSend = this->serverTime;
			}
			iter->second.heartbeatWaiting = true;
			++iter->second.heartbeatTry;

			this->totalSend += heartbeatPack.getDataSize();
		}
		++iter;
	}
}

ClientID Server::addClient(const sf::IpAddress & ip, const PortNumber & port)
{
	ClientID id = static_cast<ClientID>(Network::NullID);
	sf::Lock lock(this->mutex);
	for (auto & iter : this->clients)
	{
		if (iter.second.clientIP == ip && iter.second.clientPort == port) return id;
	}
	id = this->lastID;
	ClientInfo info(ip, port, this->serverTime);
	clients.insert(std::make_pair(id, info));
	this->lastID++;
   return id;
}

ClientID Server::getClientID(const sf::IpAddress & ip, const PortNumber & port)
{
	ClientID id = static_cast<ClientID>(Network::NullID);
	for (auto & iter : this->clients)
	{
		if (iter.second.clientIP == ip && iter.second.clientPort == port)
		{
			id = iter.first;
			break;
		}
	}
	return id;
}

bool Server::hasClient(ClientID & id)
{
	return (this->clients.find(id) != this->clients.end());
}

bool Server::hasClient(const sf::IpAddress & ip, const PortNumber & port)
{
	return (getClientID(ip, port) >= 0);
}

bool Server::getClientInfo(const ClientID & id, ClientInfo & info)
{
	sf::Lock lock(this->mutex);
	bool valid = false;

	for (auto & iter : this->clients)
	{
		if (iter.first == id)
		{
			info = iter.second;
			valid = true;
		}
	}

	return valid;
}

bool Server::removeClient(const ClientID & id)
{
	sf::Lock lock(mutex);
	bool valid = false;
	auto iter = this->clients.find(id);
	
	if (iter != this->clients.end())
	{
		sf::Packet packet;
		StampPacket(PacketType::Disconnect, packet);
		send(id, packet);
		this->clients.erase(id);
		valid = true;
	}
	return valid;
}

bool Server::removeClient(const sf::IpAddress & ip, const PortNumber & port)
{
	sf::Lock lock(mutex);
	bool valid = false;

	for (auto iter = this->clients.begin(); iter != this->clients.end(); ++iter)
	{
		if (iter->second.clientIP == ip && iter->second.clientPort == port)
		{
			sf::Packet packet;
			StampPacket(PacketType::Disconnect, packet);
			send(iter->first, packet);
			this->clients.erase(iter);
			valid = true;
			break;
		}
	}
	return valid;
}

void Server::disconnectAll()
{
   DEBUG_COUT("Disconnect all clients!");
	if (isRunning() == true)
	{
		sf::Packet packet;
		StampPacket(PacketType::Disconnect, packet);
		
		broadcast(packet);

		sf::Lock lock(this->mutex);
		this->clients.clear();
	}
}

bool Server::start()
{
	bool valid = false;
	if (!isRunning())
	{
		if (this->inUDPSocket.bind(static_cast<int>(Network::ServerPort)) == sf::Socket::Status::Done)
		{
			this->outUDPSocket.bind(sf::Socket::AnyPort);
			setup();

			DEBUG_COUT("Incoming port is " << this->inUDPSocket.getLocalPort() << "  Outcoming port is " << this->outUDPSocket.getLocalPort());

			this->running = true;
			this->listenThread.launch();
			valid = true;
		}
	}
	return valid;
}

bool Server::stop()
{
   DEBUG_COUT("Server stopped!");
	bool valid = false;
	if (isRunning() == true)
	{
		disconnectAll();
		this->running = false;
		this->inUDPSocket.unbind();
		valid = true;
	}
	return valid;
}

bool Server::isRunning()
{
	return this->running;
}

size_t Server::getClientCount()
{
	return this->clients.size();
}

std::string Server::getClientList()
{
	return std::string();
}

sf::Mutex & Server::getMutex()
{
	return this->mutex;
}

const sf::Time Server::getTime()
{
   return this->serverTime;
}

void Server::setup()
{
	this->running = false;
	this->lastID = 0;
	this->totalReceive = 0;
	this->totalSend = 0;
}

