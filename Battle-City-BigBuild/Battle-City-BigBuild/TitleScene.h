#pragma once
#include "GameNode.h"

class Image;
class Button;
class Animation;
class TitleScene : public GameNode
{
private:
	// 타이틀
	Image* img;
	// 고르는거 관련
	Image* SelectImg;
	POINT Selectpos;
	Animation* SelectAni;
	int imgFrame[2];	// 탱크의 애니메이션이 2개의 프레임으로 구성됨
	int size = 64;
	bool start = false;

	Button* BattileButton;
	Button* MapToolButton;
	int PosY;

public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	TitleScene() {};
	virtual ~TitleScene() {};
};