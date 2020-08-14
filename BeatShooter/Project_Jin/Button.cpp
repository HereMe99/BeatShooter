#include "stdafx.h"
#include "Button.h"


Button::Button(Texture* texture, D3DXVECTOR2 pos, D3DXVECTOR2 size, function<void()> event)
	:mTexture(texture), Event(event), color(1, 1, 1, 1),
	mIsDown(false), mState(Button::BtnState::NOMAL),shader(nullptr)
{

	mWorld.pos = { pos.x,pos.y,0 };
	mWorld.size = size;
	mCollisionWorld.size = mWorld.size;
	mTexture->SetWorldScale(&mWorld, size);
	D3DXCreateEffectFromFile(DEVICE,
		L"CharacterShader.hlsl",
		NULL, NULL, D3DXSHADER_DEBUG, NULL, &shader, NULL);
}

Button::~Button()
{
}

void Button::Update()
{
	if (mWorld.parentsWorld != nullptr)
	{

	mCollisionWorld.pos = { mWorld.parentsWorld->_41 + mWorld.pos.x
		, mWorld.parentsWorld->_42 + mWorld.pos.y
		, mWorld.parentsWorld->_43 + mWorld.pos.z };
	}
	else
	{
		mCollisionWorld = mWorld;
	}

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (!mIsDown)
		{
			mIsDown = true;
			if (mCollisionWorld.IsCollision(mousePos))
			{
				mState = BtnState::DOWN;
				color = { 0.3f,0.3f,0.3f,1.0f };
			}
		}
	}
	else
	{
		if (mIsDown)
		{
			mIsDown = false;
			if (mCollisionWorld.IsCollision(mousePos))
			{
				if (mState == BtnState::DOWN || mState == BtnState::DOWN_OUT)
				{
					Event();
				}
			}
			else
			{
				mState = BtnState::NOMAL;
				color = { 1,1,1,1 };
			}
		}
		else
		{
			if (mCollisionWorld.IsCollision(mousePos))
			{
				mState = BtnState::OVER;
				color = { 0.8f,0.8f,0.8f,1.0f };
			}
			else
			{
				mState = BtnState::NOMAL;
				color = { 1,1,1,1 };
			}
		}
	}

	mWorld.Update();
	mCollisionWorld.Update();
}

void Button::Render()
{
//	mWorld.SetWorld();
	shader->SetTexture("map", mTexture->GetDxTexture());
	shader->SetFloatArray("color", (float*)&color, 4);
	shader->Begin(NULL, NULL);
	shader->BeginPass(2);
	mWorld.SetWorld();
	mTexture->Render();
	shader->EndPass();
	shader->End();


}
