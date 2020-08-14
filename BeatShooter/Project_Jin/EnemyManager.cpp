#include "stdafx.h"
#include "EnemyManager.h"


EnemyManager::EnemyManager(Vector3& playerPos, Camera& camera, UINT maxEnemyNum)
	:mCamera(camera), mCurSpawnEnemyNum(0), mIsPattern(false), mIsActive(true)
	, mPlayerPos(playerPos)
{
	for (int count = 0; count < (int)maxEnemyNum; count++)
	{
		mEnemys.push_back(NEW Enemy(playerPos, EnemyType::CIRCLE_ENEMY));
		mEnemys.push_back(NEW Enemy(playerPos, EnemyType::TRI_ENEMY));
		mEnemys.push_back(NEW Enemy(playerPos, EnemyType::RECT_ENEMY));
		mEnemys.push_back(NEW Enemy(playerPos, EnemyType::HEXA_ENEMY));
	}
	mWaveWaringTexture = Texture::Add(L"res/wave_waring.png");
	mEnemyPatternTimer = NEW Timer(5.0f, true);
	mEnemyPatternTimer->timerFunc = bind(&EnemyManager::EnemyRandomSpawn, this);
	mEnemyPatternTimer->StartTimer();

	mWaveWaringTimer = NEW Timer(3.0f, false);
	mWaveWaringTimer->timerFunc = bind(&EnemyManager::DisableWaring, this);
	mEnemySpawnTimer = NEW Timer(0.0f, true);
	mEnemySpawnTimer->timerFunc = bind(&EnemyManager::EnemySpawnTimer, this);
	mIsWaveWaring = false;
	mWaveWaringWorld.pos = { CENTER_X,CENTER_Y,0 };
	mWaveWaringWorld.scale = { 1.5f,1.5f };
	EnemyPatternLoad();
	mCurPatternNum = Utility::Random(0, mPatterns.size() - 1);
}

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : mEnemys)
	{
		SAFE_DELETE(enemy);
	}
	SAFE_DELETE(mEnemyPatternTimer);
	SAFE_DELETE(mWaveWaringTimer);
	SAFE_DELETE(mEnemySpawnTimer);
	mEnemys.clear();
}

void EnemyManager::Update()
{
	if (!mIsActive)
	{
		return;
	}
	for (Enemy* enemy : mEnemys)
	{
		enemy->Update();
	}

	mEnemyPatternTimer->Update();

	if (mIsPattern)
	{
		mEnemySpawnTimer->Update();
	}

	if (mIsWaveWaring)
	{
		mWaveWaringWorld.rot.z -= 2 * DELTATIME;
		mWaveWaringTimer->Update();
	}
}

void EnemyManager::LastUpdate()
{
	if (!mIsActive)
	{
		return;
	}
	for (Enemy* enemy : mEnemys)
	{
		enemy->LastUpdate();
	}
	Collsion();
	mWaveWaringWorld.Update();
}

void EnemyManager::PrevRender()
{
	if (!mIsActive)
	{
		return;
	}
	for (Enemy* enemy : mEnemys)
	{
		enemy->PrevRender();
	}
}

void EnemyManager::Render()
{
	if (!mIsActive)
	{
		return;
	}

	if (mIsWaveWaring)
	{
		mWaveWaringWorld.SetWorld();
		mWaveWaringTexture->Render();
	}
	for (Enemy* enemy : mEnemys)
	{
		enemy->Render();
	}
}

void EnemyManager::Waring()
{
	mWaveWaringWorld.rot.z = GAME_MANAGER->GetSpinLightAngle();
	mIsWaveWaring = true;
	mWaveWaringTimer->StartTimer();
}

void EnemyManager::DisableWaring()
{
	mIsWaveWaring = false;
}

void EnemyManager::EnemyRandomSpawn()
{
	if (GetActiveEnemyCount() > 3)
	{
		return;
	}
	if (!mIsPattern)
	{
		Waring();
		mCurSpawnEnemyNum = 0;
		mCurPatternNum = Utility::Random(0, mPatterns.size() - 1);
		SOUND->Play("waveWaring", 1.0f);
		mIsPattern = true;
		mEnemySpawnTimer->StartTimer();
	}
}

