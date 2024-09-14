//
// Created by Blas Antunez on 8/9/24.
//

#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tile.h"

using std::vector;
using std::string;

class Board
{
    vector<vector<Tile>> tiles;
    int rows, columns, mineCount, flagCount, digitDisplay = mineCount - flagCount;
    bool debugMode = false;
    bool gameOver = false;
    bool gameWin = false;

public:
    void initialize(const string& filename);
    void configInitialize(const string& configFilename);
    void placeMine();
    void calcAdjacentMines();
    void revealTile(int row, int column);
    void flagTile(int row, int column);
    void reset();
    void clear();
    void draw(sf::RenderWindow& window) const;
    void debug();
    void gameOverState();
    bool flagAllMines();
    bool checkWinCondition();
    bool setGameOver(bool value);
    bool setGameWin(bool value);
    bool getGameOver();
    bool getGameWin();
    int getRows();
    int getColumns();
    int getMineCount();
    int getFlagCount();
    int setFlagCount();
};

#endif //BOARD_H
