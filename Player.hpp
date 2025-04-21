#pragma once
#include <raylib.h>
#include <fstream>
#include "Constants.hpp"

using namespace std;

class Player {
private:
    int x, y, score, moves, lives;
    Texture2D texture;
    bool textureLoaded = false;

public:
    Player();
    void loadTexture();
    void move(int newX, int newY);
    void draw() const;
    void save(ofstream& file) const;
    void load(ifstream& file);
    int getX() const { return x; }
    int getY() const { return y; }
    int getScore() const { return score; }
    void setScore(int newScore) { score = newScore; }
    int getMoves() const { return moves; }
    int getLives() const { return lives; }
    void resetMoves();
    void incrementMoves();

    void loseLife() { lives--; }
    void reset(int newX, int newY) { x = newX; y = newY; }
    void resetStats() { score = moves = 0; lives = 3; }
};
