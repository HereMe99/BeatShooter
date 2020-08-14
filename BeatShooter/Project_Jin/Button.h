#pragma once

class Button
{
public:
	Button(Texture* texture, D3DXVECTOR2 pos, D3DXVECTOR2 size,function<void()> event);
	~Button();

	void Update();
	void Render();

	void SetWorld(World world) { mWorld = world; }
	void SetParentsWorld(World* parentsWorld)
	{
		mWorld.SetParentsWorld(&parentsWorld->matWorld);
	}
protected:
	enum class BtnState
	{
		NOMAL,
		OVER,
		DOWN,
		DOWN_OUT
	}mState;

	Texture* mTexture;
	D3DXCOLOR color;
	LPD3DXEFFECT shader;

	World mWorld;
	World mCollisionWorld;

	bool mIsDown;

	function<void()> Event;
};
