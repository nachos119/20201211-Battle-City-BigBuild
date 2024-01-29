#include "UI.h"
#include "Image.h"
#include "Player.h"

HRESULT UI::Init()
{
	sEnemyBack = ImageManager::GetSingleton()->FindImage("sEnemyBack");
	sEnemyBackPos.x = WINSIZE_RIGHT;
	sEnemyBackPos.y = 65;

	sEnemy = ImageManager::GetSingleton()->FindImage("sEnemy");
	sEnemyPos.x = WINSIZE_RIGHT;
	sEnemyPos.y = 65;

	sPlayer = ImageManager::GetSingleton()->FindImage("sPlayer");
	sPlayerPos.x = WINSIZE_RIGHT;
	sPlayerPos.y = WINSIZE_Y / 2 + 28 + 38;

	stage = ImageManager::GetSingleton()->FindImage("Stage");
	stagePos.x = WINSIZE_RIGHT
		;
	stagePos.y = WINSIZE_Y - 200;

	num = ImageManager::GetSingleton()->FindImage("Number");
	numPos.x = WINSIZE_RIGHT + 30 + 14;
	numPos.y = sPlayerPos.y + 14;

	PImg = ImageManager::GetSingleton()->FindImage("P");
	PPos.x = WINSIZE_RIGHT + 30;
	PPos.y = WINSIZE_Y / 2 + 34;


	stage1 = SceneManager::GetSingleton()->GetStage1();

	return E_NOTIMPL;
}

void UI::Release()
{
}

void UI::Update()
{
}

void UI::Render(HDC hdc, int enemy, Player* player)
{
	sEnemyBack->Render(hdc, sEnemyBackPos.x, sEnemyBackPos.y, 60, 300);

	// 나올 적의 수

	for (int i = 0; i < enemy; i++)
	{
		sEnemy->Render(hdc, sEnemyPos.x, sEnemyPos.y, 30, 30);
		if (i % 2 == 0)
		{
			sEnemyPos.x += 30;
		}
		else
		{
			sEnemyPos.x -= 30;
			sEnemyPos.y += 30;
		}
	}
	sEnemyPos.x = WINSIZE_RIGHT;
	sEnemyPos.y = 65;


	// 스테이지 깃발
	stage->Render(hdc, stagePos.x, stagePos.y, 70, 70);
	num->FrameRender(hdc, stagePos.x + 49, stagePos.y + 84, (*stage1), 0);


	// 1P
	num->FrameRender(hdc, PPos.x - 16, PPos.y + 14, 1, 0);
	PImg->Render(hdc, PPos.x, PPos.y, 28, 28);
	sPlayer->Render(hdc, sPlayerPos.x, sPlayerPos.y, 28, 28);
	num->FrameRender(hdc, numPos.x, numPos.y, player->GetHp(), 0);

	//// 2P
	//num->FrameRender(hdc, PPos.x - 16, PPos.y + 90, 2, 0);
	//PImg->Render(hdc, PPos.x, PPos.y + 76, 28, 28);
	//sPlayer->Render(hdc, sPlayerPos.x, sPlayerPos.y+76, 28, 28);
	//num->FrameRender(hdc, numPos.x, numPos.y+76, 1, 0);
}
