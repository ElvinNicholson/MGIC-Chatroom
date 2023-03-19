#ifndef CHATROOM_CLIENTUI_H
#define CHATROOM_CLIENTUI_H

// External Library
#include <SFML/Graphics.hpp>
#include <atomic>

// Source code
#include "Objects/GameObject.h"
#include "../Networking/MessagePacket.h"

class ClientUI
{
 public:
  ClientUI();

  void update(float dt);
  void render(sf::RenderWindow &window);
  void eventHandler(sf::Event event);

  std::string getInput();
  void clearInput();

  void displayNewText(std::string name, sf::Color name_color, std::string message);
  void nameClient(std::string);
  void addToHistory(const MessagePacket& messagePacket);
  std::string getName();

  void createNewChannel();
  int getNumberOfRooms();

  std::atomic<bool> sending = false;
  std::atomic<int> room;

 protected:

 private:
  bool isMouseWithinRect(sf::Vector2i mouse_pos, sf::FloatRect floatRect);
  void scrollText(float mouse_delta);
  void switchRoom(int room_);
  void changeChannelName(int room_);

  std::vector<std::vector<MessagePacket>> text_history;

  std::unique_ptr<sf::Texture> background_texture;
  std::unique_ptr<GameObject> background;

  std::unique_ptr<sf::Texture> connect_texture;
  std::unique_ptr<GameObject> connect;

  std::unique_ptr<sf::Texture> overlay_texture;
  std::unique_ptr<GameObject> overlay;

  std::unique_ptr<sf::Texture> channel_texture;
  std::unique_ptr<GameObject> channel;

  std::unique_ptr<sf::Font> font;
  std::unique_ptr<sf::Text> input_text;
  std::vector<std::unique_ptr<sf::Text>> displayed_text;

  std::vector<std::unique_ptr<sf::Text>> channel_text;
  std::unique_ptr<sf::Text> channel_name_text;

  std::unique_ptr<std::string> input_string;

  std::unique_ptr<sf::Vector2i> mouse_pos;
  std::unique_ptr<float> mouse_scroll_y;

  std::unique_ptr<std::string> my_name;

  const sf::FloatRect create_channel_button = sf::FloatRect(300, 100, 20, 20);
};

#endif // CHATROOM_CLIENTUI_H
