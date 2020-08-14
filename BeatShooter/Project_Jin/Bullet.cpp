#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
	:mBulletTexture(nullptr), mIsActive(false), mIsEffect(false), mIsPlayerBullet(false), mBulletSpeed(0)
	, mDirection(Vector2(0.0f, 0.0f)), mAtk(100.0f)

{
	mEffectWorld.scale = { 0.2f,0.2f };
	mWorldBlur = NEW MotionTrailer(3, mWorld, 0.02f, mBulletTexture);
	mActiveTimer = NEW Timer(1.5f, false);
	mActiveTimer->timerFunc = bind(&Bullet::Disable, this);
	mBulletEffectTimer = NEW Timer(0.2f, false);
	mBulletEffectTimer->timerFunc = bind(&Bullet::EffectTimer, this);
//	mEffectTexture = Texture::Add(L"res/bulletEffect.png");
}

Bullet::~Bullet()
{
	SAFE_DELETE(mWorldBlur);
	SAFE_DELETE(mActiveTimer);
	SAFE_DELETE(mBulletEffectTimer);
}

void Bullet::Update()
{
	if (mIsEffect)
	{
		mBulletEffectTimer->Update();
		if (mBulletEffectTimer->curTime < mBulletEffectTimer->limitTime * 0.5f)
		{
			mEffectWorld.scale += {2.0f * DELTATIME, 2.0f * DELTATIME};
		}
		else
		{
			mEffectWorld.scale -= {2.0f * DELTATIME, 2.0f * DELTATIME};
		}
	}
	if (!mIsActive)
	{
		return;
	}

	mWorld.pos += Vector3(mDirection.x * mBulletSpeed * DELTATIME, mDirection.y * mBulletSpeed * DELTATIME, 0);
	mWorld.rot.z = Utility::GetAngle(mDirection) + 0.5f * PI;
	mActiveTimer->Update();
}

void Bullet::LastUpdate()
{

	if (mIsActive)
	{

		mWorldBlur->Update();
		mWorld.Update();
		Collsion();
	}
	if (mIsEffect)
	{
		mEffectWorld.Update();
	}
}

void Bullet::PrevRender()
{

}

void Bullet::Render()
{
	if (mIsEffect)
	{
		mEffectWorld.SetWorld();
		mEffectTexture->Render();
	}

	if (!mIsActive)
	{
		return;
	}

	mWorldBlur->Render();
	mWorld.SetWorld();
	mBulletTexture->Render();
}




void Bullet::Fire(Texture* bulletTexture, Texture* effectTexture, Vector3 firePos, Vector2 direction, Vector2 size, float bulletSpeed, float atk)
{
	mBulletTexture = bulletTexture;
	mEffectTexture = effectTexture;

	mWorld.pos = firePos;
	mWorld.rot.z = Utility::GetAngle(direction);
	mWorld.size = size;
	bulletTexture->SetWorldScale(&mWorld, size);

	mWorldBlur->SetBlurTexture(bulletTexture);
	mWorldBlur->SetBlurWorlds(mWorld);
	mDirection = direction;
	mBulletSpeed = bulletSpeed;
	mAtk = atk;
	mIsActive = true;
	mIsEffect = false;

	mActiveTimer->StartTimer();
}

void Bullet::Disable()
{
	mEffectWorld.pos = mWorld.pos;
	mEffectWorld.rot.z = Utility::Random(0.0f, PI);
	mEffectWorld.scale = { 0.2f,0.2f };
	mIsActive = false;
	mIsEffect = true;
	mBulletEffectTimer->StartTimer();
	SOUND->Play("bulletHit", 0.2f);
	//mWorld = World();
}

void Bullet::Collsion()
{
	if (!mIsActive)
	{
		return;
	}
	if (Utility::Distance(Vector2(mWorld.pos.x, mWorld.pos.y), Vector2(CENTER_X, CENTER_Y)) > COLLSIONSIZE + 50)
	{
		Disable();
	}
	if (mIsPlayerBullet)
	{
		for (Enemy* enemy : GAME_MANAGER->GetEnemyManger()->GetEnemys())
		{
			if (enemy->GetIsAcive())
			{
				if (CollisionObjByCicle(*enemy))
				{
					enemy->HitEnemy(mDirection);
					Disable();
					return;
				}
			}
		}
	}
}

void Bullet::EffectTimer()
{
	mIsEffect = false;
	mBulletEffectTimer->StopTimer();
}
