#include "stdafx.h"
#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager(Camera& camera, World& bgWorld, World& spinLightWorld)
	:mCamera(camera), mBgWorld(bgWorld), mScore(0),mSpinLightWorld(spinLightWorld)
{
	SOUND->Add("bgm", "res/bgm.ogg", true);
	SOUND->Add("fire", "res/shot.ogg", false);
	SOUND->Add("waveWaring", "res/wave_siren.ogg", false);
	SOUND->Add("enemyWarning", "res/warn.ogg", false);
	SOUND->Add("explosion", "res/explosion.ogg", false);
	SOUND->Add("start", "res/start.ogg", false);
	SOUND->Add("playerExplosion", "res/explosion2.ogg", false);
	SOUND->Add("bulletHit", "res/hit.ogg", false);

	Texture* playerTexture = Texture::Add(L"res/player.png");
	Texture* playerBulletTexture = Texture::Add(L"res/playerBullet.png");
	mPlayer = NEW Player(camera, playerTexture, playerBulletTexture);
	mObjs.push_back(mPlayer);
	mBgmHighpass = HIGHPASS_AMOUNT;
	mBgmFrequency = FREQUENCY_DEFAULT;
	mIsHighpass = false;
	mEnemyManager = NEW EnemyManager(mPlayer->GetWorld().pos, camera, 100);

	for (Enemy* enemy : mEnemyManager->GetEnemys())
	{
		mObjs.push_back(enemy);
	}

	mIsShake = false;
	mIsPlayerHitFX = false;
	mShakeTimer = NEW Timer(0.1f, false);
	mShakeTimer->timerFunc = bind(&GameManager::ShakeTimer, this);
	mBgmFxTimer = NEW Timer(2.0f, false);
	mBgmFxTimer->timerFunc = bind(&GameManager::BgmFxTimer, this);

	Vector3 numberStartPos = { CENTER_X + 250.0f,WIN_HEIGHT * 0.1f,0 };
	int count = 0;
	for (NumberTexture& numberTexture : mNumberTextures)
	{

		for (int textureIndex = 0; textureIndex < 10; textureIndex++)
		{
			numberTexture.mTextures[textureIndex] = Texture::Add(L"res/number.png", { 64.0f * textureIndex ,0 }, { 64.0f + 64.0f * textureIndex ,64 });
		}
		numberTexture.mWorld.pos = numberStartPos;
		numberTexture.mWorld.pos.x -= 50 * count;
		numberTexture.mNumber = -1;
		count++;
	}

	mColorShader = Shader::GetShader(ShaderType::ALPHAON);
	SOUND->Play("bgm", 0.3f);
	SOUND->DspHighpassSetting("bgm");
	SOUND->DspSetHighpass();
	mBestScore = 0;
	mIsBestScore = false;
}

GameManager::~GameManager()
{
	SAFE_DELETE(mPlayer);
	SAFE_DELETE(mEnemyManager);
	SAFE_DELETE(mShakeTimer);
	SAFE_DELETE(mBgmFxTimer);
}
void GameManager::Init()
{
	mScore = 0;
	mPlayer->Init();
	mPlayer->ActivePlayer();
	mEnemyManager->DisableEnemys();
	mEnemyManager->SetIsActive(true);
	mIsHighpass = false;
	mIsPlayerHitFX = true;
	mIsBestScore = false;
}
void GameManager::Update()
{
	if (mBestScore <= mScore)
	{
		mBestScore = mScore;
		mIsBestScore = true;
	}
	if (mPlayer->GetIsDie())
	{
		if (KEY_DOWN('R'))
		{
			GAME_MANAGER->Init();
			SCENE->ChangeScene("play");
		}
		if (KEY_DOWN(VK_ESCAPE))
		{
			SCENE->ChangeScene("title");
		}
	}
	mBgmFxTimer->Update();
	mEnemyManager->Update();
	mPlayer->Update();
	mShakeTimer->Update();
	UpdateHighpass();

}

void GameManager::LastUpdate()
{
	mEnemyManager->LastUpdate();
	mPlayer->LastUpdate();
	for (NumberTexture& numberTexture : mNumberTextures)
	{
		numberTexture.mWorld.Update();
	}
}

void GameManager::PrevRender()
{
	mPlayer->PrevRender();
	mEnemyManager->PrevRender();
}

