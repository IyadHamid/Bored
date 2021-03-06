#include <cassert>
#include <algorithm>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "AudioData.hpp"
#include "SamplerRecorder.hpp"
#include "FFT.h"
#include "Ball.hpp"
#include "TitleBar.hpp"
#include "SampleDraw.h"
#include "Enviroment.h"

int main() {
    if (!sf::SoundRecorder::isAvailable())
        return 1;

    sf::ContextSettings settings{};
    settings.antialiasingLevel = 8;
    sf::Vector2u res(env::resX, env::resY);
    sf::Vector2f center(res.x / 2.f, res.y / 2.f);
    sf::RenderWindow window(sf::VideoMode(res.x, res.y), "Bored", sf::Style::None, settings);
    sf::Clock clock;

    sf::Mouse mouse;
    bool mouseDrag = false;
    sf::Vector2i mouseOffset;

    TitleBar titleBar(sf::Vector2f(res.x, 32));

    std::vector<double> psamples(env::chunk);
    std::vector<double> windowSample;
    for (size_t i = 0; i < env::chunk; i++)
        windowSample.push_back(hanningWindow(i, env::chunk));

    Ball ball(25, env::inner);
    ball.setPhysics(7000, .0001, 1.5);

    SamplerRecorder input(env::clock);
    input.setDeviceSearch("stereomix");
    input.setChannelCount(2);
    input.start();

    
    //window.setFramerateLimit(80);
    window.setVerticalSyncEnabled(true);

    bool gainedFocus = false;

    AudioData data(input.getData().first, env::chunk, windowSample);

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
                ball.addForce(-10000, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                ball.addForce(10000, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                window.setFramerateLimit(40);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                window.setFramerateLimit(0);
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
        data.update(samples.first, samples.second, windowSample, dt);


        drawAudioCircle(window, data, center, env::inner, env::outer, dt);
        drawWaveInCircle(window, data, center, env::inner);

        window.draw(ball);
        
        ball.tick(center, dt);
        titleBar.tick("");

        window.draw(titleBar);

        window.display();
        window.clear();
    }
    return 0;
}

int WinMain() {
    return main();
}