#include "SnakeController.hpp"
#include "Button.hpp"
#include "GameAsset.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "Screen.hpp"
#include "SnakeConfig.hpp"
#include "SnakeGameRenderer.hpp"
#include "SnakeModel.hpp"
#include "utility.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

sf::Texture SnakeController::generateBackgroundGrid() {
    sf::VertexArray gridVertices(sf::PrimitiveType::Triangles,
        SnakeConfig::GRID_DIMENSIONS * SnakeConfig::GRID_DIMENSIONS * 6);
    int square_counter = 0;

    for (int row = 0; row < SnakeConfig::GRID_DIMENSIONS; row++) {
        std::vector<sf::Vector2f> columnValues;
        auto                      row_fl = static_cast<float>(row);
        for (int column = 0; column < SnakeConfig::GRID_DIMENSIONS; column++) {
            sf::Color square_color;
            if ((column + row) % 2 == 0) {
                square_color.r = 163;
                square_color.g = 201;
                square_color.b = 173;
            } else {
                square_color.r = 122;
                square_color.g = 163;
                square_color.b = 133;
            }

            auto         col_fl = static_cast<float>(column);
            sf::Vector2f top_left = {
                col_fl * SnakeConfig::GRID_SIZE, row_fl * SnakeConfig::GRID_SIZE
            };
            sf::Vector2f top_right{
                col_fl * SnakeConfig::GRID_SIZE + SnakeConfig::GRID_SIZE,
                row_fl * SnakeConfig::GRID_SIZE
            };
            sf::Vector2f bottom_left{
                col_fl * SnakeConfig::GRID_SIZE,
                row_fl * SnakeConfig::GRID_SIZE + SnakeConfig::GRID_SIZE
            };
            sf::Vector2f bottom_right{
                col_fl * SnakeConfig::GRID_SIZE + SnakeConfig::GRID_SIZE,
                row_fl * SnakeConfig::GRID_SIZE + SnakeConfig::GRID_SIZE
            };

            columnValues.push_back(top_left);

            gridVertices[square_counter * 6].position = top_left;
            gridVertices[square_counter * 6].color = square_color;
            gridVertices[square_counter * 6 + 1].position = top_right;
            gridVertices[square_counter * 6 + 1].color = square_color;
            gridVertices[square_counter * 6 + 2].position = bottom_right;
            gridVertices[square_counter * 6 + 2].color = square_color;

            gridVertices[square_counter * 6 + 3].position = top_left;
            gridVertices[square_counter * 6 + 3].color = square_color;
            gridVertices[square_counter * 6 + 4].position = bottom_right;
            gridVertices[square_counter * 6 + 4].color = square_color;
            gridVertices[square_counter * 6 + 5].position = bottom_left;
            gridVertices[square_counter * 6 + 5].color = square_color;

            square_counter++;
        }
    }

    sf::RenderTexture rendertex(SnakeConfig::WINDOW_DIMENSIONS);
    rendertex.draw(gridVertices);
    rendertex.display();
    
    sf::Texture texture = rendertex.getTexture();
    return texture;
}

GameAssets SnakeController::loadGameAssets()
{
    GameAssets assets;
    tryLoadFont(assets.font, "assets/ui-font.ttf");
    tryLoadTexture(assets.eyes, "assets/eyes.png");
    tryLoadTexture(assets.apple_texture, "assets/apple.png");
    tryLoadTexture(assets.gameover_logo, "assets/gameover.png");
    tryLoadTexture(assets.main_logo, "assets/logo.png");
    assets.background = generateBackgroundGrid();

    return assets;
}

void SnakeController::tryLoadFont(sf::Font& out, string filename)
{
    if (!out.openFromFile(filename)) {
        throw std::runtime_error("Asset not found: " + filename);
    }
}

void SnakeController::tryLoadTexture(sf::Texture& out, string filename)
{
    if (!out.loadFromFile(filename)) {
        throw std::runtime_error("Asset not found: " + filename);
    }
}

SnakeController::SnakeController() :
    m_assets(loadGameAssets()),
    m_window(sf::VideoMode(SnakeConfig::WINDOW_DIMENSIONS),
        SnakeConfig::GAME_TITLE, sf::Style::Titlebar | sf::Style::Close),
    m_renderer(m_assets),
    m_endScreen(m_assets.font, m_assets.gameover_logo, ""),
    m_startScreen(m_assets.font, m_assets.main_logo, "")
{
    // m_window.setVerticalSyncEnabled(true);
    m_window.setKeyRepeatEnabled(false);

    sf::View gridView(sf::FloatRect({0, 0}, {800, 800}));
    m_window.setView(gridView);

    Button playButton(
        SnakeConfig::BUTTON_1_POS, "PLAY AGAIN", m_assets.font, [this]() {
            m_model = SnakeModel();
            m_gameState = GameMode::GAME;
            m_renderer.update(
                {m_model.getPlayer().getBodyPositions(),
                    m_model.getPlayer().getPosition(),
                    m_model.getPlayer().getMoveDirection(),
                    m_model.getScore()});
        });
    m_endScreen.addButton(playButton);

    Button startButton(SnakeConfig::BUTTON_1_POS, "START", m_assets.font,
        [this]() { m_gameState = GameMode::GAME; });
    m_startScreen.addButton(startButton);

    Button quitButton(SnakeConfig::BUTTON_2_POS, "QUIT", m_assets.font,
        [this]() { m_window.close(); });
    m_endScreen.addButton(quitButton);
    m_startScreen.addButton(quitButton);
}

