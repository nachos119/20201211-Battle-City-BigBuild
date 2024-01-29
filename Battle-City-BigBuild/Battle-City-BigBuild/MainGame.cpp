#include "MainGame.h"
#include "Image.h"
#include "TileMapToolScene.h"
#include "BattleScene.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "Item.h"
#include "EndingScene.h"

HRESULT MainGame::Init()
{
	ImageManager::GetSingleton()->Init();
	TimerManager::GetSingleton()->Init();
	KeyManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	SoundManager::GetSingleton()->Init();

	hdc = GetDC(g_hWnd);

	ImageManager::GetSingleton()->AddImage("BackGround", "Image/backGround.bmp", WINSIZE_X, WINSIZE_Y);
	ImageManager::GetSingleton()->AddImage("GameOver", "Image/badending.bmp", 1024, 896, 1, 1);
	ImageManager::GetSingleton()->AddImage("sEnemyBack", "Image/enemyBack.bmp", 60, 300, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("sEnemy", "Image/SEnemy.bmp", 30, 30, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Player", "Image/player.bmp", 512, 512, 8, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Player2", "Image/player2.bmp", 512, 512, 8, 8, true, RGB(255, 0, 255));		// <- 2P 플레이어
	ImageManager::GetSingleton()->AddImage("Enemy", "Image/enemy.bmp", 512, 512, 8, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Enemy2", "Image/enemy2.bmp", 512, 512, 8, 8, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Missile", "Image/missile.bmp", 80, 20, 4, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("sPlayer", "Image/SPlayer.bmp", 30, 30, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Stage", "Image/Flag.bmp", 64, 60, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Number", "Image/Number.bmp", 280, 28, 10, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("P", "Image/P.bmp", 28, 28, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("MissileBoom", "Image/boom.bmp", 192, 64, 3, 1, true, RGB(255, 0, 255));

	ImageManager::GetSingleton()->AddImage("Eagle", "Image/eagle.bmp", 128, 64, 2, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Helmet", "Image/helmet.bmp", 128, 64, 2, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("sGameover", "Image/gameover.bmp", 124, 60, 1, 1, true, RGB(255, 0, 255));

	// 씬
	ImageManager::GetSingleton()->AddImage("TitleScene", "Image/title.bmp", 1024, 896, 1, 1);
	ImageManager::GetSingleton()->AddImage("LoadingScene", "Image/load.bmp", 1024, 896, 1, 1);
	ImageManager::GetSingleton()->AddImage("StageE", "Image/stage.bmp", 156, 28, 1, 1, true, RGB(255, 0, 255));

	// UI
	ImageManager::GetSingleton()->AddImage("PAUSE", "Image/pause.bmp", 156, 28, 1, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Item", "Image/item.bmp", 448, 64, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("BattileMapToolButton", "Image/button2.bmp", 300, 140, 2, 2); 

	// 이펙트
	ImageManager::GetSingleton()->AddImage("Eagle Boom", "Image/eagle boom.bmp", 1152, 128, 9, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("TankBoom", "Image/tank boom.bmp", 896, 128, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Shovel", "Image/shovel.bmp", 128, 96, 1, 1, true, RGB(255, 0, 255));

	Item::GetSingleton()->Init();

	isInit = true;

	// 씬 추가
	SceneManager::GetSingleton()->AddScene("TitleScene", new TitleScene());
	SceneManager::GetSingleton()->AddScene("BattleScene", new BattleScene());
	SceneManager::GetSingleton()->AddScene("MapToolScene", new TileMapToolScene());
	SceneManager::GetSingleton()->AddScene("LoadingScene", new LoadingScene());
	SceneManager::GetSingleton()->AddScene("EnedingScene", new EndingScene());

	// 사운드 추가
	SoundManager::GetSingleton()->AddSound("Shout", "Sound/fire.mp3", false, false);
	SoundManager::GetSingleton()->AddSound("EnemyDie", "Sound/die.mp3", false, false);
	SoundManager::GetSingleton()->AddSound("EnemyHit", "Sound/EnemyHit.mp3", false, false);
	SoundManager::GetSingleton()->AddSound("blockHit", "Sound/blockHit.mp3", false, false);
	SoundManager::GetSingleton()->AddSound("GameStart", "Sound/GameStart.mp3", false, false);
	SoundManager::GetSingleton()->AddSound("itemeat", "Sound/itemeat.mp3", false, false);

	


	backBuffer = new Image();
	backBuffer->Init(max(WINSIZE_X, WINSIZE_TILE_MAP_X), max(WINSIZE_Y, WINSIZE_TILE_MAP_Y));

	backGround = ImageManager::GetSingleton()->FindImage("BackGround");
	

	SceneManager::GetSingleton()->ChangeScene("TitleScene");
	//SceneManager::GetSingleton()->ChangeScene("BattleScene");
	return S_OK;
}

void MainGame::Release()
{
	SAFE_RELEASE(backBuffer);

	Item::GetSingleton()->Release();
	SceneManager::GetSingleton()->Release();
	SoundManager::GetSingleton()->Release();
	ImageManager::GetSingleton()->Release();
	TimerManager::GetSingleton()->Release();
	KeyManager::GetSingleton()->Release();
	
	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();
	
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	HDC backDC = backBuffer->GetMemDC();
	backGround->Render(backDC, 0, 0, WINSIZE_X, WINSIZE_Y);

	SceneManager::GetSingleton()->Render(backDC);

	char szText[128];

	//wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	//TextOut(backDC, 10, 5, szText, strlen(szText));
	//
	//wsprintf(szText, "Clicked X : %d, Y : %d", mouseData.clickedPosX, mouseData.clickedPosY);
	//TextOut(backDC, 10, 30, szText, strlen(szText));
	//
	//wsprintf(szText, "g_time : %d", (int)g_time);
	//TextOut(backDC, WINSIZE_X - 300, 0, szText, strlen(szText));

	TimerManager::GetSingleton()->Render(backDC);

	// 백버퍼 복사(출력)
	backBuffer->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_WINDOWPOSCHANGED:
		if (isInit)
		{
			Render();
		}
		break;
	case WM_SYSCOMMAND:
		switch (wParam & 0xfff0)
		{
		case SC_MOVE:
		case SC_SIZE:
			TimerManager::GetSingleton()->SetIsSCE_MOVE(true);
			break;
		}
		break;  
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;

	case WM_LBUTTONDOWN:
		mouseData.clickedPosX = LOWORD(lParam);
		mouseData.clickedPosY = HIWORD(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
