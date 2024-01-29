#include "Player.h"
#include "Image.h"
#include "Animation.h"
#include "MissileManager.h"
#include "Item.h"

HRESULT Player::Init()
{
	StartSet();		// 탱크 초기 값 설정 (위치, 속도, 미사일...)

	maxMissileCount = 10;
	hp = 3;
	isItemCollision = false;
	isLife = true;
	isBoom = false;
	isHelmet = false;
	EnemyColP = false;

	img = ImageManager::GetSingleton()->FindImage("Player");
	helmetImg = ImageManager::GetSingleton()->FindImage("Helmet");
	boomImg = ImageManager::GetSingleton()->FindImage("TankBoom");
	ani = new Animation();
	helmetAni = new Animation();

	missileMgr = new MissileManager();
	missileMgr->Init(maxMissileCount);

	ani->Init(512, 512, size, size);
	ani->SetPlayFrame(imgFrame, 2, true, false);
	ani->SetKeyFrameUpdateTime(0.01f);	// 애니메이션 속도
	ani->Start();

	helmetAni->Init(128, 64, size, size);
	helmetAni->SetPlayFrame(0, 1, true, false);
	helmetAni->SetKeyFrameUpdateTime(0.01f);	// 애니메이션 속도
	helmetAni->Start();
	return S_OK;
}

void Player::Release()
{
	SAFE_DELETE(missileMgr);
	SAFE_DELETE(ani);
	SAFE_DELETE(helmetAni);
}

void Player::Update()
{
	missileMgr->Update();
	ItemDrop();

	//if (KeyManager::GetSingleton()->IsOnceKeyDown(KEY_Y))
	//{
	//	isHelmet = true;
	///*	if (isBoom)
	//		isBoom = false;
	//	else
	//		isBoom = true;*/
	//}

	if (isLife && !isBoom)
	{
		// 시작하자마지 누르면 터짐 확인
		if (KeyManager::GetSingleton()->IsOnceKeyDown(KEY_G))
		{
			Fire();
		}

		if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_W))
		{
			Movement(STATE_UP, nextTank);

			// 위로 가상 이동
			float tempPosY = pos.y;
			tempPosY -= (int)(tankSpeed * TimerManager::GetSingleton()->GetTimeElapsed());

			// 위쪽 블록과 충돌 확인
			if (isCollision)
			{
			}
			else
			{
				pos.y = tempPosY;
			}

			if (EnemyColP)
			{
				tankSpeedTemp = tankSpeed;
				pos.y += 3;
				EnemyColP = false;
			}

			missilePos.y = pos.y - size / 2;	// 미사일이 포신 끝에서 발사 되는 위치
			missilePos.x = pos.x;
		}
		else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_A))
		{
			Movement(STATE_LEFT, nextTank);

			float tempPosX = pos.x;
			tempPosX -= (int)(tankSpeed * TimerManager::GetSingleton()->GetTimeElapsed());

			// 위쪽 블록과 충돌 확인
			if (isCollision)
			{
			}
			else
			{
				pos.x = tempPosX;
			}

			if (EnemyColP)
			{
				tankSpeedTemp = tankSpeed;
				pos.x += 3;
				EnemyColP = false;
			}
			missilePos.y = pos.y;
			missilePos.x = pos.x - (size / 2);	// 미사일이 포신 끝에서 발사 되는 위치
		}
		else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_S))
		{
			Movement(STATE_DOWN, nextTank);

			float tempPosY = pos.y;
			tempPosY += (int)(tankSpeed * TimerManager::GetSingleton()->GetTimeElapsed());

			// 위쪽 블록과 충돌 확인
			if (isCollision)
			{
			}
			else
			{
				pos.y = tempPosY;
			}

			if (EnemyColP)
			{
				tankSpeedTemp = tankSpeed;
				pos.y -= 3;
				EnemyColP = false;
			}

			missilePos.y = pos.y + size / 2;	// 미사일이 포신 끝에서 발사 되는 위치
			missilePos.x = pos.x;
		}
		else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_D))
		{
			Movement(STATE_RIGHT, nextTank);

			float tempPosX = pos.x;
			tempPosX += (int)(tankSpeed * TimerManager::GetSingleton()->GetTimeElapsed());

			// 위쪽 블록과 충돌 확인
			if (isCollision)
			{
			}
			else
			{
				pos.x = tempPosX;
			}

			if (EnemyColP)
			{
				tankSpeedTemp = tankSpeed;
				pos.x -= 3;
				EnemyColP = false;
			}

			missilePos.y = pos.y;
			missilePos.x = pos.x + size / 2;	// 미사일이 포신 끝에서 발사 되는 위치
		}
	}
	//else
	//{
	//	isBoom = true;
	//}

	if (isHelmet)
	{
		helmetTimer += TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (helmetTimer >= 100.0f)
	{
		isHelmet = false;
		helmetTimer = 0.0f;
	}

	if (isBoom)
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
	}

	if (!isLife)
	{
		StartSet();

		boomFrame = 0;
		//missileMgr->Init(missileCount);
		Item::GetSingleton()->SetIsSpuerTank(false);
		Item::GetSingleton()->SetStarUPgrade(1);

		createTime += TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (createTime >= 0.05f)
	{
		hp--;
		isLife = true;
		imgFrame[FIRST_FRAME] = 0;
		imgFrame[SECOND_FRAME] = 1;

		createTime = 0.0f;
	}

	ani->SetPlayFrame(imgFrame, 2, true, false);
	helmetAni->UpdateFrmae();
	missileMgr->SetIsTankLife(isLife);
}

