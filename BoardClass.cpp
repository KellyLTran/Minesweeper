
#include "BoardClass.h"
#include "LeaderboardClass.h"
using namespace std;

// Constructor of the board class
// sf::RenderWindow& window Creates a reference to an instance of the SFML class sf::RenderWindow, which keeps changes in the same window
BoardClass::BoardClass(int width, int height, int tileSize, int mineCount, sf::RenderWindow& window, string playerName) : _window(window) {
    _playerName = playerName;
    _width = width;
    _height = height;
    _tileSize = tileSize;
    _mineCount = mineCount;
    _flagsPlaced = 0;
    LoadTextures();
    GenerateBoard();
    SetupHappyFaceButton();
    // debug mode used for testing; it is false/turned off when the game starts
    _debugMode = false;

    // Passes 2 floating-point numbers, x-coordinate and y-coordinate of the debug button's position to SFML's Vector2f class
    // Multiplies _width and _tileSize for total number of columns, converts it to float, then adjusts x and y coord positon values according to instructions
    _debugButtonPosition = sf::Vector2f((float)(_width * _tileSize) - 304, 32 * ((float)_height + 0.5f));

    // Uses member function .setPosition() of SFML's sprite class to draw sprites in the intended, updated position defined above 
    _debugButtonSprite.setPosition(_debugButtonPosition);

    // Uses member function .restart() of SFML's clock class to reset the timer and return elapsed times
    _timerClock.restart();

    // Uses loadFromFile function to load font file "files/font.ttf" into the object _font, but throws an error if it fails
    if (!_font.loadFromFile("files/font.ttf")) {
        throw runtime_error("Failed to load font");
    }

    // Otherwise, sets font of _timerText object to font loaded, then changes character size to 32 and color to red using SFML's functions
    _timerText.setFont(_font);
    _timerText.setCharacterSize(32);
    _timerText.setFillColor(sf::Color::Red);

    // Initializes boolean variables to have the game not be inititally paused or ended
    _gamePaused = false;
    _gameEnded = false;

    // Initializes these time variables to SFML's constant zero 
    _totalElapsedTime = sf::Time::Zero;
    _pauseTime = sf::Time::Zero;
    _updateTimer = true;
    LoadTextures();
    _leaderboardOpen = false;
    _leaderboardPauseTime = sf::Time::Zero;
    _leaderboardPause = false;

    // Uses loadFromFile function to load texture file "files/images/leaderboard.png" into the object leaderboardTexture, but throws an error if it fails
    if (!_leaderboardTexture.loadFromFile("files/images/leaderboard.png")) {
        throw runtime_error("Failed to load leaderboard button texture");
    }

    // Otherwise, sets texture of _leaderboardButton object to tecture loaded
    _leaderboardButton.setTexture(_leaderboardTexture);

    // Uses member function .setPosition() of SFML's sprite class to draw sprites in the intended, updated position defined based on instructions
    _leaderboardButton.setPosition((_width * _tileSize) - 176, 32 * ((float)_height + 0.5f));
}

void BoardClass::GenerateBoard() {
    _tiles.clear();

    // Iterates over the height of the board starting from the top row where y = 0 to the bottom where y < _height
    for (int y = 0; y < _height; y++) {

        // Iterates over the width of the board starting from the left column where x = 0 to the right column where x < _height
        for (int x = 0; x < _width; x++) {

            // Adds a new tile to the vector _tiles at the calculated position and passes arguments concerning tile textures
            _tiles.emplace_back(x * _tileSize, y * _tileSize, _hiddenTexture, _revealedTexture, _flagTexture);
        }
    }

    // Uses class mt19937 to generate random numbers and uses seed time(0) to generate different seeds or number sequences
    // Uses C++ class mt19937 to generate random numbers, which is initialized with a seed time(0) to have different number sequences
    mt19937 rng(time(0));

    // Creates a uniform distribution of integers ranging from 0 to the last index of the _tiles vector
    uniform_int_distribution<int> dist(0, _tiles.size() - 1);

    // Loops through random indices or positions until all of the number of mines specified are placed
    for (int i = 0; i < _mineCount; i++) {
        int index = dist(rng);

        // Avoids duplicate mines by moving to a new random position if the current position has a mine
        while (_tiles[index].HasMine()) {
            index = dist(rng);
        }

        // Checks that the selected position is in range before placing the mine
        if (index >= 0 && index < _tiles.size()) {
            _tiles[index].SetMine();
        }
    
    }

    // Loops through each column and row 
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int count = 0;

            // Loops through each neighboring tile to the selected one by adding and subtracting 1 from the position coordinates
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;

                    // Checks that the neighboring tile are in range, then increases the count by 1 if there is a mine
                    // Multiples ny by the width to get into the specific starting index of selected row and adds nx to move to the specific tile 
                    if (nx >= 0 && nx < _width && ny >= 0 && ny < _height) {
                        if (_tiles[ny * _width + nx].HasMine()) {
                            count++;
                        }
                    }
                }
            }

            // Places the count of neighboring mines on the selected tile 
            _tiles[y * _width + x].SetNeighborMines(count);
        }
    }
}

