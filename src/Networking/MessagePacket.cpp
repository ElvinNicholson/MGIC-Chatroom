#include "MessagePacket.h"

sf::Packet& operator<<(sf::Packet& packet, const MessagePacket& messagePacket)
{
  return packet << messagePacket.message << messagePacket.name << messagePacket.room;
}

sf::Packet& operator>>(sf::Packet& packet, MessagePacket& messagePacket)
{
  return packet >> messagePacket.message >> messagePacket.name >> messagePacket.room;
}
