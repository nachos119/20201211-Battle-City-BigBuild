#pragma once
#include "GameNode.h"

class Image;
class Button;
class Animation;
class EndingScene : public GameNode
{
private:
	// ≈∏¿Ã∆≤
	Image* img;

	
public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;
};
