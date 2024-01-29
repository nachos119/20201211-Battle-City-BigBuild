#include "Enemy.h"
#include "Image.h"
#include "Animation.h"
#include "MissileManager.h"
#include "Item.h"

HRESULT Enemy::Init()
{
	goal.x = WINSIZE_LEFT + (GAME_WINSIZE / 2);
	goal.y = WINSIZE_TOP + (GAME_WINSIZE / 2) - 32;
	pos.x = NULL;
	pos.y = NULL;
	tankSpeed = 200.0f;
	missileCount = 1;
	isLife = false;
	isBoom = false;
	hp = 1;
	tankUpgrade = 4;
	angle = 270.0f;
	isStop = false;
	Summon = false;
	ItemEnemy = false;
	ItemEnemyFrame = false;
	isTankUpgrade = false;
	state = STATE_DOWN;
	imgFrame[FIRST_FRAME] = 36;
	imgFrame[SECOND_FRAME] = 37;
	missileSpeed = 300.0f;
	missileMgr = new MissileManager();
	missileMgr->Init(10);

	img = ImageManager::GetSingleton()->FindImage("Enemy");
	img2 = ImageManager::GetSingleton()->FindImage("Enemy2");
	boomImg = ImageManager::GetSingleton()->FindImage("TankBoom");

	ani = new Animation();
	ani->Init(512, 512, size, size);
	ani->SetPlayFrame(imgFrame, 2, true, false);
	ani->SetKeyFrameUpdateTime(0.01f);
	ani->Start();

	srand(time(NULL));

	

	return S_OK;
}

void Enemy::Release()
{
	SAFE_DELETE(missileMgr);
	SAFE_DELETE(ani);

	
}

void Enemy::Update()
{
	missileMgr->Update();
	//if (!isBoom || isLife)
	//{
	//	missileMgr->Update();
	//}

	/*if (isTankUpgrade) // 이게 트루면 진화 함수 작동
	{*/
	if (tankUpgrade < 8)
		TankUpgrade(tankUpgrade);	// tankUpgrade의 숫자에 따라 진화하는 단계가 달라짐 0이면 진화 안함
	/*	isTankUpgrade = false;
	}*/

	if (!isLife)
	{		
		pos.x = NULL;
		pos.y = NULL;
		/*isLife = false;*/
			// 결론 - 이게 폴스면 다음게 나옴
		//if (ItemEnemy)
		//{

		//	ItemEnemy = false;
		//}
		
		Summon = false;	
		/*boomFrame = 0;*/
		/*createTime += TimerManager::GetSingleton()->GetTimeElapsed();*/
	}

	if (itemTrigger)
	{
		isBoom = true;
		Item::GetSingleton()->SetIsTrigger(false);
	}

	if (hp <= 0 && !isBoom)
	{
		isBoom = true;
			
		//pos.x = NULL;
		//pos.y = NULL;
		//isLife = false;
		//Summon = false;
		//if (ItemEnemy)
		//{
		//	// 아이템 소환
		//	ItemEnemy = false;
		//}

	}
	if (isLife && !isBoom)
	{
		missileMgr->Update();
		isStop = Item::GetSingleton()->Watch();
		if (isStop == false)
		{
			if (PlayerCol)
			{
				tankSpeed = 0;

				if (state == STATE_LEFT)
				{
					Movement(STATE_RIGHT, nextTank, 360.0f);
					pos.x += 1;
				}
				else if (state == STATE_RIGHT)
				{
					Movement(STATE_LEFT, nextTank, 180.0f);
					pos.x -= 1;
				}
				else if (state == STATE_UP)
				{
					Movement(STATE_DOWN, nextTank, 270.0f);
					pos.y += 1;
				}
				else if (state == STATE_DOWN)
				{
					Movement(STATE_UP, nextTank, 90.0f);
					pos.y -= 1;
				}
				PlayerCol = false;
			}
			if (isCollision)
			{
				tankSpeed = 0;

				if (state == STATE_LEFT)
				{
					randNum = rand() % 4;

					switch (randNum)
					{
					case 0:
						Movement(STATE_RIGHT, nextTank, 360.0f);
						pos.x += 1;
						break;
					case 1:
						Movement(STATE_UP, nextTank, 90.0f);
						pos.y -= 1;
						break;
					case 2:
					case 3:
						Movement(STATE_DOWN, nextTank, 270.0f);
						pos.y += 1;
						break;
					}
				}
				else if (state == STATE_RIGHT)
				{
					randNum = rand() % 4;

					switch (randNum)
					{
					case 0:
						Movement(STATE_LEFT, nextTank, 180.0f);
						pos.x -= 1;
						break;
					case 1:
						Movement(STATE_UP, nextTank, 90.0f);
						pos.y -= 1;
						break;
					case 2:
					case 3:
						Movement(STATE_DOWN, nextTank, 270.0f);
						pos.y += 1;
						break;
					}
				}
				else if (state == STATE_UP)
				{
					randNum = rand() % 4;

					switch (randNum)
					{
					case 0:
						Movement(STATE_LEFT, nextTank, 180.0f);
						pos.x -= 1;
						break;
					case 1:
						Movement(STATE_RIGHT, nextTank, 360.0f);
						pos.x += 1;
						break;
					case 2:
					case 3:
						Movement(STATE_DOWN, nextTank, 270.0f);
						pos.y += 1;
						break;
					}
				}
				else if (state == STATE_DOWN)
				{
					randNum = rand() % 3;

					switch (randNum)
					{
					case 0:
						Movement(STATE_LEFT, nextTank, 180.0f);
						pos.x -= 1;
						break;
					case 1:
						Movement(STATE_RIGHT, nextTank, 360.0f);
						pos.x += 1;
						break;
					case 2:
						Movement(STATE_UP, nextTank, 90.0f);
						pos.y -= 1;
						break;
					}
				}
				isCollision = false;
			}


			ani->UpdateFrmae();
			ani->SetPlayFrame(imgFrame, 2, true, false);

			pos.x += (int)(cosf(RADIAN(angle)) *tankSpeed * TimerManager::GetSingleton()->GetTimeElapsed());
			pos.y -= (int)(sinf(RADIAN(angle)) *tankSpeed * TimerManager::GetSingleton()->GetTimeElapsed());

			missilePos.x = pos.x;
			missilePos.y = pos.y;

			timer += TimerManager::GetSingleton()->GetTimeElapsed();
			if (timer >= 0.5f)
			{
				Fire();
				timer = 0;
			}

			if (!isCollision)
			{
				tankSpeed = 200;
			}

			if (ItemEnemy)	// 아이템 가진 에너미 애니메이션 프레임 변경
			{
				frameETimer += TimerManager::GetSingleton()->GetTimeElapsed();
				if (frameETimer >= 0.3f)
				{
					if (ItemEnemyFrame) ItemEnemyFrame = false;
					else if (!ItemEnemyFrame) ItemEnemyFrame = true;

					frameETimer = 0;
				}
			}
		}
	}
	if (isBoom && boomFrame != 8)
	{
		boomTimer += TimerManager::GetSingleton()->GetTimeElapsed();
	}

	if (boomTimer >= 0.05f)
	{
		boomFrame++;
		boomTimer = 0;
	}
	if (boomFrame == 8)
	{
		isBoom = false;
		isLife = false;
		if (ItemEnemy)
		{
			Item::GetSingleton()->SetIsItem(true);
			ItemEnemy = false;
		}
		/*boomFrame = 0;*/
		//createTime += TimerManager::GetSingleton()->GetTimeElapsed();
	}

	//if (!isLife)
	//{
	//	pos.x = NULL;
	//	pos.y = NULL;
	//	/*isLife = false;*/
	//	Summon = false;		// 결론 - 이게 폴스면 다음게 나옴
	//	if (ItemEnemy)
	//	{
	//		Item::GetSingleton()->SetIsCreate(true);
	//		ItemEnemy = false;
	//	}
	//	/*boomFrame = 0;*/
	//	/*createTime += TimerManager::GetSingleton()->GetTimeElapsed();*/
	//}
	//if (createTime >= 1.0f)
	//{
	//	hp--;

	//	//createTime = 0.0f;
	//}

}

