// main.cpp

/*
	Win32 API
	: Application Programming Interface
	�ü���� �������α׷� ������ ���� �����ϴ� �Լ��� ����

	�ϵ���� ----------�ü��(Windows)---------- �������α׷�
						 ( API �Լ� )
*/

#include <Windows.h>
#include "MainGame.h"

// ��������	
HINSTANCE	g_hInstance;		// ���α׷� �ν��Ͻ� �ڵ�
HWND		g_hWnd;				// ������ �ڵ�
LPSTR		g_lpszClass = (LPSTR)TEXT("Battle City");
MainGame	g_mainGame;
float		g_time;
POINT		g_ptMouse;

void MakeRectangle(HDC hdc, int x, int y, int size);
void MakeStar(HDC hdc, int x, int y, int ratio);

// ���� �׷��� �׸� �׸���.
void MakeRectangle(HDC hdc, int x, int y, int size)
{
	MoveToEx(hdc, x, y, NULL);		// Ư����ǥ�� �̵�
	LineTo(hdc, x + size, y);
	LineTo(hdc, x + size, y + size);
	LineTo(hdc, x, y + size);
	LineTo(hdc, x, y);
}

// ���� �׸��� �Լ�
void MakeStar(HDC hdc, int x, int y, int ratio)
{
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x + 10 * ratio, y);
	LineTo(hdc, x + 2 * ratio, y + 7 * ratio);
	LineTo(hdc, x + 5 * ratio, y - 4 * ratio);
	LineTo(hdc, x + 8 * ratio, y + 7 * ratio);
	LineTo(hdc, x, y);
}

// Ŭ���� ��ǥ�� �׸� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
//bool IsInRect(RECT rc, MOUSE_DATA mouseData)
//{
//	if (rc.left <= mouseData.clickedPosX && mouseData.clickedPosX <= rc.right
//		&& rc.top <= mouseData.clickedPosY && mouseData.clickedPosY <= rc.bottom)
//	{
//		return true;
//	}
//
//	return false;
//}

// ��Ʈ�� ��Ʈ�� �浹�� Ȯ���ϴ� �Լ�
bool CheckRectCollision(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left ||
		rc1.left > rc2.right ||
		rc1.top > rc2.bottom ||
		rc1.bottom < rc2.top)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam);

// �����Լ�
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance,
	LPSTR _lpszCmdParam, int nCmdShow)
{
	// �����츦 �����ϱ� ���� �⺻����
	g_hInstance = _hInstance;
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;	// Ŭ������ ���� �޸�
	wndClass.cbWndExtra = 0;	// �������� ���� �޸�
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ���
	RegisterClass(&wndClass);

	// ������ ����
	g_hWnd = CreateWindow(g_lpszClass, g_lpszClass,
		WS_OVERLAPPEDWINDOW,
		50, 50, WINSIZE_X, WINSIZE_Y,
		NULL, NULL, g_hInstance, NULL);

	// ������ ���
	ShowWindow(g_hWnd, nCmdShow);

	// ���� �۾� ������� ����
	SetWindowSize(0, 0, WINSIZE_X, WINSIZE_Y);

	g_mainGame.Init();

	// �޽��� ť�� �ִ� �޽��� ó��
	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) 
				break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TimerManager::GetSingleton()->Update(); 

			g_mainGame.Update();
			g_mainGame.Render();

			g_time += TimerManager::GetSingleton()->GetTimeElapsed();
		}
	}

	g_mainGame.Release();

	return message.wParam;
}

// �ݹ��Լ� : ������ �޽���(�̺�Ʈ)�� ó���ϴ� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, 
	WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, iMessage,
		wParam, lParam);
}
