#include "AudioData.hpp"

#include "FFT.h"

AudioData::AudioData(std::vector<sf::Int16> samples, const size_t chunk, const std::vector<double> window) : samples(samples), chunk(chunk) {

	samples.resize(chunk * 2);
	samplesL = CArray(chunk);
	samplesR = CArray(chunk);
	amplitudeL = 1.0;
	amplitudeR = 1.0;
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