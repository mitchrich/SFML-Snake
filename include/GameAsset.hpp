#pragma once

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>
#include <string>

using std::string;

struct GameAssets {
    sf::Font    font;
    sf::Texture eyes;
    sf::Texture apple_texture;
    sf::Texture gameover_logo;
    sf::Texture main_logo;
    sf::Texture background;
};

