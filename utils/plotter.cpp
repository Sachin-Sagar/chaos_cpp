#include "plotter.hpp"
#include "settings.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <optional>
#include <SFML/Config.hpp> // Gives us SFML_VERSION_MAJOR

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
    
    // 1. WINDOW CREATION
#if SFML_VERSION_MAJOR >= 3
    settings.antiAliasingLevel = 4; // Notice the capital 'A'
    sf::RenderWindow window(sf::VideoMode({(unsigned int)PlotSettings::WINDOW_WIDTH, (unsigned int)PlotSettings::WINDOW_HEIGHT}), 
                            m_title, sf::Style::Default, sf::State::Windowed, settings);
#else
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(PlotSettings::WINDOW_WIDTH, PlotSettings::WINDOW_HEIGHT), 
                            m_title, sf::Style::Default, settings);
#endif
    
    sf::Font font;
    
    // 2. FONT LOADING
#if SFML_VERSION_MAJOR >= 3
    if (!font.openFromFile(PlotSettings::FONT_PATH)) {
#else
    if (!font.loadFromFile(PlotSettings::FONT_PATH)) {
#endif
        std::cerr << "Error: Could not load font from " << PlotSettings::FONT_PATH << ".\n";
    }

    while (window.isOpen()) {
        
        // 3. EVENT POLLING
#if SFML_VERSION_MAJOR >= 3
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (scroll->delta > 0) m_zoom *= 1.1f;
                else m_zoom *= 0.9f;
            }
        }
#else
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) m_zoom *= 1.1f;
                else m_zoom *= 0.9f;
            }
        }
#endif

        float moveSpeed = 0.05f / m_zoom;
        
        // 4. KEYBOARD INPUT
#if SFML_VERSION_MAJOR >= 3
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_offsetY += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_offsetY -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_offsetX -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_offsetX += moveSpeed;
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) m_offsetY += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) m_offsetY -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_offsetX -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_offsetX += moveSpeed;
#endif

        window.clear(PlotSettings::BACKGROUND_COLOR);
        drawGridAndAxes(window, font);
        drawData(window);
        window.display();
    }
}