// Method to handle user's left click, with x and y parameters representing clicked position 
void BoardClass::HandleLeftClick(float x, float y) {

    // Function does not execute if the leaderboard is open
    if (_leaderboardOpen) {
        return;
    }

    // Handles the pause/play button if the game has not ended, the leader board is not open, and if the clicked x and y coords are within bounds of the pause/play button
    if (_gameEnded == false && _leaderboardOpen == false) {
        if (_pausePlayButton.getGlobalBounds().contains(x, y)) {
            HandlePausePlayButtonClick(x, y);
            return;
        }
    }

    // Checks if the clicked x and y coords are within bounds of the Happy Face Button
    if (_happyFaceButton.getGlobalBounds().contains(x, y)) {

        // Clears all tiles to reset the game
        _tiles.clear();

        // Calls HandleHappyFaceClick() function to handle when the happy face button is clicked
        HandleHappyFaceClick();

        // Makes sure that the game is reset but not ended, so there is no winning or losing
        _gameEnded = false;
        return;
    }

    // Opens the leaderboard if the clicked x and y coords are within bounds of the Leader Board Button
    if (_leaderboardButton.getGlobalBounds().contains(x, y)) {
        _leaderboardOpen = true;

        // Draws and displays the current game state on the window 
        Draw(_window);
        _window.display();

        // Initializes leaderboard with data from file leaderboard.txt
        LeaderboardClass leaderboard("files/leaderboard.txt");

        // Checks if leaderboard is displayed in _window 
        if (leaderboard.DisplayLeaderBoardOnly(_window, leaderboard)) {

            // Resets certain game condiitons based on if the game has ended or not
            if (_gameEnded == false) {
                _gamePaused = false;
                _leaderboardOpen = false;
                _leaderboardPause = false;
                _updateTimer = false;
            }
            else {
                _gamePaused = false;
                _leaderboardOpen = false;
            }

            // Draws and displays the updated game state on the window 
            Draw(_window);
            _window.display();
            return;
        }
    }

    // Function does not execute if the game has ended
    if (_gameEnded) {
        return;
    }

    // Checks if the clicked x and y coords are within bounds of the debug button
    if (_debugButtonSprite.getGlobalBounds().contains(x, y)) {

        // Toggles debug mode on to off or off to on
        _debugMode = !_debugMode;
        return;
    }
    for (int i = 0; i < _tiles.size(); i++) {

        // Creates a reference to the current tile to avoid copies
        TileClass& tile = _tiles[i];

        // Checks if the user clicked on the current tile and if the tile is not flagged
        if (tile.ContainsPoint(x, y) && !tile.IsFlagged()) {

            // mineClicked is true if the tile.Reveal() returns true and vice versa to identify when a tile has a mine or not
            bool mineClicked = tile.Reveal();

            // Turns happy face into the loser face after clicking the mine if mineClicked is true
            if (mineClicked) {
                _happyFaceButton.setTexture(_loseFaceTexture);

                // Loops through all tiles and reveals those with mines then ends the game
                for (vector<TileClass>::iterator iter = _tiles.begin(); iter != _tiles.end(); ++iter) {
                    TileClass& tile = *iter;
                    if (tile.HasMine()) {
                        tile.Reveal();
                    }
                }
                _gameEnded = true;
            }
            else {
                // Reveals neighboring tiles if they do not have mines in any of them
                if (tile.GetNeighborMines() == 0) {

                    // Uses modulus and division to get the correct x and y coordinates from the index
                    RevealAdjacentTiles(i % _width, i / _width);
                }
            }
            break;
        }
    }
    Draw(_window);
    _window.display();

    // Turns happy face into the winner face if winner conditions are met
    bool won = CheckWinCondition();
    if (won) {
        _happyFaceButton.setTexture(_winFaceTexture);

        // Iterates through each tile to toggle its flag if the current tile has a mine
        for (vector<TileClass>::iterator iter = _tiles.begin(); iter != _tiles.end(); ++iter) {
            TileClass& tile = *iter;
            if (tile.HasMine()) {
                tile.ToggleFlag();
            }
        }
        _gameEnded = true;
        Draw(_window);
        _window.display();

        // Displays leaderboard with updated winner conditions, which includes the user's name and elapsed game time
        _leaderboardOpen = true;
        LeaderboardClass leaderboard("files/leaderboard.txt");
        if (leaderboard.LeaderboardAfterWon(_window, leaderboard, _playerName, GetElapsedTimeAsString())) {
            if (_gameEnded == false) {
                _gamePaused = false;
                _leaderboardOpen = false;
                _leaderboardPause = false;
                _updateTimer = false;
            }
            else {
                _gamePaused = false;
                _leaderboardOpen = false;
            }
            Draw(_window);
            _window.display();
            return;
        }
    }
}

