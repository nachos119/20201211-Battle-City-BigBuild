#pragma once
#include "GameNode.h"

class Image;
class Button;
class Animation;
class LoadingScene : public GameNode
{
private:
	// ≈∏¿Ã∆≤
	Image* img;
	Image* stage;
	Image* Number;

	POINT imgUpPos;
	POINT imgDownPos;
	POINT stagePos;
	POINT numPose;
	float checkTime;
	bool close = false;

	int* stage1;
public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;
};

