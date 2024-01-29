#pragma once
#include "pch.h"
#include "Singleton.h"

class Image;
class Player;
class Enemy;
class EnemyManager;
class Item : public Singleton<Item>
{
private:

	struct StarInfo {
		int missileCount = 1;
		float missileSpeed = 450.0f;
		int nextTank = 0;
		float damage = 1.0f;
		float tankSpeed = 200.0f;
		int tankUpdate;
	};

	Image* img;
	Player* player1;
	Enemy* enemy;
	POINT pos;
	EnemyManager* EnemyMgr;

	StarInfo star;

	int imgFrame[2];	// 탱크의 애니메이션이 2개의 프레임으로 구성됨
	int size;
	bool isCreate;
	bool isCollision;
	bool isSetPos;
	bool isStop;
	bool isTrigger;
	int isShovel;
	bool isSuper;
	int alpha;
	int itemKind;
	float time;
	int tankUpdate;
	int tankUpgrade;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	int GetKind() { return itemKind; }
	POINT GetPos() { return pos; }
	bool GetIsCreate() { return isCreate; }

	void Helmet();
	bool Watch() { return isStop; }
	void Watch1();
	void Shovel();
	void SetStarInfo(int tankUpgrade);
	void SetStarUPgrade(int tankUpgrade) { star.tankUpdate = tankUpgrade; }
	void SetIsItem(bool isItem) 
	{ 
		isCreate = isItem;
		isSetPos = isItem;
	}
	StarInfo GetStarInfo() { return star; }
	void Grenade();
	void Tank();

	bool GetIsTrigger() { return isTrigger; }

	int GetIsShovel() { return isShovel; }
	void SetIsShovel(int isShovel) { this->isShovel = isShovel; }
	bool GetIsSpuerTank() { return isSuper; }

	void SetIsSpuerTank(bool isSuper) { this->isSuper = isSuper; }
	void SetIsTrigger(bool trigger) { isTrigger = trigger; }
	void SetIsCollision(bool isCollision) { this->isCollision = isCollision; }
	/*void SetCreate(bool isCreate) { this->isCreate = isCreate; }*/

	Item() :pos({0,0}) {}

};

