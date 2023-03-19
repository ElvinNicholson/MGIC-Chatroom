#ifndef CHATROOM_SERVER_H
#define CHATROOM_SERVER_H

// Standard Library
#include <thread>
#include <list>
#include <iostream>
#include <mutex>
#include <memory>

// External Library
#include <SFML/Network.hpp>

class Server
{
 public:
  void init();
  void run();
  void listen(sf::TcpSocket& cSocket);
  void send(sf::Packet packet, unsigned short sender);

 protected:

 private:
  std::vector<std::thread> workers;
  std::vector<std::unique_ptr<sf::TcpSocket>> connections;

  std::unique_ptr<sf::TcpListener> listener;
  std::unique_ptr<sf::TcpSocket> socket;

  std::mutex mutex;

  bool running = true;
};

#endif // CHATROOM_SERVER_H
