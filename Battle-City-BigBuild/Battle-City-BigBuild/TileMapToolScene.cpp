#include "TileMapToolScene.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"

HRESULT TileMapToolScene::Init()
{
	SetWindowSize(50, 50, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y);

	// 이미지 등록
	sampleTile = ImageManager::GetSingleton()->AddImage("SampleTile", "Image/maptiles.bmp", 160, 96, SAMPLE_TILE_X, SAMPLE_TILE_Y);
	ImageManager::GetSingleton()->AddImage("SaveLoadButton", "Image/button1.bmp", 300, 140, 2, 2);

	// 세이브, 로드 버튼
	saveButton = new Button();
	saveButton->Init("SaveLoadButton", WINSIZE_TILE_MAP_X - sampleTile->GetWidth(), sampleTile->GetHeight() + 400, {0, 0}, {0, 1});
	saveButton->SetButtonFunc(ButtonFunction::TileInfoSave, tileInfo);

	loadButton = new Button();
	loadButton->Init("SaveLoadButton", WINSIZE_TILE_MAP_X - sampleTile->GetWidth(), sampleTile->GetHeight() + 500, { 1, 0 }, { 1, 1 });
	loadButton->SetButtonFunc(ButtonFunction::TileInfoLoad, tileInfo);

	// 셀렉트 영역 초기화
	selectTileInfo.frame.left = 0;
	selectTileInfo.frame.right = 0;
	selectTileInfo.frame.top = 0;
	selectTileInfo.frame.bottom = 0;

	// 오른쪽 상단에 샘플 타일의 정보를 세팅
	for (int i = 0; i < SAMPLE_TILE_Y; i++)
	{
		for (int j = 0; j < SAMPLE_TILE_X; j++)
		{
			if (i == 0 && j == 1)
			{
				SetRect(&sampleTileInfo[i * SAMPLE_TILE_X + j].rc,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE + TILE_SIZE / 2),
					i * TILE_SIZE,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE + TILE_SIZE),
					i * TILE_SIZE + TILE_SIZE);
			}
			else if (i == 0 && j == 2)
			{
				SetRect(&sampleTileInfo[i * SAMPLE_TILE_X + j].rc,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE),
					i * TILE_SIZE + TILE_SIZE / 2,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE + TILE_SIZE),
					i * TILE_SIZE + TILE_SIZE);
			}
			else if (i == 0 && j == 3)
			{
				SetRect(&sampleTileInfo[i * SAMPLE_TILE_X + j].rc,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE),
					i * TILE_SIZE,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE + TILE_SIZE / 2),
					i * TILE_SIZE + TILE_SIZE);
			}
			else if (i == 0 && j == 4)
			{
				SetRect(&sampleTileInfo[i * SAMPLE_TILE_X + j].rc,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE),
					i * TILE_SIZE,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE + TILE_SIZE),
					i * TILE_SIZE + TILE_SIZE / 2);
			}
			else
			{
				SetRect(&sampleTileInfo[i * SAMPLE_TILE_X + j].rc,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE),
					i * TILE_SIZE,
					(WINSIZE_TILE_MAP_X - sampleTile->GetWidth()) + (j * TILE_SIZE + TILE_SIZE),
					i * TILE_SIZE + TILE_SIZE);
			}

			sampleTileInfo[i * SAMPLE_TILE_X + j].frameX = j;
			sampleTileInfo[i * SAMPLE_TILE_X + j].frameY = i;
		}
	}

	// 샘플 타일 터레인 세팅
	for (int i = 7; i < SAMPLE_TILE_X*SAMPLE_TILE_Y; i++)
	{
		sampleTileInfo[i].terrian = TERRAIN::EARTH;
	}

	for (int i = 0; i < 5; i++)
	{
		sampleTileInfo[i].terrian = TERRAIN::WALL;
	}
	sampleTileInfo[5].terrian = TERRAIN::CEMENT;
	sampleTileInfo[6].terrian = TERRAIN::GRASS;
	sampleTileInfo[10].terrian = TERRAIN::WATER;
	sampleTileInfo[14].terrian = TERRAIN::GREY;

	// 왼쪽 상단에 메인 타일의 정보를 세팅
	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			SetRect(&tileInfo[i * TILE_X + j].rc,
				j * TILE_SIZE, i * TILE_SIZE,
				j * TILE_SIZE + TILE_SIZE, i * TILE_SIZE + TILE_SIZE);

			tileInfo[i * TILE_X + j].terrian = TERRAIN::EARTH;
			tileInfo[i * TILE_X + j].frameX = 4;
			tileInfo[i * TILE_X + j].frameY = 1;
			tileInfo[i * TILE_X + j].isCollision = false;
		}
	}

	return S_OK;
}


void TileMapToolScene::Release()
{
}

