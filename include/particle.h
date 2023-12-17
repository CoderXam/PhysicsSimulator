#pragma once

struct TailPart
{
    sf::CircleShape circle;
    float alpha;

    TailPart(float circleRadius, sf::Vector2f initialPosition, float initialAlpha);
};


class Particle
{
public:
    class Arrow
    {
    public:
        sf::VertexArray points;

        Arrow(sf::Vector2f startPosition, sf::Vector2f endPosition, sf::Color arrowColor);
    };

    sf::VertexArray vertices;
    sf::Vector2f position, velocity, acceleration;
    std::vector<TailPart> tail;
    int maxTailSize;
    std::vector<Arrow> arrows;
    sf::CircleShape locator;
    int location;
    bool locatorActive;
    static bool arrowsVisible;

    bool operator==(const Particle& other) const;

    Particle(sf::Vector2f initialPosition, sf::Vector2f initialVelocity);
    void Move(float timeInterval);
    void setVerticesPositions();
    void updateArrows();
    void draw(sf::RenderWindow& window);
};