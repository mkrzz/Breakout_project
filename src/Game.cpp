#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>
#include "Vector2.h"



Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{

  

  in_menu = true;

  if (!font.loadFromFile("../Data/Fonts/ka1.ttf"))
  {
    std::cout << "Font did not load";
  }

  //play text
  play_text.setFont(font);
  play_text.setString("Press enter to play");
  play_text.setCharacterSize(30);
  play_text.setLetterSpacing(3);
  play_text.setFillColor(sf::Color::White);
  play_text.setPosition(
    window.getSize().x / 2 - play_text.getGlobalBounds().width / 2, 360);

  //title text
  title_text.setFont(font);
  title_text.setString("Breakout");
  title_text.setCharacterSize(50);
  title_text.setLetterSpacing(3);
  title_text.setFillColor(sf::Color::White);
  title_text.setPosition(
    window.getSize().x / 2 - title_text.getGlobalBounds().width / 2, 180);

  //lives text 
  lives_text.setFont(font);
  lives_text.setCharacterSize(24);
  lives_text.setFillColor(sf::Color::White);
  lives_text.setPosition(650, 530); 
  updateLivesText();



  //paddle texture
  if (!paddleTexture.loadFromFile("../Data/Images/paddleBlue.png"))
  {
    std::cout << "Paddle did not load";
    return false;
  }
  //paddle position
  paddle.initialiseSprite(paddleTexture, "../Data/Images/paddleBlue.png");
  float paddleWidth  = paddle.getSprite()->getGlobalBounds().width;
  float paddleHeight = paddle.getSprite()->getGlobalBounds().height;
  paddle.getSprite()->setPosition((800 - paddleWidth) / 2, 500);

  //ball texture
  if (!ballTexture.loadFromFile("../Data/Images/ball.png"))
  {
    std::cout << "ball did not load";
  }

  ball.initialiseSprite(ballTexture,"../Data/Images/ball.png");
  ball.getSprite()->setPosition(400, 400);
  ball.getSprite()->setScale(.2, .2 ); 

  //block colour textures
  if ((!blue_texture.loadFromFile("../Data/Images/element_blue_rectangle.png")) ||
      (!green_texture.loadFromFile("../Data/Images/element_green_rectangle.png")) ||
      (!grey_texture.loadFromFile("../Data/Images/element_grey_rectangle.png")) ||
      (!purple_texture.loadFromFile("../Data/Images/element_purple_rectangle.png")) ||
      (!red_texture.loadFromFile("../Data/Images/element_red_rectangle.png")) ||
      (!yellow_texture.loadFromFile("../Data/Images/element_yellow_rectangle.png")))
  {
    std::cout << "texture did not load";
  }
  
  //loop array of colour blocks which is supposed to be random colour placement 
  //but somehow has grouped the colours
  for (int i = 0; i < columns; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      if ((i * rows + j) % 6 == 0)
      {
        blocks[i * rows + j].addTexture(
          blue_texture, GameObject::Colour::BLUE);
      }
      else if ((i * rows + j) % 6 == 1)
      {
        blocks[i * rows + j].addTexture(
          green_texture, GameObject::Colour::GREEN);
      }
      else if ((i * rows + j) % 6 == 2)
      {
        blocks[i * rows + j].addTexture(
          grey_texture, GameObject::Colour::GREY);
      }
      else if ((i * rows + j) % 6 == 3)
      {
        blocks[i * rows + j].addTexture(
          purple_texture, GameObject::Colour::PURPLE);
      }
      else if ((i * rows + j) % 6 == 4)
      {
        blocks[i * rows + j].addTexture(
          red_texture, GameObject::Colour::RED);
      }
      else if ((i * rows + j) % 6 == 5)
      {
        blocks[i * rows + j].addTexture(
          yellow_texture, GameObject::Colour::YELLOW);
      }

      blocks[i * rows + j].getSprite()->setPosition((i + .25) * 70, j * 33);
    }
  }
  
  ball.setDirection(Vector2(1.0f, -1.0f));

  

  return true;
}

void Game::updateLivesText()
{
  lives_text.setString("Lives: " + std::to_string(player_lives));
}

