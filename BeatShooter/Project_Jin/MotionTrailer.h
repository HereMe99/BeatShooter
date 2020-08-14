#pragma once
class MotionTrailer final
{
public:
	MotionTrailer(UINT blurCount, World& focusWorld
		,float blurSec, Texture* texture);
	virtual ~MotionTrailer();

	void Update();
	void Render();

	void TimerFunc();

	inline void SetBlurTexture(Texture* blurTexture);
	void SetBlurWorlds(World world);
private:
	const UINT BLUR_COUNT;
	Timer* mBlurTimer;
	Texture* mBlurTexture;
	vector<World> mBlurWorlds;
	World& mFocusWorld;
	LPD3DXEFFECT mShader;
};

void MotionTrailer::SetBlurTexture(Texture* blurTexture)
{
	mBlurTexture = blurTexture;
}