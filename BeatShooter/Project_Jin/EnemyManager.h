#pragma once
enum class PatternType
{
	TRACKING,
	POS
};
struct EnemySpawnInfo
{
	EnemyType spawnEnemyType;
	Vector2 spawnPos;
	float spawnTime;
}; 
typedef vector<EnemySpawnInfo> VecSpawnInfo;
struct EnemyPattern
{
	PatternType mPatternType;
	VecSpawnInfo mVecSpawnInfo;
};
class EnemyManager
{
public:
	
	EnemyManager(Vector3& playerPos, Camera& camera,UINT maxEnemyNum);
	~EnemyManager();

	void Update();
	void LastUpdate();
	void PrevRender();
	void Render();

	void Waring();
	void DisableWaring();
	void EnemyRandomSpawn();
	void EnemySpawnTimer();
	void ActiveEnemy(EnemyType enemyType,Vector3 activePos);
	void KillEnemys();
	void DisableEnemys();
	UINT GetActiveEnemyCount();
	void Collsion();

	void EnemyPatternLoad();
	void AddTrackingCirclePattern(EnemyType enemyTpye, Vector2 distance, UINT maxPatternNum, UINT maxEnemyNum,float patternTime, float enemySpawnTime);

	inline vector<Enemy*>	GetEnemys();
	inline void				SetIsActive(bool isActive);
private:
	UINT					mCurPatternNum;
	UINT					mCurSpawnEnemyNum;
	bool					mIsActive;
	bool					mIsWaveWaring;
	bool					mIsPattern;
	World					mWaveWaringWorld;

	Vector3&				mPlayerPos;
	vector<Enemy*>			mEnemys;
	Camera&					mCamera;
	Timer*					mEnemyPatternTimer;
	Timer*					mEnemySpawnTimer;
	Timer*					mWaveWaringTimer;
	Texture*				mWaveWaringTexture;
	vector<EnemyPattern>	mPatterns;
	BossEnemy*				mBossEnemy;
};

inline vector<Enemy*> EnemyManager::GetEnemys()
{
	return mEnemys;
}

inline void EnemyManager::SetIsActive(bool isActive)
{ 
	mIsActive = isActive;
}


