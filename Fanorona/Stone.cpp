#include <list>
#include <cstdlib>
using namespace std;

#include "Stone.h"
#include "Constants.h"
#include "Util.h"
#include "Board.h"

//Creates a New Stone
_Stone NewStone(int x, int y, int Colour)
{
	_Stone TempStone;

	TempStone.x = x;
	TempStone.y = y;
	TempStone.CapturedStones = 0;
	TempStone.LastCapturedStones = 0;
	TempStone.Colour = Colour;
	TempStone.LastPosition = NewPoint2D(-1, -1);

	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			TempStone.History[i][j] = 0;
		}
	}
	TempStone.History[y][x] = 1;
	return TempStone;
}

//Returns true if the move to NewX, NewY by the Stone is Legal else returns false
bool LegalMove(_Stone Stone, _Board Board, int NewX, int NewY)
{
	bool Legal = false;
	int DeltaX;
	int DeltaY;
	int DeltaOldX;
	int DeltaOldY;

	if(IsOn(Board, NewX, NewY))
	{
		if(StoneExists(Board, NewX, NewY) == false && Stone.History[NewY][NewX] == 0)
		{
			DeltaX = abs(NewX - Stone.x);
			DeltaY = abs(NewY - Stone.y);

			if((DeltaX == 0 || DeltaX == 1) && (DeltaY == 0 || DeltaY == 1))
			{
				if(((Stone.x)%2 != (Stone.y)%2) && DeltaX + DeltaY == 1)
				{
					//For any point you can move horizontally or vertically if empty
					Legal = true;
				}
				else if(((Stone.x)%2 == (Stone.y)%2) && DeltaX + DeltaY > 0)
				{
					//For a storng position one can move horizontally, vertically and diagonally
					Legal = true;
				}
				
				if(Legal && (Stone.LastPosition.x >= 0 && Stone.LastPosition.y >= 0))
				{
					DeltaOldX = Stone.x - Stone.LastPosition.x;
					DeltaOldY = Stone.y - Stone.LastPosition.y;

					if(Stone.LastCapturedStones == 0)
					{
						//Can't move if in last move no stones were captured
						Legal = false;
					}
					else if((Stone.y + DeltaOldY == NewY) && (Stone.x + DeltaOldX == NewX))
					{
						//Can't move in last moved direction
						
						Legal = false;
					}
					/*else if(CaptureStones(Stone, Board, NewX, NewY, APPROACH, false) == 0 
							|| CaptureStones(Stone, Board, NewX, NewY, WITHDRAW, false) == 0 
							&& Stone.CapturedStones == 0
							)
					{
						A non capturing move is not allowed if no stones have been captured till now
						Legal = false;
					}*/
				}
			}
		}
	}
	return Legal;
}

//Returns the maximum number of stones that can be captured by any move from current location
//If No Capturing move can be made 0 is returned
int CanMakeCapturingMove(_Stone Stone, _Board Board, int &X, int &Y, int &Action)
{
	_Point2D Neighbours[8] = {{Stone.x + 1 , Stone.y}, {Stone.x - 1, Stone.y}, {Stone.x, Stone.y + 1}, {Stone.x, Stone.y - 1}, {Stone.x + 1, Stone.y + 1}, {Stone.x + 1, Stone.y - 1}, {Stone.x - 1, Stone.y - 1}, {Stone.x - 1, Stone.y + 1}};
	int TempX = -1;
	int TempY = -1;
	int TempAction = NOACTION;
	int TempNumber = 0;
	int Number = 0;

	for(int i =0 ; i < 8; i++)
	{
		if(LegalMove(Stone, Board, Neighbours[i].x, Neighbours[i].y))
		{
			TempNumber = CaptureStones(Stone, Board, Neighbours[i].x, Neighbours[i].y, APPROACH, false);
			if(TempNumber > Number)
			{
				Number = TempNumber;
				TempAction = APPROACH;
				TempX = Neighbours[i].x;
				TempY = Neighbours[i].y;
			}
			
			TempNumber = CaptureStones(Stone, Board, Neighbours[i].x, Neighbours[i].y, WITHDRAW, false);
			if(TempNumber > Number)
			{
				Number = TempNumber;
				TempAction = WITHDRAW;
				TempX = Neighbours[i].x;
				TempY = Neighbours[i].y;
			}
		}
	}
	X = TempX;
	Y = TempY;
	Action = TempAction;
	return Number;
}


