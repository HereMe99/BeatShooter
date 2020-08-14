#include "stdafx.h"
#include "MainGame.h"


MainGame::MainGame()
{
	Shader::Create();
	SoundManager::Create();
	//SCENE->AddScene("sideViewGame", NEW SVG);
	SCENE->AddScene("play", NEW PlayScene);
	SCENE->AddScene("title", NEW TitleScene);
	SCENE->AddScene("end", NEW EndScene);
	SCENE->ChangeScene("title");
}


MainGame::~MainGame()
{
	Device::GetInstance()->Release();
	InputManager::GetInstance()->Release();
	EffectManager::GetInstance()->Release();
	SOUND->Release();
	Texture::Release();
	Shader::Delete();
	XML->Release();
	SCENE->Release();
}

void MainGame::Update()
{
	EFFECT->Update();
	TIME->Update();
	SCENE->Update();
	SOUND->Update();
	SCENE->LastUpdate();
}

void MainGame::Render()
{
	SCENE->Render();
	TIME->Render();
}

void MainGame::Release()
{
}
