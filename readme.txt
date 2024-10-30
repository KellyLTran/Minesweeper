# 💣 Minesweeper Clone

### 🔍 Project Overview
**Minesweeper Clone** is a desktop game developed in C++ using the SFML library. It incorporates a customizable game board where users can select board dimensions and set the bomb count, creating varying levels of difficulty. The game’s objective is to reveal all tiles without detonating any hidden bombs.

### ⚙️ Functionality
- **Customizable Game Board**: Users can set board dimensions and bomb count, creating multiple difficulty levels.
- **Tile Reveal Mechanism**: Clicking on a tile without a bomb triggers an automatic reveal of adjacent tiles, displaying the number of adjacent mines.
- **Game Objective**: Players aim to uncover all grid cells without triggering any mines.
- **Progress Tracking**: Includes an active timer and the ability to flag suspected bombs, allowing users to monitor their progress.

### 🧩 Program Structure
- **Model-View-Controller (MVC) Design**: Separates game logic, user interface, and data management for structured gameplay.
- **'Game' Class**: Manages key game operations, such as tracking wins/losses and updating game status.
- **'Board' Class**: Creates and manages the game grid layout.
- **'Tile' Class**: Represents each square, which can either contain a bomb or be empty.
