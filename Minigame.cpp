#include "MiniGame.hpp"
#include <cstdlib>
using namespace std;

MiniGame::MiniGame() : type(NONE), cursorX(0), cursorY(0), playerTurn(true), rpsPlayer(0), rpsEnemy(0), rpsTimer(0) {
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            tttBoard[y][x] = 0;
}

void MiniGame::reset(MiniGameType newType) {
    type = newType;
    cursorX = cursorY = 0;
    playerTurn = true;
    rpsPlayer = rpsEnemy = rpsTimer = 0;

    // Clear the tic-tac-toe board
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            tttBoard[y][x] = 0;

    message = type == TIC_TAC_TOE ? "Tic-Tac-Toe: Your turn (X)" :
        "Press 1 = Rock, 2 = Paper, 3 = Scissors";
}

void MiniGame::loadTextures() {
    textures[0] = LoadTexture("rock.png");
    textures[1] = LoadTexture("paper.png");
    textures[2] = LoadTexture("scissors.png");
    texturesLoaded = true;
}

int MiniGame::checkTicTacToeWinner() {
    for (int i = 0; i < 3; i++) {
        if (tttBoard[i][0] && tttBoard[i][0] == tttBoard[i][1] && tttBoard[i][1] == tttBoard[i][2]) return tttBoard[i][0];
        if (tttBoard[0][i] && tttBoard[0][i] == tttBoard[1][i] && tttBoard[1][i] == tttBoard[2][i]) return tttBoard[0][i];
    }
    if (tttBoard[0][0] && tttBoard[0][0] == tttBoard[1][1] && tttBoard[1][1] == tttBoard[2][2]) return tttBoard[0][0];
    if (tttBoard[0][2] && tttBoard[0][2] == tttBoard[1][1] && tttBoard[1][1] == tttBoard[2][0]) return tttBoard[0][2];

    // Check if board is full
    bool full = true;
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            if (tttBoard[y][x] == 0) full = false;

    return full ? -1 : 0;
}

void MiniGame::aiTicTacToeMove() {
    // Try to win
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            if (tttBoard[y][x] == 0) {
                tttBoard[y][x] = 2;
                if (checkTicTacToeWinner() == 2) return;
                tttBoard[y][x] = 0;
            }

    // Block player win
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            if (tttBoard[y][x] == 0) {
                tttBoard[y][x] = 1;
                if (checkTicTacToeWinner() == 1) {
                    tttBoard[y][x] = 2;
                    return;
                }
                tttBoard[y][x] = 0;
            }

    // Pick center
    if (tttBoard[1][1] == 0) {
        tttBoard[1][1] = 2;
        return;
    }

    // Pick corner or random
    vector<pair<int, int>> corners = { {0,0}, {0,2}, {2,0}, {2,2} };
    for (auto& c : corners)
        if (tttBoard[c.first][c.second] == 0) {
            tttBoard[c.first][c.second] = 2;
            return;
        }

    vector<pair<int, int>> empty;
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            if (tttBoard[y][x] == 0)
                empty.push_back({ x, y });

    if (!empty.empty()) {
        int idx = rand() % empty.size();
        tttBoard[empty[idx].second][empty[idx].first] = 2;
    }
}

