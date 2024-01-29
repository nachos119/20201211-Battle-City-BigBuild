#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	SetWindowSize(50, 50, WINSIZE_X, WINSIZE_Y);

	img = ImageManager::GetSingleton()->FindImage("LoadingScene");

	imgUpPos.x = 0;
	imgUpPos.y = WINSIZE_Y;
	imgDownPos.x = 0;
	imgDownPos.y = -WINSIZE_Y;

	stage = ImageManager::GetSingleton()->FindImage("StageE");
	stagePos.x = (WINSIZE_X / 2) - (156 / 2) - 35;
	stagePos.y = (WINSIZE_Y / 2) - 24;

	Number = ImageManager::GetSingleton()->FindImage("Number");
	numPose.x = stagePos.x + 191;
	numPose.y = (WINSIZE_Y / 2) - 10;

	stage1 = SceneManager::GetSingleton()->GetStage1();
	(*stage1)++;
	return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
	if (imgDownPos.y < -(WINSIZE_Y / 2))
	{
		imgDownPos.y++;
	}
	if (imgUpPos.y > (WINSIZE_Y / 2))
	{
		imgUpPos.y--;
	}
	else
	{
		// 타이머 체크
		close = true;
	}

	if (close)
	{
		checkTime += TimerManager::GetSingleton()->GetTimeElapsed();
		if (checkTime >= 1.0f)
		{
			close = false;
			checkTime = 0;
			SceneManager::GetSingleton()->ChangeScene("BattleScene");
		}
	}


}

void LoadingScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);
	img->Render(hdc, imgDownPos.x, imgDownPos.y, WINSIZE_X, WINSIZE_Y);
	img->Render(hdc, imgUpPos.x, imgUpPos.y, WINSIZE_X, WINSIZE_Y);

	if (close)
	{
		stage->Render(hdc, stagePos.x, stagePos.y, 156, 28);
		Number->FrameRender(hdc, numPose.x, numPose.y, ((*stage1)), 0);
	}
}
