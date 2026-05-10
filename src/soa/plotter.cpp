#include "plotter.hpp"
#include "../../utils/settings.hpp"
#include "../../utils/timer.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <optional>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <immintrin.h>
#include <SFML/Config.hpp>

Plot2DSoA::Plot2DSoA(const std::string& title) 
    : m_title(title), m_zoom(1.0f), m_offsetX(0.0f), m_offsetY(0.0f),
      m_xMin(0), m_xMax(1), m_yMin(0), m_yMax(1) {}

void Plot2DSoA::setLimits(double xMin, double xMax, double yMin, double yMax) {
    m_xMin = xMin; m_xMax = xMax; m_yMin = yMin; m_yMax = yMax;
}

void Plot2DSoA::setLabels(const std::string& xLabel, const std::string& yLabel) {
    m_xLabel = xLabel; m_yLabel = yLabel;
}

void Plot2DSoA::setData(const Points2DSoA& points) {
    m_data = points;
}

void Plot2DSoA::show() {
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
        std::cerr << "Error: Could not load font.\n";
    }

    while (window.isOpen()) {
#if SFML_VERSION_MAJOR >= 3
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (scroll->delta > 0) m_zoom *= 1.1f;
                else m_zoom *= 0.9f;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::P) saveSnapshot(window);
                else if (keyPressed->code == sf::Keyboard::Key::R) {
                    m_zoom = 1.0f; m_offsetX = 0.0f; m_offsetY = 0.0f;
                }
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
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) saveSnapshot(window);
                if (event.key.code == sf::Keyboard::R) {
                    m_zoom = 1.0f; m_offsetX = 0.0f; m_offsetY = 0.0f;
                }
            }
        }
#endif

        float moveSpeed = 0.05f / m_zoom;
#if SFML_VERSION_MAJOR >= 3
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_offsetY += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_offsetY -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_offsetX -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_offsetX += moveSpeed;
#else
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
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

void Plot2DSoA::benchmarkRender(int iterations) {
    sf::RenderTexture dummy;
#if SFML_VERSION_MAJOR >= 3
    if (!dummy.resize({(unsigned int)PlotSettings::WINDOW_WIDTH, (unsigned int)PlotSettings::WINDOW_HEIGHT})) return;
#else
    if (!dummy.create(PlotSettings::WINDOW_WIDTH, PlotSettings::WINDOW_HEIGHT)) return;
#endif
    
    {
        ScopeTimer timer("Rendering: 100 passes (Scalar)");
        for (int i = 0; i < iterations; ++i) {
            drawDataScalar(dummy);
        }
    }

    {
        ScopeTimer timer("Rendering: 100 passes (SIMD)");
        for (int i = 0; i < iterations; ++i) {
            drawData(dummy);
        }
    }
}

void Plot2DSoA::saveSnapshot(sf::RenderWindow& window) {
    namespace fs = std::filesystem;
    if (!fs::exists("results")) fs::create_directory("results");

    sf::Texture texture;
#if SFML_VERSION_MAJOR >= 3
    if (!texture.resize(window.getSize())) return;
    texture.update(window);
#else
    if (!texture.create(window.getSize().x, window.getSize().y)) return;
    texture.update(window);
#endif
    sf::Image screenshot = texture.copyToImage();

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    std::string safeTitle = m_title;
    for (char &c : safeTitle) if (!isalnum(c)) c = '_';
    ss << "results/snapshot_SOA_" << safeTitle << "_" << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".png";
    screenshot.saveToFile(ss.str());
}

void Plot2DSoA::drawGridAndAxes(sf::RenderWindow& window, const sf::Font& font) {
#if SFML_VERSION_MAJOR >= 3
    sf::Text titleText(font, m_title + " (SoA)", 24);
#else
    sf::Text titleText(m_title + " (SoA)", font, 24);
#endif
    titleText.setFillColor(sf::Color::White);
    window.draw(titleText);
}

