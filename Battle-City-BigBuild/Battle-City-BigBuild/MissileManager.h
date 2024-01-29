#pragma once
#include "GameNode.h"
#include <vector>

class Missile;
enum class Allies;
class MissileManager : public GameNode
{
private:
	vector<Missile*> vecMissiles;
	vector<Missile*> ::iterator itMissiles;
	int maxMissileCount;
	int currentMissileCount;
	int count1;
	bool isTankLife;
	bool isTankUpdate;

public:
	HRESULT Init(int missileCount);
	void Release();
	void Update();
	void Render(HDC hdc);

	int GetMissileCount();
	int GetMaxMissileCount() { return maxMissileCount; }

	void AddMissile(POINT pos, float angle, float speed, int missileCount);

	void ChangeCount(int count) { maxMissileCount = count; }

	void SetIsTankLife(bool isTankLife) { this->isTankLife = isTankLife; };
	void SetIsTankUpdate(bool isTankUpdate) { this->isTankUpdate = isTankUpdate; };

	vector<Missile*> GetVecMissile() { return vecMissiles; }
};