//Captures Stones on the Board by making a move to NewX, NewY by the action specified by Action which can be APPROACH or WITHDRAW
//The Move should be Legal
//The Stone and Board is only affected if the real variable is set to true
//Return Value: Number of Stones Captured
int CaptureStones(_Stone &Stone, _Board &Board, int NewX, int NewY, int Action, bool real)
{
	list<_Point2D> CapturedStones;
	switch(Action)
	{
	case APPROACH:
		CapturedStones = Approach(Stone, Board, NewX, NewY);
		break;
	case WITHDRAW:
		CapturedStones = Withdraw(Stone, Board, NewX, NewY);
		break;
	default:
		return -1;
	}

	if(real == false)
		return (int)CapturedStones.size();
	
	Stone.LastPosition.x = Stone.x;
	Stone.LastPosition.y = Stone.y;
	Stone.x = NewX;
	Stone.y = NewY;
	Stone.LastCapturedStones = (int)CapturedStones.size();
	Stone.CapturedStones += Stone.LastCapturedStones;
	Stone.History[NewY][NewX] = 1;

	

	list<_Point2D>::iterator it;
	_Point2D TempPoint;

	for(it = CapturedStones.begin(); it != CapturedStones.end(); it++)
	{
		TempPoint =  *it;
		//printf("Capturing at X: %d, Y: %d\n", TempPoint.x, TempPoint.y);
		Board.Matrix[TempPoint.y][TempPoint.x] = EMPTY;
	}

	Board.Matrix[Stone.LastPosition.y][Stone.LastPosition.x] = EMPTY;
	if(Stone.Colour == WHITE)
		Board.Matrix[Stone.y][Stone.x] = SELECTED_WHITE;
	else
		Board.Matrix[Stone.y][Stone.x] = SELECTED_BLACK;


	return (int)CapturedStones.size();;
}

//Returns the list of Stones that can be captured by Approaching the point NewX,NewY
//Point is assumed to be legal
list<_Point2D> Approach(_Stone Stone, _Board Board, int NewX, int NewY)
{
	list<_Point2D> CapturedStones;
	
	int DeltaX = NewX - Stone.x;
	int DeltaY = NewY - Stone.y;
	int SignX = 1;
	int SignY = 1;

	if(DeltaX != 0)
	{
		SignX = DeltaX/abs(DeltaX);
	}
	if(DeltaY != 0)
	{
		SignY = DeltaY/abs(DeltaY);
	}

	int i = 1;
	int xx;
	int yy;
	int Current;

	while(i < Board.Columns*Board.Rows)
	{
		xx = NewX;
		if(DeltaX != 0)
		{
			xx = NewX + (SignX*i);
		}

		yy = NewY;
		if(DeltaY != 0)
		{
			yy = NewY + (SignY*i);
		}

		if(IsOn(Board, xx, yy) == false)
			break;

		Current = GetStoneColour(Board, xx, yy);

		if(Current == EMPTY)
			break;
		else if(Current != Stone.Colour)
		{
			CapturedStones.push_front(NewPoint2D(xx, yy));
		}
		else
			break;
		i++;
	}

	return CapturedStones;
}

//Returns the list of Stones that can be captured by Withdrawing to the point NewX,NewY
//Point is assumed to be legal
list<_Point2D> Withdraw(_Stone Stone, _Board Board, int NewX, int NewY)
{
	list<_Point2D> CapturedStones;
	
	int DeltaX = NewX - Stone.x;
	int DeltaY = NewY - Stone.y;
	int SignX = 1;
	int SignY = 1;

	if(DeltaX != 0)
	{
		SignX = DeltaX/abs(DeltaX);
	}
	if(DeltaY != 0)
	{
		SignY = DeltaY/abs(DeltaY);
	}

	int i = 1;
	int xx;
	int yy;
	int Current;

	while(i < Board.Columns*Board.Rows)
	{
		xx = Stone.x;
		if(DeltaX != 0)
		{
			xx = Stone.x - (SignX*i);
		}

		yy = Stone.y;
		if(DeltaY != 0)
		{
			yy = Stone.y - (SignY*i);
		}

		if(IsOn(Board, xx, yy) == false)
			break;

		Current = GetStoneColour(Board, xx, yy);

		if(Current == EMPTY)
			break;
		else if(Current != Stone.Colour)
		{
			CapturedStones.push_front(NewPoint2D(xx, yy));
		}
		else
			break;
		i++;
	}

	return CapturedStones;
}


