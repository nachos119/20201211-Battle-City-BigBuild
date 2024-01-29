#pragma once

#include <Windows.h>
#include <math.h>
#include <vector>
#include <map>
#include <string>
using namespace std;
#include "KeyManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "TimerManager.h"
#include "SoundManager.h"
#include "Item.h"
#include <ctime>

#define WINSIZE_X			1024
#define WINSIZE_Y			896
#define GAME_WINSIZE		832
#define WINSIZE_TILE_MAP_X	1200
#define WINSIZE_TILE_MAP_Y	900
#define WINSIZE_LEFT		32
#define WINSIZE_TOP			0
#define WINSIZE_RIGHT		928
#define WINSIZE_BOTTOM		896
#define PI					3.141592f

#define RADIAN(x)	(x * PI / 180.0f)
#define DEGREE(x)	(x * 180.0f / PI)
#define SAFE_DELETE(p) 		{if (p) delete p, p = nullptr; }
#define SAFE_ARR_DELETE(p) 	{if (p) delete[] p, p = nullptr; }
#define SAFE_RELEASE(p) 	{if (p) p->Release(), delete p, p = nullptr; }

#define KEY_A 0x41 
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4A
#define KEY_K 0x4B
#define KEY_L 0x4C
#define KEY_M 0x4D
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5A

enum ITEM {
	NULL_KIND,
	HELMET,
	WATCH,
	SHOVEL,
	STAR,
	GRENADE,
	TANK,
	GUN,
	ITEM_END
};

enum STATE {
	STATE_UP,
	STATE_LEFT = 2,
	STATE_DOWN = 4,	
	STATE_RIGHT = 6,
	STATE_END
};

enum TANK_FRAME {
	FIRST_FRAME,
	SECOND_FRAME,
	TANK_FRAME_END
};

enum class Allies {
	PLAYER,
	ENEMY,
	Allies_END
};

struct Argument {
	string a;
	string b;
};

struct Argument2 {
	void* arg[2];
};


extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern float g_time;
extern POINT g_ptMouse;

inline void SetWindowSize(int startX, int startY, int sizeX, int sizeY)
{
	// 윈도우 작업영역 지정
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = sizeX;
	rc.bottom = sizeY;

	// 실제 윈도우 크기 받아오기
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// 이동
	MoveWindow(g_hWnd, startX, startY, rc.right - rc.left, rc.bottom - rc.top, true);
}

template <typename T>
inline T Clamp(T value, T min, T max)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}