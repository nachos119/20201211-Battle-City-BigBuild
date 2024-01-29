#pragma once
#include "pch.h"
#include "GameNode.h"

class Player;
class Enemy;
class UI;
class TileMapToolScene;
class EnemyManager;
class Image;
class TitleScene;
class BattleScene : public GameNode
{
private:
	Image* OpenImg;
	Image* eagleImg; 
	Image* eagleBoomImg;
	Image* GameOver;
	Image* shovel;
	POINT GameOverPos;
	bool GameOverB;
	bool eagleBoomTrigger;
	int UpOpenY;
	int DownOpenY;
	int EnemyCount;
	int playEnemy;
	int eagleFrame;
	int eagleBoomFrame;
	bool sumE = false;
	Player* player1;
	Enemy* enemy1;
	UI* ui;
	TileMapToolScene* stage;
	EnemyManager* EnemyMgr;
	int* stage1;

	float sommonTimer;
	float eagleBoomTimer;
	float gameOverTimer;
	float shovelTimer;
	bool Gstart;
	bool onPlay = false;
	bool SceneEND = false;
	bool isGameOver = false;
	int x;
	int y;

	Image* grass;
	Image* water;
	float waterTimer;
	int waterFrame;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void StageLoad(int stageNum);

	// 충돌처리
	
	bool RectToRect(POINT pos1, int size1, POINT pos2, int size2);

	bool EnemyToPos(Enemy * enemy, POINT pos);
	bool EnemyToEnemy(Enemy * enemy1, Enemy * enemy2);

	bool MissileToMap(POINT pos, int size, TileMapToolScene * stage, int state);

	bool PlayerToMap(Player* player, TileMapToolScene* stage);
	bool EnemyToMap(POINT pos, int size, TileMapToolScene * stage, int state);
	int EnemyToMove(POINT pos, int size, TileMapToolScene * stage, int state);
	bool PlayerToItem(Player* player);
	bool MissileToEalge(POINT pos, int size);

	int GetTerrain(float col, float pos, TileMapToolScene * stage);

	int GoodMove(Player* player, TileMapToolScene* stage);

	BattleScene() {}
	virtual ~BattleScene() {}
};