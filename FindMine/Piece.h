#pragma once
#include "BasicH.h"
#include "PieceBitMap.h"

class Piece
{
	PIECESTATE m_state;
	PIECEBITMAPTYPE m_pieceBitMapType;
	POINT m_pos;
	static PieceBitMap* m_lpPieceBitMap;

public:
	PIECESTATE GetState();
	void ChangeState(PIECESTATE state);
	void Click(bool isRrightMouse);
	void InitPiece(PIECEBITMAPTYPE pieceBitMapType, int x, int y);
	void DrawPiece(HDC& hdc);
	
	void ReleasePiece();

	static void SetlpPieceBitMap(PieceBitMap* lpPieceBitMap);
	static void ReleaseStaticBitMap();

	Piece();
	virtual ~Piece();
};

