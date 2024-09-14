//
// Created by Blas Antunez on 8/9/24.
//

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Random.h"
#include "Board.h"
#include "Tile.h"

void Board::initialize(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    vector<string> lines;

    // Read the file line by line to determine rows and columns
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    rows = lines.size();
    columns = (rows > 0) ? lines[0].length() : 0;
    mineCount = 0;

    // Resize the tiles vector
    tiles.resize(rows, vector<Tile>(columns));

    // Set mines based on the 1's in the .brd file
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if (lines[i][j] == '1')
            {
                tiles[i][j].setMine();
                mineCount++;
            }
        }
    }

    file.close();

    calcAdjacentMines();
}

void Board::configInitialize(const string& configFilename)
{
    ifstream file(configFilename);
    if (!file.is_open())
    {
        cout << "Error opening file: " << configFilename << endl;
        return;
    }

    string line;

    // Read line 1 for columns
    if (getline(file, line))
    {
        columns = std::stoi(line);
    }
    else
    {
        cout << "Failed to read columns line." << endl;
    }

    // Read line 2 for rows
    if (getline(file, line))
    {
        rows = stoi(line);
    }
    else
    {
        cout << "Failed to read rows line." << endl;
    }

    // Read line 3 for mine count
    if (getline(file, line))
    {
        mineCount = stoi(line);
    }
    else
    {
        cout << "Failed to read mines line." << endl;
    }

    // Resize the tiles vector
    tiles.resize(rows, vector<Tile>(columns));

    file.close();

    // Initialize the board with default mines (or as needed)
    placeMine();
    calcAdjacentMines();
}

void Board::placeMine()
{
    int placedMines = 0;
    while (placedMines < mineCount)
    {
        int row = Random::Int(0, rows - 1);
        int col = Random::Int(0, columns - 1);
        if (!tiles[row][col].isMine)
        {
            tiles[row][col].setMine();
            placedMines++;
        }
    }
}

void Board::calcAdjacentMines()
{
    // Just so you know the directions of the adjacent mine tiles
    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, // Top-left, Top, Top-right
        {0, -1},/*[CLICKED]*/{0, 1},   // Left, Right
        {1, -1}, {1, 0}, {1, 1}    // Bottom-left, Bottom, Bottom-right
    };

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (!tiles[i][j].isMine)
            {
                int count = 0;
                // Check all 8 possible directions
                for (const auto& dir : directions)
                {
                    int x = i + dir[0];
                    int y = j + dir[1];

                    // Check bounds and if the adjacent tile is a mine
                    if (x >= 0 && x < rows && y >= 0 && y < columns && tiles[x][y].isMine)
                    {
                        count++;
                    }
                }
                tiles[i][j].setAdjacentMines(count);
            }
        }
    }
}

void Board::revealTile(int row, int column)
{
    if (gameOver or row < 0 or row >= rows or column < 0 or column >= columns or tiles[row][column].isRevealed or tiles[row][column].isFlagged)
    {
        return;
    }

    if (tiles[row][column].isMine)
    {
        gameOverState();
        return;
    }
    // Check all N,S,E,W over and over until adjacent mine is found
    tiles[row][column].reveal();
    if (tiles[row][column].adjacentMines == 0)
    {
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                revealTile(row + y, column + x);
            }
        }
    }
}

void Board::flagTile(int row, int column)
{
    if (row >= 0 && row < rows && column >= 0 && column < columns)
    {
        Tile& tile = tiles[row][column];
        if (!tile.isRevealed) // Only flag or unflag tiles that are not revealed
        {
            if (tile.isFlagged)
            {
                // Unflag the tile
                tile.toggleFlag();
                flagCount--;
                cout << "Flags: " << flagCount << endl;
                cout << "Total Mines: " << mineCount << endl;
                cout << "Digits To Display: " << mineCount - flagCount << endl;
            }
            else
            {
                // Flag the tile
                tile.toggleFlag();
                flagCount++;
                cout << "Flags: " << flagCount << endl;
                cout << "Total Mines: " << mineCount << endl;
                cout << "Digits To Display: " << mineCount - flagCount << endl;
            }
        }
    }
}

bool Board::flagAllMines()
{
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < columns; ++j)
            {
                if (tiles[i][j].isMine)
                {
                    tiles[i][j].isFlagged = true;
                }
            }
        }
    }
    return true;
}

