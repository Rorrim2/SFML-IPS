#include "PacketType.h"


void StampPacket(const PacketType & type, sf::Packet & packet)
{
	packet << PacketID(type);
}
