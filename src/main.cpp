#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

//screen space is the coordinate system used by sfml
//the origin is in the top left, right and down are positive
//world space is the coordinate system used for calculations in the code
//the origin is in the center, right and up are positive
sf::Vector2f screenToWorld(const sf::Vector2f& screenPos) {
    float x = screenPos.x - 400;
    float y = -(screenPos.y - 300);
    return sf::Vector2f(x, y);
}
sf::Vector2f worldToScreen(float x, float y) {
    float screenX = x + 400;
    float screenY = -y + 300;
    return sf::Vector2f(screenX, screenY);
}

void printVec(sf::Vector2f vector) {
    std::cout << vector.x;
    std::cout << ", ";
    std::cout << vector.y << std::endl;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Physics Simulator");
    sf::Clock clock;

    struct TailPart {
        sf::CircleShape circle;
        float alpha;
    };

    struct Particle {
        sf::VertexArray vertices;
        sf::Vector2f position;
        float distance;
        float direction;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        std::vector<TailPart> tail;
        sf::VertexArray Varrow; //velocity vector graphics
        sf::VertexArray Aarrow; //acceleration vector graphics
    };

    std::vector<Particle> particles;

    bool isMousePressed = false;
    sf::Vector2f pressPosition;
    sf::Vertex line[2];

    sf::CircleShape center = sf::CircleShape(5);
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
                    particle.velocity = screenToWorld(sf::Vector2f(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y)))-pressPosition;
                    particle.Varrow = sf::VertexArray(sf::Lines, 2);
                    particle.Varrow[0].position = particle.position + pressPosition;
                    particle.Varrow[1].position = particle.position + particle.velocity;
                    particle.Varrow[0].color = sf::Color::Yellow;
                    particle.Varrow[1].color = sf::Color::Yellow;
                    particle.Aarrow = sf::VertexArray(sf::Lines, 2);
                    particle.Aarrow[0].position = particle.position + pressPosition;
                    particle.Aarrow[1].position = particle.position + particle.acceleration;
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
            line[1].position = screenToWorld(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));  // End of the line
            line[1].color = sf::Color::Magenta;
        }

        //the clear, draw, display cycle
        window.clear(sf::Color::Black); //CLEAR THE SCREEN

        float deltaTime = clock.restart().asSeconds(); //deltaTime = time difference between the previous frame and the one before that

        for (auto& particle : particles)
        {
            sf::Vector2f& velocity = particle.velocity;

            float& distance = particle.distance;
            float& direction = particle.direction;

            particle.distance = sqrt(particle.position.x* particle.position.x + particle.position.y * particle.position.y);
            particle.direction = atan2(particle.position.y, particle.position.x);

            velocity.y += particle.acceleration.y * deltaTime;
            velocity.x += particle.acceleration.x * deltaTime;
            particle.acceleration = sf::Vector2f(1000000* -1.f/(particle.distance*particle.distance) * cos(particle.direction),
                1000000* -1.f / (particle.distance*particle.distance)* sin(particle.direction));

            for (size_t i = 0; i < particle.vertices.getVertexCount(); i++)
            {
                particle.vertices[i].position += velocity * deltaTime;
            }
            particle.position += velocity * deltaTime;

            particle.Varrow[0].position = particle.position;
            particle.Varrow[1].position = particle.position + particle.velocity;
            particle.Aarrow[0].position = particle.position;
            particle.Aarrow[1].position = particle.position + particle.acceleration;

            if (particle.tail.size() < 1500)
            {
                TailPart tailPart;
                tailPart.circle.setRadius(0.5);
                tailPart.circle.setPosition(particle.position);
                tailPart.alpha = 50;
                tailPart.circle.setFillColor(sf::Color(255, 255, 255, tailPart.alpha));
                particle.tail.push_back(tailPart);
            }
            
            for (auto& tailPart : particle.tail)
            {
                if (tailPart.alpha > 0)
                {
                    tailPart.alpha -= 50*deltaTime;
                }
                else if (tailPart.alpha <= 0)
                {
                    tailPart.alpha = 50;
                    tailPart.circle.setPosition(particle.position);
                }
                tailPart.circle.setFillColor(sf::Color(255, 255, 255, round(tailPart.alpha)));

                tailPart.circle.setPosition(worldToScreen(tailPart.circle.getPosition().x, tailPart.circle.getPosition().y));
                window.draw(tailPart.circle);
                tailPart.circle.setPosition(screenToWorld(tailPart.circle.getPosition()));
            }
            
            for (size_t i = 0; i < particle.vertices.getVertexCount(); i++)
            {
                particle.vertices[i].position = worldToScreen(particle.vertices[i].position.x, particle.vertices[i].position.y);
            }
            window.draw(particle.vertices);
            for (size_t i = 0; i < particle.vertices.getVertexCount(); i++)
            {
                particle.vertices[i].position = screenToWorld(particle.vertices[i].position);
            }

            particle.Varrow[0].position = worldToScreen(particle.Varrow[0].position.x, particle.Varrow[0].position.y);
            particle.Varrow[1].position = worldToScreen(particle.Varrow[1].position.x, particle.Varrow[1].position.y);
            particle.Aarrow[0].position = worldToScreen(particle.Aarrow[0].position.x, particle.Aarrow[0].position.y);
            particle.Aarrow[1].position = worldToScreen(particle.Aarrow[1].position.x, particle.Aarrow[1].position.y);
            window.draw(particle.Varrow);
            window.draw(particle.Aarrow);
            particle.Varrow[0].position = screenToWorld(particle.Varrow[0].position);
            particle.Varrow[1].position = screenToWorld(particle.Varrow[1].position);
            particle.Aarrow[0].position = screenToWorld(particle.Aarrow[0].position);
            particle.Aarrow[1].position = screenToWorld(particle.Aarrow[1].position);
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

        window.display();
    }

    return 0;
}
