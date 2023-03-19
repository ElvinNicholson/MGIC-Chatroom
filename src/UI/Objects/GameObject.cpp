#include "GameObject.h"

GameObject::GameObject()
{
  sprite = std::make_unique<sf::Sprite>();
}

void GameObject::setTexture(sf::Texture& texture)
{
  sprite -> setTexture(texture);
}

void GameObject::setIntRect(sf::IntRect intRect)
{
  sprite -> setTextureRect(intRect);
}

void GameObject::setPosition(float x, float y)
{
  sprite -> setPosition(x, y);
}

void GameObject::move(float x, float y)
{
  sprite -> move(x, y);
}

void GameObject::renderObject(sf::RenderWindow& renderWindow)
{
  renderWindow.draw(*sprite);
}

void GameObject::setOriginCentre()
{
  sf::FloatRect spriteBounds = sprite -> getLocalBounds();

  sprite -> setOrigin(spriteBounds.width/2, spriteBounds.height/2);
}

void GameObject::setScale(float x, float y)
{
  sprite -> setScale(x, y);
}

void GameObject::setRotation(float angle)
{
  sprite -> setRotation(angle);
}

sf::Vector2f GameObject::getPosition()
{
  return sprite -> getPosition();
}

sf::FloatRect GameObject::getGlobalBounds()
{
  return sprite->getGlobalBounds();
}

