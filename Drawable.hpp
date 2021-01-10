#pragma once
#include <SFML/Graphics.hpp>

class Drawable {
public:

	virtual void draw(sf::RenderWindow& window) = 0;

	virtual void update(sf::Vector2f new_position) = 0;

	virtual bool inHitbox(sf::Vector2f mouse_pos) = 0;

	virtual sf::Color getColor() = 0;

	virtual sf::Vector2f getPos() = 0;

	virtual void changeColor(sf::Color new_color) = 0;
	//virtual void setToback() = 0;

	virtual void write_object_to_file(std::ofstream& output) = 0;

};
