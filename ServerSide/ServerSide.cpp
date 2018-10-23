#include "Server.h"
#include "Window.h"

void handler(sf::IpAddress &ip, const PortNumber &port, const PacketID &packetID, sf::Packet &packet, Server *server)
{
	ClientID id = server->getClientID(ip, port);

	if (id != static_cast<ClientID>(Network::NullID))
	{
		if (static_cast<PacketType>(packetID) == PacketType::Disconnect)
		{
			server->removeClient(id);

			sf::Packet newPacket;
			StampPacket(PacketType::Message, newPacket);
			newPacket << "Client left!";
			server->broadcast(newPacket);
		}
		else if (static_cast<PacketType>(packetID) == PacketType::Message)
		{
			std::string receiveMsg;
			packet >> receiveMsg;

			std::string msg = ip.toString() + ": " + std::to_string(port) + " :" + receiveMsg;
			DEBUG_COUT(msg);
			sf::Packet newPacket;
			StampPacket(PacketType::Message, newPacket);
			newPacket << msg;

			server->broadcast(newPacket, id);
		}
	}
	else if (static_cast<PacketType>(packetID) == PacketType::Connect)
		{
			ClientID id = server->addClient(ip, port);
			sf::Packet newPacket;
			StampPacket(PacketType::Connect, newPacket);
			server->send(id, newPacket);
		}
	
}

void commandHandler(Server *server)
{
	while (server->isRunning())
	{
		std::string str;
		std::getline(std::cin, str);
		if (str == "quit")
		{
			server->stop();
			break;
		}
		else if (str == "dc")
		{
			server->disconnectAll();
		}

	}
}

int main()
{
  
	Server server(handler);
	if (server.start() == true)
	{
		sf::Thread c(&commandHandler, &server);
		c.launch();
		sf::Clock clock;
		clock.restart();
		while (server.isRunning())
		{
			server.update(clock.restart());
		}
		std::cout << "Stopping server..." << std::endl;
	}
}