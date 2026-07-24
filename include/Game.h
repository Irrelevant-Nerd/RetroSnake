#ifndef GAME
#define GAME

#include "Constants.h"
#include "Grid.h"
#include "Snake.h"
#include "Fruit.h"
#include "Text.h"
#include <SFML/Graphics.hpp>

class Game{
    public:
        sf::RenderWindow window;

        Grid grid;
        Snake snake;
        Fruit fruit;
        Text text;

    public:
        Game();
        void Run();

    private:
        void HandleEvents();
        void Render();
        void Draw();
        
        void ValidateFruitCoors();
        bool CheckBorderBounds();
        bool CheckHitSnakeBody(sf::Vector2f shapePos);
        void LevelUp();

    private:
        sf::Clock frameClock; // for displaying fps

        float deltaTime;
        float fps;

        float moveDelay {0.15f};
        int score {0};      
        int level {1};
};

#endif