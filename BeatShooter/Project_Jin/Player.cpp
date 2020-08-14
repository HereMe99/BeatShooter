#include "stdafx.h"
#include "Player.h"

Player::Player(Camera& camera, Texture* playerTexture, Texture* playerBulletTexture)
	:mCamera(camera), mPlayerSpeed(1200.0f), mPlayerHP(MAX_PLAYER_HP)
	, mPlayerATK(30.0f), mPlayerBulletSpeed(1500.0f), mBulletAngle(Vector2(0, -1))
	, mIsActive(true), mIsDie(false), mIsPlayerHitFX(false)
	, mPlayerTexture(playerTexture)
	, mPlayerBulletTexture(playerBulletTexture)
	, mPlayerBulletEffectTexture(Texture::Add(L"res/bulletEffect.png"))
	, mExplosionTexture(Texture::Add(L"res/bg.png"))
	, mBulletManager(NEW BulletManager(100))
	, mPlayerHitTimer(NEW Timer(1.0f, false))
	, mMotionTrailer(NEW MotionTrailer(5, mWorld, 0.02f, playerTexture))
{
	mWorld.pos = { CENTER_X,CENTER_Y ,0 };
	mPlayerTexture->GetRealSize(&mWorld.size);
	mPlayerTexture->SetWorldScale(&mWorld, { 100,100 });
	D3DXVec2Normalize(&mBulletAngle, &mBulletAngle);
	mPlayerHitTimer->timerFunc = bind(&Player::HitPlayerTimer, this);
	mBulletManager->SetIsPlayerBullet(true);
	
	for (int index = 0; index < MAX_PLAYER_HP; index++)
	{
		World world;
		world.pos = { (CENTER_X - 60) + index * 60,WIN_HEIGHT * 0.9f,0 };
		world.scale = { 0.5f,0.5f };
		mPlayerLifeWorlds.push_back(world);
	}
}

Player::~Player()
{
	SAFE_DELETE(mMotionTrailer);
	SAFE_DELETE(mBulletManager);
	SAFE_DELETE(mPlayerHitTimer);
}

void Player::Init()
{
	mIsPlayerHitFX = false;
	mIsActive = true;
	mIsDie = false;
	mPlayerHP = MAX_PLAYER_HP;
}

void Player::Update()
{
	for (int index = 0; index < MAX_PLAYER_HP; index++)
	{
		mPlayerLifeWorlds[index].Update();
	}

	if (mIsPlayerHitFX && mPlayerHP >= 0)
	{
		mPlayerHitTimer->Update();
		mPlayerExplosionWorld.scale.x += 1.5f * DELTATIME;
		mPlayerExplosionWorld.scale.y += 1.5f * DELTATIME;
		mPlayerExplosionWorld.Update();
		if (mPlayerHitTimer->curTime > 0.25f)
		{
			mIsActive = true;
		}
	}

	mBulletManager->Update();
	if (!mIsActive)
	{
		return;
	}
	if (mIsDie)
	{
		return;
	}
	mBulletAngle = { mCamera.GetMousePos().x - mWorld.pos.x ,mCamera.GetMousePos().y - mWorld.pos.y };
	mWorld.rot.z = Utility::GetAngle(mBulletAngle) + 0.5f * PI;
	Utility::Nomalize(mBulletAngle);
	CheckKey();
}

void Player::LastUpdate()
{
	if (Utility::Distance(Vector2(mWorld.pos.x, mWorld.pos.y), Vector2(CENTER_X, CENTER_Y)) > COLLSIONSIZE)
	{
		Vector2 tempVector2(mWorld.pos.x - CENTER_X, mWorld.pos.y - CENTER_Y);
		float outDistance = Utility::Distance(Vector2(mWorld.pos.x, mWorld.pos.y), Vector2(CENTER_X, CENTER_Y)) - COLLSIONSIZE;
		Utility::Nomalize(tempVector2);
		tempVector2 *= outDistance;
		Vector3 tempVector3(tempVector2.x, tempVector2.y, 0);
		mWorld.pos -= tempVector3;
	}

	mWorld.Update();
	mMotionTrailer->Update();
	mBulletManager->LastUpdate();
}

void Player::PrevRender()
{

}

void Player::Render()
{
	for (int index = 0; index < mPlayerHP; index++)
	{
		World::InitView();
		World::InitWorld();
		mPlayerLifeWorlds[index].SetView();
		mPlayerTexture->Render();
		mPlayerTexture->Render();
		mPlayerTexture->Render();
		GAME_MANAGER->GetCamera().SetView();
	}

	if (mIsPlayerHitFX)
	{
		mPlayerExplosionWorld.SetWorld();
		mExplosionTexture->Render();
	}
	mBulletManager->Render();

	if (!mIsActive)
	{
		return;
	}
	if (mIsDie)
	{
		return;
	}

	mMotionTrailer->Render();

	mWorld.SetWorld();
	mPlayerTexture->Render();
}

void Player::CheckKey()
{
	if (KEY_PRESS('W'))
	{
		mWorld.pos.y -= mPlayerSpeed * DELTATIME;
	}
	if (KEY_PRESS('S'))
	{
		mWorld.pos.y += mPlayerSpeed * DELTATIME;
	}
	if (KEY_PRESS('A'))
	{
		mWorld.pos.x -= mPlayerSpeed * DELTATIME;
	}
	if (KEY_PRESS('D'))
	{
		mWorld.pos.x += mPlayerSpeed * DELTATIME;
	}
	if (KEY_PRESS(VK_LBUTTON))
	{
		mBulletManager->Fire(mPlayerBulletTexture, mPlayerBulletEffectTexture, mWorld.pos
			, mBulletAngle, { 64,64 }, mPlayerBulletSpeed, mPlayerATK);
	}
}

void Player::HitPlayer()
{
	mPlayerHP--;

	GAME_MANAGER->HitPlayer();
	SOUND->Play("playerExplosion", 0.4f);


	if (mPlayerHP < 0)
	{
		DiePlayer();
		GAME_MANAGER->GetEnemyManger()->DisableEnemys();
	}
	else
	{
		GAME_MANAGER->GetEnemyManger()->KillEnemys();
	}
	ActivePlayer();
}

void Player::HitPlayerTimer()
{
	if (!mIsDie)
	{
		mIsActive = true;
	}
	mIsPlayerHitFX = false;
	mPlayerExplosionWorld.scale = { 0.0f,0.0f };
}


void Player::DiePlayer()
{
	SCENE->ChangeScene("end");
	mIsDie = true;
	GAME_MANAGER->SetBgmHighPass(true);
	mIsActive = false;
	GAME_MANAGER->GetEnemyManger()->SetIsActive(false);
}

void Player::ActivePlayer()
{
	SOUND->Play("start", 0.5f);
	mWorld.pos = { CENTER_X,CENTER_Y,0 };
	mPlayerExplosionWorld.pos = mWorld.pos;
	mPlayerExplosionWorld.scale = { 0.0f,0.0f };
	mPlayerHitTimer->StartTimer();
	mIsPlayerHitFX = true;
	mBulletManager->DisableBullets();
	mIsActive = false;
}
