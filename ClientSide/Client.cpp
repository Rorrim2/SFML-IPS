#include "Client.h"



Client::Client()
	: listenThread(&Client::listen, this), syncTimeCount(0), connected(false), isSyncCompleted(false)
{
   this->localTime = sf::Time::Zero;
}

bool Client::connect()
{
	//checking connection, maybe we are connected 
	if (this->connected == true) return false;

	this->udpSocket.bind(sf::Socket::AnyPort);
	
   std::cout << this->udpSocket.getLocalPort() << std::endl;

	sf::Packet packet;
	StampPacket(PacketType::Connect, packet);
	packet << this->playerName;

	if (this->udpSocket.send(packet, this->serverIP, this->portNumber) != sf::Socket::Status::Done)
	{
		this->udpSocket.unbind();
		return false;
	}

	this->udpSocket.setBlocking(false);
	packet.clear();

	sf::IpAddress recIp;
	PortNumber recPort;
	sf::Clock timer;
	bool validConnection = false;

	timer.restart();
	while (timer.getElapsedTime().asMilliseconds() < CONNECTION_TIMEOUT && validConnection == false)
	{
		sf::Socket::Status status = this->udpSocket.receive(packet, recIp, recPort);
      //std::cout << recPort << std::endl;

		//check socket status
      //std::cout << recIp << "  " << status << std::endl;
		if (status != sf::Socket::Done) continue;
		if (recIp != this->serverIP) continue;

		//after checking socket status, check packet property
		PacketID id;
		if ((packet >> id >> this->clientID) == false) continue;
		if (static_cast<PacketType>(id) != PacketType::Connect) continue;

		//connection ok
		this->packetHandler(id, packet, this);
		this->connected = true;
		this->udpSocket.setBlocking(true);	
		this->lastHeartBeat = serverTime;
		this->listenThread.launch();
		validConnection = true;
	}

	if (validConnection == false)
	{
		this->udpSocket.unbind();
		this->udpSocket.setBlocking(true);

		DEBUG_COUT("Connection failed!");
	}
   sendSyncTimeServer();
	return validConnection;
}

bool Client::disconnect()
{
	if (this->connected == false) return false;

	sf::Packet packet;
	StampPacket(PacketType::Disconnect, packet);

	bool valid = sendPacket(packet);
	this->connected = false;
	this->udpSocket.unbind(); // Unbind to close the listening thread.

	return valid;
}
//This function will be execute in thread to listen all message sent to this class
void Client::listen()
{
	sf::Packet packet;
	sf::IpAddress recIP;
	PortNumber recPort;

	while (this->connected == true)
	{
		packet.clear();

		sf::Socket::Status status = this->udpSocket.receive(packet, recIP, recPort);

		if (status != sf::Socket::Status::Done)
		{
			if (this->connected == true)
			{
				DEBUG_COUT("Failed receiving a packet from " << recIP << ":" << recPort << ". Status: " << status);
            continue;
			}
			else
			{
				DEBUG_COUT("Socket unbound");
            break;
			}
		}

		//ignore packat from another server
		if (recIP != this->serverIP) continue;
		
		PacketID id;
		//propert packet?
		if ((packet >> id) == false) continue;
		PacketType packetType = static_cast<PacketType>(id);
      
		if (packetType < PacketType::Disconnect || packetType > PacketType::OutOfBounds) continue;

		if (packetType == PacketType::Heartbeat)
		{
			sf::Packet tempPacket;
			StampPacket(PacketType::Heartbeat, tempPacket);
			if(sendPacket(tempPacket) == false)
			{
				DEBUG_COUT("Failed sending heartbeat!");
			}
			sf::Int32 timeStamp;
			packet >> timeStamp;
			setTime(sf::milliseconds(timeStamp));
			this->lastHeartBeat = this->serverTime;
		}
      else if (packetType == PacketType::SyncTime)
      {

         sf::Time sT;
         sf::Int32 ser, clientSend;
         packet >> clientSend >> ser;
         sT = sf::milliseconds(ser);
         syncPair.push_back(getTime().asMilliseconds() - clientSend);
         std::cout << "SyncTime  " << ser << "   " << clientSend << "  " << getTime().asMilliseconds() << "st " << sT.asSeconds()<< std::endl;

         if (this->syncTimeCount >= SYNCTIME_COUNT)
         {
            std::sort(syncPair.begin(), syncPair.end());
            this->localTime = sT + sf::microseconds(syncPair[syncPair.size() / 2]);
            this->isSyncCompleted = true;
            this->syncTimeCount = 0;
            std::cout << "Latency " << syncPair[syncPair.size() / 2]  << " Server time " << this->localTime.asSeconds() << std::endl;
         }
         else
         {
            syncTimeCount++;
            sendSyncTimeServer();
         }
      }
		else
		{
			this->packetHandler(id, packet, this);
		}
	}
}

