#include "stdafx.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
	mBgTexture = Texture::Add(L"res/boss_enemy.png");
	mTitleTexture = Texture::Add(L"res/title.png");
	mStartBtnTexture = Texture::Add(L"res/startBtn.png");
	mBgLightTexture = Texture::Add(L"res/bgLight.png");
	mCursorTexture = Texture::Add(L"res/cursor.png");

	mStartBtn = NEW Button(mStartBtnTexture, { CENTER_X,WIN_HEIGHT * 0.75f }, {540.0f,280.0f}, bind(&TitleScene::GoPlayScene, this));
}

TitleScene::~TitleScene()
{
	SAFE_DELETE(mStartBtn);
}

void TitleScene::Init()
{
	camera->SetScale({ 1.0f,1.0f,1.0f });
	mBgLightWorld.scale = { 2.0f,2.0f };
	mTitleWorld.pos = { CENTER_X,CENTER_Y * 0.5f,0 };
	//mBgLightWorld.pos = { WIN_WIDTH * 0.1f,WIN_HEIGHT * 0.8f,0 };
	mBgWorld.pos = { CENTER_X*0.4f,WIN_HEIGHT*0.7f,0 };
	mBgWorld.scale = { 0.8f,0.8f };
	mBgLightWorld.pos = mBgWorld.pos;
}

void TitleScene::Update()
{
	mStartBtn->Update();
	mCursorWorld.pos = Vector3(camera->GetMousePos().x, camera->GetMousePos().y, 0);

	mBgLightWorld.rot.z -= DELTATIME;
	mBgWorld.rot.z += DELTATIME;
	mCursorWorld.rot.z += DELTATIME;
}

void TitleScene::LastUpdate()
{
	mTitleWorld.Update();
	mBgLightWorld.Update();
	mCursorWorld.Update();
	mBgWorld.Update();
	camera->Update();
}

void TitleScene::Render()
{
	mBgLightWorld.SetWorld();
	mBgLightTexture->Render();

	mBgWorld.SetWorld();
	mBgTexture->Render();

	mTitleWorld.SetWorld();
	mTitleTexture->Render();
	
	mStartBtn->Render();
	
	mCursorWorld.SetWorld();
	mCursorTexture->Render();
}

void TitleScene::Release()
{
}

void TitleScene::GoPlayScene()
{
	//SOUND->DspSetHighpass(0.0f);
	GAME_MANAGER->SetBgmHighPass(false);
	
	SCENE->ChangeScene("play");

}
