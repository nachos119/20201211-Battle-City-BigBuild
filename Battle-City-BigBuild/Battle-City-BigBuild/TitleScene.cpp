#include "TitleScene.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"
#include "Animation.h"

HRESULT TitleScene::Init()
{
	SetWindowSize(50, 50, WINSIZE_X, WINSIZE_Y);

	img = ImageManager::GetSingleton()->FindImage("TitleScene");
	PosY = 896;
	SelectImg = ImageManager::GetSingleton()->FindImage("Player");
	Selectpos.x = 315;
	Selectpos.y = 550;

	SelectAni = new Animation();
	SelectAni->Init(512, 512, size, size);
	imgFrame[FIRST_FRAME] = 14;			// ù��° �����ӿ� 6�� ��Ʈ�� ����
	imgFrame[SECOND_FRAME] = 15;			// �ι�° �����ӿ� 7�� ��Ʈ�� ����
	SelectAni->SetPlayFrame(imgFrame, 2, true, false);
	SelectAni->SetKeyFrameUpdateTime(0.001f);	// �ִϸ��̼� �ӵ�
	SelectAni->Start();


	/*BattileButton = new Button();
	BattileButton->Init("BattileMapToolButton", WINSIZE_X / 2 - 200, WINSIZE_Y - 200, { 0, 0 }, { 0, 1 });

	MapToolButton = new Button();
	MapToolButton->Init("BattileMapToolButton", WINSIZE_X / 2 + 200, WINSIZE_Y - 200, { 1, 0 }, { 1, 1 });
*/
/*Argument* arg = new Argument;
arg->a = string("BattleScene");

Argument* argTool = new Argument;
argTool->a = string("MapToolScene");

BattileButton->SetButtonFunc(ButtonFunction::ChangeScene, (void*)arg);
MapToolButton->SetButtonFunc(ButtonFunction::ChangeScene, (void*)argTool);
*/
	return S_OK;
}

void TitleScene::Release()
{
	SAFE_RELEASE(BattileButton);
	SAFE_RELEASE(MapToolButton);
}

void TitleScene::Update()
{
	// Ÿ�̸ӷ� ������ �Ѵ�.
	if (PosY != 0)
	{
		PosY--;
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
		{
			PosY = 0;
		}
	}
	else
	{
		start = true;
	}
	if (start)
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
		{
			Selectpos.y = 550;
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
		{				
			Selectpos.y = 615;
		}

		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
		{
			if (Selectpos.y == 550)
			{
				start = false;
				SceneManager::GetSingleton()->ChangeScene("LoadingScene");
			}
			else
			{
				start = false;
				SceneManager::GetSingleton()->ChangeScene("MapToolScene");
			}
		}

		/*if (BattileButton)
			BattileButton->Update();
		if (MapToolButton)
			MapToolButton->Update();*/
	}
	SelectAni->UpdateFrmae();					// �ٲ� �������� ����
}

void TitleScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y, BLACKNESS);
	img->Render(hdc, 0, PosY, WINSIZE_X, WINSIZE_Y);

	if (start)
		SelectImg->AnimationRender(hdc, Selectpos.x, Selectpos.y, SelectAni);

	/*if (BattileButton)
		BattileButton->Render(hdc);
	if (MapToolButton)
		MapToolButton->Render(hdc);*/
}
