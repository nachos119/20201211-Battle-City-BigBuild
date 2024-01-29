#include "EndingScene.h"
#include "Image.h"

HRESULT EndingScene::Init()
{
	img = ImageManager::GetSingleton()->FindImage("GameOver");

	return S_OK;
}

void EndingScene::Release()
{
}

void EndingScene::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
	{
		SceneManager::GetSingleton()->ChangeScene("TitleScene");
	}
}

void EndingScene::Render(HDC hdc)
{
	img->Render(hdc, 0, 0, WINSIZE_X, WINSIZE_Y);
}
