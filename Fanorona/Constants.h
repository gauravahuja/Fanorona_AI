#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define EMPTY	0
#define BLACK	1
#define WHITE	2

#define SELECTED_BLACK 11
#define SELECTED_WHITE 22

#define DELETE_WITHDRAW_BLACK 81
#define DELETE_WITHDRAW_WHITE 82

#define DELETE_APPROACH_BLACK 91
#define DELETE_APPROACH_WHITE 92

#define APPROACH	100
#define WITHDRAW	101

#define MUSTCHOOSE	-1

#define NOACTION	200

#define ROWS	5
#define COLUMNS	9

#define PLAY	400
#define PAUSE	401

#define GREEDY	500
#define PRUNING	600

const char Title[] = "Fanorona";
const char Icon[] = "./Data/Icon.bmp";

const char BlackStone[] = "./Data/Black.png";
const char SelectedBlackStone[] = "./Data/SelectedBlack.png";
const char DeleteBlackStone[] = "./Data/DeleteBlack.png";


const char WhiteStone[] = "./Data/White.png";
const char SelectedWhiteStone[] = "./Data/SelectedWhite.png";
const char DeleteWhiteStone[] = "./Data/DeleteWhite.png";

const char Background[] = "./Data/Background.png";

const int Width = 500;
const int Height = 300;
const int PixelDepth = 32;
const int StoneSize = 35;	//Pixels
const int StoneSpace = 50;	//Pixels

#endif