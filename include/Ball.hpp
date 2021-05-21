#pragma once

#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable {
public:
	Ball(float radius, float maxRange, sf::Vector2f pos = sf::Vector2f(0, 0), size_t points = 50);

	void setPhysics(float gravity, float drag, float bouncy);
	void setVisibility(bool visible, float timeTotal = 0);

	void addForce(float forceX, float forceY);
	void addForce(sf::Vector2f force);

	void tick(sf::Vector2f center, float dt);
	void dragScene(float dt, sf::Vector2f dPos);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::CircleShape shape;
	sf::Vector2f pos, vel, force, dispVec = {0,0};
	float gravity, drag, bouncy;
	float timeVisible, timeTotal;
	float radius, range;
};