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

void StampPacket(const PacketType& l_type, sf::Packet& l_packet);

