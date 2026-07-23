#include "../include/Snake.h"

Snake::Snake()
{
    // head
    snake.setSize({GRID_SIZE, GRID_SIZE});
    snake.setFillColor(sf::Color::Green);
    snake.setPosition({(wWIDTH/2) / 20 * 20, (wHEIGHT/2) / 20 * 20});

    // segments
    segment.setSize({GRID_SIZE, GRID_SIZE});
    segment.setFillColor(sf::Color::Green);
}

void Snake::Direction()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        snakeXSpeed = 0.f;
        snakeYSpeed = -SPEED;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        snakeXSpeed = 0.f;
        snakeYSpeed = SPEED;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        snakeXSpeed = -SPEED;
        snakeYSpeed = 0.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        snakeXSpeed = SPEED;
        snakeYSpeed = 0.f;
    }
}

void Snake::Move(float moveDelay)
{
    if (clock.getElapsedTime().asSeconds() >= moveDelay)
    {
        if(snakeXSpeed != 0 || snakeYSpeed != 0)
        {
            sf::Vector2f oldHeadPos = snake.getPosition();
            
            snake.move({snakeXSpeed, snakeYSpeed});

            tail.push_front(oldHeadPos);

            if(!grow)
            {
                tail.pop_back();
            }
            else
            {
                grow = false;
            }
            
        }
        clock.restart();
    };
}

bool Snake::Eat(sf::Vector2f fruitPosition)
{
    if(snake.getPosition().x == fruitPosition.x && snake.getPosition().y == fruitPosition.y)
    {
        grow = true;
        return true;
    }
    return false;
}

void Snake::Draw(sf::RenderWindow& window)
{
    window.draw(snake);
    for (const auto& pos : tail)
    {
        segment.setPosition(pos);
        window.draw(segment);
    }
}

