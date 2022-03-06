#include <SFML/Graphics.hpp>

#include "Translator.h"
#include "Textbox.h"
#include "Button.h"


int main()
{
	sf::RenderWindow window;

	sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 445, (sf::VideoMode::getDesktopMode().height / 2) - 480);

	window.create(sf::VideoMode(900, 900), "Translator C++ > Pseudocod", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(centerWindow);

	window.setKeyRepeatEnabled(true);

	string clipbrd[40] = {""};

	sf::Font arial;
	arial.loadFromFile("arial.ttf");

	Textbox inputCode(15,sf::Color::White, false);
	inputCode.setFont(arial);
	inputCode.setPosition({ 350,200 });

	Button translateButton("Translate", { 200,50 }, 20, sf::Color::Yellow, sf::Color::Black);
	translateButton.setPosition({ 350, 50 });
	translateButton.setFont(arial);
	
	sf::RectangleShape border;
	border.setSize({ 600,800 });
	border.setPosition({ 150,175 });
	border.setOutlineColor(sf::Color::White);
	border.setFillColor(sf::Color::Black);
	border.setOutlineThickness(5);
	
	while (window.isOpen())
	{
		sf::Event event;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (sf::Mouse::getPosition(window).x > 100 && sf::Mouse::getPosition(window).y > 100) {
				inputCode.setSelected(true);
			}
			else {
				inputCode.setSelected(false);
			}
		}

		
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
			case sf::Event::KeyPressed:
				if (event.key.control && event.key.code == sf::Keyboard::V) {
					clipbrd[0] = sf::Clipboard::getString();
					inputCode.pastedOn(clipbrd[0]);
				}
				break;
			case sf::Event::TextEntered:
				if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::V))) {
					inputCode.typedOn(event);
				}
				break;
			case sf::Event::MouseMoved:
				if (translateButton.isMouseOver(window)) {
					translateButton.setBackColor(sf::Color::White);
				}
				else {
					translateButton.setBackColor(sf::Color::Yellow);
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (translateButton.isMouseOver(window)) {
					vector<string> output_array = translareCod(inputCode.getText());
					inputCode.translatePressedOn(output_array);
				}
			}
		}
		window.clear();
		window.draw(border);
		inputCode.drawTo(window);
		translateButton.drawTo(window);
		window.display();
	}
	return 0;
}