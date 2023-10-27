#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <list>

using namespace std;
using namespace sf;
const int width(1200), heigth(800);
int score(0);
class Bird
{
public:

    Bird(){}

    Bird(int x, int y, float weigth,float velocity,float angle, Texture& bird_texture)
    {
        this->x = x;
        this->y = y;
        this->weigth = weigth;
        this->angle = angle;
        this->velocity = velocity;
        bird_sprite.setTexture(bird_texture);
        bird_sprite.setPosition(this->x, this->y);
    }

    const int getX() { return this->x; }
    const int getY() { return this->y; }

    void setX(const int x) { this->x = x; }
    void setY(const int y) { this->y = y; }

    void setVelocity(const int Velocity) { this->velocity = velocity; }


    void jump()
    {
        velocity = -15;
        
    }

    bool isJumping() {
        return this->velocity < 0;
    }

    const Sprite getSprite() { return this->bird_sprite; }

    void update()
    {
        angle += 2;
        float curve = sin(angle) * 20;
        this->y += velocity + curve;
        if (y > heigth - 71)y = heigth - 71+curve;
        if (velocity < 0)velocity = 0;
        velocity += weigth;
        bird_sprite.setPosition(this->x, this->y);
    }


private:

    float weigth;
    float velocity;
    int x, y;
    float angle;
    Sprite bird_sprite;
};

class Obstacle
{
public:
    Obstacle() {}

    Obstacle(int x,int y ,int x2, int y2,const int gameSpeed,const Texture& obstacle_texture_up, const Texture& obstacle_texture_down)
    {
        this->x = x;
        this->y = y;
        this->x2 = x2;
        this->y2 = y2;
        this->gameSpeed = gameSpeed;

        this->obstacle_sprite_down.setTexture(obstacle_texture_down);
        this->obstacle_sprite_up.setTexture(obstacle_texture_up);


        this->obstacle_sprite_down.setPosition(x, y);
        this->obstacle_sprite_up.setPosition(x2, y2);
    }

    const Sprite getObstacleDown() { return this->obstacle_sprite_down; }
    const Sprite getObstacleUp() { return this->obstacle_sprite_up; }

    void update()
    {
        this->x -= gameSpeed;
        this->x2 -= gameSpeed;

        if (this->x < -100)this->x = width, score++;
        if (this->x2 < -100)this->x2 = width;

        this->obstacle_sprite_up.setPosition(x2, y2);
        this->obstacle_sprite_down.setPosition(x, y);
    }


private:

    Sprite obstacle_sprite_down;
    Sprite obstacle_sprite_up;

    int x, y, x2, y2;
    int gameSpeed;


};

int main()
{
    const int obstaclesNumber(4), gameSpeed(2);

    sf::RenderWindow window(sf::VideoMode(width, heigth), "Flappy bird");
    Texture bird_texture, obstacle_up_texture, obstacle_down_texture, background_texture;
    Texture game_over_texture, restart_texture;
    bird_texture.loadFromFile("images/bird.png");
    obstacle_down_texture.loadFromFile("images/pipeUp.png");
    obstacle_up_texture.loadFromFile("images/pipeDown.png");
    background_texture.loadFromFile("images/background.png");
    game_over_texture.loadFromFile("images/gameOver.png");
    restart_texture.loadFromFile("images/restart.png");

    Font font;
    font.loadFromFile("sprites/ARLRDBD.TTF");
    Text score_text;
    score_text.setFont(font);
    score_text.setPosition(width - 100, 50);
    score_text.setCharacterSize(52);
    score_text.setFillColor(Color::Black);

    Sprite background(background_texture), restart(restart_texture), gameOverSprite(game_over_texture);
    gameOverSprite.setPosition(width / 2-100, heigth / 2-100);
    restart.setPosition(width / 2-50, heigth / 2+50);
    restart.setScale(0.2, 0.2);
    Bird* bird = new Bird(100, 100, 9.81, 0,10, bird_texture);
    Clock clock;
    float timer(0),timer2(0), delay = 0.01;
    bool gameOver = false;

    list<Obstacle*> obstacles;

    srand(time(NULL));

    for (int k(0); k < obstaclesNumber; k++)
    {   
        int y = rand()% 400 - 400;
        if (y < -300)y = -300;
        int y2 = y + 700;
        if (y < -300)y2 = y + 750;
        Obstacle* obstacle = new Obstacle(width - 100+k*300, y, width - 100 + k * 300, y2, gameSpeed, obstacle_up_texture, obstacle_down_texture);
        obstacles.push_back(obstacle);
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Vector2i mousePosition = Mouse::getPosition(window);

        if (gameOver && restart.getGlobalBounds().contains(Vector2f(mousePosition)) && Mouse::isButtonPressed(Mouse::Left))
        {
            gameOver = false;
        }

        float time = clock.getElapsedTime().asSeconds();
        timer += time;
        timer2 += time;
        clock.restart();

        score_text.setString(to_string(score));

        if (Keyboard::isKeyPressed(Keyboard::Space))bird->jump(); 
        

        window.clear();
        window.draw(background);

        if (!gameOver)
        {
            for (auto obstacle: obstacles)
            {
                window.draw(obstacle->getObstacleDown());
                window.draw(obstacle->getObstacleUp());
            }

            if (timer > delay)
            {
                for (auto obstacle : obstacles)
                {
                    obstacle->update();
                    FloatRect obstacle_down_coordinate = obstacle->getObstacleDown().getGlobalBounds();
                    FloatRect obstacle_up_coordinate = obstacle->getObstacleUp().getGlobalBounds();
                    FloatRect bird_coordonate = bird->getSprite().getGlobalBounds();
                    if (bird_coordonate.intersects(obstacle_down_coordinate))gameOver = true;
                    if (bird_coordonate.intersects(obstacle_up_coordinate))gameOver = true; 
               
                }
            timer = 0;
            }

            if (timer2 > delay * 10)
            {
                bird->update();
                timer2 = 0;
            }

    
            window.draw(bird->getSprite());
            window.draw(score_text);
        }

        if (gameOver)
        {
            for (int k(0); k < obstaclesNumber; k++)
            {
                obstacles.pop_back();
            }

            for (int k(0); k < obstaclesNumber; k++)
            {
                int y = rand() % 400 - 400;
                if (y < -300)y = -300;
                int y2 = y + 700;
                if (y < -300)y2 = y + 750;
                Obstacle* obstacle = new Obstacle(width - 100 + k * 300, y, width - 100 + k * 300, y2, gameSpeed, obstacle_up_texture, obstacle_down_texture);
                obstacles.push_back(obstacle);
            }

            bird->setX(100);
            bird->setY(100);
            bird->setVelocity(0);
            window.draw(gameOverSprite);
            window.draw(restart);
        }

        window.display();
    }

 

    return 0;
}