//Returns true if the stone can Make Legal moves from its current postion
bool CanMove(_Stone Stone, _Board Board, int *x, int *y)
{
	_Point2D Neighbours[8] = {{Stone.x + 1 , Stone.y}, {Stone.x - 1, Stone.y}, {Stone.x, Stone.y + 1}, {Stone.x, Stone.y - 1}, {Stone.x + 1, Stone.y + 1}, {Stone.x + 1, Stone.y - 1}, {Stone.x - 1, Stone.y - 1}, {Stone.x - 1, Stone.y + 1}};
	for(int i =0 ; i <8; i++)
	{
		if(LegalMove(Stone, Board, Neighbours[i].x, Neighbours[i].y))
		{
			if(x != NULL && y!= NULL)
			{
				*x = Neighbours[i].x;
				*y = Neighbours[i].y;
			}
			return true;
		}
	}
	return false;
}

//Makes a move to the point NewX, NewY
//Move has to be legal
//The Stone and Board are only affected if the real variable is set to true
//Returns the Action Taken
//If the Return Value is MUSTCHOOSE(which will be less than 0) the player must decide between APPROACH or WITHDRAW
int MakeMove(_Stone &Stone, _Board &Board, int NewX, int NewY, bool real, int &NB)
{
	list<_Point2D> ApprochedStones;
	ApprochedStones = Approach(Stone, Board, NewX, NewY);
	list<_Point2D> WithdrawedStones;
	WithdrawedStones = Withdraw(Stone, Board, NewX, NewY);
	
	int NAS = (int)ApprochedStones.size();
	int NWS = (int)WithdrawedStones.size();

	if(real == false)
	{
		if(NAS && NWS)
		{
			NB = 0;
			return MUSTCHOOSE;
		}
		else if(NAS > 0)
		{
			NB = NAS;
			return APPROACH;
		}
		else if(NWS > 0)
		{
			NB = NWS;
			return WITHDRAW;
		}
		else
		{
			NB = 0;
			return NOACTION;
		}
			
	}

	//real = true

	list<_Point2D>::iterator it;
	_Point2D TempPoint;

	if(NAS && NWS)
	{
		NB = 0;
		for(it = ApprochedStones.begin(); it != ApprochedStones.end(); it++)
		{
			TempPoint = *it;
			if(Stone.Colour == BLACK)
				Board.Matrix[TempPoint.y][TempPoint.x] = DELETE_APPROACH_WHITE;
			else if(Stone.Colour == WHITE)
				Board.Matrix[TempPoint.y][TempPoint.x] = DELETE_APPROACH_BLACK;
		}

		for(it = WithdrawedStones.begin(); it != WithdrawedStones.end(); it++)
		{
			TempPoint = *it;
			if(Stone.Colour == BLACK)
				Board.Matrix[TempPoint.y][TempPoint.x] = DELETE_WITHDRAW_WHITE;
			else if(Stone.Colour == WHITE)
				Board.Matrix[TempPoint.y][TempPoint.x] = DELETE_WITHDRAW_BLACK;
		}
		return MUSTCHOOSE;
	}
	else if(NAS > 0)
	{
		NB = CaptureStones(Stone, Board, NewX, NewY, APPROACH, true);
		return APPROACH;
	}
	else if(NWS > 0)
	{
		NB = CaptureStones(Stone, Board, NewX, NewY, WITHDRAW, true);
		return WITHDRAW;
	}
	else
	{
		NB = 0;
		
		Stone.LastPosition.x = Stone.x;
		Stone.LastPosition.y = Stone.y;
		Stone.x = NewX;
		Stone.y = NewY;
		Stone.LastCapturedStones = 0;
		Stone.History[NewY][NewX] = 1;
		Board.Matrix[Stone.LastPosition.y][Stone.LastPosition.x] = EMPTY;
		if(Stone.Colour == WHITE)
			Board.Matrix[Stone.y][Stone.x] = SELECTED_WHITE;
		else
			Board.Matrix[Stone.y][Stone.x] = SELECTED_BLACK;
		return NOACTION;
	}

	
}


//Selects the Stone on the Board and Unselects others Selected stones if any
void Select(_Stone Stone, _Board &Board)
{
	UnselectALL(Board);
	if(Stone.Colour == BLACK)
	{
		Board.Matrix[Stone.y][Stone.x] = SELECTED_BLACK;
	}
	else if(Stone.Colour == WHITE)
	{
		Board.Matrix[Stone.y][Stone.x] = SELECTED_WHITE;
	}
}

//Returns true if the stone is selected on the board
bool IsSelected(_Stone Stone, _Board Board)
{
	if(Board.Matrix[Stone.y][Stone.x] == SELECTED_BLACK && Stone.Colour == BLACK)
		return true;
	else if(Board.Matrix[Stone.y][Stone.x] == SELECTED_WHITE && Stone.Colour == WHITE)
		return true;
	else
		return false;
}

