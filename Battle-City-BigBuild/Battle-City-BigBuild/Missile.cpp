#include "Missile.h"
#include "Image.h"

HRESULT Missile::Init()
{
	size = 20;
	pos.x = 9000;
	pos.y = 9000;
	time = 0.0f;
	boomFrame = 0;
	speed = NULL;
	isFire = false;

	img = ImageManager::GetSingleton()->FindImage("Missile");
	boomImg = ImageManager::GetSingleton()->FindImage("MissileBoom");

	return S_OK;
}

void Missile::Release()
{
	SoundManager::GetSingleton()->Stop("Shout");
}

void Missile::Update()
{
	if (isFire)
	{
		boomFrame = 0;
	}
	else
	{
		isBoom = true;
	}
	if (isBoom)
	{
		time += TimerManager::GetSingleton()->GetTimeElapsed();
	}

	if (time >= 0.05)
	{
		boomFrame++;
		time = 0;
	}
	if (boomFrame == 3)
	{
		isBoom = false;
	}

	if (isFire)
	{
		if (pos.x < 70 || pos.x > GAME_WINSIZE + 70 || pos.y < 40 || pos.y > GAME_WINSIZE + 40)
			isFire = false;

		if (angle == STATE_UP)
		{
			pos.y -= speed * TimerManager::GetSingleton()->GetTimeElapsed();
			frame = 0;
		}
		else if (angle == STATE_DOWN)
		{
			pos.y += speed * TimerManager::GetSingleton()->GetTimeElapsed();
			frame = 1;
		}
		else if (angle == STATE_LEFT)
		{
			pos.x -= speed * TimerManager::GetSingleton()->GetTimeElapsed();
			frame = 2;
		}
		else if (angle == STATE_RIGHT)
		{
			pos.x += speed * TimerManager::GetSingleton()->GetTimeElapsed();
			frame = 3;
		}
	}


	AdditionalMissile();
}

void Missile::Render(HDC hdc)
{
	if (isFire)
	{
		//Rectangle(hdc, pos.x - (size / 2) - 2, pos.y - (size / 2) - 2, pos.x + (size / 2) - 2, pos.y + (size / 2) - 2);
		img->FrameRender(hdc, pos.x - 2, pos.y - 2, frame, 0);
	}
	else if (!isFire && boomFrame != 3)
	{
		boomImg->FrameRender(hdc, pos.x, pos.y, boomFrame, 0);
	}
}

void Missile::Fired(POINT pos, float angle, float speed)
{
	this->isFire = true;
	this->pos = pos;
	this->angle = angle;
	this->speed = speed;
	SoundManager::GetSingleton()->Play("Shout", 0.3f);
}

void Missile::AdditionalMissile()
{
}