void Plot2D::drawGridAndAxes(sf::RenderWindow& window, const sf::Font& font) {
    // 5. TEXT CONSTRUCTORS AND GEOMETRY
#if SFML_VERSION_MAJOR >= 3
    sf::Text titleText(font, m_title, 24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({PlotSettings::WINDOW_WIDTH/2.0f - titleText.getGlobalBounds().size.x/2.0f, 20.0f});
#else
    sf::Text titleText(m_title, font, 24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(PlotSettings::WINDOW_WIDTH/2.0f - titleText.getGlobalBounds().width/2.0f, 20.0f);
#endif
    window.draw(titleText);

    sf::RectangleShape xAxis(sf::Vector2f(PlotSettings::WINDOW_WIDTH - 2*PlotSettings::MARGIN_X, 2));
#if SFML_VERSION_MAJOR >= 3
    xAxis.setPosition({PlotSettings::MARGIN_X, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y});
#else
    xAxis.setPosition(PlotSettings::MARGIN_X, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y);
#endif
    window.draw(xAxis);

    sf::RectangleShape yAxis(sf::Vector2f(2, PlotSettings::WINDOW_HEIGHT - 2*PlotSettings::MARGIN_Y));
#if SFML_VERSION_MAJOR >= 3
    yAxis.setPosition({PlotSettings::MARGIN_X, PlotSettings::MARGIN_Y});
#else
    yAxis.setPosition(PlotSettings::MARGIN_X, PlotSettings::MARGIN_Y);
#endif
    window.draw(yAxis);

#if SFML_VERSION_MAJOR >= 3
    sf::Text xLabelText(font, m_xLabel, 18);
    xLabelText.setPosition({PlotSettings::WINDOW_WIDTH/2.0f, PlotSettings::WINDOW_HEIGHT - 40.0f});
#else
    sf::Text xLabelText(m_xLabel, font, 18);
    xLabelText.setPosition(PlotSettings::WINDOW_WIDTH/2.0f, PlotSettings::WINDOW_HEIGHT - 40.0f);
#endif
    window.draw(xLabelText);

#if SFML_VERSION_MAJOR >= 3
    sf::Text yLabelText(font, m_yLabel, 18);
    yLabelText.setRotation(sf::degrees(-90.0f)); 
    yLabelText.setPosition({30.0f, PlotSettings::WINDOW_HEIGHT/2.0f + yLabelText.getGlobalBounds().size.x/2.0f});
#else
    sf::Text yLabelText(m_yLabel, font, 18);
    yLabelText.setRotation(-90.0f);
    yLabelText.setPosition(30.0f, PlotSettings::WINDOW_HEIGHT/2.0f + yLabelText.getGlobalBounds().width/2.0f);
#endif
    window.draw(yLabelText);

    sf::Text tickLabel;
    tickLabel.setFont(font);
    tickLabel.setCharacterSize(12);
    tickLabel.setFillColor(sf::Color(200, 200, 200));

    int numTicks = 10;
    float plotWidth = PlotSettings::WINDOW_WIDTH - 2 * PlotSettings::MARGIN_X;
    float plotHeight = PlotSettings::WINDOW_HEIGHT - 2 * PlotSettings::MARGIN_Y;

    for (int i = 0; i <= numTicks; ++i) {
        float x_pos = PlotSettings::MARGIN_X + (i * plotWidth / numTicks);
        double x_val = m_xMin + m_offsetX + (i * (m_xMax - m_xMin) / (numTicks * m_zoom));
        
        std::stringstream ssX; ssX << std::fixed << std::setprecision(2) << x_val;
        tickLabel.setString(ssX.str());
#if SFML_VERSION_MAJOR >= 3
        tickLabel.setPosition({x_pos - 15, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y + 5});
#else
        tickLabel.setPosition(x_pos - 15, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y + 5);
#endif
        window.draw(tickLabel);

        float y_pos = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - (i * plotHeight / numTicks);
        double y_val = m_yMin + m_offsetY + (i * (m_yMax - m_yMin) / (numTicks * m_zoom));

        std::stringstream ssY; ssY << std::fixed << std::setprecision(2) << y_val;
        tickLabel.setString(ssY.str());
#if SFML_VERSION_MAJOR >= 3
        tickLabel.setPosition({PlotSettings::MARGIN_X - 45, y_pos - 10});
#else
        tickLabel.setPosition(PlotSettings::MARGIN_X - 45, y_pos - 10);
#endif
        window.draw(tickLabel);
    }
}

void Plot2D::drawData(sf::RenderWindow& window) {
    // 6. PRIMITIVE TYPES
#if SFML_VERSION_MAJOR >= 3
    sf::VertexArray plotPixels(sf::PrimitiveType::Points);
#else
    sf::VertexArray plotPixels(sf::Points);
#endif
    float plotWidth = PlotSettings::WINDOW_WIDTH - (2 * PlotSettings::MARGIN_X);
    float plotHeight = PlotSettings::WINDOW_HEIGHT - (2 * PlotSettings::MARGIN_Y);

    for (const auto& pt : m_data) {
        float relX = (pt.x - m_xMin - m_offsetX) * m_zoom / (m_xMax - m_xMin);
        float relY = (pt.y - m_yMin - m_offsetY) * m_zoom / (m_yMax - m_yMin);

        float screen_x = PlotSettings::MARGIN_X + relX * plotWidth;
        float screen_y = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - relY * plotHeight;

        if (screen_x >= PlotSettings::MARGIN_X && screen_x <= PlotSettings::WINDOW_WIDTH - PlotSettings::MARGIN_X &&
            screen_y >= PlotSettings::MARGIN_Y && screen_y <= PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) {
#if SFML_VERSION_MAJOR >= 3
            plotPixels.append(sf::Vertex({screen_x, screen_y}, PlotSettings::POINT_COLOR));
#else
            plotPixels.append(sf::Vertex(sf::Vector2f(screen_x, screen_y), PlotSettings::POINT_COLOR));
#endif
        }
    }
    window.draw(plotPixels);
}