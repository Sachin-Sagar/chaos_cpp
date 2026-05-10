#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

class ImagePlotter {
public:
    using GeneratorFunc = std::function<std::vector<int>(int, int, double, double, double, double, int)>;

    ImagePlotter(const std::string& title, GeneratorFunc generator);
    
    void setLabels(const std::string& xLabel, const std::string& yLabel);
    void setLimits(double xMin, double xMax, double yMin, double yMax);
    void setMaxIters(int maxIters);
    void show();

private:
    void updateImage();
    void drawOverlay(sf::RenderWindow& window, const sf::Font& font);
    sf::Color getColor(int count);
    void saveSnapshot(sf::RenderWindow& window);

    std::string m_title;
    std::string m_xLabel, m_yLabel;
    GeneratorFunc m_generator;
    double m_xMin, m_xMax, m_yMin, m_yMax;
    int m_maxIters;

    sf::Image m_image;
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    bool m_needsUpdate;
};
