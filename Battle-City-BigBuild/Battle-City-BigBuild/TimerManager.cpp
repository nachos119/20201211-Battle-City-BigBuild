#include "TimerManager.h"
#include "Timer.h"

HRESULT TimerManager::Init()
{
	timer = new Timer();
	timer->Init();

	return S_OK;
}

void TimerManager::Release()
{
	delete timer;
	ReleaseSingleton();
}

void TimerManager::Update()
{
	timer->Tick();
}

void TimerManager::Render(HDC hdc)
{
	wsprintf(szText, "FPS : %d", timer->GetFPS());
	TextOut(hdc, WINSIZE_X - 100, 20, szText, strlen(szText));
}

void TimerManager::SetIsSCE_MOVE(bool isSC_MOVE)
{
	timer->SetIsSC_MOVE(isSC_MOVE);
}

float TimerManager::GetTimeElapsed()
{
	return timer->GetTimeElapsed();
}
