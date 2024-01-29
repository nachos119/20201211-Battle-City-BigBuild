#pragma once
#include "GameNode.h"

class Image;
class Player;
class UI : public GameNode
{
private:
	Image* sEnemy;
	POINT sEnemyPos;
	Image* sEnemyBack;
	POINT sEnemyBackPos;
	Image* sPlayer;
	POINT sPlayerPos;
	Image* stage;
	POINT stagePos;
	Image* num;
	POINT numPos;
	Image* PImg;
	POINT PPos;
	int hp;
	int* stage1;
public:
	virtual HRESULT Init();				// 멤버 변수 초기화, 메모리 할당
	virtual void Release();				// 메모리 해제
	virtual void Update();				// 프레임 단위 게임 로직 실행 (데이터 변경)
	// Enemy 나올 적의 수
	virtual void Render(HDC hdc, int enemy, Player* player);		// 프레임 단위 출력 (이미지, 텍스트 등)

};

