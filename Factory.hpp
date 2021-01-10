#pragma once
#include "Exceptions.hpp"
// Operators
std::ifstream& operator>>(std::ifstream& input, sf::Color& rhs) {
	std::string s;
	input >> s;
	const struct { const char* name; sf::Color color; } colors[]{
		{ "yellow", sf::Color::Yellow },
		{ "red",    sf::Color::Red },
		{ "blue",    sf::Color::Blue },
		{ "green",    sf::Color::Green },
		{ "white",    sf::Color::White },
		{"cyan", sf::Color::Cyan}
	};

	for (auto const& color : colors) {
		if (color.name == s) {
			rhs = color.color;
			return input;
		}
	}
	if (s == "") {
		throw end_of_file();
	}
	throw unknown_color(s);
}

std::ifstream& operator>>(std::ifstream& input, sf::Vector2f& rhs) {
	char c;

	if (!(input >> c)) {
		throw end_of_file();
	}
	if (c != '(') {
		throw read_exception("Kan open haakje niet lezen");
	}
	if (!(input >> rhs.x)) {
		throw read_exception("kan x coordinaat niet ophalen");
	}
	if (!(input >> c)) {
		throw end_of_file();
	}
	if (!(input >> rhs.y)) {
		throw read_exception("kan y coordinaat niet ophalen");
	}
	if (!(input >> c)) {
		throw end_of_file();
	}
	if (c != ')') {
		throw read_exception("Kan sluit haakje niet lezen");
	}

	return input;
}

std::shared_ptr<Drawable> screen_object_read(std::ifstream& input) {
	sf::Vector2f position;
	std::string name;
	sf::Color color;
	input >> position >> name;


	if (name == "CIRCLE") {
		float size;
		input >> color;
		if (!(input >> size)) {
			throw read_exception("Kan size niet uitlezen");
		};
		return std::make_shared<Circle>(position, color, size);
	}
	else if (name == "RECTANGLE") {
		sf::Vector2f size;
		input >> color;
		if (!(input >> size)) {
			throw read_exception("Kan size niet uitlezen");
		};
		return std::make_shared<Rectangle>(position, color, size);
	}

	else if (name == "PICTURE") {
		std::string filename;
		if (!(input >> filename)) {
			throw read_exception("Kan filename niet uitlezen");
		};
		return std::make_shared<Picture>(position, filename);
	}


	else if (name == "") {
		throw end_of_file();
	}

	throw unknown_shape(name);
}
