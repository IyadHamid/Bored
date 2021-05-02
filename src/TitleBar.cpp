#include "TitleBar.hpp"
#include <iostream>
#include <string>

TitleBar::TitleBar(sf::Vector2f s) : size(s) {
	titleBarRect = sf::FloatRect(0, 0, size.x, size.y);
	titleBarShape = sf::RectangleShape(size);
    titleBarShape.setFillColor(sf::Color(0x2d, 0x2d, 0x30));

    if (not font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        throw "Font not loaded";
    
    titleText = sf::Text("Title", font, size.y * 3.f / 4.f);
    
    
    closeRect = sf::FloatRect(size.x - size.y, 0, size.y, size.y);
    closeShape = sf::VertexBuffer(sf::LineStrip, sf::VertexBuffer::Static);
    closeShape.create(5);

    sf::Vector2f crossPos(size.x - size.y / 2.f, size.y / 2.f);
    float crossRadius = size.y / 6.f;
    sf::Vertex cross[5]; 
    cross[0].position = sf::Vector2f( crossRadius,  crossRadius) + crossPos;
    cross[1].position = sf::Vector2f(-crossRadius, -crossRadius) + crossPos;
    cross[2].position = sf::Vector2f( 0          ,  0          ) + crossPos;
    cross[3].position = sf::Vector2f(-crossRadius,  crossRadius) + crossPos;
    cross[4].position = sf::Vector2f( crossRadius, -crossRadius) + crossPos;

    closeShape.update(cross);

}

void TitleBar::tick(sf::Vector2i cursor) {
    static sf::Clock clock;
    static float tfps(1.0);
    float time = clock.restart().asSeconds();

    float fps = 1.f / time;
    tfps = (tfps + fps) / 2.f;

    titleText.setString(std::to_string((int)tfps));
}

void TitleBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    

    target.draw(titleBarShape, states);
    
    target.draw(titleText);
    target.draw(closeShape, states);
}