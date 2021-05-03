#include "Ball.hpp"

#include "CommonMath.h"

#include <iostream>

Ball::Ball(float r, float maxRange, sf::Vector2f p, size_t pts) : radius(r), range(maxRange - r), pos(p) {
	shape = sf::CircleShape(radius, pts);
    vel = { 0, 0 };
    impulse = { 0, 0 };
    shape.setOrigin(radius, radius);
    shape.setFillColor(sf::Color(0x22, 0xcc, 0x22));

    gravity = drag = 0;
    bouncy = 2;
    
    timeVisible = 1;
    timeTotal = 0;
}

void Ball::setPhysics(float g, float d, float b) {
    gravity = g;
    drag = d;
    bouncy = b;
}

void Ball::setVisibility(bool visible, float time) {
    if (visible) {
        sf::Color color(shape.getFillColor());
        color.a = 0xff;
        shape.setFillColor(color);
        timeTotal = 0;
        timeVisible = 1;
    }
    else if (time == 0) {
        sf::Color color(shape.getFillColor());
        color.a = 0x00;
        shape.setFillColor(color);
        timeVisible = timeTotal = 0;
    }
    else 
        timeVisible = timeTotal = time;
}

void Ball::addImpulse(float impulseX, float impulseY) {
    impulse.x += impulseX;
    impulse.y += impulseY;
}

void Ball::addImpulse(sf::Vector2f f) {
    impulse += f;
}

void Ball::tick(float dt, sf::Vector2f center) {
    if (timeVisible > 0) {
        vel += impulse * dt;
        impulse = { 0, 0 };

        pos += vel * dt;
        float mag = len(pos);
        if (mag > range) {
            pos /= mag;
            vel = vel - pos * bouncy * dot(vel, pos);
            pos *= range;
        }

        mag = len(vel);
        if (mag != 0) {
            sf::Vector2f dragForce = -vel / mag;
            dragForce *= mag * mag * drag;
            addImpulse(dragForce);
        }

        addImpulse(0, gravity);

        shape.setPosition(pos + center);

        if (timeTotal > 0) {
            timeVisible -= dt;
            if (timeVisible < 0)
                timeVisible = 0;
            sf::Color color(shape.getFillColor());
            color.a = (sf::Uint8)(timeVisible / timeTotal * 255.f);
            shape.setFillColor(color);
        }
    }
}

void Ball::dragScene(float dt, sf::Vector2f dPos) {
    pos += dPos;
    sf::Vector2f pPos = pos;
    float mag = len(pos);
    if (mag > range) {
        pos *= range / mag;

        vel += (pos - pPos) / dt;
    }
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(shape, states);

    std::vector<sf::Vertex> vec;
    vec.emplace_back(shape.getPosition());
    vec.emplace_back(shape.getPosition() + dispVec);
    target.draw(&vec[0], vec.size(), sf::PrimitiveType::Lines, states);
}