#include "../include/Fruit.h"

Fruit::Fruit()
{
    fruit.setSize({GRID_SIZE, GRID_SIZE});
    fruit.setFillColor(sf::Color::Red);
    GenerateCoors();
}

void Fruit::GenerateCoors()
{
    fruitCoorsX = Random::get(0, wWIDTH / GRID_SIZE - 1) * GRID_SIZE;
    fruitCoorsY = Random::get(0, wHEIGHT / GRID_SIZE - 1) * GRID_SIZE;
    fruit.setPosition({static_cast<float>(fruitCoorsX), static_cast<float>(fruitCoorsY)});
}

void Fruit::Draw(sf::RenderWindow& window)
{
    window.draw(fruit);
}
