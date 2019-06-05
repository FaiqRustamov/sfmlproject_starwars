#include <SFML/Graphics.hpp>
#include<math.h>
#include<cstdlib>
#include<vector>
#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
using namespace sf;
using namespace std;
class Bullet{
public:
 Sprite shape;
 Bullet(Texture *texture,Vector2f pos)
 {

 this->shape.setTexture(*texture);
 this->shape.setScale(0.3f,0.3f);
 this->shape.setPosition(pos);
 }
 };

class Player{
public:
Sprite shape;
Texture *texture;
int hp;
int hpmax;

vector<Bullet>bullets;

Player(Texture *texture){
this->hpmax=10;
this->hp=this->hpmax;
this->texture=texture;
this->shape.setTexture(*texture);
this->shape.setScale(0.2f,0.2f);
}

};
class Enemy{
public:
    Sprite shape;
    int hp;
    int hpmax;
    Enemy(Texture *texture, Vector2u windowSize){

    this->hpmax=rand()%3+1;
    this->hp=this->hpmax;
    this->shape.setTexture(*texture);
    this->shape.setScale(0.2f,0.2f);
    this->shape.setPosition(windowSize.x-this->shape.getGlobalBounds().width,rand()%(int)(windowSize.y-this->shape.getGlobalBounds().height));
    }

};

int main()
{
srand(time(NULL));

Font font;
font.loadFromFile("fonts/Dosis-Light.ttf");

RenderWindow window(VideoMode(1000, 1000), "SFML works!");
window.setFramerateLimit(60);

Texture backgroundTex;
backgroundTex.loadFromFile("images/background.jpg");
Texture playerTex;
playerTex.loadFromFile("images/ship.png");

Texture enemyTex;
enemyTex.loadFromFile("images/enemy.png");

Texture bulletTex;
bulletTex.loadFromFile("images/bullet.png");

Text scoreText;
scoreText.setFont(font);
scoreText.setCharacterSize(20);
scoreText.setFillColor(Color::White);
scoreText.setPosition(10.f,10.f);

Text gameOverText;
gameOverText.setFont(font);
gameOverText.setCharacterSize(30);
gameOverText.setFillColor(Color::Red);
gameOverText.setPosition(100.f,window.getSize().y/2);
gameOverText.setString("Game Over....");

Sprite sBackground(backgroundTex);
//User
int score=0;
Player player(&playerTex);
int  shootTimer=20;
Text hpText;
hpText.setFont(font);
hpText.setString("150");
hpText.setCharacterSize(11);
hpText.setFillColor(Color::White);
//Enemy
int enemySpawnTimer =0;
std::vector<Enemy>enemies;
Text enhpText;
enhpText.setFont(font);
enhpText.setCharacterSize(11);
enhpText.setFillColor(Color::White);
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if(player.hp>0)
        {

//User
  if (sf::Keyboard::isKeyPressed(Keyboard::Key::A))
{
player.shape.move(-10.0f,0.0f);

}
 if (Keyboard::isKeyPressed(Keyboard::Key::D))
{
player.shape.move(10.0f,0.0f);

}
  if (Keyboard::isKeyPressed(Keyboard::Key::W))
{
player.shape.move(0.0f,-10.0f);

}
 if (Keyboard::isKeyPressed(Keyboard::Key::S))
{
player.shape.move(0.0f,10.0f);

}
hpText.setPosition(player.shape.getPosition().x,player.shape.getPosition().y-hpText.getGlobalBounds().height);
hpText.setString(std::to_string(player.hp)+"/"+std::to_string(player.hpmax));
if(player.shape.getPosition().x<=0)
   player.shape.setPosition(0.f,player.shape.getPosition().y);
 if (player.shape.getPosition().x>=window.getSize().x-player.shape.getGlobalBounds().width)
   player.shape.setPosition(window.getSize().x-player.shape.getGlobalBounds().width,player.shape.getPosition().y);

 if(player.shape.getPosition().y<=0)
   player.shape.setPosition(player.shape.getPosition().x,0.f);
if (player.shape.getPosition().y>=window.getSize().y-player.shape.getGlobalBounds().height)
   player.shape.setPosition(player.shape.getPosition().x,window.getSize().y-player.shape.getGlobalBounds().height);

   if(shootTimer<15)
    shootTimer++;


if(Mouse::isButtonPressed(Mouse::Left )&& shootTimer >= 15) //ateş
{
player.bullets.push_back(Bullet(&bulletTex,player.shape.getPosition()));
shootTimer=0;
}

 for(size_t i=0;i<player.bullets.size();i++)
{
    player.bullets[i].shape.move(20.f,0.f);

    if(player.bullets[i].shape.getPosition().x > window.getSize().x)
        {
        player.bullets.erase(player.bullets.begin()+i);
        break;
        }
  for(size_t k=0;k<enemies.size();k++){
     if(player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
  {   if(enemies[k].hp<=1)
    {
      score += enemies[k].hpmax;
      enemies.erase(enemies.begin()+k);
    }
      else
        enemies[k].hp--; //take damage
      player.bullets.erase(player.bullets.begin()+i);
      break;
  }
  }
}
if(enemySpawnTimer<20)
    enemySpawnTimer++;
if(enemySpawnTimer>=20)
{
    enemies.push_back(Enemy(&enemyTex,window.getSize()));
    enemySpawnTimer=0;//reset timer
}
for(size_t i=0;i<enemies.size();i++){

    enemies[i].shape.move(-7.f,0.f);
    if(enemies[i].shape.getPosition().x<=0-enemies[i].shape.getGlobalBounds().width)
        {
            enemies.erase(enemies.begin()+i);
        break;
        }
    if(enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
     {

              enemies.erase(enemies.begin()+i);
              player.hp--; //can azalir userin
    break;
     }
}
scoreText.setString("Score --> "+to_string(score));
}
/*
else{
    return 0;
}
*/

//Draw
      window.clear();
//Background
 window.draw(sBackground);
    //User
    window.draw(player.shape);
     for(size_t i=0;i<player.bullets.size();i++){
         window.draw(player.bullets[i].shape);
        }

    //Enemy
   for(size_t i=0;i<enemies.size();i++){
        enhpText.setString(std::to_string(enemies[i].hp)+"/"+std::to_string(enemies[i].hpmax));
        enhpText.setPosition(enemies[i].shape.getPosition().x,enemies[i].shape.getPosition().y-enhpText.getGlobalBounds().height);
        window.draw(enhpText);
        window.draw(enemies[i].shape);
   }

    window.draw(scoreText);
    window.draw(hpText);
    if(player.hp<=0)
        window.draw(gameOverText);
    window.display();

 }
return 0;
    }