bool SnakeController::hasLost()
{
    const auto player = m_model.getPlayer();
    auto       headpos = player.getPosition();
    auto       bodypositions = player.getBodyPositions();

    const bool colliding =
        std::any_of(bodypositions.begin() + 1, bodypositions.end(),
            [headpos](BodyPos segment) { return segment.position == headpos; });

    if (colliding) return true;

    sf::Vector2i corner = {
        SnakeConfig::GRID_DIMENSIONS - 1, SnakeConfig::GRID_DIMENSIONS - 1
    };
    if (!pointInRect(headpos, {0, 0}, corner)) return true;

    return false;
}

void SnakeController::processGameEvents()
{
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* keyPressed =
                       event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
            case sf::Keyboard::Key::Left:
                addMoveToBuffer(Direction::LEFT);
                break;
            case sf::Keyboard::Key::Right:
                addMoveToBuffer(Direction::RIGHT);
                break;
            case sf::Keyboard::Key::Up:
                addMoveToBuffer(Direction::UP);
                break;
            case sf::Keyboard::Key::Down:
                addMoveToBuffer(Direction::DOWN);
                break;
            default:;
            }
        }
    }
}

void SnakeController::processMenuEvents(std::vector<Button>* buttons)
{
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* mouseEvent =
                       event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                sf::Vector2f clickCoords =
                    m_window.mapPixelToCoords(mouseEvent->position);
                for (Button button : *buttons) {
                    button.handleMouseClick(clickCoords);
                }
            }
        }
    }
}

void SnakeController::playSnake()
{
    processGameEvents();

    if (m_timer.getElapsedTime().asSeconds() > SnakeConfig::UPDATE_RATE) {
        if (!m_inputBuffer.empty()) {
            m_model.getPlayer().setDirection(*getNextDirection());
        }
        m_model.getPlayer().update();
        for (size_t i = 0; i < m_model.getFruitList().size(); i++) {
            if (m_model.getPlayer().getPosition() ==
                m_model.getFruitList().at(i)) {
                m_model.getPlayer().incrementLength();
                m_model.destroyFruitIndex(i);

                for (int i = 0; i < rand() % 3; i++) {
                    createFruit();
                }
            }
        }

        if (m_model.getFruitList().size() == 0) { createFruit(); }

        m_timer.restart();

        SnakeGameRenderer::GameState newGameState = {
            m_model.getPlayer().getBodyPositions(),
            m_model.getPlayer().getPosition(),
            m_model.getPlayer().getMoveDirection(),
            m_model.getScore(),
            m_model.getFruitList()
        };

        if (hasLost()) {
            m_gameState = GameMode::OVER;
            m_endScreen.setSubtext("Score: " + std::to_string(m_model.getScore()));
        } else if (m_model.getPlayer().getLength() ==
                   SnakeConfig::AVAILIABLE_SQUARES) {
            m_gameState = GameMode::OVER;
            m_endScreen.setSubtext("You Won!");
        } else {
            m_renderer.update(newGameState);
        }
    }


}

void SnakeController::createFruit()
{
    int filledSquares =
        m_model.getPlayer().getLength() + m_model.getFruitList().size() - 1;
    if (filledSquares < SnakeConfig::AVAILIABLE_SQUARES) {
        sf::Vector2i position{
            rand() % SnakeConfig::GRID_DIMENSIONS,
            rand() % SnakeConfig::GRID_DIMENSIONS
        };

        auto bodypositions = m_model.getPlayer().getBodyPositions();
        auto fruitList = m_model.getFruitList();
        while (std::any_of(fruitList.begin(), fruitList.end(),
                   [position](const sf::Vector2i& fruit_pos) {
                       return fruit_pos == position;
                   }) ||
               std::any_of(bodypositions.begin(), bodypositions.end(),
                   [position](BodyPos segment) {
                       return segment.position == position;
                   })) {
            position = {
                rand() % SnakeConfig::GRID_DIMENSIONS,
                rand() % SnakeConfig::GRID_DIMENSIONS
            };
        }

        m_model.createFruit(position);
    }
}

void SnakeController::playGame()
{
    while (m_window.isOpen()) {
        switch (m_gameState) {
        case GameMode::GAME:
            playSnake();
            m_window.clear(sf::Color::Black);
            m_window.draw(m_renderer);
            break;
        case GameMode::OVER:
            processMenuEvents(m_endScreen.getButtons());
            m_window.clear();
            m_window.draw(m_endScreen);
            break;
        case GameMode::START:
            processMenuEvents(m_startScreen.getButtons());
            m_window.clear();
            m_window.draw(m_startScreen);
            break;
        }
        m_window.display();
    }
}

void SnakeController::addMoveToBuffer(const Direction move)
{
    if (m_inputBuffer.size() < SnakeConfig::MOVE_QUEUE_SIZE) {
        if (m_inputBuffer.empty()) {
            if (const Direction player_direction =
                    m_model.getPlayer().getMoveDirection();
                player_direction != move &&
                player_direction != getOpposite(move)) {
                m_inputBuffer.push(move);
            }
        } else if (m_inputBuffer.back() != move &&
                   m_inputBuffer.back() != getOpposite(move)) {
            m_inputBuffer.push(move);
        }
    }
}

std::optional<Direction> SnakeController::getNextDirection()
{
    if (!m_inputBuffer.empty()) {
        Direction direction = m_inputBuffer.front();
        m_inputBuffer.pop();

        return direction;
    }

    return std::nullopt;
}
