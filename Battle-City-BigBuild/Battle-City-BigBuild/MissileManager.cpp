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
	// �̻����� "maxMissileCount"��ŭ �߻��� �� ����
	// vecMissiles[]�� �̻����� �̻��Ͽ��� �̻����� �߻�� ��ġ�� ��ũ�� �ٶ󺸴� ������ ����
	//vecMissiles[count1]->Fired(pos, angle);

	if (!vecMissiles[count1]->GetIsFire())
	{
		vecMissiles[count1]->Fired(pos, angle, speed);
		// ���� �̻����� �߻��ϱ� ����
		count1++;

		if (count1 >= missileCount)
		{
			count1 = 0;
		}
		// �̻����� �� �߻�Ǹ� ù��° �̻��� ���� �ٽ� �߻���	
	}
}
