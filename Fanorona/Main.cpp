#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Constants.h"
#include "Board.h"
#include "Stone.h"
#include "AIPlayer.h"

SDL_Surface *Screen;

int Initialize()
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("SDL Initialization failed: %s\n", SDL_GetError());
		return -1;
	}

	Screen = SDL_SetVideoMode(Width, Height, PixelDepth, SDL_SWSURFACE);

	if(Screen == NULL)
	{
		printf("Failed to acquire video Mode: %s\n", SDL_GetError());
		return -1;
	}

	SDL_WM_SetCaption(Title, Title);
	SDL_WM_SetIcon(SDL_LoadBMP(Icon), NULL);

	return 0;
}

void CleanUp()
{
	SDL_Quit();
}

int main( int argc, char* argv[] )
{
	
	_Board Board;
	SDL_Event Event;
	bool Running = true;
	int AIColour;
	int HumanColour;
	int Selected = WHITE;
	bool InverColour = false;
	int HumanProgress = 0;
	int HumanMove = 0;
	_Point2D MousePos = NewPoint2D(-1, -1);
	_Point2D SelectedPostion = NewPoint2D(-1, -1);
	_Stone SelectedStone;
	int Nb;
	int X;
	int Y;
	int Action;
	int Status;
	int Winner;

	srand((unsigned int)time(NULL));
	
	HumanColour  = rand()%2 + 1;
	
	if(HumanColour == BLACK)
	{
		printf("Human Colour: Black\nAIColour: White\n");
		AIColour = WHITE;
	}
	else
	{
		printf("Human Colour: White\nAIColour: Black\n");
		AIColour = BLACK;
	}
	printf("\n");

	if(Initialize() < 0)
		return -1;

	InitializeBoard(Board);
	Populate(Board);
	PopulateGUI(Board, Screen);
	
	HumanProgress = 0;
	Selected = WHITE;
	HumanMove = 0;
	Status = PLAY;

	while(Running)
	{
		MousePos = NewPoint2D(-1, -1);
		SelectedPostion = NewPoint2D(-1, -1);

		while(SDL_PollEvent(&Event))
		{
			switch(Event.type)
			{
			case SDL_QUIT:
				Running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				MousePos = NewPoint2D(Event.button.x, Event.button.y);
				SelectedPostion = GetSelectedPostion(Board, MousePos);
				//printf("Mouse X: %d, Y: %d\n", MousePos.x, MousePos.y);
				//printf("Selected X: %d, Y: %d\n", SelectedPostion.x, SelectedPostion.y);
				break;
			}
		}
		if(Status == PLAY)
		{
			if(AIColour == Selected)
			{
				AIPlay(Board, Screen,AIColour, GREEDY);

				InverColour = true;
			}
			else if(HumanColour == Selected)
			{
				switch(HumanProgress)
				{
				case 0:
					if(SelectedPostion.x >= 0 && SelectedPostion.y >= 0)
					{
						if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == HumanColour)
						{
							SelectedStone = NewStone(SelectedPostion.x, SelectedPostion.y, HumanColour);
							Select(SelectedStone, Board);
							HumanProgress = 1;
						}
					}
					break;
				case 1:
					if(CanMakeCapturingMove(SelectedStone, Board, X, Y, Action) == 0 
						|| CanMove(SelectedStone, Board) == false)//Added Today
					{
						if(HumanMove != 0)
						{
							InverColour = true;
							break;
						}
					}
					if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == HumanColour && HumanMove == 0)
					{
						SelectedStone = NewStone(SelectedPostion.x, SelectedPostion.y, HumanColour);
						Select(SelectedStone, Board);
						break;
					}
					if(SelectedPostion.x >= 0 && SelectedPostion.y >= 0)
					{
						if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == EMPTY)
						{
							if(CanMove(SelectedStone, Board) && LegalMove(SelectedStone, Board, SelectedPostion.x, SelectedPostion.y))
							{
								HumanMove = 1;
								
								if(MakeMove(SelectedStone, Board, SelectedPostion.x, SelectedPostion.y, true, Nb) == MUSTCHOOSE)
								{
									X = SelectedPostion.x;
									Y = SelectedPostion.y;
									HumanProgress = 2;
									Nb = -1;
								}
								if(Nb == 0)
								{
									InverColour = true;
								}
							}
						}
					}
					break;
				case 2:
					if(SelectedPostion.x >= 0 && SelectedPostion.y >= 0)
					{
						Nb = 0;
						Board.Matrix[SelectedStone.y][SelectedStone.x] = EMPTY;

						if(HumanColour == WHITE)
						{
							Board.Matrix[Y][X] = SELECTED_WHITE;
							if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == DELETE_APPROACH_BLACK)
							{
								for(int i = 0; i <Board.Rows; i++)
								{
									for(int j = 0; j < Board.Columns; j++)
									{
										if(Board.Matrix[i][j] == DELETE_APPROACH_BLACK)
										{
											Nb++;
											Board.Matrix[i][j] = EMPTY;
										}
										if(Board.Matrix[i][j] == DELETE_WITHDRAW_BLACK)
											Board.Matrix[i][j] = BLACK;
									}
								}
							}
							else if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == DELETE_WITHDRAW_BLACK)
							{
								for(int i = 0; i <Board.Rows; i++)
								{
									for(int j = 0; j < Board.Columns; j++)
									{
										if(Board.Matrix[i][j] == DELETE_APPROACH_BLACK)
											Board.Matrix[i][j] = BLACK;
										if(Board.Matrix[i][j] == DELETE_WITHDRAW_BLACK)
										{
											Nb++;
											Board.Matrix[i][j] = EMPTY;
										}
									}
								}
							}

						}
						else
						{
							Board.Matrix[Y][X] = SELECTED_BLACK;
							if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == DELETE_APPROACH_WHITE)
							{
								for(int i = 0; i <Board.Rows; i++)
								{
									for(int j = 0; j < Board.Columns; j++)
									{
										if(Board.Matrix[i][j] == DELETE_APPROACH_WHITE)
										{
											Nb++;
											Board.Matrix[i][j] = EMPTY;
										}
										if(Board.Matrix[i][j] == DELETE_WITHDRAW_WHITE)
											Board.Matrix[i][j] = WHITE;
									}
								}
							}
							else if(Board.Matrix[SelectedPostion.y][SelectedPostion.x] == DELETE_WITHDRAW_WHITE)
							{
								for(int i = 0; i <Board.Rows; i++)
								{
									for(int j = 0; j < Board.Columns; j++)
									{
										if(Board.Matrix[i][j] == DELETE_APPROACH_WHITE)
											Board.Matrix[i][j] = WHITE;
										if(Board.Matrix[i][j] == DELETE_WITHDRAW_WHITE)
										{
											Nb++;
											Board.Matrix[i][j] = EMPTY;
										}
									}
								}
							}
						}	
						SelectedStone.LastPosition.x = SelectedStone.x;
						SelectedStone.LastPosition.y = SelectedStone.y;
						SelectedStone.x = X;
						SelectedStone.y = Y;
						SelectedStone.LastCapturedStones = Nb;
						SelectedStone.CapturedStones += SelectedStone.LastCapturedStones;
						SelectedStone.History[Y][X] = 1;
						if(Nb > 0)
							HumanProgress = 1;
					}
					break;
				}
			}
		
			if(InverColour)
			{
				InverColour = false;
				HumanMove = 0;
				HumanProgress = 0;
				UnselectALL(Board);
				if(Selected == WHITE)
					Selected = BLACK;
				else
					Selected = WHITE;
				if(Selected == HumanColour)
					printf("Human Chance\n\n");
				else
					printf("AIPlayer Chance\n\n");
				SelectedStone = NewStone(-1,-1, -1);
			}
			Winner = CheckWinner(Board);
			if(Winner != EMPTY)
			{
				Status = PAUSE;
				if(Winner == BLACK)
					printf("\nBLACK won the game\n");
				else
					printf("\nWHITE won the game\n");
			}
		}

		PopulateGUI(Board, Screen);
		SDL_UpdateRect(Screen, 0, 0,0, 0);
		SDL_Delay(250);
	}
	
	CleanUp();
	return 0;
}