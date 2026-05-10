#include "image_plotter.hpp"
#include "settings.hpp"
#include <iostream>
#include <cmath>
#include <optional>

#if SFML_VERSION_MAJOR >= 3
ImagePlotter::ImagePlotter(const std::string& title, GeneratorFunc generator)
    : m_title(title), m_xLabel("Real"), m_yLabel("Imaginary"), m_generator(generator), 
      m_xMin(-2.0), m_xMax(1.0), m_yMin(-1.2), m_yMax(1.2),
      m_maxIters(256), m_sprite(m_texture), m_needsUpdate(true) {
}
#else
ImagePlotter::ImagePlotter(const std::string& title, GeneratorFunc generator)
    : m_title(title), m_xLabel("Real"), m_yLabel("Imaginary"), m_generator(generator), 
      m_xMin(-2.0), m_xMax(1.0), m_yMin(-1.2), m_yMax(1.2),
      m_maxIters(256), m_needsUpdate(true) {
}
#endif

void ImagePlotter::setLabels(const std::string& xLabel, const std::string& yLabel) {
    m_xLabel = xLabel;
    m_yLabel = yLabel;
}

void ImagePlotter::setLimits(double xMin, double xMax, double yMin, double yMax) {
    m_xMin = xMin; m_xMax = xMax;
    m_yMin = yMin; m_yMax = yMax;
    m_needsUpdate = true;
}

void ImagePlotter::setMaxIters(int maxIters) {
    m_maxIters = maxIters;
    m_needsUpdate = true;
}

sf::Color ImagePlotter::getColor(int count) {
    if (count >= m_maxIters) return sf::Color::Black;

    // Nice color gradient
    float t = (float)count / m_maxIters;
    uint8_t r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
    uint8_t g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
    uint8_t b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return sf::Color(r, g, b);
}

void ImagePlotter::updateImage() {
    int width = PlotSettings::WINDOW_WIDTH;
    int height = PlotSettings::WINDOW_HEIGHT;

    auto iters = m_generator(width, height, m_xMin, m_xMax, m_yMin, m_yMax, m_maxIters);

#if SFML_VERSION_MAJOR >= 3
    m_image.resize({(unsigned int)width, (unsigned int)height});
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            m_image.setPixel({(unsigned int)x, (unsigned int)y}, getColor(iters[y * width + x]));
        }
    }
#else
    m_image.create(width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            m_image.setPixel(x, y, getColor(iters[y * width + x]));
        }
    }
#endif

    if (!m_texture.loadFromImage(m_image)) {
        std::cerr << "Error: Could not load texture from image.\n";
    }
    m_sprite.setTexture(m_texture, true); // Reset rect to ensure visibility
    m_needsUpdate = false;
}

#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>

void ImagePlotter::saveSnapshot(sf::RenderWindow& window) {
    sf::Texture texture;
#if SFML_VERSION_MAJOR >= 3
    if (!texture.resize(window.getSize())) return;
    texture.update(window);
#else
    if (!texture.create(window.getSize().x, window.getSize().y)) return;
    texture.update(window);
#endif
    sf::Image screenshot = texture.copyToImage();

    std::filesystem::create_directories("results");
    
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::stringstream ss;
    ss << "results/mandelbrot_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".png";
    
    if (screenshot.saveToFile(ss.str())) {
        std::cout << "Snapshot saved to: " << ss.str() << std::endl;
    }
}

void ImagePlotter::show() {
    sf::ContextSettings settings;
#if SFML_VERSION_MAJOR >= 3
    settings.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({(unsigned int)PlotSettings::WINDOW_WIDTH, (unsigned int)PlotSettings::WINDOW_HEIGHT}), 
                            m_title, sf::Style::Default, sf::State::Windowed, settings);
#else
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(PlotSettings::WINDOW_WIDTH, PlotSettings::WINDOW_HEIGHT), 
                            m_title, sf::Style::Default, settings);
#endif

    sf::Font font;
