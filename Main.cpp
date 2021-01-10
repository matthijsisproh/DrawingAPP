#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>
#include "Drawable.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Picture.hpp"
#include "Factory.hpp"
#include "Exceptions.hpp"
#include <memory>
#include <iterator>
#include <algorithm>

void write_to_file(std::string filename, std::vector<std::shared_ptr<Drawable>> & objects ) {
	std::ofstream output;
	output.open(filename, std::ofstream::out);
	output << "";
	output.close();
	output.open(filename, std::ofstream::app | std::ofstream::out);
	for (auto& object : objects) {
		object->write_object_to_file(output);
	}
	output.close();
}

void read_from_file(std::string filename, std::vector<std::shared_ptr<Drawable>> & objects) {
	std::ifstream input(filename);

	try {
		for (;;) {
			objects.push_back(screen_object_read(input));
		}
	}
	catch (end_of_file) {
		// do nothing
	}
	catch (std::exception & problem) {
		std::cout << problem.what();
	}

}

int main(int argc, char* argv[]) {
	sf::RenderWindow window{ sf::VideoMode{ 640, 480 }, "SFML window" };

	//GUI objects
	auto selectIcon = std::make_shared<Picture>(Picture({ 600, 5 }, "icon0.png"));
	auto deleteIcon = std::make_shared<Picture>(Picture({ 600, 50 }, "icon1.jpg"));
	auto toBackIcon = std::make_shared<Picture>(Picture({ 600, 95 }, "icon2.png"));

	std::vector<std::shared_ptr<Drawable>> gui_objects = {selectIcon, deleteIcon, toBackIcon};

	// COLOR PICKER
	auto rect1 = std::make_shared<Rectangle>(Rectangle({ 600, 230 }, sf::Color::Cyan, { 40, 40 }));
	auto rect2 = std::make_shared<Rectangle>(Rectangle({ 600, 275 }, sf::Color::Blue, { 40, 40 }));
	auto rect3 = std::make_shared<Rectangle>(Rectangle({ 600, 320 }, sf::Color::Yellow, { 40, 40 }));
	auto rect4 = std::make_shared<Rectangle>(Rectangle({ 600, 365 }, sf::Color::Green, { 40, 40 }));
	auto rect5 = std::make_shared<Rectangle>(Rectangle({ 600, 410 }, sf::Color::Red, { 40, 40 }));

	std::vector<std::shared_ptr<Drawable>> colorpick_objects = {rect1, rect2, rect3, rect4, rect5};

	// SHAPE PICKER 
	auto circleshape = std::make_shared<Circle>(Circle({ 600, 140 }, sf::Color::White, 20));
	auto rectshape = std::make_shared<Rectangle>(Rectangle({ 600, 185 }, sf::Color::White, { 40, 40 }));

	std::vector<std::shared_ptr<Drawable>> shapepick_objects = { circleshape, rectshape };

	std::vector<std::shared_ptr<Drawable>> movable_objects = {};
	
	//Factory object reader
	read_from_file("objects.txt", movable_objects);

	sf::Color last_picked_color = sf::Color::White;

	//Enum
	enum State_t { IDLE, SELECT, DELETE, TOBACK, PICK_COLOR, PICK_SHAPE, CHANGE_COLOR};
	State_t State = TOBACK;

	//Init Icons
	selectIcon->init();
	deleteIcon->init();
	toBackIcon->init();

	//Lambda functions
	auto drawAllObjects = [&]() {
		window.clear();
		for (auto& object : shapepick_objects) {
			object->draw(window);
		}
		for (auto& object : movable_objects) {
			object->draw(window);
		}
		for (auto& object : colorpick_objects) {
			object->draw(window);
		}
		for (auto& object : gui_objects) {
			object->draw(window);
		}
		window.display();
	};

	auto StateSelect = [&](std::shared_ptr<Drawable> object, State_t state, sf::Vector2i mouse_pos) {
		if (object->inHitbox(sf::Vector2f(mouse_pos))) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				State = state;
			}
		}
	};

	//Main loop
	while (window.isOpen()) {
		drawAllObjects();
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

		//STATE SELECTOR
		StateSelect(deleteIcon, DELETE, mouse_pos);
		StateSelect(toBackIcon, TOBACK, mouse_pos);
		StateSelect(selectIcon, SELECT, mouse_pos);

		for (auto& object : shapepick_objects) {
			StateSelect(object, PICK_SHAPE, mouse_pos);
		}

		for (auto& object : colorpick_objects) {
			StateSelect(object, PICK_COLOR, mouse_pos);
		}

		//SWITCH STATE
		switch (State) {
		case IDLE:
		{
				drawAllObjects();
		}
			break;

		case SELECT:
		{
			for (auto& object : movable_objects) {
				if (object->inHitbox(sf::Vector2f(mouse_pos))){
					while ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))) {
						mouse_pos = sf::Mouse::getPosition(window);
						object->update(sf::Vector2f(mouse_pos));
						drawAllObjects();
					}
				}
			}
		}
			break;

		case DELETE:
		{
			std::vector<std::shared_ptr<Drawable>>::iterator object_iterator = movable_objects.end();
			for (auto& object : movable_objects) {
				if (object->inHitbox(sf::Vector2f(mouse_pos)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						object_iterator = std::find(movable_objects.begin(), movable_objects.end(), object);
				}
			}

			if (object_iterator != movable_objects.end()) {
				movable_objects.erase(object_iterator);
			}
		}
			break;

		case TOBACK:
		{
			for (auto& object : movable_objects) {
				if (object->inHitbox(sf::Vector2f(mouse_pos)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					std::reverse(movable_objects.begin(), movable_objects.end());
				}
			}
		}
			break;

		case PICK_COLOR:
		{
			for (auto& object : colorpick_objects) {
				if (object->inHitbox(sf::Vector2f(mouse_pos)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					last_picked_color = object->getColor();
					for (auto& x : shapepick_objects) {
						x->changeColor(last_picked_color);
					}
				}
			}

			State = CHANGE_COLOR;
		}
			break;
		
		case PICK_SHAPE:
		{
			if (circleshape->inHitbox(sf::Vector2f(mouse_pos))) {
				auto new_circle = std::make_shared<Circle>(sf::Vector2f(mouse_pos), last_picked_color, 20);
				movable_objects.push_back(new_circle);
				while ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))) {
					mouse_pos = sf::Mouse::getPosition(window);
					new_circle->update(sf::Vector2f(mouse_pos));
					drawAllObjects();
				}
			}
			if (rectshape->inHitbox(sf::Vector2f(mouse_pos))) {
				auto new_rect = std::make_shared<Rectangle>(sf::Vector2f(mouse_pos), last_picked_color, sf::Vector2f(40, 40));
				movable_objects.push_back(new_rect);
				while ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))) {
					mouse_pos = sf::Mouse::getPosition(window);
					new_rect->update(sf::Vector2f(mouse_pos));
					drawAllObjects();
				}
			}
			State = SELECT;
		}
			break;

		case CHANGE_COLOR:
		{
			for (auto& object : movable_objects) {
				if (object->inHitbox(sf::Vector2f(mouse_pos)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					object->changeColor(last_picked_color);
				}
			}
		}
			break;
		}

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
	}

	write_to_file("objects.txt", movable_objects);
	std::cout << "Terminating application\n";
	return 0;
}
