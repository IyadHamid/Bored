#pragma once

#include <SFML/Graphics.hpp>

#include "CommonMath.h"

void drawWaveInCircle(sf::RenderWindow& window, std::vector<sf::Int16> samples, sf::Vector2f center, float radius);

void drawAudioCircle(sf::RenderWindow& window, CArray samplesL, CArray samplesR, size_t chunk, sf::Vector2f center, double inner, double outer);