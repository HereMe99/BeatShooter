#pragma once


enum class EnemyType
{
	CIRCLE_ENEMY,
	TRI_ENEMY,
	RECT_ENEMY,
	HEXA_ENEMY,
	NONE
};


class Enemy : public GameObject
{
public:
	Enemy(Vector3& playerPos,EnemyType enemyType);
	~Enemy();

	void Init();
	virtual void Update()		 override;
	virtual void LastUpdate()	override;
	virtual void PrevRender()	override;
	virtual void Render()		override;

	bool CollsionMap();
	void EnemyPattern();

	void ActiveEnemy(Vector3 activePos);
	void HitEnemy(Vector2 collsionDir);
	void DieEnemy();
	void DisableEnemy();
	void WaringTimer();
	void ExplosionTimer();
	
	inline World& GetWorld();
	inline bool GetIsWaring();
	inline bool	GetIsAcive();
	inline EnemyType GetEnemyType();
private:

	const float CIRCLE_ENEMY_SPEED	= 500.0f;
	const float TRI_ENEMY_SPEED		= 800.0f;
	const float RECT_ENEMY_SPEED	= 1700.0f;
	const float LINE_ENEMY_SPEED	= 2000.0f;
	const float NUCKBACK_SPEED = 4000.0f;
	const int	CIRCLE_ENEMY_HP = 2;
	const int	TRI_ENEMY_HP = 2;
	const int	RECT_ENEMY_HP = 3;
	const int	LINE_ENEMY_HP = 2;

	float		mEnemySpeed;
	Vector2		mEnemyAngle;
	EnemyType	mEnemyType;
	bool		mIsMove;
	bool		mIsActive;
	bool		mIsWaring;
	bool		mIsExplosion;
	int			mEnemyHp;

	Texture*	mEnemyTexture;
	Texture*	mWaringTexture;
	Texture*	mWaringRingTexture;
	Texture*	mExplosionTexture;
	MotionTrailer*	mWorldBlur;
	World		mWaringWorld;
	World		mExplosionWorld;
	Vector3&	mPlayerPos;
	Timer*		mEnemyPatternTimer;
	Timer*		mWaringTimer;
	Timer*		mExplosionTimer;
};
inline World& Enemy::GetWorld()
{
	return mWorld;
}
inline bool Enemy::GetIsWaring()
{
	return mIsWaring;
}
inline bool Enemy::GetIsAcive()
{
	return mIsActive;
}

inline EnemyType Enemy::GetEnemyType()
{
	return mEnemyType;
}
