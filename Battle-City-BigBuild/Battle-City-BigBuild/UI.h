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
	virtual HRESULT Init();				// ��� ���� �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();				// �޸� ����
	virtual void Update();				// ������ ���� ���� ���� ���� (������ ����)
	// Enemy ���� ���� ��
	virtual void Render(HDC hdc, int enemy, Player* player);		// ������ ���� ��� (�̹���, �ؽ�Ʈ ��)

};

