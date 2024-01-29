#pragma once
#include "Character.h"

class MissileManager;
class Image;
class Animation;
class Player : public Character
{
private:
	//STATE state;
	MissileManager* missileMgr;
	Image* helmetImg;
	Animation* helmetAni;
	bool isItemCollision;
	bool isHelmet;
	bool EnemyColP;
	float createTime;
	int maxMissileCount;
	float tankSpeedTemp;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void Fire();
	void ItemDrop();
	void Movement(int state, int sheet);

	void SetIsItem(bool isItemCollision) { this->isItemCollision = isItemCollision; }	// 아이템이 충돌 되었나?
	void SetItemStar(int missileCount, float missileSpeed, int nextTank, float damage) 
	{ 
		this->missileCount = missileCount;
		this->missileSpeed = missileSpeed;
		this->nextTank = nextTank;
		this->damage = damage;
	};

	void StartSet();
	int GetHp() { return this->hp; }
	void SetHp(int hp) { this->hp -= hp; }

	//void SetIsHelmet(bool isHelmet) { this->isHelmet = isHelmet; }
	bool GetIsHelmet() { return isHelmet; }
	void SetIsBoom(bool isBoom) { this->isBoom = isBoom; }
	void SetEnemyColP(bool col) { this->EnemyColP = col; }


	MissileManager* GetMissileMgr() { return missileMgr; }
};

