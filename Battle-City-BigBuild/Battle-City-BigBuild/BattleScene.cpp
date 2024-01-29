#include "BattleScene.h"
#include "Player.h"
#include "Enemy.h"
#include "MissileManager.h"
#include "Missile.h"
#include "Image.h"
#include "UI.h"
#include "TileMapToolScene.h"
#include "EnemyManager.h"
#include "TitleScene.h"


HRESULT BattleScene::Init()
{
	Gstart = false;

	OpenImg = ImageManager::GetSingleton()->FindImage("LoadingScene");
	eagleImg = ImageManager::GetSingleton()->FindImage("Eagle");
	eagleBoomImg = ImageManager::GetSingleton()->FindImage("Eagle Boom");
	GameOver = ImageManager::GetSingleton()->FindImage("sGameover");
	shovel = ImageManager::GetSingleton()->FindImage("Shovel");

	GameOverPos.x = WINSIZE_LEFT + (GAME_WINSIZE / 2) - 50;
	GameOverPos.y = WINSIZE_Y;

	GameOverB = false;

	UpOpenY = -(WINSIZE_Y / 2);
	DownOpenY = WINSIZE_Y / 2;

	EnemyMgr = new EnemyManager();
	EnemyMgr->Init();

	player1 = new Player();
	player1->Init();

	enemy1 = new Enemy();
	enemy1->Init();

	ui = new UI();
	ui->Init();

	stage = new TileMapToolScene();
	stage->Init();

	sommonTimer = 0;
	EnemyCount = 0;
	playEnemy = 0;
	eagleFrame = 0;
	eagleBoomFrame = 0;
	eagleBoomTimer = 0.0f;
	shovelTimer = 0.0f;

	x = 0;
	y = 0;
	waterTimer = 0;
	waterFrame = 1;

	grass = ImageManager::GetSingleton()->AddImage("Grass", "Image/Grass.bmp", 32, 32, 1, 1, true, RGB(255, 0, 255));
	water = ImageManager::GetSingleton()->AddImage("Water", "Image/Water.bmp", 96, 32, 3, 1, true, RGB(255, 0, 255));

	SoundManager::GetSingleton()->Play("GameStart", 0.3f);

	SetWindowSize(50, 50, WINSIZE_X, WINSIZE_Y);

	stage1 = SceneManager::GetSingleton()->GetStage1();

	StageLoad((*stage1));

	return S_OK;
}

void BattleScene::Release()
{
	SAFE_RELEASE(player1);
	SAFE_RELEASE(enemy1);
	SAFE_RELEASE(ui);
	SAFE_RELEASE(stage);
	SAFE_RELEASE(EnemyMgr);

	SoundManager::GetSingleton()->Stop("GameStart");
	/*MissileManager::GetSingleton()->Release();*/
}

