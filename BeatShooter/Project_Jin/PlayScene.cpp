#include "stdafx.h"
#include "PlayScene.h"

PlayScene::PlayScene()
	:BLUR_COUNT(5), mRenderCount(0), mShader(Shader::GetShader(ShaderType::BLUR))
{
	mColorState = ColorState::WHITE;

	mBgWorld.scale = { 1.5f,1.5f };
	mBgLightWorld.scale = { 2.0f,2.0f };
	mBgWorld.pos = { CENTER_X,CENTER_Y,0 };
	mBgLightWorld = mBgWorld;
	mCursorWorld.scale = { 1.5f,1.5f };
	mGameoverWorld.pos = { CENTER_X,CENTER_Y * 0.5f,0 };

	mCursorTexture = Texture::Add(L"res/cursor.png");
	mBgBackTexture = Texture::Add(L"res/bgBack.png");
	mBgTexture = Texture::Add(L"res/bg.png");
	mBgLightTexture = Texture::Add(L"res/bgLight.png");
	mGameoverTexture = Texture::Add(L"res/gameover.png");
	mBgTexture->GetRealSize(&mBgSize);
	GameManager::Create(*camera, mBgWorld,mBgLightWorld);
	mCursorBlur = NEW MotionTrailer(5, mCursorWorld, 0.001f, mCursorTexture);
	mRenderTarget = NEW RenderTarget(1920, 1080);
	mShader = Shader::GetShader(ShaderType::BLUR);
	mColorShader = Shader::GetShader(ShaderType::ALPHAON);
	mColor = { 0.8f,0.3f,0.3f,1 };
}

PlayScene::~PlayScene()
{
	GAME_MANAGER->Release();
	SAFE_DELETE(mCursorBlur);
	SAFE_DELETE(mRenderTarget);
}

void PlayScene::Init()
{
	GAME_MANAGER->Init();
}

void PlayScene::Update()
{
	ColorUpdate();
	if (KEY_PRESS(VK_LBUTTON))
	{
		mCursorWorld.scale -= {DELTATIME, DELTATIME};
		if (mCursorWorld.scale.x <= 1.5f)
			mCursorWorld.scale = { 1.5f,1.5f };
	}
	else
	{
		mCursorWorld.scale += {DELTATIME, DELTATIME};
		if (mCursorWorld.scale.x >= 2.0f)
			mCursorWorld.scale = { 2.0f,2.0f };
	}
	mCursorWorld.pos = Vector3(camera->GetMousePos().x, camera->GetMousePos().y, 0);
	mCursorWorld.rot.z = Utility::GetAngle(mCursorWorld.pos - GAME_MANAGER->GetPlayerWorld().pos);
	mBgLightWorld.rot.z -= 2 * DELTATIME;
	GAME_MANAGER->Update();
}

void PlayScene::LastUpdate()
{
	mCursorWorld.Update();
	mBgWorld.Update();
	mBgLightWorld.Update();
	GAME_MANAGER->LastUpdate();
	GAME_MANAGER->ObjCollsion();


	camera->Focusing({ GAME_MANAGER->GetPlayerWorld().pos.x,GAME_MANAGER->GetPlayerWorld().pos.y });
	camera->Update();
	mCursorBlur->Update();


	mRenderTarget->BeginScene();
	mBgWorld.SetWorld();
	mColorShader->SetTexture("map", mBgBackTexture->GetDxTexture());
	mColorShader->SetFloatArray("color", (float*)&mColor, 4);
	mColorShader->Begin(NULL, NULL);
	mColorShader->BeginPass(2);
	mBgBackTexture->Render();
	mColorShader->EndPass();
	mColorShader->End();
	mColorShader->SetTexture("map", mBgTexture->GetDxTexture());
	mColorShader->SetFloatArray("color", (float*)&mColor, 4);
	mColorShader->Begin(NULL, NULL);
	mColorShader->BeginPass(2);
	mBgTexture->Render();
	mColorShader->EndPass();
	mColorShader->End();

	mBgLightWorld.SetWorld();
	mBgLightTexture->Render();
	GAME_MANAGER->Render();
	mCursorBlur->Render();
	mCursorWorld.SetWorld();
	mCursorTexture->Render();
	mRenderTarget->EndScene();

	mRenderTarget->Update();
}

void PlayScene::Render()
{
	if (GAME_MANAGER->GetIsShake())
	{
		mShader->SetTexture("map", mRenderTarget->GetTexture());
		mShader->SetFloatArray("imageSize", (float*)&Vector2(WIN_WIDTH, WIN_HEIGHT), 2);
		mShader->SetFloatArray("power", (float*)&Vector2(3, 2), 2);
		mShader->Begin(NULL, NULL);
		mShader->BeginPass(4);
		//World::InitWorld();
		World::InitView();
		mRenderTarget->Render();
		camera->SetView();
		mShader->EndPass();
		mShader->End();
	}
	else
	{
		//World::InitWorld();
		World::InitView();
		mRenderTarget->Render();
		camera->SetView();
	}

}

void PlayScene::Release()
{
}

void PlayScene::ColorUpdate()
{
	switch (mColorState)
	{
	case ColorState::WHITE:
			mColorState = ColorState::RED;
		break;
	case ColorState::RED:
		mColor.r += 0.1f * DELTATIME;
		if (mColor.g > 0.3f)
		{
			mColor.g -= 0.1f * DELTATIME;
		}
		if (mColor.b > 0.3f)
		{
			mColor.b -= 0.1f * DELTATIME;
		}
		
		if (mColor.r > 0.8f)
		{
			//mColor = { 0.7f,0.3f,0.3f,1 };
			mColorState = ColorState::GREEN;
		}
		break;
	case ColorState::GREEN:
		mColor.g += 0.1f * DELTATIME;
		if (mColor.r > 0.3f)
		{
			mColor.r -= 0.1f * DELTATIME;
		}
		if (mColor.b > 0.3f)
		{
			mColor.b -= 0.1f * DELTATIME;
		}
		if (mColor.g > 0.8f)
		{
			//mColor = { 0.3f,0.7f,0.3f,1 };
			mColorState = ColorState::BLUE;
		}
		break;
	case ColorState::BLUE:
		mColor.b += 0.1f * DELTATIME;
		if (mColor.g > 0.3f)
		{
			mColor.g -= 0.1f * DELTATIME;
		}
		if (mColor.r > 0.3f)
		{
			mColor.r -= 0.1f * DELTATIME;
		}
		
		if (mColor.b > 0.8f)
		{
			//mColor = { 0.3f,0.3f,0.7f,1 };
			mColorState = ColorState::RED;
		}
		break;
	default:
		break;
	}
}
