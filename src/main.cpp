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
        if (!font.loadFromFile("../fonts/LiberationSans-Regular.ttf"))
        {
            std::cout << "error: font load failed" << std::endl;
        }
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
        
        float deltaTime = clock.restart().asSeconds(); //deltaTime = time difference between the previous frame and the one before that
        int fps = static_cast<int>(1.f / deltaTime);
        std::string fpsString = "fps: " + std::to_string(fps) + "   particles: "+std::to_string(particles.size());
        text.setString(fpsString);

        // the clear, draw, display cycle
        window.clear(sf::Color::Black);

        for (auto& particle : particles)
        {
            sf::Vector2f& velocity = particle.velocity;

            if (getMagnitude(particle.position) < 1)
            {
                particles.erase(std::remove(particles.begin(), particles.end(), particle), particles.end());
                std::cout << "particle destroyed by center" << std::endl;
                break;
            }

            // acceleration is calculated using Newton's law of universal gravitation where the force of gravity is inversely proportional to the square of the distance.
            particle.acceleration.x = -1000000.f / (particle.position.x * particle.position.x + particle.position.y * particle.position.y) * cos(getDirection(particle.position));
            particle.acceleration.y = -1000000.f / (particle.position.x * particle.position.x + particle.position.y * particle.position.y) * sin(getDirection(particle.position));
            
            particle.Move(deltaTime);
            particle.setVerticesPositions();
            particle.updateArrows();

            //note that the end of the tail is the start of the vector
            particle.maxTailSize = 750.f * exp(-0.001f * getMagnitude(particle.velocity));
            if (particle.maxTailSize > 3000)
                particle.maxTailSize = 3000;
            if (particle.tail.size() <  particle.maxTailSize)
            {
                TailPart tailPart(0.6, particle.position, 255);
                particle.tail.push_back(tailPart);
            }
            else if (particle.tail.size() >= particle.maxTailSize)
            {
                if (particle.tail.size() > particle.maxTailSize)
                    particle.tail.erase(particle.tail.begin());
                std::rotate(particle.tail.begin(), particle.tail.begin() + 1, particle.tail.end());
                particle.tail.back().circle.setPosition(particle.position);
                particle.tail.back().alpha = 255;
                particle.tail.back().circle.setFillColor(sf::Color(255, 255, 255, round(particle.tail.back().alpha)));
                for (int i = 0; i < particle.tail.size(); i++)
                {
                    if (i * 0.5 < 255)
                    {
                        particle.tail[i].alpha = i * 0.5;
                        particle.tail[i].circle.setFillColor(sf::Color(255, 255, 255, round(particle.tail[i].alpha)));
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
            
            particle.locatorActive = true;
            switch (locate(worldToScreen(particle.position.x, particle.position.y), window))
            {
                case 1:
                    particle.locator.setPosition(sf::Vector2f(3, 13));
                    particle.locator.setRotation(315);
                    break;
                case 2:
                    particle.locator.setPosition(sf::Vector2f(worldToScreen(particle.position.x, particle.position.y).x - 12, 5));
                    particle.locator.setRotation(0);
                    break;
                case 3:
                    particle.locator.setPosition(window.getSize().x-13, 3);
                    particle.locator.setRotation(45);
                    break;
                case 4:
                    particle.locator.setPosition(sf::Vector2f(5, worldToScreen(particle.position.x, particle.position.y).y + 12));
                    particle.locator.setRotation(270);
                    break;
                case 5:
                    particle.locatorActive = false;
                    break;
                case 6:
                    particle.locator.setPosition(sf::Vector2f(window.getSize().x - 5, worldToScreen(particle.position.x, particle.position.y).y - 12));
                    particle.locator.setRotation(90);
                    break;
                case 7:
                    particle.locator.setPosition(13, window.getSize().y-3);
                    particle.locator.setRotation(225);
                    break;
                case 8:
                    particle.locator.setPosition(sf::Vector2f(worldToScreen(particle.position.x, particle.position.y).x + 12, window.getSize().y - 5));
                    particle.locator.setRotation(180);
                    break;
                case 9:
                    particle.locator.setPosition(window.getSize().x-3, window.getSize().y - 13);
                    particle.locator.setRotation(135);
                    break;
                default: std::cout << "bruh" << std::endl;
            }
            if (particle.locatorActive)
                window.draw(particle.locator);
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