#include "ClientUI.h"

ClientUI::ClientUI()
{
  room = 0;

  my_name = std::make_unique<std::string>();

  background_texture = std::make_unique<sf::Texture>();
  background_texture->loadFromFile("../Data/Images/Background.png");

  background = std::make_unique<GameObject>();
  background->setTexture(*background_texture);

  connect_texture = std::make_unique<sf::Texture>();
  connect_texture->loadFromFile("../Data/Images/Connect.png");

  connect = std::make_unique<GameObject>();
  connect->setTexture(*connect_texture);

  overlay_texture = std::make_unique<sf::Texture>();
  overlay_texture->loadFromFile("../Data/Images/Overlay.png");

  overlay = std::make_unique<GameObject>();
  overlay->setTexture(*overlay_texture);

  channel_texture = std::make_unique<sf::Texture>();
  channel_texture->loadFromFile("../Data/Images/CreateChannel.png");

  channel = std::make_unique<GameObject>();
  channel->setTexture(*channel_texture);
  channel->setPosition(220, 40);

  font = std::make_unique<sf::Font>();
  font->loadFromFile("../Data/Fonts/ggsans-Normal.ttf");

  input_text = std::make_unique<sf::Text>();
  input_text->setFont(*font);
  input_text->setCharacterSize(24);
  input_text->setPosition(365, 570);

  input_string = std::make_unique<std::string>();

  channel_name_text = std::make_unique<sf::Text>();
  channel_name_text->setFont(*font);
  channel_name_text->setCharacterSize(24);
  channel_name_text->setPosition(410, 22);

  mouse_scroll_y = std::make_unique<float>();
  *mouse_scroll_y = 0;
  mouse_pos = std::make_unique<sf::Vector2i>();

  createNewChannel();
  createNewChannel();

  changeChannelName(0);
}

void ClientUI::update(float dt)
{

}

void ClientUI::render(sf::RenderWindow& window)
{
  *mouse_pos = sf::Mouse::getPosition(window);

  /// Background
  background->renderObject(window);

  /// Channels
  for (auto& text: channel_text)
  {
    window.draw(*text);
  }

  /// Text Messages
  for (auto& text : displayed_text)
  {
    window.draw(*text);
  }

  /// Overlay
  overlay->renderObject(window);

  /// Create Channel
  if (isMouseWithinRect(*mouse_pos, create_channel_button))
  {
    channel->renderObject(window);
  }

  /// Channel name
  window.draw(*channel_name_text);

  /// Connect window
  if (my_name->empty())
  {
    connect->renderObject(window);
  }

  /// Input text box
  window.draw(*input_text);
}

void ClientUI::eventHandler(sf::Event event)
{
  /// Typing
  if (event.type == sf::Event::TextEntered)
  {
    /// Backspace
    if (event.text.unicode == 8)
    {
      if (!input_string->empty())
      {
        input_string->pop_back();
        input_text->setString(*input_string);
      }
    }
    else
    {
      *input_string += event.text.unicode;
      input_text->setString(*input_string);
    }
  }

  /// ENTER key
  else if (event.type == sf::Event::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Enter && !input_string->empty())
    {
      sending = true;
    }
  }

  /// Scroll wheel
  else if (event.type == sf::Event::MouseWheelScrolled)
  {
    scrollText(std::clamp(event.mouseWheelScroll.delta, -1.0f, 1.0f));
  }

  /// Mouse
  else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button ==  sf::Mouse::Left)
  {
    for (int i = 0; i < channel_text.size(); i++)
    {
      if (isMouseWithinRect(*mouse_pos, channel_text[i]->getGlobalBounds()))
      {
        /// Switch rooms
        switchRoom(i);
      }
    }

    if (isMouseWithinRect(*mouse_pos, create_channel_button))
    {
      createNewChannel();
    }
  }
}

std::string ClientUI::getInput()
{
  return *input_string;
}

void ClientUI::clearInput()
{
  input_string->clear();
  input_text->setString(*input_string);
}

void ClientUI::displayNewText(std::string name, sf::Color name_color, std::string message)
{
  displayed_text.emplace_back(std::make_unique<sf::Text>());
  displayed_text.back()->setFont(*font);
  displayed_text.back()->setCharacterSize(24);

  displayed_text.back()->setString(name);
  displayed_text.back()->setFillColor(name_color);
  float y_pos = 75 + (displayed_text.size() * 40) + *mouse_scroll_y;
  displayed_text.back()->setPosition(360, y_pos);

  displayed_text.emplace_back(std::make_unique<sf::Text>());
  displayed_text.back()->setFont(*font);
  displayed_text.back()->setCharacterSize(24);

  displayed_text.back()->setString(message);
  displayed_text.back()->setFillColor(sf::Color::White);
  displayed_text.back()->setPosition(360, y_pos + 30);
}

void ClientUI::nameClient(std::string name)
{
  *my_name = name;
  input_text->setPosition(450, 1220);
}

void ClientUI::scrollText(float mouse_delta)
{
  /// Stop from scrolling offscreen
  if ((displayed_text.size() <= 14) ||
      (displayed_text.back()->getPosition().y < 1150 && mouse_delta < 0) ||
      (displayed_text.front()->getPosition().y > 80 && mouse_delta > 0))
  {
    return;
  }

  *mouse_scroll_y += 20 * mouse_delta;

  for (auto& text : displayed_text)
  {
    text->move(0, 20 * mouse_delta);
  }
}

bool ClientUI::isMouseWithinRect(sf::Vector2i mouse_pos, sf::FloatRect floatRect)
{
  sf::Vector2f bounds = {floatRect.left + floatRect.width, floatRect.top + floatRect.height};

  if (
    mouse_pos.x < bounds.x &&
    mouse_pos.x > floatRect.left &&
    mouse_pos.y < bounds.y &&
    mouse_pos.y > floatRect.top
  )
  {
    return true;
  }
  return false;
}

void ClientUI::addToHistory(const MessagePacket& messagePacket)
{
  text_history.at(messagePacket.room).emplace_back(messagePacket);
}

void ClientUI::switchRoom(int room_)
{
  *mouse_scroll_y = 0;
  room = room_;
  changeChannelName(room);
  displayed_text.clear();
  for (auto& message : text_history.at(room))
  {
    sf::Color color;
    if (message.name == *my_name)
    {
      color = sf::Color(252, 145, 145);
    }
    else
    {
      color = sf::Color(117, 226, 255);
    }

    displayNewText(message.name, color, message.message);
  }
}

std::string ClientUI::getName()
{
  return *my_name;
}

void ClientUI::createNewChannel()
{
  text_history.push_back({});

  channel_text.emplace_back((std::make_unique<sf::Text>()));
  channel_text.back()->setFont(*font);
  channel_text.back()->setCharacterSize(24);
  channel_text.back()->setPosition(25, 100 + channel_text.size() * 35);
  channel_text.back()->setFillColor(sf::Color(152, 154, 162));
  channel_text.back()->setString("# CHANNEL-" + std::to_string(channel_text.size()));
}

int ClientUI::getNumberOfRooms()
{
  return text_history.size();
}

void ClientUI::changeChannelName(int room_)
{
  std::string channel_name = channel_text.at(room_)->getString();
  channel_name.erase(0, 1);
  channel_name_text->setString(channel_name);
}
