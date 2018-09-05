#include "Piece.h"

PieceBitMap* Piece::m_lpPieceBitMap = NULL;

void Piece::ChangeState(PIECESTATE state)
{
	m_state = state;
}

void Piece::Click(bool isRrightMouse)
{
	if (isRrightMouse && m_state != PIECESTATE_OPEN)
	{
		if (m_state == PIECESTATE_FLAG)
		{
			ChangeState(PIECESTATE_CLOSE);
		}
		else
		{
			ChangeState(PIECESTATE_FLAG);
		}
	}
	else if(m_state != PIECESTATE_FLAG)
	{
		ChangeState(PIECESTATE_OPEN);
	}
}

PIECESTATE Piece::GetState()
{
	return m_state;
}

void Piece::InitPiece(PIECEBITMAPTYPE pieceBitMapType, int x, int y)
{
	m_pieceBitMapType = pieceBitMapType;
	m_pos.x = x;
	m_pos.y = y;
	m_state = PIECESTATE_CLOSE;
}

void Piece::DrawPiece(HDC& hdc)
{
	if (m_state == PIECESTATE_CLOSE)
	{
		m_lpPieceBitMap->DrawPieceBitMap(PIECEBITMAPTYPE_CLOSE, hdc, 
			GAMEMAP_X_BEZEL + m_pos.x * PIECEBITMAPSIZE, GAMEMAP_Y_BEZEL + m_pos.y * PIECEBITMAPSIZE);
	}
	else if(m_state == PIECESTATE_FLAG)
	{
		m_lpPieceBitMap->DrawPieceBitMap(PIECEBITMAPTYPE_FLAG, hdc, 
			GAMEMAP_X_BEZEL + m_pos.x * PIECEBITMAPSIZE, GAMEMAP_Y_BEZEL + m_pos.y * PIECEBITMAPSIZE);
	}
	else
	{
		m_lpPieceBitMap->DrawPieceBitMap(m_pieceBitMapType, hdc,
			GAMEMAP_X_BEZEL + m_pos.x * PIECEBITMAPSIZE, GAMEMAP_Y_BEZEL + m_pos.y * PIECEBITMAPSIZE);
	}
}

void Piece::ReleasePiece()
{
}

void Piece::SetlpPieceBitMap(PieceBitMap* lpPieceBitMap)
{
	m_lpPieceBitMap = lpPieceBitMap;
}

void Piece::ReleaseStaticBitMap()
{
}

Piece::Piece()
{
}


Piece::~Piece()
{
}
