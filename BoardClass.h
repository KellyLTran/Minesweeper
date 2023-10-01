
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <ctime>
#include "TileClass.h"
using std::string;
using std::vector;

class BoardClass {
public:
    BoardClass(int width, int height, int tileSize, int mineCount, sf::RenderWindow& window, string playerName);
    void GenerateBoard();
    void HandleLeftClick(float x, float y);
    void HandleRightClick(float x, float y);
    void Draw(sf::RenderWindow& window);

private:
    string _playerName;
    int _width;
    int _height;
    int _tileSize;
    int _mineCount;
    int _flagsPlaced;
    sf::Font _font;
    sf::RenderWindow& _window;
    vector<TileClass> _tiles;
    sf::Texture _hiddenTexture;
    sf::Texture _revealedTexture;
    sf::Texture _flagTexture;
    sf::Texture _mineTexture;
    vector<sf::Texture> _numberTextures;
    void LoadTextures();
    sf::Sprite _happyFaceButton;
    sf::Texture _happyFaceTexture;
    sf::Texture _winFaceTexture;
    sf::Texture _loseFaceTexture;
    void SetupHappyFaceButton();
    void HandleHappyFaceClick();
    bool CheckWinCondition();
    sf::Texture _counterDigitsTexture;
    sf::Texture _counterNegativeTexture;
    sf::Vector2f _counterPosition;
    void DrawCounter(sf::RenderWindow& window);
    sf::Sprite _debugButtonSprite;
    sf::Vector2f _debugButtonPosition;
    sf::Texture _debugButtonTexture;
    bool _debugMode;
    sf::Clock _timerClock;
    sf::Text _timerText;
    sf::Texture _digitsTexture;
    vector<sf::Sprite> _digitsSprites;
    void DrawTimer(sf::RenderWindow& window);
    void DrawDigits(sf::RenderWindow& window, int value, int x, int y);
    string GetElapsedTimeAsString() const;
    void RestartBoard();
    bool _gamePaused;
    bool _gameEnded;
    sf::Time _pauseTime;
    sf::Time _totalElapsedTime;
    bool _updateTimer;
    sf::Texture _pauseTexture;
    sf::Texture _playTexture;
    sf::Sprite _pausePlayButton;
    void HandlePausePlayButtonClick(float x, float y);
    void RevealAdjacentTiles(int x, int y);
    sf::Texture _leaderboardTexture;
    sf::Sprite _leaderboardButton;
    bool _leaderboardOpen;
    sf::Time _leaderboardPauseTime;
    bool _leaderboardPause;
    void HandleLeaderboardClick();
};