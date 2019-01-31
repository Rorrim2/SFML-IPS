#pragma once
#include <SFML/Network.hpp>
#include "Network.h"

using PacketID = int8_t;
enum class PacketType
{
	Disconnect = -1,
	Connect = 1,
   PlayerUpdate,
   CannoBallUpdate,
   PlayerCreate,
   PlayerMove,
	Heartbeat,
	Snapshot,
   Message,
	Hurt,
   SyncTime,
	OutOfBounds
};


enum MoveDirection
{
   FORWARD     = 1,
   BACKWARD    = 2,
   LEFT        = 4,
   RIGHT       = 8,
   SHOOT_LEFT  = 16,
   SHOOT_RIGHT = 32,
   NONE        = 64
};

//Function to marking packet to recognise with type it is 
void StampPacket(const PacketType& type, sf::Packet& packet);
void StampPacketPlayerMove(sf::Packet& packet, const ClientID & id, MoveDirection dir);
void StampPacketPlayerUpdate(sf::Packet & packet, const ClientID & id, const float & x, const float & y);
void StampPacketPlayerCreate(sf::Packet& packet, const ClientID & id, const float &x, const float &y);

