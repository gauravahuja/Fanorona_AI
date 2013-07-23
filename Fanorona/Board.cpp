#include "Board.h"
#include "Constants.h"
#include <SDL.h>
#include <cstdio>

//Initilizes the Board
void InitializeBoard(_Board &Board)
{
	Board.Rows = ROWS;
	Board.Columns = COLUMNS;
	for(int i = 0; i < Board.Rows; i++)
	{
		for(int j = 0; j < Board.Columns; j++)
		{
			Board.Matrix[i][j] = EMPTY;
		}
	}
}

//Stores the initial postions of the BLACK and WHITE stones in the matrix
void Populate(_Board &Board)
{
	int x = 0;
	int y = 0;

	while (y < Board.Rows)
	{
		x = 0;
		while (x < Board.Columns)
		{
			if(y < 2)
			{
				Board.Matrix[y][x] = BLACK;
			}
			else if(y > 2)
			{
				Board.Matrix[y][x] = WHITE;
			}
			else
			{
				if(x < 4)
				{
					if(x%2 == 0)
					{
						Board.Matrix[y][x] = WHITE;
					}
					else
					{
						Board.Matrix[y][x] = BLACK;
					}
				}
				else if(x > 4)
				{
					if(x%2 == 0)
					{
						Board.Matrix[y][x] = BLACK;
					}
					else
					{
						Board.Matrix[y][x] = WHITE;
					}
				}
				else
				{
					Board.Matrix[y][x] = EMPTY;
				}
			}
			x++;
		}
		y++;
	}
}


//Gets the 2D matrix
void GetMatrix(_Board Board, int NewMatrix[5][9])
{
	for(int i = 0; i < Board.Rows; i++)
	{
		for(int j = 0; j <Board.Columns; j++)
		{
			NewMatrix[i][j] = Board.Matrix[i][j];
		}
	}
}

//Prints the Matrix
void PrintMatrix(_Board Board)
{
	for(int i = 0; i < Board.Rows; i++)
	{
		for(int j = 0; j <Board.Columns; j++)
		{
			printf("%d\t", Board.Matrix[i][j]);
			/*switch(Board.Matrix[i][j])
			{
			case BLACK:
				printf("\tB");
				break;
			case WHITE:
				printf("\tW");
				break;
			case EMPTY:
				printf("\tE");
				break;
			}*/
		}
		printf("\n");
	}
	printf("\n");
}

//Shows the Board on the Screen
void Show(_Board Board)
{
	for(int i = 0; i < Board.Rows; i++)
	{
		if(i > 0)
		{
			for(int j = 0; j < Board.Columns; j++)
			{
				printf("|");
				if(j < Board.Columns - 1)
				{
					if((i+j)%2 == 0)
					{
						printf("/");
					}
					else
					{
						printf("\\");
					}
				}
			}
			printf("\n");
		}
		
		for(int j = 0; j < Board.Columns; j++)
		{
			switch(Board.Matrix[i][j])
			{
			case BLACK:
				printf("B");
				break;
			case WHITE:
				printf("W");
				break;
			case EMPTY:
				printf("E");
				break;
			}
			if(j < Board.Columns - 1)
				printf("-");
		}
		printf("\n");
	}
	printf("\n");

}

//Returns the Colour of Stone at (x, y)
int GetStoneColour(_Board Board, int x, int y)
{
	return Board.Matrix[y][x];
	/*if(StoneExists(Board, x, y))
	{
		return Board.Matrix[y][x];
	}
	return -1;*/
}

