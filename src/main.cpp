#include <cassert>
#include <iostream>
#include <algorithm>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SamplerRecorder.hpp"
#include "FFT.h"
#include "CommonMath.h"
#include "Ball.hpp"
#include "TitleBar.hpp"
#include "SampleDraw.h"



sf::Text fps;

int main() {
    if (!sf::SoundRecorder::isAvailable())
        return 1;
    if (!sf::Shader::isAvailable())
        return 1;

    sf::ContextSettings settings{};
    settings.antialiasingLevel = 8;
    sf::Vector2u res(1024, 1024);
    sf::Vector2f center(res.x / 2.f, res.y / 2.f);
    sf::RenderWindow window(sf::VideoMode(res.x, res.y), "Bored", sf::Style::None, settings);
    sf::Clock clock;

    sf::Mouse mouse;
    bool mouseDrag = false;
    sf::Vector2i mouseOffset;

    TitleBar titleBar(sf::Vector2f(res.x, 32));

    const size_t chunk = 2048;
    const float inner = 300;
    const float outer = 200;

    std::vector<double> psamples(chunk);
    std::vector<double> windowSample;
    for (size_t i = 0; i < chunk; i++)
        windowSample.push_back(hanningWindow((double)i, (double)chunk));

    Ball ball(25, inner);
    ball.setPhysics(7000, .0001, 1.5);

    SamplerRecorder input(50);
    input.setDeviceSearch("stereomix");
    input.setChannelCount(2);

    input.start();

    bool gainedFocus = false;

    while (window.isOpen()) {

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (titleBar.closeRect.contains(event.mouseButton.x, event.mouseButton.y))
                    window.close();
                else if (titleBar.titleBarRect.contains(event.mouseButton.x, event.mouseButton.y)) {
                    mouseDrag = true;
                    mouseOffset = mouse.getPosition(window);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased)
                mouseDrag = false;
        }
        if (window.hasFocus()) {
            if (gainedFocus) {
                ball.setVisibility(true);
                gainedFocus = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                ball.addImpulse(-10000, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                ball.addImpulse(10000, 0);
            if (mouseDrag) {
                sf::Vector2f dPos;
                dPos = sf::Vector2f(window.getPosition());
                window.setPosition(mouse.getPosition() - mouseOffset);
                dPos -= sf::Vector2f(window.getPosition());
                ball.dragScene(dt, dPos);
            }
        }
        else {
            if (!gainedFocus) {
                ball.setVisibility(false, .5f);
                gainedFocus = true;
            }
        }

        auto samples = input.getData();

        samples.resize(chunk * 2);

        CArray samplesL(chunk), samplesR(chunk);

        //Set to 1 to not break ratio.
        double amplitudeL = 1.0, amplitudeR = 1.0;

        for (size_t i = 0; i < chunk; i++) {
            samplesL[i] = windowSample[i] * samples[i * 2 + 0];
            samplesR[i] = windowSample[i] * samples[i * 2 + 1];
            amplitudeL  = std::max(amplitudeL, samplesL[i].real());
            amplitudeR  = std::max(amplitudeR, samplesR[i].real());
        }

        double amplitudeRatio = amplitudeL / amplitudeR; //Not used

        fft(samplesL);
        fft(samplesR);

        drawWaveInCircle(window, samples, center, inner);
        drawAudioCircle(window, samplesL, samplesR, chunk, center, inner, outer);
        ball.tick(dt, center);
        window.draw(ball);

        window.draw(titleBar);

        window.display();
        window.clear();
    }
    return 0;
}

int WinMain() {
    return main();
}