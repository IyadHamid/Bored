#pragma once

#include <SFML/Graphics.hpp>

#include "AudioData.hpp"
#include "CommonMath.h"

void drawWaveInCircle(sf::RenderWindow& window, AudioData data, sf::Vector2f center, float radius);

void drawAudioCircle(sf::RenderWindow& window, AudioData data, sf::Vector2f center, double inner, double outer, double dt);