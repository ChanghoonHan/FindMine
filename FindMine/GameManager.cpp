#include "GameManager.h"

GameManager* GameManager::S = NULL;

void GameManager::InitGameManager(HWND& hWnd, HDC& hdc)
{
	m_hWnd = hWnd;
	srand(time(NULL));
	m_backGroundBitMap.Init(hdc, "back.bmp");
	m_PieceBitMap.InitPieceBitMap(hdc);
	Piece::SetlpPieceBitMap(&m_PieceBitMap);
	SetGame(LEVEL_EASY);
}

void GameManager::SetGame(LEVEL level)
{
	KillTimer(m_hWnd, 1);
	m_isGameStart = true;
	m_isGameClear = false;
	m_isGameOver = false;
	m_level = level;
	SetGameMapMatrix();
	m_second = 0;
}

void GameManager::SetGameMapMatrix()
{
	int x, y;
	int mineCount = 0;

	SetPieceCountndMaxMineCount();
	InitVistedPoint();
	InitGameMapMatrix();

	while (1)
	{
		if (mineCount == m_maxMineCount)
		{
			break;
		}

		x = rand() % m_pieceCountX;
		y = rand() % m_pieceCountY;
		if (m_GameMapMatrix[y][x] == PIECEVALUE_0)
		{
			m_GameMapMatrix[y][x] = PIECEVALUE_MINE;
			mineCount++;
		}
	}

	for (size_t i = 0; i < m_pieceCountY; i++)
	{
		for (size_t j = 0; j < m_pieceCountX; j++)
		{
			if (m_GameMapMatrix[i][j] == PIECEVALUE_MINE)
			{
				for (size_t k = 0; k < 8; k++)
				{
					if (i + direct[k][0] < 0 || i + direct[k][0] > m_pieceCountY-1 ||
						j + direct[k][1] < 0 || j + direct[k][1] > m_pieceCountX-1)
					{
						continue;
					}


					if ((m_GameMapMatrix[i + direct[k][0]][j + direct[k][1]] != PIECEVALUE_MINE))
					{
						m_GameMapMatrix[i + direct[k][0]][j + direct[k][1]] =
							(PIECEVALUE)(m_GameMapMatrix[i + direct[k][0]][j + direct[k][1]] + 1);
					}
				}
			}
		}
	}
	InitGameMapBitMap();
}

void GameManager::InitGameMapMatrix()
{
	memset(m_GameMapMatrix, PIECEVALUE_0, sizeof(int) * 16 * 30);
}

void GameManager::InitGameMapBitMap()
{
	for (size_t i = 0; i < m_pieceCountY; i++)
	{
		for (size_t j = 0; j < m_pieceCountX; j++)
		{
			m_GameMapBitMap[i][j].InitPiece((PIECEBITMAPTYPE)m_GameMapMatrix[i][j] ,j, i);
		}
	}
}

void GameManager::OpenSuccessiveNearPiece(int x, int y)
{
	bool isHaveMine = true;
	POINT point = { x, y };
	m_visitedPoint.push_back(point);

	if (m_GameMapMatrix[y][x] == PIECEVALUE_0)
	{
		if (m_GameMapBitMap[y][x].GetState() != PIECESTATE_FLAG)
		{
			m_GameMapBitMap[y][x].ChangeState(PIECESTATE_OPEN);
		}
		isHaveMine = false;
	}

	for (size_t i = 0; i < 8; i++)
	{
		if (y + direct[i][0] < 0 || y + direct[i][0] > m_pieceCountY-1 ||
			x + direct[i][1] < 0 || x + direct[i][1] > m_pieceCountX-1)
		{
			continue;
		}

		if (!isHaveMine && m_GameMapBitMap[y + direct[i][0]][x + direct[i][1]].GetState() != PIECESTATE_FLAG)
		{
			m_GameMapBitMap[y + direct[i][0]][x + direct[i][1]].ChangeState(PIECESTATE_OPEN);
		}

		if (m_GameMapMatrix[y + direct[i][0]][x + direct[i][1]] == PIECEVALUE_0 &&
			!IsVisited(x + direct[i][1], y + direct[i][0]))
		{
			OpenSuccessiveNearPiece(x + direct[i][1], y + direct[i][0]);
		}
	}

	return;
}

