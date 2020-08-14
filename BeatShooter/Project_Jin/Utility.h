#pragma once
namespace Utility
{
	enum class Dir
	{
		NONE,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	int Random(const int& min, const int& max);
	float Random(const float& min, const float& max);

	float Distance(IN const Vector2& pos1, IN const Vector2& pos2);

	float DegreeToRadian(float degree);
	float RadianToDegree(float radian);

	float Vector2Length(IN const Vector2& vector2);
	float Vector3Length(IN const Vector3& vector3);
	Vector2 GetVector2Nomalize(IN const Vector2& vector2);
	Vector3 GetVector3Nomalize(IN const Vector3& vector3);
	void Nomalize(OUT Vector2& vector2);
	float GetAngle(IN const Vector2& vector2);
	float GetAngle(IN const Vector3& vector3);
	void SetAngle(IN const float angle,OUT Vector2* vector2);


	Vector2 PointToVector(IN const POINT& point);
	POINT VectorToPoint(IN const Vector2& vector);

	void GetMousePos();
}