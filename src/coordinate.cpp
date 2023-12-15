#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/coordinate.h"

//world space is the coordinate system used for calculations in the code
//the origin is in the center, right and up are positive
sf::Vector2f screenToWorld(const sf::Vector2f& screenPos) 
{
    float x = screenPos.x - 400;
    float y = -(screenPos.y - 300);
    return sf::Vector2f(x, y);
}
//screen space is the coordinate system used by sfml
//the origin is in the top left, right and down are positive
sf::Vector2f worldToScreen(float x, float y) 
{
    float screenX = x + 400;
    float screenY = -y + 300;
    return sf::Vector2f(screenX, screenY);
}

// returns the location of the vector relative to the window as an index of a 3x3 matrix
// 1 2 3
// 4 5 6
// 7 8 9
int locate(sf::Vector2f position, sf::RenderWindow& window)
{
    if (position.x < 0 && position.y < 0)
        return 1;
    else if (position.y < 0 && position.x < window.getSize().x)
        return 2;
    else if (position.x > window.getSize().x && position.y < 0)
        return 3;
    else if (position.x < 0 && position.y < window.getSize().y)
        return 4;
    else if (position.x >= 0 && position.x <= window.getSize().x && position.y >= 0 && position.y <= window.getSize().y)
        return 5;
    else if (position.x > window.getSize().x && position.y < window.getSize().y)
        return 6;
    else if (position.x < 0 && position.y > window.getSize().y)
        return 7;
    else if (position.y > window.getSize().y && position.x < window.getSize().x)
        return 8;
    else if (position.x > window.getSize().x && position.y > window.getSize().y)
        return 9;
    else 
        std::cout << "Error: could not locate" << std::endl; 
        return -1;
}