void GameManager::Render()
{
	if (mPlayer->GetIsDie())
	{
		for (NumberTexture& numberTexture : mNumberTextures)
		{
			SetNumberTextures(mScore, { CENTER_X,WIN_HEIGHT*0.6f }, { 2.0f,2.0f });
			LPDIRECT3DTEXTURE9 numberDxTexture = numberTexture.GetDxTexture();
			if (numberDxTexture != nullptr)
			{
				mColorShader->SetTexture("map", numberDxTexture);
				if (mIsBestScore)
				{
					mColorShader->SetFloatArray("color", (float*)&D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f), 4);
				}
				else
				{
					mColorShader->SetFloatArray("color", (float*)&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4);
				}
				mColorShader->Begin(NULL, NULL);
				mColorShader->BeginPass(2);
				numberTexture.Render(mCamera);
				mColorShader->EndPass();
				mColorShader->End();
			}
		}
	}
	else
	{
		for (NumberTexture& numberTexture : mNumberTextures)
		{
			SetNumberTextures(mScore, { CENTER_X,WIN_HEIGHT * 0.1f }, {1.0f,1.0f});
			LPDIRECT3DTEXTURE9 numberDxTexture = numberTexture.GetDxTexture();
			if (numberDxTexture != nullptr)
			{
				mColorShader->SetTexture("map", numberDxTexture);
				if (mIsBestScore)
				{
					mColorShader->SetFloatArray("color", (float*)&D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f), 4);
				}
				else
				{
					mColorShader->SetFloatArray("color", (float*)&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 4);
				}
				mColorShader->Begin(NULL, NULL);
				mColorShader->BeginPass(2);
				numberTexture.Render(mCamera);
				mColorShader->EndPass();
				mColorShader->End();
			}
		}
	}

	mPlayer->Render();
	mEnemyManager->Render();
}


void GameManager::UpdateHighpass()
{
	if (mIsHighpass)
	{
		mBgmHighpass += (HIGHPASS_AMOUNT - mBgmHighpass) / HIGHPASS_SPEED * DELTATIME;
	}
	else
	{
		mBgmHighpass -= (mBgmHighpass / HIGHPASS_SPEED) * DELTATIME;
	}
	SOUND->DspSetHighpass(mBgmHighpass);
	SOUND->SetSoundFrequency("bgm", mBgmFrequency);
	mBgmFrequency += (FREQUENCY_DEFAULT - mBgmFrequency) / BGM_SLOW_TO_NORMAL_SPEED * DELTATIME;
	if (mBgmFrequency > FREQUENCY_DEFAULT - 1)
		mBgmFrequency = FREQUENCY_DEFAULT;
}

void GameManager::BgmFxTimer()
{
	//mBgmFrequency = FREQUENCY_DEFAULT;
	mBgWorld.scale = DEFAULT_BG_WORLD_SCALE;
}

void GameManager::HitPlayer()
{
	mBgmFxTimer->StartTimer();
	mBgmFrequency = FREQUENCY_SLOWER;
}

void GameManager::ObjCollsion()
{
}

void GameManager::CameraShake()
{
	mIsShake = true;
	mShakeTimer->StartTimer();
	mCamera.SetScale({ 0.51f,0.51f,1.0f });
}

void GameManager::ShakeTimer()
{
	mIsShake = false;
	mCamera.SetScale({ 0.5f,0.5f,1.0f });
}

void GameManager::SetNumberTextures(UINT number, Vector2 pos, Vector2 scale)
{
	string str;
	str = to_string(number); 
	for (int index = str.size() - 1; index >= 0; index--)
	{
		mNumberTextures[str.size() - 1 - index].mNumber = str.c_str()[index] - '0';
		if (str.size() % 2 == 0)
		{
			float resultPos = index - str.size() * 0.5f;
			float result = 25*scale.x+(50 * scale.x * resultPos);
			mNumberTextures[str.size() - 1 - index].mWorld.scale = scale;
			mNumberTextures[str.size() - 1 - index].mWorld.pos.x = pos.x + result;
			mNumberTextures[str.size() - 1 - index].mWorld.pos.y = pos.y;
		}
		else
		{
			float resultPos = index - (str.size() - 1) * 0.5f;
			float result = (50 * scale.x * resultPos);
			mNumberTextures[str.size() - 1 - index].mWorld.scale = scale;
			mNumberTextures[str.size() - 1 - index].mWorld.pos.x = pos.x +result;
			mNumberTextures[str.size() - 1 - index].mWorld.pos.y = pos.y;
		}
	}
	for (int index = str.size(); index < 10; index++)
	{
		mNumberTextures[index].mNumber = -1;
	}
}

