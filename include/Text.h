#ifndef TEXT
#define TEXT

#include "Constants.h"
#include <SFML/Graphics.hpp>

class Text
{
    public:
        Text();

        void Update(int score, int level);
        void Draw(sf::RenderWindow& window);

    private:
        sf::Font font;
        sf::Text scoreText;
        sf::Text levelText;
};

#endif 