bool GameManager::IsVisited(int x, int y)
{
	for (auto iter = m_visitedPoint.begin(); iter != m_visitedPoint.end(); iter++)
	{
		if (iter->x == x && iter->y == y)
		{
			return true;
		}
	}

	return false;
}

void GameManager::InitVistedPoint()
{
	m_visitedPoint.erase(m_visitedPoint.begin(), m_visitedPoint.end());
}

void GameManager::SetPieceCountndMaxMineCount()
{
	switch (m_level)
	{
	case LEVEL_EASY:
		m_pieceCountX = 9;
		m_pieceCountY = 9;
		m_maxMineCount = 7;
		break;

	case LEVEL_NORMAL:
		m_pieceCountX = 20;
		m_pieceCountY = 16;
		m_maxMineCount = 30;
		break;

	case LEVEL_HARD:
		m_pieceCountX = 30;
		m_pieceCountY = 16;
		m_maxMineCount = 70;
		break;

	default:
		break;
	}
	m_willFindMineCount = m_maxMineCount;
}

void GameManager::OperatorInputClick(int x, int y, bool isRightMouse)
{
	if (m_isGameClear || m_isGameOver ||
		x < GAMEMAP_X_BEZEL || x > 870 - GAMEMAP_X_BEZEL ||
			y < GAMEMAP_Y_BEZEL || y > 512 - GAMEMAP_Y_BEZEL)
	{
		return;
	}

	int matrixX = (x - GAMEMAP_X_BEZEL) / PIECEBITMAPSIZE;
	int matrixY = (y - GAMEMAP_Y_BEZEL) / PIECEBITMAPSIZE;

	if (matrixX > -1 && matrixX < m_pieceCountX && matrixY > -1 
		&& matrixY < m_pieceCountY && m_isGameStart)
	{
		SetTimer(m_hWnd, 1, 1000, NULL);
		m_isGameStart = false;
	}

	if (!isRightMouse)
	{
		if (m_GameMapBitMap[matrixY][matrixX].GetState() == PIECESTATE_CLOSE)
		{
			if (m_GameMapMatrix[matrixY][matrixX] == PIECEVALUE_MINE)
			{
				m_isGameOver = true;
			}
			else
			{
				InitVistedPoint();
				OpenSuccessiveNearPiece(matrixX, matrixY);
			}
		}
		m_GameMapBitMap[matrixY][matrixX].Click(isRightMouse);
	}
	else
	{
		m_GameMapBitMap[matrixY][matrixX].Click(isRightMouse);
		if (m_GameMapBitMap[matrixY][matrixX].GetState() == PIECESTATE_FLAG)
		{
			m_willFindMineCount--;
			if (m_willFindMineCount == 0)
			{
				IsGameClearWithFlagCheck();
			}
		}
		else if(m_GameMapBitMap[matrixY][matrixX].GetState() == PIECESTATE_CLOSE)
		{
			m_willFindMineCount++;
		}
	}
	IsGameClearCheck();
}

void GameManager::OperatorInputDoubleClick(int x, int y)
{
	if (m_isGameClear || m_isGameOver ||
		x < GAMEMAP_X_BEZEL || x > 870 - GAMEMAP_X_BEZEL ||
		y < GAMEMAP_Y_BEZEL || y > 512 - GAMEMAP_Y_BEZEL)
	{
		return;
	}

	int matrixX = (x - GAMEMAP_X_BEZEL) / PIECEBITMAPSIZE;
	int matrixY = (y - GAMEMAP_Y_BEZEL) / PIECEBITMAPSIZE;

	if (m_GameMapBitMap[matrixY][matrixX].GetState() == PIECESTATE_OPEN)
	{
		if (GetFlagStateCountAtNearPlace(matrixX, matrixY) ==
			m_GameMapMatrix[matrixY][matrixX])
		{
			InitVistedPoint();
			OpenAllNearPiece(matrixX, matrixY);
		}
	}
	IsGameClearCheck();
}

