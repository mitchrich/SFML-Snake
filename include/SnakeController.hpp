#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <queue>
#include <vector>
#include "SFML/Graphics/Texture.hpp"
#include "SnakeModel.hpp"
#include "Button.hpp"
#include "SnakeGameRenderer.hpp"
#include "Screen.hpp"
#include "GameAsset.hpp"

class SnakeController
{
public:
    SnakeController();
    bool hasLost();
    void playSnake();
    void playGame();

private:
    GameAssets m_assets;
    SnakeModel m_model;
    std::queue<Direction> m_inputBuffer;
    sf::RenderWindow m_window;
    SnakeGameRenderer m_renderer;
    sf::Clock m_timer;
    Screen m_endScreen;
    Screen m_startScreen;
    GameMode m_gameState = GameMode::START;

    void addMoveToBuffer(Direction move);
    std::optional<Direction> getNextDirection();
    void processGameEvents();
    void processMenuEvents(std::vector<Button>* buttons);
    void createFruit();
    GameAssets loadGameAssets();
    void tryLoadFont(sf::Font& out, string filename);
    void tryLoadTexture(sf::Texture& out, string filename);
};
