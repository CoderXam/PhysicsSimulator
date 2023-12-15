#include <SFML/Graphics.hpp>

//world space is the coordinate system used for calculations in the code
//the origin is in the center, right and up are positive
sf::Vector2f screenToWorld(const sf::Vector2f& screenPos) {
    float x = screenPos.x - 400;
    float y = -(screenPos.y - 300);
    return sf::Vector2f(x, y);
}
//screen space is the coordinate system used by sfml
//the origin is in the top left, right and down are positive
sf::Vector2f worldToScreen(float x, float y) {
    float screenX = x + 400;
    float screenY = -y + 300;
    return sf::Vector2f(screenX, screenY);
}