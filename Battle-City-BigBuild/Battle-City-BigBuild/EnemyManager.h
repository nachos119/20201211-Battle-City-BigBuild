#pragma once
#include "GameNode.h"
#include <vector>

class Enemy;
class BattleScene;
class EnemyManager : public GameNode
{
private:
	vector<Enemy*> vecEnemys;				// ���Ҹ� ��� �ڷᱸ��
	vector<Enemy*>::iterator itEnemys;		// ���� �ϳ��ϳ��� ����Ű�� ������

	int enemyMaxIdx;						// �� �ִ� ��
	int enemySoonIdx;						// ���� ���� ��
	int enemyNowIdx;						// �ʿ��ִ� ���� ��
	int enemySummonIdx;						// ��ȯ�ϴ� ���� ��
	int enemySummonMaxIdx = 20;				// ��ȯ�Ҽ��ִ� �ִ� ���� ��
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

