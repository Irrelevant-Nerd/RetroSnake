#ifndef TEXT
#define TEXT

#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>

class Text
{
    public:
        Text();

        void Update(int score, int level, float fps);
        void Draw(sf::RenderWindow& window);

    private:
        sf::Font font;
        sf::Text scoreText;
        sf::Text levelText;
        sf::Text fpsText;
};

#endif 