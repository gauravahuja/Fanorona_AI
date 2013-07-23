#ifndef _AIPLAYER_H
#define _AIPLAYER_H

#include <SDL.h>
#include "Constants.h"
#include "Board.h"
#include "Stone.h"

void AIPlay(_Board &Board, SDL_Surface* Screen, int AIColour, int Mode);
int GreedyPlay(_Board &Board, SDL_Surface* Screen, int AIColour);
list<_Stone> GetFreeStones(int Colour, _Board Board);
_Stone SelectStone(_Board Board, int Colour, int &MoveX, int &MoveY, int &Action, int &Nb);


#endif 