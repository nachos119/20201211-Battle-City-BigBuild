#pragma once
#include "pch.h"
#include "GameNode.h"

class Image;
class Missile : public GameNode
{
private:
	POINT pos;
	POINT direction;
	float size;
	float speed;
	float angle;
	float time;
	bool isFire;
	bool isBoom;
	int frame;
	int boomFrame;

	Image* img;
	Image* boomImg;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	bool GetIsFire() { return isFire; }
	void SetIsFire(bool fire) { isFire = fire; }

	void SetPos(POINT pos) { this->pos = pos; }
	POINT GetPos() { return this->pos; }

	int GetSize() { return this->size; }
	float GetAngle() { return angle; }

	void SetSpeed(float speed) { this->speed = speed; }

	void Fired(POINT pos, float angle, float speed);
	void AdditionalMissile();

	void ChangeCount() {}
};

