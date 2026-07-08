#include "SnakeGameRenderer.hpp"
#include "GameAsset.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SnakeConfig.hpp"
#include "utility.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <deque>

const sf::Color SnakeGameRenderer::PLAYER_COLOR(245, 66, 129);

SnakeGameRenderer::SnakeGameRenderer(GameAssets& assetHandler) :
    m_assetHandler(assetHandler), m_fruitSprite(createFruitSprite()),
    m_background(m_assetHandler.background),
    m_eyeSprite(assetHandler.eyes), m_scoreCounter(assetHandler.font)
{
    auto counterCenter = m_scoreCounter.getLocalBounds().getCenter();
    m_scoreCounter.setOrigin(counterCenter);
    m_scoreCounter.setPosition(SnakeConfig::SCORE_POSITION);

    m_headSprite.setOrigin({PLAYER_WIDTH / 2, PLAYER_WIDTH / 2});
    m_headSprite.setRadius(PLAYER_WIDTH / 2);
    m_headSprite.setFillColor(PLAYER_COLOR);
    sf::Vector2f      eyeBounds = m_eyeSprite.getLocalBounds().size;
    const sf::Texture eyeTexture = m_eyeSprite.getTexture();
    float             eyeScaleX = (PLAYER_WIDTH + PLAYER_WIDTH / 3) /
                                  static_cast<float>(eyeTexture.getSize().x);
    float             eyeScaleY = (PLAYER_WIDTH + PLAYER_WIDTH / 3) /
                                  static_cast<float>(eyeTexture.getSize().y);
    sf::Vector2f      eyeScale = {eyeScaleX, eyeScaleY};
    m_eyeSprite.setOrigin({eyeBounds.x / 2, eyeBounds.y / 2});
    m_eyeSprite.scale(eyeScale);

    // Tail
    m_tailStrip.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_endSprite.setRadius(PLAYER_WIDTH / 2);
    m_endSprite.setFillColor(PLAYER_COLOR);
    m_endSprite.setOrigin({PLAYER_WIDTH / 2, PLAYER_WIDTH / 2});
}

void SnakeGameRenderer::update(const GameState& newGameState)
{
    m_headSprite.setPosition(getHeadCenter(newGameState.headPos));
    m_eyeSprite.setPosition(getHeadCenter(newGameState.headPos));
    m_eyeSprite.setRotation(directionToAngle(newGameState.direction));

    m_scoreCounter.setString("Score: " + std::to_string(newGameState.score));
    auto counterCenter = m_scoreCounter.getLocalBounds().getCenter();
    m_scoreCounter.setOrigin(counterCenter);
    m_fruitPos = newGameState.fruitPos;

    updateTail(newGameState.segmentPositions);
}

void SnakeGameRenderer::draw(
    sf::RenderTarget& target, const sf::RenderStates states) const
{
    target.draw(m_background);
    drawFruit(target, states);
    drawPlayer(target, states);
    target.draw(m_scoreCounter);
}

void SnakeGameRenderer::drawPlayer(
    sf::RenderTarget& target, const sf::RenderStates states) const
{
    target.draw(m_headSprite, states);
    target.draw(m_endSprite, states);
    target.draw(m_tailStrip, states);
    target.draw(m_eyeSprite, states);
}

void SnakeGameRenderer::drawFruit(
    sf::RenderTarget& window, const sf::RenderStates& states) const
{
    auto sprite = m_fruitSprite;
    for (sf::Vector2i pos : m_fruitPos) {
        sprite.setPosition(gridCoordinates(pos));
        window.draw(sprite, states);
    };
}

sf::Sprite SnakeGameRenderer::createFruitSprite()
{
    sf::Sprite   sprite(m_assetHandler.apple_texture);
    sf::Vector2f scale = {
        SnakeConfig::GRID_SIZE /
            static_cast<float>(sprite.getTexture().getSize().x),
        SnakeConfig::GRID_SIZE /
            static_cast<float>(sprite.getTexture().getSize().y)
    };
    sprite.scale(scale);
    return sprite;
}

std::vector<sf::Vector2f> SnakeGameRenderer::calcWidthVertex(
    sf::Vector2f position, sf::Angle angle) const
{
    // Get a direction vector of the angle and stretch to the length of width
    sf::Vector2f initialVector;
    initialVector.x = PLAYER_WIDTH / 2 * std::cos(angle.asRadians());
    initialVector.y = PLAYER_WIDTH / 2 * std::sin(angle.asRadians());

    // Rotate the initial vector to both sides
    sf::Vector2f rotation1 = rotateVector(initialVector, sf::radians(M_PI_2));
    sf::Vector2f rotation2 = rotateVector(initialVector, sf::radians(-M_PI_2));

    // Apply transformation to the initial position supplied
    sf::Vector2f pos1{position.x + rotation1.x, position.y + rotation1.y};
    sf::Vector2f pos2{position.x + rotation2.x, position.y + rotation2.y};

    return std::vector<sf::Vector2f>{pos1, pos2};
}

