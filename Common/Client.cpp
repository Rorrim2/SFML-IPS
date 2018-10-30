#include "Client.h"



Client::Client(ClientPlayersManager &clientPlayersManager)
	: listenThread(&Client::listen, this), playersManager(clientPlayersManager)
{
	this->connected = false;
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
     // std::cout << recPort << std::endl;

		//check socket status
     // std::cout << recIp << "  " << status << std::endl;
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
		else
		{
			this->packetHandler(id, packet, this);
		}
	}
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
	return sf::Time();
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


void Client::setup(void(*l_handler)(const PacketID &, sf::Packet &, Client *))
{
	this->packetHandler = std::bind(l_handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void Client::unregisterPacketHandler()
{
	this->packetHandler = nullptr;
}

void Client::update(const sf::Time &time)
{
	if (this->connected == true)
	{
		this->serverTime += time;
		//this statement is only for keep variable above 0 if it go signed
		if (this->serverTime.asMilliseconds() < 0)
		{
			this->serverTime -= sf::milliseconds(sf::Int32(Network::HighestTimestamp));
			this->lastHeartBeat = this->serverTime;
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
      p << this->clientID << 200.0f << 40.0f;
      //StampPacketPlayerCreate(p, this->clientID, 200, 40);
      if (this->udpSocket.send(p, this->serverIP, this->portNumber) == sf::Socket::Status::Done)
      {
         valid = true;
      }
   }

   return valid;
}
bool Client::sendMovePlayerPacket(MoveDirection dir)
{
   bool valid = false;
   if (this->connected == true)
   {
      sf::Packet p;
      //StampPacketPlayerMove(p, this->clientID, dir);
      /*if (this->udpSocket.send(p, this->serverIP, this->portNumber) == sf::Socket::Status::Done)
      {
         valid = true;
      }*/
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

