#pragma once

#include <SFML/Graphics.hpp>
#include "drawable.hpp"
#include <fstream>

class Circle : public Drawable {
private:
	sf::Vector2f m_pos;
	sf::Color m_color;
	float m_size;
	sf::CircleShape circle;

public:
	Circle(sf::Vector2f position, sf::Color color, float size = 30.0) :
		m_pos(position), m_color(color), m_size(size)
	{}

	void draw(sf::RenderWindow& window)override {
		circle.setRadius(m_size);
		circle.setPosition(m_pos);
		circle.setFillColor(m_color);
		window.draw(circle);
	}

	void update(sf::Vector2f new_pos)override {
		//m_pos = { new_pos.x - m_size / 2, new_pos.y - m_size / 2 };
		m_pos = new_pos - sf::Vector2f(m_size, m_size) / 1.0f;
	}

	bool inHitbox(sf::Vector2f mouse_pos)override {
		if (mouse_pos.x > m_pos.x&& mouse_pos.x < m_pos.x + m_size * 2)
		{
			if (mouse_pos.y > m_pos.y&& mouse_pos.y < m_pos.y + m_size * 2)
			{
				return true;
			}
		}
		return false;
	}

	sf::Color getColor()override {
		return m_color;
	}

	void changeColor(sf::Color new_color)override {
		m_color = new_color;
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
		
		output << "(" << m_pos.x << ", " << m_pos.y << ") " << "CIRCLE " << color << " " << m_size << "\n";

	}
	

};