bool Board::checkWinCondition()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (!tiles[i][j].isMine and !tiles[i][j].isRevealed)
            {
                return false;
            }
        }
    }
    gameWin = true;
    flagAllMines();
    return true;
}

void Board::reset()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            tiles[i][j].reset();
        }
    }
    placeMine();
    calcAdjacentMines();
}

void Board::clear()
{
    {
        // Iterate through all tiles and clear mines
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                tiles[i][j].isMine = false; // Clear all of the mines - working
            }
        }
        mineCount = 0; // Reset the mine count
    }
}

void Board::draw(sf::RenderWindow& window) const
{
    int tileSize = 32;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            sf::Sprite mine;
            sf::Sprite revealedTile;
            sf::Sprite flag;
            sf::Sprite hiddenTile;
            sf::Sprite num1;
            sf::Sprite num2;
            sf::Sprite num3;
            sf::Sprite num4;
            sf::Sprite num5;
            sf::Sprite num6;
            sf::Sprite num7;
            sf::Sprite num8;
            if (debugMode)
            {
                if (tiles[i][j].isMine)
                {
                    mine.setTexture(TextureManager::GetTexture("mine"));
                }
            }
            if (tiles[i][j].isRevealed)
            {
                if (tiles[i][j].isMine)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    mine.setTexture(TextureManager::GetTexture("mine"));
                }
                else
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                }
                if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 1)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num1.setTexture(TextureManager::GetTexture("number_1"));
                }
                if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 2)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num2.setTexture(TextureManager::GetTexture("number_2"));
                }
                else if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 3)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num3.setTexture(TextureManager::GetTexture("number_3"));
                }
                else if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 4)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num4.setTexture(TextureManager::GetTexture("number_4"));
                }
                else if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 5)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num5.setTexture(TextureManager::GetTexture("number_5"));
                }
                else if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 6)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num6.setTexture(TextureManager::GetTexture("number_6"));
                }
                else if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 7)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num7.setTexture(TextureManager::GetTexture("number_7"));
                }
                else if (!tiles[i][j].isMine and tiles[i][j].adjacentMines == 8)
                {
                    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
                    num8.setTexture(TextureManager::GetTexture("number_8"));
                }
            }
            else if (tiles[i][j].isFlagged)
            {
                hiddenTile.setTexture(TextureManager::GetTexture("tile_hidden"));
                flag.setTexture(TextureManager::GetTexture("flag"));
            }
            else
            {
                hiddenTile.setTexture(TextureManager::GetTexture("tile_hidden"));
            }

            hiddenTile.setPosition(j * tileSize, i * tileSize);
            window.draw(hiddenTile);
            revealedTile.setPosition(j * tileSize, i * tileSize);
            window.draw(revealedTile);
            mine.setPosition(j * tileSize, i * tileSize);
            window.draw(mine);
            flag.setPosition(j * tileSize, i * tileSize);
            window.draw(flag);
            num1.setPosition(j * tileSize, i * tileSize);
            window.draw(num1);
            num2.setPosition(j * tileSize, i * tileSize);
            window.draw(num2);
            num3.setPosition(j * tileSize, i * tileSize);
            window.draw(num3);
            num4.setPosition(j * tileSize, i * tileSize);
            window.draw(num4);
            num5.setPosition(j * tileSize, i * tileSize);
            window.draw(num5);
            num6.setPosition(j * tileSize, i * tileSize);
            window.draw(num6);
            num7.setPosition(j * tileSize, i * tileSize);
            window.draw(num7);
            num8.setPosition(j * tileSize, i * tileSize);
            window.draw(num8);
        }
    }
}

void Board::debug()
{
    debugMode = !debugMode;
}

void Board::gameOverState()
{
    // change gameover to false and reveal all of the mines
    gameOver = true;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (tiles[i][j].isMine)
                tiles[i][j].reveal();
        }
    }
}

bool Board::getGameOver()
{
    return gameOver;
}

bool Board::getGameWin()
{
    return gameWin;
}

int Board::getRows()
{
    return rows;
}

int Board::getColumns()
{
    return columns;
}

int Board::getMineCount()
{
    return mineCount;
}

int Board::getFlagCount()
{
    return flagCount;
}

bool Board::setGameOver(bool value)
{
    gameOver = value;
    return gameOver;
}

bool Board::setGameWin(bool value)
{
    gameWin = value;
    return gameWin;
}

int Board::setFlagCount()
{
    flagCount = 0;
    return flagCount;
}

