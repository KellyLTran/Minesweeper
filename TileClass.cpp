
// <SFML/Graphics.hpp> allows drawing functions from the SFML library
#include <SFML/Graphics.hpp>
#include "TileClass.h"
using namespace std;

// Constructor of the tile class to initialize all necessary and relevant variables
TileClass::TileClass(float x, float y, sf::Texture& hiddenTexture, sf::Texture& revealedTexture, sf::Texture& flagTexture) {
    _hidden = hiddenTexture;
    _revealed = revealedTexture;
    _flag = flagTexture;
    _mine = false;
    _flagged = false;
    _revealedTile = false;
    _neighborMines = 0;
    _sprite.setPosition(x, y);
    _sprite.setTexture(hiddenTexture);
}

// Function to place a mine on the selected tile, which makes the _mine variable true
void TileClass::SetMine() {
    _mine = true;
}

// Checks if the tile has a mine, which returns true if yes and false if no by returning the _mine variable
bool TileClass::HasMine() const {
    return _mine;
}

// Sets the number of mines surrounding this tile by storing the number of neighboring mines
void TileClass::SetNeighborMines(int count) {
    _neighborMines = count;
}

// Get the number of mines surrounding this tile by returning _neighborMines
int TileClass::GetNeighborMines() const {
    return _neighborMines;
}

// Reveals this tile and checks if it contains a mine, which returns true if it has a mine, false if not
bool TileClass::Reveal() {
    _revealedTile = true;
    return _mine;
}

// Checks if this tile is revealed by returning _revealedTile
bool TileClass::IsRevealed() const {
    return _revealedTile;
}

// Changes the flag status of this tile so that if it is true, make it false and vice versa
void TileClass::ToggleFlag() {
    _flagged = !_flagged;
}

// Checks if this tile is flagged by returning _flagged
bool TileClass::IsFlagged() const {
    return _flagged;
}

// Draws the tile on the screen based on its current state
// Passes these parameters to display the mine and number of mines nearby on the window as well as having a debug mode option for testing
void TileClass::Draw(sf::RenderWindow& window, const sf::Texture& mineTexture,
    const vector<sf::Texture>& numberTextures, bool debugMode) {

    // Sets revealed tile texture if the tile isrevealed
    if (_revealedTile) {
        _sprite.setTexture(_revealed);
        window.draw(_sprite);

        // Sets mine texture if the tile has a mine
        if (_mine) {
            _sprite.setTexture(mineTexture);
            window.draw(_sprite);
        }

        // Sets the texture that corresponds to the number of neighboring mines if the tile has neighboring mines
        else if (_neighborMines > 0) {
            _sprite.setTexture(numberTextures[_neighborMines - 1]);
            window.draw(_sprite);
        }
    }

    // Sets hidden tile texture if the tile is not revealed
    else {
        _sprite.setTexture(_hidden);
        window.draw(_sprite);

        // Sets flag texture if the tile is flagged
        if (_flagged) {
            _sprite.setTexture(_flag);
            window.draw(_sprite);
        }
    }

    if (debugMode && _mine && !_revealedTile) {
        _sprite.setTexture(mineTexture);
        window.draw(_sprite);
    }
}

// Checks if a point (x, y) is inside this tile by using the getGlobalBounds and contains function on _sprite 
bool TileClass::ContainsPoint(float x, float y) const {
    return _sprite.getGlobalBounds().contains(x, y);
}

// Draws the tile as revealed on the screen
void TileClass::DrawAllRevealed(sf::RenderWindow& window, const sf::Texture& mineTexture,
    const vector<sf::Texture>& numberTextures, bool debugMode) {
    _sprite.setTexture(_revealed);
    window.draw(_sprite);

}
