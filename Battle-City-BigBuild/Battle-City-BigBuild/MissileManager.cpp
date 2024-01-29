#include "MissileManager.h"
#include "Missile.h"
#include "Image.h"

HRESULT MissileManager::Init(int missileCount)
{
	vecMissiles.clear();
	currentMissileCount = 0;

	isTankUpdate = false;
	isTankLife = true;

	maxMissileCount = missileCount;
	vecMissiles.reserve(maxMissileCount);
	for (int i = 0; i < maxMissileCount; i++)
	{
		vecMissiles.push_back(new Missile());
		vecMissiles[i]->Init();
	}

	return S_OK;
}

void MissileManager::Release()
{
	for (itMissiles = vecMissiles.begin(); itMissiles != vecMissiles.end(); itMissiles++)
	{
		(*itMissiles)->Release();
		delete (*itMissiles);
	}
	vecMissiles.clear();
	//ReleaseSingleton();
}

void MissileManager::Update()
{
	for (itMissiles = vecMissiles.begin(); itMissiles != vecMissiles.end(); itMissiles++)
	{
		(*itMissiles)->Update();
	}

	//if (!isTankLife)
	//{
	//	vecMissiles[count1]->SetIsFire(false);
	//}

	//if (isTankUpdate)
	//{
	//	count1++;
	//}
}

void MissileManager::Render(HDC hdc)
{
	for (itMissiles = vecMissiles.begin(); itMissiles != vecMissiles.end(); itMissiles++)
	{
		(*itMissiles)->Render(hdc);
	}
}

int MissileManager::GetMissileCount()
{
	count1 = currentMissileCount++;
	if (currentMissileCount > maxMissileCount - 1)
		currentMissileCount = 0;
	return 0;
}

void MissileManager::AddMissile(POINT pos, float angle, float speed, int missileCount)
{
	// 미사일을 "maxMissileCount"만큼 발사할 수 있음
	// vecMissiles[]가 미사일임 미사일에게 미사일이 발사될 위치와 탱크가 바라보는 방향을 전달
	//vecMissiles[count1]->Fired(pos, angle);

	if (!vecMissiles[count1]->GetIsFire())
	{
		vecMissiles[count1]->Fired(pos, angle, speed);
		// 다음 미사일을 발사하기 위해
		count1++;

		if (count1 >= missileCount)
		{
			count1 = 0;
		}
		// 미사일이 다 발사되면 첫번째 미사일 부터 다시 발사함	
	}
}
