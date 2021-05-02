#pragma once

#include <vector>

#include <SFML/Audio.hpp>

#include "CommonMath.h"

struct AudioData {
	AudioData(std::vector<sf::Int16> samples, const size_t chunk, const std::vector<double> window);
	std::vector<sf::Int16> samples;
	CArray samplesL, samplesR;
	double amplitudeL, amplitudeR, amplitude;
	size_t chunk;
};