#pragma once
#include "GameNode.h"
#include <vector>

class Enemy;
class BattleScene;
class EnemyManager : public GameNode
{
private:
	vector<Enemy*> vecEnemys;				// 원소를 담는 자료구조
	vector<Enemy*>::iterator itEnemys;		// 원소 하나하나를 가리키는 포인터

	int enemyMaxIdx;						// 적 최대 수
	int enemySoonIdx;						// 남은 적의 수
	int enemyNowIdx;						// 맵에있는 적의 수
	int enemySummonIdx;						// 소환하는 적의 수
	int enemySummonMaxIdx = 20;				// 소환할수있는 최대 적의 수
	int summonCheck;
	int RandNum;
	int location;

	POINT pos[3];
	BattleScene* battleScene;


public:
	HRESULT Init();						
	void Release();						
	void Update(/*int level*/);			
	void Render(HDC hdc);
	int GetEnemySoonIdx() { return this->enemySoonIdx; }
	void SetEnemySummonIdx() { this->enemySummonIdx++; }
	int GetNowEnemy() { return this->enemyNowIdx; }
	int GetMaxIdx() { return this->enemyMaxIdx; }
	POINT GetOnPos(int i) { return pos[i]; }
	int Getlocation() { return location; }
	void AddEnemy(int EnemyCount);
	void SetEnemyLife(bool isLife);
	int GetEnemyMaxIdx() { return enemyMaxIdx; }
	int GetEnemysummonCheck() { return summonCheck; }
	void Setlocation(int location) { this->location = location; }

	vector<Enemy*> GetVecEnemys() { return vecEnemys; }
};

