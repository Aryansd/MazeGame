#include "Game.hpp"
//window
int main() {
    InitWindow(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, "Maze Adventure");
    SetTargetFPS(60);
    Game game;
    game.setup();
    while (!WindowShouldClose()) {
        game.update();
        game.draw();
    }
    CloseWindow();
    return 0;
}