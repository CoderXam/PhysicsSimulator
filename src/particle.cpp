#include <SFML/Graphics.hpp>
#include "../include/particle.h"
#include <iostream>

TailPart::TailPart(float circleRadius, sf::Vector2f initialPosition, float initialAlpha)
    :alpha(50)
{
    circle.setRadius(circleRadius);
    circle.setPosition(initialPosition);
    circle.setFillColor(sf::Color(255, 255, 255, alpha));
}

Arrow::Arrow(sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Color arrowColor)
{
    points = sf::VertexArray(sf::Lines, 2);
    points[0].position = startPosition;
    points[1].position = endPosition;
    points[0].color = arrowColor;
    points[1].color = arrowColor;
}

Particle::Particle(sf::Vector2f initialPosition, sf::Vector2f initialVelocity)
    : position(initialPosition), velocity(initialVelocity)
{
    vertices = sf::VertexArray(sf::TriangleFan, 4);
    setVerticesPositions();
    vertices[0].color = sf::Color::Cyan;
    vertices[1].color = sf::Color::Cyan;
    vertices[2].color = sf::Color::Cyan;
    vertices[3].color = sf::Color::Blue;
    Arrow Varrow(initialPosition, initialPosition + initialVelocity, sf::Color().Yellow);
    Arrow Aarrow(initialPosition, initialPosition, sf::Color().Red);
    arrows.push_back(Varrow);
    arrows.push_back(Aarrow);
    locator = sf::CircleShape(12, 3);
}

void Particle::Move(float timeInterval)
{
    velocity += acceleration * timeInterval;
    position += velocity * timeInterval;
}

void Particle::setVerticesPositions()
{
    vertices[0].position = position + sf::Vector2f(10.f, 10.f);
    vertices[1].position = position + sf::Vector2f(10.f, -10.f);
    vertices[2].position = position + sf::Vector2f(-10.f, -10.f);
    vertices[3].position = position + sf::Vector2f(-10.f, 10.f);
}

void Particle::updateArrows()
{
    arrows[0].points[0].position = position;
    arrows[0].points[1].position = position + velocity;
    arrows[1].points[0].position = position;
    arrows[1].points[1].position = position + acceleration;
}

// 123
// 456
// 789
int Particle::findLocation(sf::Vector2f position, sf::RenderWindow& window)
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
    else return -1;
}

void Particle::toggleLocator(bool status)
{

}