//Unselects the Stone on the Board
void UnSelect(_Stone Stone, _Board &Board)
{
	if(IsSelected(Stone, Board))
	{
		Board.Matrix[Stone.y][Stone.x] = Stone.Colour;
	}
}

//Returns true if the Stone can be captured by the opponent given the current board status
bool CanBeCaptured(_Stone Stone, _Board Board)
{
	UnselectALL(Board);
	int x;
	int y;
	
	x = Stone.x;
	y = Stone.y;

	int OpponentColour;
	if(Stone.Colour == BLACK)
		OpponentColour = WHITE;
	else
		OpponentColour = BLACK;

	//x, y+1
	if(IsOn(Board, x, y + 1) && IsOn(Board, x, y + 2))
	{
		if((Board.Matrix[y + 1][x] == EMPTY && Board.Matrix[y + 2][x] == OpponentColour) ||
			(Board.Matrix[y + 1][x] == OpponentColour && Board.Matrix[y + 2][x] == EMPTY))
			return true;
	}

	//x + 1, y
	if(IsOn(Board, x + 1, y) && IsOn(Board, x + 2, y))
	{
		if((Board.Matrix[y][x + 1] == EMPTY && Board.Matrix[y][x + 2] == OpponentColour) ||
			(Board.Matrix[y][x + 1] == OpponentColour && Board.Matrix[y][x + 2] == EMPTY))
			return true;
	}

	//x, y-1
	if(IsOn(Board, x, y - 1) && IsOn(Board, x, y - 2))
	{
		if((Board.Matrix[y - 1][x] == EMPTY && Board.Matrix[y - 2][x] == OpponentColour) ||
			(Board.Matrix[y - 1][x] == OpponentColour && Board.Matrix[y - 2][x] == EMPTY))
			return true;
	}

	//x - 1, y
	if(IsOn(Board, x - 1, y) && IsOn(Board, x - 2, y))
	{
		if((Board.Matrix[y][x - 1] == EMPTY && Board.Matrix[y][x - 2] == OpponentColour) ||
			(Board.Matrix[y][x - 1] == OpponentColour && Board.Matrix[y][x - 2] == EMPTY))
			return true;
	}

	if(IsOnStrongPosition(Stone))
	{
		//x + 1 , y + 1
		if(IsOn(Board, x + 1, y + 1) && IsOn(Board, x + 2, y + 2))
		{
			if((Board.Matrix[y + 1][x + 1] == EMPTY && Board.Matrix[y + 2][x + 2] == OpponentColour) ||
				(Board.Matrix[y + 1][x + 1] == OpponentColour && Board.Matrix[y + 2][x + 2] == EMPTY))
				return true;
		}
		
		//x - 1 , y - 1
		if(IsOn(Board, x - 1, y - 1) && IsOn(Board, x - 2, y - 2))
		{
			if((Board.Matrix[y - 1][x - 1] == EMPTY && Board.Matrix[y - 2][x - 2] == OpponentColour) ||
				(Board.Matrix[y - 1][x - 1] == OpponentColour && Board.Matrix[y - 2][x - 2] == EMPTY))
				return true;
		}

		//x + 1 , y - 1
		if(IsOn(Board, x + 1, y - 1) && IsOn(Board, x + 2, y - 2))
		{
			if((Board.Matrix[y - 1][x + 1] == EMPTY && Board.Matrix[y - 2][x + 2] == OpponentColour) ||
				(Board.Matrix[y - 1][x + 1] == OpponentColour && Board.Matrix[y - 2][x + 2] == EMPTY))
				return true;
		}

		//x - 1 , y + 1
		if(IsOn(Board, x - 1, y + 1) && IsOn(Board, x - 2, y + 2))
		{
			if((Board.Matrix[y + 1][x - 1] == EMPTY && Board.Matrix[y + 2][x - 2] == OpponentColour) ||
				(Board.Matrix[y + 1][x - 1] == OpponentColour && Board.Matrix[y + 2][x - 2] == EMPTY))
				return true;
		}
	}
	return false;
}

//Returns true if the Stone is on a Strong Postion else false
bool IsOnStrongPosition(_Stone Stone)
{
	if(((Stone.x)%2 == (Stone.y)%2) && Stone.x >= 0 && Stone.y >= 0 && Stone.x < COLUMNS && Stone.y < ROWS)
		return true;
	return false;
}
