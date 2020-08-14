#pragma once
class Texture;
class Camera
{
private:
	D3DXMATRIX mViewMat;
	D3DXMATRIX mProjectionMat;
	D3DXVECTOR2 mCenterPos;
	D3DXVECTOR2 mCameraPos;
	D3DXVECTOR2 mFocusCenterPos;
	D3DXVECTOR3 mScale;

	bool mIsShakingLeft;
	bool mIsShake;
	UINT mCurShakeCount;
	UINT mMaxShakeCount;
	Timer* mShakeTimer;
public:
	Camera();
	~Camera();

	void Update();

	void Focusing(D3DXVECTOR2 focusCenterPos);
	void FocusingX(float posX);
	void FocusingY(float posY);
	void SetProjection();
	void SetScale(D3DXVECTOR3 scale);
	void SetView() const;
	Vector2 GetMousePos() const;
	void CameraShake(UINT shakeCount);
	void Shake();

	inline Vector2 GetFocusCenterPos() const;
	inline D3DXVECTOR3 GetScale() const;

	inline void SetCenterPos(const D3DXVECTOR2 centerPos);
};

Vector2 Camera::GetFocusCenterPos() const
{
	return mFocusCenterPos;
}

D3DXVECTOR3 Camera::GetScale() const
{
	return mScale;
}

void Camera::SetCenterPos(const D3DXVECTOR2 centerPos)
{
	mCenterPos = centerPos;
}