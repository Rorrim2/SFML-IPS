#include "PacketType.h"


void StampPacket(const PacketType & type, sf::Packet & packet)
{
	packet << PacketID(type);
}

void StampPacketPlayerMove(sf::Packet & packet, const ClientID & id, MoveDirection dir)
{
   packet << PacketID(PacketType::PlayerMove) << id << dir;
}
void StampPacketPlayerUpdate(sf::Packet & packet, const ClientID & id, const float & x, const float & y)
{
   packet << PacketID(PacketType::PlayerUpdate) << id << x << y;
}

void StampPacketPlayerCreate(sf::Packet & packet, const ClientID & id, const float & x, const float & y)
{
   packet << PacketID(PacketType::PlayerCreate) << id << x << y;
}
