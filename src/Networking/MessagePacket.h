#ifndef CHATROOM_MESSAGEPACKET_H
#define CHATROOM_MESSAGEPACKET_H

#include <SFML/Network.hpp>

class MessagePacket
{
 public:
  std::string message;
  std::string name;
  int room;

  friend sf::Packet& operator <<(sf::Packet& packet, const MessagePacket& messagePacket);
  friend sf::Packet& operator >>(sf::Packet& packet, MessagePacket& messagePacket);

 protected:

 private:
};

#endif // CHATROOM_MESSAGEPACKET_H
