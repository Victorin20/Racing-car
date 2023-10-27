#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <vector>
using namespace sf;
using namespace std;

const int width(1000), heigth(800);
class Car
{
public:

    Car(int x, int y, const Texture& carTexture, const Texture& carTexturedamaged1, const Texture& carTexturedamaged2)
    {
        health = 100;
        this->x = x;
        this->y = y;
        this->carSprite.setTexture(carTexture);
        this->carTexture = carTexture;
        this->carDamaged1 = carTexturedamaged1;
        this->carDamaged2 = carTexturedamaged2;
        this->carSprite.setPosition(this->x, this->y);
    }

    const Sprite getSprite() { return this->carSprite; }

    const int getX() { return this->x; }

    bool isAlive() { return this->health > 0; }

    void shock() { this->health -= 34; }

    void restart() { this->health = 100; }

    void checkState()
    {
        if (health == 100)
        {
            this->carSprite.setTexture(carTexture);
        }

        if (health == 66)
        {
            this->carSprite.setTexture(carDamaged1);
        }
        if (health == 32)
        {
            this->carSprite.setTexture(carDamaged2);
        }
     
    }

    void move()
    {
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            
            x -= 5;
            if (x < 150)x = 150;
            this->carSprite.setPosition(x, y);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            
            x += 5; 
            if (x > 750)x = 750;
            this->carSprite.setPosition(x, y);
        }
    }


private:
    int x, y;
    int health;
    Sprite carSprite;
    Texture carDamaged1, carDamaged2, carTexture;
};

void moveBackground(Sprite* background1, Sprite* background2)
{
    background1->setPosition(0, background1->getPosition().y + 10);
    background2->setPosition(0, background2->getPosition().y + 10);

    if (background2->getPosition().y >= 0 && background1->getPosition().y > 0)
    {
        background1->setPosition(0, -1600);
    }

    if (background1->getPosition().y <= -1600)
    {
        background2->setPosition(0, 0);
    }
}

class Coin 
{
public:

    Coin(int x, int y, const Texture& coinTexture)
    {
        show = true;
        this->x = x;
        this->y = y;
        this->coinSprite.setTexture(coinTexture);
        this->coinSprite.setScale(0.2, 0.2);

    }

    const int getY() { return this->y; }
    void setY(int y) { this->y = y; this->coinSprite.setPosition(this->x, this->y); }
    void setX(int x) { this->x = x; }

    const Sprite getCoinSprite() { return coinSprite; }
    const bool getShow() { return this->show; }
    bool isOnScreen() { return this->y < heigth; }
    void activeShow() { this->show = true; }
    void dontShow() { this->show = false; }

    void verifyIfIsOnScreen()
    {
        if (!this->isOnScreen())this->show = false;
    }

private:
    int x, y;
    Sprite coinSprite;
    bool show;
    
};

class Obstacle
{
public:
    Obstacle() {};

    Obstacle(int x, int y, const Texture& obstacleTexture) 
    {
        this->show = true;
        this->x = x;
        this->y = y;
        this->obstacleSprite.setTexture(obstacleTexture);
        this->obstacleSprite.setScale(0.2, 0.2);
        this->obstacleSprite.setPosition(x, y);
    };

    const int getY() { return this->y; }
    void setY(int y) { this->y = y; this->obstacleSprite.setPosition(this->x, this->y); }
    void setX(int x) { this->x = x; }

    const Sprite getObstacleSprite() { return obstacleSprite; }
    const bool getShow() { return this->show; }
    bool isOnScreen() { return this->y < heigth; }
    void activeShow() { this->show = true; }
    void dontShow() { this->show = false; }

    void verifyIfIsOnScreen()
    {
        if (!this->isOnScreen())this->show = false;
    }


private:
    int x, y;
    Sprite obstacleSprite;
    bool show;

};

void moveObstacles(vector<Obstacle*> obstacles)
{
    for (auto obstacle : obstacles)
    {
        obstacle->setY(obstacle->getY() + 5);
    }
}

void drawObstacles(vector<Obstacle*> obstacles, RenderWindow& window)
{
    for (auto obstacle : obstacles)
    {
        window.draw(obstacle->getObstacleSprite());
    }
}

void moveCoins(vector<Coin*> coins)
{
    for (auto coin : coins)
    {
        coin->setY(coin->getY() + 5);
        
    }
}

void drawCoins(vector<Coin*> coins, RenderWindow& window)
{
    for (auto coin : coins)
    {
        window.draw(coin->getCoinSprite());
    }
}




