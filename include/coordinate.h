#pragma once

sf::Vector2f screenToWorld(const sf::Vector2f& screenPos);
sf::Vector2f worldToScreen(float x, float y);
int locate(sf::Vector2f position, sf::RenderWindow& window);
