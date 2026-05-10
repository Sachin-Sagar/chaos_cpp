#pragma once
#include <chrono>
#include <iostream>
#include <string>

class ScopeTimer {
public:
    explicit ScopeTimer(const std::string& name) 
        : m_name(name), m_start(std::chrono::high_resolution_clock::now()) {}

    ~ScopeTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - m_start;
        std::cout << "[TIMER] " << m_name << " took: " << elapsed.count() << " ms\n";
    }

private:
    std::string m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};
