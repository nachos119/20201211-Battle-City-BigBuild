#include "EnemyManager.h"
#include "Enemy.h"
#include "BattleScene.h"


HRESULT EnemyManager::Init()
{
	enemyMaxIdx = 20;
	enemyNowIdx = 0;
	enemySoonIdx = enemyMaxIdx;
	enemySummonIdx = 1;
	location = 1;
	pos[0] = { 96, 64 };
	pos[1] = { (WINSIZE_RIGHT / 2 + 32), 64 };
	pos[2] = { WINSIZE_RIGHT - 64, 64 };
	summonCheck = 0;
	vecEnemys.reserve(enemySummonMaxIdx);		// 벡터에 사이즈 50의 메모리를 할당
	for (int i = 0; i < 20; i++)
	{
		vecEnemys.push_back(new Enemy());			// 생성
		vecEnemys[i]->Init();
	}
	RandNum = 0;

	return S_OK;
}

void EnemyManager::Release()
{
	for (itEnemys = vecEnemys.begin(); itEnemys != vecEnemys.end(); itEnemys++)
	{
		(*itEnemys)->Release();
		delete (*itEnemys);
	}
	vecEnemys.clear();
}

void EnemyManager::Update()
{
	// 생존 확인
	/*if (KeyManager::GetSingleton()->IsOnceKeyDown(KEY_O))
	{
		vecEnemys[RandNum]->SetisLife(false);
		RandNum++;
	}*/
	for (itEnemys = vecEnemys.begin(); itEnemys != vecEnemys.end(); itEnemys++)
	{
		/*if ((*itEnemys)->GetisLife())
		{*/
			(*itEnemys)->Update();
		//}
	}

	summonCheck = 0;
	for (int i = 0; i < 20; i++)
	{
		if (vecEnemys[i]->GetisSummon())
		{
			summonCheck++;
		}
	}
}

void EnemyManager::Render(HDC hdc)
{
	for (itEnemys = vecEnemys.begin(); itEnemys != vecEnemys.end(); itEnemys++)
	{
		/*if ((*itEnemys)->GetisLife())
		{*/
			(*itEnemys)->Render(hdc);
		/*}*/
	}
}

void EnemyManager::AddEnemy(int EnemyCount)
{
	
	vecEnemys[EnemyCount]->SetPos(pos[location]);
	vecEnemys[EnemyCount]->SetisLife(true);
	vecEnemys[EnemyCount]->SetisSummon(true);
	if (EnemyCount != 0 && EnemyCount % 3 == 0)
	{
		vecEnemys[EnemyCount]->SetisItemE(true);
	}
	enemySoonIdx--;
	if (location >= 2)
	{
		location = 0;
	}
	else
	{
		location++;
	}
	
}

void EnemyManager::SetEnemyLife(bool isLife)
{

}