void Client::sendSyncTimeServer()
{
   sf::Packet p;
   StampPacket(PacketType::SyncTime, p);
   p << this->localTime.asMilliseconds();
   sendPacket(p);
}

bool Client::sendPacket(sf::Packet & packet)
{
	bool valid = false;
	if (this->connected == true)
	{
		if (this->udpSocket.send(packet, this->serverIP, this->portNumber) == sf::Socket::Status::Done)
		{
			valid = true;
		}
	}

	return valid;
}

const sf::Time Client::getTime() const
{
   return this->localTime;
}

const sf::Time Client::getServerTime() const
{
   return this->serverTime;
}

const sf::Time & Client::getLastTimeHeartBeat() const
{
	return this->lastHeartBeat;
}

void Client::setTime(const sf::Time & time)
{
	this->serverTime = time;
}

void Client::setServer(const sf::IpAddress & ip, const PortNumber & portNumber)
{
	this->serverIP = ip;
	this->portNumber = portNumber;
}

bool Client::isConnected() const
{
	return this->connected;
}

bool Client::isSynced()
{
   return this->isSyncCompleted;
}


void Client::setup(void(*l_handler)(const PacketID &, sf::Packet &, Client *))
{
	this->packetHandler = std::bind(l_handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void Client::unregisterPacketHandler()
{
	this->packetHandler = nullptr;
}
///TODO correct
void Client::update(const sf::Time &time)
{
	if (this->connected == true)
	{
	   this->localTime += time;

		this->serverTime += time;
		//this statement is only for keep variable above 0 if it go signed
		if (this->serverTime.asMilliseconds() < 0)
		{
			this->serverTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
			this->lastHeartBeat = this->serverTime;
			return;
		}
      //this statement is only for keep variable above 0 if it go signed
      if (this->localTime.asMilliseconds() < 0)
      {
         this->localTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
         this->lastHeartBeat = this->localTime;
         return;
      }
		int diff = this->serverTime.asMilliseconds() - this->lastHeartBeat.asMilliseconds();
		if (diff >= static_cast<int>(Network::ClientTimeout))
		{
			//timeout
			DEBUG_COUT("Server connection time out!");
			disconnect();
		}
	}
}

sf::Mutex & Client::getMutex()
{
   return this->mutex;
}
 
void Client::setPlayerName(std::string &playerName)
{
	this->playerName = playerName;
}

bool Client::sendCreatePlayerPacket()
{
   bool valid = false;
   if (this->connected == true)
   {
      sf::Packet p;
      StampPacket(PacketType::PlayerCreate, p);
      p << this->clientID;
      if (this->udpSocket.send(p, this->serverIP, this->portNumber) == sf::Socket::Status::Done)
      {
         valid = true;
      }
   }

   return valid;
}

ClientID Client::getClientID()
{
   return this->clientID;
}
Client::~Client()
{
	udpSocket.unbind();
}

