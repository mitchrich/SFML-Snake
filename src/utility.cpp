#include <cmath>
#include "utility.hpp"
#include "SnakeConfig.hpp"

sf::Vector2f rotateVector(sf::Vector2f initialVector, sf::Angle rotation) {
    sf::Vector2f finalVector;
    float x = initialVector.x;
    float y = initialVector.y;
    finalVector.x = x*std::cos(rotation.asRadians()) - y*std::sin(rotation.asRadians());
    finalVector.y = x*std::sin(rotation.asRadians()) + y*std::cos(rotation.asRadians());

    return finalVector;
}

Direction getDirectionTo(const sf::Vector2i initial_grid, const sf::Vector2i final_grid) {
    const sf::Vector2i difference(static_cast<int>(final_grid.x) - initial_grid.x, static_cast<int>(final_grid.y) - initial_grid.y);

    if (difference.x > 0 && difference.y == 0) return Direction::RIGHT;
    if (difference.x < 0 && difference.y == 0) return Direction::LEFT;
    if (difference.y > 0 && difference.x == 0) return Direction::DOWN;
    if (difference.y < 0 && difference.x == 0) return Direction::UP;

    throw std::invalid_argument("No cardinal direction found. (diagonal or same point)");
}

Direction getOpposite(Direction direction) {
    switch (direction) {
    case (Direction::UP):
        return Direction::DOWN;
    case (Direction::DOWN):
        return Direction::UP;
    case (Direction::LEFT):
        return Direction::RIGHT;
    case (Direction::RIGHT):
        return Direction::LEFT;
    }

    throw std::invalid_argument("Invalid direction supplied");
}

sf::Vector2f directionToVector(Direction direction, float magnitude) {
    switch (direction) {
    case (Direction::UP):
        return sf::Vector2f{0, -magnitude};
    case (Direction::DOWN):
        return sf::Vector2f{0, magnitude};
    case (Direction::LEFT):
        return sf::Vector2f{-magnitude, 0};
    case (Direction::RIGHT):
        return sf::Vector2f{magnitude, 0};
    }

    throw std::invalid_argument("Invalid direction supplied");
}

sf::Angle directionToAngle(Direction direction) {
    switch (direction) {
    case Direction::UP:
        return sf::radians(-M_PI_2);
    case Direction::DOWN:
        return sf::radians(M_PI_2);
    case Direction::LEFT:
        return sf::radians(M_PI);
    case Direction::RIGHT:
        return sf::radians(0.f);
    }

    return {};
}

sf::Vector2f gridCoordinates(const sf::Vector2i position) {
    return {static_cast<float>(position.x) * SnakeConfig::GRID_SIZE, static_cast<float>(position.y)*SnakeConfig::GRID_SIZE};
}

sf::Vector2f gridCoordinates(
    const sf::Vector2i position,
    const SquareLocation location_in_square)
{
    sf::Vector2f coords = {static_cast<float>(position.x)*SnakeConfig::GRID_SIZE, static_cast<float>(position.y)*SnakeConfig::GRID_SIZE};
    switch (location_in_square)
    {
    case SquareLocation::TOP_LEFT:
        return coords;
    case SquareLocation::TOP_RIGHT:
        coords.x += SnakeConfig::GRID_SIZE;
        return coords;
    case SquareLocation::BOTTOM_LEFT:
        coords.y += SnakeConfig::GRID_SIZE;
        return coords;
    case SquareLocation::BOTTOM_RIGHT:
        coords.x += SnakeConfig::GRID_SIZE;
        coords.y += SnakeConfig::GRID_SIZE;
        return coords;
    case SquareLocation::TOP:
        coords.x += SnakeConfig::GRID_SIZE/2;
        return coords;
    case SquareLocation::BOTTOM:
        coords.x += SnakeConfig::GRID_SIZE/2;
        coords.y += SnakeConfig::GRID_SIZE;
        return coords;
    case SquareLocation::LEFT:
        coords.y += SnakeConfig::GRID_SIZE/2;
        return coords;
    case SquareLocation::RIGHT:
        coords.x += SnakeConfig::GRID_SIZE;
        coords.y += SnakeConfig::GRID_SIZE/2;
        return coords;
    case SquareLocation::CENTER:
        coords.x += SnakeConfig::GRID_SIZE/2;
        coords.y += SnakeConfig::GRID_SIZE/2;
        return coords;
    }

    throw std::invalid_argument("Invalid location in square.");
}

void shiftPosition(sf::Vector2i& initialpos, const Direction direction) {
    switch (direction) {
    case (Direction::UP):
        initialpos.y -= 1;
        break;
    case (Direction::DOWN):
        initialpos.y += 1;
        break;
    case (Direction::LEFT):
        initialpos.x -= 1;
        break;
    case (Direction::RIGHT):
        initialpos.x += 1;
        break;
    }
}

bool pointInRect(sf::Vector2i point, sf::Vector2i pos1, sf::Vector2i pos2)
{
    const int max_x = std::max(pos1.x, pos2.x);
    const int min_x = std::min(pos1.x, pos2.x);
    const int max_y = std::max(pos1.y, pos2.y);
    const int min_y = std::min(pos1.y, pos2.y);

    if (
        point.x <= max_x
        && point.x >= min_x
        && point.y <= max_y
        && point.y >= min_y
    )
    {
        return true;
    } else
    {
        return false;
    }
}
