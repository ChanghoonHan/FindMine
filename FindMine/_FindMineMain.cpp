#include <Windows.h>
#include <crtdbg.h>//메모리 릭검사
#include "GameManager.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;

LEVEL nowLevel = LEVEL_EASY;
int dialogID = IDC_RADIO4;
char lpszClass[256] = "FindMine";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace,
	LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(142);

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;//통산 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//미리만들어놓은 브러쉬 가져옴
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;//프로시저 받아옴 메세지 재정의를 모아놓은 함수
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;//윈도우 스타일

	RegisterClass(&WndClass);//os에 윈도우 등록

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 885, 570,
		NULL, (HMENU)NULL, hInstance, NULL);
	//CW_ ->x y wid hei
	//부모윈도우, idset, instand, NULL

	ShowWindow(hWnd, nCmdShow);//한번 그려옴

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);//메세지 번역 큐에서 가져와서 번역 파생처리
		DispatchMessage(&Message);//윈도우 프로시저가 콜 됨
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
//핸들, 어떤메세지, 부가정보1, 부가정보2
{
	PAINTSTRUCT ps;
	HDC hdc;
	int mousePosX, mousePosY;

	switch (iMessage)
	{
	case WM_CREATE:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->InitGameManager(hWnd, hdc);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		GameManager::GetInstance()->Update();
		InvalidateRect(hWnd, NULL, 0);
		return 0;

	case WM_LBUTTONDOWN:
		mousePosX = LOWORD(lParam);
		mousePosY = HIWORD(lParam);
		if (mousePosX > 870 / 2 - 30 && mousePosX < 870 / 2 + 30 &&
			mousePosY > 512 - 32 && mousePosY < 512 - 12)
		{
			GameManager::GetInstance()->SetGame(nowLevel);
		}
		GameManager::GetInstance()->OperatorInputClick(mousePosX, mousePosY, false);
		InvalidateRect(hWnd, NULL, 0);
		return 0;

	case WM_LBUTTONDBLCLK:
		mousePosX = LOWORD(lParam);
		mousePosY = HIWORD(lParam);
		GameManager::GetInstance()->OperatorInputDoubleClick(mousePosX, mousePosY);
		InvalidateRect(hWnd, NULL, 0);
		return 0;

	case WM_RBUTTONDOWN:
		mousePosX = LOWORD(lParam);
		mousePosY = HIWORD(lParam);
		GameManager::GetInstance()->OperatorInputClick(mousePosX, mousePosY, true);
		InvalidateRect(hWnd, NULL, 0);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->DrawGame(hdc);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_GAMESETTING_LEVEL:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, NULL, 0);
		return 0;
		break;

	case WM_DESTROY:
		GameManager::GetInstance()->Release();
		PostQuitMessage(0);//큐에 나간다는메세지 적재
		return 0;
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));//기본 메세지처리..
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hwndDlg, IDC_RADIO4, IDC_RADIO6, dialogID);
		return 0;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO5:

			break;

		case IDOK:
			if (BST_CHECKED == IsDlgButtonChecked(hwndDlg, IDC_RADIO4))
			{
				GameManager::GetInstance()->SetGame(LEVEL_EASY);
				nowLevel = LEVEL_EASY;
				dialogID = IDC_RADIO4;
			}

			if (BST_CHECKED == IsDlgButtonChecked(hwndDlg, IDC_RADIO5))
			{
				GameManager::GetInstance()->SetGame(LEVEL_NORMAL);
				nowLevel = LEVEL_NORMAL;
				dialogID = IDC_RADIO5;
			}

			if (BST_CHECKED == IsDlgButtonChecked(hwndDlg, IDC_RADIO6))
			{
				GameManager::GetInstance()->SetGame(LEVEL_HARD);
				nowLevel = LEVEL_HARD;
				dialogID = IDC_RADIO6;
			}

			EndDialog(hwndDlg, TRUE);
			break;

		case IDCANCEL:

			EndDialog(hwndDlg, TRUE);
			break;

		default:
			break;
		}
		break;
		return 0;
	}

	return 0;
}