void Plot2DSoA::drawDataScalar(sf::RenderTarget& window) {
#if SFML_VERSION_MAJOR >= 3
    sf::VertexArray plotPixels(sf::PrimitiveType::Points);
#else
    sf::VertexArray plotPixels(sf::Points);
#endif
    float plotWidth = PlotSettings::WINDOW_WIDTH - (2 * PlotSettings::MARGIN_X);
    float plotHeight = PlotSettings::WINDOW_HEIGHT - (2 * PlotSettings::MARGIN_Y);

    for (size_t i = 0; i < m_data.size(); ++i) {
        float relX = (m_data.x[i] - m_xMin - m_offsetX) * m_zoom / (m_xMax - m_xMin);
        float relY = (m_data.y[i] - m_yMin - m_offsetY) * m_zoom / (m_yMax - m_yMin);

        float screen_x = PlotSettings::MARGIN_X + relX * plotWidth;
        float screen_y = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - relY * plotHeight;

        if (screen_x >= PlotSettings::MARGIN_X && screen_x <= PlotSettings::WINDOW_WIDTH - PlotSettings::MARGIN_X &&
            screen_y >= PlotSettings::MARGIN_Y && screen_y <= PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) {
#if SFML_VERSION_MAJOR >= 3
            plotPixels.append(sf::Vertex{{screen_x, screen_y}, PlotSettings::POINT_COLOR});
#else
            plotPixels.append(sf::Vertex(sf::Vector2f(screen_x, screen_y), PlotSettings::POINT_COLOR));
#endif
        }
    }
    window.draw(plotPixels);
}

void Plot2DSoA::drawData(sf::RenderTarget& window) {
#if SFML_VERSION_MAJOR >= 3
    sf::VertexArray plotPixels(sf::PrimitiveType::Points);
#else
    sf::VertexArray plotPixels(sf::Points);
#endif
    float plotWidth = PlotSettings::WINDOW_WIDTH - (2 * PlotSettings::MARGIN_X);
    float plotHeight = PlotSettings::WINDOW_HEIGHT - (2 * PlotSettings::MARGIN_Y);

    double scaleX = (m_zoom / (m_xMax - m_xMin)) * plotWidth;
    double scaleY = (m_zoom / (m_yMax - m_yMin)) * plotHeight;
    double offX = m_xMin + m_offsetX;
    double offY = m_yMin + m_offsetY;

    __m256d v_scaleX = _mm256_set1_pd(scaleX);
    __m256d v_scaleY = _mm256_set1_pd(scaleY);
    __m256d v_offX = _mm256_set1_pd(offX);
    __m256d v_offY = _mm256_set1_pd(offY);
    __m256d v_marginX = _mm256_set1_pd(PlotSettings::MARGIN_X);
    __m256d v_marginY_base = _mm256_set1_pd(PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y);

    size_t size = m_data.size();
    size_t i = 0;

    for (; i + 3 < size; i += 4) {
        __m256d x = _mm256_loadu_pd(&m_data.x[i]);
        __m256d y = _mm256_loadu_pd(&m_data.y[i]);

        __m256d sx = _mm256_add_pd(v_marginX, _mm256_mul_pd(_mm256_sub_pd(x, v_offX), v_scaleX));
        __m256d sy = _mm256_sub_pd(v_marginY_base, _mm256_mul_pd(_mm256_sub_pd(y, v_offY), v_scaleY));

        __m128 sx_f = _mm256_cvtpd_ps(sx);
        __m128 sy_f = _mm256_cvtpd_ps(sy);

        alignas(16) float ox[4];
        alignas(16) float oy[4];
        _mm_store_ps(ox, sx_f);
        _mm_store_ps(oy, sy_f);

        for (int j = 0; j < 4; ++j) {
            if (ox[j] >= PlotSettings::MARGIN_X && ox[j] <= PlotSettings::WINDOW_WIDTH - PlotSettings::MARGIN_X &&
                oy[j] >= PlotSettings::MARGIN_Y && oy[j] <= PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) {
#if SFML_VERSION_MAJOR >= 3
                plotPixels.append(sf::Vertex{{ox[j], oy[j]}, PlotSettings::POINT_COLOR});
#else
                plotPixels.append(sf::Vertex(sf::Vector2f(ox[j], oy[j]), PlotSettings::POINT_COLOR));
#endif
            }
        }
    }

    for (; i < size; ++i) {
        float relX = (m_data.x[i] - m_xMin - m_offsetX) * m_zoom / (m_xMax - m_xMin);
        float relY = (m_data.y[i] - m_yMin - m_offsetY) * m_zoom / (m_yMax - m_yMin);

        float screen_x = PlotSettings::MARGIN_X + relX * plotWidth;
        float screen_y = (PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) - relY * plotHeight;

        if (screen_x >= PlotSettings::MARGIN_X && screen_x <= PlotSettings::WINDOW_WIDTH - PlotSettings::MARGIN_X &&
            screen_y >= PlotSettings::MARGIN_Y && screen_y <= PlotSettings::WINDOW_HEIGHT - PlotSettings::MARGIN_Y) {
#if SFML_VERSION_MAJOR >= 3
            plotPixels.append(sf::Vertex{{screen_x, screen_y}, PlotSettings::POINT_COLOR});
#else
            plotPixels.append(sf::Vertex(sf::Vector2f(screen_x, screen_y), PlotSettings::POINT_COLOR));
#endif
        }
    }
    window.draw(plotPixels);
}