void EnemyManager::EnemySpawnTimer()
{
	EnemyPattern ePattern = mPatterns[mCurPatternNum];
	if (mCurSpawnEnemyNum >= ePattern.mVecSpawnInfo.size())
	{
		mIsPattern = false;
		mEnemySpawnTimer->StopTimer();
	}
	else
	{
		float spawnTime = 0;

		while (spawnTime == 0 && mCurSpawnEnemyNum < ePattern.mVecSpawnInfo.size())
		{
			switch (ePattern.mPatternType)
			{
			case PatternType::TRACKING:
			{
				EnemySpawnInfo enemySpawnInfo = mPatterns[mCurPatternNum].mVecSpawnInfo[mCurSpawnEnemyNum];
				spawnTime = enemySpawnInfo.spawnTime;
				if (spawnTime != 0)
				{
					SOUND->Play("enemyWarning", 0.25f);
				}
				ActiveEnemy(enemySpawnInfo.spawnEnemyType,
					{ enemySpawnInfo.spawnPos.x + mPlayerPos.x, enemySpawnInfo.spawnPos.y + mPlayerPos.y,0 });
				mEnemySpawnTimer->limitTime = enemySpawnInfo.spawnTime;
				mCurSpawnEnemyNum++;
			} 
			break;
			case PatternType::POS:
			{
				EnemySpawnInfo enemySpawnInfo = mPatterns[mCurPatternNum].mVecSpawnInfo[mCurSpawnEnemyNum];
				spawnTime = enemySpawnInfo.spawnTime;
				if (spawnTime != 0)
				{
					SOUND->Play("enemyWarning", 0.25f);
				}
				ActiveEnemy(enemySpawnInfo.spawnEnemyType, { enemySpawnInfo.spawnPos.x, enemySpawnInfo.spawnPos.y,0 });
				mEnemySpawnTimer->limitTime = enemySpawnInfo.spawnTime;
				mCurSpawnEnemyNum++;
			}
			break;
			default:
				break;
			}

		}
	}
}

void EnemyManager::ActiveEnemy(EnemyType enemyType, Vector3 activePos)
{
	for (Enemy* enemy : mEnemys)
	{
		if (!enemy->GetIsWaring() && !enemy->GetIsAcive())
		{
			if (enemyType == enemy->GetEnemyType())
			{
				enemy->ActiveEnemy(activePos);
				return;
			}
		}
	}
}

void EnemyManager::KillEnemys()
{
	for (Enemy* enemy : mEnemys)
	{
		if (enemy->GetIsAcive())
		{
			enemy->DieEnemy();
		}
		if (enemy->GetIsWaring())
		{
			enemy->DieEnemy();
		}
	}
	mIsPattern = false;
	mEnemySpawnTimer->StopTimer();
	mEnemyPatternTimer->StartTimer();
}

void EnemyManager::DisableEnemys()
{
	for (Enemy* enemy : mEnemys)
	{
		if (enemy->GetIsAcive())
		{
			enemy->DisableEnemy();
		}
		if (enemy->GetIsWaring())
		{
			enemy->DisableEnemy();
		}
	}
	mIsPattern = false;
	mEnemySpawnTimer->StopTimer();
	mEnemyPatternTimer->StartTimer();
}

UINT EnemyManager::GetActiveEnemyCount()
{
	UINT activeEnemyCount = 0;
	for (Enemy* enemy : mEnemys)
	{
		if (enemy->GetIsAcive())
		{
			activeEnemyCount++;
		}
		if (enemy->GetIsWaring())
		{
			activeEnemyCount++;
		}
	}
	return activeEnemyCount;
}

