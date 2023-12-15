#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include "../include/log.h"
#include "../include/coordinate.h"
#include "../include/calculations.h"
#include "../include/particle.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics Simulator");
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("./fonts/LiberationSans-Regular.ttf"))
    {
        std::cout << "error: font load failed" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White); 

    std::cout << "Welcome to my physics simulator!" << std::endl;
    std::cout << "created by Max Levine" << std::endl;

    std::vector<Particle> particles;

    bool isMousePressed = false;
    sf::Vector2f pressPosition;
    sf::Vector2f releasePosition;
    sf::Vertex line[2];

    sf::RectangleShape center = sf::RectangleShape(sf::Vector2f(10,10));
    center.setPosition(-5, 5);
    center.setFillColor(sf::Color(255, 255, 255));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                isMousePressed = true;
                //take mouse position and convert it from screen space to world space
                pressPosition = screenToWorld(sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)));
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                if (isMousePressed)
                {
                    releasePosition = screenToWorld(sf::Vector2f(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y)));
                    Particle particle(pressPosition, releasePosition - pressPosition);
                    particles.push_back(particle);
                }
                isMousePressed = false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && particles.size() > 0)
            {
                particles.erase(particles.begin());
            }
        }
        if (isMousePressed)
        {
            line[0].position = pressPosition;
            line[0].color = sf::Color::Yellow;

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            line[1].position = screenToWorld(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));  // End of the line
            line[1].color = sf::Color::Yellow;
        }

        //the clear, draw, display cycle
        window.clear(sf::Color::Black);

        float deltaTime = clock.restart().asSeconds(); //deltaTime = time difference between the previous frame and the one before that
        int fps = static_cast<int>(1.f / deltaTime);
        std::string fpsString = "fps: " + std::to_string(fps) + "   particles: "+std::to_string(particles.size());
        text.setString(fpsString);

        for (auto& particle : particles)
        {
            sf::Vector2f& velocity = particle.velocity;

            // acceleration is calculated using Newton's law of universal gravitation where the force of gravity is inversely proportional to the square of the distance.
            particle.acceleration.x = -1000000.f / (getMagnitude(particle.position)* getMagnitude(particle.position)) * cos(getDirection(particle.position));
            particle.acceleration.y = -1000000.f / (getMagnitude(particle.position)* getMagnitude(particle.position)) * sin(getDirection(particle.position));
            
            particle.Move(deltaTime);
            particle.setVerticesPositions();
            particle.updateArrows();

            //note that the end of the tail is the start of the vector
            if (particle.tail.size() < 2000)
            {
                TailPart tailPart(0.6, particle.position, 50);
                particle.tail.push_back(tailPart);
            }
            else if (particle.tail.size() == 2000)
            {
                std::rotate(particle.tail.begin(), particle.tail.begin() + 1, particle.tail.end());
                particle.tail.back().circle.setPosition(particle.position);
                particle.tail.back().alpha = 50;
                particle.tail.back().circle.setFillColor(sf::Color(255, 255, 255, particle.tail.back().alpha));
                for (int i = 0; i < particle.tail.size(); i++)
                {
                    if (i * 0.2 < 255)
                    {
                        particle.tail[i].alpha = i * 0.2;
                        particle.tail[i].circle.setFillColor(sf::Color(255, 255, 255, particle.tail[i].alpha));
                    }
                }
            } 
            
            for (auto& tailPart : particle.tail)
            {
                tailPart.circle.setPosition(worldToScreen(tailPart.circle.getPosition().x, tailPart.circle.getPosition().y));
                window.draw(tailPart.circle);
                tailPart.circle.setPosition(screenToWorld(tailPart.circle.getPosition()));
            }
            
            for (int i = 0; i < particle.vertices.getVertexCount(); i++)
            {
                particle.vertices[i].position = worldToScreen(particle.vertices[i].position.x, particle.vertices[i].position.y);
            }
            window.draw(particle.vertices);
            for (int i = 0; i < particle.vertices.getVertexCount(); i++)
            {
                particle.vertices[i].position = screenToWorld(particle.vertices[i].position);
            }
            
            for (int i = 0; i < particle.arrows.size(); i++)
            {
                particle.arrows[i].points[0].position = worldToScreen(particle.arrows[i].points[0].position.x, particle.arrows[i].points[0].position.y);
                particle.arrows[i].points[1].position = worldToScreen(particle.arrows[i].points[1].position.x, particle.arrows[i].points[1].position.y);
                window.draw(particle.arrows[i].points);
                particle.arrows[i].points[0].position = screenToWorld(particle.arrows[i].points[0].position);
                particle.arrows[i].points[1].position = screenToWorld(particle.arrows[i].points[1].position);
            }

        }
        if (isMousePressed) {
            line[0].position = worldToScreen(line[0].position.x, line[0].position.y);
            line[1].position = worldToScreen(line[1].position.x, line[1].position.y);
            window.draw(line, 2, sf::Lines);
            line[0].position = screenToWorld(line[0].position);
            line[1].position = screenToWorld(line[1].position);
        }

        center.setPosition(worldToScreen(center.getPosition().x, center.getPosition().y));
        window.draw(center);
        center.setPosition(screenToWorld(center.getPosition()));

        window.draw(text);
        window.display();
    }

    return 0;
}