#pragma once
class RenderManager
{
private:
	GameObject * player;
	GameObject * Enemy;
public:
	RenderManager();
	~RenderManager();

	void Update();
	void LestUpdate();
	void Render();

	void Init(GameObject * player, GameObject * Enemy);
};

