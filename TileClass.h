
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <ctime>

// Brings string and vector into the current scope
using std::string;
using std::vector;

class TileClass {
public:
    TileClass(float x, float y, sf::Texture& hiddenTexture, sf::Texture& revealedTexture, sf::Texture& flagTexture);
    void SetMine();
    bool HasMine() const;
    void SetNeighborMines(int count);
    int GetNeighborMines() const;
    bool Reveal();
    bool IsRevealed() const;
    void ToggleFlag();
    bool IsFlagged() const;
    void Draw(sf::RenderWindow& window, const sf::Texture& mineTexture, const vector<sf::Texture>& numberTextures, bool debugMode = false);
    bool ContainsPoint(float x, float y) const;
    void DrawAllRevealed(sf::RenderWindow& window, const sf::Texture& mineTexture,
        const vector<sf::Texture>& numberTextures, bool debugMode);

private:
    sf::Sprite _sprite;
    sf::Texture _hidden;
    sf::Texture _revealed;
    sf::Texture _flag;
    bool _mine;
    bool _flagged;
    bool _revealedTile;
    int _neighborMines;
};