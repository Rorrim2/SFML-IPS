#pragma once
#include <SFML/Network.hpp>

using PacketID = uint8_t;
enum class PacketType
{
	Disconnect = -1,
	Connect = 1,
	Heartbeat,
	Snapshot,
	PlayerUpdate,
	Message,
	Hurt,
	OutOfBounds
};

//Function to marking packet to recognise with type it is 
void StampPacket(const PacketType& type, sf::Packet& packet);

