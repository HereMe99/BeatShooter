#pragma once

enum class Direction
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class World
{
public:
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 pos;

	D3DXVECTOR2 size;

	D3DXVECTOR3 pivot;

	D3DXMATRIX matWorld;
	D3DXMATRIX* parentsWorld;

	D3DXVECTOR2 minBox;
	D3DXVECTOR2 maxBox;

	World();
	World(D3DXVECTOR2 pos,D3DXVECTOR2 size);
	World(const World &world);
	~World();

	void Update();
	void Update(D3DXMATRIX& parentsWorld);

	void SetWorld();
	void SetView();
	void SetProjection();

	static void InitWorld();
	static void InitView();

	bool IsCollision(World* world);
	bool IsCollision(D3DXVECTOR2 pos);

	void SetParentsWorld(D3DXMATRIX* parentsWorld);
	void SetBox();

	Direction IsCollisionPush(World* world);
	Direction IsCollisionDir(World* world);

	static bool IsCollision(World* world1, World* world2);
};

