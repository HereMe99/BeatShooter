#include "stdafx.h"

int Utility::Random(const int& min, const int& max)
{
	return rand() % (max - min+1) + min;
}

float Utility::Random(const float& min, const float& max)
{
	float value = (float)rand() / RAND_MAX;
	return min + (max - min) * value;
}

float Utility::Distance(IN const Vector2& pos1, IN const Vector2& pos2)
{
	return Vector2Length(pos1 - pos2);
}

float Utility::DegreeToRadian(float degree)
{
	return degree * PI / 180;
}

float Utility::RadianToDegree(float radian)
{
	return radian * 180 / PI;
}

float Utility::Vector2Length(IN const Vector2& vector2)
{
	return (float)sqrt(vector2.x * vector2.x + vector2.y * vector2.y);
}

float Utility::Vector3Length(IN const Vector3& vector3)
{
	return (float)sqrt(vector3.x * vector3.x + vector3.y * vector3.y+ vector3.z * vector3.z);
}

Vector2 Utility::GetVector2Nomalize(IN const Vector2& vector2)
{
	float dist = Vector2Length(vector2);
	return Vector2(vector2.x / dist, vector2.y / dist);
}

Vector3 Utility::GetVector3Nomalize(IN const Vector3& vector3)
{
	float dist = Vector3Length(vector3);
	return Vector3(vector3.x / dist, vector3.y / dist, vector3.z / dist);
}
void Utility::Nomalize(OUT Vector2& vector2)
{
	float dist = Vector2Length(vector2);
	if (dist < FLT_EPSILON)
	{
		vector2 = { 0,0 };
		return;
	}
	vector2 /= dist;
}

float Utility::GetAngle(IN const Vector2& vector2)
{
	return (float)atan2(vector2.y, vector2.x);
}

float Utility::GetAngle(IN const Vector3& vector3)
{
	return (float)atan2(vector3.y, vector3.x);
}

void Utility::SetAngle(IN const float angle, OUT Vector2* vector2)
{
	vector2->x = (float)cos(angle);
	vector2->y = (float)-sin(angle);
}

