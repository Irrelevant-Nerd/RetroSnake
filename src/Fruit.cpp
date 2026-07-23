#include "../include/Fruit.h"

Fruit::Fruit()
{
    fruit.setSize({GRID_SIZE, GRID_SIZE});
    fruit.setFillColor(sf::Color::Red);
    GenerateCoors();
}

void Fruit::GenerateCoors()
{
    fruitCoorsX = (Random::get(1, wWIDTH) / 2) / 20 * 20;
    fruitCoorsY = (Random::get(1, wHEIGHT) / 2) / 20 * 20;
    fruit.setPosition({static_cast<float>(fruitCoorsX), static_cast<float>(fruitCoorsY)});
}

void Fruit::Draw(sf::RenderWindow& window)
{
    window.draw(fruit);
}