void GameManager::OpenAllNearPiece(int x, int y)
{
	for (size_t i = 0; i < 8; i++)
	{
		if (y + direct[i][0] < 0 || y + direct[i][0] > m_pieceCountY - 1 ||
			x + direct[i][1] < 0 || x + direct[i][1] > m_pieceCountX - 1)
		{
			continue;
		}

		if (m_GameMapBitMap[y + direct[i][0]][x + direct[i][1]].GetState()
			!= PIECESTATE_FLAG)
		{
			m_GameMapBitMap[y + direct[i][0]][x + direct[i][1]].ChangeState(PIECESTATE_OPEN);
			if (m_GameMapMatrix[y + direct[i][0]][x + direct[i][1]] == PIECEVALUE_MINE)
			{
				m_isGameOver = true;
			}
			if (m_GameMapMatrix[y + direct[i][0]][x + direct[i][1]] == PIECEVALUE_0)
			{
				OpenSuccessiveNearPiece(x + direct[i][1], y + direct[i][0]);
			}
		}
	}
}

int GameManager::GetFlagStateCountAtNearPlace(int x, int y)
{
	int flagStateCount = 0;
	for (size_t i = 0; i < 8; i++)
	{
		if (y + direct[i][0] < 0 || y + direct[i][0] > m_pieceCountY - 1 ||
			x + direct[i][1] < 0 || x + direct[i][1] > m_pieceCountX - 1)
		{
			continue;
		}

		if (m_GameMapBitMap[y + direct[i][0]][x + direct[i][1]].GetState() 
			== PIECESTATE_FLAG)
		{
			flagStateCount++;
		}
	}

	return flagStateCount;
}

void GameManager::IsGameClearWithFlagCheck()
{
	m_isGameClear = true;

	for (size_t i = 0; i < m_pieceCountY; i++)
	{
		for (size_t j = 0; j < m_pieceCountX; j++)
		{
			if (m_GameMapMatrix[i][j] == PIECEVALUE_MINE)
			{
				if (m_GameMapBitMap[i][j].GetState() != PIECESTATE_FLAG)
				{
					m_isGameClear = false;
				}
			}
		}
	}
}

void GameManager::IsGameClearCheck()
{
	m_isGameClear = true;

	for (size_t i = 0; i < m_pieceCountY; i++)
	{
		for (size_t j = 0; j < m_pieceCountX; j++)
		{
			if (m_GameMapMatrix[i][j] != PIECEVALUE_MINE)
			{
				if (m_GameMapBitMap[i][j].GetState() != PIECESTATE_OPEN)
				{
					m_isGameClear = false;
					return;
				}
			}
		}
	}
}

void GameManager::Update()
{
	m_second++;
}

void GameManager::DrawGame(HDC& hdc)
{
	char time[32];
	char willFindMineCount[32];
	sprintf(time, "%d", m_second);
	sprintf(willFindMineCount, "%d", m_willFindMineCount);

	m_backGroundBitMap.Draw(hdc, 0, 0, 870, 512);
	for (size_t i = 0; i < m_pieceCountY; i++)
	{
		for (size_t j = 0; j < m_pieceCountX; j++)
		{
			m_GameMapBitMap[i][j].DrawPiece(hdc);
		}
	}

	if (m_isGameClear || m_isGameOver)
	{
		KillTimer(m_hWnd, 1);
		if (m_isGameClear)
		{
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 370, 10, "GAME CLEAR!!!!!!!!!!", strlen("GAME CLEAR!!!!!!!!!!"));
			SetBkMode(hdc, OPAQUE);
		}

		if (m_isGameOver)
		{
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 370, 10, "GAME OVER!!!!!!!!!!!", strlen("GAME OVER!!!!!!!!!!!"));
			SetBkMode(hdc, OPAQUE);
		}
	}

	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 165, 480, time, strlen(time));
	TextOut(hdc, 660, 480, willFindMineCount, strlen(willFindMineCount));
	SetBkMode(hdc, OPAQUE);

	Rectangle(hdc, 870 / 2 - 30, 512 - 32, 870 / 2 + 30, 512 - 12);
	RECT rect = { 870 / 2 - 30, 512 - 30, 870 / 2 + 30, 512 - 10 };
	DrawText(hdc, "RESET", strlen("RESET"), &rect, DT_CENTER);
}

GameManager* GameManager::GetInstance()
{
	if (S == NULL)
	{
		S = new GameManager();
	}

	return S;
}

void GameManager::Release()
{
	m_backGroundBitMap.Release();

	for (size_t i = 0; i < 16; i++)
	{
		for (size_t j = 0; j < 30; j++)
		{
			m_GameMapBitMap[i][j].ReleasePiece();
		}
	}

	Piece::ReleaseStaticBitMap();

	SAFE_DELETE(S);
}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}
