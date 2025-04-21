#include "Enemy.hpp"
#include "Constants.hpp"
#include "Maze.hpp"
#include <cstdlib>
#include <cmath>

using namespace std;

Enemy::Enemy(int startX, int startY) : x(startX), y(startY) {}

void Enemy::loadTexture() {
    // Load enemy sprite from file
    texture = LoadTexture("enemy.png");
    textureLoaded = true;
}

void Enemy::move(int playerX, int playerY, const Maze& maze, int level) {
    int newX = x;
    int newY = y;

    if (rand() % 4 == 0) {
        int direction = rand() % 4;        // Choose a random direction (0-3)
        // Update position based on random direction
        if (direction == 0) newX++;      // Right
        else if (direction == 1) newX--; // Left
        else if (direction == 2) newY++; // Down
        else if (direction == 3) newY--; // Up
    }
    else {
        // Move along the axis with the larger distance to player
        if (abs(playerX - x) > abs(playerY - y)) {
            // Move horizontally toward player
            newX += (playerX > x) ? 1 : -1;
        }
        else {
            // Move vertically toward player
            newY += (playerY > y) ? 1 : -1;
        }
    }

    // Check if the new position is inside wall
    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT && !maze.isWall(newX, newY)) {
        x = newX;        // Update position if valid
        y = newY;
    }    // If invalid, the enemy does not move
}

void Enemy::draw() const {
    if (textureLoaded) {
        DrawTexture(texture, x * CELL_SIZE, y * CELL_SIZE, WHITE);
    }
    else {
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED); //if not rectangle enemy
    }
}

void Enemy::save(ofstream& file) const {
    file << x << " " << y << endl;
}

void Enemy::load(ifstream& file) {
    file >> x >> y;
}