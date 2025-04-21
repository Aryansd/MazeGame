#include "Maze.hpp"
#include <cstdlib>

using namespace std;

Maze::Maze() : exitX(-1), exitY(-1) {
    // Initialize the grid with walls on borders and empty spaces inside
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            grid[y][x] = (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) ? '0' : ' ';
}

void Maze::generate(int level, int playerX, int playerY, int enemyX, int enemyY) {
    // Reset all cells to empty space
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            grid[y][x] = ' ';

    for (int i = 0; i < WIDTH; i++) {
        grid[0][i] = '1';         // Top border
        grid[HEIGHT - 1][i] = '1';  // Bottom border
    }
    for (int i = 0; i < HEIGHT; i++) {
        grid[i][0] = '1';         // Left border
        grid[i][WIDTH - 1] = '1';   // Right border
    }

    if (level == 1) {
        int mazeLayout[10][10] = {
            {1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,1,0,0,1},
            {1,0,1,1,1,0,1,0,1,1},
            {1,0,0,0,1,0,1,0,1,1},
            {1,0,1,0,1,1,0,0,1,1},
            {1,0,1,0,0,1,0,0,1,1},
            {1,0,1,1,0,1,1,0,1,1},
            {1,0,0,0,0,0,0,0,1,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1}
        };

        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                if (mazeLayout[y][x] == 1)
                    grid[y][x] = '1';

        setExit(8, 8);  // Set exit position
    }
    else if (level == 2) {
        int mazeLayout[10][10] = {
            {1,1,1,1,1,1,1,1,1,1},
            {1,0,1,0,1,0,0,0,0,1},
            {1,0,1,0,1,1,1,1,0,1},
            {1,0,0,0,0,0,1,0,0,1},
            {1,0,1,1,1,1,1,0,1,1},
            {1,0,0,0,0,0,0,0,1,1},
            {1,1,1,1,1,0,1,0,1,1},
            {1,0,0,0,0,0,1,1,0,1},
            {1,0,1,1,1,1,1,1,0,1},
            {1,1,1,1,1,1,1,1,1,1}
        };

        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                if (mazeLayout[y][x] == 1)
                    grid[y][x] = '1';

        setExit(8, 8);  // Set exit position
    }
    else {
        int mazeLayout[10][10] = {
            {1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,1,1,1,1,0,1},
            {1,0,1,0,0,0,0,1,0,1},
            {1,0,1,0,1,1,0,1,0,1},
            {1,0,0,0,1,0,0,1,0,1},
            {1,0,1,1,1,0,1,1,0,1},
            {1,0,0,0,0,0,1,0,0,1},
            {1,0,1,1,1,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1}
        };

        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                if (mazeLayout[y][x] == 1)
                    grid[y][x] = '1';

        setExit(-1, -1);  // No exit for level 3
    }

    coins.clear();

    for (int i = 0; i < MAX_COINS; i++) {
        int cx, cy;
        do {
            cx = rand() % WIDTH;
            cy = rand() % HEIGHT;
        } while (grid[cy][cx] != ' ' || (cx == playerX && cy == playerY) ||
            (cx == enemyX && cy == enemyY) || (cx == exitX && cy == exitY));
        coins.push_back(make_pair(cx, cy));
    }
}

void Maze::loadTextures() {
    textures[0] = LoadTexture("wall.png");
    textures[1] = LoadTexture("exit.png");
    textures[2] = LoadTexture("coin.png");
    texturesLoaded = true;
}

void Maze::draw() const {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (texturesLoaded)
                DrawTexture(textures[3], x * CELL_SIZE, y * CELL_SIZE, WHITE);
            else
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);

            if (isWall(x, y)) {
                if (texturesLoaded)
                    DrawTexture(textures[0], x * CELL_SIZE, y * CELL_SIZE, WHITE);
                else
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
            }
        }
    }

    for (size_t i = 0; i < coins.size(); i++) {
        const pair<int, int>& c = coins[i];
        if (texturesLoaded)
            DrawTexture(textures[2], c.first * CELL_SIZE, c.second * CELL_SIZE, WHITE);
        else
            DrawRectangle(c.first * CELL_SIZE, c.second * CELL_SIZE, CELL_SIZE, CELL_SIZE, YELLOW);
    }

    if (exitX >= 0) {
        if (texturesLoaded)
            DrawTexture(textures[1], exitX * CELL_SIZE, exitY * CELL_SIZE, WHITE);
        else
            DrawRectangle(exitX * CELL_SIZE, exitY * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
    }
}

bool Maze::collectCoin(int x, int y, int& score) {
    for (vector<pair<int, int>>::iterator it = coins.begin(); it != coins.end(); ++it) {
        if (it->first == x && it->second == y) {
            coins.erase(it);
            score += 10;
            return true;
        }
    }
    return false;
}
