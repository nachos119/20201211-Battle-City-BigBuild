#pragma once
#include "GameNode.h"

class Image;
class Button;
class Animation;
class TitleScene : public GameNode
{
private:
	// Ÿ��Ʋ
	Image* img;
	// ���°� ����
	Image* SelectImg;
	POINT Selectpos;
	Animation* SelectAni;
	int imgFrame[2];	// ��ũ�� �ִϸ��̼��� 2���� ���������� ������
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