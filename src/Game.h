
#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include "GameObject.h"



class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void updateLivesText();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void resetGame();

 private:
  sf::RenderWindow& window;
  
  
  sf::Font font;
  sf::Text play_text;
  sf::Text quit_text;
  sf::Text title_text;
  sf::Text welcome_text;
  sf::Text lives_text;
  sf::Text end_text;

  GameObject ball;
  GameObject paddle;
  GameObject blocks[78];

  sf::Texture blue_texture;
  sf::Texture green_texture;
  sf::Texture grey_texture;
  sf::Texture purple_texture;
  sf::Texture red_texture;
  sf::Texture yellow_texture;

  sf::Texture paddleTexture;
  sf::Texture ballTexture;

  enum Colour col;

  int columns = 11;
  int rows = 6;
  int player_lives = 3;

  bool in_menu = true;
  bool enter_selected = true;
  bool is_visible     = true;
  bool game_over      = false;

  bool collisionCheck(GameObject& ball, GameObject& paddle);
  int collisionCheckBlocks(
    GameObject& ball, GameObject blocks[], int blockCount);

  float paddle_speed = 500.0f;
  float screen_width = 800.0f;
  float screen_height = 600.0f;
  
};

#endif // BREAKOUT_GAME_H
