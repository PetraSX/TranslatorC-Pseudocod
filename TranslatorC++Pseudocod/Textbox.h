#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace std;

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Textbox
{
public:
	Textbox() { }

	Textbox (int size, sf::Color color, bool sel) {
		textbox.setCharacterSize(size);
		textbox.setFillColor(color);
		isSelected = sel;
		if (sel) {
			textbox.setString("_");
		}
		else {
			textbox.setString("");
		}
	}

	void setFont(sf::Font& font) {
		textbox.setFont(font);
	}

	void setPosition(sf::Vector2f position) {
		textbox.setPosition(position);
	}

	void setLimit(bool ToF) {
		hasLimit = ToF;
	}

	void setLimit(bool ToF, int lim) {
		hasLimit = ToF;
		limit = lim;
	}

	void setSelected(bool sel) {
		isSelected = sel;
		if (!sel) {
			string t = text.str();
			string newT = "";
			for (int i = 0; i < t.length(); i++) {
				newT += t[i];
			}
			textbox.setString(newT);
		}
	}

	string getText() {
		return text.str();
	}

	void drawTo(sf::RenderWindow& window) {
		window.draw(textbox);
	}

	void typedOn(sf::Event input) {
		if (isSelected) {
			int charTyped = input.text.unicode;
			if (charTyped < 128) {
				inputLogic(charTyped);
			}
		}
	}

	void pastedOn(string &pst) {
		if (isSelected) {
			pasteLogic(pst);
		}
	}

	void translatePressedOn(vector <string> output) {
			outputLogic(output);
	}

private:
	sf::Text textbox;
	std::ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit = 1000;

	void inputLogic(int charTyped) {
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
			text << static_cast<char>(charTyped);
		}
		else if (charTyped == DELETE_KEY) {
			if (text.str().length() > 0) {
				deleteLastChar();
			}
		}
		textbox.setString(text.str() + "_");
	}

	void pasteLogic(string &paste) {
		text << paste;
		textbox.setString(text.str() + "_");
	}

	void outputLogic(vector <string> output) {
		textbox.setString("_");
		string aux = "";
		for (auto line : output)
		{
			aux += (line + '\n');
		}
		textbox.setString(aux + "_");
	}

	void deleteLastChar() {
		string t = text.str();
		string newT = "";
		for (int i = 0; i < t.length() - 1; i++) {
			newT += t[i];
		}
		text.str("");
		text << newT;

		textbox.setString(text.str());
	}
};