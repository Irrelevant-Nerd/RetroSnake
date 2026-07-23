#ifndef FRUIT
#define FRUIT

#include "Constants.h"
#include "Random.h"
#include <SFML/Graphics.hpp>
#include <list>

class Fruit
{
    public:
        Fruit();
        void GenerateCoors();

        void Draw(sf::RenderWindow& window);
    public:
        sf::Vector2f getFruitPosition(){ return fruit.getPosition(); }
    
    private:
        sf::RectangleShape fruit;

        int fruitCoorsX {};
        int fruitCoorsY {};
};

#endif