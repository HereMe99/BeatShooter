#pragma once
enum class Dir
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE
};
struct FRect
{
	float left;
	float top;
	float right;
	float bottom;

	D3DXVECTOR2 centerPos;
	D3DXVECTOR2 size;
	D3DXVECTOR2 halfSize;

	FRect();
	FRect(float left, float top, float right, float bottom);
	FRect(D3DXVECTOR2 centerPos, D3DXVECTOR2 size);
	FRect(D3DXVECTOR2 leftTopPos, float width, float height);

	void InitSize(D3DXVECTOR2 size);

	void MoveRect(float distance, Dir dir);
	Dir IsCollisionPush(FRect* rect);
	bool IsCollision(D3DXVECTOR2 pos);

	void Init(D3DXVECTOR2 centerPos);
	void Init(float left, float top, float right, float bottom);
	void InitRect(D3DXVECTOR2 centerPos, D3DXVECTOR2 size);
	void SetCenterPosX(float centerPosX);
	void SetCenterPosY(float centerPosY);

};

