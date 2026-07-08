#pragma once

#include "GameAsset.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Vector2.hpp"
#include "utility.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <deque>
#include <vector>

class SnakeGameRenderer final : public sf::Drawable {
  public:
    struct GameState {
        std::deque<BodyPos>       segmentPositions;
        sf::Vector2i              headPos;
        Direction                 direction;
        int                       score;
        std::vector<sf::Vector2i> fruitPos;
    };

    SnakeGameRenderer(GameAssets& assetHandler);
    void update(const GameState& newGameState);
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const;

  private:
    ////Constants
    //
    static const sf::Color PLAYER_COLOR;
    static constexpr float PLAYER_WIDTH = 40;
    static constexpr int   TURN_RESOLUTION = 5;

    ////Other Methods
    //
    sf::Vector2f getHeadCenter(sf::Vector2i headPos) const;
    sf::Sprite createFruitSprite();

    ////Sprite Generation Methods
    //
    std::vector<sf::Vector2f> calcWidthVertex(
        sf::Vector2f position, sf::Angle angle) const;
    void addVertices(const std::vector<sf::Vector2f>& points);
    std::vector<sf::Vector2f> generateCircleVertices(sf::Vector2i initialPos,
        Direction initialDirection, Direction finalDirection) const;
    void                      generateGridVertices();
    void                      updateTail(std::deque<BodyPos> bodyPositions);

    ////Draw Methods
    //
    void drawFruit(
        sf::RenderTarget& window, const sf::RenderStates& states) const;
    void drawPlayer(
        sf::RenderTarget& target, const sf::RenderStates states) const;

    ////General
    //
    GameAssets& m_assetHandler;
    std::vector<sf::Vector2i> m_fruitPos;
    const sf::Sprite      m_fruitSprite;

    ////Sprites
    //
    sf::VertexArray m_gridVertices;
    sf::VertexArray m_tailStrip;
    sf::CircleShape m_headSprite;
    sf::CircleShape m_endSprite;
    sf::Sprite      m_eyeSprite;
    sf::Text        m_scoreCounter;
};
