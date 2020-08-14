#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	mCenterPos = { WIN_WIDTH * 0.5f,WIN_HEIGHT * 0.5f };
	mCameraPos = { 0,0 };
	mFocusCenterPos = mCameraPos + mCenterPos;
	mScale = { 0.5f,0.5f,1.0f };
	//mScale = { 1,1,1 };
	D3DXMatrixTranslation(&mViewMat, mCameraPos.x, mCameraPos.y, 0);

	mIsShake = false;
	mIsShakingLeft = false;
	mShakeTimer = NEW Timer(0.02f, true);
	mShakeTimer->timerFunc = bind(&Camera::Shake, this);
}


Camera::~Camera()
{
	SAFE_DELETE(mShakeTimer);
}

void Camera::Update()
{
	if (mIsShake)
	{
		mShakeTimer->Update();
	}
	if (KEY_PRESS(VK_CONTROL))
	{
		if (KEY_PRESS(VK_RIGHT))
			mCameraPos.x += 200.0f * DELTATIME;

		if (KEY_PRESS(VK_LEFT))
			mCameraPos.x -= 200.0f * DELTATIME;

		if (KEY_PRESS(VK_UP))
			mCameraPos.y -= 200.0f * DELTATIME;

		if (KEY_PRESS(VK_DOWN))
			mCameraPos.y += 200.0f * DELTATIME;
	}
	//D3DXMatrixTranslation(&mViewMat, mCameraPos.x, mCameraPos.y ,0);

	D3DXMATRIX center;
	D3DXMatrixTranslation(&center, mCenterPos.x, mCenterPos.y, 0);
	D3DXMATRIX invCenter;
	D3DXMatrixInverse(&invCenter, nullptr, &center);

	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, mCameraPos.x, mCameraPos.y, 0);

	D3DXMATRIX S, scaleMat;
	D3DXMatrixScaling(&scaleMat, mScale.x, mScale.y, mScale.z);
	S = invCenter * scaleMat * center;

	mViewMat = S * T;

	DEVICE->SetTransform(D3DTS_VIEW, &mViewMat);
}

void Camera::Focusing(D3DXVECTOR2 focusCenterPos)
{
	D3DXVECTOR2 direction = D3DXVECTOR2(-(focusCenterPos.x - WIN_WIDTH * 0.5f) * mScale.x, -(focusCenterPos.y - WIN_HEIGHT * 0.5f) * mScale.y) - mCameraPos;
	const float speed = D3DXVec2Length(&direction) * 2;
	D3DXVECTOR2 normal = direction;
	D3DXVec2Normalize(&direction, &normal);
	focusCenterPos -= direction * DELTATIME * speed;
	mCameraPos += direction * DELTATIME * speed;
}

void Camera::FocusingX(const float posX)
{
	const float speed = -((posX - WIN_WIDTH * 0.5f) * mScale.x) - mCameraPos.x;
	mCameraPos.x += speed * DELTATIME * 2;
}
void Camera::FocusingY(const float posY)
{
	const float speed = -((posY - WIN_WIDTH * 0.5f) * mScale.y) - mCameraPos.y;
	mCameraPos.y += speed * DELTATIME * 2;
}

void Camera::SetProjection()
{
	D3DXMatrixOrthoOffCenterLH(&mProjectionMat, 0.0f, WIN_WIDTH, WIN_HEIGHT, 0.0f, -1, 10);
	DEVICE->SetTransform(D3DTS_PROJECTION, &mProjectionMat);
	DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void Camera::SetScale(const D3DXVECTOR3 scale)
{
	this->mScale = scale;
}

void Camera::SetView() const
{
	DEVICE->SetTransform(D3DTS_VIEW, &mViewMat);
}

Vector2 Camera::GetMousePos() const
{
	Vector3 origPos = {0,0,0};
	D3DVIEWPORT9 viewPort;
	DEVICE->GetViewport(&viewPort);
	D3DXVec3Unproject(&origPos, &D3DXVECTOR3(mousePos.x, mousePos.y,0), &viewPort, &mProjectionMat, &mViewMat,NULL);
	return { origPos.x,origPos.y };
}

void Camera::CameraShake(const UINT shakeCount)
{
	mMaxShakeCount = shakeCount;
	mIsShake = true;
	mShakeTimer->StartTimer();
}

void Camera::Shake()
{
	if (mCurShakeCount > mMaxShakeCount)
	{
		mShakeTimer->StopTimer();
		mCurShakeCount = 0;
		mMaxShakeCount = 0;
		mIsShake = false;
	} 
	else
	{
		mCurShakeCount++;
		if (mIsShakingLeft)
		{
			mCameraPos.x -= Utility::Random(5, 10);
			mIsShakingLeft = false;
		}
		else
		{
			mCameraPos.x += Utility::Random(5, 10);
			mIsShakingLeft = true;
		}
	}
}




 