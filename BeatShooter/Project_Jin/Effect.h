#pragma once
class Effect
{
private:
	Animator* ani;
	D3DXVECTOR3 pos;
	bool isRun;
	UINT aniNum;
	World world;

public:

	Effect(Animator* ani, float time = 0.1f);
	~Effect();

	void Update();
	void Render();

	void Start(World world);
	void Start(World world, World* parentsWorld);

	bool GetIsRun() { return isRun; }
};