void BattleScene::Update()
{
	POINT tempPos;
	if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_W) ||
		KeyManager::GetSingleton()->IsStayKeyDown(KEY_A) ||
		KeyManager::GetSingleton()->IsStayKeyDown(KEY_S) ||
		KeyManager::GetSingleton()->IsStayKeyDown(KEY_D))
	{
		if (PlayerToMap(player1, stage))
		{
			player1->SetIsCollision(true); // 플레이어 충돌처리
			// 예외처리
			if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_W) || KeyManager::GetSingleton()->IsStayKeyDown(KEY_S))
			{
				tempPos = player1->GetPos();
				tempPos.x -= GoodMove(player1, stage);
				player1->SetPos(tempPos);
			}
			else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_A) || KeyManager::GetSingleton()->IsStayKeyDown(KEY_D))
			{
				tempPos = player1->GetPos();
				tempPos.y -= GoodMove(player1, stage);
				player1->SetPos(tempPos);
			}
		}
		else
		{
			player1->SetIsCollision(false); 
		}

	}

	if (Gstart)
	{
		Item::GetSingleton()->Update();
		player1->Update();
		//enemy1->Update();
		EnemyMgr->Update();
		ui->Update();



		/*MissileToEalge(POINT pos, int size)*/


		if (EnemyMgr->GetEnemySoonIdx() > 0)
		{
			sommonTimer += TimerManager::GetSingleton()->GetTimeElapsed();
			if (sommonTimer > 2.0f)
			{
				if (EnemyMgr->GetEnemysummonCheck() < 5)
				{

					for (int i = 0; i < 20; i++)
					{
						if (EnemyMgr->GetVecEnemys()[i]->GetisSummon())
						{
							int location = EnemyMgr->Getlocation();
							POINT pt = EnemyMgr->GetOnPos(EnemyMgr->Getlocation());
							
							sumE =  RectToRect(EnemyMgr->GetVecEnemys()[i]->GetPos(), EnemyMgr->GetVecEnemys()[i]->GetSize(), EnemyMgr->GetOnPos(EnemyMgr->Getlocation()), 128);
							if (sumE)
							{
								if (EnemyMgr->Getlocation() >= 2)
								{
									EnemyMgr->Setlocation(0);
								}
								else
								{
									EnemyMgr->Setlocation(EnemyMgr->Getlocation() + 1);
								}
								
								sumE = false;

							}
						}
					}

					EnemyMgr->AddEnemy(EnemyCount);
					EnemyCount++;
					sommonTimer = 0;
					/*if (!onPlay)
						onPlay = true;*/
				}
			}
		}

		// 충돌체크

		MissileManager* playerMissileMgr = player1->GetMissileMgr();
		vector<Missile*> playerMissile;

		vector<Enemy*> vecEnemy = EnemyMgr->GetVecEnemys();
		vector<Enemy*> vecEnemy2 = EnemyMgr->GetVecEnemys();
		MissileManager* enemyMissileMgr;
		vector<Missile*> enemyMissile;
		bool isCollision = false;

		for (int i = 0; i < 20; i++) // 에네미 확인
		{
			if (vecEnemy[i]->GetisSummon())
			{
#pragma region 에네미 / 맵 충돌처리
				isCollision = EnemyToMap(vecEnemy[i]->GetPos(), vecEnemy[i]->GetSize(),
					stage, vecEnemy[i]->GetState());
				if (isCollision)
					vecEnemy[i]->SetIsCollision(isCollision);
#pragma endregion

#pragma region 에너미 / 독수리 충돌처리
				if (MissileToEalge(vecEnemy[i]->GetPos(), 64))
				{
					vecEnemy[i]->SetIsCollision(true);
				}
#pragma endregion

				if (Item::GetSingleton()->GetIsTrigger())
				{
					vecEnemy[i]->SetIsTrigger(true);
				}
				else
				{
					vecEnemy[i]->SetIsTrigger(false);
				}

#pragma region 에네미 / 에네미 충돌처리
				for (int l = i + 1; l < 20; l++)
				{
					if (vecEnemy2[l]->GetisSummon() && !vecEnemy[i]->GetBoom() && !vecEnemy2[l]->GetBoom())
					{
						isCollision = RectToRect(vecEnemy[i]->GetPos(), vecEnemy[i]->GetSize(), vecEnemy2[l]->GetPos(), vecEnemy2[l]->GetSize());
						if (isCollision)
						{
							vecEnemy[i]->SetCol2(isCollision);
							vecEnemy2[l]->SetCol2(isCollision);
						}
					}
				}
#pragma endregion


				enemyMissileMgr = vecEnemy[i]->GetMissileMgr();
				for (int k = 0; k < playerMissileMgr->GetMaxMissileCount(); k++)
				{
					playerMissile = playerMissileMgr->GetVecMissile();

					if (playerMissile[k]->GetIsFire()) // 플레이어 미사일 확인
					{
#pragma region 플레이어 미사일 / 맵 충돌처리

						isCollision = MissileToMap(playerMissileMgr->GetVecMissile()[k]->GetPos(),
							playerMissileMgr->GetVecMissile()[k]->GetSize(), stage, playerMissileMgr->GetVecMissile()[k]->GetAngle());

						playerMissileMgr->GetVecMissile()[k]->SetIsFire(!isCollision); // 플레이어 미사일과 맵 충돌처리
#pragma endregion

#pragma region 플레이어 미사일 / 적 충돌처리
						if (RectToRect(playerMissile[k]->GetPos(), playerMissile[k]->GetSize(), vecEnemy[i]->GetPos(), vecEnemy[i]->GetSize()))
						{
							if (!vecEnemy[i]->GetBoom())
							{
								playerMissile[k]->SetIsFire(false);
								vecEnemy[i]->SetIsHp(1);
							}
						}
#pragma endregion

#pragma region 플레이어 미사일 / 독수리 충돌처리
						if (MissileToEalge(playerMissile[k]->GetPos(), 20))
						{
							eagleBoomTrigger = true;
							playerMissile[k]->SetIsFire(false);
							eagleFrame = 1;
						}
#pragma endregion
					}
				}

#pragma region 플레이어 / 에네미 충돌처리
				if (RectToRect(player1->GetPos(), player1->GetSize(), vecEnemy[i]->GetPos(), vecEnemy[i]->GetSize())) // 플레이어 적 충돌처리
				{
					if (!EnemyMgr->GetVecEnemys()[i]->GetBoom() && !player1->GetBoom())
					{
						player1->SetEnemyColP(true);
						EnemyMgr->GetVecEnemys()[i]->SetCol2(true);
					}
				}
#pragma endregion

#pragma region 플레이어 / 독수리 충돌처리
				if (MissileToEalge(player1->GetPos(), 64))
				{
					player1->SetEnemyColP(true);
				}
#pragma endregion

				for (int j = 0; j < enemyMissileMgr->GetMaxMissileCount(); j++) // 에네미 총알 확인
				{
					enemyMissile = enemyMissileMgr->GetVecMissile();
#pragma region 에네미 미사일 / 맵 충돌처리
					if (enemyMissileMgr->GetVecMissile()[j]->GetIsFire())
					{
						isCollision = MissileToMap(enemyMissile[j]->GetPos(),
							enemyMissile[j]->GetSize(),
							stage,
							enemyMissile[j]->GetAngle());

						enemyMissile[j]->SetIsFire(!isCollision);

					}
#pragma endregion


#pragma region 에네미 미사일 / 플레이어 충돌처리
					if (RectToRect(enemyMissile[j]->GetPos(), enemyMissile[j]->GetSize(), player1->GetPos(), player1->GetSize()))
					{
						if (!player1->GetBoom())
						{
							enemyMissile[j]->SetIsFire(false);
							if (!player1->GetIsHelmet())
								player1->SetIsBoom(true);
						}
					}
#pragma endregion

#pragma region 에네미 미사일 / 독수리 충돌처리
					if (MissileToEalge(enemyMissile[j]->GetPos(), 20))
					{
						eagleBoomTrigger = true;
						enemyMissile[j]->SetIsFire(false);
						eagleFrame = 1;
					}
#pragma endregion
				}
			}
		}

	}
	else
	{
		UpOpenY--;
		DownOpenY++;
		if (DownOpenY > WINSIZE_Y)
		{
			Gstart = true;
		}
	}


	// 빈곳 체크
	/*for (int i = 0;    i < 20; i++)
	{
		if (EnemyMgr->GetVecEnemys()[i]->GetisSummon())
		{
			if (EnemyToMove(EnemyMgr->GetVecEnemys()[i]->GetPos(), EnemyMgr->GetVecEnemys()[i]->GetSize(),
				stage, EnemyMgr->GetVecEnemys()[i]->GetState()) > 2)
			{
				EnemyMgr->GetVecEnemys()[i]->SetIsCollision(true);

			}
		}
	}*/

	waterTimer += TimerManager::GetSingleton()->GetTimeElapsed();
	if (waterTimer >= 1.0f)
	{
		waterFrame++;
		if (waterFrame > 2)
		{
			waterFrame = 0;
		}
		waterTimer = 0;
	}

	

	player1->SetIsItem(PlayerToItem(player1));
	Item::GetSingleton()->SetIsCollision(PlayerToItem(player1));


	if (player1->GetHp() <= 0 || isGameOver)
	{
		GameOverB = true;
		isGameOver = false;
	}

	

	if (eagleBoomTrigger && eagleBoomFrame != 9)
	{
		eagleBoomTimer += TimerManager::GetSingleton()->GetTimeElapsed();
	}

	if (eagleBoomFrame == 9)
	{
		eagleBoomTrigger = false;
		isGameOver = true;
	}

	if (eagleBoomTimer >= 0.05f)
	{
		eagleBoomFrame++;
		eagleBoomTimer = 0;
	}

	if (Item::GetSingleton()->GetIsShovel())
	{
		shovelTimer += TimerManager::GetSingleton()->GetTimeElapsed();
	}
	if (shovelTimer >= 1.0f)
	{
		Item::GetSingleton()->SetIsShovel(false);
		shovelTimer = 0.0f;
	}

	int index[8] = { 684, 685, 686, 687, 712, 715, 740, 743 };
	int sho = Item::GetSingleton()->GetIsShovel();
	if (Item::GetSingleton()->GetIsShovel() == 1)
	{
		for (int i = 0; i < 8; i++)
		{
			stage->GetTileInfo()[index[i]].frameX = 0;
			stage->GetTileInfo()[index[i]].frameY = 1;
			stage->GetTileInfo()[index[i]].terrian = TERRAIN::CEMENT;
		}
	}
	else if (Item::GetSingleton()->GetIsShovel() == 2)
	{
		for (int i = 0; i < 8; i++)
		{
			stage->GetTileInfo()[index[i]].frameX = 0;
			stage->GetTileInfo()[index[i]].frameY = 0;
			stage->GetTileInfo()[index[i]].terrian = TERRAIN::WALL;
		}
		Item::GetSingleton()->SetIsShovel(0);
	}

	if (EnemyMgr->GetEnemySoonIdx() < 1)/* && onPlay)*/
	{
		if (EnemyMgr->GetEnemysummonCheck() < 1)
		{
			SceneManager::GetSingleton()->ChangeScene("LoadingScene");
		}
	}

	if (GameOverB)
	{
		GameOverPos.y -= TimerManager::GetSingleton()->GetTimeElapsed();
		/*gameOverTimer += TimerManager::GetSingleton()->GetTimeElapsed();*/

		if (GameOverPos.y <= (WINSIZE_Y / 2 - 30))
		{
			GameOverPos.y = WINSIZE_Y / 2 - 30;
			gameOverTimer += TimerManager::GetSingleton()->GetTimeElapsed();
		}
		if (gameOverTimer >= 1.5f)
		{
			(*stage1) = 0;
			SceneManager::GetSingleton()->ChangeScene("EnedingScene");
		}
		/*	if(SceneEND)
			{
				SceneManager::GetSingleton()->ChangeScene("EnedingScene");
			}*/
	}
}

