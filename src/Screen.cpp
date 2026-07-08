#include "Screen.hpp"
#include "Button.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SnakeConfig.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

Screen::Screen(sf::Font& font, sf::Texture& logo, std::string subtext) :
    m_logo(logo),
    m_subtext(font, subtext)
{
    auto logoCenter = m_logo.getLocalBounds().getCenter();
    m_logo.setOrigin(logoCenter);
    m_logo.setPosition(SnakeConfig::LOGO_POSITION);

    auto subtextCenter = m_subtext.getLocalBounds().getCenter();
    m_subtext.setOrigin(subtextCenter);
    m_subtext.setPosition(SnakeConfig::SUBTEXT_POSITION);
}
void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_logo);
    target.draw(m_subtext);

    for (auto button : m_buttons) {
        target.draw(button);
    }
}

void Screen::addButton(Button button) {
    m_buttons.push_back(button);
}

void Screen::setSubtext(std::string newText) {
    m_subtext.setString(newText);
    auto subtextCenter = m_subtext.getLocalBounds().getCenter();
    m_subtext.setOrigin(subtextCenter);
}

std::vector<Button>* Screen::getButtons() {return &m_buttons;}
