#include <SFML/Graphics.hpp>
#include <iostream>

// Cherno's log class
class Log
{
public:
    enum Level
    {
        LevelError, LevelWarning, LevelInfo
    };
private:
    Level m_LogLevel = LevelInfo;
public:
    void SetLevel(Level level)
    {
        m_LogLevel = level;
    }

    void Error(const char* message)
    {
        if (m_LogLevel >= LevelError)
            std::cout << "[ERROR]: " << message << std::endl;
    }
    void Warn(const char* message)
    {
        if (m_LogLevel >= LevelWarning)
            std::cout << "[WARNING]: " << message << std::endl;
    }
    void Info(const char* message)
    {
        if (m_LogLevel >= LevelInfo)
            std::cout << "[INFO]: " << message << std::endl;
    }
};

// maybe put this in the log class
// Prints a 2d vector to the console in the format "x, y"
void printVec(const sf::Vector2f& vector)
{
    std::cout << vector.x << ", " << vector.y << std::endl;
}