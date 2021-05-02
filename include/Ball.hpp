#pragma once

#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable {
public:
	Ball(float radius, float maxRange, sf::Vector2f pos = sf::Vector2f(0, 0), size_t points = 50);

	void setPhysics(float gravity, float drag, float bouncy);
	void setVisibility(bool visible, float timeTotal = 0);

	void addImpulse(float impulseX, float impulseY);
	void addImpulse(sf::Vector2f impulse);

	void tick(float dt, sf::Vector2f center);
	void dragScene(float dt, sf::Vector2f dPos);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::CircleShape shape;
	sf::Vector2f pos, vel, impulse, dispVec = {0,0};
	float gravity, drag, bouncy;
	float timeVisible, timeTotal;
	float radius, range;
};