#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

class Button final : public sf::Drawable
{
public:
    Button(sf::Vector2f position, std::string text, sf::Font& font, std::function<void()> function);
    void handleMouseClick(sf::Vector2f mousePos) const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::Text m_text;
    const std::function<void()> m_function;
};
