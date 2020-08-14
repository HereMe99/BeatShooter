#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(Vector3& playerPos, EnemyType enemyType)
	:mPlayerPos(playerPos), mEnemyType(enemyType)
	, mIsMove(true), mIsActive(false), mIsWaring(false), mIsExplosion(false)
{
	switch (mEnemyType)
	{
	case EnemyType::CIRCLE_ENEMY:
		mEnemySpeed = CIRCLE_ENEMY_SPEED;
		mEnemyHp = CIRCLE_ENEMY_HP;
		mEnemyTexture = Texture::Add(L"res/circleEnemy.png");
		mEnemyPatternTimer = NEW Timer(4.0f, false);
		mEnemyPatternTimer->timerFunc = bind(&Enemy::EnemyPattern, this);
		mEnemyPatternTimer->StartTimer();
		mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
		Utility::Nomalize(mEnemyAngle);
		break;
	case EnemyType::TRI_ENEMY:
		mEnemySpeed = TRI_ENEMY_SPEED;
		mEnemyHp = TRI_ENEMY_HP;
		mEnemyTexture = Texture::Add(L"res/triEnemy.png");
		mEnemyPatternTimer = nullptr;
		mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
		Utility::Nomalize(mEnemyAngle);

		break;
	case EnemyType::RECT_ENEMY:
		mEnemySpeed = RECT_ENEMY_SPEED;
		mEnemyHp = RECT_ENEMY_HP;
		mEnemyTexture = Texture::Add(L"res/rectEnemy.png");
		mEnemyPatternTimer = NEW Timer(1.0f, true);
		mEnemyPatternTimer->timerFunc = bind(&Enemy::EnemyPattern, this);
		mEnemyPatternTimer->StartTimer();
		mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
		Utility::Nomalize(mEnemyAngle);

		break;
	case EnemyType::HEXA_ENEMY:
		mEnemySpeed = 1600.0f;
		mEnemyHp = RECT_ENEMY_HP;
		mEnemyTexture = Texture::Add(L"res/hexaEnemy.png");
		mEnemyPatternTimer = NEW Timer(4.0f, true);
		mEnemyPatternTimer->timerFunc = bind(&Enemy::EnemyPattern, this);
		mEnemyPatternTimer->StartTimer();
		mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
		Utility::Nomalize(mEnemyAngle);

		break;
	default:
		break;
	}

	mWorldBlur = NEW MotionTrailer(3, mWorld, 0.01f, mEnemyTexture);
	mEnemyTexture->GetRealSize(&mWorld.size);
	Utility::Nomalize(mEnemyAngle);
	mWorld.pos = { 0,0,0 };

	mExplosionWorld.scale = { 0.1f,0.1f };

	mWaringTexture = Texture::Add(L"res/waring.png");
	mWaringRingTexture = Texture::Add(L"res/waring_ring.png");
	mExplosionTexture = Texture::Add(L"res/explosion.png");

	mWaringTimer = NEW Timer(1.5f, false);
	mExplosionTimer = NEW Timer(0.5f, false);
	mExplosionTimer->timerFunc = bind(&Enemy::ExplosionTimer, this);
	mWaringTimer->timerFunc = bind(&Enemy::WaringTimer, this);

}

Enemy::~Enemy()
{
	EnemyType eType = mEnemyType;
	SAFE_DELETE(mWorldBlur);
	SAFE_DELETE(mEnemyPatternTimer);
	SAFE_DELETE(mWaringTimer);
	SAFE_DELETE(mExplosionTimer);
}

void Enemy::Init()
{
	mIsMove = true;
	mIsActive = false;
	switch (mEnemyType)
	{
	case EnemyType::CIRCLE_ENEMY:
		mEnemySpeed = CIRCLE_ENEMY_SPEED;
		mEnemyHp = CIRCLE_ENEMY_HP;
		mEnemyPatternTimer->StopTimer();
		break;
	case EnemyType::TRI_ENEMY:
		mEnemySpeed = TRI_ENEMY_SPEED;
		mEnemyHp = TRI_ENEMY_HP;
		break;
	case EnemyType::RECT_ENEMY:
		mIsMove = false;
		mEnemySpeed = RECT_ENEMY_SPEED;
		mEnemyHp = RECT_ENEMY_HP;
		mEnemyPatternTimer->StopTimer();
		break;
	case EnemyType::HEXA_ENEMY:
		mEnemySpeed = 1600.0f;
		mEnemyHp = RECT_ENEMY_HP;
		mEnemyPatternTimer->StopTimer();
		break;
	default:
		break;
	}
	mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
	Utility::Nomalize(mEnemyAngle);
}

void Enemy::Update()
{
	if (mIsExplosion)
	{
		if (mExplosionTimer->curTime < mExplosionTimer->limitTime * 0.8f)
		{
			mExplosionWorld.scale += { 5.0f * DELTATIME, 5.0f * DELTATIME };
		}
		else
		{
			mExplosionWorld.scale -= { 6.0f * DELTATIME, 6.0f * DELTATIME };
		}
		mExplosionTimer->Update();
	}

	if (mIsWaring)
	{
		mWaringTimer->Update();
		mWaringWorld.rot.z -= PI * DELTATIME * 2.0f;
	}
	if (!mIsActive)
	{
		return;
	}

	if (CollsionMap() && mEnemyType != EnemyType::TRI_ENEMY)
	{
		mEnemyAngle *= -1;
	}

	if (mIsMove)
	{
		mWorld.pos += {mEnemyAngle.x* mEnemySpeed* DELTATIME, mEnemyAngle.y* mEnemySpeed* DELTATIME, 0};
	}

	switch (mEnemyType)
	{
	case EnemyType::CIRCLE_ENEMY:
		Utility::Nomalize(mEnemyAngle);
		mEnemyPatternTimer->Update();
		break;
	case EnemyType::TRI_ENEMY:
		mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
		Utility::Nomalize(mEnemyAngle);
		mWorld.rot.z += PI * 2.0f * DELTATIME;
		break;
	case EnemyType::RECT_ENEMY:
		if (!mIsMove)
		{
			mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
			Utility::Nomalize(mEnemyAngle);
			mWorld.rot.z = Utility::GetAngle(mEnemyAngle);
		}
		else
		{
			mWorld.rot.z += PI * 10.0f * DELTATIME;
		}
		mEnemyPatternTimer->Update();
		break;
	case EnemyType::HEXA_ENEMY:
		if (!mIsMove)
		{
			mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
			Utility::Nomalize(mEnemyAngle);
			mWorld.rot.z = Utility::GetAngle(mEnemyAngle);
		}
		else
		{
			mWorld.rot.z += PI * 10.0f * DELTATIME;
		}
		mEnemyPatternTimer->Update();
		break;
	default:
		break;
	}

}