int main()
{
    const int coins_number(3), obstacles_number(6);
    int score = 0, obstaclePosition(-1);
    bool gameOver = false;
   
    RenderWindow window(VideoMode(width, heigth), "Car Racing Game!");
    window.setFramerateLimit(60);

    Texture background_texture, car_texture, background2_texture, coin_texture;
    Texture obstacle_texture, carDamaged1, carDamaged2, gameOverTexture, restartTexture;
    background_texture.loadFromFile("images/background.png");
    background2_texture.loadFromFile("images/background.png");
    car_texture.loadFromFile("images/car.png");
    coin_texture.loadFromFile("images/coin.png");
    obstacle_texture.loadFromFile("images/obstacle.png");
    carDamaged1.loadFromFile("images/carDamaged1.png");
    carDamaged2.loadFromFile("images/carDamaged2.png");
    gameOverTexture.loadFromFile("images/gameOver.png");
    restartTexture.loadFromFile("images/restart.png");

    Font font;
    font.loadFromFile("sprites/ARLRDBD.TTF");

    Text score_text, gameOverScore;
    score_text.setFont(font);
    score_text.setCharacterSize(40);
    score_text.setFillColor(Color::White);
    score_text.setPosition( 40, 50);
    gameOverScore.setFont(font);
    gameOverScore.setCharacterSize(100);
    gameOverScore.setFillColor(Color::White);
    gameOverScore.setPosition(230, 100);

    Clock clock;

    float timer(0), delay(0.2);

    Sprite background(background_texture), background2(background2_texture);
    Sprite restartSprite(restartTexture), gameOverSprite(gameOverTexture);

    restartSprite.setPosition(width / 2-50, heigth / 2+50);
    restartSprite.setScale(0.2, 0.2);
    gameOverSprite.setPosition(width / 2-100, heigth / 2 - 100);

    int positions[3] = { 150, 450, 750 };
   
    Car* car = new Car(450, heigth-200, car_texture, carDamaged1, carDamaged2);
    vector<Coin*> coins;
    vector<Obstacle*>obstacles;
    srand(time(NULL));

    for (int k(0); k < coins_number; k++)
    {     
        Coin* coin = new Coin(positions[rand() % 3], 0, coin_texture);
        coins.push_back(coin);   
    }

    for (int k(0); k < obstacles_number; k++)
    {
        
        Obstacle* obstacle = new Obstacle(positions[rand() % 3], -300, obstacle_texture);
        obstacles.push_back(obstacle);
        
    }

    background2.setPosition(0, -1600);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        Vector2i mousePosition = Mouse::getPosition(window);

        float time = clock.getElapsedTime().asSeconds();
        timer += time;
        clock.restart();

        score_text.setString(to_string(score)+" $");

        window.clear();
        window.draw(background);
        window.draw(background2);

        if(!gameOver)
        {
            for(auto coin : coins)
            {
                FloatRect coinCoordonate = coin->getCoinSprite().getGlobalBounds();
                FloatRect carCoordonate = car->getSprite().getGlobalBounds();
                if (carCoordonate.intersects(coinCoordonate) && timer > delay)
                {
                    coin->dontShow();
                    score++;
                    timer = 0;
                }
                coin->verifyIfIsOnScreen();
                if (!coin->getShow())
                {
                    coin->setY(-102);
                    coin->setX(positions[rand() % 3]);
                    coin->activeShow();
                }
            }

            for (auto obstacle: obstacles)
            {
                for(auto coin: coins)
                {
                    if (coin->getCoinSprite().getGlobalBounds().contains(obstacle->getObstacleSprite().getPosition()))
                    {
                           coin->setY(-800 + rand() % 100);                 
                    }
                }
           
                FloatRect obstacleCoordonate = obstacle->getObstacleSprite().getGlobalBounds();
                FloatRect carCoordonate = car->getSprite().getGlobalBounds();
                if (carCoordonate.intersects(obstacleCoordonate) && timer > delay+0.7)
                {
                    car->shock();
                    car->checkState();
                    if (!car->isAlive())gameOver = true;
                    obstacle->dontShow();
                    timer = 0;
                }
                obstacle->verifyIfIsOnScreen();
                if (!obstacle->getShow())
                {
                     
                    obstacle->setX(positions[rand()%3]);
                    obstacle->activeShow();
                    obstacle->setY(-102);
                
                }
            }

            window.draw(car->getSprite());
            car->move();

            moveCoins(coins);
            drawCoins(coins, window);
            moveObstacles(obstacles);
            drawObstacles(obstacles, window);

            moveBackground(&background, &background2);
        
            window.draw(score_text);
        }
        if (gameOver)
        {
            gameOverScore.setString("Your score : " + to_string(score));
            window.draw(gameOverScore);
            window.draw(gameOverSprite);
            window.draw(restartSprite);

            if (Mouse::isButtonPressed(Mouse::Left) && restartSprite.getGlobalBounds().contains(Vector2f(mousePosition)))
            {
                gameOver = false;
                score = 0;
                car->restart();
                car->checkState();
                for (int k(0); k < coins_number; k++)
                {
                    coins.pop_back();
                }
                for (int k(0); k < obstacles_number; k++)
                {
                    obstacles.pop_back();
                }

                for (int k(0); k < coins_number; k++)
                {
                    Coin* coin = new Coin(positions[rand() % 3], 0, coin_texture);
                    coins.push_back(coin);
                }

                for (int k(0); k < obstacles_number; k++)
                {

                    Obstacle* obstacle = new Obstacle(positions[rand() % 3], -300, obstacle_texture);
                    obstacles.push_back(obstacle);

                }

            }

        }
        window.display();
    }
    delete car;
    return 0;
}
