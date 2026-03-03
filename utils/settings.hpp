// utils/settings.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace PlotSettings
{
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const float MARGIN_X = 120.0f;
    const float MARGIN_Y = 120.0f;

    const sf::Color BACKGROUND_COLOR = sf::Color(20, 20, 20);
    const sf::Color POINT_COLOR = sf::Color(255, 255, 255, 30);
    const sf::Color GRID_COLOR = sf::Color(100, 100, 100, 80);
    const sf::Color AXIS_COLOR = sf::Color::White;

    // Cross-platform font path detection
    #if defined(_WIN32) || defined(_WIN64)
        const std::string FONT_PATH = "C:/Windows/Fonts/arial.ttf";
    #elif __ANDROID__
        // Standard Android system font path
        const std::string FONT_PATH = "/system/fonts/Roboto-Regular.ttf";
    #else
        // Default for Linux/Generic Unix
        const std::string FONT_PATH = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    #endif
}