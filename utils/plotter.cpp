#include "plotter.hpp"
#include "settings.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

Plot2D::Plot2D(const std::string& title) 
    : m_title(title), m_zoom(1.0f), m_offsetX(0.0f), m_offsetY(0.0f),
      m_xMin(0), m_xMax(1), m_yMin(0), m_yMax(1) {}

void Plot2D::setLimits(double xMin, double xMax, double yMin, double yMax) {
    m_xMin = xMin; m_xMax = xMax; m_yMin = yMin; m_yMax = yMax;
}

void Plot2D::setLabels(const std::string& xLabel, const std::string& yLabel) {
    m_xLabel = xLabel; m_yLabel = yLabel;
}

void Plot2D::setData(const std::vector<Point2D>& points) {
    m_data = points;
}

void Plot2D::show() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(PlotSettings::WINDOW_WIDTH, PlotSettings::WINDOW_HEIGHT), 
                            m_title, sf::Style::Default, settings);
    
    sf::Font font;
    if (!font.loadFromFile(PlotSettings::FONT_PATH)) {
        std::cerr << "Error: Could not load font.\n";
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) m_zoom *= 1.1f;
                else m_zoom *= 0.9f;
            }
        }

        // Panning Speed (Adjusted by zoom so panning doesn't feel "wild" when zoomed in)
        float moveSpeed = 0.05f / m_zoom;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) m_offsetY += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) m_offsetY -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_offsetX -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_offsetX += moveSpeed;

        window.clear(PlotSettings::BACKGROUND_COLOR);

        // Pass the loaded font into the drawing function
        drawGridAndAxes(window, font);
        drawData(window);

        window.display();
    }
}

void Plot2D::drawGridAndAxes(sf::RenderWindow& window, const sf::Font& font) {
    // 1. Draw Plot Title
    sf::Text titleText(m_title, font, 24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(PlotSettings::WINDOW_WIDTH/2.0f - titleText.getGlobalBounds().width/2.0f, 20.0f);
    window.draw(titleText);

    // 2. Draw Axes Lines
    sf::RectangleShape xAxis(sf::Vector2f(PlotSettings::WINDOW_WIDTH - 2*PlotSettings::MARGIN_X, 2));
    xAxis.setPosition(PlotSettings::MARGIN_X, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y);
    window.draw(xAxis);

    sf::RectangleShape yAxis(sf::Vector2f(2, PlotSettings::WINDOW_HEIGHT - 2*PlotSettings::MARGIN_Y));
    yAxis.setPosition(PlotSettings::MARGIN_X, PlotSettings::MARGIN_Y);
    window.draw(yAxis);

    // 3. Labels
    sf::Text xLabelText(m_xLabel, font, 18);
    xLabelText.setPosition(PlotSettings::WINDOW_WIDTH/2.0f, PlotSettings::WINDOW_HEIGHT - 40.0f);
    window.draw(xLabelText);

    sf::Text yLabelText(m_yLabel, font, 18);
    yLabelText.setRotation(-90.0f); // Rotate for Y-axis
    yLabelText.setPosition(30.0f, PlotSettings::WINDOW_HEIGHT/2.0f + yLabelText.getGlobalBounds().width/2.0f);
    window.draw(yLabelText);

    // 4. Tick Marks and Numeric Values
    sf::Text tickLabel;
    tickLabel.setFont(font);
    tickLabel.setCharacterSize(12);
    tickLabel.setFillColor(sf::Color(200, 200, 200));

    int numTicks = 10;
    float plotWidth = PlotSettings::WINDOW_WIDTH - 2 * PlotSettings::MARGIN_X;
    float plotHeight = PlotSettings::WINDOW_HEIGHT - 2 * PlotSettings::MARGIN_Y;

    for (int i = 0; i <= numTicks; ++i) {
        // X-Axis Ticks
        float x_pos = PlotSettings::MARGIN_X + (i * plotWidth / numTicks);
        double x_val = m_xMin + m_offsetX + (i * (m_xMax - m_xMin) / (numTicks * m_zoom));
        
        std::stringstream ssX; ssX << std::fixed << std::setprecision(2) << x_val;
        tickLabel.setString(ssX.str());
        tickLabel.setPosition(x_pos - 15, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y + 5);
        window.draw(tickLabel);

        // Y-Axis Ticks
        float y_pos = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - (i * plotHeight / numTicks);
        double y_val = m_yMin + m_offsetY + (i * (m_yMax - m_yMin) / (numTicks * m_zoom));

        std::stringstream ssY; ssY << std::fixed << std::setprecision(2) << y_val;
        tickLabel.setString(ssY.str());
        tickLabel.setPosition(PlotSettings::MARGIN_X - 45, y_pos - 10);
        window.draw(tickLabel);
    }
}

void Plot2D::drawData(sf::RenderWindow& window) {
    sf::VertexArray plotPixels(sf::Points);
    float plotWidth = PlotSettings::WINDOW_WIDTH - (2 * PlotSettings::MARGIN_X);
    float plotHeight = PlotSettings::WINDOW_HEIGHT - (2 * PlotSettings::MARGIN_Y);

    for (const auto& pt : m_data) {
        // Calculate relative position based on limits, zoom, and offset
        float relX = (pt.x - m_xMin - m_offsetX) * m_zoom / (m_xMax - m_xMin);
        float relY = (pt.y - m_yMin - m_offsetY) * m_zoom / (m_yMax - m_yMin);

        float screen_x = PlotSettings::MARGIN_X + relX * plotWidth;
        float screen_y = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - relY * plotHeight;

        // Simple Clipping: Only draw if the point is within the axis boundaries
        if (screen_x >= PlotSettings::MARGIN_X && screen_x <= PlotSettings::WINDOW_WIDTH - PlotSettings::MARGIN_X &&
            screen_y >= PlotSettings::MARGIN_Y && screen_y <= PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) {
            plotPixels.append(sf::Vertex(sf::Vector2f(screen_x, screen_y), PlotSettings::POINT_COLOR));
        }
    }
    window.draw(plotPixels);
}