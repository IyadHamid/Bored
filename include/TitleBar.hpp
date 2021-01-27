#pragma once

#include <SFML/Graphics.hpp>

class TitleBar : public sf::Drawable {
public:
	TitleBar(sf::Vector2f size);

	void tick(sf::Vector2f cursor);

	sf::FloatRect titleBarRect, closeRect;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Vector2f size;
	
	sf::RectangleShape titleBarShape;
	sf::VertexBuffer closeShape;
};