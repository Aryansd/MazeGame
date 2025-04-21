#pragma once
#include <raylib.h>
#include <vector>
#include "Constants.hpp"
using namespace std;

class Maze {
private:
    char grid[HEIGHT][WIDTH];
    vector<pair<int, int>> coins;
    Texture2D textures[4]; // wall, exit, coin, tile
    bool texturesLoaded = false;
    int exitX, exitY;

public:
    Maze();
    void generate(int level, int playerX, int playerY, int enemyX, int enemyY);
    void loadTextures();
    void draw() const;
    bool isWall(int x, int y) const { return grid[y][x] == '0' || grid[y][x] == '1'; }
    bool collectCoin(int x, int y, int& score);
    bool atExit(int x, int y) const { return x == exitX && y == exitY; }
    void setExit(int x, int y) { exitX = x; exitY = y; }
};