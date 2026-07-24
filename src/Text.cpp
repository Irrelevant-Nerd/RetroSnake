#include "../include/Text.h"

Text::Text()
    : font("../assets/fonts/HomeVideo-BLG6G.ttf")
    , scoreText(font)
    , levelText(font)
    , fpsText(font)
{
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({wWIDTH-635, wHEIGHT-55});

    levelText.setCharacterSize(25);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition({wWIDTH-635, wHEIGHT-25});

    fpsText.setCharacterSize(25);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition({wWIDTH-170, wHEIGHT-470});
}

void Text::Update(int score, int level, float fps)
{
    scoreText.setString("SCORE: "+std::to_string(score));
    levelText.setString("LEVEL: "+std::to_string(level));

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << fps;
    fpsText.setString("FPS: "+ss.str());
}

void Text::Draw(sf::RenderWindow& window)
{
    window.draw(scoreText);
    window.draw(levelText);
    window.draw(fpsText);
}

