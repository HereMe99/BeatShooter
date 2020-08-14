#pragma once
class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();

	virtual void Update() override;
	virtual void LastUpdate() override;
	virtual void PrevRender() override;
	virtual void Render() override;

	void Fire(Texture* bulletTexture,Texture* effectTexture,Vector3 firePos, Vector2 direction, Vector2 size,float bulletSpeed, float atk);
	void Disable();
	void Collsion();
	void EffectTimer();

	void SetIsPlayerBullet(bool isPlayerBullet) { mIsPlayerBullet = isPlayerBullet; }

	bool GetIsActive() { return mIsActive; }
	bool GetIsEffect() { return mIsEffect; }
private:
	bool mIsActive;
	bool mIsEffect;
	bool mIsPlayerBullet;
	Vector2 mDirection;
	World mEffectWorld;
	float mAtk;
	float mBulletSpeed;

	Texture* mBulletTexture;
	Texture* mEffectTexture;
	Timer* mActiveTimer;
	Timer* mBulletEffectTimer;
	MotionTrailer* mWorldBlur;
};