void TileMapToolScene::Update()
{
	#pragma region 선택 영역
	// 샘플 타일 영역 안 인지 확인
	RECT rcSample;
	rcSample.left = WINSIZE_TILE_MAP_X - sampleTile->GetWidth();
	rcSample.top = 0;
	rcSample.right = WINSIZE_TILE_MAP_X;
	rcSample.bottom = sampleTile->GetHeight();

	if (PtInRect(&rcSample, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcSample.left;
			int posY = g_ptMouse.y - rcSample.top;
			int idX = posX / TILE_SIZE;
			int idY = posY / TILE_SIZE;
			
			selectTileInfo.frame.left = idX;
			selectTileInfo.frame.right = idX;
			selectTileInfo.frame.top = idY;
			selectTileInfo.frame.bottom = idY;

			selectTileInfo.terrian = sampleTileInfo[idX + idY * SAMPLE_TILE_X].terrian;

			selectStartFrame = { idX, idY };
			sampleSelectStart = true;
		}

		if (sampleSelectStart)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
			{
				int posX = g_ptMouse.x - rcSample.left;
				int posY = g_ptMouse.y - rcSample.top;
				int idX = posX / TILE_SIZE;
				int idY = posY / TILE_SIZE;

				if (selectStartFrame.x <= idX)
				{
					selectTileInfo.frame.left = selectStartFrame.x;
					selectTileInfo.frame.right = idX;
				}
				else
				{
					selectTileInfo.frame.right = selectStartFrame.x;
					selectTileInfo.frame.left = idX;
				}
				if (selectStartFrame.y <= idY)
				{
					selectTileInfo.frame.top = selectStartFrame.y;
					selectTileInfo.frame.bottom = idY;
				}
				else
				{
					selectTileInfo.frame.bottom = selectStartFrame.y;
					selectTileInfo.frame.top = idY;
				}
			}
		}

		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			sampleSelectStart = false;
		}
	}
	#pragma endregion

	#pragma region 메인 영역
	RECT rcMain;
	rcMain.left = 0;
	rcMain.top = 0;
	rcMain.right = rcMain.left + (TILE_SIZE * TILE_X);
	rcMain.bottom = rcMain.top + (TILE_SIZE * TILE_Y);
	if (PtInRect(&rcMain, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON) ||
			KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcMain.left;
			int posY = g_ptMouse.y - rcMain.top;
			int idX = posX / TILE_SIZE;
			int idY = posY / TILE_SIZE;

			for (int i = 0; i <= selectTileInfo.frame.bottom - selectTileInfo.frame.top; i++)
			{
				for (int j = 0; j <= selectTileInfo.frame.right - selectTileInfo.frame.left; j++)
				{
					//메인 영역 안에 있어야 복사
					if (idX + j > TILE_X - 1)
						continue;

					if ((idX + j) + (idY + i) * SAMPLE_TILE_X < TILE_X * TILE_Y)
					{
						tileInfo[(idX + j) + (idY + i) * TILE_X].frameX = j + selectTileInfo.frame.left;
						tileInfo[(idX + j) + (idY + i) * TILE_X].frameY = i + selectTileInfo.frame.top;
						tileInfo[(idX + j) + (idY + i) * TILE_X].terrian = selectTileInfo.terrian;
					}
				}
			}
		}
	}
	#pragma endregion

	
	saveButton->Update();
	loadButton->Update();
}

void TileMapToolScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WINSIZE_TILE_MAP_X, WINSIZE_TILE_MAP_Y, WHITENESS);

	
	if (sampleTile)
	{
		// 샘플 타일	
		sampleTile->Render(hdc, WINSIZE_TILE_MAP_X - sampleTile->GetWidth(), 0);
		//for (int i = 0; i < SAMPLE_TILE_X * SAMPLE_TILE_Y; i++)
		//{
		//	Rectangle(hdc, sampleTileInfo[i].rc.left, sampleTileInfo[i].rc.top, sampleTileInfo[i].rc.right, sampleTileInfo[i].rc.bottom);
		//}
		

		// 메인 타일
		for (int i = 0; i < TILE_X * TILE_Y; i++)
		{
			sampleTile->FrameRender(hdc, tileInfo[i].rc.left + (TILE_SIZE / 2), tileInfo[i].rc.top + (TILE_SIZE / 2),
				tileInfo[i].frameX, tileInfo[i].frameY);
		}

		// 선택된 타일들
		for (int i = 0; i <= selectTileInfo.frame.bottom - selectTileInfo.frame.top; i++)
		{
			for (int j = 0; j <= selectTileInfo.frame.right - selectTileInfo.frame.left; j++)
			{
				sampleTile->FrameRender(hdc,
					WINSIZE_TILE_MAP_X - sampleTile->GetWidth() + (j * TILE_SIZE),
					sampleTile->GetHeight() + 100 + (i * TILE_SIZE), 
					j + selectTileInfo.frame.left, i + selectTileInfo.frame.top);
			}
		}
	}
	saveButton->Render(hdc);
	loadButton->Render(hdc);
}

