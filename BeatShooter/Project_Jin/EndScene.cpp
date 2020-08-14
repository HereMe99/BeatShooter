#include "stdafx.h"
#include "EndScene.h"

EndScene::EndScene()
{
	mGameoverTexture = Texture::Add(L"res/gameover.png");
	mBgTexture = Texture::Add(L"res/face.png");
	mRestartBtnTexture = Texture::Add(L"res/restartBtn.png");
	mTitleBtnTexture = Texture::Add(L"res/titleBtn.png");
	mBgLightTexture = Texture::Add(L"res/wave_waring.png");
	mCursorTexture = Texture::Add(L"res/cursor.png");

	mGameoverWorld.pos = { CENTER_X,CENTER_Y * 0.5f,0 };
	mBgWorld.pos = { CENTER_X,CENTER_Y,0 };
	mBgWorld.scale = { 1.5f,1.5f };
	mBgLightWorld.pos = mBgWorld.pos;
	mBgLightWorld.scale = { 2.0f,2.0f };

	mRestartBtn = NEW Button(mRestartBtnTexture, { CENTER_X+340.0f,WIN_HEIGHT * 0.8f }, { 400.0f,240.0f }, bind(&EndScene::GoPlayScene, this));
	mTitleBtn = NEW Button(mTitleBtnTexture, { CENTER_X - 340.0f,WIN_HEIGHT * 0.8f }, { 400.0f,240.0f }, bind(&EndScene::GoTitleScene, this));

}

EndScene::~EndScene()
{
	SAFE_DELETE(mRestartBtn);
	SAFE_DELETE(mTitleBtn);
}

void EndScene::Init()
{
	camera->SetScale({ 1.0f,1.0f,1.0f });
	
	mBgLightWorld.rot.z = 0.0f;
}

void EndScene::Update()
{

	GAME_MANAGER->Update();
	mCursorWorld.pos = Vector3(camera->GetMousePos().x, camera->GetMousePos().y, 0);
	mBgLightWorld.rot.z -= DELTATIME;
	mCursorWorld.rot.z += DELTATIME;
	mRestartBtn->Update();
	mTitleBtn->Update();
}

void EndScene::LastUpdate()
{
	mBgLightWorld.Update();
	mCursorWorld.Update();
	mBgWorld.Update();
	mGameoverWorld.Update();
	GAME_MANAGER->LastUpdate();
	camera->Update();
}

void EndScene::Render()
{
	
	mBgLightWorld.SetWorld();
	mBgLightTexture->Render();

	//mBgWorld.SetWorld();
	//mBgTexture->Render();

	GAME_MANAGER->Render();
	World::InitView();
	World::InitWorld();
	mRestartBtn->Render();
	mTitleBtn->Render();


	//World::InitView();
	//World::InitWorld();
	mGameoverWorld.SetWorld();
	mGameoverTexture->Render();


	mCursorWorld.SetWorld();
	mCursorTexture->Render();
}

void EndScene::Release()
{
}

void EndScene::GoPlayScene()
{
	GAME_MANAGER->SetBgmHighPass(false);

	SCENE->ChangeScene("play");
}

void EndScene::GoTitleScene()
{
	GAME_MANAGER->SetBgmHighPass(true);

	SCENE->ChangeScene("title");
}
