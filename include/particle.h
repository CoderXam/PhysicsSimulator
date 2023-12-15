#pragma once

struct TailPart
{
    sf::CircleShape circle;
    float alpha;

    TailPart(float circleRadius, sf::Vector2f initialPosition, float initialAlpha);
};

class Arrow
{
public:
    sf::VertexArray points;

    Arrow(sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Color arrowColor);
};

class Particle
{
public:
    sf::VertexArray vertices;
    sf::Vector2f position, velocity, acceleration;
    std::vector<TailPart> tail;
    std::vector<Arrow> arrows;
    sf::CircleShape locator;
    static bool arrowsVisible;
    static bool locatorsActive;

    Particle(sf::Vector2f initialPosition, sf::Vector2f initialVelocity);
    void Move(float timeInterval);
    void setVerticesPositions();
    void updateArrows();
    void toggleLocator(bool status);
};