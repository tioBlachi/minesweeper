//
// Created by Blas Antunez on 8/9/24.
//

#ifndef TILE_H
#define TILE_H
#include <iostream>
using namespace std;

class Tile {
    public:
	bool isMine = false;
	bool isRevealed = false;
	bool isFlagged = false;
	int adjacentMines = 0;

	void reveal();
	void toggleFlag();
	void setMine();
	void setAdjacentMines(int count);
	void reset();
};



#endif //TILE_H