void BoardClass::HandleRightClick(float x, float y) {

    // Loops through each tile and checks if the clicked point lies within bounds of the current tile
    for (vector<TileClass>::iterator iter = _tiles.begin(); iter != _tiles.end(); ++iter) {
        TileClass& tile = *iter;
        if (tile.ContainsPoint(x, y)) {

            // previouslyFlagged is true if tile.IsFlagged() returns true and vice versa to identify when a tile has a flag or not
            bool previouslyFlagged = tile.IsFlagged();

            // Handles toggling the flag and keeps flag count the same if the current status is the same as its previous state
            tile.ToggleFlag();
            if (tile.IsFlagged() == previouslyFlagged) {
                _flagsPlaced += 0;
            }
            else {

                // Increments flag count by 1 if the tile is not revealed and is flagged
                if (tile.IsRevealed() == false) {
                    if (tile.IsFlagged()) {
                        _flagsPlaced += 1;
                    }

                    // Otherwise, the flag count decrements by 1 to represent no flag on the current status
                    else {
                        _flagsPlaced += -1;
                    }
                }
            }
            break;
        }
    }
}

void BoardClass::Draw(sf::RenderWindow& window) {

    // Updates totalElapsedTime when the game is running, not paused, and _updateTimer is true
    if (!_gamePaused && !_gameEnded && _updateTimer) {
        _totalElapsedTime = _timerClock.getElapsedTime() - _pauseTime;
    }

    // Draws all tiles as revealed if the leaderboard is open with no winning or the game is paused so that users cannot see the current game board and progress during these conditions 
    if ((_leaderboardOpen == true && CheckWinCondition() == false) || _gamePaused == true)
    {
        for (vector<TileClass>::iterator iter = _tiles.begin(); iter != _tiles.end(); ++iter) {
            TileClass& tile = *iter;
            tile.DrawAllRevealed(window, _mineTexture, _numberTextures, _debugMode);
        }
    }

    // Otherwise, draws tiles based on their current state 
    else {
        for (vector<TileClass>::iterator iter = _tiles.begin(); iter != _tiles.end(); ++iter) {
            TileClass& tile = *iter;
            tile.Draw(window, _mineTexture, _numberTextures, _debugMode);
        }
    }

    // Draws the counter, timer, happy face, debug, and pause/play button in the window
    window.draw(_happyFaceButton);
    DrawCounter(window);
    window.draw(_debugButtonSprite);
    window.draw(_pausePlayButton);
    DrawTimer(window);
}