void Game::update(float dt)
{
  if (game_over || in_menu)
  {
    return; 
  }

    //paddle movement 
  sf::Vector2 paddlePos = paddle.getSprite()->getPosition();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    
    if (paddlePos.x > 0)
    {
      paddle.movePaddle(-paddle_speed * dt, 0); 
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    
    if (paddlePos.x < screen_width - paddle.getSprite()->getGlobalBounds().width)
    {
      paddle.movePaddle(paddle_speed * dt, 0); 
    }
  }

  ball.moveBall(dt);

  sf::Vector2f ballPos = ball.getSprite()->getPosition();
  Vector2 direction = ball.getDirection(); 

  float ballRadius = ball.getSprite()->getGlobalBounds().width / 2;

  // wall collisions
  if (ballPos.x - ballRadius <= 0)
  {
    direction.x = -direction.x;   
    ball.setDirection(direction); 
  }
  if (ballPos.x + ballRadius >= window.getSize().x)
  {
    direction.x = -direction.x;   
    ball.setDirection(direction); 
  }
  if (ballPos.y - ballRadius <= 0)
  {
    direction.y = -direction.y;   
    ball.setDirection(direction); 
  }
  if (ballPos.y + ballRadius >= window.getSize().y)
  {
    player_lives--;
    updateLivesText();

    if (player_lives <= 0)
    {
      game_over = true;
    }
    else
    {
      // Reset ball position and direction
      ball.getSprite()->setPosition(400, 200);
      ball.setDirection(direction); 
    }

  }

  // Check collision with paddle
  if (collisionCheck(ball, paddle))
  {
    // Reverse Y direction when ball hits the paddle
    direction.y = -direction.y;
    
  }
  //If a block was hit
  int hitBlockIndex = Game::collisionCheckBlocks(ball, blocks, 78);
  if (hitBlockIndex != -1) 
  {
    // Reverse direction
    direction.y = -direction.y; 
  }

  ball.setDirection(direction);
}

void Game::render()
{
    if (in_menu)
    {
      window.draw(play_text);
      window.draw(title_text);
    }
    else
    {
      window.draw(*paddle.getSprite());
      window.draw(*ball.getSprite());
      window.draw(lives_text);

      for (int i = 0; i < 78; i++)
      {
       
          if (blocks[i].getIsVisible())
        {
          window.draw(*blocks[i].getSprite());
        }
          
      }
      
    }
    if (game_over)
    {
        //play again 
      sf::Text loseText;
      loseText.setFont(font);
      loseText.setString("\nYou Lose.\n Press enter to play again");
      loseText.setCharacterSize(30);
      loseText.setFillColor(sf::Color::Red);
      loseText.setPosition(
        window.getSize().x / 2 - loseText.getGlobalBounds().width / 2,
        window.getSize().y / 2);
      window.draw(loseText);

      
    }
}



void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);


}

void Game::keyPressed(sf::Event event)
{
    if (event.key.code == sf::Keyboard::Enter)
    {
      
        if (game_over)
      {
        resetGame(); 
      }
      else if (in_menu)
      {
        in_menu = false;
      }
    }

    
}

bool Game::collisionCheck(GameObject& ball, GameObject& paddle)
{
  
    // Get positions and sizes
  float ballLeft   = ball.getSprite()->getPosition().x;
  float ballRight  = ballLeft + ball.getSprite()->getGlobalBounds().width;
  float ballTop    = ball.getSprite()->getPosition().y;
  float ballBottom = ballTop + ball.getSprite()->getGlobalBounds().height;

  float paddleLeft   = paddle.getSprite()->getPosition().x;
  float paddleRight  = paddleLeft + paddle.getSprite()->getGlobalBounds().width;
  float paddleTop    = paddle.getSprite()->getPosition().y;
  float paddleBottom = paddleTop + paddle.getSprite()->getGlobalBounds().height;

  //collision detection 
  if (
    ballRight > paddleLeft && ballLeft < paddleRight &&
    ballBottom > paddleTop && ballTop < paddleBottom)
  {
    //return true if hit 
    return true; 
  }

  return false; 
  
  
}

int Game::collisionCheckBlocks(
  GameObject& ball, GameObject blocks[], int blockCount)
{
  float ballLeft   = ball.getSprite()->getPosition().x;
  float ballRight  = ballLeft + ball.getSprite()->getGlobalBounds().width;
  float ballTop    = ball.getSprite()->getPosition().y;
  float ballBottom = ballTop + ball.getSprite()->getGlobalBounds().height;

  for (int i = 0; i < blockCount; i++)
  {
    // Skip invisible blocks
    if (!blocks[i].getIsVisible()) 
      continue;

    float blockLeft = blocks[i].getSprite()->getPosition().x;
    float blockRight =
      blockLeft + blocks[i].getSprite()->getGlobalBounds().width;
    float blockTop = blocks[i].getSprite()->getPosition().y;
    float blockBottom =
      blockTop + blocks[i].getSprite()->getGlobalBounds().height;

    // breaks blocks
    if (
      ballRight > blockLeft && ballLeft < blockRight && ballBottom > blockTop &&
      ballTop < blockBottom)
    {
      
      blocks[i].setIsVisible(false); 
      return i;                      
    }
  }

  return -1;
}

void Game::resetGame()
{
  // Reset lives
  player_lives = 3;  
  updateLivesText(); 

  // Reset paddle position
  paddle.getSprite()->setPosition(400, 500);

  // Reset ball position and direction
  ball.getSprite()->setPosition(400, 400);
  ball.setDirection(Vector2(1.0f, -1.0f));

  // Reset blocks
  for (int i = 0; i < 78; i++)
  {
    blocks[i].setIsVisible(true); 
  }
  //reset game over

  game_over = false; 
}
