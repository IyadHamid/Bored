#pragma once

#include <vector>

#include <SFML/Audio.hpp>

#include "CommonMath.h"

struct AudioData {
	AudioData(std::vector<sf::Int16> samples, const size_t chunk, const std::vector<double> window);

	void update(std::vector<sf::Int16> newSamples, const std::vector<double> window, double dt);

	std::vector<sf::Int16> samples;
	CArray samplesL, samplesR;
	double amplitudeL, amplitudeR, amplitude;
	size_t chunk;

private:
	void init(const std::vector<double> window);
};