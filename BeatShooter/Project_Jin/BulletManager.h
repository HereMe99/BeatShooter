#pragma once
class BulletManager
{
public:
	BulletManager(UINT bulletCount);
	~BulletManager();

	void Update();
	void LastUpdate();
	void PrevRender();
	void Render();

	void DisableBullets();
	void FireTimerFunc();
	void SetIsPlayerBullet(bool isPlayerBullet);
	void Fire(Texture* texture, Texture* effectTexture, Vector3 firePos, Vector2 direction, Vector2 size, float bulletSpeed, float atk);
private:
	vector<Bullet*> mBullets;
	bool mIsFire;
	Timer* mFireTimer;
};

