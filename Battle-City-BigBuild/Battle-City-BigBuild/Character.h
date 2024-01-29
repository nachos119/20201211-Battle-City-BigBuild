#pragma once
#include "GameNode.h"

class Image;
class Animation;
class Character : public GameNode
{
protected:
	POINT pos;
	POINT missilePos;	// �̻����� ���ſ��� �߻�Ǵ� ������ ���� �ʿ�
	Image* img;
	Image* boomImg;
	Animation* ani;
	RECT rc;
	
	int imgFrame[2];	// ��ũ�� �ִϸ��̼��� 2���� ���������� ������
	int size = 64;
	float tankSpeed;
	float missileSpeed;
	bool isLife;
	bool isBoom;
	bool isTankUpgrade;
	int boomFrame;

	int state;
	int missileCount;
	int nextTank;		// �� ���� ���� ��ũ�� ������ �ٲ�
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

