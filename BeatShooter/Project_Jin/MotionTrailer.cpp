#include "stdafx.h"
#include "MotionTrailer.h"

MotionTrailer::MotionTrailer(UINT blurCount, World& focusWorld
	, float blurSec, Texture* texture)
	:BLUR_COUNT(blurCount),mFocusWorld(focusWorld)
	,mShader(Shader::GetShader(ShaderType::ALPHAON))
	,mBlurTimer(NEW Timer(blurSec,true))
	,mBlurTexture(texture)
{
	mBlurTimer->timerFunc = bind(&MotionTrailer::TimerFunc, this);
	for (UINT blurCount = 0; blurCount < BLUR_COUNT; blurCount++)
	{
		mBlurWorlds.push_back(World(focusWorld));
	}
	mBlurTimer->isActive = true;
}

MotionTrailer::~MotionTrailer()
{
	SAFE_DELETE(mBlurTimer);
	mBlurWorlds.clear();
}

void MotionTrailer::Update()
{
	mBlurTimer->Update();
	
	for (UINT blurCount = 0; blurCount < BLUR_COUNT; blurCount++)
	{
		mBlurWorlds[blurCount].Update();
	}
}

void MotionTrailer::Render()
{
	Vector2 imageSize = {0,0};
	mBlurTexture->GetRealSize(&imageSize);
	mShader->SetFloatArray("imageSize", (float*)&imageSize,2);
	mShader->SetTexture("map", mBlurTexture->GetDxTexture());
	
	for (UINT blurCount = 0; blurCount < BLUR_COUNT; blurCount++)
	{
		float alpha = (blurCount + 1) * (1.0f / BLUR_COUNT);
		mShader->SetFloat("alpha", alpha);
		mShader->Begin(nullptr, NULL);
		mShader->BeginPass(1);
		mBlurWorlds[blurCount].SetWorld();
		mBlurTexture->Render();
		mShader->EndPass();
		mShader->End();
	}
}

void MotionTrailer::TimerFunc()
{
	for (UINT blurCount = 0; blurCount < BLUR_COUNT; blurCount++)
	{
		if (blurCount == BLUR_COUNT - 1)
		{
			mBlurWorlds[blurCount] = mFocusWorld;
		}	
		else
		{	
			mBlurWorlds[blurCount] = mBlurWorlds[blurCount + 1];
		}
	}
}



void MotionTrailer::SetBlurWorlds(World world)
{
	for (UINT blurCount = 0; blurCount < BLUR_COUNT; blurCount++)
	{
		mBlurWorlds[blurCount] = world;
	}
}
