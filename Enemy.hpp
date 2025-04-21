#pragma once
#include <raylib.h>
#include <fstream>
using namespace std;

// Forward declarations 
class Maze;

// Enemy class representing opponents that chase the player
class Enemy {
private:
    int x, y;     //position coordinate
    Texture2D texture;         // Enemy sprite texture
    bool textureLoaded = false; // Flag to check if texture is ok 

public:
    Enemy(int startX, int startY);    // Constructor that initializes enemy at a specific position

    void loadTexture();    // Loads the enemy sprite 


    // Uses basic AI to chase player with occasional random movement
    void move(int playerX, int playerY, const Maze& maze, int level);
    // Renders the enemy on screen
    void draw() const;
    void save(ofstream& file) const;    // Save/load enemy state to/from a file
    void load(ifstream& file);
    // Getters for position
    int getX() const { return x; }
    int getY() const { return y; }

    void reset(int newX, int newY) { x = newX; y = newY; }    // Reset enemy position
};