void Enemy::LastUpdate()
{
	mExplosionWorld.Update();
	if (mIsWaring)
	{
		mWaringWorld.Update();
	}
	if (!mIsActive)
	{
		return;
	}

	mWorld.Update();
	mWorldBlur->Update();
}

void Enemy::PrevRender()
{
	if (!mIsActive)
	{
		return;
	}
}

void Enemy::Render()
{
	if (mIsExplosion)
	{
	
		mExplosionWorld.SetWorld();
		mExplosionTexture->Render();

	}
	if (mIsWaring)
	{
		mWaringWorld.SetWorld();
		mWaringRingTexture->Render();
		mWaringTexture->Render();
	}
	if (!mIsActive)
	{
		return;
	}

	mWorldBlur->Render();
	mWorld.SetWorld();
	mEnemyTexture->Render();

}

bool Enemy::CollsionMap()
{
	if (Utility::Distance(Vector2(mWorld.pos.x, mWorld.pos.y), Vector2(CENTER_X, CENTER_Y)) > COLLSIONSIZE)
	{
		Vector2 tempVector2(mWorld.pos.x - CENTER_X, mWorld.pos.y - CENTER_Y);
		float outDistance = Utility::Distance(Vector2(mWorld.pos.x, mWorld.pos.y), Vector2(CENTER_X, CENTER_Y)) - COLLSIONSIZE;
		Utility::Nomalize(tempVector2);
		tempVector2 *= outDistance;
		Vector3 tempVector3(tempVector2.x, tempVector2.y, 0);
		mWorld.pos -= tempVector3;
		return true;
	}

	return false;
}

void Enemy::EnemyPattern()
{
	switch (mEnemyType)
	{
	case EnemyType::CIRCLE_ENEMY:
		Utility::SetAngle(Utility::Random(0.0f, PI), &mEnemyAngle);
		Utility::Nomalize(mEnemyAngle);
		break;
	case EnemyType::RECT_ENEMY:
		mEnemyAngle = { mPlayerPos.x - mWorld.pos.x , mPlayerPos.y - mWorld.pos.y };
		Utility::Nomalize(mEnemyAngle);
		//mWorld.rot.z = Utility::GetAngle(mEnemyAngle);
		if (mIsMove)
		{
			mIsMove = false;
		}
		else
		{
			mIsMove = true;
		}
		break;
	case EnemyType::HEXA_ENEMY:
		Utility::SetAngle(Utility::Random(0.0f, PI), &mEnemyAngle);
		Utility::Nomalize(mEnemyAngle);
		//mWorld.rot.z = Utility::GetAngle(mEnemyAngle);
		if (mIsMove)
		{
			mIsMove = false;
		}
		else
		{
			mIsMove = true;
		}
		break;

	default:
		break;
	}
}

void Enemy::ActiveEnemy(Vector3 activePos)
{
	Init();
	mWorld.pos = activePos;
	mWorldBlur->SetBlurWorlds(mWorld);
	//mWaringWorld = mWorld;
	mWaringWorld.pos = activePos;
	mWaringWorld.rot.z = 0.0f;
	mIsWaring = true;
	mIsMove = false;
	mWaringTimer->StartTimer();
}

void Enemy::HitEnemy(Vector2 collsionDir)
{
	mEnemyHp--;
	if (mEnemyHp <= 0)
	{
		SOUND->Play("explosion", 0.3f);
		GAME_MANAGER->AddScore(100);
		DieEnemy();
	}
	mWorld.pos += {collsionDir.x* NUCKBACK_SPEED* DELTATIME, collsionDir.y* NUCKBACK_SPEED* DELTATIME, 0};
}

void Enemy::DieEnemy()
{
	if (mIsActive)
	{
		mExplosionWorld.pos = mWorld.pos;
		mExplosionTimer->StartTimer();
		mIsExplosion = true;
	}
	mIsWaring = false;
	GAME_MANAGER->CameraShake();
	GAME_MANAGER->GetCamera().CameraShake(5);
	Init();
}

void Enemy::DisableEnemy()
{
	mIsWaring = false;
	GAME_MANAGER->CameraShake();
	GAME_MANAGER->GetCamera().CameraShake(5);
	Init();
}

void Enemy::WaringTimer()
{
	Init();
	mIsWaring = false;
	mIsMove = true;
	mIsActive = true;
	mWaringWorld = World();
	if (mEnemyType != EnemyType::TRI_ENEMY)
	{
		mEnemyPatternTimer->StartTimer();
	}
}

void Enemy::ExplosionTimer()
{
	mIsExplosion = false;
	mExplosionWorld.scale = { 0.1f,0.1f };
}