//Returns the true if Stone exists at (x, y)
bool StoneExists(_Board Board, int x, int y)
{
	if(IsOn(Board, x, y))
	{
		if(Board.Matrix[y][x] != EMPTY)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


//Populates the SDL Surface with images of Stones
void PopulateGUI(_Board Board, SDL_Surface *Screen)
{
	static SDL_Surface* iBlackStone = IMG_Load(BlackStone);
	static SDL_Surface* iSelectedBlackStone = IMG_Load(SelectedBlackStone);
	static SDL_Surface* iDeleteBlackStone = IMG_Load(DeleteBlackStone);

	static SDL_Surface* iWhiteStone = IMG_Load(WhiteStone);
	static SDL_Surface* iSelectedWhiteStone = IMG_Load(SelectedWhiteStone);
	static SDL_Surface* iDeleteWhiteStone = IMG_Load(DeleteWhiteStone);
	
	static SDL_Surface* iBackground = IMG_Load(Background);
	
	if(iBlackStone == NULL || iSelectedBlackStone == NULL || iDeleteBlackStone == NULL ||
		iWhiteStone == NULL || iSelectedWhiteStone == NULL || iDeleteWhiteStone == NULL ||
		iBackground == NULL
		)
	{
		printf("Failed to load images: %s\n", IMG_GetError());
	}

	SDL_Rect ScreenRect;
	ScreenRect.h = Height;
	ScreenRect.w = Width;
	ScreenRect.x = 0;
	ScreenRect.y = 0;

	SDL_BlitSurface(iBackground, NULL, Screen, &ScreenRect);

	SDL_Rect ImageRect;
	ImageRect.h = StoneSize;
	ImageRect.w = StoneSize;
	int Start = 50 - StoneSize/2;

	for(int i = 0; i < Board.Rows; i++)
	{
		ImageRect.y = Start + StoneSpace*i;
		for(int j = 0; j < Board.Columns; j++)
		{
			ImageRect.x = Start + StoneSpace*j;

			switch(Board.Matrix[i][j])
			{
			case BLACK:
				SDL_BlitSurface(iBlackStone, NULL, Screen, &ImageRect);
				break;
			case SELECTED_BLACK:
				SDL_BlitSurface(iSelectedBlackStone, NULL, Screen, &ImageRect);
				break;
			case DELETE_APPROACH_BLACK:
			case DELETE_WITHDRAW_BLACK:
				SDL_BlitSurface(iDeleteBlackStone, NULL, Screen, &ImageRect);
				break;
			case WHITE:
				SDL_BlitSurface(iWhiteStone, NULL, Screen, &ImageRect);
				break;
			case SELECTED_WHITE:
				SDL_BlitSurface(iSelectedWhiteStone, NULL, Screen, &ImageRect);
				break;
			case DELETE_APPROACH_WHITE:
			case DELETE_WITHDRAW_WHITE:
				SDL_BlitSurface(iDeleteWhiteStone, NULL, Screen, &ImageRect);
				break;
			}
		}
	}
}

//Returns the row and column and colour for given mouse poisiton
_Point2D GetSelectedPostion(_Board Board, _Point2D MousePos)
{
	_Point2D Point = NewPoint2D(-1, -1);
	
	int Start = 50 - StoneSize/2;
	float StoneArea = StoneSize/StoneSpace;
	
	int x = MousePos.x - Start;
	int y = MousePos.y - Start;

	if(x < 0 || y < 0)
		return Point;
	
	float row = (float)y/StoneSpace;
	float col = (float)x/StoneSpace;

	x = (int)col;
	y = (int)row;

	row  = row - (float)y;
	col = col - (float)x;

	//if(row < StoneArea && col < StoneArea && x < Board.Columns && y < Board.Rows)
	//{
		Point.x = x;
		Point.y = y;
	//}
	return Point;
}

//Unselects all stones on the board
void UnselectALL(_Board &Board)
{
	for(int i = 0; i < Board.Rows; i++)
	{
		for(int j = 0; j < Board.Columns; j++)
		{
			if(Board.Matrix[i][j] == SELECTED_BLACK)
				Board.Matrix[i][j] = BLACK;
			else if(Board.Matrix[i][j] == SELECTED_WHITE)
				Board.Matrix[i][j] = WHITE;
		}
	}
}
//Gets the Postion selected stone
_Point2D GetSelectedStone(_Board Board, int &Colour)
{
	_Point2D Pos;
	Pos.x = -1;
	Pos.y = -1;
	Colour = -1;

	for(int i = 0; i < Board.Rows; i++)
	{
		for(int j = 0; j < Board.Columns; j++)
		{
			if(Board.Matrix[i][j] == SELECTED_BLACK)
			{
				Pos.x = j;
				Pos.y = i;
				Colour = BLACK;
				return Pos;
			}
			else if(Board.Matrix[i][j] == SELECTED_WHITE)
			{
				Pos.x = j;
				Pos.y = i;
				Colour = WHITE;
				return Pos;
			}
		}
	}
	return Pos;

}


//return BLACK, White of Empty for winner
int CheckWinner(_Board Board)
{
	int x = 0;
	int y = 0;
	int c = EMPTY;

	while(x < Board.Columns)
	{
		while(y < Board.Rows)
		{
			if(Board.Matrix[y][x] != EMPTY && c == EMPTY)
			{
				c = Board.Matrix[y][x];
			}
			else if(Board.Matrix[y][x] != EMPTY && Board.Matrix[y][x] != c)
			{
				return EMPTY;
			}
			y++;
		}
		y = 0;
		x++;
	}
	return c;
}


//returns if the row or col is on the board or not
bool IsOn(_Board Board, int xx, int yy)
{
	if(xx >= 0 && xx < Board.Columns && yy >= 0 && yy < Board.Rows)
		return true;
	return false;
}

//Copies OldBoard to New Board
void Copy(_Board OldBoard, _Board &NewBoard)
{
	NewBoard = OldBoard;
}
