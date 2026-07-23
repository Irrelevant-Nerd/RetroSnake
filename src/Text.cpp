#include "../include/Text.h"

Text::Text()
    : font("../assets/fonts/HomeVideo-BLG6G.ttf")
    , scoreText(font)
    , levelText(font)
{
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({wWIDTH-635, wHEIGHT-55});

    levelText.setCharacterSize(25);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition({wWIDTH-635, wHEIGHT-25});
}

void Text::Update(int score, int level)
{
    scoreText.setString("SCORE: "+std::to_string(score));
    levelText.setString("LEVEL: "+std::to_string(level));
}

void Text::Draw(sf::RenderWindow& window)
{
    window.draw(scoreText);
    window.draw(levelText);
}

