#include "Item.h"
#include "Image.h"
#include <random>
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include <random>

using namespace std;

HRESULT Item::Init()
{
	img = ImageManager::GetSingleton()->FindImage("Item");
	imgFrame[2];	// 탱크의 애니메이션이 2개의 프레임으로 구성됨
	size = 64;
	isCreate = false;
	isCollision = false;
	isSetPos = false;
	isTrigger = false;
	isShovel = 0;
	isSuper = false;
	alpha = 255;
	time = 0.0f;
	itemKind;
	isStop = false;
	star.tankUpdate = 1;

	return S_OK;
}

void Item::Release()
{

}

void Item::Update()
{
	Watch1();
	//if (!player1->GetLife())
	//{
	//	isSuper = false;
	//}

	//if (KeyManager::GetSingleton()->IsOnceKeyDown(KEY_N))
	//{
	//	//if (isShovel == 0) // 0 = idle, 1 = true, 2 = false
	//	//{
	//	//	isShovel = 1;
	//	//}
	//	//else if (isShovel == 1)
	//	//{
	//	//	isShovel = 2;
	//	//}
	//
	//	isTrigger = true;
	//	//isCreate = true;
	//	//isSetPos = true;
	//}	

	if (isSetPos)
	{



		// 의사난수가 아닌 실제 난수의 시드값을 지정
		random_device rdPos;
		random_device rdItem;



		// "Mersenne Twister" 알고리즘의 난수 생성 엔진
		mt19937 genPos(rdPos());
		mt19937 genItem(rdItem());



		// 일정 범위의 난수를 받기위해 균등 분포 객체를 정의
		uniform_int_distribution<int> disPos(0, 24);
		uniform_int_distribution<int> disItem(1, 6);



		// 균등 분포에 사용할 난수 엔진을 전달
		pos.x = disPos(genPos) * 32;
		pos.y = disPos(genPos) * 32;
		itemKind = 4;//disItem(genItem);




		isSetPos = false;
	}

	//if (isCollision)
	//{
	//	//itemKind = 3;
	//	Star();
	//	
	//	star.tankUpdate++;
	//	isCreate = false;
	//}

	if (isCollision)
	{
		switch (itemKind) {
		case WATCH:
			isStop = true;
			break;
		case SHOVEL:
			isShovel = 1;
			break;
		case STAR:
			if (star.tankUpdate < 8)
			{
				SetStarInfo(star.tankUpdate);
				star.tankUpdate++;
			}
			break;
		case GRENADE:
			isTrigger = true;
			break;
		//case TANK:

		//case GUN:
		//	break;
		}
		isCreate = false;
		//itemKind = NULL_KIND;
	}

}

void Item::Render(HDC hdc)
{
	if (isCreate)
	{
		img->AlphaRender(hdc, pos.x + WINSIZE_LEFT + 32, pos.y + WINSIZE_TOP + 32, alpha, itemKind - 1, 0);

		//HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));	// 새로운 펜 만들기
		//HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		//MoveToEx(hdc, pos.x + WINSIZE_LEFT + 32, pos.y + WINSIZE_LEFT - size / 2 + 32, NULL);
		//LineTo(hdc, pos.x + WINSIZE_LEFT + size + 32, pos.y + WINSIZE_LEFT - size / 2 + 32);
		//LineTo(hdc, pos.x + WINSIZE_LEFT + size + 32, pos.y + WINSIZE_LEFT + size / 2) + 32;
		//LineTo(hdc, pos.x + WINSIZE_LEFT + 32, pos.y + WINSIZE_LEFT + size / 2 + 32);
		//LineTo(hdc, pos.x + WINSIZE_LEFT + 32, pos.y + WINSIZE_LEFT - size / 2 + 32);
		//DeleteObject(SelectObject(hdc, hOldPen));
	}
}

void Item::Shovel()
{
}

void Item::SetStarInfo(int tankUpgrade)
{
	star.nextTank = 8 * tankUpgrade;
	star.missileCount = 1 + tankUpgrade;
	star.missileSpeed = 300.0f + 20.0f * tankUpgrade;
	star.damage = 1.0f;
	star.tankSpeed = 200.0f + 20.0f * tankUpgrade;
	if (tankUpgrade == 7)
	{
		isSuper = true;
	}
}

void Item::Watch1()
{
	/*if (KeyManager::GetSingleton()->IsOnceKeyDown(KEY_T))
	{
		if (!isStop)
		{
			isStop = true;
		}
		else if (isStop)
		{
			isStop = false;
		}
	}*/
	if (isStop)
	{
		time += TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (time >= 1.0f)
	{
		isStop = false;
		time = 0.0f;
	}
}

void Item::Grenade()
{
	//vector<Enemy*> vecEnemy = EnemyMgr->GetVecEnemys();
	//for (int i = 0; i < 5; i++)
	//{
	//	if (vecEnemy[i]->GetisSummon())
	//	{
	//		vecEnemy[i]->SetIsBoom(true);
	//	}
	//}
}

void Item::Tank()
{

}
