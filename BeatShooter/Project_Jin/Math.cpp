#include "stdafx.h"
#include "Math.h"


float Math::GetDistance(D3DXVECTOR2 p1, D3DXVECTOR2 p2)
{
	D3DXVECTOR2 temp = p1 - p2;
	return (float)sqrt(temp.x*temp.x + temp.y*temp.y);
}

float Math::GetAngle(D3DXVECTOR2 p1, D3DXVECTOR2 p2)
{
	return atan2f(p2.y - p1.y, p2.x - p1.x);
}