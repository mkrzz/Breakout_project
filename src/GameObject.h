#include <SFML/Graphics.hpp>
#include "vector2.h"

#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H

class GameObject
{
 public:

  GameObject();
  
  enum Colour
  {
    BLUE, 
    GREEN, 
    GREY,
    PURPLE,
    RED,
    YELLOW

  };
  ~GameObject();

  void addTexture(sf::Texture& texture, Colour col);
  void setIsVisible(bool visibility);
  bool getIsVisible();
 
  bool initialiseSprite(sf::Texture& texture, std::string filename);
  sf::Sprite* getSprite();

  void movePaddle(float dx, float dy);
  void moveBall(float dt);

  
  Vector2 getDirection() const;
  void setDirection(const Vector2& newDirection);
  
  private:

  sf::Sprite* sprite;

   Colour colour;

  bool is_visible;

   Vector2 direction;
};


#endif // BREAKOUT_GAMEOBJECT_H
