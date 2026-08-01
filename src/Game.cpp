#include "../include/Game.h"

Game::Game():
    window(sf::VideoMode({wWIDTH, wHEIGHT}), "Retro Snake")
{
    window.setFramerateLimit(60);
}

void Game::Run()
{
    while(window.isOpen())
    {
        HandleEvents();

        deltaTime = frameClock.restart().asSeconds();
        fps = 1.f / deltaTime;

        Render();

        if(CheckBorderBounds() || CheckHitSnakeBody(snake.getSnakePosition()))
        {
            sf::sleep(sf::seconds(2)); // sleeps for 2 sec before the window close
            window.close();
        }

        window.clear(sf::Color::Black);
        Draw();
        window.display();
    }
}

void Game::HandleEvents()
{
    while(std::optional event = window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::H)
            {
                showText = !showText;
            }
        }
    }

}

void Game::Render()
{
    // text property
    text.Update(score, level, fps);

    // snake properties
    snake.Direction();
    snake.Move(moveDelay);
    if(snake.Eat(fruit.getFruitPosition()))
    {
        score+=1;
        LevelUp();
        ValidateFruitCoors();
    }
}

void Game::ValidateFruitCoors()
{
    do
    {
        fruit.GenerateCoors();
    }
    while(fruit.getFruitPosition() == snake.getSnakePosition() || CheckHitSnakeBody(fruit.getFruitPosition()));
}

void Game::LevelUp()
{
    if(score >= (level) * 20)
    {
        level+=1;
        if(moveDelay > 0.05f) // we dont want the snake to be way too fast
        {
            moveDelay -= 0.02f;
        }
    }
}

void Game::Draw()
{
    snake.Draw(window);
    fruit.Draw(window);
    grid.Draw(window);
    if(showText)
        DrawText();
}

void Game::DrawText()
{
    text.Draw(window);
}


bool Game::CheckBorderBounds()
{
    if(snake.getSnakePosition().x < 0 || snake.getSnakePosition().x >= wWIDTH)
    {
        return true;
    }
    if(snake.getSnakePosition().y < 0 || snake.getSnakePosition().y >= wHEIGHT)
    {
        return true;
    }
    return false;
}

bool Game::CheckHitSnakeBody(sf::Vector2f shapePos)
{
    for(auto& pos : snake.getTailPosition())
    {
        if(shapePos == pos)
        {
            return true;
        }
    }
    return false;
}

