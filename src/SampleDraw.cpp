#include "SampleDraw.h"

#include <algorithm>

#include <iostream>
#include "CommonMath.h"

const double modifier(double x, const double outer, const double inner) {
    const auto equ = [outer](double x) {return -outer * std::exp2(-.01 * x); };
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
    window.draw(va.data(), va.size(), sf::PrimitiveType::LineStrip);
}

void drawAudioCircle(sf::RenderWindow& window, CArray samplesL, CArray samplesR, size_t chunk, sf::Vector2f center, double inner, double outer) {
    std::vector<sf::Vertex> va;

    bool left = true;
    for (size_t i = 0, il = 0, ir = 0; i < chunk; i++) {
        //Frequency shift from octave
        double shift = fmod(std::log2(1 + (left ? il : ir)), 1.0);
        if (isnan(shift))
            shift = 0;

        if (shift == 0 && left) {
            left = false;
            ir = il;
            shift = 1.0;
        }
        else if (shift == 0 && !left) {
            left = true;
            if (va.size() >= 16) {
                va.push_back(va[0]); //Finish loop
                window.draw(va.data(), va.size(), sf::PrimitiveType::LineStrip);
            }
            va.clear();
        }

        double value = abs(left ? samplesL[il].real() : samplesR[ir].real()) / 500;
        
        value = modifier(value, outer, inner);

      
        float angle = shift * pi * (left ? -1.0f : 1.0f);
        

        sf::Vector2f pos(value * sin(angle), value * cos(angle));
        pos += center;
        double shade = std::clamp((value - inner) / outer * 255, 0.0, 255.0);
        //va[i] = sf::Vertex(pos, sf::Color((sf::Uint8)shade, 255, 255));
        va.emplace_back(pos, sf::Color((sf::Uint8)shade, 255, 255));


        if (left)
            il++;
        else
            ir--;
    }
    //for (int i = 0; i < 8; i++)
    //    va[i].color = sf::Color(0.0, 0.0, 0.0);
    //va.push_back(va[0]); //Finish loop
}