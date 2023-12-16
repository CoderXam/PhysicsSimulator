#include <math.h>
#include <SFML/Graphics.hpp>

float getMagnitude(sf::Vector2f& vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float getDirection(sf::Vector2f& vector)
{
	return atan2(vector.y , vector.x);
}