#pragma once

#include <SFML/Graphics.hpp>

void drawWaveInCircle(sf::RenderWindow& window, std::vector<sf::Int16> samples, sf::Vector2f center, float radius);

void drawAudioCircle(sf::RenderWindow& window, std::vector<double> samplesT, std::vector<double> psamples, size_t chunk, sf::Vector2f center, double inner, double outer);