void BattleScene::Render(HDC hdc)
{
	stage->StageRender(hdc);

	if (Gstart)
	{
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (stage->GetTileInfo()[i].terrian == TERRAIN::WATER)
			{
				water->FrameRender(hdc, stage->GetTileInfo()[i].rc.left + TILE_SIZE / 2, stage->GetTileInfo()[i].rc.top + TILE_SIZE / 2, waterFrame, 0);
			}
		}

		if (!GameOverB)
		{
			player1->Render(hdc);
		}

		//enemy1->Render(hdc);
		EnemyMgr->Render(hdc);

		Item::GetSingleton()->Render(hdc);
		ui->Render(hdc, EnemyMgr->GetEnemySoonIdx(), player1);

		eagleImg->FrameRender(hdc, 480, 832, eagleFrame, 0);
		if (eagleBoomTrigger)
		{
			eagleBoomImg->FrameRender(hdc, 480, 832, eagleBoomFrame, 0);
		}
		//if (Item::GetSingleton()->GetIsShovel())
		//shovel->FrameRender(hdc, 480, 816, 0, 0);
		

		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			if (stage->GetTileInfo()[i].terrian == TERRAIN::GRASS)
			{
				grass->Render(hdc, stage->GetTileInfo()[i].rc.left, stage->GetTileInfo()[i].rc.top, TILE_SIZE, TILE_SIZE);
			}
		}
	}
	else
	{
		OpenImg->Render(hdc, 0, DownOpenY, WINSIZE_X, WINSIZE_Y / 2);
		OpenImg->Render(hdc, 0, UpOpenY, WINSIZE_X, WINSIZE_Y / 2);
	}
	if (GameOverB)
	{
		GameOver->Render(hdc, GameOverPos.x, GameOverPos.y, 200, 100);
	}
	/*MissileManager::GetSingleton()->Render(hdc);*/