void EnemyManager::Collsion()
{
	for (UINT i = 0; i < mEnemys.size(); i++)
	{
		if (mEnemys[i]->GetIsAcive())
		{
			for (UINT j = 0; j < mEnemys.size(); j++)
			{
				if (mEnemys[j]->GetIsAcive())
				{
					if (i != j)
					{
						mEnemys[i]->CollisionObjPushByCicle(*mEnemys[j]);//Enemy들 끼리의 출돌처리
					}
				}
			}

			if (mEnemys[i]->CollisionObjPushByCicle(*GAME_MANAGER->GetPlayer()))//Enemy와 플레이어의 충돌처리
			{
				GAME_MANAGER->GetPlayer()->HitPlayer();
			}
		}
	}


}

void EnemyManager::EnemyPatternLoad()
{
	string xmlFilepath = "res/enemy_pattern.xml";
	XmlDocument document;
	document.LoadFile(xmlFilepath.c_str());
	XmlElement* atlas = document.FirstChildElement();

	XmlElement* patternAtlas = atlas->FirstChildElement();
	for (; patternAtlas != NULL; patternAtlas = patternAtlas->NextSiblingElement())
	{
		XmlElement* enemySpawnAtlas = patternAtlas->FirstChildElement();
		EnemyPattern enemyPattern;
		enemyPattern.mPatternType = (PatternType)patternAtlas->UnsignedAttribute("patternType");
		for (; enemySpawnAtlas != NULL; enemySpawnAtlas = enemySpawnAtlas->NextSiblingElement())
		{
			EnemySpawnInfo enemySpawnInfo;
			enemySpawnInfo.spawnEnemyType = (EnemyType)enemySpawnAtlas->UnsignedAttribute("enemyType");
			enemySpawnInfo.spawnTime = enemySpawnAtlas->FloatAttribute("t");
			enemySpawnInfo.spawnPos = Vector2(enemySpawnAtlas->FloatAttribute("x"), enemySpawnAtlas->FloatAttribute("y"));
			enemyPattern.mVecSpawnInfo.push_back(enemySpawnInfo);
		}
		mPatterns.push_back(enemyPattern); 
	}
	AddTrackingCirclePattern(EnemyType::CIRCLE_ENEMY, { 400,400 }, 5, 10, 1.0f, 0.0f);
	AddTrackingCirclePattern(EnemyType::TRI_ENEMY, { 400,400 }, 10, 3, 0.2f, 0.0f);
	AddTrackingCirclePattern(EnemyType::RECT_ENEMY, { 300,300 }, 10, 4, 0.2f, 0.0f);
	AddTrackingCirclePattern(EnemyType::HEXA_ENEMY, { 300,300 }, 4, 6, 1.0f, 0.0f);
	AddTrackingCirclePattern(EnemyType::RECT_ENEMY, { 0,0 }, 30, 1, 0.1f, 0.1f);
}


void EnemyManager::AddTrackingCirclePattern(EnemyType enemyTpye, Vector2 distance, UINT maxPatternNum, UINT maxEnemyNum, float patternTime, float enemySpawnTime)
{
	EnemyPattern enemyPattern;
	enemyPattern.mPatternType = PatternType::TRACKING;
	for (int patternCount = 0; patternCount < maxPatternNum; patternCount++)
	{
		for (int enemyCount = 0; enemyCount < maxEnemyNum; enemyCount++)
		{
			EnemySpawnInfo enemySpawnInfo;
			enemySpawnInfo.spawnEnemyType = enemyTpye;
			enemySpawnInfo.spawnTime = enemySpawnTime;
			Vector2 dir;
			Vector2 pos = distance;
			Utility::SetAngle((enemyCount * (360 / maxEnemyNum)) * PI / 180, &dir);
			Utility::Nomalize(dir);
			pos = { pos.x * dir.x, pos.y * dir.y };
			enemySpawnInfo.spawnPos = pos;
			if (enemyCount == maxEnemyNum - 1)
			{
				enemySpawnInfo.spawnTime = patternTime;
			}
			enemyPattern.mVecSpawnInfo.push_back(enemySpawnInfo);
		}
	}
	mPatterns.push_back(enemyPattern);
}

