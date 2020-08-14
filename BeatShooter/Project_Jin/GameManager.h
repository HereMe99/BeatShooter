#pragma once

#define HIGHPASS_AMOUNT 3000.0f
#define HIGHPASS_SPEED 0.32f
#define BGM_SLOW_TO_NORMAL_SPEED 0.38f
#define FREQUENCY_DEFAULT 44100.0f
#define FREQUENCY_SLOWER 8000.0f

class NumberTexture
{
public:
	Texture* mTextures[10];
	World mWorld;
	int mNumber;

	void Render(Camera& camera)
	{
		if (mNumber == -1)
		{
			return;
		}
		World::InitWorld();
		mWorld.SetView();
		mTextures[mNumber]->Render();
		camera.SetView();
	}

	LPDIRECT3DTEXTURE9 GetDxTexture()
	{
		if (mNumber == -1)
		{
			return nullptr;
		}
		return 	mTextures[mNumber]->GetDxTexture();
	}
};

class GameManager
{
private:
	GameManager(Camera& camera, World& bgWorld, World& spinLightWorld);
	~GameManager();
public:
	inline static GameManager* GetInstance();
	inline static void Create(Camera& camera, World& bgWorld, World& spinLightWorld);
	inline static void Release();

	void Init();
	void Update();
	void LastUpdate();
	void PrevRender();
	void Render();

	void UpdateHighpass();
	void BgmFxTimer();
	void HitPlayer();
	void ObjCollsion();
	void CameraShake();
	void ShakeTimer();
	void SetNumberTextures(UINT number, Vector2 pos, Vector2 scale);

	inline World				GetPlayerWorld() const;
	static inline vector<GameObject*>	GetObjs();
	inline Player* GetPlayer() const;
	inline EnemyManager* GetEnemyManger() const;
	inline Camera& GetCamera() const;
	inline bool					GetIsShake() const;
	inline void					SetBgmFrequency(const float frequency);
	inline void					SetBgmHighPass(const bool isHighpass);
	inline void					AddScore(const UINT addNum);
	inline float				GetSpinLightAngle() const;
private:
	static GameManager* instance;

	UINT mScore;
	UINT mBestScore;
	float mBgmFrequency;
	float mBgmHighpass;
	bool mIsHighpass;
	bool mIsShake;
	bool mIsPlayerHitFX;
	bool mIsBestScore;

	NumberTexture mNumberTextures[10];

	LPD3DXEFFECT mColorShader;
	vector<GameObject*> mObjs;
	Camera& mCamera;
	World& mBgWorld;
	World& mSpinLightWorld;
	Player* mPlayer;
	EnemyManager* mEnemyManager;
	Texture* mEndTexture;


	Timer* mShakeTimer;
	Timer* mBgmFxTimer;
};


inline GameManager* GameManager::GetInstance()
{
	return instance;
}

inline void GameManager::Create(Camera& camera, World& bgWorld, World& spinLightWorld)
{
	if (instance == nullptr)
	{
		instance = NEW GameManager(camera, bgWorld, spinLightWorld);
	}
}

inline void GameManager::Release()
{
	SAFE_DELETE(instance);
}

inline World GameManager::GetPlayerWorld() const
{
	return mPlayer->GetWorld();
}

inline vector<GameObject*> GameManager::GetObjs()
{
	return vector<GameObject*>();
}

inline Player* GameManager::GetPlayer() const
{
	return mPlayer;
}

inline EnemyManager* GameManager::GetEnemyManger() const
{
	return mEnemyManager;
}

inline Camera& GameManager::GetCamera() const
{
	return mCamera;
}

inline bool GameManager::GetIsShake() const
{
	return mIsShake;
}

inline void GameManager::SetBgmFrequency(const float frequency)
{
	SOUND->SetSoundFrequency("bgm", frequency);
}

inline void GameManager::SetBgmHighPass(const bool isHighpass)
{
	mIsHighpass = isHighpass;
}

inline void GameManager::AddScore(const UINT addNum)
{
	mScore += addNum;
}
inline float GameManager::GetSpinLightAngle() const
{
	return mSpinLightWorld.rot.z;
}

