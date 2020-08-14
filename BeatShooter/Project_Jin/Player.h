#pragma once
#define MAX_PLAYER_HP 3
class Player : public GameObject
{
public:
	Player(Camera& camera, Texture* playerTexture, Texture* playerBulletTexture);
	virtual ~Player();
	void Init();
	virtual void Update()		override;
	virtual void LastUpdate()	override;
	virtual void PrevRender()	override;
	virtual void Render()		override;

	void	CheckKey();
	void	HitPlayer();
	void	HitPlayerTimer();
	void	DiePlayer();
	void	ActivePlayer();
	World&	GetWorld() { return mWorld; }
	bool GetIsDie() { return mIsDie; }
private:

	bool				mIsDie;
	bool				mIsActive;
	bool				mIsPlayerHitFX;
	float				mPlayerSpeed;
	int					mPlayerHP;
	float				mPlayerATK;
	float				mPlayerBulletSpeed;
	Vector2				mBulletAngle;
	World				mPlayerExplosionWorld;
	vector<World>		mPlayerLifeWorlds;

	Camera&				mCamera;
	MotionTrailer*		mMotionTrailer;
	Texture*			mPlayerTexture;
	Texture*			mPlayerBulletTexture;
	Texture*			mPlayerBulletEffectTexture;
	Texture*			mExplosionTexture;
	BulletManager*		mBulletManager;

	Timer*				mPlayerHitTimer;
};

