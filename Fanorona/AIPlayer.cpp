#include <list>
#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "AIPlayer.h"
#include "Stone.h"
#include "Board.h"
#include "Constants.h"

void AIPlay(_Board &Board, SDL_Surface* Screen,int AIColour, int Mode)
{
	switch(Mode)
	{
	case GREEDY:
		GreedyPlay(Board, Screen, AIColour);
		break;
	case PRUNING:
		//TODO
		break;
	}
}

int GreedyPlay(_Board &Board, SDL_Surface* Screen, int AIColour)
{
	int MoveX;
	int MoveY;
	int Action;
	int Nb;
	_Stone SelectedStone = SelectStone(Board, AIColour, MoveX, MoveY, Action, Nb);
	int AImove = 0;

	if(SelectedStone.x >= 0 && SelectedStone.y >= 0)
	{
		while(CaptureStones(SelectedStone, Board, MoveX, MoveY, Action, true) > 0)
		{
			Nb = CanMakeCapturingMove(SelectedStone, Board, MoveX, MoveY, Action);
			if(Nb == 0)
				break;
			PopulateGUI(Board, Screen);
			SDL_UpdateRect(Screen, 0, 0,0, 0);
			SDL_Delay(300);
		}
	}
	UnselectALL(Board);
	PopulateGUI(Board, Screen);
	SDL_UpdateRect(Screen, 0, 0,0, 0);
	SDL_Delay(300);
	return 0;
}



list<_Stone> GetFreeStones(int Colour, _Board Board)
{
	int x, y;
	_Stone TempStone;
	list<_Stone> FreeStones;

	UnselectALL(Board);
	for(x = 0; x <Board.Columns; x++)
	{
		for(y = 0; y < Board.Rows; y++)
		{
			if(StoneExists(Board, x, y))
			{
				if(Board.Matrix[y][x] == Colour)
				{
					TempStone = NewStone(x, y, Colour);
					if(CanMove(TempStone, Board))
					{
						if(CanBeCaptured(TempStone, Board))
							FreeStones.push_back(TempStone);
						else
							FreeStones.push_front(TempStone);
					}
				}
			}
		}
	}
	return FreeStones;
}

_Stone SelectStone(_Board Board, int Colour, int &MoveX, int &MoveY, int &Action, int &Nb)
{
	list<_Stone> FreeStones;
	FreeStones = GetFreeStones(Colour, Board);
	list<_Stone>::iterator it;
	_Stone TempStone = NewStone(-1, -1, -1);
	int tempn;

	int n = 0;
	int X =-1, Y =-1, A =NOACTION;
	_Stone SelectedStone = TempStone;

	Nb = n;
	for(it = FreeStones.begin(); it != FreeStones.end(); it++)
	{
		TempStone = *it;
		tempn = CanMakeCapturingMove(TempStone, Board, X, Y, A);
		if(tempn > n)
		{
			n = tempn;
			SelectedStone = NewStone(TempStone.x, TempStone.y, TempStone.Colour);
			MoveX = X;
			MoveY = Y;
			Nb = n;
			Action = A;
		}
	}
	if(Nb == 0)
	{
		srand(time(NULL));
		TempStone = FreeStones.back();
		SelectedStone = NewStone(TempStone.x, TempStone.y, TempStone.Colour);
		CanMove(SelectedStone, Board, &MoveX, &MoveY);
		Action  = APPROACH;
	}

	return SelectedStone;
}