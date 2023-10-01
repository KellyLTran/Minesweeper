System: Windows
Compiler: Visual Studio
SFML version: Visual C++ 15 (2017) - 64-bit
IDE: Visual Studio

Functionality:
The customizable game board allows users to choose board dimensions and bomb count, which offer varying levels of difficulty
Clicking on a tile without a bomb triggers the automated reveal of adjacent tiles, and exposed cells display the number of adjacent mines
The user must uncover all the cells on a grid without triggering hidden mines
Users can track their progress with an active timer and flag tiles that they think contain bombs.

Program Structure:
The game employs the Model-View-Controller design, which involves separation of game logic, user interface, and data management.
The 'Game' class centralizes key game operations, such as checking for wins/losses and updating the game state.
'Board' class creates the game grid, while the 'Tile' class represents individual squares that can contain bombs or be empty.

Credits:
The UF CISE Department provided resources and objectives for this project.
