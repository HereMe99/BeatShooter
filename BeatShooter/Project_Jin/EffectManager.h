#pragma once
class EffectManager
{
private:
	//map<string, vector<Effect*>> totalEffect;
	typedef vector<Effect*> vEffect;
	struct EffectMeta
	{
		vEffect effects;
		Animator* animator;
	};
	map<string, EffectMeta> totalEffect;
	static EffectManager* instance;

	EffectManager();
	~EffectManager();

	LPD3DXEFFECT shader;
	int pass;
	float value;
	D3DXCOLOR color;
	D3DXVECTOR2 uvStart;
	D3DXVECTOR2 uvEnd;

public:

	static EffectManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = NEW EffectManager;
		}
		return instance;
	}

	void AddEffect(string key, Animator* ani , float time, int poolCount);

	void Update();
	void Render(string key);

	void Release();

	void Play(string key, World world,World* parentsWorld = nullptr);

};