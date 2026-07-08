#pragma once

#include "GameAsset.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <deque>
#include <vector>
#include "utility.hpp"

class SnakeGameRenderer final : public sf::Drawable {
    public:
        struct GameState {
            std::deque<BodyPos> segmentPositions;
            sf::Vector2i headPos;
            Direction direction;
            int score;
        };
        
        SnakeGameRenderer(GameAssets& assetHandler);
        void update(const GameState& newGameState);
        void draw(sf::RenderTarget& target, const sf::RenderStates states) const;
        void createFruitSprite(sf::Vector2i position);
        void destroyFruitSprite(int index);
    private:
        ////Constants
        //
        static const sf::Color PLAYER_COLOR;
        static constexpr float PLAYER_WIDTH = 40;
        static constexpr int TURN_RESOLUTION = 5;

        ////Other Methods
        //
        sf::Vector2f getHeadCenter(sf::Vector2i headPos) const;
        
        ////Sprite Generation Methods
        //
        std::vector<sf::Vector2f> calcWidthVertex(sf::Vector2f position, sf::Angle angle) const;
        void addVertices(const std::vector<sf::Vector2f>& points);
        std::vector<sf::Vector2f> generateCircleVertices(
            sf::Vector2i initialPos, 
            Direction initialDirection,
            Direction finalDirection) const;
        void generateGridVertices();
        void updateTail(std::deque<BodyPos> bodyPositions);

        ////Draw Methods
        //
        void drawFruit(sf::RenderTarget& window, const sf::RenderStates& states) const;
        void drawPlayer(sf::RenderTarget& target, const sf::RenderStates states) const;
        
        ////General
        //
        GameAssets& m_assetHandler;

        ////Sprites
        //
        sf::Text m_scoreCounter;
        sf::VertexArray m_gridVertices;
        sf::CircleShape m_headSprite;
        sf::CircleShape m_endSprite;
        sf::Sprite m_eyeSprite;
        sf::VertexArray m_tailStrip;
        std::vector<sf::Sprite> m_fruitSpriteList{};
        std::vector<sf::Sprite> fruitSprites;
};
