
#include "WelcomeClass.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "TileClass.h"
#include "BoardClass.h"
#include "WelcomeClass.h"
#include "LeaderboardClass.h"
using namespace std;


int main() {
    bool bContinue = false;
    int columns, rows, mines;

    // Opens the file with the board's configuration and reads the number of columns, rows, and mines from the file 
    ifstream configFile("files/board_config.cfg");
    if (configFile.is_open()) {
        configFile >> columns >> rows >> mines;
        configFile.close();
    }

    // Displays error message if file fails to open
    else {
        cout << "Failed to open config file" << endl;
        return false;
    }

    // Ensures dimensions are not less than 22 columns x 16 rows
    columns = max(columns, 22);
    rows = max(rows, 16);

    // Creates the game board with each tile being 32x32 pixels
    const int tileSize = 32;

    // Width and height of the game window
    const int windowWidth = columns * tileSize;
    const int windowHeight = rows * tileSize + 100;

    // Total tiles on the board
    const int tileCount = columns * rows;
    const int mineCount = min(mines, tileCount);

    // Displays the welcome screen by passing the window's width and height and calling the run function from the welcome class
    try {
        WelcomeClass welcome(windowWidth, windowHeight);
        string playerName = welcome.Run();

        // Ensures that a player name must be entered to play the game 
        if (playerName.size() <= 0) {
            return 0;
        }

        // Creates the main game window
        sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");

        // Makes the game run at 60 frames per second
        window.setFramerateLimit(60);

        // Initializes the game board with necessary parameters
        BoardClass board(columns, rows, tileSize, mineCount, window, playerName);

        
        while (window.isOpen()) {
            sf::Event event;

            // Checks for any player events or actions like clicks or key presses and ends the game if player closes the window
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Handles left click if the player clicks the left mouse buttton 
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        board.HandleLeftClick(event.mouseButton.x, event.mouseButton.y);
                    }

                    // Handles right click if the player clicks the right mouse buttton 
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        board.HandleRightClick(event.mouseButton.x, event.mouseButton.y);
                    }
                }

                // Renders a new window and creates an instance of the leaderboard when the enter key is pressed to keep track
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        LeaderboardClass leaderboard("files/leaderboard.txt")
                        sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");
                        window.setFramerateLimit(60);

                        while (window.isOpen()) {
                            sf::Event event;
                            while (window.pollEvent(event)) {
                                if (event.type == sf::Event::Closed) {
                                    window.close();
                                }
                            }
                        }
                    }
                }
            }

            // Clears the screen, draws the game board, and displays it
            window.clear();
            board.Draw(window);
            window.display();
        }
    }

    // Catches any errors calls C++ what function to return a string describing the error to the user
    catch (const runtime_error& e) {
        cout << e.what() << endl;
        return false;
    }

    // Ends the game
    return 0;
}
