#pragma once
#include <SFML/Network.hpp>
#include "Network.h"

using PacketID = uint8_t;
enum class PacketType
{
	Disconnect = -1,
	Connect = 1,
   PlayerUpdate,
   PlayerCreate,
   PlayerMove,
	Heartbeat,
	Snapshot,
   Message,
	Hurt,
	OutOfBounds
};


enum MoveDirection
{
   FORWARD  = 1,
   BACKWARD = 2,
   LEFT     = 4,
   RIGHT    = 8
};

//Function to marking packet to recognise with type it is 
void StampPacket(const PacketType& type, sf::Packet& packet);
void StampPacketPlayerMove(sf::Packet& packet, const ClientID & id, MoveDirection dir);
void StampPacketPlayerUpdate(sf::Packet & packet, const ClientID & id, const float & x, const float & y);
void StampPacketPlayerCreate(sf::Packet& packet, const ClientID & id, const float &x, const float &y);

