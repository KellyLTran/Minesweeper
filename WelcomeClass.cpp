
#include "WelcomeClass.h"
using namespace std;

WelcomeClass::WelcomeClass(int windowWidth, int windowHeight) {

    // Creates the main window
    _window.create(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");
    if (!_font.loadFromFile("files/font.ttf")) {
        throw runtime_error("Failed to load font");
    }

    // Displays the bold, underlined welcome text with a size of 2 4, colored white, and positioned horizontally centered in the window, shifted up 150 units
    SetupText(_welcomeText, "WELCOME TO MINESWEEPER!", 24, sf::Color::White, _window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 150);
    _welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    SetupText(_inputPromptText, "Enter your name:", 20, sf::Color::White, _window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 75);
    _inputPromptText.setStyle(sf::Text::Bold);

    // Formats the input box
    _inputBox.setSize(sf::Vector2f(400, 50));
    _inputBox.setFillColor(sf::Color::Blue);

    // Sets input box's origin to be horizontally centered in the window, shifted up 45 units
    _inputBox.setOrigin(_inputBox.getSize().x / 2.0f, _inputBox.getSize().y / 2.0f - 45);
    _inputBox.setPosition(_window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 45);

    // Formats the input text
    SetupText(_text, "", 18, sf::Color::Yellow, _inputBox.getPosition().x - _inputBox.getSize().x / 2.0f + 200, _inputBox.getPosition().y - 9);
    _text.setStyle(sf::Text::Bold);

    // Formats the text input and cursor
    SetupText(_cursor, "|", 18, sf::Color::Yellow, _inputBox.getPosition().x - _inputBox.getSize().x / 2.0f + 200, _inputBox.getPosition().y);
}

void WelcomeClass::SetupText(sf::Text& text, const string& str, unsigned int size, const sf::Color& color, float x, float y) {
    text.setFont(_font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    SetText(text, x, y);
}

void WelcomeClass::SetText(sf::Text& text, float x, float y) {
    // Uses FloatRect to define a rectangle with its top-left corner's coordinates (origin) and its size (width and height)
    sf::FloatRect textRect = text.getLocalBounds();

    // Sets the origin of the text to its center with x coord accounting for middle of the width and left corner and y coord accounting for middle of the height and top corner
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

// Handles the events of user's actions and drawing for the welcome screen, then returns the user's entered name
string WelcomeClass::Run() {
    while (_window.isOpen()) {
        HandleEvents();
        Draw();
    }
    return _inputText;
}

void WelcomeClass::HandleEvents() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
            exit(0);
        }

        // Updates name input if the user types
        else if (event.type == sf::Event::TextEntered) {
            HandleTextEntered(event);
        }
        // Closes the window to start the game if the user presses the Enter key
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                if (!_inputText.empty()) {
                    _window.close();
                }
            }
        }
    }
}

void WelcomeClass::HandleTextEntered(const sf::Event& event) {

    // event.text.unicode == 8 Checks if backspace key is pressed and if input is not empty, then the last character is removed
    if (event.text.unicode == 8 && !_inputText.empty()) { 
        _inputText.pop_back();
    }

    // Accepts only alphabet letters and up to 10 characters
    else if (isalpha(event.text.unicode) && _inputText.size() < 10) { 

        // Converts the Unicode value entered by the user to its corresponding character representation
        _inputText += static_cast<char>(event.text.unicode);
    }

    if (!_inputText.empty()) {

        // Converts the first letter entered by the user to uppercase using the toupper() function
        _inputText[0] = toupper(_inputText[0]);

        // Ensures the rest of the letters starting from second character to last converts to lowercase
        transform(_inputText.begin() + 1, _inputText.end(), _inputText.begin() + 1, ::tolower);
    }
    // Updates  _text to reflect the modified _inputText string
    _text.setString(_inputText);

    // Finds where the text starts and adds it to the width of the text, then adds 5 to place the cursor position after the entered text
    // Sets cursor's y position as the same as the input box
    if (!_inputText.empty()) {
        _cursor.setPosition(_text.getPosition().x + _text.getGlobalBounds().width + 5, _inputBox.getPosition().y);
    }
}

void WelcomeClass::Draw() {
    // Fills window to be blue 
    _window.clear(sf::Color::Blue);

    // Draws the welcome text, input prompt text, input box, text, and cursor
    _window.draw(_welcomeText);
    _window.draw(_inputPromptText);
    _window.draw(_inputBox);
    _window.draw(_text);
    _window.draw(_cursor);
    _window.display();
}