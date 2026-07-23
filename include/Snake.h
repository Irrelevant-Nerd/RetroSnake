#ifndef SNAKE
#define SNAKE

#include "Constants.h"
#include <list>
#include <SFML/Graphics.hpp>

class Snake
{    
    public:
        Snake();

        void Direction();
        void Move(float moveDelay);
        bool Eat(sf::Vector2f fruitPosition);
        
        void Draw(sf::RenderWindow& window);
    
    public:
        sf::Vector2f getSnakePosition() { return snake.getPosition();}
        sf::Vector2f getSnakeSize() { return snake.getSize();}
        std::list<sf::Vector2f> getTailPosition() { return tail; }

    private:
        sf::RectangleShape snake;
        
        sf::RectangleShape segment;
        std::list<sf::Vector2f> tail;

        sf::Clock clock;

        const float SPEED {20.f};

        float snakeXSpeed {};
        float snakeYSpeed {};

        bool grow {false};
};

#endif 