#pragma once
#include "drawable.hpp"

class Rectangle : public Drawable {
	sf::Vector2f m_pos;
	sf::Color m_color;
	sf::Vector2f m_size;

public:
	Rectangle(sf::Vector2f position, sf::Color color, sf::Vector2f size = { 20, 20 }) :
		m_pos(position), m_color(color), m_size(size)
	{}

	void draw(sf::RenderWindow& window)override {
		sf::RectangleShape rectangle({ m_size.x, m_size.y });
		rectangle.setPosition(m_pos);
		rectangle.setFillColor(m_color);
		window.draw(rectangle);
	}

	void update(sf::Vector2f new_pos)override {
		m_pos = { new_pos.x - m_size.x / 2, new_pos.y - m_size.y / 2};
	}

	
	bool inHitbox(sf::Vector2f mouse_pos)override {
		if (mouse_pos.x > m_pos.x&& mouse_pos.x < m_pos.x + m_size.x)
		{
			if (mouse_pos.y > m_pos.y&& mouse_pos.y < m_pos.y + m_size.y)
			{
				return true;
			}
		}

		return false;
	}

	void changeColor(sf::Color new_color)override {
		m_color = new_color;
	}

	sf::Color getColor() override {
		return m_color;
	}

	sf::Vector2f getPos()override {
		return m_pos;
	}
	
	void write_object_to_file(std::ofstream& output)override {
		auto color = "";
		if (m_color == sf::Color::Yellow) color = "yellow";
		if (m_color == sf::Color::Red) color = "red";
		if (m_color == sf::Color::Green) color = "green";
		if (m_color == sf::Color::Blue) color = "blue";
		if (m_color == sf::Color::White) color = "white";
		if (m_color == sf::Color::Cyan) color = "cyan";

		output << "(" << m_pos.x << ", " << m_pos.y << ") " << "RECTANGLE " << color << " (" << m_size.x << ", " << m_size.y << ")\n";
	}
};