void Enemy::Render(HDC hdc)
{
	if (isLife && !isBoom)
	{
		missileMgr->Render(hdc);
		if (ItemEnemy)
		{
			//Rectangle(hdc, pos.x - (size / 2), pos.y - (size / 2), pos.x + (size / 2), pos.y + (size / 2));
			if (ItemEnemyFrame)
			{
				img2->AnimationRender(hdc, pos.x, pos.y, ani);
			}
			else
			{
				img->AnimationRender(hdc, pos.x, pos.y, ani);
			}
		}
		else
		{
			//Rectangle(hdc, pos.x - (size / 2), pos.y - (size / 2), pos.x + (size / 2), pos.y + (size / 2));
			img->AnimationRender(hdc, pos.x, pos.y, ani);
		}
	}

	if (isBoom)
	{
		boomImg->FrameRender(hdc, pos.x, pos.y, boomFrame, 0);
	}

}

void Enemy::Fire()
{
	// 미사일 매니저에게 미사일이 발사될 위치와 탱크가 바라보는 방향을 전달
	missileMgr->AddMissile({ missilePos.x, missilePos.y }, state, missileSpeed, missileCount);
}

void Enemy::Movement(int state, int sheet, float angle)
{
	this->state = state;
	imgFrame[FIRST_FRAME] = sheet + state;			// 첫번째 프레임에 6번 시트를 대입
	imgFrame[SECOND_FRAME] = sheet + state + 1;			// 두번째 프레임에 7번 시트를 대입

	this->angle = angle;
}

void Enemy::TankUpgrade(int tankUpgrade)
{
	nextTank = 8 * tankUpgrade;						// 탱크 이미지 프레임 결정
	missileCount = 1 + tankUpgrade;					// 미사일 갯수
	missileSpeed = 300.0f + 20.0f * tankUpgrade;	// 미사일 속도
	damage = 1.0f;									// 의미는 없지만 일단 만든 데미지
	tankSpeed = 200.0f + 20.0f * tankUpgrade;		// 탱크 속도
}

void Enemy::StartSet()
{
	state = STATE_DOWN;
	nextTank = 0;
	missileCount = 1;
	missileSpeed = 280.0f;
	damage = 1.0f;
	tankSpeed = 100.0f;
	angle = 270.0f;
}