void BoardClass::LoadTextures() {

    // Throws an error if it fails to load tile images concerning its revealed image, mine image, and flag image
    if (!_hiddenTexture.loadFromFile("files/images/tile_hidden.png") ||
        !_revealedTexture.loadFromFile("files/images/tile_revealed.png") ||
        !_flagTexture.loadFromFile("files/images/flag.png") ||
        !_mineTexture.loadFromFile("files/images/mine.png")) {
        throw runtime_error("Failed to load tile textures");
    }

    // Iterates through each file representing the numbers 1 to 8 for a count of mines near a selected tile 
    for (int i = 1; i <= 8; i++) {
        sf::Texture numberTexture;
        string filename = "files/images/number_" + to_string(i) + ".png";

        // Throws an error specifying which file failed to load
        if (!numberTexture.loadFromFile(filename)) {
            throw runtime_error("Failed to load number texture: " + filename);
        }

        // Otherwise, stores number textures in the _numberTextures vector
        _numberTextures.push_back(numberTexture);
    }
    
    if (!_happyFaceTexture.loadFromFile("files/images/face_happy.png") ||
        !_winFaceTexture.loadFromFile("files/images/face_win.png") ||
        !_loseFaceTexture.loadFromFile("files/images/face_lose.png")) {
        throw runtime_error("Failed to load face textures");
    }

    // Throws an error if the flag counter image fails to load
    if (!_counterDigitsTexture.loadFromFile("files/images/digits.png")) {
        throw runtime_error("Failed to load counter textures");
    }

    // Allows negative numbers if the player places more flags than mines available, which uses same texture as positive numbers
    _counterNegativeTexture = _counterDigitsTexture;
    _counterPosition = sf::Vector2f(33, 32 * ((float)_height + 0.5f) + 16);

    if (!_debugButtonTexture.loadFromFile("files/images/debug.png")) {
        throw runtime_error("Failed to load debug button texture");
    }

    // Assigns the texture to the debug button sprite
    _debugButtonSprite.setTexture(_debugButtonTexture);

    // Throws an error if the timer image fails to load
    if (!_digitsTexture.loadFromFile("files/images/digits.png")) {
        throw runtime_error("Failed to load digits texture");
    }
    // Assigns the digit texture to each sprite for each single digit 
    for (int i = 0; i < 10; i++) {
        sf::Sprite digitSprite(_digitsTexture);

        // Sets size of the Texture Rect to 21 x 32 pixels and each digit is offset by 21 (the width)
        digitSprite.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
        _digitsSprites.push_back(digitSprite);
    }
  
    if (!_pauseTexture.loadFromFile("files/images/pause.png") ||
        !_playTexture.loadFromFile("files/images/play.png")) {
        throw runtime_error("Failed to load pause/play button textures");
    }

    // Sets texture and position of pause/play button based on instructions: ((number of columns) * 32) - 240, 32 * ((number of rows)+0.5f)
    _pausePlayButton.setTexture(_pauseTexture);
    _pausePlayButton.setPosition((_width * _tileSize) - 240, 32 * (_height + 0.5f));

    if (!_leaderboardTexture.loadFromFile("files/images/leaderboard.png")) {
        throw runtime_error("Failed to load leaderboard button texture");
    }

    // // Sets texture and position of leaderboard button based on instructions: ((number of columns) * 32) - 176, 32 * ((number of rows) + 0.5f)
    _leaderboardButton.setTexture(_leaderboardTexture);
    _leaderboardButton.setPosition((_width * _tileSize) - 176, 32 * ((float)_height + 0.5f));
}

// // Sets texture and Defines and sets x and y coordiate position of happy face button based on instructions: (((number of columns) / 2.0) * 32) - 32, 32 *((number of rows)+0.5f)
void BoardClass::SetupHappyFaceButton() {
    float xPos = (_width / 2.0f) * _tileSize - _tileSize;
    float yPos = 32 * (_height + 0.5f);
    _happyFaceButton.setTexture(_happyFaceTexture);
    _happyFaceButton.setPosition(xPos, yPos);
}

// Restarts the board by resetting game board visuals, having flag count be 0, restarting the timer, setting the game to not be ended and so on
void BoardClass::RestartBoard() {
    GenerateBoard();
    _flagsPlaced = 0;
    _timerClock.restart();
    _gameEnded = false;
    _happyFaceButton.setTexture(_happyFaceTexture);
}

// Clears tiles and restarts the board when the happy face is clicked in order to reset the game
void BoardClass::HandleHappyFaceClick() {
    _tiles.clear();
    RestartBoard();
}

