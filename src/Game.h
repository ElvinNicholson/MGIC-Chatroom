#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

// External Library
#include <SFML/Graphics.hpp>

// Source code
#include "Networking/Server.h"
#include "Networking/Client.h"

class Game
{
 public:
  Game(sf::RenderWindow& game_window, bool server);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);

 private:
  bool isServer = true;

  std::unique_ptr<Server> server = nullptr;
  std::unique_ptr<Client> client = nullptr;

  sf::RenderWindow& window;

};

#endif // PLATFORMER_GAME_H