//	RECT rcPlayer = {(player1->GetPos().x - 64 / 2), (player1->GetPos().y - 64 / 2), (player1->GetPos().x + 64 / 2), (player1->GetPos().y + 64 / 2) };
////	Rectangle(hdc, rcPlayer.left, rcPlayer.top, rcPlayer.right, rcPlayer.bottom);
//
//	RECT rcItem = { (Item::GetSingleton()->GetPos().x + 64), (Item::GetSingleton()->GetPos().y - 64 / 2 + 64),
//		(Item::GetSingleton()->GetPos().x + 64 + 64), (Item::GetSingleton()->GetPos().y + 64 / 2 + 64) };
//	Rectangle(hdc, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);

	
	//Rectangle(hdc, EnemyMgr->GetOnPos(EnemyMgr->Getlocation()).x - 32, EnemyMgr->GetOnPos(EnemyMgr->Getlocation()).y - 32,
		//EnemyMgr->GetOnPos(EnemyMgr->Getlocation()).x + 32, EnemyMgr->GetOnPos(EnemyMgr->Getlocation()).y + 32);
}

void BattleScene::StageLoad(int stageNum)
{
	switch (stageNum)
	{
	case 1:
		if (stage) stage->LodeStage("Save/1.map");
		break;
	case 2:
		if (stage)stage->LodeStage("Save/2.map");
		break;
	case 3:
		if (stage)stage->LodeStage("Save/3.map");
		break;
	case 4:
		if (stage)stage->LodeStage("Save/4.map");
		break;
	case 5:
		if (stage)stage->LodeStage("Save/5.map");
		break;
	case 6:
		if (stage)stage->LodeStage("Save/6.map");
		break;
	case 7:
		if (stage)stage->LodeStage("Save/7.map");
		break;
	case 8:
		if (stage)stage->LodeStage("Save/8.map");
		break;
	case 9:
		if (stage)stage->LodeStage("Save/9.map");
		break;
	default:
		break;
	}
}

