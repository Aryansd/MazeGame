#include "Game.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor: Init game state, seed random
Game::Game() : enemy(8, 8), state(PLAYING), level(1), frame(0), moveDelay(0), lifeLostFrame(-1), statusMessage(""), statusFrame(0) {
    srand(time(nullptr));
    maze.setExit(8, 1);
}

// Setup game for current level
void Game::setup() {
    maze.loadTextures(); player.loadTexture(); enemy.loadTexture(); miniGame.loadTextures();
    maze.generate(level, player.getX(), player.getY(), enemy.getX(), enemy.getY());
    player.reset(1, 1); player.resetMoves();
    switch (level) {
    case 1: enemy.reset(8, 8); maze.setExit(8, 1); info = "Reach EXIT and avoid the enemy!"; break;
    case 2: enemy.reset(5, 5); maze.setExit(8, 1); info = "Defeat ENEMY!! Show your skills!"; break;
    default: enemy.reset(8, 8); maze.setExit(-1, -1); info = "Final showdown: BASED ON LUCK!"; break;
    }
}

// Update game logic each frame
void Game::update() {
    frame++;
    // Toggle pause
    if (IsKeyPressed(KEY_P)) state = (state == PLAYING) ? PAUSED : PLAYING;

    // Restart on game over
    if (state != PLAYING) {
        if (IsKeyPressed(KEY_R)) {
            level = 1; state = PLAYING; player.resetStats(); setup();
        }
        return;
    }

    // Save game
    if (IsKeyPressed(KEY_O)) {
        ofstream file("save.txt");
        if (file.is_open()) {
            file << level << endl; player.save(file); enemy.save(file);
            statusMessage = "Game Saved!"; statusFrame = frame;
        }
    }

    // Load game
    if (IsKeyPressed(KEY_L)) {
        ifstream file("save.txt");
        if (file.is_open()) {
            file >> level; player.load(file); enemy.load(file); setup();
            statusMessage = "Game Loaded!"; statusFrame = frame;
        }
    }

    // Mini-game logic
    if (miniGame.getType() != NONE) {
        bool won = false, lost = false;
        miniGame.update(won, lost);
        if (won) {
            if (level < 3) { level++; setup(); }
            else state = WON;
        }
        else if (lost) {
            player.loseLife();
            if (player.getLives() <= 0) state = LOST;
            else { player.reset(1, 1); setup(); lifeLostFrame = frame; }
        }
        return;
    }

    // Player movement
    moveDelay++;
    if (moveDelay >= 10) {
        int newX = player.getX(), newY = player.getY(); bool moved = false;
        if (IsKeyPressed(KEY_W)) { newY--; moved = true; }
        else if (IsKeyPressed(KEY_S)) { newY++; moved = true; }
        else if (IsKeyPressed(KEY_A)) { newX--; moved = true; }
        else if (IsKeyPressed(KEY_D)) { newX++; moved = true; }
        if (moved && !maze.isWall(newX, newY)) {
            player.move(newX, newY); player.incrementMoves();
            int score = player.getScore(); maze.collectCoin(newX, newY, score);
            player.setScore(score); moveDelay = 0;
        }
    }

    // Enemy movement
    if (frame % (60 - level * 10) == 0) enemy.move(player.getX(), player.getY(), maze, level);

    // Enemy collision
    if (player.getX() == enemy.getX() && player.getY() == enemy.getY()) {
        if (level == 1) {
            player.loseLife();
            if (player.getLives() <= 0) state = LOST;
            else { player.reset(1, 1); setup(); lifeLostFrame = frame; }
        }
        else miniGame.reset(level == 2 ? TIC_TAC_TOE : ROCK_PAPER_SCISSORS);
        return;
    }

    // Exit reached
    if (maze.atExit(player.getX(), player.getY()) && level < 3) { level++; setup(); }
}

void Game::draw() {
    BeginDrawing(); ClearBackground(BLACK);

    // Draw paused state
    if (state == PAUSED) {
        DrawRectangle(0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, Fade(BLACK, 0.5f));
        DrawText("PAUSED", WIDTH * CELL_SIZE / 2 - 60, HEIGHT * CELL_SIZE / 2 - 20, 32, WHITE);
        DrawText("Press P to Resume", WIDTH * CELL_SIZE / 2 - 90, HEIGHT * CELL_SIZE / 2 + 20, 20, GRAY);
        EndDrawing(); return;
    }

    // Draw game over or won
    if (state != PLAYING) {
        DrawRectangle(0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, Fade(BLACK, 0.6f));
        DrawRectangle(100, 150, 400, 300, Fade(DARKGRAY, 0.95f));
        DrawRectangleLines(100, 150, 400, 300, GOLD);
        string resultText = (state == WON) ? "YAAYYY!\nYou Won!" : "Game Over!\nBOOO LOSER!";
        DrawText(resultText.c_str(), 180, 200, 40, (state == WON) ? GREEN : RED);
        DrawText("Press R to Restart", 200, 320, 24, WHITE);
        DrawText(("Score: " + to_string(player.getScore())).c_str(), 200, 360, 24, YELLOW);
        EndDrawing(); return;
    }

    // Draw mini-game
    if (miniGame.getType() != NONE) { miniGame.draw(); EndDrawing(); return; }

    // Draw gameplay
    maze.draw(); enemy.draw(); player.draw();
    DrawText(("Score: " + to_string(player.getScore())).c_str(), 20, 20, 20, YELLOW);
    DrawText(("Moves: " + to_string(player.getMoves())).c_str(), 20, 50, 20, GREEN);
    DrawText(("Level: " + to_string(level)).c_str(), 20, 80, 20, DARKBLUE);
    DrawText(("Lives: " + to_string(player.getLives())).c_str(), 20, 110, 20, RED);
    DrawText("O: Save  L: Load  P: Pause", 20, 140, 16, WHITE);

    // Info message
    int infoWidth = MeasureText(info.c_str(), 20);
    DrawRectangle(WIDTH * CELL_SIZE - infoWidth - 30, 15, infoWidth + 20, 30, Fade(BLACK, 0.4f));
    DrawText(info.c_str(), WIDTH * CELL_SIZE - infoWidth - 20, 20, 20, GOLD);

    // Life lost feedback
    if (lifeLostFrame != -1 && frame - lifeLostFrame < 120)
        DrawText("Life Lost!", WIDTH * CELL_SIZE / 2 - 60, HEIGHT * CELL_SIZE / 2 - 20, 32, RED);

    // Status message
    if (!statusMessage.empty() && frame - statusFrame < 120) {
        int msgWidth = MeasureText(statusMessage.c_str(), 24);
        DrawText(statusMessage.c_str(), (WIDTH * CELL_SIZE - msgWidth) / 2, HEIGHT * CELL_SIZE - 40, 24, SKYBLUE);
    }

    EndDrawing();
}