#if SFML_VERSION_MAJOR >= 3
    if (!font.openFromFile(PlotSettings::FONT_PATH)) {
#else
    if (!font.loadFromFile(PlotSettings::FONT_PATH)) {
#endif
        std::cerr << "Error: Could not load font from " << PlotSettings::FONT_PATH << ".\n";
    }

    while (window.isOpen()) {
#if SFML_VERSION_MAJOR >= 3
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) window.close();
                if (keyPressed->code == sf::Keyboard::Key::P) saveSnapshot(window);
                
                double dx = (m_xMax - m_xMin) * 0.1;
                double dy = (m_yMax - m_yMin) * 0.1;

                if (keyPressed->code == sf::Keyboard::Key::W) { m_yMin -= dy; m_yMax -= dy; m_needsUpdate = true; }
                if (keyPressed->code == sf::Keyboard::Key::S) { m_yMin += dy; m_yMax += dy; m_needsUpdate = true; }
                if (keyPressed->code == sf::Keyboard::Key::A) { m_xMin -= dx; m_xMax -= dx; m_needsUpdate = true; }
                if (keyPressed->code == sf::Keyboard::Key::D) { m_xMin += dx; m_xMax += dx; m_needsUpdate = true; }
            }
            else if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                double zoomFactor = (scroll->delta > 0) ? 0.8 : 1.25;
                double xRange = m_xMax - m_xMin;
                double yRange = m_yMax - m_yMin;
                double xMid = (m_xMax + m_xMin) / 2.0;
                double yMid = (m_yMax + m_yMin) / 2.0;

                m_xMin = xMid - (xRange * zoomFactor) / 2.0;
                m_xMax = xMid + (xRange * zoomFactor) / 2.0;
                m_yMin = yMid - (yRange * zoomFactor) / 2.0;
                m_yMax = yMid + (yRange * zoomFactor) / 2.0;
                m_needsUpdate = true;
            }
        }
#else
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::P) saveSnapshot(window);

                double dx = (m_xMax - m_xMin) * 0.1;
                double dy = (m_yMax - m_yMin) * 0.1;

                if (event.key.code == sf::Keyboard::W) { m_yMin -= dy; m_yMax -= dy; m_needsUpdate = true; }
                if (event.key.code == sf::Keyboard::S) { m_yMin += dy; m_yMax += dy; m_needsUpdate = true; }
                if (event.key.code == sf::Keyboard::A) { m_xMin -= dx; m_xMax -= dx; m_needsUpdate = true; }
                if (event.key.code == sf::Keyboard::D) { m_xMin += dx; m_xMax += dx; m_needsUpdate = true; }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                double zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.8 : 1.25;
                double xRange = m_xMax - m_xMin;
                double yRange = m_yMax - m_yMin;
                double xMid = (m_xMax + m_xMin) / 2.0;
                double yMid = (m_yMax + m_yMin) / 2.0;

                m_xMin = xMid - (xRange * zoomFactor) / 2.0;
                m_xMax = xMid + (xRange * zoomFactor) / 2.0;
                m_yMin = yMid - (yRange * zoomFactor) / 2.0;
                m_yMax = yMid + (yRange * zoomFactor) / 2.0;
                m_needsUpdate = true;
            }
        }
#endif

        if (m_needsUpdate) {
            updateImage();
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(m_sprite);
        drawOverlay(window, font);
        window.display();
    }
}

