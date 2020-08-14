#include "stdafx.h"
#include "BulletManager.h"

BulletManager::BulletManager(UINT bulletCount)
	: mIsFire(true), mFireTimer(NEW Timer(0.07f, false))
{
	for (UINT bulletIndex = 0; bulletIndex < bulletCount; bulletIndex++)
	{
		mBullets.push_back(NEW Bullet());
	}
	mFireTimer->timerFunc = bind(&BulletManager::FireTimerFunc, this);
}

BulletManager::~BulletManager()
{
	for (Bullet* bullet : mBullets)
	{
		SAFE_DELETE(bullet);
	}
	mBullets.clear();
	SAFE_DELETE(mFireTimer);
}

void BulletManager::Update()
{
	for (Bullet* bullet : mBullets)
	{
		bullet->Update();

	}
	mFireTimer->Update();
}

void BulletManager::LastUpdate()
{
	for (Bullet* bullet : mBullets)
	{
		bullet->LastUpdate();
	}
}

void BulletManager::PrevRender()
{
	for (Bullet* bullet : mBullets)
	{
		bullet->PrevRender();
	}
}

void BulletManager::Render()
{
	for (Bullet* bullet : mBullets)
	{
		bullet->Render();
	}
}

void BulletManager::DisableBullets()
{
	for (Bullet* bullet : mBullets)
	{
		if (bullet->GetIsActive())
		{
			bullet->Disable();
		}
	}
}

void BulletManager::FireTimerFunc()
{
	mIsFire = true;
}

void BulletManager::SetIsPlayerBullet(bool isPlayerBullet)
{
	for (Bullet* bullet : mBullets)
	{
		bullet->SetIsPlayerBullet(isPlayerBullet);
	}
}

void BulletManager::Fire(Texture* bulletTexture,Texture* effectTexture, Vector3 firePos, Vector2 direction, Vector2 size, float bulletSpeed, float atk)
{
	if (mIsFire)
	{
		for (Bullet* bullet : mBullets)
		{
			if (!bullet->GetIsActive() && !bullet->GetIsEffect())
			{
				if (!bullet->GetIsEffect())
				{
					bullet->Fire(bulletTexture, effectTexture,firePos, direction, size, bulletSpeed, atk);
					SOUND->Play("fire", 0.5f);
					mIsFire = false;
					mFireTimer->StartTimer();
					return;
				}
			}
		}
	}
}
