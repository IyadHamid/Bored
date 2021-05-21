#include "SampleDraw.h"

#include <algorithm>
#include <utility>

#include "CommonMath.h"

const double modifier(const double x, const double inner, const double outer) {
    //const auto equ = [outer](double x) {return -outer * std::exp(-0.02 * x); };
    //const double intercept = equ(0);
    //return equ(x) - intercept + inner;
    const double r = outer / 2.0;
    const auto equ = [r](double x) {return r * tanh(.03 * (x - 50.0)) + r; };
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

sf::Color vertexColorCircle(const double relativeFreq) {
    const double octave = floor(relativeFreq);
    const double shade = std::clamp(octave * octave * 8.0 + 16.0, 0.0, 255.0);
    return sf::Color(0, shade, shade);
}

void subdivideCircle(std::vector<sf::Vertex>& va, std::vector<std::pair<double, double>>& values, double inner, double outer, sf::Vector2f center) {
    va.reserve(va.size() * 2);
    const double vaSize = va.size();
    for (size_t i = vaSize - 1; i > 0; i--) {

        const double angle = atan2(sin(values[i].second) + sin(values[i - 1].second), cos(values[i].second) + cos(values[i - 1].second));
        double value = (values[i].first + values[i - 1].first) / 2.0;

        values.emplace(values.begin() + i, value, angle);

        const sf::Color color = va[i].color;// vertexColorCircle(value, inner, outer);
        const sf::Vector2f pos(value * sin(angle), value * cos(angle));
        const sf::Vertex vert(pos + center, color);

        va.insert(va.begin() + i, vert);
    }
}

void drawAudioCircle(sf::RenderWindow& window, AudioData data, sf::Vector2f center, double inner, double outer, double dt) {
    static const unsigned minResolution = 64;
    

    std::vector<sf::Vertex> va;
    std::vector<std::pair<double, double>> values;
    bool left = true;
    for (size_t i = 0, il = 0, ir = 0; i < data.chunk; i++) {
        double relativeFreq = std::log2(1 + (left ? il : ir));
        //Frequency shift from octave
        double shift = fmod(relativeFreq, 1.0);
        if (isnan(shift))
            shift = 0;

        if (shift == 0 && left) [[unlikely]] {
            left = false;
            ir = il;
            shift = 1.0;
        }
        else if (shift == 0 && !left) [[unlikely]] {
            left = true;

            if (va.size() >= 1024) [[unlikely]]
                break;

            if (va.size() >= 8) {
                va.push_back(va[0]); //Finish loop
                values.push_back(values[0]);
                //Subdivide
                while (va.size() <= minResolution)
                    subdivideCircle(va, values, inner, outer, center);
                window.draw(va.data(), va.size(), sf::PrimitiveType::LineStrip);
            }

            va.clear();
            values.clear();
        }

        double angle = shift * pi * (left ? -1.0 : 1.0);

        double value = abs(left ? data.samplesL[il].real() : data.samplesR[ir].real());
        value /= data.smoothedAmplitude;

        value = modifier(value, inner, outer - (128.0 / floor(relativeFreq))); 
        values.emplace_back(value, angle);

        //inverted sin/cos to rotate pi/2
        const sf::Vector2f pos(value * sin(angle), value * cos(angle));

        const sf::Color color = vertexColorCircle(relativeFreq);
        va.emplace_back(pos + center, color);

        if (left)
            il++;
        else
            ir--;
    }

    //draw inner circle
    va.clear();
    va.reserve(minResolution);
    for (double i = 0; i < 2.0 * pi; i += 2.0 * pi / minResolution) {
        const sf::Vector2f pos(inner * cos(i), inner * sin(i));
        va.emplace_back(pos + center, sf::Color(0, 255, 255));
    }
    va.push_back(va[0]);
    window.draw(va.data(), va.size(), sf::PrimitiveType::LineStrip);
}