bool BattleScene::RectToRect(POINT pos1, int size1, POINT pos2, int size2)
{
	if (pos1.x - size1 / 2 < pos2.x + size2 / 2 &&
		pos1.y - size1 / 2 < pos2.y + size2 / 2 &&
		pos1.x + size1 / 2 > pos2.x - size2 / 2 &&
		pos1.y + size1 / 2 > pos2.y - size2 / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BattleScene::EnemyToPos(Enemy * enemy, POINT pos)
{
	POINT Enemy = enemy->GetPos();
	RECT rc;
	int EnemySize = enemy->GetSize();
	rc.left = pos.x - (EnemySize / 2);
	rc.top = pos.y - (EnemySize / 2);
	rc.right = pos.x + (EnemySize / 2);
	rc.bottom = pos.y + (EnemySize / 2);

	return false;
}

bool BattleScene::EnemyToEnemy(Enemy * enemy1, Enemy * enemy2)
{
	POINT Enemy1 = enemy1->GetPos();
	POINT Enemy2 = enemy2->GetPos();
	RECT rc;
	RECT rc1 = { Enemy1.x - 32, Enemy1.y - 32,Enemy1.x + 32, Enemy1.y + 32 };
	RECT rc2 = { Enemy2.x - 32, Enemy2.y - 32,Enemy2.x + 32, Enemy2.y + 32 };

	return	IntersectRect(&rc, &rc1, &rc2);

	// 반환받으면 1번째는 기준 2번째를 밀어내자
}

bool BattleScene::MissileToMap(POINT pos, int size, TileMapToolScene * stage, int state)
{
	switch (state)
	{
	case STATE::STATE_UP:
		for (int col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col++)
		{
			if (!(GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
				GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP, stage) == TERRAIN::GRASS ||
				GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP, stage) == TERRAIN::WATER))
			{
				stage->GetTileInfo()[x + (y*TILE_X)].isCollision = true;
				stage->ChangeTile(x, y, state);
				return true;
			}

		}
		break;
	case STATE::STATE_DOWN:
		for (int col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col++)
		{
			if (!(GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
				GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::GRASS ||
				GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::WATER))
			{
				stage->GetTileInfo()[x + (y*TILE_X)].isCollision = true;
				stage->ChangeTile(x, y, state);
				return true;
			}
		}
		break;
	case STATE::STATE_LEFT:
		for (int col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col++)
		{
			if (!(GetTerrain(pos.x - size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::EARTH ||
				GetTerrain(pos.x - size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::GRASS ||
				GetTerrain(pos.x - size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::WATER))
			{
				stage->GetTileInfo()[x + (y*TILE_X)].isCollision = true;
				stage->ChangeTile(x, y, state);
				return true;
			}
		}
		break;
	case STATE::STATE_RIGHT:
		for (int col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col++)
		{
			if (!(GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::EARTH ||
				GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::GRASS ||
				GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::WATER))
			{
				stage->GetTileInfo()[x + (y*TILE_X)].isCollision = true;
				stage->ChangeTile(x, y, state);
				return true;
			}
		}
	}
	return false;
}

bool BattleScene::PlayerToMap(Player * player, TileMapToolScene * stage)
{
	POINT pos = player->GetPos();
	int state = player->GetState();
	int size = player->GetSize();
	int col = 0;

	if (/*KeyManager::GetSingleton()->IsOnceKeyDown(KEY_W) ||*/ KeyManager::GetSingleton()->IsStayKeyDown(KEY_W))
	{
		for (col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col += TILE_SIZE - 1) // up
		{
			if (!(GetTerrain(col, pos.y - size / 2 - 1 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
				GetTerrain(col, pos.y - size / 2 - 1 - WINSIZE_TOP, stage) == TERRAIN::GRASS)) return true;

		}
	}
	else if (/*KeyManager::GetSingleton()->IsOnceKeyDown(KEY_A) ||*/ KeyManager::GetSingleton()->IsStayKeyDown(KEY_A))
	{
		for (col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col += TILE_SIZE - 1) // up
		{
			if (!(GetTerrain(pos.x - size / 2 - WINSIZE_LEFT - 1, col, stage) == TERRAIN::EARTH ||
				GetTerrain(pos.x - size / 2 - WINSIZE_LEFT - 1, col, stage) == TERRAIN::GRASS)) return true;
		}
	}
	else if (/*KeyManager::GetSingleton()->IsOnceKeyDown(KEY_S) ||*/ KeyManager::GetSingleton()->IsStayKeyDown(KEY_S))
	{
		for (col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col += TILE_SIZE - 1) // up
		{
			if (!(GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
				GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::GRASS)) return true;
		}
	}
	else if (/*KeyManager::GetSingleton()->IsOnceKeyDown(KEY_D) || */KeyManager::GetSingleton()->IsStayKeyDown(KEY_D))
	{
		for (col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col += TILE_SIZE - 1) // up
		{
			if (!(GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::EARTH ||
				GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::GRASS)) return true;
		}
	}

	return false;
}

bool BattleScene::EnemyToMap(POINT pos, int size, TileMapToolScene * stage, int state)
{
	switch (state)
	{
	case STATE::STATE_UP:
		for (int col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col++)
		{
			if (!(GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP - 1, stage) == TERRAIN::EARTH ||
				GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP - 1, stage) == TERRAIN::GRASS)) return true;
		}
		break;
	case STATE::STATE_DOWN:
		for (int col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col++)
		{
			if (!(GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
				GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::GRASS)) return true;
		}
		break;
	case STATE::STATE_LEFT:
		for (int col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col++)
		{
			if (!(GetTerrain(pos.x - size / 2 - WINSIZE_LEFT - 1, col, stage) == TERRAIN::EARTH ||
				GetTerrain(pos.x - size / 2 - WINSIZE_LEFT - 1, col, stage) == TERRAIN::GRASS)) return true;
		}
		break;
	case STATE::STATE_RIGHT:
		for (int col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col++)
		{
			if (!(GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::EARTH ||
				GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::GRASS)) return true;
		}
		break;
	}
	return false;

}

int BattleScene::EnemyToMove(POINT pos, int size, TileMapToolScene * stage, int state)
{

	int moveCheck = 0;
	// 위 검사
	for (int col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col++)
	{
		if (!(GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
			GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP, stage) == TERRAIN::GRASS ||
			GetTerrain(col, pos.y - size / 2 - WINSIZE_TOP, stage) == TERRAIN::WATER)) moveCheck++;
	}
	// 아래검사
	for (int col = pos.x - size / 2 - WINSIZE_LEFT; col < pos.x + size / 2 - WINSIZE_LEFT; col++)
	{
		if (!(GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::EARTH ||
			GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::GRASS ||
			GetTerrain(col, pos.y + size / 2 - WINSIZE_TOP, stage) == TERRAIN::WATER)) moveCheck++;
	}

	// 왼쪽검사
	for (int col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col++)
	{
		if (!(GetTerrain(pos.x - size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::EARTH ||
			GetTerrain(pos.x - size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::GRASS ||
			GetTerrain(pos.x - size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::WATER)) moveCheck++;
	}

	// 오른쪽검사
	for (int col = pos.y - size / 2 - WINSIZE_TOP; col < pos.y + size / 2 - WINSIZE_TOP; col++)
	{
		if (!(GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::EARTH ||
			GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::GRASS ||
			GetTerrain(pos.x + size / 2 - WINSIZE_LEFT, col, stage) == TERRAIN::WATER)) moveCheck++;
	}
	return moveCheck;
}

bool BattleScene::PlayerToItem(Player * player)
{
	POINT playerPos = player->GetPos();
	int state = player->GetState();
	int size = player->GetSize();
	int col = 0;
	RECT rcPlayer = { (playerPos.x - size / 2), (playerPos.y - size / 2), (playerPos.x + size / 2), (playerPos.y + size / 2) };
	RECT rcItem = { (Item::GetSingleton()->GetPos().x + 64), (Item::GetSingleton()->GetPos().y - 64 / 2 + 64),
		(Item::GetSingleton()->GetPos().x + 64 + 64), (Item::GetSingleton()->GetPos().y + 64 / 2 + 64) };
	RECT rcTemp;

	bool isItemDrop = Item::GetSingleton()->GetIsCreate();
	bool isCollision = false;

	if (isItemDrop && IntersectRect(&rcTemp, &rcPlayer, &rcItem))
	{
		isCollision = true;
	}
	else
	{
		isCollision = false;
	}

	return isCollision;
}

bool BattleScene::MissileToEalge(POINT pos, int size)
{
	RECT rcMissile = { (pos.x - size / 2), (pos.y - size / 2), (pos.x + size / 2), (pos.y + size / 2) };
	RECT rcEagle = { 448, 800, 512, 864 };
	RECT rcTemp;

	return IntersectRect(&rcTemp, &rcMissile, &rcEagle);
}

int BattleScene::GetTerrain(float col, float pos, TileMapToolScene * stage)
{
	int idx = col / TILE_SIZE; x = idx;
	int idy = pos / TILE_SIZE; y = idy;

	return stage->GetTileInfo()[idx + idy * TILE_X].terrian;
}

int BattleScene::GoodMove(Player * player, TileMapToolScene * stage)
{
	POINT pos = player->GetPos();
	int size = player->GetSize();

	int x = pos.x - size / 2  - WINSIZE_LEFT;
	int x2 = pos.x + size / 2 - WINSIZE_LEFT;
	int y = pos.y - size / 2  - WINSIZE_TOP;
	int y2 = pos.y + size / 2 - WINSIZE_TOP;

	int terrain, terrain2;
	int idx; int idy;

	int result = 0;
	
	if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_W)) // 위
	{
		y -= 1;

		terrain = GetTerrain(x, y, stage ); // 왼쪽 점
		terrain2 = GetTerrain(x2, y, stage ); // 오른쪽 점
	
		// 왼쪽 점만 충돌일 때
		if (!(terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS) && (terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS))
		{
			idx = x / TILE_SIZE;
			idy = y / TILE_SIZE;
			result = (pos.x - size / 2) - ((stage->GetTileInfo()[idx + idy * TILE_X].rc.right));
			
		}
		// 오른쪽 점만 충돌 일때
		else if (!(terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS) && (terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS))
		{
			idx = x2 / TILE_SIZE;
			idy = y / TILE_SIZE;
			result = (pos.x + size / 2) - (stage->GetTileInfo()[idx + idy * TILE_X].rc.left);
		}

		if (result < 15 && result > -15)
		{
			return result;
		}
		else
		{
			return 0;
		}
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_S)) // 아래
	{
		terrain = GetTerrain(x, y2, stage); // 왼쪽 점
		terrain2 = GetTerrain(x2, y2, stage); // 오른쪽 점

		// 왼쪽 점만 충돌일 때
		if (!(terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS) && (terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS))
		{
			idx = x / TILE_SIZE;
			idy = y2 / TILE_SIZE;
			result = (pos.x - size / 2) - ((stage->GetTileInfo()[idx + idy * TILE_X].rc.right));

		}
		// 오른쪽 점만 충돌 일때
		else if (!(terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS) && (terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS))
		{
			idx = x2 / TILE_SIZE;
			idy = y2 / TILE_SIZE;
			result = (pos.x + size / 2) - (stage->GetTileInfo()[idx + idy * TILE_X].rc.left);
		}

		if (result < 15 && result > -15)
		{
			return result;
		}
		else
		{
			return 0;
		}
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_A)) // 왼쪽
	{
		x -= 1;

		terrain = GetTerrain(x, y, stage); // 왼쪽 점
		terrain2 = GetTerrain(x, y2, stage); // 오른쪽 점

		// 왼쪽 점만 충돌일 때
		if (!(terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS) && (terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS))
		{
			idx = x / TILE_SIZE;
			idy = y / TILE_SIZE;
			result = (pos.y - size / 2) - ((stage->GetTileInfo()[idx + idy * TILE_X].rc.bottom));

		}
		// 오른쪽 점만 충돌 일때
		else if (!(terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS) && (terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS))
		{
			idx = x / TILE_SIZE;
			idy = y2 / TILE_SIZE;
			result = (pos.y + size / 2) - (stage->GetTileInfo()[idx + idy * TILE_X].rc.top);
		}

		if (result < 15 && result > -15)
		{
			return result;
		}
		else
		{
			return 0;
		}
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown(KEY_D)) // 오른쪽
	{
		terrain = GetTerrain(x2, y, stage); // 왼쪽 점
		terrain2 = GetTerrain(x2, y2, stage); // 오른쪽 점

		// 왼쪽 점만 충돌일 때
		if (!(terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS) && (terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS))
		{
			idx = x2 / TILE_SIZE;
			idy = y / TILE_SIZE;
			result = (pos.y - size / 2) - ((stage->GetTileInfo()[idx + idy * TILE_X].rc.bottom));

		}
		// 오른쪽 점만 충돌 일때
		else if (!(terrain2 == TERRAIN::EARTH || terrain2 == TERRAIN::GRASS) && (terrain == TERRAIN::EARTH || terrain == TERRAIN::GRASS))
		{
			idx = x2 / TILE_SIZE;
			idy = y2 / TILE_SIZE;
			result = (pos.y + size / 2) - (stage->GetTileInfo()[idx + idy * TILE_X].rc.top);
		}

		if (result < 15 && result > -15)
		{
			return result;
		}
		else
		{
			return 0;
		}
	}
	
	return 0;
}

