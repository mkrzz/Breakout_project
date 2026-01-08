
#include "GameObject.h"
#include <iostream>
#include "Game.h"

GameObject::GameObject()
{
  colour = Colour::BLUE;
  sprite = new sf::Sprite();
  direction = Vector2(1.0f, -1.0f);
}

GameObject::~GameObject()
{
  delete sprite;
}

void GameObject::addTexture(sf::Texture& texture, Colour col)
{
  sprite->setTexture(texture);
  colour = col;
  
}

void GameObject::setIsVisible(bool visibility) 
{
  is_visible = visibility;
}

bool GameObject::getIsVisible()
{
  return is_visible;
}

bool GameObject::initialiseSprite(sf::Texture& texture, std::string filename)
{
  sprite->setTexture(texture);
  return true;
}

sf::Sprite* GameObject::getSprite()
{
  return sprite;
}

void GameObject::movePaddle(float dx, float dy)
{
  sprite->move(dx, dy); 

}

Vector2 GameObject::getDirection() const
{
  return direction;
}

void GameObject::setDirection(const Vector2& newDirection)
{
  direction = newDirection; 
  direction.normalise();    
}

void GameObject::moveBall(float dt)
{
  float speed = 300.0f;
  sprite->move(direction.x * speed * dt, direction.y * speed * dt);
}



