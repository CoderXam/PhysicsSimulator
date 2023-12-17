#include <SFML/Graphics.hpp>
#include "../include/particle.h"
#include <iostream>
#include "../include/coordinate.h"

TailPart::TailPart(float circleRadius, sf::Vector2f initialPosition, float initialAlpha)
    :alpha(50)
{
    circle.setRadius(circleRadius);
    circle.setPosition(initialPosition);
    circle.setFillColor(sf::Color(255, 255, 255, alpha));
}

Particle::Arrow::Arrow(sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Color arrowColor)
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

void Particle::draw(sf::RenderWindow& window)
{
    for (auto& tailPart : tail)
    {
        tailPart.circle.setPosition(worldToScreen(tailPart.circle.getPosition().x, tailPart.circle.getPosition().y));
        window.draw(tailPart.circle);
        tailPart.circle.setPosition(screenToWorld(tailPart.circle.getPosition()));
    }

    for (int i = 0; i < vertices.getVertexCount(); i++)
    {
        vertices[i].position = worldToScreen(vertices[i].position.x, vertices[i].position.y);
    }
    window.draw(vertices);
    for (int i = 0; i < vertices.getVertexCount(); i++)
    {
        vertices[i].position = screenToWorld(vertices[i].position);
    }

    for (int i = 0; i < arrows.size(); i++)
    {
        arrows[i].points[0].position = worldToScreen(arrows[i].points[0].position.x, arrows[i].points[0].position.y);
        arrows[i].points[1].position = worldToScreen(arrows[i].points[1].position.x, arrows[i].points[1].position.y);
        window.draw(arrows[i].points);
        arrows[i].points[0].position = screenToWorld(arrows[i].points[0].position);
        arrows[i].points[1].position = screenToWorld(arrows[i].points[1].position);
    }
}

// Cursed c++ (ChatGPT wrote this i have no clue what it means)
bool Particle::operator==(const Particle& other) const {
    return position == other.position && position == other.position;
}