void MiniGame::update(bool& gameWon, bool& gameLost) {
    if (type == TIC_TAC_TOE) {
        if (playerTurn) {
            // Movement keys
            if (IsKeyPressed(KEY_DOWN)) cursorY = (cursorY + 1) % 3;
            if (IsKeyPressed(KEY_UP)) cursorY = (cursorY - 1 + 3) % 3;
            if (IsKeyPressed(KEY_RIGHT)) cursorX = (cursorX + 1) % 3;
            if (IsKeyPressed(KEY_LEFT)) cursorX = (cursorX - 1 + 3) % 3;

            if (IsKeyPressed(KEY_ENTER) && tttBoard[cursorY][cursorX] == 0) {
                tttBoard[cursorY][cursorX] = 1;
                int winner = checkTicTacToeWinner();
                if (winner) {
                    message = winner == 1 ? "You win!" : winner == -1 ? "Draw!" : "Enemy wins!";
                    gameWon = winner == 1;
                    gameLost = winner == 2 || winner == -1;
                    type = NONE;
                    return;
                }
                playerTurn = false;
            }
        }
        else {
            aiTicTacToeMove();
            int winner = checkTicTacToeWinner();
            if (winner) {
                message = winner == 1 ? "You win!" : winner == -1 ? "Draw!" : "Enemy wins!";
                gameWon = winner == 1;
                gameLost = winner == 2 || winner == -1;
                type = NONE;
                return;
            }
            playerTurn = true;
        }
    }
    else if (type == ROCK_PAPER_SCISSORS) {
        string moves[4] = { "", "Rock", "Paper", "Scissors" };

        if (rpsPlayer == 0) {
            if (IsKeyPressed(KEY_ONE)) rpsPlayer = 1;
            else if (IsKeyPressed(KEY_TWO)) rpsPlayer = 2;
            else if (IsKeyPressed(KEY_THREE)) rpsPlayer = 3;
            if (rpsPlayer) {
                message = "You picked " + moves[rpsPlayer] + ". Waiting...";
                rpsTimer = 30;
            }
        }
        else if (rpsTimer > 0) {
            rpsTimer--;
            if (rpsTimer == 0) {
                rpsEnemy = (rand() % 3) + 1;
                message = "You: " + moves[rpsPlayer] + " VS Enemy: " + moves[rpsEnemy] + ". ";

                bool playerWins = (rpsPlayer == 1 && rpsEnemy == 3) ||
                    (rpsPlayer == 2 && rpsEnemy == 1) ||
                    (rpsPlayer == 3 && rpsEnemy == 2);

                if (rpsPlayer == rpsEnemy) {
                    message += "It's a tie! Try again.";
                    rpsPlayer = rpsEnemy = 0;
                }
                else {
                    message += playerWins ? " You win!" : " You lose!";
                    gameWon = playerWins;
                    gameLost = !playerWins;
                    type = NONE;
                }
            }
        }
    }
}

void MiniGame::draw() const {
    DrawRectangle(60, 60, 520, 520, Fade(DARKGRAY, 0.95f));
    DrawRectangleLines(60, 60, 520, 520, GOLD);

    if (type == TIC_TAC_TOE) {
        DrawText("Tic-Tac-Toe Battle", 170, 80, 36, RED);
        DrawText(message.c_str(), 150, 130, 24, YELLOW);

        int startX = 170, startY = 200, size = 100;
        for (int i = 0; i <= 3; i++) {
            DrawLine(startX + i * size, startY, startX + i * size, startY + 3 * size, WHITE);
            DrawLine(startX, startY + i * size, startX + 3 * size, startY + i * size, WHITE);
        }

        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++) {
                int cellX = startX + x * size, cellY = startY + y * size;
                if (tttBoard[y][x] == 1) DrawText("X", cellX + 35, cellY + 20, 48, SKYBLUE);
                else if (tttBoard[y][x] == 2) DrawText("O", cellX + 35, cellY + 20, 48, ORANGE);
                if (playerTurn && x == cursorX && y == cursorY)
                    DrawRectangleLines(cellX + 2, cellY + 2, size - 4, size - 4, YELLOW);
            }

    }
    else {
        DrawText("Rock-Paper-Scissors", 160, 80, 36, RED);
        DrawText(message.c_str(), 100, 130, 24, YELLOW);

        string moves[4] = { "", "Rock", "Paper", "Scissors" };
        for (int i = 1; i <= 3; i++) {
            int yOffset = 200 + (i - 1) * 90;
            if (texturesLoaded)
                DrawTexture(textures[i - 1], 100, yOffset, WHITE);
            else
                DrawRectangle(100, yOffset, 80, 80, GRAY);
            DrawText((to_string(i) + ": " + moves[i]).c_str(), 200, yOffset + 25, 24, rpsPlayer == i ? YELLOW : SKYBLUE);
        }

        if (rpsPlayer) {
            DrawText("You:", 330, 200, 24, GREEN);
            if (texturesLoaded) DrawTexture(textures[rpsPlayer - 1], 330, 230, WHITE);
            else DrawRectangle(330, 230, 80, 80, BLACK);

            DrawText("VS", 370, 320, 32, WHITE); // Center "VS"

            if (rpsTimer == 0 && rpsEnemy) {
                DrawText("Enemy:", 330, 360, 24, ORANGE);
                if (texturesLoaded) DrawTexture(textures[rpsEnemy - 1], 330, 390, WHITE);
                else DrawRectangle(330, 390, 80, 80, ORANGE);
            }
        }
    }
}
