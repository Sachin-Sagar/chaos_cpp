#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "math_models.hpp"

class Plot2DSoA {
public:
    Plot2DSoA(const std::string& title);
    
    void setLimits(double xMin, double xMax, double yMin, double yMax);
    void setLabels(const std::string& xLabel, const std::string& yLabel);
    void setData(const Points2DSoA& points);
    void show();
    void benchmarkRender(int iterations = 100);

private:
    void saveSnapshot(sf::RenderWindow& window);
    void drawGridAndAxes(sf::RenderWindow& window, const sf::Font& font);
    void drawData(sf::RenderTarget& window);
    void drawDataScalar(sf::RenderTarget& window);

    std::string m_title;
    std::string m_xLabel, m_yLabel;
    double m_xMin, m_xMax, m_yMin, m_yMax;
    Points2DSoA m_data;

    float m_zoom;
    float m_offsetX;
    float m_offsetY;
};
