#ifndef _BOARD_H
#define _BOARD_H

#include "Util.h"
#include <SDL.h>

struct _Board
{
	int Rows;					// = 5
	int Columns;				// = 9
	int Matrix[5][9];			//Stores the poisitons of various stones
	//_Point2D Positions[5][9];	//Stores the Top Left coord (x, y) of Stone image for [i][j] Stone 
	int Chance;
};

//Initilizes the Board
void InitializeBoard(_Board &Board);

//Stores the initial postions of the BLACK and WHITE stones in the matrix
void Populate(_Board &Board);						

//Gets the 2D matrix
void GetMatrix(_Board Board, int NewMatrix[5][9]);

//Prints the Matrix
void PrintMatrix(_Board Board);

//Shows the Board on the Screen
void Show(_Board Board);

//Returns the Colour of Stone at (x, y)
int GetStoneColour(_Board Board, int x, int y);

//Returns the true if Stone exists at (x, y)
bool StoneExists(_Board Board, int x, int y);

//Populates the SDL Surface with images of Stones
void PopulateGUI(_Board Board, SDL_Surface *Screen);

//Returns the row and column for given mouse poisiton
_Point2D GetSelectedPostion(_Board Board, _Point2D MousePos);

//Unselects all stones on the board
void UnselectALL(_Board &Board);

//Gets the Postion selected stone
_Point2D GetSelectedStone(_Board Board, int &Colour);

//return BLACK, White of Empty for winner
int CheckWinner(_Board Board);

//returns if the row or col is on the board or not
bool IsOn(_Board Board, int xx, int yy);

//Copies OldBoard to New Board
void Copy(_Board OldBoard, _Board &NewBoard);

#endif