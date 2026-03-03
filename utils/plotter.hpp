#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../src/math_models.hpp" // Use the central definition

class Plot2D {
public:
    Plot2D(const std::string& title);
    
    void setLimits(double xMin, double xMax, double yMin, double yMax);
    void setLabels(const std::string& xLabel, const std::string& yLabel);
    void setData(const std::vector<Point2D>& points);
    void show();

private:
    // Pass the font by reference to these functions
    void drawGridAndAxes(sf::RenderWindow& window, const sf::Font& font);
    void drawData(sf::RenderWindow& window);

    std::string m_title;
    std::string m_xLabel, m_yLabel;
    double m_xMin, m_xMax, m_yMin, m_yMax;
    std::vector<Point2D> m_data;

    float m_zoom;
    float m_offsetX;
    float m_offsetY;
};