bool BoardClass::CheckWinCondition() {
    int correctFlags = 0;
    int revealedTiles = 0;

        // Iterates through each tile and increments correct flags count by one if the tile that is flagged also has a mine under it
        for (std::vector<TileClass>::iterator iter = _tiles.begin(); iter != _tiles.end(); ++iter) {
            TileClass& tile = *iter;
        if (tile.IsFlagged() && tile.HasMine()) {
            correctFlags++;
        }

        // Increments revealed tiles flag by one if the tile is revealed
        if (tile.IsRevealed()) {
            revealedTiles++;
        }
    }

    // Winning condition returns true if all the mines have correct flags or if all tiles without mines have been revealed
    return (correctFlags == _mineCount) || (revealedTiles == _tiles.size() - _mineCount);
}

void BoardClass::DrawCounter(sf::RenderWindow& window) {
    int remainingMines = 0;

    // Checks if game has ended, which means there are no more remaining mines
    if (_gameEnded)
    {
        remainingMines = 0;
    }
    else {

        // Otherwise, calculates remaining mines by subtracting the number of flags placed 
        remainingMines = _mineCount - _flagsPlaced;
    }

    // Checks if remaining mines count is negative, which means there are more flags placed than mines
    bool isNegative = remainingMines < 0;
    sf::Sprite digitSprite(_counterDigitsTexture);

    // Creates a sprite for the negative sign and sets the texture if remainingMines is negative 
    if (isNegative) {
        sf::Sprite negativeSprite(_counterNegativeTexture);

        // Sets the texture with x coordinate being 10 * 21 to get the texture for the negative sign, which is the 10th item on this texture
        negativeSprite.setTextureRect(sf::IntRect(10 * 21, 0, 21, 33));

        // Sets the position of the negative sign so that it is slightly to  the left of the counter number 
        negativeSprite.setPosition(_counterPosition.x - 1, _counterPosition.y);
        window.draw(negativeSprite);

        // Changes the value of the negative remainingMines to positive since the negative sign has been drawn
        remainingMines = -remainingMines;
    }


    string remainingMinesStr = to_string(remainingMines);
    for (size_t i = 0; i < remainingMinesStr.length(); i++) {
        int digit = remainingMinesStr[i] - '0';
        digitSprite.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));

        // Adjusts the x-coordinate to make room for the negative sign if the number is negative 
        if (isNegative) {
            digitSprite.setPosition(_counterPosition.x + 21 * (i + 1), _counterPosition.y);
        }
        else {
            digitSprite.setPosition(_counterPosition.x + 21 * i, _counterPosition.y);
        }
        window.draw(digitSprite);
     
    }

    // Displays the pause button is the leaderboard is open
    if (_leaderboardOpen) {
        _leaderboardButton.setTexture(_pauseTexture);
    }

    // Otherwise, displays the leaderboard button 
    else {
        _leaderboardButton.setTexture(_leaderboardTexture);
    }
    window.draw(_leaderboardButton);
}

void BoardClass::DrawTimer(sf::RenderWindow& window) {
    int elapsedTime = static_cast<int>(_totalElapsedTime.asSeconds());

    // Converts time to integer and shows the time as paused if the game is paused 
    if (_gamePaused) {
        elapsedTime = static_cast<int>(_pauseTime.asSeconds());
    }

    // Shows the total elapsed time minus the paused time when the game is continuing 
    else if (!_gameEnded) {
        elapsedTime = static_cast<int>(_timerClock.getElapsedTime().asSeconds() - _pauseTime.asSeconds());
    }

    // Shows the final time if the game has ended 
    else {
        elapsedTime = static_cast<int>(_pauseTime.asSeconds());
    }
    int minutes;
    int seconds;


    // Gets the elapsed time as seconds and converts to minutes and defines the remaining seconds if the game is not paused and has not ended
    if (!_gamePaused && !_gameEnded) {
        elapsedTime = static_cast<int>(_timerClock.getElapsedTime().asSeconds());
        minutes = elapsedTime / 60;
        seconds = elapsedTime % 60;

        // Draws the minutes from (((number of columns) * 32) - 97, 32 * ((number of rows) + 0.5f) + 16).
        int xPos = (_width * _tileSize) - 97;
        int yPos = 32 * (_height + 0.5f) + 16;
        DrawDigits(window, minutes, xPos, yPos);

        // Draws the seconds from  (((number of columns) * 32) - 54, 32 * ((number of rows) + 0.5f) + 16).
        xPos = (_width * _tileSize) - 54;
        DrawDigits(window, seconds, xPos, yPos);
    }

    // Updates the time even when game has ended or is paused 
    minutes = static_cast<int>(_totalElapsedTime.asSeconds()) / 60;
    seconds = static_cast<int>(_totalElapsedTime.asSeconds()) % 60;
    DrawDigits(window, minutes, (((_width) * 32) - 97), 32 * ((_height)+0.5f) + 16);
    DrawDigits(window, seconds, (((_width) * 32) - 54), 32 * ((_height)+0.5f) + 16);

    // Stops updating the timer and colors it green if the leaderboard is open
    if (_leaderboardOpen) {
        _updateTimer = false;
        _timerText.setFillColor(sf::Color::Green);
    }

    // Otherwise, updates the timer and colors it red
    else {
        _updateTimer = true;
        _timerText.setFillColor(sf::Color::Red);
    }

    // Displays the timer on the window
    window.draw(_timerText);
}

