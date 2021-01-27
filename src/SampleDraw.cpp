#include "SampleDraw.h"

#include <algorithm>

#include "CommonMath.h"

const double modifier(double x, const double outer, const double inner) {
    const auto equ = [outer](double x) {return -outer * std::exp2(-.02 * x); };
    const double intercept = equ(0);
    return equ(x) - intercept + inner;
}

void drawWaveInCircle(sf::RenderWindow& window, std::vector<sf::Int16> samples, sf::Vector2f center, float radius) {
    std::vector<sf::Vertex> va;
    static float waveAmplitude = 0;
    float nextWaveAmplitude = 0;
    for (size_t i = 0; i < samples.size(); i += 4) {
        sf::Vector2f pos((float)i / samples.size() * 2 * radius - radius, samples[i] / (waveAmplitude + 1.f) * radius / 4.f);
        
        if (abs(samples[i]) > nextWaveAmplitude)
            nextWaveAmplitude = abs(samples[i]);
        if (len(pos) > radius)
            pos.y = sqrt(1 - pos.x * pos.x) * (pos.y > 0 ? 1 : -1);
        pos += center;
        va.emplace_back(pos);
    }
    waveAmplitude = (waveAmplitude * 2 + nextWaveAmplitude) / 3.f;
    window.draw(&va[0], va.size(), sf::PrimitiveType::LineStrip);
}

void drawAudioCircle(sf::RenderWindow& window, std::vector<double> samplesT, std::vector<double> psamples, size_t chunk, sf::Vector2f center, double inner, double outer) {
    std::vector<sf::Vertex> va;
    for (size_t i = 0; i < chunk; i++) {
        double value = abs(samplesT[i]) / 500;
        if (value > psamples[i])
            value = (psamples[i] + 7 * value) / 8;
        else
            value = (7 * psamples[i] + value) / 8;
        psamples[i] = value;

        value = modifier(value, outer, inner);
        double shade = std::clamp((value - inner) / outer * 255, 0.0, 255.0);

        float angle = (float)i / chunk * 32 * pi + pi;

        sf::Vector2f pos(value * sin(angle), value * cos(angle));
        pos += center;
        va.emplace_back(pos, sf::Color((sf::Uint8)shade, 255, 255));
    }
    va.push_back(va[0]);
    window.draw(&va[0], va.size(), sf::PrimitiveType::LineStrip);
}
