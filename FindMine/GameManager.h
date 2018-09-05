#pragma once
#include "BasicH.h"
#include "BitMap.h"
#include "PieceBitMap.h"
#include "Piece.h"

class GameManager
{
	PieceBitMap m_PieceBitMap;
	HWND m_hWnd;
	bool m_isGameStart;
	bool m_isGameOver;
	bool m_isGameClear;
	BitMap m_backGroundBitMap;
	PIECEVALUE m_GameMapMatrix[16][30];
	Piece m_GameMapBitMap[16][30];
	vector<POINT> m_visitedPoint;
	int m_second;

	LEVEL m_level;
	int m_pieceCountX;
	int m_pieceCountY;
	int m_maxMineCount;
	int m_willFindMineCount;

	static GameManager* S;
	GameManager();

	void IsGameClearCheck();
	void IsGameClearWithFlagCheck();
	void SetPieceCountndMaxMineCount();
	void InitVistedPoint();
	bool IsVisited(int x, int y);
	void OpenSuccessiveNearPiece(int x, int y);
	void SetGameMapMatrix();
	void InitGameMapMatrix();
	void InitGameMapBitMap();
	int GetFlagStateCountAtNearPlace(int x, int y);
	void OpenAllNearPiece(int x, int y);

public:
	void Update();
	void SetGame(LEVEL level);
	void OperatorInputClick(int x, int y, bool isRightMouse);
	void OperatorInputDoubleClick(int x, int y);
	void InitGameManager(HWND& hWnd, HDC& hdc);
	void DrawGame(HDC& hdc);

	static GameManager* GetInstance();
	void Release();

	~GameManager();
};

