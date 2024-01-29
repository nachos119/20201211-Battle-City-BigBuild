#pragma once
#include "GameNode.h"

class Image;
class Animation;
class Character : public GameNode
{
protected:
	POINT pos;
	POINT missilePos;	// 미사일이 포신에서 발사되는 연출을 위해 필요
	Image* img;
	Image* boomImg;
	Animation* ani;
	RECT rc;
	
	int imgFrame[2];	// 탱크의 애니메이션이 2개의 프레임으로 구성됨
	int size = 64;
	float tankSpeed;
	float missileSpeed;
	bool isLife;
	bool isBoom;
	bool isTankUpgrade;
	int boomFrame;

	int state;
	int missileCount;
	int nextTank;		// 이 값에 따라 탱크의 종류가 바뀜
	float damage;
	int hp;
	int tankUpdate;
	float timer;
	float boomTimer;
	float helmetTimer = 0.0f;
	float createTime;
	float angle;

	bool isCollision = false;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	
	void fire(POINT tankPos);
	void Movement(int state, int sheet, float angle);
	void Movement(int state, int sheet);

	void SetIsBoom(bool isBoom) { this->isBoom = isBoom; }

	POINT GetPos() { return pos; }
	void SetPos(POINT pos) { this->pos = pos; }
	int GetSize() { return size; }
	void SetIsCollision(bool isCollision) { this->isCollision = isCollision; }
	int GetState() { return state; }
	bool GetLife() { return isLife; }
	bool GetBoom() { return isBoom; }

};

