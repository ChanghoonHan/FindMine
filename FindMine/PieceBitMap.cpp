#include "PieceBitMap.h"

void PieceBitMap::InitPieceBitMap(HDC& hdc)
{
	m_PieceBitMap[PIECEBITMAPTYPE_0].Init(hdc, "block_0.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_1].Init(hdc, "block_1.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_2].Init(hdc, "block_2.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_3].Init(hdc, "block_3.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_4].Init(hdc, "block_4.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_5].Init(hdc, "block_5.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_6].Init(hdc, "block_6.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_7].Init(hdc, "block_7.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_8].Init(hdc, "block_8.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_MINE].Init(hdc, "mine.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_CLOSE].Init(hdc, "block.bmp");
	m_PieceBitMap[PIECEBITMAPTYPE_FLAG].Init(hdc, "flag.bmp");
}

void PieceBitMap::DrawPieceBitMap(PIECEBITMAPTYPE pieceBitMapType, HDC& hdc, int x, int y)
{
	m_PieceBitMap[pieceBitMapType].Draw(hdc, x, y);
}

PieceBitMap::PieceBitMap()
{
}


PieceBitMap::~PieceBitMap()
{
}
