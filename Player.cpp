#include "Player.hpp"
#include <fstream>
using namespace std;
// Constructor initializes the player's attributes to default values
Player::Player() : x(1), y(1), score(0), moves(0), lives(3) {}
// Loads the player texture from file
void Player::loadTexture() {
    texture = LoadTexture("player.png");  // Load the texture from a file
    textureLoaded = true;  // Mark texture as loaded
}
// Moves the player to a new position
void Player::move(int newX, int newY) {
    x = newX;  // Update the x coordinate
    y = newY;  // Update the y coordinate
}
// Resets the number of moves to 0
void Player::resetMoves() {
    moves = 0;  // Reset moves counter
}
// Increments the number of moves by 1
void Player::incrementMoves() {
    moves++;  // Increase moves by 1
}
// Draws the player character on the screen
void Player::draw() const {
    if (textureLoaded) {
        DrawTexture(texture, x * CELL_SIZE, y * CELL_SIZE, WHITE);  // Draw the player texture
    }
    else {
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);  // Draw a blue square if texture isn't loaded
    }
}
// Saves the player's state to a file
void Player::save(ofstream& file) const {
    file << score << endl  // Save  score
        << moves << endl  // Save  number of moves
        << lives << endl  // Save  number of lives
        << x << " " << y << endl;  // Save  player's position
}
// Loads the player's state from a file
void Player::load(ifstream& file) {
    file >> score >> moves >> lives >> x >> y;  // Load score, moves, lives, and position
}
