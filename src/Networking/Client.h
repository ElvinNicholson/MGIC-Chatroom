#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H

// Standard Library
#include <iostream>
#include <atomic>
#include <thread>

// External Library
#include <SFML/Network.hpp>

// Source code
#include "../UI/ClientUI.h"
#include "MessagePacket.h"

class Client
{
 public:
  void init();
  void update(float dt);
  void render(sf::RenderWindow &window);
  void eventHandler(sf::Event event);

  void connect();
  void input(sf::TcpSocket& iSocket) const;
  void run();

  std::atomic<bool> running = false;
  std::atomic<bool> connected = false;

 protected:

 private:
  std::unique_ptr<sf::TcpSocket> socket;

  std::unique_ptr<ClientUI> client_UI;
};

#endif // CHATROOM_CLIENT_H
