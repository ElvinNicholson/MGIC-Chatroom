#include "Client.h"

void Client::init()
{
  client_UI = std::make_unique<ClientUI>();
}

void Client::update(float dt)
{
  client_UI->update(dt);
}

void Client::render(sf::RenderWindow& window)
{
  client_UI->render(window);
}

void Client::eventHandler(sf::Event event)
{
  client_UI->eventHandler(event);
}

void Client::connect()
{
  socket = std::make_unique<sf::TcpSocket>();

  if(socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "Connected" << std::endl;
    connected = true;
  }
  else
  {
    std::cout << "Failed to connect" << std::endl;
  }
}

void Client::run()
{
  running = true;

  std::thread input_thread ([&] {
                             input(*socket);
                           });
  input_thread.detach();

  while (running && connected)
  {
    sf::Packet packet;
    while (connected)
    {

      auto status = socket -> receive(packet);

      if (status == sf::Socket::Status::Disconnected)
      {
        connected = false;
        std::cout << "Clean Disconnection" << std::endl;
        socket->disconnect();
        break;
      }
      else
      {
        /// Receiving Messages
        MessagePacket messagePacket;

        if (packet >> messagePacket)
        {
          if (messagePacket.room > client_UI->getNumberOfRooms() - 1)
          {
            client_UI->createNewChannel();
          }

          client_UI->addToHistory(messagePacket);

          if (messagePacket.room == client_UI->room)
          {
            client_UI->displayNewText(messagePacket.name, sf::Color(117, 226, 255), messagePacket.message);
          }
        }
      }
    }
  }
}

void Client::input(sf::TcpSocket& iSocket) const
{
  while (running)
  {
    if (connected && client_UI->sending)
    {
      /// Set client's own name
      if (client_UI->getName().empty())
      {
        client_UI->nameClient(client_UI->getInput());
      }
      else
      {
        client_UI->displayNewText(client_UI->getName(), sf::Color(252, 145, 145), client_UI->getInput());

        /// Send data when Enter is pressed and there is something to send
        sf::Packet packet;
        MessagePacket messagePacket;
        messagePacket.message = client_UI->getInput();
        messagePacket.name = client_UI->getName();
        messagePacket.room = client_UI->room;
        client_UI->addToHistory(messagePacket);

        packet << messagePacket;
        iSocket.send(packet);
      }

      client_UI->sending = false;
      client_UI->clearInput();
    }
  }
}