// Draws a two-digit number on the window
void BoardClass::DrawDigits(sf::RenderWindow& window, int value, int x, int y) {

    // Gets the tens place then the ones place of the number
    int tens = value / 10;
    int ones = value % 10;

    // Sets the tens place position and draws it if it is in a valid range
    if (tens >= 0 && tens < _digitsSprites.size()) {
        _digitsSprites[tens].setPosition(x, y);
        window.draw(_digitsSprites[tens]);
    }

    // Moves to the next digit position for the ones place to position and draw if in a valid range
    x += 21;
    if (ones >= 0 && ones < _digitsSprites.size()) {
        _digitsSprites[ones].setPosition(x, y);
        window.draw(_digitsSprites[ones]);
    }
}


void BoardClass::HandlePausePlayButtonClick(float x, float y) {

    // Closes the leaderboard if it is open and pauses the game
    if (_leaderboardOpen) {
        _leaderboardOpen = false;
        _leaderboardPause = true;
        _leaderboardPauseTime = _timerClock.getElapsedTime();
    }
    else {

        // Pauses the game if it is not paused and changes the button to the play texture
        _gamePaused = !_gamePaused;
        if (_gamePaused) {
            _pausePlayButton.setTexture(_playTexture);
            _totalElapsedTime = _timerClock.getElapsedTime() - _pauseTime;
        }

        // Otherwise, resumes the game if it is paused and changes the button to the pause texture
        else {
            _pausePlayButton.setTexture(_pauseTexture);
            _pauseTime += _timerClock.getElapsedTime() - _totalElapsedTime;
            _updateTimer = true;
        }
    }
}

void BoardClass::RevealAdjacentTiles(int x, int y) {

    // Check neighboring tiles of the selected tile
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = x + dx;
            int ny = y + dy;

            // Checks if the neighboring tiles are within valid range 
            if (nx >= 0 && nx < _width && ny >= 0 && ny < _height) {
                TileClass& tile = _tiles[ny * _width + nx];

                // Checks if the tile is not flagged and not revealed
                if (!tile.IsFlagged() && !tile.IsRevealed()) {
                    bool mineClicked = tile.Reveal();

                    // Reveals times if the selected tile and its neighbors do not have mines
                    if (!mineClicked) {
                        if (tile.GetNeighborMines() == 0) {
                            RevealAdjacentTiles(nx, ny);
                        }
                    }
                }
            }
        }
    }

    // Closes the leaderboard if it is open and pauses the game
    if (_leaderboardOpen) {
        _leaderboardOpen = false;
        _leaderboardPause = true;
        _leaderboardPauseTime = _timerClock.getElapsedTime();
    }
}

// Gets the total elapsed time in the correct format of MM:SS
string BoardClass::GetElapsedTimeAsString() const {
    int totalSeconds = static_cast<int>(_totalElapsedTime.asSeconds());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Converts minutes and seconds to strings, with leading zeros if necessary
    string minutesStr;
    if (minutes < 10) {
        minutesStr = "0" + to_string(minutes);
    }
    else {
        minutesStr = to_string(minutes);
    }
    string secondsStr;
    if (seconds < 10) {
        secondsStr = "0" + to_string(seconds);
    }
    else {
        secondsStr = to_string(seconds);
    }
    return minutesStr + ":" + secondsStr;
}