void SnakeGameRenderer::updateTail(const std::deque<BodyPos> bodyPositions)
{
    m_tailStrip.clear();

    Direction previousDirection = getOpposite(bodyPositions[0].direction);
    for (size_t i = 0; i < bodyPositions.size() - 1; i++) {
        const sf::Vector2i position = bodyPositions[i].position;
        const Direction    direction = getOpposite(bodyPositions[i].direction);

        if (previousDirection != direction) {
            addVertices(
                generateCircleVertices(position, previousDirection, direction));
        } else {
            addVertices(calcWidthVertex(
                gridCoordinates(position, SquareLocation::CENTER),
                directionToAngle(direction)));
        }
        previousDirection = getOpposite(bodyPositions[i].direction);
    }

    sf::Vector2i endpos = bodyPositions[bodyPositions.size() - 1].position;
    addVertices(calcWidthVertex(gridCoordinates(endpos, SquareLocation::CENTER),
        directionToAngle(previousDirection)));
    m_endSprite.setPosition(gridCoordinates(endpos, SquareLocation::CENTER));
}

void SnakeGameRenderer::addVertices(const std::vector<sf::Vector2f>& points)
{
    for (sf::Vector2f point : points) {
        m_tailStrip.append(sf::Vertex({point, PLAYER_COLOR}));
    }
}

std::vector<sf::Vector2f> SnakeGameRenderer::generateCircleVertices(
    sf::Vector2i initialPos, Direction initialDirection,
    Direction finalDirection) const
{
    std::vector<sf::Vector2f> circleVertices;
    sf::Angle                 differenceAngle =
        directionToAngle(finalDirection) - directionToAngle(initialDirection);
    differenceAngle = differenceAngle.wrapSigned();

    SquareLocation
        potentialOrigin[2]; // First entry is for if the user turns left, second
                            // is for if they turn right.
    switch (initialDirection) {
    case Direction::RIGHT:
        potentialOrigin[0] = SquareLocation::TOP_LEFT;
        potentialOrigin[1] = SquareLocation::BOTTOM_LEFT;
        break;
    case Direction::LEFT:
        potentialOrigin[0] = SquareLocation::BOTTOM_RIGHT;
        potentialOrigin[1] = SquareLocation::TOP_RIGHT;
        break;
    case Direction::UP:
        potentialOrigin[0] = SquareLocation::BOTTOM_LEFT;
        potentialOrigin[1] = SquareLocation::BOTTOM_RIGHT;
        break;
    case Direction::DOWN:
        potentialOrigin[0] = SquareLocation::TOP_RIGHT;
        potentialOrigin[1] = SquareLocation::TOP_LEFT;
        break;
    default:
        throw std::invalid_argument("Invalid direction.");
    }

    sf::Vector2f origin;
    sf::Angle    initialAngle;
    if (differenceAngle.asRadians() < 0) {
        // Left/Counter-Clockwise

        initialAngle = directionToAngle(initialDirection) + sf::radians(M_PI_2);
        origin = gridCoordinates(initialPos, potentialOrigin[0]);
    } else {
        // Right/Clockwise

        initialAngle = directionToAngle(initialDirection) - sf::radians(M_PI_2);
        origin = gridCoordinates(initialPos, potentialOrigin[1]);
    }

    for (int i = 1; i < TURN_RESOLUTION + 1; i++) {
        sf::Angle currentAngle =
            initialAngle +
            static_cast<float>(i) * (differenceAngle / (TURN_RESOLUTION + 1));
        const float rise =
            (SnakeConfig::GRID_SIZE / 2) * std::sin(currentAngle.asRadians());
        const float run =
            (SnakeConfig::GRID_SIZE / 2) * std::cos(currentAngle.asRadians());

        const sf::Vector2f pointPosition = {origin.x + run, origin.y + rise};
        float              perpanGradient = -run / rise;
        sf::Angle          angle = sf::radians(std::atan(perpanGradient));
        angle = initialDirection == Direction::LEFT ||
                        finalDirection == Direction::LEFT
                    ? angle - sf::radians(M_PI)
                    : angle;

        std::vector<sf::Vector2f> vertices =
            calcWidthVertex(pointPosition, angle);
        circleVertices.push_back(vertices[0]);
        circleVertices.push_back(vertices[1]);
    }

    return circleVertices;
}

sf::Vector2f SnakeGameRenderer::getHeadCenter(sf::Vector2i headPos) const
{ return gridCoordinates(headPos, SquareLocation::CENTER); }
