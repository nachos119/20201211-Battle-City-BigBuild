#pragma once
#include "pch.h"
#include "Singleton.h"

class GameNode;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, GameNode*> mapSceneData;
	map<string, GameNode*> mapLoadingSceneData;
	int* stage1;
public:
	static GameNode* currScene;		// ���� ��� ���� ��
	static GameNode* loadingScene;	// ���� �� �غ� �߿� ��µ� ��
	static GameNode* readyScene;	// �غ� ���� ��

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	int* GetStage1() { return stage1; }
	// �� �߰�
	GameNode* AddScene(string key, GameNode* scene);
	GameNode* AddLoadingScene(string key, GameNode* scene);

	// �� ����
	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

	virtual ~SceneManager() {};
};


