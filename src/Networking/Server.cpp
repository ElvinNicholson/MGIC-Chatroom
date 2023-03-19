#include "Server.h"

void Server::init()
{
  if (listener == nullptr)
  {
    listener = std::make_unique<sf::TcpListener>();
  }

  if (listener->listen(53000) != sf::Socket::Done)
  {
    std::cout << "Error" << std::endl;
  }
}

void Server::run()
{
  while (running)
  {
    sf::TcpSocket& cSock = connections.emplace_back(std::make_unique<sf::TcpSocket>()).operator*();

    if (listener->accept(cSock) != sf::Socket::Done)
    {
      /// Remove invalid socket from connections vector
      connections.pop_back();
      return;
    }

    std::cout << "Client connected @ " << cSock.getRemotePort() << std::endl;

    workers.emplace_back([&] {
                           listen(cSock);
                         });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

}

void Server::listen(sf::TcpSocket& cSocket)
{
  bool continue_receiving = true;

  sf::Packet packet;

  while (continue_receiving)
  {
    auto status = cSocket.receive(packet);

    /// Handle disconnects
    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false;
      std::cout << "Disconnected" << std::endl;

      std::lock_guard<std::mutex> lock(mutex);

      for (int i = 0; i < connections.size(); i++)
      {
        if (connections[i]->getLocalPort() == cSocket.getLocalPort())
        {
          continue;
        }

        connections.erase(std::next(connections.begin(), i));
        break;
      }
    }

    /// Sending data to all connections
    send(packet, cSocket.getRemotePort());
  }

  cSocket.disconnect();
}

void Server::send(sf::Packet packet, unsigned short sender)
{
  std::lock_guard<std::mutex> lock(mutex);

  for (auto& connection : connections)
  {
    /// Don't send message back to sender
    if (connection->getRemotePort() != sender)
    {
      connection->send(packet);
    }
  }
}
