#include "SampleDraw.h"

#include <algorithm>
#include <utility>

#include "CommonMath.h"

const double modifier(double x, const double outer, const double inner) {
    const auto equ = [outer](double x) {return -outer * std::exp2(-0.02 * x); };
    const double intercept = equ(0);
    return equ(x) - intercept + inner;
}

void drawWaveInCircle(sf::RenderWindow& window, AudioData data, sf::Vector2f center, float radius) {
    std::vector<sf::Vertex> va;

    for (size_t i = 0; i < data.samples.size(); i++) {
        const auto percent = (float)i / data.samples.size();
        sf::Vector2f pos(percent * 2.f * radius - radius, (data.samples[i] / data.smoothedAmplitude) * (radius / 4.f));
        if (len(pos) > radius)
            pos.y = sqrt(1 - pos.x * pos.x) * (pos.y > 0 ? 1 : -1);
        va.emplace_back(pos + center, sf::Color(200, 200, 200));
    }

    window.draw(va.data(), va.size(), sf::PrimitiveType::LineStrip);
}

sf::Color vertexColorCircle(const double value, const double inner, const double outer) {
    double shade = (value - inner) / outer;
    shade = shade * 25 + 200.0;
    shade = std::clamp(shade, 0.0, 255.0);
    return sf::Color(0, shade, shade);
}

void subdivideCircle(std::vector<sf::Vertex>& va, std::vector<std::pair<double, double>>& values, double inner, double outer, sf::Vector2f center) {
    va.reserve(va.size() * 2);
    const double vaSize = va.size();
    for (size_t i = vaSize - 1; i > 0; i--) {

        const double angle = atan2(sin(values[i].second) + sin(values[i - 1].second), cos(values[i].second) + cos(values[i - 1].second));
        const double ratio = values[i].first > values[i - 1].first ? 0.1 : 0.9;
        double value = values[i].first * ratio + values[i - 1].first * (1.0 - ratio);

        values.emplace(values.begin() + i, value, angle);

        value = modifier(value, outer, inner);

        const sf::Color color = vertexColorCircle(value, inner, outer);
        const sf::Vector2f pos(value * sin(angle), value * cos(angle));
        const sf::Vertex vert(pos + center, color);

        va.insert(va.begin() + i, vert);
    }
}

void drawAudioCircle(sf::RenderWindow& window, AudioData data, sf::Vector2f center, double inner, double outer, double dt) {
    std::vector<sf::Vertex> va;
    std::vector<std::pair<double, double>> values;
    bool left = true;
    for (size_t i = 0, il = 0, ir = 0; i < data.chunk; i++) {

        double relativeFreq = std::log2(1 + (left ? il : ir));
        //Frequency shift from octave
        double shift = fmod(relativeFreq, 1.0);
        if (isnan(shift))
            shift = 0;

        if (shift == 0 && left) {
            left = false;
            ir = il;
            shift = 1.0;
        }
        else if (shift == 0 && !left) {
            left = true;

            if (va.size() >= 8) {
                va.push_back(va[0]); //Finish loop
                values.push_back(values[0]);    
                //Subdivide
                while (va.size() <= 64) {
                    subdivideCircle(va, values, inner, outer, center);
                }
                window.draw(va.data(), va.size(), sf::PrimitiveType::LineStrip);
            }

            va.clear();
            values.clear();
        }

        double angle = shift * pi * (left ? -1.0 : 1.0);

        double value = abs(left ? data.samplesL[il].real() : data.samplesR[ir].real());
        value /= data.smoothedAmplitude;

        value *= std::log10(relativeFreq);

        values.emplace_back(value, angle);

        value = modifier(value, outer, inner);

        const sf::Vector2f pos(value * sin(angle), value * cos(angle));

        const sf::Color color = vertexColorCircle(value, inner, outer);
        va.emplace_back(pos + center, color);

        if (left)
            il++;
        else
            ir--;
    }
}