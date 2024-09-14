//
// Created by Blas Antunez on 8/9/24.
//

#include "Tile.h"

void Tile::reveal()
{
    isRevealed = true;
}

void Tile::toggleFlag()
{
    isFlagged = !isFlagged;
}

void Tile::setMine()
{
    isMine = true;
}

void Tile::setAdjacentMines(int count)
{
    adjacentMines = count;
}

void Tile::reset()
{
    isMine = false;
    isRevealed = false;
    isFlagged = false;
    adjacentMines = 0;
}