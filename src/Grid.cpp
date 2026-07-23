#include "../include/Grid.h"

Grid::Grid()
{
    gridRow.setSize({wWIDTH, LINE_THICKNESS});
    gridRow.setFillColor(sf::Color(128, 128, 128));

    gridColumn.setSize({LINE_THICKNESS, wHEIGHT});
    gridColumn.setFillColor(sf::Color(128, 128, 128));
}

void Grid::Draw(sf::RenderWindow& window)
{
    for(int i{1}; i <= (wHEIGHT / GRID_SIZE); i++)
    {
        gridRow.setPosition({0.f, static_cast<float>((i)*GRID_SIZE)});
        window.draw(gridRow);
    }
    for(int j{1}; j <= (wWIDTH / GRID_SIZE); j++)
    {
        gridColumn.setPosition({static_cast<float>((j)*GRID_SIZE), 0.f});
        window.draw(gridColumn);
    }
}