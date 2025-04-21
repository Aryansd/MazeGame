#pragma once

#include "Maze.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "MiniGame.hpp"
#include "Constants.hpp"
#include <string>

using namespace std;

// Game states to track the current state of the game
enum GameState { PLAYING, PAUSED, WON, LOST };

/**
 * Game class - Main controller for the maze game
 * Handles game logic, updates, and rendering
 */
class Game {
private:
    // Core game objects
    Maze maze;
    Player player;
    Enemy enemy;
    MiniGame miniGame;

    // Game state and progression
    GameState state;
    int level;
    int frame;
    int moveDelay;

    // Messaging and UI
    string info;             // Instructional info displayed to player
    string statusMessage;    // Temporary messages like "Saved!" or "Loaded!"
    int statusFrame;         // Frame when the status message was set

    // Movement control
    float playerX;
    float playerY;
    float moveSpeed;

public:
    // Frame when a life was lost (used to show "Life Lost!" temporarily)
    int lifeLostFrame;

    // Constructor
    Game();

    // Game setup and core functions
    void setup();
    void update();
    void draw();

    // Player movement helpers
    void resetMoves();
    void incrementMoves();
};