void ImagePlotter::drawOverlay(sf::RenderWindow& window, const sf::Font& font) {
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

    // Calculate screen position of (0,0)
    float zeroX = (float)((0.0 - m_xMin) / (m_xMax - m_xMin) * PlotSettings::WINDOW_WIDTH);
    float zeroY = (float)(PlotSettings::WINDOW_HEIGHT - (0.0 - m_yMin) / (m_yMax - m_yMin) * PlotSettings::WINDOW_HEIGHT);

    // Draw Axis Lines through (0,0)
    if (zeroY >= 0 && zeroY <= PlotSettings::WINDOW_HEIGHT) {
        sf::RectangleShape xAxis(sf::Vector2f((float)PlotSettings::WINDOW_WIDTH, 1.0f));
        xAxis.setFillColor(sf::Color(255, 255, 255, 100)); // Slightly more translucent
#if SFML_VERSION_MAJOR >= 3
        xAxis.setPosition({0.0f, zeroY});
#else
        xAxis.setPosition(0.0f, zeroY);
#endif
        window.draw(xAxis);
    }

    if (zeroX >= 0 && zeroX <= PlotSettings::WINDOW_WIDTH) {
        sf::RectangleShape yAxis(sf::Vector2f(1.0f, (float)PlotSettings::WINDOW_HEIGHT));
        yAxis.setFillColor(sf::Color(255, 255, 255, 100));
#if SFML_VERSION_MAJOR >= 3
        yAxis.setPosition({zeroX, 0.0f});
#else
        yAxis.setPosition(zeroX, 0.0f);
#endif
        window.draw(yAxis);
    }

    // Numerical Ticks (Stay at margins for readability)
#if SFML_VERSION_MAJOR >= 3
    sf::Text tickLabel(font);
#else
    sf::Text tickLabel;
    tickLabel.setFont(font);
#endif
    tickLabel.setCharacterSize(12);
    tickLabel.setFillColor(sf::Color(200, 200, 200));

    int numTicks = 8;
    float plotWidth = PlotSettings::WINDOW_WIDTH - 2 * PlotSettings::MARGIN_X;
    float plotHeight = PlotSettings::WINDOW_HEIGHT - 2 * PlotSettings::MARGIN_Y;

    for (int i = 0; i <= numTicks; ++i) {
        // X Ticks (Real)
        float x_pos = PlotSettings::MARGIN_X + (i * plotWidth / numTicks);
        double x_val = m_xMin + (i * (m_xMax - m_xMin) / numTicks);
        
        std::stringstream ssX; ssX << std::fixed << std::setprecision(2) << x_val;
        tickLabel.setString(ssX.str());
#if SFML_VERSION_MAJOR >= 3
        tickLabel.setPosition({x_pos - 15, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y + 5});
#else
        tickLabel.setPosition(x_pos - 15, PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y + 5);
#endif
        window.draw(tickLabel);

        // Y Ticks (Imaginary)
        float y_pos = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - (i * plotHeight / numTicks);
        double y_val = m_yMin + (i * (m_yMax - m_yMin) / numTicks);

        std::stringstream ssY; ssY << std::fixed << std::setprecision(2) << y_val;
        tickLabel.setString(ssY.str());
#if SFML_VERSION_MAJOR >= 3
        tickLabel.setPosition({PlotSettings::MARGIN_X - 45, y_pos - 10});
#else
        tickLabel.setPosition(PlotSettings::MARGIN_X - 45, y_pos - 10);
#endif
        window.draw(tickLabel);
    }

    // Axes Labels
#if SFML_VERSION_MAJOR >= 3
    sf::Text xLabelText(font, m_xLabel, 18);
    xLabelText.setPosition({PlotSettings::WINDOW_WIDTH/2.0f, PlotSettings::WINDOW_HEIGHT - 40.0f});
    
    sf::Text yLabelText(font, m_yLabel, 18);
    yLabelText.setRotation(sf::degrees(-90.0f)); 
    yLabelText.setPosition({30.0f, PlotSettings::WINDOW_HEIGHT/2.0f + yLabelText.getGlobalBounds().size.x/2.0f});
#else
    sf::Text xLabelText(m_xLabel, font, 18);
    xLabelText.setPosition(PlotSettings::WINDOW_WIDTH/2.0f, PlotSettings::WINDOW_HEIGHT - 40.0f);

    sf::Text yLabelText(m_yLabel, font, 18);
    yLabelText.setRotation(-90.0f);
    yLabelText.setPosition(30.0f, PlotSettings::WINDOW_HEIGHT/2.0f + yLabelText.getGlobalBounds().width/2.0f);
#endif
    window.draw(xLabelText);
    window.draw(yLabelText);

    // Controls Overlay
#if SFML_VERSION_MAJOR >= 3
    sf::Text controlsText(font, "W/A/S/D: Pan | Scroll: Zoom | P: Snapshot | ESC: Exit", 14);
    controlsText.setFillColor(sf::Color(200, 200, 200, 180));
    controlsText.setPosition({PlotSettings::WINDOW_WIDTH - controlsText.getGlobalBounds().size.x - 20.0f, 20.0f});
#else
    sf::Text controlsText("W/A/S/D: Pan | Scroll: Zoom | P: Snapshot | ESC: Exit", font, 14);
    controlsText.setFillColor(sf::Color(200, 200, 200, 180));
    controlsText.setPosition(PlotSettings::WINDOW_WIDTH - controlsText.getGlobalBounds().width - 20.0f, 20.0f);
#endif
    window.draw(controlsText);
}
