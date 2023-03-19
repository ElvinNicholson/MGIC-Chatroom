#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window, bool server)
  : window(game_window), isServer(server)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{
  if (isServer)
  {
    server = std::make_unique<Server>();
    server->init();
    server->run();
  }
  else
  {
    client = std::make_unique<Client>();
    client->init();
    client->connect();
    std::thread client_thread ([&] {
                               client->run();
                             });
    client_thread.detach();
  }

  return true;
}

void Game::update(float dt)
{
  if (!isServer)
  {
    client->update(dt);
  }
}

void Game::render()
{
  if (!isServer)
  {
    client->render(window);
  }
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);


}

void Game::keyPressed(sf::Event event)
{
  client->eventHandler(event);
}


