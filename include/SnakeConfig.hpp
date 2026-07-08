#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

using std::string;

namespace SnakeConfig {

static constexpr float        UPDATE_RATE = 0.15;
static constexpr unsigned int WINDOW_SIZE = 800;
static constexpr float        GRID_SIZE = WINDOW_SIZE / 15.0;
static constexpr unsigned int MOVE_QUEUE_SIZE = 2;
static constexpr int GRID_DIMENSIONS = 15;
static constexpr int          AVAILIABLE_SQUARES = GRID_DIMENSIONS * GRID_DIMENSIONS;
constexpr sf::Vector2u WINDOW_DIMENSIONS = {800, 800};
const char             GAME_TITLE[] = "Snake Game";

constexpr sf::Vector2f BUTTON_1_POS = {
    static_cast<float>(SnakeConfig::WINDOW_SIZE) / 2,
    2 * static_cast<float>(SnakeConfig::WINDOW_SIZE) / 3
};

constexpr sf::Vector2f BUTTON_2_POS = {
    static_cast<float>(SnakeConfig::WINDOW_SIZE) / 2,
    2 * static_cast<float>(SnakeConfig::WINDOW_SIZE) / 3 + 50
};
constexpr sf::Vector2f LOGO_POSITION{static_cast<float>(WINDOW_SIZE) / 2, 200};

constexpr sf::Vector2f SUBTEXT_POSITION{
    static_cast<float>(WINDOW_SIZE) / 2, 300
};

constexpr sf::Vector2f SCORE_POSITION{static_cast<float>(WINDOW_SIZE) / 2, 20};
} // namespace SnakeConfig
