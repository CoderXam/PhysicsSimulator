#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics Simulator");
    sf::Clock clock;

    struct Particle {
        sf::VertexArray vertices;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        std::vector<sf::CircleShape> tail;
        sf::VertexArray Varrow; //velocity vector graphics
        sf::VertexArray Aarrow; //acceleration vector graphics
    };

    std::vector<Particle> particles;

    bool isMousePressed = false;
    sf::Vector2f pressPosition;
    sf::Vertex line[2];

    sf::CircleShape center = sf::CircleShape(5);
    center.setPosition(400 - 5, 300 - 5);
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
                pressPosition = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                if (isMousePressed)
                {
                    Particle particle;
                    particle.position = pressPosition;
                    particle.vertices = sf::VertexArray(sf::TriangleFan, 4);
                    particle.vertices[0].position = particle.position + sf::Vector2f(10.f, 10.f);
                    particle.vertices[1].position = particle.position + sf::Vector2f(10.f, -10.f);
                    particle.vertices[2].position = particle.position + sf::Vector2f(-10.f, -10.f);
                    particle.vertices[3].position = particle.position + sf::Vector2f(-10.f, 10.f);
                    particle.vertices[0].color = sf::Color::Cyan;
                    particle.vertices[1].color = sf::Color::Cyan;
                    particle.vertices[2].color = sf::Color::Cyan;
                    particle.vertices[3].color = sf::Color::Blue;
                    particle.velocity = sf::Vector2f(static_cast<float>(event.mouseButton.x) - pressPosition.x,
                        static_cast<float>(event.mouseButton.y) - pressPosition.y);
                    particle.Varrow = sf::VertexArray(sf::Lines, 2);
                    particle.Varrow[0].position = particle.position + sf::Vector2f(pressPosition.x, pressPosition.y);
                    particle.Varrow[1].position = particle.position + sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    particle.Varrow[0].color = sf::Color::Yellow;
                    particle.Varrow[1].color = sf::Color::Yellow;
                    particle.Aarrow = sf::VertexArray(sf::Lines, 2);
                    particle.Aarrow[0].position = particle.position + sf::Vector2f(pressPosition.x, pressPosition.y);
                    particle.Aarrow[1].position = particle.position + sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    particle.Aarrow[0].color = sf::Color::Red;
                    particle.Aarrow[1].color = sf::Color::Red;
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
            line[0].color = sf::Color::Red;

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            line[1].position = sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));  // End of the line
            line[1].color = sf::Color::Magenta;
        }

        //the clear, draw, display cycle
        window.clear(sf::Color::Black); //CLEAR THE SCREEN

        float deltaTime = clock.restart().asSeconds(); //deltaTime = time difference between the previous frame and the one before that

        for (auto& particle : particles)
        {
            sf::Vector2f& velocity = particle.velocity;

            velocity.y += particle.acceleration.y * deltaTime;
            velocity.x += particle.acceleration.x * deltaTime;
            particle.acceleration = sf::Vector2f((400 - particle.position.x), (300 - particle.position.y));
            for (size_t i = 0; i < particle.vertices.getVertexCount(); i++)
            {
                particle.vertices[i].position += velocity * deltaTime;
            }
            particle.position += velocity * deltaTime;

            particle.Varrow[0].position = particle.position;
            particle.Varrow[1].position = particle.position + particle.velocity;
            particle.Aarrow[0].position = particle.position;
            particle.Aarrow[1].position = particle.position + particle.acceleration;

            sf::CircleShape circle;
            circle.setRadius(0.5);
            circle.setPosition(particle.position);
            particle.tail.push_back(circle);

            for (auto& tailPart : particle.tail)
            {
                if (tailPart.getFillColor().a > 0)
                {
                    tailPart.setFillColor(sf::Color(255,255,255, tailPart.getFillColor().a-1.f*deltaTime));
                }

                window.draw(tailPart);
            }

            window.draw(particle.vertices);
            window.draw(particle.Varrow);
            window.draw(particle.Aarrow);
        }
        if (isMousePressed)
            window.draw(line, 2, sf::Lines);

        window.draw(center);

        window.display();
    }

    return 0;
}
