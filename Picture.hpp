#pragma once
#include "drawable.hpp"

class Picture : public Drawable {
private:
	sf::Texture texture;
	sf::Sprite sprite;
	std::string FileName;
	sf::Color m_color = sf::Color::White;
	sf::Vector2f m_pos;
	float m_size = 30.0;


public:
	Picture(sf::Vector2f position, std::string FileName) :
		m_pos(position), FileName(FileName)
	{}

	void init() {
		if (!texture.loadFromFile(FileName))
		{
			std::cout << "ERROR OCCURRED!";
		}
		sprite.scale(sf::Vector2f(0.2, 0.2));
		sprite.setPosition(m_pos);
		sprite.setTexture(texture);
	}

	void draw(sf::RenderWindow& window)override {
		sprite.setPosition(m_pos);
		window.draw(sprite);
	}

	void update(sf::Vector2f new_pos)override {
		m_pos = { new_pos.x - 10, new_pos.y - 10 };
	}

	
	bool inHitbox(sf::Vector2f mouse_pos)override {
		if (mouse_pos.x > m_pos.x&& mouse_pos.x < m_pos.x + m_size)
		{
			if (mouse_pos.y > m_pos.y&& mouse_pos.y < m_pos.y + m_size)
			{
				return true;
			}
		}

		return false;
	}

	sf::Color getColor() override{
		return m_color;
	}

	void changeColor(sf::Color new_color)override {
		m_color = new_color;
	}

	sf::Vector2f getPos()override {
		return m_pos;
	}
	
	void write_object_to_file(std::ofstream& output)override {
		output << "(" << m_pos.x << ", " << m_pos.y << ") " << "PICTURE " << FileName << "\n";
	}

	

};