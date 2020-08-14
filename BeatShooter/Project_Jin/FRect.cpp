#include "stdafx.h"
#include "FRect.h"



FRect::FRect()
{
	left = 0;
	right = 0;
	top = 0;
	bottom = 0;

	centerPos.x = 0;
	centerPos.y = 0;

	size.x = 0;
	size.y = 0;
}

FRect::FRect(float left, float top, float right, float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	size.x = right - left;
	size.y = bottom - top;
	halfSize.x = size.x*0.5f;
	halfSize.y = size.y * 0.5f;
	centerPos.x = left + halfSize.x;
	centerPos.y = top + halfSize.y;
}

FRect::FRect(D3DXVECTOR2 centerPos, D3DXVECTOR2 size)
{
	this->centerPos = centerPos;
	this->size = size;
	halfSize = size * 0.5f;
	Init(centerPos);
}

FRect::FRect(D3DXVECTOR2 leftTopPos, float width, float height)
{
	size.x = width;
	size.y = height;

	halfSize.x = width * 0.5f;
	halfSize.y = height * 0.5f;

	centerPos.x = leftTopPos.x + halfSize.x;
	centerPos.y = leftTopPos.y + halfSize.y;

	Init(centerPos);

}

void FRect::InitSize(D3DXVECTOR2 size)
{
	this->size = size;
	halfSize = size * 0.5f;
	left = centerPos.x - halfSize.x;
	top = centerPos.y - halfSize.y;
	right = centerPos.x + halfSize.x;
	bottom = centerPos.y + halfSize.y;
}

void FRect::Init(D3DXVECTOR2 centerPos)
{
	left = centerPos.x - halfSize.x;
	top = centerPos.y - halfSize.y;
	right = centerPos.x + halfSize.x;
	bottom = centerPos.y + halfSize.y;
	size.x = halfSize.x * 2;
	size.y = halfSize.y * 2;
	this->centerPos = centerPos;
}

void FRect::Init(float left, float top, float right, float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	size.x = right - left;
	size.y = bottom - top;
	halfSize.x = size.x*0.5f;
	halfSize.y = size.y * 0.5f;
	centerPos.x = left + halfSize.x;
	centerPos.y = top + halfSize.y;
}

void FRect::InitRect(D3DXVECTOR2 centerPos, D3DXVECTOR2 size)
{
	this->centerPos = centerPos;
	this->size = size;
	halfSize = size * 0.5f;
	Init(centerPos);
}

void FRect::MoveRect(float distance, Dir dir)
{
	switch (dir)
	{
	case Dir::UP:
		SetCenterPosY(centerPos.y - distance);
		break;
	case Dir::DOWN:
		SetCenterPosY(centerPos.y + distance);
		break;
	case Dir::RIGHT:
		SetCenterPosX(centerPos.x + distance);
		break;
	case Dir::LEFT:
		SetCenterPosX(centerPos.x - distance);
		break;
	default:
		break;
	}
}
Dir FRect::IsCollisionPush(FRect * rect)
{
	float minX = max(left, rect->left);
	float maxX = min(right, rect->right);

	float minY = max(top, rect->top);
	float maxY = min(bottom, rect->bottom);

	D3DXVECTOR2 temp = { maxX - minX, maxY - minY };
	if (temp.x < 0 || temp.y < 0)
		return Dir::NONE;

	if (temp.x > temp.y)
	{
		if (centerPos.y > rect->centerPos.y)
		{
			rect->SetCenterPosY(rect->centerPos.y - temp.y);
			return Dir::DOWN;
		}
		else
		{
			rect->SetCenterPosY(rect->centerPos.y + temp.y);
			return Dir::UP;
		}
	}
	else
	{
		if (centerPos.x > rect->centerPos.x)
		{
			rect->SetCenterPosX(rect->centerPos.x - temp.x);
			return Dir::RIGHT;
		}
		else
		{
			rect->SetCenterPosX(rect->centerPos.x + temp.x);
			return Dir::LEFT;
		}
	}
}

bool FRect::IsCollision(D3DXVECTOR2 pos)
{
	if (left<pos.x && right>pos.x)
	{
		if (top<pos.y && bottom > pos.y)
			return true;
	}
	return false;
}


void FRect::SetCenterPosX(float centerPosX)
{
	Init({ centerPosX ,centerPos.y });
}

void FRect::SetCenterPosY(float centerPosY)
{
	Init({ centerPos.x,centerPosY });
}