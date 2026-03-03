#pragma once
#include <SFML/Graphics.hpp>

namespace PlotSettings
{

    // Window resolution
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    // Margins for axes
    const float MARGIN_X = 120.0f;
    const float MARGIN_Y = 120.0f;

    // Colors
    const sf::Color BACKGROUND_COLOR = sf::Color(20, 20, 20); // Dark grey
    const sf::Color POINT_COLOR = sf::Color(255, 255, 255, 30); // Highly transparent for density
    const sf::Color GRID_COLOR = sf::Color(100, 100, 100, 80);  // Faint, non-intrusive grid
    const sf::Color AXIS_COLOR = sf::Color::White;

    // Font path
    const std::string FONT_PATH = "C:/Windows/Fonts/arial.ttf";

}