void Player::Render(HDC hdc)
{
	missileMgr->Render(hdc);

	if (isLife && !isBoom) {
		//Rectangle(hdc, pos.x - (size / 2), pos.y - (size / 2), pos.x + (size / 2), pos.y + (size / 2));
		img->AnimationRender(hdc, pos.x, pos.y, ani);	
	}
	if (isBoom)
	{
		boomImg->FrameRender(hdc, pos.x, pos.y, boomFrame, 0);
	}
	if (isHelmet)
	{
		helmetImg->AnimationRender(hdc, pos.x, pos.y, helmetAni);
	}
}

void Player::Fire()
{
	// 미사일 매니저에게 미사일이 발사될 위치와 탱크가 바라보는 방향을 전달

	missileMgr->AddMissile({ missilePos.x, missilePos.y }, state, missileSpeed, missileCount);
}

void Player::ItemDrop()
{

	if (isItemCollision)
	{
		switch (Item::GetSingleton()->GetKind()) {
		case HELMET:
			isHelmet = true;
			break;
		case STAR:
			nextTank = Item::GetSingleton()->GetStarInfo().nextTank;
			missileCount = Item::GetSingleton()->GetStarInfo().missileCount;
			missileSpeed = Item::GetSingleton()->GetStarInfo().missileSpeed;
			damage = Item::GetSingleton()->GetStarInfo().damage;
			tankSpeed = Item::GetSingleton()->GetStarInfo().tankSpeed;
			tankUpdate = Item::GetSingleton()->GetStarInfo().tankUpdate;
			break;
		case TANK:
			hp++;
			break;
		}
		//isItem = false;
	}
}

void Player::Movement(int state, int sheet)
{
	this->state = state;
	imgFrame[FIRST_FRAME] = sheet + state;			// 첫번째 프레임에 2번 시트를 대입
	imgFrame[SECOND_FRAME] = sheet + state + 1;			// 두번째 프레임에 3번 시트를 대입

	ani->UpdateFrmae();					// 바뀐 프레임을 갱신
}

void Player::StartSet()
{
	pos.x = 352.0f;
	pos.y = 832.0f;

	missilePos.y = pos.y - size / 2;
	missilePos.x = pos.x;

	state = STATE_UP;

	nextTank = 0;
	missileCount = 1;
	missileSpeed = 500.0f;
	damage = 1.0f;
	tankSpeed = 200.0f;
	tankUpdate = 0;
}

