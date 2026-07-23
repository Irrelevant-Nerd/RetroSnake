#ifndef GRID
#define GRID

#include "Constants.h"
#include <SFML/Graphics.hpp>

class Grid
{
    public:
        Grid();
        void Draw(sf::RenderWindow& window);
        
    private:
        sf::RectangleShape gridRow;
        sf::RectangleShape gridColumn;

        const float LINE_THICKNESS {1.5f};
};

#endif