#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
	:mWorld(World()),mTag(ObjTag::NONE)
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

bool GameObject::CollisionObjByCicle(GameObject& obj)
{
	Vector2 thisPos = Vector2(mWorld.pos.x,mWorld.pos.y);
	Vector2 collsionObjPos = Vector2(obj.mWorld.pos.x, obj.mWorld.pos.y);
	if (Utility::Distance(thisPos, collsionObjPos) <= (obj.mWorld.size.x * 0.4f + mWorld.size.x * 0.4f))
	{
		return true;
	}

	return false;
}

bool GameObject::CollisionObjPushByCicle(GameObject& obj)
{
	Vector2 thisPos = Vector2(mWorld.pos.x, mWorld.pos.y);
	Vector2 collsionObjPos = Vector2(obj.mWorld.pos.x, obj.mWorld.pos.y);
	if (Utility::Distance(thisPos, collsionObjPos) <= (obj.mWorld.size.x * 0.4f + mWorld.size.x * 0.4f))
	{
		Vector2 direction = collsionObjPos - thisPos;
		Utility::Nomalize(direction);
		float distance = (obj.mWorld.size.x * 0.4f + mWorld.size.x * 0.4f)-Utility::Distance(thisPos, collsionObjPos);
		direction *= distance;
		obj.mWorld.pos += Vector3(direction.x, direction.y, 0);
		return true;
	}

	return false;
}
