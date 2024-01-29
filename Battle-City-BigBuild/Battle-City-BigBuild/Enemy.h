#pragma once
#include "Character.h"

class Image;
class MissileManager;
class Enemy : public Character
{
private:
	int num;
	int randNum;
	int tankUpgrade;

	float frameETimer;
	bool isStop;
	bool Summon;
	bool ItemEnemy;
	bool ItemEnemyFrame;
	bool PlayerCol = false;
	bool itemTrigger = false;
	Image* img2;
	POINT goal;
	MissileManager* missileMgr;
	// 애니 장수?
public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Fire();
	void Movement(int state, int sheet, float angle);
	void TankUpgrade(int tankUpgrade);
	void StartSet();

	void SetIsTrigger(bool trigger) { itemTrigger = trigger; }

	bool GetisLife() { return isLife; }
	void SetisLife(bool isLife) { this->isLife = isLife; }
	void SetisSummon(bool Summon) { this->Summon = Summon; }
	void SetPos(POINT poos) { this->pos.x = poos.x; this->pos.y = poos.y; }
	bool GetisSummon() { return this->Summon; }
	void SetIsHp(int hp) { this->hp -= hp; }
	void SetisItemE(bool ItemEnemy) { this->ItemEnemy = ItemEnemy; }
	void SetCol2(bool isCollision) { this->PlayerCol = isCollision; }

	MissileManager* GetMissileMgr() { return missileMgr; }
};