void TileMapToolScene::LodeStage(const char * mapfile)
{
	DWORD readByte;
	HANDLE hFile = CreateFile(mapfile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readByte, NULL);

	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		tileInfo[i].rc.left += WINSIZE_LEFT;
		tileInfo[i].rc.right += WINSIZE_LEFT;
		tileInfo[i].rc.top += WINSIZE_TOP ;
		tileInfo[i].rc.bottom += WINSIZE_TOP;
	}

	CloseHandle(hFile);
}

void TileMapToolScene::StageRender(HDC hdc)
{
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		sampleTile->FrameRender(hdc, tileInfo[i].rc.left + (TILE_SIZE / 2), tileInfo[i].rc.top + (TILE_SIZE / 2),
			tileInfo[i].frameX, tileInfo[i].frameY);

		//if (tileInfo[i].frameX == 0 && tileInfo[i].frameY == 0)
		//{
		//	Rectangle(hdc, tileInfo[i].rc.left + (TILE_SIZE / 2), tileInfo[i].rc.top + (TILE_SIZE / 2),
		//		tileInfo[i].rc.left - (TILE_SIZE / 2), tileInfo[i].rc.top - (TILE_SIZE / 2));
		//}
	}
}

void TileMapToolScene::ChangeTile(int x, int y, int state)
{
	if (tileInfo[x + (y * TILE_X)].isCollision)
	{
		if (tileInfo[x + (y * TILE_X)].frameX == 0 && tileInfo[x + (y * TILE_X)].frameY == 0)
		{
			switch (state)
			{
			case STATE::STATE_UP:
				tileInfo[x + (y * TILE_X)].frameX = sampleTileInfo[4].frameX;
				tileInfo[x + (y * TILE_X)].frameY = sampleTileInfo[4].frameY;
				tileInfo[x + (y * TILE_X)].terrian = sampleTileInfo[4].terrian;
				break;
			case STATE::STATE_DOWN:
				tileInfo[x + (y * TILE_X)].frameX = sampleTileInfo[2].frameX;
				tileInfo[x + (y * TILE_X)].frameY = sampleTileInfo[2].frameY;
				tileInfo[x + (y * TILE_X)].terrian = sampleTileInfo[2].terrian;
				break;
			case STATE::STATE_LEFT:
				tileInfo[x + (y * TILE_X)].frameX = sampleTileInfo[3].frameX;
				tileInfo[x + (y * TILE_X)].frameY = sampleTileInfo[3].frameY;
				tileInfo[x + (y * TILE_X)].terrian = sampleTileInfo[3].terrian;
				break;
			case STATE::STATE_RIGHT:
				tileInfo[x + (y * TILE_X)].frameX = sampleTileInfo[1].frameX;
				tileInfo[x + (y * TILE_X)].frameY = sampleTileInfo[1].frameY;
				tileInfo[x + (y * TILE_X)].terrian = sampleTileInfo[1].terrian;
				break;
			default:
				break;
			}

			tileInfo[x + (y * TILE_X)].isCollision = false;
		}
		else if ((tileInfo[x + (y * TILE_X)].frameX == 1 && tileInfo[x + (y * TILE_X)].frameY == 0) ||
			(tileInfo[x + (y * TILE_X)].frameX == 2 && tileInfo[x + (y * TILE_X)].frameY == 0) ||
			(tileInfo[x + (y * TILE_X)].frameX == 3 && tileInfo[x + (y * TILE_X)].frameY == 0) ||
			(tileInfo[x + (y * TILE_X)].frameX == 4 && tileInfo[x + (y * TILE_X)].frameY == 0))
		{
			tileInfo[x + (y * TILE_X)].terrian = TERRAIN::EARTH;
			tileInfo[x + (y * TILE_X)].frameX = 4;
			tileInfo[x + (y * TILE_X)].frameY = 1;
			tileInfo[x + (y * TILE_X)].isCollision = false;
		}
		else if ((tileInfo[x + (y * TILE_X)].frameX == 0 && tileInfo[x + (y * TILE_X)].frameY == 1)
			&& Item::GetSingleton()->GetIsSpuerTank())
		{
			tileInfo[x + (y * TILE_X)].terrian = TERRAIN::EARTH;
			tileInfo[x + (y * TILE_X)].frameX = 4;
			tileInfo[x + (y * TILE_X)].frameY = 1;
			tileInfo[x + (y * TILE_X)].isCollision = false;
		}
	}
}

