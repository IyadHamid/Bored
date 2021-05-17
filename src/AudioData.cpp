#include "AudioData.hpp"

#include "FFT.h"

AudioData::AudioData(std::vector<sf::Int16> samples, const size_t chunk, const std::vector<double> window) : samples(samples), chunk(chunk) {
	samples.resize(chunk * 2);
	init(window);
}

void AudioData::update(std::vector<sf::Int16> newSamples, bool isNew, const std::vector<double> window, double dt) {
	const double factor = 1.0 - std::exp(-dt * 50.0);
	for (size_t i = 0; i < samples.size(); i++) {
		const double curr = newSamples[i];
		const double prev = samples[i];

		samples[i] = prev + factor * (curr - prev);
	}
	auto oldAmplitude = amplitude;
	init(window);
	smoothedAmplitude = oldAmplitude + factor * (amplitude - oldAmplitude);
}

void AudioData::init(const std::vector<double> window) {
	samplesL = samplesR = CArray(chunk);
	amplitudeL = amplitudeR = 1.0;
	for (size_t i = 0; i < chunk; i++) {
		samplesL[i] = window[i] * samples[i * 2 + 0];
		samplesR[i] = window[i] * samples[i * 2 + 1];
		amplitudeL = std::max(amplitudeL, samplesL[i].real());
		amplitudeR = std::max(amplitudeR, samplesR[i].real());
	}
	amplitude = std::max(amplitudeL, amplitudeR);

	fft(samplesL);
	fft(samplesR);
}