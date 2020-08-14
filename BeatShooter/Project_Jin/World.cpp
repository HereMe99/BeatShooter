#include "stdafx.h"
#include "World.h"


World::World()
	:scale({ 1,1 }), rot({ 0,0,0 }), pos({ 0,0,0 })
	, pivot({ 0,0,0 }), size({ 0,0 })
	, minBox({ 0,0 }), maxBox({ 0,0 })
{
	D3DXMatrixIdentity(&matWorld);
	parentsWorld = nullptr;
}

World::World(D3DXVECTOR2 pos, D3DXVECTOR2 size)
	:scale({ 1,1 }), rot({ 0,0,0 }), pos({ pos.x,pos.y,0 })
	, pivot({ 0,0,0 }), size(size)
	, minBox({ 0,0 }), maxBox({ 0,0 })
{
	minBox = maxBox = { 0, 0 };
	D3DXMatrixIdentity(&matWorld);
	parentsWorld = nullptr;
}

World::World(const World& world)
	:scale(world.scale), rot(world.rot), pos(world.pos)
	, pivot(world.pivot), size(world.size)
	, minBox(world.minBox), maxBox(world.maxBox)
	, matWorld(world.matWorld)
{
	D3DXMatrixIdentity(&matWorld);
	parentsWorld = nullptr;
}


World::~World()
{
}

void World::Update()
{
	if (rot.z > 2.0f*PI)
	{
		rot.z -= 2.0 * PI;
	}
	else if (rot.z < -2.0f * PI)
	{
		rot.z += 2.0 * PI;
	}
	D3DXMATRIX P, IP;
	D3DXMATRIX S, R, T;
	D3DXMATRIX matS, matR;

	D3DXMatrixTranslation(&P, pivot.x, pivot.y, pivot.z);
	D3DXMatrixInverse(&IP, NULL, &P);

	D3DXMatrixScaling(&matS, scale.x, scale.y, 1);
	S = IP * matS * P;
	D3DXMatrixRotationYawPitchRoll(&matR, rot.y, rot.x, rot.z);
	R = IP * matR * P;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

	if (nullptr == parentsWorld)
	{
		matWorld = S * R * T;
	}
	else
	{
		matWorld = S * R * T * *parentsWorld;
	}

}

void World::Update(D3DXMATRIX& parentsWorld)
{
	D3DXMATRIX P, IP;
	D3DXMATRIX S, R, T;
	D3DXMATRIX matS, matR;

	D3DXMatrixTranslation(&P, pivot.x, pivot.y, pivot.z);
	D3DXMatrixInverse(&IP, NULL, &P);

	D3DXMatrixScaling(&matS, scale.x, scale.y, 1);
	S = IP * matS * P;
	D3DXMatrixRotationYawPitchRoll(&matR, rot.y, rot.x, rot.z);
	R = IP * matR * P;
	D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);

	matWorld = S * R * T * parentsWorld;
}


void World::SetWorld()
{
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);
}

void World::SetView()
{
	DEVICE->SetTransform(D3DTS_VIEW, &matWorld);
}

void World::SetProjection()
{
	DEVICE->SetTransform(D3DTS_PROJECTION, &matWorld);
}

void World::InitWorld()
{
	D3DXMATRIX tempMat;
	D3DXMatrixIdentity(&tempMat);
	DEVICE->SetTransform(D3DTS_WORLD, &tempMat);
	//DEVICE->SetTransform(D3DTS_VIEW, &tempMat);
}

void World::InitView()
{
	D3DXMATRIX tempMat;
	D3DXMatrixIdentity(&tempMat);
	DEVICE->SetTransform(D3DTS_VIEW, &tempMat);
}



bool World::IsCollision(World* world)
{
	SetBox();
	world->SetBox();

	float x1 = max(minBox.x, world->minBox.x);
	float x2 = min(maxBox.x, world->maxBox.x);

	float y1 = max(minBox.y, world->minBox.y);
	float y2 = min(maxBox.y, world->maxBox.y);

	D3DXVECTOR2 temp(x2 - x1, y2 - y1);

	if (temp.x > 0 && temp.y > 0)
		return true;

	return false;
}

bool World::IsCollision(D3DXVECTOR2 pos)
{
	SetBox();
	if (maxBox.x > pos.x&& minBox.x < pos.x)
	{
		if (maxBox.y > pos.y&& minBox.y < pos.y)
		{
			return true;
		}
	}
	return false;
}

void World::SetParentsWorld(D3DXMATRIX* parentsWorld)
{
	this->parentsWorld = parentsWorld;
}

void World::SetBox()
{
	D3DXVECTOR2 pos2 = { pos.x,pos.y };
	minBox = pos2 - size * 0.5f;
	maxBox = pos2 + size * 0.5f;
}


Direction World::IsCollisionPush(World* world)
{
	SetBox();
	world->SetBox();
	float minX = max(minBox.x, world->minBox.x);
	float maxX = min(maxBox.x, world->maxBox.x);

	float minY = max(minBox.y, world->minBox.y);
	float maxY = min(maxBox.y, world->maxBox.y);

	D3DXVECTOR2 temp = { maxX - minX, maxY - minY };
	if (temp.x < 0 || temp.y < 0)
		return Direction::NONE;

	if (temp.x > temp.y)
	{
		if (pos.y > world->pos.y)
		{
			world->pos.y = world->pos.y - temp.y;
			//world->SetBox();
			return Direction::DOWN;
		}
		else
		{
			world->pos.y = world->pos.y + temp.y;
			//world->SetBox();
			return Direction::UP;
		}
	}
	else
	{
		if (pos.x > world->pos.x)
		{
			world->pos.x = world->pos.x - temp.x;
			//	world->SetBox();
			return Direction::LEFT;
		}
		else
		{
			world->pos.x = world->pos.x + temp.x;
			//world->SetBox();

			return Direction::RIGHT;
		}
	}
}

Direction World::IsCollisionDir(World* world)
{
	SetBox();
	world->SetBox();
	float minX = max(minBox.x, world->minBox.x);
	float maxX = min(maxBox.x, world->maxBox.x);

	float minY = max(minBox.y, world->minBox.y);
	float maxY = min(maxBox.y, world->maxBox.y);

	D3DXVECTOR2 temp = { maxX - minX, maxY - minY };
	if (temp.x < 0 || temp.y < 0)
		return Direction::NONE;

	if (temp.x > temp.y)
	{
		if (pos.y > world->pos.y)
		{
			return Direction::DOWN;
		}
		else
		{
			return Direction::UP;
		}
	}
	else
	{
		if (pos.x > world->pos.x)
		{
			return Direction::LEFT;
		}
		else
		{
			return Direction::RIGHT;
		}
	}
}

bool World::IsCollision(World* world1, World* world2)
{
	world1->SetBox();
	world2->SetBox();
	float x1 = max(world1->minBox.x, world2->minBox.x);
	float x2 = min(world1->maxBox.x, world2->maxBox.x);

	float y1 = max(world1->minBox.y, world2->minBox.y);
	float y2 = min(world1->maxBox.y, world2->maxBox.y);

	D3DXVECTOR2 temp(x2 - x1, y2 - y1);

	if (temp.x > 0 && temp.y > 0)
		return true;

	return false;
}
