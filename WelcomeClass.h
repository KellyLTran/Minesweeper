
#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
using std::string;

class WelcomeClass {
public:
    WelcomeClass(int windowWidth, int windowHeight);
    string Run();
    void SetText(sf::Text& text, float x, float y);

private:
    sf::RenderWindow _window;
    sf::Font _font;
    sf::Text _welcomeText;
    sf::Text _inputPromptText;
    sf::RectangleShape _inputBox;
    sf::Text _text;
    sf::Text _cursor;
    string _inputText;
    void SetupText(sf::Text& text, const string& str, unsigned int size, const sf::Color& color, float x, float y);
    void HandleEvents();
    void HandleTextEntered(const sf::Event& event);
    void Draw();
};