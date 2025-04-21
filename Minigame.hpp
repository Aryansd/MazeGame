#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include "Constants.hpp"
using namespace std;

enum MiniGameType { NONE, TIC_TAC_TOE, ROCK_PAPER_SCISSORS };

class MiniGame {
private:
    MiniGameType type;
    int tttBoard[3][3];
    int cursorX, cursorY;
    bool playerTurn;
    int rpsPlayer, rpsEnemy, rpsTimer;
    string message;
    Texture2D textures[3]; // rock, paper, scissors
    bool texturesLoaded = false;

    int checkTicTacToeWinner();
    void aiTicTacToeMove();

public:
    MiniGame();
    void reset(MiniGameType newType);
    void loadTextures();
    void update(bool& gameWon, bool& gameLost);
    void draw() const;
    MiniGameType getType() const { return type; }
};
