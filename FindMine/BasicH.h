#pragma once
#define SAFE_DELETE(a) if((a) != NULL) delete (a); (a)= NULL;
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

#define GAMEMAP_X_BEZEL 43
#define GAMEMAP_Y_BEZEL 46
#define PIECEBITMAPSIZE 26

const int direct[8][2] = { { -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 } };

enum LEVEL
{
	LEVEL_EASY,
	LEVEL_NORMAL,
	LEVEL_HARD,
};

enum PIECESTATE
{
	PIECESTATE_OPEN,
	PIECESTATE_CLOSE,
	PIECESTATE_FLAG,
};

enum PIECEVALUE
{
	PIECEVALUE_0,
	PIECEVALUE_1,
	PIECEVALUE_2,
	PIECEVALUE_3,
	PIECEVALUE_4,
	PIECEVALUE_5,
	PIECEVALUE_6,
	PIECEVALUE_7,
	PIECEVALUE_8,
	PIECEVALUE_MINE
};